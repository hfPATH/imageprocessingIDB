#include "stdafx.h"
#include "Target.h"
#include "ImgPrcView.h"

using namespace cv;
using namespace std;
using namespace cv::ml;

extern CImgPrcView *g_pView;

std::vector<Ptr<CTarget>> g_Targets;

std::vector<Point2i > CTarget::m_spPedRange;
Mat* CTarget::m_pUpMsk = NULL;
Mat* CTarget::m_pDnMsk = NULL;


CTarget::CTarget()
{
	m_bType = 0;
	m_ptSpeed = Point2f( 0, 0 );
	m_nTrackTime = 0;
	m_nDirct = 0;

	static cv::RNG rng( time( 0 ) );

	m_scClr = cv::Scalar( rng.uniform( 0, 255 ), rng.uniform( 0, 255 ), rng.uniform( 0, 255 ) );
	m_nStopTime = 0;
}

CTarget::~CTarget()
{
}
/*
void CTarget::BendTraj()
{
	std::vector<Point2f> center;
	std::vector<Point2f> result;
	std::vector<double> ratio;
	std::vector<Point2f> realc;
	int nSize = m_centtrj.size();
	BYTE* pStable = new BYTE[nSize];
	memset( pStable, 0, sizeof( BYTE )*nSize );

	center.push_back( m_centtrj[0] );
	ratio.push_back( m_pRatio1[0] );
	realc.push_back( Point2f( m_pTraj[0].x + m_pTraj[0].width / 2, m_pTraj[0].y + m_pTraj[0].height / 2 ) );
	for ( int i = 1; i < nSize; i++ )
	{
		if ( fabs( m_centtrj[i].x - m_centtrj[i + 1].x ) < 0.1 && fabs( m_centtrj[i].y - m_centtrj[i + 1].y ) < 0.1 )
			continue;
		pStable[i] = 1;
		center.push_back( m_centtrj[i] );
		ratio.push_back( m_pRatio1[i] );
		realc.push_back( Point2f( m_pTraj[i].x + m_pTraj[i].width / 2, m_pTraj[i].y + m_pTraj[i].height / 2 ) );
	}

	int nSize = center.size();
	for ( int i = 0; i < nSize; i++ )
	{
		double dPower = 0;
		double dSumX = 0;
		double dSumY = 0;
		int nCount = 0;
		for ( int j = MAX( i - 20, 0 ); j < MIN( i + 20, nSize ); j++ )
		{
			double d1 = exp( ( j - i ) / 10.0 ) * ratio[i];
			dSumX += d1 * realc[i].x;
			dSumY += d1 * realc[i].y;
			dPower += d1;
			nCount++;
		}
		if ( nCount )
		{
			dSumX /= dPower;
			dSumY /= dPower;
		}

		result.push_back(Point2f(dSumX,dSumY));
	}

	for ( int i = 1; i < MIN( 1000, m_centtrj.size() ); i++ )
	{
		for ( int j = 0; j < m_pRatio1[i] * 999; j++ )
			imgout.data[( i * 1000 + j ) * 3 + 1] -= 30;
		for ( int j = 0; j < m_pRatio[i] * 999; j++ )
			imgout.data[( i * 1000 + j ) * 3 + 2] -= 30;

		

		Point2f pt0 = Point2f( m_centtrj[i - 1].x, i - 1 );
		Point2f pt1 = Point2f( m_centtrj[i].x, i );
		line( imgout, pt0, pt1, cv::Scalar( 0, 0, 255 ), 1 );

		pt0 = Point2f( m_pTraj[i - 1].x + m_pTraj[i - 1].width / 2, i - 1 );
		pt1 = Point2f( m_pTraj[i].x + m_pTraj[i].width / 2, i );
		line( imgout, pt0, pt1, cv::Scalar( 255, 0, 0 ), 1 );


		pt0 = Point2f( m_centtrj[i - 1].y, i - 1 );
		pt1 = Point2f( m_centtrj[i].y, i );
		line( imgout, pt0, pt1, cv::Scalar( 0, 0, 255 ), 1 );

		pt0 = Point2f( m_pTraj[i - 1].y + m_pTraj[i - 1].height / 2, i - 1 );
		pt1 = Point2f( m_pTraj[i].y + m_pTraj[i].height / 2, i );
		line( imgout, pt0, pt1, cv::Scalar( 255, 0, 0 ), 1 );

	}
}*/
void LoadResult( CString strFileName, std::vector<double> &pdata )
{
	pdata.clear();

	CStdioFile File;
	File.Open( strFileName, CFile::modeRead );

	CString strTemp;
	while ( File.ReadString( strTemp ) )
		pdata.push_back( atof( strTemp ) );
	
	File.Close();
}
void WriteResult( CString strFileName, std::vector<double> &pdata )
{
	CStdioFile File;
	File.Open( strFileName, CFile::modeCreate | CFile::modeWrite );
	for ( int i = 0; i < pdata.size(); i++ )
	{
		CString strTemp;
		strTemp.Format( "%.2f\n", pdata[i] );
		File.WriteString( strTemp );
	}
	File.Close();
}
void LoadResult( CString strFileName, std::vector<Point2f> &pdata )
{
	pdata.clear();

	CStdioFile File;
	File.Open( strFileName, CFile::modeRead );

	CString strTemp;
	while ( File.ReadString( strTemp ) )
	{
		CString strX = strTemp.Left( strTemp.Find( "," ) );
		CString strY = strTemp.Mid( strTemp.Find( "," ) + 1);
		pdata.push_back( Point2f(atof( strX ), atof( strY)) );
	}
	File.Close();
}
void WriteResult( CString strFileName, std::vector<Point2f> &pdata )
{
	CStdioFile File;
	File.Open( strFileName, CFile::modeCreate | CFile::modeWrite );
	for ( int i = 0; i < pdata.size(); i++ )
	{
		CString strTemp;
		strTemp.Format( "%.2f,%.2f\n", pdata[i].x,pdata[i].y );
		File.WriteString( strTemp );
	}
	File.Close();
}
void LoadResult( CString strFileName, std::vector<Rect2d> &pdata )
{
	pdata.clear();

	CStdioFile File;
	File.Open( strFileName, CFile::modeRead );

	CString strTemp;
	while ( File.ReadString( strTemp ) )
	{
		CString strX = strTemp.Left( strTemp.Find( "," ) );
		strTemp = strTemp.Mid( strTemp.Find( "," ) + 1 );
		CString strY = strTemp.Left( strTemp.Find( "," ) );
		strTemp = strTemp.Mid( strTemp.Find( "," ) + 1 );
		CString strW = strTemp.Left( strTemp.Find( "," ) );
		CString strH = strTemp.Mid( strTemp.Find( "," )+1 );
		pdata.push_back( Rect2d( atof( strX ), atof( strY ), atof(strW), atof(strH) ) );
	}
	File.Close();
}
void WriteResult( CString strFileName, std::vector<Rect2d> &pdata )
{
	CStdioFile File;
	File.Open( strFileName, CFile::modeCreate | CFile::modeWrite );
	for ( int i = 0; i < pdata.size(); i++ )
	{
		CString strTemp;
		strTemp.Format( "%.2f,%.2f,%.2f,%.2f\n", pdata[i].x, pdata[i].y, pdata[i].width, pdata[i].height );
		File.WriteString( strTemp );
	}
	File.Close();
}
void CTarget::SaveTest()
{
	WriteResult( "f:\\m_pRatio1.txt", m_pRatio1 );
	WriteResult( "f:\\m_pRatio.txt", m_pRatio );
	WriteResult( "f:\\m_centtrj.txt", m_centtrj );
	WriteResult( "f:\\m_pTraj.txt", m_pTraj );
}
void CTarget::PostProcess()
{
	if ( !( m_bType ==1 && m_nDirct == 2 ) )
		return;

	int nSize = m_centtrj.size();
	if ( nSize < 2 )
		return;

	//Mat imgout( 1200, 1000, CV_8UC3, Scalar( 200, 200, 200 ) );
	//DrawTest( imgout );
/*
	std::vector<double> rsltX;
	std::vector<double> rsltY;
	std::vector<int> pnID;	 

	pnID.push_back( 0 );
	rsltX.push_back( m_centtrj[0].x - ( m_pTraj[0].x + m_pTraj[0].width / 2 ) );
	rsltY.push_back( m_centtrj[0].y - ( m_pTraj[0].y + m_pTraj[0].height / 2 ) );

	for ( int i = 1; i < nSize; i++ )
	{
		if ( fabs( m_centtrj[i].x - m_centtrj[i - 1].x ) < 0.1 && fabs( m_centtrj[i].y - m_centtrj[i - 1].y ) < 0.1 )
		{
			pnID.push_back( rsltX.size()-1 );
			continue;
		}
		pnID.push_back( rsltX.size() );
		rsltX.push_back( m_centtrj[i].x - ( m_pTraj[i].x + m_pTraj[i].width / 2 ) );
		rsltY.push_back( m_centtrj[i].y - ( m_pTraj[i].y + m_pTraj[i].height / 2 ) );
	}	
	blur( rsltX, rsltX, Size( 40, 1 ) );
	blur( rsltY, rsltY, Size( 120, 1 ) );
	*/
	for ( int i = 0; i < nSize; i++ )
	{
		//m_centtrj[i].x -= rsltX[pnID[i]];
		//m_centtrj[i].y -= rsltY[pnID[i]];

		m_pTraj[i].x = m_centtrj[i].x - m_pTraj[i].width  / 2;
		m_pTraj[i].y = m_centtrj[i].y - m_pTraj[i].height / 2;
	}

	//DrawTest( imgout );
}

void CTarget::DrawTest( Mat &imgout )
{
//	LoadResult( "f:\\m_pRatio1.txt", m_pRatio1 );
//	LoadResult( "f:\\m_pRatio.txt", m_pRatio );
//	LoadResult( "f:\\m_centtrj.txt", m_centtrj );
//	LoadResult( "f:\\m_pTraj.txt", m_pTraj );
	{
		for ( int i = 1; i < MIN( 1000, m_centtrj.size() ); i++ )
		{
			for ( int j = 0; j < m_pRatio1[i] * 999; j++ )
				imgout.data[( i * 1000 + j ) * 3 + 1] -= 30;
			for ( int j = 0; j < m_pRatio[i] * 999; j++ )
				imgout.data[( i * 1000 + j ) * 3 + 2] -= 30;

			double dPower = 0;
			double dSum = 0;
			int nCount = 0;
			for ( int j = MAX( i - 20, 0 ); j < MIN( i + 20, m_centtrj.size() ); j++ )
			{
				double d1 = exp( ( j - i ) / 40.0 );
				dSum += d1 * m_pRatio1[j];
				dPower += d1;
				nCount++;
			}
			if ( nCount )
				dSum /= dPower;

			Point2f pt0 = Point2f( m_centtrj[i - 1].x, i - 1 );
			Point2f pt1 = Point2f( m_centtrj[i].x, i );
			line( imgout, pt0, pt1, cv::Scalar( 0, 0, 255 ), 1 );

			pt0 = Point2f( m_pTraj[i - 1].x + m_pTraj[i - 1].width / 2, i - 1 );
			pt1 = Point2f( m_pTraj[i].x + m_pTraj[i].width / 2, i );
			line( imgout, pt0, pt1, cv::Scalar( 255, 0, 0 ), 1 );


			pt0 = Point2f( m_centtrj[i - 1].y, i - 1 );
			pt1 = Point2f( m_centtrj[i].y, i );
			line( imgout, pt0, pt1, cv::Scalar( 0, 0, 255 ), 1 );

			//pt0 = Point2f( m_pTraj[i - 1].y + m_pTraj[i - 1].height / 2, i - 1 );
			//pt1 = Point2f( m_pTraj[i].y + m_pTraj[i].height / 2, i );
			//line( imgout, pt0, pt1, cv::Scalar( 255, 0, 0 ), 1 );

			int j = dSum * 999;
			imgout.data[( i * 1000 + j ) * 3 + 1] = imgout.data[( i * 1000 + j ) * 3 + 0] = 0;
		}

		imwrite( "f:\\2.bmp", imgout );	
	}
}
void CTarget::Draw( Mat& img )
{
	Rect2d rctSearch = m_rctBox;
	int nSize = MIN( 30, m_rctBox.width / 3 );
	rctSearch.x = MAX( 0, rctSearch.x - nSize );
	rctSearch.y = MAX( 0, rctSearch.y - nSize );
	rctSearch.width = MIN( m_rctBox.width + nSize * 2, img.cols - 1 - rctSearch.x );
	rctSearch.height = MIN( m_rctBox.height + nSize * 2, img.rows - 1 - rctSearch.y );

	rectangle( img, m_rctBox, m_scClr, 1, 8, 0 );

#ifdef _DEBUG
	if(! ( rctSearch.width < 1 || rctSearch.height < 1 ) )
		rectangle( img, rctSearch, m_scClr, 1, 8, 0 );

	/*/ feature tracking
	for ( int k = 1; k < pPts.size(); k++ )
	{
		for ( int i = 0; i < prevPts.size(); i++ )
		{
			Point2f pt0 = Point2f( prevPts[i].x + rctSearch.x, prevPts[i].y + rctSearch.y );
			Point2f pt1 = Point2f( currPts[i].x + rctSearch.x, currPts[i].y + rctSearch.y );
			line( img, pt0, pt1, cv::Scalar( 0, 0, 255 ), 1 );
		}
	}*/
#endif

	for ( int i = 1; i < m_pTraj.size(); i++ )
	{
		Point2f pt0 = Point2f( m_pTraj[i - 1].x + m_pTraj[i - 1].width / 2, m_pTraj[i - 1].y + m_pTraj[i - 1].height / 2 );
		Point2f pt1 = Point2f( m_pTraj[i    ].x + m_pTraj[i    ].width / 2, m_pTraj[i    ].y + m_pTraj[i    ].height / 2 );
		//rectangle( img, m_pTraj[i], m_scClr, 1, 8, 0 );
		line( img, pt0, pt1, m_scClr, 1 );

		if ( m_bType != 2 )
		{
			pt0 = Point2f( m_centtrj[i - 1].x, m_centtrj[i - 1].y );
			pt1 = Point2f( m_centtrj[i].x, m_centtrj[i].y );
			line( img, pt0, pt1, cv::Scalar( 0, 0, 255 ), 1 );
		}
	}	

#ifdef _DEBUG
	for ( int i = 1; i < m_pTraj.size(); i++ )
	{
		Point2f pt0 = Point2f( m_pTraj[i - 1].x + m_pTraj[i - 1].width / 2, i-1 );
		Point2f pt1 = Point2f( m_pTraj[i].x + m_pTraj[i].width / 2, i );
		line( img, pt0, pt1, m_scClr, 1 );

		pt0 = Point2f( m_centtrj[i - 1].x, i-1 );
		pt1 = Point2f( m_centtrj[i].x, i );
		line( img, pt0, pt1, cv::Scalar( 0, 0, 255 ), 1 );


		pt0 = Point2f( m_pTraj[i - 1].y + m_pTraj[i - 1].height / 2, i - 1 );
		pt1 = Point2f( m_pTraj[i].y + m_pTraj[i].height / 2, i );
		line( img, pt0, pt1, m_scClr, 1 );

		pt0 = Point2f( m_centtrj[i - 1].y, i - 1 );
		pt1 = Point2f( m_centtrj[i].y, i );
		line( img, pt0, pt1, cv::Scalar( 255, 0, 0 ), 1 );
	}
#endif
}

double CTarget::CheckMax( std::vector<cv::Point> &pts )
{
	Vec4f line;
	fitLine( pts, line, cv::DIST_L2, 0, 1e-2, 1e-2 );

	int nFS = pts.size();
	
	double dMax = 0;

	double x0 = line[2];
	double y0 = line[3];

	if ( fabs( line[0] ) < 0.0001 )
	{
		for ( int j = 0; j < nFS; j++ )
			dMax = MAX( fabs( pts[j].x - x0 ), dMax );
	}
	else
	{
		double k = line[1] / line[0];
		for ( int j = 0; j < nFS; j++ )
			dMax = MAX( fabs( pts[j].y - ( k * ( pts[j].x - x0 ) + y0 ) ), dMax );
	}
	
	return dMax;
}

void CTarget::RefreshList(BYTE* pDel)
{
	for ( int i = 0; i < g_Targets.size(); i++ )
	{
		if ( !pDel[i] ) continue;
		swap( g_Targets[i], g_Targets[g_Targets.size()-1] );
		g_Targets.erase( g_Targets.begin() + g_Targets.size() - 1 );
	}
}

void CTarget::CheckList()
{
	int nSize = g_Targets.size();
	if ( !nSize ) return;

	// double check all objects, remove finished, lost, interlaced......
	BYTE* pDel = new BYTE[nSize];
	memset( pDel, 0, g_Targets.size() );

	/*/ remove abrupt movement
	for ( int i = 0; i < nSize; i++ )
	{
		std::vector<cv::Point> pointsX,pointsY,pointsXY;
		int nFS = g_Targets[i]->m_pTraj.size();
		for ( int j = 0; j < nFS; j++ )
		{
			double x = g_Targets[i]->m_pTraj[j].x + g_Targets[i]->m_pTraj[j].width / 2;
			double y = g_Targets[i]->m_pTraj[j].y + g_Targets[i]->m_pTraj[j].height / 2;
			pointsX.push_back( Point( j * 10, x ) );
			pointsY.push_back( Point( j * 10, y ) );
			pointsXY.push_back( Point( x, y ) );
		}
		double dMax = MAX( CheckMax( pointsX ), CheckMax( pointsY ));
		if ( dMax > 10 )
			pDel[i] = 1;
	}

	RefreshList( pDel );
	*/nSize = g_Targets.size();
	if ( !nSize )
	{
		delete pDel;
		return;
	}
	memset( pDel, 0, nSize );

	if ( g_Targets[0]->m_pFrmID.size() < 2 )
	{
		delete pDel;
		return;
	}
	int nStep = g_Targets[0]->m_pFrmID[1] - g_Targets[0]->m_pFrmID[0];

	// remove interlaced objects
	for ( int i = 0; i < (int)(nSize) - 1; i++ )
	{
		int nFS = g_Targets[i]->m_pFrmID.size();
		int nF1 = g_Targets[i]->m_pFrmID[0];
		int nF2 = g_Targets[i]->m_pFrmID[nFS - 1];

		for ( int j = i + 1; j < nSize; j++ )
		{
			int nGS = g_Targets[j]->m_pFrmID.size();
			int nG1 = g_Targets[j]->m_pFrmID[0];
			int nG2 = g_Targets[j]->m_pFrmID[nGS - 1];

			if ( nF1 >= nG2 || nF2 <= nG1 ) continue; // no interlace

			for ( int k = max( nF1, nG1 ); k < min( nF2, nG2 ); k += nStep )
			{
				Rect2d rctF = g_Targets[i]->m_pTraj[(k - nF1) / nStep];
				Rect2d rctG = g_Targets[j]->m_pTraj[(k - nG1) / nStep];
				Rect2d rct  = rctF & rctG;

				if ( rct.area() < 1 )
					continue;

				if ( rct.area() > min( rctF.area(), rctG.area() ) * 0.3 )
					pDel[i] = nF1 < nG1 ? 1 : 0; //				
	}	}	}

	RefreshList( pDel );

	delete pDel;
}
void CTarget::Init( Mat& imgC, Mat& imgB, Rect2d rct, int nDir, int nFrameID, int nType )
{
	m_lastImg = imgC.clone();
	m_rctBox = rct;
	m_TempB = imgB( m_rctBox ).clone();
	m_TempBA = m_TempB.clone();

	GetCenter2( m_rctBox, imgB );
	
	m_nFrameID = nFrameID;	

	m_TempB = imgB( m_rctBox ).clone();
	m_TempC = imgC( m_rctBox ).clone();
	m_TempBA = m_TempB.clone();

	cvtColor( m_TempC, m_TempG, CV_BGR2GRAY );	

	Mat result;
	result.create( imgC.size(), CV_32FC1 );

	m_rcInitSize = rct;
	m_nInitPos = rct.x + rct.width / 2;
	
	m_pFrmID.clear();
	m_pFrmID.push_back(nFrameID);

	m_pTraj.clear();
	m_pTraj.push_back( rct );

	m_centtrj.clear();
	m_centtrj.push_back( Point2f(( rct.x+ rct.width/2), ( rct.y+ rct.height/2 )) );
	
	
	m_nDirct = nDir; // 0: unknown, 1: forward   2: backward

	m_bType = nType; // 0: not sure; 1:car  2:pedestrian
	m_ptSpeed = Point2f( -900, -900 );


	m_pObj.clear();
	m_pObjMsk.clear();

	m_pObj.push_back(  m_TempC.clone() );
	m_pObjMsk.push_back( m_TempB.clone() );	

	m_dRatio = countNonZero( imgB( m_rctBox ) ) / (double) ( m_rctBox.area() );;

	m_pRatio.clear();
	m_pRatio.push_back( m_dRatio );

	m_pRatio1.clear();
	m_pRatio1.push_back( m_dRatio );

	m_nTrackTime = 0;
	m_nStopTime = 0;	
}

void GetPrjCenter( Mat& mprj, int& nX, int& nX0, int& nX1, int &nX2 )
{
	nX = nX0 = nX1 = nX2 = 0;

	BYTE* pPrj = mprj.data;
	int nCnt = mprj.cols;

	int nTotal = 0;
	for ( int i = 0; i < nCnt; i++ )
		nTotal += pPrj[i];

	int nMean = nTotal / nCnt / 2;
	for ( int i = 0; i < nCnt; i++ )
		pPrj[i] = MIN( pPrj[i], nMean );

	nTotal = 0;
	for ( int i = 0; i < nCnt; i++ )
	{
		nX += i * pPrj[i];
		nTotal += pPrj[i];
	}
	if ( nTotal )
		nX /= nTotal;

	nX0 = nX;

	int nMin = MAX( 5, nMean / 2 );
	int nBlankL = 0;
	int nBlankR = 0;
	int nWnd = MIN( 10, nCnt / 5 );
	for ( nBlankL = 0; nBlankL < nWnd && pPrj[nBlankL] < nMin; nBlankL++ );
	for ( nBlankR = 0; nBlankR < nWnd && pPrj[nCnt - 1 - nBlankR] < nMin; nBlankR++ );
	nBlankR = nCnt - 1 - nBlankR;

	nX1 = nBlankL;
	nX2 = nBlankR;
	if ( nBlankR >= nBlankL )
		nX = ( nX + ( nBlankR + nBlankL ) / 2.0 ) / 2.0;
}
void CTarget::FilterPrj( Mat& mprj )
{
	BYTE* pPrj = mprj.data;
	int nCnt = mprj.cols;

	int nTotal = 0;
	for ( int i = 0; i < nCnt; i++ )
		nTotal += pPrj[i];

	int nMean = nTotal / nCnt / 2;
	for ( int i = 0; i < nCnt; i++ )
		pPrj[i] = MIN( pPrj[i], nMean );

	int nMin = MAX( 3, nMean / 10 );

	for ( int i = 0; i < nCnt; i++ )
		pPrj[i] = MAX( pPrj[i]-nMin, 0);
}

void CTarget::FindMaxLen( Mat& mprj, int nLenSet, int& nX )
{
	int nX0 = nX;
	BYTE* pPrj = mprj.data;
	int nCnt = mprj.cols;

	if ( nLenSet >= nCnt ) return;
		
	int nSum = 0;
	for ( int i = 0; i < nLenSet; i++ )
		nSum += pPrj[i];

	int nMax = nSum;
	int nX1 = nX;
	nX = 0;
	for ( int i = nLenSet; i < nCnt; i++ )
	{
		nSum -= pPrj[i - nLenSet];
		nSum += pPrj[i];
		if ( nSum < nMax )
			continue;
		else if ( nSum == nMax )
		{
			nX1 = i - nLenSet;
			continue;
		}
		nMax = nSum;
		nX = i - nLenSet;
	}
	if ( !nMax )
		nX = nX0;
	if( nX1 > nX )
		nX = ( nX + nX1 ) / 2;
}
/*void CTarget::RefillBox( Mat &img, Rect2d& rectBox, int nX, int nY, Mat& obj )
{

}*/
void CTarget::GetCenter2( Rect2d& rctBox2, Mat& imgB )
{
	if ( m_rctBox.area() < 1 ) return;
	if ( m_rctBox.x + m_rctBox.width > m_lastImg.cols - 1 )
		return;

	Rect2d rctBoxL( m_rctBox.x-5, m_rctBox.y-5, m_rctBox.width+10, m_rctBox.height+10);
	rctBoxL = rctBoxL & ( Rect2d( 0, 0, m_lastImg.cols - 1, m_lastImg.rows - 1 ) );

	int nXoff = rctBoxL.x - m_rctBox.x;
	int nYoff = rctBoxL.y - m_rctBox.y;
	
	Mat maskc = imgB( rctBoxL ).clone();

	Mat mRow( 1, maskc.cols, CV_8UC1, Scalar( 0 ) );
	Mat mCol( maskc.rows, 1, CV_8UC1, Scalar( 0 ) );

	reduce( maskc, mRow, 0, REDUCE_AVG );
	reduce( maskc, mCol, 1, REDUCE_AVG );

	mCol = mCol.reshape( 0, 1 );

	int nX=0, nY=0;
	

	/*{
		BYTE* pPrj = (BYTE*) mRow.data;
		Mat hisu( 64, mRow.cols, CV_8UC1, Scalar( 200 ) );
		for ( int i = 0; i < mRow.cols; i++ )
		for ( int j = 0; j < pPrj[i] / 4; j++ )
			hisu.data[( 63 - j )*mRow.cols + i] = 64;

		g_pView->SetInterResultImage( 2, hisu, "mRow  x" );
	}
	{
		BYTE* pPrj = (BYTE*) mCol.data;
		Mat hisu( 64, mCol.cols, CV_8UC1, Scalar( 200 ) );
		for ( int i = 0; i < mCol.cols; i++ )
		for ( int j = 0; j < pPrj[i] / 4; j++ )
			hisu.data[( 63 - j )*mCol.cols + i] = 64;
		g_pView->SetInterResultImage( 2, hisu, "mCol  y" );
	}*/

	FilterPrj( mRow );
	FilterPrj( mCol );

	nX = nY = 5;
	FindMaxLen( mRow, m_rctBox.width, nX );
	FindMaxLen( mCol, m_rctBox.height, nY );

	/*{
		BYTE* pPrj = (BYTE*) mRow.data;
		Mat hisu( 64, mRow.cols, CV_8U, Scalar( 200 ) );
		for ( int i = 0; i < mRow.cols; i++ )
		for ( int j = 0; j < pPrj[i] / 4; j++ )
		{
			hisu.data[( 63 - j )*mRow.cols + i] = ( i >= nX && i<= nX+m_rctBox.width) ? 255 : 64;
		}
		g_pView->SetInterResultImage( 2, hisu, "mRow  x" );
	}
	{
		//normalize( mCol, mCol, 0.0, 63, NORM_MINMAX );
		BYTE* pPrj = (BYTE*) mCol.data;
		Mat hisu( 64, mCol.cols, CV_8U, Scalar( 200 ) );
		for ( int i = 0; i < mCol.cols; i++ )
		for ( int j = 0; j < pPrj[i] / 4; j++ )
			hisu.data[( 63 - j )*mCol.cols + i] = ( i >= nY && i <= nY + m_rctBox.height ) ? 128 : 64;
		g_pView->SetInterResultImage( 2, hisu, "mCol  y" );
	}*/

	nX += nXoff;
	nY += nYoff;
	
	//nX -= m_rctBox.width / 2;
	//nY -= m_rctBox.height / 2;
	nX = abs( nX ) > 0 ? ( nX > 0 ? MIN( 2, nX ) : MAX( -2, nX ) ) : 0;
	nY = abs( nY ) > 0 ? ( nY > 0 ? MIN( 2, nY ) : MAX( -2, nY ) ) : 0;

	if ( nX > 0 && m_rctBox.x + nX > imgB.cols - 1 - m_rctBox.width || nX < 0 && m_rctBox.x <= 0 )
		nX = 0;
	if ( nY > 0 && m_rctBox.y + nY > imgB.rows - 1 - m_rctBox.height || nY < 0 && m_rctBox.y <= 0 )
		nY = 0;

	m_rctBox.x += nX;
	m_rctBox.y += nY;

	
	int nW = m_TempBA.cols;
	int nH = m_TempBA.rows;

	int nDX = abs( nX );
	int nDY = abs( nY );

	Rect rctDes( 0, 0, nW - nDX, nH - nDY );
	Rect rctSrc( 0, 0, nW - nDX, nH - nDY );
	if ( nX > 0 )
		rctSrc.x += nX;
	else if ( !nX )
		rctDes.width = rctSrc.width = nW;
	else rctDes.x -= nX;

	if ( nY > 0 )
		rctSrc.y += nY;
	else if ( !nY )
		rctDes.height = rctSrc.height = nH;
	else rctDes.y -= nY;

	//g_pView->SetInterResultImage( 1, m_TempBA, "recenter2  m_TempBA" );

	Mat mSubImg = m_TempBA( rctSrc ).clone();
	//g_pView->SetInterResultImage( 1, mSubImg, "recenter2  mSubImg" );

	m_TempBA = imgB( m_rctBox ).clone();
	mSubImg.copyTo( m_TempBA( rctDes ) );

	/*if ( nX || nY )
	{
		CString strTemp;
		strTemp.Format( "  %ld, %ld ", nX, nY );
		g_pView->SetInterResultImage( 1, m_TempBA, "recenter2 ed  m_TempBA " + strTemp );
	}*/
}

void CTarget::Recenter2( Rect2d& rctBox, Mat& imgB  )
{
	if ( rctBox.area() < 1 ) return;
	if ( rctBox.x + rctBox.width > imgB.cols - 1 )
		return;

	Mat maskc( m_TempBA.size(), CV_8UC1, 30 );
	compare( m_TempBA, maskc, maskc, CMP_GT );

	int nW = maskc.cols;
	int nH = maskc.rows;

	Mat mRow( 1, nW, CV_8UC1, Scalar( 0 ) );
	Mat mCol( nH, 1, CV_8UC1, Scalar( 0 ) );

	reduce( maskc, mRow, 0, REDUCE_AVG );
	reduce( maskc, mCol, 1, REDUCE_AVG );

	mCol = mCol.reshape( 0, 1 );
		
	int nX, nX0, nX1, nX2;
	GetPrjCenter( mRow, nX, nX0, nX1, nX2 );
	int nY, nY0, nY1, nY2;
	GetPrjCenter( mCol, nY, nY0, nY1, nY2 );
	

	/*{
		BYTE* pPrj = (BYTE*) mRow.data;

		Mat hisu( 64, mRow.cols, CV_8U, Scalar( 200 ) );
		for ( int i = 0; i < mRow.cols; i++ )
		{			
			for ( int j = 0; j < pPrj[i] / 4; j++ )
			{
				hisu.data[( 63 - j )*mRow.cols + i] = ( i == nX ) ? 255 : 64;
			}
			for ( int j = pPrj[i] / 4; j < 63; j++ )
			{
				if ( i == nX0 || i == nX1 || i == nX2 )
					hisu.data[( 63 - j )*mRow.cols + i] = 128;
			}
		}
		g_pView->SetInterResultImage( 2, hisu, "mRow  x" );
	}
	{
		//normalize( mCol, mCol, 0.0, 63, NORM_MINMAX );
		BYTE* pPrj = (BYTE*) mCol.data;

		Mat hisu( 64, mCol.cols, CV_8U, Scalar( 200 ) );
		for ( int i = 0; i < mCol.cols; i++ )
			for ( int j = 0; j < pPrj[i] / 4; j++ )
				hisu.data[( 63 - j )*mCol.cols + i] = ( i == nY  ) ? 128 : 64;
		g_pView->SetInterResultImage( 2, hisu, "mCol  y" );
	}*/


	nX -= rctBox.width / 2;
	nY -= rctBox.height / 2;
	nX = abs( nX ) > 0 ? ( nX > 0 ? MIN( 2, nX ) : MAX( -2, nX ) ) : 0;
	nY = abs( nY ) > 0 ? ( nY > 0 ? MIN( 2, nY ) : MAX( -2, nY ) ) : 0;

	if ( nX > 0 && rctBox.x + nX > imgB.cols - 1 - rctBox.width || nX < 0 && rctBox.x <= 0 )
		nX = 0;
	if ( nY > 0 && rctBox.y + nY > imgB.rows - 1 - rctBox.height || nY < 0 && rctBox.y <= 0 )
		nY = 0;

	rctBox.x += nX;
	rctBox.y += nY;

	int nDX = abs( nX );
	int nDY = abs( nY );

	Rect rctDes( 0, 0, nW - nDX, nH - nDY );
	Rect rctSrc( 0, 0, nW - nDX, nH - nDY );
	if ( nX > 0 )
		rctSrc.x += nX;
	else if ( !nX )
		rctDes.width = rctSrc.width = nW;
	else rctDes.x -= nX;

	if ( nY > 0 )
		rctSrc.y += nY;
	else if ( !nY )
		rctDes.height = rctSrc.height = nH;
	else rctDes.y -= nY;

	//g_pView->SetInterResultImage( 1, m_TempBA, "recenter2  m_TempBA" );

	Mat mSubImg = m_TempBA( rctSrc ).clone();
	//g_pView->SetInterResultImage( 1, mSubImg, "recenter2  mSubImg" );

	m_TempBA = imgB( rctBox ).clone();
	mSubImg.copyTo( m_TempBA( rctDes ) );

	/*if ( nX || nY )
	{
		CString strTemp;
		strTemp.Format( "  %ld, %ld ", nX, nY );
		g_pView->SetInterResultImage( 1, m_TempBA, "recenter2 ed  m_TempBA " + strTemp );
	}*/
}
void CTarget::ResizeTempAB( Rect2d rctOld, Rect2d rctNew, Mat& imgB )
{
	if ( m_TempBA.cols == rctNew.width && m_TempBA.rows == rctNew.height )
		return;
	if ( rctOld.width == rctNew.width && rctOld.height == rctNew.height ) // same to the previous
		return;

//	if ( rect.width > img.cols || rect.height > img.rows )
//		return;
//	Rect rct( ( img.cols - rect.width ) / 2, ( img.rows - rect.height ) / 2, rect.width, rect.height );
//	img = img( rct ).clone();
	
	Rect2d rctS = rctNew & rctOld;
	rctS.x -= rctOld.x;
	rctS.y -= rctOld.y;

	Mat mSubImg = m_TempBA( rctS ).clone();

	Rect2d rctD = rctNew & rctOld;
	rctD.x -= rctNew.x;
	rctD.y -= rctNew.y;

	m_TempBA = imgB( rctNew ).clone();
	mSubImg.copyTo( m_TempBA( rctD ) );
}


void CTarget::Recenter(Rect2d& rctBox, Mat& imgB )
{
	if ( rctBox.width != m_TempBA.cols || rctBox.height != m_TempBA.rows )
		return;

	Mat maskc( m_TempBA.size(), CV_8UC1, 30 );
	compare( m_TempBA, maskc, maskc, CMP_GT );

	int nH = maskc.rows;
	int nW = maskc.cols;
	BYTE* pPrjH = new BYTE[nH];
	BYTE* pPrjV = new BYTE[nW];
	memset( pPrjH, 0, nH );
	memset( pPrjV, 0, nW );

	for ( int i = 0; i < nH; i++ )
	for ( int j = 0; j < nW; j++ )
	{
		pPrjH[i] += maskc.data[i*nW + j];
		pPrjV[j] += maskc.data[i*nW + j];
	}
	// filter
	for ( int i = 1; i < nH-1; i++ )
		if ( pPrjH[i] && !pPrjH[i - 1] && !pPrjH[i + 1] ) pPrjH[i] = 0;
	if ( pPrjH[0] && !pPrjH[1] && !pPrjH[2] ) pPrjH[0] = 0;

	for ( int j = 0; j < nW-1; j++ )
		if ( pPrjV[j] && !pPrjV[j - 1] && !pPrjV[j + 1] ) pPrjV[j] = 0;
	if ( pPrjV[0] && !pPrjV[1] && !pPrjV[2] ) pPrjV[0] = 0;

	int nLeft = 0;
	int nRight = nW;
	for ( int j = 0; j < nW; j++ )
	{
		nLeft = j;
		if ( pPrjV[nLeft] > 3 ) break;
	}
	for ( int j = 0; j < nW; j++ )
	{
		nRight = nW - 1 - j;
		if ( pPrjV[nRight] > 3 ) break;
	}
	int nTop = 0;
	int nBottom = nH;
	for ( int i = 0; i < nH; i++ )
	{
		nTop = i;
		if ( pPrjH[nTop] > 3 ) break;
	}
	for ( int i = 0; i < nH; i++ )
	{
		nBottom = nH - 1 - i;
		if ( pPrjH[nBottom] > 3 ) break;
	}
	delete pPrjH;
	delete pPrjV;

	g_pView->SetInterResultImage( 1, maskc, "recenter _  maskc  0" );

	// no need recenter
	//if ( nBottom > nH - 3 && nRight > nW - 3 && nLeft < 3 && nTop < 3 )
	//	return;

	double dX = ( nLeft + nRight - nW + 1 ) / 2.0;
	double dY = ( nTop + nBottom - nH + 1 ) / 2.0;

	int nX = fabs( dX ) > 0 ? (dX > 0 ? MIN( 2, dX ) : MAX( -2, dX ) ) : 0;
	int nY = fabs( dY ) > 0 ? (dY > 0 ? MIN( 2, dY ) : MAX( -2, dY ) ) : 0;
	if ( nX > 0 && rctBox.x + nX > imgB.cols - 1 - rctBox.width || nX < 0 && rctBox.x <= 0 )
		nX = 0;
	if ( nY > 0 && rctBox.y + nY > imgB.rows - 1 - rctBox.height || nY < 0 && rctBox.y <= 0 )
		nY = 0;

	rctBox.x += nX;
	rctBox.y += nY;
	
	int nDX = abs( nX );
	int nDY = abs( nY );

	Rect rctDes( 0, 0, nW - nDX, nH - nDY );
	Rect rctSrc( 0, 0, nW - nDX, nH - nDY );
	if ( nX > 0 )
		rctSrc.x += nX;
	else if ( !nX )
		rctDes.width = rctSrc.width = nW;
	else rctDes.x -= nX;

	if ( nY > 0 )
		rctSrc.y += nY;
	else if ( !nY )
		rctDes.height = rctSrc.height = nH;
	else rctDes.y -= nY;

	g_pView->SetInterResultImage( 1, m_TempBA, "recenter  m_TempBA" );

	Mat mSubImg = m_TempBA( rctSrc ).clone();
	g_pView->SetInterResultImage( 1, mSubImg, "recenter  mSubImg" );

	m_TempBA = imgB( rctBox ).clone();
	mSubImg.copyTo( m_TempBA( rctDes ) );
	
	if ( nX || nY )
	{
		CString strTemp;
		strTemp.Format( "  %ld, %ld ", nX, nY );
		g_pView->SetInterResultImage( 1, m_TempBA, "recenter ed  m_TempBA " + strTemp );
	}
}

int CTarget::AdjustRect()
{
	if ( m_rctBox.width <= m_lastImg.cols - 15 - m_rctBox.x )
		return 1;
	
	int nMargin = MAX( abs( m_ptSpeed.x ) + 2, 15);
	m_rctBox.width = m_lastImg.cols - nMargin - m_rctBox.x;
	if ( m_rctBox.width < 20 )
		return 0;

	Rect2d rctSz = m_rctBox;
	rctSz.x = rctSz.y = 0;
	m_TempBA = m_TempBA( rctSz ).clone();
	m_TempB = m_TempB( rctSz ).clone();
	m_TempC = m_TempC( rctSz ).clone();

	return 1;
}

int CTarget::CheckTraj()
{
	int nCnt = m_pTraj.size();
	if ( nCnt < 10 )
		return 1;
	
	double dMeanX = 0;
	double dMeanY = 0;
	double dDevX = 0;
	double dDevY = 0;
	for ( int i = 0; i < nCnt; i++ )
	{
		double x = m_pTraj[i].x + m_pTraj[i].width / 2;
		double y = m_pTraj[i].y + m_pTraj[i].height / 2;
		dMeanX += x;	
		dMeanY += y;
		dDevX += x*x;
		dDevY += y*y;
	}
	dMeanX /= nCnt;
	dMeanY /= nCnt;
	dDevX = ( dDevX / nCnt - dMeanX * dMeanX );
	dDevY = ( dDevY / nCnt - dMeanY * dMeanY );

	if ( MAX( dDevX, dDevY ) < 4 )
		return 0;
	return 1;
}
void CTarget::Backup( CTarget& target )
{
	target.m_TempBA  = m_TempBA.clone();
	target.m_TempB   = m_TempB.clone();
	target.m_TempG   = m_TempG.clone();
	target.m_TempC   = m_TempC.clone();
	target.m_lastImg = m_lastImg.clone();

	target.m_nFrameID   = m_nFrameID;
	target.m_nTrackTime = m_nTrackTime;
	target.m_nStopTime  = m_nStopTime;

	target.m_rctBox = m_rctBox;
	target.m_scClr  = m_scClr;

	target.m_rcInitSize = m_rcInitSize;	// initial size
	target.m_nInitPos   = m_nInitPos;   // initial pos
	target.m_dSizeR     = m_dSizeR;     // changing ratio

	target.m_dRatio = m_dRatio;

	target.m_nDirct  = m_nDirct;      // 0: unknown, 1: forward   2: backward
	target.m_bType   = m_bType;       // 0: not sure; 1:car  2:pedestrian
	target.m_ptSpeed = m_ptSpeed;     // ????

	// for drawing 
	//std::vector<int> m_pFrmID;
	//std::vector<Rect2d> m_pTraj;

	//std::vector<Point2f> prevPts;
	//std::vector<Point2f> currPts;

	//std::vector<std::vector<Point2f>> pPts;
}
void CTarget::Recover( CTarget& target )
{
	m_TempBA  = target.m_TempBA.clone();
	m_TempB   = target.m_TempB.clone();
	m_TempG	  = target.m_TempG.clone();
	m_TempC	  = target.m_TempC.clone();
	m_lastImg = target.m_lastImg.clone();

	m_nFrameID   = target.m_nFrameID;
	m_nTrackTime = target.m_nTrackTime;
	m_nStopTime  = target.m_nStopTime;

	m_rctBox = target.m_rctBox;
	m_scClr  = target.m_scClr;

	m_rcInitSize = target.m_rcInitSize;	// initial size
	m_nInitPos   = target.m_nInitPos;   // initial pos
	m_dSizeR     = target.m_dSizeR;     // changing ratio

	m_dRatio  = target.m_dRatio;

	m_nDirct  = target.m_nDirct;      // 0: unknown, 1: forward   2: backward
	m_bType   = target.m_bType;       // 0: not sure; 1:car  2:pedestrian
	m_ptSpeed = target.m_ptSpeed;     // ????

	// for drawing 
	//std::vector<int> m_pFrmID;
	//std::vector<Rect2d> m_pTraj;

	//std::vector<Point2f> prevPts;
	//std::vector<Point2f> currPts;

	//std::vector<std::vector<Point2f>> pPts;
}

int CTarget::Track( Mat& imgC, Mat& imgB0, double& dX, double& dY, int nBackOder )
{
	++m_nTrackTime;

	Mat imgB = imgB0.clone();

	if ( m_bType == 1 ) // for car
	{
		if ( m_nDirct == 2 )
			imgB = imgB & *m_pDnMsk;
		else if ( m_nDirct == 1 )
			imgB = imgB & *m_pUpMsk;
	}
	g_pView->SetInterResultImage( 1, imgB, "imgB" );

	dX = dY = 0;

	//prevPts.clear();
	//currPts.clear();

	Rect2d rctBoxBack = m_rctBox;
	double dXOff = 0;
	double dYOff = 0;

	if ( !AdjustRect() )
		return 0;

	dXOff -= m_rctBox.x + m_rctBox.width / 2 - rctBoxBack.x - rctBoxBack.width / 2;
	dYOff -= m_rctBox.y + m_rctBox.height/ 2 - rctBoxBack.y - rctBoxBack.height / 2;

	double x, y, dRatio;
	m_nStopTime = 0;
	m_nTrackTime = 30;
	if ( !m_nStopTime && BlockMatch( imgC, x, y, dRatio ) )
	{
		m_dRatio = m_dRatio*0.9 + dRatio * 0.1;
	}
	/*else if ( m_nTrackTime <= 3 ) // object missing
	;// return 0;
	else // object stop moving
	FeatureMatch( imgC, x, y );	*/

	

	m_rctBox.x += x;
	m_rctBox.y += y;

	dX = x;
	dY = y;

	dRatio = countNonZero( imgB( m_rctBox ).clone() ) / (double) ( m_rctBox.area() );

	//UpdateTemplate( img1 );

	Rect2d rctOld = m_rctBox;
	//if ( dRatio > 0.15 )
	{
		ResizeRect( m_rctBox );

		ResizeTempAB( rctOld, m_rctBox, imgB );

		if ( dRatio > 0.5 || ( dRatio > 0.18 && (( fabs(x)>=1 || fabs(y)>=1 ) || norm(m_ptSpeed) > 1) ) )
		//Recenter2( m_rctBox, imgB );
			GetCenter2( m_rctBox, imgB );
		else if( m_rctBox.width < 80 && dRatio > 0.18 )
			GetCenter2( m_rctBox, imgB );
	
		m_TempB = imgB( m_rctBox ).clone();
		m_TempC = imgC( m_rctBox ).clone();
	}

	dXOff -= m_rctBox.x + m_rctBox.width / 2 - rctOld.x - rctOld.width / 2;
	dYOff -= m_rctBox.y + m_rctBox.height / 2 - rctOld.y - rctOld.height / 2;	


	if (  dRatio > 0.15  ) // stop update
		m_TempBA = m_TempBA*0.8 + m_TempB*0.2;

	g_pView->SetInterResultImage( 1, m_TempBA, "m_TempBA" );
	g_pView->SetInterResultImage( 1, m_TempB, "m_TempB " );

	CString strTemp;
	strTemp.Format( "%.2f, %.2f", m_ptSpeed.x, m_ptSpeed.y );
	g_pView->SetInterResultImage( 1, m_TempC, "m_TempC  " + strTemp );

	
	if ( m_ptSpeed.x < -800 )
		m_ptSpeed = Point2f( x, y );

	m_ptSpeed.x = ( m_ptSpeed.x + x ) * 0.5;
	m_ptSpeed.y = ( m_ptSpeed.y + y ) * 0.5;
		

	// type and frame-play-order judging
	if ( m_nTrackTime >= 3 )
	{
		if ( fabs( m_ptSpeed.x ) < 1 && fabs( m_ptSpeed.y ) < 1 )
			m_nStopTime++;
		else
			m_nStopTime = 0;

		if ( !m_bType )
		{
			if ( fabs( m_ptSpeed.y ) < 3 && fabs( m_ptSpeed.x ) > 2 )
				m_bType = 1;  // 0 : not sure; 1 : car,  2 : pedestrian
			else
				m_bType = 2;
		}

		if ( !m_nDirct )
		{
			if ( m_ptSpeed.y < 0 ) // 0 : unknown, 1 : forward,  2 : backward
				m_nDirct = 1;
			else
				m_nDirct = 2;
		}
	}

	//m_lastImg = imgC.clone();
	if ( !CheckTraj() )
		return 0;


	if ( m_bType == 2 || ( m_bType == 1 && JudgeCar2Ped() ) ) // ped
	{
		m_bType = 2;

		if ( IsPedOut( nBackOder ) )
			return -1;
	}
	
	// finished to the left most
	if ( m_nDirct == 1 && m_rctBox.x < 180 )
		return 0;
	else if( m_rctBox.x < 110 )
		return 0;

	if ( m_ptSpeed.x * 3 + m_rctBox.x  > m_lastImg.cols - 50 ) // too small to match
		return 0;

	return 1;
}

bool CTarget::IsPedOut(int nBackOrder)
{
	if ( m_bType != 2 )
		return true;

	int x = m_rctBox.x + m_rctBox.width / 2;
	if ( x < 0 || x >= m_spPedRange.size() )
		return true;

	Point2i pt = m_spPedRange[x];
	int y = m_rctBox.y + m_rctBox.height / 2;
	if ( nBackOrder )
	{
		if ( y < pt.x || y > pt.y )
			return true;
	}
	else
	{
		if ( y < pt.x + m_rctBox.height / 2 || y > pt.y - m_rctBox.height / 2 )
			return true;
	}

	return false;
}
void CTarget::GetLenCnt(double dLen, int &nCnt )
{
	nCnt = 0;
	for ( nCnt = m_pTraj.size() - 1; nCnt > 0  && dLen > 0; nCnt-- )
		dLen -= norm( Point( m_pTraj[nCnt].y + m_pTraj[nCnt].height / 2 - m_pTraj[nCnt-1].y - m_pTraj[nCnt-1].height / 2,
					         m_pTraj[nCnt].x + m_pTraj[nCnt].width  / 2 - m_pTraj[nCnt-1].x - m_pTraj[nCnt-1].width  / 2 ));
}

bool CTarget::IsLinear(int nCnt )
{
	if ( nCnt > m_pTraj.size() )
		nCnt = m_pTraj.size();
	if ( nCnt < 0 )
		return false;

	std::vector<cv::Point> pts;
	for ( int j = m_pTraj.size()-1; j >= nCnt; j-- )
		pts.push_back( Point( m_pTraj[j].y + m_pTraj[j].height / 2, m_pTraj[j].x + m_pTraj[j].width / 2 ) );

	cv::Vec4f line_para;
	cv::fitLine( pts, line_para, cv::DIST_L2, 0, 1e-2, 1e-2 );

	double x0 = line_para[2];
	double y0 = line_para[3];

	double k = 1; // line_para[1] / line_para[0];
	if ( fabs( line_para[0] ) > 0.001 )
	{
		k = line_para[1] / line_para[0];

		double dError = 0;
		double dMax = 0;
		for ( int j = 0; j < pts.size(); j++ )
		{
			double y = k * ( pts[j].x - x0 ) + y0;
			dError += fabs( y - pts[j].y );
			dMax = MAX( dMax, fabs( y - pts[j].y ) );
		}
		dError /= pts.size();

		double dLen = norm( pts[0] - pts[pts.size() - 1] );
		if ( dError < 5 && ( dMax < 8 || dMax < dLen * 0.15 ) && pts.size() > 10 )
			return true;
	}

	return false;
}
int CTarget::GetMaxCnt(int nCnt)
{
	if ( nCnt > m_pTraj.size() )
		nCnt = m_pTraj.size();
	if ( nCnt < 0 )
		return 0;

	std::vector<cv::Point> pts;
	for ( int j = m_pTraj.size() - 1; j >= nCnt; j-- )
		pts.push_back( Point( m_pTraj[j].y + m_pTraj[j].height / 2, m_pTraj[j].x + m_pTraj[j].width / 2 ) );

	cv::Vec4f line_para;
	cv::fitLine( pts, line_para, cv::DIST_L2, 0, 1e-2, 1e-2 );

	double x0 = line_para[2];
	double y0 = line_para[3];

	double k = 1; // line_para[1] / line_para[0];
	if ( fabs( line_para[0] ) > 0.001 )
	{
		k = line_para[1] / line_para[0];

		double dLen = norm( pts[0] - pts[pts.size() - 1] );

		double dMax = 0;
		for ( int j = m_pTraj.size()-1; j >=0; j-- )
		{
			double y = k * ( m_pTraj[j].y + m_pTraj[j].height / 2 - x0 ) + y0;
			dMax = MAX( dMax, fabs( y - ( m_pTraj[j].x + m_pTraj[j].width / 2 ) ) );
			if ( dMax > 8 || dMax > dLen*0.15 )
				return j;
		}
	}
	return 0;
}
bool CTarget::IsPedIn(  )
{
	if ( m_bType != 2 )
		return false;

	int x = m_pTraj[0].x + m_pTraj[0].width / 2;
	if ( x < 0 || x >= m_spPedRange.size() )
		return true;

	Point2i pt = m_spPedRange[x];
	int y = m_pTraj[0].y + m_pTraj[0].height / 2;
	if ( y < pt.x+5 || y > pt.y-5 )
		return true;

	// try linear fitting
	if ( IsLinear( 0 ) )
		return true;

	int nCnt = 0;
	GetLenCnt( 60, nCnt );
	if ( IsLinear( nCnt ) )
	{
		nCnt = GetMaxCnt( nCnt );
		if( nCnt )
		{
			m_pTraj.erase( m_pTraj.begin(), m_pTraj.begin()+nCnt );
			m_pFrmID.erase( m_pFrmID.begin(), m_pFrmID.begin()+nCnt );
			m_pRatio.erase( m_pRatio.begin(), m_pRatio.begin()+nCnt );
			m_pRatio1.erase( m_pRatio1.begin(), m_pRatio1.begin()+nCnt );
			m_centtrj.erase( m_centtrj.begin(), m_centtrj.begin()+nCnt );
			m_pObj.erase( m_pObj.begin(), m_pObj.begin()+nCnt );
			m_pObjMsk.erase( m_pObjMsk.begin(), m_pObjMsk.begin()+nCnt );
		}		
		return true;
	}

	return false;
}

bool CTarget::IsPed()
{
	int nS = m_pTraj.size();

	if ( nS < 2 )
		return true;

	Point pt1 = Point( m_pTraj[0].x + m_pTraj[0].width / 2, m_pTraj[0].y + m_pTraj[0].height / 2 );
	Point pt2 = Point( m_pTraj[nS - 1].x + m_pTraj[nS - 1].width / 2, m_pTraj[nS - 1].y + m_pTraj[nS - 1].height / 2 );

	int nL, nR, nB, nT;
	nL = nR = pt1.x;
	nB = nT = pt1.y;
	for ( int i = 0; i < 5; i++ )
	{
		nL = MIN( nL, m_pTraj[i].x + m_pTraj[i].width / 2 );
		nR = MAX( nR, m_pTraj[i].x + m_pTraj[i].width / 2 );
		nT = MIN( nT, m_pTraj[i].y + m_pTraj[i].height / 2 );
		nB = MAX( nB, m_pTraj[i].y + m_pTraj[i].height / 2 );
	}

	// for capture state, enough speed
	if ( nB - nT < 6 )
		return false;
	
	for ( int i = 5; i < m_pTraj.size(); i++ )
	{
		nL = MIN( nL, m_pTraj[i].x + m_pTraj[i].width / 2 );
		nR = MAX( nR, m_pTraj[i].x + m_pTraj[i].width / 2 );
		nT = MIN( nT, m_pTraj[i].y + m_pTraj[i].height / 2 );
		nB = MAX( nB, m_pTraj[i].y + m_pTraj[i].height / 2 );
	}

	if ( nR - nL > 150 )
		return false;

	if ( nR - nL > (nB - nT ) * 4)
		return false;

	return true;
}
bool CTarget::JudgeCar2Ped()
{
	int nS = m_pTraj.size();

	if ( nS < 2 )
		return false;
	
	Point pt1 = Point( m_pTraj[0].x + m_pTraj[0].width / 2, m_pTraj[0].y + m_pTraj[0].height / 2 );
	Point pt2 = Point( m_pTraj[nS-1].x + m_pTraj[nS-1].width / 2, m_pTraj[nS-1].y + m_pTraj[nS-1].height / 2 );

	int nX = abs( pt1.x - pt2.x );
	int nY = abs( pt1.y - pt2.y );

	if ( nX < 10 && nY < 10 )
		return false;
	
	if ( nY > 150 )
		return true;	

	if ( !nX && nY )
		return true;

	if ( nY > nX )
		return true;

	return false;
}
int CTarget::UpdateBackward( Mat& imgC, Mat& imgB, int nFrameID )
{
	double dx = 0;
	double dy = 0;
	int nRes = Track( imgC, imgB, dx, dy, 1 );

	m_lastImg = imgC.clone();

	m_pTraj.insert( m_pTraj.begin(), m_rctBox );
	m_pFrmID.insert( m_pFrmID.begin(), nFrameID );

	double d1 = countNonZero( m_TempB ) / (double) ( m_rctBox.area() );
	m_pRatio.insert( m_pRatio.begin(), m_dRatio );
	m_pRatio1.insert( m_pRatio1.begin(), d1 );

	m_pObj.insert( m_pObj.begin(), m_TempC.clone() );
	m_pObjMsk.insert( m_pObjMsk.begin(),m_TempB.clone() );	

	int x = 0;
	int y = 0;
	if( m_bType != 2 )
		BackMatch2( imgC, x, y );

	Point2f pt;
	pt.x = x + m_rctBox.x + m_rctBox.width /2;
	pt.y = y + m_rctBox.y + m_rctBox.height/2;

	m_centtrj.insert( m_centtrj.begin(), pt ); // center point

	return nRes;
}

CTarget* CTarget::LoadTarget( CString &strPath )
{
	CString strName = strPath.Mid(strPath.ReverseFind('\\')+1);

	if ( strName.GetLength() < 1 )
		return NULL;

	CTarget* pTarget = new CTarget;

	if ( strName.GetAt( 0 ) == 'P' )
		pTarget->m_bType = 2;
	else if ( strName.GetAt( 0 ) == 'C' )
		pTarget->m_bType = 1;
	else 
		pTarget->m_bType = 0;

	if ( strName.GetAt( 1 ) == 'B' )
		pTarget->m_nDirct = 2;
	else if ( strName.GetAt( 1 ) == 'A' )
		pTarget->m_nDirct = 1;
	else 
		pTarget->m_nDirct = 0;

	
	CStdioFile File( strPath, CFile::modeRead );
	CString strTemp;
	while( File.ReadString(strTemp) )
	{
		Rect2d rctObj;
		strTemp += ";;;;;;";
		CString strData = strTemp.Left( strTemp.Find( ";" ) );
		strTemp = strTemp.Mid( strTemp.Find( ";" ) + 1 );
		pTarget->m_pFrmID.push_back( atol( strData ) );

		strData = strTemp.Left( strTemp.Find( ";" ) );
		strTemp = strTemp.Mid( strTemp.Find( ";" ) + 1 );
		rctObj.x = atof( strData );

		strData = strTemp.Left( strTemp.Find( ";" ) );
		strTemp = strTemp.Mid( strTemp.Find( ";" ) + 1 );
		rctObj.y = atof( strData );

		strData = strTemp.Left( strTemp.Find( ";" ) );
		strTemp = strTemp.Mid( strTemp.Find( ";" ) + 1 );
		rctObj.width = atof( strData );

		strData = strTemp.Left( strTemp.Find( ";" ) );
		strTemp = strTemp.Mid( strTemp.Find( ";" ) + 1 );
		rctObj.height = atof( strData );

		pTarget->m_pTraj.push_back( rctObj );
	}
	File.Close();

	return pTarget;
}
void CTarget::DrawAll( Mat &img, std::vector<Ptr<CTarget>> &targets )
{
	cv::RNG rng( time( 0 ) );

	std::vector<Ptr<CTarget>>::iterator it = targets.begin();

	int nCount = 0;
	for ( ; it != targets.end(); it++ )
	{		
		Scalar scr = Scalar( rng.uniform( 0, 255 ), rng.uniform( 0, 255 ), rng.uniform( 0, 255 ) );
		Point2f pt0 = ( ( *it )->m_pTraj[0].tl() + ( *it )->m_pTraj[0].br() ) / 2;
		//if ( pt0.x > 150 ) continue;

		nCount++;
		for ( int i = 1; i < ( *it )->m_pTraj.size(); i++ )
		{
			Point2f pt0 = ( ( *it )->m_pTraj[i - 1].tl() + ( *it )->m_pTraj[i - 1].br() ) / 2;
			Point2f pt1 = ( ( *it )->m_pTraj[i    ].tl() + ( *it )->m_pTraj[i    ].br() ) / 2;
			line( img, pt0, pt1, scr, 1 );
		}
	}
}

void CTarget::LoadTarget( CString &strPath, std::vector<Ptr<CTarget>> &targets )
{
	std::vector<Ptr<CTarget>>::iterator it = targets.begin();
	for ( ; it != targets.end(); )
		it = targets.erase( it );
	targets.clear();

	CFileFind finder;

	BOOL bWorking = finder.FindFile( strPath + "*.txt" );
	while ( bWorking )
	{
		bWorking = finder.FindNextFile();
		CTarget* pTarg = LoadTarget( finder.GetFilePath() );
		if( pTarg )
			targets.push_back( pTarg );
	}
}
void CTarget::Output(  )
{
	CString strPath;
	static int ii = 1000;
	
	if ( m_bType == 2 )
		strPath.Format( "F:\\output\\PE%ld.txt", ii++ );
	else if ( m_bType == 1 )
	{
		if( m_nDirct == 2 )
			strPath.Format( "F:\\output\\CB%ld.txt", ii++ );
		else 
			strPath.Format( "F:\\output\\CA%ld.txt", ii++ );
	}


	CStdioFile File( strPath,CFile::modeCreate|CFile::modeWrite);
	CString strTemp;
	for ( int i = 0; i < m_pTraj.size(); i++ )
	{
		strTemp.Format( "%ld;	%.2f;	%.2f;	%.2f;	%.2f\n", m_pFrmID[i], m_pTraj[i].x, m_pTraj[i].y, m_pTraj[i].width, m_pTraj[i].height );
		File.WriteString( strTemp );
	}
	File.Close();
}

int CTarget::Update( Mat& imgC, Mat& imgB, int nFrameID )
{
	double dx, dy;
	int nRes = Track( imgC, imgB, dx, dy );

	m_lastImg = imgC.clone();

	m_pTraj.push_back( m_rctBox );
	m_pFrmID.push_back( nFrameID );
	double d1 = countNonZero( m_TempB ) / (double) ( m_rctBox.area() );
	m_pRatio.push_back( m_dRatio );
	m_pRatio1.push_back( d1 );

	Point2f pt = m_centtrj[m_centtrj.size() - 1];
	pt.x += dx;
	pt.y += dy;
	m_centtrj.push_back( pt );; // center point

	return nRes;
}

void CTarget::UpdateTemplate( Mat& img )
{
	m_TempG = img( m_rctBox ).clone()*0.1 + m_TempG*0.9;
}

int CTarget::BackMatch(Mat& img, int& x, int& y)
{
	x = y = 0;
	if ( m_pObj.size() < 1 ) 
		return 0;

	int nID = MIN( 5, m_pObj.size() - 1 );
	
	Rect2d rctTemp = m_pTraj[nID];

	Mat imgTemp;
	resize( m_pObj[nID], imgTemp, m_rctBox.size() );
	Mat imgMask;
	resize( m_pObjMsk[nID], imgMask, m_rctBox.size(),0,0, INTER_NEAREST );	
	cvtColor( imgMask, imgMask, CV_GRAY2BGR );
	
	Rect2d rctSearch = m_rctBox;
	int nSize = MIN( 10, m_rctBox.width / 3 );
	rctSearch.x = MAX( 0, rctSearch.x - nSize );
	rctSearch.y = MAX( 0, rctSearch.y - nSize );
	rctSearch.width  = MIN( m_rctBox.width  + nSize * 2, img.cols - 1 - rctSearch.x );
	rctSearch.height = MIN( m_rctBox.height + nSize * 2, img.rows - 1 - rctSearch.y );

	if ( rctSearch.width < 1 || rctSearch.height < 1 )
		return 0;

	Mat result;
	result.create( rctSearch.size(), CV_32FC1 );
			
	matchTemplate( img( rctSearch ), imgTemp, result, TM_CCORR_NORMED, imgMask );

	double minVal, maxVal;
	Point minLoc, maxLoc, matchLoc;
	minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

	matchLoc.x = maxLoc.x + rctSearch.x;
	matchLoc.y = maxLoc.y + rctSearch.y;

	x = matchLoc.x - m_rctBox.x;
	y = matchLoc.y - m_rctBox.y;

	Mat res;
	normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
	result.convertTo( res, CV_8UC1, 255, 0 );

	CString strTemp;
	strTemp.Format( "%.2f - %.2f     %ld, %ld", maxVal, minVal, x, y );
	g_pView->SetInterResultImage( 1, res, "matchTemplate：  " + strTemp );
	g_pView->SetInterResultImage( 1, imgTemp, "imgTemp" );
	g_pView->SetInterResultImage( 1, imgMask, "imgMask" );

	
	return 1;
}

int CTarget::BackMatch2( Mat& img, int& x, int& y )
{
	x = y = 0;
	if ( m_pObj.size() < 1 )
		return 0;

	std::vector<Point2f> prevPts;
	std::vector<Point2f> currPts;
	std::vector<Point2f> lastPts;

	Mat refimg;
	cvtColor( m_TempC, refimg, CV_BGR2GRAY );

	double dRatio = countNonZero( m_TempB ) / (double) ( m_rctBox.area() );
	if ( dRatio > 0.1 )
	{
		goodFeaturesToTrack( refimg, prevPts, 10,    // 角点最大数目
			0.08, 3, m_TempB, 3, false, 0.4 );
	}
	else
	{
		Mat mask1( m_TempBA.size(), CV_8UC1, 30 );
		compare( m_TempBA, mask1, mask1, CMP_GT );
		
		goodFeaturesToTrack( refimg, prevPts, 10,    // 角点最大数目
			0.08, 3, mask1, 3, false, 0.4 );
	}

	Mat imgD = m_TempC.clone();

	for ( int i = 0; i < prevPts.size(); i++ )
	{
		Point2f pt0 = Point2f( prevPts[i].x - 5, prevPts[i].y );
		Point2f pt1 = Point2f( prevPts[i].x + 5, prevPts[i].y );
		line( imgD, pt0, pt1, cv::Scalar( 0, 255, 0 ), 1 );

		pt0 = Point2f( prevPts[i].x, prevPts[i].y - 5 );
		pt1 = Point2f( prevPts[i].x, prevPts[i].y + 5 );
		line( imgD, pt0, pt1, cv::Scalar( 0, 255, 0 ), 1 );
	}

	// If no found, return
	if ( !prevPts.size() )
		return 0;

	BYTE pHis[100];
	memset( pHis, 0, 100 );

	double dX = 0;
	double dY = 0;

	std::vector<uchar> status;
	std::vector<float> errors;
	std::vector<uchar> status1;
	std::vector<float> errors1;

	std::vector<Point2f> pts[100];
	std::vector<Point2f> pts0[100];

	Mat imgTotal = m_TempC.clone();
	Mat imgTotal3 = m_TempC.clone();

	for ( int j = 1; j < 10; j++ )
	{
		int nID = MIN( j*2, m_pObj.size() - 1 );

		Mat realImg;
		resize( m_pObj[nID], realImg, m_rctBox.size() );

		//hconcat( imgTotal, realImg, imgTotal );

		Mat realImgC = realImg.clone();
		cvtColor( realImg, realImg, CV_BGR2GRAY );

		status.clear();
		errors.clear();

		calcOpticalFlowPyrLK( refimg, realImg, prevPts, currPts, status, errors, Size( 6, 6 ), 0,
			TermCriteria( static_cast<int>( TermCriteria::COUNT ) + static_cast<int>( TermCriteria::EPS ), 40, 0.183328975142 ),
			OPTFLOW_LK_GET_MIN_EIGENVALS, 1e-4 );

		status1.clear();
		errors1.clear();
		calcOpticalFlowPyrLK( realImg, refimg, currPts, lastPts,  status1, errors1, Size( 6, 6 ), 0,
			TermCriteria( static_cast<int>( TermCriteria::COUNT ) + static_cast<int>( TermCriteria::EPS ), 40, 0.183328975142 ),
			OPTFLOW_LK_GET_MIN_EIGENVALS, 1e-4 );

		/*Mat imgE = imgD.clone();
		for ( int i = 0; i < prevPts.size(); i++ )
		{
			if ( status1[i] != !0 )
				continue;
			Point2f pt0 = Point2f( lastPts[i].x - 3, lastPts[i].y );
			Point2f pt1 = Point2f( lastPts[i].x + 3, lastPts[i].y );
			line( imgE, pt0, pt1, cv::Scalar(  255,0, 0 ), 1 );

			pt0 = Point2f( lastPts[i].x, lastPts[i].y - 3 );
			pt1 = Point2f( lastPts[i].x, lastPts[i].y + 3 );
			line( imgE, pt0, pt1, cv::Scalar(  255,0, 0 ), 1 );
		}
		hconcat( imgTotal3, imgE, imgTotal3 );*/

		double x1 = 0;
		double y1 = 0;
		int nCnt = 0;
		pts[0].clear();
		pts0[0].clear();
		for ( int i = 0; i < prevPts.size(); i++ )
		{
			pts[0].push_back( prevPts[i] );
			pts0[0].push_back( Point2f( m_rctBox.x + m_rctBox.width / 2, m_rctBox.y + m_rctBox.height / 2 ) );

			if ( status[i] != !0 || status1[i] != !0 )
			{
				pts[j].push_back( Point2f(-1,-1) );
				pts0[j].push_back( Point2d( -1, -1 ) );
				continue;
			}
			if ( fabs( currPts[i].y - prevPts[i].y ) > 5 || norm( lastPts[i] - prevPts[i]) > 2 )
			{
				pts[j].push_back( Point2f( -1, -1 ) );
				pts0[j].push_back( Point2d( -1, -1 ) );
				continue;
			}
			double x2 = m_rctBox.x + prevPts[i].x - ( currPts[i].x*m_pTraj[nID].width / m_rctBox.width + m_pTraj[nID].x );
			double y2 = m_rctBox.y + prevPts[i].y - ( currPts[i].y*m_pTraj[nID].height/ m_rctBox.height+ m_pTraj[nID].y );

			pts0[j].push_back(Point2d( x2 + m_pTraj[nID].x + m_pTraj[nID].width / 2, y2 + m_pTraj[nID].y + m_pTraj[nID].height / 2));

			pts[j].push_back( currPts[i] );
			pHis[i]++;
		}
	}

	//g_pView->SetInterResultImage( 1, imgTotal3, " imgTotal3 " );
		
	{
		int nCnt = 0;
		Mat matX( 1, 100, CV_32FC1 );
		Mat matY( 1, 100, CV_32FC1 );

		float *pDataX = (float*) matX.data;
		float *pDataY = (float*) matY.data;
		for ( int j = 0; j < 10; j++ )
		for ( int i = 0; i < prevPts.size(); i++ )
		{
			if ( pts0[j][i].x < 0 ) continue;
			pDataY[nCnt] = pts0[j][i].y;
			pDataX[nCnt] = pts0[j][i].x;
			nCnt++;
		}
		matX = matX( Rect( 0, 0, nCnt, 1 ) );
		matY = matY( Rect( 0, 0, nCnt, 1 ) );

		cv::sort( matX, matX, CV_SORT_EVERY_ROW | CV_SORT_ASCENDING );
		cv::sort( matY, matY, CV_SORT_EVERY_ROW | CV_SORT_ASCENDING );

		dX = 0;
		dY = 0;
		int nCount = 0;
		for ( int i = nCnt / 3; i < nCnt - nCnt / 3; i++ )
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

	

	
	/*Mat imgTotal2 = imgTotal.clone();
	Mat imgOff( 100, img.cols + 100, CV_8UC3, Scalar( 200, 200, 200 ) );
	for ( int i = 0; i < prevPts.size(); i++ )
	{
		for ( int j = 0; j<9; j++ )
		{
			Point2f pt0 = Point2f( 0, j );
			Point2f pt1 = Point2f( 100, j );
			line( imgOff, pt0, pt1, cv::Scalar( 64, 64, 64 ), 1 );
			
			{
				Point2f pt0 = Point2f( 100, j );
				Point2f pt1 = Point2f( pts0[0][0].x + 100, j );
				if( j < 4 )
					line( imgOff, pt0, pt1, cv::Scalar( 128, 128, 128), 1 );
				else
				{
					pt1 = Point2f( dX + 100, j );
					line( imgOff, pt0, pt1, cv::Scalar( 100, 200, 100 ), 1 );
				}
		}	}

		for( int j=0; j<9; j++ )
		{
			Point2f pt0 = Point2f( 0, (i+1)*10+j );
			Point2f pt1 = Point2f( pHis[i]*10, ( i + 1 ) * 10 + j );
			line( imgOff, pt0, pt1, cv::Scalar( 64, 64, 64 ), 1 );
			if ( pHis[i] )
			{			
				Point2f pt0 = Point2f( 100, ( i + 1 ) * 10 + j );
				Point2f pt1 = Point2f( pts0[j + 1][i].x +100, ( i + 1 ) * 10 + j );
				line( imgOff, pt0, pt1, cv::Scalar( (i%2)*200, 100, 200-(i%2)*200 ), 1 );
		}	}

		imgTotal2 = imgTotal.clone();
		for ( int j = 0; j < 10; j++ )
		{
			if ( pts[j][i].x < 0 ) continue;
			Point2f pt0 = Point2f( pts[j][i].x - 5 + j * m_TempC.cols, pts[j][i].y );
			Point2f pt1 = Point2f( pts[j][i].x + 5 + j * m_TempC.cols, pts[j][i].y );
			line( imgTotal2, pt0, pt1, cv::Scalar( 0, 0, 255 ), 1 );

			pt0 = Point2f( pts[j][i].x + j * m_TempC.cols, pts[j][i].y - 5 );
			pt1 = Point2f( pts[j][i].x + j * m_TempC.cols, pts[j][i].y + 5 );
			line( imgTotal2, pt0, pt1, cv::Scalar( 0, 0, 255 ), 1 );
		}
		CString strTemp;
		strTemp.Format( "%ld   :  %ld", i, pHis[i] );
		g_pView->SetInterResultImage( 1, imgTotal2, " imgTotal2 " + strTemp );
	}
	g_pView->SetInterResultImage( 1, imgOff, " imgOff " );*/


	//hconcat( realImgC, imgD, imgD );

	int nCount = 0;
	for ( int i = 0; i < prevPts.size(); i++ )
	{
		Point2f pt0 = Point2f( prevPts[i].x - 3 - pHis[i], prevPts[i].y );
		Point2f pt1 = Point2f( prevPts[i].x + 3 + pHis[i], prevPts[i].y );
		if( pHis[i] )
			line( imgD, pt0, pt1, cv::Scalar( 0, 0, 255 ), 1 );

		pt0 = Point2f( prevPts[i].x, prevPts[i].y - 3 - pHis[i] );
		pt1 = Point2f( prevPts[i].x, prevPts[i].y + 3 + pHis[i] );
		if( pHis[i] )
			line( imgD, pt0, pt1, cv::Scalar( 0, 0, 255 ), 1 );
/*
		pt0 = Point2f( currPts[i].x, currPts[i].y );
		pt1 = Point2f( prevPts[i].x + realImg.cols, prevPts[i].y );
		line( imgD, pt0, pt1, cv::Scalar( 0, 255, 0 ), 1 );
		*/
	}

	x = dX - m_rctBox.x - m_rctBox.width / 2;
	y = dY - m_rctBox.y - m_rctBox.height/ 2;
	
	/*hconcat( m_TempC, imgD, imgD );
	CString strTemp;
	for ( int i = 0; i < 10; i++ )
	{
		CString strT;
		strT.Format( "  %ld  ", pHis[i] );
		strTemp += strT;
	}
	g_pView->SetInterResultImage( 1, imgD, "imgD  " + strTemp);*/

	return 1;
}


int CTarget::BlockMatch( Mat& img, double&x, double&y, double&dRatio )
{
	x = y = 0;
	Rect2d rctSearch = m_rctBox;
	int nSize = MIN( 50, m_rctBox.width / 3 );
	rctSearch.x = MAX( 0, rctSearch.x - nSize );
	rctSearch.y = MAX( 0, rctSearch.y - nSize );
	rctSearch.width  = MIN( m_rctBox.width + nSize*2, img.cols - 1 - rctSearch.x );
	rctSearch.height = MIN( m_rctBox.height + nSize*2, img.rows - 1 - rctSearch.y );

	if ( rctSearch.width < 1 || rctSearch.height < 1 )
		return 0;

	Mat result;
	result.create( rctSearch.size(), CV_32FC1 );

	
	Mat maskc;	
	
	dRatio = countNonZero( m_TempB ) / (double) ( m_rctBox.area() );

	CString strTemp;
	strTemp.Format( "%.2f, %.2f", dRatio, m_dRatio );
	
	bool bCar = dRatio < 0.15 || ( dRatio < 0.16 && dRatio / ( m_dRatio + 0.1 ) < 0.5 );
	bool bPed = dRatio < 0.08 || ( dRatio < 0.10 && dRatio / ( m_dRatio + 0.0 ) < 0.4 );

	if ( (m_bType == 1 && bCar) || (m_bType == 2 && bPed) ) // too little, change too fast
	{
		Mat mask1( m_TempBA.size(), CV_8UC1, 30 );
		compare( m_TempBA, mask1, maskc, CMP_GT );
		cvtColor( maskc, maskc, CV_GRAY2BGR );

		g_pView->SetInterResultImage( 1, maskc, "m_TempBA > 30 for mask  " + strTemp );
	}
	else
	{
		cvtColor( m_TempB, maskc, CV_GRAY2BGR );
		g_pView->SetInterResultImage( 1, maskc, " m_TempB as mask  " + strTemp );
	}
	
	matchTemplate( img( rctSearch ), m_TempC, result, TM_CCORR_NORMED, maskc );
	
	static Mat simgPower( 1, 1, result.type() );
	//if ( simgPower.cols < 2 )
	{
		int nSize = MAX(10, m_rctBox.width / 3);
		int nSizeY = nSize / 4;
		if ( m_bType == 2 )
			nSizeY = nSize;
		Mat kernelX = getGaussianKernel( 200, nSize );
		Mat kernelY = getGaussianKernel( 200, nSizeY );
		simgPower = kernelY * kernelX.t();
	}
	Mat power;

	{
		Mat res;
				
		if ( m_ptSpeed.x < -800 )
			power = Mat( result.size(), CV_32FC1, Scalar( 1 ) );
		else
		{
			int nX = m_ptSpeed.x <= -nSize ? -nSize : m_ptSpeed.x >= nSize ? nSize : m_ptSpeed.x;
			int nY = m_ptSpeed.y <= -nSize ? -nSize : m_ptSpeed.y >= nSize ? nSize : m_ptSpeed.y;
			Rect rect( 100 - nX - nSize, 100 - nY - nSize, result.cols, result.rows );
			power = simgPower( rect ).clone();


			//simgPower.convertTo( res, CV_8UC1, 255, 0 );
			//rectangle( res, rect, Scalar( 128 ), 1 );
			//g_pView->SetInterResultImage( 1, res, "simgPower  "  );

			//power.convertTo( res, CV_8UC1, 255, 0 );
			//res.at<BYTE>( MIN(nY + nSize,res.rows-1), MIN(nX+nSize, res.cols-1) ) = 0;
			//CString strTemp;
			//strTemp.Format( "%.2f, %.2f, %ld, %ld", m_ptSpeed.x, m_ptSpeed.y, nX - nSize, nY - nSize );
			//g_pView->SetInterResultImage( 1, res, "power  " + strTemp );
		}
	}

	result.convertTo( result, power.type() );

	Mat res;
	normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
	result.convertTo( res, CV_8UC1, 255, 0 );
	g_pView->SetInterResultImage( 1, res, "result" );

	result = result.mul( power );
	result.convertTo( res, CV_8UC1, 255, 0 );


	double minVal,maxVal; 
	Point minLoc,maxLoc,matchLoc;
	minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

	matchLoc.x = maxLoc.x + rctSearch.x;
	matchLoc.y = maxLoc.y + rctSearch.y;
	Rect rctCan( matchLoc, m_rctBox.size() );

	x = matchLoc.x - m_rctBox.x;
	y = matchLoc.y - m_rctBox.y;
		
	strTemp.Format( "%.2f - %.2f     %.2f, %.2f", maxVal, minVal,x,y );
	g_pView->SetInterResultImage( 1, res, "matchTemplate：  " + strTemp );
	//g_pView->SetInterResultImage( 1, m_TempB, "m_TempB" );

	/*Mat imgC = m_TempC.clone();
	for ( int i = 0; i < imgC.rows; i++ )
	for ( int j = 0; j < imgC.cols; j++ )
	{
		if ( !maskc.data[( i*imgC.cols + j ) * 3 + 2] ) continue;
		imgC.data[( i*imgC.cols + j ) * 3 + 2] = MIN( 255, 255 - imgC.data[( i*imgC.cols + j ) * 3 + 2] );
	}
	hconcat( m_TempC, imgC, imgC );
	g_pView->SetInterResultImage( 1, imgC, "m_TempC" );

	g_pView->SetInterResultImage( 1, img( rctSearch ).clone(), "rctSearch" );*/

	
	//dRatio = 1;// countNonZero( img( rctCan ) ) / (double) ( m_rctBox.area() );
	if ( dRatio < 0.1 && dRatio / m_dRatio < 0.5 ) // too little, change too fast
		return 1;

	return 1;
}


int CTarget::FeatureMatch( Mat& img, double& x, double& y )
{/*
	x = y = 0;
	Rect2d rctSearch = m_rctBox;
	int nSize = MIN( 30, m_rctBox.width / 3 );
	rctSearch.x = MAX( 0, rctSearch.x - nSize );
	rctSearch.y = MAX( 0, rctSearch.y - nSize );
	rctSearch.width = MIN( m_rctBox.width + nSize * 2, img.cols - 1 - rctSearch.x );
	rctSearch.height = MIN( m_rctBox.height + nSize * 2, img.rows - 1 - rctSearch.y );

	if ( rctSearch.width < 1 || rctSearch.height < 1 )
		return 0;

	static bool bFirst = true;
	static Mat refimg = m_lastImg( rctSearch ).clone();
	if ( refimg.channels() == 3 )
		cvtColor( refimg, refimg, CV_RGB2GRAY );

	g_pView->SetInterResultImage( 1, refimg, "refimg" );

	Mat refMask = Mat::zeros( refimg.size(), CV_8UC1 );
	Rect2d rctMask = m_rctBox & rctSearch;
	rctMask.x -= rctSearch.x;
	rctMask.y -= rctSearch.y;
	//rectangle( refMask, rctMask, Scalar::all(255), CV_FILLED );
	//g_pView->SetInterResultImage( 1, refMask, "refMask" );

	Mat ref( m_TempBA.size(), CV_8UC1, Scalar( 30 ) );
	compare( m_TempBA, ref, ref, CMP_GT );
	ref.copyTo( refMask( rctMask ) );

//	Mat element = getStructuringElement( MORPH_RECT, Size( 3, 3 ) );
//	morphologyEx( refMask, refMask, MORPH_DILATE, element );

	g_pView->SetInterResultImage( 1, refMask, "refMask" );
	

	
	goodFeaturesToTrack( refimg, prevPts, 500,    // 角点最大数目
					0.04122,  // 质量等级，这里是0.01*max（min（e1，e2）），e1，e2是harris矩阵的特征值
					3.5496, refMask, 3, false, 0.4 );
	
	// If no found, return
	if ( !prevPts.size() )
		return 0;


	Mat realImg = img( rctSearch ).clone();
	if ( realImg.channels() == 3 )
		cvtColor( realImg, realImg, CV_RGB2GRAY );

	g_pView->SetInterResultImage( 1, realImg, "realImg" );

	std::vector<uchar> status;
	std::vector<float> errors;
	Size window = Size( 6, 6 );

	calcOpticalFlowPyrLK( refimg, realImg, prevPts, currPts, status, errors, window, 5,
		TermCriteria( static_cast<int>( TermCriteria::COUNT ) + static_cast<int>( TermCriteria::EPS ), 40, 0.183328975142 ),
		0, 1e-4 );


	float* pXY = new float[128 * 128];
	memset( pXY, 0, sizeof( float ) * 128 * 128 );

	Mat img1 = img.clone();

	double dX = 0;
	double dY = 0;
	int nCount = 0;
	for ( int i = 0; i < prevPts.size(); i++ )
	{
		if ( status[i] != !0 ) continue;

		int nX = ( currPts[i].x - prevPts[i].x ) * 4 + 64;
		int nY = ( currPts[i].y - prevPts[i].y ) * 4 + 64;
		dX += currPts[i].x - prevPts[i].x;
		dY += currPts[i].y - prevPts[i].y;
		nCount++;

		{
			Point2f pt0 = Point2f( prevPts[i].x + rctSearch.x, prevPts[i].y + rctSearch.y );
			Point2f pt1 = Point2f( currPts[i].x + rctSearch.x, currPts[i].y + rctSearch.y ); 
			line( img1, pt0, pt1, cv::Scalar( 0, 0, 255 ), 1 );
		}

		if ( nX >= 0 && nY >= 0 && nX < 128 && nY < 128 )
			pXY[nY * 128 + nX] ++;
	}
	if ( nCount )
	{
		dX /= nCount;
		dY /= nCount;
	}

	
	// 
	pXY[64 * 128 + 64] = 0;
	
	Mat m1( 128, 128, CV_32FC1, pXY );
	normalize( m1, m1, 255, 0, CV_MINMAX );
	Mat m2;
	m1.convertTo( m2, CV_8UC1 );
	g_pView->SetInterResultImage( 1, m2, " pXY " );


	double dMax = 0;
	int nMaxID = 0;
	for ( int i = 0; i < 128 * 128; i++ )
	{
		if ( pXY[i] < dMax ) continue;
		dMax = pXY[i];
		nMaxID = i;
	}
	delete pXY;
	pXY = NULL;

	if ( dMax > 1 )
	{
		x = ( ( nMaxID % 128 ) - 64 ) / 4;
		y = ( ( nMaxID / 128 ) - 64 ) / 4;
	}

	CString strTemp;
	strTemp.Format( "(%.2f, %.2f)    %.2f, %.2f ", x, y, dX, dY );
	g_pView->SetInterResultImage( 1, img1, " offset " + strTemp );

	return dMax > 1 ? 1: 0;*/
	return 0;
}

void CTarget::Store( string &strFileName )
{

}