// BinaryRegion.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "BinaryRegion.h"
#include "afxdialogex.h"


// CBinaryRegion 对话框

IMPLEMENT_DYNAMIC(CBinaryRegion, CDialogEx)

CBinaryRegion::CBinaryRegion(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{
	m_dArea = -1;
	m_dCircle = -1;
	m_dDirect = -1;
	m_dHeight = -1;
	m_nidth = -1;
}

CBinaryRegion::~CBinaryRegion()
{
}

void CBinaryRegion::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_checkArea);
	DDX_Control(pDX, IDC_CHECK2, m_checkWidth);
	DDX_Control(pDX, IDC_CHECK5, m_checkHeight);
	DDX_Control(pDX, IDC_CHECK3, m_checkCircle);
	DDX_Control(pDX, IDC_SLIDER1, m_slidArea);
	DDX_Control(pDX, IDC_SLIDER2, m_slidWidth);
	DDX_Control(pDX, IDC_SLIDER5, m_slidHeight);
	DDX_Control(pDX, IDC_SLIDER3, m_slidCircle);
	DDX_Control(pDX, IDC_EDIT3, m_editArea);
	DDX_Control(pDX, IDC_EDIT4, m_editWidth);
	DDX_Control(pDX, IDC_EDIT7, m_editHeight);
	DDX_Control(pDX, IDC_EDIT5, m_editCircle);
	DDX_Control(pDX, IDC_COMBO3, m_comboMode);
	DDX_Control(pDX, IDC_COMBO4, m_comboMethod);
}

void CBinaryRegion::UpdateValueFromCtrl()
{
	m_dArea = double(m_slidArea.GetPos()) / 1000;
	m_nidth = double(m_slidWidth.GetPos()) / 1000;
	m_dHeight = double(m_slidHeight.GetPos()) / 1000;
	m_dCircle = double(m_slidCircle.GetPos()) / 1000;

	CString strTmp;

	if (m_checkArea.GetCheck())
	{
		strTmp.Format("%.3f", m_dArea);
		m_editArea.SetWindowTextA(strTmp);
	}
	if (m_checkCircle.GetCheck())
	{
		strTmp.Format("%.3f", m_dCircle);
		m_editCircle.SetWindowTextA(strTmp);
	}
	if (m_checkHeight.GetCheck())
	{

		strTmp.Format("%.3f", m_dHeight);
		m_editHeight.SetWindowTextA(strTmp);
	}
	if (m_checkWidth.GetCheck())
	{
		strTmp.Format("%.3f", m_nidth);
		m_editWidth.SetWindowTextA(strTmp);
	}

	//轮廓模式和近似方法
	m_nMode = m_comboMode.GetCurSel();
	m_nMethod = m_comboMethod.GetCurSel() + 1;
}


BEGIN_MESSAGE_MAP(CBinaryRegion, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK1, &CBinaryRegion::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CBinaryRegion::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK5, &CBinaryRegion::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK3, &CBinaryRegion::OnBnClickedCheck3)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_COMBO3, &CBinaryRegion::OnCbnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CBinaryRegion::OnCbnSelchangeCombo4)
	ON_BN_CLICKED(IDOK, &CBinaryRegion::OnBnClickedOk)
END_MESSAGE_MAP()


// CBinaryRegion 消息处理程序


BOOL CBinaryRegion::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_slidArea.EnableWindow(FALSE);
	m_slidCircle.EnableWindow(FALSE);
	m_slidHeight.EnableWindow(FALSE);
	m_slidWidth.EnableWindow(FALSE);
	m_editArea.EnableWindow(FALSE);
	m_editCircle.EnableWindow(FALSE);
	m_editHeight.EnableWindow(FALSE);
	m_editWidth.EnableWindow(FALSE);

	m_slidArea.SetRange(0, 1000);
	m_slidCircle.SetRange(0, 1000);
	m_slidHeight.SetRange(0, 1000);
	m_slidWidth.SetRange(0, 1000);

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

	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_BINARYREGION, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//Area
void CBinaryRegion::OnBnClickedCheck1()
{
	if (m_checkArea.GetCheck())
	{
		m_slidArea.EnableWindow(TRUE);
		m_editArea.EnableWindow(TRUE);
		m_editArea.SetWindowTextA("0");
	}
	else
	{
		m_slidArea.SetPos(0);
		m_editArea.SetWindowTextA("");
		m_slidArea.EnableWindow(FALSE);
		m_editArea.EnableWindow(FALSE);
		m_dArea = -1;
	}
}

//Width
void CBinaryRegion::OnBnClickedCheck2()
{
	if (m_checkWidth.GetCheck())
	{
		m_slidWidth.EnableWindow(TRUE);
		m_editWidth.EnableWindow(TRUE);
		m_editWidth.SetWindowTextA("0");
	}
	else
	{
		m_slidWidth.SetPos(0);
		m_editWidth.SetWindowTextA("");
		m_slidWidth.EnableWindow(FALSE);
		m_editWidth.EnableWindow(FALSE);
		m_nidth = -1;
	}
}



//Height
void CBinaryRegion::OnBnClickedCheck5()
{
	if (m_checkHeight.GetCheck())
	{
		m_slidHeight.EnableWindow(TRUE);
		m_editHeight.EnableWindow(TRUE);
		m_editWidth.SetWindowTextA("0");
	}
	else
	{
		m_slidHeight.SetPos(0);
		m_editHeight.SetWindowTextA("");
		m_slidHeight.EnableWindow(FALSE);
		m_editHeight.EnableWindow(FALSE);
		m_dHeight = -1;
	}
}



//Circle
void CBinaryRegion::OnBnClickedCheck3()
{
	if (m_checkCircle.GetCheck())
	{
		m_slidCircle.EnableWindow(TRUE);
		m_editCircle.EnableWindow(TRUE);
		m_editCircle.SetWindowTextA("0");
	}
	else
	{
		m_slidCircle.SetPos(0);
		m_editCircle.SetWindowTextA("");
		m_slidCircle.EnableWindow(FALSE);
		m_editCircle.EnableWindow(FALSE);
		m_dCircle = -1;
	}
}



void CBinaryRegion::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UpdateValueFromCtrl();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_BINARYREGION, NULL, NULL);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CBinaryRegion::OnCbnSelchangeCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateValueFromCtrl();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_BINARYREGION, NULL, NULL);
}


void CBinaryRegion::OnCbnSelchangeCombo4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateValueFromCtrl();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_BINARYREGION, NULL, NULL);
}


void CBinaryRegion::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_UDDATE_BITMAP, NULL, NULL);
	CDialogEx::OnOK();
}
