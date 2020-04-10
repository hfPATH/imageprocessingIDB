// ImgPrc.h : main header file for the IMGPRC application
//

#if !defined(AFX_IMGPRC_H__6A949AD4_BA94_4096_8A06_2B35A00F1A11__INCLUDED_)
#define AFX_IMGPRC_H__6A949AD4_BA94_4096_8A06_2B35A00F1A11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CImgPrcApp:
// See ImgPrc.cpp for the implementation of this class
//

class CImgPrcApp : public CWinApp
{
public:
	CImgPrcApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgPrcApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CImgPrcApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGPRC_H__6A949AD4_BA94_4096_8A06_2B35A00F1A11__INCLUDED_)
