#include <opencv2/opencv.hpp>
using namespace cv;

int slope = 50;     
int midpoint = 127;  
Mat original_img;    

Mat createScurve(float slope_val, float midpoint_val) {
    Mat lut(1, 256, CV_8UC1);
    float center = midpoint_val / 255.0f;
    float factor = slope_val * 0.1f; 
    
    for(int i=0; i<256; ++i) {
        float x = i / 255.0f;
        float s = 1 / (1 + exp(-factor*(x - center)));
        lut.at<uchar>(i) = saturate_cast<uchar>(s * 255);
    }
    return lut;
}

static void adjustContrast(int, void*) {
    float slope_val = slope * 0.2f;       
    float midpoint_val = midpoint;        
    Mat lut = createScurve(slope_val, midpoint_val);
    Mat adjusted_img;
    LUT(original_img, lut, adjusted_img);
    imshow("Contrast Adjustment", adjusted_img);

}

int main() {
    original_img = imread("input.jpg", IMREAD_GRAYSCALE);
    namedWindow("Contrast Adjustment", WINDOW_AUTOSIZE);
    
    createTrackbar("Slope", "Contrast Adjustment", &slope, 100, adjustContrast);
    createTrackbar("Midpoint", "Contrast Adjustment", &midpoint, 255, adjustContrast);
    
    adjustContrast(0,0);
    
    while(true) {
        int key = waitKey(10);
        if(key == 27) break; 
    }
    return 0;
}
