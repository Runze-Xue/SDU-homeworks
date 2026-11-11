
#image_combine代替image_stitcher的功能
#替代功能描述
# （1）SIFT特征检测：检测出特征点+特征描述（一般流程位cv2.SIFT_create()或者cv2.ORB_create()生成检测器，然后由检测器生成描述子？？?）(检测器和描述子是什么关系？)
# （2）特征匹配（暴力匹配（Brute-Force Matcher）或者FLANN，进行交叉检测与比例测试来过滤不良匹配）
# （3）使用一个单应矩阵结合RANSAC剔除外点
# （4）相机参数优化
# （5）图像变形/投影（Image Warping）
# （6）在重叠区域找到一条最优的“切割线”，使得沿着这条线拼接时，视觉差异最小
# （7）图像融合/混合

import cv2
import numpy as np
from constants import ratio_threshhold, MIN_MATCH_COUNT
def feature_detect(imgs): # 输入图片组
    sift = cv2.SIFT_create()
    gray_imgs_list = [cv2.cvtColor(img, cv2.COLOR_BGR2GRAY) for img in imgs]
    keypoints_list = [] 
    descriptors_list = []
    for gray_img in gray_imgs_list:
        keypoint, descriptor = sift.detectAndCompute(gray_img, None)
        keypoints_list.append(keypoint)
        descriptors_list.append(descriptor)
    return keypoints_list, descriptors_list

def feature_match(keypoints_list, descriptors_list,img_nums):
    bf = cv2.BFMatcher(cv2.NORM_L2, crossCheck=False)
    all_good_matches = {}
    for i in range(img_nums):
        for j in range(i+1, img_nums):
            kp1 = keypoints_list[i]
            des1 = descriptors_list[i]
            kp2 = keypoints_list[j]
            des2 = descriptors_list[j]
            if des1 is None or des2 is None:
                print(f"    Skipping pair ({i},{j}): descriptors missing for one or both images.")
                all_good_matches[(i, j)] = []
                continue
            if des1.shape[0] == 0 or des2.shape[0] == 0:
                print(f"    Skipping pair ({i},{j}): no descriptors found for one or both images.")
                all_good_matches[(i, j)] = []
                continue
            if len(kp1) == 0 or len(kp2) == 0:
                print(f"    Skipping pair ({i},{j}): no keypoints found for one or both images (should not happen if descriptors exist).")
                all_good_matches[(i, j)] = []
                continue
            raw_matches = bf.knnMatch(des1, des2, k=2)
            good_matches = []
            for per_match in raw_matches:
                if len(per_match) == 2:
                    m, n = per_match
                    if m.distance < ratio_threshhold * n.distance:
                        good_matches.append(m)
                elif len(per_match) == 1:
                        print("len per_match = 1")
                        good_matches.append(per_match[0])
                else:
                    print("lack matches in match")
            # print("i", i, "j",j)
            all_good_matches[(i,j)] = good_matches
    return all_good_matches


def calculate_all_homographies(keypoints_list, all_good_matches, img_nums):
    homographies = {} # 存储 (i, j) -> H_ij
    for i in range(img_nums):
        for j in range(img_nums): 
            if i == j:
                continue
            pair = tuple(sorted((i, j))) 
            print("i", i, "j", j)
            if pair not in all_good_matches:
                continue
            good_matches_for_pair = all_good_matches.get(pair)
            if good_matches_for_pair is None or len(good_matches_for_pair) < MIN_MATCH_COUNT:
                print(f"Not enough good matches for pair {pair} to find homography (found {len(good_matches_for_pair) if good_matches_for_pair else 0}).")
                homographies[(i, j)] = None # or some indicator of failure
                continue
            # 提取匹配点对的坐标
            kp_query = keypoints_list[pair[0]] # 对应照片的特征点序列
            kp_train = keypoints_list[pair[1]]

            src_pts_list = []
            dst_pts_list = []
            for m in good_matches_for_pair:
                #从两张图片中分别取出对应点的坐标
                src_pts_list.append(kp_train[m.trainIdx].pt) #对应到具体的特征点
                dst_pts_list.append(kp_query[m.queryIdx].pt)
            
            src_pts = np.float32(src_pts_list).reshape(-1, 1, 2)
            dst_pts = np.float32(dst_pts_list).reshape(-1, 1, 2)

            H_map_train_to_query, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 5.0)
            
            if H_map_train_to_query is None:
                print(f"Homography could not be computed for pair {pair}.")
                # print("what is none", i, j)
                homographies[(pair[0], pair[1])] = None
                homographies[(pair[1], pair[0])] = None
                continue
            homographies[(pair[0], pair[1])] = H_map_train_to_query 
            H_map_query_to_train = np.linalg.inv(H_map_train_to_query) #计算逆矩阵
            homographies[(pair[1], pair[0])] = H_map_query_to_train
    return homographies

def stitch_images_to_central_reference(imgs, all_homographies_dict):
    num_images = len(imgs)
    if num_images == 1:
        return imgs[0]
    ref_idx = num_images // 2
    H_to_reference_frame = [None] * num_images # Stores H_ref_i (transforms img i to ref_idx frame)
    for i in range(num_images):
        if i == ref_idx:
            H_to_reference_frame[i] = np.eye(3, dtype=np.float32)
        else:
            H_i_to_ref = all_homographies_dict.get((ref_idx, i))
            if H_i_to_ref is None:
                print(f"Warning: Missing homography H_({ref_idx},{i}). Image {i} cannot be stitched to reference image {ref_idx}.")
                H_to_reference_frame[i] = None # Mark as None if direct homography is missing
            else:
                H_to_reference_frame[i] = H_i_to_ref    
    corners = []
    valid_image_indices_for_canvas = []
    for i in range(num_images):
        if H_to_reference_frame[i] is None:
            print(f"Skipping image {i} for canvas calculation as its homography to reference is missing.")
            continue
        h, w = imgs[i].shape[:2]
        img_corners = np.float32([[0, 0], [0, h - 1], [w - 1, h - 1], [w - 1, 0]]).reshape(-1, 1, 2)
        transformed_corners = cv2.perspectiveTransform(img_corners, H_to_reference_frame[i])
        corners.append(transformed_corners)
        valid_image_indices_for_canvas.append(i) # Keep track of images contributing to canvas
    if not corners:
        print("Error: No valid transformed corners to calculate panorama size. Most likely all homographies to reference are missing.")
        if imgs: return imgs[ref_idx] # Fallback to reference image if possible
        return None
    all_corners = np.concatenate(corners, axis=0)
    # 4. Determine the size of the output panorama and the translation offset
    x_min, y_min = np.int32(all_corners.min(axis=0).ravel() - 0.5)
    x_max, y_max = np.int32(all_corners.max(axis=0).ravel() + 0.5)
    translation_dist = [-x_min, -y_min]
    H_translation = np.array([[1, 0, translation_dist[0]],
                              [0, 1, translation_dist[1]],
                              [0, 0, 1]], dtype=np.float32)
    output_width = x_max - x_min
    output_height = y_max - y_min
    # Sanity check for panorama size
    if output_width <= 0 or output_height <= 0 or output_width > 30000 or output_height > 30000:
        print(f"Warning: Calculated panorama size is problematic: {output_width}x{output_height}.")
        # Fallback strategy: return the reference image, or the first image if ref_idx is out of bounds
        return imgs[ref_idx if 0 <= ref_idx < num_images else 0]
    print(f"Panorama canvas size: {output_width} x {output_height}")
    panorama = np.zeros((output_height, output_width, 3), dtype=np.uint8)
    process_order = [ref_idx] + [i for i in range(num_images) if i != ref_idx]
    for i in process_order:
        if H_to_reference_frame[i] is None:
            print(f"Skipping warping for image {i} as its homography to reference is missing.")
            continue
        print(f"Warping image {i} to reference frame of image {ref_idx}...")
        # Final transformation matrix for image i: transform to ref_idx frame, then translate for canvas
        final_H_for_i = H_translation @ H_to_reference_frame[i]
        warped_image = cv2.warpPerspective(imgs[i], final_H_for_i, (output_width, output_height))
        # Create a mask for the non-black regions of the warped image
        mask_warped = cv2.cvtColor(warped_image, cv2.COLOR_BGR2GRAY)
        _, mask_warped = cv2.threshold(mask_warped, 1, 255, cv2.THRESH_BINARY)
        if i == ref_idx: # Reference image is placed first (or during its turn in process_order)
            # Simple addition if panorama is guaranteed black here, or use copyTo
            panorama = cv2.add(panorama, warped_image) # Assumes panorama starts black
        else:
            mask_panorama_is_black = cv2.cvtColor(panorama, cv2.COLOR_BGR2GRAY)
            _, mask_panorama_is_black = cv2.threshold(mask_panorama_is_black, 0, 255, cv2.THRESH_BINARY_INV) 
            copy_mask = cv2.bitwise_and(mask_warped, mask_panorama_is_black)
            panorama = cv2.copyTo(warped_image, copy_mask, panorama)
            overlap_mask = cv2.bitwise_and(mask_warped, cv2.bitwise_not(mask_panorama_is_black))
            if cv2.countNonZero(overlap_mask) > 0:
                current_pixels_in_overlap = panorama[overlap_mask > 0].astype(np.float32)
                new_pixels_in_overlap = warped_image[overlap_mask > 0].astype(np.float32)
                blended_pixels = cv2.addWeighted(current_pixels_in_overlap, 0.5, 
                                                 new_pixels_in_overlap, 0.5, 0.0)
                panorama[overlap_mask > 0] = blended_pixels.astype(np.uint8)
    return panorama

def combine_images(imgs): #输入 ndarray格式的列表
    #return final_image, status
    img_nums = len(imgs)
    if img_nums < 2:
        print("pictures < 2")
        return None, 0
    print("feature_detecting")
    keypoints_list, descriptors_list = feature_detect(imgs)
    print("feature matching")
    all_good_matches = feature_match(keypoints_list, descriptors_list, img_nums)
    print("homographies calculation")
    all_homographies_dict = calculate_all_homographies(keypoints_list, all_good_matches, img_nums)
    print("stitching")
    panorama  = stitch_images_to_central_reference(imgs, all_homographies_dict)
    return panorama , 1
