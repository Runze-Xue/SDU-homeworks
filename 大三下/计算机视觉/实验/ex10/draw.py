import cv2
import matplotlib.pyplot as plt
import numpy as np
def draw_matches_with_ransac(img1, img2, pts1, pts2, mask_F, 
                            max_display_matches=100, 
                            inlier_color=(0, 255, 0),  
                            outlier_color=(0, 0, 255)): 
    
    h1, w1 = img1.shape[:2]
    h2, w2 = img2.shape[:2]
    canvas = np.zeros((max(h1, h2), w1 + w2, 3), dtype=np.uint8)
    canvas[:h1, :w1] = img1
    canvas[:h2, w1:w1+w2] = img2
    
    mask = mask_F.ravel().astype(bool)
    inlier_indices = np.where(mask)[0]
    outlier_indices = np.where(~mask)[0]
    
    np.random.shuffle(inlier_indices)
    np.random.shuffle(outlier_indices)
    
    display_indices = np.concatenate([
        inlier_indices[:max_display_matches//2], 
        outlier_indices[:max_display_matches//2]
    ])
    
    for idx in display_indices:
        color = inlier_color if mask[idx] else outlier_color
        pt1 = tuple(map(int, pts1[idx].ravel()))
        pt2 = tuple(map(int, pts2[idx].ravel()))
        end_pt = (pt2[0] + w1, pt2[1]) 
        cv2.line(canvas, pt1, end_pt, color, thickness=2, lineType=cv2.LINE_AA)
        cv2.circle(canvas, pt1, 5, color, -1)  
        cv2.circle(canvas, end_pt, 5, color, -1)
    
    canvas_rgb = cv2.cvtColor(canvas, cv2.COLOR_BGR2RGB)

    plt.figure(figsize=(20, 10))
    plt.imshow(canvas_rgb)
    plt.title(f"Feature Matching with RANSAC Inliers (Green) and Outliers (Red)\n"
              f"Total matches: {len(pts1)}  |  Inliers: {np.sum(mask)}  "
              f"({np.sum(mask)/len(mask):.1%})")
    plt.axis('off')
    plt.show()

