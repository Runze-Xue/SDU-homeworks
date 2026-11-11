import cv2
def stitch_images(images, stitcher_mode=cv2.Stitcher_PANORAMA, try_gpu=False):
    if len(images) < 2:
        print("Error: Need at least two images to stitch.")
        return None, cv2.Stitcher_ERR_NEED_MORE_IMGS

    print("Initializing Stitcher...")
    # hasattr 用来检测版本
    if hasattr(cv2, 'Stitcher_create'):
        stitcher = cv2.Stitcher_create(mode=stitcher_mode) # 创建stitcher实例
    elif hasattr(cv2.Stitcher, 'create'):
        stitcher = cv2.Stitcher.create(mode=stitcher_mode)
    else:
        print("Error")
        return None, 
    print("Attempting to stitch images...")
    #stitched_img 是拼接好的图片numpy.ndarray
    status, stitched_img = stitcher.stitch(images)
    if status == cv2.Stitcher_OK:
        print("Stitching successful!")
        return stitched_img, status
    elif status == cv2.Stitcher_ERR_NEED_MORE_IMGS:
        print("Error: Not enough images to stitch, or not enough keypoints found/matched.")
    elif status == cv2.Stitcher_ERR_HOMOGRAPHY_EST_FAIL:
        print("Error: Homography estimation failed. Check image overlap and content.")
    elif status == cv2.Stitcher_ERR_CAMERA_PARAMS_ADJUST_FAIL:
        print("Error: Camera parameters adjustment failed.")
    else:
        print(f"Stitching failed with status code: {status}")

    return None, status
