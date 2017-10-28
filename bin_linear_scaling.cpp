#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
void bin_linear_scale(cv::Mat& input_img, int width, int height);

int main()
{
    cv::Mat img = cv::imread("41.png", 0);
    cv::imshow("src", img);
    bin_linear_scale(img, 500, 200);

    return 0;
}


//f(i+u,j+v) = (1-u)(1-v)f(i,j) + (1-u)vf(i,j+1) + u(1-v)f(i+1,j) + uvf(i+1,j+1)  
uchar get_scale_value(cv::Mat& input_img, float raw_i, float raw_j)
{
    int i = raw_i;
    int j = raw_j;
    float u = raw_i - i;
    float v = raw_j - j;

    if (i + 1 >= input_img.rows || j + 1 >= input_img.cols)
    {
        uchar* p = input_img.ptr<uchar>(i);
        return p[j];
    }

    //注意处理边界问题，容易越界
    uchar* p = input_img.ptr<uchar>(i);
    uchar x1 = p[j];  //f(i,j)
    uchar x2 = p[j + 1];  //f(i,j+1)
    p = input_img.ptr<uchar>(i+1);
    uchar x3 = p[j];   //(i+1,j)
    uchar x4 = p[j + 1];  //f(i+1,j+1) 
     
   // printf("%d %d\n", i, j);
    return ((1-u)*(1-v)*x1+(1-u)*v*x2+u*(1-v)*x3+u*v*x4);
}

         
void bin_linear_scale(cv::Mat& input_img, int width, int height)
{
    cv::Mat output_img(height, width, CV_8UC1);
    output_img.setTo(0);
    float h_scale_rate = (float)input_img.rows / height;
    float w_scale_rate = (float)input_img.cols / width;
    for (int i = 0; i < height; i++)
    {
        uchar* p = output_img.ptr<uchar>(i);
        for (int j = 0; j < width; j++)
        {
            float i_scale = h_scale_rate * i;
            float j_scale = w_scale_rate * j;
            //cout << "i_scale: " << i_scale <<" j_scale: "<< j_scale << endl;

            p[j] = get_scale_value(input_img, i_scale, j_scale);
        }
    }

    cv::imshow("scale", output_img);
    cv::imwrite("result_500_200.png", output_img);
    cv::waitKey();
}