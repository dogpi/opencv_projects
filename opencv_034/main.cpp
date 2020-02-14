#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/*
使用自定义角点检测器实现Harris和Shi-Tomasi角点检测器
*/

Mat src,gray_src;
const char* HARRIS_OUTPUT = "harris output window";
const char* SHI_TOMASI_OUTPUT = "tomasi output window";
Mat eigenValseignVecs;
Mat Mc;
Mat tomasi_mat;
double harrisMinValue;
double harrisMaxValue;

double tomasiMaxValue;
double tomasiMinValue;

int myShiTomasi_qualityLevel = 50;
int myHarris_qualityLevel = 50;
int max_qualityLevel = 100;

void harris_call_back(int,void*);
void shi_tomasi_call_back(int,void*);
int main()
{
    src = imread("/home/dogpi/Pictures/samples/home.jpg");
    if(src.empty()){
        cout<<"Failed to load picture"<<endl;
        return -1;
    }
    imshow("Source Picture",src);

    // 转为灰度图像
    cvtColor(src,gray_src,COLOR_BGR2GRAY);

    int blockSize = 3;
    int apertureSize = 3;

    // 计算特征值特征向量
    cornerEigenValsAndVecs(gray_src,eigenValseignVecs,blockSize,apertureSize);

    // 计算响应值
    // Harris
    Mc = Mat(gray_src.size(),CV_32F);
    for(int row=0;row<gray_src.rows;row++){
        for(int col = 0;col<gray_src.cols;col++){
            float lambds_1 = eigenValseignVecs.at<Vec6f>(row,col)[0];
            float lambds_2 = eigenValseignVecs.at<Vec6f>(row,col)[1];
            Mc.at<float>(row,col) = lambds_1*lambds_2-0.04*pow((lambds_1=lambds_2),2);
        }
    }
    minMaxLoc(Mc,&harrisMinValue,&harrisMaxValue);


    // Shi-Tomasi
    cornerMinEigenVal(gray_src,tomasi_mat,blockSize,apertureSize);
    minMaxLoc(tomasi_mat,&tomasiMinValue,&tomasiMaxValue);

    namedWindow(HARRIS_OUTPUT,WINDOW_AUTOSIZE);
    createTrackbar("corners value:",HARRIS_OUTPUT,&myHarris_qualityLevel,max_qualityLevel,harris_call_back);
    harris_call_back(0,0);

    namedWindow(SHI_TOMASI_OUTPUT,WINDOW_AUTOSIZE);
    createTrackbar("corners value:",SHI_TOMASI_OUTPUT,&myShiTomasi_qualityLevel,max_qualityLevel,shi_tomasi_call_back);
    shi_tomasi_call_back(0,0);

    waitKey(0);
    return 0;
}

// Harris 角点检测
void harris_call_back(int,void*){
    Mat Harris_mat = src.clone();
    myHarris_qualityLevel = max(myHarris_qualityLevel,1);

    for(int row=0;row<gray_src.rows;row++){
        for(int col=0;col<gray_src.cols;col++){
            if(Mc.at<float>(row,col) >harrisMinValue+( harrisMaxValue - harrisMinValue )*myHarris_qualityLevel/max_qualityLevel){
                circle(Harris_mat,Point(col,row),4,Scalar(0,0,255),2,LINE_8);
            }
        }
    }
    imshow(HARRIS_OUTPUT,Harris_mat);
}
// Shi-Tomasi 角点检测
void shi_tomasi_call_back(int,void*){
    Mat Tomasi_mat = src.clone();
    myShiTomasi_qualityLevel = max(myShiTomasi_qualityLevel,1);

    for(int row=0;row<gray_src.rows;row++){
        for(int col=0;col<gray_src.cols;col++){
            if(tomasi_mat.at<float>(row,col) >tomasiMinValue+( tomasiMaxValue - tomasiMinValue )*myShiTomasi_qualityLevel/max_qualityLevel){
                circle(Tomasi_mat,Point(col,row),4,Scalar(0,0,255),2,LINE_8);
            }
        }
    }
    imshow(SHI_TOMASI_OUTPUT,Tomasi_mat);
}
