// HistDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImgPrc.h"
#include "HistDlg.h"
#include "afxdialogex.h"
#include <opencv2\opencv.hpp>


// CHistDlg �Ի���

IMPLEMENT_DYNAMIC(CHistDlg, CDialogEx)

CHistDlg::CHistDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOGHIST, pParent)
{

}

CHistDlg::~CHistDlg()
{
}

void CHistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_comboConvType);
}


BEGIN_MESSAGE_MAP(CHistDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CHistDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDOK, &CHistDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CHistDlg ��Ϣ�������


BOOL CHistDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_comboConvType.InsertString(-1, "ֱ��ͼ����");
	m_comboConvType.InsertString(-1, "ֱ��ͼƥ��");
	m_comboConvType.SetCurSel(0);
	
	::SendMessage(m_pParentWnd->GetSafeHwnd() ,WM_HISTOGRAM, WPARAM(false), NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CHistDlg::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_comboConvType.GetCurSel() == 0)
	{
		::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_HISTOGRAM, WPARAM(false), NULL);
	}
	else
	{
		CFileDialog dlg(TRUE, NULL, NULL, OF_READWRITE, TEXT("JPG�ļ�(*.jpg)|*.jpg|BMP�ļ�(*.bmp)|*.bmp|PNG�ļ�(*.png)|*.png||"));
		if (dlg.DoModal() == IDOK)
		{
			CString strFilePath = dlg.GetPathName();
			cv::Mat img = cv::imread(strFilePath.GetBuffer(), CV_LOAD_IMAGE_GRAYSCALE);
			::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_HISTOGRAM, WPARAM(true), (LPARAM)(&img));
		}
	}
}


void CHistDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMenu* pMenu = AfxGetMainWnd()->GetMenu();//��ȡ��ǰ������µĲ˵���ָ��
	ASSERT_VALID(pMenu);//������ָ���Ƿ���Ч
	pMenu->EnableMenuItem(ID_32882, MF_BYCOMMAND | MF_ENABLED);//ʹ��Ť������
	
	::SendMessage(m_pParentWnd->GetSafeHwnd(), WM_UDDATE_BITMAP, NULL, NULL);
	CDialogEx::OnOK();
}
