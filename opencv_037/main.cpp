#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

int main()
{

    Mat src = imread("/home/dogpi/Pictures/samples/lena.jpg");
    if(src.empty()){
        cout<<"Failed to load Picture"<<endl;
        return -1;
    }
    imshow("Sourec Picture",src);

    Ptr<SIFT> detector = SIFT::create(100);
    vector<KeyPoint> keypoints;
    detector->detect(src,keypoints,Mat());
    Mat dst;
    drawKeypoints(src,keypoints,dst,Scalar::all(-1),DrawMatchesFlags::DEFAULT);
    imshow("result",dst);


    waitKey(0);
    return 0;
}
