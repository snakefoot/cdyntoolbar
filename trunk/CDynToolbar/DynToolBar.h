#if !defined(AFX_DYNTOOLBAR_H__9C9EA769_9478_41BC_ACFE_1821519A323A__INCLUDED_)
#define AFX_DYNTOOLBAR_H__9C9EA769_9478_41BC_ACFE_1821519A323A__INCLUDED_

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


	// Generated message map functions
protected:
#if defined(_WIN64)
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
#else
	virtual int OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
#endif

	//{{AFX_MSG(CDynToolBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnCustomize();
	afx_msg void OnToolBarGetButtonInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnToolBarBeginAdjust(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToolBarEndAdjust(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToolBarQueryDelete(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToolBarQueryInsert(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToolBarReset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToolBarChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnInitCustomize(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNTOOLBAR_H__9C9EA769_9478_41BC_ACFE_1821519A323A__INCLUDED_)
