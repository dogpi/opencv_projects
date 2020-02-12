#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{

    // 加载图片
    Mat src = imread("/home/dogpi/Pictures/samples/cards.png");
    if( src.empty() )
    {
        cout << "Could not open or find the image!\n" << endl;
        return -1;
    }
    imshow("Source Image", src);

    // 将图片背景色转为黑色
    for(int col=0;col<src.cols;col++){
        for(int row=0;row<src.rows;row++){
            if(src.at<Vec3b>(row,col)==Vec3b(255,255,255)){
                src.at<Vec3b>(row,col) = Vec3b(0,0,0);
            }
        }
    };
    imshow("black background",src);

    // 提高对比度，使用filter2D 与拉普拉斯算子
    Mat kernel_sharpen = (Mat_<float>(3,3)<<
                          1,1,1,
                          1,-8,1,
                          1,1,1);// 这个算子会将大片颜色近似的区域值变为0，只凸显颜色落差打的地方
                                 // 很显然，白色与红色的边界会消除红色通道，因而边界是前两个通道的组合为青色
                                 // （你自己试一下Scalar(255,255,0)）
    Mat imageLaplacian;
    filter2D(src,imageLaplacian,CV_32F,kernel_sharpen);
    imshow("imageLaplacian",imageLaplacian);
    Mat sharp;
    src.convertTo(sharp,CV_32F);
    // 这里会发现为什么图像转换成32位的float类型后为什么会不清晰了呢，
    // 是因为double/float数据类型图像的范围是0~1
    // 所以要先锐化，提高边缘的值，不然边缘就不会清晰了。
    imshow("sharp",sharp);
    // 原图与锐化后的结果 得到的结果大多是边缘 也可以认为是梯度
    Mat imageResut = sharp-imageLaplacian;
    imageResut.convertTo(imageResut,CV_32F);
    //为什么不清晰的sharp减去imageLaplacian后得到的imageResult又清晰了呢？
    //因为在filter2D使用kernel_sharpen算子计算后，在红色与白色交接处的值为前两个通道值较大，红色通道值几乎为0。
    //sharp值减去imageLaplacian的黑色区域后值保持不变，也就是白色。
    // 白色减去边缘处，由于前两个通道值较大，第三个通道值特小，所以会凸显为红色
    imshow("sharp-imageLaplacian",imageResut);

    // 重新将图像转换为CV_8UC3才能使用cvtColor
    imageResut.convertTo(imageResut,CV_8UC3);
    imageLaplacian.convertTo(imageLaplacian,CV_8UC3);

    // 转为二值图像方便距离变换
    Mat binary_mat;
    cvtColor(imageResut,binary_mat,COLOR_BGR2GRAY);
    threshold(binary_mat,binary_mat,40,255,THRESH_BINARY|THRESH_OTSU);//自动计算阈值
    imshow("binary mat",binary_mat);

    // 距离变换
    Mat distance;
    distanceTransform(binary_mat,distance,DIST_L2,3);// 3为maskSize

    normalize(distance,distance,0,1.0,NORM_MINMAX);
    imshow("distance tramsform",distance);

    // 使用阈值处理，保存相应的点
    threshold(distance,distance,0.4,1.0,THRESH_BINARY);

    // 膨胀操作
    Mat kernel_ones = Mat::ones(3,3,CV_8U);
    erode(distance,distance,kernel_ones);
    imshow("peaks",distance);

    // 转为单通道，方便计算
    Mat distance_8U;
    distance.convertTo(distance_8U,CV_8U);

    // 发现轮廓
    vector<vector<Point>> contours;
    findContours(distance_8U,contours,RETR_TREE,CHAIN_APPROX_SIMPLE);

    // 绘制轮廓并填充，显示的结果与distance相同
    // 填充的颜色值会有1的差距
    // 如果要显示就不能使用CV_32S
    Mat markers = Mat::zeros(distance.size(),CV_16S);
    for(size_t i=0;i<contours.size();i++){
        drawContours(markers,contours,(int)i,Scalar((i+1)),-1);
    }

    circle(markers,Point(5,5),3,Scalar(255),-1);
    imshow("markers",markers*10000);

    // 分水岭变换
    // watershed中输出必须是CV_32S
    markers.convertTo(markers,CV_32S);
    watershed(imageResut,markers);// 图像边缘做分水岭变换


    // 显示分水岭变换图
    Mat mark;
    markers.convertTo(mark,CV_8U);
    bitwise_not(mark,mark);
    imshow("watershed",mark);

    Mat dst = Mat::zeros(markers.size(), CV_8UC3);

    vector<Vec3b> colors;
    for (size_t i = 0; i < contours.size(); i++)
    {
        int b = theRNG().uniform(0, 256);
        int g = theRNG().uniform(0, 256);
        int r = theRNG().uniform(0, 256);

        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }

     // Fill labeled objects with random colors
    // 因为markers中的轮廓都用值填充了，值分别为1,2...contours.size()
    for (int i = 0; i < markers.rows; i++)
    {
        for (int j = 0; j < markers.cols; j++)
        {
            // 值为1的取第一种颜色填充，值为2的取第二种颜色填充。
            int index = markers.at<int>(i,j);
            if (index > 0 && index <= static_cast<int>(contours.size()))
            {
                dst.at<Vec3b>(i,j) = colors[index-1];
            }
        }
    }
    imshow("Final Result", dst);

    waitKey();
    return 0;
}
