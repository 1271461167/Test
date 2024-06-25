#include <opencv2/opencv.hpp>
#include <iostream>
#include "quickdemo.h"
using namespace cv;
using namespace std;
int main(int argc, char** argv)
{
	Mat src = imread("E:\\Resources\\0626.jpg");
	//namedWindow("���봰��", WINDOW_FREERATIO);
	QuickDemo qd;
	Mat image= qd.canny_Demo(src);
	qd.hough_Demo(image);
	//imshow("���봰��", src);
	waitKey(0);
	destroyAllWindows();
	return 0;
}