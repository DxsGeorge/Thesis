#include "Sticker.h"


using namespace std;
using namespace cv;

Sticker::Sticker ()
{
	
}

Sticker::Sticker(Point pos, int posingrid, int H, int S, int V, vector<Sticker> neighbors, Point ul, Point br)
{
	this->position=pos;
	this->posingrid=posingrid;
	this->H = H;
	this->V = V;
	this->S = S;
	this->neighbors=neighbors;
	/*
	for (size_t i=0;i<neighbors.size();++i)
	{
	this->neighbors.push_back(neighbors[i]);
	}
	*/
	this->ul=ul;
	this->br=br;
	this->size = pow(Distance(ul,br),2.0)/2.0;
	if (this->neighbors.size()==3) this->iscorner=true;
	else if (this->neighbors.size()==8) this->iscenter=true;
}

bool Sticker::isCenter() 
{
	return this->iscenter;
}

bool Sticker::isCorner()
{
	return this->iscorner;
}

vector<Sticker> Sticker::getNeighbors()
{
	return this->neighbors;
}	

Point Sticker::getPosition()
{
	return this->position;
}

int Sticker::getPosinGrid()
{
	return this->posingrid;
}

int Sticker::getColor()
{
	return this->num_color;
}

float Sticker::getSize()
{
	return this->size;
}

Point Sticker::getUl()
{
	return this->ul;
}

Point Sticker::getBr()
{
	return this->br;
}

Face::Face()
{

}

Face::Face(vector<Sticker> stickers)
{
	float size=0.0;
	int col=stickers[0].getColor();
	this->iscomplete=true;
	this->stickers=stickers;
	for (size_t i=0;i<this->stickers.size();++i)
	{
		size+=stickers[i].getSize();
		if (col!=stickers[i].getColor()) this->iscomplete=false;
		if (this->stickers[i].isCenter()) this->center=this->stickers[i];
		if (stickers[i].getPosinGrid() == 0) this->ul = stickers[i].getUl();
		if (stickers[i].getPosinGrid() == 8) this->br = stickers[i].getBr();
	}
	this->centerpos=this->center.getPosition();
	this->angle=atan2((float(ul.y-br.y)),(float(ul.x-br.x)));
	this->size=size;
	

}

vector<Sticker> Face::getStickers()
{
	return this->stickers;
}

Point Face::getPos()
{
	return this->center.getPosition();
}

bool Face::isComplete()
{
	return this->iscomplete;
}

Sticker Face::getCenter()
{
	return this->center;
}

vector<Face> Face::getNeighbors()
{
	vector<Face> a = {};
	cout<<"Dont use this yet"<<endl;
	return a;
}

float Face::getAngle()
{
	return this->angle;
}

float Face::getSize()
{
	return this->size;
}

Point Face::getUl()
{
	return this->ul;
}

Point Face::getBr()
{
	return this->br;
}

SimpleFace::SimpleFace()
{
	
};

SimpleFace::SimpleFace(vector<Scalar> face)
{
	this->center = face[4];
	this->colors = face;
};

Scalar SimpleFace::getCenter()
{
	return this->center;
}

vector<Scalar> SimpleFace::getColors()
{
	return this->colors;
}

Cube::Cube(vector<MatchedFace> faces)
{
	if (faces.size() != 6) cout << "too many faces" << endl;
	this->F = faces[0];
	this->L = faces[1];
	this->B = faces[2];
	this->R = faces[3];
	this->U = faces[4];
	this->D = faces[5];
}

MatchedFace::MatchedFace(vector<int> color)
{
	this->colors = color;
}

tuple<tuple<int,int>,tuple<int,int>> neighbors(int f, int s)
{
	tuple<int, int> a;
	tuple<int, int> b;
	if (f == 0 && s == 0) { a=make_tuple(1, 2 ); b = make_tuple( 4, 0 );}
	if (f == 0 && s == 1) { a = make_tuple( 4, 3 ); b = make_tuple(-1, -1);}
	if (f == 0 && s == 2) { a = make_tuple( 4, 6 ); b = make_tuple( 3, 0 );}
	if (f == 0 && s == 3) { a = make_tuple( 1, 5 ); b = make_tuple( -1, -1 );}
	if (f == 0 && s == 5) { a = make_tuple( 3, 3 ); b = make_tuple( -1, -1 );}
	if (f == 0 && s == 6) { a = make_tuple( 1, 8 ); b = make_tuple( 5, 2 );}
	if (f == 0 && s == 7) { a = make_tuple( 5, 5 ); b = make_tuple( -1, -1 );}
	if (f == 0 && s == 8) { a = make_tuple( 3, 6 ); b = make_tuple( 5, 8 );}

	if (f == 1 && s == 0) { a = make_tuple( 0, 2 ); b = make_tuple( 4, 2 );}
	if (f == 1 && s == 1) { a = make_tuple( 4, 1 ); b = make_tuple( -1, -1 );}
	if (f == 1 && s == 2) { a = make_tuple( 4, 0 ); b = make_tuple( 0, 0 );}
	if (f == 1 && s == 3) { a = make_tuple( 2, 5 ); b = make_tuple( -1, -1 );}
	if (f == 1 && s == 5) { a = make_tuple( 0, 3 ); b = make_tuple( -1, -1 );}
	if (f == 1 && s == 6) { a = make_tuple( 2, 8 ); b = make_tuple( 5, 0 );}
	if (f == 1 && s == 7) {a = make_tuple(5, 1); b = make_tuple(-1, -1);}
	if (f == 1 && s == 8) {a = make_tuple(0, 6); b = make_tuple(5, 2);}

	if (f == 2 && s == 0) {a = make_tuple(4, 8); b = make_tuple(3, 2);}
	if (f == 2 && s == 1) {a = make_tuple(4, 5); b = make_tuple(-1, -1);}
	if (f == 2 && s == 2) {a = make_tuple(4, 2); b = make_tuple(1, 0);}
	if (f == 2 && s == 3) {a = make_tuple(3, 5); b = make_tuple(-1, -1);}
	if (f == 2 && s == 5) {a = make_tuple(1, 3); b = make_tuple(-1, -1);}
	if (f == 2 && s == 6) {a = make_tuple(3, 8); b = make_tuple(5, 6);}
	if (f == 2 && s == 7) {a = make_tuple(5, 3); b = make_tuple(-1, -1);}
	if (f == 2 && s == 8) {a = make_tuple(1, 6); b = make_tuple(5, 0);}

	if (f == 3 && s == 0) {a = make_tuple(4, 6); b = make_tuple(0, 2);}
	if (f == 3 && s == 1) {a = make_tuple(4, 7); b = make_tuple(-1, -1);}
	if (f == 3 && s == 2) {a = make_tuple(4, 8); b = make_tuple(2, 0);}
	if (f == 3 && s == 3) {a = make_tuple(0, 5); b = make_tuple(-1, -1);}
	if (f == 3 && s == 5) {a = make_tuple(2, 3); b = make_tuple(-1, -1);}
	if (f == 3 && s == 6) {a = make_tuple(0, 8); b = make_tuple(5, 8);}
	if (f == 3 && s == 7) {a = make_tuple(5, 7); b = make_tuple(-1, -1);}
	if (f == 3 && s == 8) {a = make_tuple(2, 6); b = make_tuple(5, 6);}

	if (f == 4 && s == 0) {a = make_tuple(1, 2); b = make_tuple(0, 0);}
	if (f == 4 && s == 1) {a = make_tuple(1, 1); b = make_tuple(-1, -1);}
	if (f == 4 && s == 2) {a = make_tuple(1, 0); b = make_tuple(2, 2);}
	if (f == 4 && s == 3) {a = make_tuple(0, 1); b = make_tuple(-1, -1);}
	if (f == 4 && s == 5) {a = make_tuple(2, 1); b = make_tuple(-1, -1);}
	if (f == 4 && s == 6) {a = make_tuple(0, 2); b = make_tuple(3, 0);}
	if (f == 4 && s == 7) {a = make_tuple(3, 1); b = make_tuple(-1, -1);}
	if (f == 4 && s == 8) {a = make_tuple(3, 2); b = make_tuple(2, 0);}

	if (f == 5 && s == 0) {a = make_tuple(1, 6); b = make_tuple(2, 8);}
	if (f == 5 && s == 1) {a = make_tuple(1, 7); b = make_tuple(-1, -1);}
	if (f == 5 && s == 2) {a = make_tuple(1, 8); b = make_tuple(0, 6);}
	if (f == 5 && s == 3) {a = make_tuple(2, 7); b = make_tuple(-1, -1);}
	if (f == 5 && s == 5) {a = make_tuple(0, 7); b = make_tuple(-1, -1);}
	if (f == 5 && s == 6) {a = make_tuple(2, 6); b = make_tuple(3, 8);}
	if (f == 5 && s == 7) {a = make_tuple(3, 7); b = make_tuple(-1, -1);}
	if (f == 5 && s == 8) {a = make_tuple(3, 6); b = make_tuple(0, 8);}
	tuple<tuple<int,int>, tuple<int,int>> neighbor(a,b);
	return neighbor;
}