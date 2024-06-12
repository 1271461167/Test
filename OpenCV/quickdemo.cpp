#include "quickdemo.h"
#include <iostream>
using namespace cv;
using namespace std;
void QuickDemo::colorSpace_Demo(Mat& image)
{
	Mat gray;
	cvtColor(image, gray, COLOR_BGR2GRAY);
	imshow("input", gray);
}
static void onMouse(int event, int x, int y, int flags, void* userdata)
{
	Mat pic = *((Mat*)userdata);
	Mat image;
	static int h = pic.rows;
	static int w = pic.cols;
	if (event == EVENT_MOUSEWHEEL)
	{
		int value = getMouseWheelDelta(flags);
		if (w>=960&&w<=3840)
		{
			h += 9 * (value / 120);
			w += 16 * (value / 120);
			resize(pic, image, Size(w, h), 0, 0, INTER_LINEAR);
			cout << value << endl;
			imshow("鼠标绘制", image);
		}
		else 
		{
			if (w < 960)
			{
				w = 960;
				h = 540;
			}
			if (w > 3840)
			{
				w = 3840;
				h = 2160;
			}
		}
	}


}

void QuickDemo::mouseDrawing_Demo(Mat& image)
{
	namedWindow("鼠标绘制", WINDOW_AUTOSIZE);
	setMouseCallback("鼠标绘制", onMouse, (void*)&image);
	imshow("鼠标绘制", image);
}
