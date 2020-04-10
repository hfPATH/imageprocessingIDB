#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>
#include <unordered_map>


using namespace std; 

extern bool LoadVectorFromBMPFile(CString &FileName, BYTE *&pBitmapData, int &dwHeight, int &dwWidth,WORD &flag);
extern bool WriteBMPFileFromVector(CString &FileName, BYTE *pBitmapData,int dwHeight,int dwWidth,WORD flag);
extern bool TranslateVectorToBitmap(int dwHeight,int dwWidth,BYTE* pVector,BYTE*& pBitmapData,WORD flag);
extern bool TranslateBitmapToVector(int dwHeight,int dwWidth,BYTE* pBitmapData,BYTE*& pVector,WORD flag);
extern bool CreateBITMAPINFO(BITMAPINFO *&pbi,int dwHeight, int dwWidth,WORD flag);


namespace proc 
{
	//BGR转灰度
	void BGR2Gray(BYTE* pBgr, BYTE*& pGray, int nW, int nH)
	{
		BYTE* pTmp = new BYTE[nH * nW];
		for (int i = 0; i < nW * nH; ++i)
		{
			pTmp[i] = (pBgr[i * 3] + pBgr[i * 3 + 1] + pBgr[i * 3 + 2]) / 3;
		}

		if (pGray) { delete[] pGray; }
		pGray = new BYTE[nH * nW];
		memcpy(pGray, pTmp, nH * nW);

		delete[] pTmp;
		pTmp = nullptr;
	}

	//灰度转BGR
	void Gray2BGR(BYTE* pGray, BYTE*& pBgr, int nW, int nH)
	{
		BYTE* pTmp = new BYTE[nH * nW * 3];
		for (int i = 0; i < nW * nH; ++i)
		{
			pTmp[i * 3] = pTmp[i * 3 + 1] = pTmp[i * 3 + 2] = pGray[i];
		}
		
		if (pBgr) { delete[] pBgr; }
		pBgr = new BYTE[nH * nW * 3];
		memcpy(pBgr, pTmp, nH * nW * 3);

		delete[] pTmp;
		pTmp = nullptr;
	}


	//投影到水平方向
	void ProjectHorizon(BYTE* pBmpSrc, int nW, int nH, int*& pProjH)
	{
		if (pProjH)
		{
			delete pProjH;
		}
		pProjH = new int[nW];
		memset(pProjH, 0, nW * sizeof(int));
	
		for (int i = 0; i < nH; ++i)
		{
			for (int j = 0; j < nW; ++j)
			{
				pProjH[j] += pBmpSrc[i * nW + j];
			}
		}
	}

	//投影到竖直方向
	void ProjectVertical(BYTE* pBmpSrc, int nW, int nH, int*& pProjH)
	{
		if (pProjH)
		{
			delete pProjH;
		}
		pProjH = new int[nH];
		memset(pProjH, 0, nH * sizeof(int));

		for (int i = 0; i < nH; ++i)
		{
			for (int j = 0; j < nW; ++j)
			{
				pProjH[i] += pBmpSrc[i * nW + j];
			}
		}
	}

	//显示数组
	cv::Mat Vector2Mat(int* pPrj, int nLength, bool isVertical = false)
	{
		int nMin = pPrj[0];
		int nMax = pPrj[0];
		for (int i = 0; i < nLength; ++i)
		{
			if (pPrj[i] > nMax)
			{
				nMax = pPrj[i];
			}
			else if (pPrj[i] < nMin)
			{
				nMin = pPrj[i];
			}
		}
	

		if (isVertical)
		{
			cv::Mat projImg(nLength, 256, CV_8U, cv::Scalar(200));

			for (int i = 0; i < nLength; ++i)
			{
				cv::line(projImg, cv::Point(255 - (pPrj[i] - nMin) * 255 / (nMax - nMin), i), cv::Point(255, i), cv::Scalar::all(0));
			}

			return projImg;
		}
		else
		{
			cv::Mat projImg(256, nLength, CV_8U, cv::Scalar(200));

			for (int i = 0; i < nLength; ++i)
			{
				cv::line(projImg, cv::Point(i, 255 - (pPrj[i] - nMin) * 255 / (nMax - nMin)), cv::Point(i, 255), cv::Scalar::all(0));
			}

			return projImg;
		}
	}

	//直方图均衡化
	cv::Mat HistEqualization(cv::Mat& image)
	{
		cv::Mat img_gray;
		if (image.channels() == 3)
		{
			cvtColor(image, img_gray, CV_BGR2GRAY);
		}
		else
		{
			image.copyTo(img_gray);
		}

		MatND hist;       
		int dims = 1;
		float hranges[] = { 0, 255 };
		const float *ranges[] = { hranges };    
		int size = 256;
		int channels = 0;
		cv::calcHist(&img_gray, 1, &channels, Mat(), hist, dims, &size, ranges);  
		cv::Mat equalize_Hist;
		cv::equalizeHist(img_gray, equalize_Hist);

		return equalize_Hist;
	}

	bool Cal_Hist(cv::Mat Gray_img, cv::MatND &hist) 
	{
		int bins = 256;
		int hist_size[] = { bins };
		float range[] = { 0, 256 };
		const float* ranges[] = { range };

		int channels[] = { 0 };
		//计算直方图    
		calcHist(&Gray_img, 1, channels, Mat(), // do not use mask      
			hist, 1, hist_size, ranges,
			true, // the histogram is uniform      
			false);
		if (hist.data == 0) return false;
		return true;
	}

	//直方图匹配，输入灰度图像
	void HistSpecification(cv::Mat input_img, cv::Mat dst_img, cv::Mat &output_img)//单通道  
	{
		int i, j;

		//计算输入，规定图像的直方图  
		cv::MatND input_hist, dst_hist;
		Cal_Hist(input_img, input_hist);
		Cal_Hist(dst_img, dst_hist);

		//计算概率直方图  
		cv::MatND input_p_hist, dst_p_hist;
		input_p_hist = cv::MatND::zeros(input_hist.size[0], input_hist.size[1], CV_32FC1);//原始概率直方图  
		dst_p_hist = cv::MatND::zeros(dst_hist.size[0], dst_hist.size[1], CV_32FC1);//规定概率直方图  
		float input_totalnum = 0;
		float dst_totalnum = 0;
		for (i = 0; i < input_hist.rows; i++)
			input_totalnum += input_hist.at<float>(i);
		for (i = 0; i < dst_hist.rows; i++)
			dst_totalnum += dst_hist.at<float>(i);
		for (i = 0; i < input_hist.rows; i++)
			input_p_hist.at<float>(i) = input_hist.at<float>(i) / input_totalnum;
		for (i = 0; i < dst_hist.rows; i++)
			dst_p_hist.at<float>(i) = dst_hist.at<float>(i) / dst_totalnum;

		//计算累计直方图  
		cv::MatND input_c_hist, dst_c_hist;
		input_c_hist = cv::MatND::zeros(input_hist.size[0], input_hist.size[1], CV_32FC1);//原始累计直方图  
		dst_c_hist = cv::MatND::zeros(dst_hist.size[0], dst_hist.size[1], CV_32FC1);//规定累计直方图  
		float input_accum_p = 0;
		float dst_accum_p = 0;
		for (i = 0; i < input_hist.rows; i++)
		{
			input_accum_p += input_p_hist.at<float>(i);
			input_c_hist.at<float>(i) = input_accum_p;
		}
		for (i = 0; i < dst_hist.rows; i++)
		{
			dst_accum_p += dst_p_hist.at<float>(i);
			dst_c_hist.at<float>(i) = dst_accum_p;
		}

		//计算单映射规则  
		cv::MatND SML = cv::MatND::zeros(input_hist.size[0], input_hist.size[1], CV_32FC1);//SML单映射规则  
		for (i = 0; i < input_c_hist.rows; i++)
		{
			int minind = 0;
			float minval = 1;
			for (j = 0; j < dst_c_hist.rows; j++)
			{
				float abssub = abs(input_c_hist.at<float>(i) - dst_c_hist.at<float>(j));
				if (abssub < minval)
				{
					minval = abssub;
					minind = j;
				}
			}
			SML.at<float>(i) = minind;
		}

		//计算输出图像  
		output_img.release();
		output_img = cv::Mat::zeros(input_img.size[0], input_img.size[1], CV_8UC1);
		for (i = 0; i < input_img.rows; i++)
		{
			for (j = 0; j < input_img.cols; j++)
			{
				output_img.at<uchar>(i, j) = SML.at<float>(input_img.at<uchar>(i, j));
			}
		}
	}

	//几何均值
	void GeometricMeanFilter(Mat& imgSrc, Mat& resDst, int dwHeight, int dwWidth, int nDepth, int filtRadius)
	{
		int i, j, k, m, n;
		int nCurrRow, nCurrCol;
		double dTemp;
		const int& nChannels = nDepth / 8;
		const double& dPower = 1.0 / (2 * filtRadius + 1) / (2 * filtRadius + 1);	//幂

		BYTE* pTemp = new BYTE[dwHeight * dwWidth * nChannels];

		for (i = 0; i < dwHeight; i++)
			for (j = 0; j < dwWidth; j++)
				for (k = 0; k < nChannels; k++)
				{
					dTemp = 1.0;
					for (m = -filtRadius; m <= filtRadius; m++)
						for (n = -filtRadius; n <= filtRadius; n++)
						{
							//判断边界
							//判断边界
							nCurrRow = i + m > 0 ? i + m : 0;
							nCurrRow = nCurrRow < dwHeight ? nCurrRow : dwHeight - 1;
							nCurrCol = j + n > 0 ? j + n : 0;
							nCurrCol = nCurrCol < dwWidth ? nCurrCol : dwWidth - 1;

							dTemp *= pow(imgSrc.data[(nCurrRow * dwWidth + nCurrCol) * nChannels + k], dPower);
						}
					pTemp[(i * dwWidth + j) * nChannels + k] = dTemp > 255 ? 255 : int(dTemp);
				}

		//if (pDstBmp)	delete pDstBmp;
		//BYTE* pDstBmp = new BYTE[dwWidth * dwHeight * nChannels];
		//::memcpy(pDstBmp, pTemp, dwWidth * dwHeight * nChannels);
		//if (resDst) delete resDst;
		resDst = Mat::zeros(dwHeight, dwWidth, CV_8U);
		memcpy(resDst.data, pTemp, dwHeight * dwWidth * sizeof(BYTE) * nChannels);
		delete[] pTemp;
		pTemp = NULL;
	}

	//谐波均值
	void HarmonicMeanFilter(Mat& imgSrc, Mat& resDst, int dwHeight, int dwWidth, int nDepth, int filtRadius)
	{
		int i, j, k, m, n;
		int nCurrRow, nCurrCol;
		double dTemp;
		const int& nChannels = nDepth / 8;
		const int& nSize = (2 * filtRadius + 1) * (2 * filtRadius + 1);

		BYTE* pTemp = new BYTE[dwHeight * dwWidth * nChannels];

		for (i = 0; i < dwHeight; i++)
			for (j = 0; j < dwWidth; j++)
				for (k = 0; k < nChannels; k++)
				{
					dTemp = 0.0;
					for (m = -filtRadius; m <= filtRadius; m++)
						for (n = -filtRadius; n <= filtRadius; n++)
						{
							//判断边界
							nCurrRow = i + m > 0 ? i + m : 0;
							nCurrRow = nCurrRow < dwHeight ? nCurrRow : dwHeight - 1;
							nCurrCol = j + n > 0 ? j + n : 0;
							nCurrCol = nCurrCol < dwWidth ? nCurrCol : dwWidth - 1;

							if (imgSrc.data[(nCurrRow * dwWidth + nCurrCol) * nChannels + k] == 0)
								imgSrc.data[(nCurrRow * dwWidth + nCurrCol) * nChannels + k] ++;	//避免除0

							dTemp += 1.0 / imgSrc.data[(nCurrRow * dwWidth + nCurrCol) * nChannels + k];
						}
					dTemp = nSize / dTemp;
					pTemp[(i * dwWidth + j) * nChannels + k] = dTemp > 255 ? 255 : int(dTemp);
				}

		//if (pDstBmp)	delete pDstBmp;
		resDst = Mat::zeros(dwHeight, dwWidth, CV_8U);
		memcpy(resDst.data, pTemp, dwHeight * dwWidth * sizeof(BYTE) * nChannels);

		delete pTemp;
		pTemp = NULL;
	}


	//逆谐波均值
	void InvHarmonicMeanFilter(Mat& imgSrc, Mat& resDst, int dwHeight, int dwWidth, int nDepth, int filtRadius, int dPower)
	{
		int i, j, k, m, n;
		double dTemp, dTemp1, dTemp2;
		int nCurrRow, nCurrCol;
		const int& nChannels = nDepth / 8;
		const int& nSize = (2 * filtRadius + 1) * (2 * filtRadius + 1);
		BYTE* pTemp = new BYTE[dwHeight * dwWidth * nChannels];

		for (i = 0; i < dwHeight; i++)
			for (j = 0; j < dwWidth; j++)
				for (k = 0; k < nChannels; k++)
				{
					dTemp1 = dTemp2 = 0;
					for (m = -filtRadius; m <= filtRadius; m++)
						for (n = -filtRadius; n <= filtRadius; n++)
						{
							//判断边界
							nCurrRow = i + m > 0 ? i + m : 0;
							nCurrRow = nCurrRow < dwHeight ? nCurrRow : dwHeight - 1;
							nCurrCol = j + n > 0 ? j + n : 0;
							nCurrCol = nCurrCol < dwWidth ? nCurrCol : dwWidth - 1;

							if (imgSrc.data[(nCurrRow * dwWidth + nCurrCol) * nChannels + k] == 0)
								imgSrc.data[(nCurrRow * dwWidth + nCurrCol) * nChannels + k] ++;

							dTemp1 += pow(imgSrc.data[(nCurrRow * dwWidth + nCurrCol) * nChannels + k], dPower + 1);
							dTemp2 += pow(imgSrc.data[(nCurrRow * dwWidth + nCurrCol) * nChannels + k], dPower);
						}
					if (dTemp2 == 0)
						dTemp = 0;
					else
						dTemp = dTemp1 / dTemp2;

					pTemp[(i * dwWidth + j) * nChannels + k] = dTemp < 255 ? int(dTemp + 0.5) : 255;
				}

		//if (pDstBmp)	delete pDstBmp;
		//pDstBmp = new BYTE[dwWidth * dwHeight * nChannels];
		//::memcpy(pDstBmp, pTemp, dwWidth * dwHeight * nChannels);
		resDst = Mat::zeros(dwHeight, dwWidth, CV_8U);
		memcpy(resDst.data, pTemp, dwHeight * dwWidth * sizeof(BYTE) * nChannels);
		delete pTemp;
		pTemp = NULL;
	}
	
};

