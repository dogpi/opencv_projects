#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

int main()
{
    Mat sub_src = imread("/home/dogpi/Pictures/samples/box.png",COLOR_BGR2GRAY);
    Mat src = imread("/home/dogpi/Pictures/samples/box_in_scene.png",COLOR_BGR2GRAY);

    if(sub_src.empty()||src.empty()){
        cout<<"Failed to load pictures"<<endl;
        return -1;
    }
    imshow("sub src",sub_src);
    imshow("src",src);

    vector<KeyPoint> keypoints_1;
    Mat descriptors_1;
    Ptr<SURF> detector = SURF::create(300);
    detector->detectAndCompute(sub_src,Mat(),keypoints_1,descriptors_1,false);
    vector<KeyPoint> keypoints_2;
    Mat descriptors_2;
    detector->detectAndCompute(src,Mat(),keypoints_2,descriptors_2,false);

    BFMatcher matcher;
    vector<DMatch> matches;
    matcher.match(descriptors_1,descriptors_2,matches);


    Mat matche_image;
    drawMatches(sub_src,keypoints_1,src,keypoints_2,matches,matche_image);
    imshow("match",matche_image);

    waitKey(0);
    return 0;
}
