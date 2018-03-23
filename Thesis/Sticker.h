#pragma once

#include <opencv2/highgui/highgui.hpp>
#include "Line.h"
#include <cmath>
#include <iostream>

using namespace std;
using namespace cv;


class Sticker
{
private:
	bool iscorner;
	bool iscenter;
	vector<Sticker> neighbors;
	Point position;
	int posingrid;
	Point ul,br;
	int num_color;
	float size;
	int H, S, V;

public:
	Sticker();
	Sticker(Point pos, int posingrid, int H, int S, int V, vector<Sticker> neighbors, Point ul, Point br);
	bool isCorner();
	bool isCenter();
	vector<Sticker> getNeighbors();
	Point getPosition();
	int getColor();
	int getPosinGrid();
	float getSize();
	Point getUl();
	Point getBr();
};

class Face
{
private:
	vector<Sticker> stickers;
	Sticker center;
	vector<Face> neighbors;
	Point centerpos;
	Point ul, br;
	bool iscomplete;
	float angle;
	float size;

public:
	Face();
	Face(vector<Sticker> stickers);
	vector<Sticker> getStickers();
	Sticker getCenter();
	vector<Face> getNeighbors();
	Point getPos();
	bool isComplete();
	float getAngle();
	float getSize();
	Point getUl();
	Point getBr();
};

class SimpleFace
{
private:
	Scalar center;
	vector<Scalar> colors;

public:
	SimpleFace();
	SimpleFace(vector<Scalar> face);
	Scalar getCenter();
	vector<Scalar> getColors();
};

class MatchedFace
{
public:
	vector<int> colors;
	MatchedFace();
	MatchedFace(vector<int> color);
};

class ColorFace
{
public:
	vector<char> colors;
	ColorFace();
	ColorFace(vector<char>);
};

class Cube
{
public:
	MatchedFace F; //front
	MatchedFace L; //left
	MatchedFace R; //right
	MatchedFace U; //under
	MatchedFace D; //down
	MatchedFace B; //back

	ColorFace cF; //same
	ColorFace cR;
	ColorFace cL;
	ColorFace cU;
	ColorFace cD;
	ColorFace cB;

	Cube();
	Cube(vector<MatchedFace> faces);
	vector<MatchedFace> faces;
	vector<char> colors_c;
	void printFaces();
	void ColToCenters();
	void NumToColors();
};

tuple<tuple<int,int>,tuple<int,int>> neighbors(int f, int s);

Scalar colavg(Mat src, Point2f point, float dist);