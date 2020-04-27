
// GenVelocityDlg.h : 头文件
//

#pragma once
#include <cstring>
#include <algorithm>
#include <vector>
#include <fstream>
#include "ReadData.hpp"
#include "afxwin.h"

using namespace std;

// CGenVelocityDlg 对话框
class CGenVelocityDlg : public CDialogEx
{
// 构造
public:
	CGenVelocityDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GENVELOCITY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	//Path
	CString		m_csPath;
	string		m_ssPath;

	//写文件
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

	// 生成的消息映射函数
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
