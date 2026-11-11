#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::Mat image = cv::imread("test.jpg", cv::IMREAD_COLOR);
    // 创建命名窗口（可调整大小）
    cv::namedWindow("Display Window", cv::WINDOW_AUTOSIZE);

    // 在窗口中显示图像
    cv::imshow("Display Window", image);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}