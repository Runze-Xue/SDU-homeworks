# --- 模块三：投影与裁剪 (Projection and Cropping Phase) ---
# OpenCV的Stitcher类在内部处理了投影。
# 我们需要做的是裁剪掉拼接后产生的黑色边框。
import cv2
def crop_black_borders(image):
    """
    裁剪掉图像四周的黑色边框（假设黑色为(0,0,0)）。
    """
    if image is None:
        return None

    print("\nCropping black borders...") 
    # 转换为灰度图
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # 设置阈值，将非黑色像素变为白色，黑色像素保持黑色
    # cv2.threshold(src, thresh, maxval, type)
    # 如果像素值 > thresh，则设为 maxval。否则，根据type设置。
    # THRESH_BINARY: > thresh -> maxval, else -> 0
    _, thresh = cv2.threshold(gray, 1, 255, cv2.THRESH_BINARY) # 任何非纯黑的像素都会>1

    # 查找轮廓
    # cv2.findContours(image, mode, method)
    # RETR_EXTERNAL: 只检测最外层轮廓
    # CHAIN_APPROX_SIMPLE: 压缩水平、垂直和对角线段，只保留它们的端点
    if cv2.__version__.startswith('3.'):
        _, contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    else: # OpenCV 4.x+
        contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    if not contours:
        print("Warning: No contours found for cropping. Returning original image.")
        return image

    # 找到最大的轮廓（即有效图像区域）
    cnt = max(contours, key=cv2.contourArea)

    # 获取该轮廓的边界框
    x, y, w, h = cv2.boundingRect(cnt)

    # 裁剪图像
    cropped_img = image[y:y+h, x:x+w]
    print(f"Cropped from ({image.shape[1]}x{image.shape[0]}) to ({cropped_img.shape[1]}x{cropped_img.shape[0]})")
    return cropped_img