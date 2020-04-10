// DlgMosaic.cpp : implementation file
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "DlgMosaic.h"
#include "afxdialogex.h"
#include "ImgPrcView.h"



// CDlgMosaic dialog

IMPLEMENT_DYNAMIC(CDlgMosaic, CDialogEx)

CDlgMosaic::CDlgMosaic(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG19, pParent)
	, m_strSignPath( _T( "d:\\sign\\24_2.bmp" ) )
	, m_strOutputPath( _T( "I:\\24" ) )
	, m_dRatio( 1 )
	, m_nFrames( 15 )
	, m_nLeftOffX( 780 )
	, m_nRightOffX( 80 )
	, m_strLeft( _T( "g:\\SIM\\24\\l" ) )
	, m_strRight( _T( "g:\\SIM\\24\\r" ) )
	, m_nLeftOffY( 135 )
	, m_nRightOffY( 135 )
	, m_nMidX( 480 )
	, m_nMidY( 135 )
{
	m_pThread = NULL;
}

CDlgMosaic::~CDlgMosaic()
{
}

void CDlgMosaic::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange( pDX );
	DDX_Control( pDX, IDC_PROGRESS1, m_ctrProgress );
	DDX_Text( pDX, IDC_EDIT1, m_strSignPath );
	DDX_Text( pDX, IDC_EDIT2, m_strOutputPath );
	DDX_Text( pDX, IDC_EDIT16, m_dRatio );
	DDV_MinMaxDouble( pDX, m_dRatio, 0.001, 100 );
	DDX_Text( pDX, IDC_EDIT17, m_nFrames );
	DDV_MinMaxInt( pDX, m_nFrames, 0, 1000 );
	DDX_Text( pDX, IDC_EDIT_LEFT2, m_nLeftOffX );
	DDX_Text( pDX, IDC_EDIT_LEFT3, m_nRightOffX );
	DDX_Text( pDX, IDC_EDIT_LEFT, m_strLeft );
	DDX_Text( pDX, IDC_EDIT_RIGHT, m_strRight );
	DDX_Text( pDX, IDC_EDIT_LEFT4, m_nLeftOffY );
	DDX_Text( pDX, IDC_EDIT_LEFT5, m_nRightOffY );
	DDX_Text( pDX, IDC_EDIT_LEFT6, m_nMidX );
	DDX_Text( pDX, IDC_EDIT_LEFT7, m_nMidY );
}


BEGIN_MESSAGE_MAP(CDlgMosaic, CDialogEx)
	ON_BN_CLICKED( IDC_BUTTON2, &CDlgMosaic::OnBnClickedButton2 )
	ON_BN_CLICKED( IDC_BUTTON1, &CDlgMosaic::OnBnClickedButton1 )

	ON_MESSAGE( WM_PROGRESS1, &CDlgMosaic::OnProgress )

	ON_BN_CLICKED( IDOK2, &CDlgMosaic::OnBnClickedOk2 )
	ON_WM_DESTROY()
	ON_BN_CLICKED( IDC_BUTTON12, &CDlgMosaic::OnBnClickedButton12 )
	ON_BN_CLICKED( IDC_BUTTON11, &CDlgMosaic::OnBnClickedButton11 )
END_MESSAGE_MAP()

// CDlgMosaic message handlers
int CALLBACK BrowseCallBackFun( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
{
	switch ( uMsg )
	{
	case BFFM_INITIALIZED:  
		::SendMessage( hwnd, BFFM_SETSELECTION, TRUE, lpData );
		::SendMessage( hwnd, BFFM_SETSTATUSTEXT, 0, lpData );
		//::SetWindowText( hwnd, TEXT( "Set output path" ) );
		break;
	case BFFM_SELCHANGED:
	{
		TCHAR pszPath[MAX_PATH];
		SHGetPathFromIDList( (LPCITEMIDLIST) lParam, pszPath );
		::SendMessage( hwnd, BFFM_SETSTATUSTEXT, TRUE, (LPARAM) pszPath );
	}
	break;
	}
	return 0;
}


LRESULT CDlgMosaic::OnProgress( WPARAM w, LPARAM l )
{
	m_ctrProgress.SetPos( (int) ( w ) );
	return S_OK;
}

void CDlgMosaic::OnBnClickedButton2()
{
	UpdateData( TRUE );
	TCHAR pszPath[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = this->GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = TEXT( "Set output path" );
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_USENEWUI;
	bi.lpfn = BrowseCallBackFun; 
	bi.lParam = (LPARAM) LPCTSTR( m_strOutputPath);
	bi.iImage = 0;

	LPITEMIDLIST pidl = SHBrowseForFolder( &bi );
	if ( pidl == NULL )
		return;

	if ( !SHGetPathFromIDList( pidl, pszPath ) )
		return;
	
	m_strOutputPath = pszPath;
	UpdateData( FALSE );
}


void CDlgMosaic::OnBnClickedButton1()
{
	UpdateData( TRUE );
	static char szFilter[] = "|PNG文件(*.png)|*.png|BMP文件(*.bmp)|*.bmp||";
	CFileDialog dlg( TRUE, "*.png|*.bmp", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter );
	dlg.m_ofn.lpstrInitialDir = m_strSignPath;

	if ( dlg.DoModal() != IDOK ) return;

	m_strSignPath = dlg.GetPathName();
	UpdateData( FALSE );
}


BOOL CDlgMosaic::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ctrProgress.SetRange( 0, 100 );
	m_ctrProgress.SetPos( 0 );
	return TRUE; 
}

void CDlgMosaic::OnBnClickedOk2()
{
	UpdateData( TRUE );

	m_pThread = AfxBeginThread( CImgPrcView::SignMosaic, this );	
}


void CDlgMosaic::OnDestroy()
{
	CDialogEx::OnDestroy();

	if ( m_pThread )
		TerminateThread( m_pThread->m_hThread, 0 );
}

void CDlgMosaic::OnBnClickedButton11()
{
	UpdateData( TRUE );

	TCHAR pszPath[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = TEXT( "Set left images path" );
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_USENEWUI;
	bi.lpfn = BrowseCallBackFun;
	bi.lParam = (LPARAM) LPCTSTR( m_strLeft );
	bi.iImage = 0;

	LPITEMIDLIST pidl = SHBrowseForFolder( &bi );
	if ( pidl == NULL )
		return;

	if ( !SHGetPathFromIDList( pidl, pszPath ) )
		return;

	m_strLeft = pszPath;
	UpdateData( FALSE );
}


void CDlgMosaic::OnBnClickedButton12()
{
	UpdateData( TRUE );

	TCHAR pszPath[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = TEXT( "Set right images path" );
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_USENEWUI;
	bi.lpfn = BrowseCallBackFun;
	bi.lParam = (LPARAM) LPCTSTR( m_strRight );
	bi.iImage = 0;

	LPITEMIDLIST pidl = SHBrowseForFolder( &bi );
	if ( pidl == NULL )
		return;

	if ( !SHGetPathFromIDList( pidl, pszPath ) )
		return;

	m_strRight = pszPath;
	UpdateData( FALSE );
}
