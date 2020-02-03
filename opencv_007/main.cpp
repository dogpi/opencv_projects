#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat src = imread("/home/dogpi/Downloads/opencv-4.2.0/samples/data/lena.jpg");
    if(src.empty()){
        cout<<"could load image"<<endl;
        return -1;
    }
    string input_title = "input image";
    string output_title = "blur image";
    namedWindow(input_title,WINDOW_AUTOSIZE);
    imshow(input_title,src);

    Mat dst;
    blur(src,dst,Size(5,5),Point(-1,-1));
    imshow(output_title,dst);

    GaussianBlur(src,dst,Size(5,5),9,9);
    imshow("gaus blur",dst);


    waitKey(0);
    return 0;
}
