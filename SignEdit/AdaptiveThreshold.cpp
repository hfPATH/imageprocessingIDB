// AdaptiveThreshold.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "AdaptiveThreshold.h"
#include "afxdialogex.h"


// CAdaptiveThreshold 对话框

IMPLEMENT_DYNAMIC(CAdaptiveThreshold, CDialogEx)

CAdaptiveThreshold::CAdaptiveThreshold(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG11, pParent)
{

	//  m_nEditThreshold = 0;
	m_nEditRadius = 0;
	m_dEditDev = 0.0;
	m_nEditAlg = 0;
	m_nEditType = 0;
}

CAdaptiveThreshold::~CAdaptiveThreshold()
{
}

void CAdaptiveThreshold::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboAlg);
	DDX_Control(pDX, IDC_COMBO2, m_comboType);
	DDX_Control(pDX, IDC_SLIDER4, m_sliderRadius);
	DDX_Control(pDX, IDC_EDIT3, m_editRadius);
	DDX_Control(pDX, IDC_SLIDER5, m_sliderDev);
	DDX_Control(pDX, IDC_EDIT4, m_editDev);
}


BEGIN_MESSAGE_MAP(CAdaptiveThreshold, CDialogEx)
	ON_EN_KILLFOCUS(IDC_EDIT3, &CAdaptiveThreshold::OnKillfocusEdit3)
	ON_EN_KILLFOCUS(IDC_EDIT4, &CAdaptiveThreshold::OnKillfocusEdit4)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAdaptiveThreshold::OnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CAdaptiveThreshold::OnSelchangeCombo2)
	ON_BN_CLICKED(IDOK, &CAdaptiveThreshold::OnBnClickedOk)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


void CAdaptiveThreshold::OnKillfocusEdit3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTmp;
	m_editRadius.GetWindowTextA(strTmp);
	m_sliderRadius.SetPos(atoi(strTmp));
	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_ADAPTIVETHRESHOLD, NULL, NULL);
	}
}


void CAdaptiveThreshold::OnKillfocusEdit4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTmp;
	m_editDev.GetWindowTextA(strTmp);
	m_sliderDev.SetPos(atoi(strTmp) * 100);
	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_ADAPTIVETHRESHOLD, NULL, NULL);
	}
}


void CAdaptiveThreshold::OnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_ADAPTIVETHRESHOLD, NULL, NULL);
	}
}


void CAdaptiveThreshold::OnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_ADAPTIVETHRESHOLD, NULL, NULL);
	}
}


void CAdaptiveThreshold::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_UDDATE_BITMAP, NULL, NULL);
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();//获取当前主框架下的菜单栏指针
	ASSERT_VALID(pMenu);//检查这个指针是否有效
	pMenu->EnableMenuItem(ID_32877, MF_BYCOMMAND | MF_ENABLED);//使按扭可用
	CDialogEx::OnOK();
}


void CAdaptiveThreshold::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CString strTmp;
	strTmp.Format("%d", m_sliderRadius.GetPos());
	m_editRadius.SetWindowTextA(strTmp);

	double dTmp = double(m_sliderDev.GetPos()) / 100;
	m_dEditDev = dTmp;
	strTmp.Format("%.2f", dTmp);
	m_editDev.SetWindowTextA(strTmp);

	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_ADAPTIVETHRESHOLD, NULL, NULL);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL CAdaptiveThreshold::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_comboAlg.InsertString(-1, "ADAPTIVE_THRESH_MEAN_C");
	m_comboAlg.InsertString(-1, "ADAPTIVE_THRESH_GAUSSIAN_C");
	m_comboAlg.SetCurSel(0);

	m_comboType.InsertString(-1, "THRESH_BINARY");
	m_comboType.InsertString(-1, "THRESH_BINARY_INV");
	m_comboType.SetCurSel(0);

	m_sliderRadius.SetRange(1, 5);
	m_sliderRadius.SetPos(0);
	m_editRadius.SetWindowTextA("1");

	m_sliderDev.SetRange(-255 * 100, 255 * 100);
	m_sliderDev.SetPos(0);
	m_editDev.SetWindowTextA("-255.00");

	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_ADAPTIVETHRESHOLD, NULL, NULL);	
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


bool CAdaptiveThreshold::GetItemValue()
{
	CString strTmp;

	m_nEditAlg = m_comboAlg.GetCurSel();
	m_nEditType = m_comboType.GetCurSel();

	m_editRadius.GetWindowTextA(strTmp);
	m_nEditRadius = atoi(strTmp);

	m_editDev.GetWindowTextA(strTmp);
	m_dEditDev = atoi(strTmp);
	return true;
}
