// A bug fix of MFC CToolBar that may cause incorrect tooltip be
// shown for a toolbar button
// 
// Implemented by Mingliang Zhu
// mailto:perryzh#msn.com
//


#if !defined(AFX_DynTOOLBAR_H__9C9EA769_9478_41BC_ACFE_1821519A323A__INCLUDED_)
#define AFX_DynTOOLBAR_H__9C9EA769_9478_41BC_ACFE_1821519A323A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DynToolBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDynToolBar window

class CDynToolBar : public CToolBar
{
// Construction
public:
	CDynToolBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynToolBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDynToolBar();

	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	// Generated message map functions
protected:
	//{{AFX_MSG(CDynToolBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DynTOOLBAR_H__9C9EA769_9478_41BC_ACFE_1821519A323A__INCLUDED_)
