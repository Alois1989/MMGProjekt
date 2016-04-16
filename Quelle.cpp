#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\core.hpp>
#include <cstdio>
#include <iostream>
using namespace cv;
using namespace std;

int main ()
{
	Mat pic;
	pic = imread("Lenna.png");
	imshow("Test", pic);
	
	cout << " Hello World " << endl ;



	waitKey();
	


	return 0;
	


}

