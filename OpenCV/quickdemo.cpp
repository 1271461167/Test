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
static bool checkInRang(int r, int c, int rows, int cols) {
	if (r >= 0 && r < rows && c >= 0 && c < cols)
		return true;
	else
		return false;
}
static void trace(cv::Mat& edgeMag_noMaxsup, cv::Mat& edge, int TL, int r, int c, int rows, int cols) {
	if (edge.at<uchar>(r, c) == 0) {
		edge.at<uchar>(r, c) = 255;
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				int mag = edgeMag_noMaxsup.at<ushort>(r + i, c + j);
				if (checkInRang(r + i, c + j, rows, cols) && mag >= TL)
					trace(edgeMag_noMaxsup, edge, TL, r + i, c + j, rows, cols);
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
Mat QuickDemo::canny_Demo(Mat& image)
{
	Mat src_gray, gauss, gauss1;
	Mat gradx, grady;
	if (image.channels() > 1)
		cvtColor(image, src_gray, COLOR_BGR2GRAY);
	//高斯滤波
	GaussianBlur(src_gray, gauss, Size(5, 5), 0);
	//Canny(gauss, dst, 50, 150);
	//将CV_8UC1转换成CV_16SC1卷积会产生负数
	gauss.convertTo(gauss1, CV_16SC1, 1, 0);
	//梯度计算  Sobel算子
	Mat m1 = (Mat_<char>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);//水平梯度
	Mat m3 = (Mat_<char>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);//竖直梯度
	filter2D(gauss1, gradx, -1, m1);//水平梯度矩阵
	filter2D(gauss1, grady, -1, m3);//竖直梯度矩阵
	Mat grad = Mat::zeros(gradx.size(), CV_16UC1);
	for (int i = 0; i < gradx.cols; i++)
	{
		for (int j = 0; j < gradx.rows; j++)
		{
			grad.at<ushort>(j, i) = cvRound(sqrt(pow(gradx.at<short>(j, i), 2) + pow(grady.at<short>(j, i), 2)));
		}
	}
	//convertScaleAbs(grad, dst);
#pragma region  Sobel算子



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
	//imshow("sobel", abs_grad_x);
#pragma endregion
	//梯度幅值的非极大值抑制
#pragma region 非极大值抑制
	int g1, g2, g3, g4;
	double dTemp1, dTemp2;
	Mat result = Mat::zeros(image.size(), CV_16UC1);
	double weight;
	for (int i = 1; i < image.cols - 1; i++)
	{
		for (int j = 1; j < image.rows - 1; j++)
		{
			if (grad.at<ushort>(j, i) == 0)
			{
				result.at<ushort>(j, i) = 0;
				continue;
			}
			//|gx|>|gy|
			if (abs(gradx.at<short>(j, i)) > abs(grady.at<short>(j, i)))
			{
				weight = (double)abs(grady.at<short>(j, i)) / abs(gradx.at<short>(j, i));
				g2 = grad.at<ushort>(j, i - 1);
				g4 = grad.at<ushort>(j, i + 1);
				//同号
				//	g1
				//	g2	C	g4
				//			g3
				if (gradx.at<short>(j, i) * grady.at<short>(j, i) > 0)
				{
					g1 = grad.at<ushort>(j - 1, i - 1);
					g3 = grad.at<ushort>(j + 1, i + 1);
					dTemp1 = weight * g1 + (1 - weight) * g2;
					dTemp2 = weight * g3 + (1 - weight) * g4;
					if (grad.at<ushort>(j, i) >= dTemp1 && grad.at<ushort>(j, i) >= dTemp2)
					{
						result.at<ushort>(j, i) = grad.at<ushort>(j, i);
					}
					else
					{
						result.at<ushort>(j, i) = 0;
					}
				}
				//异号
				//			g3
				//	g2	C	g4
				//	g1
				else
				{
					g1 = grad.at<ushort>(j + 1, i - 1);
					g3 = grad.at<ushort>(j - 1, i + 1);
					dTemp1 = weight * g1 + (1 - weight) * g2;
					dTemp2 = weight * g3 + (1 - weight) * g4;
					if (grad.at<ushort>(j, i) >= dTemp1 && grad.at<ushort>(j, i) >= dTemp2)
					{
						result.at<ushort>(j, i) = grad.at<ushort>(j, i);
					}
					else
					{
						result.at<ushort>(j, i) = 0;
					}
				}
			}
			//|gx|<=|gy|
			else
			{
				weight = (double)abs(gradx.at<short>(j, i)) / abs(grady.at<short>(j, i));
				g2 = grad.at<ushort>(j - 1, i);
				g4 = grad.at<ushort>(j + 1, i);
				//同号
				//	g1	g2
				//		C	
				//		g4	g3
				if (gradx.at<short>(j, i) * grady.at<short>(j, i) > 0)
				{
					g1 = grad.at<ushort>(j - 1, i - 1);
					g3 = grad.at<ushort>(j + 1, i + 1);
					dTemp1 = weight * g1 + (1 - weight) * g2;
					dTemp2 = weight * g3 + (1 - weight) * g4;
					if (grad.at<ushort>(j, i) >= dTemp1 && grad.at<ushort>(j, i) >= dTemp2)
					{
						result.at<ushort>(j, i) = grad.at<ushort>(j, i);
					}
					else
					{
						result.at<ushort>(j, i) = 0;
					}
				}
				//异号
				//		g2	g1
				//		C	
				//	g3	g4
				else
				{
					g1 = grad.at<ushort>(j - 1, i + 1);
					g3 = grad.at<ushort>(j + 1, i - 1);
					dTemp1 = weight * g1 + (1 - weight) * g2;
					dTemp2 = weight * g3 + (1 - weight) * g4;
					if (grad.at<ushort>(j, i) >= dTemp1 && grad.at<ushort>(j, i) >= dTemp2)
					{
						result.at<ushort>(j, i) = grad.at<ushort>(j, i);
					}
					else
					{
						result.at<ushort>(j, i) = 0;
					}
				}
			}

		}
	}
#pragma endregion
	//双阈值化
#pragma region 双阈值化
	//Mat edge2 = cv::Mat::zeros(image.size(), CV_8UC1);
	//for (int r = 1; r < image.rows - 1; ++r) {
	//	for (int c = 1; c < image.cols - 1; ++c) {
	//		int mag = result.at<ushort>(r, c);
	//		//大于高阈值，为确定边缘点
	//		if (mag >= 120)
	//			trace(result, edge2, 50, r, c, image.rows, image.cols);
	//		else if (mag <= 50)
	//			edge2.at<uchar>(r, c) = 0;
	//	}
	//}
	int TH = 120, TL = 50;
	vector<Point2i> vcGrow;
	Mat edge = Mat::zeros(image.size(), CV_8UC1);
	for (int i = 0; i < edge.rows; i++)
	{
		for (int j = 0; j < edge.cols; j++)
		{
			int value = result.at<ushort>(i, j);
			if (value >= TH)
			{
				edge.at<uchar>(i, j) = 255;
				vcGrow.push_back(Point2i(i, j));
			}
		}
	}
	while (!vcGrow.empty())
	{
		Point2i pt = vcGrow.back();
		vcGrow.pop_back();
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if (checkInRang(pt.x + i, pt.y + j, edge.rows, edge.cols) == false)
					continue;
				if (edge.at<uchar>(pt.x + i, pt.y + j) == 0 && result.at<ushort>(pt.x + i, pt.y + j) >= TL)
				{
					edge.at<uchar>(pt.x + i, pt.y + j) = 255;
					vcGrow.push_back(Point2i(pt.x + i, pt.y + j));
				}
			}
		}
	}

#pragma endregion

	imshow("Gauss", gauss);
	imshow("edge", edge);
	return edge;
}
void QuickDemo::hough_Demo(Mat& image)
{
	if (image.empty())
		return;
	int row = image.rows;
	int col = image.cols;
	double rho=1, theat=1;
	int houghMat_cols = 360 / theat;
	int houghMat_rows = sqrt(col*col+row*row);
	Mat houghMat = Mat::zeros(houghMat_rows, houghMat_cols,CV_16UC1);
	int Threshold = 250;
	vector<Vec2f> lines;
	//vector<Vec2f> lines2;
	//HoughLines(image, lines2,1,CV_PI/180,100,0,0);
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (image.at<uchar>(i, j) != 0)
			{
				for (int k=0;k<360;k+=theat)
				{
					double r = i * sin(k*CV_PI/180) + j * cos(k * CV_PI / 180);
					if (r>=0)
					{
						int R = cvRound(r);
						int T = k / theat;
						houghMat.at<ushort>(R, T) += 1;
					}
				}
			}
		}
	}
	for (int i=0;i< houghMat_rows;i++)
	{
		for (int j=0;j< houghMat_cols;j++)
		{
			if (houghMat.at<ushort>(i, j)> Threshold)
			{
				Vec2f line(i,j*theat*CV_PI/180);
				lines.push_back(line);
			}
		}
	}
	//剔除相近直线
	vector<Vec2f> newLines;
	Vec2f mline = lines[0];
	newLines.push_back(mline);
	for (int i=1;i< lines.size();i++)
	{
		for (int j=0;j<newLines.size();j++)
		{
			if (abs(newLines[j][0] - lines[i][0]) <= 20 && abs(newLines[j][1] - lines[i][1]) <= 20 * CV_PI / 180)
			{						
				break;
			}
			else
			{
				if (j == newLines.size()-1)
				{
					mline = lines[i];
					newLines.push_back(mline);
				}				
			}

		}
	}
	//画直线
	Point pt1, pt2;
	Mat src = imread("E:\\Resources\\0626.jpg");
	for (int i = 0; i < newLines.size(); i++)
	{
		float r = newLines[i][0];
		float th = newLines[i][1];
		double a = cos(th);
		double b = sin(th);
		double x0 = a * r, y0 = b * r;
		double length = max(row, col);
		pt1.x = cvRound(x0 + length * (-b));
		pt1.y = cvRound(y0 + length * (a));
		pt2.x = cvRound(x0 - length * (-b));
		pt2.y = cvRound(y0 - length * (a));
		line(src, pt1, pt2, Scalar(255), 1,LINE_AA);
		imshow("image", src);
	}


}
