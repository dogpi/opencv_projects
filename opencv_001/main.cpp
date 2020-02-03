#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
    Mat src = imread("/home/dogpi/Downloads/opencv-4.2.0/samples/data/lena.jpg");
    if(src.empty()){
        cout<<"read file fail"<<endl;
        return -1;
    }
    namedWindow("input image window",WINDOW_AUTOSIZE);
    namedWindow("out image window",WINDOW_AUTOSIZE);
    namedWindow("out image window2",WINDOW_AUTOSIZE);
    imshow("input image window",src);


    // 掩膜操作（提高对比度）
    // 图像为三通道，使用二维数组表示。
    // 每行有x个像素点，每个像素点有三个数值，所以每行有x*channels列
    // offsetx为通道数，当前位置加上offset就是该行的下一个像素相应通道的值
    // 掩膜操作就是把中心元素的值(c)与周围四个元素(x,y,z,v)的差，加到中心元素上，
    // 就是c+c-y+c-x+c-z+c-v=5c-(x+y+z+v)
    int cols = src.cols * src.channels();
    int rows = src.rows;
    int offsetx = src.channels();
    Mat dst(src.size(),src.type());// 这里要对dst进行初始化，否则不显示。

    double t = getTickCount();
    // 因为要从第二行开始，第一个的像素没有4个相邻的元素
    for(int row =1 ; row<(rows-1);row++){
        // 使用指针访问Mat中的数据
        // 按行访问
        const uchar* precious = src.ptr<uchar>(row-1);
        const uchar* current = src.ptr<uchar>(row);
        const uchar* next = src.ptr<uchar>(row+1);
        uchar* output = dst.ptr<uchar>(row);
        for(int col =offsetx;col<(cols-1);col++){
            // 使用saturate_cast<uchar>对溢出的值进行饱和转换
            output[col] = saturate_cast<uchar>(
                        5*current[col]-(
                            current[col-offsetx]+
                            current[col+offsetx]+
                            precious[col]+next[col]));
        }
    }
    double timecount = (getTickCount()-t)/getTickFrequency();
    cout<<"spend time: "<<timecount<<endl;
    imshow("out image window",dst);

    double t2 = getTickCount();
    // 上面是手动操作Mat中的数值进行掩膜操作，还可以直接使用函数进行掩膜操作
    Mat kernel = (Mat_<char>(3,3) << 0,-1,0,-1,5,-1,0,-1,0);
    filter2D(src,dst,src.depth()/* -1(深度自动与输入图像一致) */,kernel);
    /*  filter2D
        Convolves an image with the kernel.
        The function applies an arbitrary linear filter to an image. In-place operation is supported. When the aperture is partially outside the image, the function interpolates outlier pixel values according to the specified border mode.
        The function does actually compute correlation, not the convolution:
        \texttt{dst} (x,y) = \sum _{ \stackrel{0\leq x' < \texttt{kernel.cols},}{0\leq y' < \texttt{kernel.rows}} } \texttt{kernel} (x',y')* \texttt{src} (x+x'- \texttt{anchor.x} ,y+y'- \texttt{anchor.y} )
        That is, the kernel is not mirrored around the anchor point. If you need a real convolution, flip the kernel using flip and set the new anchor to (kernel.cols - anchor.x - 1, kernel.rows - anchor.y - 1).
        The function uses the DFT-based algorithm in case of sufficiently large kernels (~11 x 11 or larger) and the direct algorithm for small kernels.
        Parameters
            src	input image.
            dst	output image of the same size and the same number of channels as src.
            ddepth	desired depth of the destination image, see combinations
            kernel	convolution kernel (or rather a correlation kernel), a single-channel floating point matrix; if you want to apply different kernels to different channels, split the image into separate color planes using split and process them individually.
            anchor	anchor of the kernel that indicates the relative position of a filtered point within the kernel; the anchor should lie within the kernel; default value (-1,-1) means that the anchor is at the kernel center.
            delta	optional value added to the filtered pixels before storing them in dst.
            borderType	pixel extrapolation method, see BorderTypes
    */
    timecount = (getTickCount()-t2)/getTickFrequency();
    cout<<"spend time: "<<timecount<<endl;
    imshow("out image window2",dst);
    // 有消耗的时间可以得出，opencv提供的函数速度更快计算出掩膜后的图像
    waitKey(0);
    return 0;
}
