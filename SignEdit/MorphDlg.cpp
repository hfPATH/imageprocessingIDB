// MorphDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "MorphDlg.h"
#include "afxdialogex.h"
#include "ImgPrcView.h"

// CMorphDlg 对话框

IMPLEMENT_DYNAMIC(CMorphDlg, CDialogEx)

CMorphDlg::CMorphDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CMorphDlg::~CMorphDlg()
{
}

void CMorphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editThreshold);
	DDX_Control(pDX, IDC_COMBO1, m_comboShape);
	DDX_Control(pDX, IDC_COMBO2, m_comboConvType);
	DDX_Control(pDX, IDC_SLIDER1, m_sliderThreshold);
}

void CMorphDlg::GetItemValue()
{
	m_nShape = m_comboShape.GetCurSel();
	m_nType = m_comboConvType.GetCurSel();

	CString strTmp;
	m_editThreshold.GetWindowTextA(strTmp);
	this->UpdateWindow();
	m_nThreshold = atoi(strTmp);
}


BEGIN_MESSAGE_MAP(CMorphDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMorphDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMorphDlg::OnBnClickedCancel)
	ON_WM_HSCROLL()
	ON_EN_KILLFOCUS(IDC_EDIT1, &CMorphDlg::OnEnKillfocusEdit1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CMorphDlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CMorphDlg::OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CMorphDlg 消息处理程序


BOOL CMorphDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_comboShape.InsertString(-1, "MORPH_RECT");
	m_comboShape.InsertString(-1, "MORPH_CROSS");
	m_comboShape.InsertString(-1, "MORPH_ELLIPSE");

	//操作类型
	m_comboConvType.InsertString(-1, "MORPH_ERODE");
	m_comboConvType.InsertString(-1, "MORPH_DILATE");
	m_comboConvType.InsertString(-1, "MORPH_OPEN");
	m_comboConvType.InsertString(-1, "MORPH_CLOSE");
	m_comboConvType.InsertString(-1, "MORPH_GRADIENT");
	m_comboConvType.InsertString(-1, "MORPH_TOPHAT");
	m_comboConvType.InsertString(-1, "MORPH_BLACKHAT");
	m_comboConvType.InsertString(-1, "MORPH_HITMISS");


	m_sliderThreshold.SetRange(0, 200);
	m_sliderThreshold.SetPos(1);

	m_comboShape.SetCurSel(0);
	m_comboConvType.SetCurSel(0);

	CString str;
	str.Format("%d", m_sliderThreshold.GetPos());
	m_editThreshold.SetWindowTextA(str);


	//初始化值
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_MORPHOLOGY, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CMorphDlg::OnBnClickedOk()
{
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_UDDATE_BITMAP, NULL, NULL);
	CDialogEx::OnOK();
}


void CMorphDlg::OnBnClickedCancel()
{
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();//获取当前主框架下的菜单栏指针
	ASSERT_VALID(pMenu);//检查这个指针是否有效
	pMenu->EnableMenuItem(ID_32874, MF_BYCOMMAND | MF_ENABLED);//使按扭可用
	// TODO: 在此添加控件通知处理程序代码

	CDialogEx::OnCancel();
}


void CMorphDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString strTmp;
	strTmp.Format("%d", m_sliderThreshold.GetPos());
	m_editThreshold.SetWindowTextA(strTmp);
	GetItemValue();

	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_MORPHOLOGY, NULL, NULL);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CMorphDlg::OnEnKillfocusEdit1()
{
	CString strTmp;
	m_editThreshold.GetWindowTextA(strTmp);
	m_sliderThreshold.SetPos(atoi(strTmp));
	GetItemValue();

	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_MORPHOLOGY, NULL, NULL);
}


void CMorphDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_MORPHOLOGY, NULL, NULL);
}


void CMorphDlg::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_MORPHOLOGY, NULL, NULL);
}
