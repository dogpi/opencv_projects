#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

int main()
{
    Mat sub_image = imread("/home/dogpi/Pictures/samples/box.png",COLOR_BGR2GRAY);
    Mat main_image = imread("/home/dogpi/Pictures/samples/box_in_scene.png",COLOR_BGR2GRAY);
    if(sub_image.empty()||main_image.empty()){
        cout<<"Failed to load pictures"<<endl;
        return -1;
    }
    imshow("sub image",sub_image);
    imshow("main image",main_image);

    // 创建特征检测对象
    Ptr<BRISK> detector = BRISK::create();

    vector<KeyPoint> keypoints_sub;
    Mat descriptors_sub;
    vector<KeyPoint> keypoints_main;
    Mat descriptors_main;

    // 得到特征点和描述子
    double t = getTickCount();
    detector->detectAndCompute(sub_image,Mat(),keypoints_sub,descriptors_sub,false);
    detector->detectAndCompute(main_image,Mat(),keypoints_main,descriptors_main,false);

    // 创建暴力匹配器
    BFMatcher matcher(NORM_HAMMING);
    vector<DMatch> matches;
    // 匹配描述子，得到匹配关系matches
    matcher.match(descriptors_sub,descriptors_main,matches);
    t = ((double)getTickCount()-t)/getTickFrequency();

    // 选择合适的关键点
    // 1、计算对应描述子的最大最小距离
    double max_dist = 0;
    double min_dist = 1000;
    for(int i=0;i<matches.size();i++){
        if(matches[i].distance<min_dist){
            min_dist = matches[i].distance;
        }
        if(matches[i].distance>max_dist){
            max_dist = matches[i].distance;
        }
    }
    // 2、根据描述子的距离选择matches
    vector<DMatch> good_matches;
    for(int i=0;i<matches.size();i++){
        double dist = matches[i].distance;
        if(dist<2*min_dist){
            good_matches.push_back(matches[i]);
        }
    }
    cout<<"spend time:"<<t<<endl;


    Mat dst;
    drawMatches(sub_image,keypoints_sub,main_image,keypoints_main,good_matches,dst,Scalar::all(-1),Scalar::all(-1),vector<char>(),DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    // 根据matches找到对应的keypoint点，再找到对应图像上的Point
    vector<Point2f> sub_points;
    vector<Point2f> main_points;
    for(int i=0;i<good_matches.size();i++){
        sub_points.push_back(keypoints_sub[good_matches[i].queryIdx].pt);
        main_points.push_back(keypoints_main[good_matches[i].trainIdx].pt);
    }

    // 对KeyPoint进行映射，得到sub_image在main_image中的位置。
    Mat H = findHomography(sub_points,main_points,RANSAC);

    vector<Point2f> sub_coeners(4);
    sub_coeners[0] = Point2f(0,0);//左上角
    sub_coeners[1] = Point2f(sub_image.cols,0);//右上角
    sub_coeners[2] = Point2f(sub_image.cols,sub_image.rows);//右下角
    sub_coeners[3] = Point2f(0,sub_image.rows);//左下角

    vector<Point2f> main_corners(4);
    perspectiveTransform(sub_coeners,main_corners,H);

    line(dst,main_corners[0]+Point2f(sub_image.cols,0),main_corners[1]+Point2f(sub_image.cols,0),Scalar(0,255,0),2,8);
    line(dst,main_corners[1]+Point2f(sub_image.cols,0),main_corners[2]+Point2f(sub_image.cols,0),Scalar(0,255,0),2,8);
    line(dst,main_corners[2]+Point2f(sub_image.cols,0),main_corners[3]+Point2f(sub_image.cols,0),Scalar(0,255,0),2,8);
    line(dst,main_corners[3]+Point2f(sub_image.cols,0),main_corners[0]+Point2f(sub_image.cols,0),Scalar(0,255,0),2,8);

    imshow("detector",dst);

    waitKey(0);
    return 0;
}
