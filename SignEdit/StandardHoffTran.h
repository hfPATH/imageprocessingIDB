#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CStandardHoffTran �Ի���

class CStandardHoffTran : public CDialogEx
{
	DECLARE_DYNAMIC(CStandardHoffTran)

public:
	CStandardHoffTran(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStandardHoffTran();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG13 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderDistance;
	CEdit m_editDistance;
	CSliderCtrl m_sliderAngle;
	CEdit m_editAngle;
	CSliderCtrl m_sliderThreshold;
	CEdit m_editThreshold;
	double m_dDistance;
	double m_dAngle;
	int m_nThreshold;
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusEdit1();
	afx_msg void OnKillfocusEdit3();
	afx_msg void OnKillfocusEdit4();
	afx_msg void OnBnClickedOk();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	bool GetItemValue();
//	afx_msg void OnBnClickedCancel();
};
