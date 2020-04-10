// HistDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "HistDlg.h"
#include "afxdialogex.h"
#include <opencv2\opencv.hpp>


// CHistDlg 对话框

IMPLEMENT_DYNAMIC(CHistDlg, CDialogEx)

CHistDlg::CHistDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOGHIST, pParent)
{

}

CHistDlg::~CHistDlg()
{
}

void CHistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboConvType);
}


BEGIN_MESSAGE_MAP(CHistDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CHistDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDOK, &CHistDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CHistDlg 消息处理程序


BOOL CHistDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_comboConvType.InsertString(-1, "直方图均衡");
	m_comboConvType.InsertString(-1, "直方图匹配");
	m_comboConvType.SetCurSel(0);
	
	::SendMessage(m_pParentWnd->GetSafeHwnd() ,WM_HISTOGRAM, WPARAM(false), NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CHistDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_comboConvType.GetCurSel() == 0)
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_HISTOGRAM, WPARAM(false), NULL);
	}
	else
	{
		CFileDialog dlg(TRUE, NULL, NULL, OF_READWRITE, TEXT("JPG文件(*.jpg)|*.jpg|BMP文件(*.bmp)|*.bmp|PNG文件(*.png)|*.png||"));
		if (dlg.DoModal() == IDOK)
		{
			CString strFilePath = dlg.GetPathName();
			cv::Mat img = cv::imread(strFilePath.GetBuffer(), CV_LOAD_IMAGE_GRAYSCALE);
			::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_HISTOGRAM, WPARAM(true), (LPARAM)(&img));
		}
	}
}


void CHistDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();//获取当前主框架下的菜单栏指针
	ASSERT_VALID(pMenu);//检查这个指针是否有效
	pMenu->EnableMenuItem(ID_32882, MF_BYCOMMAND | MF_ENABLED);//使按扭不可用
	
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_UDDATE_BITMAP, NULL, NULL);
	CDialogEx::OnOK();
}
