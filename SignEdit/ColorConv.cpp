// ColorConv.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "ColorConv.h"
#include "afxdialogex.h"


// CColorConv �Ի���

IMPLEMENT_DYNAMIC(CColorConv, CDialogEx)

CColorConv::CColorConv(CWnd* pParent )
	: CDialogEx(IDD_DIALOG9, pParent)
{
	m_nConvType = 0;
}

CColorConv::~CColorConv()
{
}

void CColorConv::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboConvType);
}


BEGIN_MESSAGE_MAP(CColorConv, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CColorConv::OnSelchangeCombo1)
	ON_BN_CLICKED(IDOK, &CColorConv::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CColorConv::OnBnClickedButton1)
END_MESSAGE_MAP()


// CColorConv ��Ϣ�������


void CColorConv::OnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_COLORCVT, NULL, NULL);
	}
}


void CColorConv::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_UDDATE_BITMAP, (WPARAM)1, NULL);
	CDialogEx::OnOK();
}


BOOL CColorConv::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_comboConvType.InsertString(-1, "CV_GRAY2RGB"); 
	m_comboConvType.InsertString(-1, "CV_RGB2GRAY"); 
	m_comboConvType.InsertString(-1, "CV_RGB2HSV");
	m_comboConvType.SetCurSel(0);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_COLORCVT, NULL, NULL);		
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


bool CColorConv::GetItemValue()
{
	CString strTmp;
	int nTmp = m_comboConvType.GetCurSel();
	switch (nTmp)
	{
	case 0:
		m_nConvType = 8;
		break;
	case 1:
		m_nConvType = 7;
		break;
	case 2:
		m_nConvType = 41;
		break;
	default:
		break;
	}
	return true;
}


void CColorConv::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
