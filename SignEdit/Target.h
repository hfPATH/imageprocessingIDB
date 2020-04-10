#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;
using namespace cv::ml;

class CTarget
{
public:
	CTarget();
	~CTarget();

	static std::vector<Point2i > m_spPedRange;
		
	
	void Init( Mat& imgC, Mat& imgB, Rect2d rct, int nDir, int nFrameID, int nType=0 );

	int  Update( Mat& imgC, Mat& imgB, int nFrameID );
	int  UpdateBackward( Mat& imgC, Mat& imgB, int nFrameID );
	int	 Track( Mat& imgC, Mat& imgB, double& dX, double& dY, int nBackOder = 0 );

	void Draw( Mat& img );
	
	int BlockMatch(Mat& img, double&x, double&y, double&dRatio );
	int FeatureMatch( Mat& img, double&x, double&y );

	void Store(string &strFileName);
	void UpdateTemplate( Mat& img );
	void Recenter(Rect2d& rctBox, Mat& imgB);
	void Recenter2(Rect2d& rctBox, Mat& imgB);
	void GetCenter2( Rect2d& rctBox1, Mat& imgB );
	void DrawTest( Mat &img );
	void PostProcess();
	void SaveTest();
	bool JudgeCar2Ped();
	int AdjustRect();

	static double CheckMax( std::vector<cv::Point> &pts );
	static void RefreshList( BYTE* pDel );
	static void CheckList();

	int GetMaxCnt( int nCnt );
	void GetLenCnt( double dLen, int &nCnt );
	bool IsLinear( int nCnt );
	bool IsPed();
	bool IsPedOut( int nBackOrder );
	bool IsPedIn( );
	int  CheckTraj();
	void FilterPrj( Mat& mprj );
	void FindMaxLen( Mat& mprj, int nLenSet, int& nX );

	double GetNewSize( double x )
	{
		double dh0, dh1;

		if ( x > m_lastImg.cols - 352 )
			dh1 = 120 + ( x - ( m_lastImg.cols - 352 ) ) * 58 / 240.0;
		else
			dh1 = 80 + ( x - ( m_lastImg.cols - 626 ) ) * 41 / 273.0;

		if( m_nInitPos > m_lastImg.cols - 352 )
			dh0 = 120 + ( m_nInitPos - ( m_lastImg.cols - 352 ) )* 58 / 240.0;
		else 
			dh0 = 80 + ( m_nInitPos - ( m_lastImg.cols - 626 ) ) * 41 / 273.0;

		return dh1 / dh0;
	}
	void ResizeTempAB( Rect2d rctOld, Rect2d rctNew, Mat& imgB );
	void ResizeRect( Rect2d& rect )
	{
		// if object exceeds right side, return
		if ( rect.x + rect.width >= m_lastImg.cols - 10 )
			return;

		double dRatio = GetNewSize( rect.x + rect.width / 2 );

		int nW0 = rect.width;
		int nW1 = m_rcInitSize.width * dRatio;
		if ( abs(nW0 - nW1) > 1 )
		{
			int nAdd = ( nW0 - nW1 ) / 2;
			rect.x += nAdd;
			rect.width -= nAdd * 2;// integer decrease, to keep center
		}
		rect.width = min( m_lastImg.cols - 1 - rect.x, rect.width );

		int nH0 = rect.height;
		int nH1 = m_rcInitSize.height * dRatio;
		if ( nH0 - nH1 > 1 )
		{
			int nAdd = ( nH0 - nH1 ) / 2;
			rect.y += nAdd;
			rect.height -= nAdd * 2;
		}
		rect.height = min( m_lastImg.rows - 1 - rect.y, rect.height );
	}

	void Backup( CTarget& target );
	void Recover( CTarget& target );
	int BackMatch( Mat& img, int& x, int& y );
	int BackMatch2( Mat& img, int& x, int& y );

	void Output();
	static CTarget* LoadTarget( CString &strPath );
	static void LoadTarget( CString &strPath, std::vector<Ptr<CTarget>> &targets );
	static void DrawAll( Mat &img, std::vector<Ptr<CTarget>> &targets );

	Mat m_TempBA;
	Mat m_TempB;
	Mat m_TempG;
	Mat m_TempC;
	Mat m_lastImg;

	std::vector<int> m_pFrmID;
	std::vector<double> m_pRatio;
	std::vector<double> m_pRatio1;
	std::vector<Rect2d> m_pTraj;
	std::vector<Point2f> m_centtrj; // center point
	std::vector<Mat> m_pObj;
	std::vector<Mat> m_pObjMsk;

	int m_nFrameID;
	int m_nTrackTime;
	int m_nStopTime;
	Rect2d m_rctBox;

	Scalar m_scClr;

	Rect m_rcInitSize; // initial size
	int  m_nInitPos;   // initial pos
	double m_dSizeR;   // changing ratio

	double m_dRatio;

	int m_nDirct;      // 0: unknown, 1: forward   2: backward
	int m_bType;       // 0: not sure; 1:car  2:pedestrian
	Point2f m_ptSpeed;

	// for drawing 
	//std::vector<Point2f> prevPts;
	//std::vector<Point2f> currPts;


	static Mat* m_pUpMsk;
	static Mat* m_pDnMsk;

	std::vector<std::vector<Point2f>> pPts;
};

