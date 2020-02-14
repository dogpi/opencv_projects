#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat src,gray_src;
const char* output_window_title = "subpix result";
// 最大角点数50-100
int corner_value = 50;
int corner_max = 100;
double qualityLevel = 0.01;
int blockSize = 3;

void call_back(int,void*);

int main()
{
    src = imread("/home/dogpi/Pictures/samples/home.jpg");
    if(src.empty()){
        cout<<"Failed to load picture"<<endl;
        return -1;
    }
    imshow("Source Picture",src);

    // 转为灰度图像
    cvtColor(src,gray_src,COLOR_BGR2GRAY);

    namedWindow(output_window_title,WINDOW_AUTOSIZE);
    createTrackbar("maxCorner:",output_window_title,&corner_value,corner_max,call_back);
    call_back(0,0);

    waitKey(0);
    return 0;
}

void call_back(int,void*){
    system("clear");
    // Shi-Tomasi获取角点
    vector<Point2f> corners;
    goodFeaturesToTrack(gray_src,corners,corner_value,qualityLevel,10,Mat(),blockSize);
    cout<<"number of corners:"<<corners.size()<<endl;

    vector<Point2f> subcorners(corners.begin(),corners.end());
    Size windowSize = Size(5,5);
    Size zeroZone= Size(-1,-1);
    TermCriteria tc = TermCriteria(TermCriteria::EPS+TermCriteria::MAX_ITER,40,0.001);
    cornerSubPix(gray_src,subcorners,windowSize,zeroZone,tc);
    cout<<"corners"<<"\t\t\t"<<"subcorners"<<endl;
    for(int i=0;i<corners.size();i++){
        cout<<corners[i]<<"\t\t"<<subcorners[i]<<endl;
    }
}
