#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CConerDetector �Ի���

class CConerDetector : public CDialogEx
{
	DECLARE_DYNAMIC(CConerDetector)

public:
	CConerDetector(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConerDetector();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void UpdateDataFromCtrl();		//�ӿؼ���������

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboConvType;
	CSliderCtrl m_slidThreshold;
	CEdit m_editThreshold;
	CSliderCtrl m_slidMaxNum;
	CEdit m_editMaxNum;
	CSliderCtrl m_slidMinDistance;
	CEdit m_editMinDistance;

	//����
	int m_nType;
	double m_dThreshold;
	int m_nMaxNum;
	int m_nMinDistance;
	virtual BOOL OnInitDialog();
	CStatic m_text2;
	CStatic m_text1;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedOk();
};
