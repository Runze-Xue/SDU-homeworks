import cv2
import numpy as np
import matplotlib.pyplot as plt
import argparse
import draw as dr

parse = argparse.ArgumentParser()
parse.add_argument('-l', '--left', type=str, required=True)
parse.add_argument('-r', '--right', type=str, required=True)
args = parse.parse_args()

# img_left = cv2.imread(args.left + '.jpg', cv2.IMREAD_GRAYSCALE)
# img_right = cv2.imread(args.right + '.jpg', cv2.IMREAD_GRAYSCALE)
img_left = cv2.imread(args.left + '.jpg')
img_right = cv2.imread(args.right + '.jpg')

scale_factor = 1  
if scale_factor != 1:
    img_left = cv2.resize(img_left, None, fx=scale_factor, fy=scale_factor)
    img_right = cv2.resize(img_right, None, fx=scale_factor, fy=scale_factor)

sift = cv2.SIFT_create()
kp1, des1 = sift.detectAndCompute(img_left, None)
kp2, des2 = sift.detectAndCompute(img_right, None)


FLANN_INDEX_KDTREE = 1
index_params = dict(algorithm=FLANN_INDEX_KDTREE, trees=4)  
search_params = dict(checks=50)  
flann = cv2.FlannBasedMatcher(index_params, search_params)

matches12 = flann.knnMatch(des1, des2, k=2)
matches21 = flann.knnMatch(des2, des1, k=2)

good_matches12 = [m for m, n in matches12]
good_matches21 = [m for m, n in matches21]

# print("good-match l r", good_matches12)
# print("good_matches r l", good_matches21)
reverse_dict = {m.trainIdx: m.queryIdx for m in good_matches21}  
print("reverse_dict示例:", list(reverse_dict.items())[:5])


# good_matches = [m for m in good_matches12 if reverse_dict.get(m.trainIdx, -1) == m.queryIdx]
good_matches = [m for m in good_matches12]

# print("pre --- pre goodmatches", good_matches)
kp1_pts = np.array([kp.pt for kp in kp1])  
kp2_pts = np.array([kp.pt for kp in kp2])
pts1 = kp1_pts[[m.queryIdx for m in good_matches]]
pts2 = kp2_pts[[m.trainIdx for m in good_matches]]

scaled_spatial_threshold = 100 * scale_factor 
scaled_ransac_threshold = 3.0 * scale_factor  

dists = np.linalg.norm(pts1 - pts2, axis=1)
# print("pre_good_matches", good_matches)
good_matches = [m for m, d in zip(good_matches, dists) if d < scaled_spatial_threshold ] 
# print("good_matches", good_matches)

pts1 = np.float32([kp1[m.queryIdx].pt for m in good_matches]).reshape(-1, 1, 2)
pts2 = np.float32([kp2[m.trainIdx].pt for m in good_matches]).reshape(-1, 1, 2)
print("pts1", pts1)
print("pts2", pts2)

F, mask_F = cv2.findFundamentalMat(
    pts1, pts2, cv2.FM_RANSAC, 
    ransacReprojThreshold=scaled_ransac_threshold,  
    confidence=0.99
)
inliers_pts1 = pts1[mask_F.ravel() == 1]
inliers_pts2 = pts2[mask_F.ravel() == 1]
_, H1, H2 = cv2.stereoRectifyUncalibrated(inliers_pts1, inliers_pts2, F, img_left.shape[:2])

rect_left = cv2.warpPerspective(img_left, H1, (img_left.shape[1], img_left.shape[0]), 
                               flags=cv2.INTER_LINEAR | cv2.WARP_FILL_OUTLIERS)
rect_right = cv2.warpPerspective(img_right, H2, (img_right.shape[1], img_right.shape[0]),
                                flags=cv2.INTER_LINEAR | cv2.WARP_FILL_OUTLIERS)

def draw_lines(img, num=10):
    step = img.shape[0] // num
    for y in range(0, img.shape[0], step):
        cv2.line(img, (0, y), (img.shape[1], y), (0, 255, 0), 1)
    return img

plt.figure(figsize=(20, 10))
titles = ["Rectified Left", "Rectified Right", "Original Left", "Original Right"]
images = [draw_lines(rect_left), draw_lines(rect_right), 
          draw_lines(img_left.copy()), draw_lines(img_right.copy())]

for i in range(4):
    plt.subplot(2, 2, i+1)
    plt.imshow(cv2.cvtColor(images[i], cv2.COLOR_BGR2RGB))
    plt.title(titles[i])
    plt.axis('off')

plt.tight_layout()
plt.show()