#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>


using namespace std;
void quantizie(cv::Mat& input_img, int level);

#define LEVEL 128

int main()
{
    cv::Mat img = cv::imread("41.png", 0);
    cv::imshow("src", img);
    quantizie(img, LEVEL);

    return 0;
}

uchar get_value(int level, uchar v)
{
    int block_num = level - 1;
    int block_size = 256 / block_num;  
    for (int i = 1; i <= block_num; i++)
    {
        if (v > block_size * i)
        {
            continue;
        }

        int mid_value = block_size * i / 2;
        int left = block_size * (i - 1);
        int right = block_size * i - 1;
        if (v < mid_value)
        {
            return left;
        }
        else
        {
            return right;
        }
    }

    return v;
}

void quantizie(cv::Mat& input_img, int level)
{
    cv::Mat output_img = input_img.clone();
    output_img.setTo(0);
  

    for (int i = 0; i < output_img.rows; i++)
    {
        uchar* p1 = input_img.ptr<uchar>(i);
        uchar* p2 = output_img.ptr<uchar>(i);
        for (int j = 0; j < output_img.cols; j++)
        {
            p2[j] = get_value(level, p1[j]);
            //p2[j] = (p1[j] / range) * range;
        }
    }

    cv::imshow("quantize", output_img);
    cv::imwrite("quantize128.png", output_img);
    cv::waitKey();
}