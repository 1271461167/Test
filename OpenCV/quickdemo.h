#pragma once
#include <opencv2/opencv.hpp>
using namespace cv;
class  QuickDemo
{
public:
	void colorSpace_Demo(Mat& image);
	void mouseDrawing_Demo(Mat& image);
	void histogram_Demo(Mat& image);
	void video_Demo();
	void region_Grow(Mat& image);
	void canny_Demo(Mat& image);
private:

};
