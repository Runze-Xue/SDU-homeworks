import cv2
import numpy as np
def post_process_image(image, brightness=0, contrast=1.0, sharpness_strength=0.0):
    """
    对图像进行简单的后期处理。
    brightness: 亮度调整值 (-255 to 255)
    contrast: 对比度调整因子 (e.g., 1.0 for no change, >1 for more, <1 for less)
    sharpness_strength: 锐化强度 (0.0 to 1.0 or more)
    """
    if image is None:
        return None

    print("\nApplying post-processing...")
    processed_image = image.copy()

    # 亮度与对比度调整
    # new_image = alpha * original_image + beta
    # alpha: contrast, beta: brightness
    if brightness != 0 or contrast != 1.0:
        print(f"  Adjusting brightness by {brightness}, contrast by {contrast}")
        processed_image = cv2.convertScaleAbs(processed_image, alpha=contrast, beta=brightness)

    # 锐化 (简单的拉普拉斯锐化示例)
    if sharpness_strength > 0:
        print(f"  Applying sharpening with strength {sharpness_strength}")
        gray = cv2.cvtColor(processed_image, cv2.COLOR_BGR2GRAY)
        laplacian = cv2.Laplacian(gray, cv2.CV_64F)
        sharpened_laplacian = cv2.convertScaleAbs(laplacian)
        # 将锐化细节添加到原图的亮度通道，或简单地与原图混合
        # 这是一个非常基础的锐化，实际应用可能需要更复杂的方法
        # Convert sharpened_laplacian to 3 channels to add
        sharpened_laplacian_bgr = cv2.cvtColor(sharpened_laplacian, cv2.COLOR_GRAY2BGR)
        # Blend: a weighted sum. Be careful with data types and clipping.
        # Ensure processed_image is float for addition if necessary, then clip and convert back to uint8
        # A simpler approach (might not be ideal for strong sharpening):
        # processed_image = cv2.addWeighted(processed_image, 1 + sharpness_strength, sharpened_laplacian_bgr, -sharpness_strength, 0)
        # More robust: apply sharpening kernel
        kernel = np.array([[0, -1, 0],
                           [-1,  5, -1],
                           [0, -1, 0]], np.float32) * sharpness_strength + \
                 np.array([[0, 0, 0],
                           [0, 1, 0],
                           [0, 0, 0]], np.float32) * (1-sharpness_strength) # Blend with identity
        # Ensure kernel sums to 1 if sharpness_strength is low to avoid darkening, or handle carefully
        # A common sharpening kernel:
        if sharpness_strength > 0.01: # Only apply if strength is meaningful
            # Using a standard sharpening kernel and scaling its effect
            sharp_kernel = np.array([[-1, -1, -1],
                                    [-1,  9, -1],
                                    [-1, -1, -1]])
            # Apply kernel
            sharpened_img = cv2.filter2D(processed_image, -1, sharp_kernel)
            # Blend sharpened with original based on strength
            processed_image = cv2.addWeighted(processed_image, 1.0 - sharpness_strength, sharpened_img, sharpness_strength, 0)
            processed_image = np.clip(processed_image, 0, 255).astype(np.uint8)


    print("Post-processing complete.")
    return processed_image


def save_image(image, output_path):
    """
    保存图像。
    """
    if image is None:
        print("Error: No image to save.")
        return False

    try:
        cv2.imwrite(output_path, image)
        print(f"\nPanorama saved to: {output_path}")
        return True
    except Exception as e:
        print(f"Error saving image: {e}")
        return False