// IterativeThreshold.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "IterativeThreshold.h"
#include "afxdialogex.h"


// CIterativeThreshold 对话框

IMPLEMENT_DYNAMIC(CIterativeThreshold, CDialogEx)

CIterativeThreshold::CIterativeThreshold(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG12, pParent)
{

	m_nEditThreshold = 0;
}

CIterativeThreshold::~CIterativeThreshold()
{
}

void CIterativeThreshold::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_sliderThreshold);
	DDX_Control(pDX, IDC_EDIT3, m_editThreshold);
}


BEGIN_MESSAGE_MAP(CIterativeThreshold, CDialogEx)
	ON_BN_CLICKED(IDOK, &CIterativeThreshold::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_EDIT3, &CIterativeThreshold::OnKillfocusEdit3)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CIterativeThreshold 消息处理程序


void CIterativeThreshold::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_UDDATE_BITMAP, NULL, NULL);
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();//获取当前主框架下的菜单栏指针
	ASSERT_VALID(pMenu);//检查这个指针是否有效
	pMenu->EnableMenuItem(ID_32908, MF_BYCOMMAND | MF_ENABLED);//使按扭可用
	CDialogEx::OnOK();
}


void CIterativeThreshold::OnKillfocusEdit3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTmp;
	m_editThreshold.GetWindowTextA(strTmp);
	m_sliderThreshold.SetPos(atoi(strTmp));

	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_ITERATIVETHRESHOLD, NULL, NULL);
	}
}


void CIterativeThreshold::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString strTmp;
	strTmp.Format("%d", m_sliderThreshold.GetPos());
	m_editThreshold.SetWindowTextA(strTmp);
	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_ITERATIVETHRESHOLD, NULL, NULL);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL CIterativeThreshold::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_sliderThreshold.SetRange(0, 255);
	m_sliderThreshold.SetPos(100);
	m_editThreshold.SetWindowTextA("100");
	//初始化值
	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_ITERATIVETHRESHOLD, NULL, NULL);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


bool CIterativeThreshold::GetItemValue()
{
	CString strTmp;
	m_editThreshold.GetWindowTextA(strTmp);
	m_nEditThreshold = atoi(strTmp);
	return true;
}
