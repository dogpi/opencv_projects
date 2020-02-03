#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat src,gray_src,dst;
    src = imread("/home/dogpi/Pictures/samples/lena.jpeg");
    if(src.empty()){
        return -1;
    }
//    imshow("input",src);

    Mat kernel_robert_x = (Mat_<int>(2,2)<<1,0,0,-1);
    filter2D(src,dst,-1,kernel_robert_x);
    imshow("robert x",dst);

    Mat kernel_robert_y = (Mat_<int>(2,2)<<0,1,-1,0);
    filter2D(src,dst,-1,kernel_robert_y);
    imshow("robert y",dst);

    Mat kernel_sobel_x = (Mat_<int>(3,3)<<-1,0,1,-2,0,2,-1,0,1);
    filter2D(src,dst,-1,kernel_sobel_x);
    imshow("sobel x",dst);

    Mat kernel_sobel_y = (Mat_<int>(3,3)<<-1,-2,-1,0, 0, 0,1, 2, 1);
    filter2D(src,dst,-1,kernel_sobel_y);
    imshow("sobel y",dst);

    Mat kernel_lapulas = (Mat_<int>(3,3)<<0,-1,0,-1, 4, -1,0, -1, 0);
    filter2D(src,dst,-1,kernel_lapulas);
    imshow("lapulas y",dst);

    Mat kernel = (Mat_<int>(3,3)<<0,-1,0,-1,5 , -1,0, -1, 0);
    filter2D(src,dst,-1,kernel);
    imshow("filter2D",dst);


    int c = 0;
    int index = 0;
    int ksize = 3;
    while(true){
        c = waitKey(500);
        if(c==27){
            break;
        }
        ksize = 2*(index%5)+3; // kernel Size(3,3) Size(5,5) Size(7,7)...Size(11,11)

        Mat kernel_k = Mat::ones(Size(ksize,ksize),CV_32F)/(float)(ksize*ksize);//取卷积和的平均值
        filter2D(src,dst,-1,kernel_k,Point(-1,-1));
        index++;
        imshow("kernel k",dst);
    }


    waitKey(0);
    return 0;
}
