#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace cv::ml;
using namespace std;

int main()
{
    Mat src = imread("/home/dogpi/Pictures/samples/card_photo.png");
    if(src.empty()){
        cout<<"Failed to load picture"<<endl;
        return -1;
    }
    imshow("Source",src);


//    Mat kernel = (Mat_<char>(3,3) << 0,-1,0,-1,5,-1,0,-1,0);
//    filter2D(src,src,src.depth(),kernel);

    // 把图像数据输入到样本空间
    Mat points(src.cols*src.rows,3,CV_32F);
    for(int row=0;row<src.rows;row++){
        for(int col=0;col<src.cols;col++){
            points.at<float>(row*col+col,0) = src.at<Vec3b>(row,col)[0];
            points.at<float>(row*col+col,1) = src.at<Vec3b>(row,col)[1];
            points.at<float>(row*col+col,2) = src.at<Vec3b>(row,col)[2];
        }
    }
    Scalar colorTable[] = {
        Scalar(0,0,255),
        Scalar(0,255,0),
        Scalar(255,0,0),
        Scalar(0,255,255),
        Scalar(255,255,0)
    };
    // GMM训练分类
    int sample_cluster = 3;
    Mat labels;
    Ptr<EM> em_model = EM::create();
    em_model->setClustersNumber(sample_cluster);
    em_model->setCovarianceMatrixType(EM::COV_MAT_SPHERICAL);
    em_model->setTermCriteria(TermCriteria(TermCriteria::EPS+TermCriteria::COUNT,100,0.1));
    em_model->trainEM(points,noArray(),labels,noArray());

    //绘制分类后的结果
    Mat dst(src.size(),src.type());
    Vec3f sample;
    for(int row=0;row<src.rows;row++){
        for(int col=0;col<src.cols;col++){
            sample = src.at<Vec3b>(row,col);
            int index = cvRound(em_model->predict2(sample,noArray())[1]);
            circle(dst,Point(col,row),1,colorTable[index],1,-1);
        }
    }
    imshow("dst",dst);

    waitKey(0);
    return 0;
}
