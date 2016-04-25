#ifndef Histo
#define Histo
#include <vector>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\core.hpp>


class Histo
{
	vector<int> calcuHisto(const IplImage *src_pic, int anzBin);
};

#endif // !