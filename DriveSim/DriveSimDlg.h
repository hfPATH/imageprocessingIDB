
// DriveSimDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "ReadData.hpp"
#include <opencv2\opencv.hpp>
#include "DialogLeft.h"




// CDriveSimDlg 对话框
class CDriveSimDlg : public CDialogEx
{
// 构造
public:
	CDriveSimDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRIVESIM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	bool			m_bStarted;		

public:
	//dialog to show images
	CDialogLeft*	m_dlgLeft;
	CDialogLeft*	m_dlgMedium;
	CDialogLeft*	m_dlgRight;

	

	//data processing and sending thread
	CWinThread*		m_pThreadTimer;
	CWinThread*		m_pThreadProcL;
	CWinThread*		m_pThreadProcM;
	CWinThread*		m_pThreadProcR;

	CEvent m_evtL;
	CEvent m_evtM;
	CEvent m_evtR;

	static UINT ThreadTimer(void* param);
	static UINT ThreadProc(void* param);

	//data recieving thread
	CWinThread*		m_pThreadRecv;
	static UINT	ThreadRecv(void* param);

	//int				m_nImuStartPos;				//start index of imu_data (align the first frame of images)
	//int				m_nVelocityStartPos;		//start index of velocity_data

	vector<float>	m_vecPath;					//path length of vehicle integrated by the velocity data

	////if loading data successfully
	//bool			m_imuLoaded;
	//bool			m_gpsLoaded;
	//bool			m_velocityLoaded;
	//bool			m_imagesLoaded;

	//write the moment of showing images in file
	CStdioFile  m_fileDriver;
	ofstream	m_ofLeft;
	ofstream	m_ofMedium;
	ofstream	m_ofRight;
	
	CEdit	m_editImu;
	CEdit	m_editGps;
	CEdit	m_editVelocity;
	CEdit	m_editImages;
public:
	CGps			m_gpsData;
	CImu			m_imuData;
	CVelocity		m_velocityData;
	CImageData		m_imgLeft;
	CImageData		m_imgMedium;
	CImageData		m_imgRight;
	cv::Mat			m_img;

public:
	double			m_dAcc;
	float			m_fSimVelocity;			//recieved velocity of the simulater
	double			m_fSimDistance;
	

	CString			 m_csRoot;				//file path
	string           m_ssPath;
	int				 m_nCurrent;			//index of vevtor < m_vecSignSort
	static const int mc_nSignNum = 1;		//the number of signs
	CStringArray	 m_vecSignSort;			//Sign order
	void GenSignOrder();

	CString m_ImuPath;
	CString m_GpsPath;
	CString	m_VelocityPath;
	CString m_ImgsPathLeft;
	CString m_ImgsPathLeftMedium;
	CString m_ImgsPathLeftRight;
	
	void LoadSignOrder();
	void ReadData();
	void InitializeData();
	void CopyResult();
	double GetSignPosDis();

	//void SyncData();
	//void SetDistanceAttribute();

	//write in three funtions to reduce time-cost
	int Process( double dSimDistance, CImageData* pimgData, CDialogLeft* pDlgShow );
	void SendImu2Sim( );


	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton m_btnImu;
	CButton m_btnGps;
	CButton m_btnVelocity;
	CButton m_btnSart;
	CButton m_btnEnd;
	afx_msg void OnBnClickedIMU();
	afx_msg void OnBnClickedGPS();
	afx_msg void OnBnClickedVelocity();
	CButton m_btnImages;
	afx_msg void OnBnClickedImages();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedEnd();
	afx_msg LRESULT OnJoyDown( WPARAM w, LPARAM l );
	afx_msg LRESULT OnJoyMove( WPARAM w, LPARAM l );
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
};


extern CDriveSimDlg* g_pWnd;

class CSimThread
{
public:
	CSimThread( string spath, ofstream* pioStream, CImageData& imgData, CDialogLeft*& pDlg, CWinThread*& pthread, CEvent &evt )
	{
		ssTimeFile = spath;
		pStream = pioStream;
		pImgData = &imgData;
		imgData.nCurFrameID = 0;
		ppDlgShow = &pDlg;
		ppThreadProc = &pthread;
		pEvent = &evt;
	};
	string       ssTimeFile;
	ofstream   * pStream;
	CImageData * pImgData;
	CDialogLeft**ppDlgShow;
	CWinThread** ppThreadProc;
	CEvent	  *  pEvent;

	void OpenFile()
	{
		if ( !pStream )
			return;
		pStream->open( ssTimeFile );
		( *pStream ) << "timeStamp,\tlocalTime,\tfromBootTime,\tSim-speed,\tSignLeft, \t\SignTop, \tSignRight, \tSignBottom\n";
	};

	void CloseFile()
	{
		if( pStream )
			pStream->close();
		( *ppThreadProc ) = nullptr;

		(*ppDlgShow)->m_bStop = TRUE;
		(*ppDlgShow)->Invalidate(TRUE);
		(*ppDlgShow)->UpdateWindow();
	};

	void OutputTm( int nIndex )
	{
		if ( !pStream )
			return;

		SYSTEMTIME st;
		GetLocalTime( &st );

		( *pStream ) << pImgData->timeStamp[nIndex] << ",\t" << st.wMonth << "-"
			<< st.wDay << "-" << st.wHour << ":" << st.wMinute << ":"
			<< st.wSecond << "." << st.wMilliseconds << ",\t" <<GetTickCount()<< ",\t" << g_pWnd->m_fSimVelocity << ",\t" 
			<< g_pWnd->m_dlgMedium->m_nLeft << ",\t" << g_pWnd->m_dlgMedium->m_nTop << ",\t" << g_pWnd->m_dlgMedium->m_nRight << ",\t" << g_pWnd->m_dlgMedium->m_nBottom <<endl;
	};

	void Process()
	{
		int nIndex = -1;
		if ( (nIndex = g_pWnd->Process( g_pWnd->m_fSimDistance, pImgData, *ppDlgShow )) < 0 )
			return;

		OutputTm( nIndex );
	};
};