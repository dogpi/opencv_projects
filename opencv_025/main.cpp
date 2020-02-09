#include <iostream>
#include <opencv2/opencv.hpp>
/*
    轮廓发现
        1、canny提轮廓
        2、findContours提取轮廓
        3、drawContours绘制轮廓
*/
using namespace cv;
using namespace std;


Mat src,dst;
int threshold_value = 20;
int threshold_max = 400;
RNG rng(1234);
void call_back(int,void*);
int main()
{
    src = imread("/home/dogpi/Pictures/samples/stuff.jpg");
    if(src.empty()){
        return -1;
    }
    imshow("src",src);
    namedWindow("result",WINDOW_AUTOSIZE);
    namedWindow("binary",WINDOW_AUTOSIZE);
    createTrackbar("threshold value:","binary",&threshold_value,threshold_max,call_back);
    call_back(0,0);
    waitKey(0);
    return 0;
}
void call_back(int,void*){
    vector<vector<Point>> contours;//存放轮廓
    vector<Vec4i> hierarchy;
    GaussianBlur(src,src,Size(3,3),11,11);
    Canny(src,dst,threshold_value,threshold_value*2,5,false);
    findContours(dst,contours,hierarchy,RETR_TREE,CHAIN_APPROX_TC89_L1,Point(0,0));
    Mat drawImage = Mat::zeros(dst.size(),CV_8UC3);
    for(int i = 0;i<contours.size();i++){
        Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
        drawContours(drawImage,contours,i,color,2,LINE_8,hierarchy,0,Point(0,0));
    }
    imshow("binary",dst);
    imshow("result",drawImage);
}
