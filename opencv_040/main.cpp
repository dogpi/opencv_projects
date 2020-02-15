#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
int radius_value = 3;
int radius_max = 20;
Mat src,gray_src;

void call_back(int,void*);

int main()
{
    // 读取图像
    src = imread("/home/dogpi/Pictures/samples/lena.jpg");
    if(src.empty()){
        cout<<"Failed to load picture"<<endl;
        return -1;
    }
    imshow("Source picture",src);

    //转为灰度图像
    cvtColor(src,gray_src,COLOR_BGR2GRAY);
    Mat dst(Size(src.cols-2,src.rows-2),CV_8U);
    //计算LBP
    for(int row=1;row<gray_src.rows;row++){
        for(int col=0;col<gray_src.cols;col++){
            uchar center = gray_src.at<uchar>(row,col);
            //由于计算后的值在0-255之间，所以要使用uchar来保存最为合适
            uchar lbpcode = 0;
            lbpcode|=(gray_src.at<uchar>(row-1,col-1)>center)<<7;
            lbpcode|=(gray_src.at<uchar>(row-1,col)>center)<<6;
            lbpcode|=(gray_src.at<uchar>(row-1,col+1)>center)<<5;
            lbpcode|=(gray_src.at<uchar>(row,col+1)>center)<<4;
            lbpcode|=(gray_src.at<uchar>(row+1,col+1)>center)<<3;
            lbpcode|=(gray_src.at<uchar>(row+1,col)>center)<<2;
            lbpcode|=(gray_src.at<uchar>(row+1,col-1)>center)<<1;
            lbpcode|=(gray_src.at<uchar>(row,col-1)>center)<<0;
            // 不要直接把值赋值给gray_src，会影响下一次循环的值。
            // gray_src.at<uchar>(row,col) = lbpcode;
            dst.at<uchar>(row,col) = lbpcode;
        }
    }
    // imshow("LBP",gray_src);
    imshow("LBP",dst);

    // 改进版LBP
    namedWindow("ELBP",WINDOW_AUTOSIZE);
    createTrackbar("Radius:","ELBP",&radius_value,radius_max,call_back);
    call_back(0,0);

    waitKey(0);
    return 0;
}
void call_back(int,void*){
    int neighbors = 8;
    radius_value = max(radius_value,1);
    Mat dst(Size(src.cols-2*radius_value,src.rows-2*radius_value),CV_8U);

    //对每个像素进行处理
    for(int row=radius_value;row<src.rows-2*radius_value;row++){
        for(int col=radius_value;col<src.cols-2*radius_value;col++){

            uchar center = gray_src.at<uchar>(row,col);
            uchar lbpcode=0;
            // 取8个点
            // 计算8个点的坐标
            for(int k=0;k<neighbors;k++){
                float x = row+static_cast<float>(radius_value*cos(2.0*CV_PI*k/neighbors));
                float y = col-static_cast<float>(radius_value*sin(2.0*CV_PI*k/neighbors));

                // 由于不是整数，要对取到的点进行双线性插值计算，取像素值
                // 要使用双线性插值计算就要得带点周围的4个整值点，也就是图像中真是存在的点
                // 对取到的点进行取整即可
                int x1 = static_cast<int>(floor(x));
                int x2 = static_cast<int>(ceil(x));
                int y1 = static_cast<int>(floor(y));
                int y2 = static_cast<int>(ceil(y));

                // 计算四个点(x1,y1),(x1,y2),(x2,y1),(x2,y2)的权重
                // 将坐标映射到0-1之间，不使用四个点直接计算权重是因为如果四个点相同，计算得带的权重就会是0，插值也会为0
                float tx = x-x1;
                float ty = y-y1;

                float w1 = (1-tx)*(1-ty);
                float w2 = tx*(1-ty);
                float w3 = (1-tx)*ty;
                float w4 = tx*ty;

                // 计算出8个点中一个的像素值
                float value = gray_src.at<uchar>(x1,y1)*w1+
                              gray_src.at<uchar>(x1,y2)*w2+
                              gray_src.at<uchar>(x2,y1)*w3+
                              gray_src.at<uchar>(x2,y2)*w4;
                lbpcode |=(value>center)<<(neighbors-k-1);
            }
            dst.at<uchar>(row-radius_value,col-radius_value) = lbpcode;
        }
    }
    imshow("ELBP",dst);
}
