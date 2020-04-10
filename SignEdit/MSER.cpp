// MSER.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "MSER.h"
#include "afxdialogex.h"


// CMSER 对话框

IMPLEMENT_DYNAMIC(CMSER, CDialogEx)

CMSER::CMSER(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG6, pParent)
{
	m_dAreaThreshold = 0;
	m_nDelta = 0;
	m_dMaxVariation = 0;
	m_dMinDiversity = 0;
	m_dMinMargin = 0;
	m_nEdgeBlurSize = 0;
	m_nMaxArea = 0;
	m_nMaxEvolution = 0;
	m_nMinArea = 0;
}

CMSER::~CMSER()
{
}

void CMSER::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_slidDelta);
	DDX_Control(pDX, IDC_EDIT1, m_editDelta);
	DDX_Control(pDX, IDC_SLIDER2, m_slidMinArea);
	DDX_Control(pDX, IDC_EDIT6, m_editMinArea);
	DDX_Control(pDX, IDC_SLIDER3, m_slidMaxArea);
	DDX_Control(pDX, IDC_EDIT8, m_editMaxArea);
	DDX_Control(pDX, IDC_SLIDER5, m_slidMaxVariation);
	DDX_Control(pDX, IDC_EDIT9, m_editMaxVariation);
	DDX_Control(pDX, IDC_SLIDER6, m_slidMinDiversity);
	DDX_Control(pDX, IDC_EDIT10, m_editMinDiversity);
	DDX_Control(pDX, IDC_SLIDER7, m_slidMaxEvolution);
	DDX_Control(pDX, IDC_EDIT11, m_editMaxEvolution);
	DDX_Control(pDX, IDC_SLIDER8, m_slidAreaThreshold);
	DDX_Control(pDX, IDC_EDIT12, m_editAreaThreshold);
	DDX_Control(pDX, IDC_SLIDER9, m_slidMinMargin);
	DDX_Control(pDX, IDC_EDIT13, m_editMinMargin);
	DDX_Control(pDX, IDC_SLIDER10, m_slidEdgeBlurSize);
	DDX_Control(pDX, IDC_EDIT14, m_editEdgeBlurSize);
}


BEGIN_MESSAGE_MAP(CMSER, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CMSER 消息处理程序


BOOL CMSER::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_slidDelta.SetRange(1, 255);
	m_slidDelta.SetPageSize(1);
	m_slidDelta.SetLineSize(1);
	m_slidMinArea.SetRange(1, 1000);		//最小面积 
	m_slidMaxArea.SetRange(1, 1440000);		//最大面积
	m_slidMinArea.SetPageSize(50);			//鼠标点击或者PgUp按键滑动间隔
	m_slidMinArea.SetLineSize(1);			//左、右箭头滑动间隔
	m_slidMaxArea.SetPageSize(200);
	m_slidMaxArea.SetLineSize(1);
	m_slidMaxEvolution.SetRange(1, 1000);
	m_slidAreaThreshold.SetRange(0, 1000);
	m_slidMinMargin.SetRange(0, 1000);
	m_slidEdgeBlurSize.SetRange(1, 100);

	m_slidDelta.SetPos(5);
	m_slidMinArea.SetPos(60);
	m_slidMaxArea.SetPos(14400);
	m_slidMaxVariation.SetPos(25);
	m_slidMinDiversity.SetPos(20);
	m_slidMaxEvolution.SetPos(200);
	m_slidMinMargin.SetPos(3);
	m_slidAreaThreshold.SetPos(101);
	m_slidEdgeBlurSize.SetPos(5);
	
	UpdateDataFromCtrl();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_MSER, NULL, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CMSER::UpdateDataFromCtrl()
{
	m_nDelta = m_slidDelta.GetPos();
	m_nMinArea = m_slidMinArea.GetPos();
	m_nMaxArea = m_slidMaxArea.GetPos();
	m_dMaxVariation = m_slidMaxVariation.GetPos() / float(100);
	m_dMinDiversity = m_slidMinDiversity.GetPos() / float(100);
	m_nMaxEvolution = m_slidMaxEvolution.GetPos();
	m_dAreaThreshold = m_slidAreaThreshold.GetPos() / float(1000);
	m_dMinMargin = m_slidMinMargin.GetPos() / float(1000);
	m_nEdgeBlurSize = m_slidEdgeBlurSize.GetPos();

	m_nMaxArea = m_nMaxArea > m_nMinArea ? m_nMaxArea : m_nMinArea;

	CString strTmp;
	strTmp.Format("%d", m_nDelta);
	m_editDelta.SetWindowTextA(strTmp);
	strTmp.Format("%d", m_nMinArea);
	m_editMinArea.SetWindowTextA(strTmp);
	strTmp.Format("%d", m_nMaxArea);
	m_editMaxArea.SetWindowTextA(strTmp);
	strTmp.Format("%.2f", m_dMaxVariation);
	m_editMaxVariation.SetWindowTextA(strTmp);
	strTmp.Format("%.2f", m_dMinDiversity);
	m_editMinDiversity.SetWindowTextA(strTmp);
	strTmp.Format("%d", m_nMaxEvolution);
	m_editMaxEvolution.SetWindowTextA(strTmp);
	strTmp.Format("%.3f", m_dAreaThreshold);
	m_editAreaThreshold.SetWindowTextA(strTmp);
	strTmp.Format("%.3f", m_dMinMargin);
	m_editMinMargin.SetWindowTextA(strTmp);
	strTmp.Format("%d", m_nEdgeBlurSize);
	m_editEdgeBlurSize.SetWindowTextA(strTmp);
}


void CMSER::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UpdateDataFromCtrl();
	if (::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_MSER, NULL, NULL))
	{
		CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}
