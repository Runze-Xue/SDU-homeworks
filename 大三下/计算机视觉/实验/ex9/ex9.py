import cv2
import numpy as np
import matplotlib.pyplot as plt
import argparse

parse = argparse.ArgumentParser()
parse.add_argument('-i1', '--img1', type=str)
parse.add_argument('-i2', '--img2', type =str)

args = parse.parse_args()
image_name1 = args.img1
image_name2 = args.img2

img1 = cv2.imread(image_name1 + '.jpg')  
img2 = cv2.imread(image_name2 + '.jpg')

gray1 = cv2.cvtColor(img1, cv2.COLOR_BGR2GRAY)
gray2 = cv2.cvtColor(img2, cv2.COLOR_BGR2GRAY)

sift = cv2.SIFT_create()
kp1, des1 = sift.detectAndCompute(gray1, None)
kp2, des2 = sift.detectAndCompute(gray2, None)

FLANN_INDEX_KDTREE = 1
index_params = dict(algorithm=FLANN_INDEX_KDTREE, trees=5)
search_params = dict(checks=50)
flann = cv2.FlannBasedMatcher(index_params, search_params)
matches = flann.knnMatch(des1, des2, k=2)

good_matches = []
for m, n in matches:
    if m.distance < 0.7 * n.distance:
        good_matches.append(m)

pts1 = np.float32([kp1[m.queryIdx].pt for m in good_matches]).reshape(-1, 1, 2)
pts2 = np.float32([kp2[m.trainIdx].pt for m in good_matches]).reshape(-1, 1, 2)

F, mask_F = cv2.findFundamentalMat(pts1, pts2, cv2.FM_RANSAC, ransacReprojThreshold=1.0, confidence=0.99)

H, mask_H = cv2.findHomography(pts1, pts2, cv2.RANSAC, ransacReprojThreshold=5.0)

def draw_matches(img1, img2, pts1, pts2, mask, title):
    inlier_mask = mask.ravel() == 1
    outlier_mask = ~inlier_mask

    inlier_indices = np.where(inlier_mask)[0]
    outlier_indices = np.where(outlier_mask)[0]
    np.random.shuffle(inlier_indices)
    np.random.shuffle(outlier_indices)
    plot_indices = np.concatenate((inlier_indices[:50], outlier_indices[:20]))

    h1, w1 = img1.shape[:2]
    h2, w2 = img2.shape[:2]
    canvas = np.zeros((max(h1, h2), w1 + w2, 3), dtype=np.uint8)
    canvas[:h1, :w1] = img1
    canvas[:h2, w1:w1+w2] = img2

    for idx in plot_indices:
        color = (0, 255, 0) if inlier_mask[idx] else (0, 0, 255)  
        x1, y1 = pts1[idx].ravel()
        x2, y2 = pts2[idx].ravel()
        cv2.line(canvas, (int(x1), int(y1)), (int(x2)+w1, int(y2)), color, 5)
    
    plt.figure(figsize=(15, 10))
    plt.imshow(cv2.cvtColor(canvas, cv2.COLOR_BGR2RGB))
    plt.title(title)
    plt.axis('off')

draw_matches(img1, img2, pts1, pts2, mask_F, 'Fundamental Matrix + RANSAC')
draw_matches(img1, img2, pts1, pts2, mask_H, 'Homography + RANSAC')
plt.show()
print(f"总匹配点数: {len(good_matches)}")
print(f"基础矩阵内点数量: {np.sum(mask_F)}")
print(f"单应矩阵内点数量: {np.sum(mask_H)}")