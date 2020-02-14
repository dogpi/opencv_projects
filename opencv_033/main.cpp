#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


Mat src,gray_src;
int corner_value = 100;
int corner_max = 255;
RNG rng(time(NULL));
const char* OUTPUT_WINDOW_NAME = "output window";

void call_back(int,void*);
int main()
{

    src = imread("/home/dogpi/Pictures/samples/home.jpg");
    if(src.empty()){
        cout<<"Failed to load picture"<<endl;
        return -1;
    }
    imshow("Source Picture",src);

    cvtColor(src,gray_src,COLOR_BGR2GRAY);
    namedWindow(OUTPUT_WINDOW_NAME,WINDOW_AUTOSIZE);
    createTrackbar("maxCorners:",OUTPUT_WINDOW_NAME,&corner_value,corner_max,call_back);
    call_back(0,0);


    waitKey(0);
    return 0;
}
void call_back(int,void*){

    vector<Point2f> corners;
    goodFeaturesToTrack(gray_src,corners,corner_value,0.01,10,Mat(),3,false,0.004);

    Mat dst = src.clone();
    for(size_t i=0;i<corners.size();i++){
        circle(dst,corners[i],3,Scalar(rng.uniform(0,256),rng.uniform(0,256),rng.uniform(0,256)),2,LINE_8);
    }
    imshow(OUTPUT_WINDOW_NAME,dst);
}
