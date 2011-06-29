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
//	- RestoreState() / SaveState() is called by the MFC framework, and always saves to registry
//		- We want to save to any settings locations (CViewConfigSection)
//	- Look at http://www.codeproject.com/KB/toolbars/toolbarex.aspx for how to customize with save and restore
//		- Can one just remove buttons by hidding them ? (Yes but customize works differently)
//	- There are two ways to load a toolbar
//		- Any load or add-button operation adds to the available buttons
//		- Any manual operations are changes to default state
//			- What about operations after the toolbar has been displayed ?
//  - Consider using CViewConfigSection to store default-buttons, current-buttons
//		- Default-buttons are all buttons, where some are marked as hidden by default
//		- Default-buttons can then be used for listing all available buttons
//		- After saving default-buttons, then we can "remove" the hidden buttons
//      - Maybe the logic should be controlled through DynRebar, as it can contain multiple toolbars / controls in special order
//			- It should recognize that 

// Floating and docking of toolbars
//	- CRebar doesn't support floating toolbars
//	- CToolbar supports floating mode with help from CFrameWnd::EnableDocking()
//		EnableDocking(0);
//		m_wndToolBar.EnableDocking(0);
//		FloatControlBar(&m_wndToolBar, CPoint(100,100));
//  - Consider to undock CToolbar, and use CFrameWnd for vertical docking
//		- This will disable redocking for CDialog
//	- Consider letting CDynToolbar control automatically dock into the CDynReBar (When docked to top bar)
//  - This just means CDynReBar need to call parent frame to float
//		- Problem how to perform LoadState() / SaveState() for toolbars outside CDynReBar
//			- Maybe CDynToolbar LoadState() / SaveState() should handle where they are docked and what position

// CommandBar is a CToolbar with text-buttons. Need to create dropdown menu when clicking a text-button

// CFrameWnd already contains something called CDockBar that are created when calling EnableDocking()
//	- http://msdn.microsoft.com/en-us/library/ms997549.aspx

class CViewConfigSection;

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

	void ReplaceButton(CWnd& wndCtrl, UINT nID);

	BOOL IsVertDocked();

	void SaveState(CViewConfigSection& config);
	void LoadState(const CViewConfigSection& config);

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

	void RecalcControlPositions();
	void RecalcControlPosition(UINT nID);
	void SaveDefaultState();

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
	afx_msg void OnSave(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRestore(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

private:
	CImageList m_ImageList;
	CImageList m_DisabledImageList;
	BOOL m_bHideChildWndOnVertical;

	struct ButtonDetails
	{
		TBBUTTON	m_ButtonInfo;		// Information regarding the button
		CString		m_ButtonText;		// Text for the button
		BOOL		m_DefaultVisible;	// Button intially visible
		CWnd*		m_pButtonWnd;
	};
	CSimpleArray<ButtonDetails> m_AllButtons;
	struct ButtonControl
	{
		UINT		m_ButtonCmdId;
		CWnd*		m_pButtonWnd;
	};
	CSimpleArray<ButtonControl> m_AllButtonsCtrls;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DYNTOOLBAR_H__9C9EA769_9478_41BC_ACFE_1821519A323A__INCLUDED_)
