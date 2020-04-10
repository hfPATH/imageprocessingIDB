#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMSER 对话框

class CMSER : public CDialogEx
{
	DECLARE_DYNAMIC(CMSER)

public:
	CMSER(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMSER();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void UpdateDataFromCtrl();

	CSliderCtrl m_slidDelta;
	CEdit m_editDelta;
	CSliderCtrl m_slidMinArea;
	CEdit m_editMinArea;
	CSliderCtrl m_slidMaxArea;
	CEdit m_editMaxArea;
	CSliderCtrl m_slidMaxVariation;
	CEdit m_editMaxVariation;
	CSliderCtrl m_slidMinDiversity;
	CEdit m_editMinDiversity;
	CSliderCtrl m_slidMaxEvolution;
	CEdit m_editMaxEvolution;
	CSliderCtrl m_slidAreaThreshold;
	CEdit m_editAreaThreshold;
	CSliderCtrl m_slidMinMargin;
	CEdit m_editMinMargin;
	CSliderCtrl m_slidEdgeBlurSize;
	CEdit m_editEdgeBlurSize;

	DECLARE_MESSAGE_MAP()
public:
	int	    m_nDelta;
	int		m_nMinArea;
	int		m_nMaxArea;
	double  m_dMaxVariation;
	double  m_dMinDiversity;
	int		m_nMaxEvolution;
	double	m_dAreaThreshold;
	double	m_dMinMargin;
	int		m_nEdgeBlurSize;
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
