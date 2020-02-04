#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat src,dst;
    src = imread("/home/dogpi/Pictures/samples/lena.jpg");
    if(src.empty()){
        return -1;
    }
    imshow("lena",src);

    Mat gray_src;
    cvtColor(src,gray_src,COLOR_BGR2GRAY);

    equalizeHist(gray_src,dst);
    imshow("dst",dst);

    waitKey(0);
    return 0;
}
