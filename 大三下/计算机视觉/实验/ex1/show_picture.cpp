#include <opencv2/opencv.hpp>

#include <iostream>



int main() {

    cv::Mat image = cv::imread("test.jpg", cv::IMREAD_COLOR);

    cv::namedWindow("Display Window", cv::WINDOW_AUTOSIZE);

    cv::imshow("Display Window", image);

    cv::waitKey(0);

    cv::destroyAllWindows();



    return 0;

}