#pragma once
#include "afxcmn.h"

#define WM_PROGRESS1 WM_USER+121
// CDlgMosaic dialog

class CDlgMosaic : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMosaic)

public:
	CDlgMosaic(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMosaic();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG19 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg LRESULT OnProgress( WPARAM w, LPARAM l );
	CProgressCtrl m_ctrProgress;
	virtual BOOL OnInitDialog();

	CString m_strSignPath;
	CString m_strOutputPath;
	afx_msg void OnBnClickedOk2();
	double m_dRatio;

	CWinThread* m_pThread;
	afx_msg void OnDestroy();
	int m_nFrames;
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton11();
	int m_nLeftOffX;
	int m_nRightOffX;
	CString m_strLeft;
	CString m_strRight;
	int m_nLeftOffY;
	int m_nRightOffY;
	int m_nMidX;
	int m_nMidY;
};
