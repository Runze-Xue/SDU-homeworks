#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

void createMirrorMap(Mat& mapx, Mat& mapy, float distortion) {
    int width = mapx.cols;
    int height = mapx.rows;
    int cx = width / 2;
    int cy = height / 2;
    #pragma omp parallel for
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            float dx = (x - cx) / float(cx);
            float dy = (y - cy) / float(cy); 
            float r = sqrt(dx*dx + dy*dy);
            float theta = atan2(dy, dx);
            float new_r = r * (1 - distortion * r*r);           
            new_r = min(max(new_r, 0.0f), 1.0f);      
            mapx.at<float>(y, x) = (new_r * cos(theta) * cx) + cx;
            mapy.at<float>(y, x) = (new_r * sin(theta) * cy) + cy;
        }
    }
}

int main() {
    VideoCapture cap(0);
    int width = (int)cap.get(CAP_PROP_FRAME_WIDTH);
    int height = (int)cap.get(CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(CAP_PROP_FPS);
    fps = fps > 0 ? fps : 30.0;
    Mat mapx(height, width, CV_32FC1);
    Mat mapy(height, width, CV_32FC1);
    createMirrorMap(mapx, mapy, 0.6f); 
    VideoWriter writer;
    string output_file = "output.mp4";
    int  codec = VideoWriter::fourcc('m','p','4','v');     
    writer.open(output_file, codec, fps, Size(width, height), true);

    while (true) {
        Mat frame, distorted;
        cap >> frame;
        if (frame.empty()) break;
        remap(frame, distorted, mapx, mapy, INTER_LINEAR, BORDER_REPLICATE);
        imshow("Mirror Effect", distorted);
        if (writer.isOpened()) writer << distorted;
        if (waitKey(1) == 27) break;
    }
    cap.release();
    writer.release();
    destroyAllWindows();
    return 0;
}