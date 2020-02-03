#include <iostream>
#include <string.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    // Mat 的相关操作
    Mat src = imread("/home/dogpi/Downloads/opencv-4.2.0/samples/data/lena.jpg");
    if(src.empty()){
        cout<<"read file fail"<<endl;
        return -1;
    }

    namedWindow("input",WINDOW_AUTOSIZE);
    imshow("input",src);

    // Scalar函数，把所有的通道用指定的值赋值
    Mat dst;
    dst = Mat(src.size(),src.type());
    dst = Scalar(255,0,0);
    namedWindow("Scalar B",WINDOW_AUTOSIZE);
    imshow("Scalar B",dst);
    dst = Scalar(0,255,0);
    namedWindow("Scalar G",WINDOW_AUTOSIZE);
    imshow("Scalar G",dst);
    dst = Scalar(0,0,255);
    namedWindow("Scalar R",WINDOW_AUTOSIZE);
    imshow("Scalar R",dst);
    dst = Scalar(100,100,100);
    namedWindow("Scalar MIX",WINDOW_AUTOSIZE);
    imshow("Scalar MIX",dst);

    // 克隆
    Mat c = src.clone();
    namedWindow("clone",WINDOW_AUTOSIZE);
    imshow("clone",c);

    // 拷贝
    Mat s;
    src.copyTo(s);
    namedWindow("copyTo",WINDOW_AUTOSIZE);
    imshow("copyTo",s);

    // 色域转换
    Mat d;
    cvtColor(src,d,COLOR_BGR2GRAY);
    namedWindow("gray",WINDOW_AUTOSIZE);
    imshow("gray",d);
    cout<<"input channels:"<<src.channels()<<endl;
    cout<<"gray channels:"<<d.channels()<<endl;

    // 指针操作Mat
    cout<<"rows: "<<d.rows<<endl;
    cout<<"cols: "<<d.cols<<endl;
    cout<<"channels: "<<d.channels()<<endl;
    const uchar* firstRow = d.ptr<uchar>(5);//这是一个数组
    printf("first pixel values:%d",*firstRow);// 打印数组第一个元素的值
    int count = 0;
    while(*firstRow){
        cout<<int(*firstRow)<<" ";
        if(count % 32==0){
            cout<<endl;
        }
        count++;
        firstRow++;
        if(count>512){
            break;
        }
    }
//    cout<<"count: "<<count<<endl;

    // 定义小矩阵
    Mat kernel = (Mat_<int>(3,3) << 0,-1,0,-1,5,-1,0,-1,0 );
    cout<<kernel<<endl;

    waitKey(0);
    return 0;
}
