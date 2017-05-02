#include <opencv2/highgui/highgui.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/photo/photo.hpp"
#include "ImageFilters.h"
#include "FeatureDetector.h"
#include "Line.h"
#include "FaceFinder.h"
#include "Hough_Transform.h"
#include "lk-tracking.h"
#include "Input.h"
#include <iostream>
#include <string>
#include <Windows.h>

using namespace cv;
using namespace std;



int main()
{
	namedWindow("Video",1);
	namedWindow("Blob",1);
	VideoCapture cap(1);
	int tracking_mode=0;
	bool of;
	Mat prev_dst;
	Mat dst;
	vector<Point> points;
	vector<KeyPoint> keypoints;
	vector<Point2f> pointsf, pointsf_next;
	int shown_face_count=0;
	while(true)
	{
		
		Mat src;
		cap >> src; 
		if (!src.empty())
		{		
			dst=FilterImage(src);				
			if (tracking_mode==0)
			{
				
				bool success;						
				success=BlobDetectFeatures(dst, dst, points, keypoints);										
				if (success) 
				{
					tracking_mode=1;
					cout<<"success \n";
					KeyPoint::convert(keypoints,pointsf);
				}
			}
			else if (tracking_mode==1)
			{
				
				//FIX VECTOR POINTS SIZE	
				of=LKOpticalFlow(prev_dst,dst,pointsf,pointsf_next);
				if (!of) 
				{
					tracking_mode=0;
					points.clear();
					shown_face_count=0;
				}
				Rect rect=boundingRect(pointsf);
				vector<Point> pts;
				pts.push_back(rect.tl());
				pts.push_back(rect.br());				
				ShowFace(src, pts);
				rectangle(src,rect.tl(),rect.br(),Scalar(255,255,255),2);
				cout<<src.channels()<<" \n";
				pointsf=pointsf_next;
			}
			imshow("Video",src);
			imshow("Blob",dst);					
			prev_dst=dst;
		}
		
		if( waitKey(1) == 27 ) break;
		//Sleep(33);
	}
}
	 
