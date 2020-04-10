// PPHT.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "PPHT.h"
#include "afxdialogex.h"


// CPPHT 对话框

IMPLEMENT_DYNAMIC(CPPHT, CDialogEx)

CPPHT::CPPHT(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG14, pParent)
{

	m_dDistance = 0.0;
	m_dAngle = 0.0;
	m_nThreshold = 0;
	m_dMinLineLength = 0.0;
	m_dMaxLineGap = 0.0;
}

CPPHT::~CPPHT()
{
}

void CPPHT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_sliderDistance);
	DDX_Control(pDX, IDC_EDIT1, m_editDistance);
	DDX_Control(pDX, IDC_SLIDER2, m_sliderAngle);
	DDX_Control(pDX, IDC_EDIT3, m_editAngle);
	DDX_Control(pDX, IDC_SLIDER3, m_sliderThreshold);
	DDX_Control(pDX, IDC_EDIT4, m_editThreshold);
	DDX_Control(pDX, IDC_SLIDER4, m_sliderMinLineLength);
	DDX_Control(pDX, IDC_EDIT5, m_editMinLineLength);
	DDX_Control(pDX, IDC_SLIDER5, m_sliderMaxLineGap);
	DDX_Control(pDX, IDC_EDIT6, m_editMaxLineGap);
}


BEGIN_MESSAGE_MAP(CPPHT, CDialogEx)
	ON_WM_HSCROLL()
	ON_EN_KILLFOCUS(IDC_EDIT1, &CPPHT::OnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT3, &CPPHT::OnKillfocusEdit3)
	ON_EN_KILLFOCUS(IDC_EDIT4, &CPPHT::OnKillfocusEdit4)
	ON_EN_KILLFOCUS(IDC_EDIT5, &CPPHT::OnKillfocusEdit5)
	ON_EN_KILLFOCUS(IDC_EDIT6, &CPPHT::OnKillfocusEdit6)
	ON_BN_CLICKED(IDOK, &CPPHT::OnBnClickedOk)
END_MESSAGE_MAP()


// CPPHT 消息处理程序


BOOL CPPHT::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_sliderDistance.SetRange(1 * 100, 10 * 100);
	m_sliderDistance.SetPos(0);
	m_editDistance.SetWindowTextA(_T("1"));

	m_sliderAngle.SetRange(1 * 100, 90 * 100);
	m_sliderAngle.SetPos(0);
	m_editAngle.SetWindowTextA(_T("1"));

	m_sliderThreshold.SetRange(0, 255);
	m_sliderThreshold.SetPos(0);
	m_editThreshold.SetWindowTextA(_T("0"));

	m_sliderMinLineLength.SetRange(0 * 100, 360 * 100);
	m_sliderMinLineLength.SetPos(0);
	m_editMinLineLength.SetWindowTextA(_T("0"));

	m_sliderMaxLineGap.SetRange(0 * 100, 360 * 100);
	m_sliderMaxLineGap.SetPos(0);
	m_editMaxLineGap.SetWindowTextA(_T("0"));

	// TODO:  在此添加额外的初始化
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_PPHT, NULL, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPPHT::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString strTmp;

	double dDistanceTmp = double(m_sliderDistance.GetPos() / 100);
	m_dDistance = dDistanceTmp;
	strTmp.Format("%.2f", m_dDistance);
	m_editDistance.SetWindowTextA(strTmp);

	double dAngleTmp = double(m_sliderAngle.GetPos() / 100);
	m_dAngle = dAngleTmp;
	strTmp.Format("%.2f", m_dAngle);
	m_editAngle.SetWindowTextA(strTmp);

	strTmp.Format("%d", m_sliderThreshold.GetPos());
	m_editThreshold.SetWindowTextA(strTmp);

	double dMinLineLength = double(m_sliderMinLineLength.GetPos() / 100);
	m_dMinLineLength = dMinLineLength;
	strTmp.Format("%.2f", m_dMinLineLength);
	m_editMinLineLength.SetWindowTextA(strTmp);

	double dMaxLineGap = double(m_sliderMaxLineGap.GetPos() / 100);
	m_dMaxLineGap = dMaxLineGap;
	strTmp.Format("%.2f", m_dMaxLineGap);
	m_editMaxLineGap.SetWindowTextA(strTmp);

	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_PPHT, NULL, NULL);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CPPHT::OnKillfocusEdit1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTmp;

	m_editDistance.GetWindowTextA(strTmp);
	m_sliderDistance.SetPos(atoi(strTmp) * 100);

	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_PPHT, NULL, NULL);
	}
}


void CPPHT::OnKillfocusEdit3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTmp;

	m_editAngle.GetWindowTextA(strTmp);
	m_sliderAngle.SetPos(atoi(strTmp) * 100);

	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_PPHT, NULL, NULL);
	}
}


void CPPHT::OnKillfocusEdit4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTmp;

	m_editThreshold.GetWindowTextA(strTmp);
	m_sliderThreshold.SetPos(atoi(strTmp));

	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_PPHT, NULL, NULL);
	}
}


void CPPHT::OnKillfocusEdit5()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTmp;
	m_editMinLineLength.GetWindowTextA(strTmp);
	m_sliderMinLineLength.SetPos(atoi(strTmp) * 100);

	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_PPHT, NULL, NULL);
	}
}


void CPPHT::OnKillfocusEdit6()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTmp;
	m_editMaxLineGap.GetWindowTextA(strTmp);
	m_sliderMaxLineGap.SetPos(atoi(strTmp) * 100);

	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_PPHT, NULL, NULL);
	}
}


void CPPHT::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_UDDATE_BITMAP, NULL, NULL);	
	CDialogEx::OnOK();
}


bool CPPHT::GetItemValue()
{
	CString strTmp;

	m_editDistance.GetWindowTextA(strTmp);
	m_dDistance = atof(strTmp);

	m_editAngle.GetWindowTextA(strTmp);
	m_dAngle = atof(strTmp);

	m_editThreshold.GetWindowTextA(strTmp);
	m_nThreshold = atoi(strTmp);

	m_editMinLineLength.GetWindowTextA(strTmp);
	m_dMinLineLength = atof(strTmp);

	m_editMaxLineGap.GetWindowTextA(strTmp);
	m_dMaxLineGap = atof(strTmp);
	return true;
}