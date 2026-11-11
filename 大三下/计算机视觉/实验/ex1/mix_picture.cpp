#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
    Mat img = imread("image2.png", IMREAD_UNCHANGED);
    std::vector<Mat> channels;
    split(img, channels);
    Mat alpha = channels[3];
    imwrite("Alpha.jpg", alpha);

    Mat background = imread("background.png", IMREAD_COLOR);

    resize(background, background, Size(img.cols, img.rows)); 
    Mat foreground;
    cvtColor(img, foreground, COLOR_BGRA2BGR);
    
    Mat alpha_f;
    alpha.convertTo(alpha_f, CV_32F, 1.0 / 255.0);
    
    Mat background_f;
    background.convertTo(background_f, CV_32F, 1.0 / 255.0);
    
    Mat alpha_3ch;
    Mat alpha_channels[] = {alpha_f, alpha_f, alpha_f};
    merge(alpha_channels, 3, alpha_3ch);
    
    Mat foreground_float;
    foreground.convertTo(foreground_float, CV_32FC3);
    Mat background_float;
    background.convertTo(background_float, CV_32FC3);
    // std::cout << "Foreground: " << foreground_float.size() 
    // << ", Channels: " << foreground_float.channels() 
    // << ", Type: " << foreground_float.type() << std::endl;
    
    // std::cout << "Background: " << background_float.size() 
    // << ", Channels: " << background_float.channels() 
    // << ", Type: " << background_float.type() << std::endl;
    
    // std::cout << "Alpha_3ch: " << alpha_3ch.size() 
    // << ", Channels: " << alpha_3ch.channels() 
    // << ", Type: " << alpha_3ch.type() << std::endl;
    // return 0;
    // Mat blended = foreground.mul(alpha_3ch, 1.0);// + background.mul(Scalar::all(1.0) - alpha_3ch, 1.0);
    Mat blended = foreground_float.mul(alpha_3ch) + 
    background_float.mul(Scalar::all(1.0) - alpha_3ch);
    // 计算混合
    // return 0;    

    // 显示和保存结果
    imshow("Blended Result", blended);
    waitKey(1000);
    destroyAllWindows();
    imwrite("blended_result.jpg", blended);
    return 0;

}