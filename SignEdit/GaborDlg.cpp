// GaborDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "GaborDlg.h"
#include "afxdialogex.h"


// CGaborDlg 对话框

IMPLEMENT_DYNAMIC(CGaborDlg, CDialogEx)

CGaborDlg::CGaborDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG18, pParent)
{

}

CGaborDlg::~CGaborDlg()
{
}

void CGaborDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT6, m_kernelSizeXEdit);
	DDX_Control(pDX, IDC_EDIT7, m_kernelSizeYEdit);
	DDX_Control(pDX, IDC_EDIT8, m_anchorXEdit);
	DDX_Control(pDX, IDC_EDIT9, m_anchorYEdit);
	DDX_Control(pDX, IDC_EDIT1, m_sigmaEdit);
	DDX_Control(pDX, IDC_SLIDER3, m_sigmaSlider);
	DDX_Control(pDX, IDC_EDIT2, m_thetaEdit);
	DDX_Control(pDX, IDC_SLIDER4, m_thetaSlider);
	DDX_Control(pDX, IDC_EDIT3, m_lambdaEdit);
	DDX_Control(pDX, IDC_SLIDER5, m_lambdaSlider);
	DDX_Control(pDX, IDC_EDIT4, m_gammaEdit);
	DDX_Control(pDX, IDC_SLIDER9, m_gammaSlider);
	DDX_Control(pDX, IDC_EDIT5, m_phaseOffsetEdit);
	DDX_Control(pDX, IDC_SLIDER11, m_phaseOffsetSlider);
	DDX_Control(pDX, IDC_EDIT11, m_filterDeltaEdit);
	DDX_Control(pDX, IDC_SLIDER12, m_filterDeltaSlider);
	DDX_Control(pDX, IDC_COMBO1, m_borderTypeComBox);
	DDX_Control(pDX, IDC_CHECK2, m_centerChangeCheck);
}


BEGIN_MESSAGE_MAP(CGaborDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK2, &CGaborDlg::OnBnClickedCheck2)
	ON_EN_KILLFOCUS(IDC_EDIT6, &CGaborDlg::OnEnKillfocusEdit6)
	ON_EN_KILLFOCUS(IDC_EDIT7, &CGaborDlg::OnEnKillfocusEdit7)
	ON_EN_KILLFOCUS(IDC_EDIT8, &CGaborDlg::OnEnKillfocusEdit8)
	ON_EN_KILLFOCUS(IDC_EDIT9, &CGaborDlg::OnEnKillfocusEdit9)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CGaborDlg::OnEnKillfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CGaborDlg::OnEnKillfocusEdit2)
	ON_EN_KILLFOCUS(IDC_EDIT3, &CGaborDlg::OnEnKillfocusEdit3)
	ON_EN_KILLFOCUS(IDC_EDIT4, &CGaborDlg::OnEnKillfocusEdit4)
	ON_EN_KILLFOCUS(IDC_EDIT5, &CGaborDlg::OnEnKillfocusEdit5)
	ON_EN_KILLFOCUS(IDC_EDIT11, &CGaborDlg::OnEnKillfocusEdit11)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CGaborDlg::OnCbnSelchangeCombo1)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CGaborDlg 消息处理程序


BOOL CGaborDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_centerChangeCheck.SetCheck(0);
	m_anchorXEdit.EnableWindow(FALSE);
	m_anchorYEdit.EnableWindow(FALSE);

	m_kernelSizeXEdit.SetWindowTextA(_T("0"));
	m_kernelSizeYEdit.SetWindowTextA(_T("0"));
	m_anchorXEdit.SetWindowTextA(_T("-1"));
	m_anchorYEdit.SetWindowTextA(_T("-1"));
	m_sigmaEdit.SetWindowTextA(_T("0"));
	m_thetaEdit.SetWindowTextA(_T("0"));
	m_lambdaEdit.SetWindowTextA(_T("0"));
	m_gammaEdit.SetWindowTextA(_T("0"));
	m_phaseOffsetEdit.SetWindowTextA(_T("0.5"));
	m_filterDeltaEdit.SetWindowTextA(_T("0"));

	m_sigmaSlider.SetRange(0, 10000);
	m_sigmaSlider.SetPos(0);
	m_thetaSlider.SetRange(0, 10000);
	m_thetaSlider.SetPos(0);
	m_lambdaSlider.SetRange(0, 10000);
	m_lambdaSlider.SetPos(0);
	m_gammaSlider.SetRange(0, 10000);
	m_gammaSlider.SetPos(0);
	m_phaseOffsetSlider.SetRange(0, 200);
	m_phaseOffsetSlider.SetPos(50);
	m_filterDeltaSlider.SetRange(0, 10000);
	m_filterDeltaSlider.SetPos(0);

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

	m_anchorPoint.x = -1;
	m_anchorPoint.y = -1;
	GetValueItem();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_GABOROPERATOR, NULL, NULL);
	return TRUE;
}

void CGaborDlg::GetValueItem()
{
	CString strTmp;
	m_kernelSizeXEdit.GetWindowTextA(strTmp);
	m_kernelSize.height = atoi(strTmp);
	m_kernelSizeYEdit.GetWindowTextA(strTmp);
	m_kernelSize.width = atoi(strTmp);
	if (m_centerChangeCheck.GetCheck())
	{
		m_anchorXEdit.GetWindowTextA(strTmp);
		m_anchorPoint.x = atoi(strTmp);
		m_anchorYEdit.GetWindowTextA(strTmp);
		m_anchorPoint.y = atoi(strTmp);
	}
	m_sigma = (double)m_sigmaSlider.GetPos() / 100;
	m_theta = (double)m_thetaSlider.GetPos() / 100;
	m_lambda = (double)m_lambdaSlider.GetPos() / 100;
	m_gamma = (double)m_gammaSlider.GetPos() / 100;
	m_phaseOffset = (double)m_phaseOffsetSlider.GetPos() / 100;
	m_filterDelta = (double)m_sigmaSlider.GetPos() / 100;
	m_borderTypeComBox.GetLBText(m_borderTypeComBox.GetCurSel(), strTmp);
	CBorderTypeMap::iterator it = borderTypeMap.find(strTmp);
	m_borderType = it->second;
}


void CGaborDlg::OnBnClickedCheck2()
{
	if (m_centerChangeCheck.GetCheck())
	{
		m_anchorXEdit.EnableWindow(TRUE);
		m_anchorYEdit.EnableWindow(TRUE);
	}
	else
	{
		m_anchorXEdit.EnableWindow(FALSE);
		m_anchorYEdit.EnableWindow(FALSE);
	}
}


void CGaborDlg::OnEnKillfocusEdit6()
{
	GetValueItem();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_GABOROPERATOR, NULL, NULL);
	return;
}


void CGaborDlg::OnEnKillfocusEdit7()
{
	GetValueItem();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_GABOROPERATOR, NULL, NULL);
	return;
}


void CGaborDlg::OnEnKillfocusEdit8()
{
	GetValueItem();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_GABOROPERATOR, NULL, NULL);
	return;
}


void CGaborDlg::OnEnKillfocusEdit9()
{
	GetValueItem();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_GABOROPERATOR, NULL, NULL);
	return;
}


void CGaborDlg::OnEnKillfocusEdit1()
{
	CString strTmp; 
	m_sigmaEdit.GetWindowTextA(strTmp);
	int sigma = atof(strTmp) * 100;
	if (sigma > 10000 || sigma < 0)
	{
		MessageBox("输入应该在0-100之间！", MB_OK);
		return;
	}
	m_sigmaSlider.SetPos(sigma);
	GetValueItem();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_GABOROPERATOR, NULL, NULL);
	return;
}


void CGaborDlg::OnEnKillfocusEdit2()
{
	CString strTmp;
	m_thetaEdit.GetWindowTextA(strTmp);
	int theta = atof(strTmp) * 100;
	if (theta > 10000 || theta < 0)
	{
		MessageBox("输入应该在0-100之间！", MB_OK);
		return;
	}
	m_thetaSlider.SetPos(theta);
	GetValueItem();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_GABOROPERATOR, NULL, NULL);
	return;
}


void CGaborDlg::OnEnKillfocusEdit3()
{
	CString strTmp;
	m_lambdaEdit.GetWindowTextA(strTmp);
	int lambda = atof(strTmp) * 100;
	if (lambda > 10000 || lambda < 0)
	{
		MessageBox("输入应该在0-100之间！", MB_OK);
		return;
	}
	m_lambdaSlider.SetPos(lambda);
	GetValueItem();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_GABOROPERATOR, NULL, NULL);
	return;
}


void CGaborDlg::OnEnKillfocusEdit4()
{
	CString strTmp;
	m_gammaEdit.GetWindowTextA(strTmp);
	int gamma = atof(strTmp) * 100;
	if (gamma > 10000 || gamma < 0)
	{
		MessageBox("输入应该在0-100之间！", MB_OK);
		return;
	}
	m_gammaSlider.SetPos(gamma);
	GetValueItem();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_GABOROPERATOR, NULL, NULL);
	return;
}


void CGaborDlg::OnEnKillfocusEdit5()
{
	CString strTmp;
	m_phaseOffsetEdit.GetWindowTextA(strTmp);
	int phaseOffset = atof(strTmp) * 100;
	if (phaseOffset > 10000 || phaseOffset < 0)
	{
		MessageBox("输入应该在0-2之间！", MB_OK);
		return;
	}
	m_phaseOffsetSlider.SetPos(phaseOffset);
	GetValueItem();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_GABOROPERATOR, NULL, NULL);
	return;
}


void CGaborDlg::OnEnKillfocusEdit11()
{
	CString strTmp;
	m_filterDeltaEdit.GetWindowTextA(strTmp);
	int filterDelta = atof(strTmp) * 100;
	if (filterDelta > 10000 || filterDelta < 0)
	{
		MessageBox("输入应该在0-100之间！", MB_OK);
		return;
	}
	m_filterDeltaSlider.SetPos(filterDelta);
	GetValueItem();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_GABOROPERATOR, NULL, NULL);
	return;
}


void CGaborDlg::OnCbnSelchangeCombo1()
{
	GetValueItem();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_GABOROPERATOR, NULL, NULL);
	return;
}


void CGaborDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	GetValueItem();
	CString strTmp;
	int pos;
	pos = m_sigmaSlider.GetPos();
	strTmp.Format("%.2f", (double)pos / 100);
	m_sigmaEdit.SetWindowTextA(strTmp);
	pos = m_thetaSlider.GetPos();
	strTmp.Format("%.2f", (double)pos / 100);
	m_thetaEdit.SetWindowTextA(strTmp);
	pos = m_lambdaSlider.GetPos();
	strTmp.Format("%.2f", (double)pos / 100);
	m_lambdaEdit.SetWindowTextA(strTmp);
	pos = m_gammaSlider.GetPos();
	strTmp.Format("%.2f", (double)pos / 100);
	m_gammaEdit.SetWindowTextA(strTmp);
	pos = m_phaseOffsetSlider.GetPos();
	strTmp.Format("%.2f", (double)pos / 100);
	m_phaseOffsetEdit.SetWindowTextA(strTmp);
	pos = m_filterDeltaSlider.GetPos();
	strTmp.Format("%.2f", (double)pos / 100);
	m_filterDeltaEdit.SetWindowTextA(strTmp);

	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_GABOROPERATOR, NULL, NULL);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
