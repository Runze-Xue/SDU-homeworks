#include "Triangle.hpp"

#include "rasterizer.hpp"

#include <eigen3/Eigen/Eigen>

#include <iostream>

#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;



Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)

{

    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();



    Eigen::Matrix4f translate;

    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1,

        -eye_pos[2], 0, 0, 0, 1;

    view = translate * view;



    return view;

}



Eigen::Matrix4f get_model_matrix(float rotation_angle)

{

    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    model<<cos(rotation_angle),-sin(rotation_angle),0, 0,

     sin(rotation_angle),cos(rotation_angle),0,0,

     0,0,1.0f,0,

     0,0,0,1.0f;

    return model;

}

Eigen::Matrix4f get_rotation(Vector3f axis, float angle){

    Eigen::Matrix3f model;

    double uinit = sqrt(axis[0]*axis[0] + axis[1]*axis[1] + axis[2]*axis[2]);

    axis = axis/uinit;

    model<<

    0, -axis[2], axis[1], 

    axis[2], 0, -axis[0],

    -axis[1], axis[0], 0; 

    Eigen::Matrix3f I = Eigen::Matrix3f::Identity();

    Eigen::Matrix3f result = I + sin(angle)*model + (1-cos(angle))*model*model;

    Eigen::Matrix4f final2;

    

    final2<<

    result(0,0), result(0,1), result(0,2), 0,

    result(1,0), result(1,1), result(1,2), 0,

    result(2,0), result(2,1), result(2,2), 0,

    0, 0, 0, 1;

    return final2;

}



Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,

                                      float zNear, float zFar)

{

    // Students will implement this function

    float n = zNear;

    float f = zFar;

    float t = zNear*tan(eye_fov/2);

    float b = -zNear*tan(eye_fov/2);

    float r = aspect_ratio*zNear*tan(eye_fov/2);

    float l = -aspect_ratio*zNear*tan(eye_fov/2);

    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();



    projection<<

    2*n/(r-l), 0, (r + l)/(r - l), 0,

    0, 2*n/(t-b), (t+b)/(t-b), 0,

    0, 0, -(f + n)/(f - n), -2*f*n/(f - n),

    0, 0, -1, 0;



    return projection;

}



int main(int argc, const char** argv)

{

    float angle = 0;

    bool command_line = false;

    std::string filename = "output.png";

    int sym;

    printf("1 chose z rot, 2 chose free rot\n");

    scanf("%d",&sym);

    Vector3f RotV;

    if(sym == 2){

        double rot1, rot2, rot3;

        printf("Put in A vector with 3 parameter\n");

        scanf("%lf%lf%lf",&rot1,&rot2,&rot3);

        RotV<<rot1,rot2,rot3;

    }

    if (argc >= 3) {

        command_line = true;

        angle = std::stof(argv[2]); // -r by default

        if (argc == 4) {

            filename = std::string(argv[3]);

        }

        else

            return 0;

    }



    rst::rasterizer r(700, 700);



    Eigen::Vector3f eye_pos = {0, 0, 12};



    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};



    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};



    auto pos_id = r.load_positions(pos);

    auto ind_id = r.load_indices(ind);



    int key = 0;

    int frame_count = 0;



    if (command_line) {

        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        printf("in");

        r.set_model(get_model_matrix(angle));

        r.set_view(get_view_matrix(eye_pos));

        r.set_projection(get_projection_matrix(5, 1, 0.1, 50));



        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());

        image.convertTo(image, CV_8UC3, 1.0f);



        cv::imwrite(filename, image);



        return 0;

    }



    while (key != 27) {

        Matrix4f m2;

        m2<<-5.20052,  6.04823 ,0.203055 ,       0,

        1.88844 ,-5.20052  ,5.74945  ,      0,

        5.74945, 0.203055 ,-6.93667 ,       0,

        0 ,       0   ,     0    ,    1;

        printf("first\n");

        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        if(sym == 1)r.set_model(get_model_matrix(angle));

        else if(sym == 2)r.set_model(get_rotation(RotV, angle));

        // else if(sym == 2)r.set_model(m2);

        // std::cout<<get_rotation(RotV, angle)<<'\n';

        printf("in\n");

        r.set_view(get_view_matrix(eye_pos));   

        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));
        std::cout<<"Matrix put in as follow\n"<<m2<<'\n';
        printf("out3\n");
        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        printf("out2\n");

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());

        printf("out\n");

        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imshow("image", image);

        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if(key == 'a' || key == 'd'){

            if (key == 'a') {

                angle += 1;

            }

            else if (key == 'd') {

                angle -= 1;

            }

            }

        }

    return 0;

}

