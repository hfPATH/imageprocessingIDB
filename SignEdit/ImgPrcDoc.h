// ImgPrcDoc.h : interface of the CImgPrcDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGPRCDOC_H__7C8DC5F6_6067_4314_B606_61680BAB4418__INCLUDED_)
#define AFX_IMGPRCDOC_H__7C8DC5F6_6067_4314_B606_61680BAB4418__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CImgPrcDoc : public CDocument
{
protected: // create from serialization only
	CImgPrcDoc();
	DECLARE_DYNCREATE(CImgPrcDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImgPrcDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImgPrcDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImgPrcDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMGPRCDOC_H__7C8DC5F6_6067_4314_B606_61680BAB4418__INCLUDED_)
