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

SimpleFace::SimpleFace(vector<Scalar> face1, vector<Scalar> face2, vector<Scalar> face3)
{
	this->centerHSV = face1[4];
	this->colorsHSV = face1;
	this->centerYUV = face2[4];
	this->colorsYUV = face2;
	this->centerRGB = face3[4];
	this->colorsRGB = face3;
};

Scalar SimpleFace::getCenterHSV()
{
	return this->centerHSV;
}

Scalar SimpleFace::getCenterYUV()
{
	return this->centerYUV;
}

Scalar SimpleFace::getCenterRGB()
{
	return this->centerRGB;
}

vector<Scalar> SimpleFace::getColorsHSV()
{
	return this->colorsHSV;
}

vector<Scalar> SimpleFace::getColorsYUV()
{
	return this->colorsYUV;
}

vector<Scalar> SimpleFace::getColorsRGB()
{
	return this->colorsRGB;
}

MatchedFace::MatchedFace()
{

}

MatchedFace::MatchedFace(vector<int> color)
{
	this->colors = color;
}


bool ScalarCompareH(const Scalar& a, const Scalar& b)
{
	return a[0] < b[0];
}



bool ScalarEqual(const tuple<Scalar, char>& a, const tuple<Scalar, char>& b)
{
	int col1 = get<0>(a)[0];
	int col2 = get<0>(b)[0];
	return col1==col2;
}

MyCube::MyCube()
{

}

MyCube::MyCube(vector<MatchedFace> faces, vector<SimpleFace> colorfaces)
{
	if (faces.size() != 6) cout << "too many faces" << endl;
	this->F = faces[0];
	this->L = faces[1];
	this->R = faces[2];
	this->U = faces[3];
	this->D = faces[4];
	this->B = faces[5];
	this->faces = { this->F, this->L, this->R, this->U, this->D, this->B };

	this->colfaces = colorfaces;

}

MyCube::MyCube(vector<SimpleFace> colorfaces)
{
	this->colfaces = colorfaces;
}

void MyCube::printFaces()
{
	cout << "F: " << endl;
	cout << "| " << this->F.colors[0] << " | " << F.colors[3] << " | " << F.colors[6] << " |" << endl;
	cout << "| " << this->F.colors[1] << " | " << F.colors[4] << " | " << F.colors[7] << " |" << endl;
	cout << "| " << this->F.colors[2] << " | " << F.colors[5] << " | " << F.colors[8] << " |" << endl;

	cout << "L :" << endl;
	cout << "| " << this->L.colors[0] << " | " << L.colors[3] << " | " << L.colors[6] << " |" << endl;
	cout << "| " << this->L.colors[1] << " | " << L.colors[4] << " | " << L.colors[7] << " |" << endl;
	cout << "| " << this->L.colors[2] << " | " << L.colors[5] << " | " << L.colors[8] << " |" << endl;

	cout << "R: " << endl;
	cout << "| " << this->R.colors[0] << " | " << R.colors[3] << " | " << R.colors[6] << " |" << endl;
	cout << "| " << this->R.colors[1] << " | " << R.colors[4] << " | " << R.colors[7] << " |" << endl;
	cout << "| " << this->R.colors[2] << " | " << R.colors[5] << " | " << R.colors[8] << " |" << endl;

	cout << "U: " << endl;
	cout << "| " << this->U.colors[0] << " | " << U.colors[3] << " | " << U.colors[6] << " |" << endl;
	cout << "| " << this->U.colors[1] << " | " << U.colors[4] << " | " << U.colors[7] << " |" << endl;
	cout << "| " << this->U.colors[2] << " | " << U.colors[5] << " | " << U.colors[8] << " |" << endl;

	cout << "D: " << endl;
	cout << "| " << this->D.colors[0] << " | " << D.colors[3] << " | " << D.colors[6] << " |" << endl;
	cout << "| " << this->D.colors[1] << " | " << D.colors[4] << " | " << D.colors[7] << " |" << endl;
	cout << "| " << this->D.colors[2] << " | " << D.colors[5] << " | " << D.colors[8] << " |" << endl;

	cout << "B: " << endl;
	cout << "| " << this->B.colors[0] << " | " << B.colors[3] << " | " << B.colors[6] << " |" << endl;
	cout << "| " << this->B.colors[1] << " | " << B.colors[4] << " | " << B.colors[7] << " |" << endl;
	cout << "| " << this->B.colors[2] << " | " << B.colors[5] << " | " << B.colors[8] << " |" << endl;
}

void MyCube::printCubeCharacters()
{
	for (auto it = this->facecolors_char.begin(); it != facecolors_char.end(); ++it)
	{
		cout << endl;
		cout << "| " << (*it)[0] << " | " << (*it)[1] << " | " << (*it)[2] << " |" << endl;
		cout << "| " << (*it)[3] << " | " << (*it)[4] << " | " << (*it)[5] << " |" << endl;
		cout << "| " << (*it)[6] << " | " << (*it)[7] << " | " << (*it)[8] << " |" << endl;
		cout << endl;
	}
}

void MyCube::centerToColor()
{	
	vector<Scalar> centers,centers2;
	for (size_t i = 0; i < this->colfaces.size(); ++i)
	{
		centers.push_back(this->colfaces[i].getCenterHSV()); //sort by H value
		centers2.push_back(this->colfaces[i].getCenterHSV()); //original cube set
	}
	sort(centers.begin(), centers.end(), ScalarCompareH);
	for (auto it = centers.begin(); it != centers.end(); ++it)
	{
		Scalar color = *it;
		if (color[1] < 100) rotate(it,it+1,centers.end());
	}
	vector<tuple<Scalar, char>> colcenters;
	colcenters.push_back(make_tuple(centers[0], 'R'));
	colcenters.push_back(make_tuple(centers[1], 'O'));
	colcenters.push_back(make_tuple(centers[2], 'Y'));
	colcenters.push_back(make_tuple(centers[3], 'G'));
	colcenters.push_back(make_tuple(centers[4], 'B'));
	colcenters.push_back(make_tuple(centers[5], 'W'));
	for (auto it2 = centers2.begin(); it2 != centers2.end(); ++it2)
	{
		for (auto it = centers.begin(); it != centers.end(); ++it)
		{
			Scalar col1 = *it;
			Scalar col2 = *it2;
			auto it3 = find_if(colcenters.begin(), colcenters.end(), [&](const tuple<Scalar, char>& e) {
				return get<0>(e) == *it2;
			});
			if (abs(col1[0] - col2[0])  < 0.1)
			{
				this->centercolors.push_back(tuple<Scalar, char>(*it2, get<1>(*it3)));
			}
		}
	}
}

void MyCube::numToColor()
{
	for (auto it = this->faces.begin(); it != this->faces.end(); ++it)
	{
		vector<int> face = it->colors;
		vector<char> oneface_char;
		for (auto it2 = face.begin(); it2 != face.end(); ++it2)
		{
			oneface_char.push_back(get<1>(this->centercolors.at(*it2)));
		}
		swap(oneface_char[1], oneface_char[3]);
		swap(oneface_char[2], oneface_char[6]);
		swap(oneface_char[5], oneface_char[7]);
		this->facecolors_char.push_back(oneface_char);
	}
}

void MyCube::CubeModify(string step)
{
	int int_step, count;
	bool cw;

	if (step[0] == 'F') int_step = 0;
	else if (step[0] == 'L') int_step = 2;
	else if (step[0] == 'R') int_step = 1;
	else if (step[0] == 'U') int_step = 3;
	else if (step[0] == 'D') int_step = 4;
	else if (step[0] == 'B') int_step = 5;


	if (step[1] == '2') count = 2;
	else count = 1;

	if (step[1] == '\'') cw = false;
	else cw = true;
	vector<MatchedFace> newfaces(6);
	
	if (cw)
	{
		switch (int_step)
		{
		case 0:
			//front face
			newfaces[0] =
				//1                                 //2                           //3
				MatchedFace(
			{ this->faces[0].colors[2], this->faces[0].colors[5], this->faces[0].colors[8],
			//4                                 //5                           //6 
			this->faces[0].colors[1], this->faces[0].colors[4], this->faces[0].colors[7],
			//7                                 //8                           //9
			this->faces[0].colors[0], this->faces[0].colors[3], this->faces[0].colors[6] });

			//opposite face
			newfaces[5] = this->faces[5];

			//right face
			newfaces[1] = this->faces[1];
			newfaces[1].colors[0] = this->faces[3].colors[2];
			newfaces[1].colors[1] = this->faces[3].colors[5];
			newfaces[1].colors[2] = this->faces[3].colors[8];

			//left face
			newfaces[2] = this->faces[2];
			newfaces[2].colors[6] = this->faces[4].colors[0];
			newfaces[2].colors[7] = this->faces[4].colors[3];
			newfaces[2].colors[8] = this->faces[4].colors[6];

			//up face
			newfaces[3] = this->faces[3];
			newfaces[3].colors[2] = this->faces[2].colors[8];
			newfaces[3].colors[5] = this->faces[2].colors[7];
			newfaces[3].colors[8] = this->faces[2].colors[6];

			//down face
			newfaces[4] = this->faces[4];
			newfaces[4].colors[0] = this->faces[1].colors[2];
			newfaces[4].colors[3] = this->faces[1].colors[1];
			newfaces[4].colors[6] = this->faces[1].colors[0];
			break;

		case 1:
			//front face
			newfaces[1] =
				//1                                 //2                           //3
				MatchedFace(
			{ this->faces[1].colors[2], this->faces[1].colors[5], this->faces[1].colors[8],
			//4                                 //5                           //6 
			this->faces[1].colors[1], this->faces[1].colors[4], this->faces[1].colors[7],
			//7                                 //8                           //9
			this->faces[1].colors[0], this->faces[1].colors[3], this->faces[1].colors[6] });

			//opposite face
			newfaces[2] = this->faces[2];

			//right face
			newfaces[5] = this->faces[5];
			newfaces[5].colors[0] = this->faces[3].colors[8];
			newfaces[5].colors[1] = this->faces[3].colors[7];
			newfaces[5].colors[2] = this->faces[3].colors[6];

			//left face
			newfaces[0] = this->faces[0];
			newfaces[0].colors[6] = this->faces[4].colors[6];
			newfaces[0].colors[7] = this->faces[4].colors[7];
			newfaces[0].colors[8] = this->faces[4].colors[8];

			//up face
			newfaces[3] = this->faces[3];
			newfaces[3].colors[6] = this->faces[0].colors[6];
			newfaces[3].colors[7] = this->faces[0].colors[7];
			newfaces[3].colors[8] = this->faces[0].colors[8];

			//down face
			newfaces[4] = this->faces[4];
			newfaces[4].colors[6] = this->faces[5].colors[2];
			newfaces[4].colors[7] = this->faces[5].colors[1];
			newfaces[4].colors[8] = this->faces[5].colors[0];
			break;

		case 2:
			//front face
			newfaces[2] =
				//1                                 //2                           //3
				MatchedFace(
			{ this->faces[2].colors[2], this->faces[2].colors[5], this->faces[2].colors[8],
			//4                                 //5                           //6 
			this->faces[2].colors[1], this->faces[2].colors[4], this->faces[2].colors[7],
			//7                                 //8                           //9
			this->faces[2].colors[0], this->faces[2].colors[3], this->faces[2].colors[6] });

			//opposite face
			newfaces[1] = this->faces[1];

			//right face
			newfaces[0] = this->faces[0];
			newfaces[0].colors[0] = this->faces[3].colors[0];
			newfaces[0].colors[1] = this->faces[3].colors[1];
			newfaces[0].colors[2] = this->faces[3].colors[2];

			//left face
			newfaces[5] = this->faces[5];
			newfaces[5].colors[6] = this->faces[4].colors[2];
			newfaces[5].colors[7] = this->faces[4].colors[1];
			newfaces[5].colors[8] = this->faces[4].colors[0];

			//up face
			newfaces[3] = this->faces[3];
			newfaces[3].colors[0] = this->faces[5].colors[8];
			newfaces[3].colors[1] = this->faces[5].colors[7];
			newfaces[3].colors[2] = this->faces[5].colors[6];

			//down face
			newfaces[4] = this->faces[4];
			newfaces[4].colors[0] = this->faces[0].colors[0];
			newfaces[4].colors[1] = this->faces[0].colors[1];
			newfaces[4].colors[2] = this->faces[0].colors[2];
			break;

		case 3:
			//front face
			newfaces[3] =
				//1                                 //2                           //3
				MatchedFace(
			{ this->faces[3].colors[2], this->faces[3].colors[5], this->faces[3].colors[8],
			//4                                 //5                           //6 
			this->faces[3].colors[1], this->faces[3].colors[4], this->faces[3].colors[7],
			//7                                 //8                           //9
			this->faces[3].colors[0], this->faces[3].colors[3], this->faces[3].colors[6] });

			//opposite face
			newfaces[4] = this->faces[4];

			//right face
			newfaces[1] = this->faces[1];
			newfaces[1].colors[0] = this->faces[5].colors[0];
			newfaces[1].colors[3] = this->faces[5].colors[3];
			newfaces[1].colors[6] = this->faces[5].colors[6];

			//left face
			newfaces[2] = this->faces[2];
			newfaces[2].colors[0] = this->faces[0].colors[0];
			newfaces[2].colors[3] = this->faces[0].colors[3];
			newfaces[2].colors[6] = this->faces[0].colors[6];

			//up face
			newfaces[5] = this->faces[5];
			newfaces[5].colors[0] = this->faces[2].colors[0];
			newfaces[5].colors[3] = this->faces[2].colors[3];
			newfaces[5].colors[6] = this->faces[2].colors[6];

			//down face
			newfaces[0] = this->faces[0];
			newfaces[0].colors[0] = this->faces[1].colors[0];
			newfaces[0].colors[3] = this->faces[1].colors[3];
			newfaces[0].colors[6] = this->faces[1].colors[6];
			break;

		case 4:
			//front face
			newfaces[4] =
				//1                                 //2                           //3
				MatchedFace(
			{ this->faces[4].colors[2], this->faces[4].colors[5], this->faces[4].colors[8],
			//4                                 //5                           //6 
			this->faces[4].colors[1], this->faces[4].colors[4], this->faces[4].colors[7],
			//7                                 //8                           //9
			this->faces[4].colors[0], this->faces[4].colors[3], this->faces[4].colors[6] });

			//opposite face
			newfaces[3] = this->faces[3];

			//right face
			newfaces[1] = this->faces[1];
			newfaces[1].colors[2] = this->faces[0].colors[2];
			newfaces[1].colors[5] = this->faces[0].colors[5];
			newfaces[1].colors[8] = this->faces[0].colors[8];

			//left face
			newfaces[2] = this->faces[2];
			newfaces[2].colors[2] = this->faces[5].colors[2];
			newfaces[2].colors[5] = this->faces[5].colors[5];
			newfaces[2].colors[8] = this->faces[5].colors[8];

			//up face
			newfaces[0] = this->faces[0];
			newfaces[0].colors[2] = this->faces[2].colors[2];
			newfaces[0].colors[5] = this->faces[2].colors[5];
			newfaces[0].colors[8] = this->faces[2].colors[8];

			//down face
			newfaces[5] = this->faces[5];
			newfaces[5].colors[2] = this->faces[1].colors[2];
			newfaces[5].colors[5] = this->faces[1].colors[5];
			newfaces[5].colors[8] = this->faces[1].colors[8];
			break;

		case 5:
			//front face
			newfaces[5] =
				//1                                 //2                           //3
				MatchedFace(
			{ this->faces[5].colors[2], this->faces[5].colors[5], this->faces[5].colors[8],
			//4                                 //5                           //6 
			this->faces[5].colors[1], this->faces[5].colors[4], this->faces[5].colors[7],
			//7                                 //8                           //9
			this->faces[5].colors[0], this->faces[5].colors[3], this->faces[5].colors[6] });

			//opposite face
			newfaces[0] = this->faces[0];

			//right face
			newfaces[2] = this->faces[2];
			newfaces[2].colors[0] = this->faces[3].colors[6];
			newfaces[2].colors[1] = this->faces[3].colors[3];
			newfaces[2].colors[2] = this->faces[3].colors[0];

			//left face
			newfaces[1] = this->faces[1];
			newfaces[1].colors[6] = this->faces[4].colors[8];
			newfaces[1].colors[7] = this->faces[4].colors[5];
			newfaces[1].colors[8] = this->faces[4].colors[2];

			//up face
			newfaces[3] = this->faces[3];
			newfaces[3].colors[0] = this->faces[1].colors[6];
			newfaces[3].colors[3] = this->faces[1].colors[7];
			newfaces[3].colors[6] = this->faces[1].colors[8];

			//down face
			newfaces[4] = this->faces[4];
			newfaces[4].colors[2] = this->faces[2].colors[0];
			newfaces[4].colors[5] = this->faces[2].colors[1];
			newfaces[4].colors[8] = this->faces[2].colors[2];
			break;
		}
	}
	else
	{
		switch (int_step)
		{
		case 0:
			//front face
			newfaces[0] =
				//1                                 //2                           //3
				MatchedFace(
			{ this->faces[0].colors[6], this->faces[0].colors[3], this->faces[0].colors[0],
			//4                                 //5                           //6 
			this->faces[0].colors[7], this->faces[0].colors[4], this->faces[0].colors[1],
			//7                                 //8                           //9
			this->faces[0].colors[8], this->faces[0].colors[5], this->faces[0].colors[2] });

			//opposite face
			newfaces[5] = this->faces[5];

			//right face
			newfaces[1] = this->faces[1];
			newfaces[1].colors[0] = this->faces[4].colors[6];
			newfaces[1].colors[1] = this->faces[4].colors[3];
			newfaces[1].colors[2] = this->faces[4].colors[0];

			//left face
			newfaces[2] = this->faces[2];
			newfaces[2].colors[6] = this->faces[3].colors[8];
			newfaces[2].colors[7] = this->faces[3].colors[5];
			newfaces[2].colors[8] = this->faces[3].colors[2];

			//up face
			newfaces[3] = this->faces[3];
			newfaces[3].colors[2] = this->faces[1].colors[0];
			newfaces[3].colors[5] = this->faces[1].colors[1];
			newfaces[3].colors[8] = this->faces[1].colors[2];

			//down face
			newfaces[4] = this->faces[4];
			newfaces[4].colors[0] = this->faces[2].colors[6];
			newfaces[4].colors[3] = this->faces[2].colors[7];
			newfaces[4].colors[6] = this->faces[2].colors[8];
			break;

		case 1:
			//front face
			newfaces[1] =
				//1                                 //2                           //3
				MatchedFace(
			{ this->faces[1].colors[6], this->faces[1].colors[3], this->faces[1].colors[0],
			//4                                 //5                           //6 
			this->faces[1].colors[7], this->faces[1].colors[4], this->faces[1].colors[1],
			//7                                 //8                           //9
			this->faces[1].colors[8], this->faces[1].colors[5], this->faces[1].colors[2] });

			//opposite face
			newfaces[2] = this->faces[2];

			//right face
			newfaces[5] = this->faces[5];
			newfaces[5].colors[0] = this->faces[4].colors[8];
			newfaces[5].colors[1] = this->faces[4].colors[7];
			newfaces[5].colors[2] = this->faces[4].colors[6];

			//left face
			newfaces[0] = this->faces[0];
			newfaces[0].colors[6] = this->faces[3].colors[6];
			newfaces[0].colors[7] = this->faces[3].colors[7];
			newfaces[0].colors[8] = this->faces[3].colors[8];

			//up face
			newfaces[3] = this->faces[3];
			newfaces[3].colors[6] = this->faces[5].colors[2];
			newfaces[3].colors[7] = this->faces[5].colors[1];
			newfaces[3].colors[8] = this->faces[5].colors[0];

			//down face
			newfaces[4] = this->faces[4];
			newfaces[4].colors[6] = this->faces[0].colors[6];
			newfaces[4].colors[7] = this->faces[0].colors[7];
			newfaces[4].colors[8] = this->faces[0].colors[8];
			break;

		case 2:
			//front face
			newfaces[2] =
				//1                                 //2                           //3
				MatchedFace(
			{ this->faces[2].colors[6], this->faces[2].colors[3], this->faces[2].colors[0],
			//4                                 //5                           //6 
			this->faces[2].colors[7], this->faces[2].colors[4], this->faces[2].colors[1],
			//7                                 //8                           //9
			this->faces[2].colors[8], this->faces[2].colors[5], this->faces[2].colors[2] });

			//opposite face
			newfaces[1] = this->faces[1];

			//right face
			newfaces[0] = this->faces[0];
			newfaces[0].colors[0] = this->faces[4].colors[0];
			newfaces[0].colors[1] = this->faces[4].colors[1];
			newfaces[0].colors[2] = this->faces[4].colors[2];

			//left face
			newfaces[5] = this->faces[5];
			newfaces[5].colors[6] = this->faces[3].colors[2];
			newfaces[5].colors[7] = this->faces[3].colors[1];
			newfaces[5].colors[8] = this->faces[3].colors[0];

			//up face
			newfaces[3] = this->faces[3];
			newfaces[3].colors[0] = this->faces[0].colors[0];
			newfaces[3].colors[1] = this->faces[0].colors[1];
			newfaces[3].colors[2] = this->faces[0].colors[2];

			//down face
			newfaces[4] = this->faces[4];
			newfaces[4].colors[0] = this->faces[5].colors[8];
			newfaces[4].colors[1] = this->faces[5].colors[7];
			newfaces[4].colors[2] = this->faces[5].colors[6];
			break;

		case 3:
			//front face
			newfaces[3] =
				//1                                 //2                           //3
				MatchedFace(
			{ this->faces[3].colors[6], this->faces[3].colors[3], this->faces[3].colors[0],
			//4                                 //5                           //6 
			this->faces[3].colors[7], this->faces[3].colors[4], this->faces[3].colors[1],
			//7                                 //8                           //9
			this->faces[3].colors[8], this->faces[3].colors[5], this->faces[3].colors[2] });

			//opposite face
			newfaces[4] = this->faces[4];

			//right face
			newfaces[1] = this->faces[1];
			newfaces[1].colors[0] = this->faces[0].colors[0];
			newfaces[1].colors[3] = this->faces[0].colors[3];
			newfaces[1].colors[6] = this->faces[0].colors[6];

			//left face
			newfaces[2] = this->faces[2];
			newfaces[2].colors[0] = this->faces[5].colors[0];
			newfaces[2].colors[3] = this->faces[5].colors[3];
			newfaces[2].colors[6] = this->faces[5].colors[6];

			//up face
			newfaces[5] = this->faces[5];
			newfaces[5].colors[0] = this->faces[1].colors[0];
			newfaces[5].colors[3] = this->faces[1].colors[3];
			newfaces[5].colors[6] = this->faces[1].colors[6];

			//down face
			newfaces[0] = this->faces[0];
			newfaces[0].colors[0] = this->faces[2].colors[0];
			newfaces[0].colors[3] = this->faces[2].colors[3];
			newfaces[0].colors[6] = this->faces[2].colors[6];
			break;

		case 4:
			//front face
			newfaces[4] =
				//1                                 //2                           //3
				MatchedFace(
			{ this->faces[4].colors[6], this->faces[4].colors[3], this->faces[4].colors[0],
			//4                                 //5                           //6 
			this->faces[4].colors[7], this->faces[4].colors[4], this->faces[4].colors[1],
			//7                                 //8                           //9
			this->faces[4].colors[8], this->faces[4].colors[5], this->faces[4].colors[2] });

			//opposite face
			newfaces[3] = this->faces[3];

			//right face
			newfaces[1] = this->faces[1];
			newfaces[1].colors[2] = this->faces[5].colors[2];
			newfaces[1].colors[5] = this->faces[5].colors[5];
			newfaces[1].colors[8] = this->faces[5].colors[8];

			//left face
			newfaces[2] = this->faces[2];
			newfaces[2].colors[2] = this->faces[0].colors[2];
			newfaces[2].colors[5] = this->faces[0].colors[5];
			newfaces[2].colors[8] = this->faces[0].colors[8];

			//up face
			newfaces[0] = this->faces[0];
			newfaces[0].colors[2] = this->faces[1].colors[2];
			newfaces[0].colors[5] = this->faces[1].colors[5];
			newfaces[0].colors[8] = this->faces[1].colors[8];

			//down face
			newfaces[5] = this->faces[5];
			newfaces[5].colors[2] = this->faces[2].colors[2];
			newfaces[5].colors[5] = this->faces[2].colors[5];
			newfaces[5].colors[8] = this->faces[2].colors[8];
			break;

		case 5:
			//front face
			newfaces[5] =
				//1                                 //2                           //3
				MatchedFace(
			{ this->faces[5].colors[6], this->faces[5].colors[3], this->faces[5].colors[0],
			//4                                 //5                           //6 
			this->faces[5].colors[7], this->faces[5].colors[4], this->faces[5].colors[1],
			//7                                 //8                           //9
			this->faces[5].colors[8], this->faces[5].colors[5], this->faces[5].colors[2] });

			//opposite face
			newfaces[0] = this->faces[0];

			//right face
			newfaces[2] = this->faces[2];
			newfaces[2].colors[0] = this->faces[4].colors[2];
			newfaces[2].colors[1] = this->faces[4].colors[5];
			newfaces[2].colors[2] = this->faces[4].colors[8];

			//left face
			newfaces[1] = this->faces[1];
			newfaces[1].colors[6] = this->faces[3].colors[0];
			newfaces[1].colors[7] = this->faces[3].colors[3];
			newfaces[1].colors[8] = this->faces[3].colors[6];

			//up face
			newfaces[3] = this->faces[3];
			newfaces[3].colors[0] = this->faces[2].colors[2];
			newfaces[3].colors[3] = this->faces[2].colors[1];
			newfaces[3].colors[6] = this->faces[2].colors[0];

			//down face
			newfaces[4] = this->faces[4];
			newfaces[4].colors[2] = this->faces[1].colors[8];
			newfaces[4].colors[5] = this->faces[1].colors[7];
			newfaces[4].colors[8] = this->faces[1].colors[6];
			break;
		}
	}

	this->faces = newfaces;
	this->facecolors_char.clear();
	this->numToColor();

	if (count == 2)
	{
		if (cw)
		{
			switch (int_step)
			{
			case 0:
				//front face
				newfaces[0] =
					//1                                 //2                           //3
					MatchedFace(
				{ this->faces[0].colors[2], this->faces[0].colors[5], this->faces[0].colors[8],
				//4                                 //5                           //6 
				this->faces[0].colors[1], this->faces[0].colors[4], this->faces[0].colors[7],
				//7                                 //8                           //9
				this->faces[0].colors[0], this->faces[0].colors[3], this->faces[0].colors[6] });

				//opposite face
				newfaces[5] = this->faces[5];

				//right face
				newfaces[1] = this->faces[1];
				newfaces[1].colors[0] = this->faces[3].colors[2];
				newfaces[1].colors[1] = this->faces[3].colors[5];
				newfaces[1].colors[2] = this->faces[3].colors[8];

				//left face
				newfaces[2] = this->faces[2];
				newfaces[2].colors[6] = this->faces[4].colors[0];
				newfaces[2].colors[7] = this->faces[4].colors[3];
				newfaces[2].colors[8] = this->faces[4].colors[6];

				//up face
				newfaces[3] = this->faces[3];
				newfaces[3].colors[2] = this->faces[2].colors[8];
				newfaces[3].colors[5] = this->faces[2].colors[7];
				newfaces[3].colors[8] = this->faces[2].colors[6];

				//down face
				newfaces[4] = this->faces[4];
				newfaces[4].colors[0] = this->faces[1].colors[2];
				newfaces[4].colors[3] = this->faces[1].colors[1];
				newfaces[4].colors[6] = this->faces[1].colors[0];
				break;

			case 1:
				//front face
				newfaces[1] =
					//1                                 //2                           //3
					MatchedFace(
				{ this->faces[1].colors[2], this->faces[1].colors[5], this->faces[1].colors[8],
				//4                                 //5                           //6 
				this->faces[1].colors[1], this->faces[1].colors[4], this->faces[1].colors[7],
				//7                                 //8                           //9
				this->faces[1].colors[0], this->faces[1].colors[3], this->faces[1].colors[6] });

				//opposite face
				newfaces[2] = this->faces[2];

				//right face
				newfaces[5] = this->faces[5];
				newfaces[5].colors[0] = this->faces[3].colors[8];
				newfaces[5].colors[1] = this->faces[3].colors[7];
				newfaces[5].colors[2] = this->faces[3].colors[6];

				//left face
				newfaces[0] = this->faces[0];
				newfaces[0].colors[6] = this->faces[4].colors[6];
				newfaces[0].colors[7] = this->faces[4].colors[7];
				newfaces[0].colors[8] = this->faces[4].colors[8];

				//up face
				newfaces[3] = this->faces[3];
				newfaces[3].colors[6] = this->faces[0].colors[6];
				newfaces[3].colors[7] = this->faces[0].colors[7];
				newfaces[3].colors[8] = this->faces[0].colors[8];

				//down face
				newfaces[4] = this->faces[4];
				newfaces[4].colors[6] = this->faces[5].colors[2];
				newfaces[4].colors[7] = this->faces[5].colors[1];
				newfaces[4].colors[8] = this->faces[5].colors[0];
				break;

			case 2:
				//front face
				newfaces[2] =
					//1                                 //2                           //3
					MatchedFace(
				{ this->faces[2].colors[2], this->faces[2].colors[5], this->faces[2].colors[8],
				//4                                 //5                           //6 
				this->faces[2].colors[1], this->faces[2].colors[4], this->faces[2].colors[7],
				//7                                 //8                           //9
				this->faces[2].colors[0], this->faces[2].colors[3], this->faces[2].colors[6] });

				//opposite face
				newfaces[1] = this->faces[1];

				//right face
				newfaces[0] = this->faces[0];
				newfaces[0].colors[0] = this->faces[3].colors[0];
				newfaces[0].colors[1] = this->faces[3].colors[1];
				newfaces[0].colors[2] = this->faces[3].colors[2];

				//left face
				newfaces[5] = this->faces[5];
				newfaces[5].colors[6] = this->faces[4].colors[2];
				newfaces[5].colors[7] = this->faces[4].colors[1];
				newfaces[5].colors[8] = this->faces[4].colors[0];

				//up face
				newfaces[3] = this->faces[3];
				newfaces[3].colors[0] = this->faces[5].colors[8];
				newfaces[3].colors[1] = this->faces[5].colors[7];
				newfaces[3].colors[2] = this->faces[5].colors[6];

				//down face
				newfaces[4] = this->faces[4];
				newfaces[4].colors[0] = this->faces[0].colors[0];
				newfaces[4].colors[1] = this->faces[0].colors[1];
				newfaces[4].colors[2] = this->faces[0].colors[2];
				break;

			case 3:
				//front face
				newfaces[3] =
					//1                                 //2                           //3
					MatchedFace(
				{ this->faces[3].colors[2], this->faces[3].colors[5], this->faces[3].colors[8],
				//4                                 //5                           //6 
				this->faces[3].colors[1], this->faces[3].colors[4], this->faces[3].colors[7],
				//7                                 //8                           //9
				this->faces[3].colors[0], this->faces[3].colors[3], this->faces[3].colors[6] });

				//opposite face
				newfaces[4] = this->faces[4];

				//right face
				newfaces[1] = this->faces[1];
				newfaces[1].colors[0] = this->faces[5].colors[0];
				newfaces[1].colors[3] = this->faces[5].colors[3];
				newfaces[1].colors[6] = this->faces[5].colors[6];

				//left face
				newfaces[2] = this->faces[2];
				newfaces[2].colors[0] = this->faces[0].colors[0];
				newfaces[2].colors[3] = this->faces[0].colors[3];
				newfaces[2].colors[6] = this->faces[0].colors[6];

				//up face
				newfaces[5] = this->faces[5];
				newfaces[5].colors[0] = this->faces[2].colors[0];
				newfaces[5].colors[3] = this->faces[2].colors[3];
				newfaces[5].colors[6] = this->faces[2].colors[6];

				//down face
				newfaces[0] = this->faces[0];
				newfaces[0].colors[0] = this->faces[1].colors[0];
				newfaces[0].colors[3] = this->faces[1].colors[3];
				newfaces[0].colors[6] = this->faces[1].colors[6];
				break;

			case 4:
				//front face
				newfaces[4] =
					//1                                 //2                           //3
					MatchedFace(
				{ this->faces[4].colors[2], this->faces[4].colors[5], this->faces[4].colors[8],
				//4                                 //5                           //6 
				this->faces[4].colors[1], this->faces[4].colors[4], this->faces[4].colors[7],
				//7                                 //8                           //9
				this->faces[4].colors[0], this->faces[4].colors[3], this->faces[4].colors[6] });

				//opposite face
				newfaces[3] = this->faces[3];

				//right face
				newfaces[1] = this->faces[1];
				newfaces[1].colors[2] = this->faces[0].colors[2];
				newfaces[1].colors[5] = this->faces[0].colors[5];
				newfaces[1].colors[8] = this->faces[0].colors[8];

				//left face
				newfaces[2] = this->faces[2];
				newfaces[2].colors[2] = this->faces[5].colors[2];
				newfaces[2].colors[5] = this->faces[5].colors[5];
				newfaces[2].colors[8] = this->faces[5].colors[8];

				//up face
				newfaces[0] = this->faces[0];
				newfaces[0].colors[2] = this->faces[2].colors[2];
				newfaces[0].colors[5] = this->faces[2].colors[5];
				newfaces[0].colors[8] = this->faces[2].colors[8];

				//down face
				newfaces[5] = this->faces[5];
				newfaces[5].colors[2] = this->faces[1].colors[2];
				newfaces[5].colors[5] = this->faces[1].colors[5];
				newfaces[5].colors[8] = this->faces[1].colors[8];
				break;

			case 5:
				//front face
				newfaces[5] =
					//1                                 //2                           //3
					MatchedFace(
				{ this->faces[5].colors[2], this->faces[5].colors[5], this->faces[5].colors[8],
				//4                                 //5                           //6 
				this->faces[5].colors[1], this->faces[5].colors[4], this->faces[5].colors[7],
				//7                                 //8                           //9
				this->faces[5].colors[0], this->faces[5].colors[3], this->faces[5].colors[6] });

				//opposite face
				newfaces[0] = this->faces[0];

				//right face
				newfaces[2] = this->faces[2];
				newfaces[2].colors[0] = this->faces[3].colors[6];
				newfaces[2].colors[1] = this->faces[3].colors[3];
				newfaces[2].colors[2] = this->faces[3].colors[0];

				//left face
				newfaces[1] = this->faces[1];
				newfaces[1].colors[6] = this->faces[4].colors[8];
				newfaces[1].colors[7] = this->faces[4].colors[5];
				newfaces[1].colors[8] = this->faces[4].colors[2];

				//up face
				newfaces[3] = this->faces[3];
				newfaces[3].colors[0] = this->faces[1].colors[6];
				newfaces[3].colors[3] = this->faces[1].colors[7];
				newfaces[3].colors[6] = this->faces[1].colors[8];

				//down face
				newfaces[4] = this->faces[4];
				newfaces[4].colors[2] = this->faces[2].colors[0];
				newfaces[4].colors[5] = this->faces[2].colors[1];
				newfaces[4].colors[8] = this->faces[2].colors[2];
				break;
			}
		}
		else
		{
			switch (int_step)
				{
				case 0:
					//front face
					newfaces[0] =
						//1                                 //2                           //3
						MatchedFace(
					{ this->faces[0].colors[6], this->faces[0].colors[3], this->faces[0].colors[0],
					//4                                 //5                           //6 
					this->faces[0].colors[7], this->faces[0].colors[4], this->faces[0].colors[1],
					//7                                 //8                           //9
					this->faces[0].colors[8], this->faces[0].colors[5], this->faces[0].colors[2] });

					//opposite face
					newfaces[5] = this->faces[5];

					//right face
					newfaces[1] = this->faces[1];
					newfaces[1].colors[0] = this->faces[4].colors[6];
					newfaces[1].colors[1] = this->faces[4].colors[3];
					newfaces[1].colors[2] = this->faces[4].colors[0];

					//left face
					newfaces[2] = this->faces[2];
					newfaces[2].colors[6] = this->faces[3].colors[8];
					newfaces[2].colors[7] = this->faces[3].colors[5];
					newfaces[2].colors[8] = this->faces[3].colors[2];

					//up face
					newfaces[3] = this->faces[3];
					newfaces[3].colors[2] = this->faces[1].colors[0];
					newfaces[3].colors[5] = this->faces[1].colors[1];
					newfaces[3].colors[8] = this->faces[1].colors[2];

					//down face
					newfaces[4] = this->faces[4];
					newfaces[4].colors[0] = this->faces[2].colors[6];
					newfaces[4].colors[3] = this->faces[2].colors[7];
					newfaces[4].colors[6] = this->faces[2].colors[8];
					break;

				case 1:
					//front face
					newfaces[1] =
						//1                                 //2                           //3
						MatchedFace(
					{ this->faces[1].colors[6], this->faces[1].colors[3], this->faces[1].colors[0],
					//4                                 //5                           //6 
					this->faces[1].colors[7], this->faces[1].colors[4], this->faces[1].colors[1],
					//7                                 //8                           //9
					this->faces[1].colors[8], this->faces[1].colors[5], this->faces[1].colors[2] });

					//opposite face
					newfaces[2] = this->faces[2];

					//right face
					newfaces[5] = this->faces[5];
					newfaces[5].colors[0] = this->faces[4].colors[8];
					newfaces[5].colors[1] = this->faces[4].colors[7];
					newfaces[5].colors[2] = this->faces[4].colors[6];

					//left face
					newfaces[0] = this->faces[0];
					newfaces[0].colors[6] = this->faces[3].colors[6];
					newfaces[0].colors[7] = this->faces[3].colors[7];
					newfaces[0].colors[8] = this->faces[3].colors[8];

					//up face
					newfaces[3] = this->faces[3];
					newfaces[3].colors[6] = this->faces[5].colors[2];
					newfaces[3].colors[7] = this->faces[5].colors[1];
					newfaces[3].colors[8] = this->faces[5].colors[0];

					//down face
					newfaces[4] = this->faces[4];
					newfaces[4].colors[6] = this->faces[0].colors[6];
					newfaces[4].colors[7] = this->faces[0].colors[7];
					newfaces[4].colors[8] = this->faces[0].colors[8];
					break;

				case 2:
					//front face
					newfaces[2] =
						//1                                 //2                           //3
						MatchedFace(
					{ this->faces[2].colors[6], this->faces[2].colors[3], this->faces[2].colors[0],
					//4                                 //5                           //6 
					this->faces[2].colors[7], this->faces[2].colors[4], this->faces[2].colors[1],
					//7                                 //8                           //9
					this->faces[2].colors[8], this->faces[2].colors[5], this->faces[2].colors[2] });

					//opposite face
					newfaces[1] = this->faces[1];

					//right face
					newfaces[0] = this->faces[0];
					newfaces[0].colors[0] = this->faces[4].colors[0];
					newfaces[0].colors[1] = this->faces[4].colors[1];
					newfaces[0].colors[2] = this->faces[4].colors[2];

					//left face
					newfaces[5] = this->faces[5];
					newfaces[5].colors[6] = this->faces[3].colors[2];
					newfaces[5].colors[7] = this->faces[3].colors[1];
					newfaces[5].colors[8] = this->faces[3].colors[0];

					//up face
					newfaces[3] = this->faces[3];
					newfaces[3].colors[0] = this->faces[0].colors[0];
					newfaces[3].colors[1] = this->faces[0].colors[1];
					newfaces[3].colors[2] = this->faces[0].colors[2];

					//down face
					newfaces[4] = this->faces[4];
					newfaces[4].colors[0] = this->faces[5].colors[8];
					newfaces[4].colors[1] = this->faces[5].colors[7];
					newfaces[4].colors[2] = this->faces[5].colors[6];
					break;

				case 3:
					//front face
					newfaces[3] =
						//1                                 //2                           //3
						MatchedFace(
					{ this->faces[3].colors[6], this->faces[3].colors[3], this->faces[3].colors[0],
					//4                                 //5                           //6 
					this->faces[3].colors[7], this->faces[3].colors[4], this->faces[3].colors[1],
					//7                                 //8                           //9
					this->faces[3].colors[8], this->faces[3].colors[5], this->faces[3].colors[2] });

					//opposite face
					newfaces[4] = this->faces[4];

					//right face
					newfaces[1] = this->faces[1];
					newfaces[1].colors[0] = this->faces[0].colors[0];
					newfaces[1].colors[3] = this->faces[0].colors[3];
					newfaces[1].colors[6] = this->faces[0].colors[6];

					//left face
					newfaces[2] = this->faces[2];
					newfaces[2].colors[0] = this->faces[5].colors[0];
					newfaces[2].colors[3] = this->faces[5].colors[3];
					newfaces[2].colors[6] = this->faces[5].colors[6];

					//up face
					newfaces[5] = this->faces[5];
					newfaces[5].colors[0] = this->faces[1].colors[0];
					newfaces[5].colors[3] = this->faces[1].colors[3];
					newfaces[5].colors[6] = this->faces[1].colors[6];

					//down face
					newfaces[0] = this->faces[0];
					newfaces[0].colors[0] = this->faces[2].colors[0];
					newfaces[0].colors[3] = this->faces[2].colors[3];
					newfaces[0].colors[6] = this->faces[2].colors[6];
					break;

				case 4:
					//front face
					newfaces[4] =
						//1                                 //2                           //3
						MatchedFace(
					{ this->faces[4].colors[6], this->faces[4].colors[3], this->faces[4].colors[0],
					//4                                 //5                           //6 
					this->faces[4].colors[7], this->faces[4].colors[4], this->faces[4].colors[1],
					//7                                 //8                           //9
					this->faces[4].colors[8], this->faces[4].colors[5], this->faces[4].colors[2] });

					//opposite face
					newfaces[3] = this->faces[3];

					//right face
					newfaces[1] = this->faces[1];
					newfaces[1].colors[2] = this->faces[5].colors[2];
					newfaces[1].colors[5] = this->faces[5].colors[5];
					newfaces[1].colors[8] = this->faces[5].colors[8];

					//left face
					newfaces[2] = this->faces[2];
					newfaces[2].colors[2] = this->faces[0].colors[2];
					newfaces[2].colors[5] = this->faces[0].colors[5];
					newfaces[2].colors[8] = this->faces[0].colors[8];

					//up face
					newfaces[0] = this->faces[0];
					newfaces[0].colors[2] = this->faces[1].colors[2];
					newfaces[0].colors[5] = this->faces[1].colors[5];
					newfaces[0].colors[8] = this->faces[1].colors[8];

					//down face
					newfaces[5] = this->faces[5];
					newfaces[5].colors[2] = this->faces[2].colors[2];
					newfaces[5].colors[5] = this->faces[2].colors[5];
					newfaces[5].colors[8] = this->faces[2].colors[8];
					break;

				case 5:
					//front face
					newfaces[5] =
						//1                                 //2                           //3
						MatchedFace(
					{ this->faces[5].colors[6], this->faces[5].colors[3], this->faces[5].colors[0],
					//4                                 //5                           //6 
					this->faces[5].colors[7], this->faces[5].colors[4], this->faces[5].colors[1],
					//7                                 //8                           //9
					this->faces[5].colors[8], this->faces[5].colors[5], this->faces[5].colors[2] });

					//opposite face
					newfaces[0] = this->faces[0];

					//right face
					newfaces[2] = this->faces[2];
					newfaces[2].colors[0] = this->faces[4].colors[2];
					newfaces[2].colors[1] = this->faces[4].colors[5];
					newfaces[2].colors[2] = this->faces[4].colors[8];

					//left face
					newfaces[1] = this->faces[1];
					newfaces[1].colors[6] = this->faces[3].colors[0];
					newfaces[1].colors[7] = this->faces[3].colors[3];
					newfaces[1].colors[8] = this->faces[3].colors[6];

					//up face
					newfaces[3] = this->faces[3];
					newfaces[3].colors[0] = this->faces[2].colors[2];
					newfaces[3].colors[3] = this->faces[2].colors[1];
					newfaces[3].colors[6] = this->faces[2].colors[0];

					//down face
					newfaces[4] = this->faces[4];
					newfaces[4].colors[2] = this->faces[1].colors[8];
					newfaces[4].colors[5] = this->faces[1].colors[7];
					newfaces[4].colors[8] = this->faces[1].colors[6];
					break;
				}
		}
	}

	this->faces = newfaces;
	this->facecolors_char.clear();
	this->numToColor();
}


ColorFace::ColorFace()
{

}



tuple<tuple<int,int>,tuple<int,int>> neighbors(int f, int s)
{
	tuple<int, int> a;
	tuple<int, int> b;
	if (f == 0 && s == 0) { a = make_tuple( 2, 6 ); b = make_tuple( 3, 2 );}
	if (f == 0 && s == 1) { a = make_tuple( 2, 7 ); b = make_tuple(-1, -1);}
	if (f == 0 && s == 2) { a = make_tuple( 2, 8 ); b = make_tuple( 4, 0 );}
	if (f == 0 && s == 3) { a = make_tuple( 3, 5 ); b = make_tuple( -1, -1 );}
	if (f == 0 && s == 5) { a = make_tuple( 4, 3 ); b = make_tuple( -1, -1 );}
	if (f == 0 && s == 6) { a = make_tuple( 3, 8 ); b = make_tuple( 1, 0 );}
	if (f == 0 && s == 7) { a = make_tuple( 1, 1 ); b = make_tuple( -1, -1 );}
	if (f == 0 && s == 8) { a = make_tuple( 1, 2 ); b = make_tuple( 4, 6 );}

	if (f == 1 && s == 0) { a = make_tuple( 0, 6 ); b = make_tuple( 3, 8 );}
	if (f == 1 && s == 1) { a = make_tuple( 0, 7 ); b = make_tuple( -1, -1 );}
	if (f == 1 && s == 2) { a = make_tuple( 0, 8 ); b = make_tuple( 4, 6 );}
	if (f == 1 && s == 3) { a = make_tuple( 3, 7 ); b = make_tuple( -1, -1 );}
	if (f == 1 && s == 5) { a = make_tuple( 4, 7 ); b = make_tuple( -1, -1 );}
	if (f == 1 && s == 6) { a = make_tuple( 3, 6 ); b = make_tuple( 5, 0 );}
	if (f == 1 && s == 7) { a = make_tuple( 5, 1 ); b = make_tuple(-1, -1);}
	if (f == 1 && s == 8) { a = make_tuple( 4, 8 ); b = make_tuple(5, 2);}

	if (f == 2 && s == 0) { a = make_tuple(3, 0); b = make_tuple(5, 6);}
	if (f == 2 && s == 1) { a = make_tuple(5, 7); b = make_tuple(-1, -1);}
	if (f == 2 && s == 2) { a = make_tuple(4, 2); b = make_tuple(5, 8);}
	if (f == 2 && s == 3) { a = make_tuple(3, 1); b = make_tuple(-1, -1);}
	if (f == 2 && s == 5) { a = make_tuple(4, 1); b = make_tuple(-1, -1);}
	if (f == 2 && s == 6) { a = make_tuple(0, 0); b = make_tuple(3, 2);}
	if (f == 2 && s == 7) { a = make_tuple(0, 1); b = make_tuple(-1, -1);}
	if (f == 2 && s == 8) { a = make_tuple(0, 2); b = make_tuple(4, 0);}

	if (f == 3 && s == 0) {a = make_tuple(2, 0); b = make_tuple(5, 6);}
	if (f == 3 && s == 1) {a = make_tuple(2, 3); b = make_tuple(-1, -1);}
	if (f == 3 && s == 2) {a = make_tuple(0, 0); b = make_tuple(2, 6);}
	if (f == 3 && s == 3) {a = make_tuple(5, 3); b = make_tuple(-1, -1);}
	if (f == 3 && s == 5) {a = make_tuple(0, 3); b = make_tuple(-1, -1);}
	if (f == 3 && s == 6) {a = make_tuple(1, 6); b = make_tuple(5, 0);}
	if (f == 3 && s == 7) {a = make_tuple(1, 3); b = make_tuple(-1, -1);}
	if (f == 3 && s == 8) {a = make_tuple(0, 6); b = make_tuple(1, 0);}

	if (f == 4 && s == 0) {a = make_tuple(0, 2); b = make_tuple(2, 8);}
	if (f == 4 && s == 1) {a = make_tuple(2, 5); b = make_tuple(-1, -1);}
	if (f == 4 && s == 2) {a = make_tuple(2, 2); b = make_tuple(5, 8);}
	if (f == 4 && s == 3) {a = make_tuple(0, 5); b = make_tuple(-1, -1);}
	if (f == 4 && s == 5) {a = make_tuple(5, 5); b = make_tuple(-1, -1);}
	if (f == 4 && s == 6) {a = make_tuple(0, 8); b = make_tuple(1, 2);}
	if (f == 4 && s == 7) {a = make_tuple(1, 5); b = make_tuple(-1, -1);}
	if (f == 4 && s == 8) {a = make_tuple(1, 8); b = make_tuple(5, 2);}

	if (f == 5 && s == 0) {a = make_tuple(3, 6); b = make_tuple(1, 6);}
	if (f == 5 && s == 1) {a = make_tuple(1, 7); b = make_tuple(-1, -1);}
	if (f == 5 && s == 2) {a = make_tuple(1, 8); b = make_tuple(4, 8);}
	if (f == 5 && s == 3) {a = make_tuple(3, 3); b = make_tuple(-1, -1);}
	if (f == 5 && s == 5) {a = make_tuple(4, 5); b = make_tuple(-1, -1);}
	if (f == 5 && s == 6) {a = make_tuple(2, 0); b = make_tuple(3, 0);}
	if (f == 5 && s == 7) {a = make_tuple(2, 1); b = make_tuple(-1, -1);}
	if (f == 5 && s == 8) {a = make_tuple(2, 2); b = make_tuple(4, 2);}
	tuple<tuple<int,int>, tuple<int,int>> neighbor(a,b);
	return neighbor;
}



Scalar colavg(Mat src, Point2f point, float dist)
{
	Scalar col, sum;
	vector<Point2f> points = pointcube(point, dist);
	for (size_t i = 0; i < points.size(); ++i)
	{
		sum += Scalar(src.at<Vec3b>(points[i]));
	}
	col = Scalar(sum[0]/9,sum[1]/9,sum[2]/9);
	return col;
}