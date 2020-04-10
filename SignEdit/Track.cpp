// Track.cpp: implementation of the CTrack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Track.h"
#include <math.h>
#include <mmsystem.h>
#include "ZXDib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrack::CTrack()
{
	m_pBmp= NULL;             //输入图像:
	m_pBmp2 = NULL;//输入图像的降低分辨率图象
	m_pTemplate = NULL;          //模板图像指针
	m_pTemplate2 = NULL;
	m_pBitmapTarget= NULL;            //目标图像指针
	m_pEigenSpace = NULL;		      //特征空间指针
	m_pEigenSpace2= NULL;  //金字塔抽层后的特征空间指针
	

	m_dDelt=0.0;
	m_nNumberOfTotalEigenVectors =0;
	m_nNumberOfSelectedEigenVectors=0;
	m_dwTargetWidth =0;
	m_dwTargetHeight =0;
	m_dwBmpWidth = 0;
	m_dwBmpHeight = 0;
	m_pEigenSpace2 = NULL;
	m_pEigenSpace = NULL;
	m_pBmp = NULL;
	m_pTemplate = NULL;
	m_pTemplateRelation = NULL;
	m_ppResultImage = NULL;
	m_ppResultImage=NULL;
	m_pOutPutTargetImage=NULL;
	m_dGradationOffset = 0;
}

CTrack::~CTrack()
{
	//释放内存：
	if(!m_pEigenSpace)				 delete m_pEigenSpace;
	if(!m_pEigenSpace2)				 delete m_pEigenSpace2;
	if(!m_pBmp)						 delete m_pBmp;           //输入图像:
    if(!m_pBmp2)					 delete m_pBmp2 ;
	if(!m_pTemplate)				 delete m_pTemplate;          //模板图像指针
	if(!m_pTemplate2)
									 delete m_pTemplate2;
	if(!m_pBitmapTarget)			 delete m_pBitmapTarget;            //目标图像指针
	if(!m_pTemplateRelation) 		 delete m_pTemplateRelation;
	if(!m_ppResultImage)			 delete m_ppResultImage;
	if(!m_pOutPutTargetImage)		 delete m_pOutPutTargetImage;
}
/////////////////////////////////////////////////////
//
//  设置和文件相关的信息
//  所有跟踪的初始化信息，全部在此函数内完成
//
/////////////////////////////////////////////////////
bool CTrack::TrackSet( double *pEigenSpaceSet, double *pEigenSapceSecondLevelSet,
	int nEigenSpaceVectorNumberSet, BYTE *pTemplateSet,
	DWORD dwTargetHeightSet, DWORD dwTargetWidthSet,
	DWORD dwBmpHeightSet, DWORD dwBmpWidthSet,
	double dDeltSet,
	int nPyramidLevelSet,
	int nMaxIterateTimesSet,
	double dTemplateUpdatePowerSet,
	TRACK_TYPE TrackTypeSet,
	TRACK_TEPLATE_UPDATE_TYPE TemplateUpdateTypeSet,
	double a_Set[6] )
{
	double* pTemp = new double[dwTargetHeightSet*dwTargetWidthSet];
	for ( int i = 0; i < dwTargetHeightSet*dwTargetWidthSet; i++ )
		pTemp[i] = pTemplateSet[i];

	bool bRet = TrackSet( pEigenSpaceSet, pEigenSapceSecondLevelSet,
		nEigenSpaceVectorNumberSet, pTemp,
		dwTargetHeightSet, dwTargetWidthSet,
		dwBmpHeightSet, dwBmpWidthSet,
		dDeltSet,
		nPyramidLevelSet,
		nMaxIterateTimesSet,
		dTemplateUpdatePowerSet,
		TrackTypeSet,
		TemplateUpdateTypeSet,
		a_Set );

	return bRet;
}
bool CTrack::TrackSet(double *pEigenSpaceSet,double *pEigenSapceSecondLevelSet,
					  int nEigenSpaceVectorNumberSet,double *pBitmapTemplateSet,
					  DWORD dwTargetHeightSet,DWORD dwTargetWidthSet,
					  DWORD dwBmpHeightSet,DWORD dwBmpWidthSet,
					  double dDeltSet,
					  int nPyramidLevelSet,
					  int nMaxIterateTimesSet,
					  double dTemplateUpdatePowerSet,
					  TRACK_TYPE TrackTypeSet,
					  TRACK_TEPLATE_UPDATE_TYPE TemplateUpdateTypeSet,
					  double a_Set[6])
{
	//装入特征空间：
	if(m_pEigenSpace) delete m_pEigenSpace;
	m_pEigenSpace = pEigenSpaceSet;
	if(m_pEigenSpace2) delete m_pEigenSpace2;
	m_pEigenSpace2 = pEigenSapceSecondLevelSet;
	//设置采用的特征向量个数：
	m_nNumberOfSelectedEigenVectors = nEigenSpaceVectorNumberSet;

	//设置目标大小
	m_dwTargetWidth = dwTargetWidthSet;
	m_dwTargetHeight = dwTargetHeightSet;

	//装入模板：:并0均值化处理
	if(TrackTypeSet==TT_AFFINE)
	{
		m_pTemplate = pBitmapTemplateSet;
		ZeroMean(m_pTemplate,m_dwTargetWidth*m_dwTargetHeight,m_dTempalteGradationOffset);
		PyramidSampleImage(m_dwTargetHeight,m_dwTargetWidth,m_pTemplate,m_pTemplate2);
	}else 
	{
		m_pTemplate = NULL;
		m_pTemplate2 = NULL;
		m_dTempalteGradationOffset = 0;
	}

	//设置输入图像大小：
	m_dwBmpWidth = dwBmpWidthSet;
	m_dwBmpHeight = dwBmpHeightSet;

	//参数
	m_dDelt = dDeltSet;
	m_nPyramidLevel = nPyramidLevelSet;
	m_nMaxIterateTimes = nMaxIterateTimesSet;
	m_dTemplateUpdatePower = dTemplateUpdatePowerSet;

	//跟踪方式和模板更新方式：
	TrackType = TrackTypeSet;
	TemplateUpdateType = TemplateUpdateTypeSet;
	

    ////======以下部分为初始化部分：===============
	for(int i=0;i<6;i++)
	{
		a[i]=a_last[i]=a_Set[i];
		a_add[i]= 0;
	}

	return true;
}
////////////////////////////////////////////////////////////
//
//   得到当前参数所对应的边框：
//
////////////////////////////////////////////////////////////
bool CTrack::GetRect(CPoint * pPoint)
{
    if(!pPoint) 	return FALSE;
	double x,y,x_center,y_center;
	
	x_center = (double)(m_dwTargetWidth-1)/2;
	y_center = (double)(m_dwTargetHeight-1)/2;

	a[1] += 1;  a[5] += 1;
    /////////////// 0 //////////
	x = a[0] - x_center*a[1] + y_center*a[2] ;
	y = a[3] - x_center*a[4] + y_center*a[5] ;
	pPoint[0].y = (int) (y+0.5);
	pPoint[0].x = (int) (x+0.5);

	//////////// 1 //////////////
	x =  a[0] + x_center*a[1] + y_center*a[2] ;
	y =  a[3] + x_center*a[4] + y_center*a[5] ;
	pPoint[1].y = (int) (y+0.5);
	pPoint[1].x = (int) (x+0.5);

	///////////// 2 ///////////////
	x =  a[0] + x_center*a[1] - y_center*a[2] ;
	y =  a[3] + x_center*a[4] - y_center*a[5] ;
	pPoint[2].y = (int) (y+0.5);
	pPoint[2].x = (int) (x+0.5);

	////////// 3 ////////////////
	x =  a[0] - x_center*a[1] - y_center*a[2] ;
	y =  a[3] - x_center*a[4] - y_center*a[5] ;
	pPoint[3].y = (int) (y+0.5);
	pPoint[3].x = (int) (x+0.5);


	a[1] -= 1; a[5] -= 1;
	return TRUE;
}
bool CTrack::GetRect( Point2d * pPoint )
{
	if ( !pPoint ) 	return FALSE;
	double x, y, x_center, y_center;


	x_center = (double) ( m_dwTargetWidth - 1 ) / 2,0;
	y_center = (double) ( m_dwTargetHeight - 1 ) / 2,0;

	a[1] += 1;  a[5] += 1;
	/////////////// 0 //////////
	x = a[0] - x_center*a[1] + y_center*a[2];
	y = a[3] - x_center*a[4] + y_center*a[5];
	pPoint[0].y = y ;
	pPoint[0].x = x ;

	//////////// 1 //////////////
	x = a[0] + x_center*a[1] + y_center*a[2];
	y = a[3] + x_center*a[4] + y_center*a[5];
	pPoint[1].y = y;
	pPoint[1].x = x;

	///////////// 2 ///////////////
	x = a[0] + x_center*a[1] - y_center*a[2];
	y = a[3] + x_center*a[4] - y_center*a[5];
	pPoint[2].y = y;
	pPoint[2].x = x;

	////////// 3 ////////////////
	x = a[0] - x_center*a[1] - y_center*a[2];
	y = a[3] - x_center*a[4] - y_center*a[5];
	pPoint[3].y = y;
	pPoint[3].x = x;


	a[1] -= 1; a[5] -= 1;
	return TRUE;
}
////////////////////////////////////////////////////////////
//
//   得到当前参数所对应的边框：
//
////////////////////////////////////////////////////////////
bool CTrack::GetRect(double * pPoint)
{
    if(!pPoint) 	return FALSE;
	double x,y,x_center,y_center;

	x_center = (double)(m_dwTargetWidth-1)/2;
	y_center = (double)(m_dwTargetHeight-1)/2;

	a[1] += 1;  a[5] += 1;
    /////////////// 0 //////////
	x = a[0] - x_center*a[1] + y_center*a[2] ;
	y = a[3] - x_center*a[4] + y_center*a[5] ;
	pPoint[0] = x;
	pPoint[1] = y;
	

	//////////// 1 //////////////
	x =  a[0] + x_center*a[1] + y_center*a[2] ;
	y =  a[3] + x_center*a[4] + y_center*a[5] ;
	pPoint[2] = x;
	pPoint[3] = y;

	///////////// 2 ///////////////
	x =  a[0] + x_center*a[1] - y_center*a[2] ;
	y =  a[3] + x_center*a[4] - y_center*a[5] ;
	pPoint[4] = x;
	pPoint[5] = y;

	////////// 3 ////////////////
	x =  a[0] - x_center*a[1] - y_center*a[2] ;
	y =  a[3] - x_center*a[4] - y_center*a[5] ;
	pPoint[6] = x;
	pPoint[7] = y;


	a[1] -= 1; a[5] -= 1;
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
//
//  计算当前金字塔层的I,到输入图像中抠取目标区域
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CTrack::SampleTargetWithAffineParameter(int level,double* a_set,double *&pTargetOut)
{
	double x,y,temp,x1,x2,x3,y1,y2,y3;
	int    nFactor  = 1<<level;
	m_dwTargetWidth /= nFactor;
	m_dwTargetHeight/= nFactor;
	double x_center = m_dwTargetWidth/2;
	double y_center = m_dwTargetHeight/2;
	int    dMagnifyX,dMagnifyY;
	dMagnifyY = nFactor;
    dMagnifyX = nFactor;
    DWORD position,i,j,position_l=0,dwObjectSize_now=m_dwTargetHeight*m_dwTargetWidth,dwImageWidth_now = m_dwBmpWidth*nFactor;
	double x_addition;
	double y_addition;
	
	if(pTargetOut) delete pTargetOut;
	pTargetOut = new double [m_dwTargetWidth*m_dwTargetHeight];
	
    //================有效性检测========================	
	if(a_set[1]*a_set[1]+a_set[4]*a_set[4] <0.001 || a_set[2]*a_set[2]+a_set[5]*a_set[5] <0.001)
	{
	//	AfxMessageBox("在采集当前选择框所覆盖的目标区时，区域瞬间变得太小！");
	//	dwTargetWindowWidth *= nFactor;		dwTargetWindowHeight*= nFactor;
	//	return FALSE;
	}
	if(a_set[1]*a_set[1]+a_set[4]*a_set[4] >9 || a_set[2]*a_set[2]+a_set[5]*a_set[5] >9)
	{
	//	AfxMessageBox("在采集当前选择框所覆盖的目标区时，区域瞬间变得太大！");
	//	dwTargetWindowWidth *= nFactor;		dwTargetWindowHeight*= nFactor;
	//	return FALSE;
	}
	//检测匹配框是否出界:(检测四个顶点是否出界)
	x  = (a_set[0]-x_center*a_set[1]-y_center*a_set[2] -x_center)*nFactor;// + m_CenterX;
	x1 = (a_set[0]+x_center*a_set[1]-y_center*a_set[2] +x_center)*nFactor;// + m_CenterX;
	x2 = (a_set[0]-x_center*a_set[1]+y_center*a_set[2] -x_center)*nFactor;// + m_CenterX;
	x3 = (a_set[0]+x_center*a_set[1]+y_center*a_set[2] +x_center)*nFactor;// + m_CenterX;
	y  = (a_set[3]-x_center*a_set[4]-y_center*a_set[5] -y_center)*nFactor;// + m_CenterY;
	y1 = (a_set[3]+x_center*a_set[4]-y_center*a_set[5] -y_center)*nFactor;// + m_CenterY;
	y2 = (a_set[3]-x_center*a_set[4]+y_center*a_set[5] +y_center)*nFactor;// + m_CenterY;
	y3 = (a_set[3]+x_center*a_set[4]+y_center*a_set[5] +y_center)*nFactor;// + m_CenterY;
	if(x<2||x1<2||x2<2||x3<2|| x>m_dwBmpWidth-2||x1>m_dwBmpWidth-2||x2>m_dwBmpWidth-2||x3>m_dwBmpWidth-2||  y<2||y1<2||y2<2||y3<2|| y1>m_dwBmpHeight-2||y2>m_dwBmpHeight-2||y3>m_dwBmpHeight-2||y>m_dwBmpHeight-2 ) 
	{
	//	AfxMessageBox("在采集当前选择框所覆盖的目标区时，发生越界！");
		m_dwTargetWidth *= nFactor;		m_dwTargetHeight*= nFactor;
		return FALSE;
	}

	double x_Left;
	double y_Left;
	//仿射变换,上中心偏移量
	x_Left = (a_set[0] -x_center*a_set[1] -y_center*a_set[2] -x_center)*nFactor;//  + m_CenterX ;
	y_Left = (a_set[3] -x_center*a_set[4] -y_center*a_set[5] -y_center)*nFactor;//  + m_CenterY ;

	x_addition = (a_set[1]+1)*nFactor;
	y_addition = (a_set[4])*nFactor;

	//采集目标图象
	for(j=0; j<m_dwTargetHeight; j++)
	{
		x = x_Left;
		y = y_Left;
		x_Left += (a_set[2])*nFactor; 
		y_Left += (a_set[5]+1)*nFactor;
		for(i=0; i<m_dwTargetWidth; i++)
		{
			//计算当前点在输入图像中的位置
			position = (int)(y) * m_dwBmpWidth + (int)(x);

            /////采集该点处的灰度值
			//抽样：在2*2临域内采用内插法：
			temp  = m_pBmp[position] - (m_pBmp[position] - m_pBmp[position+1])* (x-(int)x);
			position += m_dwBmpWidth;
			temp -= (temp-(m_pBmp[position] - (m_pBmp[position] - m_pBmp[position+1])* (x-(int)x)))*(y-(int)y);
			//插值结果赋给相应的标准化目标点:
			pTargetOut[position_l] = temp;

			position_l ++;//计算下一个点
			x += x_addition;
			y += y_addition;
		}
	}

	m_dwTargetWidth *= nFactor;
	m_dwTargetHeight*= nFactor;

    return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
//
//  计算当前金字塔层的I,到输入图像中抠取目标区域
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CTrack::SampleTargetWithAffineParameter(int level,double* a_set,BYTE *&pTargetOut)
{
	double x,y,temp,x1,x2,x3,y1,y2,y3;
	int    nFactor  = 1<<level;
	m_dwTargetWidth /= nFactor;
	m_dwTargetHeight/= nFactor;
	double x_center = m_dwTargetWidth/2;//((double)dwTargetWindowWidth)/(2*nFactor);
	double y_center = m_dwTargetHeight/2;//((double)dwTargetWindowWidth)/(2*nFactor);
	int    dMagnifyX,dMagnifyY;
	dMagnifyY = nFactor;
    dMagnifyX = nFactor;
    DWORD position,i,j,position_l=0,dwObjectSize_now=m_dwTargetHeight*m_dwTargetWidth,dwImageWidth_now = m_dwBmpWidth*nFactor;
	double x_addition;
	double y_addition;
	
	if(pTargetOut) delete pTargetOut;
	pTargetOut = new BYTE [m_dwTargetWidth*m_dwTargetHeight];
	
    //================有效性检测========================	
	if(a_set[1]*a_set[1]+a_set[4]*a_set[4] <0.001 || a_set[2]*a_set[2]+a_set[5]*a_set[5] <0.001)
	{
	//	AfxMessageBox("在采集当前选择框所覆盖的目标区时，区域瞬间变得太小！");
	//	dwTargetWindowWidth *= nFactor;		dwTargetWindowHeight*= nFactor;
	//	return FALSE;
	}
	if(a_set[1]*a_set[1]+a_set[4]*a_set[4] >9 || a_set[2]*a_set[2]+a_set[5]*a_set[5] >9)
	{
	//	AfxMessageBox("在采集当前选择框所覆盖的目标区时，区域瞬间变得太大！");
	//	dwTargetWindowWidth *= nFactor;		dwTargetWindowHeight*= nFactor;
	//	return FALSE;
	}
	//检测匹配框是否出界:(检测四个顶点是否出界)
	x  = (a_set[0]-x_center*a_set[1]-y_center*a_set[2] -x_center)*nFactor;// + m_CenterX;
	x1 = (a_set[0]+x_center*a_set[1]-y_center*a_set[2] +x_center)*nFactor;// + m_CenterX;
	x2 = (a_set[0]-x_center*a_set[1]+y_center*a_set[2] -x_center)*nFactor;// + m_CenterX;
	x3 = (a_set[0]+x_center*a_set[1]+y_center*a_set[2] +x_center)*nFactor;// + m_CenterX;
	y  = (a_set[3]-x_center*a_set[4]-y_center*a_set[5] -y_center)*nFactor;// + m_CenterY;
	y1 = (a_set[3]+x_center*a_set[4]-y_center*a_set[5] -y_center)*nFactor;// + m_CenterY;
	y2 = (a_set[3]-x_center*a_set[4]+y_center*a_set[5] +y_center)*nFactor;// + m_CenterY;
	y3 = (a_set[3]+x_center*a_set[4]+y_center*a_set[5] +y_center)*nFactor;// + m_CenterY;
	if(x<2||x1<2||x2<2||x3<2|| x>m_dwBmpWidth-2||x1>m_dwBmpWidth-2||x2>m_dwBmpWidth-2||x3>m_dwBmpWidth-2||  y<2||y1<2||y2<2||y3<2|| y1>m_dwBmpHeight-2||y2>m_dwBmpHeight-2||y3>m_dwBmpHeight-2||y>m_dwBmpHeight-2 ) 
	{
	//	AfxMessageBox("在采集当前选择框所覆盖的目标区时，发生越界！");
		m_dwTargetWidth *= nFactor;		m_dwTargetHeight*= nFactor;
		return FALSE;
	}

	double x_Left;
	double y_Left;
	//仿射变换,上中心偏移量
	x_Left = (a_set[0] -x_center*a_set[1] -y_center*a_set[2] -x_center)*nFactor;//  + m_CenterX ;
	y_Left = (a_set[3] -x_center*a_set[4] -y_center*a_set[5] -y_center)*nFactor;//  + m_CenterY ;

	x_addition = (a_set[1]+1)*nFactor;
	y_addition = (a_set[4])*nFactor;

	//采集目标图象
	for(j=0; j<m_dwTargetHeight; j++)
	{
		x = x_Left;
		y = y_Left;
		x_Left += (a_set[2])*nFactor; 
		y_Left += (a_set[5]+1)*nFactor;
		for(i=0; i<m_dwTargetWidth; i++)
		{
			//计算当前点在输入图像中的位置
			position = (int)(y) * m_dwBmpWidth + (int)(x);

            /////采集该点处的灰度值
			//抽样：在2*2临域内采用内插法：
			temp  = m_pBmp[position] - (m_pBmp[position] - m_pBmp[position+1])* (x-(int)x);
			position += m_dwBmpWidth;
			temp -= (temp-(m_pBmp[position] - (m_pBmp[position] - m_pBmp[position+1])* (x-(int)x)))*(y-(int)y);
			//插值结果赋给相应的标准化目标点:
			pTargetOut[position_l] = (int)temp;

			position_l ++;//计算下一个点
			x += x_addition;
			y += y_addition;
		}
	}

	m_dwTargetWidth *= nFactor;
	m_dwTargetHeight*= nFactor;

    return TRUE;
}
//////////////////////////////////////////////////////////////////////////////
//
//  计算拟合图像值：
//
//////////////////////////////////////////////////////////////////////////////
bool CTrack::ReconstructImageUseSVD(double* pImageTarget,DWORD dwSize,double* pVector,DWORD dwVectorNumber,double*& pImitate)
{
	if(pImitate) delete pImitate;
	pImitate = new double[dwSize];

	double *C=NULL;
	if(!(C=(double*)new double [dwVectorNumber]))
	{
		AfxMessageBox("分配内存失败！C \n PyramidCalculateImitateTargetImage()");
		return FALSE;
	}
    double temp;		
	DWORD position = 0;
    //计算拟合系数C值
	int i, j;
	for( j=0; j<dwVectorNumber; j++)
	{
		 C[j] = 0;
		 for(DWORD i=0; i<dwSize; i++)
			 C[j] += pVector[i+position] * pImageTarget[i];
		 position += dwSize;
	}

	//计算拟合图像
	for(i=0; i<dwSize; i++)
	{
		  position = 0;
		  temp = 0;
		  for(j=0; j<dwVectorNumber; j++){
			  temp += pVector[i+position] * C[j];
			  position += dwSize;
		  }
		  pImitate[i] = temp>255? 255:temp;
	  }
	if(!C) delete C;

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
//  从内存中装入模板图像：(目标大小已经在TrackSet中设置)
////////////////////////////////////////////////////////////////////////////////////////////////
bool CTrack::LoadTemplate(BYTE *pBitmapData)
{
	DWORD dwSize = m_dwTargetWidth  * m_dwTargetWidth;
	if(m_pTemplate)
		delete m_pTemplate;
	
	m_pTemplate = (double*) new double [dwSize];
	if(!m_pTemplate)
	{
		AfxMessageBox("CTrack::LoadTemplate 分配内存出错!");
		return FALSE;
	}
	double dMean = 0;
	for( int i=0; i< dwSize; i++)
	{
		m_pTemplate[i] = (int)pBitmapData[i];
		dMean += m_pTemplate[i];
	}
	dMean /= dwSize;
	for(int i=0; i< dwSize; i++)
	{
		m_pTemplate[i] -= dMean;
	}

	PyramidSampleImage(m_dwTargetHeight,m_dwTargetWidth,m_pTemplate,m_pTemplate2);


//	SmoothImage(dwTargetWindowWidth,dwTargetWindowWidth,pBitmapTemplate);
	//使用梯度图像
//	if(!Sobel(pBitmapTemplate,dwTargetWindowWidth,dwTargetWindowWidth))
//		return false;

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////
//
//  改变图像的大小：
////////////////////////////////////////////////////////////////////////////////////////////
bool CTrack::ChangeImageSize(BYTE *pBitmap, DWORD dwWidth, DWORD dwHeight, BYTE *&pBitmapResult, DWORD dwWidthResult, DWORD dwHeightResult)
{
	if(!(dwWidth && dwHeight && dwWidthResult && dwHeightResult))
	{
		AfxMessageBox("CTrack::ChangeImageSize  \n 图像大小不能为0!");
		return FALSE;
	}
	if(pBitmapResult) delete pBitmapResult;
	pBitmapResult = new BYTE[dwWidthResult*dwHeightResult];
    double ratio_x,ratio_y;
	ratio_x = (double) dwWidth;
	ratio_x /= (double) dwWidthResult;
	ratio_y = (double) dwHeight;
	ratio_y /= (double) dwHeightResult;
	DWORD i,j,position,position_l=0;
	double x,y,temp,temp1;
	
	for(j=1; j<dwHeightResult-1; j++)
	for(i=1; i<dwWidthResult-1; i++)
	{
		x = i;
		y = j;
		x *= ratio_x;
		y *= ratio_y;
		position = ((int)y) * dwWidth + (int)(x);
		temp1 = pBitmap[position] - pBitmap[position+1];
		temp  = (double)pBitmap[position] - temp1* (x-(int)x);
		position += dwWidth;
		temp1 = pBitmap[position] - pBitmap[position+1];
		temp -= (temp-((double)pBitmap[position] - temp1* (x-(int)x)))*(y-(int)y);
        
		pBitmapResult[j*dwWidthResult+i] = (BYTE) temp;
	}

	//最后一行用边界外拓法：
	DWORD dwSize = dwHeightResult*dwWidthResult;
	for(i=0;i<dwSize;i+=dwWidthResult)
	{
		pBitmapResult[i]=pBitmapResult[i+1];
	    pBitmapResult[i+dwWidthResult-1] = pBitmapResult[i+dwWidthResult-2];
	}
	for(j=0;j<dwWidthResult;j++)
	{
		pBitmapResult[j]=pBitmapResult[j+dwWidthResult];
		pBitmapResult[dwSize-j-1] = pBitmapResult[dwSize-j-1-dwWidthResult];
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////
//
//  改变图像的大小：
////////////////////////////////////////////////////////////////////////////////////////////
bool CTrack::ChangeImageSize(double *pBitmap, DWORD dwWidth, DWORD dwHeight, double *&pBitmapResult, DWORD dwWidthResult, DWORD dwHeightResult)
{
	if(!(dwWidth && dwHeight && dwWidthResult && dwHeightResult))
	{
		AfxMessageBox("CTrack::ChangeImageSize  \n 图像大小不能为0!");
		return FALSE;
	}
	if(pBitmapResult) delete pBitmapResult;
	pBitmapResult = new double[dwWidthResult*dwHeightResult];
    double ratio_x,ratio_y;
	ratio_x = (double) dwWidth;
	ratio_x /= (double) dwWidthResult;
	ratio_y = (double) dwHeight;
	ratio_y /= (double) dwHeightResult;
	DWORD i,j,position,position_l=0;
	double x,y,temp,temp1;
	
	for(j=1; j<dwHeightResult-1; j++)
	for(i=1; i<dwWidthResult-1; i++)
	{
		x = i;
		y = j;
		x *= ratio_x;
		y *= ratio_y;
		position = ((int)y) * dwWidth + (int)(x);
		temp1 = pBitmap[position] - pBitmap[position+1];
		temp  = (double)pBitmap[position] - temp1* (x-(int)x);
		position += dwWidth;
		temp1 = pBitmap[position] - pBitmap[position+1];
		temp -= (temp-((double)pBitmap[position] - temp1* (x-(int)x)))*(y-(int)y);
        
		pBitmapResult[j*dwWidthResult+i] = temp;
	}
	//最后一行用边界外拓法：
	DWORD dwSize = dwHeightResult*dwWidthResult;
	for(i=0;i<dwSize;i+=dwWidthResult)
	{
		pBitmapResult[i]=pBitmapResult[i+1];
	    pBitmapResult[i+dwWidthResult-1] = pBitmapResult[i+dwWidthResult-2];
	}
	for(j=0;j<dwWidthResult;j++)
	{
		pBitmapResult[j]=pBitmapResult[j+dwWidthResult];
		pBitmapResult[dwSize-j-1] = pBitmapResult[dwSize-j-1-dwWidthResult];
	}
	return true;
}
/////////////////////////////////////////////////////////////////////////////////
//
//将输入的布尔型向量图像转变为四分之一大小
/////////////////////////////////////////////////////////////////////////////////
bool CTrack::ChangeImageSize(BYTE *pBmp, DWORD dwHeight, DWORD dwWidth, BYTE *&pBitmapResult)
{
 	DWORD position=0;
 	DWORD dwHeight_now,dwWidth_now;
 	dwHeight_now=dwHeight>>1;
 	dwWidth_now = dwWidth>>1;
 
 	DWORD k=0,l=0;
 	for(DWORD i=0; i<dwHeight_now; i++)	
 	{
 		l=0;
 		for(DWORD j=0; j<dwWidth_now; j++)
 		{
 			pBitmapResult[position]=pBmp[k+l];
			l+=2;
 			position ++;
		}
		k=i<<1;
		k*=dwWidth;
 	}
 	return TRUE;
 }
////////////////////////////////////////////////////////////////////////////////////////////////////////
//  按照当前的参数到输入图像中截取目标图像：大小由目标长宽给出
//  得到的图像是0均值化的
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTrack::GetTargetImageByGivenAffineParameters(double*& pResult,double* a)
{
	double x,y,temp,x1,x2,x3,y1,y2,y3;
	double x_center = ((double)m_dwTargetWidth-1)/2;
	double y_center = ((double)m_dwTargetHeight-1)/2;
	double dMean    = 0;
	double dVariance = 0;
    DWORD position,i,j,position_l=0,dwObjectSize_now=m_dwTargetWidth*m_dwTargetHeight;
	double x_addition;
	double y_addition;
	
	
    //================有效性检测========================	
	if(a[1]*a[1]+a[4]*a[4] <0.001 || a[2]*a[2]+a[5]*a[5] <0.001)
	{
	//	AfxMessageBox("在采集当前选择框所覆盖的目标区时，区域瞬间变得太小！");
	//	return false;
	}
	if(a[1]*a[1]+a[4]*a[4] >9 || a[2]*a[2]+a[5]*a[5] >9)
	{
	//	AfxMessageBox("在采集当前选择框所覆盖的目标区时，区域瞬间变得太大！");
	//	return false;
	}
	//检测匹配框是否出界:(检测四个顶点是否出界)
	x  = a[0]-x_center*a[1]-y_center*a[2] -x_center ;
	x1 = a[0]+x_center*a[1]-y_center*a[2] +x_center ;
	x2 = a[0]-x_center*a[1]+y_center*a[2] -x_center ;
	x3 = a[0]+x_center*a[1]+y_center*a[2] +x_center ;
	y  = a[3]-x_center*a[4]-y_center*a[5] -y_center ;
	y1 = a[3]+x_center*a[4]-y_center*a[5] -y_center ;
	y2 = a[3]-x_center*a[4]+y_center*a[5] +y_center ;
	y3 = a[3]+x_center*a[4]+y_center*a[5] +y_center ;
	if(x<2||x1<2||x2<2||x3<2|| x>m_dwBmpWidth-2||x1>m_dwBmpWidth-2||x2>m_dwBmpWidth-2||x3>m_dwBmpWidth-2||  y<2||y1<2||y2<2||y3<2|| y1>m_dwBmpHeight-2||y2>m_dwBmpHeight-2||y3>m_dwBmpHeight-2||y>m_dwBmpHeight-2 ) 
	{
	//	AfxMessageBox("在采集当前选择框所覆盖的目标区时，发生越界！");
		return false;
	}

	//申请空间
	if(pResult) delete pResult;
	pResult = new double [m_dwTargetHeight*m_dwTargetWidth];

	double x_Left;
	double y_Left;
	//仿射变换,上中心偏移量
	x_Left = a[0] -x_center*a[1] -y_center*a[2] -x_center  ;
	y_Left = a[3] -x_center*a[4] -y_center*a[5] -y_center  ;

	x_addition = a[1]+1;
	y_addition = a[4];

	//采集目标图象
	for(j=0; j<m_dwTargetHeight; j++)
	{
		x = x_Left;
		y = y_Left;
		x_Left += a[2]; 
		y_Left += a[5]+1;
		for(i=0; i<m_dwTargetWidth; i++)
		{
			//计算当前点在输入图像中的位置
			position = (int)(y) * m_dwBmpWidth + (int)(x);

    /////============采集该点处的灰度值================
			//抽样：在2*2临域内采用内插法：
			temp  = m_pBmp[position] - (m_pBmp[position] - m_pBmp[position+1])* (x-(int)x);
			position += m_dwBmpWidth ;
			temp -= (temp-(m_pBmp[position] - (m_pBmp[position] - m_pBmp[position+1])* (x-(int)x)))*(y-(int)y);
			//插值结果赋给相应的标准化目标点:
			pResult[position_l] = temp;
			dMean += temp;
			dVariance+= temp*temp;

			position_l ++;//计算下一个点
			x += x_addition;
			y += y_addition;
		}
	}

	// 0 均值化:
	position = m_dwTargetWidth * m_dwTargetHeight;
	dMean /= (double)position;
	m_dVarianceTarget = sqrt(dVariance/position - dMean*dMean);

	if(TrackType==TT_AFFINE)
	{
		m_dGradationOffset= (BYTE) dMean;
		for(i=0; i<position;i++)
			pResult[i] -= dMean;
	} else
		m_dGradationOffset=0;

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 得到目标的图像：
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTrack::GetTargetBitmap(BYTE *&pTargetOut)
{
	if(!m_pBitmapTarget) 
	{	AfxMessageBox("没有目标图像");	return false;}

	DWORD i,j;
	j = m_dwTargetWidth*m_dwTargetHeight;

	if(pTargetOut) delete pTargetOut;
	pTargetOut = new BYTE[j];

	for(i=0;i<j;i++)
		pTargetOut[i] = (BYTE)(m_pBitmapTarget[i]+m_dGradationOffset);
	
	
	return true;
}
////////////////////////////////////////////////////////////////////////////////////
// 得到模板的图像：
////////////////////////////////////////////////////////////////////////////////////
bool CTrack::GetTemplateBitmap(BYTE *&pTemplateOut)
{
	if(!m_pTemplate) 
		return false;

	DWORD i,j;
	j = m_dwTargetHeight * m_dwTargetWidth;

	if(pTemplateOut) delete pTemplateOut;
	pTemplateOut = new BYTE [j];

	for(i=0;i<j;i++)
		pTemplateOut[i] = (BYTE) (m_pTemplate[i]+m_dTempalteGradationOffset);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  计算实际目标图象和真实位置之间的仿射差:a
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CTrack::CalculateParameter(double *pTemplateImage, int nTargetHeight, int nTargetWidth, double *pTargetImage,double *pTargetDifference, double delt,double *a)
{
	//有效性检测:
	if(!pTemplateImage || !pTargetImage) return false;

	DWORD dwSize = nTargetHeight*nTargetWidth;
	int position = 0;
	int position_l = 0;

	//分配内存,初始化	  
    double b[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	//设置当前b值
    double b_next[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	  
	bool bConverge = FALSE;
	int nCount = 	0;
	double delt_delt = delt*delt;

	double temp_b0_1,temp_b0_2,temp_b1_1,temp_b1_2,temp_b2_1,temp_b2_2,temp_b3_1,temp_b3_2,temp_b4_1,temp_b4_2,temp_b5_1,temp_b5_2;
	double iCenter_x,iCenter_y;
    iCenter_x = ((double)nTargetWidth-1)/2;
	iCenter_y = ((double)nTargetHeight-1)/2;
	  
	double temp_delt_05 = delt_delt*0.5;
	double temp_position;
	double temp_position_dsize;
	double ii,jj,temp_x;
	double temp,tempi,tempj,temp1,temp2;
	int i,j;
	bool bFirst = TRUE;
	  
	  while((nCount<2)&&(!bConverge))
	  {
		  temp_b0_2 = temp_b1_2 = temp_b2_2 = temp_b3_2 = temp_b4_2 = temp_b5_2 = 0;

		  //以下的参数只需要计算一次就行了:
		  if(bFirst)
			temp_b0_1 = temp_b1_1 = temp_b2_1 = temp_b3_1 = temp_b4_1 = temp_b5_1 = 0;
	
		  position  = 0;
		  for(j=0; j<nTargetHeight; j++)
		  {
			  for(i=0; i<nTargetWidth; i++) 
			  {
			  //对于小背景可以通过设置阈值滤掉,但是对于复杂背景设置应将阈值放大
	//		 if((type==1)||((type==4)&&((level && m_pTargetMatricSecond[position])||(!level && m_pTargetMatric[position]))))
			  {
				  ii = (double)i-iCenter_x;
				  jj = (double)j-iCenter_y;
				  //temp_position 为在position处的差分值:x
				  temp_position = pTargetDifference[position];
				  temp_position_dsize = pTargetDifference[position+dwSize];
				  //temp为x的平方
				  temp  = temp_position * temp_position;
				  //横纵坐标的平方
				  tempi = ii*ii;
				  tempj = jj*jj;
				  
				  //求fai(x,del);
				  temp_x = ((temp_position       * (b[0]+ii*b[1]+jj*b[2]) )+ 
					       (temp_position_dsize * (b[3]+ii*b[4]+jj*b[5]) )  )+
					       (pTargetImage[position] - pTemplateImage[position]);
				  temp_x = (2 * temp_x * delt_delt) / ((delt_delt + temp_x*temp_x)*(delt_delt + temp_x*temp_x));//temp_x*2;// 
				  
				  //最后的标志_1为计算分母的累加值,_2为计算分子的累加值
				  if(bFirst) temp_b0_1 += temp;
				  temp1      = temp_position  *  temp_x;
				  temp_b0_2 += temp1;
				  
				  if(bFirst) temp_b1_1 += temp * tempi;
				  temp_b1_2 += temp1 * ii; 

				  if(bFirst) temp_b2_1 += temp * tempj;
				  temp_b2_2 += temp1 * jj;

				  temp = temp_position_dsize * temp_position_dsize;

				  if(bFirst) temp_b3_1 += temp;
				  temp2      = temp_position_dsize  * temp_x;
				  temp_b3_2 += temp2;

				  if(bFirst) temp_b4_1 += temp * tempi;
				  temp_b4_2 += temp2 * ii;

				  if(bFirst) temp_b5_1 += temp * tempj;
				  temp_b5_2 += temp2 * jj;
			  }  
				  position ++;
			  }
			 
		  }
		  
		  if(bFirst)
		  {
			  temp_b0_1 = temp_delt_05 / temp_b0_1;
			  temp_b1_1 = temp_delt_05 / temp_b1_1;
			  temp_b2_1 = temp_delt_05 / temp_b2_1;
			  temp_b3_1 = temp_delt_05 / temp_b3_1;
			  temp_b4_1 = temp_delt_05 / temp_b4_1;
			  temp_b5_1 = temp_delt_05 / temp_b5_1;
		  }
		  bFirst = FALSE;
		  b_next[0] = temp_b0_2 * temp_b0_1;
		  b_next[1] = temp_b1_2 * temp_b1_1;
		  b_next[2] = temp_b2_2 * temp_b2_1;
		  b_next[3] = temp_b3_2 * temp_b3_1;
		  b_next[4] = temp_b4_2 * temp_b4_1;
		  b_next[5] = temp_b5_2 * temp_b5_1;
         

		  bConverge = TRUE;
		  if(b_next[0] >0.1 || b_next[0]<-0.1|| b_next[3]>0.1||b_next[3]<-0.1)
			  bConverge = FALSE;
		  if((bConverge)&&((b_next[1] > 0.005 || b_next[1]<-0.005)||(b_next[4] > 0.005 || b_next[4]<-0.005)||(b_next[5] > 0.005 || b_next[5]<-0.005))||(b_next[2] > 0.005 || b_next[2]<-0.005))
			  bConverge = FALSE;

		   for(i=0; i<6; i++)
			  b[i] -= b_next[i];

		  nCount ++;
	  }
	
	//拷贝结果:
	for(i=0; i<6; i++)
	   a[i] = b[i];

	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
//
//  计算当前金字塔层的I,到输入图像中抠取目标区域
//
/////////////////////////////////////////////////////////////////////////////////////////////////
bool CTrack::PyramidSampleTargetImageFromPictureAndCalculateDifferenceMatric(DWORD dHeight_now,DWORD dWidth_now,double* a,double* pInputImage,DWORD ImageWidth,DWORD ImageHeight,double *pTargetImage,double *pDifferenceMatric)
{
	double x,y,temp,x1,x2,x3,y1,y2,y3;
	double x_center = ((double)dWidth_now-1)/2;
	double y_center = ((double)dHeight_now-1)/2;
	double dMean    = 0;
	double dVariance = 0;
    DWORD position,i,j,position_l=0,dwObjectSize_now=dWidth_now*dHeight_now;
	double x_addition;
	double y_addition;
	
	
    //================有效性检测========================	
	if(a[1]*a[1]+a[4]*a[4] <0.001 || a[2]*a[2]+a[5]*a[5] <0.001)
	{
	//	AfxMessageBox("在采集当前选择框所覆盖的目标区时，区域瞬间变得太小！");
	//	return false;
	}
	if(a[1]*a[1]+a[4]*a[4] >9 || a[2]*a[2]+a[5]*a[5] >9)
	{
	//	AfxMessageBox("在采集当前选择框所覆盖的目标区时，区域瞬间变得太大！");
	//	return false;
	}
	//检测匹配框是否出界:(检测四个顶点是否出界)
	x  = a[0]-x_center*a[1]-y_center*a[2] -x_center ;
	x1 = a[0]+x_center*a[1]-y_center*a[2] +x_center ;
	x2 = a[0]-x_center*a[1]+y_center*a[2] -x_center ;
	x3 = a[0]+x_center*a[1]+y_center*a[2] +x_center ;
	y  = a[3]-x_center*a[4]-y_center*a[5] -y_center ;
	y1 = a[3]+x_center*a[4]-y_center*a[5] -y_center ;
	y2 = a[3]-x_center*a[4]+y_center*a[5] +y_center ;
	y3 = a[3]+x_center*a[4]+y_center*a[5] +y_center ;
	if(x<2||x1<2||x2<2||x3<2|| x>ImageWidth-2||x1>ImageWidth-2||x2>ImageWidth-2||x3>ImageWidth-2||  y<2||y1<2||y2<2||y3<2|| y1>ImageHeight-2||y2>ImageHeight-2||y3>ImageHeight-2||y>ImageHeight-2 ) 
	{
	//	AfxMessageBox("在采集当前选择框所覆盖的目标区时，发生越界！");
		return false;
	}

	double x_Left;
	double y_Left;
	//仿射变换,上中心偏移量
	x_Left = a[0] -x_center*a[1] -y_center*a[2] -x_center  ;
	y_Left = a[3] -x_center*a[4] -y_center*a[5] -y_center  ;

	x_addition = a[1]+1;
	y_addition = a[4];

	//采集目标图象
	for(j=0; j<dHeight_now; j++)
	{
		x = x_Left;
		y = y_Left;
		x_Left += a[2]; 
		y_Left += a[5]+1;
		for(i=0; i<dWidth_now; i++)
		{
			//计算当前点在输入图像中的位置
			position = (int)(y) * ImageWidth + (int)(x);

       //============采集该点处的灰度值================
			//抽样：在2*2临域内采用内插法：
			temp  = pInputImage[position] - (pInputImage[position] - pInputImage[position+1])* (x-(int)x);
			position += ImageWidth;
			temp -= (temp-(pInputImage[position] - (pInputImage[position] - pInputImage[position+1])* (x-(int)x)))*(y-(int)y);
			//插值结果赋给相应的标准化目标点:
			pTargetImage[position_l] = temp;
			dMean += temp;
			dVariance+= temp*temp;

	  //===========下面计算差分矩阵==================
			/*
			// 行差分
			position = (int)(y+0.5) * ImageWidth + (int)(x+0.5);
			temp = 2*(pInputImage[position+1])+pInputImage[position+1+ImageWidth]+pInputImage[position+1-ImageWidth]
				-2*(pInputImage[position-1])-pInputImage[position-1+ImageWidth]-pInputImage[position-1-ImageWidth];
			temp /= 8;
			pDifferenceMatric[position_l] = temp;
			// 列差分
			temp = 2*pInputImage[position+ImageWidth]+pInputImage[position+ImageWidth-1]+pInputImage[position+ImageWidth+1]
				-2*pInputImage[position-ImageWidth]-pInputImage[position-ImageWidth-1]-pInputImage[position-ImageWidth+1];
			temp /= 8;
			pDifferenceMatric[position_l+dwObjectSize_now] = temp;

			position_l ++;//计算下一个点
			x += x_addition;
			y += y_addition;
			*/
				// 行差分
			position = (int)(y+0.5) * ImageWidth + (int)(x+0.5);
			//temp = 2*(pInputImage[position+1])+pInputImage[position+1+ImageWidth]+pInputImage[position+1-ImageWidth]
			//	-2*(pInputImage[position-1])-pInputImage[position-1+ImageWidth]-pInputImage[position-1-ImageWidth];
			//temp /= 8;
			temp = (pInputImage[position+1])-pInputImage[position-1];
			temp /= 2;
			pDifferenceMatric[position_l] = temp;

			// 列差分
			//temp = 2*pInputImage[position+ImageWidth]+pInputImage[position+ImageWidth-1]+pInputImage[position+ImageWidth+1]
			//	-2*pInputImage[position-ImageWidth]-pInputImage[position-ImageWidth-1]-pInputImage[position-ImageWidth+1];
			//temp /= 8;
			temp = pInputImage[position+ImageWidth]-pInputImage[position-ImageWidth];
			//	-4*pInputImage[position-ImageWidth]-pInputImage[position-ImageWidth-1]-pInputImage[position-ImageWidth+1];
			temp /= 2;			
			pDifferenceMatric[position_l+dwObjectSize_now] = temp;
			position_l ++;//计算下一个点
			x += x_addition;
			y += y_addition;
		}
	}

	// 0 均值化:
	position = dWidth_now * dHeight_now;
	dMean /= (double)position;

	m_dVarianceTarget = sqrt(dVariance/position - dMean*dMean);
	m_dGradationOffset = (BYTE) dMean;

	if(TrackType==TT_AFFINE)
	{
		for(i=0; i<position; i++)
			pTargetImage[i] -= dMean;//*30.0/dVariance;

	}

    return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  计算金字塔当前层
//  主要用于计算b参数，也就是计算一次a(n+1),来调整一次a
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTrack::PyramidLevel(double* a,double delt,double* pInputImage,DWORD dwImageHeight,DWORD dwImageWidth,double*& pTemplate,double* pEigenSpace,DWORD dwTargetHeight,DWORD dwTargetWidth)
{
	  int i;
      //计算当前层的图像大小
	  DWORD dwSize   = dwTargetWidth * dwTargetHeight;

	  //设置目标图像和差分图像数组
	  double *pTargetImage      = (double*) new double [dwSize];
	  double *pTargetDifference = (double*) new double [dwSize*2];

//===========================抠取目标图象,并计算差分图像===============================================
	  if(!PyramidSampleTargetImageFromPictureAndCalculateDifferenceMatric(dwTargetHeight,dwTargetWidth,a,pInputImage,dwImageWidth,dwImageHeight,pTargetImage,pTargetDifference))
	  	  goto ERROR_PyramidLevel;
//==============计算重构图像====================================
	  if(TrackType==TT_SVD_AFFINE)
	  {
		  if(!ReconstructImageUseSVD(pTargetImage,dwSize,pEigenSpace,m_nNumberOfSelectedEigenVectors,pTemplate))
			  goto ERROR_PyramidLevel;
	  }else if(TrackType==TT_KL_AFFINE)
	  {
		  if(!ReconstructImageUseKL(pTargetImage,dwSize,pEigenSpace,m_nNumberOfSelectedEigenVectors,pTemplate))
			  goto ERROR_PyramidLevel;
	  }

///=============求解仿射参数=======================================
	double b[6];
	if(!CalculateParameter(pTemplate, dwTargetHeight, dwTargetWidth, pTargetImage, pTargetDifference, delt, b))
		goto ERROR_PyramidLevel;

    //在原来的基础上加上计算出来得增量,得到目标的最终姿态的仿射参数表示
	for(i=0;i<6;i++)
  	   a[i] += b[i];

	//释放无用空间;
	delete []pTargetImage;
	delete []pTargetDifference;
	return true;
	//错误返回：	
ERROR_PyramidLevel:
	delete []pTargetImage;
	delete []pTargetDifference;
	return false;
}
///////////////////////////////////////////////////////////////////////////////////
//
//  跟踪主函数，也是与外部的调用接口:
//  作用是：对于当前帧的输入图像做一次检测目标, 这只是一个面向外界的启动按钮函数
//
///////////////////////////////////////////////////////////////////////////////////
bool CTrack::Track( BYTE* pBitmap )
{
	double* pBmpSet = new double[m_dwBmpWidth*m_dwBmpHeight];
	for( int i=0; i< m_dwBmpWidth*m_dwBmpHeight; i++ )
		pBmpSet[i] = pBitmap[i];

	bool bRes = Track( pBmpSet );
	delete pBmpSet;
	return bRes;
}
bool CTrack::Track(double* pBmpSet)
{
	//将当前帧输入图像的指针保留下来：
	int j = m_dwBmpWidth*m_dwBmpHeight;
	if(m_pBmp) delete m_pBmp;
	m_pBmp = new double [j];
	memcpy(m_pBmp,pBmpSet,j*sizeof(double));
	
	PyramidSampleImage(m_dwBmpHeight,m_dwBmpWidth,m_pBmp,m_pBmp2);

    //保留上一帧的参数,作为当前帧的初始值：
	int i;
	for(i=0; i<6; i++)	a[i] = a_last[i];

 	//计算当前参数下搜索的情况，根据返回值判断是否需要搜索：
	m_bSearchStatus = false;

	if ( !Search( 2, m_bSearchStatus ) )  goto TRACK_ERROR;
	/*i = Search(0,m_bSearchStatus);
 	if(!i) goto TRACK_ERROR;
	if(i==2) //使用搜索：如果当前的模板和目标之间的误差较大，则采用搜索方法
	{	
		//先将参数恢复为原来的值
		for(i=0; i<6; i++)	a[i] = a_last[i];
		//在搜索一次，如果仍然找不到，则返回错误!
		m_bSearchStatus= true;
		if(TrackType==TT_SVD_AFFINE )
		{
			a[0] = a_last[0]; a[1] = a_last[1];	a[2] = 0;
			a[3] = a_last[3]; a[4] = 0;		a[5] = a_last[5];
			if(!(i = Search(4,m_bSearchStatus))) goto TRACK_ERROR;
			else if(i==2) 
			{
				a[0] = a_last[0];a[1] = a_last[1];	a[2] = 0;
				a[3] = a_last[3];a[4] = 0;			a[5] = a_last[5];
				if(!Search(6,m_bSearchStatus))  goto TRACK_ERROR;
			}
		}
		else if(TrackType == TT_AFFINE)
		{
			if(!Search(2,m_bSearchStatus))  goto TRACK_ERROR;
		}

		m_bSearchStatus = false;
		if(!Search(0,m_bSearchStatus)) goto TRACK_ERROR;
	}*/
	
	//在屏幕中画出结果来(将标准化的目标图像 取出来)：
	if(!GetTargetImageByGivenAffineParameters(m_pBitmapTarget,a))
		goto TRACK_ERROR;
	
	if(TemplateUpdateType == TTUT_PWOER && TrackType == TT_AFFINE)
	{
		if(m_dTemplateUpdatePower > 0.00001)
			m_dTemplateUpdatePower = 0.0125;
		else
			m_dTemplateUpdatePower = 0;
	
		UpdateTemplate();
	}
	
	//参数预测
	for(i=0;i<6;i++)
	{
		a_add[i] = a_add[i]*0.5 + (a[i]-a_last[i])*0.5;
		a_last[i] = a[i];
	}
	return true;

TRACK_ERROR:
	for(i=0;i<6;i++)
		a[i] = 0;
	return false;
}
////////////////////////////////////////////////////////////////////////
//
//采用搜索机制，找出极值点  bFine:寻找为0，跟踪为1
////////////////////////////////////////////////////////////////////////
int CTrack::Search(int nScope,bool bFine)
{
	DWORD dwSize = m_dwTargetWidth*m_dwTargetHeight;  //目标大小
	DWORD dwErrorCounterMin,dwErrorCounter;
	double a_min[6];  //保存最小值
	double a_last[6]; //保存a
	int m,n,x,y;
	int nStep = bFine ? 1:2;
	nScope /= 2;

	double threshold;  //对于不同的情况设置不同的提前退出门限:
	if(TrackType==TT_SVD_AFFINE || TrackType==TT_KL_AFFINE) threshold = 0.02;
	else if(TrackType==TT_AFFINE) threshold=0.2;
	
	//设置初始值
	int i;
	for( i=0; i<6; i++)	  a_last[i]=a[i];

	//计算初始点处的误差： 
	dwErrorCounterMin  = CalculateCurrentParameters(a,m_dDelt,bFine);  
/*	for(i=0; i<6; i++)  a_min[i] = a[i];
	if(dwErrorCounterMin<threshold*dwSize) 	goto EXIT_SEARCH;

	for( m=1;  m<nScope; m++ )
	for( x=-m; x<=m;     x++ )
	{
		y = m - abs(x);
		for( n=0; n<2; y=-y,n++ )
		{
			for(i=0; i<6; i++)	  a[i] = a_last[i];
			a[0] += x*nStep;
			a[3] += y*nStep;
			
			//计算在新的参数下的模板和实际得到的图象的误差
			dwErrorCounter = CalculateCurrentParameters(a,m_dDelt,bFine);
			if( dwErrorCounter<dwErrorCounterMin )
			{
				dwErrorCounterMin = dwErrorCounter;
				for(i=0; i<6; i++)  a_min[i] = a[i];
				if(dwErrorCounterMin<threshold*dwSize) 
					goto EXIT_SEARCH;  // 设定一个提前退出门限:
			}
		}
	}
	
EXIT_SEARCH:
	for(i=0;i<6;i++) 
		a[i] = a_min[i];

	//================有效性检测========================	
	if(dwErrorCounterMin<m_dwTargetWidth*m_dwTargetHeight) 
	{
		if(dwErrorCounterMin>(m_dwTargetWidth*m_dwTargetHeight*0.72)) 
			return 2;
		else
			return 1;
	}
	else 
	{
		AfxMessageBox("CTrack::Searching()目标不存在!");
		return 0;
	}*/
	return 1;
}
////////////////////////////////////////////////////////////////////////////////////
//
//  计算当前参数下的匹配结果：
//  本函数的主要目的是引起金字塔形运算
//
////////////////////////////////////////////////////////////////////////////////////
DWORD CTrack::CalculateCurrentParameters(double *a, double delt_now,bool bTypeCourseOrFine=TRUE)
{
	DWORD dwErrorCounter;
	int	  nCount_2=0;
	bool  bConverge = false;
	int   nLoopNumber = m_nMaxIterateTimes=8;
	int   i;

	double *pTemp=NULL;

	DWORD dwTargetHeight_now,dwTargetWidth_now;
	DWORD dwImageHeight_now,dwImageWidth_now;
	DWORD dwSize = m_dwTargetHeight * m_dwTargetWidth;
	int level ,nFactor;
	
	//准备数据:输入图象、模板图象:
	double* pInputImage[2];
	pInputImage[0] =  m_pBmp;
	pInputImage[1] =  m_pBmp2;
	
	//设置当前模板图象指针；
	double **ppTemplate[2];
	//设置当前特征空间：
	double *pEigenSpace[2];

	//准备模板图象:2个金字塔层
	ppTemplate[0] = &m_pTemplate;
	ppTemplate[1] = &m_pTemplate2;

	if(TrackType==TT_AFFINE)
		pEigenSpace[0] = pEigenSpace[1] = NULL;
	else if(TrackType==TT_SVD_AFFINE || TrackType==TT_KL_AFFINE)
	{   //准备特征空间:2个金字塔层
		pEigenSpace[0] = m_pEigenSpace;
		pEigenSpace[1] = m_pEigenSpace2;
	}else{
		dwErrorCounter = dwSize +1;
		goto EXIT_CalculateCurrentParameters;
	}

	double a_back[6];
	for(i=0;i<6;i++)
		a_back[i] = a[i];
  
		
//通过降低Delt值,做下列循环
//   while (nCount<3)  在一般的算法中没有考虑这一步
	{
		//在当前Delt值处作15遍
	  while ((nCount_2<nLoopNumber)&&(!bConverge))
		{
		    //采用的金字塔层数:
		    level   = m_nPyramidLevel-1;
			nFactor = 1<<level;
		  
			//将仿射变换参数映射到最高一层
			if(level)
			{
				a[0] /= nFactor;	
				a[3] /= nFactor; 
			
				if((m_dwTargetHeight)%2) 	a[3] -= 0.5;
				else 				  	    a[3] -= 0.25;
				if((m_dwTargetWidth) %2)	a[0] -= 0.5;
				else 				    	a[0] -= 0.25;
			}

			//从金字塔顶到底做一遍:level从1->0
	    	while (level>=0)
			{
				 //需要将大小进行映射:
				nFactor = 1 << level;
				dwTargetHeight_now = m_dwTargetHeight / nFactor;
				dwTargetWidth_now  = m_dwTargetWidth  / nFactor;
				
				dwImageHeight_now = m_dwBmpHeight / nFactor;
				dwImageWidth_now  = m_dwBmpWidth  / nFactor;
				
				//如果出错，则返回一个比实际目标面积大的值
		    	if(!PyramidLevel(a,delt_now,pInputImage[level],dwImageHeight_now,dwImageWidth_now,*(ppTemplate[level]),pEigenSpace[level],dwTargetHeight_now,dwTargetWidth_now)) 
				{
					dwErrorCounter = dwSize +1;
					goto EXIT_CalculateCurrentParameters;
				}

				//将仿射变换参数映射到底一层
				if(level)
				{	
					a[0] *= 2;		a[3] *= 2;	

					if((m_dwTargetHeight)%2) 	a[3] += 1;
					else 				  	    a[3] += 0.5;
					if((m_dwTargetWidth) %2)	a[0] += 1;
					else 				    	a[0] += 0.5;
				}
				//跳至下一层:
				level--; 
				
				//如果是搜索阶段，则只在目标的第二层金字塔中搜寻，所以跳开第0层搜索
				if(!bTypeCourseOrFine) level=-1;
			}
			//判断a是否收敛
			bConverge = true;
			if((a_back[0]-a[0] >0.1) || (a_back[0]-a[0]<-0.1) || (a_back[3]-a[3] >0.1) || (a_back[3]-a[3]<-0.1))			  bConverge = FALSE;
		    if((bConverge)&&((a_back[1]-a[1] > 0.005 || a_back[1]-a[1]<-0.005)||(a_back[4] -a[4]> 0.005 || a_back[4]-a[4]<-0.005)||(a_back[5]-a[5] > 0.005 || a_back[5]-a[5]<-0.005))||(a_back[2]-a[2] > 0.005 || a_back[2]-a[2]<-0.005))		  bConverge = false;

			//将本次的结果作为下次的初值:
	    	for(i=0; i<6; i++)         a_back[i] = a[i];

			//如果是当前帧最终结果，则求出和模板的相似性度量结果
			if(bConverge || nCount_2 ==nLoopNumber-1)
			{
				//抠取目标，计算误差：
				if(!GetTargetImageByGivenAffineParameters(m_pBitmapTarget,a))
					return dwSize+1;
				if(TrackType==TT_SVD_AFFINE)
					ReconstructImageUseSVD(m_pBitmapTarget,m_dwTargetHeight*m_dwTargetWidth,m_pEigenSpace,m_nNumberOfSelectedEigenVectors,m_pTemplate);
				else if(TrackType==TT_KL_AFFINE)
					ReconstructImageUseKL(m_pBitmapTarget,m_dwTargetHeight*m_dwTargetWidth,m_pEigenSpace,m_nNumberOfSelectedEigenVectors,m_pTemplate);
				dwErrorCounter = CheckResult(m_pBitmapTarget,m_pTemplate,m_dwTargetHeight*m_dwTargetWidth);

				goto EXIT_CalculateCurrentParameters;
			}

			//计算下一次,累加次数加1
			nCount_2 ++;
		}

		//修改Delt值再作
//		delt_now *= 0.80;  //注意此次修改Delt值累加
		//累计次数
//		nCount++;
	}

	for(i=0;i<6;i++)
		a[i] = a_back[i];

EXIT_CalculateCurrentParameters:
	
	return dwErrorCounter;
}
//////////////////////////////////////////////////////////////////
//
//  抽层：
//////////////////////////////////////////////////////////////////
bool CTrack::PyramidSampleImage(DWORD dwHeight, DWORD dwWidth, double *pImageInput,double *&pImageOutput)
{
	DWORD dwHeight_now = dwHeight/2;
	DWORD dwWidth_now  = dwWidth/2;
	
	if(pImageOutput) delete []pImageOutput;
	pImageOutput = (double*) new double [dwHeight_now*dwWidth_now];
	if(!pImageOutput) 
	{
		AfxMessageBox("PyramidSampleImage:pImage 内存申请错误!");
		return false;
	}

	DWORD dwPosition = 0;
	DWORD dwPosition_l = dwWidth;
	DWORD temp0 = dwWidth%2;

	DWORD i,j;
	for(i=0;i<dwHeight_now;i++)
	for(j=0;j<dwWidth_now;j++)
		pImageOutput[dwPosition++] = pImageInput[(i*dwWidth+j)*2];

	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
//  零均值化
//////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTrack::ZeroMean(double *pBitmap, DWORD dwSize,double& dMean)
{
	DWORD i;
	dMean = 0;
	for(i=0;i<dwSize;i++)
		dMean += pBitmap[i];
	
	dMean /= (double) dwSize;
	
	for(i=0;i<dwSize;i++)
		pBitmap[i]-= dMean;

    return true;
}
////////////////////////////////////////////////////////////////////////////////////////
//  检查当前结果 和模板图象的相似性程度
////////////////////////////////////////////////////////////////////////////////////////
DWORD CTrack::CheckResult(double *pTarget, double *pTemplate, DWORD dwSize)
{
	DWORD  dwResult=0;
	double dthreshold,temp;

	dthreshold = m_dVarianceTarget / 4;

	for(DWORD i=0;i<dwSize;i++)
	{
		temp = pTarget[i]-pTemplate[i];
		if(temp<0) temp = 0-temp;
		if(temp>dthreshold) 
			dwResult ++;
	}

	return dwResult;
}

////////////////////////////////////////////////////////////////////////////////
//
//  测试用将某块区域以图像格式写到硬盘里
//
////////////////////////////////////////////////////////////////////////////////
bool CTrack::Test(DWORD dwHeight, DWORD dwWidth, double *pInput)
{
	  static CString str="e:\\test000.bmp";
	  DWORD dwSize = dwHeight*dwWidth;
	  BYTE* pTemp=new BYTE[dwSize];
	  int i,j = dwSize;
	  for(i=0;i<j;i++)
		  pTemp[i] = (BYTE)(pInput[i] + m_dGradationOffset);
	  CZXDib dib;
	  dib.WriteBMPFileFromVector(str,pTemp,dwHeight,dwWidth,8);
	  dib.FindNextFileName(str,1);
	  return true;
}
////////////////////////////////////////////////////////////////////////////////////
//  重构目标图象
////////////////////////////////////////////////////////////////////////////////////
bool CTrack::ReconstructImageUseKL(double *pTarget, DWORD dwSize,double *pEigenSpace,int nNumberOfEigens,double *&pResult)
{
    double *C=NULL;
	if(!(C=(double*)new double [nNumberOfEigens]))
	{
		AfxMessageBox("分配内存失败！C \n PyramidCalculateImitateTargetImage()");
		return false;
	}
    double temp;		
	DWORD position = 0;
	DWORD i,j;
	
	//减去均值：
	if(pResult) delete []pResult;
	pResult =  new double[dwSize];
	for(i=0;i<dwSize;i++)
		pResult[i] = pTarget[i] - pEigenSpace[i];
	
    //计算拟合系数C值
	position = dwSize;
	for( j=0; j<nNumberOfEigens; j++){
		 C[j] = 0;
		 for( i=0; i<dwSize; i++)
			 C[j] += pEigenSpace[i+position] * pResult[i];
		 position += dwSize;
	}

	//计算拟合图像
	for( i=0; i<dwSize; i++)
	{
		position = dwSize;
		temp = 0;
		for(j=0; j<nNumberOfEigens; j++){
			temp += pEigenSpace[i+position] * C[j];
			position += dwSize;
		}
		pResult[i] = temp>255? 255:temp;
	}

	delete []C;
	
	for(i=0;i<dwSize;i++)
		pResult[i] += pEigenSpace[i];

	return true;
}

//////////////////////////////////////////////////////////////////
// 更新模板：
//////////////////////////////////////////////////////////////////
void CTrack::UpdateTemplate()
{
	//更换模板:采用上一贞加权方式
	DWORD dwSize = m_dwTargetHeight*m_dwTargetWidth;
	for(DWORD i=0; i<dwSize; i++)
		m_pTemplate[i] = m_pTemplate[i]*(1-m_dTemplateUpdatePower)+m_pBitmapTarget[i]*m_dTemplateUpdatePower;
	PyramidSampleImage(m_dwTargetHeight,m_dwTargetWidth,m_pTemplate,m_pTemplate2);

	return ;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// 互相关跟踪
///////////////////////////////////////////////////////////////////////////////////////////
bool CTrack::RelationTrack(double *pBitmapPicture, DWORD dwWidthPicture, DWORD dwHeightPicture, double *pBitmapTemplate, RECT& rect)
{
   int dwWidthTemplate;
   int dwHeightTemplate;
   int dwSearchWindow_x;
   int dwSearchWindow_y;
   dwSearchWindow_x = dwSearchWindow_y=10;
   dwWidthTemplate = rect.right-rect.left;
   dwHeightTemplate = rect.bottom-rect.top;

   if(!pBitmapTemplate || !pBitmapPicture) 
	   return FALSE;
   if(rect.right+dwWidthTemplate>(int)dwWidthPicture || rect.left<0 || rect.top<0 || rect.bottom+dwHeightTemplate>(int)dwHeightPicture)
   {
	   AfxMessageBox("互相关算法截取目标区时发生越界!");
	   return FALSE;
   }

   int i,j,k,l;
   DWORD position,position_l;
   int target_x = 0,target_y=0;
   double counter=0;
   double min_counter = dwWidthTemplate*dwHeightTemplate*255;
   double temp;
   
   //找最佳匹配点
   for (l=0-dwSearchWindow_y; l<dwSearchWindow_y; l++)
   for (k=0-dwSearchWindow_x; k<dwSearchWindow_x; k++)
   {
	   counter=0;
	   position=0;
	   for(j=rect.top+l; j<=rect.top+l+dwHeightTemplate; j++)
	   for(i=rect.left+k; i<=rect.left+k+dwWidthTemplate; i++)
	   {
		   temp = pBitmapTemplate[position];
		   position++;
		   temp -= pBitmapPicture[j*dwWidthPicture+i];
		   if(temp<0) temp = 0-temp;
		   counter += temp;
	   }
	   if(min_counter>counter)
	   {
		   target_x=k;
		   target_y=l;
		   min_counter = counter;
	   }
   }

   //最佳位置：
   rect.left += target_x;
   rect.right += target_x;
   rect.top+= target_y;
   rect.bottom+= target_y;

   target_x =rect.left;
   target_y =rect.top;

    
   return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
// 互相关跟踪
///////////////////////////////////////////////////////////////////////////////////////////
POINT CTrack::RelationTrack(BYTE *pBitmapPicture, DWORD dwHeightPicture,DWORD dwWidthPicture,  BYTE *pBitmapTemplate, DWORD dwTemplateHeight,DWORD dwTemplateWidth,RECT rectSearck)
{
   POINT point;
   point.x = -1;
   point.y = -1;

   if(!pBitmapTemplate || !pBitmapPicture)   return point;

   if(rectSearck.right+dwTemplateWidth>(int)dwWidthPicture || rectSearck.left<0 || rectSearck.top<0 || rectSearck.bottom+dwTemplateHeight>(int)dwHeightPicture)
   {   AfxMessageBox("互相关算法截取目标区时发生越界!");
	   return point;
   }

   int i,j,k,l;
   double counter=0;
   double min_counter = dwTemplateWidth*dwTemplateHeight*255;
   double temp;
   
   //找最佳匹配点
   for (k=rectSearck.top; k<=rectSearck.bottom; k++)
   for (l=rectSearck.left; l<=rectSearck.right; l++)
   {
	   counter=0;
	   for(j=0; j<dwTemplateHeight; j++)
	   for(i=0; i<dwTemplateWidth;  i++)
	   {
		   temp = pBitmapTemplate[j*dwTemplateWidth+i];
		   temp -= pBitmapPicture[(j+k)*dwWidthPicture+(i+l)];
		   if(temp<0) temp = 0-temp;
		   counter += temp;
	   }
	   if(min_counter>counter)
	   {
		   point.x=l;
		   point.y=k;
		   min_counter = counter;
	   }
   }
      
   return point;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//    互相关跟踪    pMask:使用掩膜
//
///////////////////////////////////////////////////////////////////////////////////////////
POINT CTrack::RelationTrack(BYTE *pBitmapPicture, DWORD dwHeightPicture,DWORD dwWidthPicture,  BYTE *pBitmapTemplate, DWORD dwTemplateHeight,DWORD dwTemplateWidth,BYTE* pMask,RECT rectSearck)
{
   POINT point;
   point.x = -1;
   point.y = -1;

   if(!pBitmapTemplate || !pBitmapPicture || !pMask)   return point;

   if(rectSearck.right+dwTemplateWidth>(int)dwWidthPicture || rectSearck.left<0 || rectSearck.top<0 || rectSearck.bottom+dwTemplateHeight>(int)dwHeightPicture)
   {   AfxMessageBox("互相关算法截取目标区时发生越界!");
	   return point;
   }

   int i,j,k,l;
   double counter=0;
   double min_counter = dwTemplateWidth*dwTemplateHeight*255;
   double temp;
   
   //找最佳匹配点   pMask
   for (k=rectSearck.top; k<=rectSearck.bottom; k++)
   for (l=rectSearck.left; l<=rectSearck.right; l++)
   {
	   counter=0;
	   for(j=0; j<dwTemplateHeight; j++)
	   for(i=0; i<dwTemplateWidth;  i++)
	   {
		   if(pMask[j*dwTemplateWidth+i])
		   {
			   temp = pBitmapTemplate[j*dwTemplateWidth+i];
			   temp -= pBitmapPicture[(j+k)*dwWidthPicture+(i+l)];
			   if(temp<0) temp = 0-temp;
			   counter += temp;
		   }
	   }
	   if(min_counter>counter)
	   {
		   point.x=l;
		   point.y=k;
		   min_counter = counter;
	   }
   }
      
   return point;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
// 互相关跟踪
///////////////////////////////////////////////////////////////////////////////////////////
bool CTrack::RelationTrack(BYTE *pBitmapPicture, DWORD dwHeightPicture,DWORD dwWidthPicture, BYTE *pBitmapTemplate, DWORD dwTemplateHeight,DWORD dwTemplateWidth, RECT rectSearck,POINT*& point,int nPoint)
{
   if(!pBitmapTemplate || !pBitmapPicture)   return false;

   if(rectSearck.right+dwTemplateWidth>(int)dwWidthPicture || rectSearck.left<0 || rectSearck.top<0 || rectSearck.bottom+dwTemplateHeight>(int)dwHeightPicture)
   {   AfxMessageBox("互相关算法截取目标区时发生越界!");
	   return false;
   }

   if(point) delete point;
   point = new POINT [nPoint];

   int i,j,k,l;
   double counter=0;
   double min_counter = dwTemplateWidth*dwTemplateHeight*255;
   double temp;

   for(i=0;i<nPoint;i++)
	   point[i].x = point[i].y = -1;

   struct Nod
   {
	   int nvalue;
	   int x,y;
	   Nod* pnext;
   };
   Nod* pNod=NULL;
   Nod *pNow, *pLast;
   int nNod = 0;
   
   //找最佳匹配点
   for (k=rectSearck.top; k<=rectSearck.bottom; k++)
   for (l=rectSearck.left; l<=rectSearck.right; l++)
   {
	   counter=0;
	   for(j=0; j<dwTemplateHeight; j++)
	   for(i=0; i<dwTemplateWidth;  i++)
	   {
		   temp = pBitmapTemplate[j*dwTemplateWidth+i];
		   temp -= pBitmapPicture[(j+k)*dwWidthPicture+(i+l)];
		   if(temp<0) temp = 0-temp;
		   counter += temp;
	   }
	
	   if(!pNod)
	   {
		   pNod = new Nod[1];
		   pNod->pnext = NULL;
		   pNod->nvalue = counter;
		   pNod->x = l;
		   pNod->y = k;
		   nNod = 1;
	   }else 
	   {
		   pLast = pNow = pNod;
		   while(pNow && pNow->nvalue < counter)
		   {
			   pLast = pNow;
			   pNow = pNow->pnext;
		   }
		   if(pNow == pNod)
		   { 
			   pNow = new Nod[1];
			   pNow->nvalue = counter;
			   pNow->x = l;
			   pNow->y = k;
			   pNow->pnext = pNod;
			   pNod = pNow;
			   nNod ++;
		   } else
		   {
			   pNow = new Nod[1];
			   pNow->nvalue = counter;
			   pNow->x = l;
			   pNow->y = k;
			   
			   pNow->pnext = pLast->pnext;
			   pLast->pnext = pNow;
			   nNod ++;
		   
			   if(nNod > nPoint)
			   {
				   while(pNow->pnext)
				   {
					   pLast = pNow;
					   pNow = pNow->pnext;
				   }
				   pLast->pnext = NULL;
				   delete pNow;
			   }
		   }
	   }
  }

   pNow = pNod;
   for(i=0;i<nPoint && pNow;i++)
   {
	   point[i].x = pNow->x;
	   point[i].y = pNow->y;
	   pNow = pNow->pnext;
   }

      
   return point;
}
/////////////////////////////////////////////////////////////////////////
//
//  寻找下一个文件名
//
/////////////////////////////////////////////////////////////////////////
bool CTrack::FindNextFileName(CString &FileName,int iStep)
{
	int nCurrentPosition;
	int position = FileName.Find(".bmp");
	if(position<0) 
		position = FileName.Find(".BMP");
	position--;

	while(iStep>0)
	{
		nCurrentPosition = position;
		char ch =  FileName.GetAt(nCurrentPosition);
		while (ch=='9' && nCurrentPosition>=0)
		{
			FileName.SetAt(nCurrentPosition,'0');
			nCurrentPosition--;
			ch =  FileName.GetAt(nCurrentPosition);
		}
		ch++;
		FileName.SetAt(nCurrentPosition,ch);

		iStep--;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//　　模板更新
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTrack::UpdateTemplate(BYTE *pTemplate_old, BYTE *pTemplate_new, DWORD dwHeight, DWORD dwWidth, double factor)
{
	DWORD i,dwSize;
	dwSize = dwHeight*dwWidth;
	double temp;

	for(i=0;i<dwSize;i++)
	{
		temp = (double)(pTemplate_old[i])*(1-factor)+(double)(pTemplate_new[i])*factor;
		pTemplate_old[i] = (int)temp;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// 互相关跟踪
///////////////////////////////////////////////////////////////////////////////////////////
POINT CTrack::RelationTrack(BYTE *pBitmapPicture, DWORD dwHeightPicture,DWORD dwWidthPicture,  BYTE *pBitmapTemplate, DWORD dwTemplateHeight,DWORD dwTemplateWidth,int gate,RECT rectSearck)
{
   POINT point;
   point.x = -1;
   point.y = -1;

   if(!pBitmapTemplate || !pBitmapPicture)   return point;

   if(rectSearck.right+dwTemplateWidth>=(int)dwWidthPicture)
	   rectSearck.right = dwWidthPicture -1- dwTemplateWidth;
   if(rectSearck.right<=rectSearck.left)
   {
	   AfxMessageBox("互相关算法无法截取目标区!");
	   return point;
   }
   if(rectSearck.bottom+dwTemplateHeight>=(int)dwHeightPicture)
	   rectSearck.bottom = dwHeightPicture -1- dwTemplateHeight;
   if(rectSearck.top>=rectSearck.bottom)
   {
	   AfxMessageBox("互相关算法无法截取目标区!");
	   return point;
   }
   

   if( rectSearck.left<0 || rectSearck.top<0 )
   {   AfxMessageBox("互相关算法截取目标区时发生越界!");
	   return point;
   }

   int i,j,k,l;
   double counter=0;
   double min_counter = dwTemplateWidth*dwTemplateHeight*255;
   double temp;
   
   //找最佳匹配点
   for (k=rectSearck.top; k<=rectSearck.bottom; k++)
   for (l=rectSearck.left; l<=rectSearck.right; l++)
   {
	   counter=0;
	   for(j=0; j<dwTemplateHeight; j++)
	   for(i=0; i<dwTemplateWidth;  i++)
	   {
		   temp = pBitmapTemplate[j*dwTemplateWidth+i];
		   temp -= pBitmapPicture[(j+k)*dwWidthPicture+(i+l)];
		   if(temp<0) temp = 0-temp;
		   if(temp>gate)
			   counter += temp;
	   }
	   if(min_counter>counter)
	   {
		   point.x=l;
		   point.y=k;
		   min_counter = counter;
	   }
   }
      
   return point;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   相似性区域分析
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CTrack::ResembleAreaAnalysis(BYTE *pTemplate, DWORD dwHeight, DWORD dwWidth, BYTE *pTarget, BYTE *&pResult)
{
	if(pResult) delete pResult;
	pResult = new BYTE [dwHeight*dwWidth];
	memset(pResult,255,sizeof(BYTE)*dwHeight*dwWidth);

	int temp,correlation_max;
	int nOffset;

	int i,j,k,l;
	for(i=3; i<dwHeight-3; i++)
	for(j=3; j<dwWidth-3 ; j++)
	{
		correlation_max = 255*dwHeight*dwWidth;
		nOffset = 0;
		for(k=-2;k<3;k++)//搜索 在3*3大小区域
		for(l=-2;l<3;l++)
		{                        //下面计算以(i,j)为中心的3*3小区域相关值:绝对差法
			temp =  abs((int)(pTemplate[(i+k-1)*dwWidth+j+l-1]) - (int)(pTarget[(i-1)*dwWidth+j-1]));
			temp += abs((int)(pTemplate[(i+k-1)*dwWidth+j+l])   - (int)(pTarget[(i-1)*dwWidth+j])); 
			temp += abs((int)(pTemplate[(i+k-1)*dwWidth+j+l+1]) - (int)(pTarget[(i-1)*dwWidth+j+1])); 
			temp += abs((int)(pTemplate[(i+k)*dwWidth+j+l-1])   - (int)(pTarget[i*dwWidth+j-1])); 
			temp += abs((int)(pTemplate[(i+k)*dwWidth+j+l])     - (int)(pTarget[i*dwWidth+j])); 
			temp += abs((int)(pTemplate[(i+k)*dwWidth+j+l+1])   - (int)(pTarget[i*dwWidth+j+1])); 
			temp += abs((int)(pTemplate[(i+k+1)*dwWidth+j+l-1]) - (int)(pTarget[(i+1)*dwWidth+j-1])); 
			temp += abs((int)(pTemplate[(i+k+1)*dwWidth+j+l])   - (int)(pTarget[(i+1)*dwWidth+j])); 
			temp += abs((int)(pTemplate[(i+k+1)*dwWidth+j+l+1]) - (int)(pTarget[(i+1)*dwWidth+j+1])); 
			
			//temp = abs((int)(pTemplate[(i+k)*dwWidth+j+l])     - (int)(pTarget[i*dwWidth+j])); 
			if(correlation_max>temp) 
			{
				correlation_max = temp;
				nOffset = (k+1)*(k+1) + (l+1)*(l+1);
			}
		}
		pResult[i*dwWidth+j] = correlation_max>255? 255:correlation_max;// nOffset;
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////
//     采用方差归一积互相关:    //
////////////////////////////////////////////////////////////////////////////////
POINT CTrack::CorrelationMatchDeviationUnify(BYTE *pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE* pTemplate,DWORD dwTemplateHeight,DWORD dwTemplateWidth,RECT rectSearch)
{
	POINT pointResult;
	pointResult.x = -1;
	pointResult.y = -1;

	if(!pBitmap || !pTemplate) 
	   return pointResult;
   
	if(rectSearch.left > dwWidth || rectSearch.bottom > dwHeight) return pointResult;
    if(dwTemplateHeight+rectSearch.bottom > dwHeight )
		rectSearch.bottom = dwHeight - dwTemplateHeight;
	if(dwTemplateWidth + rectSearch.right > dwWidth)
		rectSearch.right = dwWidth - dwTemplateWidth;
	
	
   DWORD dwSize= dwTemplateHeight*dwTemplateWidth;
   int i,j,k,l,position;
   double counter=0,temp;
   double max_counter = 0;
   double temp_mean,temp_mean_input;
   double temp_delt,temp_delt_input;

   temp_mean = 0;
   temp_delt = 0;
   for(i=0; i<dwSize; i++)
   {
	   temp = pTemplate[i];
	   temp_mean += temp;
	   temp_delt += temp*temp;
   }
   temp_mean /= dwSize;
   temp_delt = sqrt((temp_delt/(double)dwSize - temp_mean*temp_mean));
   

   //找最佳匹配点
   for (l=rectSearch.top; l<rectSearch.bottom; l++)
   for (k=rectSearch.left; k<rectSearch.right; k++)
   {
	  //求解当前小窗口处方差和均值
	  temp_mean_input=0;
	  temp_delt_input=0;
	  for(i=l;i<l+dwTemplateHeight;i++)
	  for(j=k;j<k+dwTemplateWidth;j++)
	  {
		  temp = pBitmap[i*dwWidth+j];
		  temp_mean_input += temp;
		  temp_delt_input += temp*temp;
	  }
	  temp_mean_input /= dwSize;
	  temp_delt_input  = sqrt(temp_delt_input/(double)dwSize - temp_mean_input*temp_mean_input);

	  //计算当前点处的相关直
	  counter=0;
	  position = 0;
      for(i=l;i<l+dwTemplateHeight;i++)
	  for(j=k;j<k+dwTemplateWidth;j++)
	  {
		  temp = ((double)(pTemplate[position])-temp_mean) * ((double)(pBitmap[i*dwWidth+j]) - temp_mean_input);
		  counter += temp;
		  position++;
	   }
	   counter /= temp_delt_input;

	   if(max_counter<counter)
	   {
		   pointResult.x=k;
		   pointResult.y=l;
		   max_counter = counter;
	   }
	}

    return pointResult;
}
////////////////////////////////////////////////////////////////////////////////
//     采用方差归一积互相关:    //
////////////////////////////////////////////////////////////////////////////////
POINT CTrack::CorrelationMatchDeviationUnify(BYTE *pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE* pTemplate,DWORD dwTemplateHeight,DWORD dwTemplateWidth,BYTE* pMask,RECT rectSearch)
{
	POINT pointResult;
	pointResult.x = -1;
	pointResult.y = -1;

	if(!pBitmap || !pTemplate || !pMask) 
	   return pointResult;
   
	if(rectSearch.left > dwWidth || rectSearch.bottom > dwHeight) return pointResult;
    if(dwTemplateHeight+rectSearch.bottom > dwHeight )
		rectSearch.bottom = dwHeight - dwTemplateHeight;
	if(dwTemplateWidth + rectSearch.right > dwWidth)
		rectSearch.right = dwWidth - dwTemplateWidth;
	
	
   DWORD dwSize= dwTemplateHeight*dwTemplateWidth;
   int i,j,k,l,position;
   double counter=0,temp;
   double max_counter = 0;
   double temp_mean,temp_mean_input;
   double temp_delt,temp_delt_input;

   temp_mean = 0;
   temp_delt = 0;
   for(i=0; i<dwSize; i++)
   {
	   if(pMask[i])
	   {
		   temp = pTemplate[i];
		   temp_mean += temp;
		   temp_delt += temp*temp;
	   }
   }
   temp_mean /= dwSize;
   temp_delt = sqrt((temp_delt/(double)dwSize - temp_mean*temp_mean));
   

   //找最佳匹配点
   for (l=rectSearch.top; l<rectSearch.bottom; l++)
   for (k=rectSearch.left; k<rectSearch.right; k++)
   {
	  //求解当前小窗口处方差和均值
	  position = 0;
	  temp_mean_input=0;
	  temp_delt_input=0;
	  for(i=l;i<l+dwTemplateHeight;i++)
	  for(j=k;j<k+dwTemplateWidth;j++)
	  {
		  if(pMask[position])
		  {
			  temp = pBitmap[i*dwWidth+j];
			  temp_mean_input += temp;
			  temp_delt_input += temp*temp;
		  }
		  position ++;
	  }
	  temp_mean_input /= dwSize;
	  temp_delt_input  = sqrt(temp_delt_input/(double)dwSize - temp_mean_input*temp_mean_input);

	  //计算当前点处的相关直
	  counter=0;
	  position = 0;
      for(i=l;i<l+dwTemplateHeight;i++)
	  for(j=k;j<k+dwTemplateWidth;j++)
	  {
		  if(pMask[position])
		  {
			  temp = ((double)(pTemplate[position])-temp_mean) * ((double)(pBitmap[i*dwWidth+j]) - temp_mean_input);
			  counter += temp;
		  }
		  position++;
	   }
	   counter /= temp_delt_input;

	   if(max_counter<counter)
	   {
		   pointResult.x=k;
		   pointResult.y=l;
		   max_counter = counter;
	   }
	}

    return pointResult;
}