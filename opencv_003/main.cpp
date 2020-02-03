#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat src = imread("/home/dogpi/Downloads/opencv-4.2.0/samples/data/lena.jpg");
    if(src.empty()){
        cout<<"read file fail"<<endl;
        return -1;
    }

    namedWindow("input",WINDOW_AUTOSIZE);
    imshow("input",src);

    Mat gray_src;
    cvtColor(src,gray_src,COLOR_BGR2GRAY);
    namedWindow("gray",WINDOW_AUTOSIZE);
    imshow("gray",gray_src);

    int height = gray_src.rows;
    int width = gray_src.cols;

    // 单通道图像颜色反转
//    for(int row = 0;row<height;row++){
//        for(int col=0;col<width;col++){
//            int gray = gray_src.at<uchar>(row,col);
//            gray_src.at<uchar>(row,col) = 255-gray;
//        }
//    }
//    namedWindow("convert",WINDOW_AUTOSIZE);
//    imshow("convert",gray_src);

    // 三通道图像颜色翻转
//    Mat dst = src.clone();
//    for(int row = 0;row<height;row++){
//        for(int col=0;col<width;col++){
//            // 这里使用Vec3b是因为每个像素点有三个元素，Vec3b是一个数组对象，有三个char元素
//            int b = dst.at<Vec3b>(row,col)[0];
//            int g = dst.at<Vec3b>(row,col)[1];
//            int r = dst.at<Vec3b>(row,col)[2];
//            dst.at<Vec3b>(row,col)[0] = 255-b;
//            dst.at<Vec3b>(row,col)[1] = 255-g;
//            dst.at<Vec3b>(row,col)[2] = 255-r;
//        }
//    }
//    namedWindow("convert2",WINDOW_AUTOSIZE);
//    imshow("convert2",dst);

    // 使用函数进行颜色翻转
    bitwise_not(gray_src,gray_src);
    namedWindow("convert",WINDOW_AUTOSIZE);
    imshow("convert",gray_src);

    bitwise_not(src,src);
    namedWindow("convert2",WINDOW_AUTOSIZE);
    imshow("convert2",src);

    waitKey(0);
    return 0;
}
