#include "lk-tracking.h"

bool LKOpticalFlow(Mat prev, Mat next, vector<Point2f> points, vector<Point2f> &next_points)
{
	TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.3);
	Size subPixWinSize(10,10), winSize(5,5);
	const int MAX_COUNT = 500;
	vector<uchar> status;
	Mat err;
	if (prev.channels()==3) cvtColor(prev,prev,CV_BGR2GRAY);
	calcOpticalFlowPyrLK(prev, next, points, next_points, status, err, winSize, 3, termcrit,0);
	int count=0;
	for (size_t i=0;i<status.size();i++)
	{
		if (int(status[i])==1) count++;
	}
	if (count<3) return false;
	else return true;
}
