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
	VideoCapture cap;
	char *filename = "http://192.168.1.5:8080/video?.mjpeg";
	cap.open(1);
	int tracking_mode = 2;
	int undetectednum = 100;
	bool of;
	Mat prev_dst;
	Mat dst,sr1;
	Mat gray, prev_gray;
	Mat hsv;
	vector<Point> points, pt, lastpt;
	vector<KeyPoint> keypoints;
	vector<Point2f> pointsf, pointsf_next;
	int shown_face_count = 0;
	int thr = 100;
	int succ = 0;
	int detected = 0;
	int tracking = 0;
	int count = 0;
	bool colorextract = false;
	Mat rvec, tvec;
	vector<Point3f> normal; 
	normal.push_back(Point3f(.0, .0, .0));
	normal.push_back(Point3f(1.0, .0, .0));
	normal.push_back(Point3f(.0, .0, .0));
	normal.push_back(Point3f(.0, 1.0, .0));
	normal.push_back(Point3f(.0, .0, .0));
	normal.push_back(Point3f(.0, .0, 1.0));
	Mat cam = Mat(3, 3, CV_64F);
	cam = (Mat_<float>(3, 3) << 747.468709, 0.000000, 304.749530, 0.000000, 744.363317, 275.458171, 0.000000, 0.000000, 1.000000);
	Mat dist = Mat(5, 1, CV_64F);
	dist = (Mat_<float>(5, 1) << -0.020035, -0.311615, 0.004544, -0.005086, 0.000000);
	Point v1(0, 0), v2(0, 0), p0(0, 0);
	vector<Point> prevface{ Point(0, 0), Point(5, 0), Point(0, 5) };
	int stat = 0;
	while (true)
	{

		Mat src;
		cap >> src;
		//src += Scalar(50, 50, 50);
		if (!src.empty())
		{
			cvtColor(src, gray, CV_BGR2GRAY);
			pyrDown(gray, gray, Size(src.cols / 2, src.rows / 2));
			flip(gray, gray, 1);
			dst = FilterImage(src);
			cvtColor(src, hsv, CV_BGR2HSV);
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
					vector<Point2f> corners;
					corners=FindCubeCorners(pts);
					
					line(src,corners[0],corners[2],Scalar(255,255,255),2 );
					line(src, corners[3], corners[1], Scalar(255, 255, 255), 2);
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
				line(src, recpoints[0], recpoints[1], Scalar(255, 255, 255), 2);
				line(src, recpoints[1], recpoints[2], Scalar(255, 255, 255), 2);
				line(src, recpoints[2], recpoints[3], Scalar(255, 255, 255), 2);
				line(src, recpoints[3], recpoints[0], Scalar(255, 255, 255), 2);
				pointsf = pointsf_next;
			}
			else if (tracking_mode == 2)
			{
				if (tracking > 0)
				{
					//do lk trakcing
					detected = 2;
					colorextract = true;
					TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.3);
					Size subPixWinSize(10, 10), winSize(5, 5);
					vector<uchar> status;
					Mat err;
					calcOpticalFlowPyrLK(prev_gray, gray, pointsf, pointsf_next, status, err, winSize);
					for (size_t i = 0; i < pointsf.size(); ++i)
					{
						circle(src, pointsf_next[i], 2, Scalar(0, 255, 0), 1);
					}
					circle(src, pointsf_next[0], 2, Scalar(255, 0, 0), 1);
					circle(src, pointsf_next[1], 2, Scalar(0, 255, 0), 1);
					circle(src, pointsf_next[2], 2, Scalar(0, 0, 255), 1);
					circle(src, pointsf_next[3], 2, Scalar(255, 255, 255), 1);
					pointsf = pointsf_next;

					//check outliers from lk tracing
					for (size_t i = 0; i < status.size(); ++i)
					{
						if (status[i] == 0) stat++;
					}
					if (stat == 3)
					{
						tracking = 0;
						stat = 0;
					}
					for (size_t i = 0; i < pointsf_next.size(); ++i)
					{
						if (pointsf_next[i].x > src.cols/2 || pointsf_next[i].y > src.rows/2) tracking = 0;
					}
					//Find cube pose in 3D
					rvec = FindCubeOrientation(pointsf_next, cam, dist, tvec);
					vector<Point2f> impts;
					projectPoints(normal, rvec, tvec, cam, dist, impts);
					//cout << "imgpts: " << impts << endl;
					line(src, impts[0], impts[1], Scalar(0, 0, 255), 1);
					line(src, impts[2], impts[3], Scalar(255, 0, 255), 1);
					line(src, impts[4], impts[5], Scalar(0, 255, 255), 1);

					//Check if lost
#if 0
					float ds1 = Distance(pointsf_next[0], pointsf_next[1]);
					float ds2 = Distance(pointsf_next[2], pointsf_next[3]);
					if (max(ds1, ds2) / min(ds1, ds2)>1.4) tracking = 0;
					float ds3 = Distance(pointsf_next[0], pointsf_next[2]);
					float ds4 = Distance(pointsf_next[1], pointsf_next[3]);
					if (max(ds3, ds4) / min(ds3, ds4) > 1.4) tracking = 0;
					if (ds1 < 10 || ds2 < 10 || ds3 < 10 || ds4 < 10) tracking = 0;
					if (tracking == 0)
					{
						detected = 0;
						colorextract = false;
					}
#endif					
				}
				if (tracking == 0)
				{
					//find cube face
					vector<Vec4i> lines;
					vector<Point> edges;
					HoughLinesP(dst, lines, 1, CV_PI / 45, thr, 10, 5);
					for (size_t i = 0; i < lines.size(); i++)
					{
						line(src, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(0, 0, 255), 1);
					}
					if (lines.size() < 50) thr = max(thr - 1, 2);
					else thr++;
					//cout << "lines: " << lines.size() << "threshold: " << thr << endl;
					FindCubeFace1(lines, prevface, succ, detected, tracking, v1, v2, p0, pointsf, pt);

				}
				else
				{
					//tracking mode 
					Point p(pointsf[0]), p1(pointsf[1]), p2(pointsf[2]);
					v1 = Point(p1.x - p.x, p1.y - p.y);
					v2 = Point(p2.x - p.x, p2.y - p.y);
					pt = { Point(p.x - v1.x - v2.x, p.y - v1.y - v2.y),
						Point(p.x + 2 * v2.x - v1.x, p.y + 2 * v2.y - v1.y),
						Point(p.x + 2 * v1.x - v2.x, p.y + 2 * v1.y - v2.y) };
					prevface = { pt[0], pt[1], pt[2] };
				}
				if (detected > 0 || undetectednum < 1) 
				{
					if (detected <= 0)
					{
						undetectednum++;
						pt = lastpt;
					}
					else
					{
						undetectednum = 0;
						lastpt = pt;
					}
					Point p3(pt[2].x + pt[1].x - pt[0].x, pt[2].y + pt[1].y - pt[0].y);

					line(src, pt[0], pt[1], Scalar(0, 255, 0), 2);
					line(src, pt[1], p3, Scalar(0, 255, 0), 2);
					line(src, p3, pt[2], Scalar(0, 255, 0), 2);
					line(src, pt[2], pt[0], Scalar(0, 255, 0), 2);

					pt = winded(pt[0], pt[1], pt[2], p3);

					v1 = Point(pt[1].x - pt[0].x, pt[1].y - pt[0].y);
					v2 = Point(pt[3].x - pt[0].x, pt[3].y - pt[0].y);
					p0 = Point(pt[0].x, pt[0].y);

					vector<Point2f> ep;
					int i = 1;
					int j = 5;

					if (colorextract)
					{
						//cube is tracked
						for (size_t k = 0; k < 9; ++k)
						{
							ep.push_back(Point2f((p0.x + i*v1.x / 6.0 + j*v2.x / 6.0), (p0.y + i*v1.y / 6.0 + j*v2.y / 6.0)));
							i += 2;
							if (i == 7)
							{
								i = 1;
								j -= 2;
							}
						}
						float rad = Distance(v1, Point(0, 0)) / 6.0;
						for (size_t i = 0; i < ep.size(); ++i)
						{
							if (ep[i].x>rad && ep[i].x < src.cols / 2 - rad && ep[i].y>rad && ep[i].y < src.rows - rad)
							{
								Vec3b color = hsv.at<Vec3b>(ep[i]);
								Scalar col = ColorHSV(color);
								//circle(src, ep[i], rad, col, -1);
								
							}
						}
					}
					
				}

			}
			imshow("Video", src);
			imshow("Blob", dst);
			imshow("Gray",gray);
			prev_dst = dst;
			prev_gray = gray;
			count++;
		}
		

		if (waitKey(1) == 27) break;
		//Sleep(33);
	}
}
