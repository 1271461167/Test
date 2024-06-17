#include <opencv2/opencv.hpp>
#include <iostream>
#include "quickdemo.h"
using namespace cv;
using namespace std;
int main(int argc, char** argv)
{
	Mat src = imread("E:\\Resources\\test.jpg");
	//namedWindow("输入窗口", WINDOW_FREERATIO);
	QuickDemo qd;
	qd.canny_Demo(src);
	//imshow("输入窗口", src);
	waitKey(0);
	destroyAllWindows();
	return 0;
}