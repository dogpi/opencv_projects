#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void liner(Mat &mat);
void recter(Mat &mat);
void ellipser(Mat &mat);
void circler(Mat &mat);
void polyliner(Mat &mat);
int main()
{
    Mat map(512,512,CV_8UC3);
    map= Scalar(255,255,255);   //512*512 白色图片

    liner(map);
    recter(map);
    ellipser(map);
    circler(map);
    polyliner(map);
    putText(map,"Hello Opencv",Point(100,100),FONT_HERSHEY_SIMPLEX,1.0,Scalar(0,255,255),2,LINE_8);
    namedWindow("show",WINDOW_AUTOSIZE);
    imshow("show",map);

    waitKey(0);
    return 0;
}

void liner(Mat &mat){
    Point a(100,100);
    Point b;
    b.x=200;
    b.y=200;

    Scalar color = Scalar(0,0,255); //红色
    line(mat,a,b,color,5,LINE_AA);  // 第5个参数为线的粗细  第6个参数为平滑程度LINE_8有锯齿，速度快，LINE_AA反锯齿
}
void recter(Mat &mat){
    Rect rect = Rect(200,100,300,200);  // 矩形位置从点200,100开始，宽300，高200
    rectangle(mat,rect,Scalar(255,0,0),1,LINE_AA);
}
void ellipser(Mat &mat){
    // 椭圆           圆心       横轴半长 纵轴ban长  椭圆旋转角度   椭圆的范围，从0度画到360度
    ellipse(mat,Point(300,300),Size(100,200),10        ,  0,360,Scalar(255,0,0),1,LINE_8);
}
void circler(Mat &mat){
    circle(mat,Point(300,300),50,Scalar(100,200,100),1,LINE_8);
}
void polyliner(Mat &mat){

//    Point point[1][5];
//    point[0][0] = Point(100,100);
//    point[0][1] = Point(150,150);
//    point[0][2] = Point(200,300);
//    point[0][3] = Point(150,200);
//    point[0][4] = Point(100,100);

    Point point[5];
    point[0] = Point(100,100);
    point[1] = Point(150,150);
    point[2] = Point(200,300);
    point[3] = Point(150,200);
    point[4] = Point(100,100);

    // polylines中要传入的参数是const Point* *pts，也就是一个Point类型的指针，存放着Point对象的指针
    // 也就是一个Point*数组，里面存放着Point*
    const Point* ppts[] = {point};
    int n = 5;
    const int *a = &n;
    // 画多边形
    polylines(mat,ppts,a,1,true,Scalar(0,0,0),1,LINE_8,0);
    // 给多边形填充颜色
    fillPoly(mat,ppts,a,1,Scalar(111,111,123));
}
