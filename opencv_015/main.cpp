#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat src,dst;
    src = imread("/home/dogpi/Pictures/samples/cat.jpg");
    if(src.empty()){
        return -1;
    }
    imshow("input",src);

    int top = (int)(0.05*src.rows);
    int bottom = (int)(0.05*src.rows);
    int left = (int)(0.05*src.rows);
    int right = (int)(0.05*src.rows);


    // 有卷积操作的函数可以指定边缘处理方式
    GaussianBlur(src,dst,Size(3,3),0,0,BORDER_WRAP);
    imshow("Gaus BORDER_WRAR",dst);

    RNG rng(12345);
    int borderType = BORDER_DEFAULT;
    int c = 0;
    while(true){
        c=waitKey(500);
        if(c==27){
            break;
        }
        if((char)c == 'r'){
            borderType=BORDER_REFLECT;
        }else if ((char)c=='w') {
            borderType=BORDER_WRAP;
        }else if ((char)c=='c') {
            borderType=BORDER_CONSTANT;
        }else {
            borderType=BORDER_DEFAULT;
        }
        copyMakeBorder(src,dst,top,bottom,left,right,borderType,Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255)));
        imshow("output",dst);
    }

    waitKey(0);
    return 0;
}
