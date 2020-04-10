// ϵͳView.cpp : implementation of the CImgPrcView class
//
#include "stdafx.h"
#include "ImgPrc.h"
#include "ImgPrcDoc.h"
#include "ImgPrcView.h"
#include "ImageProcess.h"
#include "DlgFiles.h"
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <opencv2\opencv.hpp>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "ZXDib.h"
#include <Memory>
#include <Mmsystem.h>
#include <math.h>
#include "DlgMosaic.h"
#include "Track.h"

int     g_nCornerType2[4];
Point2d g_dCorner2[4];

int     g_nCornerType[4];
Point2d g_dCorner[4];
int  g_nCornerID = 0;

Point2d g_dCornerAffine[4];

CTrack tracker;

CStringArray g_strFileNames;
int g_nFileID = 0;
BOOL g_bShowAll = FALSE;

CMap<CString, LPCTSTR, Point2d, Point2d&> g_mapFile2Coner0;
CMap<CString, LPCTSTR, Point2d, Point2d&> g_mapFile2Coner1;
CMap<CString, LPCTSTR, Point2d, Point2d&> g_mapFile2Coner2;
CMap<CString, LPCTSTR, Point2d, Point2d&> g_mapFile2Coner3;

CMap<CString, LPCTSTR, int, int&> g_mapFile2Type0;
CMap<CString, LPCTSTR, int, int&> g_mapFile2Type1;
CMap<CString, LPCTSTR, int, int&> g_mapFile2Type2;
CMap<CString, LPCTSTR, int, int&> g_mapFile2Type3;

CMap<CString, LPCTSTR, BYTE, BYTE&> g_mapFile2BackR;
CMap<CString, LPCTSTR, BYTE, BYTE&> g_mapFile2BackG;
CMap<CString, LPCTSTR, BYTE, BYTE&> g_mapFile2BackB;


//#include "SurfDetector.h"
#include "Harris.hpp"
#include "Target.h"
#include "Buffer.h"


#include <opencv2\features2d.hpp>
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2/tracking.hpp>  

//#define min(x,y) ((x)<(y)?(x):(y))
//#define max(x,y) ((x)>(y)?(x):(y))

using namespace cv;
using namespace std;
using namespace cv::ml;


CZXDib dib;

int g_nBackPos = -1;
CTarget* g_pTarget = NULL;

CBuffer g_buff;
CRect g_rctSel;

//Mat g_maskfixed;
Rect2d g_rctFixed = { 0,0,172,240 };
Point g_ptFixed[] = { Point( 41,67 ),Point( 19,109 ),Point( 45,191 ),Point( 747,483 ),Point( 711,475 ) };
int g_nFixedCnt = 5;

cv::Rect rectROI = { 0, 0, 960, 540 };// { 460,200,200,220};// { 1406, 270, 352, 646 };//{ 976,220,352,546 };//{ 991,159,352,546 };//  1432, 882};
const int STARTFRM = 80670;// 2350; // 94251;//350;// 89563; //  90745;// 89700;//  300;// 133244;// 44190; //
const double ROTATE = 0;// -60;
Ptr<BackgroundSubtractorMOG2> mog;
HOGDescriptor bodyHog;
CascadeClassifier *cascade;

std::vector<RECT> g_rctObjs;
std::vector<Rect> g_rctMogObjs;
std::vector<int> g_rctMogVald;

extern std::vector<Ptr<CTarget>> g_Targets;

COLORREF g_crObjs[] = { RGB( 255,0,0 ), RGB( 0,255,0 ), RGB( 0,0,255 ), RGB( 255,255,0 ), RGB( 0,255,255 ) };
string g_strTracker[] = { "KCF","MEDIANFLOW","TLD","BOOSTING",  "MIL" };
int g_nTrackerCnt = 1;

std::vector<Ptr<Tracker>> g_trackers;
std::vector<Rect2d> g_rctTrckObjs;

Mat g_refImg;

Mat g_mapMatrix;
Mat g_roadmsku;
Mat g_roadmskd;
Rect2d g_rctTrck;
Point g_ptMidLine[2];
double g_dMLineAngle = 0;

std::vector<Point2f> prevPts, currPts;

Mat g_imObjs;
Mat g_templ;
Rect2d g_rctMatch;

DWORD g_dwStartTime = 0;


bool bFirst = true;
int  nPos = 0;
bool bRecapture = true;
int  nFrame = 0;
int bProcessFirst = 1;

#ifdef _DEBUG 
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEBUGMODE 1
template<class T>
inline T Min( T a, T b )
{
	return a < b ? a : b;
}

template<class T>
inline T Max( T a, T b )
{
	return a > b ? a : b;
}


CImgPrcView* g_pView;
CImgPrcView* g_cpView;

IMPLEMENT_DYNCREATE( CImgPrcView, CScrollView )

BEGIN_MESSAGE_MAP( CImgPrcView, CScrollView )
	//{{AFX_MSG_MAP(CImgPrcView)
	ON_COMMAND( ID_FILE_OPEN, OnFileOpen )
	ON_COMMAND( ID_FILE_SAVE, OnFileSave )
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND( ID_FILE_PRINT, CScrollView::OnFilePrint )
	ON_COMMAND( ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint )
	ON_COMMAND( ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview )
	ON_COMMAND( ID_FILE_OPEN2, OnFileOpen2 )
	ON_COMMAND( ID_32874, &CImgPrcView::OnCreateMorphDialog )
	ON_WM_SETCURSOR()
	//�Զ�����Ϣ
	ON_MESSAGE( WM_MORPHOLOGY, &CImgPrcView::OnMorphology )
	ON_MESSAGE( WM_UDDATE_BITMAP, &CImgPrcView::OnUpdateBitmap )
	ON_MESSAGE( WM_HISTOGRAM, &CImgPrcView::OnHistgramOperate )
	ON_MESSAGE( WM_BINARYREGION, &CImgPrcView::OnBinaryRegion )
	ON_MESSAGE( WM_POLYGON, &CImgPrcView::OnPolygon )
	ON_MESSAGE( WM_CONERDETECTOR, &CImgPrcView::OnConerDetect )
	ON_MESSAGE( WM_MSER, &CImgPrcView::OnMSER )
	ON_MESSAGE( WM_SMOOTH, &CImgPrcView::OnhdlSmooth )
	ON_MESSAGE( WM_COLORCVT, &CImgPrcView::OnColorConv )
	ON_MESSAGE( WM_THRESHOLDSEG, &CImgPrcView::OnThresholdSeg )
	ON_MESSAGE( WM_ADAPTIVETHRESHOLD, &CImgPrcView::OnAdaptiveThreshold )
	ON_MESSAGE( WM_ITERATIVETHRESHOLD, &CImgPrcView::OnIterativeThreshold )
	ON_MESSAGE( WM_STANDARDHOFFTRAN, &CImgPrcView::OnStandardHoffTran )
	ON_MESSAGE( WM_PPHT, &CImgPrcView::OnPPHT )
	ON_MESSAGE( WM_HOUGHCIRCLETRAN, &CImgPrcView::OnHoughCircleTran )

	//
	ON_COMMAND( ID_32877, &CImgPrcView::OnGrayHistogram )
	ON_COMMAND( ID_32878, &CImgPrcView::OnBGRHistogram )
	ON_COMMAND( ID_32879, &CImgPrcView::OnHorizonProjection )
	ON_COMMAND( ID_32880, &CImgPrcView::OnVerticalProjection )
	ON_COMMAND( ID_32882, &CImgPrcView::OnCreateHistDialog )
	ON_COMMAND( ID_32883, &CImgPrcView::OnCreateRegionDialog )
	ON_COMMAND( ID_32886, &CImgPrcView::OnCreatePolygonDialog )
	ON_COMMAND( ID_32887, &CImgPrcView::OnCreateCornerDialog )
	ON_COMMAND( ID_32888, &CImgPrcView::OnCreateMSERDialog )
	ON_COMMAND( ID_32902, &CImgPrcView::OnMenuSmooth )
	//ON_COMMAND(ID_32903, &CImgPrcView::OnLOGandDOG)
	ON_MESSAGE( WM_LOGANDDOG, &CImgPrcView::OnLoganddog )
	ON_COMMAND( ID_32905, &CImgPrcView::OnMenuColorConv )
	ON_COMMAND( ID_32906, &CImgPrcView::OnMenuThresholdSeg )
	ON_COMMAND( ID_32907, &CImgPrcView::OnMenuAdaptiveThreshold )
	ON_COMMAND( ID_32908, &CImgPrcView::OnMenuIterativeThreshold )
	ON_COMMAND( ID_32909, &CImgPrcView::OnMenuStandardHoffTran )
	ON_COMMAND( ID_32910, &CImgPrcView::OnCreatePPHTDialog )
	ON_COMMAND( ID_32911, &CImgPrcView::OnCreateHoughCircleDialog )

	//edge
	ON_COMMAND( ID_Menu32896, &CImgPrcView::OnOpenCVSobel )
	ON_COMMAND( ID_32897, &CImgPrcView::OnRobertOperator )
	ON_COMMAND( ID_32912, &CImgPrcView::OnOpenCVCanny )
	ON_MESSAGE( WM_OPENCVSOBEL, &CImgPrcView::OnOpencvsobel )
	ON_MESSAGE( WM_ROBERT, &CImgPrcView::OnRobert )
	ON_MESSAGE( WM_OPENCVCANNY, &CImgPrcView::OnOpencvcanny )
	ON_MESSAGE( WM_CANCELOPERA, &CImgPrcView::OnCancelopera )
	ON_MESSAGE( WM_OPENCVLAPLACIAN, &CImgPrcView::OnOpencvlaplacian )
	ON_COMMAND( ID_32900, &CImgPrcView::OnLaplacian )
	ON_COMMAND( ID_32901, &CImgPrcView::OnGaborOperator )
	ON_MESSAGE( WM_GABOROPERATOR, &CImgPrcView::OnGaborMessoperator )
	ON_WM_SIZE()
	ON_COMMAND( ID_32915, &CImgPrcView::OnLoGandDoG )
	ON_COMMAND( ID_LASTFRAME, &CImgPrcView::OnLastframe )
	ON_COMMAND( ID_NEXTFRAME, &CImgPrcView::OnNextframe )
	ON_COMMAND( ID_SETBG, &CImgPrcView::OnSetbg )
	ON_COMMAND( ID_TRAFFIC_BODY, &CImgPrcView::OnTrafficBody )
	ON_COMMAND( ID_TRACKFEATURE, &CImgPrcView::OnTrackfeature )
	ON_MESSAGE( WM_FEATURETRACK, &CImgPrcView::OnFeatureTrack )
	ON_COMMAND( ID_TRAFFIC_BACKWARD, &CImgPrcView::OnTrafficBackward )
	ON_COMMAND( ID_TRAFFIC_FORWARD, &CImgPrcView::OnTrafficForward )
	ON_COMMAND( ID_TRAFFIC_INITTARGET, &CImgPrcView::OnTrafficInittarget )
	ON_COMMAND( ID_TRAFFIC_SHOWALL, &CImgPrcView::OnTrafficShowall )
	ON_COMMAND( ID_SIGN_RECOGNIZE, &CImgPrcView::OnSignRecognize )
	ON_COMMAND( ID_SIGN_NEXTIMG, &CImgPrcView::OnSignNextimg )
	ON_COMMAND( ID_SIGN_LASTIMG, &CImgPrcView::OnSignLastimg )
	ON_COMMAND( ID_SIGN_DELCUR, &CImgPrcView::OnSignDelcur )
	ON_COMMAND( ID_SIGN_SAVE, &CImgPrcView::OnSignSave )
	ON_COMMAND( ID_SIGN_SAVEBACKPATTERN, &CImgPrcView::OnSaveBack )
	ON_COMMAND( ID_SIGN_LOAD, &CImgPrcView::OnSignLoad )
	ON_COMMAND( ID_SIGN_SHOWALL, &CImgPrcView::OnSignShowall )
	ON_COMMAND( ID_SIGN_PROCESS, &CImgPrcView::OnSignProcess )
	ON_COMMAND( ID_SIGN_AUTOFINDSIG, &CImgPrcView::OnSignAutofindsig )
	ON_COMMAND( ID_SIGN_FINDBACKPATTERN, &CImgPrcView::OnSignFindbackpattern )
	ON_COMMAND( ID_SIGN_FUSIONTEXTURE, &CImgPrcView::OnSignFusiontexture )
	ON_COMMAND( ID_SIGN_FILTER, &CImgPrcView::OnSignFilter )
	ON_COMMAND( ID_BUTTONTRACKSET, &CImgPrcView::OnButtontrackset )
	ON_COMMAND( ID_BUTTONTRACK, &CImgPrcView::OnButtontrack )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImgPrcView construction/destruction
HANDLE m_hSemaphore = NULL;


int compare00( const void *elem1, const void *elem2 )
{
	return( ( (CString*) elem2 )->CompareNoCase( *( (CString*) elem1 ) )*-1 );
}


CImgPrcView::CImgPrcView()
{
	//m_hSemaphore = CreateSemaphore( NULL, 1, 1, NULL );

	// TODO: add construction code here
	m_pBitmap = NULL;
	m_pBitmap2 = NULL;

	m_nWidth2 = m_nHeight2 = 0;
	m_nWidth = m_nHeight = 0;
	m_pBitmap = NULL;


	m_ppItrImage = NULL;
	m_pnItrImgHeight = NULL;
	m_pnItrImgWidth = NULL;
	m_pItrImageFlag = NULL;
	m_nItrImageCount = 0;
	m_pstrItrMark = NULL;
	m_pBitmapDisplay = NULL;
	m_pBitmapInfo = NULL;
	m_strTitle = "";

	m_pBmpProc = NULL;
	m_nHeightProc = m_nWidthProc = 0;
	m_flagProc = 8;
	m_isBinary = FALSE;

	//��������ָ���ʼ��
	m_pMorphDlg = NULL;
	m_pHitogram = NULL;
	m_pBinaryRegion = NULL;
	m_pPolygon = NULL;
	m_pConer = NULL;
	m_pMSER = NULL;
	m_pSiftDlg = NULL;
	m_pSmoothDlg = NULL;
	//OnFileOpen();
	m_pLogdogDlg = NULL;
	m_pColorConv = NULL;
	m_pThresholdSeg = NULL;
	m_pIterativeThreshold = NULL;
	m_pStandardHoffTran = NULL;
	m_pPPHT = NULL;
	m_pHoughCircleTran = NULL;
	m_pFtrTrack = NULL;

	//edge
	m_pSobelDlg = NULL;
	m_pRobertDlg = NULL;
	m_pCannyDlgEx = NULL;
	m_pLaplacianDlg = NULL;
	m_pGaborDlg = NULL;

	m_rectShow = CFRect( 0, 0, 0, 0 );
	m_bMouseDown = FALSE;
	m_dScaleRatio = 1;

	m_pVideo = new VideoCapture;

	m_bTracking = FALSE;
	m_bSelecting = FALSE;
	m_nImgShowID = 0;

	g_pView = this;
	g_mapMatrix = Mat::eye( 2, 3, CV_8U );
	m_bReFocus = FALSE;

	m_nFileSource = 0;

	g_dCorner[0] = g_dCorner[1] = g_dCorner[2] = g_dCorner[3] = Point2d( -1, -1 );
	g_nCornerType[0] = g_nCornerType[1] = g_nCornerType[2] = g_nCornerType[3] = 0;
}

CImgPrcView::~CImgPrcView()
{
	if ( m_pBitmapInfo ) delete m_pBitmapInfo;
	m_pBitmapInfo = nullptr;
	FreeInterResultImage();
	if ( m_pBitmap ) delete[]m_pBitmap;
	m_pBitmap = nullptr;
	if ( m_pBitmap2 ) delete[]m_pBitmap2;
	m_pBitmap2 = nullptr;
	if ( m_pMorphDlg ) delete m_pMorphDlg;
	m_pMorphDlg = nullptr;
	if ( m_pBmpProc )	delete[] m_pBmpProc;
	m_pBmpProc = nullptr;
	if ( m_pHitogram ) delete m_pHitogram;
	m_pHitogram = nullptr;
	if ( m_pBinaryRegion ) delete m_pBinaryRegion;
	m_pBinaryRegion = nullptr;
	if ( m_pPolygon ) delete m_pPolygon;
	m_pPolygon = nullptr;
	if ( m_pConer ) delete m_pConer;
	m_pConer = nullptr;
	if ( m_pMSER ) delete m_pMSER;
	m_pMSER = nullptr;
	if ( m_pSiftDlg ) delete m_pSiftDlg;
	m_pSiftDlg = nullptr;
	if ( m_pColorConv ) delete m_pColorConv;
	m_pColorConv = nullptr;
	if ( m_pThresholdSeg ) delete m_pThresholdSeg;
	m_pThresholdSeg = nullptr;
	if ( m_pIterativeThreshold ) delete m_pIterativeThreshold;
	m_pIterativeThreshold = nullptr;
	if ( m_pStandardHoffTran ) delete m_pStandardHoffTran;
	m_pStandardHoffTran = nullptr;
	if ( m_pPPHT ) delete m_pPPHT;
	m_pPPHT = nullptr;
	if ( m_pHoughCircleTran ) delete m_pHoughCircleTran;
	m_pHoughCircleTran = nullptr;

	delete m_pFtrTrack;

	//edge
	if ( m_pSobelDlg ) delete m_pSobelDlg;
	m_pSobelDlg = NULL;
	if ( m_pRobertDlg ) delete m_pRobertDlg;
	m_pRobertDlg = NULL;
	if ( m_pCannyDlgEx ) delete m_pCannyDlgEx;
	m_pCannyDlgEx = NULL;
	if ( m_pLaplacianDlg ) delete m_pLaplacianDlg;
	m_pLaplacianDlg = NULL;
	if ( m_pGaborDlg ) delete m_pGaborDlg;
	m_pGaborDlg = NULL;

	delete m_pVideo;
}

void FilterH( BYTE* pBitmap, int nH, int nW, int nHSize, int nVSize, int*& pRes )
{
	int i, j, m, n;

	int* pInt = NULL;
	dib.GetInteImg( pBitmap, nH, nW, pInt );

	if ( pRes ) delete pRes;
	pRes = new int[nH*nW];
	memset( pRes, 0, sizeof( int )*nH*nW );

	for ( i = nVSize + 1; i < nH - nVSize; i++ )
		for ( j = nHSize + 1; j < nW - nHSize; j++ )
		{
			int nTemp1 = pInt[( i + nVSize )*nW + j + nHSize] + pInt[( i - nVSize )*nW + j] - pInt[( i - nVSize )*nW + j + nHSize] - pInt[( i + nVSize )*nW + j];
			int nTemp2 = pInt[( i + nVSize )*nW + j - 1] + pInt[( i - nVSize )*nW + j - nHSize - 1] - pInt[( i - nVSize )*nW + j - 1] - pInt[( i + nVSize )*nW + j - nHSize - 1];
			int nTemp3 = pInt[( i + nVSize )*nW + j + nHSize + 1] + pInt[( i - nVSize )*nW + j + 1] - pInt[( i - nVSize )*nW + j + nHSize + 1] - pInt[( i + nVSize )*nW + j + 1];
			int nTemp4 = pInt[( i + nVSize )*nW + j] + pInt[( i - nVSize )*nW + j - nHSize] - pInt[( i - nVSize )*nW + j] - pInt[( i + nVSize )*nW + j - nHSize];

			if ( j > nW / 2 )
				pRes[i*nW + j] = nTemp1 > nTemp2 ? nTemp1 - nTemp2 : 0; //nTemp3 > nTemp4 ? nTemp3 - nTemp4 : 0;
			else
				pRes[i*nW + j] = nTemp2 > nTemp1 ? nTemp2 - nTemp1 : 0;
		}

	delete pInt;
}
void FilterH( BYTE* pBitmap, int nH, int nW, int nHSize, int nVSize, int*& pRes, int nLf )
{
	int i, j, m, n;

	int* pInt = NULL;
	dib.GetInteImg( pBitmap, nH, nW, pInt );

	if ( pRes ) delete pRes;
	pRes = new int[nH*nW];
	memset( pRes, 0, sizeof( int )*nH*nW );

	for ( i = nVSize + 1; i < nH - nVSize; i++ )
		for ( j = nHSize + 1; j < nW - nHSize; j++ )
		{
			int nTemp1 = pInt[( i + nVSize )*nW + j + nHSize] + pInt[( i - nVSize )*nW + j] - pInt[( i - nVSize )*nW + j + nHSize] - pInt[( i + nVSize )*nW + j];
			int nTemp2 = pInt[( i + nVSize )*nW + j - 1] + pInt[( i - nVSize )*nW + j - nHSize - 1] - pInt[( i - nVSize )*nW + j - 1] - pInt[( i + nVSize )*nW + j - nHSize - 1];
			int nTemp3 = pInt[( i + nVSize )*nW + j + nHSize + 1] + pInt[( i - nVSize )*nW + j + 1] - pInt[( i - nVSize )*nW + j + nHSize + 1] - pInt[( i + nVSize )*nW + j + 1];
			int nTemp4 = pInt[( i + nVSize )*nW + j] + pInt[( i - nVSize )*nW + j - nHSize] - pInt[( i - nVSize )*nW + j] - pInt[( i + nVSize )*nW + j - nHSize];

			if ( nLf )
				pRes[i*nW + j] = nTemp2 > nTemp1 ? nTemp2 - nTemp1 : 0; //nTemp3 > nTemp4 ? nTemp3 - nTemp4 : 0;
			else
				pRes[i*nW + j] = nTemp1 > nTemp2 ? nTemp1 - nTemp2 : 0;
		}

	delete pInt;
}
void FilterHC2( BYTE* pBitmap, int nH, int nW, int nHSize, int nVSize, float*& pnRes, int nLf )
{
	BYTE* pTemp = new BYTE[nH*nW];

	delete pnRes;
	pnRes = new float[nH*nW];
	memset( pnRes, 0, nH*nW * sizeof( float ) );

	for ( int i = 0; i < 3; i++ )
	{
		for ( int j = 0; j < nH*nW; j++ )
			pTemp[j] = pBitmap[j * 3 + i];

		int* pTemp1 = NULL;
		FilterH( pTemp, nH, nW, nHSize, nVSize, pTemp1, nLf );

		for ( int j = 0; j < nH*nW; j++ )
			pnRes[j] += pTemp1[j];

		delete pTemp1;
	}
	delete pTemp;
}
void FilterHC( BYTE* pBitmap, int nH, int nW, int nHSize, int nVSize, BYTE*& pRes )
{
	BYTE* pTemp = new BYTE[nH*nW];

	int* pnRes = new int[nH*nW];
	memset( pnRes, 0, nH*nW * sizeof( int ) );

	for ( int i = 0; i < 3; i++ )
	{
		for ( int j = 0; j < nH*nW; j++ )
			pTemp[j] = pBitmap[j * 3 + i];

		int* pTemp1 = NULL;
		FilterH( pTemp, nH, nW, nHSize, nVSize, pTemp1 );

		for ( int j = 0; j < nH*nW; j++ )
			pnRes[j] += pTemp1[j];

		delete pTemp1;
	}

	dib.RegularToGrayLevel( pnRes, nH, nW, pRes );

	delete pTemp;
	delete pnRes;
}

void FilterV( BYTE* pBitmap, int nH, int nW, int nHSize, int nVSize, int*& pRes )
{
	int i, j, m, n;

	int* pInt = NULL;
	dib.GetInteImg( pBitmap, nH, nW, pInt );

	if ( pRes ) delete pRes;
	pRes = new int[nH*nW];
	memset( pRes, 0, sizeof( int )*nH*nW );

	for ( i = nVSize + 1; i < nH - nVSize; i++ )
		for ( j = nHSize + 1; j < nW - nHSize; j++ )
		{
			int nTemp1 = pInt[( i + nVSize - 1 )*nW + j + nHSize] + pInt[( i - 1 )*nW + j - nHSize] - pInt[( i - 1 )*nW + j + nHSize] - pInt[( i + nVSize - 1 )*nW + j - nHSize];
			int nTemp2 = pInt[( i - 1 )*nW + j + nHSize] + pInt[( i - nVSize - 1 )*nW + j - nHSize] - pInt[( i - nVSize - 1 )*nW + j + nHSize] - pInt[( i - 1 )*nW + j - nHSize];
			int nTemp3 = pInt[( i + nVSize )*nW + j + nHSize] + pInt[(i) *nW + j - nHSize] - pInt[(i) *nW + j + nHSize] - pInt[( i + nVSize )*nW + j - nHSize];
			int nTemp4 = pInt[(i) *nW + j + nHSize] + pInt[( i - nVSize )*nW + j - nHSize] - pInt[( i - nVSize )*nW + j + nHSize] - pInt[(i) *nW + j - nHSize];

			if ( i > nH / 2 )
				pRes[i*nW + j] = nTemp3 > nTemp4 ? nTemp3 - nTemp4 : 0; //nTemp1 > nTemp2 ? nTemp1 - nTemp2 : 0; //
			else
				pRes[i*nW + j] = nTemp4 > nTemp3 ? nTemp4 - nTemp3 : 0; //nTemp2 > nTemp1 ? nTemp2 - nTemp1 : 0;
		}

	delete pInt;
}
void FilterV( BYTE* pBitmap, int nH, int nW, int nHSize, int nVSize, int*& pRes, int nUp )
{
	int i, j, m, n;

	int* pInt = NULL;
	dib.GetInteImg( pBitmap, nH, nW, pInt );

	if ( pRes ) delete pRes;
	pRes = new int[nH*nW];
	memset( pRes, 0, sizeof( int )*nH*nW );

	for ( i = nVSize + 1; i < nH - nVSize; i++ )
		for ( j = nHSize + 1; j < nW - nHSize; j++ )
		{
			int nTemp1 = pInt[( i + nVSize - 1 )*nW + j + nHSize] + pInt[( i - 1 )*nW + j - nHSize] - pInt[( i - 1 )*nW + j + nHSize] - pInt[( i + nVSize - 1 )*nW + j - nHSize];
			int nTemp2 = pInt[( i - 1 )*nW + j + nHSize] + pInt[( i - nVSize - 1 )*nW + j - nHSize] - pInt[( i - nVSize - 1 )*nW + j + nHSize] - pInt[( i - 1 )*nW + j - nHSize];
			int nTemp3 = pInt[( i + nVSize )*nW + j + nHSize] + pInt[(i) *nW + j - nHSize] - pInt[(i) *nW + j + nHSize] - pInt[( i + nVSize )*nW + j - nHSize];
			int nTemp4 = pInt[(i) *nW + j + nHSize] + pInt[( i - nVSize )*nW + j - nHSize] - pInt[( i - nVSize )*nW + j + nHSize] - pInt[(i) *nW + j - nHSize];

			if ( nUp )
				pRes[i*nW + j] = nTemp3 > nTemp4 ? nTemp3 - nTemp4 : 0; //nTemp1 > nTemp2 ? nTemp1 - nTemp2 : 0; //
			else
				pRes[i*nW + j] = nTemp4 > nTemp3 ? nTemp4 - nTemp3 : 0; //nTemp2 > nTemp1 ? nTemp2 - nTemp1 : 0;
		}

	delete pInt;
}

void FilterVC( BYTE* pBitmap, int nH, int nW, int nHSize, int nVSize, BYTE*& pRes )
{
	BYTE* pTemp = new BYTE[nH*nW];

	int* pnRes = new int[nH*nW];
	memset( pnRes, 0, nH*nW * sizeof( int ) );

	for ( int i = 0; i < 3; i++ )
	{
		for ( int j = 0; j < nH*nW; j++ )
			pTemp[j] = pBitmap[j * 3 + i];

		int* pTemp1 = NULL;
		FilterV( pTemp, nH, nW, nHSize, nVSize, pTemp1 );

		for ( int j = 0; j < nH*nW; j++ )
			pnRes[j] += pTemp1[j];

		delete pTemp1;
	}

	dib.RegularToGrayLevel( pnRes, nH, nW, pRes );

	delete pTemp;
	delete pnRes;
}
void FilterVC2( BYTE* pBitmap, int nH, int nW, int nHSize, int nVSize, float*& pnRes, int nUp )
{
	BYTE* pTemp = new BYTE[nH*nW];

	delete pnRes;
	pnRes = new float[nH*nW * sizeof( float )];
	memset( pnRes, 0, nH*nW * sizeof( float ) );

	for ( int i = 0; i < 3; i++ )
	{
		for ( int j = 0; j < nH*nW; j++ )
			pTemp[j] = pBitmap[j * 3 + i];

		int* pTemp1 = NULL;
		FilterV( pTemp, nH, nW, nHSize, nVSize, pTemp1, nUp );

		for ( int j = 0; j < nH*nW; j++ )
			pnRes[j] += pTemp1[j];

		delete pTemp1;
	}
	delete pTemp;
}

void CImgPrcView::LoadMappingData( CString& strFileName, Mat& img )
{
	CStdioFile File;
	if ( !File.Open( strFileName, CFile::modeRead ) )
		return;

	vector<Point2f> ptImg;
	vector<Point2f> ptGPS;
	CString strTemp;
	while ( File.ReadString( strTemp ) )
	{
		strTemp += ";;;;";

		strTemp = strTemp.Mid( strTemp.Find( ";" ) + 1 );
		CString strData = strTemp.Left( strTemp.Find( "," ) );
		strTemp = strTemp.Mid( strTemp.Find( "," ) + 1 );
		double x = atof( strData );
		strData = strTemp.Left( strTemp.Find( ";" ) );
		double y = atof( strData );
		strTemp = strTemp.Mid( strTemp.Find( ";" ) + 1 );
		ptImg.push_back( Point2f( x, y ) );
		strData = strTemp.Left( strTemp.Find( "," ) );
		x = atof( strData );
		strTemp = strTemp.Mid( strTemp.Find( "," ) + 1 );
		strData = strTemp.Left( strTemp.Find( ";" ) );
		y = atof( strData );
		ptGPS.push_back( Point2f( x, y ) );
	}
	File.Close();

	Mat h = findHomography( ptImg, ptGPS, CV_LMEDS );
	Mat img2;
	Size size( 2088, 1110 );
	warpPerspective( img, img2, h, size );

	for ( int i = 0; i < ptGPS.size(); i++ )
	{
		Point2f pt1 = ptGPS[i];
		line( img2, Point2f( pt1.x - 20, pt1.y ), Point2f( pt1.x + 21, pt1.y ), Scalar( 0, 0, 255 ), 2, 8 );
		line( img2, Point2f( pt1.x, pt1.y - 20 ), Point2f( pt1.x, pt1.y + 21 ), Scalar( 0, 0, 255 ), 2, 8 );
	}
	SetInterResultImage( 1, img2, "23" );
	imwrite( "f:\\1.bmp", img2 );
}

BOOL CImgPrcView::PreCreateWindow( CREATESTRUCT& cs )
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow( cs );
}

/////////////////////////////////////////////////////////////////////////////
// CImgPrcView drawing
void CImgPrcView::OnDraw( CDC* pDC )
{
	FocusMovement();

	// ��ȡ��ǰ�ͻ�����С
	CRect rectClient;
	GetClientRect( &rectClient );

	int nClientWidth = rectClient.Width();
	int nClientHeight = rectClient.Height();

	// �����ڲ��ռ�, ���ڴ洢λͼ, ˫����
	CDC MemDC;
	MemDC.CreateCompatibleDC( pDC );
	MemDC.SetStretchBltMode( COLORONCOLOR );

	// ����һ����ͻ�����ͬ����С��λͼ
	CBitmap bmp;
	bmp.CreateCompatibleBitmap( pDC, nClientWidth, nClientHeight );

	LOGFONT logFont;
	ZeroMemory( (void*) &logFont, sizeof( logFont ) );
	strcpy( logFont.lfFaceName, "Arial" );
	logFont.lfHeight = -18;
	logFont.lfWeight = 400;
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfOutPrecision = 3;
	logFont.lfClipPrecision = 2;
	logFont.lfQuality = 1;
	logFont.lfPitchAndFamily = 0;

	MemDC.SetTextColor( RGB( 32, 32, 32 ) );

	CFont font;
	font.CreateFontIndirect( &logFont );
	MemDC.SelectObject( font );


	// ѡ��DC
	CBitmap* pOldBm = MemDC.SelectObject( &bmp );
	CSize szClient = CSize( nClientWidth, nClientHeight );

	CBrush brush;
	brush.CreateSolidBrush( RGB( 255, 255, 255 ) );
	MemDC.SelectObject( brush );
	MemDC.FillRect( rectClient, &brush );

	MemDC.SetBkMode( TRANSPARENT );

	CRect rectDoc;
	CalTotalRect( pDC, rectDoc );
	double dRatio = nClientWidth / m_rectShow.Width();
	//double dRHeight = nClientHeight/ m_rectShow.Height();


	CRect rectImg( 0, 0, m_nWidth, m_nHeight );
	CRect rctSrc, rctDest;
	if ( m_pBitmap )
	{
		GetSrcDest( rectImg, rctSrc, rctDest );

		Mat src( m_nHeight, m_nWidth, m_flag == 8 ? CV_8UC1 : CV_8UC3, m_pBitmap );
		Mat img = src.clone();

		Draw( &MemDC, img.data, m_nHeight, m_nWidth, m_flag, rctSrc, rctDest );

		if ( ( rctSrc.right - rctSrc.left ) * ( rctSrc.bottom - rctSrc.top ) > 1 )
		{
			strcpy( logFont.lfFaceName, "Arial" );
			logFont.lfHeight = -18;
			logFont.lfWeight = 400;

			MemDC.SetTextColor( RGB( 32, 32, 32 ) );

			CFont font1;
			font1.CreateFontIndirect( &logFont );
			MemDC.SelectObject( font1 );

			CPen pen1( 0, 1, RGB( 255, 0, 0 ) );
			CPen pen2( 0, 2, RGB( 0, 0, 255 ) );
			CPen pen3( 0, 1, RGB( 100, 50, 55 ) );

			double dRatio = 2.0 * ( rctDest.right - rctDest.left ) / ( rctSrc.right - rctSrc.left );
			dRatio = MIN( dRatio, 32 );


			if ( g_bShowAll )
			{
				Point2d pt[4];
				int n[4];
				CString strFile;

				POSITION pos0 = g_mapFile2Coner0.GetStartPosition();
				POSITION pos1 = g_mapFile2Coner1.GetStartPosition();
				POSITION pos2 = g_mapFile2Coner2.GetStartPosition();
				POSITION pos3 = g_mapFile2Coner3.GetStartPosition();

				POSITION ps0 = g_mapFile2Type0.GetStartPosition();
				POSITION ps1 = g_mapFile2Type1.GetStartPosition();
				POSITION ps2 = g_mapFile2Type2.GetStartPosition();
				POSITION ps3 = g_mapFile2Type3.GetStartPosition();
				while ( pos0 )
				{
					g_mapFile2Coner0.GetNextAssoc( pos0, strFile, pt[0] );
					g_mapFile2Coner1.GetNextAssoc( pos1, strFile, pt[1] );
					g_mapFile2Coner2.GetNextAssoc( pos2, strFile, pt[2] );
					g_mapFile2Coner3.GetNextAssoc( pos3, strFile, pt[3] );

					g_mapFile2Type0.GetNextAssoc( ps0, strFile, n[0] );
					g_mapFile2Type1.GetNextAssoc( ps1, strFile, n[1] );
					g_mapFile2Type2.GetNextAssoc( ps2, strFile, n[2] );
					g_mapFile2Type3.GetNextAssoc( ps3, strFile, n[3] );

					for ( int i = 0; i < 4; i++ )
					{
						MemDC.SelectObject( &pen3 );
						int x0 = ( pt[i].x - rctSrc.left ) / ( rctSrc.right - rctSrc.left ) * ( rctDest.right - rctDest.left ) + rctDest.left;
						int y0 = ( pt[i].y - ( m_nHeight - rctSrc.bottom ) ) / ( rctSrc.bottom - rctSrc.top ) * ( rctDest.bottom - rctDest.top ) + rctDest.top;
						int x1 = ( pt[( i + 1 ) % 4].x - rctSrc.left ) / ( rctSrc.right - rctSrc.left ) * ( rctDest.right - rctDest.left ) + rctDest.left;
						int y1 = ( pt[( i + 1 ) % 4].y - ( m_nHeight - rctSrc.bottom ) ) / ( rctSrc.bottom - rctSrc.top ) * ( rctDest.bottom - rctDest.top ) + rctDest.top;

						MemDC.MoveTo( x0, y0 );
						MemDC.LineTo( x1, y1 );

						MemDC.SelectObject( n[i] ? &pen2 : &pen1 );
						MemDC.MoveTo( x0 - 3, y0 - 3 );
						MemDC.LineTo( x0 + 3, y0 - 3 );
						MemDC.LineTo( x0 + 3, y0 + 3 );
						MemDC.LineTo( x0 - 3, y0 + 3 );
						MemDC.LineTo( x0 - 3, y0 - 3 );
					}
				}
			}

			for ( int i = 0; i < 4; i++ )
			{
				MemDC.SelectObject( &pen1 );
				int x0 = ( g_dCorner[i].x - rctSrc.left ) / ( rctSrc.right - rctSrc.left ) * ( rctDest.right - rctDest.left ) + rctDest.left;
				int y0 = ( g_dCorner[i].y - ( m_nHeight - rctSrc.bottom ) ) / ( rctSrc.bottom - rctSrc.top ) * ( rctDest.bottom - rctDest.top ) + rctDest.top;
				int x1 = ( g_dCorner[( i + 1 ) % 4].x - rctSrc.left ) / ( rctSrc.right - rctSrc.left ) * ( rctDest.right - rctDest.left ) + rctDest.left;
				int y1 = ( g_dCorner[( i + 1 ) % 4].y - ( m_nHeight - rctSrc.bottom ) ) / ( rctSrc.bottom - rctSrc.top ) * ( rctDest.bottom - rctDest.top ) + rctDest.top;

				MemDC.MoveTo( x0, y0 );
				MemDC.LineTo( x1, y1 );

				MemDC.SelectObject( g_nCornerType[i] ? &pen2 : &pen1 );
				MemDC.MoveTo( x0 - 3, y0 - 3 );
				MemDC.LineTo( x0 + 3, y0 - 3 );
				MemDC.LineTo( x0 + 3, y0 + 3 );
				MemDC.LineTo( x0 - 3, y0 + 3 );
				MemDC.LineTo( x0 - 3, y0 - 3 );

				CString strTemp;
				strTemp.Format( "%ld", i );
				switch ( i )
				{
				case 0:	MemDC.TextOutA( x0 - dRatio - 9, y0 - dRatio - 18, strTemp );	break;
				case 1:	MemDC.TextOutA( x0 + dRatio, y0 - dRatio - 18, strTemp );	break;
				case 2: MemDC.TextOutA( x0 + dRatio, y0 + dRatio, strTemp );	break;
				case 3:	MemDC.TextOutA( x0 - dRatio - 9, y0 + dRatio, strTemp );	break;
				}
			}

			CPen pen4( 0, 1, RGB( 0, 255, 0 ) );
			MemDC.SelectObject( &pen4 );
			for ( int i = 0; i < 4; i++ )
			{
				int x0 = ( g_dCornerAffine[i].x - rctSrc.left ) / (double) ( rctSrc.right - rctSrc.left ) * ( rctDest.right - rctDest.left ) + rctDest.left;
				int y0 = ( g_dCornerAffine[i].y - ( m_nHeight - rctSrc.bottom ) ) / (double) ( rctSrc.bottom - rctSrc.top ) * ( rctDest.bottom - rctDest.top ) + rctDest.top;
				int x1 = ( g_dCornerAffine[( i + 1 ) % 4].x - rctSrc.left ) / (double) ( rctSrc.right - rctSrc.left ) * ( rctDest.right - rctDest.left ) + rctDest.left;
				int y1 = ( g_dCornerAffine[( i + 1 ) % 4].y - ( m_nHeight - rctSrc.bottom ) ) / (double) ( rctSrc.bottom - rctSrc.top ) * ( rctDest.bottom - rctDest.top ) + rctDest.top;
				MemDC.MoveTo( x0, y0 );
				MemDC.LineTo( x1, y1 );
			}
		}
		//Mat img1( m_nHeight, m_nWidth, CV_8UC3, m_pBitmap );
		//ellipse( img1, Point( 450, 450 ), Size( 400, 250 ), 0, 0, 360, Scalar( 0, 0, 225 ), 5, 8 );//绘制第一个椭圆，大椭圆，颜色为红色
		//ellipse( img1, Point( 650, 650 ), Size( 150, 150 ), 0, 180, 360, Scalar( 255, 255, 255 ),5,8 );
		//line( img1, Point( 0, 0 ), Point( 100, 500 ), Scalar( 255, 255, 255 ), 1 );
	}

	for ( int i = 0; i < m_nItrImageCount; i++ )
	{
		CalCurImgRect( &MemDC, i, rectImg );
		GetSrcDest( rectImg, rctSrc, rctDest );
		Draw( &MemDC, m_ppItrImage[i], m_pnItrImgHeight[i], m_pnItrImgWidth[i], m_pItrImageFlag[i], rctSrc, rctDest );

		CRect rectTxt = CRect( 0, 0, 100, 100 );
		MemDC.DrawText( m_pstrItrMark[i], &rectTxt, DT_CALCRECT );

		rectTxt.OffsetRect( rctDest.right + 3, ( rctDest.top + rctDest.bottom ) / 2 );
		MemDC.DrawText( m_pstrItrMark[i], &rectTxt, DT_VCENTER );
	}

	// ��ͼ
	pDC->BitBlt( 0, 0, nClientWidth, nClientHeight, &MemDC, 0, 0, SRCCOPY );

	MemDC.SelectObject( pOldBm );
	DeleteDC( MemDC );
}

void CImgPrcView::FocusMovement()
{
	if ( !m_bReFocus || !m_strFocusTitle.GetLength() )
		return;

	CRect rectImg;
	CRect rctSrc, rctDest;

	for ( int i = 0; i < m_nItrImageCount; i++ )
	{
		if ( m_pstrItrMark[i].Compare( m_strFocusTitle ) )
			continue;

		CalCurImgRect( NULL, i, rectImg );

		double x0 = m_dFocusX + rectImg.left;
		double y0 = m_dFocusY + rectImg.top;

		// ��ȡ��ǰ�ͻ�����С
		CRect rectClient;
		GetClientRect( &rectClient );

		double x = ( m_MousePoint.x / (double) rectClient.Width() ) * m_rectShow.Width() + m_rectShow.left;
		double y = ( m_MousePoint.y / (double) rectClient.Height() ) * m_rectShow.Height() + m_rectShow.top;

		m_rectShow.OffsetRect( x0 - x, y0 - y );
		break;
	}
	m_bReFocus = FALSE;
}
void CImgPrcView::GetCurMouse( double& x, double &y, int &r, int &g, int &b, CString &strTitle )
{
	CRect rectImg;
	CRect rctSrc, rctDest;
	strTitle = "";
	x = y = r = g = b = -1;

	rectImg = CRect( 0, 0, m_nWidth, m_nHeight );
	if ( m_pBitmap )
	{
		GetSrcDest( rectImg, rctSrc, rctDest );

		if ( rctDest.PtInRect( m_MousePoint ) )
		{
			CRect rectClient;
			GetClientRect( &rectClient );

			x = ( m_MousePoint.x / (double) rectClient.Width() ) * m_rectShow.Width() + m_rectShow.left;
			y = ( m_MousePoint.y / (double) rectClient.Height() ) * m_rectShow.Height() + m_rectShow.top;

			x -= rectImg.left;
			y -= rectImg.top;

			int nX = x;
			int nY = y;
			if ( nX >= 0 && nX < m_nWidth && nY >= 0 && nY < m_nHeight )
			{
				int nPos = nY*m_nWidth + nX;
				if ( m_flag == 24 )
				{
					b = m_pBitmap[nPos * 3 + 0];
					g = m_pBitmap[nPos * 3 + 1];
					r = m_pBitmap[nPos * 3 + 2];
				}
				else
				{
					r = g = b = m_pBitmap[nPos];
				}
			}
			strTitle = "original image";
			return;
		}
	}

	for ( int i = 0; i < m_nItrImageCount; i++ )
	{
		CalCurImgRect( NULL, i, rectImg );
		GetSrcDest( rectImg, rctSrc, rctDest );

		if ( !rctDest.PtInRect( m_MousePoint ) )
			continue;

		CRect rectClient;
		GetClientRect( &rectClient );

		x = ( m_MousePoint.x / (double) rectClient.Width() ) * m_rectShow.Width() + m_rectShow.left;
		y = ( m_MousePoint.y / (double) rectClient.Height() ) * m_rectShow.Height() + m_rectShow.top;

		x -= rectImg.left;
		y -= rectImg.top;

		int nX = x;
		int nY = y;
		if ( nX >= 0 && nX < m_pnItrImgWidth[i] && nY >= 0 && nY < m_pnItrImgHeight[i] )
		{
			int nPos = nY*m_pnItrImgWidth[i] + nX;
			if ( m_pItrImageFlag[i] == 24 )
			{
				b = m_ppItrImage[i][nPos * 3 + 0];
				g = m_ppItrImage[i][nPos * 3 + 1];
				r = m_ppItrImage[i][nPos * 3 + 2];
			}
			else
			{
				r = g = b = m_ppItrImage[i][nPos];
			}
		}

		strTitle = m_pstrItrMark[i];
		break;
	}
}
void CImgPrcView::CurrentFocusPos()
{
	CRect rectImg;
	CRect rctSrc, rctDest;

	for ( int i = 0; i < m_nItrImageCount; i++ )
	{
		CalCurImgRect( NULL, i, rectImg );
		GetSrcDest( rectImg, rctSrc, rctDest );

		if ( !rctDest.PtInRect( m_MousePoint ) )
			continue;

		CRect rectClient;
		GetClientRect( &rectClient );

		m_dFocusX = ( m_MousePoint.x / (double) rectClient.Width() ) * m_rectShow.Width() + m_rectShow.left;
		m_dFocusY = ( m_MousePoint.y / (double) rectClient.Height() ) * m_rectShow.Height() + m_rectShow.top;

		m_dFocusX -= rectImg.left;
		m_dFocusY -= rectImg.top;

		m_strFocusTitle = m_pstrItrMark[i];

		m_bReFocus = TRUE;
		break;
	}
}

void CImgPrcView::GetSrcDest( CRect rctImg, CRect& rctSrc, CRect& rctDest )
{
	// ��ȡ��ǰ�ͻ�����С
	CRect rectClient;
	GetClientRect( &rectClient );

	int nClientWidth = rectClient.Width();
	int nClientHeight = rectClient.Height();

	double dRatio = nClientWidth / m_rectShow.Width();

	CFRect frctSrc, frctImg( rctImg );
	frctSrc.InterSectRect( &m_rectShow, &frctImg );

	// ת��Ϊͼ������
	frctSrc.OffsetRect( -rctImg.left, -rctImg.top );

	rctSrc.left = frctSrc.left;
	rctSrc.top = frctSrc.top;
	rctSrc.right = MIN( rctImg.Width(), frctSrc.right + 1 );
	rctSrc.bottom = MIN( rctImg.Height(), frctSrc.bottom + 1 );

	CFRect frctSrc1;
	frctSrc1.InterSectRect( &m_rectShow, &frctImg );
	rctDest.left = ( frctSrc1.left - m_rectShow.left + rctSrc.left - frctSrc.left )*dRatio;
	rctDest.top = ( frctSrc1.top - m_rectShow.top + rctSrc.top - frctSrc.top )*dRatio;
	rctDest.right = rctDest.left + rctSrc.Width()*dRatio;
	rctDest.bottom = rctDest.top + rctSrc.Height()*dRatio;

	if ( frctImg.bottom > m_rectShow.bottom || frctImg.top < m_rectShow.top )
	{
		int nTemp = rctSrc.bottom;
		rctSrc.bottom = rctImg.Height() - rctSrc.top;
		rctSrc.top = rctImg.Height() - nTemp;
		if ( rctSrc.top < 0 )
		{
			rctDest.bottom += rctSrc.top*dRatio;
			rctSrc.top = 0;
		}
	}
}


void CImgPrcView::Draw( CDC*pDC, BYTE* pImg, int nHeight, int nWidth, WORD flag, CRect rectSrc, CRect rectDest )
{
	BYTE* pImageDisplay = NULL;
	BITMAPINFO* pBitmapInfo = NULL;

	TranslateVectorToBitmap( nHeight, nWidth, pImg, pImageDisplay, flag );
	CreateBITMAPINFO( pBitmapInfo, nHeight, nWidth, flag );

	int n = 0;
	if ( rectDest.Height() == rectSrc.Height() && rectDest.top == 0 && rectSrc.top == 0 )
		n = 1;

	StretchDIBits( pDC->GetSafeHdc(),    // stretchDibits bug.....
		rectDest.left, rectDest.top + n,
		rectDest.Width(), rectDest.Height() - n,
		rectSrc.left, rectSrc.top + n,
		rectSrc.Width(), rectSrc.Height() - n,
		pImageDisplay,
		pBitmapInfo,
		DIB_RGB_COLORS,
		SRCCOPY );


	delete pImageDisplay;
	delete pBitmapInfo;
}

void CImgPrcView::CalTotalRect( CDC* pDC, CRect& rectTotal )
{
	rectTotal.left = rectTotal.top = rectTotal.bottom = rectTotal.right = 0;

	int nMaxTextLen = 0;
	for ( int i = 0; i < m_nItrImageCount; i++ )
	{
		rectTotal.right = max( (int) ( rectTotal.right ), m_pnItrImgWidth[i] );
		rectTotal.bottom += m_pnItrImgHeight[i] + 10;
	}
	rectTotal.right += m_nWidth + 30;
	rectTotal.bottom = max( (int) ( rectTotal.bottom ), m_nHeight );
}

void CImgPrcView::CalCurImgRect( CDC* pDC, int nID, CRect& rectImg )
{
	int nTop = 0;
	for ( int i = 0; i < m_nItrImageCount && i<nID; i++ )
		nTop += m_pnItrImgHeight[i] + 10;

	rectImg.left = rectImg.top = rectImg.bottom = rectImg.right = 0;
	rectImg.top = nTop;

	rectImg.right = m_pnItrImgWidth[nID];
	rectImg.bottom = rectImg.top + m_pnItrImgHeight[nID];

	rectImg.left += m_nWidth + 30;
	rectImg.right += m_nWidth + 30;
	//rectImg.bottom = max(rectImg.bottom, m_nHeight);
}

Point2f CImgPrcView::GetCentroidFromBinary( BYTE* pBinary, int dwHeight, int dwWidth )
{
	int			i, j;
	int			nArea, nMomentX, nMomentY;
	Point2f		pointTmp;

	nArea = nMomentX = nMomentY = 0;
	for ( i = 0; i < m_nHeight; i++ )
		for ( j = 0; j < m_nWidth; j++ )
		{
			if ( pBinary[i * m_nWidth + j] == 255 )
			{
				nMomentY += i;
				nMomentX += j;
				nArea++;
			}
		}

	if ( nArea == 0 )
	{
		pointTmp.y = double( dwHeight ) / 2;
		pointTmp.x = double( dwWidth ) / 2;
	}
	else
	{
		pointTmp.y = double( nMomentY ) / nArea;
		pointTmp.x = double( nMomentX ) / nArea;
	}

	return pointTmp;
}

void CImgPrcView::FreeInterResultImage( bool bInvalide )
{
	if ( !m_nItrImageCount )
		return;

	for ( int i = 0; i<m_nItrImageCount; i++ )
	{
		delete[]m_ppItrImage[i];
	}
	if ( m_ppItrImage )			delete[]m_ppItrImage;
	m_ppItrImage = NULL;
	if ( m_pnItrImgHeight )	delete[]m_pnItrImgHeight;
	m_pnItrImgHeight = NULL;
	if ( m_pnItrImgWidth )	delete[]m_pnItrImgWidth;
	m_pnItrImgWidth = NULL;
	if ( m_pItrImageFlag )		delete[]m_pItrImageFlag;
	m_pItrImageFlag = NULL;
	if ( m_pstrItrMark )			delete[]m_pstrItrMark;
	m_pstrItrMark = NULL;

	m_nItrImageCount = 0;

	if ( this->m_hWnd && bInvalide )
	{
		Invalidate( FALSE );
		UpdateWindow();
	}
}
void CImgPrcView::SetInterResultImage( int nID, Mat& img, CString strMark )
{
	SetInterResultImage( nID, img.data, img.rows, img.cols, img.channels() * 8, strMark );
}
void CImgPrcView::SetInterResultImage( int nID, double* pBitmap, int dwHeight, int dwWidth, WORD flag, CString strMark )
{
	double dMax = pBitmap[0];
	double dMin = pBitmap[0];
	for ( int i = 0; i < dwHeight*dwWidth; i++ )
	{
		dMax = max( dMax, pBitmap[i] );
		dMin = min( dMin, pBitmap[i] );
	}

	BYTE* pbTemp = new BYTE[dwHeight*dwWidth];
	memset( pbTemp, 0, sizeof( BYTE )*dwHeight*dwWidth );

	if ( dMax - dMin > 0.00001 )
	{
		for ( int i = 0; i < dwHeight*dwWidth; i++ )
			pbTemp[i] = ( pBitmap[i] - dMin ) * 255 / ( dMax - dMin );
	}
	SetInterResultImage( nID, pbTemp, dwHeight, dwWidth, flag, strMark );
	delete pbTemp;
}
void CImgPrcView::SetInterResultImage( int nID, BYTE* pBitmap, int dwHeight, int dwWidth, WORD flag, CString strMark )
{
	if ( nID <= 0 )
	{
		FreeInterResultImage( false );
		m_nItrImageCount = m_nImgShowID = 0;
	}

	/*#ifndef _DEBUG
	else
	return;
	#endif*/

	nID = m_nImgShowID++;

	if ( m_nItrImageCount <= nID )
		AddInterResultImage( pBitmap, dwHeight, dwWidth, flag, strMark );
	else
	{
		delete m_ppItrImage[nID];
		m_ppItrImage[nID] = new BYTE[dwHeight*dwWidth*flag / 8];
		memcpy( m_ppItrImage[nID], pBitmap, dwHeight*dwWidth*flag / 8 );

		m_pnItrImgHeight[nID] = dwHeight;
		m_pnItrImgWidth[nID] = dwWidth;
		m_pItrImageFlag[nID] = flag;
		m_pstrItrMark[nID] = strMark;
	}
}
void CImgPrcView::AddInterResultImage( BYTE* pBitmap, int dwHeight, int dwWidth, WORD flag, CString strMark )
{
	BYTE** ppNewResultImage = new BYTE*[m_nItrImageCount + 1];
	::memcpy( ppNewResultImage, m_ppItrImage, sizeof( BYTE* )*m_nItrImageCount );

	ppNewResultImage[m_nItrImageCount] = new BYTE[dwHeight*dwWidth*flag / 8];
	::memcpy( ppNewResultImage[m_nItrImageCount], pBitmap, sizeof( BYTE )*dwHeight*dwWidth*flag / 8 );

	delete m_ppItrImage;
	m_ppItrImage = ppNewResultImage;
	ppNewResultImage = NULL;


	int * pTemp = new int[m_nItrImageCount + 1];
	::memcpy( pTemp, m_pnItrImgHeight, sizeof( int )*m_nItrImageCount );
	pTemp[m_nItrImageCount] = dwHeight;
	delete m_pnItrImgHeight;
	m_pnItrImgHeight = pTemp;
	pTemp = NULL;

	pTemp = new int[m_nItrImageCount + 1];
	::memcpy( pTemp, m_pnItrImgWidth, sizeof( int )*m_nItrImageCount );
	pTemp[m_nItrImageCount] = dwWidth;
	delete m_pnItrImgWidth;
	m_pnItrImgWidth = pTemp;
	pTemp = NULL;

	WORD *pTemp1 = new WORD[m_nItrImageCount + 1];
	::memcpy( pTemp1, m_pItrImageFlag, sizeof( WORD )*m_nItrImageCount );
	pTemp1[m_nItrImageCount] = flag;
	delete m_pItrImageFlag;
	m_pItrImageFlag = pTemp1;
	pTemp1 = NULL;

	CString *pTemp2 = new CString[m_nItrImageCount + 1];
	for ( long i = 0; i<m_nItrImageCount; i++ )
		pTemp2[i] = m_pstrItrMark[i];
	pTemp2[m_nItrImageCount] = strMark;
	delete[]m_pstrItrMark;
	m_pstrItrMark = pTemp2;
	pTemp2 = NULL;

	m_nItrImageCount++;
}

void CImgPrcView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes( MM_TEXT, sizeTotal );

	CRect rectClient;
	GetClientRect( &rectClient );
	m_rectShow = CFRect( rectClient );
	m_dScaleRatio = 1;

	//m_HCursorHand = ::LoadCursor(AfxGetResourceHandle(), MAKEINTRESOURCE(IDC_HAND));

	m_nHeight = 100;
	m_nWidth = 100;
	m_flag = 24;

	g_pView = this;
	g_cpView = this;

	mog = createBackgroundSubtractorMOG2( 100, 25, false );
	mog->setVarThreshold( 20 );

	//bodyHog.setSVMDetector( HOGDescriptor::getDefaultPeopleDetector() );
	//cascade = new CascadeClassifier( "d:/haarcascade_mcs_upperbody.xml" );
}

/////////////////////////////////////////////////////////////////////////////
// CImgPrcView printing

BOOL CImgPrcView::OnPreparePrinting( CPrintInfo* pInfo )
{
	return DoPreparePrinting( pInfo );
}
void CImgPrcView::OnBeginPrinting( CDC* /*pDC*/, CPrintInfo* /*pInfo*/ )
{
}
void CImgPrcView::OnEndPrinting( CDC* /*pDC*/, CPrintInfo* /*pInfo*/ )
{
}

/////////////////////////////////////////////////////////////////////////////
// CImgPrcView diagnostics

#ifdef _DEBUG
void CImgPrcView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImgPrcView::Dump( CDumpContext& dc ) const
{
	CScrollView::Dump( dc );
}

CImgPrcDoc* CImgPrcView::GetDocument() // non-debug version is inline
{
	ASSERT( m_pDocument->IsKindOf( RUNTIME_CLASS( CImgPrcDoc ) ) );
	return (CImgPrcDoc*) m_pDocument;
}
#endif //_DEBUG


extern bool LoadVectorFromBMPFile( CString &FileName, BYTE *&pBitmapData, int &dwHeight, int &dwWidth, WORD &flag );

/////////////////////////////////////////////////////////////////////////////
// CImgPrcView message handlers
void CImgPrcView::OnFileOpen()
{
	m_flag = m_flag2 = 0;

	if ( m_pBitmap2 )
		delete[]m_pBitmap2;
	m_pBitmap2 = NULL;

	static char szFilter[] = "|PNG file(*.png)|*.png|MP4 file(*.mp4)|*.mp4|BMP file(*.bmp)|*.bmp||";
	CFileDialog dlg( TRUE, "*.png|*.mp4|*.bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter );
	if ( dlg.DoModal() != IDOK ) return;

	m_strFileName = dlg.GetPathName();

	g_strFileNames.RemoveAll();
	g_nFileID = 0;

	CFileFind finder;
	if ( !finder.FindFile( m_strFileName ) )
		return;

	CString strExt = dlg.GetFileExt();
	{
		CString strPath = m_strFileName.Left( m_strFileName.ReverseFind( '\\' ) );
		BOOL bWorking = finder.FindFile( strPath + "\\*." + strExt );
		while ( bWorking )
		{
			bWorking = finder.FindNextFile();
			strPath = finder.GetFilePath();
			strPath = strPath.Mid( strPath.ReverseFind( '\\' ) + 1 );
			g_strFileNames.Add( strPath );
		}

		strPath = m_strFileName.Mid( m_strFileName.ReverseFind( '\\' ) + 1 );

		qsort( g_strFileNames.GetData(), g_strFileNames.GetSize(), sizeof( CString ), compare00 );
		for ( int i = 0; i < g_strFileNames.GetCount(); i++ )
		{
			if ( !g_strFileNames[i].CompareNoCase( strPath ) )
				g_nFileID = i;
		}
	}
	if ( !strExt.CompareNoCase( "bmp" ) || !strExt.CompareNoCase( "png" ) )
	{
		if ( !LoadBitmap() )
			return;
		m_nFileSource = 1;
		m_nFrmID = 1;
	}
	else if ( !strExt.CompareNoCase( "mp4" ) )
	{
		m_nFileSource = 2;
		m_nFrmID = 0;
		m_nFrmCnt = 0;

		String strFileName( m_strFileName );
		if ( !m_pVideo->open( strFileName ) )
			return;
		m_nFrmCnt = m_pVideo->get( CV_CAP_PROP_FRAME_COUNT );

		Mat frame;

		m_nHeight = rectROI.height;// frame.rows;// / 2;
		m_nWidth = rectROI.width; // frame.cols;// / 2;
		m_flag = 24;

		// set first frame
		m_nFrmID = STARTFRM;
		m_pVideo->set( CV_CAP_PROP_POS_FRAMES, m_nFrmID );

		if ( !m_pVideo->read( frame ) )
			return;

		m_nFrmID = m_pVideo->get( CV_CAP_PROP_POS_FRAMES );

		// ����һ��Mat���Ͳ��������趨������
		//Point2f center( rectROI.x + rectROI.width/2, rectROI.y + rectROI.height / 2 );//��ת����  
		//Mat M = getRotationMatrix2D( center, ROTATE, 1 );//������ת�ķ���任����  

		//warpAffine( frame, frame, M, frame.size() );//����任  
		//imageROI = frame( rectROI );

		//CString strFileName1 = "E:\\berkeley\\zhan\\code\\ImgPrc_MDI_20171213\\data\\cord.txt";
		//this->SetInterResultImage( 0, frame, "img" );
		//LoadMappingData( strFileName1, frame );

		Mat imageROI;
		resize( frame, frame, Size( 960, 540 ) );
		imageROI = frame( rectROI ).clone();

		m_nHeight = imageROI.rows;
		m_nWidth = imageROI.cols;

		delete m_pBitmap;
		m_pBitmap = new BYTE[m_nHeight*m_nWidth*m_flag / 8];

		//Mat dst( m_nHeight, m_nWidth, CV_8UC3, m_pBitmap );
		//resize( imageROI, dst, dst.size(), 0, 0, INTER_NEAREST );
		memcpy( m_pBitmap, imageROI.data, m_nHeight*m_nWidth*m_flag / 8 );

		CString strTemp;
		strTemp.Format( "%ld/%ld", m_nFrmID, m_nFrmCnt );
		GetDocument()->SetTitle( strTemp );
	}

	g_Targets.clear();
	g_buff.clear();


	bFirst = true;
	nPos = 0;
	bRecapture = true;
	nFrame = 0;
	bProcessFirst = true;

	OnSignLoad();
	OnBackLoad();

	GetDocument()->SetTitle( m_strFileName );
	FreeInterResultImage();
	Invalidate( true );
	UpdateWindow();
}

void CImgPrcView::OnFileSave()
{
	CString strFileName;
	static char szFilter[] = "BMP file(*.bmp)|*.bmp||";
	CFileDialog dlg( false, "bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter );
	if ( dlg.DoModal() != IDOK ) return;

	strFileName = dlg.GetPathName();
	if ( !WriteBMPFileFromVector( strFileName, m_pBitmap, m_nHeight, m_nWidth, m_flag ) )
		return;
}


bool CImgPrcView::LoadBitmap()
{
	String strFileName = format( "%s", m_strFileName );
	Mat img = imread( strFileName );

	if ( img.channels() == 4 )
		cvtColor( img, img, CV_BGRA2BGR );

	delete m_pBitmap;
	m_flag = img.channels() * 8;
	m_nHeight = img.rows;
	m_nWidth = img.cols;
	m_pBitmap = new BYTE[m_nWidth * m_nHeight * m_flag / 8];
	memcpy( m_pBitmap, img.data, m_nWidth * m_nHeight * m_flag / 8 );

	if ( m_pBmpProc )
		delete m_pBmpProc;

	m_nHeightProc = m_nHeight;
	m_nWidthProc = m_nWidth;
	m_flagProc = m_flag;

	//�ж��Ƿ�Ϊ2ֵͼ
	m_isBinary = true;
	for ( size_t i = 0; i < m_nHeight * m_nWidth; ++i )
	{
		if ( m_pBitmap[i] != 0 && m_pBitmap[i] != 255 )
		{
			m_isBinary = false;
			break;
		}
	}
	if ( m_flag == 32 )
	{
		m_flagProc = 24;
		m_pBmpProc = new BYTE[m_nHeightProc * m_nWidthProc * m_flagProc / 8];
		for ( int i = 0; i < m_nHeightProc * m_nWidthProc; i++ )
		{
			m_pBmpProc[i * 3 + 2] = m_pBitmap[i * 4 + 2];
			m_pBmpProc[i * 3 + 1] = m_pBitmap[i * 4 + 1];
			m_pBmpProc[i * 3 + 0] = m_pBitmap[i * 4 + 0];
		}
		memcpy( m_pBitmap, m_pBmpProc, m_nHeightProc * m_nWidthProc * m_flagProc / 8 );
		m_flag = 24;
	}
	else
	{
		m_pBmpProc = new BYTE[m_nHeightProc * m_nWidthProc * m_flagProc / 8];
		memcpy( m_pBmpProc, m_pBitmap, m_nHeightProc * m_nWidthProc * m_flagProc / 8 );
	}

	//if( m_flag == 24 )
	//{
	//	BYTE* pTemp = new BYTE[m_nHeight*m_nWidth];
	//	for( int i=0; i<m_nHeight*m_nWidth; i++)
	//		pTemp[i] = (m_pBitmap[i*3+2] +m_pBitmap[i*3+1] +m_pBitmap[i*3+0] ) / 3;
	//	delete m_pBitmap;
	//	m_pBitmap = pTemp;
	//	pTemp = NULL;
	//	m_flag = 8;
	//}
	GetDocument()->SetTitle( m_strFileName );
	return true;
}

void CImgPrcView::OnFileOpen2()
{
	m_flag = m_flag2 = 0;
	CDlgFiles dlg;
	if ( dlg.DoModal() != IDOK )
		return;

	m_strFileName = dlg.m_strFilePath1;
	LoadVectorFromBMPFile( m_strFileName, m_pBitmap, m_nHeight, m_nWidth, m_flag );

	m_strFileName2 = dlg.m_strFilePath2;
	LoadVectorFromBMPFile( m_strFileName2, m_pBitmap2, m_nHeight2, m_nWidth2, m_flag2 );

	FreeInterResultImage();
	AddInterResultImage( m_pBitmap2, m_nHeight2, m_nWidth2, m_flag2, "�ڶ���ͼ���ң�" );

	Invalidate( FALSE );
	UpdateWindow();

}

//ˢ�´�����ԭͼ
LRESULT CImgPrcView::OnUpdateBitmap( WPARAM wParam, LPARAM lParam )
{
	if ( wParam )
	{
		Invalidate( FALSE );
		UpdateWindow();

		return S_OK;
	}
	if ( !m_pBmpProc )
	{
		return NULL;
	}
	if ( m_pBitmap ) delete[] m_pBitmap;
	m_nHeight = m_nHeightProc;
	m_nWidth = m_nWidthProc;
	m_flag = m_flagProc;
	m_pBitmap = new BYTE[m_nHeight * m_nWidth * m_flag / 8];
	memcpy( m_pBitmap, m_pBmpProc, m_nHeight * m_nWidth * m_flag / 8 );

	m_isBinary = true;
	if ( m_flag == 8 )
	{
		for ( size_t i = 0; i < m_nHeight * m_nWidth; ++i )
		{
			if ( m_pBitmap[i] != 0 && m_pBitmap[i] != 255 )
			{
				m_isBinary = false;
				break;
			}
		}
	}
	else
	{
		m_isBinary = false;
	}


	delete[] m_pBmpProc;
	m_pBmpProc = nullptr;

	FreeInterResultImage();
	Invalidate( FALSE );
	UpdateWindow();
}


void CImgPrcView::OnCreateMorphDialog()
{
	if ( !m_pBitmap )
	{
		MessageBox( "���ͼƬ.", MB_OK );
		return;
	}

	// TODO: �ڴ���������������
	if ( m_pMorphDlg ) delete m_pMorphDlg;
	m_pMorphDlg = new CMorphDlg( this );
	m_pMorphDlg->Create( IDD_DIALOG2 );
	m_pMorphDlg->ShowWindow( SW_SHOWNORMAL );
}

//��̬ѧ����
LRESULT CImgPrcView::OnMorphology( WPARAM wParam, LPARAM lParam )
{
	if ( !m_pBitmap )
	{
		return NULL;
	}

	int type = m_flag == 24 ? CV_8UC3 : CV_8UC1;
	Mat img = Mat::zeros( m_nHeight, m_nWidth, type );
	memcpy( img.data, m_pBitmap, img.total() * m_flagProc / 8 );

	int kernel_size = m_pMorphDlg->m_nThreshold;
	int kernel_type = m_pMorphDlg->m_nShape;
	int operate_type = m_pMorphDlg->m_nType;

	if ( operate_type == 7 && m_flag == 24 )
	{
		MessageBox( "���б任����Ҷ�ͼ��.", MB_OK );
		return NULL;
	}


	Mat imRes;
	Mat element = getStructuringElement( kernel_type, cv::Size( kernel_size * 2 + 1, kernel_size * 2 + 1 ) );
	morphologyEx( img, imRes, operate_type, element );

	if ( m_pBmpProc ) delete[] m_pBmpProc;
	m_flagProc = imRes.channels() * 8;
	m_nHeightProc = imRes.rows;
	m_nWidthProc = imRes.cols;
	m_pBmpProc = new BYTE[imRes.total() * imRes.channels()];
	memcpy( m_pBmpProc, imRes.data, imRes.total() * imRes.channels() );

	SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );

	imRes.release();
	img.release();
	element.release();

	Invalidate( FALSE );
	UpdateWindow();
	return LRESULT();
}

//ֱ��ͼ����͹涨��
LRESULT CImgPrcView::OnHistgramOperate( WPARAM wParam, LPARAM lParam )
{
	if ( !m_pBitmap )
	{
		return NULL;
	}

	if ( bool( wParam ) == false )	//ֱ��ͼ����
	{
		int type = m_flag == 8 ? CV_8UC1 : CV_8UC3;
		Mat img = Mat::zeros( m_nHeight, m_nWidth, type );

		memcpy( img.data, m_pBitmap, m_nHeight * m_nWidth * img.channels() );
		Mat img_equal = proc::HistEqualization( img );

		if ( m_pBmpProc ) delete[] m_pBmpProc;
		m_pBmpProc = new BYTE[img_equal.total() * img_equal.channels()];
		memcpy( m_pBmpProc, img_equal.data, img_equal.total() * img_equal.channels() );
		m_flagProc = img_equal.channels() * 8;
		m_nHeightProc = img_equal.rows;
		m_nWidthProc = img_equal.cols;

		//FreeInterResultImage();
		SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );

		img.release();
		img_equal.release();

		Invalidate( false );
		UpdateWindow();

	}
	else
	{
		//����ԭͼ
		Mat imgSrc = Mat::zeros( m_nHeight, m_nWidth, CV_8UC1 );
		memcpy( imgSrc.data, m_pBitmap, imgSrc.total() );

		//����ģ��ͼ��
		Mat imgMask = *(Mat*) lParam;
		cv::imshow( "mask", imgMask );

		//ֱ��ͼƥ��
		Mat imgShow;
		proc::HistSpecification( imgSrc, imgMask, imgShow );
		m_nHeightProc = imgShow.rows;
		m_nWidthProc = imgShow.cols;
		m_flagProc = imgShow.channels() * 8;
		if ( m_pBmpProc ) delete m_pBmpProc;
		m_pBmpProc = new BYTE[m_nHeightProc * m_nWidthProc * imgShow.channels()];
		memcpy( m_pBmpProc, imgShow.data, m_nHeightProc * m_nWidthProc * imgShow.channels() );
		//FreeInterResultImage();
		SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );

		imgShow.release();
		imgMask.release();
		imgSrc.release();

		Invalidate( FALSE );
		UpdateWindow();

		cv::waitKey( 0 );
		cv::destroyAllWindows();
	}

	return LRESULT();
}

LRESULT CImgPrcView::OnBinaryRegion( WPARAM wPram, LPARAM lParam )
{
	assert( m_isBinary );
	assert( m_pBinaryRegion );

	//����Mat
	Mat img = Mat::zeros( m_nHeight, m_nWidth, CV_8UC1 );
	memcpy( img.data, m_pBitmap, img.total() );

	int mode = m_pBinaryRegion->m_nMode;
	int method = m_pBinaryRegion->m_nMethod;
	double dAreaCretiria = m_pBinaryRegion->m_dArea;
	double dHeightCretiria = m_pBinaryRegion->m_dHeight;
	double dWidthCretiria = m_pBinaryRegion->m_nidth;
	double dCircleCretiria = m_pBinaryRegion->m_dCircle;

	Point2f pointCenter;
	vector<vector<Point>> contours;
	vector<cv::Vec4i> hiecracy;

	findContours( img, contours, hiecracy, mode, method );
	int nSize = contours.size();
	for ( size_t i = 0; i < contours.size(); ++i )
	{

		Rect rect = boundingRect( contours[i] );
		size_t count = contours[i].size();
		double dAreaRatio = contourArea( contours[i] ) / double( img.total() );
		double dWidthRatio = rect.width / double( img.cols );
		double dHeigtRatio = rect.height / double( img.rows );

		if ( dAreaCretiria != -1 )
		{
			if ( dAreaRatio < dAreaCretiria )
			{
				contours[i].clear();
				nSize--;
				continue;
			}
		}
		if ( dHeightCretiria != -1 )
		{
			if ( dHeigtRatio < dHeightCretiria )
			{
				contours[i].clear();
				nSize--;
				continue;
			}
		}
		if ( dWidthCretiria != -1 )
		{
			if ( dWidthRatio < dWidthCretiria )
			{
				contours[i].clear();
				nSize--;
				continue;
			}
		}

		if ( count < 6 )
		{
			continue;	//��Բ���������Ҫ6��
		}


		Mat pointsf;
		Mat( contours[i] ).convertTo( pointsf, CV_32F );
		RotatedRect ellipse = fitEllipse( pointsf );

		double dCircleLike = MIN( ellipse.size.width, ellipse.size.height ) /
			MAX( ellipse.size.width, ellipse.size.height );

		if ( dCircleCretiria != -1 )
		{
			if ( dCircleLike < dCircleCretiria )
			{
				contours[i].clear();
				nSize--;
				continue;
			}
		}

	}

	//���� 
	Mat imgShow = Mat::zeros( img.size(), CV_8UC1 );
	drawContours( imgShow, contours, -1, 255, -1 );

	m_flagProc = 8;
	m_nHeightProc = imgShow.rows;
	m_nWidthProc = imgShow.cols;
	if ( m_pBmpProc ) delete m_pBmpProc;
	m_pBmpProc = new BYTE[m_nHeightProc * m_nWidthProc];
	memcpy( m_pBmpProc, imgShow.data, imgShow.total() );

	img.release();
	imgShow.release();

	//FreeInterResultImage();
	SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );
	Invalidate( FALSE );
	UpdateWindow();

	return LRESULT();
}

LRESULT CImgPrcView::OnPolygon( WPARAM wParam, LPARAM lParam )
{
	assert( m_pBitmap );
	assert( m_flag == 8 );

	//����Mat
	Mat img( m_nHeight, m_nWidth, CV_8UC1 );
	memcpy( img.data, m_pBitmap, img.total() );

	//����
	const int& nMode = m_pPolygon->m_nMode;
	const int& nMethod = m_pPolygon->m_nMethod;
	const int& nPolygonType = m_pPolygon->m_nType;

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours( img, contours, hierarchy, nMode, nMethod );

	RNG rng( 12345 );

	vector<vector<Point> >hull( contours.size() );

	Mat imgRect = Mat::zeros( img.size(), CV_8UC3 );
	Mat imgCircle = Mat::zeros( img.size(), CV_8UC3 );
	Mat imgHull = Mat::zeros( img.size(), CV_8UC3 );
	Mat imgEllipse = Mat::zeros( img.size(), CV_8UC3 );
	for ( size_t i = 0; i < contours.size(); ++i )
	{

		Scalar color = Scalar( rng.uniform( 0, 255 ), rng.uniform( 0, 255 ), rng.uniform( 0, 255 ) );

		//������ͨ��
		drawContours( imgRect, contours, i, Scalar( 255, 255, 255 ), -1 );
		drawContours( imgCircle, contours, i, Scalar( 255, 255, 255 ), -1 );
		drawContours( imgHull, contours, i, Scalar( 255, 255, 255 ), -1 );
		drawContours( imgEllipse, contours, i, Scalar( 255, 255, 255 ), -1 );

		//��С��Ӿ���
		RotatedRect rect = minAreaRect( contours[i] );
		Point2f P[4];
		rect.points( P );
		for ( int j = 0; j <= 3; j++ )
		{
			line( imgRect, P[j], P[( j + 1 ) % 4], color, 2 );
		}

		//��С���Բ
		Point2f center; float radius;
		minEnclosingCircle( contours[i], center, radius );
		circle( imgCircle, center, radius, color, 2 );


		//��Բ���
		size_t count = contours[i].size();
		if ( count < 6 )
		{
			continue;	//��Բ���������Ҫ6��
		}

		Mat pointsf;
		Mat( contours[i] ).convertTo( pointsf, CV_32F );
		RotatedRect ellipse = fitEllipse( pointsf );	//��Բ���
		cv::ellipse( imgEllipse, ellipse, color, 2 );

		//͹��
		convexHull( Mat( contours[i] ), hull[i], false );
		drawContours( imgHull, hull, i, color, 2 );
	}

	//��ʾ
	if ( m_pBmpProc ) delete m_pBmpProc;
	m_pBmpProc = new BYTE[img.total() * 3];
	m_nHeightProc = img.rows;
	m_nWidthProc = img.cols;
	m_flagProc = 24;
	m_isBinary = false;

	switch ( nPolygonType )
	{
	case 0:
	{
		memcpy( m_pBmpProc, imgRect.data, img.total() * 3 );
		break;
	}
	case 1:
	{
		memcpy( m_pBmpProc, imgEllipse.data, img.total() * 3 );
		break;
	}
	case 2:
	{
		memcpy( m_pBmpProc, imgCircle.data, img.total() * 3 );
		break;
	}
	case 3:
	{
		memcpy( m_pBmpProc, imgHull.data, img.total() * 3 );
		break;
	}
	default:
		break;
	}
	//FreeInterResultImage();
	SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );
	Invalidate( FALSE );
	UpdateWindow();

	img.release();
	imgCircle.release();
	imgRect.release();
	imgEllipse.release();
	imgHull.release();

	return LRESULT();
}


//�ǵ���
LRESULT CImgPrcView::OnConerDetect( WPARAM wParam, LPARAM lParam )
{
	assert( m_pBitmap );

	//����ԭͼ
	int type = m_flag == 8 ? CV_8UC1 : CV_8UC3;
	Mat img = Mat::zeros( m_nHeight, m_nWidth, type );
	memcpy( img.data, m_pBitmap, m_nHeight * m_nWidth * img.channels() );
	if ( img.channels() == 3 )
	{
		cvtColor( img, img, CV_BGR2GRAY );
	}


	switch ( m_pConer->m_nType )
	{
	case 0:
	{
		std::vector<cv::Point> corners;
		cv::goodFeaturesToTrack( img, corners,
			m_pConer->m_nMaxNum,
			m_pConer->m_dThreshold,
			m_pConer->m_nMinDistance );

		cvtColor( img, img, CV_GRAY2RGB );
		proc::Harris().drawOnImage( img, corners, cv::Scalar( 0, 0, 255 ) );//��ǽǵ�  
		break;
	}
	case 1:	//harris
	{
		proc::Harris harris;

		//����ǵ�
		harris.detect( img );

		//��ýǵ�
		vector<Point> pts;
		harris.getCorners( pts, m_pConer->m_dThreshold );

		//��ǽǵ�
		cvtColor( img, img, CV_GRAY2RGB );
		harris.drawOnImage( img, pts, cv::Scalar( 0, 0, 255 ) );//��ǽǵ�  
		break;
	}
	case 2:	//���ٽǵ���  
	{
		std::vector<KeyPoint> keypoints;
		Ptr<FastFeatureDetector>  fast = FastFeatureDetector::create( m_pConer->m_dThreshold, true );
		fast->detect( img, keypoints );
		cv::drawKeypoints( img, keypoints, img,
			cv::Scalar::all( 255 ), cv::DrawMatchesFlags::DRAW_OVER_OUTIMG );
	}
	}

	//��ʾ
	if ( m_pBmpProc ) delete[] m_pBmpProc;
	m_pBmpProc = new BYTE[img.total() * img.channels()];
	m_nHeightProc = img.rows;
	m_nWidthProc = img.cols;
	m_flagProc = img.channels() * 8;
	memcpy( m_pBmpProc, img.data, img.total() * img.channels() );

	//FreeInterResultImage();
	SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );
	Invalidate( FALSE );
	UpdateWindow();

	img.release();

	return LRESULT();
}

LRESULT CImgPrcView::OnMSER( WPARAM wParam, LPARAM lParam )
{
	Mat img( m_nHeight, m_nWidth, CV_8UC1, m_pBitmap );
	vector<Rect> mserRegions;
	vector<vector<Point>> mserPts;

	//params
	Ptr<MSER> mser = MSER::create( m_pMSER->m_nDelta,
		m_pMSER->m_nMinArea,
		m_pMSER->m_nMaxArea,
		m_pMSER->m_dMaxVariation,
		m_pMSER->m_dMinDiversity,
		m_pMSER->m_nMaxEvolution,
		m_pMSER->m_dAreaThreshold,
		m_pMSER->m_dMinMargin,
		m_pMSER->m_nEdgeBlurSize );
	mser->detectRegions( img, mserPts, mserRegions );

	cvtColor( img, img, CV_GRAY2BGR );
	for ( int i = 0; i < mserPts.size(); ++i )
	{
		for ( int j = 0; j < mserPts[i].size(); ++j )
		{
			img.at<cv::Vec3b>( mserPts[i][j].y, mserPts[i][j].x )[0] = 128;
			img.at<cv::Vec3b>( mserPts[i][j].y, mserPts[i][j].x )[1] = 128;
			img.at<cv::Vec3b>( mserPts[i][j].y, mserPts[i][j].x )[2] = 0;
		}
	}

	if ( m_pBmpProc ) delete[] m_pBmpProc;
	m_pBmpProc = new BYTE[img.total() * 3];
	m_flagProc = 24;
	m_nHeightProc = img.rows;
	m_nWidthProc = img.cols;
	memcpy( m_pBmpProc, img.data, img.total() * 3 );

	//FreeInterResultImage();
	SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );
	Invalidate();
	UpdateWindow();

	img.release();
	return LRESULT();
}

LRESULT CImgPrcView::OnhdlSmooth( WPARAM wParam, LPARAM lParam )
{
	if ( !m_pBitmap )
	{
		return NULL;
	}

	int type = m_flag == 24 ? CV_8UC3 : CV_8UC1;
	Mat img = Mat::zeros( m_nHeight, m_nWidth, type );
	memcpy( img.data, m_pBitmap, img.total() * m_flagProc / 8 );

	int kernel_size = m_pSmoothDlg->m_nThreshold;
	int kernel_type = m_pSmoothDlg->m_nType;
	int sigmaColor = m_pSmoothDlg->m_nsigmaColor;
	int sigmaSpace = m_pSmoothDlg->m_nsigmaSpace;
	int dpower = m_pSmoothDlg->Q;
	//int operate_type = m_pMorphDlg->m_nType;

	Mat imRes;
	switch ( kernel_type )
	{
	case 0:
		blur( img, imRes, cv::Size( kernel_size * 2 + 1, kernel_size * 2 + 1 ), Point( -1, -1 ) );
		break;
	case 1:
		GaussianBlur( img, imRes, cv::Size( kernel_size * 2 + 1, kernel_size * 2 + 1 ), 0, 0 );
		break;
	case 2:
		boxFilter( img, imRes, -1, cv::Size( kernel_size * 2 + 1, kernel_size * 2 + 1 ) );
		break;
	case 3:
		medianBlur( img, imRes, kernel_size * 2 + 1 );
		break;
	case 4:
		bilateralFilter( img, imRes, -1, sigmaColor, sigmaSpace );
		break;
	case 5:
	{
		if ( img.channels() == 3 )
		{
			MessageBox( "请打开灰度图！", MB_OK );
			return LRESULT();
		}
		else
			proc::GeometricMeanFilter( img, imRes, m_nHeight, m_nWidth, 8, kernel_size );
		break;
	}

	case 6:
	{
		if ( img.channels() == 3 )
		{
			MessageBox( "请打开灰度图！", MB_OK );
			return LRESULT();
		}

		else
			proc::HarmonicMeanFilter( img, imRes, m_nHeight, m_nWidth, 8, kernel_size );
		break;
	}

	case 7:
	{
		if ( img.channels() == 3 ) {
			MessageBox( "请打开灰度图！", MB_OK );
			return LRESULT();
		}

		else
			proc::InvHarmonicMeanFilter( img, imRes, m_nHeight, m_nWidth, 8, kernel_size, dpower );
		break;
	}
	default:
		break;
	}


	if ( m_pBmpProc ) delete[] m_pBmpProc;
	m_flagProc = imRes.channels() * 8;
	m_nHeightProc = imRes.rows;
	m_nWidthProc = imRes.cols;
	m_pBmpProc = new BYTE[imRes.total() * imRes.channels()];
	memcpy( m_pBmpProc, imRes.data, imRes.total() * imRes.channels() );

	//FreeInterResultImage();
	SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );

	//CString strTmp;
	//strTmp.Format("��̬ѧ�� �ṹԪ�뾶: %d", kernel_type);

	Invalidate( FALSE );
	UpdateWindow();
	return LRESULT();
}

//LRESULT CImgPrcView::OnSIFT(WPARAM wParam, LPARAM lParam)
//{
//	assert(m_pSiftDlg);
//	assert(m_pBitmap);
//
//	//����Mat
//	int type = m_flag == 8 ? CV_8UC1 : CV_8UC3;
//	Mat img(m_nHeight, m_nWidth, type);
//	
//
//	return LRESULT();
//}

//�Ҷ�ֱ��ͼ
void CImgPrcView::OnGrayHistogram()
{
	Mat src;
	if ( m_pBmpProc != nullptr )
	{
		if ( m_flagProc == 8 )
		{
			src = Mat::zeros( m_nHeightProc, m_nWidthProc, CV_8UC1 );
			memcpy( src.data, m_pBmpProc, src.total() );
		}
		else
		{
			src = Mat::zeros( m_nHeightProc, m_nWidthProc, CV_8UC3 );
			memcpy( src.data, m_pBmpProc, src.total() * 3 );
			cvtColor( src, src, CV_RGB2GRAY );
		}
	}
	else
	{
		if ( m_flag == 8 )
		{
			src = Mat::zeros( m_nHeight, m_nWidth, CV_8UC1 );
			memcpy( src.data, m_pBitmap, src.total() );
		}
		else
		{
			src = Mat::zeros( m_nHeight, m_nWidth, CV_8UC3 );
			memcpy( src.data, m_pBitmap, src.total() * 3 );
			cvtColor( src, src, CV_RGB2GRAY );
		}

	}

	int bins = 256;
	int hist_size[] = { bins };
	float range[] = { 0, 256 };
	const float* ranges[] = { range };
	MatND hist;
	int channels[] = { 0 };

	cv::calcHist( &src, 1, channels, Mat(),
		hist, 1, hist_size, ranges,
		true,
		false );

	double max_val;
	cv::minMaxLoc( hist, 0, &max_val, 0, 0 );
	int scale = 2;
	int hist_height = 256;
	Mat hist_img = Mat::zeros( hist_height, bins*scale, CV_8UC3 );
	for ( int i = 0; i<bins; i++ )
	{
		float bin_val = hist.at<float>( i );
		int intensity = cvRound( bin_val*hist_height / max_val );  //Ҫ���Ƶĸ߶�  
		rectangle( hist_img, Point( i*scale, hist_height - 1 ),
			Point( ( i + 1 )*scale - 1, hist_height - intensity ),
			CV_RGB( 255, 255, 255 ) );
	}

	cv::namedWindow( "Gray Histogram", 0 );
	cv::imshow( "Gray Histogram", hist_img );
	cv::waitKey( 0 );
	cv::destroyAllWindows();
}

//RGBֱ��ͼ
void CImgPrcView::OnBGRHistogram()
{
	Mat src;
	if ( m_pBmpProc != nullptr )
	{
		if ( m_flagProc == 8 )
		{
			MessageBox( "�������ɫͼ.", MB_OK );
			return;
		}
		else
		{
			src = Mat::zeros( m_nHeightProc, m_nWidthProc, CV_8UC3 );
			memcpy( src.data, m_pBmpProc, src.total() * 3 );
		}
	}
	else
	{
		if ( !m_pBitmap ) return;

		if ( m_flag == 8 )
		{
			MessageBox( "�������ɫͼ.", MB_OK );
			return;
		}
		else
		{
			src = Mat::zeros( m_nHeight, m_nWidth, CV_8UC3 );
			memcpy( src.data, m_pBitmap, src.total() * 3 );
		}

	}

	int bins = 256;
	int hist_size[] = { bins };
	float range[] = { 0, 256 };
	const float* ranges[] = { range };
	MatND hist_r, hist_g, hist_b;
	int channels_r[] = { 0 };

	cv::calcHist( &src, 1, channels_r, Mat(),
		hist_r, 1, hist_size, ranges,
		true,
		false );

	int channels_g[] = { 1 };
	cv::calcHist( &src, 1, channels_g, Mat(),
		hist_g, 1, hist_size, ranges,
		true,
		false );

	int channels_b[] = { 2 };
	cv::calcHist( &src, 1, channels_b, Mat(),
		hist_b, 1, hist_size, ranges,
		true,
		false );
	double max_val_r, max_val_g, max_val_b;
	cv::minMaxLoc( hist_r, 0, &max_val_r, 0, 0 );
	cv::minMaxLoc( hist_g, 0, &max_val_g, 0, 0 );
	cv::minMaxLoc( hist_b, 0, &max_val_b, 0, 0 );
	int scale = 1;
	int hist_height = 256;
	Mat hist_img = Mat::zeros( hist_height, bins * 3, CV_8UC3 );
	for ( int i = 0; i<bins; i++ )
	{
		float bin_val_r = hist_r.at<float>( i );
		float bin_val_g = hist_g.at<float>( i );
		float bin_val_b = hist_b.at<float>( i );
		int intensity_r = cvRound( bin_val_r*hist_height / max_val_r );
		int intensity_g = cvRound( bin_val_g*hist_height / max_val_g );
		int intensity_b = cvRound( bin_val_b*hist_height / max_val_b );
		rectangle( hist_img, Point( i*scale, hist_height - 1 ),
			Point( ( i + 1 )*scale - 1, hist_height - intensity_r ),
			CV_RGB( 255, 0, 0 ) );

		rectangle( hist_img, Point( ( i + bins )*scale, hist_height - 1 ),
			Point( ( i + bins + 1 )*scale - 1, hist_height - intensity_g ),
			CV_RGB( 0, 255, 0 ) );

		rectangle( hist_img, Point( ( i + bins * 2 )*scale, hist_height - 1 ),
			Point( ( i + bins * 2 + 1 )*scale - 1, hist_height - intensity_b ),
			CV_RGB( 0, 0, 255 ) );

	}
	cv::imshow( "RGB Histogram", hist_img );
	cv::waitKey( 0 );
	cv::destroyAllWindows();
}

//ˮƽͶӰ
void CImgPrcView::OnHorizonProjection()
{
	BYTE* pTmp = nullptr;
	int nW, nH;
	int* pPrj = nullptr;

	if ( m_pBmpProc != nullptr )
	{
		nH = m_nHeightProc;
		nW = m_nWidthProc;
		if ( m_flagProc == 24 )
		{
			proc::BGR2Gray( m_pBmpProc, pTmp, nW, nH );
		}
		else
		{
			pTmp = new BYTE[nH * nW];
			memcpy( pTmp, m_pBmpProc, nH * nW );
		}
	}
	else
	{
		if ( !m_pBitmap )
		{
			return;
		}

		nH = m_nHeight;
		nW = m_nWidth;
		if ( m_flagProc == 24 )
		{
			proc::BGR2Gray( m_pBitmap, pTmp, nW, nH );
		}
		else
		{
			pTmp = new BYTE[nW * nH];
			memcpy( pTmp, m_pBitmap, nW * nH );
		}

	}

	proc::ProjectHorizon( pTmp, nW, nH, pPrj );
	Mat imgShow = proc::Vector2Mat( pPrj, nW );
	cv::imshow( "ˮƽͶӰ", imgShow );
	cv::waitKey( 0 );
	cv::destroyAllWindows();
	delete[] pTmp;
	delete[] pPrj;
	pTmp = nullptr;
	pPrj = nullptr;
}

//��ֱͶӰ
void CImgPrcView::OnVerticalProjection()
{
	BYTE* pTmp = nullptr;
	int nW, nH;
	int* pPrj = nullptr;

	if ( m_pBmpProc != nullptr )
	{
		nH = m_nHeightProc;
		nW = m_nWidthProc;
		if ( m_flagProc == 24 )
		{
			proc::BGR2Gray( m_pBmpProc, pTmp, nW, nH );
		}
		else
		{
			pTmp = new BYTE[nH * nW];
			memcpy( pTmp, m_pBmpProc, nH * nW );
		}
	}
	else
	{
		if ( !m_pBitmap )
		{
			return;
		}

		nH = m_nHeight;
		nW = m_nWidth;
		if ( m_flagProc == 24 )
		{
			proc::BGR2Gray( m_pBitmap, pTmp, nW, nH );
		}
		else
		{
			pTmp = new BYTE[nW * nH];
			memcpy( pTmp, m_pBitmap, nW * nH );
		}

	}

	proc::ProjectVertical( pTmp, nW, nH, pPrj );
	Mat imgShow = proc::Vector2Mat( pPrj, nH, true );
	cv::imshow( "��ֱͶӰ", imgShow );
	delete[] pTmp;
	delete[] pPrj;
	pTmp = nullptr;
	pPrj = nullptr;

	cv::waitKey( 0 );
	cv::destroyAllWindows();
}

void CImgPrcView::OnCreateHistDialog()
{
	// TODO: �ڴ���������������
	if ( !m_pBitmap )
	{
		MessageBox( "�����ͼƬ.", MB_OK );
		return;
	}
	if ( m_flag == 24 )
	{
		MessageBox( "������Ҷ�ͼ��.", MB_OK );
		return;
	}

	if ( m_pHitogram ) delete m_pHitogram;
	m_pHitogram = new CHistDlg( this );
	m_pHitogram->Create( IDD_DIALOGHIST, this );
	m_pHitogram->ShowWindow( SW_SHOWNORMAL );
}


void CImgPrcView::OnCreateRegionDialog()
{
	// TODO: �ڴ���������������
	if ( !m_pBitmap )
	{

		MessageBox( "�����ͼƬ.", MB_OK );
		return;
	}
	if ( !m_isBinary )
	{
		MessageBox( "�������ֵͼ��.", MB_OK );
		return;
	}

	if ( m_pBinaryRegion ) delete m_pBinaryRegion;
	m_pBinaryRegion = new CBinaryRegion( this );
	m_pBinaryRegion->Create( IDD_DIALOG3, this );
	m_pBinaryRegion->ShowWindow( SW_SHOWNORMAL );
}


void CImgPrcView::OnCreatePolygonDialog()
{
	// TODO: �ڴ���������������
	if ( !m_pBitmap )
	{
		MessageBox( "�����ͼƬ.", MB_OK );
		return;
	}
	if ( !m_isBinary )
	{
		MessageBox( "�������ֵͼ��.", MB_OK );
		return;
	}

	if ( m_pPolygon ) delete m_pPolygon;
	m_pPolygon = new CPolygon( this );
	m_pPolygon->Create( IDD_DIALOG4, this );
	m_pPolygon->ShowWindow( SW_SHOWNORMAL );
}


void CImgPrcView::OnCreateCornerDialog()
{
	// TODO: �ڴ���������������
	if ( !m_pBitmap )
	{
		MessageBox( "�����ͼƬ.", MB_OK );
		return;
	}

	if ( m_pConer ) delete m_pConer;
	m_pConer = new CConerDetector( this );
	m_pConer->Create( IDD_DIALOG5, this );
	m_pConer->ShowWindow( SW_SHOWNORMAL );
}


void CImgPrcView::OnCreateMSERDialog()
{
	if ( !m_pBitmap )
	{
		MessageBox( "δ��ͼ��!", MB_OK );
		return;
	}

	if ( m_pMSER )	delete m_pMSER;
	m_pMSER = new CMSER( this );
	m_pMSER->Create( IDD_DIALOG6, this );
	m_pMSER->ShowWindow( SW_SHOWNORMAL );
}


//void CImgPrcView::OnMenu32889()
//{
//	if (!m_pBitmap)
//	{
//		MessageBox("δ��ͼ��!", MB_OK);
//		return;
//	}
//
//	if (m_pSiftDlg) delete m_pSiftDlg;
//	m_pSiftDlg = new CSiftDialog(this);
//	m_pSiftDlg->Create(IDD_DIALOG7, this);
//	m_pSiftDlg->ShowWindow(SW_SHOWNORMAL);
//}


void CImgPrcView::OnMenuSmooth()
{
	// TODO: Add your command handler code here
	// ͼ��ƽ��
	if ( !m_pBitmap )
	{
		MessageBox( "���ͼƬ.", MB_OK );
		return;
	}

	// TODO: �ڴ���������������
	if ( m_pSmoothDlg ) delete m_pSmoothDlg;
	m_pSmoothDlg = new SmoothDlg( this );
	m_pSmoothDlg->Create( IDD_Smooth );
	m_pSmoothDlg->ShowWindow( SW_SHOWNORMAL );
}


//void CImgPrcView::OnLOGandDOG()
//{
//	if (!m_pBitmap)
//	{
//		MessageBox("���ͼƬ.", MB_OK);
//		return;
//	}
//
//	// TODO: �ڴ���������������
//	if (m_pLogdogDlg) delete m_pLogdogDlg;
//	m_pLogdogDlg = new logdogDlg(this);
//	m_pLogdogDlg->Create(IDD_DIALOG8);
//	m_pLogdogDlg->ShowWindow(SW_SHOWNORMAL);
//	// TODO: Add your command handler code here
//}


afx_msg LRESULT CImgPrcView::OnLoganddog( WPARAM wParam, LPARAM lParam )
{
	if ( !m_pBitmap )
	{
		return NULL;
	}

	int type = m_flag == 24 ? CV_8UC3 : CV_8UC1;
	Mat img = Mat::zeros( m_nHeight, m_nWidth, type );
	memcpy( img.data, m_pBitmap, img.total() * m_flagProc / 8 );

	int kernel_size = m_pLogdogDlg->m_nThreshold;
	int kernel_type = m_pLogdogDlg->m_nType;
	//int operate_type = m_pMorphDlg->m_nType;

	Mat imRes;
	switch ( kernel_type )
	{
	case 0:
	{
		// ��˹ƽ��   
		GaussianBlur( img, img, cv::Size( kernel_size * 2 + 1, kernel_size * 2 + 1 ), 0, 0, cv::BORDER_DEFAULT );
		Laplacian( img, imRes, CV_16S, kernel_size * 2 + 1 );
		break;
		//convertScaleAbs(imRes, imRes);
	}
	case 1:
	{
		GaussianBlur( img, imRes, Size( kernel_size * 2 + 1, kernel_size * 2 + 1 ), 0 );
		imRes = img - imRes;
		normalize( imRes, imRes, 255, 0, CV_MINMAX );
		break;
	}
	}
	if ( m_pBmpProc ) delete[] m_pBmpProc;
	m_flagProc = imRes.channels() * 8;
	m_nHeightProc = imRes.rows;
	m_nWidthProc = imRes.cols;
	m_pBmpProc = new BYTE[imRes.total() * imRes.channels()];
	memcpy( m_pBmpProc, imRes.data, imRes.total() * imRes.channels() );

	//FreeInterResultImage();
	SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );

	//CString strTmp;
	//strTmp.Format("��̬ѧ�� �ṹԪ�뾶: %d", kernel_type);

	Invalidate( FALSE );
	UpdateWindow();
	return LRESULT();
}

//ɫ��ת��
void CImgPrcView::OnMenuColorConv()
{
	// TODO: �ڴ���������������
	if ( !m_pBitmap )
	{
		MessageBox( "���ͼƬ.", MB_OK );
		return;
	}
	if ( m_pColorConv )	delete m_pColorConv;
	m_pColorConv = new CColorConv( this );
	m_pColorConv->Create( IDD_DIALOG9 );
	m_pColorConv->ShowWindow( SW_SHOWNORMAL );
}

LRESULT CImgPrcView::OnColorConv( WPARAM wParam, LPARAM lParam )
{
	if ( !m_pBitmap )
	{
		return NULL;
	}

	int type = m_flag == 24 ? CV_8UC3 : CV_8UC1;
	Mat img = Mat::zeros( m_nHeight, m_nWidth, type );//type��������
	memcpy( img.data, m_pBitmap, img.total() * m_flag / 8 );

	int code = m_pColorConv->m_nConvType;
	int dstCn = 0;
	Mat imRes;

	if ( code == CV_GRAY2RGB && img.channels() == 3 )
	{
		//MessageBox("ѡ��ʶ�ת��", MB_OK);
		return NULL;
	}

	if ( ( code == CV_RGB2GRAY ) && img.channels() == 1 )
	{
		MessageBox( "输入彩色图", MB_OK );
		return NULL;
	}

	if ( ( code == CV_RGB2HSV ) && img.channels() == 1 )
	{
		MessageBox( "输入彩色图", MB_OK );
		return NULL;
	}
	cvtColor( img, imRes, code, dstCn );

	if ( m_pBmpProc ) delete[] m_pBmpProc;
	m_flagProc = imRes.channels() * 8;
	m_nHeightProc = imRes.rows;
	m_nWidthProc = imRes.cols;
	m_pBmpProc = new BYTE[imRes.total() * imRes.channels()];
	memcpy( m_pBmpProc, imRes.data, imRes.total() * imRes.channels() );

	if ( imRes.channels() != 1 )
	{
		Mat imgChannel[3];
		split( imRes, imgChannel );

		for ( int i = 0; i < imRes.channels(); i++ )
			SetInterResultImage( i, imgChannel[i].data, m_nHeightProc, m_nWidthProc, 8, "" );
	}
	else
		SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );

	Invalidate( FALSE );
	UpdateWindow();
	return LRESULT();
}

//固定阈值及OTSU分割
void CImgPrcView::OnMenuThresholdSeg()
{
	// TODO: 
	if ( !m_pBitmap )
	{
		MessageBox( "请打开图片.", MB_OK );
		return;
	}

	if ( m_flag != 8 )
	{
		MessageBox( "请打开灰度图！", MB_OK );
		return;
	}
	// TODO:
	if ( m_pThresholdSeg ) delete m_pThresholdSeg;
	m_pThresholdSeg = new CThresholdSeg( this );
	m_pThresholdSeg->Create( IDD_DIALOG10 );
	m_pThresholdSeg->ShowWindow( SW_SHOWNORMAL );
}


LRESULT CImgPrcView::OnThresholdSeg( WPARAM wParam, LPARAM lParam )
{
	if ( !m_pBitmap )
	{
		return NULL;
	}
	int type = m_flag == 24 ? CV_8UC3 : CV_8UC1;
	Mat img = Mat::zeros( m_nHeight, m_nWidth, type );
	memcpy( img.data, m_pBitmap, img.total() * m_flag / 8 );

	int nType = m_pThresholdSeg->m_nType;
	int nSize = m_pThresholdSeg->m_nThreshold;
	int nMaxval = 255;
	Mat imRes;

	if ( nType == CV_THRESH_BINARY || nType == CV_THRESH_BINARY_INV )
		nMaxval = 255;

	if ( nType == CV_THRESH_OTSU )
	{
		nMaxval = 255;
	}

	threshold( img, imRes, nSize, nMaxval, nType );
	m_isBinary = true;

	if ( m_pBmpProc ) delete[] m_pBmpProc;
	m_flagProc = imRes.channels() * 8;
	m_nHeightProc = imRes.rows;
	m_nWidthProc = imRes.cols;
	m_pBmpProc = new BYTE[imRes.total() * imRes.channels()];
	memcpy( m_pBmpProc, imRes.data, imRes.total() * imRes.channels() );

	//FreeInterResultImage();
	SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );
	Invalidate( FALSE );
	UpdateWindow();
	return LRESULT();
}

//����Ӧ��ֵ
void CImgPrcView::OnMenuAdaptiveThreshold()
{
	// TODO: �ڴ���������������
	if ( !m_pBitmap )
	{
		MessageBox( "���ͼƬ.", MB_OK );
		return;
	}
	if ( m_flag != 8 )
	{
		MessageBox( "�򿪻Ҷ�ͼ��", MB_OK );
		return;
	}

	// TODO: �ڴ���������������
	if ( m_pAdaptiveThreshold ) delete m_pAdaptiveThreshold;
	m_pAdaptiveThreshold = new CAdaptiveThreshold( this );
	m_pAdaptiveThreshold->Create( IDD_DIALOG11 );
	m_pAdaptiveThreshold->ShowWindow( SW_SHOWNORMAL );
}

LRESULT CImgPrcView::OnAdaptiveThreshold( WPARAM wParam, LPARAM lParam )
{
	if ( !m_pBitmap )
	{
		return NULL;
	}
	int type = m_flag == 24 ? CV_8UC3 : CV_8UC1;
	Mat img = Mat::zeros( m_nHeight, m_nWidth, type );//type��������
	memcpy( img.data, m_pBitmap, img.total() * m_flag / 8 );

	int a = img.type();
	int nMaxValue = 255;
	int nAdaptiveMethod = m_pAdaptiveThreshold->m_nEditAlg;
	int nThresholdType = m_pAdaptiveThreshold->m_nEditType;
	int nBlockSize = ( m_pAdaptiveThreshold->m_nEditRadius ) * 2 + 1;
	double dC = m_pAdaptiveThreshold->m_dEditDev;
	Mat imRes;

	adaptiveThreshold( img, imRes, nMaxValue, nAdaptiveMethod, nThresholdType, nBlockSize, dC );
	m_isBinary = true;

	if ( m_pBmpProc ) delete[] m_pBmpProc;
	m_flagProc = imRes.channels() * 8;
	m_nHeightProc = imRes.rows;
	m_nWidthProc = imRes.cols;
	m_pBmpProc = new BYTE[imRes.total() * imRes.channels()];
	memcpy( m_pBmpProc, imRes.data, imRes.total() * imRes.channels() );

	//FreeInterResultImage();
	SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );
	Invalidate( TRUE );
	UpdateWindow();
	return LRESULT();
}

//������������
void CImgPrcView::OnMenuIterativeThreshold()
{
	// TODO: �ڴ���������������
	if ( !m_pBitmap )
	{
		MessageBox( "���ͼƬ.", MB_OK );
		return;
	}

	if ( m_flag != 8 )
	{
		MessageBox( "��򿪻Ҷ�ͼ��", MB_OK );
		return;
	}
	// TODO: �ڴ���������������
	if ( m_pIterativeThreshold ) delete m_pIterativeThreshold;
	m_pIterativeThreshold = new CIterativeThreshold( this );
	m_pIterativeThreshold->Create( IDD_DIALOG12 );
	m_pIterativeThreshold->ShowWindow( SW_SHOWNORMAL );
}

void IterativeThreshold( Mat &src, Mat &dst, int n );
LRESULT CImgPrcView::OnIterativeThreshold( WPARAM wParam, LPARAM lParam )
{
	if ( !m_pBitmap )
	{
		return NULL;
	}
	int type = m_flag == 24 ? CV_8UC3 : CV_8UC1;//ͨ����
	Mat img = Mat::zeros( m_nHeight, m_nWidth, type );//type��������
	memcpy( img.data, m_pBitmap, img.total() * m_flag / 8 );

	int nThreshold = m_pIterativeThreshold->m_nEditThreshold;
	Mat imRes;

	IterativeThreshold( img, imRes, nThreshold );
	m_isBinary = true;

	if ( m_pBmpProc ) delete[] m_pBmpProc;
	m_flagProc = imRes.channels() * 8;
	m_nHeightProc = imRes.rows;
	m_nWidthProc = imRes.cols;
	m_pBmpProc = new BYTE[img.total() * imRes.channels()];
	memcpy( m_pBmpProc, imRes.data, imRes.total() * imRes.channels() );

	//FreeInterResultImage();
	SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );
	Invalidate( FALSE );
	UpdateWindow();
	return LRESULT();
}

//迭代阈值分割定义
void IterativeThreshold( Mat &src, Mat &dst, int n )
{
	int nThreshold = 0;                          //历史阈值
	int nMeansF = 0;                             //前景灰度均值
	int nObjectNum = 0;                            //实质像素点个数
	int nMeansB = 0;                             //背景灰度均值
	int nBackNum = 0;                              //背景像素点个数
	int nCol = src.cols * src.channels();       //每行的像素个数

	while ( abs( nThreshold - n ) > 10 )             //迭代停止条件
	{
		nThreshold = n;
		for ( int i = 0; i < src.rows; ++i )
		{
			uchar* pData = src.ptr<uchar>( i );
			for ( int j = 0; j < nCol; ++j )
			{
				if ( pData[j] < nThreshold )   //背景
				{
					++nBackNum;
					nMeansB += pData[j];
				}
				else                        //前景
				{
					++nObjectNum;
					nMeansF += pData[j];
				}
			}
		}
		if ( nBackNum == 0 || nObjectNum == 0 )     //防止出现除以0的计算
			continue;
		n = ( nMeansB / nBackNum + nMeansF / nObjectNum ) / 2;
	}
	cv::threshold( src, dst, n, 255, 0 );     //进行阈值分割
}
//调出标准霍夫变换窗口
void CImgPrcView::OnMenuStandardHoffTran()
{
	// TODO: 在此添加命令处理程序代码
	if ( !m_pBitmap )
	{
		MessageBox( "请打开图片.", MB_OK );
		return;
	}

	if ( !m_isBinary )
	{
		MessageBox( "请打开灰度二值图" );
		return;
	}
	// TODO: 在此添加命令处理程序代码
	if ( m_pStandardHoffTran ) delete m_pStandardHoffTran;
	m_pStandardHoffTran = new CStandardHoffTran( this );
	m_pStandardHoffTran->Create( IDD_DIALOG13 );
	m_pStandardHoffTran->ShowWindow( SW_SHOWNORMAL );
}

//标准霍夫变换操作
LRESULT CImgPrcView::OnStandardHoffTran( WPARAM wParam, LPARAM lParam )
{
	if ( !m_pBitmap )
	{
		return NULL;
	}
	int type = m_flag == 24 ? CV_8UC3 : CV_8UC1;
	Mat img = Mat::zeros( m_nHeight, m_nWidth, type );
	memcpy( img.data, m_pBitmap, img.total() * m_flag / 8 );

	double dRho = m_pStandardHoffTran->m_dDistance;
	double dTheta = ( m_pStandardHoffTran->m_dAngle )*( CV_PI / 180 );
	int nThreshold = m_pStandardHoffTran->m_nThreshold;
	double srn = 0.0;
	double stn = 0.0;
	Mat imRes;
	vector<Vec2f>lines;

	cvtColor( img, imRes, CV_GRAY2BGR );
	HoughLines( img, lines, dRho, dTheta, nThreshold, 0, 0, 0, CV_PI );

	for ( size_t i = 0; i < lines.size(); i++ )
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos( theta ), b = sin( theta );
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound( x0 + 1000 * ( -b ) );
		pt1.y = cvRound( y0 + 1000 * ( a ) );
		pt2.x = cvRound( x0 - 1000 * ( -b ) );
		pt2.y = cvRound( y0 - 1000 * ( a ) );
		line( imRes, pt1, pt2, Scalar( 0, 0, 255 ), 1, LINE_AA );
	}

	if ( m_pBmpProc ) delete[] m_pBmpProc;
	m_flagProc = imRes.channels() * 8;
	m_nHeightProc = imRes.rows;
	m_nWidthProc = imRes.cols;
	m_pBmpProc = new BYTE[img.total() * imRes.channels()];
	memcpy( m_pBmpProc, imRes.data, imRes.total() * imRes.channels() );

	//FreeInterResultImage();
	SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );
	Invalidate( FALSE );
	UpdateWindow();
	return LRESULT();
}

//调出累计概率霍夫变换
void CImgPrcView::OnCreatePPHTDialog()
{
	// TODO: 在此添加命令处理程序代码
	if ( !m_pBitmap )
	{
		MessageBox( "请打开图片.", MB_OK );
		return;
	}

	if ( !m_isBinary )
	{
		MessageBox( "请打开灰度二值图" );
		return;
	}
	// TODO: 在此添加命令处理程序代码
	if ( m_pPPHT ) delete m_pPPHT;
	m_pPPHT = new CPPHT( this );
	m_pPPHT->Create( IDD_DIALOG14 );
	m_pPPHT->ShowWindow( SW_SHOWNORMAL );
}

LRESULT CImgPrcView::OnPPHT( WPARAM wParam, LPARAM lParam )
{
	if ( !m_pBitmap )
	{
		return NULL;
	}

	int type = m_flag == 24 ? CV_8UC3 : CV_8UC1;//通道数
	Mat img = Mat::zeros( m_nHeight, m_nWidth, type );//type数据类型
	memcpy( img.data, m_pBitmap, img.total() * m_flag / 8 );

	double dRho = m_pPPHT->m_dDistance;
	double dTheta = ( m_pPPHT->m_dAngle )*( CV_PI / 180 );
	int nThreshold = m_pPPHT->m_nThreshold;
	double dMinLineLength = m_pPPHT->m_dMinLineLength;
	double dMaxLineLength = m_pPPHT->m_dMaxLineGap;
	Mat imRes;
	vector<Vec4i>lines;

	cvtColor( img, imRes, CV_GRAY2BGR );
	HoughLinesP( img, lines, dRho, dTheta, nThreshold, dMinLineLength, dMaxLineLength );

	//依次在图中绘制出每条线段
	for ( size_t i = 0; i < lines.size(); i++ )
	{
		Vec4i l = lines[i];
		line( imRes, Point( l[0], l[1] ), Point( l[2], l[3] ), Scalar( 0, 0, 255 ), 1, LINE_AA );
	}

	if ( m_pBmpProc ) delete[] m_pBmpProc;
	m_flagProc = imRes.channels() * 8;
	m_nHeightProc = imRes.rows;
	m_nWidthProc = imRes.cols;
	m_pBmpProc = new BYTE[imRes.total() * imRes.channels()];
	memcpy( m_pBmpProc, imRes.data, imRes.total() * imRes.channels() );

	//FreeInterResultImage();
	SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );
	Invalidate( FALSE );
	UpdateWindow();
	return LRESULT();
}

//调出霍夫圆变换窗口
void CImgPrcView::OnCreateHoughCircleDialog()
{
	// TODO: 在此添加命令处理程序代码
	if ( !m_pBitmap )
	{
		MessageBox( "请打开图片.", MB_OK );
		return;
	}

	if ( m_flag != 8 )
	{
		MessageBox( "请打开灰度图！", MB_OK );
		return;
	}
	// TODO: 在此添加命令处理程序代码
	if ( m_pHoughCircleTran ) delete m_pHoughCircleTran;
	m_pHoughCircleTran = new CHoughCircleTran( this );
	m_pHoughCircleTran->Create( IDD_DIALOG15 );
	m_pHoughCircleTran->ShowWindow( SW_SHOWNORMAL );
}

LRESULT CImgPrcView::OnHoughCircleTran( WPARAM wParam, LPARAM lParam )
{
	if ( !m_pBitmap )
	{
		return NULL;
	}
	int type = m_flag == 24 ? CV_8UC3 : CV_8UC1;
	Mat img = Mat::zeros( m_nHeight, m_nWidth, type );
	memcpy( img.data, m_pBitmap, img.total()*m_flag / 8 );

	int nMethod = HOUGH_GRADIENT;
	double dDp = m_pHoughCircleTran->m_dResolutionSet;
	double dMinDist = m_pHoughCircleTran->m_dMinCenterDist;
	double dParam1 = m_pHoughCircleTran->m_dHighThres;
	double dParam2 = m_pHoughCircleTran->m_dAccuThres;
	int nMinRadius = 0;
	int nMaxRadius = 0;
	vector<Vec3f>circles;
	Mat imRes;

	cvtColor( img, imRes, CV_GRAY2BGR );
	HoughCircles( img, circles, nMethod, dDp, dMinDist, dParam1, dParam2, nMinRadius, nMaxRadius );

	//依次在图中绘制出圆
	for ( size_t i = 0; i < circles.size(); i++ )
	{
		//参数定义
		Point center( cvRound( circles[i][0] ), cvRound( circles[i][1] ) );
		int radius = cvRound( circles[i][2] );
		//绘制圆心
		circle( imRes, center, 3, Scalar( 0, 255, 0 ), -1, 4, 0 );
		//绘制圆轮廓
		circle( imRes, center, radius, Scalar( 0, 0, 255 ), 1, 4, 0 );
	}

	if ( m_pBmpProc ) delete[] m_pBmpProc;
	m_flagProc = imRes.channels() * 8;
	m_nHeightProc = imRes.rows;
	m_nWidthProc = imRes.cols;
	m_pBmpProc = new BYTE[imRes.total() * imRes.channels()];
	memcpy( m_pBmpProc, imRes.data, imRes.total() * imRes.channels() );

	//FreeInterResultImage();
	SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );
	Invalidate( FALSE );
	UpdateWindow();
	return LRESULT();
}


void CImgPrcView::OnOpenCVSobel()
{
	if ( !m_pBitmap )
	{
		MessageBox( "请打开图片！", MB_OK );
		return;
	}

	if ( m_pSobelDlg ) delete m_pSobelDlg;
	m_pSobelDlg = new CSobelDlg( this );
	m_pSobelDlg->Create( IDD_SOBEL );
	m_pSobelDlg->ShowWindow( SW_SHOWNORMAL );
}


void CImgPrcView::OnRobertOperator()
{
	if ( !m_pBitmap )
	{
		MessageBox( "请打开图片！", MB_OK );
		return;
	}

	if ( m_pRobertDlg ) delete m_pRobertDlg;
	m_pRobertDlg = new CRobertDlg( this );
	m_pRobertDlg->Create( IDD_ROBERT );
	m_pRobertDlg->ShowWindow( SW_SHOWNORMAL );
}


void CImgPrcView::OnOpenCVCanny()
{
	if ( !m_pBitmap )
	{
		MessageBox( "请打开图片！", MB_OK );
		return;
	}

	if ( m_pCannyDlgEx ) delete m_pCannyDlgEx;
	m_pCannyDlgEx = new CCannyDlgEx( this );
	m_pCannyDlgEx->Create( IDD_DIALOG16 );
	m_pCannyDlgEx->ShowWindow( SW_SHOWNORMAL );
}


LRESULT CImgPrcView::OnOpencvsobel( WPARAM wParam, LPARAM lParam )
{
	if ( !m_pBitmap )
	{
		return NULL;
	}

	int type = m_flag == 24 ? CV_8UC3 : CV_8UC1;
	Mat img = Mat( m_nHeight, m_nWidth, type );
	memcpy( img.data, m_pBitmap, img.total() * m_flag / 8 );

	Mat imgRes;
	EdgeAlg::Sobel( img, imgRes, m_pSobelDlg->dx, m_pSobelDlg->dy, m_pSobelDlg->kSize, m_pSobelDlg->scale, m_pSobelDlg->delta, m_pSobelDlg->borderType );

	if ( m_pBmpProc ) delete[] m_pBmpProc;
	m_flagProc = 8 * imgRes.channels();
	m_nHeightProc = imgRes.rows;
	m_nWidthProc = imgRes.cols;
	m_pBmpProc = new BYTE[m_nHeightProc * m_nWidthProc * imgRes.channels()];
	memcpy( m_pBmpProc, imgRes.data, sizeof( BYTE ) * m_nHeightProc * m_nWidthProc * imgRes.channels() );

	//FreeInterResultImage();
	SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );
	Invalidate( FALSE );
	UpdateWindow();
	return LRESULT();
}


LRESULT CImgPrcView::OnRobert( WPARAM wParam, LPARAM lParam )
{
	if ( !m_pBitmap )
	{
		return LRESULT();
	}

	int type = m_flag == 24 ? CV_8UC3 : CV_8UC1;
	Mat img = Mat( m_nHeight, m_nWidth, type );
	memcpy( img.data, m_pBitmap, img.total() * m_flag / 8 );

	int dx = m_pRobertDlg->m_xCheck.GetCheck();
	int dy = m_pRobertDlg->m_yCheck.GetCheck();

	Mat imgRes;
	if ( !EdgeAlg::Robert( img, imgRes, dx, dy ) )
	{
		MessageBox( "没有选择X或Y方向", MB_OK );
		return LRESULT();
	}


	if ( m_pBmpProc ) delete[] m_pBmpProc;
	m_flagProc = 8 * imgRes.channels();
	m_nHeightProc = imgRes.rows;
	m_nWidthProc = imgRes.cols;
	m_pBmpProc = new BYTE[m_nHeightProc * m_nWidthProc * imgRes.channels()];
	memcpy( m_pBmpProc, imgRes.data, sizeof( BYTE ) * m_nHeightProc * m_nWidthProc * imgRes.channels() );

	//FreeInterResultImage();
	SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );
	Invalidate( FALSE );
	UpdateWindow();

	return LRESULT();
}


LRESULT CImgPrcView::OnOpencvcanny( WPARAM wParam, LPARAM lParam )
{
	if ( !m_pBitmap ) return LRESULT();

	int type = m_flag == 24 ? CV_8UC3 : CV_8UC1;
	Mat img = Mat( m_nHeight, m_nWidth, type );
	memcpy( img.data, m_pBitmap, img.total() * m_flag / 8 );

	Mat imgRes;
	EdgeAlg::Canny( img, imgRes, m_pCannyDlgEx->threshold1, m_pCannyDlgEx->threshold2, m_pCannyDlgEx->sobelSize, m_pCannyDlgEx->L2gradient );

	if ( m_pBmpProc ) delete[] m_pBmpProc;
	m_flagProc = 8 * imgRes.channels();
	m_nHeightProc = imgRes.rows;
	m_nWidthProc = imgRes.cols;
	m_pBmpProc = new BYTE[m_nHeightProc * m_nWidthProc * imgRes.channels()];
	memcpy( m_pBmpProc, imgRes.data, sizeof( BYTE ) * m_nHeightProc * m_nWidthProc * imgRes.channels() );

	//FreeInterResultImage();
	SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );
	Invalidate( FALSE );
	UpdateWindow();

	return LRESULT();
}


LRESULT CImgPrcView::OnCancelopera( WPARAM wParam, LPARAM lParam )
{
	FreeInterResultImage();
	Invalidate( TRUE );
	UpdateWindow();
	return LRESULT();
}


LRESULT CImgPrcView::OnOpencvlaplacian( WPARAM wParam, LPARAM lParam )
{
	if ( !m_pBitmap )
	{
		return NULL;
	}

	int type = m_flag == 24 ? CV_8UC3 : CV_8UC1;
	Mat img = Mat( m_nHeight, m_nWidth, type );
	memcpy( img.data, m_pBitmap, img.total() * m_flag / 8 );

	Mat imgRes;
	EdgeAlg::Laplacian( img, imgRes, m_pLaplacianDlg->sobelSize, m_pLaplacianDlg->scale, m_pLaplacianDlg->delta, m_pLaplacianDlg->borderType );

	if ( m_pBmpProc ) delete[] m_pBmpProc;
	m_flagProc = 8 * imgRes.channels();
	m_nHeightProc = imgRes.rows;
	m_nWidthProc = imgRes.cols;
	m_pBmpProc = new BYTE[m_nHeightProc * m_nWidthProc * imgRes.channels()];
	memcpy( m_pBmpProc, imgRes.data, sizeof( BYTE ) * m_nHeightProc * m_nWidthProc * imgRes.channels() );

	//FreeInterResultImage();
	SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );
	Invalidate( FALSE );
	UpdateWindow();
	return LRESULT();
}


void CImgPrcView::OnLaplacian()
{
	if ( !m_pBitmap )
	{
		MessageBox( "请打开图片！", MB_OK );
		return;
	}

	if ( m_pLaplacianDlg ) delete m_pLaplacianDlg;
	m_pLaplacianDlg = new CLaplacianDlg( this );
	m_pLaplacianDlg->Create( IDD_DIALOG17 );
	m_pLaplacianDlg->ShowWindow( SW_SHOWNORMAL );
}


void CImgPrcView::OnGaborOperator()
{
	if ( !m_pBitmap )
	{
		MessageBox( "请打开图片！", MB_OK );
		return;
	}

	if ( m_pGaborDlg ) delete m_pGaborDlg;
	m_pGaborDlg = new CGaborDlg( this );
	m_pGaborDlg->Create( IDD_DIALOG18 );
	m_pGaborDlg->ShowWindow( SW_SHOWNORMAL );
}


LRESULT CImgPrcView::OnGaborMessoperator( WPARAM wParam, LPARAM lParam )
{
	if ( !m_pBitmap )
	{
		return NULL;
	}

	int type = m_flag == 24 ? CV_8UC3 : CV_8UC1;
	Mat img = Mat( m_nHeight, m_nWidth, type );
	memcpy( img.data, m_pBitmap, img.total() * m_flag / 8 );

	Mat imgRes;
	EdgeAlg::Gabor( img, imgRes, m_pGaborDlg->m_kernelSize, m_pGaborDlg->m_sigma,
		m_pGaborDlg->m_theta, m_pGaborDlg->m_lambda, m_pGaborDlg->m_gamma,
		m_pGaborDlg->m_phaseOffset, m_pGaborDlg->m_anchorPoint, m_pGaborDlg->m_filterDelta,
		m_pGaborDlg->m_borderType );

	if ( m_pBmpProc ) delete[] m_pBmpProc;
	m_flagProc = 8 * imgRes.channels();
	m_nHeightProc = imgRes.rows;
	m_nWidthProc = imgRes.cols;
	m_pBmpProc = new BYTE[m_nHeightProc * m_nWidthProc * imgRes.channels()];
	memcpy( m_pBmpProc, imgRes.data, sizeof( BYTE ) * m_nHeightProc * m_nWidthProc * imgRes.channels() );

	//FreeInterResultImage();
	SetInterResultImage( 0, m_pBmpProc, m_nHeightProc, m_nWidthProc, m_flagProc, "" );
	Invalidate( FALSE );
	UpdateWindow();
	return LRESULT();
}


// 处理鼠标滚动消息
BOOL CImgPrcView::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt )
{
	// 判断按键是否按下
	BOOL bCtrlKeyDwn = ( GetKeyState( VK_LCONTROL ) & 0x8000 || GetKeyState( VK_RCONTROL ) & 0x8000 );
	BOOL bShftKeyDwn = ( GetKeyState( VK_LSHIFT ) & 0x8000 || GetKeyState( VK_RSHIFT ) & 0x8000 );

	// 移动当前显示窗口
	CRect rectClient;
	GetClientRect( &rectClient );
	double dRatio = m_rectShow.Width() / rectClient.Width();

	// Ctrl按键没有按下，则认为是拖动
	if ( !bCtrlKeyDwn )
	{
		// 移动显示矩形框		
		if ( bShftKeyDwn )
			m_rectShow.OffsetRect( zDelta > 0 ? -20 * dRatio : 20 * dRatio, 0 );
		else
			m_rectShow.OffsetRect( 0, zDelta < 0 ? 20 * dRatio : -20 * dRatio );

		Invalidate( FALSE );
		return TRUE;
	}

	// 获取放缩后的尺寸
	dRatio = m_dScaleRatio;
	dRatio *= ( zDelta > 0 ? 0.8 : 1.25 );

	// 避免过大
	double dMax = 0x07FFFFFF;
	//if( zDelta > 0 && m_DocSize.cx > dMax * dRatio )	return TRUE;
	//if( zDelta > 0 && m_DocSize.cy > dMax * dRatio )	return TRUE;

	// 缩放当前显示窗口:
	GetClientRect( &rectClient );
	ClientToScreen( &rectClient );

	CRect rectTotal;
	CalTotalRect( GetDC(), rectTotal );

	// 限制放大倍数，过大则返回
	if ( ( m_rectShow.Width() < 50 && zDelta > 0 ) || ( m_rectShow.Width() > 200 * rectTotal.Width() && zDelta <= 0 ) )
		return TRUE;

	pt.x -= rectClient.left;
	pt.y -= rectClient.top;

	double dX, dY;
	dX = pt.x * m_dScaleRatio + m_rectShow.left;
	dY = pt.y * m_dScaleRatio + m_rectShow.top;

	// 计算两次比例缩放之比
	dRatio /= m_dScaleRatio;

	// 以当前点为中心进行缩放
	m_rectShow.left = ( m_rectShow.left - dX ) * dRatio + dX;
	m_rectShow.right = ( m_rectShow.right - dX ) * dRatio + dX;
	m_rectShow.top = ( m_rectShow.top - dY ) * dRatio + dY;
	m_rectShow.bottom = ( m_rectShow.bottom - dY ) * dRatio + dY;

	m_dScaleRatio = m_rectShow.Width() / rectClient.Width();

	Invalidate( FALSE );
	return TRUE;
}


BOOL CImgPrcView::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message )
{
	return TRUE;
}

// 处理鼠标拖动消息
void CImgPrcView::OnMouseMove( UINT nFlags, CPoint point )
{
	if ( m_bMouseDown )
		m_bDragging = TRUE;

	CRect rectClient;
	GetClientRect( &rectClient );
	CString strText;
	strText.Format( "%ld,	%ld", point.x, point.y );
	CString strTitle;
	int r, g, b;
	double dx, dy;
	GetCurMouse( dx, dy, r, g, b, strTitle );
	CString strCord;
	if ( dx >= 0 )
	{
		CString str1;
		str1.Format( "%.3f", dx );
		str1.TrimRight( "0" );
		str1.TrimRight( "." );

		strCord = str1;
		str1.Format( "%.3f", dy );
		str1.TrimRight( "0" );
		str1.TrimRight( "." );
		strCord = strCord + ",   " + str1;
	}
	CString strrgb;
	if ( r >= 0 )
		strrgb.Format( "%ld,	%ld,	%ld", r, g, b );
	ShowCursorInfo( strText, strCord, strrgb, strTitle );

	if ( m_bMouseDown )
	{
		SetCursor( LoadCursor( NULL, IDC_HAND ) );

		double dRatio = m_rectShow.Width() / rectClient.Width();
		if ( 0 )//m_bSelecting )
		{
			g_rctSel.right = ( point.x ) * dRatio + m_rectShow.left;
			g_rctSel.bottom = ( point.y ) * dRatio + m_rectShow.top;
		}
		else
		{
			m_rectShow.OffsetRect( ( m_MousePoint.x - point.x )*dRatio, ( m_MousePoint.y - point.y )*dRatio );
		}
	}

	// 移动当前显示窗口
	m_MousePoint = point;

	Invalidate( FALSE );

	CScrollView::OnMouseMove( nFlags, point );
}

void CImgPrcView::OnRButtonUp( UINT nFlags, CPoint point )
{
	//ReleaseCapture();	
	SetCursor( LoadCursor( NULL, IDC_ARROW ) );
	Invalidate( TRUE );
	UpdateWindow();
	CScrollView::OnRButtonUp( nFlags, point );
}

void CImgPrcView::OnRButtonDblClk( UINT nFlags, CPoint point )
{
	CRect rectClient;
	GetClientRect( &rectClient );

	m_dScaleRatio = 1.0;
	m_rectShow = CFRect( rectClient );

	Invalidate( TRUE );
	UpdateWindow();
}

void CImgPrcView::DrawSelection( Mat& img, const cv::Scalar &color )
{
	cv::rectangle( img, cv::Point( g_rctSel.left, g_rctSel.top ), cv::Point( g_rctSel.right, g_rctSel.bottom ), color );
}

void CImgPrcView::OnLButtonDown( UINT nFlags, CPoint point )
{
	m_bMouseDown = TRUE;
	m_bDragging = FALSE;
	m_MousePoint = point;

	CRect rectClient;
	GetClientRect( &rectClient );

	if ( nFlags&MK_CONTROL )
	{
		m_bSelecting = TRUE;

		double dRatio = m_rectShow.Width() / rectClient.Width();
		g_rctSel.right = g_rctSel.left = ( point.x ) * dRatio + m_rectShow.left;
		g_rctSel.bottom = g_rctSel.top = ( point.y ) * dRatio + m_rectShow.top;
	}

	SetCursor( LoadCursor( NULL, IDC_HAND ) );
	CScrollView::OnLButtonDown( nFlags, point );
}
void CImgPrcView::OnLButtonUp( UINT nFlags, CPoint point )
{
	if ( !m_bMouseDown ) return;
	m_bMouseDown = FALSE;
	if ( !m_pBitmap ) return;

	CRect rectClient;
	GetClientRect( &rectClient );

	CString strTitle;
	int r, g, b;
	double dx, dy;
	GetCurMouse( dx, dy, r, g, b, strTitle );

	if ( !m_bDragging )
	{
		if ( strTitle.Compare( "original image" ) == 0 )
		{
			int nID = g_nCornerID;
			double dMin = 50;
			for ( int i = 0; i < 4; i++ )
			{
				double dTemp = sqrt( pow( dx - g_dCorner[i].x, 2.0 ) + pow( dy - g_dCorner[i].y, 2.0 ) );
				if ( dMin < dTemp ) continue;
				dMin = dTemp;
				nID = i;
			}
			g_nCornerType[nID] = nFlags & MK_SHIFT ? !g_nCornerType[nID] : g_nCornerType[nID];
			g_dCorner[nID] = Point2d( dx, dy );

			g_nCornerID = ( g_nCornerID + 1 ) % 4;
		}
	}



	/*if ( nFlags&MK_CONTROL )
	{
	double dRatio = m_rectShow.Width() / rectClient.Width();
	g_rctSel.right = ( point.x ) * dRatio + m_rectShow.left;;
	g_rctSel.bottom = ( point.y ) * dRatio + m_rectShow.top;

	if ( g_rctSel.left > g_rctSel.right )
	swap( g_rctSel.left, g_rctSel.right );
	if ( g_rctSel.top > g_rctSel.bottom )
	swap( g_rctSel.top, g_rctSel.bottom );


	Mat frame( m_nHeight, m_nWidth, m_flag == 8 ? CV_8UC1 : CV_8UC3, m_pBitmap );
	//imshow( "12",frame );
	Rect2d roi;
	roi = Rect2d( g_rctSel.left, g_rctSel.top, g_rctSel.Width(), g_rctSel.Height() );

	for ( auto iter = g_trackers.begin(); iter != g_trackers.end(); iter++ )
	( *iter ).release();

	g_trackers.clear();
	g_rctTrckObjs.clear();

	for ( int i = 0; i < g_nTrackerCnt; i++ )
	{
	g_trackers.push_back( Tracker::create( g_strTracker[i] ) );
	g_rctTrckObjs.push_back( roi );

	Rect2d roi = g_rctTrckObjs.at( i );
	Rect rct = Rect( roi.x, roi.y, roi.width, roi.height );
	if ( roi.x + roi.width >= frame.cols || roi.y + roi.height >= frame.rows || roi.x < 0 || roi.y < 0 )
	return;

	Mat img2;
	frame( rct ).copyTo( img2 );
	SetInterResultImage( i, img2.data, rct.height, rct.width, 24, "" );

	g_trackers.at( i )->init( frame, roi );
	}
	g_rctTrck = g_rctTrckObjs.at( 0 );


	m_bTracking = TRUE;
	m_bSelecting = FALSE;

	g_templ = Mat( 1, 1, CV_8UC1 );
	prevPts.clear();
	currPts.clear();
	}	*/

	Invalidate( FALSE );
	UpdateWindow();

	CScrollView::OnLButtonUp( nFlags, point );
}

void CImgPrcView::OnSize( UINT nType, int cx, int cy )
{
	CScrollView::OnSize( nType, cx, cy );

	CRect rectClient;
	GetClientRect( &rectClient );
	if ( cx < 1 || cy < 1 )
		return;
	m_rectShow = CFRect( rectClient );
}


void CImgPrcView::OnLoGandDoG()
{
	if ( !m_pBitmap )
	{
		MessageBox( "请打开图片.", MB_OK );
		return;
	}

	// TODO: 
	if ( m_pLogdogDlg ) delete m_pLogdogDlg;
	m_pLogdogDlg = new logdogDlg( this );
	m_pLogdogDlg->Create( IDD_DIALOG8 );
	m_pLogdogDlg->ShowWindow( SW_SHOWNORMAL );
	// TODO: Add your command handler code here
}

void CImgPrcView::OnLastframe()
{
	if ( !m_pVideo->isOpened() )
		return;

	//FreeInterResultImage();
	if ( m_pBitmap2 )
		SetInterResultImage( 0, m_pBitmap2, m_nHeight, m_nWidth, m_flag, "BK" );
	else
		;// return;


	m_nFrmID = max( 1, m_nFrmID - 1 );

	m_pVideo->set( CV_CAP_PROP_POS_FRAMES, m_nFrmID );
	m_nFrmID = m_pVideo->get( CV_CAP_PROP_POS_FRAMES );

	Mat img1;
	m_pVideo->read( img1 );

	resize( img1, img1, Size( 960, 540 ) );
	//UpdateImgFromROI( img1 );
	Process( img1 );

	Invalidate( FALSE );
	UpdateWindow();

	CString strTemp;
	strTemp.Format( "%ld/%ld", m_nFrmID, m_nFrmCnt );
	GetDocument()->SetTitle( strTemp );
}

void CImgPrcView::UpdateImgFromROI( Mat& srcImg )
{
	Mat imageROI;
	//imageROI = img1( rectROI );

	Point2f center( rectROI.x + rectROI.width / 2, rectROI.y + rectROI.height / 2 );//旋转中心  
	Mat M = getRotationMatrix2D( center, ROTATE, 1 );//计算旋转的仿射变换矩阵  

	warpAffine( srcImg, srcImg, M, srcImg.size() );//仿射变换  
	imageROI = srcImg( rectROI );

	Mat img( m_nHeight, m_nWidth, CV_8UC3, m_pBitmap );
	resize( imageROI, img, img.size(), 0, 0, INTER_NEAREST );
	//memcpy( m_pBitmap, imageROI.data, m_nHeight*m_nWidth*m_flag / 8 );
}

bool FindNextFileName( CString &FileName, int iStep )
{
	int nCurrentPosition;
	int position = FileName.Find( ".bmp" );
	if ( position < 0 )
		position = FileName.Find( ".BMP" );
	position--;

	while ( iStep > 0 )
	{
		nCurrentPosition = position;
		char ch = FileName.GetAt( nCurrentPosition );
		while ( ch == '9' && nCurrentPosition >= 0 )
		{
			FileName.SetAt( nCurrentPosition, '0' );
			nCurrentPosition--;
			ch = FileName.GetAt( nCurrentPosition );
		}
		ch++;
		FileName.SetAt( nCurrentPosition, ch );

		iStep--;
	}
	return true;
}

const size_t choice = 3;
// choice
// 1    2    3
// calcOpticalFlowFarneback   calcOpticalFlowSF   calcOpticalFlowPyrLK
void drawOptFlowMap( const Mat& flow, Mat& cflowmap, int step, const Scalar& color )
{
	// cflowmap is the pre frame with the line of Optical Flow
	// flow is a V-by-2 array, store float point
	//      store the delta of x,y
	// step is every step pixel
	for ( int y = 0; y < cflowmap.rows; y += step )
		for ( int x = 0; x < cflowmap.cols; x += step )
		{
			const Point2f& fxy = flow.at< Point2f>( y, x );
			line( cflowmap, Point( x, y ), Point( cvRound( x + fxy.x ), cvRound( y + fxy.y ) ),
				color );
			circle( cflowmap, Point( cvRound( x + fxy.x ), cvRound( y + fxy.y ) ), 1, color, -1 );
		}
}
void drawOptFlowMap( Mat& cflowmap, int step, const Scalar& color, vector<Point2f> &retPts )
{
	// same as above, retPts is the next frame point
	auto it = retPts.begin();
	for ( int y = 0; y < cflowmap.rows; y += step )
		for ( int x = 0; x < cflowmap.cols; x += step )
		{
			line( cflowmap, Point( x, y ), *it, color );
			circle( cflowmap, *it, 1, color, -1 );
			it++;
		}
}

void drawOptFlowMap( Mat &flow, Mat& cflowmap )
{
	// same as above, retPts is the next frame point
	for ( int y = 0; y < cflowmap.rows; y++ )
		for ( int x = 0; x < cflowmap.cols; x++ )
		{
			const Point2f& fxy = flow.at< Point2f>( y, x );
			cflowmap.at<BYTE>( y, x ) = MIN( 255, norm( fxy ) * 50 );
		}
}



void CImgPrcView::OnNextframe()
{
	return;
	g_dwStartTime = GetTickCount();
	int nStartFrame = m_nFrmID;
	if ( m_nFileSource == 1 )
	{
		/*{
		m_nFrmID++;

		Mat img1( m_nHeight, m_nWidth, CV_8UC3, m_pBitmap );

		Mat flow; //flow = aft - pre
		Mat pre = img1.clone();

		FindNextFileName( m_strFileName, 8 );
		LoadBitmap();

		Mat img2( m_nHeight, m_nWidth, CV_8UC3, m_pBitmap );
		Mat aft = img2.clone();

		Mat cflow1 = pre;
		Mat cflow2 = aft;// store the  3-channel mat of frame, cflow is for show color with line

		cvtColor( aft, aft, CV_BGR2GRAY );
		cvtColor( pre, pre, CV_BGR2GRAY );

		calcOpticalFlowFarneback( pre, aft, flow, 0.5, 1, 3, 3, 5, 1.2, 0 ); // info in the flow; note that input mat should in 1-channel
		//drawOptFlowMap( flow, cflow1, 2, CV_RGB( 0, 255, 0 ) );

		cvtColor( cflow1, cflow1, CV_BGR2GRAY );
		drawOptFlowMap( flow, cflow1 );

		SetInterResultImage( 0, cflow1, "cflow1" );

		drawOptFlowMap( flow, cflow2, 5, CV_RGB( 0, 255, 0 ) );
		SetInterResultImage( 1, cflow2, "cflow1" );

		SetInterResultImage( 1, cflow2, "cflow1" );


		Invalidate( FALSE );
		UpdateWindow();

		GetDocument()->SetTitle( m_strFileName );
		}*/
		//do
		{	m_nFrmID++;

		Mat img1( m_nHeight, m_nWidth, CV_8UC3, m_pBitmap );
		Process( img1 );

		FindNextFileName( m_strFileName, 2 );
		LoadBitmap();

		Invalidate( FALSE );
		UpdateWindow();

		GetDocument()->SetTitle( m_strFileName );
		}//while ( m_nFrmID < 348 );

		return;
	}

NEXT:
	if ( !m_pVideo->isOpened() && m_nFileSource == 2 )
		return;

	//m_nFrmID = max( 1, m_nFrmID + 1 );
	//m_pVideo->set( CV_CAP_PROP_POS_FRAMES, m_nFrmID );

	Mat img1;
	for ( int i = 0; i < 2; i++ )
		m_pVideo->read( img1 );
	m_nFrmID = m_pVideo->get( CV_CAP_PROP_POS_FRAMES );

	if ( img1.rows < 1 )
		return;

	resize( img1, img1, Size( 960, 540 ), 0, 0, INTER_NEAREST );
	img1 = img1( rectROI ).clone();
	//UpdateImgFromROI( img1 );

	memcpy( m_pBitmap, img1.data, m_nHeight*m_nWidth*m_flag / 8 );
	Process( img1 );

	/*if ( m_bTracking )
	{
	for ( int i = 0; i < g_nTrackerCnt; i++ )
	{
	g_trackers.at( i )->update( img1, g_rctTrckObjs.at( i ) );

	Rect2d roi = g_rctTrckObjs.at( i );
	Rect rct = Rect( roi.x, roi.y, roi.width, roi.height );
	Mat img2;
	img1( rct ).copyTo( img2 );
	SetInterResultImage( 7+i, img2, "" );
	}
	for ( int i = 0; i < g_nTrackerCnt; i++ )
	cv::rectangle( img1, g_rctTrckObjs.at( i ), g_crObjs[i] );
	SetInterResultImage( 7 + g_nTrackerCnt, img1, "" );
	}*/


	Invalidate( FALSE );
	UpdateWindow();

	CString strTemp;
	strTemp.Format( "%ld/%ld    %.2f", m_nFrmID, m_nFrmCnt, ( m_nFrmID - nStartFrame ) / (double) ( ( GetTickCount() - g_dwStartTime ) / 1000.0 ) );
	GetDocument()->SetTitle( strTemp );

#ifndef _DEBUG
	if ( m_nFrmID < 291000 ) goto NEXT;
#endif
}


void CImgPrcView::OnSetbg()
{
	delete m_pBitmap2;
	m_pBitmap2 = new BYTE[m_nHeight*m_nWidth*m_flag / 8];
	memcpy( m_pBitmap2, m_pBitmap, m_nHeight*m_nWidth*m_flag / 8 );

	//FreeInterResultImage();
	SetInterResultImage( 0, m_pBitmap2, m_nHeight, m_nWidth, m_flag, "BK" );

	Invalidate( FALSE );
	UpdateWindow();
}


void CImgPrcView::OnTrafficBody()
{
	OnSetbg();
	//Process();

	Invalidate( FALSE );
	UpdateWindow();
}

LRESULT CImgPrcView::OnFeatureTrack( WPARAM wParam, LPARAM lParam )
{
	if ( !m_pBitmap )
	{
		return NULL;
	}

	return LRESULT();
}

void CImgPrcView::OnTrackfeature()
{
	if ( m_pFtrTrack ) delete m_pFtrTrack;
	m_pFtrTrack = new CDlgFtrTrack( this );
	m_pFtrTrack->Create( IDD_DLGTRACK );
	m_pFtrTrack->ShowWindow( SW_SHOWNORMAL );
}


// old 
/*void CImgPrcView::Process()
{
BYTE* pTemp = new BYTE[m_nHeight*m_nWidth];
for ( int i = 0; i < m_nHeight*m_nWidth; i++ )
pTemp[i] = max( abs( m_pBitmap[i * 3 + 0] - m_pBitmap2[i * 3 + 0] ), max( abs( m_pBitmap[i * 3 + 1] - m_pBitmap2[i * 3 + 1] ), abs( m_pBitmap[i * 3 + 2] - m_pBitmap2[i * 3 + 2] ) ) );

SetInterResultImage( 1, pTemp, m_nHeight, m_nWidth, 8, "diff" );
delete pTemp;

Mat srcImg = Mat( m_nHeight, m_nWidth, CV_8UC3, m_pBitmap );
Mat imageROI;

srcImg.copyTo( imageROI );
mog->setVarThreshold( 20 );

Mat mask;
mog->apply( imageROI, mask, -1 );

SetInterResultImage( 2, mask.data, m_nHeight, m_nWidth, 8, "mog" );

/*{
vector<Rect> found;

//进行检测
bodyHog.detectMultiScale( mask, found, 0, Size( 8, 8 ), Size( 32, 32 ), 1.31, 2.0, false );

//画长方形，圈出行人
for ( int i = 0; i < found.size(); i++ )
rectangle( imageROI, found[i].tl(), found[i].br(), Scalar( 0, 0, 255 ), 3 );

Mat img( m_nHeight, m_nWidth, CV_8UC3 );
resize( imageROI, img, img.size(), 0, 0, INTER_NEAREST );

SetInterResultImage( 3, img.data, m_nHeight, m_nWidth, 24, "hog" );
}

{
Mat result = mask.clone();

vector<Rect> found;
cascade->detectMultiScale( result, found, 1.1, 1, CV_HAAR_DO_CANNY_PRUNING );

for ( int i = 0; i < found.size(); i++ )
rectangle( result, found[i].tl(), found[i].br(), Scalar( 255, 255, 255 ), 3 );

Mat img( m_nHeight, m_nWidth, CV_8UC1 );
resize( result, img, img.size(), 0, 0, INTER_NEAREST );
SetInterResultImage( 3, img.data, m_nHeight, m_nWidth, img.channels()*8, "cascade" );
}
{
std::vector<cv::Point> corners;
Mat result = mask.clone();

goodFeaturesToTrack( result, corners,
100,    // 角点最大数目
0.001,  // 质量等级，这里是0.01*max（min（e1，e2）），e1，e2是harris矩阵的特征值
6 );    // 两个角点之间的距离容忍度

cvtColor( result, result, CV_GRAY2RGB );
proc::Harris().drawOnImage( result, corners, cv::Scalar( 0, 0, 255 ) );//标记角点

Mat img( m_nHeight, m_nWidth, CV_8UC1 );
resize( result, img, img.size(), 0, 0, INTER_NEAREST );
SetInterResultImage( 4, img.data, m_nHeight, m_nWidth, img.channels() * 8, "goodFeaturesToTrack" );
}

{
static std::vector<Point2f> prevPts, currPts, newPts, savePts;
std::vector<uchar> status;
std::vector<float> errors;
Mat prevDesc, currDesc;

Size window = Size( 6, 6 );

Mat frame = mask.clone();
static Mat previousFrameBW;
static Mat currentFrameBW;
cvtColor( mask, frame, CV_GRAY2RGB );
cvtColor( frame, currentFrameBW, CV_RGB2GRAY );

savePts.clear();
currPts.clear();

if ( !prevPts.empty() )
{
calcOpticalFlowPyrLK( previousFrameBW, currentFrameBW, prevPts, currPts, status, errors, window, 5,
TermCriteria( static_cast<int>( TermCriteria::COUNT ) + static_cast<int>( TermCriteria::EPS ), 20, 0.183328975142 ),
0, 1e-4 );

for ( int i = 0; i < currPts.size() && i < prevPts.size(); i++ )
{
if ( status[i] != !0 ) continue;
double dLen = norm( currPts[i] - prevPts[i] );
if ( dLen < 4 || dLen > 60 ) continue;

line( frame, prevPts[i], currPts[i], cv::Scalar( 0, 0, 255 ), 1 );
savePts.push_back( currPts[i] );
}
}

// adding new features, using mask around existing feature positions
Mat featureMask = Mat::ones( frame.size(), CV_8UC1);// mask.clone();
double minFeatureDisKLT = 1.54964337411;
for ( unsigned int n = 0; n < savePts.size(); n++ )
for ( int j = MAX( 0, savePts[n].x - minFeatureDisKLT ); j < MIN( m_nWidth, savePts[n].x + minFeatureDisKLT + 1 ); j++ )
for ( int i = MAX( 0, savePts[n].y - minFeatureDisKLT ); i < MIN( m_nHeight, savePts[n].y + minFeatureDisKLT + 1 ); i++ )
;// featureMask.at<uchar>( i, j ) = 0;

goodFeaturesToTrack( currentFrameBW, newPts, 100,    // 角点最大数目
0.001,  // 质量等级，这里是0.01*max（min（e1，e2）），e1，e2是harris矩阵的特征值
6, featureMask );

for ( unsigned int i = 0; i < newPts.size(); i++ )
savePts.push_back( newPts[i] );

Mat img( m_nHeight, m_nWidth, CV_8UC1 );
resize( frame, img, img.size(), 0, 0, INTER_NEAREST );
SetInterResultImage( 4, img.data, m_nHeight, m_nWidth, img.channels() * 8, "goodFeaturesToTrack" );

previousFrameBW = currentFrameBW.clone();
prevPts = savePts;
}////

{
static std::vector<Point2f> prevPts, currPts, newPts, savePts;
std::vector<uchar> status;
std::vector<float> errors;
Mat prevDesc, currDesc;

Size window = Size( 6, 6 );

Mat frame = srcImg.clone();
static Mat previousFrameBW;
static Mat currentFrameBW;
cvtColor( frame, currentFrameBW, CV_RGB2GRAY );

savePts.clear();
currPts.clear();

if ( !prevPts.empty() )
{
calcOpticalFlowPyrLK( previousFrameBW, currentFrameBW, prevPts, currPts, status, errors, window, 0,
TermCriteria( static_cast<int>( TermCriteria::COUNT ) + static_cast<int>( TermCriteria::EPS ), 20, 0.183328975142 ),
0, 1e-4 );

for ( int i = 0; i < currPts.size() && i < prevPts.size(); i++ )
{
if ( status[i] != !0 ) continue;
double dLen = norm( currPts[i] - prevPts[i] );
if ( dLen < 4 || dLen > 60 ) continue;

line( frame, prevPts[i], currPts[i], cv::Scalar( 0, 0, 255 ), 1 );
savePts.push_back( currPts[i] );
}
}

// adding new features, using mask around existing feature positions
Mat featureMask = Mat::ones( frame.size(), CV_8UC1 );// mask.clone();
double minFeatureDisKLT = 1.54964337411;
for ( unsigned int n = 0; n < savePts.size(); n++ )
for ( int j = MAX( 0, savePts[n].x - minFeatureDisKLT ); j < MIN( m_nWidth, savePts[n].x + minFeatureDisKLT + 1 ); j++ )
for ( int i = MAX( 0, savePts[n].y - minFeatureDisKLT ); i < MIN( m_nHeight, savePts[n].y + minFeatureDisKLT + 1 ); i++ )
featureMask.at<uchar>( i, j ) = 0;

goodFeaturesToTrack( currentFrameBW, newPts, 100,    // 角点最大数目
0.08122,  // 质量等级，这里是0.01*max（min（e1，e2）），e1，e2是harris矩阵的特征值
3.5496, featureMask, 7, false, 0.4 );

{
for ( int i = 0; i < newPts.size(); i++ )
{
line( frame, Point2f( newPts[i].x - 5, newPts[i].y ), Point2f( newPts[i].x + 5, newPts[i].y ), cv::Scalar( 255, 0, 0 ), 1 );
line( frame, Point2f( newPts[i].x, newPts[i].y - 5 ), Point2f( newPts[i].x, newPts[i].y + 5 ), cv::Scalar( 255, 0, 0 ), 1 );
}
}

for ( unsigned int i = 0; i < newPts.size(); i++ )
savePts.push_back( newPts[i] );

Mat img( m_nHeight, m_nWidth, CV_8UC1 );
resize( frame, img, img.size(), 0, 0, INTER_NEAREST );
SetInterResultImage( 3, img.data, m_nHeight, m_nWidth, img.channels() * 8, "goodFeaturesToTrack" );

previousFrameBW = currentFrameBW.clone();
prevPts = savePts;
}

Invalidate( FALSE );
UpdateWindow();
}*/

inline Rect GetSurroundBox( vector<Point> & pts )
{
	Rect rctObj( 0, 0, 0, 0 );

	int nl, nr, nt, nb;
	nl = nt = 99999;
	nr = nb = -1;
	for ( int j = 0; j < pts.size(); j++ )
	{
		nl = nl < pts[j].x ? nl : pts[j].x;
		nt = nt < pts[j].y ? nt : pts[j].y;
		nr = nr > pts[j].x ? nr : pts[j].x;
		nb = nb > pts[j].y ? nb : pts[j].y;
	}
	if ( nr - nl < 2 || nb - nt < 2 )
		return rctObj;

	rctObj.x = nl;
	rctObj.y = nt;
	rctObj.width = nr - nl;
	rctObj.height = nb - nt;

	return rctObj;
}

void CombinObjects( std::vector<Rect> &rctObjs, int nMaxDist )
{
	std::vector<Rect> rest;

	bool bcontinue = true;
	while ( bcontinue )
	{
		bcontinue = false;
		for ( int i = 0; i < rctObjs.size(); i++ )
		{
			Rect rct = ( rctObjs[i] + Point( -nMaxDist, -nMaxDist ) ) + Size( 2 * nMaxDist, 2 * nMaxDist );

			for ( int j = i + 1; j < rctObjs.size(); j++ )
			{
				if ( !( rct & rctObjs[j] ).area() )
					continue;

				if ( rctObjs[i].width > 20 && rctObjs[i].height > 30 && rctObjs[j].width > 20 && rctObjs[j].height > 30 )
					continue;
				rctObjs[i] |= rctObjs[j];
				swap( rctObjs[j], rctObjs[rctObjs.size() - 1] );
				rctObjs.pop_back();

				rct = ( rctObjs[i] + Point( -nMaxDist, -nMaxDist ) ) + Size( 2 * nMaxDist, 2 * nMaxDist );
				bcontinue = true;
				break;
			}
		}
	}
}

void CImgPrcView::GetCentroidBox( Mat& img, Rect2d& rctObj, int nMaxW, int nMaxH )
{
	Mat fmask;
	if ( rctObj.area() < 1 ) return;
	Rect2d rctImg( 0, 0, img.cols - 1, img.rows - 1 );
	rctObj = rctObj & rctImg;

	fmask = img( rctObj ).clone();

	int nW = fmask.cols;
	int nH = fmask.rows;

	Mat mRow( 1, nW, CV_8UC1, Scalar( 0 ) );
	Mat mCol( nH, 1, CV_8UC1, Scalar( 0 ) );

	reduce( fmask, mRow, 0, REDUCE_AVG );
	reduce( fmask, mCol, 1, REDUCE_AVG );

	mCol = mCol.reshape( 0, 1 );

#ifdef _DEBUG

	{
		BYTE* pPrj = (BYTE*) mRow.data;

		Mat hisu( 64, mRow.cols, CV_8U, Scalar( 200 ) );
		for ( int i = 0; i < mRow.cols; i++ )
			for ( int j = 0; j < pPrj[i] / 4; j++ )
				hisu.data[( 63 - j )*mRow.cols + i] = 64;
		SetInterResultImage( 2, hisu, "mRow" );
	}
	{
		//normalize( mCol, mCol, 0.0, 63, NORM_MINMAX );
		BYTE* pPrj = (BYTE*) mCol.data;

		Mat hisu( 64, mCol.cols, CV_8U, Scalar( 200 ) );
		for ( int i = 0; i < mCol.cols; i++ )
			for ( int j = 0; j < pPrj[i] / 4; j++ )
				hisu.data[( 63 - j )*mCol.cols + i] = 64;
		SetInterResultImage( 2, hisu, "mCol" );
	}
#endif

	boxFilter( mRow, mRow, -1, Size( nMaxW, 1 ), Point( -1, -1 ), true, BORDER_CONSTANT );
	boxFilter( mCol, mCol, -1, Size( nMaxH, 1 ), Point( -1, -1 ), true, BORDER_CONSTANT );

#ifdef _DEBUG
	rectangle( img, rctObj, Scalar( 128 ) );
	SetInterResultImage( 2, img, "GetCentroidBox  before" );
#endif
	rctObj.width = MIN( nMaxW, rctObj.width );
	rctObj.height = MIN( nMaxH, rctObj.height );

	double minVal, maxVal;
	Point minLoc, maxLoc;
	minMaxLoc( mRow, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
	int i, j;
	for ( i = maxLoc.x; i < mRow.cols; i++ )
		if ( mRow.data[i] != mRow.data[maxLoc.x] ) break;
	int nX = ( i + maxLoc.x - rctObj.width ) / 2;
	minMaxLoc( mCol, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
	for ( j = maxLoc.x; j < mCol.cols; j++ )
		if ( mCol.data[j] != mCol.data[maxLoc.x] ) break;
	int nY = ( j + maxLoc.x - rctObj.height ) / 2;
#ifdef _DEBUG
	{
		BYTE* pPrj = (BYTE*) mRow.data;

		Mat hisu( 64, mRow.cols, CV_8U, Scalar( 200 ) );
		for ( int i = 0; i < mRow.cols; i++ )
			for ( int j = 0; j < pPrj[i] / 4; j++ )
				hisu.data[( 63 - j )*mRow.cols + i] = ( i == nX + rctObj.width / 2 ) ? 128 : 64;
		SetInterResultImage( 2, hisu, "mRow  boxFilter" );
	}
	{
		//normalize( mCol, mCol, 0.0, 63, NORM_MINMAX );
		BYTE* pPrj = (BYTE*) mCol.data;

		Mat hisu( 64, mCol.cols, CV_8U, Scalar( 200 ) );
		for ( int i = 0; i < mCol.cols; i++ )
			for ( int j = 0; j < pPrj[i] / 4; j++ )
				hisu.data[( 63 - j )*mCol.cols + i] = ( i == nY + rctObj.height / 2 ) ? 128 : 64;
		SetInterResultImage( 2, hisu, "mCol  boxFilter" );
	}
#endif		
	rctObj.x += nX;
	rctObj.y += nY;

	rctObj = rctObj & Rect2d( 5, 5, img.cols - 5, img.rows - 5 );

#ifdef _DEBUG	
	rectangle( img, rctObj, Scalar( 255 ) );
	SetInterResultImage( 2, img, "GetCentroidBox 1" );
#endif
}
void CImgPrcView::GetPedSearchArea( Mat& roadMaskU, Mat& roadMaskD, std::vector<Point2i >& pedRange )
{
	pedRange.clear();

	int nH = roadMaskU.rows;
	int nW = roadMaskU.cols;

	for ( int i = 0; i < roadMaskU.cols; i++ )
	{
		int y1, y2;
		BYTE* pData = roadMaskU.data;
		for ( y1 = 0; y1 < nH - 1 && !pData[y1*nW + i]; y1++ );

		pData = roadMaskD.data;
		for ( y2 = nH - 1; y2 >= y1 && !pData[y2*nW + i]; y2-- );

		y1 -= i * 20 / roadMaskU.cols;
		y2 += i * 20 / roadMaskU.cols;
		if ( y2 <= y1 )
			y1 = y2 = 0;

		pedRange.push_back( Point2i( y1, y2 ) );
	}
}

void CImgPrcView::GetPedSearchArea( Mat& roadMask, int x, int &y1, int &y2 )
{
	BYTE* pData = roadMask.data;
	int nH = roadMask.rows;
	int nW = roadMask.cols;

	for ( y1 = 0; y1 < nH - 1 && !pData[y1*nW + x]; y1++ );
	for ( y2 = y1; y2 < nH - 1 && pData[y2*nW + x]; y2++ );

	if ( y2 <= y1 )
		y1 = y2 = 0;
}

int CImgPrcView::CheckObjects( Mat& mask, Mat& roadMask, vector<Rect>& rctObjs, bool bPed, int* pnPedCnt )
{
	Mat fmask;
	Mat img;
	mask.copyTo( img, roadMask( Rect( roadMask.cols - mask.cols, 0, mask.cols, mask.rows ) ) );

	GetProject( img );

	bool bFoundPed = false;
	Rect2d rctObj;

	// get the rect for vehicle

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours( img, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );

	g_rctMogObjs.clear();

	if ( !contours.size() )
		return 0;

	for ( int idx = 0; idx >= 0; idx = hierarchy[idx][0] )
	{
		if ( hierarchy[idx][3] != -1 ) continue;

		Rect rctObj = GetSurroundBox( contours[idx] );

		if ( rctObj.width < 5 || rctObj.height < 5 )
			continue;
		rctObj += Point( -3, -3 );
		rctObj += Size( 6, 6 );
		rctObj = rctObj & Rect( 0, 0, img.cols, img.rows );
		g_rctMogObjs.push_back( rctObj );
	}

	Mat dst;
	cvtColor( img, dst, CV_GRAY2BGR );
	rctObj = Rect( 0, 0, 0, 0 );


	if ( bPed )
	{
		for ( int i = 0; i < g_rctMogObjs.size(); i++ )
		{
			int x = g_rctMogObjs[i].x;
			int y1, y2;
			GetPedSearchArea( roadMask, x + g_rctMogObjs[i].width / 2, y1, y2 );

			if ( x < 350 || x > img.cols - 30 || ( g_rctMogObjs[i].width < 20 && g_rctMogObjs[i].height < 20 ) ) continue;

			int y12 = ( y1 + y2 ) / 2;
			int nsz = ( x - 350 ) * 8.0 / 600.0 + 8;
			int nY = g_rctMogObjs[i].y + g_rctMogObjs[i].height / 2;

#ifdef _DEBUG
			line( dst, Point( x, y1 ), Point( x, y2 ), Scalar( 255, 255, 0 ), 2 );
			line( dst, Point( x - 3, y12 - nsz ), Point( x + 3, y12 - nsz ), Scalar( 0, 0, 255 ), 1 );
			line( dst, Point( x - 3, y12 + nsz ), Point( x + 3, y12 + nsz ), Scalar( 0, 0, 255 ), 1 );
			line( dst, Point( x - 9, nY ), Point( x + 9, nY ), Scalar( 0, 255, 0 ), 1 );
#endif

			if ( nY < y12 - nsz || nY > y12 + nsz ) continue; // 
			if ( g_rctMogObjs[i].width > 20 && g_rctMogObjs[i].height > 20 &&
				g_rctMogObjs[i].width < 70 && g_rctMogObjs[i].height < 60 )
			{
				bFoundPed = true;
#ifdef _DEBUG
				rectangle( dst, g_rctMogObjs[i], cv::Scalar( 0, 0, 255 ), 2 );
				line( dst, Point( x, y1 ), Point( x, y2 ), Scalar( 0, 255, 255 ), 1 );
#endif

				Rect2d rctPed = g_rctMogObjs[i];
				rctObjs.push_back( rctPed );
			}
		}
	}

	CombinObjects( rctObjs, 5 );

	CombinObjects( g_rctMogObjs, 20 );

	if ( pnPedCnt )
		*pnPedCnt = rctObjs.size();


	for ( int i = 0; i < g_rctMogObjs.size(); i++ )
	{
		int x = g_rctMogObjs[i].x + g_rctMogObjs[i].width;
		if ( x > img.cols - 30 || x < img.cols - 80 || g_rctMogObjs[i].height < 50 )
			rectangle( dst, g_rctMogObjs[i], cv::Scalar( 0, 255, 255 ) );
		else
		{
			rectangle( dst, g_rctMogObjs[i], cv::Scalar( 255, 255, 0 ), 2 );
			if ( g_rctMogObjs[i].area() > rctObj.area() )
				rctObj = g_rctMogObjs[i];
		}
	}

	//GetCentroidBox( mask.clone(), rctObj, 168, 90 );
	SetInterResultImage( 2, dst, "g_roadmsku" );


	if ( rctObj.width * rctObj.height > 100 )
	{
		double d1 = countNonZero( mask( rctObj ) ) / (double) ( rctObj.area() );
		if ( d1 > 0.2 )
			rctObjs.push_back( rctObj );
	}

	return ( rctObjs.size() ) ? 1 : 0;
}

void CImgPrcView::OnTrafficForward()
{
	CurrentFocusPos();

	Mat srcImg( 100, 10, CV_8UC1, Scalar( 128 ) );
	SetInterResultImage( 0, srcImg, "blank" );

	if ( g_nBackPos < 0 )
		return;

	Mat img, mask;
	unsigned int nID;
	g_nBackPos = g_buff.GetNextBuff( g_nBackPos, nID, img, mask );

	SetInterResultImage( 0, img, "img" );

	srcImg = img.clone();

	g_pTarget->Update( img, mask, nID );
	g_pTarget->Draw( srcImg );

	SetInterResultImage( 1, srcImg, "srcImg" );
	SetInterResultImage( 1, mask, "mask" );

	CString strTemp;
	strTemp.Format( "%ld", nID );
	GetDocument()->SetTitle( strTemp );

	Invalidate( FALSE );
	UpdateWindow();
}

void CImgPrcView::OnTrafficInittarget()
{
	CurrentFocusPos();
	if ( bFirst )
	{
		bFirst = false;
		nPos = g_buff.GetStartPos();
		nPos = g_buff.GetLastBuff( nPos );
	}
	BackProcess( g_pTarget, nPos, bRecapture, nFrame );

	Mat srcImg = g_pTarget->m_lastImg.clone();
	g_pTarget->Draw( srcImg );
	SetInterResultImage( 1, srcImg, "srcImg" );

	CString strTemp;
	strTemp.Format( "%ld", nPos );
	GetDocument()->SetTitle( strTemp );

	Invalidate( FALSE );
	UpdateWindow();
	return;

	Mat img, mask;
	unsigned int nID;
	g_nBackPos = g_buff.GetLastBuff( g_nBackPos, nID, img, mask );
	if ( g_nBackPos < 0 )
		return;

	Mat maskud = mask.clone();
	if ( g_pTarget->m_nDirct == 2 )
		maskud = maskud & g_roadmskd;
	else
		maskud = maskud & g_roadmsku;

	if ( g_pTarget )
		g_pTarget->Init( img, maskud, g_pTarget->m_rctBox, g_pTarget->m_nDirct, nID );
}

void CImgPrcView::OnTrafficBackward()
{
	CurrentFocusPos();

	Mat srcImg( 100, 10, CV_8UC1, Scalar( 128 ) );
	SetInterResultImage( 0, srcImg, "blank" );

	if ( g_nBackPos < 0 )
		return;

	Mat img, mask;
	unsigned int nID;
	g_nBackPos = g_buff.GetLastBuff( g_nBackPos, nID, img, mask );

	SetInterResultImage( 0, img, "img" );

	srcImg = img.clone();

	int nTest = 0;
	imwrite( format( "f:\\1\\Back_%ld_%ld.bmp", nTest, nID ), srcImg );

	Mat maskud = mask.clone();
	if ( g_pTarget->m_nDirct == 2 )
		maskud = maskud & g_roadmskd;
	else
		maskud = maskud & g_roadmsku;

	g_pTarget->UpdateBackward( img, maskud, nID );

	g_pTarget->Draw( srcImg );
	SetInterResultImage( 1, srcImg, "srcImg" );
	SetInterResultImage( 1, mask, "mask" );
	SetInterResultImage( 1, maskud, "mask" );

	CString strTemp;
	strTemp.Format( "%ld", nID );
	GetDocument()->SetTitle( strTemp );

	Invalidate( FALSE );
	UpdateWindow();
}

void CImgPrcView::BackProcess( CTarget* pTarget, int &nPos, bool &bRecapture, int& nFrame )
{
	if ( nPos >= 0 )
	{
		static Mat img, mask;
		static unsigned int nID = 0;

		nFrame++;

		//if ( !( nFrame > 5 && bRecapture ) )
		{
			nPos = g_buff.GetLastBuff( nPos, nID, img, mask );

			SetInterResultImage( 0, img, "img" );

			if ( !pTarget->UpdateBackward( img, mask, nID ) )
				return;

			/*if ( nFrame == 5 )
			{
			pTarget->Init( img, mask, pTarget->m_rctBox, pTarget->m_nDirct, nID );
			nPos = g_buff.GetNextBuff( nPos );
			nPos = g_buff.GetNextBuff( nPos );
			}*/
		}
		/*/if ( nFrame > 20 && bRecapture )
		else
		{
		if ( nPos >= 0 )
		{
		nPos = g_buff.GetNextBuff( nPos, nID, img, mask );

		SetInterResultImage( 0, img, "img" );
		pTarget->Update( img, mask, nID );
		SetInterResultImage( 1, img, "img" );
		}
		if( nPos < 0 )
		{
		bRecapture = false;
		nPos = g_buff.GetStartPos();
		pTarget->Init( img, mask, pTarget->m_rctBox, pTarget->m_nDirct, nID );
		nPos = g_buff.GetLastBuff( nPos );
		}
		}*/
	}
	//pTarget->PostProcess();
	//pTarget->Recover( t0 );
}


void CImgPrcView::BackProcess( CTarget*& pTarget )
{
	CTarget t0;
	pTarget->Backup( t0 );
	int nPos = g_buff.GetStartPos();
	nPos = g_buff.GetLastBuff( nPos );

	static int nTest = 0;
	nTest++;
	bool bRecapture = true;
	int nFrame = 0;
	while ( nPos >= 0 )
	{
		nFrame++;
		if ( pTarget->m_bType == 2 && nFrame > 100 ) //
			if ( !pTarget->IsPed() )
				break; // break;

		Mat img, mask;
		unsigned int nID;
		nPos = g_buff.GetLastBuff( nPos, nID, img, mask );
		//imwrite( format( "f:\\11\\Back_%ld.bmp", nID ), img );

#ifdef _DEBUG
		if ( nFrame == 1 )
		{
			SetInterResultImage( 1, img, "img" );
			SetInterResultImage( 1, mask, "mask" );
		}
#endif

		if ( !pTarget->UpdateBackward( img, mask, nID ) )
			break;

#ifdef _DEBUG		

		Mat srcImg = img.clone();
		pTarget->Draw( srcImg );
		imwrite( format( "f:\\1\\Back_%ld_%ld.bmp", nTest, nFrame ), srcImg );
#endif

		/*if ( nFrame > 5 && bRecapture )
		{
		pTarget->Init( img, mask, pTarget->m_rctBox, pTarget->m_nDirct, nID );

		//srcImg = img.clone();
		//pTarget->Draw( srcImg );
		//imwrite( format( "f:\\1\\Back_%ld.bmp", nTest++ ), srcImg );

		nPos = g_buff.GetNextBuff( nPos );
		nPos = g_buff.GetNextBuff( nPos );

		while ( nPos >= 0 )
		{
		nPos = g_buff.GetNextBuff( nPos, nID, img, mask );
		pTarget->Update( img, mask, nID );

		//srcImg = img.clone();
		//pTarget->Draw( srcImg );
		//imwrite( format( "f:\\1\\Back_%ld.bmp", nTest++ ), srcImg );

		//SetInterResultImage( 0, img, "img" );
		}
		bRecapture = false;

		nPos = g_buff.GetStartPos();
		pTarget->Init( img, mask, pTarget->m_rctBox, pTarget->m_nDirct, nID );

		//srcImg = img.clone();
		//pTarget->Draw( srcImg );
		//imwrite( format( "f:\\1\\Back_%ld.bmp", nTest++ ), srcImg );
		nPos = g_buff.GetLastBuff( nPos );

		pTarget->Backup( t0 );
		}*/
	}

	pTarget->PostProcess();

	pTarget->Recover( t0 );

	// pedestrian
	if ( pTarget->m_bType == 2 )
	{
		if ( pTarget->m_pTraj.size() < 1 )
		{
			delete pTarget;
			pTarget = NULL;
		}
		else if ( !pTarget->IsPedIn() )
		{
			delete pTarget;
			pTarget = NULL;
		}
	}
}

void CImgPrcView::SegmentPrj( int* pPrj, int nLen, int nGate, vector<int> &objx, vector<int> &objy )
{
	objx.clear();
	objy.clear();

	for ( int i = 0; i < nLen; i++ )
	{
		for ( ; i < nLen; i++ )
			if ( pPrj[i] >= nGate ) break;

		if ( i >= nLen ) break;
		objx.push_back( i );

		for ( ; i < nLen; i++ )
			if ( pPrj[i] < nGate ) break;
		objy.push_back( i );
	}

	int nSize = objx.size();
	for ( int i = 0; i < nSize; i++ )
	{
		if ( objy[i] - objx[i] < 3 )
		{
			objx.erase( objx.begin() + i );
			objy.erase( objy.begin() + i );
			i--;
			nSize--;
		}
	}

	nSize = objx.size() - 1;
	for ( int i = 0; i < nSize; i++ )
	{
		if ( objx[i + 1] - objy[i] < 11 )
		{
			objy[i] = objy[i + 1];
			objx.erase( objx.begin() + i + 1 );
			objy.erase( objy.begin() + i + 1 );
			i--;
			nSize--;
		}
	}
}

void CImgPrcView::GetProject( Mat& mog )
{
	int nW = mog.cols;
	int nH = mog.rows;

	Mat mRow;
	reduce( mog, mRow, 0, REDUCE_SUM, CV_32S );

	int* pPrj = (int*) mRow.data;

	vector<int> objx, objy;
	SegmentPrj( pPrj, nW, 10 * 255, objx, objy );

	Mat hisu( 64, nW, CV_8U, Scalar( 200 ) );
	for ( int i = 0; i < nW; i++ )
	{
		int nY = MIN( pPrj[i] / 255, 63 );
		for ( int j = 0; j < nY; j++ )
		{
			hisu.data[( 63 - j )*nW + i] = 64;
		}
		hisu.data[( 63 - 10 )*nW + i] += 50;
	}

	for ( int i = 0; i < objx.size(); i++ )
	{
		int x1 = objx[i];
		int x2 = objy[i];
		for ( int j = x1; j < x2; j++ )
		{
			hisu.data[( 0 )*nW + j] = ( i % 2 ) * 100;
			hisu.data[( 1 )*nW + j] = ( i % 2 ) * 100;
		}
	}
	CString strTemp;
	{
		int nGate = 10 * 255;
		int nSize = objx.size();
		for ( int i = 0; i < nSize; i++ )
		{
			int nCnt = 0;
			int nMean = 0;
			if ( objy[i] - objx[i] > 9 )
			{
				for ( int j = objx[i]; j < objy[i]; j++ )
				{
					nCnt += pPrj[j] >= nGate ? 1 : 0;
					nMean += pPrj[j];
				}
				nMean /= objy[i] - objx[i];
			}
			CString strT1;
			strT1.Format( "(%ld, %.2f, %ld)  ", objy[i] - objx[i], nCnt / (double) ( objy[i] - objx[i] ), nMean / 255 );
			if ( nCnt * 10 / 8 < objy[i] - objx[i] || nMean < nGate )
			{
				objx.erase( objx.begin() + i );
				objy.erase( objy.begin() + i );
				i--;
				nSize--;
				continue;
			}
			strTemp += strT1;
		}
	}

	Mat msk( 1, nW, CV_8UC1, Scalar( 0 ) );
	for ( int i = 0; i < objx.size(); i++ )
	{
		int x1 = objx[i];
		int x2 = objy[i];
		for ( int j = x1; j < x2; j++ )
		{
			if ( x2 > 350 )
				msk.data[j] = 255;
			hisu.data[( 5 )*nW + j] = ( i % 2 ) * 100;
			hisu.data[( 6 )*nW + j] = ( i % 2 ) * 100;
		}
	}

	for ( int k = 0; k < nH; k++ )
		mog.row( k ) &= msk;

	g_pView->SetInterResultImage( 2, hisu, "mRow  x " + strTemp );
	g_pView->SetInterResultImage( 2, mog, "mog  x " );
}

void CImgPrcView::Process( Mat& srcImg )
{
	CurrentFocusPos();

	int nH = srcImg.rows;
	int nW = srcImg.cols;

	//SetInterResultImage( 0, srcImg, "diff" );

	double x = 0, y = 0;

	//if ( m_nFileSource != 1 )
	{
		if ( bProcessFirst )
		{
			bProcessFirst = 0;
			g_refImg = srcImg.clone();
			FindMidLine( srcImg );
			return;
		}
		RemoveCamShake2( g_refImg, srcImg, x, y );
	}

	warpAffine( srcImg, srcImg, g_mapMatrix, g_roadmsku.size() );
	SetInterResultImage( -1, srcImg, "warpAffine" );

	Invalidate( FALSE );
	UpdateWindow();
	return;

	nH = srcImg.rows;
	nW = srcImg.cols;

	//FindMidLine( srcImg );

	Mat mask;
	mog->setVarThreshold( 10 );
	mog->apply( srcImg, mask, -1 );

	//SetInterResultImage( 2, mask, "mog" );

	//if ( g_templ.rows > 10 )
	//	SilhouetteMatch( mask, g_templ );

	std::vector<Ptr<CTarget>>::iterator it = g_Targets.begin();
	for ( ; it != g_Targets.end(); )
	{
		int nRes = ( *it )->Update( srcImg, mask, m_nFrmID );
		if ( nRes <= 0 )
		{
			if ( nRes == 0 )
				( *it )->Output();
			it = g_Targets.erase( it );
		}
		else it++;
	}
	CTarget::CheckList();


	BYTE* pdata = mask.data;
	for ( int i = 1; i<nH - 1; i++ )
		for ( int j = 1; j < nW - 1; j++ )
		{
			if ( !pdata[i*nW + j] )
				continue;
			if ( pdata[i*nW + j + 1] || pdata[i*nW + j - 1] || pdata[( i - 1 )*nW + j] || pdata[( i + 1 )*nW + j] )
				continue;
			pdata[i*nW + j] = 0;
		}



	Mat element = getStructuringElement( MORPH_RECT, Size( 3, 3 ) );
	morphologyEx( mask, mask, MORPH_DILATE, element );

	element = getStructuringElement( MORPH_RECT, Size( 3, 3 ) );
	morphologyEx( mask, mask, MORPH_ERODE, element );

	SetInterResultImage( 2, mask, "mog - morph" );


	//Mat msk = mask & g_roadmsku;
	//GetProject( srcImg, msk );
	//msk = mask & g_roadmskd;
	//GetProject( srcImg, msk );

	{
		Mat img1( srcImg.size(), CV_8UC1, Scalar( 255 ) );

		for ( int i = 0; i < nH*nW; i++ )
		{
			int r = srcImg.data[i * 3 + 2];
			int g = srcImg.data[i * 3 + 1];
			int b = srcImg.data[i * 3 + 0];
			if ( g > b + 5 && g > r + 10 && r > b + 10 )
				img1.data[i] = 0;
		}
		SetInterResultImage( 2, img1, "img1 -12 morph" );

		element = getStructuringElement( MORPH_RECT, Size( 3, 3 ) );
		morphologyEx( img1, img1, MORPH_ERODE, element );

		mask &= img1;
		SetInterResultImage( 2, mask, "mog -12 morph" );
	}


	// update buffer
	g_buff.Add( m_nFrmID, srcImg, mask );

	vector<Rect> rctObjs;
	Rect2d rctObj;
	if ( CheckObjects( mask, g_roadmsku, rctObjs, false ) )
	{
		for ( int k = 0; k < rctObjs.size(); k++ )
		{
			rctObj = rctObjs[k];
			bool bFound = false;
			for ( int i = 0; i < g_Targets.size(); i++ )
			{
				Rect rct = g_Targets[i]->m_rctBox & rctObj;
				if ( rct.area() < rctObj.area() * 0.2 ) continue;
				bFound = true;
				break;
			}
			if ( !bFound && rctObj.width < mask.cols / 2 )
			{
				CTarget* pTarget = new CTarget;

				// recheck box
				GetCentroidBox( mask.clone(), rctObj, 168, 90 );

				pTarget->Init( srcImg, mask, rctObj, 1, m_nFrmID, 1 );

				g_nBackPos = g_buff.GetStartPos();
				g_nBackPos = g_buff.GetLastBuff( g_nBackPos );
				g_pTarget = pTarget;
				{
					bFirst = true;
					nPos = 0;
					bRecapture = true;
					nFrame = 0;
				}
				BackProcess( pTarget );
				if ( pTarget )
					g_Targets.push_back( pTarget );
			}
		}
	}

	rctObjs.clear();
	int nPedCnt = 0;
	if ( CheckObjects( mask, g_roadmskd, rctObjs, true, &nPedCnt ) ) // true
	{
		for ( int k = 0; k < rctObjs.size(); k++ )
		{
			rctObj = rctObjs[k];
			bool bFound = false;
			for ( int i = 0; i < g_Targets.size(); i++ )
			{
				Rect rct = g_Targets[i]->m_rctBox & rctObj;
				if ( rct.area() < rctObj.area() * 0.4 ) continue;
				bFound = true;
				break;
			}
			if ( !bFound && rctObj.width < mask.cols / 2 )
			{
				CTarget* pTarget = new CTarget;

				// recheck box
				GetCentroidBox( mask.clone(), rctObj, 168, 90 );

				pTarget->Init( srcImg, mask, rctObj, 2, m_nFrmID, k < nPedCnt ? 2 : 0 );

				g_nBackPos = g_buff.GetStartPos();
				g_nBackPos = g_buff.GetLastBuff( g_nBackPos );
				g_pTarget = pTarget;
				{
					bFirst = true;
					nPos = 0;
					bRecapture = true;
					nFrame = 0;
				}
				BackProcess( pTarget );

				if ( pTarget )
					g_Targets.push_back( pTarget );
			}
		}
	}



	for ( int i = 0; i < g_Targets.size(); i++ )
		g_Targets[i]->Draw( srcImg );

	{
		BYTE* pData = g_roadmskd.data;
		for ( int i = 0; i < srcImg.rows; i++ )
			for ( int j = 0; j < srcImg.cols; j++ )
			{
				if ( pData[i*srcImg.cols + j] ) continue;
				srcImg.data[( i*srcImg.cols + j ) * 3 + 2] = MAX( 0, srcImg.data[( i*srcImg.cols + j ) * 3 + 2] - 30 );
			}

		pData = g_roadmsku.data;
		for ( int i = 0; i < srcImg.rows; i++ )
			for ( int j = 0; j < srcImg.cols; j++ )
			{
				if ( pData[i*srcImg.cols + j] ) continue;
				srcImg.data[( i*srcImg.cols + j ) * 3 + 0] = MAX( 0, srcImg.data[( i*srcImg.cols + j ) * 3 + 2] - 30 );
			}
	}


#ifdef _DEBUG
	SetInterResultImage( 1, srcImg, " target tracking " );
#else
	SetInterResultImage( -1, srcImg, " target tracking " );
#endif

	Invalidate( FALSE );
	UpdateWindow();
	return;
}
void CImgPrcView::RemoveCamShake( Mat &referImg, Mat &img, double&x, double&y )
{
	Mat resImg = img.clone();

	Point* pts = new Point[g_nFixedCnt];
	for ( int i = 0; i < g_nFixedCnt; i++ )
	{
		Point pt = g_ptFixed[i];

		Rect2d rctTemp;
		rctTemp.x = max( 0, g_ptFixed[i].x - 15 );
		rctTemp.y = max( 0, g_ptFixed[i].y - 15 );
		rctTemp.width = min( rctTemp.width + 30, img.cols - 1 - rctTemp.x );
		rctTemp.height = min( rctTemp.height + 30, img.rows - 1 - rctTemp.y );

		Rect2d rctSearch;
		rctSearch.x = max( 0, g_ptFixed[i].x - 40 );
		rctSearch.y = max( 0, g_ptFixed[i].y - 40 );
		rctSearch.width = min( rctSearch.width + 80, img.cols - 1 - rctSearch.x );
		rctSearch.height = min( rctSearch.height + 80, img.rows - 1 - rctSearch.y );

		Mat result;
		int result_cols = rctSearch.width - rctTemp.width + 1;
		int result_rows = rctSearch.height - rctTemp.height + 1;
		result.create( result_rows, result_cols, CV_32FC1 );

		matchTemplate( img( rctSearch ), referImg( rctTemp ), result, CV_TM_CCORR_NORMED );

		normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

		double minVal, maxVal;
		Point minLoc, maxLoc;
		minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

		pts[i].x = maxLoc.x - 25;
		pts[i].y = maxLoc.y - 25;

		line( resImg, pts[i] + g_ptFixed[i], g_ptFixed[i], Scalar( 255, 0, 0 ), 1 );

		rectangle( resImg, rctSearch, Scalar( 255, 0, 0 ), 1 );
		rectangle( resImg, rctTemp, Scalar( 255, 255, 0 ), 1 );

		Mat img_display;
		result.convertTo( img_display, CV_8UC1, 255.0 );
		SetInterResultImage( 1, img_display, "match" );
	}

	x = y = 0;
	for ( int i = 0; i < g_nFixedCnt; i++ )
	{
		x += pts[i].x;
		y += pts[i].y;
	}
	x /= g_nFixedCnt;
	y /= g_nFixedCnt;

	{
		static int bFirst = 1;
		static Mat mapX;
		static Mat mapY;

		if ( bFirst )
		{
			bFirst = 0;
			mapX.create( img.size(), CV_32FC1 );
			mapY.create( img.size(), CV_32FC1 );

			for ( int j = 0; j < img.rows; j++ )
				for ( int i = 0; i < img.cols; i++ )
				{
					mapX.at<float>( j, i ) = i;
					mapY.at<float>( j, i ) = j;
				}
		}

		Mat map_x = mapX + x;
		Mat map_y = mapY + y;
		remap( img, img, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar( 255, 255, 255 ) );
	}
	delete[]pts;
	SetInterResultImage( 1, resImg, "cam shift" );
}

void CImgPrcView::RemoveCamShake2( Mat &referImg, Mat &img, double&x, double&y )
{/*
 static bool bFirst = true;
 static std::vector<Point2f> prevPts;
 static Mat refer;
 static Mat mapX;
 static Mat mapY;

 if ( bFirst )
 {
 bFirst = false;

 mapX.create( img.size(), CV_32FC1 );
 mapY.create( img.size(), CV_32FC1 );

 for ( int j = 0; j < img.rows; j++ )
 for ( int i = 0; i < img.cols; i++ )
 {
 mapX.at<float>( j, i ) = i;
 mapY.at<float>( j, i ) = j;
 }

 refer = referImg( g_rctFixed ).clone();
 if ( refer.channels() == 3 )
 cvtColor( refer, refer, CV_RGB2GRAY );

 goodFeaturesToTrack( refer, prevPts, 32,    // �ǵ������Ŀ
 0.04122,  // �����ȼ���������0.01*max��min��e1��e2������e1��e2��harris���������ֵ
 3.5496, g_maskfixed, 3, false, 0.4 );
 }

 // If no found, return
 if ( !prevPts.size() )
 return;

 std::vector<Point2f> currPts;
 std::vector<uchar> status;
 std::vector<float> errors;
 std::vector<Point2f> pts;
 Size window = Size( 6, 6 );

 Mat realImg = img( g_rctFixed ).clone();
 if ( realImg.channels() == 3 )
 cvtColor( realImg, realImg, CV_RGB2GRAY );

 calcOpticalFlowPyrLK( refer, realImg, prevPts, currPts, status, errors, window, 0,
 TermCriteria( static_cast<int>( TermCriteria::COUNT ) + static_cast<int>( TermCriteria::EPS ), 20, 0.183328975142 ),
 0, 1e-4 );


 //Mat resImg = img.clone();
 for ( int i = 0; i < prevPts.size(); i++ )
 {
 if ( status[i] != !0 )
 pts.push_back( prevPts[i] - currPts[i] );
 }

 double dX = 0;
 double dY = 0;
 {
 int nCnt = 0;
 Mat matX( 1, 100, CV_32FC1 );
 Mat matY( 1, 100, CV_32FC1 );

 float *pDataX = (float*) matX.data;
 float *pDataY = (float*) matY.data;
 for ( int i = 0; i < pts.size(); i++ )
 {
 pDataY[nCnt] = pts[i].y;
 pDataX[nCnt] = pts[i].x;
 nCnt++;
 }
 matX = matX( Rect( 0, 0, nCnt, 1 ) );
 matY = matY( Rect( 0, 0, nCnt, 1 ) );

 cv::sort( matX, matX, CV_SORT_EVERY_ROW | CV_SORT_ASCENDING );
 cv::sort( matY, matY, CV_SORT_EVERY_ROW | CV_SORT_ASCENDING );

 int nCount = 0;
 for ( int i = nCnt / 4; i < nCnt - nCnt / 4; i++ )
 {
 dX += pDataX[i];
 dY += pDataY[i];
 nCount++;
 }
 if ( nCount )
 {
 dX /= nCount;
 dY /= nCount;
 }
 }

 //if ( fabs( MAX( dX, dY ) ) > 0.1 )
 {
 Mat map_x = mapX - dX;
 Mat map_y = mapY - dY;
 remap( img, img, map_x, map_y, CV_INTER_LINEAR, BORDER_CONSTANT, Scalar( 255, 255, 255 ) );
 }

 if ( fabs( dX ) > 2 || fabs( dY ) > 2 )
 AfxMessageBox( " " );

 x = dX = 0;
 y = dY = 0;
 SetInterResultImage( 1, referImg, "referImg" );*/
}

void CImgPrcView::FindMidLine( Mat& img )
{
	Rect2d roi( 0, img.rows / 3, img.cols, img.rows * 2 / 3 );
	Mat imgline = img( roi ).clone();

	Mat aChannels[3];
	split( imgline, aChannels );

	Mat kernel = Mat::ones( 17, 23, CV_32F );
	float* pKernel = (float*) kernel.data;
	memset( pKernel + ( kernel.rows / 2 ) * kernel.cols, 0, sizeof( float )*kernel.cols );
	for ( int i = 0; i < kernel.cols; i++ )
		for ( int j = 0; j < kernel.rows / 4; j++ )
		{
			pKernel[j * kernel.cols + i] = 0;
			pKernel[( kernel.rows - 1 - j ) * kernel.cols + i] = -2;
		}
	Mat src_f1;
	filter2D( aChannels[2], src_f1, CV_32F, kernel );

	for ( int i = 0; i < kernel.cols; i++ )
		for ( int j = 0; j < kernel.rows / 4; j++ )
		{
			pKernel[j * kernel.cols + i] = -2;
			pKernel[( kernel.rows - 1 - j ) * kernel.cols + i] = 0;
		}
	Mat src_f2;
	filter2D( aChannels[2], src_f2, CV_32F, kernel );

	Mat src_f = min( src_f1, src_f2 );
	src_f = max( src_f, Mat::zeros( src_f.size(), CV_32F ) );

	normalize( src_f, src_f, 255.0, 0.0, NORM_MINMAX );
	src_f.convertTo( imgline, CV_8U );
	SetInterResultImage( 1, imgline );

	int nH = imgline.rows;
	int nW = imgline.cols;
	BYTE* pImg = imgline.data;

	float *pHis = new float[nH * 40];
	memset( pHis, 0, nH * 40 * sizeof( float ) );

	int nID = 0;
	for ( double dAngle = 0; dAngle < 10; dAngle += 0.5, nID++ )
	{
		double dCos = cos( dAngle*CV_PI / 180 );
		double dSin = sin( dAngle*CV_PI / 180 );

		for ( int i = 2; i < nH - 2; i++ )
			for ( int j = 2; j < nW - 2; j++ )
			{
				int y = ( j - nW / 2 )*dSin + ( i - nH / 2 )*dCos + nH / 2;
				if ( y < 1 || y >= nH ) continue;
				pHis[y + nID*nH] += pImg[i*nW + j] * pImg[i*nW + j];
			}
	}

	Mat his( 1, nH * 40, CV_32F, pHis );
	double maxVal = 0;
	Point maxLoc;
	minMaxLoc( his, NULL, &maxVal, NULL, &maxLoc );

	double dAngle = -( maxLoc.x / nH ) * 0.5 * CV_PI / 180;
	double dCos = cos( dAngle );
	double dSin = sin( dAngle );
	int x = roi.x + nW / 2;
	int y = roi.y + maxLoc.x % nH;

	int nY2 = ( imgline.cols - 1 - x ) * dSin / dCos + y;
	int nY1 = -x * dSin / dCos + y;

	g_ptMidLine[0] = Point( 0, nY1 );
	g_ptMidLine[1] = Point( imgline.cols - 1, nY2 );
	g_dMLineAngle = dAngle;

	imgline = img.clone();
	line( imgline, g_ptMidLine[0], g_ptMidLine[1], Scalar( 0, 0, 255 ) );

	SetInterResultImage( 1, imgline, "mid line" );

	{
		double a = sin( g_dMLineAngle ), b = cos( g_dMLineAngle );
		double dLen = sqrt( pow( g_ptMidLine[0].x - g_ptMidLine[1].x, 2 ) + pow( g_ptMidLine[0].y - g_ptMidLine[1].y, 2 ) );

		int height_rotate = 400;
		int width_rotate = dLen - height_rotate* a / b;

		g_roadmsku = Mat::zeros( height_rotate, width_rotate, CV_8UC1 );
		g_roadmskd = Mat::zeros( height_rotate, width_rotate, CV_8UC1 );

		Point2f ptS[3];
		ptS[0] = Point2f( nW / 2, ( g_ptMidLine[0].y + g_ptMidLine[1].y ) / 2 );
		ptS[1] = Point2f( nW - 1, g_ptMidLine[1].y + 200 / b );
		ptS[2] = Point2f( ptS[1].x + 200 * a, ptS[1].y - 200 * b );
		Point2f ptD[3];
		ptD[0] = Point2f( width_rotate / 2, 199 );
		ptD[1] = Point2f( width_rotate - 1, 399 );
		ptD[2] = Point2f( width_rotate - 1, 199 );

		g_mapMatrix = getAffineTransform( ptS, ptD );

		Point root_points[1][40];
		root_points[0][0] = Point( 0, 207 );
		root_points[0][1] = Point( 183, 196 );
		root_points[0][2] = Point( width_rotate - 1, 196 );
		root_points[0][3] = Point( width_rotate - 1, 100 );
		root_points[0][4] = Point( 195, 161 );
		root_points[0][5] = Point( 0, 196 );

		const Point* ppt[1] = { root_points[0] };
		int npt[] = { 6 };
		fillPoly( g_roadmsku, ppt, npt, 1, Scalar( 255 ) );

		npt[0] = 6;
		root_points[0][0] = Point( 0, 212 );
		root_points[0][1] = Point( 179, 202 );
		root_points[0][2] = Point( width_rotate - 1, 202 );
		root_points[0][3] = Point( width_rotate - 1, 312 );
		root_points[0][4] = Point( 149, 244 );
		root_points[0][5] = Point( 0, 202 + 32 );
		fillPoly( g_roadmskd, ppt, npt, 1, Scalar( 255 ) );

		GetPedSearchArea( g_roadmsku, g_roadmskd, CTarget::m_spPedRange );

		CTarget::m_pDnMsk = &g_roadmskd;
		CTarget::m_pUpMsk = &g_roadmsku;
	}
	delete pHis;
}

void CImgPrcView::SilhouetteMatch( Mat& img, Mat& templ )
{
	Rect2d rctSearch = g_rctMatch;
	rctSearch.x = MAX( 0, rctSearch.x - 30 );
	rctSearch.y = MAX( 0, rctSearch.y - 30 );
	rctSearch.width = min( rctSearch.width + 60, img.cols - 1 - rctSearch.x );
	rctSearch.height = min( rctSearch.height + 60, img.rows - 1 - rctSearch.y );

	if ( rctSearch.width < 1 || rctSearch.height < 1 )
		return;

	Mat result;
	result.create( rctSearch.size(), CV_32FC1 );

	matchTemplate( img( rctSearch ), templ, result, CV_TM_CCORR );

	normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;
	minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

	matchLoc.x = maxLoc.x + rctSearch.x;
	matchLoc.y = maxLoc.y + rctSearch.y;
	int dx = matchLoc.x - g_rctMatch.x;
	int dy = matchLoc.y - g_rctMatch.y;

	g_rctMatch.x += dx;
	g_rctMatch.y += dy;

	Mat img_display;
	cvtColor( img, img_display, CV_GRAY2RGB );

	rectangle( img_display, g_rctMatch, Scalar( 0, 0, 255 ), 1, 8, 0 );
	rectangle( img_display, rctSearch, Scalar( 0, 255, 0 ), 1, 8, 0 );
	SetInterResultImage( 1, img_display, "silhouette match" );
	SetInterResultImage( 1, templ, "template" );
}

void CImgPrcView::Process1( Mat& srcImg, Mat &mog )
{

}

void CImgPrcView::OnTrafficShowall()
{
	CString strPath = "f:\\output\\";
	std::vector<Ptr<CTarget>> targets;
	CTarget::LoadTarget( strPath, targets );
	Mat img = imread( "f:\\0.bmp" );
	CTarget::DrawAll( img, targets );

	m_nHeight = img.rows;// frame.rows;// / 2;
	m_nWidth = img.cols; // frame.cols;// / 2;
	m_flag = 24;

	if ( m_pBitmap ) delete m_pBitmap;
	m_pBitmap = new BYTE[m_nHeight*m_nWidth * 3];
	memcpy( m_pBitmap, img.data, m_nHeight*m_nWidth * 3 );

	Invalidate( FALSE );
	UpdateWindow();
}

void CImgPrcView::ShowCursorInfo( CString strText1, CString strText2, CString strText3, CString strText4 )
{
	CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd();
	if ( !pFrame->m_wndStatusBar.GetSafeHwnd() )
		return;
	CStatusBar *pStatus = &pFrame->m_wndStatusBar;

	pStatus->SetPaneText( 1, strText1, TRUE );
	pStatus->SetPaneText( 2, strText2, TRUE );
	pStatus->SetPaneText( 3, strText3, TRUE );
	pStatus->SetPaneText( 4, strText4, TRUE );
}

Point2d Crosspoint2( Point2d pt1, Point2d pt2, Point2d pt3, Point2d pt4 )
{
	double x, y;
	double X1 = pt1.x - pt2.x;
	double Y1 = pt1.y - pt2.y;
	double X2 = pt3.x - pt4.x;
	double Y2 = pt3.y - pt4.y;

	if ( fabs( X1*Y2 - X2*Y1 )<0.000001 )
		return Point2d( ( pt2.x + pt3.x ) / 2, ( pt2.y + pt3.y ) / 2 );

	double A = X1*pt1.y - Y1*pt1.x, B = X2*pt3.y - Y2*pt3.x;
	y = ( A*Y2 - B*Y1 ) / ( X1*Y2 - X2*Y1 );
	x = ( B*X1 - A*X2 ) / ( Y1*X2 - Y2*X1 );

	return Point2d( x, y );
}
void FindPeaks( int *pnHis, int nW, int& nMaxCnt, int nGate, int* pnPeak )
{
	memset( pnPeak, 0, sizeof( int )*nW );

	int pnPos[100];
	memset( pnPos, 0, sizeof( int ) * nMaxCnt );
	int nCnt = 0;

	int i, j, m;
	for ( i = 4; i < nW - 4; i++ )
	{
		for ( j = -4; j < 4; j++ )
		{
			if ( !j ) continue;
			if ( pnHis[i] > pnHis[j + i] ) continue;
			break;
		}
		if ( j < 4 ) continue;

		for ( j = 0; j < nCnt; j++ )
		{
			if ( pnHis[i] < pnHis[pnPos[j]] ) continue;
			break;
		}

		if ( j >= nMaxCnt && pnHis[i] < nGate ) continue;

		for ( m = nCnt; m > j; m-- )
			pnPos[m] = pnPos[m - 1];
		pnPos[j] = i;
		nCnt = min( nMaxCnt, nCnt + 1 );
	}
	for ( i = 0; i<nCnt - 1; i++ )
		for ( int j = i + 1; j < nCnt; j++ )
		{
			if ( pnPos[i] > pnPos[j] )
			{
				int nTemp = pnPos[j];
				pnPos[j] = pnPos[i];
				pnPos[i] = nTemp;
			}
		}

	for ( i = 0; i < nCnt; i++ )
		pnPeak[i] = pnPos[i];

	nMaxCnt = nCnt;
}

Point2d Crosspoint( Point2d pt1, Point2d pt2, Point2d pt3, Point2d pt4 )
{
	Point2d pt( 0, 0 );
	// line1's cpmponent
	double X1 = pt2.x - pt1.x;//b1
	double Y1 = pt2.y - pt1.y;//a1
							  // line2's cpmponent
	double X2 = pt4.x - pt3.x;//b2
	double Y2 = pt4.y - pt3.y;//a2
							  // distance of 1,2
	double X21 = pt3.x - pt1.x;
	double Y21 = pt3.y - pt1.y;
	// determinant
	double D = Y1*X2 - Y2*X1;// a1b2-a2b1
							 // 
	if ( fabs( D ) < 0.00001 )
		return Point2d( ( pt2.x + pt3.x ) / 2, ( pt2.y + pt3.y ) / 2 );

	// cross point
	pt.x = ( X1*X2*Y21 + Y1*X2*pt1.x - Y2*X1*pt3.x ) / D;
	pt.y = -( Y1*Y2*X21 + X1*Y2*pt1.y - X2*Y1*pt3.y ) / D;

	return pt;
}

/*{
CFile File( "F:\\peggy\\1922\\com_image7\\1000.png", CFile::modeRead );
int nSize = File.GetLength();
char* pBuff = new char[nSize];
File.Read( pBuff, nSize );
File.Close();


Mat img_decode;
vector<uchar> imgd;
imgd.resize( nSize );
imgd.assign( pBuff, pBuff + nSize );
DWORD dwTime = GetTickCount();
for ( int i = 0; i < 100; i++ )
img_decode = imdecode( imgd, CV_LOAD_IMAGE_COLOR );
dwTime = GetTickCount() - dwTime;
CString strTemp;
strTemp.Format( "%ld", dwTime );
AfxMessageBox( strTemp );
//imshow( "pic", img_decode );
return;
}*/

void CImgPrcView::FindSign()
{
	int nL = MIN( g_dCorner[0].x, MIN( g_dCorner[1].x, MIN( g_dCorner[2].x, g_dCorner[3].x ) ) );
	int nR = MAX( g_dCorner[0].x, MAX( g_dCorner[1].x, MAX( g_dCorner[2].x, g_dCorner[3].x ) ) );
	int nT = MIN( g_dCorner[0].y, MIN( g_dCorner[1].y, MIN( g_dCorner[2].y, g_dCorner[3].y ) ) );
	int nB = MAX( g_dCorner[0].y, MAX( g_dCorner[1].y, MAX( g_dCorner[2].y, g_dCorner[3].y ) ) );

	double dLeft = nL - 10;
	double dTop = nT - 10;
	double dWidth = nR - nL + 20;
	double dHeight = nB - nT + 20;

	static Rect rct1( dLeft, dTop, dWidth, dHeight );

	Mat img = imread( format( "%s", m_strFileName ) );

	Rect rct0 = rct1;
	//Mat img( m_nHeight, m_nWidth, CV_8UC3, m_pBitmap );

	Mat imgs = img( rct1 ).clone();
	//rectangle( imgs, rct1, Scalar( 0, 0, 255 ), 1 );
	//SetInterResultImage( 0, imgs, "local" );
	//Invalidate( FALSE );
	//UpdateWindow();


	BYTE* pResV = NULL;
	FilterVC( imgs.data, imgs.rows, imgs.cols, 10, 1, pResV );
	//SetInterResultImage( 1, pResV, imgs.rows, imgs.cols, 8, "" );

	BYTE* pResH = NULL;
	FilterHC( imgs.data, imgs.rows, imgs.cols, 1, 10, pResH );
	//SetInterResultImage( 1, pResH, imgs.rows, imgs.cols, 8, "" );

	for ( int i = 0; i < imgs.rows*imgs.cols; i++ )
		pResV[i] = MAX( pResV[i], pResH[i] );

	Mat imRes( imgs.size(), CV_8UC1, pResV );

	//GaussianBlur( imgs, imRes, Size( 6 * 2 + 1, 6 * 2 + 1 ), 0 );
	//imRes = imgs - imRes;
	//normalize( imRes, imRes, 255, 0, CV_MINMAX );
	//cvtColor( imRes, imRes, CV_BGR2GRAY );

	//SetInterResultImage( 0, imgs, "imgs" );
	//SetInterResultImage( 1, imRes, "FilterVC" );

	Mat mRow( 1, imRes.cols, CV_32SC1 );
	reduce( imRes, mRow, 0, REDUCE_SUM, CV_32SC1 );

	Mat mCol( imRes.rows, 1, CV_32SC1, Scalar( 0 ) );
	reduce( imRes, mCol, 1, REDUCE_SUM, CV_32SC1 );

	mCol = mCol.reshape( 0, 1 );

	nL = nR = nT = nB = 0;
	{
		double minVal, maxVal;
		Point minLoc, maxLoc, matchLoc;
		minMaxLoc( mRow, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
		nL = maxLoc.x;


		Mat mRow1 = mRow.clone();
		normalize( mRow, mRow1, 0, 255, NORM_MINMAX );
		int* pPrj = (int*) mRow1.data;

		Mat hisu( 64, mRow.cols, CV_8U, Scalar( 200 ) );
		for ( int i = 0; i < mRow.cols; i++ )
			for ( int j = 0; j < pPrj[i] / 4; j++ )
				hisu.data[( 63 - j )*mRow.cols + i] = 64;

		Point pt0 = Point( maxLoc.x, 0 );
		Point pt1 = Point( maxLoc.x, 63 - pPrj[(int) nL] / 4 );
		line( hisu, pt0, pt1, Scalar( 128 ), 1 );

		pPrj = (int*) mRow.data;
		for ( int i = MAX( 0, pt0.x - 10 ); i < MIN( mRow.cols, pt0.x + 10 ); i++ )
			pPrj[i] = 0;
		minMaxLoc( mRow, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
		nR = maxLoc.x;
	}

	if ( nL > nR )		swap( nL, nR );
	int nSize = MIN( 10, ( nR - nL ) / 4 );

	{
		double minVal, maxVal;
		Point minLoc, maxLoc, matchLoc;

		int* pPrj = (int*) mCol.data;
		for ( int i = 1; i < mCol.cols - 1; i++ )
			pPrj[i] = ( pPrj[i - 1] + pPrj[i] + pPrj[i + 1] ) / 3;

		int nTotal = 0;
		int nFrom = 0;
		for ( int i = 0; i < mCol.cols / 2 - 1; i++ )
		{
			if ( pPrj[i] <= nTotal ) continue;
			nTotal = pPrj[i];
			nFrom = i;
		}
		nT = nFrom;

		int* pnPeak = new int[mCol.cols];
		int nMaxCnt = 4;
		FindPeaks( pPrj, mCol.cols, nMaxCnt, 5, pnPeak );

		/*{
		Mat mCol1 = mCol.clone();
		normalize( mCol, mCol1, 0, 255, NORM_MINMAX );
		int* pPrj = (int*) mCol1.data;

		Mat hisu( 64, mCol.cols, CV_8U, Scalar( 200 ) );
		for ( int i = 0; i < mCol.cols; i++ )
		for ( int j = 0; j < pPrj[i] / 4; j++ )
		hisu.data[( 63 - j )*mCol.cols + i] = 64;

		for ( int i = 0; i < nMaxCnt; i++ )
		hisu.data[pnPeak[i]] = 255;

		SetInterResultImage( 1, hisu, "hisu" );
		}*/

		for ( int i = 0; i < nMaxCnt; i++ )
		{
			if ( pnPeak[i] == nT )
			{
				for ( int j = i; j < nMaxCnt; j++ )
					pnPeak[j - i] = pnPeak[j];
				nMaxCnt -= i;
				break;
			}
		}

		// nsHeight					
		Mat mCol1 = mCol.clone();
		normalize( mCol, mCol1, 0, 255, NORM_MINMAX );
		pPrj = (int*) mCol1.data;

		Mat hisu( 64, mCol.cols, CV_8U, Scalar( 200 ) );
		for ( int i = 0; i < mCol.cols; i++ )
			for ( int j = 0; j < pPrj[i] / 4; j++ )
				hisu.data[( 63 - j )*mCol.cols + i] = 64;

		Point pt0 = Point( maxLoc.x, 0 );
		Point pt1 = Point( maxLoc.x, 63 - pPrj[(int) nT] / 4 );
		line( hisu, pt0, pt1, Scalar( 128 ), 1 );

		pPrj = (int*) mCol.data;
		for ( int i = MAX( 0, pt0.x - nSize ); i < MIN( mRow.cols, pt0.x + nSize ); i++ )
			pPrj[i] = 0;
		minMaxLoc( mCol, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
		nB = maxLoc.x;

		if ( nT > nB )		swap( nT, nB );

		delete pnPeak;
	}

	if ( nT > nB )		swap( nT, nB );


	Mat imgs1 = imgs.clone();
	Rect2i rctDet( nL, nT, nR - nL, nB - nT );
	for ( int m = nT; m<nB; m++ )
	{
		int n = nL;
		imgs1.data[( m*imgs1.cols + n ) * 3 + 2] = 255;
		n = nR;
		imgs1.data[( m*imgs1.cols + n ) * 3 + 2] = 255;
	}
	for ( int n = nL; n < nR; n++ )
	{
		int m = nT;
		imgs1.data[( m*imgs1.cols + n ) * 3 + 2] = 255;
		m = nB;
		imgs1.data[( m*imgs1.cols + n ) * 3 + 2] = 255;
	}
	SetInterResultImage( 1, imgs1, "imgs1" );


	// fine detection below:
	Point2d ptCenters[4];
	double pdAngle[4];
	nSize = MIN( 40, ( nB - nT ) / 2 );
	//if ( nSize < 100 )
	{
		// if too small, omit the non-parrell

		// top-bottom
		int nOffY = 40;
		if ( nT + rct1.y - nSize / 2 < nOffY )
			nOffY = nT + rct1.y - nSize / 2;
		Rect rctU = Rect( nL + rct1.x - 40, nT + rct1.y - nSize / 2 - nOffY, nR - nL + 80, nSize + 80 );
		Rect rctD = Rect( nL + rct1.x - 40, nB + rct1.y - nSize / 2 - 40, nR - nL + 80, nSize + 80 );

		Mat imgrctU = img( rctU ).clone();
		resize( imgrctU, imgrctU, imgrctU.size() * 4 );
		g_pView->SetInterResultImage( 2, imgrctU, "Top  y" );

		Mat imgrctD = img( rctD ).clone();
		resize( imgrctD, imgrctD, imgrctD.size() * 4 );
		//g_pView->SetInterResultImage( 2, imgrctD, "Down  y" );

		vector<float> ptsval;
		vector<int> ptsvalU;
		vector<int> ptsvalD;

		Point2d centerU( imgrctU.cols / 2.0, imgrctU.rows / 2.0 );
		Point2d centerD( imgrctD.cols / 2.0, imgrctD.rows / 2.0 );
		for ( double angle = -8.0; angle < 8; angle += 0.1 )
		{
			double minVal, maxValU, maxValD;
			Point minLoc, maxLocU, maxLocD, matchLoc;
			{
				int nW0 = imgrctU.cols - 40 * 4 * 2;
				int nH0 = imgrctU.rows - 40 * 4 * 2;
				BYTE* pRes = new BYTE[nH0*nW0 * 3];
				/*dib.CutRegionFromRotatePictureC( imgrctU.data, imgrctU.rows, imgrctU.cols, imgrctU.cols / 2.0, imgrctU.rows / 2.0, angle,
				pRes,
				imgrctU.cols / 2.0 - nW0 / 2.0, imgrctU.rows / 2.0 - nH0 / 2.0,
				imgrctU.cols / 2.0 + nW0 / 2.0, imgrctU.rows / 2.0 + nH0 / 2.0,
				nH0, nW0 );*/


				Mat dstU( nH0, nW0, CV_8UC3, pRes );
				Mat rot = getRotationMatrix2D( centerU, angle, 1.0 );
				warpAffine( imgrctU, dstU, rot, imgrctU.size() );
				Rect rctS( 40 * 4, nOffY * 4, nW0, nH0 );
				dstU = dstU( rctS ).clone();



				float* pnResU = NULL;
				FilterVC2( dstU.data, dstU.rows, dstU.cols, 10, 5, pnResU, 0 );

				Mat resH( dstU.rows, dstU.cols, CV_32FC1, pnResU );
				Mat mCol1( dstU.rows, 1, CV_32FC1 );
				reduce( resH, mCol1, 1, REDUCE_SUM, CV_32FC1 );


				minMaxLoc( mCol1, &minVal, &maxValU, &minLoc, &maxLocU );

				//Mat res2;
				//resH.convertTo( res2, CV_8UC1 );
				//CString stTemp;
				//stTemp.Format( "%.2f,		%.2f", angle,maxValU );
				//g_pView->SetInterResultImage( 2, res2, " " + stTemp );

				delete pnResU;
				delete pRes;
			}
			{
				Mat rot = getRotationMatrix2D( centerD, angle, 1.0 );
				Mat dstD;
				warpAffine( imgrctD, dstD, rot, imgrctD.size() );
				Rect rctS( 40 * 4, 40 * 4, dstD.cols - 40 * 4 * 2, dstD.rows - 40 * 4 * 2 );
				dstD = dstD( rctS ).clone();

				g_pView->SetInterResultImage( 2, dstD, "Down  y" );

				float* pnResD = NULL;
				FilterVC2( dstD.data, dstD.rows, dstD.cols, 10, 5, pnResD, 1 );
				Mat resH( dstD.rows, dstD.cols, CV_32FC1, pnResD );
				Mat mCol1( dstD.rows, 1, CV_32FC1 );
				reduce( resH, mCol1, 1, REDUCE_SUM, CV_32FC1 );

				minMaxLoc( mCol1, &minVal, &maxValD, &minLoc, &maxLocD );
				delete pnResD;
			}
			ptsval.push_back( maxValU + maxValD );
			ptsvalU.push_back( maxLocU.y );
			ptsvalD.push_back( maxLocD.y );
		}

		double dMaxV = 1;
		Point2d ptMax;
		Mat imgBest;
		double dBestAngle = 0;

		for ( int i = 0; i<3; i++ )
			for ( int j = 1; j < ptsval.size() - 1; j++ )
				ptsval[j] = ( ptsval[j - 1] + ptsval[j + 1] + ptsval[j] ) / 3.0;

		double dPosU = 0, dPosD = 0;
		for ( int i = 0; i < ptsval.size(); i++ )
		{
			if ( ptsval[i] < dMaxV ) continue;
			dMaxV = ptsval[i];
			dPosU = ptsvalU[i];
			dPosD = ptsvalD[i];
			dBestAngle = ( i*0.1 - 8 ) * CV_PI / 180;
		}

		Mat rot = getRotationMatrix2D( centerU, dBestAngle, 1.0 );
		Mat dst;
		warpAffine( imgrctU, dst, rot, imgrctU.size() );
		Rect rctS( 40 * 4, nOffY * 4, dst.cols - 40 * 4 * 2, dst.rows - 40 * 4 - nOffY * 4 );
		dst = dst( rctS ).clone();
		line( dst, Point( 0, dPosU ), Point( dst.cols - 1, dPosU ), Scalar( 0, 0, 255 ) );
		g_pView->SetInterResultImage( 2, dst, "Top  y" );

		double dy = dPosU - dst.rows / 2.0;
		centerU.x = ( dst.cols / 2.0 + dy * sin( dBestAngle ) + 40 * 4 ) / 4.0 + rctU.x - rct1.x;
		centerU.y = ( dst.rows / 2.0 + dy * cos( dBestAngle ) + 40 * 4 ) / 4.0 + rctU.y - rct1.y;
		ptCenters[0] = centerU;
		pdAngle[0] = 90.0 * CV_PI / 180 - dBestAngle;

		line( imgs, Point( centerU.x - 5, centerU.y ), Point( centerU.x + 5, centerU.y ), Scalar( 0, 0, 255 ) );
		line( imgs, Point( centerU.x, centerU.y - 5 ), Point( centerU.x, centerU.y + 5 ), Scalar( 0, 0, 255 ) );
		SetInterResultImage( 3, imgs, "DOG" );



		rot = getRotationMatrix2D( centerD, dBestAngle, 1.0 );
		warpAffine( imgrctD, dst, rot, imgrctD.size() );
		rctS = Rect( 40 * 4, 40 * 4, dst.cols - 40 * 4 * 2, dst.rows - 40 * 4 * 2 );
		dst = dst( rctS ).clone();
		line( dst, Point( 0, dPosD ), Point( dst.cols - 1, dPosD ), Scalar( 0, 0, 255 ) );
		g_pView->SetInterResultImage( 2, dst, "Bottom  y" );

		dy = dPosD - dst.rows / 2.0;
		centerD.x = ( dst.cols / 2.0 + dy * sin( dBestAngle ) + 40 * 4 ) / 4.0 + rctD.x - rct1.x;
		centerD.y = ( dst.rows / 2.0 + dy * cos( dBestAngle ) + 40 * 4 ) / 4.0 + rctD.y - rct1.y;
		ptCenters[2] = centerD;
		pdAngle[2] = 90.0 * CV_PI / 180 - dBestAngle;



		normalize( ptsval, ptsval, 0, 63, NORM_MINMAX );

		Mat hisu( 64, ptsval.size(), CV_8U, Scalar( 200 ) );
		for ( int i = 0; i < ptsval.size(); i++ )
			for ( int j = 0; j < ptsval[i]; j++ )
				hisu.data[( 63 - j )* ptsval.size() + i] = 64;

		//g_pView->SetInterResultImage( 2, imgBest, "Top  " );
		g_pView->SetInterResultImage( 2, hisu, "Top  y" );





		// left-right
		nSize = MIN( 40, ( nR - nL ) / 2 );
		Rect rctL = Rect( nL + rct1.x - nSize / 2 - 40, nT + rct1.y - nOffY, nSize + 80, nB - nT + 80 );
		Rect rctR = Rect( nR + rct1.x - nSize / 2 - 40, nT + rct1.y - nOffY, nSize + 80, nB - nT + 80 );

		Mat imgrctL = img( rctL ).clone();
		resize( imgrctL, imgrctL, imgrctL.size() * 4 );
		g_pView->SetInterResultImage( 2, imgrctL, "Top  y" );

		Mat imgrctR = img( rctR ).clone();
		resize( imgrctR, imgrctR, imgrctR.size() * 4 );
		//g_pView->SetInterResultImage( 2, imgrctR, "Rown  y" );

		ptsval.clear();
		vector<int> ptsvalL;
		vector<int> ptsvalR;

		Point2d centerL( imgrctL.cols / 2.0, imgrctL.rows / 2.0 );
		Point2d centerR( imgrctR.cols / 2.0, imgrctR.rows / 2.0 );
		for ( double angle = -8.0; angle < 8; angle += 0.1 )
		{
			double minVal, maxValL, maxValR;
			Point minLoc, maxLocL, maxLocR, matchLoc;
			{
				Mat rot = getRotationMatrix2D( centerL, angle, 1.0 );
				Mat dstL;
				warpAffine( imgrctL, dstL, rot, imgrctL.size() );
				Rect rctS( 40 * 4, nOffY * 4, dstL.cols - 40 * 4 * 2, dstL.rows - 40 * 4 * 2 );
				dstL = dstL( rctS ).clone();

				float* pnResL = NULL;
				FilterHC2( dstL.data, dstL.rows, dstL.cols, 5, 10, pnResL, 1 );
				Mat resH( dstL.rows, dstL.cols, CV_32FC1, pnResL );
				//Mat res2;
				//resH.convertTo( res2, CV_8UC1 );
				//g_pView->SetInterResultImage( 2, dstL, "left-dstL  y" );
				//g_pView->SetInterResultImage( 2, res2, "left-right  y" );

				Mat mCol1( dstL.cols, 1, CV_32FC1 );
				reduce( resH, mCol1, 0, REDUCE_SUM, CV_32FC1 );

				minMaxLoc( mCol1, &minVal, &maxValL, &minLoc, &maxLocL );
				delete pnResL;
			}
			{
				Mat rot = getRotationMatrix2D( centerR, angle, 1.0 );
				Mat dstR;
				warpAffine( imgrctR, dstR, rot, imgrctR.size() );
				Rect rctS( 40 * 4, nOffY * 4, dstR.cols - 40 * 4 * 2, dstR.rows - 40 * 4 * 2 );
				dstR = dstR( rctS ).clone();

				float* pnResR = NULL;
				FilterHC2( dstR.data, dstR.rows, dstR.cols, 5, 10, pnResR, 0 );
				Mat resH( dstR.rows, dstR.cols, CV_32FC1, pnResR );
				Mat mCol1( dstR.cols, 1, CV_32FC1 );
				reduce( resH, mCol1, 0, REDUCE_SUM, CV_32FC1 );

				minMaxLoc( mCol1, &minVal, &maxValR, &minLoc, &maxLocR );
				delete pnResR;
			}
			ptsval.push_back( maxValL + maxValR );
			ptsvalL.push_back( maxLocL.x );
			ptsvalR.push_back( maxLocR.x );
		}

		dMaxV = 1;
		dBestAngle = 0;

		for ( int i = 0; i < 3; i++ )
			for ( int j = 1; j < ptsval.size() - 1; j++ )
				ptsval[j] = ( ptsval[j - 1] + ptsval[j + 1] + ptsval[j] ) / 3.0;

		double dPosL = 0, dPosR = 0;
		for ( int i = 0; i < ptsval.size(); i++ )
		{
			if ( ptsval[i] < dMaxV ) continue;
			dMaxV = ptsval[i];
			dPosL = ptsvalL[i];
			dPosR = ptsvalR[i];
			dBestAngle = ( i*0.1 - 8 ) * CV_PI / 180;
		}

		rot = getRotationMatrix2D( centerL, dBestAngle, 1.0 );
		warpAffine( imgrctL, dst, rot, imgrctL.size() );
		rctS = Rect( 40 * 4, 40 * 4, dst.cols - 40 * 4 * 2, dst.rows - 40 * 4 * 2 );
		dst = dst( rctS ).clone();
		line( dst, Point( dPosL, 0 ), Point( dPosL, dst.rows - 1 ), Scalar( 0, 255, 0 ) );
		g_pView->SetInterResultImage( 2, dst, "left-right  y" );

		dy = dPosL - dst.cols / 2.0;
		centerL.x = ( dst.cols / 2.0 + dy * cos( dBestAngle ) + 40 * 4 ) / 4.0 + rctL.x - rct1.x;
		centerL.y = ( dst.rows / 2.0 + dy * sin( dBestAngle ) + 40 * 4 ) / 4.0 + rctL.y - rct1.y;
		ptCenters[3] = centerL;
		pdAngle[3] = -dBestAngle;


		rot = getRotationMatrix2D( centerR, dBestAngle, 1.0 );
		warpAffine( imgrctR, dst, rot, imgrctR.size() );
		rctS = Rect( 40 * 4, 40 * 4, dst.cols - 40 * 4 * 2, dst.rows - 40 * 4 * 2 );
		dst = dst( rctS ).clone();
		line( dst, Point( dPosR, 0 ), Point( dPosR, dst.rows - 1 ), Scalar( 0, 255, 0 ) );
		g_pView->SetInterResultImage( 2, dst, "left  y" );

		dy = dPosR - dst.cols / 2.0;
		centerR.x = ( dst.cols / 2.0 + dy * cos( dBestAngle ) + 40 * 4 ) / 4.0 + rctR.x - rct1.x;
		centerR.y = ( dst.rows / 2.0 + dy * sin( dBestAngle ) + 40 * 4 ) / 4.0 + rctR.y - rct1.y;
		ptCenters[1] = centerR;
		pdAngle[1] = -dBestAngle;
	}

	Mat imgM = img( rct0 ).clone(); // imRes.clone();		
	resize( imgM, imgM, Size( imRes.cols * 8, imRes.rows * 8 ) );
	//cvtColor( imgM, imgM, CV_GRAY2BGR );

	Point2d pts[8];
	for ( int i = 0; i < 4; i++ )
	{
		line( imgM, Point( ptCenters[i].x * 8 - 3, ptCenters[i].y * 8 ), Point( ptCenters[i].x * 8 + 3, ptCenters[i].y * 8 ), Scalar( 0, 0, 255 ) );
		line( imgM, Point( ptCenters[i].x * 8, ptCenters[i].y * 8 - 3 ), Point( ptCenters[i].x * 8, ptCenters[i].y * 8 + 3 ), Scalar( 0, 0, 255 ) );

		pts[2 * i + 0] = Point2d( ptCenters[i].x + ( nB - nT ) / 2.0*sin( pdAngle[i] ), ptCenters[i].y + ( nB - nT ) / 2.0*cos( pdAngle[i] ) );
		pts[2 * i + 1] = Point2d( ptCenters[i].x - ( nB - nT ) / 2.0*sin( pdAngle[i] ), ptCenters[i].y - ( nB - nT ) / 2.0*cos( pdAngle[i] ) );

		line( imgM, Point( pts[2 * i + 0].x * 8, pts[2 * i + 0].y * 8 ), Point( pts[2 * i + 1].x * 8, pts[2 * i + 1].y * 8 ), Scalar( 0, 255, 255 ) );
	}
	SetInterResultImage( 1, imgM, "imgM" );


	for ( int i = 0; i < 4; i++ )
	{
		g_dCorner[i] = Crosspoint( pts[2 * i + 0], pts[2 * i + 1], pts[2 * ( ( i + 1 ) % 4 ) + 0], pts[2 * ( ( i + 1 ) % 4 ) + 1] );
		g_dCorner[i] += Point2d( rct0.x, rct0.y );
	}

	Invalidate( FALSE );
	UpdateWindow();

	delete pResV;
	delete pResH;
}

void CImgPrcView::OnSignRecognize()
{
	/*{
	//String strFileName = format( "E:\\berkeley\\zhan\\New folder\\res\\%ld.bmp", 1000 );
	String strFileName = format( "E:\\berkeley\\zhan\\peggy\\3\\%ld.bmp", 1004 );
	Mat src = imread( strFileName );
	if ( src.rows < 1 ) return;
	char image_name[50];
	int isColor = 1;
	int fps = 30;
	int frameWidth = src.cols;
	int frameHeight = src.rows;

	VideoWriter writer( "E:\\berkeley\\zhan\\peggy\\test_l.avi", VideoWriter::fourcc( 'M', 'J', 'P', 'G' ), fps,
	Size( frameWidth, frameHeight ), isColor );

	for ( int i = 4; i < 349; i++ )
	{
	strFileName = format( "E:\\berkeley\\zhan\\peggy\\3\\%ld.bmp", i + 1000 );
	src = imread( strFileName, 1 );
	if ( src.rows < 1 ) break;
	writer.write( src );
	}
	return;
	}*/

	/*for( int i=0; i<200; i++ )
	{
	if ( !m_pVideo->isOpened() )
	return;

	Mat img1;
	m_pVideo->read( img1 );

	String strFileName = format( "E:\\berkeley\\zhan\\New folder\\%ld.bmp", i + 1000 );
	imwrite( strFileName, img1 );
	}*/
	static int nsWidth = -1;
	static double nsHeight = -1;


	double dLeft = 610;
	double dWidth = 45;
	double dTop = 290;
	double dHeight = 45;
	//static Rect rct1( 610, 290, 45, 44 );
	static Rect rct1( 710, 50, 170, 188 );// 168 );
	static int ii = 347;// 4; //
	int iii = ii++;
	//for ( int iii = 4; iii < 350; iii++ )
	{

		//String strFileName = format( "E:\\berkeley\\zhan\\New folder\\%ld.bmp", i + 1000 );
		String strFileName = format( "F:\\peggy\\1922\\com_image7\\%ld.png", iii + 1000 );
		Mat img = imread( strFileName );
		//Rect rct( 1026, 199, 300, 260 );

		Rect rct0 = rct1;

		//Mat img( m_nHeight, m_nWidth, CV_8UC3, m_pBitmap );

		Mat imgs = img( rct1 ).clone();
		//rectangle( imgs, rct1, Scalar( 0, 0, 255 ), 1 );
		//SetInterResultImage( 0, imgs, "local" );
		//Invalidate( FALSE );
		//UpdateWindow();


		BYTE* pResV = NULL;
		FilterVC( imgs.data, imgs.rows, imgs.cols, 10, 1, pResV );
		//SetInterResultImage( 1, pResV, imgs.rows, imgs.cols, 8, "" );

		BYTE* pResH = NULL;
		FilterHC( imgs.data, imgs.rows, imgs.cols, 1, 10, pResH );
		//SetInterResultImage( 1, pResH, imgs.rows, imgs.cols, 8, "" );

		for ( int i = 0; i < imgs.rows*imgs.cols; i++ )
			pResV[i] = MAX( pResV[i], pResH[i] );

		Mat imRes( imgs.size(), CV_8UC1, pResV );

		//GaussianBlur( imgs, imRes, Size( 6 * 2 + 1, 6 * 2 + 1 ), 0 );
		//imRes = imgs - imRes;
		//normalize( imRes, imRes, 255, 0, CV_MINMAX );
		//cvtColor( imRes, imRes, CV_BGR2GRAY );
		SetInterResultImage( 0, imgs, "imgs" );
		SetInterResultImage( 1, imRes, "FilterVC" );

		Mat mRow( 1, imRes.cols, CV_32SC1 );
		reduce( imRes, mRow, 0, REDUCE_SUM, CV_32SC1 );

		Mat mCol( imRes.rows, 1, CV_32SC1, Scalar( 0 ) );
		reduce( imRes, mCol, 1, REDUCE_SUM, CV_32SC1 );

		mCol = mCol.reshape( 0, 1 );

		int nL, nR, nT, nB;
		nL = nR = nT = nB = 0;
		{
			double minVal, maxVal;
			Point minLoc, maxLoc, matchLoc;
			minMaxLoc( mRow, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
			nL = maxLoc.x;


			Mat mRow1 = mRow.clone();
			normalize( mRow, mRow1, 0, 255, NORM_MINMAX );
			int* pPrj = (int*) mRow1.data;

			Mat hisu( 64, mRow.cols, CV_8U, Scalar( 200 ) );
			for ( int i = 0; i < mRow.cols; i++ )
				for ( int j = 0; j < pPrj[i] / 4; j++ )
					hisu.data[( 63 - j )*mRow.cols + i] = 64;

			Point pt0 = Point( maxLoc.x, 0 );
			Point pt1 = Point( maxLoc.x, 63 - pPrj[(int) nL] / 4 );
			line( hisu, pt0, pt1, Scalar( 128 ), 1 );

			pPrj = (int*) mRow.data;
			for ( int i = MAX( 0, pt0.x - 10 ); i < MIN( mRow.cols, pt0.x + 10 ); i++ )
				pPrj[i] = 0;
			minMaxLoc( mRow, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
			nR = maxLoc.x;
			pPrj = (int*) mRow1.data;
			pt0 = Point( maxLoc.x, 0 );
			pt1 = Point( maxLoc.x, 63 - pPrj[(int) nR] / 4 );
			line( hisu, pt0, pt1, Scalar( 128 ), 1 );
			g_pView->SetInterResultImage( 2, hisu, "mRow  x" );
		}

		if ( nL > nR )		swap( nL, nR );
		int nSize = MIN( 10, ( nR - nL ) / 4 );

		{
			double minVal, maxVal;
			Point minLoc, maxLoc, matchLoc;

			int* pPrj = (int*) mCol.data;
			for ( int i = 1; i < mCol.cols - 1; i++ )
			{
				pPrj[i] = ( pPrj[i - 1] + pPrj[i] + pPrj[i + 1] ) / 3;
			}

			int nTotal = 0;
			int nFrom = 0;
			for ( int i = 0; i < mCol.cols / 2 - 1; i++ )
			{
				if ( pPrj[i] <= nTotal ) continue;
				nTotal = pPrj[i];
				nFrom = i;
			}
			nT = nFrom;

			int* pnPeak = new int[mCol.cols];
			int nMaxCnt = 4;
			FindPeaks( pPrj, mCol.cols, nMaxCnt, 5, pnPeak );

			/*{
			Mat mCol1 = mCol.clone();
			normalize( mCol, mCol1, 0, 255, NORM_MINMAX );
			int* pPrj = (int*) mCol1.data;

			Mat hisu( 64, mCol.cols, CV_8U, Scalar( 200 ) );
			for ( int i = 0; i < mCol.cols; i++ )
			for ( int j = 0; j < pPrj[i] / 4; j++ )
			hisu.data[( 63 - j )*mCol.cols + i] = 64;

			for ( int i = 0; i < nMaxCnt; i++ )
			hisu.data[pnPeak[i]] = 255;

			SetInterResultImage( 1, hisu, "hisu" );
			}*/

			for ( int i = 0; i < nMaxCnt; i++ )
			{
				if ( pnPeak[i] == nT )
				{
					for ( int j = i; j < nMaxCnt; j++ )
						pnPeak[j - i] = pnPeak[j];
					nMaxCnt -= i;
					break;
				}
			}

			/*{
			Mat mCol1 = mCol.clone();
			normalize( mCol, mCol1, 0, 255, NORM_MINMAX );
			int* pPrj = (int*) mCol1.data;

			Mat hisu( 64, mCol.cols, CV_8U, Scalar( 200 ) );
			for ( int i = 0; i < mCol.cols; i++ )
			for ( int j = 0; j < pPrj[i] / 4; j++ )
			hisu.data[( 63 - j )*mCol.cols + i] = 64;

			for ( int i = 0; i < nMaxCnt; i++ )
			hisu.data[pnPeak[i]] = 255;

			SetInterResultImage( 1, hisu, "hisu" );
			}*/

			// nsHeight			
			if ( nsHeight < 1 )
			{
				Mat mCol1 = mCol.clone();
				normalize( mCol, mCol1, 0, 255, NORM_MINMAX );
				pPrj = (int*) mCol1.data;

				Mat hisu( 64, mCol.cols, CV_8U, Scalar( 200 ) );
				for ( int i = 0; i < mCol.cols; i++ )
					for ( int j = 0; j < pPrj[i] / 4; j++ )
						hisu.data[( 63 - j )*mCol.cols + i] = 64;

				Point pt0 = Point( maxLoc.x, 0 );
				Point pt1 = Point( maxLoc.x, 63 - pPrj[(int) nT] / 4 );
				line( hisu, pt0, pt1, Scalar( 128 ), 1 );

				pPrj = (int*) mCol.data;
				for ( int i = MAX( 0, pt0.x - nSize ); i < MIN( mRow.cols, pt0.x + nSize ); i++ )
					pPrj[i] = 0;
				minMaxLoc( mCol, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
				nB = maxLoc.x;

				if ( nT > nB )		swap( nT, nB );

				pPrj = (int*) mCol1.data;
				pt0 = Point( maxLoc.x, 0 );
				pt1 = Point( maxLoc.x, 63 - pPrj[(int) nB] / 4 );
				line( hisu, pt0, pt1, Scalar( 128 ), 1 );

				g_pView->SetInterResultImage( 2, hisu, "mCol  y" );
			}
			else
			{
				nB = nT + nsHeight;
				double nMax = mCol.cols;
				for ( int i = 1; i < nMaxCnt; i++ )
				{
					if ( fabs( pnPeak[i] - ( nT + nsHeight ) ) > nMax ) continue;
					{
						nMax = fabs( pnPeak[i] - ( nT + nsHeight ) );
						nB = pnPeak[i];
					}
				}

				{
					Mat mCol1 = mCol.clone();
					normalize( mCol, mCol1, 0, 255, NORM_MINMAX );
					int* pPrj = (int*) mCol1.data;

					Mat hisu( 64, mCol.cols, CV_8U, Scalar( 200 ) );
					for ( int i = 0; i < mCol.cols; i++ )
						for ( int j = 0; j < pPrj[i] / 4; j++ )
							hisu.data[( 63 - j )*mCol.cols + i] = 64;

					for ( int i = 1; i < nMaxCnt; i++ )
						hisu.data[pnPeak[i]] = 255;

					for ( int i = 0; i < nsHeight; i++ )
						hisu.data[i + nT + mCol.cols] = 255;

					SetInterResultImage( 1, hisu, "mCol  y2" );
				}
			}
			delete pnPeak;
		}

		if ( nT > nB )		swap( nT, nB );
		if ( nsHeight < 0 )
			nsHeight = nB - nL;
		nsHeight = nsHeight * 0.5 + ( nB - nT )*0.5;
		nsHeight = MAX( nsHeight, nR - nL );


		Mat imgs1 = imgs.clone();
		Rect2i rctDet( nL, nT, nR - nL, nB - nT );
		for ( int m = nT; m<nB; m++ )
		{
			int n = nL;
			imgs1.data[( m*imgs1.cols + n ) * 3 + 2] = 255;
			n = nR;
			imgs1.data[( m*imgs1.cols + n ) * 3 + 2] = 255;
		}
		for ( int n = nL; n < nR; n++ )
		{
			int m = nT;
			imgs1.data[( m*imgs1.cols + n ) * 3 + 2] = 255;
			m = nB;
			imgs1.data[( m*imgs1.cols + n ) * 3 + 2] = 255;
		}
		SetInterResultImage( 1, imgs1, "imgs1" );


		// fine detection below:

		Point2d ptCenters[4];
		double pdAngle[4];
		nSize = MIN( 40, ( nB - nT ) / 2 );
		//if ( nSize < 100 )
		{
			// if too small, omit the non-parrell

			// top-bottom
			int nOffY = 40;
			if ( nT + rct1.y - nSize / 2 < nOffY )
				nOffY = nT + rct1.y - nSize / 2;
			Rect rctU = Rect( nL + rct1.x - 40, nT + rct1.y - nSize / 2 - nOffY, nR - nL + 80, nSize + 80 );
			Rect rctD = Rect( nL + rct1.x - 40, nB + rct1.y - nSize / 2 - 40, nR - nL + 80, nSize + 80 );

			Mat imgrctU = img( rctU ).clone();
			resize( imgrctU, imgrctU, imgrctU.size() * 4 );
			g_pView->SetInterResultImage( 2, imgrctU, "Top  y" );

			Mat imgrctD = img( rctD ).clone();
			resize( imgrctD, imgrctD, imgrctD.size() * 4 );
			//g_pView->SetInterResultImage( 2, imgrctD, "Down  y" );

			vector<float> ptsval;
			vector<int> ptsvalU;
			vector<int> ptsvalD;

			Point2d centerU( imgrctU.cols / 2.0, imgrctU.rows / 2.0 );
			Point2d centerD( imgrctD.cols / 2.0, imgrctD.rows / 2.0 );
			for ( double angle = -8.0; angle < 8; angle += 0.1 )
			{
				double minVal, maxValU, maxValD;
				Point minLoc, maxLocU, maxLocD, matchLoc;
				{
					Mat rot = getRotationMatrix2D( centerU, angle, 1.0 );
					Mat dstU;
					warpAffine( imgrctU, dstU, rot, imgrctU.size() );
					Rect rctS( 40 * 4, nOffY * 4, dstU.cols - 40 * 4 * 2, dstU.rows - 40 * 4 * 2 );
					dstU = dstU( rctS ).clone();

					float* pnResU = NULL;
					FilterVC2( dstU.data, dstU.rows, dstU.cols, 10, 5, pnResU, 0 );

					Mat resH( dstU.rows, dstU.cols, CV_32FC1, pnResU );
					Mat mCol1( dstU.rows, 1, CV_32FC1 );
					reduce( resH, mCol1, 1, REDUCE_SUM, CV_32FC1 );


					minMaxLoc( mCol1, &minVal, &maxValU, &minLoc, &maxLocU );

					//Mat res2;
					//resH.convertTo( res2, CV_8UC1 );
					//CString stTemp;
					//stTemp.Format( "%.2f,		%.2f", angle,maxValU );
					//g_pView->SetInterResultImage( 2, res2, " " + stTemp );

					delete pnResU;
				}
				{
					Mat rot = getRotationMatrix2D( centerD, angle, 1.0 );
					Mat dstD;
					warpAffine( imgrctD, dstD, rot, imgrctD.size() );
					Rect rctS( 40 * 4, 40 * 4, dstD.cols - 40 * 4 * 2, dstD.rows - 40 * 4 * 2 );
					dstD = dstD( rctS ).clone();

					float* pnResD = NULL;
					FilterVC2( dstD.data, dstD.rows, dstD.cols, 10, 5, pnResD, 1 );
					Mat resH( dstD.rows, dstD.cols, CV_32FC1, pnResD );
					Mat mCol1( dstD.rows, 1, CV_32FC1 );
					reduce( resH, mCol1, 1, REDUCE_SUM, CV_32FC1 );

					minMaxLoc( mCol1, &minVal, &maxValD, &minLoc, &maxLocD );
					delete pnResD;
				}
				ptsval.push_back( maxValU + maxValD );
				ptsvalU.push_back( maxLocU.y );
				ptsvalD.push_back( maxLocD.y );
			}

			double dMaxV = 1;
			Point2d ptMax;
			Mat imgBest;
			double dBestAngle = 0;

			for ( int i = 0; i<3; i++ )
				for ( int j = 1; j < ptsval.size() - 1; j++ )
					ptsval[j] = ( ptsval[j - 1] + ptsval[j + 1] + ptsval[j] ) / 3.0;

			double dPosU = 0, dPosD = 0;
			for ( int i = 0; i < ptsval.size(); i++ )
			{
				if ( ptsval[i] < dMaxV ) continue;
				dMaxV = ptsval[i];
				dPosU = ptsvalU[i];
				dPosD = ptsvalD[i];
				dBestAngle = ( i*0.1 - 8 ) * CV_PI / 180;
			}

			Mat rot = getRotationMatrix2D( centerU, dBestAngle, 1.0 );
			Mat dst;
			warpAffine( imgrctU, dst, rot, imgrctU.size() );
			Rect rctS( 40 * 4, nOffY * 4, dst.cols - 40 * 4 * 2, dst.rows - 40 * 4 - nOffY * 4 );
			dst = dst( rctS ).clone();
			line( dst, Point( 0, dPosU ), Point( dst.cols - 1, dPosU ), Scalar( 0, 0, 255 ) );
			g_pView->SetInterResultImage( 2, dst, "Top  y" );

			double dy = dPosU - dst.rows / 2.0;
			centerU.x = ( dst.cols / 2.0 + dy * sin( dBestAngle ) + 40 * 4 ) / 4.0 + rctU.x - rct1.x;
			centerU.y = ( dst.rows / 2.0 + dy * cos( dBestAngle ) + 40 * 4 ) / 4.0 + rctU.y - rct1.y;
			ptCenters[0] = centerU;
			pdAngle[0] = 90.0 * CV_PI / 180 - dBestAngle;

			line( imgs, Point( centerU.x - 5, centerU.y ), Point( centerU.x + 5, centerU.y ), Scalar( 0, 0, 255 ) );
			line( imgs, Point( centerU.x, centerU.y - 5 ), Point( centerU.x, centerU.y + 5 ), Scalar( 0, 0, 255 ) );
			SetInterResultImage( 3, imgs, "DOG" );



			rot = getRotationMatrix2D( centerD, dBestAngle, 1.0 );
			warpAffine( imgrctD, dst, rot, imgrctD.size() );
			rctS = Rect( 40 * 4, 40 * 4, dst.cols - 40 * 4 * 2, dst.rows - 40 * 4 * 2 );
			dst = dst( rctS ).clone();
			line( dst, Point( 0, dPosD ), Point( dst.cols - 1, dPosD ), Scalar( 0, 0, 255 ) );
			g_pView->SetInterResultImage( 2, dst, "Bottom  y" );

			dy = dPosD - dst.rows / 2.0;
			centerD.x = ( dst.cols / 2.0 + dy * sin( dBestAngle ) + 40 * 4 ) / 4.0 + rctD.x - rct1.x;
			centerD.y = ( dst.rows / 2.0 + dy * cos( dBestAngle ) + 40 * 4 ) / 4.0 + rctD.y - rct1.y;
			ptCenters[2] = centerD;
			pdAngle[2] = 90.0 * CV_PI / 180 - dBestAngle;



			normalize( ptsval, ptsval, 0, 63, NORM_MINMAX );

			Mat hisu( 64, ptsval.size(), CV_8U, Scalar( 200 ) );
			for ( int i = 0; i < ptsval.size(); i++ )
				for ( int j = 0; j < ptsval[i]; j++ )
					hisu.data[( 63 - j )* ptsval.size() + i] = 64;

			//g_pView->SetInterResultImage( 2, imgBest, "Top  " );
			g_pView->SetInterResultImage( 2, hisu, "Top  y" );





			// left-right
			nSize = MIN( 40, ( nR - nL ) / 2 );
			Rect rctL = Rect( nL + rct1.x - nSize / 2 - 40, nT + rct1.y - nOffY, nSize + 80, nB - nT + 80 );
			Rect rctR = Rect( nR + rct1.x - nSize / 2 - 40, nT + rct1.y - nOffY, nSize + 80, nB - nT + 80 );

			Mat imgrctL = img( rctL ).clone();
			resize( imgrctL, imgrctL, imgrctL.size() * 4 );
			g_pView->SetInterResultImage( 2, imgrctL, "Top  y" );

			Mat imgrctR = img( rctR ).clone();
			resize( imgrctR, imgrctR, imgrctR.size() * 4 );
			//g_pView->SetInterResultImage( 2, imgrctR, "Rown  y" );

			ptsval.clear();
			vector<int> ptsvalL;
			vector<int> ptsvalR;

			Point2d centerL( imgrctL.cols / 2.0, imgrctL.rows / 2.0 );
			Point2d centerR( imgrctR.cols / 2.0, imgrctR.rows / 2.0 );
			for ( double angle = -8.0; angle < 8; angle += 0.1 )
			{
				double minVal, maxValL, maxValR;
				Point minLoc, maxLocL, maxLocR, matchLoc;
				{
					Mat rot = getRotationMatrix2D( centerL, angle, 1.0 );
					Mat dstL;
					warpAffine( imgrctL, dstL, rot, imgrctL.size() );
					Rect rctS( 40 * 4, nOffY * 4, dstL.cols - 40 * 4 * 2, dstL.rows - 40 * 4 * 2 );
					dstL = dstL( rctS ).clone();

					float* pnResL = NULL;
					FilterHC2( dstL.data, dstL.rows, dstL.cols, 5, 10, pnResL, 1 );
					Mat resH( dstL.rows, dstL.cols, CV_32FC1, pnResL );
					//Mat res2;
					//resH.convertTo( res2, CV_8UC1 );
					//g_pView->SetInterResultImage( 2, dstL, "left-dstL  y" );
					//g_pView->SetInterResultImage( 2, res2, "left-right  y" );

					Mat mCol1( dstL.cols, 1, CV_32FC1 );
					reduce( resH, mCol1, 0, REDUCE_SUM, CV_32FC1 );

					minMaxLoc( mCol1, &minVal, &maxValL, &minLoc, &maxLocL );
					delete pnResL;
				}
				{
					Mat rot = getRotationMatrix2D( centerR, angle, 1.0 );
					Mat dstR;
					warpAffine( imgrctR, dstR, rot, imgrctR.size() );
					Rect rctS( 40 * 4, nOffY * 4, dstR.cols - 40 * 4 * 2, dstR.rows - 40 * 4 * 2 );
					dstR = dstR( rctS ).clone();

					float* pnResR = NULL;
					FilterHC2( dstR.data, dstR.rows, dstR.cols, 5, 10, pnResR, 0 );
					Mat resH( dstR.rows, dstR.cols, CV_32FC1, pnResR );
					Mat mCol1( dstR.cols, 1, CV_32FC1 );
					reduce( resH, mCol1, 0, REDUCE_SUM, CV_32FC1 );

					minMaxLoc( mCol1, &minVal, &maxValR, &minLoc, &maxLocR );
					delete pnResR;
				}
				ptsval.push_back( maxValL + maxValR );
				ptsvalL.push_back( maxLocL.x );
				ptsvalR.push_back( maxLocR.x );
			}

			dMaxV = 1;
			dBestAngle = 0;

			for ( int i = 0; i < 3; i++ )
				for ( int j = 1; j < ptsval.size() - 1; j++ )
					ptsval[j] = ( ptsval[j - 1] + ptsval[j + 1] + ptsval[j] ) / 3.0;

			double dPosL = 0, dPosR = 0;
			for ( int i = 0; i < ptsval.size(); i++ )
			{
				if ( ptsval[i] < dMaxV ) continue;
				dMaxV = ptsval[i];
				dPosL = ptsvalL[i];
				dPosR = ptsvalR[i];
				dBestAngle = ( i*0.1 - 8 ) * CV_PI / 180;
			}

			rot = getRotationMatrix2D( centerL, dBestAngle, 1.0 );
			warpAffine( imgrctL, dst, rot, imgrctL.size() );
			rctS = Rect( 40 * 4, 40 * 4, dst.cols - 40 * 4 * 2, dst.rows - 40 * 4 * 2 );
			dst = dst( rctS ).clone();
			line( dst, Point( dPosL, 0 ), Point( dPosL, dst.rows - 1 ), Scalar( 0, 255, 0 ) );
			g_pView->SetInterResultImage( 2, dst, "left-right  y" );

			dy = dPosL - dst.cols / 2.0;
			centerL.x = ( dst.cols / 2.0 + dy * cos( dBestAngle ) + 40 * 4 ) / 4.0 + rctL.x - rct1.x;
			centerL.y = ( dst.rows / 2.0 + dy * sin( dBestAngle ) + 40 * 4 ) / 4.0 + rctL.y - rct1.y;
			ptCenters[3] = centerL;
			pdAngle[3] = -dBestAngle;



			rot = getRotationMatrix2D( centerR, dBestAngle, 1.0 );
			warpAffine( imgrctR, dst, rot, imgrctR.size() );
			rctS = Rect( 40 * 4, 40 * 4, dst.cols - 40 * 4 * 2, dst.rows - 40 * 4 * 2 );
			dst = dst( rctS ).clone();
			line( dst, Point( dPosR, 0 ), Point( dPosR, dst.rows - 1 ), Scalar( 0, 255, 0 ) );
			g_pView->SetInterResultImage( 2, dst, "left  y" );

			dy = dPosR - dst.cols / 2.0;
			centerR.x = ( dst.cols / 2.0 + dy * cos( dBestAngle ) + 40 * 4 ) / 4.0 + rctR.x - rct1.x;
			centerR.y = ( dst.rows / 2.0 + dy * sin( dBestAngle ) + 40 * 4 ) / 4.0 + rctR.y - rct1.y;
			ptCenters[1] = centerR;
			pdAngle[1] = -dBestAngle;



			normalize( ptsval, ptsval, 0, 63, NORM_MINMAX );

			hisu = Mat( 64, ptsval.size(), CV_8U, Scalar( 200 ) );
			for ( int i = 0; i < ptsval.size(); i++ )
				for ( int j = 0; j < ptsval[i]; j++ )
					hisu.data[( 63 - j )* ptsval.size() + i] = 64;

			//g_pView->SetInterResultImage( 2, imgBest, "Top  " );
			g_pView->SetInterResultImage( 2, hisu, "right  y" );
		}
		/*/ top
		{

		//Mat imgrct = imgs( rct ).clone();
		//resize( imgrct, imgrct, imgrct.size() * 4 );
		//g_pView->SetInterResultImage( 2, imgrct, "Top  y" );

		Rect rct = Rect( nL + rct1.x - 40, nT + rct1.y - nSize / 2 - 40, nR - nL + 80, nSize + 80 );

		Mat imgrct = img( rct ).clone();
		resize( imgrct, imgrct, imgrct.size() * 4 );
		g_pView->SetInterResultImage( 2, imgrct, "Top  y" );



		double dMaxV = 1;
		Point2d ptMax;
		Mat imgBest = imgrct.clone();
		vector<float> ptsval;
		vector<int> ptsvalX;
		double dBestAngle = 0;
		Point2d center( imgrct.cols / 2.0, imgrct.rows / 2.0 );
		for ( double angle = -8.0; angle < 8; angle += 0.1 )
		{
		Mat rot = getRotationMatrix2D( center, angle, 1.0 );

		Mat dst;
		warpAffine( imgrct, dst, rot, imgrct.size() );

		Rect rctS( 40*4, 40*4, dst.cols - 40*4*2, dst.rows - 40 * 4 * 2 );
		dst = dst( rctS ).clone();

		float* pnResH = NULL;
		FilterVC2( dst.data, dst.rows, dst.cols,  10, 5, pnResH, 0 );

		Mat resH( dst.rows, dst.cols, CV_32FC1, pnResH );
		Mat mCol1( dst.rows, 1, CV_32FC1 );
		reduce( resH, mCol1, 1, REDUCE_SUM, CV_32FC1 );

		double minVal, maxVal;
		Point minLoc, maxLoc, matchLoc;
		minMaxLoc( mCol1, &minVal, &maxVal, &minLoc, &maxLoc );

		ptsval.push_back( maxVal );
		ptsvalX.push_back( maxLoc.y );
		delete pnResH;
		//CString strTemp;
		//strTemp.Format( "%.2f", maxVal );
		//g_pView->SetInterResultImage( 2, dst, "Top  y " + strTemp );
		}
		for( int i=0; i<3; i++ )
		for ( int j = 1; j < ptsval.size()-1; j++ )
		ptsval[j] = (ptsval[j-1] + ptsval[j+1] + ptsval[j]) / 3.0;

		double dPosY = 0;
		for ( int i = 0; i < ptsval.size(); i++ )
		{
		if ( ptsval[i] < dMaxV ) continue;
		dMaxV = ptsval[i];
		dPosY = ptsvalX[i];
		dBestAngle = (i*0.1 - 8) * CV_PI / 180;
		}

		Mat rot = getRotationMatrix2D( center, dBestAngle, 1.0 );
		Mat dst;
		warpAffine( imgrct, dst, rot, imgrct.size() );
		Rect rctS( 40 * 4, 40 * 4, dst.cols - 40 * 4 * 2, dst.rows - 40 * 4 * 2 );
		dst = dst( rctS ).clone();
		line( dst, Point( 0, dPosY ), Point( dst.cols - 1, dPosY ), Scalar( 0, 0, 255 ) );
		g_pView->SetInterResultImage( 2, dst, "Top  y" );


		double dy = dPosY - dst.rows / 2.0;
		center.x = ( dst.cols / 2.0 + dy * sin( dBestAngle ) + 40 * 4 ) / 4.0 + rct.x - rct1.x;
		center.y = ( dst.rows / 2.0 + dy * cos( dBestAngle ) + 40 * 4 ) / 4.0 + rct.y - rct1.y;
		ptCenters[0] = center;
		pdAngle[0] = 90.0 * CV_PI / 180 - dBestAngle;

		normalize( ptsval, ptsval, 0, 63, NORM_MINMAX );

		Mat hisu( 64, ptsval.size(), CV_8U, Scalar( 200 ) );
		for ( int i = 0; i < ptsval.size(); i++ )
		for ( int j = 0; j < ptsval[i]; j++ )
		hisu.data[( 63 - j )* ptsval.size() + i] = 64;

		//g_pView->SetInterResultImage( 2, imgBest, "Top  " );
		g_pView->SetInterResultImage( 2, hisu, "Top  y" );
		}

		// bottom
		{
		Rect rct = Rect( MAX( 0, nL - 10 ), MAX( (nL+nB)/2, nB - nSize ),
		MIN( imgs.cols - MAX( 0, nL - 10 ), nR - nL + 20 ),
		MIN( imgs.rows - MAX( ( nL + nB ) / 2, nB - nSize ), nSize*2 ) );
		Mat imgrct = imRes( rct ).clone();
		resize( imgrct, imgrct, imgrct.size() * 4 );

		double dMaxV = 1;
		Point2d ptMax;
		Mat imgBest = imgrct.clone();
		vector<float> ptsval;
		double dBestAngle = 0;
		Point2d center( imgrct.cols / 2.0, imgrct.rows / 2.0 );
		for ( double angle = -8.0; angle < 8; angle += 0.1 )
		{
		Mat rot = getRotationMatrix2D( center, angle, 1 );

		Mat dst;
		warpAffine( imgrct, dst, rot, imgrct.size() );

		Mat mCol1( dst.rows, 1, CV_32FC1 );
		reduce( dst, mCol1, 1, REDUCE_SUM, CV_32FC1 );

		double minVal, maxVal;
		Point minLoc, maxLoc, matchLoc;
		minMaxLoc( mCol1, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

		ptsval.push_back( maxVal );
		if ( maxVal < dMaxV ) continue;
		dMaxV = maxVal;
		imgBest = dst.clone();
		ptMax = maxLoc;
		dBestAngle = angle * CV_PI / 180;
		}
		double dy = ptMax.y - imgBest.rows / 2.0;
		center.x = ( imgrct.cols / 2.0 + dy * sin( dBestAngle ) ) / 4.0 + rct.x;
		center.y = ( imgrct.rows / 2.0 + dy * cos( dBestAngle ) ) / 4.0 + rct.y;
		ptCenters[2] = center;
		pdAngle[2] = 90.0 * CV_PI / 180 - dBestAngle;

		Mat hisu( 64, ptsval.size(), CV_8U, Scalar( 200 ) );
		for ( int i = 0; i < ptsval.size(); i++ )
		for ( int j = 0; j < ptsval[i] * 63 / dMaxV; j++ )
		hisu.data[( 63 - j )* ptsval.size() + i] = 64;

		//g_pView->SetInterResultImage( 2, imgBest, "Bottom  " );
		g_pView->SetInterResultImage( 2, hisu, "Bottom  y" );
		}

		// left
		{
		Rect rct = Rect( MAX( 0, nL - 30 ), MAX( 0, nT - 10 ),
		MIN( imgs.cols - MAX( 0, nL - 30 ), 60 ),
		MIN( imgs.rows - MAX( 0, nT - 10 ), nB - nT + 20 ) );
		Mat imgrct = imRes( rct ).clone();
		resize( imgrct, imgrct, imgrct.size() * 4 );

		double dMaxV = 1;
		Point2d ptMax;
		Mat imgBest = imgrct.clone();
		vector<float> ptsval;
		double dBestAngle = 0;
		Point2d center( imgrct.cols / 2.0, imgrct.rows / 2.0 );
		for ( double angle = -8.0; angle < 8; angle += 0.1 )
		{
		Mat rot = getRotationMatrix2D( center, angle, 1 );

		Mat dst;
		warpAffine( imgrct, dst, rot, imgrct.size() );

		Mat mRow1( dst.rows, 1, CV_32FC1 );
		reduce( dst, mRow1, 0, REDUCE_SUM, CV_32FC1 );

		double minVal, maxVal;
		Point minLoc, maxLoc, matchLoc;
		minMaxLoc( mRow1, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

		ptsval.push_back( maxVal );
		if ( maxVal < dMaxV ) continue;
		dMaxV = maxVal;
		imgBest = dst.clone();
		ptMax = maxLoc;
		dBestAngle = angle * CV_PI / 180;
		}
		double dy = ptMax.x - imgBest.cols / 2.0;
		center.x = ( imgrct.cols / 2.0 + dy * cos( dBestAngle ) ) / 4.0 + rct.x;
		center.y = ( imgrct.rows / 2.0 + dy * sin( dBestAngle ) ) / 4.0 + rct.y;
		ptCenters[3] = center;
		pdAngle[3] = -dBestAngle;

		Mat hisu( 64, ptsval.size(), CV_8U, Scalar( 200 ) );
		for ( int i = 0; i < ptsval.size(); i++ )
		for ( int j = 0; j < ptsval[i] * 63 / dMaxV; j++ )
		hisu.data[( 63 - j )* ptsval.size() + i] = 64;

		//g_pView->SetInterResultImage( 2, imgBest, "Left  " );
		g_pView->SetInterResultImage( 2, hisu, "Left  y" );
		}

		// right
		{
		Rect rct = Rect( MAX( (nL+ nR)/2, nR - 30 ), MAX( 0, nT - 10 ),
		MIN( imgs.cols - MAX( ( nL + nR ) / 2, nR - 30 ), 60 ),
		MIN( imgs.rows - MAX( 0, nT - 10 ), nB - nT + 20 ) );
		Mat imgrct = imRes( rct ).clone();
		resize( imgrct, imgrct, imgrct.size() * 4 );

		double dMaxV = 1;
		Point2d ptMax;
		Mat imgBest = imgrct.clone();
		vector<float> ptsval;
		double dBestAngle = 0;
		Point2d center( imgrct.cols / 2.0, imgrct.rows / 2.0 );
		for ( double angle = -8.0; angle < 8; angle += 0.1 )
		{
		Mat rot = getRotationMatrix2D( center, angle, 1 );

		Mat dst;
		warpAffine( imgrct, dst, rot, imgrct.size() );

		Mat mRow1( dst.rows, 1, CV_32FC1 );
		reduce( dst, mRow1, 0, REDUCE_SUM, CV_32FC1 );

		double minVal, maxVal;
		Point minLoc, maxLoc, matchLoc;
		minMaxLoc( mRow1, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

		ptsval.push_back( maxVal );
		if ( maxVal < dMaxV ) continue;
		dMaxV = maxVal;
		imgBest = dst.clone();
		ptMax = maxLoc;
		dBestAngle = angle * CV_PI / 180;
		}
		double dy = ptMax.x - imgBest.cols / 2.0;
		center.x = ( imgrct.cols / 2.0 + dy * cos( dBestAngle ) ) / 4.0 + rct.x;
		center.y = ( imgrct.rows / 2.0 + dy * sin( dBestAngle ) ) / 4.0 + rct.y;
		ptCenters[1] = center;
		pdAngle[1] = -dBestAngle;

		Mat hisu( 64, ptsval.size(), CV_8U, Scalar( 200 ) );
		for ( int i = 0; i < ptsval.size(); i++ )
		for ( int j = 0; j < ptsval[i] * 63 / dMaxV; j++ )
		hisu.data[( 63 - j )* ptsval.size() + i] = 64;

		hisu.data[i] = 255;

		//g_pView->SetInterResultImage( 2, imgBest, "Left  " );
		g_pView->SetInterResultImage( 2, hisu, "Left  y" );
		}*/

		Mat imgM = img( rct0 ).clone(); // imRes.clone();		
		resize( imgM, imgM, Size( imRes.cols * 8, imRes.rows * 8 ) );
		//cvtColor( imgM, imgM, CV_GRAY2BGR );

		Point2d pts[8];
		for ( int i = 0; i < 4; i++ )
		{
			line( imgM, Point( ptCenters[i].x * 8 - 3, ptCenters[i].y * 8 ), Point( ptCenters[i].x * 8 + 3, ptCenters[i].y * 8 ), Scalar( 0, 0, 255 ) );
			line( imgM, Point( ptCenters[i].x * 8, ptCenters[i].y * 8 - 3 ), Point( ptCenters[i].x * 8, ptCenters[i].y * 8 + 3 ), Scalar( 0, 0, 255 ) );

			//line( imgM, Point( ptCenters[i].x + ( nB - nT ) / 2.0*sin( pdAngle[i]), ptCenters[i].y + ( nB - nT ) / 2.0*cos( pdAngle[i] ) ),
			//	Point( ptCenters[i].x - ( nB - nT ) / 2.0*sin( pdAngle[i] ), ptCenters[i].y - ( nB - nT ) / 2.0*cos( pdAngle[i] ) ), Scalar( 128 ) );

			pts[2 * i + 0] = Point2d( ptCenters[i].x + ( nB - nT ) / 2.0*sin( pdAngle[i] ), ptCenters[i].y + ( nB - nT ) / 2.0*cos( pdAngle[i] ) );
			pts[2 * i + 1] = Point2d( ptCenters[i].x - ( nB - nT ) / 2.0*sin( pdAngle[i] ), ptCenters[i].y - ( nB - nT ) / 2.0*cos( pdAngle[i] ) );

			line( imgM, Point( pts[2 * i + 0].x * 8, pts[2 * i + 0].y * 8 ), Point( pts[2 * i + 1].x * 8, pts[2 * i + 1].y * 8 ), Scalar( 0, 255, 255 ) );
		}
		SetInterResultImage( 1, imgM, "imgM" );


		Point2d pts1[4];
		for ( int i = 0; i < 4; i++ )
		{
			pts1[i] = Crosspoint( pts[2 * i + 0], pts[2 * i + 1], pts[2 * ( ( i + 1 ) % 4 ) + 0], pts[2 * ( ( i + 1 ) % 4 ) + 1] );
			pts1[i] += Point2d( rct0.x, rct0.y );
		}


		imgs = img.clone();
		for ( int i = 0; i < 4; i++ )
		{
			//line( imgs, pts1[i], pts1[( i + 1 ) % 4], Scalar( 0, 0, 255 ) );
		}

		Mat imgSign = imread( "E:\\berkeley\\zhan\\peggy\\3.bmp" );
		resize( imgSign, imgSign, Size( ( nR - nL ) * 2, ( nB - nT ) * 2 ), 0, 0, INTER_AREA );
		GaussianBlur( imgSign, imgSign, Size( 3, 3 ), 0.6 );
		SetInterResultImage( 3, imgSign, "imgSign" );


		int nHeightSet = imgs.rows; //1080;
		int nWidthSet = imgs.cols;// 1920;
		int nW = imgs.cols;
		int nH = imgs.rows;

		/*calculate the homography */
		Mat f1( 4, 2, CV_32F );
		Mat f2( 4, 2, CV_32F );

		for ( int i = 0; i < 4; i++ )
		{
			f1.at<float>( i, 0 ) = pts1[i].x * nWidthSet / (double) nW;
			f1.at<float>( i, 1 ) = pts1[i].y * nHeightSet / (double) nH;
		}
		f2.at<float>( 3, 0 ) = 0;
		f2.at<float>( 3, 1 ) = 0;
		f2.at<float>( 0, 0 ) = imgSign.cols;
		f2.at<float>( 0, 1 ) = 0;
		f2.at<float>( 1, 0 ) = imgSign.cols;
		f2.at<float>( 1, 1 ) = imgSign.rows;
		f2.at<float>( 2, 0 ) = 0;
		f2.at<float>( 2, 1 ) = imgSign.rows;
		Mat H = findHomography( f1, f2, CV_LMEDS );

		//resize( imgs, imgs, Size( nWidthSet, nHeightSet ) );

		double* pM = (double*) H.data;
		int nLastY = -1;
		int nLastX = -1;
		for ( int i = 0; i < imgs.rows; i++ )
		{
			int x0 = imgs.cols, x1 = 0;
			for ( int j = 0; j < imgs.cols; j++ )
			{
				double x = ( pM[0 * 3 + 0] * j + pM[0 * 3 + 1] * i + pM[0 * 3 + 2] ) / ( pM[2 * 3 + 0] * j + pM[2 * 3 + 1] * i + pM[2 * 3 + 2] );
				double y = ( pM[1 * 3 + 0] * j + pM[1 * 3 + 1] * i + pM[1 * 3 + 2] ) / ( pM[2 * 3 + 0] * j + pM[2 * 3 + 1] * i + pM[2 * 3 + 2] );
				if ( x < 0 || x >= imgSign.cols || y < 0 || y >= imgSign.rows )
					continue;
				x0 = MIN( j, x0 );
				x1 = MAX( j, x1 );
			}

			for ( int j = x0; j < x1; j++ )
			{
				double x = ( pM[0 * 3 + 0] * j + pM[0 * 3 + 1] * i + pM[0 * 3 + 2] ) / ( pM[2 * 3 + 0] * j + pM[2 * 3 + 1] * i + pM[2 * 3 + 2] );
				double y = ( pM[1 * 3 + 0] * j + pM[1 * 3 + 1] * i + pM[1 * 3 + 2] ) / ( pM[2 * 3 + 0] * j + pM[2 * 3 + 1] * i + pM[2 * 3 + 2] );
				if ( x < 0 || x >= imgSign.cols || y < 0 || y >= imgSign.rows )
					continue;

				double dRatio = 1;
				if ( j == x0 || j == x1 - 1 )
					dRatio = 0.4;
				else if ( j == x0 + 1 || j == x1 - 2 )
					dRatio = 0.8;

				for ( int k = 0; k < 3; k++ )
				{
					int position = (int) (y) * imgSign.cols + (int) ( x );
					//
					int temp = imgSign.data[position * 3 + k] - ( imgSign.data[position * 3 + k] - imgSign.data[( position + 1 ) * 3 + k] )* ( x - (int) x );
					position += imgSign.cols;
					if ( position >= imgSign.cols * imgSign.rows )
						temp -= ( temp )*( y - (int) y );
					else
						temp -= ( temp - ( imgSign.data[position * 3 + k] - ( imgSign.data[position * 3 + k] - imgSign.data[( position + 1 ) * 3 + k] )* ( x - (int) x ) ) )*( y - (int) y );

					imgs.data[( i*imgs.cols + j ) * 3 + k] = temp* dRatio + imgs.data[( i*imgs.cols + j ) * 3 + k] * ( 1 - dRatio );
				}
			}
		}

		SetInterResultImage( 3, imRes, "imRes" );
		SetInterResultImage( 3, imgs, "DOG" );
		Invalidate( FALSE );
		UpdateWindow();

		strFileName = format( "E:\\berkeley\\zhan\\peggy\\4\\%ld.bmp", iii + 1000 );
		imwrite( strFileName, imgs );


		double dOffX = 0;
		double dOffY = 0;
		for ( int i = 0; i < 4; i++ )
		{
			dOffX += pts1[i].x;
			dOffY += pts1[i].y;
		}
		dOffX /= 4;
		dOffY /= 4;

		/*line( imgs, Point( dOffX - 3, dOffY ), Point( dOffX + 3, dOffY ), Scalar( 0, 0, 255 ) );
		line( imgs, Point( dOffX, dOffY - 3 ), Point( dOffX, dOffY + 3 ), Scalar( 0, 0, 255 ) );
		rectangle( imgs, rct1, Scalar( 0, 0, 255 ) );
		SetInterResultImage( 1, imgs, "img" );*/

		line( imgs, Point( rct1.x + rct1.width / 2 - 3, rct1.y + rct1.height / 2 ), Point( rct1.x + rct1.width / 2 + 3, rct1.y + rct1.height / 2 ), Scalar( 0, 255, 0 ) );
		line( imgs, Point( rct1.x + rct1.width / 2, rct1.y + rct1.height / 2 - 3 ), Point( rct1.x + rct1.width / 2, rct1.y + rct1.height / 2 + 3 ), Scalar( 0, 255, 0 ) );

		rct1.x = dOffX - ( nR - nL ) / 2 - 20;
		rct1.y = dOffY - ( nB - nT ) / 2 - 20;

		rct1.width = ( nR - nL ) + 40;
		rct1.height = ( nB - nT ) + 40;

		/*rectangle( imgs, rct1, Scalar( 255, 0, 0 ) );
		SetInterResultImage( 1, imgs, "img" );*/
	}

	Invalidate( FALSE );
	UpdateWindow();
}


BOOL CImgPrcView::PreTranslateMessage( MSG* pMsg )
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		BOOL bCtrlKeyDwn = ( GetKeyState( VK_LCONTROL ) & 0x8000 || GetKeyState( VK_RCONTROL ) & 0x8000 );
		double dScale = bCtrlKeyDwn ? 0.1 : 1;

		switch ( pMsg->wParam )
		{
		case ' ':
			OnSignAutofindsig();
			break;
		case 's':
		case 'S':
			OnSignSave();
			break;
		case 'l':
		case 'L':
			OnSignLoad();
			break;
		case 'd':
		case 'D':
			Delete();
			break;
		case 'a':
		case 'A':
			OnSignShowall();
			break;
		case 'n':
		case 'N':
			OnSignLastimg();
			if ( bCtrlKeyDwn )
				OnButtontrack();
			break;
		case 'm':
		case 'M':
			OnSignNextimg();
			if ( bCtrlKeyDwn )
				OnButtontrack();
			break;
		case 't':
		case 'T':
			OnButtontrackset();
			break;
		case VK_RIGHT:
			Move( dScale, 0 );
			break;
		case VK_LEFT:
			Move( -dScale, 0 );
			break;
		case VK_UP:
			Move( 0, -dScale );
			break;
		case VK_DOWN:
			Move( 0, dScale );
			break;
		case 'c':
		case 'C':
			Copy();
			break;
		case 'v':
		case 'V':
			Paste();
			break;
		}
	}

	return CScrollView::PreTranslateMessage( pMsg );
}
void  CImgPrcView::Delete()
{
	for ( int i = 0; i < 4; i++ )
	{
		g_dCorner[i] = Point2d( -1, -1 );
		g_nCornerType[i] = 0;
	}
	g_nCornerID = 0;

	Invalidate( FALSE );
	UpdateWindow();
}

void  CImgPrcView::Copy()
{
	for ( int i = 0; i < 4; i++ )
	{
		g_nCornerType2[i] = g_nCornerType[i];
		g_dCorner2[i] = g_dCorner[i];
	}
}
void  CImgPrcView::Paste()
{
	for ( int i = 0; i < 4; i++ )
	{
		g_nCornerType[i] = g_nCornerType2[i];
		g_dCorner[i] = g_dCorner2[i];
	}
	Invalidate( FALSE );
	UpdateWindow();
}
void CImgPrcView::Move( double x, double y )
{
	for ( int i = 0; i < 4; i++ )
	{
		g_dCorner[i].x += x;
		g_dCorner[i].y += y;
	}
	Invalidate( FALSE );
	UpdateWindow();
}
void CImgPrcView::OnSignNextimg()
{
	OnSignSave();

	g_nFileID = MIN( g_nFileID + 1, g_strFileNames.GetCount() - 1 );

	if ( g_nFileID < 0 || g_nFileID >= g_strFileNames.GetCount() )
		return;

	CString strPath = m_strFileName.Left( m_strFileName.ReverseFind( '\\' ) );
	m_strFileName = strPath + "\\" + g_strFileNames[g_nFileID];
	if ( !LoadBitmap() )
		return;

	CString strFile = m_strFileName.Mid( m_strFileName.ReverseFind( '\\' ) + 1 );

	for ( int i = 0; i < 4; i++ )
	{
		g_dCorner[i] = Point2d( -1, -1 );
		g_nCornerType[i] = 0;
	}

	g_mapFile2Coner0.Lookup( strFile, g_dCorner[0] );
	g_mapFile2Coner1.Lookup( strFile, g_dCorner[1] );
	g_mapFile2Coner2.Lookup( strFile, g_dCorner[2] );
	g_mapFile2Coner3.Lookup( strFile, g_dCorner[3] );

	g_mapFile2Type0.Lookup( strFile, g_nCornerType[0] );
	g_mapFile2Type1.Lookup( strFile, g_nCornerType[1] );
	g_mapFile2Type2.Lookup( strFile, g_nCornerType[2] );
	g_mapFile2Type3.Lookup( strFile, g_nCornerType[3] );

	g_nCornerID = 0;

	Invalidate( FALSE );
	UpdateWindow();
}

void CImgPrcView::OnSignLastimg()
{
	OnSignSave();

	g_nFileID = MAX( g_nFileID - 1, 0 );

	if ( g_nFileID < 0 || g_nFileID >= g_strFileNames.GetCount() )
		return;

	CString strPath = m_strFileName.Left( m_strFileName.ReverseFind( '\\' ) );
	m_strFileName = strPath + "\\" + g_strFileNames[g_nFileID];
	if ( !LoadBitmap() )
		return;

	CString strFile = m_strFileName.Mid( m_strFileName.ReverseFind( '\\' ) + 1 );

	for ( int i = 0; i < 4; i++ )
	{
		g_dCorner[i] = Point2d( -1, -1 );
		g_nCornerType[i] = 0;
	}

	g_mapFile2Coner0.Lookup( strFile, g_dCorner[0] );
	g_mapFile2Coner1.Lookup( strFile, g_dCorner[1] );
	g_mapFile2Coner2.Lookup( strFile, g_dCorner[2] );
	g_mapFile2Coner3.Lookup( strFile, g_dCorner[3] );

	g_mapFile2Type0.Lookup( strFile, g_nCornerType[0] );
	g_mapFile2Type1.Lookup( strFile, g_nCornerType[1] );
	g_mapFile2Type2.Lookup( strFile, g_nCornerType[2] );
	g_mapFile2Type3.Lookup( strFile, g_nCornerType[3] );

	g_nCornerID = 0;

	Invalidate( FALSE );
	UpdateWindow();
}
void CImgPrcView::OnSignDelcur()
{
	for ( int i = 0; i < 4; i++ )
	{
		g_dCorner[i].x = -1;
		g_dCorner[i].y = -1;
		g_nCornerType[i] = 0;
	}
}
void CImgPrcView::OnSaveBack()
{
	CWaitCursor w;
	// statistic back patterns
	StatisticBackPattern();

	CString strPath = m_strFileName.Left( m_strFileName.ReverseFind( '\\' ) );
	CString strFile = m_strFileName.Mid( m_strFileName.ReverseFind( '\\' ) + 1 );

	strPath = m_strFileName.Left( m_strFileName.ReverseFind( '\\' ) );
	strFile = strPath + "\\back.txt";

	Point2d pt0;

	CStdioFile File( strFile, CFile::modeCreate | CFile::modeWrite );
	CString strTemp;

	for ( int i = 0; i < g_strFileNames.GetCount(); i++ )
	{
		strFile = g_strFileNames[i];

		BYTE r, g, b;
		g_mapFile2BackR.Lookup( strFile, r );
		g_mapFile2BackG.Lookup( strFile, g );
		g_mapFile2BackB.Lookup( strFile, b );

		strTemp.Format( "%s:	%ld	%ld	%ld\n", strFile, r, g, b );

		File.WriteString( strTemp );
	}
	File.Close();
}

void CImgPrcView::OnSignSave()
{
	CString strPath = m_strFileName.Left( m_strFileName.ReverseFind( '\\' ) );
	CString strFile = m_strFileName.Mid( m_strFileName.ReverseFind( '\\' ) + 1 );

	g_mapFile2Coner0.SetAt( strFile, g_dCorner[0] );
	g_mapFile2Coner1.SetAt( strFile, g_dCorner[1] );
	g_mapFile2Coner2.SetAt( strFile, g_dCorner[2] );
	g_mapFile2Coner3.SetAt( strFile, g_dCorner[3] );

	g_mapFile2Type0.SetAt( strFile, g_nCornerType[0] );
	g_mapFile2Type1.SetAt( strFile, g_nCornerType[1] );
	g_mapFile2Type2.SetAt( strFile, g_nCornerType[2] );
	g_mapFile2Type3.SetAt( strFile, g_nCornerType[3] );

	strPath = m_strFileName.Left( m_strFileName.ReverseFind( '\\' ) );
	strFile = strPath + "\\corner.txt";

	Point2d pt1, pt2, pt3, pt0;
	int n1, n2, n3, n0;

	CStdioFile File( strFile, CFile::modeCreate | CFile::modeWrite );
	CString strTemp;

	for ( int i = 0; i < g_strFileNames.GetCount(); i++ )
	{
		strFile = g_strFileNames[i];

		if ( !g_mapFile2Coner0.Lookup( strFile, pt0 ) )
			continue;

		g_mapFile2Coner1.Lookup( strFile, pt1 );
		g_mapFile2Coner2.Lookup( strFile, pt2 );
		g_mapFile2Coner3.Lookup( strFile, pt3 );

		g_mapFile2Type0.Lookup( strFile, n0 );
		g_mapFile2Type1.Lookup( strFile, n1 );
		g_mapFile2Type2.Lookup( strFile, n2 );
		g_mapFile2Type3.Lookup( strFile, n3 );

		if ( pt0.x < 0 && pt0.y < 0 && pt1.x < 0 && pt1.y < 0 && pt2.x < 0 && pt2.y < 0 && pt3.x < 0 && pt3.y < 0 )
			continue;

		strTemp.Format( "%s:	(%f	%f)	(%f	%f)	(%f	%f)	(%f	%f)	%ld	%ld	%ld	%ld\n", strFile,
			pt0.x, pt0.y, pt1.x, pt1.y,
			pt2.x, pt2.y, pt3.x, pt3.y, n0, n1, n2, n3 );

		File.WriteString( strTemp );
	}
	File.Close();
}

void CImgPrcView::OnBackLoad()
{
	g_mapFile2BackB.RemoveAll();
	g_mapFile2BackG.RemoveAll();
	g_mapFile2BackR.RemoveAll();

	CString strPath = m_strFileName.Left( m_strFileName.ReverseFind( '\\' ) );
	CString strFile = strPath + "\\back.txt";
	CFileFind finder;
	if ( !finder.FindFile( strFile ) )
		return;

	int n1, n2, n3, n0;

	CStdioFile File( strFile, CFile::modeRead );
	CString strTemp;
	while ( File.ReadString( strTemp ) )
	{
		strTemp.TrimRight();
		strTemp.TrimLeft();
		if ( strTemp.GetLength() < 1 )
			break;

		strFile = strTemp.Left( strTemp.Find( ":" ) );
		strTemp = strTemp.Mid( strFile.GetLength() + 1 );

		BYTE b, g, r;
		strTemp.TrimLeft();
		r = atol( strTemp );
		strTemp = strTemp.Mid( strTemp.Find( "	" ) + 1 );
		g = atol( strTemp );
		strTemp = strTemp.Mid( strTemp.Find( "	" ) + 1 );
		b = atol( strTemp );

		g_mapFile2BackB.SetAt( strFile, b );
		g_mapFile2BackG.SetAt( strFile, g );
		g_mapFile2BackR.SetAt( strFile, r );
	}
	File.Close();
}
void CImgPrcView::OnSignLoad()
{
	g_mapFile2Coner0.RemoveAll();
	g_mapFile2Coner1.RemoveAll();
	g_mapFile2Coner2.RemoveAll();
	g_mapFile2Coner3.RemoveAll();

	g_mapFile2Type0.RemoveAll();
	g_mapFile2Type1.RemoveAll();
	g_mapFile2Type2.RemoveAll();
	g_mapFile2Type3.RemoveAll();

	CString strPath = m_strFileName.Left( m_strFileName.ReverseFind( '\\' ) );
	CString strFile = strPath + "\\corner.txt";
	CFileFind finder;
	if ( !finder.FindFile( strFile ) )
		return;

	Point2d pt1, pt2, pt3, pt0;
	int n1, n2, n3, n0;

	CStdioFile File( strFile, CFile::modeRead );
	CString strTemp;
	while ( File.ReadString( strTemp ) )
	{
		strFile = strTemp.Left( strTemp.Find( ":" ) );
		strTemp = strTemp.Mid( strFile.GetLength() + 1 );
		sscanf( strTemp, "	(%lf	%lf)	(%lf	%lf)	(%lf	%lf)	(%lf	%lf)	%ld	%ld	%ld	%ld",
			&pt0.x, &pt0.y, &pt1.x, &pt1.y,
			&pt2.x, &pt2.y, &pt3.x, &pt3.y, &n0, &n1, &n2, &n3 );

		if ( pt0.x < 0 && pt0.y < 0 && pt1.x < 0 && pt1.y < 0 && pt2.x < 0 && pt2.y < 0 && pt3.x < 0 && pt3.y < 0 )
			continue;

		g_mapFile2Coner0.SetAt( strFile, pt0 );
		g_mapFile2Coner1.SetAt( strFile, pt1 );
		g_mapFile2Coner2.SetAt( strFile, pt2 );
		g_mapFile2Coner3.SetAt( strFile, pt3 );

		g_mapFile2Type0.SetAt( strFile, n0 );
		g_mapFile2Type1.SetAt( strFile, n1 );
		g_mapFile2Type2.SetAt( strFile, n2 );
		g_mapFile2Type3.SetAt( strFile, n3 );
	}
	File.Close();

	strFile = m_strFileName.Mid( m_strFileName.ReverseFind( '\\' ) + 1 );

	g_mapFile2Coner0.Lookup( strFile, g_dCorner[0] );
	g_mapFile2Coner1.Lookup( strFile, g_dCorner[1] );
	g_mapFile2Coner2.Lookup( strFile, g_dCorner[2] );
	g_mapFile2Coner3.Lookup( strFile, g_dCorner[3] );

	g_mapFile2Type0.Lookup( strFile, g_nCornerType[0] );
	g_mapFile2Type1.Lookup( strFile, g_nCornerType[1] );
	g_mapFile2Type2.Lookup( strFile, g_nCornerType[2] );
	g_mapFile2Type3.Lookup( strFile, g_nCornerType[3] );

	g_nCornerID = 0;

	Invalidate( FALSE );
	UpdateWindow();
}
void CImgPrcView::OnSignShowall()
{
	g_bShowAll = !g_bShowAll;
	Invalidate( FALSE );
	UpdateWindow();
}


double ProjectX( double* pM, double x, double y )
{
	return ( pM[0 * 3 + 0] * x + pM[0 * 3 + 1] * y + pM[0 * 3 + 2] ) / ( pM[2 * 3 + 0] * x + pM[2 * 3 + 1] * y + pM[2 * 3 + 2] );
}
double ProjectY( double* pM, double x, double y )
{
	return ( pM[1 * 3 + 0] * x + pM[1 * 3 + 1] * y + pM[1 * 3 + 2] ) / ( pM[2 * 3 + 0] * x + pM[2 * 3 + 1] * y + pM[2 * 3 + 2] );
}
void ProjectXY( double *pM, double x, double y, double &u, double &v )
{
	u = ProjectX( pM, x, y );
	v = ProjectY( pM, x, y );
}
void GetPos( double destx, double desty, double &x, double &y, double* pM )
{
	for ( double dStep = 0.2; dStep > 0.000001; dStep *= 0.5 )
	{
		double dScope = dStep * 5;
		double dMin = 1000000;
		double dMinx = x;
		double dMiny = y;
		for ( double dx = x - dScope; dx < x + dScope; dx += dStep )
			for ( double dy = y - dScope; dy < y + dScope; dy += dStep )
			{
				double drx, dry;
				ProjectXY( pM, dx, dy, drx, dry );
				if ( ( drx - destx )*( drx - destx ) + ( dry - desty )*( dry - desty ) > dMin )
					continue;
				dMin = ( drx - destx )*( drx - destx ) + ( dry - desty )*( dry - desty );
				dMinx = dx;
				dMiny = dy;
			}
		x = dMinx;
		y = dMiny;
	}
}
void CalProjectPt( double* pM, double x, double y, double u[9], double v[9] )
{
	ProjectXY( pM, x - 1, y - 1, u[0], v[0] );
	ProjectXY( pM, x, y - 1, u[1], v[1] );
	ProjectXY( pM, x + 1, y - 1, u[2], v[2] );

	ProjectXY( pM, x - 1, y, u[3], v[3] );
	ProjectXY( pM, x, y, u[4], v[4] );
	ProjectXY( pM, x + 1, y, u[5], v[5] );

	ProjectXY( pM, x - 1, y + 1, u[6], v[6] );
	ProjectXY( pM, x, y + 1, u[7], v[7] );
	ProjectXY( pM, x + 1, y + 1, u[8], v[8] );
}
void UpdateLocalPt( BYTE r, BYTE g, BYTE b, Mat& matD, int nds, int ndy, double dRatio2 = 1 )
{
	matD.data[( ndy*matD.cols + nds ) * 3 + 2] = r * dRatio2 + matD.data[( ndy*matD.cols + nds ) * 3 + 2] * ( 1.0 - dRatio2 );
	matD.data[( ndy*matD.cols + nds ) * 3 + 1] = g * dRatio2 + matD.data[( ndy*matD.cols + nds ) * 3 + 1] * ( 1.0 - dRatio2 );
	matD.data[( ndy*matD.cols + nds ) * 3 + 0] = b * dRatio2 + matD.data[( ndy*matD.cols + nds ) * 3 + 0] * ( 1.0 - dRatio2 );
}
void UpdateLocalPt( Mat& matS, Mat& matD, double dx, double dy, int nds, int ndy, double dRatio, double dRatio2 = 1 )
{
	for ( int k = 0; k < 3; k++ )
	{
		int position = (int) (dy) * matS.cols + (int) ( dx );

		int temp = matS.data[position * 3 + k] - ( matS.data[position * 3 + k] - matS.data[( position + 1 ) * 3 + k] )* ( dx - (int) dx );
		position += matS.cols;
		if ( position >= matS.cols * matS.rows )
			temp -= ( temp )*( dy - (int) dy );
		else
			temp -= ( temp - ( matS.data[position * 3 + k] - ( matS.data[position * 3 + k] - matS.data[( position + 1 ) * 3 + k] )* ( dx - (int) dx ) ) )*( dy - (int) dy );

		temp = temp * dRatio + matD.data[( ndy*matD.cols + nds ) * 3 + k] * ( 1 - dRatio );
		matD.data[( ndy*matD.cols + nds ) * 3 + k] = temp * dRatio2 + matD.data[( ndy*matD.cols + nds ) * 3 + k] * ( 1.0 - dRatio2 );
	}
}

void GetMeanValue( Mat &img, double* pM, double x, double y, BYTE& r, BYTE& g, BYTE& b )
{
	double u1, v1;
	int nr, ng, nb;
	nr = ng = nb = 0;
	r = g = b = 0;
	int nCount = 0;

	const int nSample = 20;

	for ( int i = 0; i<nSample; i++ )
		for ( int j = 0; j<nSample; j++ )
		{
			ProjectXY( pM, x + j / (double) nSample, y + i / (double) nSample, u1, v1 );
			int u = u1 + 0.5;
			int v = v1 + 0.5;
			if ( u >= img.cols || u < 0 || v < 0 || v >= img.rows )
				continue;

			nr += img.data[( v*img.cols + u ) * 3 + 2];
			ng += img.data[( v*img.cols + u ) * 3 + 1];
			nb += img.data[( v*img.cols + u ) * 3 + 0];
			nCount++;
		}
	if ( !nCount )
		return;
	r = ( nr + nCount / 2 ) / nCount;
	g = ( ng + nCount / 2 ) / nCount;
	b = ( nb + nCount / 2 ) / nCount;
}

void UpdateLocalPt( Mat img, int ny, int nx, Mat &imgSign, double* pM, double dRatio2 = 1 )
{
	double u1, v1;
	int nr, ng, nb;
	nr = ng = nb = 0;
	int nCount = 0;

	BYTE r = img.data[( ny*img.cols + nx ) * 3 + 2];
	BYTE g = img.data[( ny*img.cols + nx ) * 3 + 1];
	BYTE b = img.data[( ny*img.cols + nx ) * 3 + 0];

	const int nSample = 20;

	for ( int i = 0; i < nSample; i++ )
		for ( int j = 0; j < nSample; j++ )
		{
			ProjectXY( pM, nx + j / (double) nSample, ny + i / (double) nSample, u1, v1 );
			int u = u1 + 0.5;
			int v = v1 + 0.5;
			if ( u >= imgSign.cols || u < 0 || v < 0 || v >= imgSign.rows )
				continue;

			nr += imgSign.data[( v*imgSign.cols + u ) * 3 + 2];
			ng += imgSign.data[( v*imgSign.cols + u ) * 3 + 1];
			nb += imgSign.data[( v*imgSign.cols + u ) * 3 + 0];
			nCount++;
		}
	if ( !nCount )
		return;

	double dRatio = nCount / (double) ( nSample*nSample );
	r = ( 1.0 - dRatio ) * r + ( dRatio * nr ) / nCount;
	g = ( 1.0 - dRatio ) * g + ( dRatio * ng ) / nCount;
	b = ( 1.0 - dRatio ) * b + ( dRatio * nb ) / nCount;

	img.data[( ny*img.cols + nx ) * 3 + 2] = img.data[( ny*img.cols + nx ) * 3 + 2] * ( 1 - dRatio2 ) + dRatio2*r;
	img.data[( ny*img.cols + nx ) * 3 + 1] = img.data[( ny*img.cols + nx ) * 3 + 1] * ( 1 - dRatio2 ) + dRatio2*g;
	img.data[( ny*img.cols + nx ) * 3 + 0] = img.data[( ny*img.cols + nx ) * 3 + 0] * ( 1 - dRatio2 ) + dRatio2*b;
}

Mat GetHomography( Point2d pt[4], Mat &imgSign )
{
	Mat f1( 4, 2, CV_32F );
	Mat f2( 4, 2, CV_32F );

	f1.at<float>( 0, 0 ) = pt[1].x;
	f1.at<float>( 0, 1 ) = pt[1].y;
	f1.at<float>( 1, 0 ) = pt[2].x;
	f1.at<float>( 1, 1 ) = pt[2].y;
	f1.at<float>( 2, 0 ) = pt[3].x;
	f1.at<float>( 2, 1 ) = pt[3].y;
	f1.at<float>( 3, 0 ) = pt[0].x;
	f1.at<float>( 3, 1 ) = pt[0].y;

	f2.at<float>( 3, 0 ) = 0;
	f2.at<float>( 3, 1 ) = 0;
	f2.at<float>( 0, 0 ) = imgSign.cols;
	f2.at<float>( 0, 1 ) = 0;
	f2.at<float>( 1, 0 ) = imgSign.cols;
	f2.at<float>( 1, 1 ) = imgSign.rows;
	f2.at<float>( 2, 0 ) = 0;
	f2.at<float>( 2, 1 ) = imgSign.rows;

	return findHomography( f1, f2, CV_LMEDS );
}

UINT CImgPrcView::SignMosaic( LPVOID lp )
{
	CDlgMosaic *pDlg = (CDlgMosaic*) lp;

	CString strPath = g_pView->m_strFileName.Left( g_pView->m_strFileName.ReverseFind( '\\' ) );
	if ( strPath.GetLength() < 1 )
	{
		pDlg->m_pThread = NULL;
		return 1;
	}

	CString strFrameFile = pDlg->m_strSignPath;
	strFrameFile = strFrameFile.Left( strFrameFile.ReverseFind( '\\' ) ) + "\\a.png";
	Mat imgText = imread( format( "%s", strFrameFile ) );
	strFrameFile = strFrameFile.Left( strFrameFile.ReverseFind( '\\' ) ) + "\\meter.png";
	Mat meter = imread( format( "%s", strFrameFile ) );
	strFrameFile = strFrameFile.Left( strFrameFile.ReverseFind( '\\' ) ) + "\\meter0.bmp";
	Mat meter0 = imread( format( "%s", strFrameFile ), CV_LOAD_IMAGE_GRAYSCALE );

	CFileFind finder;
	if ( !finder.FindFile( pDlg->m_strOutputPath + "\\m\\*.*" ) )
		CreateDirectory( pDlg->m_strOutputPath + "\\m", NULL );
	if ( !finder.FindFile( pDlg->m_strOutputPath + "\\l\\*.*" ) )
		CreateDirectory( pDlg->m_strOutputPath + "\\l", NULL );
	if ( !finder.FindFile( pDlg->m_strOutputPath + "\\r\\*.*" ) )
		CreateDirectory( pDlg->m_strOutputPath + "\\r", NULL );


	string strfile = format( "%s", pDlg->m_strSignPath );
	Mat imgSign0 = imread( strfile );
	Mat imgSign;

	Point2d pt[4];
	int n[4];
	BYTE pBack[3];
	CString strFile;

	const int nHeightSet = 1080;
	const int nWidthSet = 1920;

	CStringArray arrFiles;
	POSITION pos = g_mapFile2Coner0.GetStartPosition();
	while ( pos )
	{
		g_mapFile2Coner0.GetNextAssoc( pos, strFile, pt[0] );
		arrFiles.Add( strFile );
	}
	qsort( arrFiles.GetData(), arrFiles.GetSize(), sizeof( CString ), compare00 );

	int nTotal = g_mapFile2Coner0.GetCount();
	int nAngle = 0;
	{
		// mid side
		CString strP = strPath;
		if ( strP.GetLength() > 1 )
		{
			CString strFile = strP + "\\*.png";
			BOOL bWorking = finder.FindFile( strFile );
			while ( bWorking )
			{
				bWorking = finder.FindNextFile();
				nTotal++;
			}
		}

		// left side
		strP = pDlg->m_strLeft;
		if ( strP.GetLength() > 1 )
		{
			CString strFile = strP + "\\*.png";
			BOOL bWorking = finder.FindFile( strFile );
			while ( bWorking )
			{
				bWorking = finder.FindNextFile();
				nTotal++;
			}
		}

		// right side
		strP = pDlg->m_strRight;
		if ( strP.GetLength() > 1 )
		{
			CString strFile = strP + "\\*.png";
			BOOL bWorking = finder.FindFile( strFile );
			while ( bWorking )
			{
				bWorking = finder.FindNextFile();
				nTotal++;
			}
		}
	}
	int nCount = 0;
	for ( int k = 0; k<arrFiles.GetCount(); k++ )
	{
		strFile = arrFiles[k];
		g_mapFile2Coner0.Lookup( strFile, pt[0] );
		g_mapFile2Coner1.Lookup( strFile, pt[1] );
		g_mapFile2Coner2.Lookup( strFile, pt[2] );
		g_mapFile2Coner3.Lookup( strFile, pt[3] );

		g_mapFile2BackR.Lookup( strFile, pBack[2] );
		g_mapFile2BackG.Lookup( strFile, pBack[1] );
		g_mapFile2BackB.Lookup( strFile, pBack[0] );

		int nL = MIN( pt[0].x, MIN( pt[1].x, MIN( pt[2].x, pt[3].x ) ) );
		int nR = MAX( pt[0].x, MAX( pt[1].x, MAX( pt[2].x, pt[3].x ) ) );
		int nT = MIN( pt[0].y, MIN( pt[1].y, MIN( pt[2].y, pt[3].y ) ) );
		int nB = MAX( pt[0].y, MAX( pt[1].y, MAX( pt[2].y, pt[3].y ) ) );

		if ( nR - nL < 5 || nB - nT < 5 )
			continue;

		Mat imgs = imread( format( "%s", strPath + "\\" + strFile ) );

		int nOffX = pDlg->m_nMidX * imgs.cols / nWidthSet;
		int nOffY = pDlg->m_nMidY * imgs.rows / nHeightSet;
		Rect2i rect3( nOffX, nOffY, imgs.cols / 2, imgs.rows / 2 );
		double dRat0 = nWidthSet / (double) ( imgs.cols / 2 );
		resize( imgs( rect3 ).clone(), imgs, Size( nWidthSet, nHeightSet ), 0, 0 );

		double dRat1 = pDlg->m_dRatio; // 3.6;
		double dFrmRt = k / (double) ( pDlg->m_nFrames );
		dRat1 = k < pDlg->m_nFrames ? ( dRat1 * dFrmRt + ( 1 - dFrmRt ) ) : dRat1;

		double dX = 0;
		double dY = 0;
		for ( int i = 0; i < 4; i++ )
		{
			pt[i].x = ( pt[i].x - nOffX ) * dRat0;
			pt[i].y = ( pt[i].y - nOffY ) * dRat0;
			dX += pt[i].x;
			dY += pt[i].y;
		}
		dX /= 4;
		dY /= 4;


		for ( int i = 0; i < 4; i++ )
		{
			pt[i].x = ( pt[i].x - dX ) * dRat1 + dX;
			pt[i].y = ( pt[i].y - dY ) * dRat1 + dY;
		}

		resize( imgSign0, imgSign, Size( MIN( imgSign0.cols, ( nR - nL ) * 4 * dRat1 * dRat0 ), MIN( imgSign0.rows, ( nB - nT ) * 4 * dRat1 * dRat0 ) ), 0, 0, INTER_NEAREST );

		if ( ( nR - nL ) * dRat1 * dRat0 < 70 )
			g_pView->FusionFrame_Texture( imgText, imgSign, imgSign, pBack, 0.5, 0.0, 15 );
		else
		{
			double d0 = MIN( 1, ( ( ( nR - nL ) * dRat1 * dRat0 ) - 70 ) / 31.0 ) * 0.3;
			double d1 = MAX( d0, MIN( 0.8, ( ( ( nR - nL ) * dRat1 * dRat0 ) - 200 ) / 250.0 ) );
			g_pView->FusionFrame_Texture( imgText, imgSign, imgSign, pBack, 0.5, d1, 15 );
		}


		// fading in and out
		dFrmRt = 1;
		if ( k < pDlg->m_nFrames )
			dFrmRt = k / (double) ( pDlg->m_nFrames );

		Mat H = GetHomography( pt, imgSign );
		double* pM = (double*) H.data;


		int nTop = imgs.rows;
		int nLeft = imgs.cols;
		int nRight = 0;
		int nBottom = -1;
		for ( int i = 0; i < imgs.rows; i++ )
		{
			int bFound = 0;
			for ( int j = 0; j < imgs.cols; j++ )
			{
				double x = ProjectX( pM, j, i );
				double y = ProjectY( pM, j, i );
				if ( x < 0 || x >= imgSign.cols || y < 0 || y >= imgSign.rows )
					continue;
				nTop = MIN( nTop, i );
				nLeft = MIN( j, nLeft );
				nRight = MAX( j, nRight );
				bFound = 1;
				nBottom = i;
			}
			if ( nBottom >= 0 && !bFound )
			{
				nBottom = i;
				break;
			}
		}

		if ( nLeft < nRight && nTop < nBottom )
		{
			for ( int i = MAX( 0, nTop - 2 ); i < MIN( imgs.rows, nBottom + 2 ); i++ )
				for ( int j = MAX( 0, nLeft - 2 ); j < MIN( imgs.cols, nRight + 2 ); j++ )
					UpdateLocalPt( imgs, i, j, imgSign, pM, dFrmRt );
		}

		Mat channels[4];
		split( meter, channels );

		Rect2i rct( ( imgs.cols - meter.cols ) / 2, imgs.rows - 1 - meter.rows, meter.cols, meter.rows );
		Mat imgMeter = imgs( rct ).clone();

		for ( int i = 0; i < meter.rows*meter.cols; i++ )
		{
			double dRatio = MAX( channels[0].data[i], MAX( channels[1].data[i], channels[2].data[i] ) ) / 255.0;
			for ( int k = 0; k < 3; k++ )
				imgMeter.data[i * 3 + k] = imgMeter.data[i * 3 + k] * ( 1 - dRatio ) + channels[k].data[i] * dRatio;
		}

		/*Point root_points[1][40];
		root_points[0][0] = Point( 0, 207 );
		root_points[0][1] = Point( 183, 196 );
		root_points[0][2] = Point( width_rotate - 1, 196 );
		root_points[0][3] = Point( width_rotate - 1, 100 );
		root_points[0][4] = Point( 195, 161 );
		root_points[0][5] = Point( 0, 196 );

		const Point* ppt[1] = { root_points[0] };
		int npt[] = { 6 };
		fillPoly(imgMeter,)*/
		nAngle++;
		double dSin = sin( -nAngle*CV_PI / 180 ) * ( imgMeter.rows - 80 );
		double dCos = cos( -nAngle*CV_PI / 180 ) * ( imgMeter.rows - 80 );

		line( imgMeter, Point( imgMeter.cols / 2, imgMeter.rows - 10 ), Point( imgMeter.cols / 2 - dCos, imgMeter.rows - 10 + dSin ), Scalar( 255, 255, 255 ), 5 );
		imgMeter.copyTo( imgs( rct ) );

		strFile.Replace( ".png", ".bmp" );
		string strFileName = format( "%s", pDlg->m_strOutputPath + "\\m\\" + strFile );
		imwrite( strFileName, imgs );

		pDlg->PostMessageA( WM_PROGRESS1, nCount * 100 / ( nTotal ), 0 );
		nCount++;
	}

	// mid side
	if ( strPath.GetLength() > 1 )
	{
		CString strFile = strPath + "\\*.png";
		BOOL bWorking = finder.FindFile( strFile );
		while ( bWorking )
		{
			bWorking = finder.FindNextFile();
			strFile = finder.GetFileName();

			strFile.Replace( ".png", ".bmp" );
			CString strFileD = pDlg->m_strOutputPath + "\\m\\" + strFile;

			CFileFind find2;
			if ( find2.FindFile( strFileD ) ) continue;

			string strFileName = format( "%s", strFileD );
			Mat imgs = imread( format( "%s", finder.GetFilePath() ) );

			int nOffX = pDlg->m_nMidX * imgs.cols / nWidthSet;
			int nOffY = pDlg->m_nMidY * imgs.rows / nHeightSet;

			Rect2i rect3( nOffX, nOffY, imgs.cols / 2, imgs.rows / 2 );
			double dRat0 = nWidthSet / (double) ( imgs.cols / 2 );
			resize( imgs( rect3 ).clone(), imgs, Size( nWidthSet, nHeightSet ), 0, 0 );

			imwrite( strFileName, imgs );

			pDlg->PostMessageA( WM_PROGRESS1, ( nCount ) * 100 / ( nTotal ), 0 );
			nCount++;
		}
	}

	// left side
	strPath = pDlg->m_strLeft;
	if ( strPath.GetLength() > 1 )
	{
		nCount = 0;
		CString strFile = strPath + "\\*.png";
		BOOL bWorking = finder.FindFile( strFile );
		while ( bWorking )
		{
			bWorking = finder.FindNextFile();
			strFile = finder.GetFileName();

			Mat imgs = imread( format( "%s", finder.GetFilePath() ) );
			if ( imgs.rows < 1 || imgs.cols < 1 )
			{
				nCount++;
				continue;
			}

			int nOffX = pDlg->m_nLeftOffX * nW / nWidthSet;
			int nOffY = pDlg->m_nLeftOffY * nH / nHeightSet;

			Rect2i rect3( nOffX, nOffY, nW / 2, nH / 2 );
			double dRat0 = nWidthSet / (double) ( nW / 2 );
			resize( imgs( rect3 ).clone(), imgs, Size( nWidthSet, nHeightSet ), 0, 0 );

			strFile.Replace( ".png", ".bmp" );
			string strFileName = format( "%s", pDlg->m_strOutputPath + "\\l\\" + strFile );
			imwrite( strFileName, imgs );

			pDlg->PostMessageA( WM_PROGRESS1, ( nCount ) * 100 / ( nTotal ), 0 );
			nCount++;
		}
	}

	// right side
	strPath = pDlg->m_strRight;
	if ( strPath.GetLength() > 1 )
	{
		CString strFile = strPath + "\\*.png";
		CFileFind finder;
		BOOL bWorking = finder.FindFile( strFile );
		while ( bWorking )
		{
			bWorking = finder.FindNextFile();
			strFile = finder.GetFileName();

			Mat imgs = imread( format( "%s", finder.GetFilePath() ) );

			int nOffX = pDlg->m_nRightOffX * imgs.cols / nWidthSet;
			int nOffY = pDlg->m_nRightOffY * imgs.rows / nHeightSet;

			Rect2i rect3( nOffX, nOffY, imgs.cols / 2, imgs.rows / 2 );
			double dRat0 = nWidthSet / (double) ( imgs.cols / 2 );
			resize( imgs( rect3 ).clone(), imgs, Size( nWidthSet, nHeightSet ), 0, 0 );

			strFile.Replace( ".png", ".bmp" );
			string strFileName = format( "%s", pDlg->m_strOutputPath + "\\r\\" + strFile );
			imwrite( strFileName, imgs );

			pDlg->PostMessageA( WM_PROGRESS1, ( nCount ) * 100 / ( nTotal ), 0 );
			nCount++;
		}
	}

	pDlg->PostMessageA( WM_PROGRESS1, 100, 0 );

	pDlg->m_pThread = NULL;

	return 1;
}
/*
{
CDlgMosaic *pDlg = (CDlgMosaic*)lp;

CString strPath = g_pView->m_strFileName.Left( g_pView->m_strFileName.ReverseFind( '\\' ) );
if ( strPath.GetLength() < 1 )
{
pDlg->m_pThread = NULL;
return 1;
}

string strfile = format( "%s", pDlg->m_strSignPath );
Mat imgSign0 = imread( strfile );
Mat imgSign;


Point2d pt[4];
int n[4];
CString strFile;

CStringArray arrFiles;
POSITION pos = g_mapFile2Coner0.GetStartPosition();
while ( pos )
{
g_mapFile2Coner0.GetNextAssoc( pos, strFile, pt[0] );
arrFiles.Add( strFile );
}
qsort( arrFiles.GetData(), arrFiles.GetSize(), sizeof( CString ), compare00 );


int nCount = 0;
for( int k=0; k<arrFiles.GetCount(); k++ )
{
strFile = arrFiles[k];
g_mapFile2Coner0.Lookup( strFile, pt[0] );
g_mapFile2Coner1.Lookup( strFile, pt[1] );
g_mapFile2Coner2.Lookup( strFile, pt[2] );
g_mapFile2Coner3.Lookup( strFile, pt[3] );

g_mapFile2Type0.Lookup( strFile, n[0] );
g_mapFile2Type1.Lookup( strFile, n[1] );
g_mapFile2Type2.Lookup( strFile, n[2] );
g_mapFile2Type3.Lookup( strFile, n[3] );

//if( strFile.Find( "1529102854468343711.png" ) < 0 )
//	continue;

int nL = MIN( pt[0].x, MIN( pt[1].x, MIN( pt[2].x, pt[3].x ) ) );
int nR = MAX( pt[0].x, MAX( pt[1].x, MAX( pt[2].x, pt[3].x ) ) );
int nT = MIN( pt[0].y, MIN( pt[1].y, MIN( pt[2].y, pt[3].y ) ) );
int nB = MAX( pt[0].y, MAX( pt[1].y, MAX( pt[2].y, pt[3].y ) ) );

if ( nR - nL < 5 || nB - nT < 5 )
continue;

imgSign = imgSign0.clone();
//resize( imgSign0, imgSign, Size( ( nR - nL ) * 4, ( nB - nT ) * 4 ), 0, 0, INTER_AREA );
//GaussianBlur( imgSign, imgSign, Size( 3, 3 ), 0.3 );

string strfile = format( "%s", strPath + "\\" + strFile );
CFileFind finder;
if ( !finder.FindFile( strPath + "\\" + strFile ) )
continue;
Mat imgs = imread( strfile );

int nHeightSet = 1080; //imgs.rows; //
int nWidthSet  = 1920; //imgs.cols; //
int nW = imgs.cols;
int nH = imgs.rows;

Mat f1( 4, 2, CV_32F );
Mat f2( 4, 2, CV_32F );

f1.at<float>( 0, 0 ) = pt[1].x;
f1.at<float>( 0, 1 ) = pt[1].y;
f1.at<float>( 1, 0 ) = pt[2].x;
f1.at<float>( 1, 1 ) = pt[2].y;
f1.at<float>( 2, 0 ) = pt[3].x;
f1.at<float>( 2, 1 ) = pt[3].y;
f1.at<float>( 3, 0 ) = pt[0].x;
f1.at<float>( 3, 1 ) = pt[0].y;

f2.at<float>( 3, 0 ) = 0;
f2.at<float>( 3, 1 ) = 0;
f2.at<float>( 0, 0 ) = imgSign.cols;
f2.at<float>( 0, 1 ) = 0;
f2.at<float>( 1, 0 ) = imgSign.cols;
f2.at<float>( 1, 1 ) = imgSign.rows;
f2.at<float>( 2, 0 ) = 0;
f2.at<float>( 2, 1 ) = imgSign.rows;
Mat H = findHomography( f1, f2, CV_LMEDS );
warpPerspective( imgs, imgSign, H, imgSign.size() );

string strFileName = format( "%s", pDlg->m_strOutputPath + "\\" + strFile );
imwrite( strFileName, imgSign );

pDlg->PostMessageA( WM_PROGRESS1, nCount * 100 / g_mapFile2Coner0.GetCount(), 0 );
nCount++;
}

pDlg->PostMessageA( WM_PROGRESS1, 100, 0 );

pDlg->m_pThread = NULL;

return 1;
}*/

void CImgPrcView::OnSignProcess()
{
	CDlgMosaic *pdlg = new CDlgMosaic;
	pdlg->Create( IDD_DIALOG19, this );
	pdlg->ShowWindow( SW_SHOW );
}


void CImgPrcView::OnSignAutofindsig()
{
	FindSign();
}


void CImgPrcView::GetBackColor( int nID, BYTE pBack[3] )
{
	pBack[0] = pBack[1] = pBack[2] = 0;

	Point2d pt[4];
	int n[4];

	if ( nID >= g_strFileNames.GetCount() )
		return;

	CString strPath = m_strFileName.Left( m_strFileName.ReverseFind( '\\' ) );
	CString strFileName = g_strFileNames[nID];

	CString strFile = strFileName.Mid( strFileName.ReverseFind( '\\' ) + 1 );

	g_mapFile2Coner0.Lookup( strFile, pt[0] );
	g_mapFile2Coner1.Lookup( strFile, pt[1] );
	g_mapFile2Coner2.Lookup( strFile, pt[2] );
	g_mapFile2Coner3.Lookup( strFile, pt[3] );

	int nL = MIN( pt[0].x, MIN( pt[1].x, MIN( pt[2].x, pt[3].x ) ) );
	int nR = MAX( pt[0].x, MAX( pt[1].x, MAX( pt[2].x, pt[3].x ) ) );
	int nT = MIN( pt[0].y, MIN( pt[1].y, MIN( pt[2].y, pt[3].y ) ) );
	int nB = MAX( pt[0].y, MAX( pt[1].y, MAX( pt[2].y, pt[3].y ) ) );

	if ( nR - nL < 5 || nB - nT < 5 )
		return;

	Mat imgSign( 400, 400, CV_8UC3 );

	Mat imgs = imread( format( "%s", strPath + "\\" + strFileName ) );

	Mat f1( 4, 2, CV_32F );
	Mat f2( 4, 2, CV_32F );

	f1.at<float>( 0, 0 ) = pt[1].x;
	f1.at<float>( 0, 1 ) = pt[1].y;
	f1.at<float>( 1, 0 ) = pt[2].x;
	f1.at<float>( 1, 1 ) = pt[2].y;
	f1.at<float>( 2, 0 ) = pt[3].x;
	f1.at<float>( 2, 1 ) = pt[3].y;
	f1.at<float>( 3, 0 ) = pt[0].x;
	f1.at<float>( 3, 1 ) = pt[0].y;

	f2.at<float>( 3, 0 ) = 0;
	f2.at<float>( 3, 1 ) = 0;
	f2.at<float>( 0, 0 ) = imgSign.cols;
	f2.at<float>( 0, 1 ) = 0;
	f2.at<float>( 1, 0 ) = imgSign.cols;
	f2.at<float>( 1, 1 ) = imgSign.rows;
	f2.at<float>( 2, 0 ) = 0;
	f2.at<float>( 2, 1 ) = imgSign.rows;

	Mat H = findHomography( f1, f2, CV_LMEDS );
	warpPerspective( imgs, imgSign, H, imgSign.size() );

	int pnHis[3][256];
	for ( int j = 0; j < 3; j++ )
		memset( pnHis[j], 0, sizeof( int ) * 256 );

	BYTE* pData = imgSign.data;
	for ( int i = 0; i < imgSign.cols*imgSign.rows; i++ )
	{
		pnHis[0][pData[i * 3 + 0]]++;
		pnHis[1][pData[i * 3 + 1]]++;
		pnHis[2][pData[i * 3 + 2]]++;
	}

	for ( int j = 0; j < 3; j++ )
	{
		int nRatio = imgSign.cols*imgSign.rows*0.1;
		for ( int i = 0; i < 256; i++ )
		{
			nRatio -= pnHis[j][i];
			if ( nRatio > 0 ) continue;
			pBack[j] = i;
			break;
		}
	}
	SetInterResultImage( 1, imgSign, "" );
}

void CImgPrcView::AddBackNoise( Mat &imgSign, double dScale, int nBlurSize )
{
	RNG rng( -1 );
	for ( int i = 0; i < imgSign.cols*imgSign.rows; i++ )
	{
		BOOL bZero = imgSign.data[i * 3 + 0] + imgSign.data[i * 3 + 1] + imgSign.data[i * 3 + 2] < 1;
		for ( int j = 0; j < 3; j++ )
		{
			int nTemp = ( bZero ? rng.gaussian( dScale ) : imgSign.data[i * 3 + j] + rng.gaussian( dScale / 2 ) );
			imgSign.data[i * 3 + j] = MAX( 0, MIN( 255, nTemp ) );
		}
	}

	boxFilter( imgSign, imgSign, -1, Size( nBlurSize, nBlurSize ) );
}

void CImgPrcView::OnSignFindbackpattern()
{
	FreeInterResultImage();

	static char szFilter[] = "|PNG file(*.png)|*.png|MP4 file(*.mp4)|*.mp4|BMP file(*.bmp)|*.bmp||";
	CFileDialog dlg( TRUE, "*.png|*.mp4|*.bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter );
	//if ( dlg.DoModal() != IDOK ) return;

	string strfile = "E:\\berkeley\\zhan\\peggy\\3.bmp"; // format( "%s", dlg.GetPathName() );
	Mat imgSign0 = imread( strfile );
	Mat imgSign = imgSign0.clone();

	SetInterResultImage( 1, imgSign, "imgSign0" );

	AddBackNoise( imgSign, 15, 3 );

	SetInterResultImage( 2, imgSign, "imgSign" );

	Invalidate( FALSE );
	UpdateWindow();
}

void CImgPrcView::StatisticBackPattern()
{
	CString strPath = m_strFileName.Left( m_strFileName.ReverseFind( '\\' ) );
	if ( strPath.GetLength() < 1 )
		return;

	Point2d pt[4];
	int n[4];

	BYTE *ppBack[3];
	for ( int j = 0; j < 3; j++ )
		ppBack[j] = new BYTE[g_strFileNames.GetCount()];

	int nCount = 0;
	for ( int k0 = 0, k = 0; k < g_strFileNames.GetCount(); k++ )
	{
		ppBack[0][k] = ppBack[1][k] = ppBack[2][k] = 255;

		CString strFile = g_strFileNames[k];
		if ( !g_mapFile2Coner0.Lookup( strFile, pt[0] ) )
			continue;

		BYTE pBack[3];
		GetBackColor( k, pBack );
		ppBack[0][k] = pBack[0];
		ppBack[1][k] = pBack[1];
		ppBack[2][k] = pBack[2];

		for ( int i = k0 + 1; i < k; i++ )
		{
			ppBack[0][i] = ( i - k0 ) * ( pBack[0] - ppBack[0][k] ) / (double) ( k - k0 ) + ppBack[0][k];
			ppBack[1][i] = ( i - k0 ) * ( pBack[1] - ppBack[1][k] ) / (double) ( k - k0 ) + ppBack[1][k];
			ppBack[2][i] = ( i - k0 ) * ( pBack[2] - ppBack[2][k] ) / (double) ( k - k0 ) + ppBack[2][k];
		}
		k0 = k;
	}

	int n0 = 0;
	while ( n0 < g_strFileNames.GetCount() && ppBack[0][n0] == 255 ) n0++;
	for ( int i = 0; i < n0 && i < g_strFileNames.GetCount(); i++ )
	{
		ppBack[0][i] = ppBack[0][n0];
		ppBack[1][i] = ppBack[1][n0];
		ppBack[2][i] = ppBack[2][n0];
	}

	n0 = g_strFileNames.GetCount() - 1;
	while ( n0 >= 0 && ppBack[0][n0] == 255 ) n0--;
	for ( int i = g_strFileNames.GetCount() - 1; i > n0; i-- )
	{
		ppBack[0][i] = ppBack[0][n0];
		ppBack[1][i] = ppBack[1][n0];
		ppBack[2][i] = ppBack[2][n0];
	}

	for ( int i = 0; i < 4; i++ )
		for ( int j = 1; j < nCount - 1; j++ )
		{
			ppBack[0][j] = ( ppBack[0][j - 1] + ppBack[0][j + 1] + ppBack[0][j] ) / 3;
			ppBack[1][j] = ( ppBack[1][j - 1] + ppBack[1][j + 1] + ppBack[1][j] ) / 3;
			ppBack[2][j] = ( ppBack[2][j - 1] + ppBack[2][j + 1] + ppBack[2][j] ) / 3;
		}

	g_mapFile2BackB.RemoveAll();
	g_mapFile2BackG.RemoveAll();
	g_mapFile2BackR.RemoveAll();

	for ( int k = 0; k < g_strFileNames.GetCount(); k++ )
	{
		g_mapFile2BackB.SetAt( g_strFileNames[k], ppBack[0][k] );
		g_mapFile2BackG.SetAt( g_strFileNames[k], ppBack[1][k] );
		g_mapFile2BackR.SetAt( g_strFileNames[k], ppBack[2][k] );
	}

	for ( int j = 0; j < 3; j++ )
		delete ppBack[j];
}

// matSign : downsample
void CImgPrcView::FusionFrame_Texture( Mat &matFrame, Mat& matSign, Mat& rest, BYTE pBack[3], double dFrame, double dTexture, double dBackground, double dNoise )
{
	Mat imgSign = matSign.clone();

	int nWnd = 40 * imgSign.cols / matFrame.cols;
	Size sz = imgSign.size();
	sz.width += nWnd * 2;
	sz.height += nWnd * 2;
	resize( matFrame, rest, sz, 0, 0, INTER_AREA );

	for ( int m = 0; m < rest.cols; ++m )
		for ( int n = 0; n < rest.rows; ++n )
		{
			int nID = ( n*rest.cols + m ) * 3;
			if ( m > nWnd && n > nWnd && m < rest.cols - nWnd && n < rest.rows - nWnd )
			{
				continue;
			}
			rest.data[0 + nID] = rest.data[0 + nID] * dFrame + ( 1 - dFrame ) * pBack[0];
			rest.data[1 + nID] = rest.data[1 + nID] * dFrame + ( 1 - dFrame ) * pBack[1];
			rest.data[2 + nID] = rest.data[2 + nID] * dFrame + ( 1 - dFrame ) * pBack[2];
		}

	if ( dNoise > 1 )
		AddBackNoise( imgSign, dNoise, dNoise );

	for ( int m = 0; m < imgSign.cols; ++m )
		for ( int n = 0; n < imgSign.rows; ++n )
		{
			int i = ( n * imgSign.cols + m ) * 3;
			BOOL bZero = ( matSign.data[i + 0] + matSign.data[i + 1] + matSign.data[i + 2] ) < 10;
			for ( int j = 0; j < 3; j++ )
			{
				int nID = j + ( ( n + nWnd )*rest.cols + m + nWnd ) * 3;
				int nSign = imgSign.data[j + i];
				int nLedB = rest.data[nID];
				int nBack = pBack[j];
				if ( bZero )
					rest.data[nID] = nLedB * dTexture + ( nSign * ( 1 - dBackground ) + nBack * dBackground )*( 1 - dTexture );
				else
					rest.data[nID] = nLedB * dTexture + ( nSign )*( 1 - dTexture );
			}
		}
}

void CImgPrcView::OnSignFusiontexture()
{
	static char szFilter[] = "|BMP file(*.bmp)|*.bmp|PNG file(*.png)|*.png||";
	CFileDialog dlg( TRUE, "*.bmp|*.png", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter );
	//if ( dlg.DoModal() != IDOK ) return;

	CString strFilePath = "E:\\berkeley\\zhan\\peggy\\sign\\19_2.bmp"; // dlg.GetPathName();
	CString strPath = strFilePath.Left( strFilePath.ReverseFind( '\\' ) );

	string strfile = format( "%s", strFilePath );
	Mat imgSign0 = imread( strfile );

	string strfile2 = format( "%s", strPath + "\\a.png" );
	Mat imgText = imread( strfile2 );

	Point2d pt[4];
	BYTE pBack[3];
	CString strFileTitle = m_strFileName.Mid( m_strFileName.ReverseFind( '\\' ) + 1 );
	g_mapFile2Coner0.Lookup( strFileTitle, pt[0] );
	g_mapFile2Coner1.Lookup( strFileTitle, pt[1] );
	g_mapFile2Coner2.Lookup( strFileTitle, pt[2] );
	g_mapFile2Coner3.Lookup( strFileTitle, pt[3] );

	g_mapFile2BackR.Lookup( strFileTitle, pBack[2] );
	g_mapFile2BackG.Lookup( strFileTitle, pBack[1] );
	g_mapFile2BackB.Lookup( strFileTitle, pBack[0] );

	int nL = MIN( pt[0].x, MIN( pt[1].x, MIN( pt[2].x, pt[3].x ) ) );
	int nR = MAX( pt[0].x, MAX( pt[1].x, MAX( pt[2].x, pt[3].x ) ) );
	int nT = MIN( pt[0].y, MIN( pt[1].y, MIN( pt[2].y, pt[3].y ) ) );
	int nB = MAX( pt[0].y, MAX( pt[1].y, MAX( pt[2].y, pt[3].y ) ) );

	if ( nR - nL < 5 || nB - nT < 5 )
		return;

	Mat imgs = imread( format( "%s", m_strFileName ) );

	int nHeightSet = 1080; //imgs.rows; //
	int nWidthSet = 1920; //imgs.cols; // 
	int nW = imgs.cols;
	int nH = imgs.rows;

	int nOffX = ( nWidthSet / 4 ) * nW / nWidthSet;
	int nOffY = ( nHeightSet / 4 ) * nH / nHeightSet;
	Rect2i rect3( nOffX, nOffY, nW / 2, nH / 2 );
	double dRat0 = nWidthSet / (double) ( nW / 2 );
	resize( imgs( rect3 ).clone(), imgs, Size( nWidthSet, nHeightSet ), 0, 0 );

	double dRat1 = 1; // pDlg->m_dRatio; // 3.6;
	double dFrmRt = 1; // k / (double) ( pDlg->m_nFrames );
	double dX = 0;
	double dY = 0;
	for ( int i = 0; i < 4; i++ )
	{
		pt[i].x = ( pt[i].x - nOffX ) * dRat0;
		pt[i].y = ( pt[i].y - nOffY ) * dRat0;
		dX += pt[i].x;
		dY += pt[i].y;
	}
	dX /= 4;
	dY /= 4;

	for ( int i = 0; i < 4; i++ )
	{
		pt[i].x = ( pt[i].x - dX ) * dRat1 + dX;
		pt[i].y = ( pt[i].y - dY ) * dRat1 + dY;
	}

	Mat imgSign;
	resize( imgSign0, imgSign, Size( MIN( imgSign0.cols, ( nR - nL ) * 16 * dRat1 * dRat0 ), MIN( imgSign0.rows, ( nB - nT ) * 16 * dRat1 * dRat0 ) ), 0, 0, INTER_NEAREST );

	SetInterResultImage( 0, imgSign, "resize" );

	if ( ( nR - nL ) * dRat1 * dRat0 < 70 )
		FusionFrame_Texture( imgText, imgSign.clone(), imgSign, pBack, 0.5, 0.1, 0.2, 5 );
	else
	{
		double d0 = MIN( 1, ( ( ( nR - nL ) * dRat1 * dRat0 ) - 70 ) / 31.0 ) * 0.3;
		double d1 = MAX( d0, MIN( 0.8, ( ( ( nR - nL ) * dRat1 * dRat0 ) - 200 ) / 250.0 ) );
		FusionFrame_Texture( imgText, imgSign.clone(), imgSign, pBack, 0.5, d1, 0.2, 15 );
	}

	SetInterResultImage( 1, imgSign, "imgSign2" );

	// fading in and out
	dFrmRt = 1;

	Mat H = GetHomography( pt, imgSign );
	double* pM = (double*) H.data;

	int nTop = imgs.rows;
	int nLeft = imgs.cols;
	int nRight = 0;
	int nBottom = -1;
	for ( int i = 0; i < imgs.rows; i++ )
	{
		int bFound = 0;
		for ( int j = 0; j < imgs.cols; j++ )
		{
			double x = ProjectX( pM, j, i );
			double y = ProjectY( pM, j, i );
			if ( x < 0 || x >= imgSign.cols || y < 0 || y >= imgSign.rows )
				continue;
			nTop = MIN( nTop, i );
			nLeft = MIN( j, nLeft );
			nRight = MAX( j, nRight );
			bFound = 1;
			nBottom = i;
		}
		if ( nBottom >= 0 && !bFound )
		{
			nBottom = i;
			break;
		}
	}

	if ( nLeft < nRight && nTop < nBottom )
	{
		for ( int i = MAX( 0, nTop - 2 ); i < MIN( imgs.rows, nBottom + 2 ); i++ )
			for ( int j = MAX( 0, nLeft - 2 ); j < MIN( imgs.cols, nRight + 2 ); j++ )
				UpdateLocalPt( imgs, i, j, imgSign, pM, dFrmRt );
	}

	SetInterResultImage( 1, imgs, "OnSignFusiontexture" );

	Invalidate( FALSE );
	UpdateWindow();
}

double GetArea( Point2d pt[4] )
{
	double a = norm( pt[0] - pt[1] );
	double b = norm( pt[1] - pt[2] );
	double c = norm( pt[0] - pt[3] );
	if ( ( a + b > c ) && ( a + c > b ) && ( b + c > a ) && ( a - b < c ) && ( a - c < b ) && ( b - c < a ) )
	{
		double s = ( a + b + c ) / 2;
		return 2 * sqrt( s * ( s - a ) * ( s - b ) * ( s - c ) );
	}
	return 0;
}

void CImgPrcView::OnSignFilter()
{
	string strFileName = format( "%s", m_strFileName );

	Mat matImg = imread( strFileName );

	Point2d pt[4];
	double *pRatio = NULL;
	double *ppdX[5];
	double *ppdY[5];
	for ( int i = 0; i < 5; i++ )
	{
		ppdX[i] = new double[g_strFileNames.GetCount()];
		ppdY[i] = new double[g_strFileNames.GetCount()];
	}
	pRatio = new double[g_strFileNames.GetCount()];


	int nCount = 0;
	for ( int i = 0; i < g_strFileNames.GetCount(); i++ )
	{
		CString strFile = g_strFileNames[i];
		if ( !g_mapFile2Coner0.Lookup( strFile, pt[0] ) )
			continue;
		g_mapFile2Coner1.Lookup( strFile, pt[1] );
		g_mapFile2Coner2.Lookup( strFile, pt[2] );
		g_mapFile2Coner3.Lookup( strFile, pt[3] );

		double dX = 0;
		double dY = 0;
		for ( int k = 0; k < 4; k++ )
		{
			dX += pt[k].x;
			dY += pt[k].y;
		}
		dX /= 4;
		dY /= 4;

		ppdX[4][nCount] = dX;
		ppdY[4][nCount] = dY;

		pRatio[nCount] = GetArea( pt );

		for ( int k = 0; k < 4; k++ )
		{
			pt[k].x = ( pt[k].x - dX );
			pt[k].y = ( pt[k].y - dY );
			ppdX[k][nCount] = pt[k].x;
			ppdY[k][nCount] = pt[k].y;
			//pt[k].x = ( pt[k].x ) * 15 + 500;
			//pt[k].y = ( pt[k].y ) * 15 + 350;
		}
		//line( matImg, pt[0], pt[1], Scalar( 0, 0, 255 ) );
		//line( matImg, pt[0], pt[3], Scalar( 0, 0, 255 ) );
		//line( matImg, pt[3], pt[2], Scalar( 0, 0, 255 ) );
		//line( matImg, pt[1], pt[2], Scalar( 0, 0, 255 ) );
		nCount++;
	}

	/*double dMax = 0;
	double dMin = pRatio[0];
	for ( int i = 0; i < nCount; i++ )
	{
	dMax = MAX( dMax, pRatio[i] );
	dMin = MIN( dMin, pRatio[i] );
	}

	for ( int i = 1; i < nCount; i++ )
	{
	line( matImg,
	Point( (i - 1)*3, 400 * ( pRatio[i - 1] - dMin ) / ( dMax - dMin ) ),
	Point( i*3, 400 * ( pRatio[i] - dMin ) / ( dMax - dMin ) ),
	Scalar( 0, 0, 255 ) );
	}

	for ( int i = 0; i < 10; i++ )
	for ( int k = 1; k < nCount - 1; k++ )
	{
	pRatio[k] = ( pRatio[k - 1] + pRatio[k + 1] + pRatio[k] ) / 3;
	for ( int m = 0; m < 4; m++ )
	{
	ppdX[m][k] = ( ppdX[m][k - 1] + ppdX[m][k + 1] + ppdX[m][k] ) / 3;
	ppdY[m][k] = ( ppdY[m][k - 1] + ppdY[m][k + 1] + ppdY[m][k] ) / 3;
	}
	}

	for ( int i = 1; i < nCount; i++ )
	{
	line( matImg,
	Point( ( i - 1 ) * 3, 400 * ( pRatio[i - 1] - dMin ) / ( dMax - dMin ) ),
	Point( i * 3, 400 * ( pRatio[i] - dMin ) / ( dMax - dMin ) ),
	Scalar( 0, 255, 0 ) );
	}*/
	/*
	for ( int k = 0; k < 5; k++ )
	{
	double dMax = 0;
	double dMin = ppdX[k][0];
	for ( int i = 0; i < nCount; i++ )
	{
	dMax = MAX( dMax, ppdX[k][i] );
	dMin = MIN( dMin, ppdX[k][i] );
	}

	Scalar sc = Scalar( 0, 0, 255 );
	if ( k == 1 )
	sc = Scalar( 255, 0, 0 );
	else if( k==2  )
	sc = Scalar( 0, 255, 0 );
	else if( k==3  )
	sc = Scalar( 0, 255, 255 );
	else if( k==4 )
	sc = Scalar( 255, 0, 255 );

	for ( int i = 1; i < nCount; i++ )
	{
	line( matImg,
	Point( ( i - 1 ) * 3, 400 * ( ppdX[k][i - 1] - dMin ) / ( dMax - dMin ) ),
	Point( i * 3, 400 * ( ppdX[k][i] - dMin ) / ( dMax - dMin ) ),
	sc );
	}

	for ( int m = 0; m < 10; m++ )
	for ( int i = 1; i < nCount-1; i++ )
	{
	ppdX[k][i] = ( ppdX[k][i - 1] + ppdX[k][i + 1] + ppdX[k][i] ) / 3;
	ppdY[k][i] = ( ppdY[k - 1][i] + ppdY[k + 1][i] + ppdY[k][i] ) / 3;
	}

	for ( int i = 1; i < nCount; i++ )
	{
	line( matImg,
	Point( ( i - 1 ) * 3, 400 * ( ppdX[k][i - 1] - dMin ) / ( dMax - dMin ) ),
	Point( i * 3, 400 * ( ppdX[k][i] - dMin ) / ( dMax - dMin ) ),
	sc );
	}
	}

	SetInterResultImage( 0, matImg, "" );
	Invalidate( FALSE );
	UpdateWindow();
	return;*/


	for ( int i = 0; i < 8; i++ )
		for ( int k = 1; k < nCount - 1; k++ )
		{
			for ( int m = 0; m < 4; m++ )
			{
				ppdX[m][k] = ( ppdX[m][k - 1] + ppdX[m][k + 1] + ppdX[m][k] ) / 3;
				ppdY[m][k] = ( ppdY[m][k - 1] + ppdY[m][k + 1] + ppdY[m][k] ) / 3;
			}
		}

	nCount = 0;
	for ( int i = 0; i < g_strFileNames.GetCount(); i++ )
	{
		CString strFile = g_strFileNames[i];
		if ( !g_mapFile2Coner0.Lookup( strFile, pt[0] ) )
			continue;
		g_mapFile2Coner1.Lookup( strFile, pt[1] );
		g_mapFile2Coner2.Lookup( strFile, pt[2] );
		g_mapFile2Coner3.Lookup( strFile, pt[3] );

		double dX = 0, dY = 0;
		for ( int k = 0; k < 4; k++ )
		{
			dX += pt[k].x;
			dY += pt[k].y;
		}
		dX /= 4;
		dY /= 4;

		for ( int k = 0; k < 4; k++ )
		{
			pt[k].x = dX + ppdX[k][nCount];
			pt[k].y = dY + ppdY[k][nCount];
		}
		g_mapFile2Coner0.SetAt( strFile, pt[0] );
		g_mapFile2Coner1.SetAt( strFile, pt[1] );
		g_mapFile2Coner2.SetAt( strFile, pt[2] );
		g_mapFile2Coner3.SetAt( strFile, pt[3] );

		nCount++;
	}

	Invalidate( FALSE );
	UpdateWindow();
}


void CImgPrcView::OnButtontrackset()
{
	CString strPath = m_strFileName.Left( m_strFileName.ReverseFind( '\\' ) );
	CString strFile = m_strFileName.Mid( m_strFileName.ReverseFind( '\\' ) + 1 );

	string strFileName = format( "%s", m_strFileName );
	Mat matImg = imread( strFileName );

	Point2d pt[4];

	if ( !g_mapFile2Coner0.Lookup( strFile, pt[0] ) )
		return;
	g_mapFile2Coner1.Lookup( strFile, pt[1] );
	g_mapFile2Coner2.Lookup( strFile, pt[2] );
	g_mapFile2Coner3.Lookup( strFile, pt[3] );


	Mat f1( 4, 2, CV_32F );
	Mat f2( 4, 2, CV_32F );

	f1.at<float>( 0, 0 ) = pt[1].x;
	f1.at<float>( 0, 1 ) = pt[1].y;
	f1.at<float>( 1, 0 ) = pt[2].x;
	f1.at<float>( 1, 1 ) = pt[2].y;
	f1.at<float>( 2, 0 ) = pt[3].x;
	f1.at<float>( 2, 1 ) = pt[3].y;
	f1.at<float>( 3, 0 ) = pt[0].x;
	f1.at<float>( 3, 1 ) = pt[0].y;

	double dL = MIN( pt[0].x, MIN( pt[1].x, MIN( pt[2].x, pt[3].x ) ) );
	double dR = MAX( pt[0].x, MAX( pt[1].x, MAX( pt[2].x, pt[3].x ) ) );
	double dT = MIN( pt[0].y, MIN( pt[1].y, MIN( pt[2].y, pt[3].y ) ) );
	double dB = MAX( pt[0].y, MAX( pt[1].y, MAX( pt[2].y, pt[3].y ) ) );

	f2.at<float>( 3, 0 ) = 0;
	f2.at<float>( 3, 1 ) = 0;
	f2.at<float>( 0, 0 ) = dR - dL;
	f2.at<float>( 0, 1 ) = 0;
	f2.at<float>( 1, 0 ) = dR - dL;
	f2.at<float>( 1, 1 ) = dB - dT;
	f2.at<float>( 2, 0 ) = 0;
	f2.at<float>( 2, 1 ) = dB - dT;

	int nW = dR - dL + 2;
	int nH = dB - dT + 2;

	Mat imgSign( nH, nW, CV_8UC3 );
	Mat H = findHomography( f1, f2, CV_LMEDS );
	warpPerspective( matImg, imgSign, H, imgSign.size() );

	Mat imgc;
	cvtColor( imgSign, imgc, CV_BGR2GRAY );

	double a[6];
	a[1] = a[5] = 0;
	a[2] = a[4] = 0;
	a[0] = ( dL + dR ) / 2.0;
	a[3] = ( dB + dT ) / 2.0;

	tracker.TrackSet( NULL, NULL, 0,
		imgc.data, imgc.rows, imgc.cols, matImg.rows, matImg.cols,
		64, 1, 32, 0,
		TT_AFFINE, TTUT_PWOER, a );

	SetInterResultImage( 0, imgSign, "" );
	Invalidate( FALSE );
	UpdateWindow();
}


void CImgPrcView::OnButtontrack()
{
	CString strPath = m_strFileName.Left( m_strFileName.ReverseFind( '\\' ) );
	CString strFile = m_strFileName.Mid( m_strFileName.ReverseFind( '\\' ) + 1 );

	string strFileName = format( "%s", m_strFileName );
	Mat matImg = imread( strFileName );
	cvtColor( matImg, matImg, CV_BGR2GRAY );

	tracker.Track( matImg.data );

	BYTE* pTarget = NULL;
	tracker.GetTargetBitmap( pTarget );
	BYTE* pTemp = NULL;
	tracker.GetTemplateBitmap( pTemp );
	tracker.GetRect( g_dCornerAffine );

	Mat imgTgt( tracker.m_dwTargetHeight, tracker.m_dwTargetWidth, CV_8UC1, pTarget );
	Mat imgTmp( tracker.m_dwTargetHeight, tracker.m_dwTargetWidth, CV_8UC1, pTemp );
	hconcat( imgTgt, imgTmp, imgTgt );

	{
		Mat img = imread( strFileName );
		g_mapFile2Coner0.Lookup( strFile, g_dCorner[0] );
		g_mapFile2Coner1.Lookup( strFile, g_dCorner[1] );
		g_mapFile2Coner2.Lookup( strFile, g_dCorner[2] );
		g_mapFile2Coner3.Lookup( strFile, g_dCorner[3] );

		Mat imgSign0( 400, 400, CV_8UC3 );
		Mat f1( 4, 2, CV_32F );
		Mat f2( 4, 2, CV_32F );
		f1.at<float>( 0, 0 ) = g_dCorner[1].x;
		f1.at<float>( 0, 1 ) = g_dCorner[1].y;
		f1.at<float>( 1, 0 ) = g_dCorner[2].x;
		f1.at<float>( 1, 1 ) = g_dCorner[2].y;
		f1.at<float>( 2, 0 ) = g_dCorner[3].x;
		f1.at<float>( 2, 1 ) = g_dCorner[3].y;
		f1.at<float>( 3, 0 ) = g_dCorner[0].x;
		f1.at<float>( 3, 1 ) = g_dCorner[0].y;
		f2.at<float>( 3, 0 ) = 0;
		f2.at<float>( 3, 1 ) = 0;
		f2.at<float>( 0, 0 ) = 400;
		f2.at<float>( 0, 1 ) = 0;
		f2.at<float>( 1, 0 ) = 400;
		f2.at<float>( 1, 1 ) = 400;
		f2.at<float>( 2, 0 ) = 0;
		f2.at<float>( 2, 1 ) = 400;

		Mat H0 = findHomography( f1, f2, CV_LMEDS );
		warpPerspective( img, imgSign0, H0, imgSign0.size() );


		Mat imgSign1( 400, 400, CV_8UC3 );
		f1.at<float>( 0, 0 ) = g_dCornerAffine[2].x;
		f1.at<float>( 0, 1 ) = g_dCornerAffine[2].y;
		f1.at<float>( 1, 0 ) = g_dCornerAffine[1].x;
		f1.at<float>( 1, 1 ) = g_dCornerAffine[1].y;
		f1.at<float>( 2, 0 ) = g_dCornerAffine[0].x;
		f1.at<float>( 2, 1 ) = g_dCornerAffine[0].y;
		f1.at<float>( 3, 0 ) = g_dCornerAffine[3].x;
		f1.at<float>( 3, 1 ) = g_dCornerAffine[3].y;

		g_dCorner[0] = g_dCornerAffine[3];
		g_dCorner[3] = g_dCornerAffine[0];
		g_dCorner[1] = g_dCornerAffine[2];
		g_dCorner[2] = g_dCornerAffine[1];

		g_mapFile2Coner0.SetAt( strFile, g_dCorner[0] );
		g_mapFile2Coner1.SetAt( strFile, g_dCorner[1] );
		g_mapFile2Coner2.SetAt( strFile, g_dCorner[2] );
		g_mapFile2Coner3.SetAt( strFile, g_dCorner[3] );

		Mat H1 = findHomography( f1, f2, CV_LMEDS );
		warpPerspective( img, imgSign1, H1, imgSign1.size() );

		//string strF = format( "%s", "F:\\peggy\\e1\\" + strFile );
		hconcat( imgSign0, imgSign1, imgSign1 );

		//imwrite( strF, imgSign1 );
	}

	//SetInterResultImage( 0, imgTgt, "target - template" );

	Invalidate( FALSE );
	UpdateWindow();

	delete pTarget;
}
