#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{

    Mat src = imread("/home/dogpi/Pictures/samples/card_photo.png");
    if(src.empty()){
        cout<<"Failed to load image"<<endl;
        return -1;
    }
    imshow("source",src);

//    blur(src,src,Size(3,3));
//    GaussianBlur(src,src,Size(5,5),9,9);

    Mat labels;
    Mat points(src.rows*src.cols,src.channels(),CV_32FC3);
    // 把像素值赋值给points
    for(int row=0;row<src.rows;row++){
        for(int col =0;col<src.cols;col++){
            int index = row*src.cols+col;
            points.at<float>(index,0) = src.at<Vec3b>(row,col)[0];
            points.at<float>(index,1) = src.at<Vec3b>(row,col)[1];
            points.at<float>(index,2) = src.at<Vec3b>(row,col)[2];
        }
    }
    Mat centers;
    kmeans(points,3,labels,TermCriteria(TermCriteria::EPS+TermCriteria::COUNT,10,0.1),3,KMEANS_PP_CENTERS,centers);
    Scalar colorTable[] = {
        Scalar(0,0,255),
        Scalar(0,255,0),
        Scalar(255,0,0),
        Scalar(0,255,255),
        Scalar(255,255,0)
    };
    Mat result(src.size(),CV_8UC3);
    for(int row=0;row<src.rows;row++){
        for(int col =0;col<src.cols;col++){
            int index = row*src.cols+col;
            int label = labels.at<int>(index);
            result.at<Vec3b>(row,col)[0] = colorTable[label][0];
            result.at<Vec3b>(row,col)[1] = colorTable[label][1];
            result.at<Vec3b>(row,col)[2] = colorTable[label][2];
        }
    }


    imshow("Kmeans",result);
    waitKey(0);
    return 0;
}
