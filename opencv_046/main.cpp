#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

using namespace cv;
using namespace cv::xfeatures2d;
using namespace std;

CascadeClassifier face_cascade;
CascadeClassifier eye_cascade;
void findFace(Mat frame);

int main()
{
    VideoCapture cap;
    cap.open(0);
    if(!cap.isOpened()){
        cout<<"Failed to open camera"<<endl;
        return -1;
    }

    face_cascade.load("/home/dogpi/Downloads/opencv-4.2.0/data/haarcascades/haarcascade_frontalface_alt.xml");
    eye_cascade.load("/home/dogpi/Downloads/opencv-4.2.0/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml");

    Mat frame;
    char c=0;

    while (cap.read(frame)) {
        c=waitKey(33);
        if(c==27){
            cout<<"close capture"<<endl;
            break;
        }
        findFace(frame);
    }
    waitKey(0);
    return 0;
}

void findFace(Mat frame){
    flip(frame,frame,1);
    Mat gray;
    vector<Rect> faces,eyes;
    cvtColor(frame,gray,COLOR_BGR2GRAY);
    equalizeHist(gray,gray);

    face_cascade.detectMultiScale(gray,faces,1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30));

    for(int i=0;i<faces.size();i++){
        rectangle(frame,faces[i],Scalar(0,255,0),2,8);
        Mat face_rect = gray(faces[i]);
        eye_cascade.detectMultiScale(face_rect,eyes,1.1, 2, 0 |CASCADE_SCALE_IMAGE, Size(20, 20));
        for(int j=0;j<eyes.size();j++){
            rectangle(frame,Rect(faces[i].x+eyes[j].x,faces[i].y+eyes[j].y,eyes[j].width,eyes[j].height),Scalar(0,0,255),2,8);
        }
    }

    imshow("show",frame);
}
