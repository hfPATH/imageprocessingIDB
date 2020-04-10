// ZXDib.h: interface for the CZXDib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZXDIB_H__8BCD4139_996B_446F_BC26_4DBDC7FA84F1__INCLUDED_)
#define AFX_ZXDIB_H__8BCD4139_996B_446F_BC26_4DBDC7FA84F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <direct.h>
#include <complex>
using namespace std;

class CZXDib  
{
	//×Ô¶¨Òå£º
public:  

	CString GetAppDirectory();
	bool CutRegionFromRotatePictureC( BYTE *pBitmap, int dwHeight, int dwWidth, double dCenterY, double dCenterX, double dAngel, BYTE *&pResult, double x_from, double y_from, double x_to, double y_to, int &dwResultHegiht, int &dwResultWidth );
	void ImgGaussSmooth1(int* pBitmap, int dwHeight, int dwWidth, float seta);
	void GetInteImg(BYTE* pBitmap,int dwHeight,int dwWidth,int*& pIntImg);
	void RegularToGrayLevel(float *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE*& pResult);
	void CreateMasks(int nH, int nW, double dAngleStep, double dWidth, BYTE*& pMask, int &nCnt );
	void GetSNR(int* pBitmap,double* pIntImg,double* pIntImg2,DWORD dwHeight,DWORD dwWidth,long nSize,long nSize2,double*& pResult);
	void GetInteImg(int* pBitmap,int dwHeight,int dwWidth,double*& pIntImg);
	bool MedianFilterH(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, int nFilter, BYTE *&pResult);
	void ProjectVertical(int* pBitmap,DWORD dwHeight,DWORD dwWidth,double*& pResult);
	void ProjectVertical(int* pBitmap, DWORD dwHeight, DWORD dwWidth, int*& pResult);
	void SobelHorizon(unsigned char *pBitmap, int dwHeight, int dwWidth, int nFilterSize, int **pResult);
	bool Sobel1(BYTE * pImage, DWORD dwHeight, DWORD dwWidth, BYTE* &pResult);
	void MeanFilter(int* pPrj, int nW, int nWnd);
	void RepeatMean(int* pData, int nW, int nCount);
	void ErosionH(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth);
	void ErosionV(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth);
	void DialtionH(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth);
	void DialtionV(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth);
	void MorphV(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth);
	void Morph(int* pnX, int nW, int nSize);
	void Morph4(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth);
	void MorphV2(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth);
	void MorphH3(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth);
	void MorphH2(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth);
	void Morph3(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth);
	void AT(BYTE* pBmp, int nHeight, int nWidth);
	void GetHorizonMeanColor(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE*& pResult);
	void GetPolarImgColor(BYTE* pBmp, int nH, int nW, int nX, int nY, BYTE*& pRest, int &nResH, int &nResW);
	void FlatterImg(BYTE* pBitmap, int dwHeight, int dwWidth, int nSizeX, int nSizeY, BYTE*& pRest );
	void ImgGaussSmoothH(BYTE* pBitmap, int dwHeight, int dwWidth, float seta);
	void Morph2(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth);
	void ProjectHorizon(int* pBitmap,DWORD dwHeight,DWORD dwWidth,double*& pResult);
	void Fourier(BYTE* pData, int nLen, double*& pdR, double*& pdI, int& nResW);
	void GetPolarImgSetW(BYTE* pBmp, int nH, int nW, int nX, int nY, int nResW, int nR, BYTE*& pRest, int &nResH);
	void GetInteImg2(int* pBitmap,int dwHeight,int dwWidth,double*& pIntImg);
	void GetInteImg2(BYTE* pBitmap, int dwHeight, int dwWidth, int*& pIntImg);
	void GetLocalDeviation(BYTE* pBitmap,int dwHeight,int dwWidth,int nSzH,int nSzW, double*& pDev);
	void GetInteImg2(BYTE* pBitmap,int dwHeight,int dwWidth,double*& pIntImg);
	void GetInteImg(BYTE* pBitmap,int dwHeight,int dwWidth,double*& pIntImg);
	void SobelVertical(unsigned char*pBitmap, int dwHeight, int dwWidth, int nSizeH, int nSizeW, int** pResult);
	void SobelVertical(unsigned char*pBitmap, int dwHeight, int dwWidth, int nFilterSize, int** pResult);
	void GetPolarImg(BYTE* pBmp, int nH, int nW, int nX, int nY, BYTE*& pRest, int &nResH, int &nResW);
	void LabelImage(unsigned char* pBitmap,int dwHeight,int dwWidth,unsigned char byTarget,RECT*& pRect,int &nCount,int*& pResult, int*& pnArea);
	void Morph2V(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth);
	double GetMean(double* pdData, int nLengh);
	void GetCurvHis(int* pHis, int nLength, int nFilterSize, double*& pResult);
	void SobelHorizon(unsigned char *pBitmap, int dwHeight, int dwWidth, int **pResult);
	int  RegularToGrayLevel(int *pBitmap, DWORD dwHeight, DWORD dwWidth,BYTE*& pResult);
	void SobelVertical(unsigned char*pBitmap, int dwHeight, int dwWidth, int** pResult);
	void MorphH(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth);
	void PriwittH(unsigned char *pBitmap, int dwHeight, int dwWidth,int nSizeX, int nSizeY,unsigned char **pResult);
	void PCA(double* pdData, int nSampleCnt, int nDim, double*& pdU, double*& pdSig);
	void AdjustHSI(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,long nH,long nS,long nI,BYTE* pResult);
	void AdjustHSI(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,long nHScopeFrom,long nHScopeTo,long nH,long nS,long nI,BYTE* pResult);
	bool TranslateHistogramToVector(BYTE *pHg, DWORD dwLength,BYTE *&pResult);
	bool TranslateHistogramToVector(double *pHg, DWORD dwLength, BYTE *&pResult);
	bool TranslateHistogramToVector(int *pHg, DWORD dwLength,BYTE *&pResult);
	void ProjectVertical(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,double*& pResult);
	void MorphByMask(BYTE* pImage,int nHeight,int nWidth,BYTE* pMask,int nMskHeight, int nMskWidth);
	void ProjectHorizon(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,double* pResult, long &nPos);
	void LabelGrayImage(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE byGate,RECT*& pRect,long &nCount);
	void APlusD(BYTE* pBitmap1,DWORD dwHeight1,DWORD dwWidth1,BYTE* pBitmap2,DWORD dwHeight2,DWORD dwWidth2,BYTE*& pRes,DWORD &dwHeight, DWORD& dwWidth);
	void APlusB(BYTE* pBitmap1,DWORD dwHeight1,DWORD dwWidth1,BYTE* pBitmap2,DWORD dwHeight2,DWORD dwWidth2,BYTE*& pRes,DWORD &dwHeight, DWORD& dwWidth);
	void ProjectVertical(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE*& pResult);
	void ProjectHorizon(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE*& pResult);
	void GetSNR(BYTE* pBitmap,double* pIntImg,double* pIntImg2,DWORD dwHeight,DWORD dwWidth,long nSize,long nSize2,double*& pResult);
	double SegmentByKSigma(BYTE* pImage, int nHeight, int nWidth, double dK);
	bool MultitargetCode(BYTE *pBitmap,int dwHeight,int dwWidth,int areaSize,int num,POINT*& ptPos);
	int MultitargetCode(BYTE *pBitmap, int dwHeight, int dwWidth, int areaSize, int num, POINT*& pos, double *&pSNR);
	bool RegularToGrayLevel1(double *pBitmap, DWORD dwHeight, DWORD dwWidth,double& dMax,double& dMin,BYTE*& pResult);
	double GetHuai(double d);
	double GaussNoise(double dMean, double dSigma);
	BOOL Fourier(BYTE* lpDIBBits, DWORD dwHeight, DWORD dwWidth,double*& pdResult);
	bool TranslateHistogramToVector(BYTE *pHg, DWORD dwHeight,DWORD dwWidth,DWORD dwResHeight,BYTE *&pResult);
	void RotationImage(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,double dCenterY,double dCenterX,double dAngle,BYTE*& pResult);
	int LabelImage1(BYTE* pImage,int dwWidth,int dwHeight,unsigned int* pRect);
	void Polar(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE*& pResult);
	void RotationBlur(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,double dCenterY,double dCenterX,double dAngle,BYTE*& pResult);
	bool RegularToGrayLevel(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth,RECT& rect,BYTE*& pResult);
	bool RegularToGrayLevel(double *pBitmap, DWORD dwHeight, DWORD dwWidth,RECT& rect,BYTE*& pResult);
	bool FindLastFileName(CString &FileName,int iStep);
	void GetBand(BYTE* pColorImg,DWORD dwHeight,DWORD dwWidth,BYTE*& pBitmap,int nBand);
	void EreaseRound(double* pBitmap,DWORD dwHeight,DWORD dwWidth,int nLeft,int nTop,int nRight,int nBottom,double bValue);
	void MeanFilter(unsigned char *pBitmap, int dwHeight, int dwWidth,long nWndSizeX,long nWndSizeY);
	bool RegularToGrayLevelWidthMaxMin(double *pBitmap, DWORD dwHeight, DWORD dwWidth,double dMax,double dMin,BYTE*& pResult);
	void RegularToGrayLevel(double *pBitmap, DWORD dwHeight, DWORD dwWidth,double& dMax,double& dMin,BYTE*& pResult);
	int  RegularToGrayLevel(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth);
	bool GetHistgramImage(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE*& pHisImg,DWORD &dwHisImgHeight,DWORD &dwHisImgWidth);
	bool TranslateHistogramToVector(DWORD *pHg, BYTE *&pResult,bool bRegular = true );
	bool DrawLineInTheColorMatric2(int x_from, int y_from, int x_to, int y_to, BYTE *pBitmap,DWORD dwHeight,DWORD dwWidth,RGBQUAD color);
	double AddGaussNoise_Fast(double dMean,double dDeviation);
	bool DrawRectInColorImg(RECT rect,BYTE *pBitmap,DWORD dwHeight,DWORD dwWidth,RGBQUAD color);
	void UnwrapImage(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,long nOffsetX,long nOffsetY,BYTE*& pResult);
	BOOL IFourier(complex<double> *TInput, DWORD dwHeight, DWORD dwWidth,BYTE*& pResult,bool bRegular=false);
	void SPOMF(complex<double> *pImage,complex<double> *pTemplate,DWORD dwHeight,DWORD dwWidth,BYTE*& pResult);
	void AppandImage(BYTE* pBitmap,DWORD dwHeight, DWORD dwWidth,DWORD dwResultHeight,DWORD dwResultWidth,BYTE*& pReslt);
	BOOL Fourier(BYTE* lpDIBBits, DWORD dwHeight, DWORD dwWidth,complex<double> *&TResult,DWORD &dwResultHeight,DWORD& dwResultWidth);
	BOOL Fourier(BYTE* lpDIBBits, DWORD dwHeight, DWORD dwWidth);
	void FFT(complex<double> * TD, complex<double> * FD, int r);
	void IFFT(complex<double> * FD, complex<double> * TD, int r);
	void SegmentByCorrelation(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth);
	void Morph(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth);
	void GetHSIFromRGB(BYTE* pColorImg,DWORD dwHeight,DWORD dwWidth,BYTE*& pH,BYTE*& pS,BYTE *& pI );
	bool SegmentByOTSU_M(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth);
	void SobelVertical(unsigned char*pBitmap, int dwHeight, int dwWidth, unsigned char** pResult);
	void SobelHorizon(unsigned char *pBitmap, int dwHeight, int dwWidth, unsigned char **pResult);
	void LabelImage(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE byTarget,RECT*& pRect,long &nCount);
	double GetPFByGivenSNR_PD(long nBase,long nExp,double dSNR,double dPDTotal);
	double CNK(long nN,long nK);
	bool GetHistgram(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, long pHis[256]);
	void HistgromRegular(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,long pnHGRefer[256]);
	//void HistgromRegular(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,long pnHGRefer[256]);
	void SubImage(BYTE* pImage1,BYTE* pImage2,BYTE* pResult,long nImgHeight,long nImgWidth,long nFilterWidth);
	void AppendImage(BYTE* pTemp1,BYTE* pTemp2,DWORD dwHeight,DWORD dwWidth1,DWORD dwWidth2,BYTE*& pResult);
	bool GetRegularHis(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,WORD flag, BYTE*& pResult,DWORD &dwResultHeight,DWORD &dwResultWidth);
	void SubImage(BYTE* pImage1, BYTE* pImage2,DWORD dwHeight,DWORD dwWidth);
	bool GetHistgram(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, DWORD*& pHis);
	bool ScanRegion(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE bTargetColor,int*& pResult);
	bool SeedGrow(DWORD i,DWORD j,DWORD& Number,BYTE flag,int & egg,BYTE *pBitmap, DWORD& dwHeight, DWORD& dwWidth, int* pResult);		
	bool CutRegionFromRotatePictureD(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, double dCenterY, double dCenterX,double dAngel,BYTE *&pResult, double x_from, double y_from, double x_to, double y_to,DWORD &dwResultHegiht,DWORD &dwResultWidth);
	bool CutRegionFromRotatePicture (BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, double dCenterY, double dCenterX,double dAngel,BYTE *&pResult, DWORD x_from, DWORD y_from, DWORD x_to, DWORD y_to);
	bool CutRegionFromSidePicture(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, double dCenterY, double dCenterX,double dRatio,BYTE *&pResult, DWORD x_from, DWORD y_from, DWORD x_to, DWORD y_to);		
	void GrayErosionFilter(BYTE * pInput, BYTE*& pResult,long nHeight,long nWidth,long nWndSize);	
	void GrayDilationFilter(BYTE* pInput, BYTE*& pImgResult,long nHeight,long nWidth, long nWndSize);
	void GetRectHistgram(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,RECT rect,double dMeanR[256],double dMeanG[256],double dMeanB[256]);
	void HistgromBanlance(long pHG[256],BYTE pnMap[256]);
	void RGBRegularCell(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,long nBandCount,long nCellHeight,long nCellWidth,long x, long y,double dMean[9],double dDeviation[9],long nHCellCount,long nVCellCount,double dSeta,BYTE* &pResult);
	void GetRectMeanAndVariant(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,RECT rect,double& dMeanR,double& dMeanG,double& dMeanB,double& dDeviationR,double& dDeviationG,double& dDeviationB);
	void RGBRegular(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,double dMeanR,double dMeanG,double dMeanB,double dDeviationR,double dDeviationG,double dDeviationB,BYTE* &pReslt);
	void GetMeanAndVariant(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,double& dMeanR,double& dMeanG,double& dMeanB,double& dDeviationR,double& dDeviationG,double& dDeviationB);
	void LocalHSIRegular(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,double pHisH[361],double pHisB[255],double pHisS[100],BYTE* &pReslt);
	void LocalHistgromRegular(const double * pHGOrigin,const double* pHGDestin,DWORD dwLength,long*& pnMap);
	void LocalHistgromRegular(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,double pdHGRefer[360],long nWndSize,double dSeta);
	void LocalHistgromBanlance(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,long nWndSize,double dSeta);
	void ImgMeanSmooth(BYTE* pBitmap, int dwHeight, int dwWidth, long nWndWidth);
	void ImgGaussSmooth(BYTE* pBitmap, int dwHeight, int dwWidth, float seta);
	bool ChangeImageSize(BYTE *pBitmap, DWORD dwHeight,DWORD dwWidth,  BYTE *&pBitmapResult, DWORD dwHeightResult, DWORD dwWidthResult);
	void FussionPCA(DWORD nHeight,DWORD nWidth,BYTE *lpPanBand,BYTE *lpMultiBand,BYTE* pResult);
	bool GetHSBFromRGB(double &H,double &S,double &Y,BYTE R,BYTE G,BYTE B);
	bool GetRGBFromHSB(BYTE& R,BYTE& G,BYTE& B,double H,double S,double Y);
	bool GetHSVFromRGB(int r,int g,int b,double &hr,double &sr,double &v);
	bool GetRGBFromHSV(double h,double s,double v,BYTE &rr,BYTE &gr,BYTE &br);
	bool GetHSIFromRGB(double R,double G,double B,double &H,double &S,double &I);
	void FussionHSI(DWORD nHeight,DWORD nWidth,BYTE *lpPanBand,BYTE *lpMultiBand);
	int  GetHSIFromRGB(unsigned char *pColorBitmap, LONG dwHeight, LONG dwWidth, unsigned char *&pResult);
	void Fussion(DWORD nHeight,DWORD nWidth,BYTE *lpPanBand,BYTE *lpMultiBand);
    bool TranslateVectorToMetric(BYTE *pHg, DWORD dwLength,BYTE *&pResult);
    bool GetHSIFromRGB(BYTE R,BYTE G,BYTE B,double &H,double &S,double &I);
    bool GetRGBFromHSI(double H,double S,double I,BYTE &R,BYTE &G,BYTE &B);
	bool AddGaussNoise(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, double dMean, double dDeviation, BYTE *&pResult);
	bool Test(DWORD dwHeight, DWORD dwWidth, BYTE *pInput);
	bool LoadMultiImageFromGivenFileName(CString strFileName,int nFileNumber,DWORD &dwHeight,DWORD &dwWidth,BYTE** &pOutput);
	bool RegularToGrayLevel(DWORD *pBitmap, DWORD dwHeight, DWORD dwWidth,BYTE*& pResult);
	bool RegularToGrayLevel(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth,BYTE*& pResult);
	bool SelectBiggestBlock(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth,BYTE flag,DWORD dwMinBlockSize);
	bool GetRidOfSmallBlock(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth,BYTE flag,DWORD dwMinBlockSize);
	bool SeedGrow(DWORD i,DWORD j,BYTE *pBitmap, DWORD& dwHeight, DWORD& dwWidth, DWORD& Number,BYTE& flag,BYTE& egg,WORD& Delete,BYTE* colorarray);
    bool SeedGrow(DWORD i,DWORD j,BYTE *pBitmap, DWORD& dwHeight, DWORD& dwWidth, DWORD& Number,BYTE& flag,BYTE& egg);
	bool DrawLineInTheColorMatric(double x1, double y1, double x2, double y2, BYTE *pBitmap, int dwHeight, int dwWidth, RGBQUAD color);
    bool DrawLineInTheColorMatric1(int x_from, int y_from, int x_to, int y_to, BYTE *pBitmap,DWORD dwHeight,DWORD dwWidth,RGBQUAD color);
	bool DrawLineInTheMatric(double x1, double y1, double x2, double y2, BYTE *pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE color);
    bool DrawLineInTheMatric1(double x1, double y1, double x2, double y2, BYTE *pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE color);
    bool DrawLineInTheColorMatric1(int x_from, int y_from, int x_to, int y_to, BYTE *pBitmap,DWORD dwHeight,DWORD dwWidth,RGBQUAD color,double dLineWidth);
	bool Histogram(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE* &pResult);
	bool Histogram(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, DWORD *&pResult,WORD flag=8);
	bool Sobel(BYTE *& pImage, DWORD dwHeight, DWORD dwWidth);
	bool Sobel(BYTE * pImage, DWORD dwHeight, DWORD dwWidth,BYTE* &pResult);
	bool Sobel(BYTE * pImage, DWORD dwHeight, DWORD dwWidth,double* &pResult);
	bool GetBitmapInfo(BITMAPINFO *&bmi, CString &FileName);
	bool GetBMPFileHeader(BITMAPFILEHEADER &bfh,CString &FileName);
	bool TranslateVectorToBitmap(DWORD dwHeight,DWORD dwWidth,BYTE* pVector,BYTE*& pBitmapData,WORD flag);
	bool TranslateBitmapToVector(DWORD dwHeight,DWORD dwWidth,BYTE* pBitmapData,BYTE*& pVector,WORD flag);
	bool FindNextFileName(CString &FileName,int iStep);
	bool LoadVectorFromBMPFile(CString &FileName, BYTE *&ppBitmapData, DWORD &dwHeight, DWORD &dwWidth,WORD &flag);
	bool WriteBMPFileFromVector(CString &FileName, BYTE *pBitmapData,DWORD dwHeight,DWORD dwWidth,WORD flag);
	bool CreateBITMAPFILEHEADER(DWORD dwHeight, DWORD dwWidth, BITMAPFILEHEADER *&pBmi,WORD flag);
	bool CreateBITMAPINFO(BITMAPINFO *&pbi,DWORD dwHeight, DWORD dwWidth,WORD flag);
	void GetLocalSNR(BYTE* pBitmap, int dwHeight, int dwWidth, int nSzH, int nSzW, double*& pDev);


public:
	bool ShedSegment(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE*& pResult);
	bool BoxesImageAnalysis(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, DWORD dwTemplateHeight, DWORD dwTemplateWidth, int nBoxHeight, int nBoxWidth, BYTE *&pResult);
	bool BoxesImageAnalysis(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,DWORD dwTemplateHeight,DWORD dwTemplateWidth,int nBoxHeight,int nBoxWidth, double threshold,BYTE*& pResult);
	bool ProjectGrayImageXYHistogram(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE *&pHg_x,BYTE*& pHg_y);
	bool SumGrayLevelOfSubArea(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, DWORD dwTemplateHeight,DWORD dwTemplateWidth,BYTE *&pResult);
	bool MarkAreas(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE flag,BYTE *&pResult);
	bool GetRidOfOuterArea(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE flag_rid,BYTE flag_fill,BYTE*& pResult);
	bool ProjectXYHistogram(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE flag, DWORD *&pHg_X, DWORD *&pHg_Y);
	bool ProjectXYHistogram(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE flag, BYTE *&pHg_X, BYTE *&pHg_Y);
	bool SegmentByArea(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, DWORD dwCount, BYTE *&pResult);
	bool Laplacian(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE*& pResult);
	bool Roberts(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE*& pResult);
	bool MedianFilter(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE*& pResult);
	bool SUSANCornerDetect(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE*& pResult);
	bool LoadDoubleDataFromFile(CString strFileName,DWORD& dwHeight,DWORD& dwWidth,double*&pResult);
	bool WriteDoubleMatricToFile(CString strFileName, DWORD dwHeight, DWORD dwWidth,double* pMatric);
	bool NagelSimilarity(BYTE* pBitmap1,BYTE* pBitmap2,DWORD dwHeight,DWORD dwWidth,double*&pResult);
	bool CorrelationOfTwoVectors(BYTE *pBitmap1, BYTE *pBitmap2, DWORD dwHeight, DWORD dwWidth, float fFlag,float &fResult);
	bool StatisticParametersAboutMeanAndVariance(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, float fMean[3], float fVariance[3]);
	bool StatisticParametersAboutMeanAndVariance(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,float& fMean,float&fVariance);
	bool UnifyByMeanAndVariance(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, float pMeanOutput[3], float pVarianceOutput[3],float pMeanInput[3],float pVarianceInput[3]);
	bool UnifyByMeanAndVariance(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, float fMeanOutput, float fVarianceOutPut,float fMeanInput,float fVarianceInput);
	int  SegmentByDownHill(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth);
	bool SegmentByOTSU(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth);
	bool VarianceByRGB2thColor(BYTE *pColorBitmap, DWORD dwHeight, DWORD dwWidth,BYTE* pBKImage, BYTE *&pResultImage);
	bool GetHSIFromRGB(BYTE *pColorBitmap, DWORD dwHeight, DWORD dwWidth, BYTE *&pResult);
	bool CoarsePositionByColorAndVariance(BYTE **pBKImage, DWORD dwHeight, DWORD dwWidth, int nBKnumber,BYTE *pBitmap, WORD flag, RECT &rect);
	bool Translate8To24Color(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE *&pResult);
	bool Translate24To8Color(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE* &pResult);
	bool CutRegionFromPicture(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE*& pResult,DWORD x_from,DWORD y_from,DWORD x_to,DWORD y_to,WORD flag);
	bool CutRegionFromPicture(double *pBitmap, DWORD dwHeight, DWORD dwWidth, double *&pResult, DWORD x_from, DWORD y_from, DWORD x_to, DWORD y_to);
	bool CoarsePositionByColorSegment(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,RECT& rect);
	bool Uncover2ValueImageByRect(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth,DWORD dwRectHeight,DWORD dwRectWidth);
	bool ColorBasedSegment(BYTE* pColorBitmap,DWORD dwHeight,DWORD dwWidth,BYTE* &pResult);
	bool CoarsePositionByMeanAndVariance(BYTE **pBKmage, DWORD dwHeight, DWORD dwWidth, int nNumberBK, BYTE *pInputImage,RECT &rect,WORD flag);
	bool VarianceImage(BYTE **ppBitmap, DWORD dwHeight, DWORD dwWidth,int nFileNumber, BYTE *pMeanImage, BYTE *&pResultImage,WORD flag);
	bool VarianceImage(BYTE *ppBitmap, DWORD dwHeight, DWORD dwWidth,BYTE *pMeanImage, BYTE *&pResultImage,WORD flag);
	bool MeanImage(BYTE **pBitmap, DWORD dwHeight, DWORD dwWidth, int nNumber, BYTE *&pResult,WORD flag);
	double RegularToGrayLevel(double *pBitmap, DWORD dwHeight, DWORD dwWidth,BYTE*& pResult);
	bool ProjectAndFindTargetRegion(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,RECT &rect);
	bool Cover2ValueImageByRect(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth,DWORD dwRectHeight,DWORD dwRectWidth);
	bool BlackAndWhite(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,unsigned char flag);
	bool HistogramSmoothBy5Points(BYTE *pHg,int level=256);
	bool HistogramSmoothBy5Points(DWORD *pHg,int level);
	bool TranslateHistogramToVector(BYTE* pHg,BYTE* &pResult);
	bool TranslateHistogramToVector(BYTE *pHg, DWORD dwHeight,DWORD dwWidth,BYTE *&pResult);
	CPoint * Bezier4(CPoint *p, int num);
	bool ATB(double *pA,double* pB, int raws, double *pResult);
	bool SVD(double *pMultiImageMatric,double* pEigenSpaceLeft,double* pEigenSpaceRight,long nPerVectorSize,long nNumberOfVector);
	bool AB(double *pA, double *pB, int nArow, int nAcol, int nBcol,double *pResult);
		
	BOOL Match(BYTE*pBuf,long nHeight,long nWidth,BYTE* pDestBuf,long nDestHeight,long nDestWidth,double& x,double& y,double& maxr);
	void GetRidOfLighting(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,double dMeanObj,double dDeviationObj,long dwWndWidth);
	void MeanFilter5(unsigned char *pBitmap, int dwHeight, int dwWidth);

	CZXDib();
	virtual ~CZXDib();

};

#endif // !defined(AFX_ZXDIB_H__8BCD4139_996B_446F_BC26_4DBDC7FA84F1__INCLUDED_)





















