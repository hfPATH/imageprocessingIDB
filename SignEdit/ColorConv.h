#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CColorConv 对话框

class CColorConv : public CDialogEx
{
	DECLARE_DYNAMIC(CColorConv)

public:
	CColorConv(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CColorConv();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG9 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	bool GetItemValue();
	int m_nConvType;
	CComboBox m_comboConvType;
	afx_msg void OnBnClickedButton1();
};
