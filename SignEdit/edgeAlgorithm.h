#pragma once

#include "opencv2/opencv.hpp"
#include <cmath>
#include <stack>

using namespace std;
using namespace cv;

namespace EdgeAlg
{
	bool Robert(Mat& inputImage, Mat& outputImage, int dx, int dy);
	bool Canny(Mat& inputImage, Mat& outputImage, double threshold1, double threshold2, int apertureSize, bool L2gradient);
	bool Sobel(Mat& inputImage, Mat& outputImage, int dx, int dy, int kSize, double scale, double delta, int borderType);
	bool Laplacian(Mat& inputImage, Mat& outputImage, int kSize, double scale, double delta, int dorderType);
	bool Gabor(Mat& inputImage, Mat& outputImage, Size GaborKernelSize, double sigma, double theta, double lambda, double gamma, double psi, Point anchor, double delta, int borderType);
	bool RegionGrow(Mat& inputImage, Mat& outputImage, cv::Point seed, int threshold);
	
};
