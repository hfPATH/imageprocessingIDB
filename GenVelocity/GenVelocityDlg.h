
// GenVelocityDlg.h : ͷ�ļ�
//

#pragma once
#include <cstring>
#include <algorithm>
#include <vector>
#include <fstream>
#include "ReadData.hpp"
#include "afxwin.h"

using namespace std;

// CGenVelocityDlg �Ի���
class CGenVelocityDlg : public CDialogEx
{
// ����
public:
	CGenVelocityDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GENVELOCITY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	//Path
	CString		m_csPath;
	string		m_ssPath;

	//д�ļ�
	ofstream	m_ofLeft;
	ofstream	m_ofMedium;
	ofstream	m_ofRight;
	ofstream	m_ofImu;

	//Data
	CGps		m_gpsData;
	CImu		m_imuData;
	CVelocity	m_velocityData;
	CImageData	m_imgLeft;
	CImageData	m_imgMedium;
	CImageData	m_imgRight;

	CString		m_csGPS;
	CString		m_csIMU;
	CString		m_csVelocity;
	CString		m_csImage;

	void ReadData();
	void WriteData();
	void CalDistance();

	size_t		m_ullImuStart;
	size_t		m_ullGpsStart;
	size_t		m_ullVelocityStart;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CEdit m_editPath;
	afx_msg void OnBnClickedButton2();
	CButton m_btn;
};
