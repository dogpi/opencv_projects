#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat src,dst;
    src = imread("/home/dogpi/Pictures/cat.jpg");
    if(src.empty()){
        return -1;
    }

    imshow("input cat image",src);

    // 降采样
    pyrDown(src,dst,Size(src.cols/2,src.rows/2));
    imshow("pyrDown",dst);

    // 上采样
    pyrUp(src,dst,Size(src.cols*2,src.rows*2));
    imshow("pyrUp",dst);


    Mat g1,g2,dog_image;

    GaussianBlur(src,g1,Size(5,5),0,0);
    GaussianBlur(g1,g2,Size(5,5),0,0);
    subtract(g1,g2,dog_image);
    // 直接显示的高斯不同不明显
    // 使用归一化处理使显示清晰
    normalize(dog_image,dog_image,255,0,NORM_MINMAX);

    imshow("DOG image",dog_image);

    waitKey(0);
    return 0;
}
