#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    // 创建图像
    const int r = 100;
    Mat src = Mat::zeros(Size(4*r,4*r),CV_8UC3);

    //定义点集合，用于生成多边形
    Point vert[6];// 六边形
    vert[0] = Point( 3*r/2, static_cast<int>(1.34*r) );
    vert[1] = Point( 1*r, 2*r );
    vert[2] = Point( 3*r/2, static_cast<int>(2.866*r) );
    vert[3] = Point( 5*r/2, static_cast<int>(2.866*r) );
    vert[4] = Point( 3*r, 2*r );
    vert[5] = Point( 5*r/2, static_cast<int>(1.34*r) );
    const Point* ppt[1] = {vert};
    // 绘制多边形
//    for(size_t i=0;i<vert.size();i++){
//        line(src,vert[i],vert[(i+1)%6],Scalar(255,255,255),1);
//    }
    int npt = 6;
    fillPoly(src,ppt,(const int *)&npt,1,Scalar(255,255,255),LINE_AA);

    imshow("Source Picture",src);

    Mat gray_src;
    cvtColor(src,gray_src,COLOR_BGR2GRAY);

    //这里不使用CV_32F的原因是防止distance中的距离都是[0.0-1.1]之间的实数
    //在计算最大距离时可以得到准确的最大距离。
    Mat distance(src.size(),CV_8U);
    distanceTransform(gray_src,distance,DIST_L2,3);

    imshow("distance",distance);

    double maxValue;
    Point maxPoint;

    minMaxLoc(distance,0,&maxValue,0,&maxPoint);
    cout<<"MaxPoint: "<<maxPoint<<"\tMaxValue: "<<maxValue<<endl;


    circle(src,maxPoint,maxValue,Scalar(0,0255),2,LINE_AA);
    circle(src,maxPoint,2,Scalar(0,0255),2,LINE_AA);
    imshow("result",src);

    waitKey(0);
    return 0;
}
