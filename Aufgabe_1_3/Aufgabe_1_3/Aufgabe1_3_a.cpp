#include <cstdio>
#include <opencv2\calib3d.hpp>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\opencv.hpp>
#include <Windows.h>
#include <opencv2\world.hpp>
#include <opencv2\photo.hpp>



// Initialize trackbar parameters (x coordinate, y coordinate, size of ROI)
int ROI_x = 0;
int ROI_y = 0;
int ROI_SIZE = 160;

/* Initialize flags to zero. These flags determine the histogram for which
image channel should be displayed. The change in the flag values is triggered
by left mouse click in the panel window */
bool CLICK_red = 0;
bool CLICK_green = 0;
bool CLICK_blue = 0;

/* Declare the header for callback function of mouse events; the function that is called
whenever a mouse event occurs in the panel window (more explanation later) */
void my_mouse_callback(int event, int x, int y, int flags, void* param);

/* Define callback function for the x position of the region of interest. This function
is called whenever the position of the corresponding trackbar changes. The new position
of the trackbar is stopre in the variable "position". */
void ROI_xpos_callback(int position)
{
	/* Check to see whether the width of the region of interest with the new x position
	exceeds 640 pixels. If this is the case, the region of interest will be outside the
	boundaries of a 640x480 image. Hence, we adjust the x position to the maximum allowable
	value. The 640 pixel limit was determined based on the image size captured from our video
	device. If different image sizes are used the value should be adjusted accordingly. */
	if (position + ROI_SIZE * 4 > 640)
	{
		ROI_x = 640 - ROI_SIZE * 4;
	}
	else
	{
		ROI_x = position;
	}
}
/* Define callback function for the y position of the region of interest. The function is
called when the position of the corresponding trackbar changes. */
void ROI_ypos_callback(int position)
{
	/* Check to see whether the height of the region of interest with the new y position
	exceeds 480 pixels. If this is the case, the region of interest will be outside the
	boundaries of the 640x480 image. Hence, we adjust the y position to the maximum allowable
	value. The 480 pixel limit was determined based on the image size captured from our video
	device. If different image sizes are used the value should be adjusted accordingly. */
	if (position + ROI_SIZE * 3 > 480)
	{
		ROI_y = 480 - ROI_SIZE * 3;
	}
	else
	{
		ROI_y = position;
	}
}
/* Define callback function for size of the region ot interest. The function is called
when the position of the corresponding trackbar changes */
void ROI_size_callback(int position)
{
	/* Check to make sure the size of ROI is not zero. If the size is zero set
	the value to 1 */
	if (position == 0)
	{
		ROI_SIZE = 1;
	}
	/* Check to see if the region of interest at this new size exceeds the 640x480
	image boundary. If that is the case, calculate the distance of the x coordinate
	of ROI from the right side and the distance of the y coordinate of ROI from the
	bottom of the image. Divide the distances by their appropriate ratios (width/4
	and height/3) to obtain the sizes. Choose the minimum value to ensure the ROI
	remains within the boundaries of the 640x480 image.
	The reason we divide width by 4 and height by 3 is because for a specific size of
	ROI, we assume a 4/3 ratio of width/height. This will become more clear later. */
	else if ((ROI_x + position * 4 > 640) || (ROI_y + position * 3 > 480))
	{
		ROI_SIZE = min((640 - ROI_x) / 4, (480 - ROI_y) / 3);
	}
	else
	{
		ROI_SIZE = position;
	}
}

int main(int argc, char *argv[])
{
	// variable to store the keyboard input
	int c;

	/* Declare template colors for later use when drawing histograms. The colors are defined
	as CvScalar, an OpenCV primitive data type. CvScalar has a single member "val" which is
	a pointer to an array containing 4 double-percision floating-point numbers. When we are
	not concerned with memory, CvScalar can be used to represent 1,2 or 3 real numbers (in
	these cases the unused component is ignored).*/
	CvScalar color_red, color_blue, color_green, color_white;
	color_red = cvScalar(0, 0, 255, 0);
	color_blue = cvScalar(255, 0, 0, 0);
	color_green = cvScalar(0, 255, 0, 0);
	color_white = cvScalar(255, 255, 255, 0);

	/* Declare the centre of the circles that are going be filled in the selection panel image.
	The centres are defined as cvPoint which is an OpenCV primitive data type for storing two
	integer values. The x and y coordinates of the centres are based on the histogram panel
	image that is going to be loaded later in the code */
	CvPoint pt1, pt2, pt3;
	pt1.x = 70;
	pt1.y = 80;
	pt2.x = 200;
	pt2.y = 80;
	pt3.x = 330;
	pt3.y = 80;

	/* Allocate memory for all images. */
	IplImage *src_img, *histogram, *predisp_img, *disp_img, *zoom_img, *disp_zoom_img, *panel,
		*red_img, *green_img, *blue_img, *red_histogram, *green_histogram, *blue_histogram;

	/* Initialize histogram parameters: "hist_size" is the size of the histogram (or the
	number of bins assigned to the histogram); "range" is the range of intensity values
	considered in the histogram; "*ranges" is a pointer to "range" (we require a pointer
	to "range" because the OpenCV function cvCreateHist accepts the range parameter as
	a pointer to range value pairs). */
	int hist_size = 256;
	float range[] = { 0,256 };
	float *ranges[] = { range };

	/* cvCreateHist function is used to create the histograms. Following is the explanation
	of the various arguments of the function :
	1.  First argument: Defines how many dimensions the histogram will have. To keep things
	simple this tutorial only uses one dimensional histograms.
	2.  Second argument: Defines the size of the dimensions. If we were using higher dimensional
	histograms we would have to pass an array of sizes, one for each dimension, but we are
	only using one dimension so the array we pass only has one value in it. We choose to make
	the size of the dimension 256 because the depth of the images we are working with is 8 bit,
	meaning that we will get one bin for each intensity level. We could have chosen to have
	less bins but the histogram would have less resolution. We could have chosen more bins but
	we would not show any more information.
	3.  Third argument: Tells OpenCV how to store the data in the histogram. There are two
	options for this: CV_HIST_ARRAY and CV_HIST_SPARSE. The second option is useful for
	storing multidimensional histograms that will have most of their bins with counts of
	zero. Since we are using a one dimensional histogram, we don’t expect sparse data so
	we choose the first option.
	4.  Fourth argument: Used to specify the range of values for each dimension.  Each range
	is stored as an array of two values, the minimum and maximum value for that dimension.
	Again we are using one dimension so it looks a bit redundant to make an array of arrays,
	but only with one array. We have to do this though, because this fourth argument expects
	an array to 2 value arrays. It is important to know how OpenCV creates the bins based on
	the range values and the histogram size. OpenCV will take the value supplied in ranges and
	break it into as many sub intervals as defined in hist_size.
	5.  Fifth argument: Defines if the sub intervals are to be split uniformly or not. For
	this case we definitely want each bin to be the same width as the others so we choose
	this argument to be 1.
	*/
	CvHistogram* hist_red = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	CvHistogram* hist_blue = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);
	CvHistogram* hist_green = cvCreateHist(1, &hist_size, CV_HIST_ARRAY, ranges, 1);

	// Declare variables used for finding the maximum value of the histograms
	double max_value = 0;
	double max_value_red = 0;
	double max_value_green = 0;
	double max_value_blue = 0;
	double find_max = 0;

	/* Define the parameter scale which is used when drawing the rectangles of the histogram.
	In our case the scale is 3 because we chose to make the rectangles 2 pixels wide and
	leave a black pixel in between subsequent rectangles. The application of this variable will
	become clear later on in the code*/
	int scale = 3;

	// create the windows for displaying output

	// "Region of Interest" shows the selected region of interest
	cvNamedWindow("Region of Interest", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("Region of Interest", 5, 485);
	// "mainWin" shows the captured image
	cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("mainWin", 5, -15);
	// "histogramWin" shows the histogram
	cvNamedWindow("histogramWin", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("histogramWin", 435, 5);
	// "panelWin" shows the panel for selecting Red channel, Green channel and Blue channel histograms
	cvNamedWindow("panelWin", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("panelWin", 435, 535);

	/* cvCreateTrackbar is a function for creating GUI sliders. The various arguments
	of the function are explained below.
	1.  First argument: declares the name of the trackbar.
	2.  Second argument: name of the parent window to which the trackbar will be attached.
	When the trackbar is added to a parent window, it will either be on top or the
	bottom of the window; it will not occlude the existing image in the window.
	3.  Third argument: a pointer to an integer that will be set automatically to the
	value to which the slider has moved. This argument also detemines the initial
	position of the slider.
	4.  Fourth argument: integer that assigns the numerical value for the maximum value
	of the slider.
	5.  Fifth argument: a pointer to a callback function that will be automatically called
	whenever the slider is moved. */
	cvCreateTrackbar("size", "mainWin", &ROI_SIZE, 160, ROI_size_callback);
	cvCreateTrackbar("x pos", "mainWin", &ROI_x, 640, ROI_xpos_callback);
	cvCreateTrackbar("y pos", "mainWin", &ROI_y, 480, ROI_ypos_callback);

	// Capture from video device #1

	/* Be sure to select the correct video device. Another option is to capture from
	an avi file. In that case use cvCaptureFromAvi("myfile.avi"). If the avi file is
	not in the project folder, include the entire path of the file. Also use \\ instead
	of \ to separate directories. */
	CvCapture* capture = cvCaptureFromCAM(1);

	while (1)
	{
		// Clear all max values to zero
		max_value = 0; max_value_red = 0; max_value_green = 0; max_value_blue = 0;

		/* Load a png image with the histogram axis. Instead of having OpenCV draw the axis,
		the axis is drawn in paint and then loaded as an image. You can download the picture
		here: https://siddhantahuja.files.wordpress.com/2010/05/histogram_scale.png
		Be sure to change the path below to wherever you choose to save the image. */
		histogram = cvLoadImage("histogram_scale.png");

		// Initialize the 3 images that will show the histogram for each channel

		/* cvCreateImage is a function for creating an empty image. The first argument
		must be a CvSize (similze to CvPoint) data type containing the width and the length
		of the image. The second argument is the depth of the image. The last argument is
		the number of channels. In this case for the first argument we use the function
		cvGetSize to obtain the width and length of the histogram axis image loaded previously */
		red_histogram = cvCreateImage(cvGetSize(histogram), IPL_DEPTH_8U, 3);
		green_histogram = cvCreateImage(cvGetSize(histogram), IPL_DEPTH_8U, 3);
		blue_histogram = cvCreateImage(cvGetSize(histogram), IPL_DEPTH_8U, 3);

		// Retrieve captured frame
		src_img = cvQueryFrame(capture);

		/* Create an image half the size of the captured image. This image will be used to
		display the region of interest (after zooming in) */
		disp_zoom_img = cvCreateImage(cvSize((src_img->width) / 2, (src_img->height) / 2), IPL_DEPTH_8U, 3);

		/* Create an image the same size as the captured image. This image will be temporarily used
		to outline the region of interest in the captured image. */
		predisp_img = cvCreateImage(cvSize((src_img->width), (src_img->height)), IPL_DEPTH_8U, 3);

		/* Copy "src_img" to "predisp_img". cvCopy is function for copying one image to another. Both
		images must have the same size and number of channels. The last argument is for passing a mask
		to only copy pixels in a certain region of the image. */
		cvCopy(src_img, predisp_img, 0);

		/* Use the function cvRectangle to draw a rectangle around the region of interest. The ratio of
		widtth/height for the rectangle is 4/3 and the size is determined by the user (by adjusting the
		trackbar for "size"). The various arguments of cvRectangle are explained below:
		1.  First argument: the image in which the rectangle should be drawn.
		2.  Second argument: a cvPoint data type representing one corner of the rectangle.
		3.  Third argument: a cvPoint data type representing the corner of the rectangle opposite
		(diagonally) to the second argument.
		4.  Fourth argument: cvScalar data type for determining the color of the rectangle.
		5.  Last 3 arguments: line thickness, line type (4 or 8 connected), shift (zero in this case) */
		cvRectangle(predisp_img, cvPoint(ROI_x, ROI_y), cvPoint(ROI_x + 4 * ROI_SIZE, ROI_y + 3 * ROI_SIZE), color_red, 4, 4, 0);

		// Create an image smaller than the captured image (resizing done for display purposes)
		disp_img = cvCreateImage(cvSize((src_img->width) / 1.6, (src_img->height) / 1.6), IPL_DEPTH_8U, 3);

		/* Copy "predisp_img" image (which contains the region of interest rectangle) into the smaller
		sized "disp_img" using the cvResize function. The last argument is the interpolation method
		used when converting the image from one size to another. */
		cvResize(predisp_img, disp_img, CV_INTER_LINEAR);

		/**************************** Begin Working Inside the Region of Interest **********************************/

		/* Use the function cvSetImageROI to select only the region of interest from the captured
		image. The first argument is the image which the region of interest is selected from and the
		second argument is a cvRect data type containing information about the region of interest.
		The first two arguments in cvRect represent the coordinates of the top left corner of ROI and
		the last two arguments represent the width and height of the ROI respectively. */
		cvSetImageROI(src_img, cvRect(ROI_x, ROI_y, 4 * ROI_SIZE, 3 * ROI_SIZE));

		// Create 3 single channel images to store each channel data and split the source image into the RGB channels.

		/* Note that each channel image is the same size (width and height) as the source image. The depth is 8 bits
		and these new images have only one channel (since they are storing only one channel). The function cvCvtPixToPlane
		actually separates the source into its three channels and stores them in the images we just created. Instead of
		cvCvtPixToPlane we can also use cvSplit. The order of the channels in cvCvtPixToPlane functio is the same as the
		order of channels in the data array of the source image (in this case BGR) */
		blue_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
		green_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
		red_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);
		cvMat newPic;
		newPic = cvSplit(src_img, blue_img, green_img, red_img);
		//cvCvtPixToPlane(src_img, blue_img, green_img, red_img, 0);
	
		// Calculate a histogram for each channel.

		/*The first argument takes the image we would like to calculate the histogram for.
		Note that cvCalcHist can only take a one channel image. The next argument says which
		histogram will be populated. The third argument turns on or off accumulator mode.
		Since we want the histogram to update for each frame we want to clear the contents
		before adding new ones and the accumulator mode should be off. The final parameter
		is a mask that can tell cvCalcHist to unly calculate a certain part of the image.
		By setting the mask null we calculate for the whole image.*/
		cvCalcHist(&red_img, hist_red, 0, NULL);
		cvCalcHist(&blue_img, hist_blue, 0, NULL);
		cvCalcHist(&green_img, hist_green, 0, NULL);

		// Creat an image the same size as the region of interest
		/* It should be noted that cvGetSize in this case will not return the size of
		the original captured image. It will return the size of the region of interest
		set using the cvSetImageROI function. */
		zoom_img = cvCreateImage(cvGetSize(src_img), 8, 3);

		// Copy the region of interest from the captured image, into the image created above
		cvCopy(src_img, zoom_img, 0);

		// Resize the region of interest into the larger "disp_zoom" image
		cvResize(zoom_img, disp_zoom_img, CV_INTER_LINEAR);

		// Turn off the region of interest using cvResetImageROI function
		cvResetImageROI(src_img);

		/*********************** Finished Working Inside the Region of Interest *********************************/

		// Search through the histograms for their maximum value and store it

		/* The code for finding the maximum value of the histogram for each channel is shown below. It is done with a
		simple for loop that checks every value in each bin. The function used to get the values out of the histogram is
		cvQueryHistValue_1D. This function is made for getting values out of a one dimensional histogram. Its arguments
		are the histogram you want to look into, and the bin number you want to see. The reason we need to find this
		max value is that we will scale the y-axis of the histogram so that it fits on the screen. We do this by dividing
		each bin value by the max value (this is done in the next step).

		An alternative method of obtaining the maximum values of the histogram is using the function cvGetMinMaxHistValue.
		As an example we can find the maximum value of the red channel histogram by using the code
		"cvGetMinMaxHistValue(hist_red,0,&max_value_red,0,0)". */
		for (int i = 0; i < hist_size; i++)
		{
			find_max = cvQueryHistValue_1D(hist_red, i);
			if (find_max > max_value_red)
			{
				max_value_red = find_max;
			}
		}
		for (int i = 0; i < hist_size; i++)
		{
			find_max = cvQueryHistValue_1D(hist_green, i);
			if (find_max > max_value_green)
			{
				max_value_green = find_max;
			}
		}
		for (int i = 0; i < hist_size; i++)
		{
			find_max = cvQueryHistValue_1D(hist_blue, i);
			if (find_max > max_value_blue)
			{
				max_value_blue = find_max;
			}
		}

		// Print the maximum value of the histogram for each channel
		printf("RED   MAX = %f\n", max_value_red);
		printf("GREEN MAX = %f\n", max_value_green);
		printf("BLUE  MAX = %f\n", max_value_blue);

		// Calculate the maximum value of all histograms

		/* When calculating the maximum value of all histograms we must first check to see
		which histograms are selected by the user to be displayed. This is done by the following
		if-else statements checking the flag variables assigned to each histogram (red,green,blue) */
		if (CLICK_red)
		{
			if (CLICK_green)
			{
				if (CLICK_blue)
				{
					max_value = max(max(max_value_red, max_value_green), max_value_blue);
				}
				else
				{
					max_value = max(max_value_red, max_value_green);
				}
			}
			else
			{
				if (CLICK_blue)
				{
					max_value = max(max_value_red, max_value_blue);
				}
				else
				{
					max_value = max_value_red;
				}
			}
		}
		else if (CLICK_green)
		{
			if (CLICK_blue)
			{
				max_value = max(max_value_green, max_value_blue);
			}
			else
			{
				max_value = max_value_green;
			}
		}
		else
		{
			if (CLICK_blue)
			{
				max_value = max_value_blue;
			}
			else
			{
				//do nothing
			}
		}

		// Print the overall maximum value of histograms
		printf("MAXIMUM = %f\n", max_value);

		// Draw the histogram for each channel, if the flag for that channel is set to 1

		/* First we see an if statement that controls whether or not we draw the histogram. It is based
		on a flag that is set in the next step by user mouse input. This allows the user to select which channel
		or combination of channels that they want to see. The next step is using the function cvScale to scale
		the histogram bin values. Its first argument is the source to be scaled and the second is the destination
		for result. Here we used the same place for source and destination meaning that the bins are scaled and then
		stored back in the same place. The last argument is the scale factor. A factor of 438/max_value was
		used because the highest bar that we want to draw is 438 pixels high (so it does not go out of the
		bounds of the picture we used for the axis).

		Inside the for loop we see a line of code that draws all the rectangles representing the bars of the
		histogram. The function cvRectangle is used to do this. As explained previously, it requires that we pass
		it which image to draw on, two points to define the rectangles, the color, line thickness, line type
		and shift value. The image used to draw the histogram is the corresponding empty image created for each
		separate channel. The first point of the rectangle that is selected is the bottom left corner. In our case
		we have chosen to start drawing the rectangles from the 15th pixel of image form the left side and the 448th
		pixel from the top. These values are chosen based on the dimensions of the specific histogram axis image
		loaded previously. Be sure to adjust these values if you choose to change the image which the histograms are
		drawn on. We have also decided to make the rectangles 2 pixels wide and leave a black pixel in between each
		subsequent rectangle. Hence, we multiply i (histogram bin) by scale (set to 3) to make sure the rectangles
		are drawn every 3 pixels. The second point of the rectangle is the top right corner. Since the rectangles
		are chosen to be 2 pixels wide, the width coordinate of this point is one pixel to the right of the first
		point. The height coordinate of the second point is calculated by subtracting the bin value from 448 (the
		bottom of the rectangle). Since the bins were scaled to have a maximum value of 438, there is a minimum
		distance of 10 pixels  between the top of the histogram scale image and the rectangles. Once again these
		values are determined specifically for our axis image and they should be adjusted accordingly if the image
		is changed. We specify line thickness as -1 (which means a filled rectangle) and line type as 8(8-connected
		lines). One other issue that should be noted is the use of the cvRound function. cvRound is used to convert
		the value obtained from cvQueryHistValue_1D to an integer.

		The last function is cvAdd (performs elementwise addition of two arrays) which is used to simplify drawing
		the multiple histograms with overlap. The first two arguments are the source images added to each other.
		The third argument is the destination image which in this case is the histogram axis image. The final
		argument is for passing a mask (in this case 0) to only add specific regions of the two images. By adding
		colors on top of each other no extra code is required to deal with the overlap of different histograms. */
		if (CLICK_red == 1)
		{

			cvScale(hist_red->bins, hist_red->bins, 438 / max_value, 0);
			for (int i = 0; i < hist_size; i++)
			{
				cvRectangle(red_histogram, cvPoint(i*scale + 15, 448), cvPoint(i*scale + 16, 448 - cvRound(cvQueryHistValue_1D(hist_red, i))), color_red, -1, 8, 0);
			}
			cvAdd(histogram, red_histogram, histogram, 0);
		}
		if (CLICK_green == 1)
		{

			cvScale(hist_green->bins, hist_green->bins, 438 / max_value, 0);
			for (int i = 0; i < hist_size; i++)
			{
				cvRectangle(green_histogram, cvPoint(i*scale + 15, 448), cvPoint(i*scale + 16, 448 - cvRound(cvQueryHistValue_1D(hist_green, i))), color_green, -1, 8, 0);
			}
			cvAdd(histogram, green_histogram, histogram, 0);
		}
		if (CLICK_blue == 1)
		{

			cvScale(hist_blue->bins, hist_blue->bins, 438 / max_value, 0);
			for (int i = 0; i < hist_size; i++)
			{
				cvRectangle(blue_histogram, cvPoint(i*scale + 15, 448), cvPoint(i*scale + 16, 448 - cvRound(cvQueryHistValue_1D(hist_blue, i))), color_blue, -1, 8, 0);
			}
			cvAdd(histogram, blue_histogram, histogram, 0);
		}

		/* Load a png image with the selection panel drawn in paint. The coordinates setup up in
		the mouse callback function for selecting the different channels is according to this specific
		image. If you choose to make your own panel image make sure the right adjustments are made
		in the code. You can download the picture here:
		<a href="https://siddhantahuja.files.wordpress.com/2010/05/histogram_panel1.png"><img src="https://siddhantahuja.files.wordpress.com/2010/05/histogram_panel1.png?w=510" style="max-width:100%;" originalw="510" width="400" height="110" scale="2"></a>
		Be sure to change the path below to wherever you choose to save the image. */
		panel = cvLoadImage("Histogram_panel.png");

		/* Declare the function cvSetMouseCallback which registers a callback whenever a mouse event occurs.
		In other words we want to call the my_mouse_callback function (defined after "main") whenever the user
		doese something with the mouse over a particular window (in our case the panel selection window).
		The first argument of cvSetMouseCallback function is the window which the callback is registered to
		(only mouse events in this particular window will trigger the callback function). The second argument
		is the name of the callback function which is "my_mouse_callback". The final argument is a void pointer
		that can be used to pass any additional information to the callback function in the form of a pointer
		to whatever kind of structure that is needed. The void pointer is commonly used when the callback is a
		static member function of a class (object oriented programming). In our case we set the pointer to the
		panel image but we do not need to perform any operations on the panel image inside the callback function
		itself */
		cvSetMouseCallback("panelWin", my_mouse_callback, (void*)panel);

		/* Draw circles in designated area of the histogram panel image to indicate which histogram is being
		displayed. The function used for drawing circles is cvCircle. The first argument is the image in which
		the circle is drawn. The second argument is a cvPoint data type that determines the centre of the circle.
		The coordinates of the circle centres are set at the beginning of "main" according to the circle areas on
		the panel image. The third and fourth arguments are the radius and the color of the circle. The radius in
		our case is 7 pixels and the color (which corresponds to the histogram of the channel being displayed) is
		one of the template colors defined at the beginning of "main". The last three arguments are line thickness,
		line type and shift which are the same as the parameters in cvRectangle function. */
		if (CLICK_red == 1)
		{
			cvCircle(panel, pt1, 7, color_red, -1, 9, 0);
		}
		else
		{
			cvCircle(panel, pt1, 7, color_white, -1, 9, 0);
		}
		if (CLICK_green == 1)
		{
			cvCircle(panel, pt2, 7, color_green, -1, 9, 0);
		}
		else
		{
			cvCircle(panel, pt2, 7, color_white, -1, 9, 0);
		}
		if (CLICK_blue == 1)
		{
			cvCircle(panel, pt3, 7, color_blue, -1, 9, 0);
		}
		else
		{
			cvCircle(panel, pt3, 7, color_white, -1, 9, 0);
		}

		// Show the images in the windows
		cvShowImage("mainWin", disp_img);
		cvShowImage("Region of Interest", disp_zoom_img);
		cvShowImage("histogramWin", histogram);
		cvShowImage("panelWin", panel);

		/* Wait for a key input for 10 milliseconds. If the value in cvWaitKey function is 0, the program
		will wait indefinitely until a key is pressed by the user. */
		c = cvWaitKey(10);

		// Terminate program if Esc key is pressed
		if (c == 27)break;

		// release the images
		cvReleaseImage(&red_img);
		cvReleaseImage(&green_img);
		cvReleaseImage(&blue_img);
		cvReleaseImage(&red_histogram);
		cvReleaseImage(&blue_histogram);
		cvReleaseImage(&green_histogram);
		cvReleaseImage(&predisp_img);
		cvReleaseImage(&disp_img);
		cvReleaseImage(&histogram);
		cvReleaseImage(&zoom_img);
		cvReleaseImage(&disp_zoom_img);
		cvReleaseImage(&panel);

	}
	return 0;
}

/* Define the function that is called everytime a mouse event occurs in the selection panel window.
Whenever a mouse event occurs, OpenCV will fill the arguments of the callback function with their
appropriate values. The first argument is an integer representing the type of event that has occured
(e.g. mousemove, left button down, right button double click, etc). The second and third arguments are
the coordinates of the mouse pointer when the event occured. The fourth argument is a flag that indicates
special conditions present at the time of the mouse event (such as the shift key being pressed). The last
argument is a pointer that was passed to the callback function when the event occured. As explained
previously the pointer points to the histogram panel image. For our purpose we will not be using the last
two arguments. */
void my_mouse_callback(int event, int x, int y, int flags, void* param)
{
	switch (event)
	{
		// Check to see if left mouse button was pressed
	case CV_EVENT_LBUTTONDOWN:
	{
		/* Change the flag variables for histogram display based on the
		coordinates of the mouse event. The ranges correspond to the rectangles
		in the histogram panel image. If a different image is selected these
		values need to be adjusted appropriately. The R,G,B flags are XORed
		with 1 to change states (they act like a toggle switch that is
		triggered by left mouse click).*/
		if ((x > 10 && x < 130) && (y > 10 && y < 60))
		{
			CLICK_red = CLICK_red ^ 1;
		}
		else if ((x > 140 && x < 260) && (y > 10 && y < 60))
		{
			CLICK_green = CLICK_green ^ 1;
		}
		else if ((x > 270 && x < 390) && (y > 10 && y < 60))
		{
			CLICK_blue = CLICK_blue ^ 1;
		}
	}break;
	}
}