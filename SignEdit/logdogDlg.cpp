// logdogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "logdogDlg.h"
#include "afxdialogex.h"


// logdogDlg dialog

IMPLEMENT_DYNAMIC(logdogDlg, CDialogEx)

logdogDlg::logdogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG8, pParent)
	, m_nThreshold(0)
	, m_nType(0)
{

}

logdogDlg::~logdogDlg()
{
}

void logdogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ncombpType);
	DDX_Control(pDX, IDC_SLIDER1, m_nSlide);
	DDX_Control(pDX, IDC_EDIT3, m_editThreshold);
}

void logdogDlg::GetItemValue()
{
	m_nType = m_ncombpType.GetCurSel();
	CString strTmp;
	m_editThreshold.GetWindowTextA(strTmp);
	m_nThreshold = atoi(strTmp);
}


BEGIN_MESSAGE_MAP(logdogDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &logdogDlg::OnCbnSelchangeCombo1)
	ON_WM_HSCROLL()
	ON_EN_KILLFOCUS(IDC_EDIT3, &logdogDlg::OnEnKillfocusEdit3)
	ON_EN_CHANGE(IDC_EDIT3, &logdogDlg::OnEnChangeEdit3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &logdogDlg::OnNMCustomdrawSlider1)
	ON_BN_CLICKED(IDOK, &logdogDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &logdogDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// logdogDlg message handlers


void logdogDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
}


void logdogDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CString str;
	str.Format("%d", m_nSlide.GetPos());
	m_editThreshold.SetWindowTextA(str);
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_LOGANDDOG, NULL, NULL);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void logdogDlg::OnEnKillfocusEdit3()
{
	CString strTmp;
	m_editThreshold.GetWindowTextA(strTmp);
	GetItemValue();

	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_LOGANDDOG, NULL, NULL);
	// TODO: Add your control notification handler code here
}


void logdogDlg::OnEnChangeEdit3()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void logdogDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


BOOL logdogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_ncombpType.InsertString(-1, "LoG");
	m_ncombpType.InsertString(-1, "DoG");
	m_ncombpType.SetCurSel(0);
	m_nSlide.SetRange(0, 10);
	m_nSlide.SetPos(1);
	CString str;
	str.Format("%d", m_nSlide.GetPos());
	m_editThreshold.SetWindowTextA(str);
	// TODO:  Add extra initialization here
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_LOGANDDOG, NULL, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void logdogDlg::OnBnClickedOk()
{
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_UDDATE_BITMAP, NULL, NULL);


	CDialogEx::OnOK();
	// TODO: Add your control notification handler code here
	
}


void logdogDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
