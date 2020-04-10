#pragma once
#include "afxwin.h"


// CHistDlg 对话框

class CHistDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistDlg)

public:
	CHistDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHistDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGHIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboConvType;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedOk();
};
