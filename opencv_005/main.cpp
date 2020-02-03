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

    int height = src.rows;
    int width = src.cols;

    Mat dst=Mat::zeros(src.size(),src.type());
    double alpha = 1.8;
    double beta = 30;
    for(int row=0;row<height;row++){
        for(int col=0;col<width;col++){
            if(dst.channels()==3){
                uchar b = src.at<Vec3b>(row,col)[0];
                uchar g = src.at<Vec3b>(row,col)[1];
                uchar r = src.at<Vec3b>(row,col)[2];

                dst.at<Vec3b>(row,col)[0] = saturate_cast<uchar>(b*alpha+beta);
                dst.at<Vec3b>(row,col)[1] = saturate_cast<uchar>(g*alpha+beta);
                dst.at<Vec3b>(row,col)[2] = saturate_cast<uchar>(r*alpha+beta);

            }else if (src.channels()==1) {
                uchar v = src.at<uchar>(row,col);
                dst.at<uchar>(row,col) = saturate_cast<uchar>(v*alpha + beta);
            }
        }
    }

    imshow("output",dst);

    waitKey(0);
    return 0;
}
