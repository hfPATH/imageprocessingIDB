// RobertDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "RobertDlg.h"
#include "afxdialogex.h"


// CRobertDlg dialog

IMPLEMENT_DYNAMIC(CRobertDlg, CDialogEx)

CRobertDlg::CRobertDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ROBERT, pParent)
{

}

CRobertDlg::~CRobertDlg()
{
}

void CRobertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_XCHECK, m_xCheck);
	DDX_Control(pDX, IDC_YCHECK, m_yCheck);
}


BEGIN_MESSAGE_MAP(CRobertDlg, CDialogEx)
	ON_BN_CLICKED(IDC_XCHECK, &CRobertDlg::OnBnClickedXcheck)
	ON_BN_CLICKED(IDC_YCHECK, &CRobertDlg::OnBnClickedYcheck)
	ON_BN_CLICKED(IDOK, &CRobertDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRobertDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CRobertDlg message handlers


BOOL CRobertDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_xCheck.SetCheck(1);
	m_yCheck.SetCheck(1);
	// TODO:  Add extra initialization here

	UpdateData(FALSE);
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_ROBERT, NULL, NULL);
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CRobertDlg::OnBnClickedXcheck()
{
	UpdateData(TRUE);
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_ROBERT, NULL, NULL);
	UpdateData(FALSE);
	Invalidate(FALSE);
	UpdateWindow();
}


void CRobertDlg::OnBnClickedYcheck()
{
	UpdateData(TRUE);
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_ROBERT, NULL, NULL);
	UpdateData(FALSE);
	Invalidate(FALSE);
	UpdateWindow();
}


void CRobertDlg::OnBnClickedOk()
{
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_UDDATE_BITMAP, NULL, NULL);
	CDialogEx::OnOK();
}


void CRobertDlg::OnBnClickedCancel()
{
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_CANCELOPERA, NULL, NULL);
	CDialogEx::OnCancel();
}
