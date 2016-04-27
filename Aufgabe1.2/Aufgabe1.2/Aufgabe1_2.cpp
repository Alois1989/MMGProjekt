#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <cstdio>
#include <sstream>
#include <iostream>



using namespace cv;
using namespace std;


int main()
{

	//Bild einlesen
	Mat img = imread("Lenna.png");
	imshow("Original", img);

	//leere Bilddummies fuer jew. Kanaele
	Mat blau, rot, gruen;

	//Originialbild in Kanalbilder komplett kopieren
	rot = img.clone();
	gruen = img.clone();
	blau = img.clone();

	//Farbraume abziehen
	rot = rot - Scalar(255, 255, 0);

	gruen = gruen - Scalar(255, 0, 255);

	blau = blau - Scalar(0, 255, 255);

	//Bild ausgeben
	imshow("Blaukanal", blau);
	imshow("Grünkanal", gruen);
	imshow("Rotkanal", rot);

	//Kanaele tauschen

	Mat picRGB = imread("Lenna.png");

	for (int y = 0; y < picRGB.rows; y++) {
		for (int x = 0; x < picRGB.cols; x++) {

			//Kanal zwischenspeichern
			uchar buf = picRGB.at<Vec3b>(y, x)[2];
			//Vertauschung der Kanaele
			picRGB.at<Vec3b>(y, x)[2] = picRGB.at<Vec3b>(y, x)[0];
			picRGB.at<Vec3b>(y, x)[0] = buf;
		}
	}

	imshow("BGR2RGB", picRGB);
	imwrite("BGR2RGB.png", picRGB);


	waitKey();

	return 0;



}