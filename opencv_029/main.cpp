#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(){

    // 创建图像
    const int r = 100;
    Mat src = Mat::zeros(Size(4*r,4*r),CV_8UC3);

    //定义点集合，用于生成多边形
    vector<Point2f> vert(6);// 六边形
    vert[0] = Point( 3*r/2, static_cast<int>(1.34*r) );
    vert[1] = Point( 1*r, 2*r );
    vert[2] = Point( 3*r/2, static_cast<int>(2.866*r) );
    vert[3] = Point( 5*r/2, static_cast<int>(2.866*r) );
    vert[4] = Point( 3*r, 2*r );
    vert[5] = Point( 5*r/2, static_cast<int>(1.34*r) );

    // 绘制多边形
    for(size_t i=0;i<vert.size();i++){
        line(src,vert[i],vert[(i+1)%6],Scalar(255,255,255),1);
    }

//    imshow("src",src);

    Mat gray_src;
    cvtColor(src,gray_src,COLOR_BGR2GRAY);

    // findContours
    vector<vector<Point>> contours;
    findContours(gray_src,contours,RETR_TREE,CHAIN_APPROX_SIMPLE);

//    RNG rng(1234);
//    for(size_t i=0;i<contours.size();i++){
//        Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
//        drawContours(src,contours,(int)i,color,1,LINE_8);
//    }
//    imshow("src",src);

    // 计算距离contur的距离
    Mat raw_dist(src.size(),CV_32F);
    for(int i=0;i<src.rows;i++){
        for(int j=0;j<src.cols;j++){
            // pointPolygonTest最后一个参数设置为true，在多边形外的距离为负值，在多边形内的距离为正值，在多边形边上的值为0
            raw_dist.at<float>(i,j) = (float)pointPolygonTest(contours[0],Point2f((float)j,(float)i),true);
        }
    }

    double minVal,maxVal;
    Point maxDistPt,minDistPt;
    minMaxLoc(raw_dist,&minVal,&maxVal,&minDistPt,&maxDistPt);
    minVal = abs(minVal);
    maxVal = abs(maxVal);
    cout<<"raw_dist Mat at(x,y):"<<raw_dist.at<float>(minDistPt)<<endl;
    cout<<"minValue:"<<minVal<<"\nmaxValue:"<<maxVal<<endl;
    // 绘制距离变换图
    for(int i=0;i<src.rows;i++){
        for(int j=0;j<src.cols;j++){
            if(raw_dist.at<float>(i,j)<0){
                src.at<Vec3b>(i,j)[0] = (uchar)(255-abs(raw_dist.at<float>(i,j))*255/minVal);//(1-abs(矩形外的值)/abs(最小值))×255
            }else if( raw_dist.at<float>(i,j) > 0 )
            {
                src.at<Vec3b>(i,j)[2] = (uchar)(255 - raw_dist.at<float>(i,j) * 255 / maxVal);
            }
            else
            {
                src.at<Vec3b>(i,j)[0] = 255;
                src.at<Vec3b>(i,j)[1] = 255;
                src.at<Vec3b>(i,j)[2] = 255;
            }
        }
    }
    circle(src, maxDistPt, (int)maxVal, Scalar(255,255,255));
    circle(src, maxDistPt, (int)2, Scalar(255,255,255),2,LINE_AA);
    imshow("src",src);
    waitKey(0);
    return 0;
}
