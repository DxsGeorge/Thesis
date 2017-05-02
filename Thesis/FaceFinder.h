#pragma once

#include "Line.h"
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <map>
#include <math.h>
#include <array>

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

string ColorMatcherMulti(Vec3b values, Vec3b values1);

float middle(float x , float y, float z);

vector<Point> FindCubeCorners(vector<Point> points);