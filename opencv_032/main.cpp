#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat src,gray_src;
int threshold_value = 150;
int threshold_max = 255;
void call_back(int,void*);
int main()
{
    //Harris corner
    src = imread("/home/dogpi/Pictures/samples/building.jpg");
    if(src.empty()){
        cout<<"Failed to load picture"<<endl;
        return -1;
    }
    imshow("Source Picture",src);

    cvtColor(src,gray_src,COLOR_BGR2GRAY);

    namedWindow("result",WINDOW_AUTOSIZE);
    createTrackbar("threshold value:","result",&threshold_value,threshold_max,call_back);
    call_back(0,0);

    waitKey(0);
    return 0;
}
void call_back(int,void*){
    Mat dst;
    dst = Mat::zeros(src.size(),CV_32F);
    cornerHarris(gray_src,dst,2,3,0.04);
    Mat dst_norm;
    normalize(dst,dst_norm,0,255,NORM_MINMAX,CV_32F);
    Mat dst_abs;
    convertScaleAbs(dst_norm,dst_abs);
    Mat result = src.clone();
    for(int row=0;row<dst_norm.rows;row++){
        for(int col=0;col<dst_norm.cols;col++){
            if((int)dst_norm.at<float>(row,col)>threshold_value){
                circle(result,Point(col,row),2,Scalar(0,0,255),2,LINE_8);
            }
        }
    }
    imshow("result",result);
}
