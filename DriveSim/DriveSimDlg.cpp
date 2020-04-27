
// DriveSimDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DriveSim.h"
#include "DriveSimDlg.h"
#include "afxdialogex.h"
#include "dib.hpp"
#include <fstream>
#include "winsock.h"

#include <Mmsystem.h>
#include <afxmt.h>

#pragma comment(lib, "winmm.lib")

JOYINFO joyinfo;
JOYINFOEX joyinfo2;

int JoyMsg = 0;

double g_dJoyXMax = 0;
double g_dJoyXMin = 0;

double g_dJoyYMax = 0;
double g_dJoyYMin = 0;

double g_dJoyRMax = 0;
double g_dJoyRMin = 0;

double g_dSignDist = 0;
double g_dSignDistEnd = 0;
int    g_nSignShowTime = 1;

CString g_strJoyOutput;

double g_dRealSpeed = 0;
double g_nMinSpeed = 0;
double g_nMaxSpeed = 100;


LARGE_INTEGER g_liPerfFreq;
LARGE_INTEGER g_liPerfStart;

CArray<Point2d, Point2d&> g_arrTurnScope;
CArray<int, int> g_arrTurnType;

CMutex g_mutxDriver;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace cv;
using namespace std;

#define TIMER_FAST  123		
#define TIMER_LOW   321		

CDriveSimDlg* g_pWnd = NULL;


SOCKADDR_IN addrSrv;

struct UDP_MSG
{
	unsigned int time;
	float av1 = 0;
	float av2 = 0;
	float av3 = 0;

	float heading = 0;
	float pitch = 0;
	float roll = 0;

	float acx = 0;
	float acy = 0;
	float acz = 0;

	float vex = 0;
	float vey = 0;
	float vez = 0;

	int pox = 0;
	int poy = 0;
	int poz = 0;
	int id;
} updmsg;


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


// CDriveSimDlg 对话框



CDriveSimDlg::CDriveSimDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DRIVESIM_DIALOG, pParent)
{
	m_pThreadTimer = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	g_pWnd = this;
}

void CDriveSimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON3, m_btnImu);
	DDX_Control(pDX, IDC_BUTTON4, m_btnGps);
	DDX_Control(pDX, IDC_BUTTON5, m_btnVelocity);
	DDX_Control(pDX, IDC_BUTTON7, m_btnSart);
	DDX_Control(pDX, IDC_BUTTON6, m_btnEnd);
	DDX_Control(pDX, IDC_BUTTON8, m_btnImages);
	DDX_Control(pDX, IDC_EDIT1, m_editImu);
	DDX_Control(pDX, IDC_EDIT3, m_editGps);
	DDX_Control(pDX, IDC_EDIT2, m_editVelocity);
	DDX_Control(pDX, IDC_EDIT4, m_editImages);
}

BEGIN_MESSAGE_MAP(CDriveSimDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON3, &CDriveSimDlg::OnBnClickedIMU)
	ON_BN_CLICKED(IDC_BUTTON4, &CDriveSimDlg::OnBnClickedGPS)
	ON_BN_CLICKED(IDC_BUTTON5, &CDriveSimDlg::OnBnClickedVelocity)
	ON_BN_CLICKED(IDC_BUTTON8, &CDriveSimDlg::OnBnClickedImages)
	ON_BN_CLICKED(IDC_BUTTON7, &CDriveSimDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_BUTTON6, &CDriveSimDlg::OnBnClickedEnd)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_TIMER()
	ON_MESSAGE( MM_JOY1BUTTONDOWN, &CDriveSimDlg::OnJoyDown )
	ON_MESSAGE(MM_JOY1MOVE, &CDriveSimDlg::OnJoyMove )
	ON_WM_DESTROY()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDriveSimDlg 消息处理程序

BOOL CDriveSimDlg::OnInitDialog()
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

	// TODO: 	
	WSADATA data;
	WSAStartup( MAKEWORD( 2, 2 ), &data );

	JoyMsg = joySetCapture( GetSafeHwnd(), JOYSTICKID1, 0, false );

	JOYCAPS jc;
	joyGetDevCaps( JOYSTICKID1, &jc, sizeof( jc ) );
	g_dJoyXMax = jc.wXmax; // oil
	g_dJoyXMin = jc.wXmin;
	g_dJoyYMax = jc.wYmax; // brake
	g_dJoyYMin = jc.wYmin;
	g_dJoyRMax = jc.wRmax;
	g_dJoyRMin = jc.wRmin;



	memset( &updmsg, 0, sizeof( updmsg ) );

	addrSrv.sin_addr.S_un.S_addr = inet_addr( "127.0.0.1" );
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons( 5678 );


	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	TCHAR *pChr = _tcsrchr(szFilePath, _T('\\'));
	pChr++;
	*pChr = _T('\0');
	m_csRoot = szFilePath;
	
	g_strJoyOutput = m_csRoot;
	

	InitializeData();

	m_dlgLeft = new CDialogLeft(0, 0, this);   //the Left Window
	m_dlgLeft->Create(IDD_DIALOG_LEFT);
	m_dlgLeft->ShowWindow(SW_SHOWMAXIMIZED);
	
	m_dlgLeft->m_bMidWnd = FALSE;
	m_dlgLeft->m_bStop = TRUE;
	
	m_dlgMedium = new CDialogLeft(1920, 0, this);
	m_dlgMedium->Create(IDD_DIALOG_LEFT);
	m_dlgMedium->ShowWindow(SW_SHOWMAXIMIZED);
	
	m_dlgMedium->m_bMidWnd = TRUE;
	m_dlgMedium->m_bStop = TRUE;
	m_dlgMedium->m_strText = "Push button when ready";
	m_dlgMedium->Invalidate(FALSE);
	m_dlgMedium->UpdateWindow();

	m_dlgRight = new CDialogLeft(5460, 0, this);
	m_dlgRight->Create(IDD_DIALOG_LEFT);
	m_dlgRight->ShowWindow(SW_SHOWMAXIMIZED);	
	
	return TRUE; 
}

LRESULT CDriveSimDlg::OnJoyMove( WPARAM w, LPARAM l )
{
	JOYINFOEX joyInfoEx;
	ZeroMemory( &joyInfoEx, sizeof( joyInfoEx ) );

	joyInfoEx.dwSize = sizeof( joyInfoEx );
	joyInfoEx.dwFlags = JOY_RETURNALL;

	joyGetPosEx( JOYSTICKID1, &joyInfoEx );
	
	
	int nR = joyInfoEx.dwRpos;
	nR = g_dJoyRMax - joyInfoEx.dwRpos;
	if ( abs(nR) > 3 )
	{
		m_dAcc = 0 - (double)(g_dJoyRMax - joyInfoEx.dwRpos) * 40 / (g_dJoyRMax - g_dJoyRMin);
	}
	else
	{
		if( g_dJoyYMax - joyInfoEx.dwYpos > 3)
			m_dAcc = 10 * pow((g_dJoyYMax - joyInfoEx.dwYpos) / (g_dJoyYMax - g_dJoyYMin),2);

		m_dAcc -= 0.03;
	}	

	if ( m_pThreadTimer )
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		CString strTemp;
		strTemp.Format(_T("%ld-%ld-%ld:%ld:%ld.%ld	%ld,%ld,%ld\n"),
			st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
			joyInfoEx.dwXpos, joyInfoEx.dwYpos, joyInfoEx.dwRpos);

		g_mutxDriver.Lock();
		if ( m_fileDriver.m_hFile != CFile::hFileNull )
			m_fileDriver.WriteString(strTemp);
		g_mutxDriver.Unlock();
	}	
	return S_OK;
}

LRESULT CDriveSimDlg::OnJoyDown( WPARAM w, LPARAM l )
{
	static int nState = -1;

	if ( w & JOY_BUTTON3 )
	{
		if (nState == -1)
		{
			OnBnClickedStart();
			nState = 1;
		}
	}
	if ( w & JOY_BUTTON4 )
	{
		OnBnClickedEnd();
		nState = -1;
	}
	return S_OK;
}


void CDriveSimDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDriveSimDlg::OnPaint()
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
HCURSOR CDriveSimDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//Rough estimation method for transform gps coordinate to distance coordinate
void GPS2Distance(const Point2f& origin, const vector<Point2f>& gps, vector<Point2f>& distance)
{
	//Point2f:: x~latitude, y~longitude
	//the latitude of oringin is regard as the standard

	const float fRadius = 6371000;					// radius of the earth,the unit is meter.
	float fDistancePerLat = CV_PI * fRadius / 180;		//meters per latitude
	float fDistancePerLgn = CV_PI * fRadius / 180 * cosf(origin.x * CV_PI / 180);

	float fLatDistance, fLngDistance;
	for (auto it = gps.begin(); it != gps.end(); ++it)
	{
		fLatDistance = (it->x - origin.x) * fDistancePerLat;
		fLngDistance = (it->y - origin.y) * fDistancePerLgn;
		distance.push_back(Point2f(fLatDistance, fLngDistance));
	}
}

//Initialize all the members
void CDriveSimDlg::InitializeData()
{
	m_nCurrent = 0;
	m_vecPath.clear();
	m_bStarted = false;
	m_dAcc = 0;
	m_fSimVelocity = 0;    //test
	m_fSimDistance = 0;

	m_img.release();

	if (m_dlgLeft) delete m_dlgLeft;
	m_dlgLeft = nullptr;
	if (m_dlgMedium) delete m_dlgMedium;
	m_dlgMedium = nullptr;
	if (m_dlgRight) delete m_dlgRight;
	m_dlgRight = nullptr;


	if (m_pThreadProcL)		delete m_pThreadProcL;
	m_pThreadProcL = nullptr;
	if (m_pThreadProcM)		delete m_pThreadProcM;
	m_pThreadProcM = nullptr;
	if (m_pThreadProcR)		delete m_pThreadProcR;
	m_pThreadProcR = nullptr;
	if (m_pThreadRecv)		delete m_pThreadRecv;
	m_pThreadRecv = nullptr;

	g_liPerfFreq = { 0 };
	g_liPerfStart = { 0 };

	//GenSignOrder();	  //Set sign showing order
	LoadSignOrder();
	m_nCurrent = 0;

	m_btnEnd.EnableWindow(false);
}


void CDriveSimDlg::OnBnClickedIMU()
{
	CString filepath = m_csRoot;
	CFileDialog fdlg(TRUE, filepath, NULL,
		OFN_HIDEREADONLY | OFN_READONLY, NULL, NULL);
	if (fdlg.DoModal() == IDOK)
	{
		m_ImuPath = fdlg.GetPathName();
		m_editImu.SetWindowText(m_ImuPath);
		Invalidate(FALSE);
		UpdateWindow();
	}
}

void CDriveSimDlg::OnBnClickedGPS()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filepath = m_csRoot;
	CFileDialog fdlg(TRUE, filepath, NULL,
		OFN_HIDEREADONLY | OFN_READONLY, NULL, NULL);
	if (fdlg.DoModal() == IDOK)
	{
		m_GpsPath = fdlg.GetPathName();
		m_editGps.SetWindowText(m_GpsPath);
		Invalidate(FALSE);
		UpdateWindow();
	}
}

void CDriveSimDlg::OnBnClickedVelocity()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filepath = m_csRoot;
	CFileDialog fdlg(TRUE, filepath, NULL,
		OFN_HIDEREADONLY | OFN_READONLY, NULL, NULL);
	if (fdlg.DoModal() == IDOK)
	{
		m_VelocityPath = fdlg.GetPathName();
		m_editVelocity.SetWindowText(m_VelocityPath);
		Invalidate(FALSE);
		UpdateWindow();
	}
}

void CDriveSimDlg::OnBnClickedImages()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR pszPath[MAX_PATH];
	BROWSEINFO bi;
	bi.hwndOwner = GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = TEXT("Select trajectory folder:");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	if (pidl == NULL)
		return;

	string ssRoot;
	if (SHGetPathFromIDList(pidl, pszPath))
	{
		m_ImgsPathLeft = pszPath;
		m_editImages.SetWindowText(m_VelocityPath);
		Invalidate(FALSE);
		UpdateWindow();
	}
}

void CDriveSimDlg::LoadSignOrder()
{
	m_vecSignSort.RemoveAll();

	CString strFileName;
	strFileName.Format("%s\\order.txt", m_csRoot);
	CFileFind finder;
	if (!finder.FindFile(strFileName))
		return;
	
	CStdioFile File(strFileName,CFile::modeRead);
	CString strTemp;
	while (File.ReadString(strTemp))
	{
		strTemp.TrimLeft();
		strTemp.TrimRight();
		if (strTemp.GetLength() < 1) continue;
		m_vecSignSort.Add(strTemp);
	}
	File.Close();
}
void CDriveSimDlg::GenSignOrder()
{/*
	m_vecSignSort
	m_vecSignSort.resize(mc_nSignNum);	

	if (mc_nSignNum == 1)
	{
		m_vecSignSort[0] = 0;
	}
	else
	{
		int i;
		long lT = GetTickCount();
		srand(lT);
		int nMax = m_vecSignSort.size() - 1;
		for (i = 0; i <= nMax; ++i)
			m_vecSignSort[i] = i;
		for (i = nMax; i >= 1; --i)
			swap(m_vecSignSort[i], m_vecSignSort[rand() % (i+1)]);
	}
	
	//记录sign顺序
	string ssTmp = m_csRoot + "sign_order.txt";
	ofstream of(ssTmp);
	for (int i = 0; i < m_vecSignSort.size() - 1; ++i)
	{
		of << m_vecSignSort[i] << ",";
	}
	of << m_vecSignSort[m_vecSignSort.size() - 1] << "\n";
	of.close();

	m_nCurrent = 0;*/
}

void CDriveSimDlg::ReadData()
{
	CString strFileName;

	//IMU
	string ssTmp = m_ssPath + "\\imu.csv";
	m_imuData.readData(ssTmp);

	//Left Images
	ssTmp = m_ssPath + "\\l";
	m_imgLeft.recordPaths(ssTmp);

	//Medium Images
	ssTmp = m_ssPath + "\\m";
	m_imgMedium.recordPaths(ssTmp);
		
	strFileName.Format("%s\\m\\signpos.txt", m_ssPath.c_str());
	m_imgMedium.loadsignpos(strFileName);

	//Right Imgaes
	ssTmp = m_ssPath + "\\r";
	m_imgRight.recordPaths(ssTmp);

	int i = 0;
	string ssContent;
	ifstream iFile;

	//l
	assert(m_imgLeft.distance.empty() 
		&& m_imgLeft.velocity.empty());
	ssTmp = m_ssPath + "\\l.txt";
	iFile.open(ssTmp);
	while (getline(iFile, ssContent))
	{
		vector<string> tmp = split(ssContent, ",");
		if (i < m_imgLeft.timeStamp.size())
		{
			m_imgLeft.distance.push_back(stof(tmp[0]));
			m_imgLeft.velocity.push_back(stof(tmp[1]));
			i ++;
		}
		else
			break;
	}
	iFile.close();

	//m
	i = 0;
	assert(m_imgMedium.distance.empty()
		&& m_imgMedium.velocity.empty());
	ssTmp = m_ssPath + "\\m.txt";
	iFile.open(ssTmp);
	while (getline(iFile, ssContent))
	{
		vector<string> tmp = split(ssContent, ",");
		if (i < m_imgMedium.timeStamp.size())
		{
			m_imgMedium.distance.push_back(stof(tmp[0]));
			m_imgMedium.velocity.push_back(stof(tmp[1]));
			i++;
		}
		else
			break;
	}
	iFile.close();

	//r
	i = 0;
	assert(m_imgRight.distance.empty()
		&& m_imgRight.velocity.empty());
	ssTmp = m_ssPath + "\\r.txt";
	iFile.open(ssTmp);
	while (getline(iFile, ssContent))
	{
		vector<string> tmp = split(ssContent, ",");
		if (i < m_imgRight.timeStamp.size())
		{
			m_imgRight.distance.push_back(stof(tmp[0]));
			m_imgRight.velocity.push_back(stof(tmp[1]));
			i++;
		}
		else
			break;
	}
	iFile.close();

	//imu
	i = 0;
	assert(m_imuData.distance.empty());
	ssTmp = m_ssPath + "\\imu.txt";
	iFile.open(ssTmp);
	while (getline(iFile, ssContent))
	{
		if (i < m_imuData.timeStamp.size())
		{
			m_imuData.distance.push_back(stof(ssContent));
			i++;
		}
		else
			break;
	}
	iFile.close();

	/*{
		m_dlgMedium->m_pAngleX.clear();
		m_dlgMedium->m_pAngleY.clear();
		m_dlgMedium->m_pAngleZ.clear();

		for ( int i = 0; i < m_imuData.av_x.size(); i++ )
		{
			int nCount = 0;
			double x = 0, y = 0, z = 0;
			for (int j = MAX(0, i - 1); j < MIN(m_imuData.av_x.size(), i+1); j++)
			{
				x += m_imuData.heading[j];
				y += m_imuData.roll[j];
				z += m_imuData.pitch[j];
				nCount++;
			}
			x /= nCount;
			y /= nCount;
			z /= nCount;

			m_dlgMedium->m_pAngleX.push_back( x );
			m_dlgMedium->m_pAngleY.push_back( y );
			m_dlgMedium->m_pAngleZ.push_back( z );
		}
	}*/

	// max speed
	g_nMinSpeed = 0;
	g_nMaxSpeed = 75;

	ssTmp = m_ssPath + "\\speedlimit.txt";
	strFileName.Format("%s", ssTmp.c_str());
	CFileFind finder;
	if (finder.FindFile(strFileName))
	{
		iFile.open(ssTmp);
		getline(iFile, ssContent);
		g_nMinSpeed = stoi(ssContent);
		getline(iFile, ssContent);
		g_nMaxSpeed = stoi(ssContent);
		iFile.close();
	}

	// change lane.txt
	g_arrTurnScope.RemoveAll();
	g_arrTurnType.RemoveAll();

  	ssTmp = m_ssPath + "\\change_lane.txt";
	strFileName.Format("%s", ssTmp.c_str());
	if (finder.FindFile(strFileName))
	{
		CString strTemp;
		CStdioFile File(strFileName,CFile::modeRead);
		while ( File.ReadString(strTemp) )
		{			
			g_arrTurnType.Add(atol(strTemp));

			if (!File.ReadString(strTemp)) break;
			ssContent = format("%s",strTemp);
			ssContent = m_ssPath + "\\m\\" + ssContent + ".bmp";
			double dStart = m_imgMedium.getImgDist(ssContent);

			if (!File.ReadString(strTemp)) break;
			ssContent = format("%s", strTemp);
			ssContent = m_ssPath + "\\m\\" + ssContent + ".bmp";
			double dEnd = m_imgMedium.getImgDist(ssContent);

			g_arrTurnScope.Add(Point2d(dStart, dEnd));     
		}

		iFile.close();
	}
	
	{
		string fileName = m_ssPath + "\\sign_pos.txt";
		ifstream iFile(fileName);
		string ssTmp1;
		getline(iFile, ssTmp1);
		size_t ullTime1 = stoull(ssTmp1);
		getline(iFile, ssTmp1);
		size_t ullTime2 = stoull(ssTmp1);
		getline(iFile, ssTmp1);
		g_nSignShowTime = stoull(ssTmp1);
		iFile.close();

		g_nSignShowTime = MAX(1, g_nSignShowTime);

		g_dSignDist = 0;
		for (int i = 0; i < m_imgMedium.timeStamp.size(); ++i)
		{
			if (ullTime1 > m_imgMedium.timeStamp[i]) continue;

			g_dSignDist = m_imgMedium.distance[i];
			break;
		}
		for (int i = 0; i < m_imgMedium.timeStamp.size(); ++i)
		{
			if (ullTime2 > m_imgMedium.timeStamp[i]) continue;

			g_dSignDistEnd = m_imgMedium.distance[i];
			break;
		}
	}
}

int CDriveSimDlg::Process( double dSimDistance, CImageData* pImgData, CDialogLeft* pDlgShow )
{
	string ssPath2Show = "";

	int nX=-1, nY=-1;
	float fSugVelosity = 0;
	int nIndex = -1;
	for (size_t i = 0; i < pImgData->timeStamp.size(); ++i)
	{
		if ( dSimDistance < pImgData->distance[i] )
			break;
	
		pImgData->nCurFrameID = i + 1;  //update, to decrease the processing time
		ssPath2Show.assign( pImgData->imgPath[i]);
		fSugVelosity = pImgData->velocity[i];
		nX = pImgData->signposx[i];
		nY = pImgData->signposy[i];
		nIndex = i;
	}

	int nShowDirect = 0;
	for (int i = 0; i < g_arrTurnScope.GetCount(); i++)
	{
		if (dSimDistance < g_arrTurnScope[i].x || dSimDistance > g_arrTurnScope[i].y) continue;
		nShowDirect = g_arrTurnType[i];
		break;
	}

	int nInSignRange = -1;
    if (g_dSignDist < g_pWnd->m_fSimDistance)	nInSignRange = g_nMinSpeed;

	//if image changed
	if (nIndex < 0)//&& m_fSimVelocity > 0.1 )
	{
		if( m_fSimVelocity < 1)
			pDlgShow->UpdateImage(ssPath2Show, fSugVelosity, m_fSimVelocity, nShowDirect,nX,nY, nInSignRange);
		return -1;
	}
			
	if( pDlgShow == m_dlgMedium )
		g_dRealSpeed = fSugVelosity;

	pDlgShow->UpdateImage(ssPath2Show, fSugVelosity, m_fSimVelocity, nShowDirect, nX, nY, nInSignRange);

	if (pDlgShow == m_dlgMedium)
		return nIndex;
	return -1;
}

void CDriveSimDlg::SendImu2Sim()
{
	SOCKET sockClient = socket( AF_INET, SOCK_DGRAM, 0 );

	int len = sizeof( SOCKADDR );
	
	int nID = m_imuData.nCurID;

	int nCount = 0;
	double x=0, y=0, z=0;
	for ( int i = MAX( 0, nID - 20 ); i < MIN( m_imuData.av_x.size(), nID + 21 ); i++ )
	{
		x += m_imuData.heading[i];
		y += m_imuData.roll[i];
		z += m_imuData.pitch[i];
		nCount++;
	}	

	x /= nCount;
	y /= nCount;
	z /= nCount;

	static double dAcc = 0;
	if (m_dAcc < 0) 
		dAcc = dAcc * 0.5 + m_dAcc * 0.2*0.5;
	else
		dAcc = dAcc * 0.5 + m_dAcc * 1 * 0.5;

	updmsg.heading = (m_imuData.heading[0]   - x);
	updmsg.roll    = (y    - m_imuData.roll[0]);
	updmsg.pitch   = (z   - m_imuData.pitch[0]) + dAcc * 0.002;
	

	//
	updmsg.time++;
	sendto( sockClient, (char*)&updmsg, sizeof( UDP_MSG ), 0, (SOCKADDR*) &addrSrv, len );
	
	closesocket( sockClient );
}

double CDriveSimDlg::GetSignPosDis()
{
	
	return 0;
}

UINT CDriveSimDlg::ThreadTimer( void* param )
{
	CDriveSimDlg* pWnd = (CDriveSimDlg*) param;

	BOOL bFirst = TRUE;
	int nMultiple = 0;
	const int& nProcInterv = 10;

	string strFileName = g_pWnd->m_ssPath + "\\drivermotion.txt";
	CString stra(strFileName.c_str());
	g_mutxDriver.Lock();
	g_pWnd->m_fileDriver.Open( stra, CFile::modeCreate | CFile::modeWrite );
	g_mutxDriver.Unlock();	

	QueryPerformanceCounter(&g_liPerfStart);
	QueryPerformanceFrequency(&g_liPerfFreq);
	g_pWnd->m_fSimVelocity = 0;
	g_pWnd->m_fSimDistance = 10.1;

	g_pWnd->m_dlgLeft->m_bStop   = FALSE;
	g_pWnd->m_dlgMedium->m_bStop = FALSE;
	g_pWnd->m_dlgRight->m_bStop  = FALSE;	
		
	size_t lasttime = 0;
	int nLastSend = 0;
	while ( pWnd->m_bStarted )
	{
		LARGE_INTEGER liPerfNow = { 0 };
		QueryPerformanceCounter( &liPerfNow );
		size_t time = ( ( ( liPerfNow.QuadPart - g_liPerfStart.QuadPart ) *	1000 ) / g_liPerfFreq.QuadPart );
		
		if ( time < nMultiple * nProcInterv ) continue;

		g_pWnd->m_fSimVelocity += g_pWnd->m_dAcc * ( time - lasttime ) / 1000.0;
		if (g_pWnd->m_fSimVelocity < 0)
			g_pWnd->m_fSimVelocity = 0;

		if (bFirst && g_pWnd->m_fSimVelocity > g_nMinSpeed)
			bFirst = FALSE;

		if(!bFirst)
			g_pWnd->m_fSimVelocity = MAX(g_nMinSpeed, g_pWnd->m_fSimVelocity);

		g_pWnd->m_fSimVelocity = MIN(g_nMaxSpeed, g_pWnd->m_fSimVelocity);

		if (g_dSignDist < g_pWnd->m_fSimDistance )
		{
			g_pWnd->m_fSimVelocity = (g_dSignDistEnd - g_dSignDist) / g_nSignShowTime;
			//g_pWnd->m_fSimVelocity = g_dRealSpeed;
		}

		g_pWnd->m_fSimDistance += g_pWnd->m_fSimVelocity * ( time - lasttime ) / 1000.0;  //the distance of Simulater
		lasttime = time;		

		for ( int i = 0; i < g_pWnd->m_imuData.acc_x.size(); i++ )
		{
			if ( g_pWnd->m_fSimDistance >= g_pWnd->m_imuData.distance[i] ) continue;
			g_pWnd->m_imuData.nCurID = i;
			g_pWnd->SendImu2Sim();
			break;
		}

		g_pWnd->m_evtL.SetEvent();
		g_pWnd->m_evtM.SetEvent();
		g_pWnd->m_evtR.SetEvent();
		
		nMultiple = ( time / nProcInterv ) + 1;		
	}

	pWnd->m_pThreadTimer = nullptr;
	g_mutxDriver.Lock();
	g_pWnd->m_fileDriver.Close();
	g_mutxDriver.Unlock();
	return 1;
}
int term = 0;


UINT CDriveSimDlg::ThreadProc(void* param)
{
	CSimThread * pParm = (CSimThread*)param;

	pParm->OpenFile();	
	
	while ( g_pWnd->m_bStarted )
	{
		WaitForSingleObject( pParm->pEvent->m_hObject, 1000 );
		pParm->Process( );
	}

	pParm->CloseFile();	
	return 0;
}
void CDriveSimDlg::CopyResult()
{
	CTime tm=CTime::GetCurrentTime();
	CString strTimeDir = tm.Format("\\result\\%Y%m%d_%H%M%S");

	strTimeDir = m_csRoot + strTimeDir;
		
	CFileFind finder;
	if (!finder.FindFile(strTimeDir + "\\*.*"))
		CreateDirectory(strTimeDir, NULL);

	for (int i = 0; i < m_vecSignSort.GetCount(); i++)
	{
		CString strPathD = strTimeDir + "\\" + m_vecSignSort[i];
		if (!finder.FindFile(strPathD + "\\*.*"))
			CreateDirectory(strPathD, NULL);

		CString strPathS = m_csRoot + "\\" + m_vecSignSort[i];
		CopyFile(strPathS +"\\drivermotion.txt" , strPathD + "\\drivermotion.txt", FALSE);
		CopyFile(strPathS + "\\time_flag_medium.txt", strPathD + "\\time_flag_medium.txt", FALSE);
	}
}

void CDriveSimDlg::OnBnClickedStart()
{
	memset( &updmsg, 0, sizeof( updmsg ) );
		
	m_dlgLeft->m_bMidWnd = FALSE;
	m_dlgLeft->m_bStop = TRUE;

	m_dlgRight->m_bMidWnd = FALSE;
	m_dlgRight->m_bStop = TRUE;
	
	m_dlgMedium->m_bMidWnd = TRUE;
	m_dlgMedium->m_bStop = TRUE;
	if(m_nCurrent>=0 && m_nCurrent<m_vecSignSort.GetCount())
		m_dlgMedium->m_strText.Format("[%s]  Loading......",  m_vecSignSort[m_nCurrent]);
	else 
		m_dlgMedium->m_strText = "Loading......";
	m_dlgMedium->Invalidate(FALSE);
	m_dlgMedium->UpdateWindow();	
	

	//SyncData();
	while (m_pThreadProcL || m_pThreadProcM || m_pThreadProcR)
	{
		m_bStarted = false;
		Sleep(30);
	}

	m_bStarted = true;

	//确定显示的Sign文件夹
	if (m_nCurrent < m_vecSignSort.GetCount() )
	{
		m_ssPath = format("%s", m_csRoot + "\\" + m_vecSignSort[m_nCurrent++]);		
		ReadData();			
	}
	else
	{
		m_dlgMedium->m_strText = "The simulation is complete, thank you!";
		m_dlgMedium->Invalidate(FALSE);
		m_dlgMedium->UpdateWindow();
		Sleep(3000);
		CopyResult();
		CDialog::OnOK();
		return;
	}


	m_fSimVelocity = 0;	

	string sstmFileM = m_ssPath + "\\time_flag_medium.txt";

	CSimThread *pramL = new CSimThread( "", NULL, m_imgLeft, m_dlgLeft, m_pThreadProcL, m_evtL );
	CSimThread *pramM = new CSimThread( sstmFileM, &m_ofMedium, m_imgMedium, m_dlgMedium, m_pThreadProcM, m_evtM );
	CSimThread *pramR = new CSimThread( "", NULL, m_imgRight, m_dlgRight, m_pThreadProcR, m_evtR );

	m_pThreadProcL = AfxBeginThread( ThreadProc,  pramL );
	m_pThreadProcM = AfxBeginThread( ThreadProc,  pramM );
	m_pThreadProcR = AfxBeginThread( ThreadProc,  pramR );

	Sleep(100);
	m_pThreadTimer = AfxBeginThread(ThreadTimer, this);
	m_btnEnd.EnableWindow(true);
}


void CDriveSimDlg::OnBnClickedEnd()
{
	m_dlgMedium->m_strText = "Push button when ready";
	m_bStarted = FALSE;
	m_dlgMedium->Invalidate(FALSE);
	m_dlgMedium->UpdateWindow();
	Sleep(1000);

	//CDialog::OnOK();
}

BOOL CDriveSimDlg::PreTranslateMessage(MSG* pMsg)
{	
	switch ( pMsg->message )
	{		
		case  WM_KEYUP: //&& m_bStarted)
		{
			switch ( pMsg->wParam )
			{
				case 'M':
				case 'm':
				{
					m_fSimVelocity = m_fSimVelocity + 2;
					m_fSimVelocity = MIN(75, m_fSimVelocity);
					break;
				}
				case 'N':
				case 'n':
				{
					m_fSimVelocity = MAX(0, m_fSimVelocity-10);
					break;
				}
				case VK_BACK:
				{
					InitializeData();
					break;
				}
				case 'Q':
				case 'q':
				{
					CRect rtClient;
					GetWindowRect( rtClient );
					::SetWindowPos( GetSafeHwnd(), HWND_TOPMOST, rtClient.left,
						rtClient.top, rtClient.Width(), rtClient.Height(), SWP_SHOWWINDOW );
					Invalidate( FALSE );
					UpdateWindow();
					break;
				}
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDriveSimDlg::OnDestroy()
{
	if (m_pThreadTimer)	TerminateThread(m_pThreadTimer->m_hThread, 1);
	if (m_pThreadProcL)	TerminateThread(m_pThreadProcL->m_hThread, 1);
	if (m_pThreadProcR)	TerminateThread(m_pThreadProcR->m_hThread, 1);
	if (m_pThreadProcM)	TerminateThread(m_pThreadProcM->m_hThread, 1);

	joyReleaseCapture( JOYSTICKID1 );
	CDialogEx::OnDestroy();	
}

void CDriveSimDlg::OnClose()
{
	CDialog::OnOK();
	CDialogEx::OnClose();
}
