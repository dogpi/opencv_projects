#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat src,gray_src,dst;
int threshold_value = 127;
int threshold_max = 255;
int threshold_method_value = 2;
int threshold_method_max = 4;
void call_back_threshold_value(int,void*);
int main()
{
    src = imread("/home/dogpi/Downloads/opencv-4.2.0/samples/data/chicky_512.png");
    if(src.empty()){
        return -1;
    }
    imshow("src",src);
    namedWindow("output",WINDOW_AUTOSIZE);

    cvtColor(src,gray_src,COLOR_BGR2GRAY);
    imshow("gray image",gray_src);
    createTrackbar("threshold","output",&threshold_value,threshold_max,call_back_threshold_value);
    createTrackbar("method","output",&threshold_method_value,threshold_method_max,call_back_threshold_value);
    call_back_threshold_value(0,0);

    waitKey(0);
    return 0;
}

void call_back_threshold_value(int ,void*){
    threshold(gray_src,dst,threshold_value,threshold_max,threshold_method_value);
    imshow("output",dst);
}
