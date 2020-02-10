#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int threshold_value = 100;
int threshold_max = 255;
Mat gray_src;
RNG rng(1234);

void call_back(int,void*);
int main()
{
    // 加载图像
    Mat src = imread("/home/dogpi/Pictures/samples/hotball.jpg");
    if(src.empty()){
        cout<<"Failed to load picture"<<endl;
        return -1;
    }
    imshow("src",src);

    // 转为灰度图像
    cvtColor(src,gray_src,COLOR_BGR2GRAY);
    blur(gray_src,gray_src,Size(3,3),Point(-1,-1));

    // 创建窗口
    namedWindow("result",WINDOW_AUTOSIZE);

    // 调用回调函数
    createTrackbar("threshold value:","result",&threshold_value,threshold_max,call_back);
    call_back(0,0);

    waitKey(0);
    return 0;
}
void call_back(int,void*){
    // 边缘提取
    Mat canny_mat;
//    Canny(gray_src,canny_mat,threshold_value,threshold_value*2,3);
    threshold(gray_src,canny_mat,threshold_value,threshold_max,THRESH_BINARY);
    imshow("threshold",canny_mat);
    // 发现轮廓
    vector<vector<Point>> contours;
    findContours(canny_mat,contours,RETR_TREE,CHAIN_APPROX_SIMPLE);

    // 外接多边形
    // 矩形
    // 圆
    vector<vector<Point>> contours_poly(contours.size());
    vector<Rect> boundRect(contours.size());
    vector<Point2f> ccs(contours.size());    // 圆心
    vector<float> radius(contours.size());   // 圆的半径
    for(size_t i = 0;i<contours.size();i++){
        approxPolyDP(contours[i],contours_poly[i],3,true);
        boundRect[i] = boundingRect(contours_poly[i]);
        minEnclosingCircle(contours_poly[i],ccs[i],radius[i]);
    }
    Mat drawImage = Mat::zeros(canny_mat.size(),CV_8UC3);

    // 绘制多边形，矩形，圆
    for(size_t i=0;i<contours.size();i++){
        Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
        drawContours(drawImage,contours_poly,(int)i,color);
        rectangle(drawImage,boundRect[i].tl(),boundRect[i].br(),color,2);
        circle(drawImage,ccs[i],(int)radius[i],color,2);
    }
    imshow("result",drawImage);
}
