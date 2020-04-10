#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSiftDialog 对话框

class CSiftDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSiftDialog)

public:
	CSiftDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSiftDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG7 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	CSliderCtrl m_slidFeatures;
	CEdit m_editFeatures;
	CSliderCtrl m_slidOctaveLayers;
	CEdit m_editOctaveLayers;
	CSliderCtrl m_slidContrastThreshold;
	CEdit m_editContrastThreshold;
	CSliderCtrl m_slidEdgeThreshold;
	CEdit m_editEdgeThreshold;
	CSliderCtrl m_slidSigma;
	CEdit m_editSigma;
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);


	void UpdateDataFromCtrl();


	DECLARE_MESSAGE_MAP()
public:
	int m_nFeatures;
	int m_nOctaveLayers;
	double m_dContrastThreshold;
	double m_dEdgeThreshold;
	double m_dSigma;
	

};
