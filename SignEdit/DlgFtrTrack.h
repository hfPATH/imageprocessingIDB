#pragma once


// CDlgFtrTrack dialog

class CDlgFtrTrack : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFtrTrack)

public:
	CDlgFtrTrack(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFtrTrack();

	CWnd* m_pParentWnd;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGTRACK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support	
	DECLARE_MESSAGE_MAP()
public:
	int m_nMaxNFeature;
	double m_dQuality;
	double m_dDistance;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
};
