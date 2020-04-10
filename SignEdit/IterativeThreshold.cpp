// IterativeThreshold.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "IterativeThreshold.h"
#include "afxdialogex.h"


// CIterativeThreshold �Ի���

IMPLEMENT_DYNAMIC(CIterativeThreshold, CDialogEx)

CIterativeThreshold::CIterativeThreshold(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG12, pParent)
{

	m_nEditThreshold = 0;
}

CIterativeThreshold::~CIterativeThreshold()
{
}

void CIterativeThreshold::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_sliderThreshold);
	DDX_Control(pDX, IDC_EDIT3, m_editThreshold);
}


BEGIN_MESSAGE_MAP(CIterativeThreshold, CDialogEx)
	ON_BN_CLICKED(IDOK, &CIterativeThreshold::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_EDIT3, &CIterativeThreshold::OnKillfocusEdit3)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CIterativeThreshold ��Ϣ�������


void CIterativeThreshold::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_UDDATE_BITMAP, NULL, NULL);
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();//��ȡ��ǰ������µĲ˵���ָ��
	ASSERT_VALID(pMenu);//������ָ���Ƿ���Ч
	pMenu->EnableMenuItem(ID_32908, MF_BYCOMMAND | MF_ENABLED);//ʹ��Ť����
	CDialogEx::OnOK();
}


void CIterativeThreshold::OnKillfocusEdit3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strTmp;
	m_editThreshold.GetWindowTextA(strTmp);
	m_sliderThreshold.SetPos(atoi(strTmp));

	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_ITERATIVETHRESHOLD, NULL, NULL);
	}
}


void CIterativeThreshold::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString strTmp;
	strTmp.Format("%d", m_sliderThreshold.GetPos());
	m_editThreshold.SetWindowTextA(strTmp);
	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_ITERATIVETHRESHOLD, NULL, NULL);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL CIterativeThreshold::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_sliderThreshold.SetRange(0, 255);
	m_sliderThreshold.SetPos(100);
	m_editThreshold.SetWindowTextA("100");
	//��ʼ��ֵ
	if (GetItemValue())
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_ITERATIVETHRESHOLD, NULL, NULL);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


bool CIterativeThreshold::GetItemValue()
{
	CString strTmp;
	m_editThreshold.GetWindowTextA(strTmp);
	m_nEditThreshold = atoi(strTmp);
	return true;
}
