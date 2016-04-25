#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace cv;

void imhist(Mat image, int histogram[]){

	for (int i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}

	for (int y = 0; y < image.rows; y++)
	for (int x = 0; x < image.cols; x++)
		histogram[(int)image.at<uchar>(y, x)]++;
}


int main()
{
	Mat src = imread("Lenna.png");

	if (!src.data)
	{
		return -1;
	}

	vector<Mat> bgr_planes;
	split(src, bgr_planes);


	int n = 2;
	while (n < 9){
		/// Anzahl der Bins hier 2^2, 2^4, 2^6, 2^8

		int histSize = pow(2.0, n);

		/// Set the ranges ( for B,G,R) )
		float range[] = { 0, 256 };
		const float* histRange = { range };

		bool uniform = true; bool accumulate = false;

		Mat b_hist, g_hist, r_hist;
		int histogramr[256], histogramg[256], histogramb[256];


		/// Compute the histograms:

		imhist(bgr_planes[0], histogramb);
		imhist(bgr_planes[1], histogramg);
		imhist(bgr_planes[2], histogramr);


		// Draw the histograms for B, G and R
		int hist_w = 512; int hist_h = 400;
		int bin_w = cvRound((double)hist_w / histSize);

		Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

		/// Normalize the result to [ 0, histImage.rows ]
		int maxb = histogramb[0];
		for (int i = 1; i < 256; i++){
			if (maxb < histogramb[i]){
				maxb = histogramb[i];
			}
		}
		int maxg = histogramg[0];
		for (int i = 1; i < 256; i++){
			if (maxg < histogramg[i]){
				maxg = histogramg[i];
			}
		}
		int maxr = histogramr[0];
		for (int i = 1; i < 256; i++){
			if (maxr < histogramr[i]){
				maxr = histogramr[i];
			}
		}

		for (int i = 0; i < 255; i++){
			histogramb[i] = ((double)histogramb[i] / maxb)*histImage.rows;
			histogramg[i] = ((double)histogramg[i] / maxg)*histImage.rows;
			histogramr[i] = ((double)histogramr[i] / maxr)*histImage.rows;
			cout << "teste0" << endl;
		}

		/// Draw for each channel
		for (int i = 1; i < histSize; i++)
		{
			line(histImage, Point(bin_w*(i - 1), hist_h - histogramb[i]),
				Point(bin_w*(i), hist_h - histogramb[i]),
				Scalar(255, 0, 0), 2, 8, 0);
			cout << "teste1" << endl;
			line(histImage, Point(bin_w*(i - 1), hist_h - histogramg[i]),
				Point(bin_w*(i), hist_h - histogramg[i]),
				Scalar(0, 255, 0), 2, 8, 0);
			cout << "teste2" << endl;
			line(histImage, Point(bin_w*(i - 1), hist_h - histogramr[i]),
				Point(bin_w*(i), hist_h - histogramr[i]),
				Scalar(0, 0, 255), 2, 8, 0);
			cout << "teste3" << endl;
		}

		/// Display
		namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
		imshow("calcHist Demo", histImage);

		waitKey(0);

		n+=2;
	}

	return 0;
}
