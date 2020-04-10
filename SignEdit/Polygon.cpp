// Polygon.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "Polygon.h"
#include "afxdialogex.h"


// CPolygon 对话框

IMPLEMENT_DYNAMIC(CPolygon, CDialogEx)

CPolygon::CPolygon(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{
	
}

CPolygon::~CPolygon()
{
}

void CPolygon::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_comboMode);
	DDX_Control(pDX, IDC_COMBO3, m_comboMethod);
	DDX_Control(pDX, IDC_COMBO1, m_comboPolygon);
}

void CPolygon::UpdateDataFromCtrl()
{
	m_nMode = m_comboMode.GetCurSel();
	m_nMethod = m_comboMethod.GetCurSel() + 1;
	m_nType = m_comboPolygon.GetCurSel();
}


BEGIN_MESSAGE_MAP(CPolygon, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CPolygon::OnCbnSelchangeCombo2)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CPolygon::OnCbnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CPolygon::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CPolygon 消息处理程序


BOOL CPolygon::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_comboMode.InsertString(-1, "RECT_EXTERNAL");
	m_comboMode.InsertString(-1, "RETR_LIST");
	m_comboMode.InsertString(-1, "RETR_CCOMP");
	m_comboMode.InsertString(-1, "RETR_TREE");
	m_comboMode.SetCurSel(0);
	m_nMode = 0;

	m_comboMethod.InsertString(-1, "CHAIN_APPROX_NONE");
	m_comboMethod.InsertString(-1, "CHAIN_APPROX_SIMPLE");
	m_comboMethod.InsertString(-1, "CHAIN_APPROX_TC89_L1");
	m_comboMethod.InsertString(-1, "CHAIN_APPROX_TC89_KCOS");
	m_comboMethod.SetCurSel(0);
	m_nMethod = 1;

	m_comboPolygon.InsertString(-1, "最小外接矩形");
	m_comboPolygon.InsertString(-1, "椭圆拟合");
	m_comboPolygon.InsertString(-1, "最小外接圆");
	m_comboPolygon.InsertString(-1, "凸包");
	m_comboPolygon.SetCurSel(0);
	m_nType = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPolygon::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDataFromCtrl();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_POLYGON, NULL, NULL);
}


void CPolygon::OnCbnSelchangeCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDataFromCtrl();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_POLYGON, NULL, NULL);
}


void CPolygon::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDataFromCtrl();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_POLYGON, NULL, NULL);
}
