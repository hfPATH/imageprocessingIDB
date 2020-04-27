// DialogLeft.cpp : ÊµÏÖÎÄ¼þ
//

#include "stdafx.h"
#include "DriveSim.h"
#include "DialogLeft.h"
#include "afxdialogex.h"
#include <string>
#include <vector>
#include <iterator> 
#include <afxmt.h>
#include "DriveSimDlg.h"

using namespace std;

// CDialogLeft
#define DRAW_TIMER 1

CMutex g_mutxImg;


extern LARGE_INTEGER g_liPerfFreq;
extern LARGE_INTEGER g_liPerfStart;

IMPLEMENT_DYNAMIC(CDialogLeft, CDialogEx)

CDialogLeft::CDialogLeft(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LEFT, pParent)
{
	m_rectFullScrean.left = 0;
	m_rectFullScrean.top = 0;
	m_bMidWnd = FALSE;
	m_bUpdateZero = FALSE;
	m_nShowDirct = 0;
	m_bStop = TRUE;
	m_nLeft = -1;
	m_nTop = -1;
	m_nRight = -1;
	m_nBottom = -1;
}

CDialogLeft::CDialogLeft(int leftPos, int topPos, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_LEFT, pParent)
{
	m_rectFullScrean.left = leftPos;
	m_rectFullScrean.top = topPos;
	m_bMidWnd = FALSE;
	m_bUpdateZero = FALSE;
	m_nShowDirct = 0;
	m_bStop = TRUE;
	m_nLeft = -1;
	m_nTop = -1;
	m_nRight = -1;
	m_nBottom = -1;
}

CDialogLeft::~CDialogLeft()
{
}

void CDialogLeft::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogLeft, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_MESSAGE(WM_MYMESSAGE, &CDialogLeft::OnMyMessage)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDialogLeft ÏûÏ¢´¦Àí³ÌÐò
LRESULT	CDialogLeft::OnMyMessage(WPARAM wParam, LPARAM lParam)
{

	Invalidate(FALSE);
	UpdateWindow();

	return LRESULT();
}

BOOL CDialogLeft::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_rectFullScrean.right = m_rectFullScrean.left + 2016;
	m_rectFullScrean.bottom = m_rectFullScrean.top + 1200;
	pBitmapInfo = nullptr;
	m_fSuggestVelocity = 0;
	m_fCurrentVelocity = 0;

	MoveWindow(m_rectFullScrean, 1);

	font.CreateFont(120,                                        //字体大小
		0, 0, 0, FW_NORMAL,
		FALSE, FALSE,
		0,
		ANSI_CHARSET,                  // nCharSet
		OUT_DEFAULT_PRECIS,     // nOutPrecision
		CLIP_DEFAULT_PRECIS,     // nClipPrecision
		DEFAULT_QUALITY,             // nQuality
		DEFAULT_PITCH | FF_SWISS, _T("Arial"));             // 字体
	pBitmapInfo = nullptr;

	m_imgRight = Mat(100, 100, CV_8UC3, Scalar(0, 0, 0));

	vector<Point> contour;
	contour.push_back(Point(199, 100));
	contour.push_back(Point(100, 2));
	contour.push_back(Point(100, 66));
	contour.push_back(Point(0, 66));
	contour.push_back(Point(0, 133));
	contour.push_back(Point(100, 133));
	contour.push_back(Point(100, 199));
	contour.push_back(Point(199, 100));
	for (int i = 0; i < contour.size(); i++)
	{
		contour[i].x /= 2;
		contour[i].y /= 2;
	}

	// create a pointer to the data as an array of points (via a conversion to 
	const cv::Point *pts = (const cv::Point*) Mat(contour).data;
	int npts = Mat(contour).rows;

	// draw the polygon 
	fillPoly(m_imgRight, &pts, &npts, 1, Scalar(0, 255, 0));

	flip(m_imgRight, m_imgLeft, 1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // Òì³£: OCX ÊôÐÔÒ³Ó¦·µ»Ø FALSE
}

bool TranslateVectorToBitmap(int dwHeight, int dwWidth, BYTE* pVector, BYTE*& pBitmapData, WORD flag)
{
	int i, j;
	if (flag == 24) dwWidth *= 3;
	else if (flag != 8) return false;
	int dwWriteWidth = (dwWidth + 3) / 4 * 4;

	if (pBitmapData) delete[]pBitmapData;
	pBitmapData = (BYTE*) new BYTE[dwHeight*dwWriteWidth];

	for (i = 0; i<dwHeight; i++)
		for (j = 0; j<dwWidth; j++)
			pBitmapData[(i)*dwWriteWidth + j] = pVector[i*dwWidth + j];
	//pBitmapData[(dwHeight-1-i)*dwWriteWidth+j]   = pVector[i*dwWidth+j];

	return true;
}
bool CreateBITMAPINFO(BITMAPINFO *&pbi, int dwHeight, int dwWidth, WORD flag)
{
	if (flag == 24) dwWidth *= 3;
	int dwWriteSize = dwHeight * ((dwWidth + 3) / 4 * 4);

	if (pbi) delete pbi;
	if (flag == 24)
		pbi = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)];
	else
		pbi = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	pbi->bmiHeader.biSize = sizeof(BITMAPINFO) - 4;//ÎÞÂÛÊÇºÚ°×Í¼»¹ÊÇ²ÊÍ¼¶¼ÊÇ40
	pbi->bmiHeader.biBitCount = flag;
	pbi->bmiHeader.biHeight = -dwHeight;

	if (flag == 24)
		pbi->bmiHeader.biWidth = dwWidth / 3;
	else
		pbi->bmiHeader.biWidth = dwWidth;

	pbi->bmiHeader.biSizeImage = dwWriteSize;
	pbi->bmiHeader.biCompression = 0;
	pbi->bmiHeader.biPlanes = 1;
	pbi->bmiHeader.biClrImportant = 0;
	pbi->bmiHeader.biClrUsed = 0;
	pbi->bmiHeader.biXPelsPerMeter =
		pbi->bmiHeader.biYPelsPerMeter = 0;
	pbi->bmiColors[0].rgbBlue =
		pbi->bmiColors[0].rgbGreen =
		pbi->bmiColors[0].rgbRed =
		pbi->bmiColors[0].rgbReserved = 0;

	//ÈôÊÇ256É«Í¼ÏñÔò£¬Ð´µ÷É«°å
	if (flag == 8)
	{
		for (int i = 1; i<256; i++)
		{
			BYTE temp = (BYTE)i;
			pbi->bmiColors[i].rgbRed =
				pbi->bmiColors[i].rgbGreen =
				pbi->bmiColors[i].rgbBlue = (BYTE)i;
			pbi->bmiColors[i].rgbReserved = 0;

		}
	}

	return true;
}

extern double g_nMaxSpeed;
extern double g_nMinSpeed;

void CDialogLeft::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectClient;
	GetClientRect(&rectClient);

	int nClientWidth  = rectClient.Width();
	int nClientHeight = rectClient.Height();

	if (m_bStop)
	{
		LOGFONT logFont;
		ZeroMemory((void*)&logFont, sizeof(logFont));
		strcpy(logFont.lfFaceName, "Arial");
		logFont.lfHeight = -180;
		logFont.lfWeight = 400;
		logFont.lfCharSet = DEFAULT_CHARSET;
		logFont.lfOutPrecision = 3;
		logFont.lfClipPrecision = 2;
		logFont.lfQuality = 1;		
		logFont.lfPitchAndFamily = 0;

		dc.SetTextColor(RGB(250, 250, 250));

		CFont font;
		font.CreateFontIndirect(&logFont);
		dc.SelectObject(font);
		dc.SetBkMode(TRANSPARENT);

		CBrush brush(RGB(128, 128, 128));
		dc.SelectObject(&brush);
		dc.FillRect(rectClient,&brush);

		if (m_bMidWnd)
		{
			CRect rct;
			dc.DrawText(m_strText, -1, &rct, DT_CALCRECT);
			dc.TextOut(nClientWidth / 2 - rct.Width() / 2, nClientHeight / 2 - rct.Height() / 2, m_strText);
		}
		return;
	}	

		
	if (!m_bMidWnd)
	{
		//g_mutxImg.Lock();
		if (img.rows > 1)
		{
			CreateBITMAPINFO(pBitmapInfo, img.rows, img.cols, 24);

			StretchDIBits(dc.GetSafeHdc(), 0, 0, nClientWidth,
				nClientHeight, 0, 0, img.cols, img.rows,
				img.data, pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
		}
		//g_mutxImg.Unlock();
		return;
	}
		
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	MemDC.SetStretchBltMode(COLORONCOLOR);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, nClientWidth, nClientHeight);

	CBitmap* pOldBm = MemDC.SelectObject(&bmp);
	MemDC.SetBkMode(TRANSPARENT);

	g_mutxImg.Lock();

	CreateBITMAPINFO(pBitmapInfo, img.rows, img.cols, 24);

	StretchDIBits(MemDC.GetSafeHdc(), 0, 0, nClientWidth,
		nClientHeight, 0, 0, img.cols, img.rows,
		img.data, pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	
	//
	if (m_nTop >= 0 && m_nLeft >= 0)
	{
		BITMAPINFO* pBitmapInfoS = NULL;
		CreateBITMAPINFO(pBitmapInfoS, m_imgS.rows, m_imgS.cols, 24);

		StretchDIBits(MemDC.GetSafeHdc(), m_nLeft, m_nTop, m_imgS.cols, m_imgS.rows, 0, 0, m_imgS.cols, m_imgS.rows,
			m_imgS.data, pBitmapInfoS, DIB_RGB_COLORS, SRCCOPY);

		delete pBitmapInfoS;
	}

	if ( m_imgM.rows > 0)
	{
		BITMAPINFO* pBitmapInfoM = NULL;
		CreateBITMAPINFO(pBitmapInfoM, m_imgM.rows, m_imgM.cols, 24);

		int nX = (1920 - m_imgM.cols);
		int nY = (1080 - 1 - m_imgM.rows) * 2;
		StretchDIBits(MemDC.GetSafeHdc(), nX, nY, m_imgM.cols * 2, m_imgM.rows * 2, 0, 0, m_imgM.cols, m_imgM.rows,
			m_imgM.data, pBitmapInfoM, DIB_RGB_COLORS, SRCCOPY);

		delete pBitmapInfoM;

		double nAngle = (m_fCurrentVelocity * 180) / 120.0;
		double dSin = sin(-nAngle * CV_PI / 180) * (m_imgM.rows - 60) * 2;
		double dCos = cos(-nAngle * CV_PI / 180) * (m_imgM.rows - 60) * 2;

		CPen pen1(0, 5, RGB(255, 255, 255));
		MemDC.SelectObject(&pen1);
		MemDC.MoveTo(nClientWidth / 2, nClientHeight - 10);
		MemDC.LineTo(nClientWidth / 2 - dCos, nClientHeight - 10 + dSin);	
	}

	g_mutxImg.Unlock();

	/*if (m_bMidWnd)
	{
		CPen pen1(0, 1, RGB(255, 0, 0));
		MemDC.SelectObject(&pen1);


		extern CDriveSimDlg* g_pWnd;
		int nID = g_pWnd->m_imuData.nCurID;

		if (m_pAngleX.size() > 1)
		{
			MemDC.MoveTo(0, (m_pAngleX[nID]- m_pAngleX[0]) * 10 * 180 / CV_PI + 360);
			for (int i = nID,j=0; i < m_pAngleX.size() ; i++,j++)
				MemDC.LineTo(j * 2, (m_pAngleX[i] - m_pAngleX[0]) * 10 * 180 / CV_PI + 720);

			MemDC.MoveTo(0, m_pAngleY[nID] * 180 / CV_PI + 720);
			for (int i = nID,j=0; i < m_pAngleY.size() ; i++,j++)
				MemDC.LineTo(j * 2, (m_pAngleY[i] - m_pAngleY[0]) * 10 * 180 / CV_PI + 1080);

			MemDC.MoveTo(0, m_pAngleZ[nID] * 180 / CV_PI + 1080);
			for (int i = nID,j=0; i < m_pAngleZ.size() ; i++,j++)
				MemDC.LineTo(j * 2, (m_pAngleZ[i] - m_pAngleZ[0]) * 10 * 180 / CV_PI + 1440);
		}
	}*/

	dc.BitBlt(0, 0, nClientWidth, nClientHeight, &MemDC, 0, 0, SRCCOPY);

	

	MemDC.SelectObject(pOldBm);
	DeleteDC(MemDC);
}
BOOL CDialogLeft::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYUP ||
		pMsg->message == MM_JOY1BUTTONDOWN ||
		pMsg->message == MM_JOY1BUTTONUP ||
		pMsg->message == MM_JOY2BUTTONDOWN)//&& m_bStarted)
	{
		GetParent()->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDialogLeft::UpdateImage(std::string& strFileName, float fSuggestV, float fCurrentV, int bShowDirect, int nX, int nY,int nInSignSpeed ) // 
{
	m_nShowDirct = bShowDirect;
	if (!m_bMidWnd && strFileName.length() < 1) return;

	
	if (strFileName.length() > 1)
	{
		if (m_bMidWnd)
		{
			CString strFileN;
			strFileN.Format("%s", strFileName.c_str());
			strFileN.Replace(".bmp", "_m.bmp");
			string strFile = format("%s", strFileN);
			Mat imgM = imread(strFile, IMREAD_COLOR);

			m_nLeft = nX;
			m_nTop = nY;

			if (m_nShowDirct)
			{
				int nOffX = imgM.cols - m_imgLeft.cols;

				BYTE* pArrow = m_imgRight.data;
				if (m_nShowDirct == 1)
				{
					pArrow = m_imgLeft.data;
					nOffX = 0;
				}

				for (int j = 0; j < m_imgLeft.cols; j++)
				for (int i = 0; i < m_imgLeft.rows; i++)
				{
					int nMax = MAX(pArrow[(i*m_imgLeft.cols + j) * 3 + 0],
						MAX(pArrow[(i*m_imgLeft.cols + j) * 3 + 1],
							pArrow[(i*m_imgLeft.cols + j) * 3 + 2]));
					if (nMax < 2) continue;
					nMax = MIN(nMax, 0.70 * 255);
					for (int k = 0; k < 3; k++)
						imgM.data[(i*imgM.cols + j + nOffX) * 3 + k] = (imgM.data[(i*imgM.cols + j + nOffX) * 3 + k] * (255 - nMax) + pArrow[(i*m_imgLeft.cols + j) * 3 + k] * nMax) / 255;
				}
			}

			g_mutxImg.Lock();

			img = imread(strFileName);
			m_imgM = imgM.clone();

			strFileN.Replace("_m.bmp", "_s.bmp");
			strFile = format("%s", strFileN);
			m_imgS = imread(strFile);


			m_nRight = m_nLeft + m_imgS.cols;
			m_nBottom = m_nTop + m_imgS.rows;
			g_mutxImg.Unlock();
		}
		else
		{
			img = imread(strFileName);
		}
	}
	else
	{
		if (m_bUpdateZero)
			return;
		m_bUpdateZero = TRUE;
	}

	if (fCurrentV > 0.1)
		m_bUpdateZero = FALSE;

	m_fSuggestVelocity = fSuggestV;
	m_fCurrentVelocity = nInSignSpeed < 0 ? fCurrentV : nInSignSpeed;

	Invalidate(FALSE);
	UpdateWindow();
}

BOOL CDialogLeft::OnEraseBkgnd(CDC* pDC)
{
	//if(m_bMidWnd)
	return FALSE;
	// TODO: Add your message handler code here and/or call default

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CDialogLeft::Terminate()
{
	EndDialog(1);
	//CDialog::OnOK();
}