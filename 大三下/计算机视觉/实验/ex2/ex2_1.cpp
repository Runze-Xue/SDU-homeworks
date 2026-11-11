#include <opencv2/opencv.hpp>
#include <chrono>
using namespace cv;

void alpha_blend_pointer(const Mat& img_float, const Mat& background_float, Mat& blended) {
    int width = img_float.cols;
    int height = img_float.rows;
    
    const uchar* img_row = img_float.data;
    const uchar* bg_row = background_float.data;
    uchar* blended_row = blended.data;
    
    const int img_step = img_float.step;
    const int bg_step = background_float.step;
    const int blended_step = blended.step;

    for (int y = 0; y < height; ++y) {
        const float* img_ptr = reinterpret_cast<const float*>(img_row);
        const float* bg_ptr = reinterpret_cast<const float*>(bg_row);
        float* blended_ptr = reinterpret_cast<float*>(blended_row);
        
        for (int x = 0; x < width; ++x) {
            const float B_fg = img_ptr[0];
            const float G_fg = img_ptr[1];
            const float R_fg = img_ptr[2];
            const float alpha = img_ptr[3];  // Alpha通道
            // printf("this is %d\n", x);
            // std::cout << "背景通道数: " << background_float.channels() << std::endl;

            const float inv_alpha = 1.0f - alpha;
            const float B_bg = bg_ptr[0];
            // return ;

            const float G_bg = bg_ptr[1];
            const float R_bg = bg_ptr[2];
            blended_ptr[0] = B_fg * alpha + B_bg * inv_alpha;
            blended_ptr[1] = G_fg * alpha + G_bg * inv_alpha;
            blended_ptr[2] = R_fg * alpha + R_bg * inv_alpha;
            img_ptr += 4;   
            bg_ptr += 3;    
            blended_ptr += 3;
        }
        
        img_row += img_step;
        bg_row += bg_step;
        blended_row += blended_step;
    }
}

int main() {
    Mat img = imread("image2.png", IMREAD_UNCHANGED);
    Mat background = imread("background.png", IMREAD_COLOR);
    resize(background, background, img.size());
    Mat foreground;
    cvtColor(img, foreground, COLOR_BGRA2BGR);    
    Mat alpha = Mat(img.rows, img.cols, CV_32FC1);
    if (img.channels() == 4) {
            img.reshape(1, img.rows).col(3).convertTo(alpha, CV_32F, 1/255.0);
        } else {
        alpha.setTo(1.0);
    }

    Mat background_float;
    background.convertTo(background_float, CV_32FC3, 1/255.0);
    Mat img_float;
    img.convertTo(img_float, CV_32FC4, 1/255.0); 
    Mat blended_pointer(img.size(), CV_32FC3);
    
    auto t1 = std::chrono::high_resolution_clock::now();
    alpha_blend_pointer(img_float, background_float, blended_pointer);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "指针方法耗时 μs: "<< std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count()<<'\n';
    Mat output;
    blended_pointer.convertTo(output, CV_8UC3, 255);
    imwrite("result.jpg", output);

    return 0;
}