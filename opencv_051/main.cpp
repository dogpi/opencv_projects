#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat src;
Mat mask;
Mat bgd_model,fgd_model;
Rect rect;
bool init = false;
char * WINDOW_TITLE = "GrabCut";
char * WINDOW_RECT = "get rect";
void mouse_call_back(int event,int x,int y,int flags,void *userdata);
void set_mask(Mat mask,Rect rect);
void run_grabcut();
void show_image(Mat &src,Mat *mask);
int main()
{
    // 加载图像
    src = imread("/home/dogpi/Pictures/samples/card_photo.png");
    if(src.empty()){
        cout<<"Failed to load picture"<<endl;
        return -1;
    }
    namedWindow(WINDOW_TITLE,WINDOW_AUTOSIZE);
    namedWindow(WINDOW_RECT,WINDOW_AUTOSIZE);
//    imshow("Source",src);

    // 可以得到一个矩形
    setMouseCallback(WINDOW_RECT,mouse_call_back,0);
    imshow(WINDOW_RECT,src);
    // GrabCut需要一个mask也就是掩码，根据掩码区分前景和背景
    // mask大小与src大小形同
    mask.create(src.size(),CV_8U);

    while (true) {
        char c = waitKey(0);
        if(c == 'n'){
            run_grabcut();
            show_image(src,&mask);
        }
        if((int)c==27){
            break;
        }
    }

    waitKey(0);
    return 0;
}
void mouse_call_back(int event,int x,int y,int flags,void *userdata){
    // 判断鼠标左键是否按下并且移动鼠标
    Mat result_rect;
    switch(event){
    case EVENT_LBUTTONDOWN:
        rect.x = x;
        rect.y = y;
        init=false;
        break;
    case EVENT_MOUSEMOVE:
        if(flags&EVENT_LBUTTONDOWN){
            rect = Rect(Point(rect.x,rect.y),Point(x,y));
            result_rect = src.clone();
            rectangle(result_rect,rect,Scalar(0,0,255),2,8);
            imshow(WINDOW_RECT,result_rect);
        }
        break;
    case EVENT_LBUTTONUP:
        if(rect.width>1&&rect.height>1){
            set_mask(mask,rect);
        }
        break;
    }
}

void set_mask(Mat mask,Rect rect){
    mask.setTo(GC_BGD); //设置mask所以数据都是背景
    rect.x = max(0, rect.x);
    rect.y = max(0, rect.y);
    rect.width = min(rect.width, src.cols - rect.x);
    rect.height = min(rect.height, src.rows - rect.y);
    mask(rect).setTo(Scalar(GC_PR_FGD)); //设置矩形区域内为前景
}

void run_grabcut(){
    if (rect.width < 2 || rect.height < 2) {
            return;
    }
    if(init){
        // 在没有获得矩形框（也就是前景）时，默认rect为最大值，也就是所有像素都是前景，
        // 所有mask的值都是1，显示图像时相当于拷贝src
        grabCut(src,mask,rect,bgd_model,fgd_model,1);
    }else {
        grabCut(src,mask,rect,bgd_model,fgd_model,1,GC_INIT_WITH_RECT);
        init = true;
    }
}
void show_image(Mat &src,Mat *mask = NULL){
    Mat result;
    if(mask){
        Mat bin_mask_tmp;
        bin_mask_tmp.create(src.size(),CV_8U);
        bin_mask_tmp = (*mask)&1;
        if(init){
            src.copyTo(result,bin_mask_tmp);
        }else {
            src.copyTo(result);
        }
    }else {
        src.copyTo(result);
    }
    imshow(WINDOW_TITLE,result);
}
