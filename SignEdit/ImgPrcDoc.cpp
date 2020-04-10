// ImgPrcDoc.cpp : implementation of the CImgPrcDoc class
//

#include "stdafx.h"
#include "ImgPrc.h"

#include "ImgPrcDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImgPrcDoc

IMPLEMENT_DYNCREATE(CImgPrcDoc, CDocument)

BEGIN_MESSAGE_MAP(CImgPrcDoc, CDocument)
	//{{AFX_MSG_MAP(CImgPrcDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImgPrcDoc construction/destruction

CImgPrcDoc::CImgPrcDoc()
{
	// TODO: add one-time construction code here

}

CImgPrcDoc::~CImgPrcDoc()
{
}

BOOL CImgPrcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CImgPrcDoc serialization

void CImgPrcDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CImgPrcDoc diagnostics

#ifdef _DEBUG
void CImgPrcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImgPrcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImgPrcDoc commands
