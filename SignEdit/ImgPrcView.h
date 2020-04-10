// View.h : interface of the CImgPrcView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEW_H__6B8B9C31_49BE_4AFA_81F5_80A16794BC0C__INCLUDED_)
#define AFX_VIEW_H__6B8B9C31_49BE_4AFA_81F5_80A16794BC0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//自定义消息

#include <afxtempl.h>
#include "MainFrm.h"
#include "ImgPrcDoc.h"
#include <opencv2\opencv.hpp>
#include "MorphDlg.h"
#include "HistDlg.h"
#include "BinaryRegion.h"
#include "Polygon.h"
#include "ConerDetector.h"
#include "MSER.h"
#include "SiftDialog.h"
#include "SmoothDlg.h"
#include "logdogDlg.h"
#include "ColorConv.h"
#include "ThresholdSeg.h"
#include "AdaptiveThreshold.h"
#include "IterativeThreshold.h"
#include "StandardHoffTran.h"
#include "PPHT.h"
#include "HoughCircleTran.h"
#include "SobelDlg.h"
#include "RobertDlg.h"
#include "CannyDlgEx.h"
#include "LaplacianDlg.h"
#include "GaborDlg.h"
#include "edgeAlgorithm.h"
#include "FRect.h"
#include "DlgFtrTrack.h"
#include "Target.h"

using namespace cv;


#define __DEBUG  true	/*调试*/

class CImgPrcView: public CScrollView
{
protected: // create from serialization only
	CImgPrcView();
	DECLARE_DYNCREATE( CImgPrcView )

	// Attributes
public:

	int m_nFileSource; // 1:bmp, 2:mp4
	BYTE* m_pBitmap;
	int m_nWidth, m_nHeight;
	WORD  m_flag;

	VideoCapture *m_pVideo;
	int m_nFrmID;
	int m_nFrmCnt;

	CString m_strFileName;

	BYTE* m_pBitmap2;
	int m_nWidth2, m_nHeight2;
	WORD  m_flag2;

	CString m_strFileName2;

	CFRect m_rectShow;

	//当前处理的图片
	BYTE* m_pBmpProc;
	int m_nWidthProc, m_nHeightProc;
	WORD m_flagProc;
	BOOL m_isBinary;

	BOOL m_bSelecting;
	BOOL m_bMouseDown;
	BOOL m_bDragging;
	CPoint m_MousePoint;
	double m_dScaleRatio;

	bool LoadBitmap();//加载BPM图像文件
	void GetSrcDest( CRect rctImg, CRect& rctSrc, CRect& rctDest );
	void Draw( CDC*pDC, BYTE* pImg, int nHeight, int nWidth, WORD flag, CRect rectSrc, CRect rectDest );
	void AddInterResultImage( BYTE* pBitmap, int dwHeight, int dwWidth, WORD flag, CString strMark );
	void SetInterResultImage( int nID, BYTE* pBitmap, int dwHeight, int dwWidth, WORD flag, CString strMark );
	void SetInterResultImage( int nID, double* pBitmap, int dwHeight, int dwWidth, WORD flag, CString strMark );
	void SetInterResultImage( int nID, Mat& img, CString strMark = "" );
	void FreeInterResultImage( bool bInvalide = true );

	void RemoveCamShake( Mat &referImg, Mat &img, double&x, double&y );
	void RemoveCamShake2( Mat &referImg, Mat &img, double&x, double&y );
	void SilhouetteMatch( Mat& img, Mat& templ );
	void GetPedSearchArea( Mat& roadMask, int x, int &y1, int &y2 );
	void GetPedSearchArea( Mat& roadMaskU, Mat& roadMaskD, std::vector<Point2i >& pPedRange );

	void Process( Mat& srcImg );
	void UpdateImgFromROI( Mat& srcImg );
	void FindMidLine( Mat& img );
	int CheckObjects( Mat& mask, Mat& roadMask, vector<Rect>& rctObjs, bool bPed = false, int* pnPedCnt = NULL );
	void GetCentroidBox( Mat& img, Rect2d& rctObj, int nMaxW, int nMaxH );

	void CurrentFocusPos();
	void FocusMovement();
	void LoadMappingData( CString& strFileName, Mat& img );
	void BackProcess( CTarget*& pTarget );
	void BackProcess( CTarget* pTarget, int &nPos, bool &bRecapture, int& nFrame );
	void GetProject( Mat& mog );
	void SegmentPrj( int* pPrj, int nLen, int nGate, vector<int> &objx, vector<int> &objy );

	void ShowCursorInfo( CString strText, CString strText2, CString strText3, CString strText4 );
	void GetCurMouse( double& x, double &y, int &r, int &g, int &b, CString &strTitle );
	void  Delete();

	BOOL m_bReFocus;
	CString m_strFocusTitle;
	double m_dFocusX;
	double m_dFocusY;

	BYTE** m_ppItrImage;
	int* m_pnItrImgHeight;
	int* m_pnItrImgWidth;
	WORD*  m_pItrImageFlag;
	int  m_nItrImageCount;
	CString * m_pstrItrMark;

	CString m_strTitle;

	BYTE* m_pBitmapDisplay;
	BITMAPINFO* m_pBitmapInfo;

	CImgPrcDoc* GetDocument();

#if CV_SUPPORT
	Mat Image_1;
	Mat Image_2;

	void ConvToMat( void )
	{
		if ( m_pBitmap )
		{
			if ( m_flag == 24 )
				Image_1 = Mat( m_nWidth, m_nHeight, CV_8UC3 );
			else if ( m_flag == 8 )
				Image_1 = Mat( m_nWidth, m_nHeight, CV_8UC1 );
			Image_1.data = m_pBitmap;
		}
		else
		{
			Image_1 = Mat( 0, 0, CV_8UC3 );
			Image_1.data = NULL;
		}
		if ( m_pBitmap2 )
		{
			if ( m_flag2 == 24 )
				Image_2 = Mat( m_nWidth2, m_nHeight2, CV_8UC3 );
			else if ( m_flag2 == 8 )
				Image_2 = Mat( m_nWidth2, m_nHeight2, CV_8UC1 );
			Image_2.data = m_pBitmap2;
		}
		else
		{
			Image_2 = Mat( 0, 0, CV_8UC3 );
			Image_2.data = NULL;
		}
	}
#endif

private:
	double GetCurRatio();
	void DrawSelection( Mat& img, const cv::Scalar &color );
	void Draw( CDC*pDC, SIZE szClient, CFRect rectShow );
	void CalTotalRect( CDC* pDC, CRect& rectTotal );
	void CalCurImgRect( CDC* pDC, int nID, CRect& rectShow );
	void Process1( Mat& srcImg, Mat &mask );
	void Move( double x, double y );
	void Copy();
	void Paste();
	void FindSign();
	void GetBackColor( int nID, BYTE pBack[3] );
	void AddBackNoise( Mat &imgSign, double dScale = 10, int nBlurSize = 2 );
	void StatisticBackPattern();
	void OnBackLoad();
	void FusionFrame_Texture( Mat &matFrame, Mat& matSign, Mat& rest, BYTE pBack[3], double dFrame, double dTexture, double dBackground, double dNoise = 15 );


	// Operations
public:
	static UINT SignMosaic( LPVOID l );

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgPrcView)
public:
	virtual void OnDraw( CDC* pDC );  // overridden to draw this view
	Point2f GetCentroidFromBinary( BYTE* pBinary, int dwHeight, int dwWidth );
	virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting( CPrintInfo* pInfo );
	virtual void OnBeginPrinting( CDC* pDC, CPrintInfo* pInfo );
	virtual void OnEndPrinting( CDC* pDC, CPrintInfo* pInfo );
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CImgPrcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump( CDumpContext& dc ) const;
#endif

protected:

	// Generated message map functions
protected:
	//{{AFX_MSG(CImgPrcView)
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen2();
	afx_msg BOOL OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message );
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnRButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnMouseMove( UINT nFlags, CPoint point );
	afx_msg BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );
	afx_msg void OnRButtonDblClk( UINT nFlags, CPoint point );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnCreateMorphDialog();


	//自定义消息处理
	afx_msg LRESULT OnMorphology( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnUpdateBitmap( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnHistgramOperate( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnBinaryRegion( WPARAM wPram, LPARAM lParam );
	afx_msg LRESULT OnPolygon( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnConerDetect( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnMSER( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnhdlSmooth( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnColorConv( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnThresholdSeg( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnAdaptiveThreshold( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnIterativeThreshold( WPARAM wParam, LPARAM lParam );
	afx_msg	LRESULT OnStandardHoffTran( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnPPHT( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnHoughCircleTran( WPARAM wParam, LPARAM lParam );
	afx_msg void OnLastframe();
	afx_msg void OnNextframe();


private:
	CMorphDlg* m_pMorphDlg;
	CHistDlg* m_pHitogram;
	CBinaryRegion* m_pBinaryRegion;
	CPolygon* m_pPolygon;
	CConerDetector* m_pConer;
	CMSER* m_pMSER;
	CSiftDialog* m_pSiftDlg;
	SmoothDlg* m_pSmoothDlg;
	logdogDlg* m_pLogdogDlg;
	CColorConv* m_pColorConv;
	CThresholdSeg* m_pThresholdSeg;
	CAdaptiveThreshold* m_pAdaptiveThreshold;
	CIterativeThreshold* m_pIterativeThreshold;
	CStandardHoffTran* m_pStandardHoffTran;
	CPPHT* m_pPPHT;
	CHoughCircleTran* m_pHoughCircleTran;
	CDlgFtrTrack *m_pFtrTrack;

	//edge
	CSobelDlg* m_pSobelDlg;
	CRobertDlg* m_pRobertDlg;
	CCannyDlgEx* m_pCannyDlgEx;
	CLaplacianDlg* m_pLaplacianDlg;
	CGaborDlg* m_pGaborDlg;

	BOOL m_bTracking;
	int m_nImgShowID;

public:
	afx_msg void OnGrayHistogram();
	afx_msg void OnBGRHistogram();
	afx_msg void OnHorizonProjection();
	afx_msg void OnVerticalProjection();
	afx_msg void OnCreateHistDialog();
	afx_msg void OnCreateRegionDialog();
	afx_msg void OnCreatePolygonDialog();
	afx_msg void OnCreateCornerDialog();
	afx_msg void OnCreateMSERDialog();
	afx_msg void OnMenuSmooth();
protected:
	afx_msg LRESULT OnLoganddog( WPARAM wParam, LPARAM lParam );
public:
	afx_msg void OnMenuColorConv();
	afx_msg void OnMenuThresholdSeg();
	afx_msg void OnMenuAdaptiveThreshold();
	afx_msg void OnMenuIterativeThreshold();
	afx_msg void OnMenuStandardHoffTran();
	afx_msg void OnCreatePPHTDialog();
	afx_msg void OnCreateHoughCircleDialog();
	afx_msg void OnOpenCVSobel();
	afx_msg void OnRobertOperator();
	afx_msg void OnOpenCVCanny();
protected:
	afx_msg LRESULT OnOpencvsobel( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnRobert( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnOpencvcanny( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnCancelopera( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnOpencvlaplacian( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnGaborMessoperator( WPARAM wParam, LPARAM lParam );
public:
	afx_msg void OnLaplacian();
	afx_msg void OnGaborOperator();
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnLoGandDoG();
	afx_msg void OnSetbg();
	afx_msg void OnTrafficBody();
	afx_msg void OnTrackfeature();
	afx_msg LRESULT OnFeatureTrack( WPARAM wParam, LPARAM lParam );
	afx_msg void OnTrafficBackward();
	afx_msg void OnTrafficForward();
	afx_msg void OnTrafficInittarget();
	afx_msg void OnTrafficShowall();
	afx_msg void OnSignRecognize();
	virtual BOOL PreTranslateMessage( MSG* pMsg );
	afx_msg void OnSignNextimg();
	afx_msg void OnSignLastimg();
	afx_msg void OnSignDelcur();
	afx_msg void OnSignSave();
	afx_msg void OnSaveBack();
	afx_msg void OnSignLoad();

	afx_msg void OnSignShowall();
	afx_msg void OnSignProcess();
	afx_msg void OnSignAutofindsig();
	afx_msg void OnSignFindbackpattern();
	afx_msg void OnSignFusiontexture();
	afx_msg void OnSignFilter();
	afx_msg void OnButtontrackset();
	afx_msg void OnButtontrack();
};
#ifndef _DEBUG  // debug version in 系统View.cpp
inline CImgPrcDoc* CImgPrcView::GetDocument()
{
	return (CImgPrcDoc*) m_pDocument;
}
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEW_H__6B8B9C31_49BE_4AFA_81F5_80A16794BC0C__INCLUDED_)

