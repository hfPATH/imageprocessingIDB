#pragma once


// CSobelDlg dialog

#include "opencv2/opencv.hpp"
#include "afxwin.h"
#include "afxcmn.h"

class CSobelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSobelDlg)

public:
	CSobelDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSobelDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOBEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();

	CBorderTypeMap borderTypeMap;
	
	//CEdit m_dxEdit;
	//CEdit m_dyEdit;
	CEdit m_scaleEdit;
	CEdit m_deltaEdit;
	//CComboBox m_kSizeComBox;
	CComboBox m_borderTypeComBox;
	CEdit m_kSizeEdit;
	CButton m_xCheck;
	CButton m_yCheck;
	CSliderCtrl m_kSizeSlider;
	CSliderCtrl m_scaleSlider;
	CSliderCtrl m_deltaSlider;

	int dx;
	int dy;
	int kSize;
	double scale;
	double delta;
	int borderType;
	void GetItemValue();

public:
	afx_msg void OnBnClickedXcheck();
	afx_msg void OnBnClickedYcheck();
	afx_msg void OnEnKillfocusKsizeedit();
	afx_msg void OnEnKillfocusScaleedit();
	afx_msg void OnEnKillfocusDeltaedit();
	afx_msg void OnCbnSelchangeBordertypecombo();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnStnClickedKsizestatic();
};
