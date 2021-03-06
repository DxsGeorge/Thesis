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
	Scalar centerHSV;
	Scalar centerYUV;
	Scalar centerRGB;
	vector<Scalar> colorsHSV;
	vector<Scalar> colorsYUV;
	vector<Scalar> colorsRGB;

public:
	SimpleFace();
	SimpleFace(vector<Scalar> face1, vector<Scalar> face2, vector<Scalar> face3);
	Scalar getCenterHSV();
	Scalar getCenterYUV();
	Scalar getCenterRGB();
	vector<Scalar> getColorsHSV();
	vector<Scalar> getColorsYUV();
	vector<Scalar> getColorsRGB();
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

class MyCube
{
public:
	MatchedFace F; //front
	MatchedFace L; //left
	MatchedFace R; //right
	MatchedFace U; //under
	MatchedFace D; //down
	MatchedFace B; //back

	vector<SimpleFace> colfaces;

	MyCube();
	MyCube(vector<MatchedFace> faces, vector<SimpleFace> colorfaces);
	MyCube(vector<SimpleFace> colorfaces);
	vector<MatchedFace> faces;
	vector <tuple<Scalar, char>> centercolors;
	vector<vector<char>> facecolors_char;
	void printFaces();
	void centerToColor();
	void numToColor();
	void printCubeCharacters();
	void CubeModify(string step);
};

tuple<tuple<int,int>,tuple<int,int>> neighbors(int f, int s);

Scalar colavg(Mat src, Point2f point, float dist);

