#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/*直方图比较*/

int main()
{
    // 加载图像
    Mat src1 = imread("/home/dogpi/Pictures/samples/lena.jpg");
    Mat src2 = imread("/home/dogpi/Pictures/samples/lena_tmpl.jpg");
    Mat src3 = imread("/home/dogpi/Pictures/samples/cat.jpg");
    if(src1.empty()||src2.empty()||src3.empty()){
        return -1;
    }
//    imshow("src1",src1);
//    imshow("src2",src2);
//    imshow("src3",src3);

    // 色域转换BGR->HSV
    Mat hsv_src1,hsv_src2,hsv_src3;
    cvtColor(src1,hsv_src1,COLOR_BGR2HSV);
    cvtColor(src2,hsv_src2,COLOR_BGR2HSV);
    cvtColor(src3,hsv_src3,COLOR_BGR2HSV);
    imshow("hsv_src1",hsv_src1);
    imshow("hsv_src2",hsv_src2);
    imshow("hsv_src3",hsv_src3);

    // 直方图计算
    int channels[] = {0,1}; // 通道
    Mat hist_src1,hist_src2,hist_src3;// 存放直方图数据
    int h_bins = 50;
    int s_bins = 60;
    int histSize[] = {h_bins,s_bins};
    float h_ranges[]={0,180};
    float s_ranges[]={0,255};
    const float *ranges[] = {h_ranges,s_ranges};
    calcHist(&hsv_src1,1,channels,Mat(),hist_src1,2,histSize,ranges,true,false);
    calcHist(&hsv_src2,1,channels,Mat(),hist_src2,2,histSize,ranges,true,false);
    calcHist(&hsv_src3,1,channels,Mat(),hist_src3,2,histSize,ranges,true,false);

    // 归一化
    normalize(hist_src1,hist_src1,0,1,NORM_MINMAX,-1,Mat());
    normalize(hist_src2,hist_src2,0,1,NORM_MINMAX,-1,Mat());
    normalize(hist_src3,hist_src3,0,1,NORM_MINMAX,-1,Mat());

    // 直方图比较
    for(int i=0;i<4;i++){
        int compare_method = i;
        // 都是与自己比较，所以结果是两幅图片相同。
        double base_src1 = compareHist(hist_src1,hist_src1,compare_method);
        double base_src2 = compareHist(hist_src2,hist_src2,compare_method);
        double base_src3 = compareHist(hist_src3,hist_src3,compare_method);
        cout<<"Method:"<<compare_method<<endl;
        cout<<"base_src1:"<<base_src1<<"\tbase_src2:"<<base_src2<<"\tbase_src3:"<<base_src3<<endl;
        cout<<"--------------------------------------------------------------------------------"<<endl;
    }

    waitKey(0);
    return 0;
}
