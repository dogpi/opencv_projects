#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace cv::ml;
using namespace std;

int main()
{
    Scalar colorTable[] = {
        Scalar(0,0,255),
        Scalar(0,255,0),
        Scalar(255,0,0),
        Scalar(0,255,255),
        Scalar(255,255,0)
    };
    RNG rng(1234);
    Mat mat = Mat::zeros(500,500,CV_8UC3);
    // 按照某些规则生成随机点
    int sample_count = rng.uniform(5,1000);// 随机生成的点数
    int sample_cluster = rng.uniform(2,5);// 随机点的类数
    Mat points(sample_count,2,CV_32FC1);  // sample_count行2列
    // 给points赋值
    for(int k = 0;k<sample_cluster;k++){
        Point center;
        center.x = rng.uniform(0,mat.cols);
        center.y = rng.uniform(0,mat.rows);
        Mat pointBlock = points.rowRange(k*sample_count/sample_cluster,k==sample_cluster-1?sample_count:(k+1)*sample_count/sample_cluster);
        rng.fill(pointBlock,RNG::NORMAL,Scalar(center.x,center.y),Scalar(mat.cols*0.05,mat.rows*0.05));
    }
    randShuffle(points,1,&rng);

    // GMM高斯混合模型分类
    Mat labels;
    Ptr<EM> em_model = EM::create();
    em_model->setClustersNumber(sample_cluster);
    em_model->setCovarianceMatrixType(EM::COV_MAT_SPHERICAL);
    em_model->setTermCriteria(TermCriteria(TermCriteria::EPS+TermCriteria::COUNT,100,0.1));
    em_model->trainEM(points,noArray(),labels,noArray());

    // 画出分类后的点
    Mat sample(1,2,CV_32FC1);// 与points的类型相同
    for(int row=0;row<mat.rows;row++){
        for(int col=0;col<mat.cols;col++){
            sample.at<float>(0) = (float)col;
            sample.at<float>(1) = (float)row;
            int predict_num = cvRound(em_model->predict2(sample,noArray())[1]);
            Scalar color = colorTable[predict_num];
            circle(mat,Point(col,row),1,color*0.75,-1);
        }
    }
    imshow("GMM",mat);

    waitKey(0);
    return 0;
}
