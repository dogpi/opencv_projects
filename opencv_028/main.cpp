#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat gray_src;
int threshodl_value = 100;
int threshold_max = 255;
RNG rng(1234);

void call_back(int,void*);

int main()
{
    // 加载图像
    Mat src = imread("/home/dogpi/Pictures/samples/cattorn001.jpg");
    if(src.empty()){
        cout<<"Failed to load picture"<<endl;
        return -1;
    }
    imshow("src",src);

    // 转为灰度图像
    cvtColor(src,gray_src,COLOR_BGR2GRAY);
//    imshow("gray src",gray_src);


    // 回调函数
    namedWindow("result",WINDOW_AUTOSIZE);
    createTrackbar("threshold value:","result",&threshodl_value,threshold_max,call_back);
    call_back(0,0);

    waitKey(0);
    return 0;
}

void call_back(int,void*){
    // 提取图像边缘
    Mat canny_mat;
    Canny(gray_src,canny_mat,threshodl_value,threshodl_value*2,3);

    // 发现轮廓
    vector<vector<Point>> contours;
    findContours(canny_mat,contours,RETR_TREE,CHAIN_APPROX_SIMPLE);

    // 计算monents
    vector<Moments> mu(contours.size());
    for(size_t i = 0;i<contours.size();i++){
        mu[i] = moments(contours[i]);
    }

    // 计算质量中心
    vector<Point2f> mc(contours.size());
    for(size_t i=0;i<contours.size();i++){
        mc[i] = Point2f(mu[i].m10/(mu[i].m00+1e-5),mu[i].m01/(mu[i].m00+1e-5));
        cout << "mc[" << i << "]=" << mc[i] << endl;
    }

    // 绘制轮廓
    Mat drawImage = Mat::zeros(canny_mat.size(),CV_8UC3);
    for(size_t i=0;i<contours.size();i++){
        Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
        drawContours(drawImage,contours,(int)i,color);
        circle(drawImage,mc[i],4,color,-1);
    }
    imshow("result",drawImage);

    cout << "\t Info: Area and Contour Length \n";
    for( size_t i = 0; i < contours.size(); i++ )
    {
        // 计算contour的面积和弧长
        cout << " * Contour[" << i << "] - Area (M_00) = " << std::fixed << std::setprecision(2) << mu[i].m00
             << " - Area OpenCV: " << contourArea(contours[i]) << " - Length: " << arcLength( contours[i], true ) << endl;
    }
}
