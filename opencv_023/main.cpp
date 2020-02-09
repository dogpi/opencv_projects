#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/*
    反射投影：
        原理：
            函数cv :: calcBackProject计算直方图的反向投影。
            也就是说，类似于calcHist，该函数在每个位置（x，y）收集输入图像中所选通道的值，并找到相应的直方图bin。
            但是该函数不是增加它，而是读取bin值，按比例缩放它，并将其存储在backProject（x，y）中。
            在统计方面，该函数根据直方图表示的经验概率分布计算每个元素值的概率。
            例如，查看如何在场景中查找和跟踪明亮的对象：跟踪之前，将对象显示给相机，使其几乎覆盖整个画面。
            计算色调直方图。直方图可能具有很强的最大值，对应于对象中的主要颜色。
            跟踪时，请使用该预先计算的直方图计算每个输入视频帧的色相平面的反投影。
            限制背投阈值以抑制较弱的色彩。抑制颜色饱和度不足且像素太暗或太亮的像素也可能是有意义的。
            在结果图片中找到连接的组件，然后选择最大的组件。
        步骤：
            1、是图像转换到HSV色域空间
            2、取出HSV空间的H通道进行calcHist,对直方图结果进行normalize
            3、对归一化后的结果进型calcBackProject，得到反向投影图像。

*/


Mat src,hue,hsv;
int bins = 30;
int max_bins = 360;
void call_back(int,void*);
int main()
{
    src = imread("/home/dogpi/Pictures/samples/hand2.jpg");
    if(src.empty()){
        return -1;
    }
    imshow("src",src);
    cvtColor(src,hsv,COLOR_BGR2HSV);
    imshow("hsv",hsv);
    hue.create(hsv.size(),hsv.depth());

    int channels[] = {0,0};
    mixChannels(&hsv,1,&hue,1,channels,1);

    namedWindow("hue",WINDOW_NORMAL);
    createTrackbar("hue bins:","hue",&bins,max_bins,call_back);
    call_back(0,0);
    waitKey(0);
    return 0;
}
void call_back(int ,void*){
    MatND hist;
    int histSize = MAX(bins,2);
    float hue_range[] = {0,360};
    const float* ranges[] = {hue_range};
    calcHist(&hue,1,0,Mat(),hist,1,&histSize,ranges,true,false);
    normalize(hist,hist,0,255,NORM_MINMAX,-1,Mat());
    MatND backproject;
    //calcBackProject(&hueImage, 1, 0, hist, backProjection, &ranges, 1, true);
    calcBackProject(&hue,1,0,hist,backproject,ranges,1,true);
    imshow("backproject",backproject);
    int w = 400;
    int h = 400;
    int bin_w = cvRound((double)w/histSize);
    Mat histimage=Mat::zeros(w,h,CV_8UC3);
    for(int i = 1;i<histSize;i++){
        rectangle(histimage,Point((i-1)*bin_w,h),Point(i*bin_w,h-hist.at<float>(i)),Scalar(255,0,0),-1);
    }
    imshow("hist",histimage);
}
