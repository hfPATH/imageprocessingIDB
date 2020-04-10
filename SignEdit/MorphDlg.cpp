// MorphDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "MorphDlg.h"
#include "afxdialogex.h"
#include "ImgPrcView.h"

// CMorphDlg �Ի���

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


// CMorphDlg ��Ϣ�������


BOOL CMorphDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_comboShape.InsertString(-1, "MORPH_RECT");
	m_comboShape.InsertString(-1, "MORPH_CROSS");
	m_comboShape.InsertString(-1, "MORPH_ELLIPSE");

	//��������
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


	//��ʼ��ֵ
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_MORPHOLOGY, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CMorphDlg::OnBnClickedOk()
{
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_UDDATE_BITMAP, NULL, NULL);
	CDialogEx::OnOK();
}


void CMorphDlg::OnBnClickedCancel()
{
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();//��ȡ��ǰ������µĲ˵���ָ��
	ASSERT_VALID(pMenu);//������ָ���Ƿ���Ч
	pMenu->EnableMenuItem(ID_32874, MF_BYCOMMAND | MF_ENABLED);//ʹ��Ť����
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CDialogEx::OnCancel();
}


void CMorphDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_MORPHOLOGY, NULL, NULL);
}


void CMorphDlg::OnCbnSelchangeCombo2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetItemValue();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_MORPHOLOGY, NULL, NULL);
}
