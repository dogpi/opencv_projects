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

    FlannBasedMatcher matcher;
    vector<DMatch> matches;
    matcher.match(descriptors_1,descriptors_2,matches);

    // 选择一些好的matche
    double minDist = 100;
    double maxDist = 0;

    for(int i=0;i<descriptors_1.rows;i++){
        double dist = matches[i].distance;
        if(dist>maxDist){
            maxDist =dist;
        }
        if(dist<minDist){
            minDist=dist;
        }
    }
    vector<DMatch> goodMatchs;
    for(int i=0;i<descriptors_1.rows;i++){
        double dist = matches[i].distance;
        if(dist<max(2*minDist,0.02)){
            goodMatchs.push_back(matches[i]);
        }
    }

    Mat matche_image;
    drawMatches(sub_src,keypoints_1,src,keypoints_2,goodMatchs,matche_image,Scalar::all(-1), Scalar::all(-1),
                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );//不会绘制单个关键点。
    imshow("match",matche_image);

    waitKey(0);
    return 0;
}
