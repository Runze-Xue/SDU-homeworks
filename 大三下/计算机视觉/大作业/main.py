
import cv2
import argparse
from video_pick import load_images_from_folder, load_images_from_video
from image_stitcher import stitch_images
from image_combine import combine_images
from project_crop import crop_black_borders
from post_process import post_process_image, save_image
import os
import time 

start_time =  time.time()
ascii_temp_dir = "C:\\Temp\\cv_app_temp" 

if not os.path.exists(ascii_temp_dir):
    try:
        os.makedirs(ascii_temp_dir)
        print(f"tmp dictionary: {ascii_temp_dir}")
    except OSError as e:
        print(f"can't create tmp dictionary {ascii_temp_dir}: {e}. ")
        exit() 

print(f"set tmp dictionary as : {ascii_temp_dir}")
os.environ['TEMP'] = ascii_temp_dir
os.environ['TMP'] = ascii_temp_dir


if __name__ == "__main__":
    
    parser = argparse.ArgumentParser(description="Stitch images into a panorama.")
    parser.add_argument("--video_path", type=str, default = None, help=" path of  video to pick, allow none")
    parser.add_argument("--video_internal", type=int, default = None, help=" internal of  video to pick, allow none")
    parser.add_argument("--images", type=str, default = "images", help="Folder containing images to stitch.")
    parser.add_argument("-o", "--output", type=str, default="panorama_output.jpg", help="Output panorama image file.")
    parser.add_argument("-t", "--type", type=str, default = "ctr", help= "exp or ctr")
    args = parser.parse_args()
    print("Module1 : Pick images from videos ")
    if args.video_path is None:
        input_images = load_images_from_folder(args.images) #列表，元素格式为numpy.ndarray格式 存储图片
    else:
        input_images = load_images_from_video(args.video_path, args.video_internal)
    # sift = cv2.SIFT_create()
    # for i, img in enumerate(input_images):    
    #     kp = sift.detect(img, None)
    #     print(f"图像{i}特征点数: {len(kp)}") 
    #     vis = cv2.drawKeypoints(img, kp, None, flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
    #     cv2.imwrite(f"features_{i}.jpg", vis)
    print("--- Module 2: Image Stitching ---")
    if args.type == "ctr":
        stitcher_mode_flag = cv2.Stitcher_PANORAMA 
        panorama, status = stitch_images(input_images, stitcher_mode=stitcher_mode_flag)

        if status != cv2.Stitcher_OK:
            print("Stitching process failed. Exiting.")
            exit()
    else:
        panorama, status = combine_images(input_images)
    # print("--- Module 3: Cropping ---")
    # cropped_panorama = crop_black_borders(panorama)
    # if cropped_panorama is None:
    #     print("Cropping failed. Using uncropped panorama.")
    #     cropped_panorama = panorama
    # print("--- Module 4: Post-Processing and Output ---")
    # final_panorama = post_process_image(cropped_panorama, brightness=5, contrast=1.05, sharpness_strength=0.1)
    final_panorama = panorama
    save_image(final_panorama, args.output)
    end_time = time.time()
    print("use_time", end_time-start_time)
    # cv2.imshow("Source Image 1 (example)", input_images[0])
    # cv2.imshow("Stitched Panorama", final_panorama)
    # print("Press any key to close windows...")
    # cv2.waitKey(0)
    # cv2.destroyAllWindows()



