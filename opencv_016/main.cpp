#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat src,dst;
    src = imread("/home/dogpi/Pictures/samples/baboon.jpg");
    if(src.empty()){
        return -1;
    }
    imshow("input",src);

    Mat s1,s2;
    Sobel(src,dst,-1,1,0,3);
    cvtColor(dst,s1,COLOR_BGR2GRAY);
    imshow("Sobel x",dst);

    Sobel(src,dst,-1,0,1,3);
    cvtColor(dst,s2,COLOR_BGR2GRAY);
    imshow("Sobel y",dst);
    addWeighted(s1,0.5,s2,0.5,0,dst);
//    Sobel(src,dst,-1,1,1,3);
    imshow("Sobel xy",dst);

    // Sobel的改进函数
    Mat c1,c2;
    Scharr(src,dst,-1,1,0);
    cvtColor(dst,s1,COLOR_BGR2GRAY);
    imshow("Scharr x",dst);
    Scharr(src,dst,-1,0,1);
    cvtColor(dst,s2,COLOR_BGR2GRAY);
    imshow("Scharr y",dst);
    addWeighted(s1,0.5,s2,0.5,0,dst);
    imshow("Scharr xy",dst);

    //拉普拉斯 二阶导数取图像边界
    Mat src2 = imread("/home/dogpi/Pictures/samples/lena.jpg");
    imshow("lena",src2);
    Mat dst_gaus;
    // 高斯滤波去小噪点
    GaussianBlur(src2,dst_gaus,Size(3,3),0,0);
    Mat gray_lena;
    // 转换为灰度图像
    cvtColor(dst_gaus,gray_lena,COLOR_BGR2GRAY);
//    imshow("gray lena",gray_lena);
    Mat result;
    Laplacian(gray_lena,result,CV_16S,3);
    // 转换成8位
    convertScaleAbs(result,result);

    threshold(result,result,0,255,THRESH_OTSU);
    imshow("laplance result",result);
    waitKey(0);
    return 0;
}
