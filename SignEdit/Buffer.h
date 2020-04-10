#pragma once
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <opencv2\opencv.hpp>

using namespace cv;

#define MAX_BUFFER  3600

struct STU_BUF
{
	Mat Img; // gray
	Mat Msk;
};


class CBuffer
{
public:
	CBuffer( );
	~CBuffer();

	void clear();
	void Add( unsigned int nFrmID, Mat &img, Mat& mogMask );
	int  GetStartPos();
	int  GetCurBuff( unsigned int& nFrmID, Mat& Img, Mat& Mask );
	int  GetLastBuff( int nID, unsigned int& nFrmID, Mat& Img, Mat& Mask );
	int  GetLastBuff( int nID );
	int	 GetNextBuff( int nID, unsigned int& nFrmID, Mat& Img, Mat& Mask );
	int	 GetNextBuff( int nID );

	static STU_BUF* m_pBuff;
	static unsigned int *m_pFrmIDs;
	static unsigned int m_nCurID;
};

