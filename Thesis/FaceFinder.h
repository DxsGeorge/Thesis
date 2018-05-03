#pragma once

#include "Line.h"
#include "MyHash.h"
#include "Sticker.h"
#include <Eigen/Core>
#include <Eigen/Dense>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <map>
#include <math.h>
#include <cmath>
#include <array>
#include <algorithm>

using namespace std;
using namespace cv;

vector<Point> FindIntersectionSpots (vector<Line> lines, int l_offset, float p_offset);

vector<Square> FindSquares(vector<Point> points, float offset);

vector<Square> FindSquares(vector <Line> lines,int p_offset, float l_offset);

vector <Square> FindGridSquares(vector<Square> squares, float offset1);

vector<Line> gridLines(vector<Line> lines, float offset);

float roundf(float val);

void ShowFace(Mat src, vector<Point> points);

void ShowFaceCorner(Mat src, vector<Point> points);

string ColorMatcher(Vec3b values, int maxval[]);

string ColorMatcherHSV(Vec3b values);

string ColorMatcherHLS(Vec3b values);

Scalar ColorHSV(Vec3b values);

string ColorMatcherMulti(Vec3b values, Vec3b values1);

float middle(float x , float y, float z);

vector<Point2f> FindCubeCorners(vector<Point> points);

vector<Point2f> FindCubeCorners2(vector<Point> points);

bool FindIntersection(Vec4i l1, Vec4i l2, Point &pt);

float AngletoX(Vec4i pt);

void FindCubeFace1(vector<Vec4i> pts, vector<Point> &prevface, int &succ, int &detected, int &tracking,
	Point &v1, Point &v2, Point &p0, vector<Point2f> &features, vector<Point> &pt);

int compfaces(vector<Point> f1, vector<Point> f2);

vector<Point> winded(Point p1, Point p2, Point p3, Point p4);

Mat FindCubeOrientation(vector<Point2f> points, Mat cam, Mat dist, Mat &tvec);

int ColMatcher(vector<int> st_color, vector<vector<int>> colors);

float ScalarCompare(Scalar a, Scalar b);

float FaceCompare(SimpleFace a, SimpleFace b);

float FaceCompareYUV(SimpleFace a, SimpleFace b);

float CompareOnlyH(Scalar a, Scalar b);

int MatchToCenter(vector<Scalar> centers, Scalar color);

float ptdstHSV(Scalar a, Scalar b);

float ptdstw2(Scalar a, Scalar b);

float ptdstRGB(Scalar a, Scalar b);

float ptdstYUV(Scalar a, Scalar b);

float ptdstGRAY(float a, float b);

MyCube ProcessColors(vector<SimpleFace> faces);

void FindFaceStep(int step, MyCube cube);

int StepFace(string step);

void ShowStep(string step_char, int step, Mat img, MyCube cube);

void DrawCubeGrid(int img_w, int img_h, Mat img);

void DrawArrow(Mat img, int img_w, int img_h, int count, bool cw);

void StepShower(MyCube cube, string step, Mat &img);

void DrawFaces(Mat img, int h, int w);

void FacesViewer(Mat &img, MyCube cube);



