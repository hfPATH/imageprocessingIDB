#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// logdogDlg dialog

class logdogDlg : public CDialogEx
{
	DECLARE_DYNAMIC(logdogDlg)

public:
	logdogDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~logdogDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG8 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void GetItemValue();
	DECLARE_MESSAGE_MAP()

public:
	int m_nThreshold;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnKillfocusEdit3();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	CComboBox m_ncombpType;
	CSliderCtrl m_nSlide;
	CEdit m_editThreshold;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	int m_nType;
};
