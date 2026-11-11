import cv2
import numpy as np # 确保导入 numpy

def feature_detect(imgs): # 输入图片组
    sift = cv2.SIFT_create()
    gray_imgs = [cv2.cvtColor(img, cv2.COLOR_BGR2GRAY) for img in imgs]
    keypoints_list = []  # 更改变量名以示区分列表
    descriptors_list = [] # 更改变量名以示区分列表
    for gray_img in gray_imgs:
        keypoints, descriptors = sift.detectAndCompute(gray_img, None)
        keypoints_list.append(keypoints)
        descriptors_list.append(descriptors)
    return keypoints_list, descriptors_list


def combine_images(imgs_input_list): # 输入 ndarray格式的列表
    print("Step 1: Detecting features and computing descriptors...")
    all_keypoints, all_descriptors = feature_detect(imgs_input_list)
    print(f"  Detected features for {len(all_keypoints)} images.")

    num_images = len(all_keypoints)
    if num_images < 2:
        print("  Need at least two images to perform matching. Returning.")
        return None, None # 或者其他合适的返回值

    print("\nStep 2: Performing feature matching between image pairs...")
    # 创建 BFMatcher 对象
    # NORM_L2 适用于 SIFT/SURF 描述子
    # crossCheck=False 因为我们将使用 knnMatch 和 Lowe's ratio test
    bf = cv2.BFMatcher(cv2.NORM_L2, crossCheck=False)

    # 存储所有图像对之间的优质匹配
    # 字典的键是元组 (i, j) 代表图像索引对，值是 cv2.DMatch 对象的列表
    all_good_matches_info = {}
    ratio_thresh = 0.75  # Lowe's ratio test 阈值

    for i in range(num_images):
        for j in range(i + 1, num_images): # 避免重复匹配 (i,j) 和 (j,i) 以及自身匹配 (i,i)
            print(f"  Matching image {i} with image {j}...")

            kp1 = all_keypoints[i]
            des1 = all_descriptors[i]
            kp2 = all_keypoints[j]
            des2 = all_descriptors[j]

            # 确保描述子存在且不为空
            if des1 is None or des2 is None:
                print(f"    Skipping pair ({i},{j}): descriptors missing for one or both images.")
                all_good_matches_info[(i, j)] = []
                continue
            if des1.shape[0] == 0 or des2.shape[0] == 0:
                print(f"    Skipping pair ({i},{j}): no descriptors found for one or both images.")
                all_good_matches_info[(i, j)] = []
                continue
            if len(kp1) == 0 or len(kp2) == 0:
                print(f"    Skipping pair ({i},{j}): no keypoints found for one or both images (should not happen if descriptors exist).")
                all_good_matches_info[(i, j)] = []
                continue


            try:
                # 使用 knnMatch 找到每对描述子的 k 个最佳匹配 (k=2)
                raw_matches = bf.knnMatch(des1, des2, k=2)
            except cv2.error as e:
                print(f"    Error during knnMatch for pair ({i}, {j}): {e}")
                print(f"    Des1 shape: {des1.shape}, Des2 shape: {des2.shape}")
                all_good_matches_info[(i, j)] = []
                continue

            good_matches = []
            # 应用 Lowe's Ratio Test
            for m_n_pair in raw_matches:
                # 确保我们得到了两个邻居 (如果 des2 中的描述子少于 k=2，m_n_pair 可能只有一个元素)
                if len(m_n_pair) == 2:
                    m, n = m_n_pair # m 是最近邻，n 是次近邻
                    if m.distance < ratio_thresh * n.distance:
                        good_matches.append(m) # 只保留好的匹配 m

            all_good_matches_info[(i, j)] = good_matches
            print(f"    Found {len(good_matches)} good matches between image {i} and image {j} after Lowe's ratio test.")

            # (可选) 在这里可以添加可视化某对匹配的代码，如果需要调试
            # if len(good_matches) > 10: # 例如，只显示匹配数量大于10的
            #     img1_display = imgs_input_list[i]
            #     img2_display = imgs_input_list[j]
            #     img_matches_display = cv2.drawMatches(img1_display, kp1, img2_display, kp2,
            #                                        good_matches, None,
            #                                        flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS)
            #     cv2.imshow(f"Good Matches {i}-{j}", img_matches_display)
            #     if cv2.waitKey(0) & 0xFF == ord('q'): # 按 'q' 退出
            #         cv2.destroyAllWindows()
            #         return all_keypoints, all_good_matches_info # 提前返回
            #     cv2.destroyWindow(f"Good Matches {i}-{j}")


    print("\nFeature matching completed for all pairs.")
    # 函数返回原始的 keypoints 列表和所有图像对之间的匹配信息
    # 注意：all_descriptors 也可以一并返回，如果后续步骤还需要原始描述子
    return all_keypoints, all_descriptors, all_good_matches_info


import cv2
import numpy as np

# 假设你已经有了这些变量：
# keypoints_list: 列表，每个元素是某张图的 cv2.KeyPoint 对象列表
# descriptors_list: 列表，每个元素是某张图的描述子 (numpy array)
# all_good_matches: 字典，键是 (i, j) 元组，值是 cv2.DMatch 对象列表
# imgs: 列表，每个元素是读取的图像 (numpy array BGR)
# img_nums: 图像数量
# ratio_threshhold = 0.75 (来自你的 feature_match 函数)

MIN_MATCH_COUNT = 10 # 至少需要多少好的匹配点来计算单应性

def calculate_all_homographies(keypoints_list, all_good_matches, img_nums):
    """
    计算所有相邻图像对之间的单应性矩阵。
    H_ij 表示将图像 j 的点变换到图像 i 的坐标系。
    """
    homographies = {} # 存储 (i, j) -> H_ij

    for i in range(img_nums):
        for j in range(img_nums): # 考虑所有可能的直接或间接连接，但通常我们先计算相邻的
            if i == j:
                continue

            # 我们通常计算 H_ij (j -> i) 或 H_ji (i -> j)
            # 在你的 all_good_matches 中，(idx1, idx2) with idx2 > idx1
            # matches 是 img_idx1 (query) 和 img_idx2 (train) 之间的
            # 所以，我们计算 H_idx1_idx2，它将图像 idx2 的点映射到图像 idx1
            
            # 假设你的 all_good_matches 是这样填充的: (idx_smaller, idx_larger)
            # 我们期望 homographies[(i,j)] 是将图像 j 变换到图像 i 的 H
            # 你的 all_good_matches 键是 (i,j) where j > i.
            # kp1 = keypoints_list[i], kp2 = keypoints_list[j]
            # good_matches 在 kp1 和 kp2 之间

            # 我们需要找到 H_ij，将图像 j 的点变换到图像 i 的坐标系
            # 如果 (i, j) 在 all_good_matches 中 (意味着 i < j)
            # good_matches 是 kp_i 和 kp_j 之间的匹配
            # kp_i[m.queryIdx] 对应 kp_j[m.trainIdx]
            # 要将 j 变换到 i，src_pts 来自 j (train), dst_pts 来自 i (query)

            pair = tuple(sorted((i, j))) # (min_idx, max_idx)
            
            if pair not in all_good_matches:
                # print(f"No matches pre-calculated for pair {pair}, trying reverse or skipping.")
                # You might want to handle cases where you need H_ji but only have H_ij
                # by inverting H_ij: H_ji = np.linalg.inv(H_ij)
                # For now, let's assume all_good_matches provides what we need directly for sequential.
                continue


            good_matches_for_pair = all_good_matches.get(pair)
            if good_matches_for_pair is None or len(good_matches_for_pair) < MIN_MATCH_COUNT:
                print(f"Not enough good matches for pair {pair} to find homography (found {len(good_matches_for_pair) if good_matches_for_pair else 0}).")
                homographies[(i, j)] = None # or some indicator of failure
                continue

            # 提取匹配点对的坐标
            # queryIdx 对应 pair[0] (较小编号的图像)
            # trainIdx 对应 pair[1] (较大编号的图像)
            kp_query = keypoints_list[pair[0]]
            kp_train = keypoints_list[pair[1]]

            src_pts_list = []
            dst_pts_list = []
            for m in good_matches_for_pair:
                # 要将 pair[1] (train) 变换到 pair[0] (query)
                src_pts_list.append(kp_train[m.trainIdx].pt)
                dst_pts_list.append(kp_query[m.queryIdx].pt)
            
            src_pts = np.float32(src_pts_list).reshape(-1, 1, 2)
            dst_pts = np.float32(dst_pts_list).reshape(-1, 1, 2)

            # 计算单应性矩阵 H_pair[0]_pair[1] (将 pair[1] 变换到 pair[0])
            H_map_train_to_query, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 5.0)
            
            if H_map_train_to_query is None:
                print(f"Homography could not be computed for pair {pair}.")
                # Store None for both directions or handle appropriately
                homographies[(pair[0], pair[1])] = None
                homographies[(pair[1], pair[0])] = None
                continue

            # H_map_train_to_query transforms points from image pair[1] to image pair[0]
            # So, homographies[(pair[0], pair[1])] = H_map_train_to_query
            homographies[(pair[0], pair[1])] = H_map_train_to_query
            
            # Also store the inverse homography H_pair[1]_pair[0]
            try:
                H_map_query_to_train = np.linalg.inv(H_map_train_to_query)
                homographies[(pair[1], pair[0])] = H_map_query_to_train
            except np.linalg.LinAlgError:
                print(f"Could not invert homography for pair {pair}.")
                homographies[(pair[1], pair[0])] = None
                
            print(f"Homography H_({pair[0]},{pair[1]}) (transforms {pair[1]} to {pair[0]}) computed.")
            # print(H_map_train_to_query)

    return homographies


def stitch_images_sequentially(imgs, all_homographies_dict):
    """
    按顺序拼接图像 (0->1, 1->2, ...)，将所有图像变换到第一张图像的坐标系。
    all_homographies_dict[(i,j)] 是将图像 j 变换到图像 i 的 H 矩阵。
    """
    if not imgs:
        return None
    num_images = len(imgs)
    if num_images == 1:
        return imgs[0]

    # 最终的累积单应性矩阵，将每个图像变换到参考图像 (imgs[0]) 的坐标系
    # H_final[k] 会将 imgs[k] 变换到 imgs[0] 的坐标系
    H_to_reference = [np.eye(3)] * num_images # H_to_reference[0] 是单位矩阵

    # 计算累积单应性矩阵
    # H_to_reference[k] = H_to_reference[k-1] @ H_from_k_to_k-1
    # H_from_k_to_k-1 is all_homographies_dict[(k-1, k)]
    current_H_to_ref = np.eye(3)
    for k in range(1, num_images):
        # H_k_prev_maps_k_to_prev = all_homographies_dict.get((k - 1, k)) # Maps k to k-1
        # More robust: Find a path if direct H is not good or available.
        # For simple sequential, we expect H(k-1, k)
        H_k_to_k_minus_1 = all_homographies_dict.get((k - 1, k))
        
        if H_k_to_k_minus_1 is None:
            print(f"Error: Missing homography H_({k-1},{k}) to stitch image {k} to {k-1}. Stitching might fail or be incomplete.")
            # Option 1: Stop stitching
            # return None
            # Option 2: Try to skip this image and stitch subsequent ones relative to an earlier image (more complex)
            # Option 3: Assume identity (bad idea generally)
            # For now, we'll let it try to proceed, subsequent H will be wrong.
            # A better approach might be to build a graph and find paths.
            # Let's assume for now that the H will be used directly to warp to the evolving panorama
            # This requires adjusting the target coordinate system of H_k_to_k-1.
            # The below accumulation assumes H maps to the *previous image's original space*
            H_to_reference[k] = H_to_reference[k-1] @ H_k_to_k_minus_1 if H_k_to_k_minus_1 is not None else H_to_reference[k-1] # If H fails, treat as identity relative to previous
            if H_k_to_k_minus_1 is None:
                 H_to_reference[k] = np.eye(3) # Error case: can't chain, so this img won't be aligned to img0
        else:
            H_to_reference[k] = H_to_reference[k-1] @ H_k_to_k_minus_1

    # --- 确定最终全景图的画布大小和偏移 ---
    corners = []
    for k in range(num_images):
        h, w = imgs[k].shape[:2]
        # 原始图像的四个角点
        img_corners = np.float32([[0, 0], [0, h-1], [w-1, h-1], [w-1, 0]]).reshape(-1, 1, 2)
        if H_to_reference[k] is None: # Should not happen if we handle missing H above
             print(f"Warning: H_to_reference[{k}] is None. Using identity for corner calculation.")
             transformed_corners = img_corners
        else:
            transformed_corners = cv2.perspectiveTransform(img_corners, H_to_reference[k])
        corners.append(transformed_corners)

    all_corners = np.concatenate(corners, axis=0)

    x_min, y_min = np.int32(all_corners.min(axis=0).ravel() - 0.5)
    x_max, y_max = np.int32(all_corners.max(axis=0).ravel() + 0.5)

    # 平移矩阵，将所有图像移动到画布的非负坐标区域
    translation_dist = [-x_min, -y_min]
    H_translation = np.array([[1, 0, translation_dist[0]],
                              [0, 1, translation_dist[1]],
                              [0, 0, 1]], dtype=np.float32)

    output_width = x_max - x_min
    output_height = y_max - y_min
    
    if output_width <= 0 or output_height <= 0 or output_width > 20000 or output_height > 20000: # Sanity check
        print(f"Warning: Calculated panorama size is problematic: {output_width}x{output_height}. Using first image as fallback.")
        return imgs[0]

    panorama = np.zeros((output_height, output_width, 3), dtype=np.uint8)
    print(f"Panorama canvas size: {output_width} x {output_height}")

    # --- 变形并拼接图像 ---
    for k in range(num_images):
        print(f"Warping image {k}...")
        # 最终的变换矩阵 = 平移矩阵 @ (将图像k变换到参考图像0的矩阵)
        if H_to_reference[k] is None:
            print(f"Skipping warp for image {k} as its homography to reference is missing.")
            continue
            
        final_H_for_k = H_translation @ H_to_reference[k]

        warped_image = cv2.warpPerspective(imgs[k], final_H_for_k, (output_width, output_height))
        
        # 简单融合：直接覆盖非黑色像素 (可以改进为 alpha blending 或 multi-band blending)
        # 创建一个掩码，表示 warped_image 中非黑色的部分
        mask_warped = cv2.cvtColor(warped_image, cv2.COLOR_BGR2GRAY)
        _, mask_warped = cv2.threshold(mask_warped, 1, 255, cv2.THRESH_BINARY)
        
        # 在 panorama 中清除即将被 warped_image 覆盖的区域 (对于更复杂的融合，这一步可能不需要)
        # panorama = cv2.bitwise_and(panorama, panorama, mask=cv2.bitwise_not(mask_warped))
        # panorama = cv2.add(panorama, warped_image)

        # 更安全的逐像素叠加 (避免在重叠区域直接覆盖导致信息丢失)
        # 仅在 panorama 当前像素为黑色的地方直接拷贝 warped_image 的像素
        # 这是一种非常基础的拼接，先来的图像有优先权
        if k == 0: # 第一个图像直接放
             panorama = cv2.add(panorama, warped_image)
        else: # 后续图像，只在 panorama 为黑色的地方添加
            # A more robust way is to average or use a blender.
            # For now, let's do a simple overlay where new image overwrites.
            # This can be improved by stitching to the *current panorama* rather than just img[0]
            # Or by using a proper blending technique.
            
            # Create a mask of where the current panorama is black
            mask_panorama_is_black = cv2.cvtColor(panorama, cv2.COLOR_BGR2GRAY)
            _, mask_panorama_is_black = cv2.threshold(mask_panorama_is_black, 0, 255, cv2.THRESH_BINARY_INV) # 0->255, others->0

            # Where panorama is black AND warped_image is not black, copy from warped_image
            copy_mask = cv2.bitwise_and(mask_warped, mask_panorama_is_black)
            panorama = cv2.copyTo(warped_image, copy_mask, panorama)

            # For remaining non-black parts of warped_image, average with panorama if panorama is also non-black
            # This is a very crude form of blending in overlap
            overlap_mask = cv2.bitwise_and(mask_warped, cv2.bitwise_not(mask_panorama_is_black))
            if cv2.countNonZero(overlap_mask) > 0:
                # Simple averaging (can cause ghosting if misaligned)
                # panorama_roi = cv2.bitwise_and(panorama, panorama, mask=overlap_mask)
                # warped_roi = cv2.bitwise_and(warped_image, warped_image, mask=overlap_mask)
                # blended_roi = cv2.addWeighted(panorama_roi, 0.5, warped_roi, 0.5, 0)
                # panorama = cv2.copyTo(blended_roi, overlap_mask, panorama)
                
                # Simpler: just overwrite (later images on top)
                panorama[overlap_mask > 0] = warped_image[overlap_mask > 0]


    return panorama

# --- 主流程 ---
# 假设 imgs, keypoints_list, all_good_matches, img_nums 已定义

# 1. 计算所有图像对之间的单应性矩阵
# all_homographies_pairwise[(i,j)] 表示将图像 j 变换到图像 i 的坐标系
print("Calculating pairwise homographies...")
all_homographies_pairwise = calculate_all_homographies(keypoints_list, all_good_matches, img_nums)

# 检查是否有足够的同形异义词
if not any(all_homographies_pairwise.values()):
    print("Error: No homographies could be computed. Cannot stitch.")
    # exit() # 或者返回错误

# 2. 拼接图像
print("\nStitching images sequentially...")
# 确保 imgs 列表是有效的图像数据
valid_imgs = [img for img in imgs if img is not None and img.size > 0]
if len(valid_imgs) != img_nums:
    print(f"Warning: Number of valid images ({len(valid_imgs)}) doesn't match img_nums ({img_nums}).")
    # Potentially update img_nums and filter other lists or handle error

stitched_panorama = stitch_images_sequentially(valid_imgs, all_homographies_pairwise)

if stitched_panorama is not None:
    # (可选) 裁剪掉边缘的黑色区域 (高级步骤)
    # stitched_panorama = crop_black_borders(stitched_panorama)

    cv2.imshow("Stitched Panorama", stitched_panorama)
    cv2.imwrite("stitched_panorama.jpg", stitched_panorama)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
else:
    print("Stitching failed.")