#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat img(Size(500,500),CV_8UC3);
    RNG rng(time(NULL));

    Scalar colorTable[] = {
        Scalar(0,0,255),
        Scalar(0,255,0),
        Scalar(255,0,0),
        Scalar(0,255,255),
        Scalar(255,255,0)
    };

    // 生成的点数
    int sample_count = rng.uniform(5,1000);
    // 数据集 sample_count行，单列，双通道。相当于sample_count行 2列
    Mat points(sample_count,1,CV_32FC2);

    // 由于是模拟，所以我们根据预定的类数在指定的几个区域生成随机点
    // 生成样本
    int num_class = rng.uniform(2,5);
    for(int k=0;k<num_class;k++){
        // 围绕这个中心生成样本点
        Point center;
        center.x = rng.uniform(0,img.cols);
        center.y = rng.uniform(0,img.rows);
        // 获取指针
        // 把points分为k段，每次获取一段
        Mat point = points.rowRange(k*(sample_count/num_class),k==num_class-1?sample_count:(k+1)*(sample_count/num_class));
        // 给points中的k段中的每段赋值，赋值为正动态分布，均值为center.x,center,y方差为img.cols*0.05,img.rows*0.95
        rng.fill(point,RNG::NORMAL,Scalar(center.x,center.y),Scalar(img.cols*0.05,img.rows*0.05));
    }
    // 对point中的数据进行随机交换位置
    randShuffle(points,1,&rng);

    Mat labels;
    Mat centers;
    // 终止条件
    TermCriteria t = TermCriteria(TermCriteria::EPS+TermCriteria::COUNT,10,0.1);
    kmeans(points,num_class,labels,t,3,KMEANS_PP_CENTERS,centers);

    // 背景为白色
    // 显示数据点
    img = Scalar::all(255);
    for(int i=0;i<sample_count;i++){
        int index = labels.at<int>(i);
        Point p = points.at<Point2f>(i);
        circle(img,p,2,colorTable[index],-1,0);
    }

    imshow("Kmeans",img);

    waitKey(0);
    return 0;
}
