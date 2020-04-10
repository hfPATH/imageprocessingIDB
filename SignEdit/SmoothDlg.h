#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// SmoothDlg dialog

class SmoothDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SmoothDlg)

public:
	SmoothDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~SmoothDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Smooth };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void GetItemValue();	//获取控件的值
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CComboBox m_comboShape;
	CEdit m_editThreshold;

	int m_nThreshold;
	int m_nType;

	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnKillfocusEdit1();
	//afx_msg void OnBnClickedRadio1();
	CEdit m_editSigmaSpace;
	afx_msg void OnEnChangeEdit3();
	CEdit m_peditsigmaColor;
	int m_nsigmaColor;
	int m_nsigmaSpace;
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnKillfocusEdit4();
	afx_msg void OnEnKillfocusEdit3();
	CSliderCtrl m_sliderThreshold;
	afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnEnAlignRtlEcEdit2();
	int Q;
	afx_msg void OnEnHscrollEdit2();
	afx_msg void OnEnChangeEdit2();
	CEdit m_editQ;
	afx_msg void OnEnKillfocusEdit2();
	afx_msg void OnEnChange1010();
	afx_msg void OnNMCustomdraw1015(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStnClickedStatic1();
};
