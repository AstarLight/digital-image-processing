#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>


using namespace std;
using namespace cv;



//卷积运算
uchar cov2d(Mat& roi, Mat& filter)
{
    //cout << roi << endl;
    //cout << filter << endl;
    int sum = 0;
    for (int i = 0; i < filter.rows; i++)
    { 
        uchar* pRoi = roi.ptr<uchar>(i);
        uchar* pFilter = filter.ptr<uchar>(i);
        for (int j = 0; j < filter.cols; j++)
        {
           sum += pRoi[j] * pFilter[j];
           //cout << sum << endl;
        }
    }
    //cout << sum << endl;
    sum /=9;

    if (sum > 255) sum = 255;
    if (sum < 0) sum = 0;
#if 0
    if (sum >= 255)
    {
        sum = 255;
    }
    else
    {
        sum = 0;
    }
#endif
    return sum;
}

void kernel_turnover(Mat& kernel)
{

}

Mat filter2d(Mat& input,Mat& kernel)
{
    //滤波器旋转180度
    kernel_turnover(kernel);


    Mat padding_input(input.rows+ kernel.cols - 1, input.cols + kernel.rows - 1, CV_8UC1);
    padding_input.setTo(0);

    //padding：边界补零
    int i_offsets = kernel.rows%2;
    int j_offset = kernel.cols % 2;
    for (int i = 0; i < input.rows; i++)
    {
        uchar* p1 = input.ptr<uchar>(i);
        uchar* p2 = padding_input.ptr<uchar>(i+ i_offsets);
        for (int j =0; j < input.cols; j++)
        {
            p2[j+ j_offset] = p1[j];
        }
    }
   // cout << "rows:" << padding_input.rows << " " << "cols:" << padding_input.cols << endl;
    Mat final_output(input.rows, input.cols,CV_8UC1);
    //convolution
    for (int i = 0; i < final_output.rows; i++)
    {
        uchar* p1 = final_output.ptr<uchar>(i);
        //uchar* p2 = padding_input.ptr<uchar>(i);
        for (int j = 0; j < final_output.cols; j++)
        {
            Rect r(j, i, kernel.cols, kernel.rows);
            //cout << r << endl;
            Mat roi = padding_input(r);
            uchar vaule = cov2d(roi, kernel);
            p1[j] = vaule;
        }
    }

    return final_output;
}


void high_boost_filter(Mat& img,Mat& mask)
{
    int k = 1;
    for (int i = 0; i < img.rows; i++)
    {
        uchar* p1 = img.ptr<uchar>(i);
        uchar* p2 = mask.ptr<uchar>(i);
        for (int j = 0; j < img.cols; j++)
        {
            //printf("%d\n", p2[j]);
            int value = p1[j] + p2[j] * k;
            value = value > 255 ? 255 : value;                
            p1[j] = value;
        }
    }
}

int main()
{
    Mat img = imread("C:\\Users\\李浚时\\Desktop\\DIP作业\\hw2_input\\41.png", 0);
    imshow("src", img);
    //short k[3][3] = { {0,1,0},{1,-4,1},{0,1,0} };

    //Mat kernel(3, 3, CV_8S, (short*)k);
    //kernel.setTo(1);
    Mat kernel(3, 3, CV_8UC1);
    kernel.at<uchar> (0, 0 ) = 1;
    kernel.at<uchar>( 0, 1 ) = 1;
    kernel.at<uchar>(0, 2 ) = 1;
    kernel.at<uchar>(1, 0 ) = 1;
    kernel.at<uchar>(1, 1 ) = 1;
    kernel.at<uchar>(1, 2 ) = 1;
    kernel.at<uchar>(2, 0 ) = 1;
    kernel.at<uchar>(2, 1 ) = 1;
    kernel.at<uchar>(2, 2 ) = 1;

    cout << kernel << endl;
    Mat out = filter2d(img, kernel);
    imshow("output", out);

    Mat mask = img - out;
    imshow("mask", mask);
    //waitKey();
    high_boost_filter(img, mask);
    imshow("boost", img);
    waitKey();

    return 0;
}