#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CThresholdSeg �Ի���

class CThresholdSeg : public CDialogEx
{
	DECLARE_DYNAMIC(CThresholdSeg)

public:
	CThresholdSeg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CThresholdSeg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG10 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderThreshold;
	CEdit m_editThreshold;
	CComboBox m_comboType;
	int m_nThreshold;
	int m_nType;
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	bool GetItemValue();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnKillfocusEdit1();
};
