#include <opencv2/opencv.hpp>
#include <cmath>
#include <vector>

cv::Mat gaussianBlur(const cv::Mat& src, double sigma) {
    int window_size = static_cast<int>(std::floor(6 * sigma - 1));
    if (window_size < 1) window_size = 1;
    if (window_size % 2 == 0) window_size += 1;
    int k_half = (window_size - 1) / 2;

    std::vector<double> kernel(window_size);
    double sum = 0.0;
    for (int i = 0; i < window_size; ++i) {
        double x = i - k_half;
        double g = std::exp(-(x * x) / (2 * sigma * sigma)) / (std::sqrt(2 * M_PI) * sigma);
        kernel[i] = g;
        sum += g;
    }
    for (int i = 0; i < window_size; ++i) {
        kernel[i] /= sum;
    }

    cv::Mat src_float;
    src.convertTo(src_float, CV_64F);

    cv::Mat temp = cv::Mat::zeros(src.size(), CV_64F);
    for (int i = 0; i < src_float.rows; ++i) {
        for (int j = 0; j < src_float.cols; ++j) {
            double sum_val = 0.0;
            for (int k = 0; k < window_size; ++k) {
                int pos = j - k_half + k;
                if (pos < 0) pos = -pos - 1;
                else if (pos >= src_float.cols) pos = 2 * src_float.cols - pos - 1;
                sum_val += src_float.at<double>(i, pos) * kernel[k];
            }
            temp.at<double>(i, j) = sum_val;
        }
    }
    cv::Mat dst_float = cv::Mat::zeros(src.size(), CV_64F);
    for (int j = 0; j < src_float.cols; ++j) {
        for (int i = 0; i < src_float.rows; ++i) {
            double sum_val = 0.0;
            for (int k = 0; k < window_size; ++k) {
                int pos = i - k_half + k;
                if (pos < 0) pos = -pos - 1;
                else if (pos >= src_float.rows) pos = 2 * src_float.rows - pos - 1;
                sum_val += temp.at<double>(pos, j) * kernel[k];
            }
            dst_float.at<double>(i, j) = sum_val;
        }
    }
    cv::Mat dst;
    dst_float.convertTo(dst, src.type());
    return dst;
}

int main() {
    cv::Mat image = cv::imread("input.png", cv::IMREAD_COLOR);
    double sigma = 10.0;
    std::vector<cv::Mat> channels;
    cv::split(image, channels);
    for (auto& channel : channels) {
        channel = gaussianBlur(channel, sigma);
    }
    cv::Mat blurred;
    cv::merge(channels, blurred);
    cv::imshow("Original", image);
    cv::imshow("Blurred", blurred);
    cv::waitKey(0);
    return 0;
}