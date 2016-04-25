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
#include <C:\Users\SeBro\Documents\GitHub\MMGProjekt\Blatt1\Blatt1\Histo.h>

using namespace cv;
using namespace std;




vector<int> calcuHisto(const IplImage *src_pic, int anzBin)
{
	CvSize size = cvGetSize(src_pic);
	double binSize = 256.0 / anzBin;        //new definition
	vector<int> histogram(anzBin, 0);

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
