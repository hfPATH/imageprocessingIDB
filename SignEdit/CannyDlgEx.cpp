// CannyDlgEx.cpp : implementation file
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "CannyDlgEx.h"
#include "afxdialogex.h"


// CCannyDlgEx dialog

IMPLEMENT_DYNAMIC(CCannyDlgEx, CDialogEx)

CCannyDlgEx::CCannyDlgEx(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG16, pParent)
{

}

CCannyDlgEx::~CCannyDlgEx()
{
}

void CCannyDlgEx::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK, m_L2gradientCheck);
	DDX_Control(pDX, IDC_EDIT1, m_threEdit1);
	DDX_Control(pDX, IDC_EDIT2, m_threEdit2);
	DDX_Control(pDX, IDC_EDIT3, m_sobelSizeEdit);
	DDX_Control(pDX, IDC_THRESLIDER1, m_threSlider1);
	DDX_Control(pDX, IDC_THRESLIDER2, m_threSlider2);
	DDX_Control(pDX, IDC_SLIDER2, m_sobelSizeSlider);
}


BEGIN_MESSAGE_MAP(CCannyDlgEx, CDialogEx)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CCannyDlgEx::OnEnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CCannyDlgEx::OnEnKillfocusEdit2)
	ON_WM_HSCROLL()
	ON_EN_KILLFOCUS(IDC_EDIT3, &CCannyDlgEx::OnEnKillfocusEdit3)
	ON_BN_CLICKED(IDC_CHECK1, &CCannyDlgEx::OnBnClickedCheck1)
	ON_BN_CLICKED(IDOK, &CCannyDlgEx::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCannyDlgEx::OnBnClickedCancel)
END_MESSAGE_MAP()


// CCannyDlgEx message handlers


BOOL CCannyDlgEx::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_L2gradientCheck.SetCheck(0);
	m_threEdit1.SetWindowTextA(_T("0"));
	m_threEdit2.SetWindowTextA(_T("0"));

	m_sobelSizeEdit.SetWindowTextA(_T("1"));

	m_threSlider1.SetRange(0, 256 * 100);
	m_threSlider1.SetPos(0);
	m_threSlider2.SetRange(0, 256 * 100);
	m_threSlider2.SetPos(0);
	m_sobelSizeSlider.SetRange(0, 3);
	m_sobelSizeSlider.SetPos(1);

	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_OPENCVCANNY, NULL, NULL);

	return TRUE;
}

void CCannyDlgEx::GetItemValue()
{
	CString strTmp;
	int posTmp;
	posTmp = m_threSlider1.GetPos();
	threshold1 = double(posTmp) / 100;
	strTmp.Format("%.2f", threshold1);
	m_threEdit1.SetWindowTextA(strTmp);

	posTmp = m_threSlider2.GetPos();
	threshold2 = double(posTmp) / 100;
	strTmp.Format("%.2f", threshold2);
	m_threEdit2.SetWindowTextA(strTmp);
	
	posTmp = m_sobelSizeSlider.GetPos();
	sobelSize = posTmp;
	strTmp.Format("%d", sobelSize);
	m_sobelSizeEdit.SetWindowTextA(strTmp);
	
	L2gradient = m_L2gradientCheck.GetCheck();
}

void CCannyDlgEx::OnEnKillfocusEdit1()
{
	CString strTmp;
	m_threEdit1.GetWindowTextA(strTmp);
	double threshold1Tmp = atof(strTmp);
	int threshold1 = (int)(threshold1Tmp * 100);
	m_threSlider1.SetPos(threshold1);
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_OPENCVCANNY, NULL, NULL);
}


void CCannyDlgEx::OnEnKillfocusEdit2()
{
	CString strTmp;
	m_threEdit2.GetWindowTextA(strTmp);
	double threshold2Tmp = atof(strTmp);
	int threshold2 = (int)(threshold2Tmp * 100);
	m_threSlider2.SetPos(threshold2);
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_OPENCVCANNY, NULL, NULL);
}


void CCannyDlgEx::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	double threshold1Tmp = (double)m_threSlider1.GetPos() / 100;
	double threshold2Tmp = (double)m_threSlider2.GetPos() / 100;
	int sobelSizeTmp = m_sobelSizeSlider.GetPos();
	CString strTmp;
	strTmp.Format("%.2f", threshold1Tmp);
	m_threEdit1.SetWindowTextA(strTmp);
	strTmp.Format("%.2f", threshold2Tmp);
	m_threEdit2.SetWindowTextA(strTmp);
	strTmp.Format("%d", sobelSizeTmp);
	m_sobelSizeEdit.SetWindowTextA(strTmp);

	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_OPENCVCANNY, NULL, NULL);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CCannyDlgEx::OnEnKillfocusEdit3()
{
	CString strTmp;
	m_sobelSizeEdit.GetWindowTextA(strTmp);
	int sobelSizeTmp = atoi(strTmp);
	m_sobelSizeSlider.SetPos(sobelSizeTmp);

	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_OPENCVCANNY, NULL, NULL);
}


void CCannyDlgEx::OnBnClickedCheck1()
{
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_OPENCVCANNY, NULL, NULL);
}


void CCannyDlgEx::OnBnClickedOk()
{
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_UDDATE_BITMAP, NULL, NULL);
	CDialogEx::OnOK();
}


void CCannyDlgEx::OnBnClickedCancel()
{
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_CANCELOPERA, NULL, NULL);
	CDialogEx::OnCancel();
}
