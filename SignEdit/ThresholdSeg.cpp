// ThresholdSeg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "ThresholdSeg.h"
#include "afxdialogex.h"


// CThresholdSeg �Ի���

IMPLEMENT_DYNAMIC(CThresholdSeg, CDialogEx)

CThresholdSeg::CThresholdSeg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG10, pParent)
{

	m_nThreshold = 0;
	m_nType = 0;
}

CThresholdSeg::~CThresholdSeg()
{
}

void CThresholdSeg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_sliderThreshold);
	DDX_Control(pDX, IDC_EDIT1, m_editThreshold);
	DDX_Control(pDX, IDC_COMBO1, m_comboType);
}


BEGIN_MESSAGE_MAP(CThresholdSeg, CDialogEx)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CThresholdSeg::OnSelchangeCombo1)
	ON_BN_CLICKED(IDOK, &CThresholdSeg::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CThresholdSeg::OnKillfocusEdit1)
END_MESSAGE_MAP()


// CThresholdSeg ��Ϣ�������


BOOL CThresholdSeg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//��ֵ����
	m_comboType.InsertString(-1, "THRESH_BINARY");
	m_comboType.InsertString(-1, "THRESH_BINARY_INV");
	m_comboType.InsertString(-1, "THRESH_TRUNC");
	m_comboType.InsertString(-1, "THRESH_TOZERO");
	m_comboType.InsertString(-1, "THRESH_TOZERO_INV");	
	//����Ϊ5����ʵ��Ӧ����8
	m_comboType.InsertString(-1, "CV_THRESH_OTSU");

	m_comboType.SetCurSel(0);
	m_sliderThreshold.SetRange(0, 255);
	m_sliderThreshold.SetPos(0);
	m_editThreshold.SetWindowTextA("0");

	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_THRESHOLDSEG, NULL, NULL);	
	}	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CThresholdSeg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString strTmp;
	strTmp.Format("%d", m_sliderThreshold.GetPos());
	m_editThreshold.SetWindowTextA(strTmp);

	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_THRESHOLDSEG, NULL, NULL);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


bool CThresholdSeg::GetItemValue()
{
	m_nType = m_comboType.GetCurSel();

	int nTmp = m_comboType.GetCurSel();
	if (nTmp == 5)
		m_nType = 8;

	CString strTmp;
	m_editThreshold.GetWindowTextA(strTmp);
	m_nThreshold = atoi(strTmp);
	return true;
}


void CThresholdSeg::OnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_THRESHOLDSEG, NULL, NULL);
	}
}


void CThresholdSeg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_UDDATE_BITMAP, NULL, NULL);
	CDialogEx::OnOK();
}


void CThresholdSeg::OnKillfocusEdit1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strTmp;
	m_editThreshold.GetWindowTextA(strTmp);
	m_sliderThreshold.SetPos(atoi(strTmp));

	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_THRESHOLDSEG, NULL, NULL);
	}
}
