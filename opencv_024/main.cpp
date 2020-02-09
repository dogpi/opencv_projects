#include <iostream>
#include <opencv2/opencv.hpp>
/*
enum TemplateMatchModes {
    TM_SQDIFF        = 0,
    TM_SQDIFF_NORMED = 1,
    TM_CCORR         = 2,
    TM_CCORR_NORMED  = 3,
    TM_CCOEFF        = 4,
    TM_CCOEFF_NORMED = 5
};
*/
using namespace cv;
using namespace std;
Mat src = imread("/home/dogpi/Pictures/samples/pic1.png");
Mat tmp = imread("/home/dogpi/Pictures/samples/templ.png");
int width = src.cols-tmp.cols+1;
int height = src.rows-tmp.rows+1;
int method = TM_SQDIFF;
int methodmax = TM_CCOEFF_NORMED;
void call_back(int,void*);
int main()
{

    if(src.empty()||tmp.empty()){
        cout<<"could not load image"<<endl;
        return -1;
    }
//    imshow("src",src);
    imshow("temple",tmp);
    namedWindow("result",WINDOW_AUTOSIZE);
    createTrackbar("method","result",&method,methodmax,call_back);
    call_back(0,0);



    waitKey(0);
    return 0;
}
void call_back(int,void*){
    Mat result(width,height,CV_32FC1);
    Mat dst;
    src.copyTo(dst);
    matchTemplate(dst,tmp,result,method);    //该方法的最小值为最匹配
    normalize(result,result,0,1,NORM_MINMAX,-1,Mat());
    double min,max;
    Point minPoint,maxPoint;
    Point tmpPoint;
    minMaxLoc(result,&min,&max,&minPoint,&maxPoint,Mat());
    if(method==TM_SQDIFF||method==TM_SQDIFF_NORMED){
        tmpPoint = minPoint;    //只有这两种方法是值越小越相关。
    }
    else {
        tmpPoint = maxPoint;    // 其他方法都是值越大越相关。
    }

    rectangle(dst,Rect(tmpPoint.x,tmpPoint.y,tmp.cols,tmp.rows),Scalar(255,0,0),1,LINE_AA);
    stringstream ss;
    string string_method;
    ss<<method;
    ss>>string_method;
    putText(dst,string_method,Point(tmpPoint.x,tmpPoint.y),FONT_HERSHEY_SIMPLEX,0.25,Scalar(255,0,0));
    imshow("result",dst);
}
