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

    Mat gray_src;
    cvtColor(src,gray_src,COLOR_BGR2GRAY);
    resize(src,gray_src,Size(64,128));
    imshow("gray_src",gray_src);
    HOGDescriptor detector(Size(64,128),Size(16,16),Size(8,8),Size(8,8),9);
    vector<float> descriptors;
    vector<Point> locations;
    detector.compute(gray_src,descriptors,Size(0,0),Size(0,0),locations);
    cout<<"number of HOG descriptors:"<<descriptors.size()<<endl;//结果一定是3780



    waitKey(0);
    return 0;
}
