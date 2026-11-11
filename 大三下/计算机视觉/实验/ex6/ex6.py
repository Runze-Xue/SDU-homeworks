import cv2
import numpy as np
import matplotlib.pyplot as plt
import time

img1 = cv2.imread('img1.jpg', cv2.IMREAD_GRAYSCALE)
img2 = cv2.imread('img2.jpg', cv2.IMREAD_GRAYSCALE)

def process_and_match(feature_extractor, norm_type, algo_name, cross_check=True):
    start_time = time.time()
    kp1, des1 = feature_extractor.detectAndCompute(img1, None)
    kp2, des2 = feature_extractor.detectAndCompute(img2, None)
    detection_time = time.time() - start_time
    bf = cv2.BFMatcher(norm_type, crossCheck=cross_check)
    start_time = time.time()
    matches = bf.match(des1, des2)
    matching_time = time.time() - start_time
    matches = sorted(matches, key=lambda x: x.distance)
    img_matches = cv2.drawMatches(img1, kp1, img2, kp2, matches[:30], None, flags=cv2.DrawMatchesFlags_NOT_DRAW_SINGLE_POINTS,matchesThickness=3)
    plt.figure(figsize=(12, 6))
    plt.title(f'{algo_name} match result')
    plt.imshow(img_matches[:, :, ::-1]) 
    plt.axis('off')
    plt.show()

    print(f"{algo_name} algorithm")
    print(f"key points in image1 {len(kp1)}")
    print(f"key points in image2 {len(kp2)}")
    print(f"matches number {len(matches)}")
    print(f"detection time cost {detection_time:.4f} 秒")
    print(f"matching time cost{matching_time:.4f} 秒")
    return kp1, kp2, matches

sift = cv2.SIFT_create()
process_and_match(sift, cv2.NORM_L2, "SIFT")
akaze = cv2.AKAZE_create()
process_and_match(akaze, cv2.NORM_HAMMING, "AKAZE")
orb = cv2.ORB_create(nfeatures=500)
process_and_match(orb, cv2.NORM_HAMMING, "ORB")

