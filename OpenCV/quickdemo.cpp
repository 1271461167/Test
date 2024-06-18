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
		if (w >= 960 && w <= 3840)
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
static void RegionGrow(Mat& src, Mat& dst, Point2i pt, int th)
{
	dst = Mat::zeros(src.size(), CV_8UC1);		//创建一个空白区域填充为黑色
	dst.at<uchar>(pt.y, pt.x) = 255;			//标记生长点
	int nGrowLabel = 0;							//标记是否生长过
	int nSrcValue = src.at<uchar>(pt.y, pt.x);	//生长起点灰度值
	int nCurValue = 0;							//当前点灰度值
	Point2i ptGrowing;							//待生长位置
	int DIR[8][2] = { {-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1} };//生长方向
	vector<Point2i> vcGrowpt;					//生长点栈
	vcGrowpt.push_back(pt);						//生长起始点入栈
	while (!vcGrowpt.empty())
	{
		pt = vcGrowpt.back();
		vcGrowpt.pop_back();
		for (int i = 0; i < 8; i++)
		{
			ptGrowing.x = pt.x + DIR[i][0];
			ptGrowing.y = pt.y + DIR[i][1];
			if (ptGrowing.x < 0 || ptGrowing.y < 0 || ptGrowing.x >(src.cols - 1) || (ptGrowing.y > src.rows - 1))
				continue;
			nGrowLabel = dst.at<uchar>(ptGrowing.y, ptGrowing.x);
			if (nGrowLabel == 0)
			{
				nCurValue = src.at<uchar>(ptGrowing.y, ptGrowing.x);
				if (abs(nCurValue - nSrcValue) <= th)
				{
					vcGrowpt.push_back(ptGrowing);
					dst.at<uchar>(ptGrowing.y, ptGrowing.x) = 255;
				}


			}


		}

	}


}
static void onGrow(int event, int x, int y, int flags, void* userdata)
{
	Mat pic = *((Mat*)userdata);
	Mat src_gray, dst, gauss;
	if (pic.channels() > 1)
		cv::cvtColor(pic, src_gray, COLOR_BGR2GRAY);
	Point2i pt;
	char str[16];
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
	{
		pt = Point2i(x, y);
		cout << "(x,y)=" << "(" << x << "," << y << ")" << endl;
	}
	break;
	case EVENT_LBUTTONUP:
	{
		circle(pic, Point2i(x, y), 1, Scalar(0, 0, 255), -1);
		sprintf_s(str, "(%d,%d)", x, y);
		namedWindow("dst", WINDOW_AUTOSIZE);
		pt = Point2i(x, y);
		GaussianBlur(src_gray, gauss, Size(5, 5), 0);
		RegionGrow(gauss, dst, pt, 30);
		bitwise_and(gauss, dst, dst);
		imshow("dst", dst);
		imshow("测试窗口", pic);
	}
	default:
		break;
	}
}
void QuickDemo::mouseDrawing_Demo(Mat& image)
{
	namedWindow("鼠标绘制", WINDOW_AUTOSIZE);
	setMouseCallback("鼠标绘制", onMouse, (void*)&image);
	imshow("鼠标绘制", image);
}
void QuickDemo::histogram_Demo(Mat& image)
{
	vector<Mat> bgr_plane;
	//分离BGR通道
	split(image, bgr_plane);
	const int channels[] = { 0 };
	const int bins[] = { 256 };
	const float hranges[] = { 0,255 };
	const float* ranges[] = { hranges };
	Mat b_hist;
	Mat g_hist;
	Mat r_hist;
	//计算直方图
	calcHist(&bgr_plane[0], 1, 0, Mat(), b_hist, 1, bins, ranges);
	calcHist(&bgr_plane[1], 1, 0, Mat(), g_hist, 1, bins, ranges);
	calcHist(&bgr_plane[2], 1, 0, Mat(), r_hist, 1, bins, ranges);

	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / bins[0]);
	Mat histImage = Mat::zeros(hist_h, hist_w, CV_8UC3);
	//归一化
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	//绘制直方图
	for (int i = 1; i < bins[0]; i++)
	{
		line(histImage, Point((i - 1) * bin_w, hist_h - cvRound(b_hist.at<float>(i - 1))), Point(i * bin_w, hist_h - cvRound(b_hist.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point((i - 1) * bin_w, hist_h - cvRound(g_hist.at<float>(i - 1))), Point(i * bin_w, hist_h - cvRound(g_hist.at<float>(i))), Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point((i - 1) * bin_w, hist_h - cvRound(r_hist.at<float>(i - 1))), Point(i * bin_w, hist_h - cvRound(r_hist.at<float>(i))), Scalar(0, 0, 255), 2, 8, 0);
	}
	imshow("BGR直方图", histImage);

}
void QuickDemo::video_Demo()
{
	VideoCapture capture(0);
	Mat frame;
	while (true)
	{
		capture.read(frame);
		if (frame.empty())
			break;
		flip(frame, frame, 1);
		imshow("摄像头", frame);

		int c = waitKey(10);
		if (c == 27)
			break;
	}
	capture.release();
}
void QuickDemo::region_Grow(Mat& image)
{
	namedWindow("测试窗口", WINDOW_AUTOSIZE);
	setMouseCallback("测试窗口", onGrow, (void*)&image);
	imshow("测试窗口", image);
}
void QuickDemo::canny_Demo(Mat& image)
{
	Mat dst, src_gray, gauss,gauss1;
	dst = Mat::zeros(image.size(), CV_16SC1);
	Mat M1, M3;
	if (image.channels() > 1)
		cvtColor(image, src_gray, COLOR_BGR2GRAY);
	//高斯滤波
	GaussianBlur(src_gray, gauss, Size(5, 5), 0);
	//将CV_8UC1转换成CV_16SC1卷积会产生负数
	gauss.convertTo(gauss1,CV_16SC1,1,0);
	//梯度计算  Sobel算子
	Mat m1 = (Mat_<char>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);//水平梯度
	Mat m3 = (Mat_<char>(3, 3) << 1, 0, -1, 2, 0, -2, 1, 0, -1);//竖直梯度
	filter2D(gauss1, M1, -1, m1);//水平梯度矩阵
	filter2D(gauss1, M3, -1, m3);//竖直梯度矩阵
	Mat grad = Mat::zeros(M1.size(), CV_16UC1);
	for (int i=0;i<M1.cols;i++)
	{
		for (int j = 0; j < M1.rows; j++)
		{
			grad.at<ushort>(j, i) = cvRound(sqrt(pow(M1.at<short>(j, i), 2)+pow(M3.at<short>(j, i), 2)));
		}
	}

		//Mat grad_x, grad_y;
		//Mat abs_grad_x, abs_grad_y;
		////    计算x方向的梯度
		//Sobel(gauss, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
		//convertScaleAbs(grad_x, abs_grad_x);

		////    计算y方向的梯度
		//Sobel(gauss, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
		//convertScaleAbs(grad_y, abs_grad_y);

		////    合并梯度
		//Mat dstImage;
		//addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, dstImage);
		////imshow("sobel", abs_grad_x);

	//梯度幅值的非极大值抑制
	int g1, g2, g3, g4;
	double weight;
	double dTemp, dTemp1, dTemp2;
	
	imshow("dst", gauss);
}
