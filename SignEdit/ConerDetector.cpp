// ConerDetector.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "ConerDetector.h"
#include "afxdialogex.h"


// CConerDetector �Ի���

IMPLEMENT_DYNAMIC(CConerDetector, CDialogEx)

CConerDetector::CConerDetector(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG5, pParent)
{
	m_nType = 0;
	m_nMaxNum = 0;
	m_dThreshold = 0;
	m_nMinDistance = 0;
}

CConerDetector::~CConerDetector()
{
}

void CConerDetector::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboConvType);
	DDX_Control(pDX, IDC_SLIDER1, m_slidThreshold);
	DDX_Control(pDX, IDC_EDIT1, m_editThreshold);
	DDX_Control(pDX, IDC_SLIDER3, m_slidMaxNum);
	DDX_Control(pDX, IDC_EDIT3, m_editMaxNum);
	DDX_Control(pDX, IDC_SLIDER4, m_slidMinDistance);
	DDX_Control(pDX, IDC_EDIT4, m_editMinDistance);
	DDX_Control(pDX, IDC_TEXT2, m_text2);
	DDX_Control(pDX, IDC_TEXT1, m_text1);
}

void CConerDetector::UpdateDataFromCtrl()
{
	m_nType = m_comboConvType.GetCurSel();
	if (m_nType == 0)
	{
		m_text1.ShowWindow(SW_SHOW);
		m_slidMaxNum.ShowWindow(SW_SHOW);
		m_slidMinDistance.ShowWindow(SW_SHOW);
		m_text2.ShowWindow(SW_SHOW);
		m_editMaxNum.ShowWindow(SW_SHOW);
		m_editMinDistance.ShowWindow(SW_SHOW);

		m_slidThreshold.SetRange(1, 1000);
		m_slidMaxNum.SetRange(0, 10000);		//���ǵ���Ŀ
		m_slidMinDistance.SetRange(0, 100);		//�����ǵ����С��Ŀ
		m_slidThreshold.SetPos(10);
		m_slidMaxNum.SetPos(200);
		m_slidMinDistance.SetPos(10);

		m_dThreshold = m_slidThreshold.GetPos() / double(1000);
		m_nMaxNum = m_slidMaxNum.GetPos();
		m_nMinDistance = m_slidMinDistance.GetPos();

		CString strTmp;
		strTmp.Format("%.3f", m_dThreshold);
		m_editThreshold.SetWindowTextA(strTmp);
		strTmp.Format("%d", m_nMaxNum);
		m_editMaxNum.SetWindowTextA(strTmp);
		strTmp.Format("%d", m_nMinDistance);
		m_editMinDistance.SetWindowTextA(strTmp);
	}
	else if (m_nType == 1)
	{
		m_text1.ShowWindow(SW_HIDE);
		m_slidMaxNum.ShowWindow(SW_HIDE);
		m_slidMinDistance.ShowWindow(SW_HIDE);
		m_text2.ShowWindow(SW_HIDE);
		m_editMaxNum.ShowWindow(SW_HIDE);
		m_editMinDistance.ShowWindow(SW_HIDE);

		m_slidThreshold.SetRange(1, 1000);
		m_slidThreshold.SetPos(10);
		m_dThreshold = m_slidThreshold.GetPos() / double(1000);

		CString strTmp;
		strTmp.Format("%.3f", m_dThreshold);
		m_editThreshold.SetWindowTextA(strTmp);
	}
	else
	{
		m_text1.ShowWindow(SW_HIDE);
		m_slidMaxNum.ShowWindow(SW_HIDE);
		m_slidMinDistance.ShowWindow(SW_HIDE);
		m_text2.ShowWindow(SW_HIDE);
		m_editMaxNum.ShowWindow(SW_HIDE);
		m_editMinDistance.ShowWindow(SW_HIDE);

		m_slidThreshold.SetRange(0, 200);
		m_slidThreshold.SetPos(10);
		m_dThreshold = m_slidThreshold.GetPos();

		CString strTmp;
		strTmp.Format("%.1f ", m_dThreshold);
		m_editThreshold.SetWindowTextA(strTmp);
	}
}


BEGIN_MESSAGE_MAP(CConerDetector, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CConerDetector::OnCbnSelchangeCombo1)
	ON_WM_HSCROLL()
	ON_BN_CLICKED( IDOK, &CConerDetector::OnBnClickedOk )
END_MESSAGE_MAP()


// CConerDetector ��Ϣ�������


BOOL CConerDetector::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_comboConvType.InsertString(-1, "�Ľ���Harris�ǵ�");
	m_comboConvType.InsertString(-1, "Harris�ǵ���");
	m_comboConvType.InsertString(-1, "Fast�ǵ���");
	m_comboConvType.SetCurSel(0);

	UpdateDataFromCtrl();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_CONERDETECTOR, NULL, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CConerDetector::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateDataFromCtrl();
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_CONERDETECTOR, NULL, NULL);
}


void CConerDetector::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_nType == 0)
	{
		m_dThreshold = m_slidThreshold.GetPos() / double(1000);
		m_nMaxNum = m_slidMaxNum.GetPos();
		m_nMinDistance = m_slidMinDistance.GetPos();

		CString strTmp;
		strTmp.Format("%.3f", m_dThreshold);
		m_editThreshold.SetWindowTextA(strTmp);
		strTmp.Format("%d", m_nMaxNum);
		m_editMaxNum.SetWindowTextA(strTmp);
		strTmp.Format("%d", m_nMinDistance);
		m_editMinDistance.SetWindowTextA(strTmp);
	}
	else if (m_nType == 1)
	{
		m_dThreshold = m_slidThreshold.GetPos() / double(1000);
		CString strTmp;
		strTmp.Format("%.3f", m_dThreshold);
		m_editThreshold.SetWindowTextA(strTmp);
	}
	else
	{
		m_dThreshold = m_slidThreshold.GetPos();
		CString strTmp;
		strTmp.Format("%.1f", m_dThreshold);
		m_editThreshold.SetWindowTextA(strTmp);
	}
	::SendMessageA(m_pParentWnd->GetSafeHwnd(), WM_CONERDETECTOR, NULL, NULL);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CConerDetector::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
