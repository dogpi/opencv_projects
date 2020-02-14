#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

int main()
{
    Mat image_1 = imread("/home/dogpi/Pictures/samples/graf1.png",COLOR_BGR2GRAY);
    Mat image_2 = imread("/home/dogpi/Pictures/samples/graf3.png",COLOR_BGR2GRAY);

    if(image_1.empty()||image_2.empty()){
        cout<<"Failed to load pictures"<<endl;
        return -1;
    }
    imshow("image_1",image_1);
    imshow("image_2",image_2);
    Ptr<SURF> dector = SURF::create(100);

    vector<KeyPoint> keyPoints;
    dector->detect(image_1,keyPoints,Mat());
    drawKeypoints(image_1,keyPoints,image_1,Scalar::all(-1),DrawMatchesFlags::DEFAULT);
    imshow("image_1 output",image_1);
    dector->detect(image_2,keyPoints,Mat());
    drawKeypoints(image_2,keyPoints,image_2,Scalar::all(-1),DrawMatchesFlags::DEFAULT);
    imshow("image_2 output",image_2);


    waitKey(0);
    return 0;
}
