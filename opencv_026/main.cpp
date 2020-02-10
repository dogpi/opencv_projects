#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat src_gray;
int threshold_value = 100;
int threshold_max = 255;
RNG rng(1234);

void call_back(int,void*);
int main()
{

    // 读取原图像
    Mat src = imread("/home/dogpi/Pictures/samples/backgroundblack.png");
    if(src.empty()){
        cout<<"Failed to load picture"<<endl;
    }
    imshow("src",src);

    // 转为灰度图像并做处理
    cvtColor(src,src_gray,COLOR_BGR2GRAY);
    blur(src_gray,src_gray,Size(3,3));

    // 创建窗口
    namedWindow("result",WINDOW_AUTOSIZE);

    // 回调函数
    createTrackbar("threshold value:","result",&threshold_value,threshold_max,call_back);
    call_back(0,0);

    waitKey(0);
    return 0;
}
// 图像处理函数
void call_back(int,void*){
    // 边缘提取
    Mat canny_mat;
    Canny(src_gray,canny_mat,threshold_value,threshold_value*2);

    // 发现轮廓
    vector<vector<Point>> contours; // 存储轮廓
    findContours(canny_mat,contours,RETR_TREE,CHAIN_APPROX_SIMPLE);

    // 发现凸包
    vector<vector<Point>> hull(contours.size());
    for(size_t i=0;i<contours.size();i++){
        convexHull(contours[i],hull[i]);
    }

    Mat drawImage = Mat::zeros(canny_mat.size(),CV_8UC3);
    for(size_t i = 0;i<contours.size();i++){
        Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
//        drawContours(drawImage,contours,(int)i,color);
        drawContours(drawImage,hull,(int)i,color);
    }

    imshow("result",drawImage);
}
