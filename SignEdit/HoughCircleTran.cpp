// HoughCircleTran.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "HoughCircleTran.h"
#include "afxdialogex.h"


// CHoughCircleTran 对话框

IMPLEMENT_DYNAMIC(CHoughCircleTran, CDialogEx)

CHoughCircleTran::CHoughCircleTran(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG15, pParent)
{

	m_dResolutionSet = 0.0;
	m_dMinCenterDist = 0.0;
	m_dAccuThres = 0.0;
	m_dHighThres = 0.0;
}

CHoughCircleTran::~CHoughCircleTran()
{
}

void CHoughCircleTran::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_sliderResolutionSet);
	DDX_Control(pDX, IDC_EDIT1, m_editResolutionSet);
	DDX_Control(pDX, IDC_SLIDER2, m_sliderMinCircleDist);
	DDX_Control(pDX, IDC_EDIT3, m_editMinCenterDist);
	DDX_Control(pDX, IDC_SLIDER3, m_sliderHighThres);
	DDX_Control(pDX, IDC_EDIT4, m_editHighThres);
	DDX_Control(pDX, IDC_SLIDER4, m_sliderAccuThres);
	DDX_Control(pDX, IDC_EDIT5, m_editAccuThres);
}


BEGIN_MESSAGE_MAP(CHoughCircleTran, CDialogEx)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CHoughCircleTran::OnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT3, &CHoughCircleTran::OnKillfocusEdit3)
	ON_EN_KILLFOCUS(IDC_EDIT4, &CHoughCircleTran::OnKillfocusEdit4)
	ON_EN_KILLFOCUS(IDC_EDIT5, &CHoughCircleTran::OnKillfocusEdit5)
	ON_BN_CLICKED(IDOK, &CHoughCircleTran::OnBnClickedOk)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CHoughCircleTran 消息处理程序


BOOL CHoughCircleTran::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_sliderResolutionSet.SetRange(1 * 100, 4 * 100);
	m_sliderResolutionSet.SetPos(0);
	m_editResolutionSet.SetWindowTextA("1.00");

	m_sliderMinCircleDist.SetRange(1 * 100, 360 * 100);
	m_sliderMinCircleDist.SetPos(0);
	m_editMinCenterDist.SetWindowTextA("1.00");

	m_sliderHighThres.SetRange(1 * 100, 255 * 100);
	m_sliderHighThres.SetPos(100 * 100);
	m_editHighThres.SetWindowTextA("100.00");

	m_sliderAccuThres.SetRange(50* 100, 100 * 100);
	m_sliderAccuThres.SetPos(0 * 100);
	m_editAccuThres.SetWindowTextA("50.00");
	// TODO:  在此添加额外的初始化
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_HOUGHCIRCLETRAN, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CHoughCircleTran::OnKillfocusEdit1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTmp;
	m_editMinCenterDist.GetWindowTextA(strTmp);
	m_sliderMinCircleDist.SetPos(atoi(strTmp) * 100);
	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_HOUGHCIRCLETRAN, NULL, NULL);
	}
}


void CHoughCircleTran::OnKillfocusEdit3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTmp;
	m_editHighThres.GetWindowTextA(strTmp);
	m_sliderHighThres.SetPos(atoi(strTmp) * 100);
	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_HOUGHCIRCLETRAN, NULL, NULL);
	}
}


void CHoughCircleTran::OnKillfocusEdit4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTmp;
	m_editHighThres.GetWindowTextA(strTmp);
	m_sliderHighThres.SetPos(atoi(strTmp) * 100);
	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_HOUGHCIRCLETRAN, NULL, NULL);
	}
}


void CHoughCircleTran::OnKillfocusEdit5()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTmp;
	m_editResolutionSet.GetWindowTextA(strTmp);
	m_sliderResolutionSet.SetPos(atoi(strTmp) * 100);
	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_HOUGHCIRCLETRAN, NULL, NULL);
	}
}


void CHoughCircleTran::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_UDDATE_BITMAP, NULL, NULL);
	CDialogEx::OnOK();
}


void CHoughCircleTran::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString strTmp;

	double dResolutionSet = double(m_sliderResolutionSet.GetPos()) / 100;
	m_dResolutionSet = dResolutionSet;
	strTmp.Format("%.2f", m_dResolutionSet);
	m_editResolutionSet.SetWindowTextA(strTmp);

	double dMinCenterDist = double(m_sliderMinCircleDist.GetPos()) / 100;
	m_dMinCenterDist = dMinCenterDist;
	strTmp.Format("%.2f", m_dMinCenterDist);
	m_editMinCenterDist.SetWindowTextA(strTmp);

	double dHighThres = double(m_sliderHighThres.GetPos()) / 100;
	m_dHighThres = dHighThres;
	strTmp.Format("%.2f", m_dHighThres);
	m_editHighThres.SetWindowTextA(strTmp);

	double dAccuThres = double(m_sliderAccuThres.GetPos()) / 100;
	m_dAccuThres = dAccuThres;
	strTmp.Format("%.2f", m_dAccuThres);
	m_editAccuThres.SetWindowTextA(strTmp);

	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_HOUGHCIRCLETRAN, NULL, NULL);
	}


	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


bool CHoughCircleTran::GetItemValue()
{
	CString strTmp;

	m_editResolutionSet.GetWindowTextA(strTmp);
	m_dResolutionSet = atof(strTmp);

	m_editMinCenterDist.GetWindowTextA(strTmp);
	m_dMinCenterDist = atof(strTmp);

	m_editHighThres.GetWindowTextA(strTmp);
	m_dHighThres = atof(strTmp);

	m_editAccuThres.GetWindowTextA(strTmp);
	m_dAccuThres = atof(strTmp);
	return true;
}
