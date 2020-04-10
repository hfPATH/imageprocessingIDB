#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CIterativeThreshold �Ի���

class CIterativeThreshold : public CDialogEx
{
	DECLARE_DYNAMIC(CIterativeThreshold)

public:
	CIterativeThreshold(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CIterativeThreshold();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG12 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderThreshold;
	CEdit m_editThreshold;
	int m_nEditThreshold;
	afx_msg void OnBnClickedOk();
	afx_msg void OnKillfocusEdit3();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();
	bool GetItemValue();
};
