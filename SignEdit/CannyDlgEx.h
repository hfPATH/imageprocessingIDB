#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CCannyDlgEx dialog

class CCannyDlgEx : public CDialogEx
{
	DECLARE_DYNAMIC(CCannyDlgEx)

public:
	CCannyDlgEx(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCannyDlgEx();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG16 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton m_L2gradientCheck;
	CEdit m_threEdit1;
	CEdit m_threEdit2;
	CEdit m_sobelSizeEdit;
	CSliderCtrl m_threSlider1;
	CSliderCtrl m_threSlider2;
	CSliderCtrl m_sobelSizeSlider;

	double threshold1;
	double threshold2;
	int sobelSize;
	bool L2gradient;
	void GetItemValue();
	
	afx_msg void OnEnKillfocusEdit1();
	afx_msg void OnEnKillfocusEdit2();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnKillfocusEdit3();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
};
