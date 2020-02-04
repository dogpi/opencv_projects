#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat src,dst;
Mat map_x,map_y;
int num=0;
void update_map();

int main()
{
    src = imread("/home/dogpi/Pictures/samples/lena.jpg");
    if(src.empty()){
        return -1;
    }
    imshow("src",src);
\

    map_x.create(src.size(),CV_32FC1);
    map_y.create(src.size(),CV_32FC1);

    int c = 0;
    while (true) {
        c = waitKey(500);
        if(c==27){
            break;
        }
        update_map();
        remap(src,dst,map_x,map_y,INTER_LINEAR,BORDER_CONSTANT,Scalar(0,255,0));
        num = c%4;
        imshow("dst",dst);
    }

    waitKey(0);
    return 0;
}

void update_map(){
    for(int row=0;row<src.rows;row++){
        for(int col=0;col<src.cols;col++){
            switch (num) {
            case 0:// 图像缩小为原来的二分之一，相当于降采样
                if(col>=src.cols*0.25&&col<=src.cols*0.75&&row>=src.rows*0.25&&row<=src.rows*0.75){
                    // 保留偶数行和偶数列的像素
                    map_x.at<float>(row,col) = 2*(col-src.cols*0.25);
                    map_y.at<float>(row,col) = 2*(row-src.rows*0.25);
                }else {
                    map_x.at<float>(row,col) = 0;
                    map_y.at<float>(row,col) = 0;
                }
                break;
            case 1:// 图像关于y轴翻转
                map_x.at<float>(row,col) = src.cols-col;
                map_y.at<float>(row,col) = row;
                break;
            case 2:// 图像关于x轴翻转
                map_x.at<float>(row,col) = col;
                map_y.at<float>(row,col) = src.rows-row;
                break;
            case 3:// 图像关于xy轴翻转
                map_x.at<float>(row,col) = src.cols-col;
                map_y.at<float>(row,col) = src.rows-row;
                break;
            }
        }
    }
}
