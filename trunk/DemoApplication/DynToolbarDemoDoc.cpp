// DynToolbarDemoDoc.cpp : implementation of the CDynToolbarDemoDoc class
//

#include "stdafx.h"
#include "DynToolbarDemo.h"

#include "DynToolbarDemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynToolbarDemoDoc

IMPLEMENT_DYNCREATE(CDynToolbarDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CDynToolbarDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CDynToolbarDemoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynToolbarDemoDoc construction/destruction

CDynToolbarDemoDoc::CDynToolbarDemoDoc()
{
	// TODO: add one-time construction code here

}

CDynToolbarDemoDoc::~CDynToolbarDemoDoc()
{
}

BOOL CDynToolbarDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDynToolbarDemoDoc serialization

void CDynToolbarDemoDoc::Serialize(CArchive& ar)
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
// CDynToolbarDemoDoc diagnostics

#ifdef _DEBUG
void CDynToolbarDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDynToolbarDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDynToolbarDemoDoc commands
