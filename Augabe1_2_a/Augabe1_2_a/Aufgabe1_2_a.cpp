#include<iostream>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using std::cout;
using std::cin;
using std::endl;

using namespace std;
using namespace cv;

int main()
{
	Mat image = imread("Lenna.png");
	Mat r = image.clone();
	Mat g = image.clone();
	Mat b = image.clone();

	r -= Scalar(255, 255, 0);

	imshow("Red channel", r);

	g -= Scalar(255, 0, 255);

	imshow("Green channel", g);

	b -= Scalar(0, 255, 255);

	imshow("Blue channel", b);


	waitKey();
	return 0;

}