#include <opencv2/opencv.hpp>
#include <iostream>
#include "quickdemo.h"
using namespace cv;
using namespace std;
int main(int argc,char** argv)
{
	Mat src = imread("D:\\source\\cat.jpg");
	//namedWindow("���봰��", WINDOW_FREERATIO);
	QuickDemo qd;
	qd.video_Demo();
	//imshow("���봰��", src);
	waitKey(0);
	destroyAllWindows();
	return 0;
}