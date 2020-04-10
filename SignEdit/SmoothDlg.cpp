// SmoothDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "SmoothDlg.h"
#include "afxdialogex.h"


// SmoothDlg dialog

IMPLEMENT_DYNAMIC(SmoothDlg, CDialogEx)

SmoothDlg::SmoothDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_Smooth, pParent)
	, m_nsigmaColor(0)
	, m_nsigmaSpace(0)
	, Q(0)
{

}

SmoothDlg::~SmoothDlg()
{
}

void SmoothDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboShape);
	DDX_Control(pDX, IDC_EDIT2, m_editThreshold);
	DDX_Control(pDX, 1010, m_editSigmaSpace);
	DDX_Control(pDX, 1011, m_peditsigmaColor);
	DDX_Control(pDX, 1015, m_sliderThreshold);
	DDX_Control(pDX, IDC_EDIT1Q, m_editQ);
}

void SmoothDlg::GetItemValue()
{
	m_nType = m_comboShape.GetCurSel();
	CString strTmp;
	m_editThreshold.GetWindowTextA(strTmp);
	m_nThreshold = atoi(strTmp);
	CString str1, str2, str3;
	m_peditsigmaColor.GetWindowTextA(str1);
	m_nsigmaColor = atoi(str1);
	m_editSigmaSpace.GetWindowTextA(str2);
	m_nsigmaSpace = atoi(str2);
	m_editQ.GetWindowTextA(str3);
	Q = atoi(str3);


}
BEGIN_MESSAGE_MAP(SmoothDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &SmoothDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &SmoothDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO1, &SmoothDlg::OnCbnSelchangeCombo1)
	ON_EN_CHANGE(IDC_EDIT2, &SmoothDlg::OnEnChangeEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT2, &SmoothDlg::OnEnKillfocusEdit1)
	//ON_BN_CLICKED(IDC_RADIO1, &SmoothDlg::OnBnClickedRadio1)
	ON_EN_CHANGE(IDC_EDIT3, &SmoothDlg::OnEnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, &SmoothDlg::OnEnChangeEdit4)
	ON_EN_KILLFOCUS(IDC_EDIT4, &SmoothDlg::OnEnKillfocusEdit4)
	ON_EN_KILLFOCUS(IDC_EDIT3, &SmoothDlg::OnEnKillfocusEdit3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &SmoothDlg::OnNMCustomdrawSlider2)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON3, &SmoothDlg::OnBnClickedButton3)
	ON_EN_ALIGN_RTL_EC(IDC_EDIT1, &SmoothDlg::OnEnAlignRtlEcEdit2)
	ON_EN_HSCROLL(IDC_EDIT1, &SmoothDlg::OnEnHscrollEdit2)
	ON_EN_CHANGE(IDC_EDIT1, &SmoothDlg::OnEnChangeEdit2)
	ON_EN_KILLFOCUS(IDC_EDIT1, &SmoothDlg::OnEnKillfocusEdit2)
	ON_EN_CHANGE(1010, &SmoothDlg::OnEnChange1010)
	ON_NOTIFY(NM_CUSTOMDRAW, 1015, &SmoothDlg::OnNMCustomdraw1015)
	ON_STN_CLICKED(IDC_STATIC1, &SmoothDlg::OnStnClickedStatic1)
END_MESSAGE_MAP()


// SmoothDlg message handlers


void SmoothDlg::OnBnClickedOk()
{
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_UDDATE_BITMAP, NULL, NULL);
	

	CDialogEx::OnOK();
	// TODO: Add your control notification handler code here
	
}


void SmoothDlg::OnBnClickedCancel()
{
	
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


BOOL SmoothDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_comboShape.InsertString(-1, "blur");
	m_comboShape.InsertString(-1, "GaussianBlur");
	m_comboShape.InsertString(-1, "BoxFilter");
	m_comboShape.InsertString(-1, "medianBlur");
	m_comboShape.InsertString(-1, "bilateralFilter");
	m_comboShape.InsertString(-1, "GeometricmeanFilter");
	m_comboShape.InsertString(-1, "HarmonicMeanFilter");
	m_comboShape.InsertString(-1, "InvHarmonicMeanFilter");
	m_comboShape.SetCurSel(0);
	m_sliderThreshold.SetRange(0, 100);
	m_sliderThreshold.SetPos(1);
	CString str;
	str.Format("%d", m_sliderThreshold.GetPos());
	m_editThreshold.SetWindowTextA(str);

	//³õÊ¼»¯Öµ
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_SMOOTH, NULL, NULL);
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void SmoothDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
	//GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
	if (m_comboShape.GetCurSel() == 7)
	{
		GetDlgItem(IDC_STATIC8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT1Q)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC6)->ShowWindow(SW_SHOW);
		GetDlgItem(1015)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC5)->ShowWindow(SW_HIDE);
		m_editSigmaSpace.ShowWindow(SW_HIDE);
		m_peditsigmaColor.ShowWindow(SW_HIDE);
		m_editThreshold.ShowWindow(SW_SHOW);
	}
	else if (m_comboShape.GetCurSel() != 4)
	{
		GetDlgItem(IDC_STATIC8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT1Q)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC6)->ShowWindow(SW_SHOW);
		GetDlgItem(1015)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC5)->ShowWindow(SW_HIDE);
		m_editSigmaSpace.ShowWindow(SW_HIDE);
		m_peditsigmaColor.ShowWindow(SW_HIDE);
		m_editThreshold.ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_STATIC8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT1Q)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC6)->ShowWindow(SW_HIDE);
		GetDlgItem(1015)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC5)->ShowWindow(SW_SHOW);
		m_editSigmaSpace.ShowWindow(SW_SHOW);
		m_peditsigmaColor.ShowWindow(SW_SHOW);
		m_editThreshold.ShowWindow(SW_HIDE);

	}
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_SMOOTH, NULL, NULL);
}


void SmoothDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void SmoothDlg::OnEnKillfocusEdit1()
{
	// TODO: Add your control notification handler code here
	CString strTmp;
	m_editThreshold.GetWindowTextA(strTmp);
	GetItemValue();

	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_SMOOTH, NULL, NULL);
}



void SmoothDlg::OnEnChangeEdit3()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void SmoothDlg::OnEnChangeEdit4()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void SmoothDlg::OnEnKillfocusEdit4()
{
	// TODO: Add your control notification handler code here
	CString str1;
	m_peditsigmaColor.GetWindowTextA(str1);
	GetItemValue();

	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_SMOOTH, NULL, NULL);
}


void SmoothDlg::OnEnKillfocusEdit3()
{
	// TODO: Add your control notification handler code here
	CString str2;
	m_editSigmaSpace.GetWindowTextA(str2);
	GetItemValue();

	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_LOGANDDOG, NULL, NULL);
}


void SmoothDlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void SmoothDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CString str;
	str.Format("%d", m_sliderThreshold.GetPos());
	m_editThreshold.SetWindowTextA(str);
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_SMOOTH, NULL, NULL);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void SmoothDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
}


void SmoothDlg::OnEnAlignRtlEcEdit2()
{
	// This feature requires Windows 2000 or greater.
	// The symbols _WIN32_WINNT and WINVER must be >= 0x0500.
	// TODO: Add your control notification handler code here
}


void SmoothDlg::OnEnHscrollEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_SCROLL flag ORed into the lParam mask.

	// TODO:  Add your control notification handler code here
}


void SmoothDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void SmoothDlg::OnEnKillfocusEdit2()
{
	CString strTmp;
	m_editThreshold.GetWindowTextA(strTmp);
	GetItemValue();

	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_SMOOTH, NULL, NULL);
	// TODO: Add your control notification handler code here
}


void SmoothDlg::OnEnChange1010()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void SmoothDlg::OnNMCustomdraw1015(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void SmoothDlg::OnStnClickedStatic1()
{
	// TODO: Add your control notification handler code here
}
