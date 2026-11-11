import cv2
import numpy as np
import os


def load_images_from_video():
    print("under building")


#从文件夹中加载图片
def load_images_from_folder(folder_path):
    images = []
    valid_extensions = ('.png', '.jpg')
    filenames = sorted([f for f in os.listdir(folder_path) if f.lower().endswith(valid_extensions)])

    print(f"Loading {len(filenames)} images from '{folder_path}':")
    
    for filename in filenames:
        img_path = os.path.join(folder_path, filename)
        img = cv2.imread(img_path)
        images.append(img)
        print(f"  Loaded {filename} ({img.shape[1]}x{img.shape[0]})")
    return images

