#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat windows_log = imread("/home/dogpi/Downloads/opencv-4.2.0/samples/data/WindowsLogo.jpg");
    Mat linux_log = imread("/home/dogpi/Downloads/opencv-4.2.0/samples/data/LinuxLogo.jpg");

    if(windows_log.empty()||linux_log.empty()){
        cout<<"read file fail"<<endl;
        return -1;
    }
    string windows_window = "windows log";
    string linux_window = "linux log";
    namedWindow(windows_window,WINDOW_AUTOSIZE);
    namedWindow(linux_window,WINDOW_AUTOSIZE);

    imshow(windows_window,windows_log);
    imshow(linux_window,linux_log);

    if(windows_log.size()!=linux_log.size()){
        cout<<"image size is not same"<<endl;
        return -2;
    }
    Mat dst;
    double alpha = 0.3;
    double beta = 1-alpha;
    double gamma = 30;
    addWeighted(windows_log,alpha,linux_log,beta,gamma,dst);
    imshow("addWeighted",dst);

    waitKey(0);
    return 0;
}
