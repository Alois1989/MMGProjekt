#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <cstdio>
#include <sstream>

using namespace cv;
using namespace std;

void createImage(int br, int h);

int main()
{

}


void createImage(int br, int h)
{

	Mat pic;
	//Bild leer mit breite hoehe erstellen





	//durchlaufen und mit formel jew Pixel beschreiben

	for (int y = 0; y < pic.rows; y++)
	{

		uchar* imgPtr = pic.ptr<uchar>(y);
		for (int x = 0; x < pic.cols; x++)
		{
			for (int y = 0; y < pic.rows; y++)
			{

			
				for (int c = 0; c < noChann; c++)
				{
					uchar valAt = imgPtr[x*noChann + c];

					imgPtr[x*noChann + c] = valAt * sobelFilter;
				}
			}
		}
	}
	imshow("Ausgabebild", pic);
}


//Main für 2_1 auskommentiert

int main()
{
	
	createImage(400, 400);

}


/*
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Point start;
Point ende;
Point neuerStart;
Point neuesEnd;


void setStart(Point start)
{
	neuerStart = start;
}

void setEnd(Point ende)
{
	neuesEnd = ende;
	cout << neuerStart << " : " << neuesEnd << endl;
}

Mat bemalt(Point start, Point ende, Mat *srcPic)
{
	cvDrawRect(&srcPic, start, ende, 1, 1, 8, 8);
	return *srcPic;
}


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{

	
	if (event == EVENT_LBUTTONDOWN)
	{
		cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		start.x = x;
		start.y = y;
		setStart(start);
	}

	if (event == CV_EVENT_LBUTTONUP)
	{
		cout << "Left button of the mouse is released - position (" << x << ", " << y << ")" << endl;
		ende.x = x;
		ende.y = y;
		setEnd(ende);
	}

	else if (event == EVENT_RBUTTONDOWN)
	{
		cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == EVENT_MBUTTONDOWN)
	{
		cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == EVENT_MOUSEMOVE)
	{
		cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;

	}
}


int main(int argc, char** argv)
{
	// Read image from file 
	Mat img = imread("Lenna.png");

	//if fail to read the image
	if (img.empty())
	{
		cout << "Error loading the image" << endl;
		return -1;
	}

	//Create a window
	namedWindow("My Window", 1);

	//set the callback function for any mouse event
	setMouseCallback("My Window", CallBackFunc, NULL);

	cout << start << " : " << ende << endl;

	//show the image
	imshow("My Window", img);

	cout << start << " : " << ende << endl;
	Mat novo = bemalt(&img, neuerStart, neuesEnd);


	// Wait until user press some key
	waitKey(0);

	return 0;

}
*/

/*
#include<opencv2/highgui/highgui.hpp>
#include<opencv2\imgproc.hpp>
#include<opencv2\core.hpp>


using namespace cv;
using namespace std;

Point point1, point2; ///* vertical points of the bounding box 

int drag = 0;

Rect rect; ///* bounding box 

Mat img, roiImg; ///* roiImg - the part of the image in the bounding box 

int select_flag = 0;

void mouseHandler(int event, int x, int y, int flags, void* param)
{
	if (event == CV_EVENT_LBUTTONDOWN && !drag)
	{
		///* left button clicked. ROI selection begins 
		point1 = Point(x, y);
		drag = 1;
	}

	if (event == CV_EVENT_MOUSEMOVE && drag)
	{
		///* mouse dragged. ROI being selected 
		Mat img1 = img.clone();
		point2 = Point(x, y);
		rectangle(img1, point1, point2, CV_RGB(255, 0, 0), 3, 8, 0);
		imshow("image", img1);
	}

	if (event == CV_EVENT_LBUTTONUP && drag)
	{
		point2 = Point(x, y);
		rect = Rect(point1.x, point1.y, x - point1.x, y - point1.y);
		drag = 0;
		roiImg = img(rect);
	}

	if (event == CV_EVENT_LBUTTONUP)
	{
		///* ROI selected 
		select_flag = 1;
		drag = 0;
	}
}

int main()
{

	

	int k;
	
	Mat img = imread("Lenna.png");
	
	imshow("image", img);
	while (1)
	{
		//cap >> img;
		cvSetMouseCallback("image", mouseHandler, NULL);
		if (select_flag == 1)
		{
			imshow("ROI", roiImg); ///* show the image bounded by the box 
		}
		rectangle(img, rect, CV_RGB(255, 0, 0), 3, 8, 0);
		imshow("image", img);
		k = waitKey(10);
		if (k == 27)
		{
			break;
		}
	}
	return 0;
}

*/

