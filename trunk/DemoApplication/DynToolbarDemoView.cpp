// DynToolbarDemoView.cpp : implementation of the CDynToolbarDemoView class
//

#include "stdafx.h"
#include "DynToolbarDemo.h"

#include "DynToolbarDemoDoc.h"
#include "DynToolbarDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynToolbarDemoView

IMPLEMENT_DYNCREATE(CDynToolbarDemoView, CView)

BEGIN_MESSAGE_MAP(CDynToolbarDemoView, CView)
	//{{AFX_MSG_MAP(CDynToolbarDemoView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynToolbarDemoView construction/destruction

CDynToolbarDemoView::CDynToolbarDemoView()
{
	// TODO: add construction code here

}

CDynToolbarDemoView::~CDynToolbarDemoView()
{
}

BOOL CDynToolbarDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDynToolbarDemoView drawing

void CDynToolbarDemoView::OnDraw(CDC* pDC)
{
	CDynToolbarDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CDynToolbarDemoView printing

BOOL CDynToolbarDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDynToolbarDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDynToolbarDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDynToolbarDemoView diagnostics

#ifdef _DEBUG
void CDynToolbarDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CDynToolbarDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDynToolbarDemoDoc* CDynToolbarDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDynToolbarDemoDoc)));
	return (CDynToolbarDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDynToolbarDemoView message handlers
