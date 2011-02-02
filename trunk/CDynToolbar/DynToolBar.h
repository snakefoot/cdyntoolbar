#if !defined(AFX_DYNTOOLBAR_H__9C9EA769_9478_41BC_ACFE_1821519A323A__INCLUDED_)
#define AFX_DYNTOOLBAR_H__9C9EA769_9478_41BC_ACFE_1821519A323A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DynToolBar.h : header file
//

// CRebar - Makes it possible to display Chevron
// CRebar - Allowed one to convert "ugly" toolbars to flat toolbars (CToolbar now does that automatically)
// CRebar - Allows one to put a bitmap behind the toolbars (Just like the old IE 3.0)
// CRebar - Looks good on Win7/Vista

// CFrameWnd::LoadBarState() / SaveBarState() handles position floating/undocked toolbars. http://support.microsoft.com/kb/232845
//	- How do we handle this with CRebar ?
// CToolBar::RestoreState() / SaveState() handles buttons hidden
//	- This should work well with CRebar, but we still need to remember toolbar positions ?
//		- Look at http://www.codeproject.com/KB/toolbars/toolbarex.aspx for how to customize with save and restore
//	- Seems CDockState can load and save to the registry for us

// Floating and docking of toolbars
//	- CRebar doesn't support floating toolbars
//	- CToolbar supports floating mode with help from CFrameWnd::EnableDocking()
//		EnableDocking(0);
//		m_wndToolBar.EnableDocking(0);
//		FloatControlBar(&m_wndToolBar, CPoint(100,100));

// CommandBar is a CToolbar with text-buttons. Need to create dropdown menu when clicking a text-button

// CFrameWnd already contains something called CDockBar that are created when calling EnableDocking()
//	- http://msdn.microsoft.com/en-us/library/ms997549.aspx

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
	bool AddSeparator();
	bool AddButton(int nIdCommand, HICON hIcon);
	bool AddButton(int nIdCommand, LPCTSTR lpIconName);
	bool AddButton(int nIdCommand, UINT lpIconName);
	bool AddButton(TBBUTTON& tbb, HICON hIcon);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynToolBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDynToolBar();

	//BOOL LoadToolBar(LPCTSTR lpszResourceName);
	//BOOL LoadToolBar(UINT nIDResource);

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

private:
	CImageList m_ImageList;
	CImageList m_DisabledImageList;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNTOOLBAR_H__9C9EA769_9478_41BC_ACFE_1821519A323A__INCLUDED_)
