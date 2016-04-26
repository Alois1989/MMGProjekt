#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <cstdio>
#include <sstream>



using namespace cv;
using namespace std;


int main()
{

	//Bild einlesen
	Mat img = imread("Lenna.png");
	imshow("Original", img);

	Mat blau, rot, gruen;

	vector<Mat> channels(3);
	// split img:
	for (int i = 0; i < 3;i++)
	{
		split(img, channels[]);
		// Farbkanaele übertragen
	}

	blau = channels[0];

	gruen = channels[1];
	rot = channels[2];



	imshow("Blaukanal", blau);
	imshow("Grünkanal", gruen);
	imshow("Rotkanal", rot);

	waitKey();

	return 0;



}