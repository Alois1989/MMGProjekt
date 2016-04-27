//Ursprüngliche Version der Aufgabe die nach neuem Ansatz gaenzlich verworfen wurde IplImage ist sehr unbequem!!
/*
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\core.hpp>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>

using namespace cv;
using namespace std;

vector<int> calcuHisto(const IplImage *src_pic, int anzBin);
Mat drawHisto(const IplImage *src_pic, int anzBin, vector <int> histo);


int main(int argc, char* argv[])
{
	//Bild laden als Mat
	Mat pic = imread("Lenna.png",0);

	//Bild laden als Ipl
	IplImage *image = cvLoadImage("Lenna.png", CV_LOAD_IMAGE_GRAYSCALE);
	
	//Berechnung des Histogrammvektors
	vector<int> histogram = calcuHisto(image,40);
	
	//Ausgabe des Histogrammvektors
	for (int x = 0; x < histogram.size(); x++)
	{
		//Speicher Bingröße absolut
		float buf1 = (255.0 / histogram.size());
		//Speicher Schlusswert der momentanen Bin
		float buf2 = buf1 * (x + 1);
		//Speicher Anfangswert der momentanen Bin
		float buf3 = buf1 * x;


		cout << "Bin " << x + 1 << " hat |";
		cout << histogram[x];
		cout << "| - Werte zwischen " << buf3 << " und " << buf2 << endl;
	}


	//Farbintensitätsveränderung
	//-----------------------------------------------------------------------------------------------------
	//Stärke der Veränderung andgeben:
	float intens = 0.2;
	//int noChann = pic.channels(); 
	//Für Grauwert nur 1 Channel
	int noChann = 1;
	for (int y = 0; y < pic.rows; y++)
		{

			uchar* imgPtr = pic.ptr<uchar>(y);
			for (int x = 0; x < pic.cols; x++)
			{
				for (int c = 0; c < noChann; c++)
				{
					uchar colVal = imgPtr[x*noChann + c];

					imgPtr[x*noChann + c] = colVal*intens;
				}
			}
		}
	//-----------------------------------------------------------------------------------------------------

	//Manipuliertes Bild ausgeben
	imshow("Manipuliert", pic);

	//Startbild anzeigen
	cvShowImage("Bild", image);
	
	
	waitKey();

	//Ipl Bildspeicher wieder freigeben
	cvReleaseImage(&image);


	return 0;

}


vector<int> calcuHisto(const IplImage *src_pic, int anzBin)
{
	CvSize size = cvGetSize(src_pic);
	double binSize = 256.0 / anzBin;        
	vector<int> histogram(anzBin, 0);        
	
											 
	for (int y = 0; y<size.height; y++)
	{
		//Bezieht sich auf eine komplette
		const uchar *src_pic_point = (uchar *)(src_pic->imageData + y*src_pic->widthStep);

		//goes through each column
		for (int x = 0; x<size.width; x++)
		{
			//for each bin
			for (int z = 0; z < anzBin; z++)
			{
				//check both upper and lower limits
				if (src_pic_point[x] >= z*binSize && src_pic_point[x] < (z + 1)*binSize)
				{
					//increment the index that contains the point
					histogram[z]++;
				}
			}
		}
	}
	return histogram;
}

//Nicht funktionierende Bildausgabevariante für das Histogramm
Mat drawHisto(const IplImage *src_pic, int anzBin, vector <int> histo)
{
	

	CvSize size = cvGetSize(src_pic);
	double binSize = 256.0 / anzBin;        //binbreite
	vector<int> histogram(anzBin, 0);        
	Mat histoPic(400,400, CV_8UC3);
									 

											 
	for (int y = 0; y<size.height; y++) //Zeilen
	{
		//ließt ganze Zeile ein
		const uchar *src_pic_point = (uchar *)(src_pic->imageData + y*src_pic->widthStep);
		cout << y << ".Zeile von " << size.height << endl;
		//geht durch alle Spalten
		for (int x = 0; x<size.width; x++)
		{
			//pro Bin
			for (int z = 0; z < anzBin; z++)
			{
				Point pt1 = src_pic_point[x];
				Point pt2 = histo[z] - 400;

				line(histoPic, pt2, pt1, CV_RGB(255, 0, 0), binSize, 8, 0);
				//überprüft die jew Bin Zuordnung -- größere/kleinere
				
			}
		}
	}
	return histoPic;


}

*/


//Neuer Ansatz nach erhoehter recherche
#include<iostream>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
	Mat image1 = imread("Lenna.png", CV_LOAD_IMAGE_COLOR);
	Mat gray1 = imread("Lenna.png", CV_LOAD_IMAGE_GRAYSCALE);

	/*
	//Ueberbelichtetes Bild
	Mat image1 = imread("Lenna1.jpg", CV_LOAD_IMAGE_COLOR);
	Mat gray1 = imread("Lenna1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	*/

	/*
	//Unterbelichtets Bild
	Mat image1 = imread("Lenna2.jpg", CV_LOAD_IMAGE_COLOR);
	Mat gray1 = imread("Lenna2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	*/

	double intens = 8;
	
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
	//Hier Aufgrund der nicht Normalisierten Form mit einem Schaetzwert von Hoehe
	int nNormH = 800;
	int nNormBr = 400;

	Mat histImageNnorm(nNormH, nNormBr, CV_8UC1, Scalar(255, 255, 255));

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
		line(histImageNnorm, Point(bin_br*(i), nNormH),
			Point(bin_br*(i), nNormH - histogramNnorm[i]),
			Scalar(0, 0, 0), 1, 8, 0);
	}



	//Eindimensionales Integerarray mit 256 Spalten erstellen
	int histogram2[255];

	//Histogrammarray mit Nullen ueberschreiben
	for (int i = 0; i < 255; i++) {
		histogram2[i] = 0;
	}
	//Histogrammarray1 mit Korekturfaktor "intens" in Histogrammarray2 kopieren
	for (int i = 0; i < 255; i++) {
		histogram2[i] = histogram1[i] * intens;
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

	imshow("Histogram manipuliertes Bild", histImage2);

	imshow("Originalbild", image1);

	imshow("Graubild", gray1);

	imshow("Manipuliert", newImage);

	//Abspeichern der Histogrammbilder
	imwrite("GraustufenHistogrammnormalisiertOhneManipulation.png", histImage1);
	imwrite("GraustufenHistogrammnormalisiertMitManipulation.png", histImage2);

	waitKey();
	return 0;
}
