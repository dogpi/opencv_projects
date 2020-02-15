#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

int main()
{

    Mat src = imread("/home/dogpi/Pictures/samples/people02.jpg");
    if(src.empty()){
        cout<<"Failed to load Picture"<<endl;
        return -1;
    }
    imshow("Sourec Picture",src);

    HOGDescriptor hog = HOGDescriptor();
    hog.setSVMDetector(hog.getDefaultPeopleDetector());

    vector<Rect> peoples;
    hog.detectMultiScale(src,peoples,0,Size(8,8),Size(32,32),1.05,2.0);
    for(int i =0;i<peoples.size();i++){
        rectangle(src,peoples[i],Scalar(0,0,255),2,8,0);
    }


    imshow("HOG SVM Detector Demo",src);


    waitKey(0);
    return 0;
}
