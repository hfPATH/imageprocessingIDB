// DlgFtrTrack.cpp : implementation file
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "DlgFtrTrack.h"
#include "afxdialogex.h"


// CDlgFtrTrack dialog

IMPLEMENT_DYNAMIC(CDlgFtrTrack, CDialogEx)

CDlgFtrTrack::CDlgFtrTrack(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLGTRACK, pParent)
	, m_nMaxNFeature( 0 )
	, m_dQuality( 0 )
	, m_dDistance( 0 )
{
	m_pParentWnd = pParent;
}

CDlgFtrTrack::~CDlgFtrTrack()
{
}

void CDlgFtrTrack::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Text( pDX, IDC_EDIT3, m_nMaxNFeature );
	DDV_MinMaxInt( pDX, m_nMaxNFeature, 1, 99999999 );
	DDX_Text( pDX, IDC_EDIT10, m_dQuality );
	DDX_Text( pDX, IDC_EDIT15, m_dDistance );
	DDV_MinMaxDouble(pDX, m_dDistance, 1, 20);
}


BEGIN_MESSAGE_MAP(CDlgFtrTrack, CDialogEx)
	ON_BN_CLICKED( IDC_BUTTON1, &CDlgFtrTrack::OnBnClickedButton1 )
	ON_BN_CLICKED( IDOK, &CDlgFtrTrack::OnBnClickedOk )
END_MESSAGE_MAP()


// CDlgFtrTrack message handlers


void CDlgFtrTrack::OnBnClickedButton1()
{
	::SendMessageA( m_pParentWnd->GetSafeHwnd(), WM_FEATURETRACK, NULL, NULL );
}


void CDlgFtrTrack::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
