#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace cv;

void calcuHist(Mat img, int histogram[]){

	for (int i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}

	for (int y = 0; y < img.rows; y++)
	for (int x = 0; x < img.cols; x++)
		histogram[(int)img.at<uchar>(y, x)]++;
}


int main()
{
	Mat src = imread("Lenna.png");

	if (!src.data)
	{
		return -1;
	}
	//Vektor für die Farbkanaele
	vector<Mat> bgr_channs;
	//Bild src in Kanaele aufsplitten
	split(src, bgr_channs);

	//Start intitial fuer 2^2 Bins in while-Schleife
	int n = 2;

	while (n < 9){

		//Wird pro Schleifendurchlauf um 2 erhoeht 2^n Bins...
		int histSize = pow(2.0, n);

		//Leerbilder für jew. Histogramm erstellen
		Mat bla_hist, grue_hist, rot_hist;

		//Arrays für jew. Histogramme erstellen
		int histogramRo[256], histogramGru[256], histogramBla[256];


		//Histogramme für die jew. Kanaele erstellen mit jew. Kanalvektor an calcuhist übergeben
		
		calcuHist(bgr_channs[2], histogramRo);
		
		calcuHist(bgr_channs[1], histogramGru);
		
		calcuHist(bgr_channs[0], histogramBla);


		
		int hist_br = 512; int hist_h = 400;

		int bin_br = cvRound((double)hist_br / histSize);

		Mat histImage(hist_h, hist_br, CV_8UC3, Scalar(0, 0, 0));

		
		//Jew. Maximalwerte in den Einzelnen Kanalarrays ermitteln fuer Normalisierung
		int maxro = histogramRo[0];
		for (int i = 1; i < 256; i++){
			if (maxro < histogramRo[i]){
				maxro = histogramRo[i];
			}
		}
		int maxgru = histogramGru[0];
		for (int i = 1; i < 256; i++){
			if (maxgru < histogramGru[i]){
				maxgru = histogramGru[i];
			}
		}
		
		int maxbla = histogramBla[0];
		for (int i = 1; i < 256; i++){
			if (maxbla < histogramBla[i]){
				maxbla = histogramBla[i];
			}
		}

		//Normalisieren
		for (int i = 0; i < 255; i++){
			histogramBla[i] = ((double)histogramBla[i] / maxbla)*histImage.rows;
			histogramGru[i] = ((double)histogramGru[i] / maxgru)*histImage.rows;
			histogramRo[i] = ((double)histogramRo[i] / maxro)*histImage.rows;
		}

		/// Jeder Kanal wird mit entsprechender Farbe gezeichnet
		for (int i = 1; i < histSize; i++)
		{
			line(histImage, Point(bin_br*(i - 1), hist_h - histogramRo[i]),
				Point(bin_br*(i), hist_h - histogramRo[i]),
				Scalar(0, 0, 255), 2, 8, 0);
			cout << "Wird berechnet" << endl;
			line(histImage, Point(bin_br*(i - 1), hist_h - histogramGru[i]),
				Point(bin_br*(i), hist_h - histogramGru[i]),
				Scalar(0, 255, 0), 2, 8, 0);
			cout << "Wird berechnet" << endl;
			line(histImage, Point(bin_br*(i - 1), hist_h - histogramBla[i]),
				Point(bin_br*(i), hist_h - histogramBla[i]),
				Scalar(255, 0, 0), 2, 8, 0);
			cout << "Wird berechnet" << endl;
		}

		//Anzeige der Bilder
	
		imshow("Histogramm", histImage);
		cout << "Histogramm fuer 2 ^" << n << " Bins. Fuer naechst groessere Binstufe bitte Fenster schliessen!" << endl;
		waitKey(0);

		n+=2;
	}

	return 0;
}
