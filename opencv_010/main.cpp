#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat src,dst;
    src = imread("/home/dogpi/Pictures/i.png");
    if(src.empty()){
        cout<<"could not load image"<<endl;
        return -1;
    }
    imshow("src",src);

    Mat dilate_image,erode_image;
    Mat cat = imread("/home/dogpi/Pictures/cat.jpg");
    Mat kernel = getStructuringElement(MORPH_RECT,Size(3,3),Point(-1,-1));
    dilate(cat,dilate_image,kernel,Point(-1,-1));
    erode(cat,erode_image,kernel,Point(-1,-1));
    imshow("dilate-erode",dilate_image-erode_image);

    Mat kernel1 = getStructuringElement(MORPH_RECT,Size(11,11),Point(-1,-1));
    morphologyEx(src,dst,MORPH_OPEN,kernel1,Point(-1,-1));
    imshow("open",dst);

    Mat kernel2 = getStructuringElement(MORPH_RECT,Size(7,7),Point(-1,-1));
    morphologyEx(src,dst,MORPH_CLOSE,kernel2,Point(-1,-1));
    imshow("close",dst);



    waitKey(0);
    return 0;
}
