#pragma once
#include "afxwin.h"


// CHistDlg �Ի���

class CHistDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistDlg)

public:
	CHistDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHistDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGHIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboConvType;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedOk();
};
