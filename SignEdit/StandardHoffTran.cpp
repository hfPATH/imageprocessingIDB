// StandardHoffTran.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "StandardHoffTran.h"
#include "afxdialogex.h"


// CStandardHoffTran �Ի���

IMPLEMENT_DYNAMIC(CStandardHoffTran, CDialogEx)

CStandardHoffTran::CStandardHoffTran(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG13, pParent)
{

	m_dDistance = 0.0;
	m_dAngle = 0.0;
	m_nThreshold = 0;
}

CStandardHoffTran::~CStandardHoffTran()
{
}

void CStandardHoffTran::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_sliderDistance);
	DDX_Control(pDX, IDC_EDIT1, m_editDistance);
	DDX_Control(pDX, IDC_SLIDER2, m_sliderAngle);
	DDX_Control(pDX, IDC_EDIT3, m_editAngle);
	DDX_Control(pDX, IDC_SLIDER3, m_sliderThreshold);
	DDX_Control(pDX, IDC_EDIT4, m_editThreshold);
}


BEGIN_MESSAGE_MAP(CStandardHoffTran, CDialogEx)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CStandardHoffTran::OnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT3, &CStandardHoffTran::OnKillfocusEdit3)
	ON_EN_KILLFOCUS(IDC_EDIT4, &CStandardHoffTran::OnKillfocusEdit4)
	ON_BN_CLICKED(IDOK, &CStandardHoffTran::OnBnClickedOk)
	ON_WM_HSCROLL()
//	ON_BN_CLICKED(IDCANCEL, &CStandardHoffTran::OnBnClickedCancel)
END_MESSAGE_MAP()


// CStandardHoffTran ��Ϣ�������


BOOL CStandardHoffTran::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_sliderDistance.SetRange(1 * 100, 100 * 100);
	m_sliderDistance.SetPos(0);
	m_editDistance.SetWindowTextA("1.00");

	m_sliderAngle.SetRange(1 * 100, 90 * 100);
	m_sliderAngle.SetPos(0);
	m_editAngle.SetWindowTextA("1.00");

	m_sliderThreshold.SetRange(0, 255);
	m_sliderThreshold.SetPos(100);
	m_editThreshold.SetWindowTextA("100");
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_STANDARDHOFFTRAN, NULL, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CStandardHoffTran::OnKillfocusEdit1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strTmp;

	m_editDistance.GetWindowTextA(strTmp);
	m_sliderDistance.SetPos(atoi(strTmp) * 100);

	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_STANDARDHOFFTRAN, NULL, NULL);
	}
}


void CStandardHoffTran::OnKillfocusEdit3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strTmp;

	m_editAngle.GetWindowTextA(strTmp);
	m_sliderAngle.SetPos(atoi(strTmp) * 100);

	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_STANDARDHOFFTRAN, NULL, NULL);
	}
}


void CStandardHoffTran::OnKillfocusEdit4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strTmp;

	m_editThreshold.GetWindowTextA(strTmp);
	m_sliderThreshold.SetPos(atoi(strTmp));

	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_STANDARDHOFFTRAN, NULL, NULL);
	}
}


void CStandardHoffTran::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_UDDATE_BITMAP, NULL, NULL);
	CDialogEx::OnOK();
}


void CStandardHoffTran::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString strTmp;

	double dDistanceTmp = double(m_sliderDistance.GetPos() / 100);
	m_dDistance = dDistanceTmp;
	strTmp.Format("%.2f", m_dDistance);
	m_editDistance.SetWindowTextA(strTmp);

	double dAngleTmp = double(m_sliderAngle.GetPos() / 100);
	m_dAngle = dAngleTmp;
	strTmp.Format("%.2f", m_dAngle);
	m_editAngle.SetWindowTextA(strTmp);

	//int nThresholdTmp = int(m_sliderThreshold.GetPos());
	//nThreshold = nThresholdTmp;
	strTmp.Format("%d", m_sliderThreshold.GetPos());
	m_editThreshold.SetWindowTextA(strTmp);
	//m_sliderThreshold.SetPos(nThreshold);

	/*::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_STANDARDHOFFTRAN, NULL, NULL);*/
	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_STANDARDHOFFTRAN, NULL, NULL);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


bool CStandardHoffTran::GetItemValue()
{
	CString strTmp;

	m_editDistance.GetWindowTextA(strTmp);
	m_dDistance = atof(strTmp);

	m_editAngle.GetWindowTextA(strTmp);
	m_dAngle = atof(strTmp);

	m_editThreshold.GetWindowTextA(strTmp);
	m_nThreshold = atoi(strTmp);
	return true;
}

