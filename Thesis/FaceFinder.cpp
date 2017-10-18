#include "FaceFinder.h"


float roundf(float val)
{
	return val >= 0.0f ? floorf(val + 0.5f) : ceilf(val - 0.5f);
}

vector<Point> FindIntersectionSpots (vector<Line> lines, int p_offset, float l_offset)
{
	vector <Point> SamePoints;
//remove same lines
	/*
	for (size_t i=0; i<lines.size(); i++)
	{
		Line l(lines[i].x1,lines[i].y1,lines[i].x2,lines[i].y2);
		lines.erase(remove_if(lines.begin()+i, lines.end(),[&l,s_offset,l_offset] (Line &other) {return l.same_as(other,s_offset,l_offset);}));
	}
	*/
	for( size_t i = 0; i < lines.size(); i++ )
    {
		
		for (size_t j = i+1; j < lines.size(); j++)
		{
			if (lines[i].same_as(lines[j], p_offset, l_offset)) break;
 //vertical lines
			if (PerpLines(lines[i],lines[j],l_offset))
			{
 //only start points
				Point A=Point(lines[i].x1,lines[i].y1);
				Point B=Point(lines[i].x2,lines[i].y2);
				Point C=Point(lines[j].x1,lines[j].y1);
				Point D=Point(lines[j].x2,lines[j].y2);
				if (SamePoint(Point(lines[i].x1,lines[i].y1),Point(lines[j].x1,lines[j].y1),p_offset)||
					SamePoint(Point(lines[i].x2,lines[i].y2),Point(lines[j].x1,lines[j].y1),p_offset))
				{
					SamePoints.push_back(Point(lines[j].x1,lines[j].y1));
					break;
				}
				else if (SamePoint(Point(lines[i].x1,lines[i].y1),Point(lines[j].x2,lines[j].y2),p_offset)||
						SamePoint(Point(lines[i].x2,lines[i].y2),Point(lines[j].x2,lines[j].y2),p_offset)) 
				{
					SamePoints.push_back(Point(lines[j].x2,lines[j].y2));
					break;
				}
			
	// intersections
				Point a;
				//if(LineIntersection2(A, B, C, D, a)) SamePoints.push_back(a);	
				if(intersection3(A, B, C, D, a)) SamePoints.push_back(a);	
			}
		}
	}
	return SamePoints;
}

vector<Square> FindSquares(vector <Point> points, float offset)
{
	vector<Square> squares;
	multimap<float, array<Point,2>> distances;
	for (size_t i=0;i<points.size();i++)
	{
		for (size_t j=i+1;j<points.size();j++)
		{
			float dist=Distance(points[i],points[j]);
			dist=roundf(dist);
			array<Point,2> pointa;
			pointa[0]=points[i];
			pointa[1]=points[j];
			distances.insert(pair<float,array<Point,2>>(dist,pointa));
		}
	}
	for (multimap<float, array<Point,2>>::iterator it = distances.begin(); it!=distances.end(); ++it)
	{
		float dist = (*it).first;
		if  (distances.count(dist)<4) break;
		pair<multimap<float,array<Point,2>>::iterator,multimap<float,array<Point,2>>::iterator> range = distances.equal_range(dist);
		for (multimap<float, array<Point,2>>::iterator it2 = range.first; it2!=range.second ; ++it2)
		{
			if (isSquare((*it).second,(*it2).second,(*it).first,offset)) 
				squares.push_back(Square((*it).second[0],(*it).second[1],(*it2).second[0],(*it2).second[1]));
		}
	}
	return squares;
}

vector<Square> FindSquares(vector <Line> lines, int p_offset, float l_offset)
{
	vector<Square> squares;
	multimap<float, array<Point,3> > points;
	for (size_t i=0;i<lines.size();i++)
	{
		for (size_t j=i+1;j<lines.size();j++)
		{
			Point a;
			if (max(lines[i].length(),lines[j].length())/min(lines[i].length(),lines[j].length())<1.3)
			{
				if (PerpLines(lines[i],lines[j],l_offset))
				{					
					if (SamePoint(Point(lines[i].x1,lines[i].y1),Point(lines[j].x1,lines[j].y1),p_offset)) 
					{
						array<Point,3> points_a;
						points_a[0]=Point(lines[i].x1,lines[i].y1);
						points_a[1]=Point(lines[i].x2,lines[i].y2);
						points_a[2]=Point(lines[j].x2,lines[j].y2);
						float dist=Distance(points_a[1],points_a[2]);
						points.insert(pair<float,array<Point,3> >(dist,points_a));
					}
					else if	(SamePoint(Point(lines[i].x1,lines[i].y1),Point(lines[j].x2,lines[j].y2),p_offset)) 
					{
						array<Point,3> points_a;
						points_a[0]=Point(lines[i].x1,lines[i].y1);
						points_a[1]=Point(lines[i].x2,lines[i].y2);
						points_a[2]=Point(lines[j].x1,lines[j].y1);
						float dist=Distance(points_a[1],points_a[2]);
						points.insert(pair<float,array<Point,3> >(dist,points_a));
					}
					else if	(SamePoint(Point(lines[i].x2,lines[i].y2),Point(lines[j].x1,lines[j].y1),p_offset))
					{
						array<Point,3> points_a;
						points_a[0]=Point(lines[i].x2,lines[i].y2);
						points_a[1]=Point(lines[i].x1,lines[i].y1);
						points_a[2]=Point(lines[j].x2,lines[j].y2);
						float dist=Distance(points_a[1],points_a[2]);
						points.insert(pair<float,array<Point,3> >(dist,points_a));
					}
					else if	(SamePoint(Point(lines[i].x2,lines[i].y2),Point(lines[j].x2,lines[j].y2),p_offset)) 
					{
						array<Point,3> points_a;
						points_a[0]=Point(lines[i].x2,lines[i].y2);
						points_a[1]=Point(lines[i].x1,lines[i].y1);
						points_a[2]=Point(lines[j].x1,lines[j].y1);
						float dist=Distance(points_a[1],points_a[2]);
						points.insert(pair<float,array<Point,3> >(dist,points_a));
					}					
				}
			}
			if (intersection3(lines[i].getPoint(1),lines[i].getPoint(2),lines[j].getPoint(1),lines[j].getPoint(2),a))
			{
				if (PerpLines(lines[i],lines[j],l_offset))
				{					
					if (max(Distance(lines[i].getPoint(1),a),Distance(lines[j].getPoint(1),a))/min(Distance(lines[i].getPoint(1),a),Distance(lines[j].getPoint(1),a))<1.3)
					{
						array<Point,3> points_a;
						points_a[0]=a;
						points_a[1]=lines[i].getPoint(1);
						points_a[2]=lines[j].getPoint(1);
						float dist=Distance(points_a[1],points_a[2]);
						points.insert(pair<float,array<Point,3> >(dist,points_a));
					}
					else if (max(Distance(lines[i].getPoint(1),a),Distance(lines[j].getPoint(2),a))/min(Distance(lines[i].getPoint(1),a),Distance(lines[j].getPoint(2),a))<1.3)
					{
						array<Point,3> points_a;
						points_a[0]=a;
						points_a[1]=lines[i].getPoint(1);
						points_a[2]=lines[j].getPoint(2);
						float dist=Distance(points_a[1],points_a[2]);
						points.insert(pair<float,array<Point,3> >(dist,points_a));
					}
					else if (max(Distance(lines[i].getPoint(2),a),Distance(lines[j].getPoint(1),a))/min(Distance(lines[i].getPoint(2),a),Distance(lines[j].getPoint(1),a))<1.3)
					{
						array<Point,3> points_a;
						points_a[0]=a;
						points_a[1]=lines[i].getPoint(2);
						points_a[2]=lines[j].getPoint(1);
						float dist=Distance(points_a[1],points_a[2]);
						points.insert(pair<float,array<Point,3> >(dist,points_a));
					}
					else if (max(Distance(lines[i].getPoint(2),a),Distance(lines[j].getPoint(2),a))/min(Distance(lines[i].getPoint(2),a),Distance(lines[j].getPoint(2),a))<1.3)
					{
						array<Point,3> points_a;
						points_a[0]=a;
						points_a[1]=lines[i].getPoint(2);
						points_a[2]=lines[j].getPoint(2);
						float dist=Distance(points_a[1],points_a[2]);
						points.insert(pair<float,array<Point,3> >(dist,points_a));
					}
				}
			}
		}
	}
	for (multimap<float, array<Point,3>>::iterator it = points.begin(); it!=points.end(); ++it)
	{
		float dist = (*it).first;
		multimap<float, array<Point,3>>::iterator itlow=points.lower_bound(dist-10);
		multimap<float, array<Point,3>>::iterator ithi=points.upper_bound(dist+10);		
		//pair<multimap<float,array<Point,3>>::iterator,multimap<float,array<Point,3>>::iterator> range = points.equal_range(dist);
		//for (multimap<float, array<Point,3>>::iterator it2 = range.first; it2!=range.second ; ++it2)
		for (multimap<float, array<Point,3>>::iterator it2 = itlow; it2!=ithi ; ++it2)
		{
			Point points1[3];
			Point points2[3];
			copy(begin((*it).second),end((*it).second),begin(points1));
			copy(begin((*it2).second),end((*it2).second),begin(points2));
			if (isSquare2(points1,points2,dist,Distance((*it).second[0],(*it2).second[0]),p_offset))
			{
				Point A=(*it).second[1];
				Point B=(*it).second[0];
				Point C=(*it).second[2];
				Point D=(*it2).second[0];
				Square *square1 = new Square(A,B,C,D);
				int area=square1->Area();
				if (area>50) squares.push_back(*square1);
			}
			
		}
	}
	return squares;
}

vector <Square> FindGridSquares(vector<Square> squares, float offset1)
{
	vector<Square> gridsquares;
	for (size_t i=0;i<squares.size();++i)
	{ 
		int detects=0;
		vector<Square> tempsquares;
		for (size_t j=i+1;j<squares.size();++j)
		{
			tempsquares.clear();
			if ((VerticalLines(Line(squares[i].getPoint(1),squares[i].getPoint(2)),Line(squares[j].getPoint(1),squares[j].getPoint(2)),offset1) ||
				VerticalLines(Line(squares[i].getPoint(2),squares[i].getPoint(3)),Line(squares[j].getPoint(1),squares[j].getPoint(2)),offset1)) &&
				abs(squares[i].Area()-squares[j].Area())<1000 && 
				absDifference(squares[i].getCenter(),squares[j].getCenter())>=squares[i].sideSize() &&
				abs(squares[i].sideSize()-squares[j].sideSize())<20)
			{
				detects++;
				tempsquares.push_back(squares[j]);
			}
			tempsquares.push_back(squares[i]);
			if (detects>6) 
			{
				for (size_t j=0; j<tempsquares.size(); ++j)
				{
					gridsquares.push_back(tempsquares[j]);						
				}
			}
		}
	}
	if (gridsquares.size()>5) cout<<"Success! \n";
	return gridsquares;
}

vector<Line> gridLines(vector<Line> lines, float offset)
{
	vector<Line> gridlines;
	int detects1,detects2;
	for (size_t i=0; i<lines.size();++i)
	{
		detects1=detects2=0;
		for (size_t j=i+1;j<lines.size();++j)
		{
			if (
				CheckParallel(lines[i],lines[j]) && 
				max(lines[i].length(),lines[j].length())/min(lines[i].length(),lines[j].length())<1.2 
				//max(lines[i].length(),Distance(lines[i],lines[j]))/min(lines[i].length(),Distance(lines[i],lines[j]))<1.2
				)
			{
				detects1++;
			}
		}
		if (detects1>2) 
		{
			for (size_t j=i+1;j<lines.size();++j)
			{
				if (
					VerticalLines(lines[i],lines[j],offset) && 
					max(lines[i].length(),lines[j].length())/min(lines[i].length(),lines[j].length())<1.2 &&
					intersection3(lines[i].getPoint(1),lines[i].getPoint(2),lines[j].getPoint(1),lines[j].getPoint(2))
					) 
				{
					detects2++;
				}
			}
		}
		if (detects2>2) gridlines.push_back(lines[i]);
	}
	if (gridlines.size()>=4) cout<<"Success! \n\a";
	return gridlines;

}

void ShowFace(Mat src, vector<Point> points)
{
	bool hsv=true;
	Mat src1;
	if (hsv) cvtColor(src,src1,CV_BGR2HSV);
	else cvtColor(src,src1,CV_BGR2HLS);
	Vec3b topleft=src1.at<Vec3b>(points[0]);
	Vec3b midtop=src1.at<Vec3b>(Point((points[0].x+points[1].x)/2,points[0].y));
	Vec3b topright=src1.at<Vec3b>(Point(points[1].x,points[0].y));
	Vec3b midleft=src1.at<Vec3b>(Point(points[0].x,(points[1].y+points[1].y)/2));
	Vec3b mid=src1.at<Vec3b>(Point((points[0].x+points[1].x)/2,(points[0].y+points[1].y)/2));
	Vec3b midright=src1.at<Vec3b>(Point(points[1].x,(points[0].y+points[1].y)/2));
	Vec3b leftbot=src1.at<Vec3b>(Point(points[0].x,points[1].y));
	Vec3b midbot=src1.at<Vec3b>(Point((points[0].x+points[1].x)/2,points[1].y));
	Vec3b rightbot=src1.at<Vec3b>(points[1]);
	if (hsv)
	{
		cout<<ColorMatcherHSV(topleft)<<"|"<<ColorMatcherHSV(midtop)<<"|"<<ColorMatcherHSV(topright)<<"\n";
		cout<<ColorMatcherHSV(midleft)<<"|"<<ColorMatcherHSV(mid)<<"|"<<ColorMatcherHSV(midright)<<"\n";
		cout<<ColorMatcherHSV(leftbot)<<"|"<<ColorMatcherHSV(midbot)<<"|"<<ColorMatcherHSV(rightbot)<<"\n";
	}
	else 
	{
		cout<<ColorMatcherHLS(topleft)<<"|"<<ColorMatcherHLS(midtop)<<"|"<<ColorMatcherHLS(topright)<<"\n";
		cout<<ColorMatcherHLS(midleft)<<"|"<<ColorMatcherHLS(mid)<<"|"<<ColorMatcherHLS(midright)<<"\n";
		cout<<ColorMatcherHLS(leftbot)<<"|"<<ColorMatcherHLS(midbot)<<"|"<<ColorMatcherHLS(rightbot)<<"\n";
	}

}

void ShowFaceCorner(Mat src, vector<Point> points)
{
	// botright 0, botleft 1, topleft 2, topright 3
	Mat src1;
	Vec3b topleft1 = src.at<Vec3b>(points[2]);
	Vec3b midtop1 = src.at<Vec3b>(Point((points[2].x + points[3].x) / 2, (points[2].y + points[3].y) / 2));
	Vec3b topright1 = src.at<Vec3b>(Point(points[3].x, points[3].y));
	Vec3b midleft1 = src.at<Vec3b>(Point((points[1].x + points[2].x) / 2, (points[1].y + points[2].y) / 2));
	Vec3b mid1 = src.at<Vec3b>(Point((points[0].x + points[2].x) / 2, (points[0].y + points[2].y) / 2));
	Vec3b midright1 = src.at<Vec3b>(Point((points[3].x + points[0].x) / 2, (points[0].y + points[3].y) / 2));
	Vec3b leftbot1 = src.at<Vec3b>(Point(points[1].x, points[1].y));
	Vec3b midbot1 = src.at<Vec3b>(Point((points[1].x + points[0].x) / 2, (points[1].y + points[0].y) / 2));
	Vec3b rightbot1 = src.at<Vec3b>(points[0]);
	cvtColor(src, src1, CV_BGR2HSV);
	Vec3b topleft = src1.at<Vec3b>(points[2]);
	Vec3b midtop = src1.at<Vec3b>(Point((points[2].x + points[3].x) / 2, (points[2].y + points[3].y) / 2));
	Vec3b topright = src1.at<Vec3b>(Point(points[3].x, points[3].y));
	Vec3b midleft = src1.at<Vec3b>(Point((points[1].x + points[2].x) / 2, (points[1].y + points[2].y) / 2));
	Vec3b mid = src1.at<Vec3b>(Point((points[0].x + points[2].x) / 2, (points[0].y + points[2].y) / 2));
	Vec3b midright = src1.at<Vec3b>(Point((points[3].x + points[0].x) / 2, (points[0].y + points[3].y) / 2));
	Vec3b leftbot = src1.at<Vec3b>(Point(points[1].x, points[1].y));
	Vec3b midbot = src1.at<Vec3b>(Point((points[1].x + points[0].x) / 2, (points[1].y + points[0].y) / 2));
	Vec3b rightbot = src1.at<Vec3b>(points[0]);
	cout << ColorMatcherMulti(topleft, topleft1) << "|" << ColorMatcherMulti(midtop, midtop1) << "|" << ColorMatcherMulti(topright, topright1) << "\n";
	cout << ColorMatcherMulti(midleft, midleft1) << "|" << ColorMatcherMulti(mid, mid1) << "|" << ColorMatcherMulti(midright, midright1) << "\n";
	cout << ColorMatcherMulti(leftbot, leftbot1) << "|" << ColorMatcherMulti(midbot, midbot1) << "|" << ColorMatcherMulti(rightbot, rightbot1) << "\n";
}


float middle(float a, float b, float c)
{
	return a>b ? (c>a ? a : (b>c ? b : c)) : (c>b ? b : (a>c ? a : c));
}

string ColorMatcher(Vec3b values, int maxval[])
{
	//B = 0 | G = 1 | R = 2
	//BGR values below
	//RED(0,0,255) 0, GREEN (68,154,0) 0.44, BLUE(155,61,0) 0.39, ORANGE(0,80,254) 0.31, YELLOW (0,215,255) 0.84, WHITE (255,255,255) 1
	float max1=max(values[0],max(values[1],values[2]));
	float div=middle(values[0],values[1],values[2])/max1;
	if (div<0.3) return "red";
	else if (div<0.39) return "orange";
	else if (div<0.45) return "blue";
	else if (div<0.85) return "green";
	else if (div<1) return "yellow";
	else return "white";

}

string ColorMatcherHSV(Vec3b values)
{
	if (values.val[1]<20 || values.val[2]>230) return "white";
	if (values.val[0]<10 || values.val[0]>150) return "red";
	else if (values.val[0]<15) return "orange";
	else if (values.val[0]<45) return "yellow";
	else if (values.val[0]<100) return "green";
	else if (values.val[0]<150) return "blue";

}

string ColorMatcherHLS(Vec3b values)
{
	if (values.val[2]>95) return "white";
	if (values.val[0]<5 || values.val[0]>355) return "red";
	else if (values.val[0]<39) return "orange";
	else if (values.val[0]<60) return "yellow";
	else if (values.val[0]<120) return "green";
	else if (values.val[0]<240) return "blue";
}

Scalar ColorHSV(Vec3b values)
{
	if (values.val[2]>95) return Scalar(255,255,255); //white
	if (values.val[0]<5 || values.val[0]>355) return Scalar(0,0,255); //red
	else if (values.val[0]<39) return Scalar(0,105,255); //orange
	else if (values.val[0]<60) return Scalar(0,252,215); //yellow
	else if (values.val[0]<120) return Scalar(0,255,0); //green
	else if (values.val[0]<240) return Scalar(255,0,0); //blue
}

string ColorMatcherMulti(Vec3b values, Vec3b values1)
{
	int maxrgb = max(max(values1.val[0], values.val[1]), values.val[2]);
	int minrgb = min(min(values1.val[0], values.val[1]), values.val[2]);
	int midrgb = middle(values1.val[0], values.val[1], values.val[2]);
	if (maxrgb / midrgb < 0.1 && midrgb / minrgb < 0.1) return "white";
	if (values.val[0]<10 || values.val[0]>150) return "red";
	else if (values.val[0]<15) return "orange";
	else if (values.val[0]<45) return "yellow";
	else if (values.val[0]<100) return "green";
	else if (values.val[0]<150) return "blue";
}

vector<Point2f> FindCubeCorners(vector<Point> points)
{
	//MUST FIX
	vector<float> distance;
	for (size_t i = 0; i < points.size(); i++)
	{
		for (size_t j = i + 1; j < points.size(); j++)
		{
			distance.push_back(Distance(points[i], points[j]));
		}
	}
	float mindist = distance[0];
	for (size_t i = 1; i < distance.size(); i++)
	{
		if (distance[i] < mindist) mindist = distance[i];
	}
	mindist = mindist/4;
	int minx, miny, maxx, maxy;
	minx = maxx = points[0].x;
	miny = maxy = points[0].y;
	for (size_t i = 0; i < points.size(); i++)
	{
		if (points[i].x>maxx) maxx = points[i].x;
		if (points[i].x>maxy) maxy = points[i].y;
		if (points[i].x<minx) minx = points[i].x;
		if (points[i].x<miny) miny = points[i].y;
	}
	Point2f sticker0 = Point(minx, maxy);
	Point2f sticker1 = Point(maxx, maxy);
	Point2f sticker2 = Point(maxx, miny);
	Point2f sticker3 = Point(minx, miny);
	Point2f diag1, diag2;
	diag1.x = sticker0.x - sticker2.x;
	diag1.y = sticker0.y - sticker2.y;
	diag2.x = sticker1.x - sticker3.x;
	diag2.y = sticker1.y - sticker3.y;
	Point2f absdiag1, absdiag2;
	absdiag1.x = diag1.x / sqrt(diag1.x*diag1.x + diag1.y*diag1.y);
	absdiag1.y = diag1.y / sqrt(diag1.x*diag1.x + diag1.y*diag1.y);
	absdiag2.x = diag2.x / sqrt(diag2.x*diag2.x + diag2.y*diag2.y);
	absdiag2.y = diag2.y / sqrt(diag2.x*diag2.x + diag2.y*diag2.y);
	Point2f corner0 = sticker0 + mindist*absdiag1;
	Point2f corner1 = sticker1 + mindist*absdiag2;
	Point2f corner2 = sticker2 - mindist*absdiag1;
	Point2f corner3 = sticker3 - mindist*absdiag2;
	vector<Point2f> corner_points;
	corner_points.push_back(corner0);
	corner_points.push_back(corner1);
	corner_points.push_back(corner2);
	corner_points.push_back(corner3);
	return corner_points;
}

vector<Point2f> FindCubeCorners2(vector<Point> points)
{
	vector<Point2f> corners;
	float maxdist;
	vector<Point> diagpoints;
	diagpoints.reserve(2);
	maxdist = Distance(points[0], points[1]);
	diagpoints[0] = points[0];
	diagpoints[1] = points[1];
	for (size_t i = 0; i < points.size(); i++)
	{
		for (size_t j = i + 1; j>points.size(); j++)
		{
			if (Distance(points[i], points[j]) > maxdist)
			{
				maxdist = Distance(points[i], points[j]);
				diagpoints[0] = points[0];
				diagpoints[1] = points[1];
			}
		}
	}
	float angle = atan((diagpoints[0].y - diagpoints[1].y) / (diagpoints[0].x - diagpoints[1].x));
	float sidesize = maxdist / sqrt(2);

	return corners;
}

bool FindIntersection(Vec4i l1, Vec4i l2, float &ua, float &ub, Point &pt)
{
	Point p1(l1[0], l1[1]) , p2 (l1[2], l1[3]), p3 (l2[0], l2[1]) , p4(l2[2], l2[3]);
	float den = (p4.y - p3.y)*(p2.x - p1.x) - (p4.x - p3.x)*(p2.y - p1.y);
	if (abs(den) < 0.1) return false;
	ua = (p4.x - p3.x)*(p1.y - p3.y) - (p4.y - p3.y)*(p1.x - p3.x);
	ub = (p2.x - p1.x)*(p1.y - p3.y) - (p2.y - p1.y)*(p1.x - p3.x);
	ua = ua / den;
	ub = ub / den;
	float x = p1.x + ua*(p2.x - p1.x);
	float y = p1.y + ua*(p2.y - p1.y);
	pt = Point(x, y);
	if (ua>0 && ub>0 && ua<1 && ub<1) return true;
}

float AngletoX(Vec4i pt)
{
	float ang = atan(float(pt[1] - pt[3] )/ float(pt[0] - pt[2]));
	ang >= 0 ? ang : ang += CV_PI;
	return ang;
}

void FindCubeFace1(vector<Vec4i> lines, vector<Point> &prevface, int &succ, int &detected, int &tracking,
	Point &v1, Point &v2, Point &p0, vector<Point2f> &features, vector<Point> &pt)
{
	//find cube corners based on paper 
	vector <LinePair> lp;
	vector <Point> edges, test, minps;
	float qwe = 0.06;
	int minch = 10000;
	for (size_t i = 0; i < lines.size(); i++)
	{
		for (size_t j = i + 1; j < lines.size(); j++)
		{
			int matched = 0;
			LinePair hypothesis;
			Point p1, p2, p3, p4;
			float dd1, dd2;
			p1 = Point(lines[i][0], lines[i][1]);
			p2 = Point(lines[i][2], lines[i][3]);
			p3 = Point(lines[j][0], lines[j][1]);
			p4 = Point(lines[j][2], lines[j][3]);
			dd1 = Distance(p1, p2);
			dd2 = Distance(p3, p4);
			if (max(dd1, dd2) / min(dd1, dd2) > 1.3) continue;
			if (SamePoint(p1, p3, 10))
			{
				hypothesis = LinePair(lines[i], lines[j]);
				matched++;
			}
			if (SamePoint(p1, p4, 10))
			{
				hypothesis = LinePair(lines[i], lines[j]);
				matched++;
			}
			if (SamePoint(p2, p3, 10))
			{
				hypothesis = LinePair(lines[i], lines[j]);
				matched++;
			}
			if (SamePoint(p2, p4, 10))
			{
				hypothesis = LinePair(lines[i], lines[j]);
				matched++;
			}
			if (matched == 0)
			{
				//check intersection at 1/3 or 2/3
				Point pt, temp, same, end1, end2;
				float ua, ub;
				if (FindIntersection(lines[i], lines[j], ua, ub, pt))
				{
					int ok1 = 0, ok2 = 0;
					if (abs(ua - 1.0 / 3) < 0.05) ok1 = 1;
					if (abs(ua - 2.0 / 3) < 0.05)  ok1 = 2;
					if (abs(ub - 1.0 / 3)<0.05)  ok2 = 1;
					if (abs(ub - 2.0 / 3)<0.05) ok2 = 2;
					if (ok1>0 && ok2>0)
					{
						if (ok1 == 2)
						{
							temp = p1;
							p1 = p2;
							p2 = temp;

						}
						if (ok2 == 2)
						{
							temp = p3;
							p3 = p4;
							p4 = temp;
						}
						end1 = Point((p3.x + 2.0 / 3 * (p2.x - p1.x)), (p3.y + 2.0 / 3 * (p2.y - p1.y)));
						end2 = Point((p1.x + 2.0 / 3 * (p4.x - p3.x)), (p1.y + 2.0 / 3 * (p4.y - p3.y)));
						same = Point((p1.x - 1.0 / 3 * (p4.x - p3.x)), (p1.y - 1.0 / 3 * (p4.y - p3.y)));
						hypothesis = LinePair(same, end1, end2);
						matched = 1;
					}
				}
			}
			if (matched == 1)
			{
				float ang1, ang2, ang;
				ang1 = atan(float(p1.y - p2.y) / float(p1.x - p2.x));
				ang2 = atan(float(p3.y - p4.y) / float(p3.x - p4.x));
				if (ang1 < 0) ang1 += CV_PI;
				if (ang2 < 0) ang2 += CV_PI;
				ang = abs(abs(ang2 - ang1) - CV_PI / 2);
				if (ang < 0.5) lp.push_back(hypothesis);
			}
		}
	}

	//check how many lines align to a grid
	for (size_t i = 0; i < lp.size(); i++)
	{
		Point same, end1, end2;
		same = lp[i].getsame();
		end1 = lp[i].getend1();
		end2 = lp[i].getend2();
		float ang1, ang2, distance;
		distance = lp[i].getDist();
		if (end1.x - same.x != 0) ang1 = atan((end1.y - same.y) / (end1.x - same.x));
		else ang1 = CV_PI / 2;
		if (end2.x - same.x != 0) ang2 = atan((end2.y - same.y) / (end2.x - same.x));
		else ang2 = CV_PI / 2;
		if (ang1 < 0) ang1 += CV_PI;
		if (ang2 < 0) ang2 += CV_PI;
		distance = 1.7*distance;
		int evidence = 0;
		int totallines = 0;
		Eigen::Matrix3f A, Ainv;
		A << end2.x - same.x, end1.x - same.x, same.x,
			end2.y - same.y, end1.y - same.y, same.y,
			0, 0, 1;
		Ainv = A.inverse();
		for (size_t j = 0; j < lines.size(); j++)
		{
			float ang = AngletoX(lines[j]);
			float a1, a2;
			a1 = abs(abs(ang - ang1) - CV_PI / 2);
			a2 = abs(abs(ang - ang2) - CV_PI / 2);
			if (a1 > 0.1 && a2 > 0.1) continue;
			Point q1, q2;
			q1 = Point(lines[j][0], lines[j][1]); q2 = Point(lines[j][2], lines[j][3]);
			Eigen::Matrix<float, 3, 1> v;
			v << q1.y,
				q1.x,
				1;
			Eigen::Matrix<float, 3, 1> vp1 = Ainv*v;
			if (vp1(0, 0) > 1.1 || vp1(0, 0)<-0.1) continue;
			if (vp1(1, 0) > 1.1 || vp1(1, 0)<-0.1) continue;
			if ((abs(vp1(0, 0) - 1 / 3.0) > qwe)
				&&
				(abs(vp1(0, 0) - 2 / 3.0) > qwe)
				&&
				(abs(vp1(1, 0) - 1 / 3.0) > qwe)
				&&
				(abs(vp1(1, 0) - 2 / 3.0) > qwe)) continue;
			Eigen::Matrix<float, 3, 1> v2;
			v2 << q2.y,
				q2.x,
				1;
			Eigen::Matrix<float, 3, 1> vp2 = Ainv*v2;
			if (vp2(0, 0) > 1.1 || vp2(0, 0)<-0.1) continue;
			if (vp2(1, 0) > 1.1 || vp2(1, 0)<-0.1) continue;
			if ((abs(vp2(0, 0) - 1 / 3.0) > qwe)
				&&
				(abs(vp2(0, 0) - 2 / 3.0) > qwe)
				&&
				(abs(vp2(1, 0) - 1 / 3.0) > qwe)
				&&
				(abs(vp2(1, 0) - 2 / 3.0) > qwe)) continue;
			lp[i].evidence++;
		}
	}
	std::sort(lp.begin(), lp.end(), [](LinePair a, LinePair b){ return (a.evidence > b.evidence); });
	if (lp.size() > 0)
	{
		//check for same grid at least 3 times
		for (size_t i = 0; i < lp.size(); ++i)
		{

			if (lp[i].evidence > 0.05*lines.size())
			{
				Point same(lp[i].getsame()), end1(lp[i].getend1()), end2(lp[i].getend2());
				Point p3(end2.x + end1.x - same.x, end2.y + end1.y - same.y);
				test = { same, end1, end2, p3 };
				p3 = Point(prevface[2].x + prevface[1].x - prevface[0].x, prevface[2].y + prevface[1].y - prevface[0].y);
				vector<Point> tc{ prevface[0], prevface[1], prevface[2], p3 };
				int ch = compfaces(test, tc);
				if (ch < minch)
				{
					minch = ch;
					minps = { same, end1, end2 };
				}
			}
		}
		if (minps.size() > 0)
		{
			prevface = minps;
			if (minch < 10)
			{
				succ += 1;
				pt = prevface;
				detected = 1;
			}
		}
		else
		{
			succ = 0;
		}
		if (succ > 2)
		{
			pt = {};
			for (int i = 1; i < 3; ++i)
			{
				for (int j = 1; j < 3; ++j)
				{
					pt.push_back(Point(p0.x + float(i) / 3 * v1.x + float(j) / 3 * v2.x, p0.y + float(i) / 3 * v1.y + float(j) / 3 * v2.y));
				}
			}
			features = {};
			for (size_t i = 0; i < pt.size(); ++i)
			{
				features.push_back(Point2f(pt[i].x, pt[i].y));
			}
			tracking = 1;
			succ = 0;
		}
	}
}

int compfaces(vector<Point> f1, vector<Point> f2)
{
	int totd = 0;
	for (int i = 0; i < f1.size(); ++i)
	{
		int mind = 10000;
		for (int j = 0; j < f2.size(); ++j)
		{
			float d = Distance(f1[i], f2[j]);
			if (d < mind) mind = d;
		}
		totd += mind;
	}
	
	return (totd / 4);
}

vector<Point> winded(Point p1, Point p2, Point p3, Point p4)
{
	vector<AnglePoint> ps;
	vector<Point> ps1;
	Point2f avg = (0.25*(p1.x + p2.x + p3.x + p4.x), 0.25*(p1.y + p2.y + p3.y + p4.y));
	AnglePoint ts1(atan2(p1.y - avg.y, p1.x - avg.x), p1);
	AnglePoint ts2(atan2(p2.y - avg.y, p2.x - avg.x), p2);
	AnglePoint ts3(atan2(p3.y - avg.y, p3.x - avg.x), p3);
	AnglePoint ts4(atan2(p4.y - avg.y, p4.x - avg.x), p4);
	ps = {ts1, ts2, ts3, ts4};
	std::sort(ps.begin(), ps.end(), [](AnglePoint a, AnglePoint b){ return (a.getAngle() > b.getAngle()); });
	for (size_t i = 0; i < ps.size(); i++)
	{
		ps1.push_back(ps[i].getPoint());
	}
	return ps1;
}

Mat FindCubeOrientation(vector<Point2f> points, Mat cam, Mat dist)
{
	vector<Point3f> obj;
	Mat rvec, tvec, cam, dist;
	obj.push_back(Point3f(0.0, 0.0, 0.0));
	obj.push_back(Point3f(0.0, 1.0, 0.0));
	obj.push_back(Point3f(1.0, 1.0, 0.0));
	obj.push_back(Point3f(1.0, 0.0, 0.0));
	bool success = solvePnP(obj, points, cam, dist, rvec, tvec);
	return rvec;
}

