import cv2
import numpy as np
import time

def main():
    GOOD_MATCH_PERCENT = 0.15  
    MIN_MATCH_COUNT = 10     
    template = cv2.imread("template.jpg", cv2.IMREAD_COLOR)
    # template = cv2.imread("template2.png", cv2.IMREAD_COLOR)
    # template = cv2.imread("template3.jpg", cv2.IMREAD_COLOR)
    hT, wT = template.shape[:2]
    detector = cv2.AKAZE_create()
    kp1, des1 = detector.detectAndCompute(template, None)

    matcher = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)

    cap = cv2.VideoCapture(0)

    prev_time = time.time()
    while True:
        ret, frame = cap.read()
        if not ret:
            break
        hF, wF = frame.shape[:2]
        scale = hF / float(hT)
        tpl_resized = cv2.resize(template, None, fx=scale, fy=scale)
        vis = np.hstack((tpl_resized, frame))
        offset = tpl_resized.shape[1]
        kp2, des2 = detector.detectAndCompute(frame, None)
        matched_pts = []  
        if des2 is not None and len(kp2) >= MIN_MATCH_COUNT:
            matches = matcher.match(des1, des2)
            matches = sorted(matches, key=lambda x: x.distance)
            keep = max(int(len(matches)*GOOD_MATCH_PERCENT), MIN_MATCH_COUNT)
            matches = matches[:keep]
            for m in matches:
                x1, y1 = kp1[m.queryIdx].pt
                x1, y1 = int(x1*scale), int(y1*scale)
                x2, y2 = kp2[m.trainIdx].pt
                x2_int, y2_int = int(x2), int(y2)
                matched_pts.append((x2_int, y2_int))
            if len(matched_pts) >= MIN_MATCH_COUNT:
                xs, ys = zip(*matched_pts)
                x_min, x_max = min(xs), max(xs)
                y_min, y_max = min(ys), max(ys)
                top_left     = (x_min + offset, y_min)
                bottom_right = (x_max + offset, y_max)
                cv2.rectangle(vis, top_left, bottom_right, (255,0,0), 3)
        cv2.imshow("match", vis)

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
