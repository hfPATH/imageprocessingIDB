#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "opencv2/opencv.hpp"


// CGaborDlg 对话框

class CGaborDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGaborDlg)

public:
	CGaborDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGaborDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG18 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CEdit m_kernelSizeXEdit;
	CEdit m_kernelSizeYEdit;
	CEdit m_anchorXEdit;
	CEdit m_anchorYEdit;
	CEdit m_sigmaEdit;
	CSliderCtrl m_sigmaSlider;
	CEdit m_thetaEdit;
	CSliderCtrl m_thetaSlider;
	CEdit m_lambdaEdit;
	CSliderCtrl m_lambdaSlider;
	CEdit m_gammaEdit;
	CSliderCtrl m_gammaSlider;
	CEdit m_phaseOffsetEdit;
	CSliderCtrl m_phaseOffsetSlider;
	CEdit m_filterDeltaEdit;
	CSliderCtrl m_filterDeltaSlider;
	CComboBox m_borderTypeComBox;

	CBorderTypeMap borderTypeMap;

public:
	cv::Size m_kernelSize;
	cv::Point m_anchorPoint;
	double m_sigma;
	double m_theta;
	double m_lambda;
	double m_gamma;
	double m_phaseOffset;
	double m_filterDelta;
	int m_borderType;

	void GetValueItem();
	CButton m_centerChangeCheck;
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnEnKillfocusEdit6();
	afx_msg void OnEnKillfocusEdit7();
	afx_msg void OnEnKillfocusEdit8();
	afx_msg void OnEnKillfocusEdit9();
	afx_msg void OnEnKillfocusEdit1();
	afx_msg void OnEnKillfocusEdit2();
	afx_msg void OnEnKillfocusEdit3();
	afx_msg void OnEnKillfocusEdit4();
	afx_msg void OnEnKillfocusEdit5();
	afx_msg void OnEnKillfocusEdit11();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
