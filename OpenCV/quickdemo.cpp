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
			imshow("������", image);
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
	dst = Mat::zeros(src.size(), CV_8UC1);		//����һ���հ��������Ϊ��ɫ
	dst.at<uchar>(pt.y, pt.x) = 255;			//���������
	int nGrowLabel = 0;							//����Ƿ�������
	int nSrcValue = src.at<uchar>(pt.y, pt.x);	//�������Ҷ�ֵ
	int nCurValue = 0;							//��ǰ��Ҷ�ֵ
	Point2i ptGrowing;							//������λ��
	int DIR[8][2] = { {-1,-1},{-1,0},{-1,1},{0,1},{1,1},{1,0},{1,-1},{0,-1} };//��������
	vector<Point2i> vcGrowpt;					//������ջ
	vcGrowpt.push_back(pt);						//������ʼ����ջ
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
	Mat src_gray, dst;
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
		RegionGrow(src_gray, dst, pt, 40);
		imshow("dst", dst);
		imshow("���Դ���", pic);
	}
	default:
		break;
	}
}
void QuickDemo::mouseDrawing_Demo(Mat& image)
{
	namedWindow("������", WINDOW_AUTOSIZE);
	setMouseCallback("������", onMouse, (void*)&image);
	imshow("������", image);
}
void QuickDemo::histogram_Demo(Mat& image)
{
	vector<Mat> bgr_plane;
	//����BGRͨ��
	split(image, bgr_plane);
	const int channels[] = { 0 };
	const int bins[] = { 256 };
	const float hranges[] = { 0,255 };
	const float* ranges[] = { hranges };
	Mat b_hist;
	Mat g_hist;
	Mat r_hist;
	//����ֱ��ͼ
	calcHist(&bgr_plane[0], 1, 0, Mat(), b_hist, 1, bins, ranges);
	calcHist(&bgr_plane[1], 1, 0, Mat(), g_hist, 1, bins, ranges);
	calcHist(&bgr_plane[2], 1, 0, Mat(), r_hist, 1, bins, ranges);

	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / bins[0]);
	Mat histImage = Mat::zeros(hist_h, hist_w, CV_8UC3);
	//��һ��
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	//����ֱ��ͼ
	for (int i = 1; i < bins[0]; i++)
	{
		line(histImage, Point((i - 1) * bin_w, hist_h - cvRound(b_hist.at<float>(i - 1))), Point(i * bin_w, hist_h - cvRound(b_hist.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point((i - 1) * bin_w, hist_h - cvRound(g_hist.at<float>(i - 1))), Point(i * bin_w, hist_h - cvRound(g_hist.at<float>(i))), Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point((i - 1) * bin_w, hist_h - cvRound(r_hist.at<float>(i - 1))), Point(i * bin_w, hist_h - cvRound(r_hist.at<float>(i))), Scalar(0, 0, 255), 2, 8, 0);
	}
	imshow("BGRֱ��ͼ", histImage);

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
		imshow("����ͷ", frame);

		int c = waitKey(10);
		if (c == 27)
			break;
	}
	capture.release();
}
void QuickDemo::region_Grow(Mat& image)
{
	namedWindow("���Դ���", WINDOW_AUTOSIZE);
	setMouseCallback("���Դ���", onGrow, (void*)&image);
	imshow("���Դ���", image);
}
