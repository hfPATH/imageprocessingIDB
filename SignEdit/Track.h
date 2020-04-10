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

	//�ڵ�ǰ������ͼ����ƥ�䣬�ҵ�Ŀ��ķ������
	//�����������ṹ���������ֵ
	//���ո�����Ȩֵ����ģ�壺
	void UpdateTemplate();
	//ͨ��˫���Բ�ֵ�ķ�ʽ�ı�ͼ��Ĵ�С
	bool ChangeImageSize(BYTE *pBmp, DWORD dwHeight, DWORD dwWidth, BYTE *&pBitmapResult);
	bool ChangeImageSize(BYTE* pBitmap,DWORD dwWidth,DWORD dwHeight,BYTE *&pBitmapResult,DWORD dwWidthResult,DWORD dwHeightResult);
	bool ChangeImageSize(double *pBitmap, DWORD dwWidth, DWORD dwHeight, double *&pBitmapResult, DWORD dwWidthResult, DWORD dwHeightResult);
	//��ظ���������
	//������Ѳ���
	int SearchForOptimalParameter(RECT &RectSearchField,int stepx,int stepy);
	//���ⲿ�ľ�����װ��ģ��ͼ�񣺿������ڲ�ģ��ָ��
	bool LoadTemplate(BYTE* pBitmapData);
	//���ļ���װ��ģ��
	//���������㲽���е� ����Ŀ������ͼ��
	bool ReconstructImageUseSVD(double* pImageTarget,DWORD dwSize,double* pVector,DWORD dwVectorNumber,double*& pImitate);
	bool ReconstructImageUseKL(double *pTarget, DWORD dwSize,double *pEigenSpace,int nNumberOfEigens,double *&pResult);
	//���������㲽���е� �Ӹ���ͼ���г�ȡƥ���������������ͼ����Ϊ��ǰ���ͼ��
	bool SampleTargetImageFromPictureAndCalculateDifferenceMatric(int level,DWORD dHeight_now,DWORD dWidth_now,double* a_next,DWORD ImageWidth,DWORD ImageHeight,double *&pImageLevel,double *&pDiffrenceMatric);
	bool SampleTargetWithAffineParameter(int level,double* a_set,BYTE *&pTargetOut);
	bool SampleTargetWithAffineParameter(int level,double* a_set,double *&pTargetOut);
	//��ȡƥ�����ĸ��ǵ������
	bool GetRect(CPoint*);
	bool GetRect(double * );
	bool GetRect( Point2d *  );
	//���㵱ǰlevel��������: levelΪ0��1 
	//���г�ʼ������
    bool TrackSet(double *pEigenSpaceSet,double *pEigenSapceSecondLevelSet,int nEigenSpaceVectorNumberSet,double *pBitmapTemplateSet,DWORD dwTargetHeightSet,DWORD dwTargetWidthSet,DWORD dwBmpHeightSet,DWORD dwBmpWidthSet,double dDeltSet,int nPyramidLevelSet,int nMaxIterateTimesSet,double dTemplateUpdatePowerSet,TRACK_TYPE TrackTypeSet,TRACK_TEPLATE_UPDATE_TYPE TemplateUpdateTypeSet,double a[6]);
	bool TrackSet( double *pEigenSpaceSet, double *pEigenSapceSecondLevelSet, int nEigenSpaceVectorNumberSet, BYTE *pBitmapTemplateSet, DWORD dwTargetHeightSet, DWORD dwTargetWidthSet, DWORD dwBmpHeightSet, DWORD dwBmpWidthSet, double dDeltSet, int nPyramidLevelSet, int nMaxIterateTimesSet, double dTemplateUpdatePowerSet, TRACK_TYPE TrackTypeSet, TRACK_TEPLATE_UPDATE_TYPE TemplateUpdateTypeSet, double a[6] );

	//���캯����
	CTrack();
	//����������
	virtual ~CTrack();

  	RECT m_RelationResultRect;
	double** m_ppResultImage;
	BYTE * m_pOutPutTargetImage;
	

	TRACK_TYPE                TrackType;     //��������
	TRACK_TEPLATE_UPDATE_TYPE TemplateUpdateType;//ģ����·�ʽ

	double *m_pBmp;             //����ͼ��:
	double *m_pBmp2;			 //����ͼ��Ľ��������:
	DWORD m_dwBmpWidth;					//����ͼ����
	DWORD m_dwBmpHeight;				 //����ͼ��߶�

	double* m_pTemplate;				//ģ��ͼ��ָ��
	double* m_pTemplate2;				//ģ��ͼ��Ľ��������:
	double m_dTemplateUpdatePower;      //ģ�����Ȩֵ

	double* m_pBitmapTarget;            //Ŀ��ͼ��ָ��
	DWORD m_dwTargetWidth;              //Ŀ��ͼ����
	DWORD m_dwTargetHeight;             //Ŀ��ͼ��߶�

	double* m_pEigenSpace;		        //�����ռ�ָ��
	double* m_pEigenSpace2;				//����������������ռ�ָ��
	int m_nNumberOfSelectedEigenVectors;//ѡ�õ������ռ�ά��
	int m_nNumberOfTotalEigenVectors;   //�ܹ��������ռ�ά��

	int m_nPyramidLevel;                //�㷨���õĽ���������
	int m_nMaxIterateTimes;             //�㷨���õ�����������
	double m_dDelt;                     //�㷨���õ�һ������������״�Ĳ���
	double m_affine[6];                 //Ŀ��ķ����������
	double m_dVarianceTarget;           //ͳ��Ŀ��ķ���,�����趨��������
	double m_dGradationOffset;          //���ڹ�һ����ɵĻҶ�ƫ����
	double m_dTempalteGradationOffset;  //���ڹ�һ����ɵĻҶ�ƫ����

	bool m_bSaveResultFile;              //������ͼ���ļ�
	bool m_bSaveTargetFile;              //����Ŀ��ͼ���ļ�
	
	RECT m_SearchWindowRect;             //������������:

	bool m_bSearchStatus;                //����״̬���ڴ�״̬�£���ֻ�ڽ������ĵڶ�����������

	//����任������
	double a_next[6],a[6],a_add[6],a_last[6];
	

	//������㷨ģ��
	double* m_pTemplateRelation;
	
};
#endif // !defined(AFX_TRACK_H__B95A6D58_2C3B_4D56_9731_8101DD2E335E__INCLUDED_)
