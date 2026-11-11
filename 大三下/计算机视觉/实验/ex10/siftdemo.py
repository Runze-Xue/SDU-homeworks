import cv2
import numpy as np
import matplotlib.pyplot as plt
import argparse

def process_and_match(feature_extractor, norm_type, algo_name, cross_check=True):
    kp1, des1 = feature_extractor.detectAndCompute(img1, None)
    kp2, des2 = feature_extractor.detectAndCompute(img2, None)

    bf = cv2.BFMatcher(norm_type, crossCheck=cross_check)
    matches = bf.match(des1, des2)
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
    return kp1, kp2, matches

parse = argparse.ArgumentParser()
parse.add_argument('-l', '--left', type=str, required=True)
parse.add_argument('-r', '--right', type=str, required=True)
args = parse.parse_args()

img_left = cv2.imread(args.left + '.jpg')
img_right = cv2.imread(args.right + '.jpg')
img1 = cv2.imread('image1.jpg', cv2.IMREAD_GRAYSCALE)
img2 = cv2.imread('image2.jpg', cv2.IMREAD_GRAYSCALE)


sift = cv2.SIFT_create()
process_and_match(sift, cv2.NORM_L2, "SIFT")


