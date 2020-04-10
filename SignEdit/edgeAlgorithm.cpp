#include "StdAfx.h"
#include "edgeAlgorithm.h"

namespace EdgeAlg
{
	bool Robert(Mat & inputImage, Mat & outputImage, int dx, int dy)
	{
		if (inputImage.empty()) return false;
		Mat imgSrc;
		if (inputImage.channels() > 1)
		{
			cvtColor(inputImage, imgSrc, COLOR_BGR2GRAY);
		}
		else
		{
			inputImage.copyTo(imgSrc);
		}

		Mat imgProc(imgSrc.rows + 1, imgSrc.cols + 1, imgSrc.type());
		for (int i = 0; i <= imgSrc.rows; i++)
		{
			if (i == imgSrc.rows)
			{
				memcpy(imgProc.ptr<uchar>(i), imgProc.ptr<uchar>(i - 1), imgProc.cols);
				break;
			}
			for (int j = 0; j <= imgSrc.cols; j++)
			{
				if (j == imgSrc.cols)
				{
					imgProc.at<uchar>(i, j) = imgProc.at<uchar>(i, j - 1);
					break;
				}
				imgProc.at<uchar>(i, j) = imgSrc.at<uchar>(i, j);
			}
		}

		Mat grad_x = Mat::zeros(imgSrc.size(), CV_16S);
		Mat grad_y = Mat::zeros(imgSrc.size(), CV_16S);;
		if (dx)
		{
			for (int i = 0; i < imgSrc.rows; i++)
			{
				int16_t* pGradXline = grad_x.ptr<int16_t>(i);
				int16_t* pGradYline = grad_y.ptr<int16_t>(i);
				for (int j = 0; j < imgSrc.cols; j++)
				{
					pGradXline[j] = imgProc.at<uchar>(i + 1, j + 1) - imgProc.at<uchar>(i, j);
				}
			}
		}
		if (dy)
		{
			for (int i = 0; i < imgSrc.rows; i++)
			{
				int16_t* pGradYline = grad_y.ptr<int16_t>(i);
				for (int j = 0; j < imgSrc.cols; j++)
				{
					pGradYline[j] = imgProc.at<uchar>(i + 1, j) - imgProc.at<uchar>(i, j + 1);
				}
			}
		}

		Mat imgTmp;
		if (dx)
		{
			imgTmp = Mat::zeros(grad_x.size(), CV_32F);
		}
		else if (dy)
		{
			imgTmp = Mat::zeros(grad_y.size(), CV_32F);
		}
		else
		{
			return false;
		}

		for (int i = 0; i < imgSrc.rows; i++)
		{
			for (int j = 0; j < imgSrc.cols; j++)
			{
				imgTmp.at<float>(i, j) = std::sqrt(std::pow(grad_x.at<int16_t>(i, j), 2) + std::pow(grad_y.at<int16_t>(i, j), 2));
			}
		}

		Mat imgNorm;
		normalize(imgTmp, imgNorm, 1.0, 0.0, NORM_MINMAX);

		outputImage = Mat::zeros(imgNorm.size(), CV_8U);
		for (int i = 0; i < imgSrc.rows; i++)
		{
			for (int j = 0; j < imgSrc.cols; j++)
			{
				outputImage.at<uchar>(i, j) = (int)(imgNorm.at<float>(i, j) * 255 + 0.5);
			}
		}

		return true;
	}

	bool Canny(Mat & inputImage, Mat & outputImage, double threshold1, double threshold2, int kHalfSize, bool L2gradient)
	{
		if (inputImage.empty())
		{
			return false;
		}

		if (threshold1 < 0 || threshold2 < 0)
		{
			return false;
		}

		if (kHalfSize <= 0) return false;
		int apertureSize = kHalfSize * 2 + 1;

		Mat imgSrc;
		if (inputImage.channels() > 1)
		{
			cvtColor(inputImage, imgSrc, COLOR_BGR2GRAY);
		}
		else
		{
			inputImage.copyTo(imgSrc);
		}

		cv::Canny(imgSrc, outputImage, threshold1, threshold2, apertureSize, L2gradient);

		return true;
	}

	bool Sobel(Mat & inputImage, Mat & outputImage, int dx, int dy, int kSize, double scale, double delta, int borderType)
	{
		if (inputImage.empty())
		{
			return false;
		}

		Mat imgSrc;
		if (inputImage.channels() > 1)
		{
			cvtColor(inputImage, imgSrc, cv::COLOR_BGR2GRAY);
		}
		else
		{
			inputImage.copyTo(imgSrc);
		}

		Mat grad_x = Mat::zeros(imgSrc.size(), CV_16S);
		Mat	grad_y = Mat::zeros(imgSrc.size(), CV_16S);
		if (dx)
		{
			Sobel(imgSrc, grad_x, CV_16S, 1, 0, kSize, scale, delta, borderType);
		}
		if (dy)
		{
			Sobel(imgSrc, grad_y, CV_16S, 0, 1, kSize, scale, delta, borderType);
		}

		Mat imgTmp;
		if (dx)
		{
			imgTmp = Mat::zeros(grad_x.size(), CV_32F);
		}
		else if (dy)
		{
			imgTmp = Mat::zeros(grad_y.size(), CV_32F);
		}
		else
		{
			return false;
		}

		for (int i = 0; i < imgTmp.rows; i++)
		{
			float* pTmpDataLine = imgTmp.ptr<float>(i);
			int16_t* pXGradDataLine = grad_x.ptr<int16_t>(i);
			int16_t* pYGradDataLine = grad_y.ptr<int16_t>(i);
			for (int j = 0; j < imgTmp.cols * imgTmp.channels(); j++)
			{
				pTmpDataLine[j] = std::sqrt(std::pow(pXGradDataLine[j], 2) + std::pow(pYGradDataLine[j], 2));
			}
		}
		Mat imgTmpNorm;
		normalize(imgTmp, imgTmpNorm, 1.0, 0.0, NORM_MINMAX);

		outputImage = Mat::zeros(imgTmpNorm.size(), CV_8U);
		for (int i = 0; i < outputImage.rows; i++)
		{
			uchar* pResDataLine = outputImage.ptr<uchar>(i);
			float* pTmpNormDataLine = imgTmpNorm.ptr<float>(i);
			for (int j = 0; j < outputImage.cols; j++)
			{
				pResDataLine[j] = (int)(pTmpNormDataLine[j] * 255 + 0.5);
			}
		}

		return true;
	}

	bool Laplacian(Mat & inputImage, Mat & outputImage, int kSize, double scale, double delta, int dorderType)
	{
		if (inputImage.empty()) return false;
		if (kSize < 0) return false;

		Mat imgSrc;
		if (inputImage.channels() > 1)
		{
			cvtColor(inputImage, imgSrc, COLOR_BGR2GRAY);
		}
		else
		{
			inputImage.copyTo(imgSrc);
		}

		cv::Laplacian(imgSrc, outputImage, CV_16S, kSize, scale, delta, dorderType);

		return true;
	}

	bool Gabor(Mat & inputImage, Mat & outputImage, Size GaborKernelSize, double sigma, double theta, double lambda, double gamma, double psi, Point anchor, double delta, int borderType)
	{
		if (inputImage.empty()) return false;

		Mat imgSrc;
		if (inputImage.channels() > 1)
		{
			cvtColor(inputImage, imgSrc, COLOR_BGR2GRAY);
		}
		else
		{
			inputImage.copyTo(imgSrc);
		}

		Mat gaborKernel = cv::getGaborKernel(GaborKernelSize, sigma, theta, lambda, gamma, psi * CV_PI);
		cv::filter2D(imgSrc, outputImage, CV_8U, gaborKernel, anchor, delta, borderType);
	}

	bool RegionGrow(Mat & inputImage, Mat & outputImage, cv::Point seed, int threshold)
	{
		if (inputImage.empty()) return false;
		Mat imgSrc;
		if (inputImage.channels() > 1)
			cvtColor(inputImage, imgSrc, COLOR_BGR2GRAY);
		else
			inputImage.copyTo(imgSrc);
		if (seed.x < 0 || seed.y < 0 || seed.x > imgSrc.cols || seed.y > imgSrc.rows) return false;
		if (threshold < 0 || threshold > 255) return false;
		
		static int nDx[] = { -1, 0, 1, 0 };
		static int nDy[] = { 0, 1, 0, -1 };
		stack<cv::Point> GrowStk;
		GrowStk.push(seed);
		cv::Point CurPoint;
		outputImage = Mat::zeros(imgSrc.size(), CV_8U);
		outputImage.at<uchar>(seed) = 255;

		while (GrowStk.size() > 0)
		{
			CurPoint = GrowStk.top();
			GrowStk.pop();
			
			for (int i = 0; i < 4; i++)
			{
				cv::Point PointTmp;
				PointTmp.x = CurPoint.x + nDx[i];
				PointTmp.y = CurPoint.y + nDy[i];
				if (PointTmp.x > 0 &&
					PointTmp.x < imgSrc.cols &&
					PointTmp.y > 0 &&
					PointTmp.y < imgSrc.rows &&
					abs(imgSrc.at<uchar>(PointTmp) - imgSrc.at<uchar>(CurPoint)) < threshold &&
					outputImage.at<uchar>(PointTmp) != 255)
				{
					outputImage.at<uchar>(PointTmp) = 255;
					GrowStk.push(PointTmp);
				}
			}
		}
	}

};