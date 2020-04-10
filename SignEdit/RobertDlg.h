#pragma once
#include "afxwin.h"


// CRobertDlg dialog

class CRobertDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRobertDlg)

public:
	CRobertDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRobertDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROBERT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton m_xCheck;
	CButton m_yCheck;
	afx_msg void OnBnClickedXcheck();
	afx_msg void OnBnClickedYcheck();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
