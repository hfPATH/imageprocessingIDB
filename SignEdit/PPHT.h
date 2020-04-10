#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CPPHT 对话框

class CPPHT : public CDialogEx
{
	DECLARE_DYNAMIC(CPPHT)

public:
	CPPHT(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPPHT();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG14 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderDistance;
	CEdit m_editDistance;
	CSliderCtrl m_sliderAngle;
	CEdit m_editAngle;
	CSliderCtrl m_sliderThreshold;
	CEdit m_editThreshold;
	CSliderCtrl m_sliderMinLineLength;
	CEdit m_editMinLineLength;
	CSliderCtrl m_sliderMaxLineGap;
	CEdit m_editMaxLineGap;
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKillfocusEdit1();
	afx_msg void OnKillfocusEdit3();
	afx_msg void OnKillfocusEdit4();
	afx_msg void OnKillfocusEdit5();
	afx_msg void OnKillfocusEdit6();
	afx_msg void OnBnClickedOk();
	bool GetItemValue();
	double m_dDistance;
	double m_dAngle;
	int m_nThreshold;
	double m_dMinLineLength;
	double m_dMaxLineGap;
};
