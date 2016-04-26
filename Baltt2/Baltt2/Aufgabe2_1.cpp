/*#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <cstdio>
#include <sstream>

using namespace cv;
using namespace std;

Mat funcWalkthrough(Mat pic, int dimMat);

int main()
{

}


Mat funcwalkthroug(Mat pic, int dimMat)
{
	Mat newPic;
	int noChann = pic.channels;
	double testFilter[5][5] = {};
	double sobelFilter[3][3] = { -1, 0, 1,- 2, 0, 2, - 1, 0, 1};
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
	return pic;
}
*/

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

#include<iostream>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
	Mat image1 = imread("Lenna.png",CV_LOAD_IMAGE_COLOR);
	Mat gray1 = imread("Lenna.png",CV_LOAD_IMAGE_GRAYSCALE);


	double alpha = 1;

	//cvtColor(image1, gray1, CV_BGR2GRAY);

	Mat newImage = gray1.clone();

	//Eindimensionales Integerarray mit 256 Spalten erstellen
	int histogram1[255];	

	
	//Histogrammarray mit Nullen ueberschreiben 
	for (int i = 0; i < 255; i++) {
		histogram1[i] = 0;
	}

	//Grauwerte aus Bild in Histogrammarray einlesen
	for (int y = 0; y < gray1.rows; y++)
		for (int x = 0; x < gray1.cols; x++)
			histogram1[(int)gray1.at<uchar>(y, x)]++;

	//feste Histogrammabmessungen
	int hist_br = 200; 
	int hist_h = 400;

	//breite der Bins berechnen
	int bin_br = cvRound((double)hist_br / 256);


	//Histogrammbild original erstellen
	Mat histImage1(hist_h, hist_br, CV_8UC1, Scalar(255, 255, 255));		


	//Hoechsten Wert des Histogramms finden
	int max1 = histogram1[0];
	for (int i = 1; i < 256; i++) {
		if (max1 < histogram1[i]) {
			max1 = histogram1[i];
		}
	}

	//Normalisieren
	for (int i = 0; i < 255; i++) {
		histogram1[i] = ((double)histogram1[i] / max1)*histImage1.rows;
	}

	//Histogramm zeichnen
	for (int i = 0; i < 255; i++)
	{
		line(histImage1, Point(bin_br*(i), hist_h),
			Point(bin_br*(i), hist_h - histogram1[i]),
			Scalar(0, 0, 0), 1, 8, 0);
	}


	//Histogrammbild für nicht Normalisiertes Hist erstellen
	Mat histImageNnorm(800, 400, CV_8UC1, Scalar(255, 255, 255));

	//Eindimensionales Integerarray mit 256 Spalten erstellen
	int histogramNnorm[255];


	//Histogrammarray mit Nullen ueberschreiben 
	for (int i = 0; i < 255; i++) {
		histogramNnorm[i] = 0;
	}

	// Grauwerte aus Bild in Histogrammarray einlesen
		for (int y = 0; y < gray1.rows; y++)
			for (int x = 0; x < gray1.cols; x++)
				histogramNnorm[(int)gray1.at<uchar>(y, x)]++;


	//Histogramm zeichnen
	for (int i = 0; i < 255; i++)
	{
		line(histImageNnorm, Point(bin_br*(i), hist_h),
			Point(bin_br*(i), hist_h - histogramNnorm[i]),
			Scalar(0, 0, 0), 1, 8, 0);
	}



	//Eindimensionales Integerarray mit 256 Spalten erstellen
	int histogram2[255];									
	
	//Histogrammarray mit Nullen ueberschreiben
	for (int i = 0; i < 255; i++) {
		histogram2[i] = 0;
	}
	//Histogrammarray1 mit Korekturfaktor "alpha" in Histogrammarray2 kopieren
	for (int i = 0; i < 255; i++) {
		histogram2[i] = histogram1[i] * alpha;
	}

	//Kontrastverstärktes Bild aus Histogrammarray2 erstellen
	for (int y = 0; y < gray1.rows; y++)
		for (int x = 0; x < gray1.cols; x++)
			newImage.at<uchar>(y, x) = histogram2[gray1.at<uchar>(y, x)];

	//Histogrammbild für Histogrammarray2 erstellen
	Mat histImage2(hist_h, hist_br, CV_8UC1, Scalar(255, 255, 255));		
	
	//Hoechsten Wert des Histogramms finden
	int max2 = histogram2[0];
	for (int i = 1; i < 256; i++) {
		if (max2 < histogram2[i]) {
			max2 = histogram2[i];
		}
	}

	//Normalisieren
	for (int i = 0; i < 255; i++) {
		histogram2[i] = ((double)histogram2[i] / max2)*histImage2.rows;
	}

	//Histogramm zeichnen
	for (int i = 0; i < 255; i++)
	{
		line(histImage2, Point(bin_br*(i), hist_h),
			Point(bin_br*(i), hist_h - histogram2[i]),
			Scalar(0, 0, 0), 1, 8, 0);
	}






	//Ausgaben

	imshow("Histogram1", histImage1);

	imshow("Histogram nicht Normalisiert", histImageNnorm);

	imshow("Histogram2", histImage2);

	imshow("Originalbild", image1);

	imshow("Graubild", gray1);
	
	imshow("Manipuliert", newImage);

	//Abspeichern der Histogrammbilder
	imwrite("GraustufenHistogrammnormalisiert1.png", histImage1);
	imwrite("GraustufenHistogrammnormalisiert2.png", histImage2);

	waitKey();
	return 0;
}
