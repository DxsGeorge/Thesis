// Thesis.cpp : Defines the entry point for the console application.
//



#include "stdafx.h"
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/photo/photo.hpp"
#include "ImageFilters.h"
#include "FeatureDetector.h"
#include "Line.h"
#include "FaceFinder.h"
#include "Hough_Transform.h"
#include "lk-tracking.h"
#include "Input.h"
#include <iostream>
#include <string>
#include <Windows.h>

using namespace cv;
using namespace std;



int main()
{
	namedWindow("Video", 1);
	namedWindow("Blob", 1);
	namedWindow("Gray", 1);
	VideoCapture cap(1);
	int tracking_mode = 0;
	bool of;
	Mat prev_dst;
	Mat dst;
	Mat gray, prev_gray;
	vector<Point> points;
	vector<KeyPoint> keypoints;
	vector<Point2f> pointsf, pointsf_next;
	int shown_face_count = 0;
	while (true)
	{

		Mat src;
		cap >> src;
		
		if (!src.empty())
		{
			cvtColor(src, gray, CV_BGR2GRAY);
			pyrDown(gray, gray,Size(src.cols / 2, src.rows / 2));
			flip(gray, gray, 1);
			dst = FilterImage(src);
			if (tracking_mode == 0)
			{

				bool success;
				success = BlobDetectFeatures(dst, dst, points, keypoints);
				if (success)
				{
					//tracking_mode = 1;
					cout << "success \n";
					KeyPoint::convert(keypoints, pointsf);
					RotatedRect rect = minAreaRect(pointsf);
					vector<Point> pts;
					Point2f recpoints[4];
					rect.points(recpoints);
					pts.push_back(recpoints[0]);
					pts.push_back(recpoints[1]);
					pts.push_back(recpoints[2]);
					pts.push_back(recpoints[3]);
					//ShowFaceCorner(src, pts);
					vector<Point> corners;
					corners=FindCubeCorners(pts);
					circle(src, corners[0], 2, Scalar(255, 255, 255), 2);
					circle(src, corners[1], 2, Scalar(255, 255, 255), 2);
					circle(src, corners[3], 2, Scalar(255, 255, 255), 2);
					circle(src, corners[2], 2, Scalar(255, 255, 255), 2);
				}
			}
			else if (tracking_mode == 1)
			{

				//FIX VECTOR POINTS SIZE	
				of = LKOpticalFlow(prev_gray, gray, pointsf, pointsf_next);
				if (!of)
				{
					tracking_mode = 0;
					points.clear();
					shown_face_count = 0;
				}
				RotatedRect rect = minAreaRect(pointsf);
				vector<Point> pts;
				Point2f recpoints[4];
				rect.points(recpoints);
				pts.push_back(recpoints[0]);
				pts.push_back(recpoints[1]);
				pts.push_back(recpoints[2]);
				pts.push_back(recpoints[3]);
				ShowFaceCorner(src, pts);
				line(src, recpoints[0], recpoints[1], Scalar(255, 255, 255),2);
				line(src, recpoints[1], recpoints[2], Scalar(255, 255, 255), 2);
				line(src, recpoints[2], recpoints[3], Scalar(255, 255, 255), 2);
				line(src, recpoints[3], recpoints[0], Scalar(255, 255, 255), 2);
				pointsf = pointsf_next;
			}
			imshow("Video", src);
			imshow("Blob", dst);
			imshow("Gray",gray);
			prev_dst = dst;
			prev_gray = gray;
		}

		if (waitKey(1) == 27) break;
		//Sleep(33);
	}
}
