#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
void scale(cv::Mat& input_img, int width, int height);

int main()
{
    cv::Mat img = cv::imread("41.png",0);
    cv::imshow("src", img);
    scale(img, 450, 300);

    return 0;
}

uchar get_scale_value(cv::Mat& input_img, int i, int j)
{
    uchar* p = input_img.ptr<uchar>(i);
    return p[j];
    
}

void scale(cv::Mat& input_img,int width,int height)
{
    cv::Mat output_img(height, width, CV_8UC1);
    output_img.setTo(0);
    float h_scale_rate = (float)input_img.rows/ height;
    float w_scale_rate = (float)input_img.cols / width;
    for (int i = 0; i < height; i++)
    {
        uchar* p = output_img.ptr<uchar>(i);
        for (int j = 0; j < width; j++)
        {
            int i_scale = h_scale_rate * i;
            int j_scale = w_scale_rate * j;
            //cout << "i_scale: " << i_scale <<" j_scale: "<< j_scale << endl;
 
            p[j] = get_scale_value(input_img,i_scale, j_scale);
        }
    }

    cv::imshow("scale", output_img);
    cv::imwrite("result.png", output_img);
    cv::waitKey();

}