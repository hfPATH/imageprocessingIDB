#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CLaplacianDlg 对话框

class CLaplacianDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLaplacianDlg)

public:
	CLaplacianDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLaplacianDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG17 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CBorderTypeMap borderTypeMap;
	CComboBox m_borderTypeComBox;
	CEdit m_kSizeEdit;
	CEdit m_scaleEdit;
	CEdit m_deltaEdit;
	CSliderCtrl m_kSizeSlider;
	CSliderCtrl m_scaleSlider;
	CSliderCtrl m_deltaSlider;
	void GetItemValue();
	afx_msg void OnEnKillfocusKsizeedit();
	afx_msg void OnEnKillfocusScaleedit();
	afx_msg void OnEnKillfocusDeltaedit();
	afx_msg void OnCbnSelchangeBordertypecombo();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	int sobelSize;
	double scale;
	double delta;
	int borderType;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
