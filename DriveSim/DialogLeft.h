#pragma once

#include <Mmsystem.h>
#include <io.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <assert.h>

// CDialogLeft 
#include <opencv2\opencv.hpp>
using namespace cv;
using namespace std;

class CDialogLeft : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogLeft)

public:
	CDialogLeft(CWnd* pParent = NULL);   // 标准构造函数
	CDialogLeft(int leftPos, int topPos, CWnd* pParent = NULL);
	virtual ~CDialogLeft();
	Mat				img;
	Mat				m_imgS;
	Mat				m_imgM;
	Mat				m_imgLeft;
	Mat				m_imgRight;
	int				m_nLeft;
	int				m_nTop;
		int         m_nRight;
	int m_nBottom;
	float			m_fSuggestVelocity;
	float			m_fCurrentVelocity;

	BOOL m_bStop;
	CString m_strText;

	vector<double>  m_pAngleX;
	vector<double>  m_pAngleY;
	vector<double>  m_pAngleZ;
	

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LEFT };
#endif

protected:
	CRect			m_rectFullScrean;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	void Terminate();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg LRESULT	OnMyMessage(WPARAM wParam, LPARAM lParam);
	void UpdateImage(std::string& strFileName, float fSuggestV, float fCurrentV, int bShowDirect = 0, int nX=-1, int nY = -1, int nInSignSpeed=-1);
	CFont font;
	BITMAPINFO* pBitmapInfo;
	BOOL m_bMidWnd;
	BOOL m_bUpdateZero;
	int m_nShowDirct;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
