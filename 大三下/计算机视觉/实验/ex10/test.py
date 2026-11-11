import cv2
import numpy as np
import argparse

parse = argparse.ArgumentParser()
parse.add_argument('-l', '--left', type=str)
parse.add_argument('-r', '--right', type=str)

args = parse.parse_args()

# 1. 参数配置
use_orb = True
n_features = 800

# 2. 读取图像
imgL = cv2.imread(args.left + '.jpg')
imgR = cv2.imread(args.right + '.jpg')

# 3. 特征检测与描述
if use_orb:
    detector = cv2.ORB_create(nfeatures=n_features)
else:
    detector = cv2.SIFT_create(nfeatures=n_features)

kp1, des1 = detector.detectAndCompute(imgL, None)
kp2, des2 = detector.detectAndCompute(imgR, None)

# 4. 匹配器（一步互检）
if use_orb:
    matcher = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)
else:
    matcher = cv2.BFMatcher(cv2.NORM_L2, crossCheck=True)

matches = matcher.match(des1, des2)
# 只取前200个匹配，减少后续计算量
matches = sorted(matches, key=lambda x: x.distance)[:200]

# 5. 直接用 OpenCV 画出匹配（C++ 层面加速）
matched_vis = cv2.drawMatches(
    imgL, kp1, imgR, kp2, matches, None,
    matchColor=(0,255,0), singlePointColor=(0,0,255),
    flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS
)
cv2.imshow("Fast Matches", matched_vis)
cv2.waitKey(1)

# 6. 构造匹配点数组
pts1 = np.float32([kp1[m.queryIdx].pt for m in matches]).reshape(-1,1,2)
pts2 = np.float32([kp2[m.trainIdx].pt for m in matches]).reshape(-1,1,2)

# 7. RANSAC 估计基础矩阵
F, mask = cv2.findFundamentalMat(pts1, pts2, cv2.FM_RANSAC, 1.0)
if F is None or mask is None:
    raise RuntimeError("Fundamental matrix estimation failed")

# 8. 取内点
in1 = pts1[mask.ravel()==1]
in2 = pts2[mask.ravel()==1]

# 9. 未标定矫正
_, H1, H2 = cv2.stereoRectifyUncalibrated(in1, in2, F, imgSize=(imgL.shape[1], imgL.shape[0]))
rectL = cv2.warpPerspective(imgL, H1, (imgL.shape[1], imgL.shape[0]))
rectR = cv2.warpPerspective(imgR, H2, (imgR.shape[1], imgR.shape[0]))

# 10. 直接用 OpenCV 显示结果
cv2.imshow("Rectified Left",  rectL)
cv2.imshow("Rectified Right", rectR)
cv2.waitKey(0)
cv2.destroyAllWindows()
