#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat src_gray;
    Mat src = imread("/home/dogpi/Pictures/samples/lines.png"
                      "");
    if(src.empty()){
        return -1;
    }
    imshow("lena",src);

    //边缘检测
    Canny(src,src_gray,100,200);
//    imshow("src canny",src_gray);

    //霍夫变换——直线检测
    vector<Vec4f> lines;
    HoughLinesP(src_gray,lines,1,CV_PI/180.0,10,0,10);
    Scalar color = Scalar(0,255,0);
    for(int i = 0;i<lines.size();i++){
        Vec4f line_ = lines[i];
        line(src,Point(line_[0],line_[1]),Point(line_[2],line_[3]),color,3,LINE_AA);
    }
    imshow("houghlines",src);

    Mat input = imread("/home/dogpi/Pictures/samples/smarties.png");
    if(input.empty()){
        return -1;
    }
//    imshow("smarties.png",input);
    Mat output;
    medianBlur(input,output,3);
    cvtColor(output,output,COLOR_BGR2GRAY);
    // 霍夫变换——圆检测
    vector<Vec3f> circles;
    HoughCircles(output,circles,HOUGH_GRADIENT,1,10,100,30,5,50);
    Mat dst;
    input.copyTo(dst);
    for(size_t i=0;i<circles.size();i++){
        Vec3f cc = circles[i];
        circle(dst,Point(cc[0],cc[1]),cc[2],Scalar(0,0,0),3,LINE_AA);
        circle(dst,Point(cc[0],cc[1]),2,Scalar(111,111,111),3,LINE_AA);
    }
    imshow("hough circle",dst);



    waitKey(0);
    return 0;
}
