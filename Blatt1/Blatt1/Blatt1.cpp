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

int main(int argc, char* argv[])
{
	//Bild laden

	IplImage *image = cvLoadImage("Lenna.png", CV_LOAD_IMAGE_GRAYSCALE);
	//Nummer der Farbkanaele abfragen
	//int numberOfChannels = image.channels();

	//Bild anzeigen
	cvShowImage("Bild", image);

	vector<int> histogram = calcuHisto(image,10);
	for (int x = 0; x < histogram.size(); x++)
	{
	cout << histogram[x] << endl;
	}
	//neues Bild anzeigen
	//imshow("Test",);
	//imshow("Manipuliertes Bild", image);

	waitKey();


	cvReleaseImage(&image);
	return 0;

}


vector<int> calcuHisto(const IplImage *src_pic, int anzBin)
{
	CvSize size = cvGetSize(src_pic);
	double binSize = 256.0 / anzBin;        //new definition
	vector<int> histogram(anzBin, 0);        //i don't know if this works so I
											 //so I will leave it

											 //goes through all rows
	for (int y = 0; y<size.height; y++)
	{
		//grabs an entire row of the imageData
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

/*vector<int> calcuHisto(const IplImage *src_pic, int anzBin) 
{
	CvSize size = cvGetSize(src_pic);
	int binSize = (size.width / 256.0)*anzBin;
	vector<int> histogram(anzBin,0);
	cout << size.height << endl;
	cout << size.width << endl;
	cout << "Bingroesse: " << binSize << endl;
	for (int y = 0; y<size.height; y++) 
	{
		const uchar *src_pic_point =
			(uchar *)(src_pic->imageData + y*src_pic->widthStep);
		for (int x = 0; x<size.width; x++) 
		{
			for (int z = 0; z <= anzBin; z++)
			{
				if (src_pic_point[x] < z*binSize)
				{
					histogram[src_pic_point[x]]++;
					cout << "Test" << endl;
				}
				else
				{
					cout << "Not working" <<endl;
				}
				cout  << "Z: " << z << endl;
			}
			
		}
	}
	return histogram;
}
*/

/*Mat calcuHisto(Mat & image, float anzBin);


int sample()
{
	Mat src, dst, erg;

	/// Load image
	src = imread("Lenna.png", 1);
	dst = imread("Lenna.png", CV_LOAD_IMAGE_GRAYSCALE);
	erg = calcuHisto(dst, 10.0);
	cout << "Histo of mine: " << erg << endl;

	if (!src.data)
	{
		return -1;
	}

	/// Separate the image in 3 places ( B, G and R )
	vector<Mat> bgr_planes;
	split(src, bgr_planes);

	/// Establish the number of bins
	int histSize = 256;

	/// Set the ranges ( for B,G,R) )
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true; bool accumulate = false;

	Mat b_hist, g_hist, r_hist;

	/// Compute the histograms:
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

	cout << "Test " << b_hist << endl;

	// Draw the histograms for B, G and R
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	/// Normalize the result to [ 0, histImage.rows ]
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	/// Draw for each channel
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
	//	line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
	//		Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
	//		Scalar(0, 255, 0), 2, 8, 0);
	//	line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
	//		Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
	//		Scalar(0, 0, 255), 2, 8, 0);
	}

	/// Display
	namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
	imshow("calcHist Demo", histImage);
	imshow("Graubild", dst);

	waitKey(0);

	return 0;
}


*/