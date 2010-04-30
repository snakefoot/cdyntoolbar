// DynToolbarDemoView.h : interface of the CDynToolbarDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DynTOOLBARDEMOVIEW_H__EED1183B_C078_4E2C_AD79_1DB6572B3443__INCLUDED_)
#define AFX_DynTOOLBARDEMOVIEW_H__EED1183B_C078_4E2C_AD79_1DB6572B3443__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDynToolbarDemoView : public CView
{
protected: // create from serialization only
	CDynToolbarDemoView();
	DECLARE_DYNCREATE(CDynToolbarDemoView)

// Attributes
public:
	CDynToolbarDemoDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynToolbarDemoView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDynToolbarDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDynToolbarDemoView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DynToolbarDemoView.cpp
inline CDynToolbarDemoDoc* CDynToolbarDemoView::GetDocument()
   { return (CDynToolbarDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DynTOOLBARDEMOVIEW_H__EED1183B_C078_4E2C_AD79_1DB6572B3443__INCLUDED_)
