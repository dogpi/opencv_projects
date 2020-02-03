#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat src,gray_src,binary_src;
    src = imread("/home/dogpi/Pictures/lines.png");
    if(src.empty()){
        cout<<"coould not load image"<<endl;
        return -1;
    }
    imshow("input image",src);

    cvtColor(src,gray_src,COLOR_BGRA2GRAY);
    imshow("gray image",gray_src);

    adaptiveThreshold(~gray_src,binary_src,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,9,-2);
    imshow("binary image",binary_src);

    Mat kernel_h = getStructuringElement(MORPH_RECT,Size(src.cols/16,1),Point(-1,-1));
    Mat kernel_v = getStructuringElement(MORPH_RECT,Size(1,src.rows/16),Point(-1,-1));

    Mat lines_h,lines_v;

    morphologyEx(binary_src,lines_h,MORPH_OPEN,kernel_h,Point(-1,-1));
    imshow("open op lines h",lines_h);
    morphologyEx(binary_src,lines_v,MORPH_OPEN,kernel_v,Point(-1,-1));
    imshow("open op lines v",lines_v);

    waitKey(0);
    return 0;
}
