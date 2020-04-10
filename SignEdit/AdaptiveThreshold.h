#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CAdaptiveThreshold 对话框

class CAdaptiveThreshold : public CDialogEx
{
	DECLARE_DYNAMIC(CAdaptiveThreshold)

public:
	CAdaptiveThreshold(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAdaptiveThreshold();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG11 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboAlg;
	CComboBox m_comboType;
	CSliderCtrl m_sliderRadius;
	CEdit m_editRadius;
	CSliderCtrl m_sliderDev;
	CEdit m_editDev;
	int m_nEditRadius;
	double m_dEditDev;
	int m_nEditAlg;
	int m_nEditType;
	afx_msg void OnKillfocusEdit3();
	afx_msg void OnKillfocusEdit4();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeCombo2();
	afx_msg void OnBnClickedOk();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	bool GetItemValue();
};
