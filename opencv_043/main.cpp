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
        if(dist<max(3*minDist,0.02)){
            goodMatchs.push_back(matches[i]);
        }
    }

    Mat matche_image;
    drawMatches(sub_src,keypoints_1,src,keypoints_2,goodMatchs,matche_image,Scalar::all(-1), Scalar::all(-1),
                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );//不会绘制单个关键点。
    imshow("match",matche_image);

    // 要想找到子图像与匹配到的图像的关系，就要找到匹配点之间的关系
    vector<Point2f> sub_points;
    vector<Point2f> main_points;

    for(int i=0;i<goodMatchs.size();i++){
        // 先得到匹配点在keypoint中的位置
        // 再取keypoint在Mat也就是原图像中的位置
        sub_points.push_back(keypoints_1[goodMatchs[i].queryIdx].pt);// DMatch内的对象是一一对应的 子图中的是queryIdx
        main_points.push_back(keypoints_2[goodMatchs[i].trainIdx].pt);// 训练图中的是 trainIdx，也就是说queryIdx对应trainIdx
    }

    // 找到子图与完整图的特征点之间的关系
    Mat H = findHomography(sub_points,main_points,RANSAC);

    // 子图像的四个角的点
    vector<Point2f> sub_corners(4);
    sub_corners[0] = Point(0,0); // 左上角
    sub_corners[1] = Point(sub_src.cols,0); //右上角
    sub_corners[2] = Point(sub_src.cols,sub_src.rows);    // 右下角
    sub_corners[3] = Point(0,sub_src.rows);   // 左下角

    // 根据子图与完整图之间的关系，将子图上的位置映射到完整图片上的位置
    vector<Point2f> main_corners(4);
    perspectiveTransform(sub_corners,main_corners,H);

    line(matche_image,main_corners[0]+Point2f(sub_src.cols,0),main_corners[1]+Point2f(sub_src.cols,0),Scalar(0,0,255),2,LINE_8);
    line(matche_image,main_corners[1]+Point2f(sub_src.cols,0),main_corners[2]+Point2f(sub_src.cols,0),Scalar(0,0,255),2,LINE_8);
    line(matche_image,main_corners[2]+Point2f(sub_src.cols,0),main_corners[3]+Point2f(sub_src.cols,0),Scalar(0,0,255),2,LINE_8);
    line(matche_image,main_corners[3]+Point2f(sub_src.cols,0),main_corners[0]+Point2f(sub_src.cols,0),Scalar(0,0,255),2,LINE_8);

    imshow("find",matche_image);

    waitKey(0);
    return 0;
}
