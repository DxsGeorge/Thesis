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

class Cube
{
public:
	SimpleFace F; //front
	SimpleFace L; //left
	SimpleFace R; //right
	SimpleFace U; //under
	SimpleFace D; //down
	SimpleFace B; //back
	Cube(vector<SimpleFace> faces);
};

class MatchedFace
{
public:
	vector<String> colors;
	MatchedFace(vector<String> color);
};