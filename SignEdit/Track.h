// Track.h: interface for the CTrack class.
//
//////////////////////////////////////////////////////////////////////
#include <afxwin.h>

#if !defined(AFX_TRACK_H__B95A6D58_2C3B_4D56_9731_8101DD2E335E__INCLUDED_)
#define AFX_TRACK_H__B95A6D58_2C3B_4D56_9731_8101DD2E335E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <opencv2\opencv.hpp>
#include "TrackerParameters.h"
using namespace cv;

class CTrack  
{
public:
	bool Track( BYTE* pBitmap );
	bool ResembleAreaAnalysis(BYTE* pTemplate,DWORD dwHeight,DWORD dwWidth, BYTE* pTarget,BYTE* &pResult);
	bool UpdateTemplate(BYTE* pTemplate_old,BYTE* pTemplate_new,DWORD dwHeight,DWORD dwWidth,double  factor);
	bool FindNextFileName(CString &FileName,int iStep);

	bool RelationTrack(double *pBitmapPicture, DWORD dwWidthPicture, DWORD dwHeightPicture, 
			           double *pBitmapTemplate,
					   RECT& rect);
	bool RelationTrack(BYTE *pBitmapPicture,  DWORD dwHeightPicture, DWORD dwWidthPicture, 
			           BYTE *pBitmapTemplate, DWORD dwTemplateHeight,DWORD dwTemplateWidth, 
					   RECT  rectSearck,
					   POINT *&point,
					   int   nPoint);
	POINT RelationTrack(BYTE *pBitmapPicture,  DWORD dwHeightPicture, DWORD dwWidthPicture,
		                BYTE *pBitmapTemplate, DWORD dwTemplateHeight,DWORD dwTemplateWidth, 
						RECT rectSearck);
	POINT RelationTrack(BYTE *pBitmapPicture,  DWORD dwHeightPicture, DWORD dwWidthPicture,  
		                BYTE *pBitmapTemplate, DWORD dwTemplateHeight,DWORD dwTemplateWidth,
						int gate,
						RECT rectSearck);
	POINT RelationTrack(BYTE *pBitmapPicture,  DWORD dwHeightPicture, DWORD dwWidthPicture,  
		                BYTE *pBitmapTemplate, DWORD dwTemplateHeight,DWORD dwTemplateWidth,
						BYTE* pMask,
						RECT rectSearck);
	POINT CorrelationMatchDeviationUnify(BYTE *pBitmap,DWORD dwHeight,DWORD dwWidth,
		                BYTE* pTemplate,DWORD dwTemplateHeight,DWORD dwTemplateWidth,
						RECT rectSearch);
	POINT CorrelationMatchDeviationUnify(BYTE *pBitmap,DWORD dwHeight,DWORD dwWidth,
		                BYTE* pTemplate,DWORD dwTemplateHeight,DWORD dwTemplateWidth,
						BYTE* pMask,
						RECT rectSearch);

	bool ZeroMean(double *pBitmap, DWORD dwSize,double& dMean);
	bool Test(DWORD dwHeight, DWORD dwWidth, double *pInput);
	DWORD CheckResult(double* pTarget,double* pTemplate,DWORD dwSize);
	bool ZeroMean(double* pBitmap,DWORD dwSize);
	bool PyramidSampleImage(DWORD dwHeight, DWORD dwWidth, double *pImageInput,double *&pImageOutput);
	int Search(int nScope,bool bFine);
	bool Track(double* pBmpSet);
	DWORD CalculateCurrentParameters(double *a, double delt_now,bool bTypeCourseOrFine);
	bool PyramidLevel(double* a,double delt,double* pInputImage,DWORD dwImageHeight,DWORD dwImageWidth,double*& pTemplate,double* pEigenSpace,DWORD dwTargetHeight,DWORD dwTargetWidth);
	bool PyramidSampleTargetImageFromPictureAndCalculateDifferenceMatric(DWORD dHeight_now,DWORD dWidth_now,double* a,double* pInputImage,DWORD ImageWidth,DWORD ImageHeight,double *pTargetImage,double *pDifferenceMatric);
	bool CalculateParameter(double *pTemplateImage, int nTargetHeight, int nTargetWidth, double *pTargetImage,double *pTargetDifference, double delt,double *a);
	bool GetTemplateBitmap(BYTE *&pTemplateOut);
	bool GetTargetBitmap  (BYTE *&pTargetOut);


	bool GetTargetImageByGivenAffineParameters(double*&pResult,double* a);

	//在当前的输入图像中匹配，找到目标的仿射参数
	//遍历金字塔结构：返回误差值
	//按照给定的权值更新模板：
	void UpdateTemplate();
	//通过双线性差值的方式改变图像的大小
	bool ChangeImageSize(BYTE *pBmp, DWORD dwHeight, DWORD dwWidth, BYTE *&pBitmapResult);
	bool ChangeImageSize(BYTE* pBitmap,DWORD dwWidth,DWORD dwHeight,BYTE *&pBitmapResult,DWORD dwWidthResult,DWORD dwHeightResult);
	bool ChangeImageSize(double *pBitmap, DWORD dwWidth, DWORD dwHeight, double *&pBitmapResult, DWORD dwWidthResult, DWORD dwHeightResult);
	//相关跟踪主函数
	//搜索最佳参数
	int SearchForOptimalParameter(RECT &RectSearchField,int stepx,int stepy);
	//从外部的矩阵中装入模板图像：拷贝给内部模板指针
	bool LoadTemplate(BYTE* pBitmapData);
	//从文件中装入模板
	//金字塔计算步骤中的 计算目标的拟合图像：
	bool ReconstructImageUseSVD(double* pImageTarget,DWORD dwSize,double* pVector,DWORD dwVectorNumber,double*& pImitate);
	bool ReconstructImageUseKL(double *pTarget, DWORD dwSize,double *pEigenSpace,int nNumberOfEigens,double *&pResult);
	//金字塔计算步骤中的 从跟踪图像中抽取匹配框所囊括的区域图像，作为当前结果图像
	bool SampleTargetImageFromPictureAndCalculateDifferenceMatric(int level,DWORD dHeight_now,DWORD dWidth_now,double* a_next,DWORD ImageWidth,DWORD ImageHeight,double *&pImageLevel,double *&pDiffrenceMatric);
	bool SampleTargetWithAffineParameter(int level,double* a_set,BYTE *&pTargetOut);
	bool SampleTargetWithAffineParameter(int level,double* a_set,double *&pTargetOut);
	//提取匹配框的四个角的坐标点
	bool GetRect(CPoint*);
	bool GetRect(double * );
	bool GetRect( Point2d *  );
	//计算当前level金字塔层: level为0或1 
	//进行初始化设置
    bool TrackSet(double *pEigenSpaceSet,double *pEigenSapceSecondLevelSet,int nEigenSpaceVectorNumberSet,double *pBitmapTemplateSet,DWORD dwTargetHeightSet,DWORD dwTargetWidthSet,DWORD dwBmpHeightSet,DWORD dwBmpWidthSet,double dDeltSet,int nPyramidLevelSet,int nMaxIterateTimesSet,double dTemplateUpdatePowerSet,TRACK_TYPE TrackTypeSet,TRACK_TEPLATE_UPDATE_TYPE TemplateUpdateTypeSet,double a[6]);
	bool TrackSet( double *pEigenSpaceSet, double *pEigenSapceSecondLevelSet, int nEigenSpaceVectorNumberSet, BYTE *pBitmapTemplateSet, DWORD dwTargetHeightSet, DWORD dwTargetWidthSet, DWORD dwBmpHeightSet, DWORD dwBmpWidthSet, double dDeltSet, int nPyramidLevelSet, int nMaxIterateTimesSet, double dTemplateUpdatePowerSet, TRACK_TYPE TrackTypeSet, TRACK_TEPLATE_UPDATE_TYPE TemplateUpdateTypeSet, double a[6] );

	//构造函数：
	CTrack();
	//析构函数：
	virtual ~CTrack();

  	RECT m_RelationResultRect;
	double** m_ppResultImage;
	BYTE * m_pOutPutTargetImage;
	

	TRACK_TYPE                TrackType;     //跟踪类型
	TRACK_TEPLATE_UPDATE_TYPE TemplateUpdateType;//模板更新方式

	double *m_pBmp;             //输入图像:
	double *m_pBmp2;			 //输入图像的金字塔抽层:
	DWORD m_dwBmpWidth;					//输入图像宽度
	DWORD m_dwBmpHeight;				 //输入图像高度

	double* m_pTemplate;				//模板图像指针
	double* m_pTemplate2;				//模板图像的金字塔抽层:
	double m_dTemplateUpdatePower;      //模板更新权值

	double* m_pBitmapTarget;            //目标图像指针
	DWORD m_dwTargetWidth;              //目标图像宽度
	DWORD m_dwTargetHeight;             //目标图像高度

	double* m_pEigenSpace;		        //特征空间指针
	double* m_pEigenSpace2;				//金字塔抽层后的特征空间指针
	int m_nNumberOfSelectedEigenVectors;//选用的特征空间维数
	int m_nNumberOfTotalEigenVectors;   //总共的特征空间维数

	int m_nPyramidLevel;                //算法采用的金字塔层数
	int m_nMaxIterateTimes;             //算法采用的最大迭代次数
	double m_dDelt;                     //算法采用的一个控制曲线形状的参数
	double m_affine[6];                 //目标的仿射参数数组
	double m_dVarianceTarget;           //统计目标的方差,用于设定更新门限
	double m_dGradationOffset;          //由于归一化造成的灰度偏移量
	double m_dTempalteGradationOffset;  //由于归一化造成的灰度偏移量

	bool m_bSaveResultFile;              //保存结果图像文件
	bool m_bSaveTargetFile;              //保存目标图像文件
	
	RECT m_SearchWindowRect;             //设置搜索区域:

	bool m_bSearchStatus;                //搜索状态：在此状态下，将只在金字塔的第二层上搜索；

	//仿射变换参数：
	double a_next[6],a[6],a_add[6],a_last[6];
	

	//互相关算法模板
	double* m_pTemplateRelation;
	
};
#endif // !defined(AFX_TRACK_H__B95A6D58_2C3B_4D56_9731_8101DD2E335E__INCLUDED_)
