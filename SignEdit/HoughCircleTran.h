#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CHoughCircleTran �Ի���

class CHoughCircleTran : public CDialogEx
{
	DECLARE_DYNAMIC(CHoughCircleTran)

public:
	CHoughCircleTran(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHoughCircleTran();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG15 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderResolutionSet;
	CEdit m_editResolutionSet;
	CSliderCtrl m_sliderMinCircleDist;
	CEdit m_editMinCenterDist;
	CSliderCtrl m_sliderHighThres;
	CEdit m_editHighThres;
	CSliderCtrl m_sliderAccuThres;
	CEdit m_editAccuThres;
	double m_dResolutionSet;
	double m_dMinCenterDist;
	double m_dAccuThres;
	double m_dHighThres;
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusEdit1();
	afx_msg void OnKillfocusEdit3();
	afx_msg void OnKillfocusEdit4();
	afx_msg void OnKillfocusEdit5();
	afx_msg void OnBnClickedOk();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	bool GetItemValue();
};
