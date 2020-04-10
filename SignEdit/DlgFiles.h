#if !defined(AFX_DLGFILES_H__C85B3328_DE5A_4717_8FF9_C986BCE97DDA__INCLUDED_)
#define AFX_DLGFILES_H__C85B3328_DE5A_4717_8FF9_C986BCE97DDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFiles.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFiles dialog

class CDlgFiles : public CDialog
{
// Construction
public:
	CDlgFiles(CWnd* pParent = NULL);   // standard constructor

	BYTE* m_pBitmap;
	int m_nWidth,m_nHeight;
	WORD m_flag;
	
	CString m_strFileName;
	
	
	BYTE* m_pBitmap2;
	int m_nWidth2,m_nHeight2;
	WORD m_flag2;
	
	CString m_strFileName2;

// Dialog Data
	//{{AFX_DATA(CDlgFiles)
	enum { IDD = IDD_DIALOGFILES };
	
	//}}AFX_DATA
public:
	CString	m_strFilePath1;
	CString	m_strFilePath2;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFiles)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFiles)
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFILES_H__C85B3328_DE5A_4717_8FF9_C986BCE97DDA__INCLUDED_)
