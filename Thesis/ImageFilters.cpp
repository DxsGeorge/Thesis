#include "ImageFilters.h"

using namespace cv;
using namespace std;

Mat FilterImage(Mat &src)
{
	bool laplace = false;
	if (laplace)
	{
		flip(src, src, 1);
		Mat src1;
		src.copyTo(src1);
		pyrDown(src1, src1, Size(src.cols / 2, src.rows / 2));
		pyrDown(src, src, Size(src.cols / 2, src.rows / 2));
		if (src1.channels() == 3) cvtColor(src1, src1, CV_BGR2GRAY);
		Mat dst(src.cols, src.rows, CV_8UC1);
		Mat dst1(src.cols, src.rows, CV_8UC1);
		GaussianBlur(src1, src1, Size(7, 7), 0, 0);
		Laplacian(src1, dst, CV_16SC1, 3);
		compare(dst, 8, dst, CV_CMP_GT);
		compare(src1, 100, dst1, CV_CMP_LT);
		bitwise_and(dst, dst1, dst);
		convertScaleAbs(dst, dst, 1, 0);
		Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
		//erode(dst,dst,kernel);
		//dilate(dst,dst,kernel);
		dilate(dst, dst, kernel);
		erode(dst, dst, kernel);
		GaussianBlur(dst, dst, Size(3, 3), 0, 0);
		return dst;
	}
	else if (1)
	{
		flip(src, src, 1);
		Mat src1;
		src.copyTo(src1);
		pyrDown(src1, src1, Size(src.cols / 2, src.rows / 2));
		pyrDown(src, src, Size(src.cols / 2, src.rows / 2));
		cvtColor(src, src1, CV_BGR2GRAY);
		Mat dst(src.cols, src.rows, CV_8UC1);
		Mat dst1(src.cols, src.rows, CV_8UC1);
		GaussianBlur(src1, src1, Size(3, 3), 0, 0);
		Laplacian(src1, dst, CV_16SC1, 3);
		compare(dst, 8, dst, CV_CMP_GT);
		compare(src1, 100, dst1, CV_CMP_LT);
		bitwise_and(dst, dst1, dst);
		convertScaleAbs(dst, dst, 1, 0);
		return dst;

	}
	else
	{
		flip(src, src, 1);
		Mat src1;
		src.copyTo(src1);
		pyrDown(src1, src1, Size(src.cols / 2, src.rows / 2));
		pyrDown(src, src, Size(src.cols / 2, src.rows / 2));
		Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
		cvtColor(src, src1, CV_BGR2GRAY);
		//GaussianBlur(src1, src1, Size(3,3), 0, 0);
		Mat dst(src.cols, src.rows, CV_8UC1);
		Canny(src1, dst, 15, 45, 3, true);
		
		dilate(dst, dst, element);
		/*
		erode(dst, dst, element);
		dilate(dst, dst, element);
		dilate(dst, dst, element);
		dilate(dst, dst, element);
		erode(dst, dst, element);
		erode(dst, dst, element);
		erode(dst, dst, element);
		//erode(dst, dst, element);
		//erode(dst, dst, element);
		//GaussianBlur(dst, dst, Size(5, 5), 0, 0);
		*/
		return dst;
	}

}