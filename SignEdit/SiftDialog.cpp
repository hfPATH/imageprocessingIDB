// SiftDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "SiftDialog.h"
#include "afxdialogex.h"


// CSiftDialog 对话框

IMPLEMENT_DYNAMIC(CSiftDialog, CDialogEx)

CSiftDialog::CSiftDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG7, pParent)
{

}

CSiftDialog::~CSiftDialog()
{
}

void CSiftDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_slidFeatures);
	DDX_Control(pDX, IDC_EDIT1, m_editFeatures);
	DDX_Control(pDX, IDC_SLIDER2, m_slidOctaveLayers);
	DDX_Control(pDX, IDC_EDIT6, m_editOctaveLayers);
	DDX_Control(pDX, IDC_SLIDER3, m_slidContrastThreshold);
	DDX_Control(pDX, IDC_EDIT8, m_editContrastThreshold);
	DDX_Control(pDX, IDC_SLIDER5, m_slidEdgeThreshold);
	DDX_Control(pDX, IDC_EDIT9, m_editEdgeThreshold);
	DDX_Control(pDX, IDC_SLIDER6, m_slidSigma);
	DDX_Control(pDX, IDC_EDIT10, m_editSigma);
}


BEGIN_MESSAGE_MAP(CSiftDialog, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CSiftDialog 消息处理程序


BOOL CSiftDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_slidFeatures.SetRange(0, 10);
	m_slidOctaveLayers.SetRange(1, 10);
	m_slidContrastThreshold.SetRange(0, 100);
	m_slidEdgeThreshold.SetRange(0, 1000);
	m_slidSigma.SetRange(0, 1000);
	
	//默认参数
	m_slidFeatures.SetPos(0);
	m_slidOctaveLayers.SetPos(3);
	m_slidContrastThreshold.SetPos(4);
	m_slidEdgeThreshold.SetPos(100);
	m_slidSigma.SetPos(160);

	UpdateDataFromCtrl();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_SIFT, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CSiftDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UpdateDataFromCtrl();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_SIFT, NULL, NULL);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CSiftDialog::UpdateDataFromCtrl()
{
	m_nFeatures = m_slidFeatures.GetPos();
	m_nOctaveLayers = m_slidOctaveLayers.GetPos();
	m_dContrastThreshold = m_slidContrastThreshold.GetPos() / float(100);
	m_dEdgeThreshold = m_slidEdgeThreshold.GetPos() / float(10);
	m_dSigma = m_slidSigma.GetPos() / float(100);

	CString strTmp;
	strTmp.Format("%d", m_nFeatures);
	m_editFeatures.SetWindowTextA(strTmp);
	strTmp.Format("%d", m_nOctaveLayers);
	m_editOctaveLayers.SetWindowTextA(strTmp);
	strTmp.Format("%.2f", m_dContrastThreshold);
	m_editContrastThreshold.SetWindowTextA(strTmp);
	strTmp.Format("%.2f", m_dEdgeThreshold);
	m_editEdgeThreshold.SetWindowTextA(strTmp);
	strTmp.Format("%.2f", m_dSigma);
	m_editSigma.SetWindowTextA(strTmp);
}
