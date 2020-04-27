
// GenVelocityDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GenVelocity.h"
#include "GenVelocityDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CString		g_csRoot;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGenVelocityDlg 对话框



CGenVelocityDlg::CGenVelocityDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GENVELOCITY_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGenVelocityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editPath);
	DDX_Control(pDX, IDC_BUTTON1, m_btn);
}

BEGIN_MESSAGE_MAP(CGenVelocityDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CGenVelocityDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CGenVelocityDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CGenVelocityDlg 消息处理程序

BOOL CGenVelocityDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//获取当前exe所在的路径
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	TCHAR *pChr = _tcsrchr(szFilePath, _T('\\'));
	pChr++;
	*pChr = _T('\0');
	g_csRoot = szFilePath;

	m_btn.EnableWindow(false);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGenVelocityDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGenVelocityDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGenVelocityDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int CALLBACK BrowseCallbackProc(HWND  hwnd, UINT  uMsg, LPARAM  lParam, LPARAM  lpData)
{
	switch (uMsg) {
	case   BFFM_INITIALIZED:
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)g_csRoot.GetBuffer());
		g_csRoot.ReleaseBuffer();
		break;
	case   BFFM_SELCHANGED:
	{
		TCHAR curr[MAX_PATH];
		SHGetPathFromIDList((LPCITEMIDLIST)lParam, curr);
		::SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)curr);
	}
	break;
	}
	return   0;
}

void CGenVelocityDlg::ReadData()
{
	//IMU
	string ssTmp = m_ssPath + "\\imu.csv";
	m_imuData.readData(ssTmp);

	//GPS
	ssTmp = m_ssPath + "\\gps.csv";
	m_gpsData.readData(ssTmp);

	//Velocity
	ssTmp = m_ssPath + "\\speed.csv";
	m_velocityData.readData(ssTmp);

	//Left Images
	ssTmp = m_ssPath + "\\l";
	m_imgLeft.recordPaths(ssTmp);

	//Medium Images
	ssTmp = m_ssPath + "\\m";
	m_imgMedium.recordPaths(ssTmp);

	//Right Imgaes
	ssTmp = m_ssPath + "\\r";
	m_imgRight.recordPaths(ssTmp);

	//sync time stamp
	size_t i = 0;
	const size_t& ullStartFrame = max(max(m_imgLeft.timeStamp.at(0), m_imgMedium.timeStamp.at(0)),
		m_imgRight.timeStamp.at(0));
	for (i = 0; i < m_velocityData.timeStamp.size(); ++i)
	{
		if (m_velocityData.timeStamp[i] >= ullStartFrame)
		{
			m_ullVelocityStart = i;
			break;
		}
	}
}

void CGenVelocityDlg::CalDistance()
{
	int i = 0;

	float fPath = 0;
	double fTime = 0;
	m_velocityData.distance.resize(m_velocityData.timeStamp.size());
	for (i = 0; i <= m_ullVelocityStart; ++i)
	{
		m_velocityData.distance[i] = 0;
	}
	for (i = m_ullVelocityStart + 1; i < m_velocityData.timeStamp.size(); ++i)
	{
		//update
		fTime = (m_velocityData.timeStamp[i] - m_velocityData.timeStamp[i - 1]) / powf(10, 9);
		fPath += m_velocityData.velocity[i - 1] * fTime;

		m_velocityData.distance[i] = fPath;
	}


	//distance of IMU
	int nIndex = m_ullVelocityStart;
	m_imuData.distance.resize(m_imuData.timeStamp.size());
	for (i = 0; i < m_imuData.timeStamp.size(); ++i)
	{
		if (m_imuData.timeStamp[i] <= m_velocityData.timeStamp[m_ullVelocityStart])
		{
			m_imuData.distance[i] = 0;
		}
		else
		{
			double fTime = 0;

			if (m_imuData.timeStamp[i] > m_velocityData.timeStamp[m_velocityData.timeStamp.size() - 1] &&
				m_imuData.timeStamp[i - 1] <= m_velocityData.timeStamp[m_velocityData.timeStamp.size() - 1])
			{
				fTime = (m_imuData.timeStamp[i] - m_velocityData.timeStamp[m_velocityData.timeStamp.size() - 1]) / powf(10, 9);
				m_imuData.distance[i] = m_velocityData.distance[m_velocityData.timeStamp.size() - 1] +
					abs(m_velocityData.velocity[m_velocityData.timeStamp.size() - 1] * fTime);
			}
			else if (m_imuData.timeStamp[i - 1] > m_velocityData.timeStamp[m_velocityData.timeStamp.size() - 1])
			{
				fTime = (m_imuData.timeStamp[i] - m_imuData.timeStamp[i - 1]) / powf(10, 9);
				m_imuData.distance[i] = m_imuData.distance[i - 1] +
					abs(m_velocityData.velocity[m_velocityData.timeStamp.size() - 1] * fTime);
			}
			else
			{
				for (int j = nIndex; j <= m_velocityData.timeStamp.size() - 1; ++j)
				{
					if (m_imuData.timeStamp[i] > m_velocityData.timeStamp[j])
						continue;

					nIndex = j - 1 > 0 ? j - 1 : 0;
					fTime = (m_imuData.timeStamp[i] - m_velocityData.timeStamp[nIndex]) / powf(10, 9);		//nano-second to second
					m_imuData.distance[i] = m_velocityData.distance[nIndex] + abs(m_velocityData.velocity[nIndex] * fTime);

					break;
				}
			}
		}
	}
	
	//distance of left images
	nIndex = m_ullVelocityStart;
	m_imgLeft.distance.resize(m_imgLeft.timeStamp.size());
	m_imgLeft.velocity.resize(m_imgLeft.timeStamp.size());
	for (int i = 0; i < m_imgLeft.timeStamp.size(); ++i)
	{
		if (m_imgLeft.timeStamp[i] <= m_velocityData.timeStamp[m_ullVelocityStart])
		{
			m_imgLeft.distance[i] = 0;
			m_imgLeft.velocity[i] = 0;
		}
		else
		{
			double fTime = 0;

			if (m_imgLeft.timeStamp[i] > m_velocityData.timeStamp[m_velocityData.timeStamp.size() - 1] &&
				m_imgLeft.timeStamp[i - 1] <= m_velocityData.timeStamp[m_velocityData.timeStamp.size() - 1])
			{
				fTime = (m_imgLeft.timeStamp[i] - m_velocityData.timeStamp[m_velocityData.timeStamp.size() - 1]) / powf(10, 9);
				m_imgLeft.distance[i] = m_velocityData.distance[m_velocityData.timeStamp.size() - 1]
					+ abs(m_velocityData.velocity[m_velocityData.timeStamp.size() - 1] * fTime);
				m_imgLeft.velocity[i] = m_velocityData.velocity[m_velocityData.timeStamp.size() - 1];
			}
			else if (m_imgLeft.timeStamp[i - 1] > m_velocityData.timeStamp[m_velocityData.timeStamp.size() - 1])
			{
				fTime = (m_imgLeft.timeStamp[i] - m_imgLeft.timeStamp[i - 1]) / powf(10, 9);
				m_imgLeft.distance[i] = m_imgLeft.distance[i - 1] +
					abs(m_velocityData.velocity[m_velocityData.timeStamp.size() - 1] * fTime);
				m_imgLeft.velocity[i] = m_velocityData.velocity[m_velocityData.timeStamp.size() - 1];
			}
			else
			{
				for (int j = nIndex; j <= m_velocityData.timeStamp.size() - 1; ++j)
				{
					if (m_imgLeft.timeStamp[i] > m_velocityData.timeStamp[j])
						continue;

					nIndex = j - 1 > 0 ? j - 1 : 0;//update index
					fTime = (m_imgLeft.timeStamp[i] - m_velocityData.timeStamp[nIndex]) / powf(10, 9);		//nano-second to second
					m_imgLeft.distance[i] = m_velocityData.distance[nIndex] + abs(m_velocityData.velocity[nIndex] * fTime);
					m_imgLeft.velocity[i] = m_velocityData.velocity[nIndex];
					break;
				}
			}
		}
	}

	//distance of medium images
	nIndex = m_ullVelocityStart;
	m_imgMedium.distance.resize(m_imgMedium.timeStamp.size());
	m_imgMedium.velocity.resize(m_imgMedium.timeStamp.size());
	for (int i = 0; i < m_imgMedium.timeStamp.size(); ++i)
	{
		if (m_imgMedium.timeStamp[i] <= m_velocityData.timeStamp[m_ullVelocityStart])
		{
			m_imgMedium.distance[i] = 0;
			m_imgMedium.velocity[i] = 0;
		}
		else
		{
			double fTime = 0;
			if (m_imgMedium.timeStamp[i] > m_velocityData.timeStamp[m_velocityData.timeStamp.size() - 1] &&
				m_imgMedium.timeStamp[i - 1] <= m_velocityData.timeStamp[m_velocityData.timeStamp.size() - 1])
			{
				fTime = (m_imgMedium.timeStamp[i] - m_velocityData.timeStamp[m_velocityData.timeStamp.size() - 1]) / powf(10, 9);
				
				m_imgMedium.distance[i] = m_velocityData.distance[m_velocityData.timeStamp.size() - 1]
					+ abs(m_velocityData.velocity[m_velocityData.timeStamp.size() - 1] * fTime);

				m_imgMedium.velocity[i] = m_velocityData.velocity[m_velocityData.timeStamp.size() - 1];
			}
			else if (m_imgMedium.timeStamp[i - 1] > m_velocityData.timeStamp[m_velocityData.timeStamp.size() - 1])
			{
				fTime = (m_imgMedium.timeStamp[i] - m_imgMedium.timeStamp[i - 1]) / powf(10, 9);
				m_imgMedium.distance[i] = m_imgMedium.distance[i - 1] +
					abs(m_velocityData.velocity[m_velocityData.timeStamp.size() - 1] * fTime);
				m_imgMedium.velocity[i] = m_velocityData.velocity[m_velocityData.timeStamp.size() - 1];
			}
			else
			{
				for (int j = nIndex; j <= m_velocityData.timeStamp.size() - 1; ++j)
				{
					if (m_imgMedium.timeStamp[i] > m_velocityData.timeStamp[j])
						continue;

					nIndex = j - 1 > 0 ? j - 1 : 0;//update index
					fTime = (m_imgMedium.timeStamp[i] - m_velocityData.timeStamp[nIndex]) / powf(10, 9);		//nano-second to second
					m_imgMedium.distance[i] = m_velocityData.distance[nIndex] + abs(m_velocityData.velocity[nIndex] * fTime);
					m_imgMedium.velocity[i] = m_velocityData.velocity[nIndex];
					break;

				}
			}
		}
	}

	//distance of right images
	nIndex = m_ullVelocityStart;
	m_imgRight.distance.resize(m_imgRight.timeStamp.size());
	m_imgRight.velocity.resize(m_imgRight.timeStamp.size());
	for (int i = 0; i < m_imgRight.timeStamp.size(); ++i)
	{
		if (m_imgRight.timeStamp[i] <= m_velocityData.timeStamp[m_ullVelocityStart])
		{
			m_imgRight.distance[i] = 0;
			m_imgRight.velocity[i] = 0;
		}
		else
		{
			double fTime = 0;
			if (m_imgRight.timeStamp[i] > m_velocityData.timeStamp[m_velocityData.timeStamp.size() - 1] &&
				m_imgRight.timeStamp[i - 1] <= m_velocityData.timeStamp[m_velocityData.timeStamp.size() - 1])
			{
				fTime = (m_imgRight.timeStamp[i] - m_velocityData.timeStamp[m_velocityData.timeStamp.size() - 1]) / powf(10, 9);
				m_imgRight.distance[i] = m_velocityData.distance[m_velocityData.timeStamp.size() - 1]
					+ abs(m_velocityData.velocity[m_velocityData.timeStamp.size() - 1] * fTime);
				m_imgRight.velocity[i] = m_velocityData.velocity[m_velocityData.timeStamp.size() - 1];
			}
			else if (m_imgRight.timeStamp[i - 1] > m_velocityData.timeStamp[m_velocityData.timeStamp.size() - 1])
			{
				fTime = (m_imgRight.timeStamp[i] - m_imgRight.timeStamp[i - 1]) / powf(10, 9);
				m_imgRight.distance[i] = m_imgRight.distance[i - 1] +
					abs(m_velocityData.velocity[m_velocityData.timeStamp.size() - 1] * fTime);
				m_imgRight.velocity[i] = m_velocityData.velocity[m_velocityData.timeStamp.size() - 1];
			}
			else
			{
				for (int j = nIndex; j <= m_velocityData.timeStamp.size() - 1; ++j)
				{
					if (m_imgRight.timeStamp[i] > m_velocityData.timeStamp[j])
						continue;

					nIndex = j-1 > 0 ? j-1 : 0;//update index
					fTime = (m_imgRight.timeStamp[i] - m_velocityData.timeStamp[nIndex]) / powf(10, 9);		//nano-second to second
					m_imgRight.distance[i] = m_velocityData.distance[nIndex] + abs(m_velocityData.velocity[nIndex] * fTime);
					m_imgRight.velocity[i] = m_velocityData.velocity[nIndex];
					break;
				}
			}
		}
	}
}

void CGenVelocityDlg::WriteData()
{
	CalDistance();

	//left image
	string ssTmp = m_ssPath + "\\l.txt";
	m_ofLeft.open(ssTmp);
	for (int i = 0; i < m_imgLeft.timeStamp.size(); ++i)
	{
		m_ofLeft << m_imgLeft.distance[i] << "," 
			<< m_imgLeft.velocity[i] << "\n";
	}

	//medium image
	ssTmp = m_ssPath + "\\m.txt";
	m_ofMedium.open(ssTmp);
	for (int i = 0; i < m_imgMedium.timeStamp.size(); ++i)
	{
		m_ofMedium << m_imgMedium.distance[i] << ","
			<< m_imgMedium.velocity[i] << "\n";
	}

	//right image
	ssTmp = m_ssPath + "\\r.txt";
	m_ofRight.open(ssTmp);
	for (int i = 0; i < m_imgRight.timeStamp.size(); ++i)
	{
		m_ofRight << m_imgRight.distance[i] << ","
			<< m_imgRight.velocity[i] << "\n";
	}

	//imu
	ssTmp = m_ssPath + "\\imu.txt";
	m_ofImu.open(ssTmp);
	for (int i = 0; i < m_imuData.timeStamp.size(); ++i)
	{
		m_ofImu << m_imuData.distance[i] << "\n";
	}

	m_ofImu.close();
	m_ofLeft.close();
	m_ofMedium.close();
	m_ofRight.close();
}

//generate files
void CGenVelocityDlg::OnBnClickedButton1()
{
	ReadData();
	WriteData();
	MessageBox(TEXT("Done!"), MB_OK);
	m_btn.EnableWindow(false);
}


//change path
void CGenVelocityDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR pszPath[MAX_PATH];
	BROWSEINFO bi;
	bi.pidlRoot = NULL;
	bi.hwndOwner = this->GetSafeHwnd();
	bi.pszDisplayName = NULL;
	bi.lpszTitle = TEXT("Select trajectory folder:");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = 0;
	bi.iImage = 0;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl == NULL)
		return;

	string ssRoot;
	if (SHGetPathFromIDList(pidl, pszPath))
	{
		m_editPath.SetWindowTextW(pszPath);
		m_csPath = pszPath;
		m_ssPath = CT2A(m_csPath.GetBuffer());
		m_csPath.ReleaseBuffer();

		m_btn.EnableWindow(true);
		Invalidate(FALSE);
		UpdateWindow();
	}
}
