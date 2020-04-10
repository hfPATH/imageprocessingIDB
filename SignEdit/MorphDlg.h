#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// CMorphDlg �Ի���

class CMorphDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMorphDlg)

public:
	CMorphDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMorphDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	void GetItemValue();	//��ȡ�ؼ���ֵ

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_editThreshold;
	CComboBox m_comboShape;
	CComboBox m_comboConvType;
	CSliderCtrl m_sliderThreshold;

	int m_nThreshold;
	int m_nShape;
	int m_nType;


	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnKillfocusEdit1();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo2();
};
