#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat src = imread("/home/dogpi/Downloads/opencv-4.2.0/samples/data/lena.jpg");
    if(src.empty()){
        cout<<"could not load image"<<endl;
        return -1;
    }
    namedWindow("source image",WINDOW_AUTOSIZE);
    namedWindow("medianBlur",WINDOW_AUTOSIZE);
    namedWindow("bilateralFilter",WINDOW_AUTOSIZE);

    imshow("source image",src);

    Mat median_image;
    medianBlur(src,median_image,3);
    imshow("medianBlur",median_image);

    // 双边滤波  保留轮廓特征
    Mat bilateral_image;
    bilateralFilter(src,bilateral_image,15,100,5);
    imshow("bilateralFilter",bilateral_image);

    Mat gauss_image;
    GaussianBlur(src,gauss_image,Size(3,3),3,3);
    imshow("gauss",gauss_image);

    Mat kernel_image;
    Mat kernel=(Mat_<int>(3,3)<<0,-1,0,-1,5,-1,0,-1,0);
    filter2D(src,kernel_image,-1,kernel,Point(-1,-1));
    imshow("kernel",kernel_image);

    waitKey(0);
    return 0;
}
