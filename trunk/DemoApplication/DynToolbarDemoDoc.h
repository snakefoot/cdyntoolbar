// DynToolbarDemoDoc.h : interface of the CDynToolbarDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIXEDTOOLBARDEMODOC_H__BE33B398_EA12_4DF6_8ECD_3020379D173F__INCLUDED_)
#define AFX_FIXEDTOOLBARDEMODOC_H__BE33B398_EA12_4DF6_8ECD_3020379D173F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDynToolbarDemoDoc : public CDocument
{
protected: // create from serialization only
	CDynToolbarDemoDoc();
	DECLARE_DYNCREATE(CDynToolbarDemoDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynToolbarDemoDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDynToolbarDemoDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDynToolbarDemoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIXEDTOOLBARDEMODOC_H__BE33B398_EA12_4DF6_8ECD_3020379D173F__INCLUDED_)
