#include "stdafx.h"
#include "Buffer.h"


STU_BUF* CBuffer::m_pBuff = NULL;
unsigned int *CBuffer::m_pFrmIDs = NULL;
unsigned int CBuffer::m_nCurID = 0;

CBuffer::CBuffer()
{
	if( !m_pBuff )
	{
		m_pBuff   = new STU_BUF[MAX_BUFFER];	
		m_pFrmIDs = new unsigned int[MAX_BUFFER];
		memset( m_pFrmIDs, 0, sizeof( unsigned int )*MAX_BUFFER );
		m_nCurID = 0;
	}
}
CBuffer::~CBuffer()
{
}
void CBuffer::clear()
{
	memset( m_pFrmIDs, 0, sizeof( unsigned int )*MAX_BUFFER );
	m_nCurID = 0;
}
void CBuffer::Add( unsigned int nFrmID, Mat &img, Mat& mogMask )
{
	m_pFrmIDs[m_nCurID] = nFrmID;

	m_pBuff[m_nCurID].Img = img.clone();
	m_pBuff[m_nCurID].Msk = mogMask.clone();

	m_nCurID = (m_nCurID + 1) % MAX_BUFFER;	// recur
}
int CBuffer::GetStartPos( )
{
	int nID0 = ( m_nCurID - 1 + MAX_BUFFER ) % MAX_BUFFER;
	int nID1 = ( m_nCurID - 2 + MAX_BUFFER ) % MAX_BUFFER;

	if ( m_pFrmIDs[nID0] <= m_pFrmIDs[nID1] )
		return -1;

	return nID0;
}
int CBuffer::GetCurBuff( unsigned int& nFrmID, Mat& Img, Mat& Mask )
{
	int nID0 = ( m_nCurID - 1 + MAX_BUFFER ) % MAX_BUFFER;
	int nID1 = ( m_nCurID - 2 + MAX_BUFFER ) % MAX_BUFFER;

	if ( m_pFrmIDs[nID0] <= m_pFrmIDs[nID1] )
		return -1;

	Img  = m_pBuff[nID0].Img;
	Mask = m_pBuff[nID0].Msk;
	return nID0;
}
// return current unit and nID--;
int CBuffer::GetLastBuff( int nID, unsigned int& nFrmID, Mat& Img, Mat& Mask )
{
	if ( nID < 0 )
		return -1;

	int nID0 = ( nID + MAX_BUFFER ) % MAX_BUFFER;
	nID = ( nID - 1 + MAX_BUFFER ) % MAX_BUFFER;

	Img = m_pBuff[nID0].Img;
	Mask = m_pBuff[nID0].Msk;
	nFrmID = m_pFrmIDs[nID0];

	if ( m_pFrmIDs[nID0] <= m_pFrmIDs[nID] || !m_pFrmIDs[nID] )
		return -1;
	
	return nID;
}
// return current unit and nID--;
int CBuffer::GetLastBuff( int nID )
{
	if ( nID < 0 )
		return -1;

	int nID0 = ( nID + MAX_BUFFER ) % MAX_BUFFER;
	nID = ( nID - 1 + MAX_BUFFER ) % MAX_BUFFER;

	if ( m_pFrmIDs[nID0] <= m_pFrmIDs[nID] || !m_pFrmIDs[nID] )
		return -1;

	return nID;
}

int CBuffer::GetNextBuff( int nID, unsigned int& nFrmID, Mat& Img, Mat& Mask )
{
	if ( nID < 0 )
		return -1;

	int nID0 = ( nID + MAX_BUFFER ) % MAX_BUFFER;
	nID = ( nID + 1 + MAX_BUFFER ) % MAX_BUFFER;

	Img = m_pBuff[nID0].Img;
	Mask = m_pBuff[nID0].Msk;
	nFrmID = m_pFrmIDs[nID0];

	if ( m_pFrmIDs[nID0] >= m_pFrmIDs[nID] || !m_pFrmIDs[nID] )
		return -1; 
	
	return nID;
}

int CBuffer::GetNextBuff( int nID )
{
	if ( nID < 0 )
		return -1;

	int nID0 = ( nID + MAX_BUFFER ) % MAX_BUFFER;
	nID = ( nID + 1 + MAX_BUFFER ) % MAX_BUFFER;

	if ( m_pFrmIDs[nID0] >= m_pFrmIDs[nID] || !m_pFrmIDs[nID] )
		return -1;

	return nID;
}

