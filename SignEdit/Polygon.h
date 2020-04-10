#pragma once
#include "afxwin.h"


// CPolygon 对话框

class CPolygon : public CDialogEx
{
	DECLARE_DYNAMIC(CPolygon)

public:
	CPolygon(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPolygon();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void UpdateDataFromCtrl();		//从控件中更新数据

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboMode;
	CComboBox m_comboMethod;
	CComboBox m_comboPolygon;
	virtual BOOL OnInitDialog();

	int m_nMode;
	int m_nMethod;
	int m_nType;
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnCbnSelchangeCombo1();
};
