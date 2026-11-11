#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>
cv::Mat image;

cv::Mat create_kernel(float sigma){
    int size_kernel = cvRound(sigma * 6) | 1; 
    int radius = size_kernel / 2;
    // int radius = 5;
    cv::Mat kernel(size_kernel, size_kernel, CV_32FC1);
    float sigma2 = sigma * sigma;
    float sigma_6 = sigma2 *sigma2 *sigma2;
    float scale = 1.0 / (2* M_PI *sigma_6);
    for(int y = -radius; y <= radius; y++){
        for(int x = -radius; x <= radius; x++){
            float r_sq = x*x + y*y;
            float exponent = cv::exp(-r_sq/ (2* sigma2));
            float value = (r_sq - 2 * sigma2) * exponent * scale;
            kernel.at<float>(y+radius, x + radius) = value;
        }
    }
    return kernel;
}


int main(){
	image = cv::imread("input.png", cv::IMREAD_GRAYSCALE);
    cv::Mat kernel2 = create_kernel(2);
    cv::Mat kernel3 = create_kernel(30);
    cv::Mat filter_image2, filter_image3, image2, image3;
    cv::filter2D(image, filter_image2, CV_32F, kernel2);
    cv::filter2D(image, filter_image3, CV_32F, kernel3);
    cv::normalize(filter_image2, image2, 0, 255, cv::NORM_MINMAX, CV_8U);
    cv::normalize(filter_image3, image3, 0, 255, cv::NORM_MINMAX, CV_8U);
    cv::imwrite("sigma_2.png", image2);
    cv::imwrite("sigma_30.png", image3);
}
