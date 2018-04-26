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
#include "Sticker.h"
#include "Hough_Transform.h"
#include "lk-tracking.h"

#include <iostream>
#include <string>
#include <Windows.h>


#include "Input.hpp"


using namespace cv;
using namespace std;



int main()
{
	VideoCapture cap;
	char *filename = "http://192.168.1.5:8080/video?.mjpeg";
	cap.open(0);
	int tracking_mode = 2;
	int undetectednum = 100;
	bool of;
	Mat prev_dst;
	Mat dst,sr1;
	Mat gray, prev_gray;
	Mat rgb, hsv, yuv;
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
	bool parsedsolution = false;
	int solution_iterator=0;
	string solution;
	vector<string> steps;
	bool finished = false;
	MyCube cube;

	int assigned[6][9] = {};
	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			assigned[i][j] = -1;
		}
	}
	for (int i = 0; i < 6; ++i)
	{
		assigned[i][4] = i;
	}

	vector<Scalar> face1, face2, face3;

	//PnP initializers
	Mat rvec, tvec;
	vector<Point3f> normal;
	normal.push_back(Point3f(.0, .0, .0));
	normal.push_back(Point3f(1.0, .0, .0));
	normal.push_back(Point3f(.0, .0, .0));
	normal.push_back(Point3f(.0, 1.0, .0));
	normal.push_back(Point3f(.0, .0, .0));
	normal.push_back(Point3f(.0, .0, 1.0));
	
	//camera calibration matrices
	Mat cam = Mat(3, 3, CV_64F);
	cam = (Mat_<float>(3, 3) << 747.468709, 0.000000, 304.749530, 0.000000, 744.363317, 275.458171, 0.000000, 0.000000, 1.000000);
	Mat dist = Mat(5, 1, CV_64F);
	dist = (Mat_<float>(5, 1) << -0.020035, -0.311615, 0.004544, -0.005086, 0.000000);


	Point v1(0, 0), v2(0, 0), p0(0, 0);
	vector<Point> prevface{ Point(0, 0), Point(5, 0), Point(0, 5) };

	int stat = 0;

	vector<SimpleFace> faces;

	bool unassigned = true;

	//sample cube colors for test

	Scalar CR = Scalar(8, 190, 100);
	Scalar O = Scalar(13, 190, 100);
	Scalar Y = Scalar(30, 195, 100);
	Scalar G = Scalar(70, 195, 100);
	Scalar CB = Scalar(120, 195, 100);
	Scalar W = Scalar(37, 20, 200);

	vector<Scalar> f = { O, CR, CB, Y, O, W, O, W, O };
	vector<Scalar> l = { CB, CR, CB, O, CB, CR, CR, O, G };
	vector<Scalar> r = { W, W, CB, CB, G, Y, G, Y, CR };
	vector<Scalar> u = { CR, W, W, G, W, G, G, CB, W };
	vector<Scalar> d = { W, CB, CR, O, Y, O, Y, CB, Y };
	vector<Scalar> b = { Y, Y, O, CR, CR, G, G, G, Y };
	
	SimpleFace F(f,f,f), L(l,l,l), R(r,r,r), U(u,u,u), D(d,d,d), B(b,b,b);
	//


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
			src.copyTo(rgb);
			cvtColor(src, hsv, CV_BGR2HSV);
			cvtColor(src, yuv, CV_BGR2YCrCb);
			if (tracking_mode == 2)
			{
				if (tracking > 0)
				{
					//do lk trakcing
					detected = 2;
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
					

					//Check if lost
#if 1
					float ds1 = Distance(pointsf_next[0], pointsf_next[1]);
					float ds2 = Distance(pointsf_next[2], pointsf_next[3]);
					if (max(ds1, ds2) / min(ds1, ds2)>1.4) tracking = 0;
					float ds3 = Distance(pointsf_next[0], pointsf_next[2]);
					float ds4 = Distance(pointsf_next[1], pointsf_next[3]);
					if (max(ds3, ds4) / min(ds3, ds4) > 1.4) tracking = 0;
					if (ds1 < 10 || ds2 < 10 || ds3 < 10 || ds4 < 10) tracking = 0;
#endif
					if (tracking == 0)
					{
						detected = 0;
						colorextract = false;
					}				
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
#if 1
					line(src, pt[0], pt[1], Scalar(0, 255, 0), 2);
					line(src, pt[1], p3, Scalar(0, 255, 0), 2);
					line(src, p3, pt[2], Scalar(0, 255, 0), 2);
					line(src, pt[2], pt[0], Scalar(0, 255, 0), 2);
#endif					
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
						rad = 0.8*rad;
						float stickdist = Distance(ep[0], ep[1]);
						for (size_t i = 0; i < ep.size(); ++i)
						{
							//if (ep[i].x>rad && ep[i].x < src.cols / 2 - rad && ep[i].y>rad && ep[i].y < src.rows - rad)
							{
								Scalar col_avg1, col_avg2, col_avg3;
								vector<Point2f> cubepoints = pointcube(ep[i], stickdist);
								
								Vec3b color = yuv.at<Vec3b>(ep[i]);
								col_avg1 = colavg(hsv, ep[i], stickdist);
								col_avg2 = colavg(yuv, ep[i], stickdist);
								col_avg3 = colavg(rgb, ep[i], stickdist);
								vector<Scalar> col_avg = { col_avg1, col_avg2, col_avg3 };
								face1.push_back(col_avg1);
								face2.push_back(col_avg2);
								face3.push_back(col_avg3);
								for (size_t it = 0; it < cubepoints.size(); ++it)
								{
									circle(src, cubepoints[it], 2, Scalar(255, 255, 255), 1);
								}
							}
						}
						bool newface = true;
						if (face1.size() != 9) newface = false;
						for (size_t i = 0; i < faces.size() && newface; ++i)
						{
							//if (ScalarCompare(faces[i].getCenterHSV(), face1[4]) < 25.5)
							//if (CompareOnlyH(faces[i].getCenterHSV(), face1[4]) < 25) 
							/*if	(face2.size() < 9 || FaceCompareYUV(faces[i], face2) < 1.8)
							{
								cout << "wrong face" << endl;
								newface = false;
							}
							*/
						}
						if (newface)
						{
							faces.push_back(SimpleFace(face1,face2,face3));
						}
						face1.clear();
						face2.clear();
						face3.clear();
						cout << faces.size() << endl;
						colorextract = false;
					}
					
				}

			}

			vector<SimpleFace> samplefaces;
			samplefaces = { F, L, R, U, D, B };
			vector<vector<Scalar>> prntcols;
			
			if (faces.size() == 6 && unassigned)
			//if (unassigned)
			{
				cube = MyCube(faces);
				for (size_t j = 0; j < faces.size(); ++j)
				{
					prntcols.push_back(faces[j].getColorsHSV());
				}
				for (size_t j = 0; j < prntcols.size(); ++j)
				{
					cout << j << endl;
					for (size_t i = 0; i < prntcols[j].size(); ++i)
					{
						cout << prntcols[j][i] << endl;
					}
				}
				cube = ProcessColors(faces);
				cube.centerToColor();
				cube.numToColor();
				cube.printCubeCharacters();
				unassigned = false;
				
			}
			vector<string> cube_set = { "F:", "R:", "L:", "U:", "D:", "B:" };
			
			
			
			if (faces.size() == 6 && !parsedsolution)
			//if (!parsedsolution)
			{
				
				for (auto it = cube.facecolors_char.begin(); it != cube.facecolors_char.end(); ++it)
				{
					cube_set[it - cube.facecolors_char.begin()] += 
						string () + (*it)[0] + (*it)[1] + (*it)[2] + (*it)[3] + (*it)[4] + (*it)[5] + (*it)[6] + (*it)[7] + (*it)[8];
				}
				solution = Input(cube_set);
				cout << solution << endl;
				istringstream iss(solution);
				copy(istream_iterator<string>(iss),
					istream_iterator<string>(),
					back_inserter(steps));
				parsedsolution = true;
			}

			if (parsedsolution) //phase of showing solution on screen
			{
				
				if (solution_iterator < steps.size() && solution != "error")
				{
					cout << steps[solution_iterator] << endl;
					int stepface = StepFace(steps[solution_iterator]);
					//ShowStep(steps[solution_iterator], stepface, src, cube); //todo fit step to cube face
					StepShower(cube, steps[solution_iterator],src);
				}
				else if ((solution_iterator >= steps.size()) && (!finished))
				{
					cout << "Bye" << endl;
					finished = true;
				}
			}
			
			imshow("Video", src);
			imshow("Canny", dst);
			//imshow("HSV", hsv);
			prev_dst = dst;
			prev_gray = gray;
			count++;
		}
		
		char c = waitKey(11);
		if (c == 27) break;
		if (c == 'c') imwrite("C:/Users/user/Documents/Visual Studio 2013/Projects/Thesis/sample.jpg", src);
		if (c == 'r')
		{	
			cout << "faces cleared" << endl;
			faces.clear();
			parsedsolution = false;
			finished = false;
		}
		if (c == ' ') colorextract = true;
		if (c == 'n' || c == 'N') solution_iterator++;
		//Sleep(33);
	}
}
