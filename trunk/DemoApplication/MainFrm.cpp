// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MainFrm.h"

#include "DynToolbarDemo.h"
#include "ViewConfigSection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC /*|CBRS_GRIPPER*/) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndDynToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC| CCS_ADJUSTABLE  /*|CBRS_GRIPPER*/) ||
		!m_wndDynToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create Dyn toolbar\n");
		return -1;      // fail to create
	}
	
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	if(!m_wndComboBox.Create(CBS_DROPDOWN | CBS_SORT | WS_VISIBLE |
		WS_TABSTOP | WS_VSCROLL, CRect(0,0,100,120), &m_wndDynToolBar, ID_FILE_NEW))
	{
		TRACE(_T("Failed to create combo-box\n"));
		return FALSE;
	}

	m_wndDynToolBar.ReplaceButton(m_wndComboBox, ID_FILE_NEW);
	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar, "Original MFC Toolbar") ||
		!m_wndReBar.AddBar(&m_wndDynToolBar, "Dyn Toolbar", NULL, RBBS_GRIPPERALWAYS | RBBS_BREAK | RBBS_USECHEVRON)
		)
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndDynToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);
	//DockControlBar(&m_wndDynToolBar);

	//LoadBarState("ToolbarInf");
	//m_wndDynToolBar.GetToolBarCtrl().RestoreState(HKEY_CURRENT_USER, _T("Software\\Local AppWizard-Generated Applications\\DynToolbarDemo"), _T("BarState"));

	CViewConfigSectionWinApp dynToolBarState("DynToolBar");
	m_wndDynToolBar.LoadState(dynToolBarState);
	CViewConfigSectionWinApp dynReBarState("DynReBar");
	m_wndReBar.LoadState(dynReBarState);
	return 0;
}

void CMainFrame::OnClose()
{
	//SaveBarState("ToolbarInf");
	//m_wndDynToolBar.GetToolBarCtrl().SaveState(HKEY_CURRENT_USER, _T("Software\\Local AppWizard-Generated Applications\\DynToolbarDemo"), _T("BarState"));

	CViewConfigSectionWinApp dynToolBarState("DynToolBar");
	m_wndDynToolBar.SaveState(dynToolBarState);
	CViewConfigSectionWinApp dynReBarState("DynReBar");
	m_wndReBar.SaveState(dynReBarState);

	CFrameWnd::OnClose();
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

