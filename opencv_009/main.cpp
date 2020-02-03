#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int dilate_element_size = 3;
int dilate_max_size = 21;
int erode_element_size = 3;
int erode_max_size = 21;
Mat dilate_image,erode_image;
Mat src = imread("/home/dogpi/Pictures/i.png");
Mat cat = imread("/home/dogpi/Pictures/cat.jpg");
void call_back_dilate(int,void*);
void call_back_erode(int,void*);
int main()
{
    if(src.empty()){
        cout<<"could not load image"<<endl;
        return -1;
    }
    imshow("input",src);
    Mat gray_src;
    cvtColor(src,gray_src,COLOR_BGR2BGRA);

    Mat kernel = getStructuringElement(MORPH_RECT,Size(3,3),Point(-1,-1));
    // 膨胀
    dilate(gray_src,dilate_image,kernel,Point(-1,-1));
    imshow("dilate",dilate_image);
    // 腐蚀
    erode(gray_src,erode_image,kernel,Point(-1,-1));
    imshow("erode",erode_image);


    if(cat.empty()){
        cout<<"could not load image"<<endl;
        return -1;
    }
    imshow("cat",cat);

    namedWindow("dilate valiable",WINDOW_AUTOSIZE);
    namedWindow("erode valiable",WINDOW_AUTOSIZE);
    createTrackbar("value","dilate valiable",&dilate_element_size,dilate_max_size,call_back_dilate);
    createTrackbar("value","erode valiable",&erode_element_size,erode_max_size,call_back_erode);
    call_back_dilate(0,0);
    call_back_erode(0,0);


    waitKey(0);
    return 0;
}
void call_back_dilate(int,void*){
    int s = dilate_element_size*2+1;
    Mat kernel = getStructuringElement(MORPH_RECT,Size(s,s),Point(-1,-1));
    dilate(cat,dilate_image,kernel,Point(-1,-1));
    imshow("dilate valiable",dilate_image);
}
void call_back_erode(int,void*){
    int s = erode_element_size*2+1;
    Mat kernel = getStructuringElement(MORPH_RECT,Size(s,s),Point(-1,-1));
    erode(cat,erode_image,kernel,Point(-1,-1));
    imshow("erode valiable",erode_image);
}
