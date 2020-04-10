// LaplacianDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "LaplacianDlg.h"
#include "afxdialogex.h"


// CLaplacianDlg 对话框

IMPLEMENT_DYNAMIC(CLaplacianDlg, CDialogEx)

CLaplacianDlg::CLaplacianDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG17, pParent)
{

}

CLaplacianDlg::~CLaplacianDlg()
{
}

void CLaplacianDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BORDERTYPECOMBO, m_borderTypeComBox);
	DDX_Control(pDX, IDC_KSIZEEDIT, m_kSizeEdit);
	DDX_Control(pDX, IDC_SCALEEDIT, m_scaleEdit);
	DDX_Control(pDX, IDC_DELTAEDIT, m_deltaEdit);
	DDX_Control(pDX, IDC_SLIDER1, m_kSizeSlider);
	DDX_Control(pDX, IDC_SLIDER2, m_scaleSlider);
	DDX_Control(pDX, IDC_SLIDER3, m_deltaSlider);
}


BEGIN_MESSAGE_MAP(CLaplacianDlg, CDialogEx)
	ON_EN_KILLFOCUS(IDC_KSIZEEDIT, &CLaplacianDlg::OnEnKillfocusKsizeedit)
	ON_EN_KILLFOCUS(IDC_SCALEEDIT, &CLaplacianDlg::OnEnKillfocusScaleedit)
	ON_EN_KILLFOCUS(IDC_DELTAEDIT, &CLaplacianDlg::OnEnKillfocusDeltaedit)
	ON_CBN_SELCHANGE(IDC_BORDERTYPECOMBO, &CLaplacianDlg::OnCbnSelchangeBordertypecombo)
	ON_BN_CLICKED(IDOK, &CLaplacianDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CLaplacianDlg::OnBnClickedCancel)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CLaplacianDlg 消息处理程序


BOOL CLaplacianDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	borderTypeMap.insert(CBorderTypeMap::value_type("BORDER_CONSTANT", 0));
	borderTypeMap.insert(CBorderTypeMap::value_type("BORDER_REPLICATE", 1));
	borderTypeMap.insert(CBorderTypeMap::value_type("BORDER_REFLECT", 2));
	borderTypeMap.insert(CBorderTypeMap::value_type("BORDER_WRAP", 3));
	borderTypeMap.insert(CBorderTypeMap::value_type("BORDER_REFLECT_101", 4));
	borderTypeMap.insert(CBorderTypeMap::value_type("BORDER_TRANSPARENT", 5));
	borderTypeMap.insert(CBorderTypeMap::value_type("BORDER_REFLECT101", 4));
	borderTypeMap.insert(CBorderTypeMap::value_type("BORDER_DEFAULT", 4));
	borderTypeMap.insert(CBorderTypeMap::value_type("BORDER_ISOLATED", 16));

	m_borderTypeComBox.InsertString(-1, "BORDER_CONSTANT");
	m_borderTypeComBox.InsertString(-1, "BORDER_REPLICATE");
	m_borderTypeComBox.InsertString(-1, "BORDER_REFLECT");
	m_borderTypeComBox.InsertString(-1, "BORDER_WRAP");
	m_borderTypeComBox.InsertString(-1, "BORDER_REFLECT_101");
	m_borderTypeComBox.InsertString(-1, "BORDER_TRANSPARENT");
	m_borderTypeComBox.InsertString(-1, "BORDER_REFLECT101");
	m_borderTypeComBox.InsertString(-1, "BORDER_DEFAULT");
	m_borderTypeComBox.InsertString(-1, "BORDER_ISOLATED");
	m_borderTypeComBox.SetCurSel(7);

	m_kSizeEdit.SetWindowTextA(_T("1"));
	m_scaleEdit.SetWindowTextA(_T("1.0"));
	m_deltaEdit.SetWindowTextA(_T("0.0"));

	m_kSizeSlider.SetRange(0, 100);
	m_kSizeSlider.SetPos(1);
	m_scaleSlider.SetRange(0, 20000);
	m_scaleSlider.SetPos(100);
	m_deltaSlider.SetRange(0, 20000);
	m_deltaSlider.SetPos(0);

	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_OPENCVLAPLACIAN, NULL, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CLaplacianDlg::GetItemValue()
{
	CString strTmp;
	int posTmp;
	posTmp = m_kSizeSlider.GetPos();
	sobelSize = 2 * posTmp + 1;
	strTmp.Format("%d", posTmp);
	m_kSizeEdit.SetWindowTextA(strTmp);

	posTmp = m_scaleSlider.GetPos();
	scale = double(posTmp) / 100;
	strTmp.Format("%.2f", scale);
	m_scaleEdit.SetWindowTextA(strTmp);

	posTmp = m_deltaSlider.GetPos();
	delta = double(posTmp) / 100;
	strTmp.Format("%.2f", delta);
	m_deltaEdit.SetWindowTextA(strTmp);

	m_borderTypeComBox.GetLBText(m_borderTypeComBox.GetCurSel(), strTmp);
	CBorderTypeMap::iterator it = borderTypeMap.find(strTmp);
	if (it == borderTypeMap.end())
	{
		MessageBox("没有找到相应的边界填充类型！", MB_OK);
		return;
	}
	else
	{
		borderType = it->second;
	}
}

void CLaplacianDlg::OnEnKillfocusKsizeedit()
{
	CString strTmp;
	m_kSizeEdit.GetWindowTextA(strTmp);
	int sobelSizeTmp = atoi(strTmp);
	m_kSizeSlider.SetPos(sobelSizeTmp);

	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_OPENCVLAPLACIAN, NULL, NULL);
	return;
}


void CLaplacianDlg::OnEnKillfocusScaleedit()
{
	CString strTmp;
	m_scaleEdit.GetWindowTextA(strTmp);
	double scaleTmp = atof(strTmp);
	m_scaleSlider.SetPos(int(scaleTmp * 100));

	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_OPENCVLAPLACIAN, NULL, NULL);
	return;
}


void CLaplacianDlg::OnEnKillfocusDeltaedit()
{
	CString strTmp;
	m_deltaEdit.GetWindowTextA(strTmp);
	double deltaTmp = atof(strTmp);
	m_deltaSlider.SetPos(int(deltaTmp * 100));

	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_OPENCVLAPLACIAN, NULL, NULL);
	return;
}


void CLaplacianDlg::OnCbnSelchangeBordertypecombo()
{
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_OPENCVLAPLACIAN, NULL, NULL);
	return;
}


void CLaplacianDlg::OnBnClickedOk()
{
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_UDDATE_BITMAP, NULL, NULL);
	CDialogEx::OnOK();
}


void CLaplacianDlg::OnBnClickedCancel()
{
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_CANCELOPERA, NULL, NULL);
	CDialogEx::OnCancel();
}


void CLaplacianDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int sobelSizeTmp = m_kSizeSlider.GetPos();
	double scaleTmp = double(m_scaleSlider.GetPos()) / 100;
	double deltaTmp = double(m_deltaSlider.GetPos()) / 100;
	CString strTmp;
	strTmp.Format("%d", sobelSizeTmp);
	m_kSizeEdit.SetWindowTextA(strTmp);
	strTmp.Format("%.2f", scaleTmp);
	m_scaleEdit.SetWindowTextA(strTmp);
	strTmp.Format("%.2f", deltaTmp);
	m_deltaEdit.SetWindowTextA(strTmp);

	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_OPENCVLAPLACIAN, NULL, NULL);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
