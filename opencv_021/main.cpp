#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat src;
    src = imread("/home/dogpi/Pictures/samples/lena.jpg");
    if(src.empty()){
        return -1;
    }
    imshow("lena",src);

    // 分通道计算
    vector<Mat> bgr_mats;
    split(src,bgr_mats);
    imshow("b mat",bgr_mats[0]);
    imshow("g mat",bgr_mats[1]);
    imshow("r mat",bgr_mats[2]);

    Mat b_hist,g_hist,r_hist;
    const int histsize = 256;
    float range[] = {0,255};
    const float * histrange = range;
    calcHist(&bgr_mats[0],1,0,Mat(),b_hist,1,&histsize,&histrange,true,false);
    calcHist(&bgr_mats[1],1,0,Mat(),g_hist,1,&histsize,&histrange,true,false);
    calcHist(&bgr_mats[2],1,0,Mat(),r_hist,1,&histsize,&histrange,true,false);

    int hist_h = 400;
    int hist_w = 512;
    int bin_w = hist_w/histsize;
    Mat dst(hist_w,hist_h,CV_8UC3,Scalar(0,0,0));
    //  归一化，防止直方图超过窗口大小
    normalize(b_hist,b_hist,0,hist_h,NORM_MINMAX,-1);
    normalize(g_hist,g_hist,0,hist_h,NORM_MINMAX,-1);
    normalize(r_hist,r_hist,0,hist_h,NORM_MINMAX,-1);

    for(int i=1;i<histsize;i++){
        line(dst,Point((i-1)*bin_w,hist_h-cvRound(b_hist.at<float>(i-1))),
                 Point((i)*bin_w,hist_h-cvRound(b_hist.at<float>(i))),Scalar(255,0,0),2,LINE_AA);
        line(dst,Point((i-1)*bin_w,hist_h-cvRound(g_hist.at<float>(i-1))),
                 Point((i)*bin_w,hist_h-cvRound(g_hist.at<float>(i))),Scalar(0,255,0),2,LINE_AA);
        line(dst,Point((i-1)*bin_w,hist_h-cvRound(r_hist.at<float>(i-1))),
                 Point((i)*bin_w,hist_h-cvRound(r_hist.at<float>(i))),Scalar(0,0,255),2,LINE_AA);
    }
    imshow("dst",dst);


    waitKey(0);
    return 0;
}
/*

void cv::normalize 	( 	InputArray  	src,
        InputOutputArray  	dst,
        double  	alpha = 1,
        double  	beta = 0,
        int  	norm_type = NORM_L2,
        int  	dtype = -1,
        InputArray  	mask = noArray()
    )
Python:
    dst	=	cv.normalize(	src, dst[, alpha[, beta[, norm_type[, dtype[, mask]]]]]	)

#include <opencv2/core.hpp>

Normalizes the norm or value range of an array.

The function cv::normalize normalizes scale and shift the input array elements so that

∥dst∥Lp=alpha

(where p=Inf, 1 or 2) when normType=NORM_INF, NORM_L1, or NORM_L2, respectively; or so that

minIdst(I)=alpha,maxIdst(I)=beta

when normType=NORM_MINMAX (for dense arrays only). The optional mask specifies a sub-array to be normalized. This means that the norm or min-n-max are calculated over the sub-array, and then this sub-array is modified to be normalized. If you want to only use the mask to calculate the norm or min-max but modify the whole array, you can use norm and Mat::convertTo.

In case of sparse matrices, only the non-zero values are analyzed and transformed. Because of this, the range transformation for sparse matrices is not allowed since it can shift the zero level.

Possible usage with some positive example data:
vector<double> positiveData = { 2.0, 8.0, 10.0 };
vector<double> normalizedData_l1, normalizedData_l2, normalizedData_inf, normalizedData_minmax;
// Norm to probability (total count)
// sum(numbers) = 20.0
// 2.0      0.1     (2.0/20.0)
// 8.0      0.4     (8.0/20.0)
// 10.0     0.5     (10.0/20.0)
normalize(positiveData, normalizedData_l1, 1.0, 0.0, NORM_L1);
// Norm to unit vector: ||positiveData|| = 1.0
// 2.0      0.15
// 8.0      0.62
// 10.0     0.77
normalize(positiveData, normalizedData_l2, 1.0, 0.0, NORM_L2);
// Norm to max element
// 2.0      0.2     (2.0/10.0)
// 8.0      0.8     (8.0/10.0)
// 10.0     1.0     (10.0/10.0)
normalize(positiveData, normalizedData_inf, 1.0, 0.0, NORM_INF);
// Norm to range [0.0;1.0]
// 2.0      0.0     (shift to left border)
// 8.0      0.75    (6.0/8.0)
// 10.0     1.0     (shift to right border)
normalize(positiveData, normalizedData_minmax, 1.0, 0.0, NORM_MINMAX);

Parameters
    src	input array.
    dst	output array of the same size as src .
    alpha	norm value to normalize to or the lower range boundary in case of the range normalization.
    beta	upper range boundary in case of the range normalization; it is not used for the norm normalization.
    norm_type	normalization type (see cv::NormTypes).
    dtype	when negative, the output array has the same type as src; otherwise, it has the same number of channels as src and the depth =CV_MAT_DEPTH(dtype).
    mask	optional operation mask.
*/
