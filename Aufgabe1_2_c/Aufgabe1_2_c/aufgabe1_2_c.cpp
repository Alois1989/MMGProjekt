#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace cv;

int main()
{
	Mat image = imread("Lenna.png");


	for (int y = 0; y < image.rows; y++){
		for (int x = 0; x < image.cols; x++){

			uchar hilf = image.at<Vec3b>(y, x)[2];
			image.at<Vec3b>(y, x)[2] = image.at<Vec3b>(y, x)[0];
			image.at<Vec3b>(y, x)[0] = hilf;
		}
	}

	imshow("rgbtobgr", image);
	imwrite ("rgbtobgr.png", image);

	waitKey(0);
	return 0;
}