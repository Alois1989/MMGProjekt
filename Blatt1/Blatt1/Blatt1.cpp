
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



