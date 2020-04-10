// SobelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "SobelDlg.h"
#include "afxdialogex.h"


// CSobelDlg dialog

IMPLEMENT_DYNAMIC(CSobelDlg, CDialogEx)

CSobelDlg::CSobelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SOBEL, pParent)
{

}

CSobelDlg::~CSobelDlg()
{
}

void CSobelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_KSIZECOMBO1, m_kSizeComBox);
	//DDX_Control(pDX, IDC_DXEDIT, m_dxEdit);
	//DDX_Control(pDX, IDC_DYEDIT, m_dyEdit);
	DDX_Control(pDX, IDC_SCALEEDIT, m_scaleEdit);
	DDX_Control(pDX, IDC_DELTAEDIT, m_deltaEdit);
	DDX_Control(pDX, IDC_BORDERTYPECOMBO, m_borderTypeComBox);
	DDX_Control(pDX, IDC_KSIZEEDIT, m_kSizeEdit);
	DDX_Control(pDX, IDC_XCHECK, m_xCheck);
	DDX_Control(pDX, IDC_YCHECK, m_yCheck);
	DDX_Control(pDX, IDC_SLIDER1, m_kSizeSlider);
	DDX_Control(pDX, IDC_SLIDER2, m_scaleSlider);
	DDX_Control(pDX, IDC_SLIDER3, m_deltaSlider);
}

BOOL CSobelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	borderTypeMap.insert(CBorderTypeMap::value_type("BORDER_CONSTANT", 0));
	borderTypeMap.insert(CBorderTypeMap::value_type("BORDER_REPLICATE", 1));
	borderTypeMap.insert(CBorderTypeMap::value_type("BORDER_REFLECT", 2));
	borderTypeMap.insert(CBorderTypeMap::value_type("BORDER_WRAP", 3));
	borderTypeMap.insert(CBorderTypeMap::value_type("BORDER_REFLECT_101", 4));
	borderTypeMap.insert(CBorderTypeMap::value_type("BORDER_TRANSPARENT", 5));
	borderTypeMap.insert(CBorderTypeMap::value_type("BORDER_REFLECT101", 4));
	borderTypeMap.insert(CBorderTypeMap::value_type("BORDER_DEFAULT", 4));
	borderTypeMap.insert(CBorderTypeMap::value_type("BORDER_ISOLATED", 16));

	m_borderTypeComBox.InsertString(-1, "BORDER_CONSTANT");
	m_borderTypeComBox.InsertString(-1, "BORDER_REPLICATE");
	m_borderTypeComBox.InsertString(-1, "BORDER_REFLECT");
	m_borderTypeComBox.InsertString(-1, "BORDER_WRAP");
	m_borderTypeComBox.InsertString(-1, "BORDER_REFLECT_101");
	m_borderTypeComBox.InsertString(-1, "BORDER_TRANSPARENT");
	m_borderTypeComBox.InsertString(-1, "BORDER_REFLECT101");
	m_borderTypeComBox.InsertString(-1, "BORDER_DEFAULT");
	m_borderTypeComBox.InsertString(-1, "BORDER_ISOLATED");
	m_borderTypeComBox.SetCurSel(7);

	m_kSizeEdit.SetWindowTextA(_T("1"));
	m_kSizeSlider.SetRange(0, 100);
	m_kSizeSlider.SetPos(1);
	m_scaleEdit.SetWindowTextA(_T("1.0"));
	m_scaleSlider.SetRange(0, 20000);
	m_scaleSlider.SetPos(100);
	m_deltaEdit.SetWindowTextA(_T("0.0"));
	m_deltaSlider.SetRange(0, 20000);
	m_deltaSlider.SetPos(0);

	m_xCheck.SetCheck(1);
	m_yCheck.SetCheck(1);

	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_OPENCVSOBEL, NULL, NULL);
	return TRUE;
}


BEGIN_MESSAGE_MAP(CSobelDlg, CDialogEx)
	ON_BN_CLICKED(IDC_XCHECK, &CSobelDlg::OnBnClickedXcheck)
	ON_BN_CLICKED(IDC_YCHECK, &CSobelDlg::OnBnClickedYcheck)
	ON_EN_KILLFOCUS(IDC_KSIZEEDIT, &CSobelDlg::OnEnKillfocusKsizeedit)
	ON_EN_KILLFOCUS(IDC_SCALEEDIT, &CSobelDlg::OnEnKillfocusScaleedit)
	ON_EN_KILLFOCUS(IDC_DELTAEDIT, &CSobelDlg::OnEnKillfocusDeltaedit)
	ON_CBN_SELCHANGE(IDC_BORDERTYPECOMBO, &CSobelDlg::OnCbnSelchangeBordertypecombo)
	ON_BN_CLICKED(IDOK, &CSobelDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSobelDlg::OnBnClickedCancel)
	ON_WM_HSCROLL()
	ON_STN_CLICKED(IDC_KSIZESTATIC, &CSobelDlg::OnStnClickedKsizestatic)
END_MESSAGE_MAP()

void CSobelDlg::GetItemValue()
{
	int posTmp;
	CString strTmp;
	dx = m_xCheck.GetCheck();
	dy = m_yCheck.GetCheck();
	posTmp = m_kSizeSlider.GetPos();
	kSize = 2 * posTmp + 1;
	strTmp.Format("%d", posTmp);
	m_kSizeEdit.SetWindowTextA(strTmp);
	posTmp = m_scaleSlider.GetPos();
	scale = double(posTmp) / 100;
	strTmp.Format("%.2f", scale);
	m_scaleEdit.SetWindowTextA(strTmp);
	posTmp = m_deltaSlider.GetPos();
	delta = double(posTmp) / 100;
	strTmp.Format("%.2f", delta);
	m_deltaEdit.SetWindowTextA(strTmp);

	m_borderTypeComBox.GetLBText(m_borderTypeComBox.GetCurSel(), strTmp);
	CBorderTypeMap::iterator it = borderTypeMap.find(strTmp);
	if (it == borderTypeMap.end())
	{
		MessageBox("没有找到相应的边界填充类型！", MB_OK);
		return;
	}
	else
	{
		borderType = it->second;
	}
}

void CSobelDlg::OnBnClickedXcheck()
{
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_OPENCVSOBEL, NULL, NULL);
}

void CSobelDlg::OnBnClickedYcheck()
{
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_OPENCVSOBEL, NULL, NULL);
}

void CSobelDlg::OnEnKillfocusKsizeedit()
{
	CString strTmp;
	m_kSizeEdit.GetWindowTextA(strTmp);
	int posTmp = atoi(strTmp);
	m_kSizeSlider.SetPos(posTmp);
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_OPENCVSOBEL, NULL, NULL);
}

void CSobelDlg::OnEnKillfocusScaleedit()
{
	CString strTmp;
	m_scaleEdit.GetWindowTextA(strTmp);
	int posTmp = atof(strTmp) * 100;
	m_scaleSlider.SetPos(posTmp);
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_OPENCVSOBEL, NULL, NULL);
}

void CSobelDlg::OnEnKillfocusDeltaedit()
{
	CString strTmp;
	m_deltaEdit.GetWindowTextA(strTmp);
	int posTmp = atof(strTmp) * 100;
	m_deltaSlider.SetPos(posTmp);
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_OPENCVSOBEL, NULL, NULL);
}

void CSobelDlg::OnCbnSelchangeBordertypecombo()
{
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_OPENCVSOBEL, NULL, NULL);
}

void CSobelDlg::OnBnClickedOk()
{
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_UDDATE_BITMAP, NULL, NULL);
	CDialogEx::OnOK();
}

void CSobelDlg::OnBnClickedCancel()
{
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_CANCELOPERA, NULL, NULL);
	CDialogEx::OnCancel();
}


void CSobelDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int posTmp;
	CString strTmp;
	posTmp = m_kSizeSlider.GetPos();
	kSize = 2 * posTmp + 1;
	strTmp.Format("%d", posTmp);
	m_kSizeEdit.SetWindowTextA(strTmp);
	posTmp = m_scaleSlider.GetPos();
	scale = double(posTmp) / 100;
	strTmp.Format("%.2f", scale);
	m_scaleEdit.SetWindowTextA(strTmp);
	posTmp = m_deltaSlider.GetPos();
	delta = double(posTmp) / 100;
	strTmp.Format("%.2f", delta);
	m_deltaEdit.SetWindowTextA(strTmp);

	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_OPENCVSOBEL, NULL, NULL);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CSobelDlg::OnStnClickedKsizestatic()
{
	// TODO: 在此添加控件通知处理程序代码
}
