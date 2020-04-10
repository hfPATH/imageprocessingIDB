// DlgFiles.cpp : implementation file
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "DlgFiles.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFiles dialog


CDlgFiles::CDlgFiles(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFiles::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFiles)
	m_strFilePath1 = _T("");
	m_strFilePath2 = _T("");
	//}}AFX_DATA_INIT
}


void CDlgFiles::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFiles)
	DDX_Text(pDX, IDC_EDIT1, m_strFilePath1);
	DDX_Text(pDX, IDC_EDIT2, m_strFilePath2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFiles, CDialog)
	//{{AFX_MSG_MAP(CDlgFiles)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDlgFiles::OnButton1() 
{
	static char szFilter[] = "BMP文件(*.bmp)|*.bmp||";
	CFileDialog dlg(TRUE,"bmp",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
	if(dlg.DoModal()!=IDOK)
		return;
	m_strFilePath1 = dlg.GetPathName();
	UpdateData(FALSE);	
}

void CDlgFiles::OnButton2() 
{
	static char szFilter[] = "BMP文件(*.bmp)|*.bmp||";
	CFileDialog dlg(TRUE,"bmp",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);
	if(dlg.DoModal()!=IDOK)
		return;
	m_strFilePath2 = dlg.GetPathName();
	UpdateData(FALSE);	
	
}
