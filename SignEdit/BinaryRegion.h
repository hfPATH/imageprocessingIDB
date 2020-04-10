#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CBinaryRegion �Ի���

class CBinaryRegion : public CDialogEx
{
	DECLARE_DYNAMIC(CBinaryRegion)

public:
	CBinaryRegion(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBinaryRegion();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void UpdateValueFromCtrl();			//�ӿռ���²���
	DECLARE_MESSAGE_MAP()
public:
	
	CButton m_checkArea;
	CButton m_checkWidth;
	CButton m_checkHeight;
	CButton m_checkCircle;
	CSliderCtrl m_slidArea;
	CSliderCtrl m_slidWidth;
	CSliderCtrl m_slidHeight;
	CSliderCtrl m_slidCircle;
	CEdit m_editArea;
	CEdit m_editWidth;
	CEdit m_editHeight;
	CEdit m_editCircle;
	CComboBox m_comboMode;
	CComboBox m_comboMethod;
	virtual BOOL OnInitDialog();

	//ɸѡ�Ĳ�����-1����ʹ�ø���ɸѡ
	double m_nidth;
	double m_dHeight;
	double m_dArea;
	double m_dCircle;
	double m_dDirect;
	int m_nMode;
	int m_nMethod;

	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnBnClickedOk();
};
