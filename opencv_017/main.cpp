#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat lena = imread("/home/dogpi/Pictures/samples/lena.jpg");
Mat lena_gray,dst;
int threshold_value = 100;
int threshold_max = 255;
void call_back(int,void*);
int main()
{
    if(lena.empty()){
        return -1;
    }
    imshow("lena",lena);
    namedWindow("canny",WINDOW_AUTOSIZE);
    cvtColor(lena,lena_gray,COLOR_BGR2GRAY);
    createTrackbar("threshold","canny",&threshold_value,threshold_max,call_back);
    call_back(0,0);

    waitKey(0);
    return 0;
}
void call_back(int,void*){
    Mat output;
    blur(lena_gray,lena_gray,Size(3,3),Point(-1,-1));
    Canny(lena_gray,output,threshold_value,threshold_value*2,3);
    dst.create(lena.size(),lena.type());

    Mat kernel = getStructuringElement(MORPH_RECT,Size(3,3),Point(-1,-1));
    dilate(output,output,kernel,Point(-1,-1));

    // output为二值图像，轮廓为白色，output作为掩码，lena拷贝output上点为1的像素到dst
    lena.copyTo(dst,output);
    imshow("canny",dst);
}
