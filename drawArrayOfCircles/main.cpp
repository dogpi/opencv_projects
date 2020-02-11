#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int mindist_value = 38;
int mindist_max = 100;
int param1_value = 11;
int param1_max = 100;
int param2_value =5;
int param2_max = 100;
int minradius_value = 6;
int minradius_max = 50;
int maxradius_value = 10;
int maxradius_max = 50;
int threshold_value = 97;
int threshold_max=255;
Mat src(Size(850,850),CV_8UC3,Scalar(255,255,255));
void call_back(int,void*);
int main(){
    double the = 75;
    int x = 40;
    int y = 40;
    double cos_value = cos(CV_PI*(the/180));
    double sin_value = sin(CV_PI*(the/180));

    // 画一个16*16的矩阵
    for(int i=1;i<=16;i++){
        for(int j=1;j<=16;j++){
            circle(src,Point(x+(j-1)*sin_value*x-(i-1)*cos_value*y+200,y+(j-1)*cos_value*y+(i-1)*sin_value*y),4,Scalar(0,255,0),5,LINE_AA);
        }
    }
    // 添加一个不规则点
    circle(src,Point(420,400),4,Scalar(0,255,0),5,LINE_AA);

    namedWindow("result",WINDOW_AUTOSIZE);
    // 创建TrackBar方便调试
    createTrackbar("mindist:","result",&mindist_value,mindist_max,call_back);
    createTrackbar("param1:","result",&param1_value,param1_max,call_back);
    createTrackbar("param2:","result",&param2_value,param2_max,call_back);
    createTrackbar("minradius:","result",&minradius_value,minradius_max,call_back);
    createTrackbar("maxradius:","result",&maxradius_value,maxradius_max,call_back);
    createTrackbar("threshold:","result",&threshold_value,threshold_max,call_back);
    call_back(0,0);

    waitKey(0);
    return 0;
}
void call_back(int,void*){
    // 原图转为灰度图像
    Mat  gray_src;
    medianBlur(src,gray_src,3);
    cvtColor(gray_src,gray_src,COLOR_BGR2GRAY);

    // 对灰度图像二值化  白色作为前景色
    Mat binary_mat;
    threshold(~gray_src,binary_mat,threshold_value,threshold_max,THRESH_BINARY);

    //霍夫圆检测 检测符合规则的圆
    vector<Vec3f> circles;
    HoughCircles(binary_mat,circles,HOUGH_GRADIENT,1,mindist_value,param1_value,param2_value,minradius_value,maxradius_value);

    // 在原图上绘制检测到的圆
    Mat result;
    src.copyTo(result);
    for(int i=0;i<circles.size();i++){
        Vec3f cc = circles[i];
        circle(result,Point(cc[0],cc[1]),cc[2],Scalar(0,0,255),2,LINE_AA);
    }
    imshow("result",result);

    // 创建一个mask，在copyTo时值不为0的像素会保存。
    Mat tmp(Size(850,850),CV_8UC3,Scalar(0,0,0));
    for(int i=0;i<circles.size();i++){
        Vec3f cc = circles[i];
        circle(tmp,Point(cc[0],cc[1]),cc[2]-3,Scalar(255,255,255),LINE_AA,-1);
    }

    // 把mask转为二值图像。背景为黑色，前景为白色圆。
    cvtColor(tmp,gray_src,COLOR_BGR2GRAY);
    threshold(~gray_src,binary_mat,97,255,THRESH_BINARY);


    Mat dst1,dst2;
    // 显示符合规则的圆
    src.copyTo(dst1,~binary_mat);
    for(int row =0;row<dst1.rows;row++){
        for(int col=0;col<dst1.cols;col++){
            if(dst1.at<Vec3b>(row,col)[0]==0&&dst1.at<Vec3b>(row,col)[1]==0&&dst1.at<Vec3b>(row,col)[2]==0){
                dst1.at<Vec3b>(row,col)[0]=255;
                dst1.at<Vec3b>(row,col)[1]=255;
                dst1.at<Vec3b>(row,col)[2]=255;
            }
        }
    }
    imshow("ok",dst1);

    // 不符合规则的点
    src.copyTo(dst2,binary_mat);
    for(int row =0;row<dst2.rows;row++){
        for(int col=0;col<dst2.cols;col++){
            if(dst2.at<Vec3b>(row,col)[0]==0&&dst2.at<Vec3b>(row,col)[1]==0&&dst2.at<Vec3b>(row,col)[2]==0){
                dst2.at<Vec3b>(row,col)[0]=255;
                dst2.at<Vec3b>(row,col)[1]=255;
                dst2.at<Vec3b>(row,col)[2]=255;
            }
        }
    }
    imshow("not ok",dst2);
}

