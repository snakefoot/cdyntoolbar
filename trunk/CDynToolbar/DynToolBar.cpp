#include "stdafx.h"
#include "DynToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynToolBar

CDynToolBar::CDynToolBar()
{
}

CDynToolBar::~CDynToolBar()
{
}


BEGIN_MESSAGE_MAP(CDynToolBar, CToolBar)
	//{{AFX_MSG_MAP(CDynToolBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_NOTIFY_REFLECT(TBN_GETBUTTONINFO,OnToolBarGetButtonInfo)
	ON_NOTIFY_REFLECT(TBN_BEGINADJUST, OnToolBarBeginAdjust)
	ON_NOTIFY_REFLECT(TBN_ENDADJUST, OnToolBarEndAdjust)
	ON_NOTIFY_REFLECT(TBN_QUERYDELETE, OnToolBarQueryDelete)
	ON_NOTIFY_REFLECT(TBN_QUERYINSERT, OnToolBarQueryInsert)
	ON_NOTIFY_REFLECT(TBN_TOOLBARCHANGE, OnToolBarChange)
	ON_NOTIFY_REFLECT(TBN_RESET, OnToolBarReset)
	ON_NOTIFY_REFLECT(TBN_INITCUSTOMIZE , OnInitCustomize)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynToolBar message handlers
#if defined(_WIN64)
INT_PTR CDynToolBar::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
#else
int CDynToolBar::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
#endif
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));
	
	// check child windows first by calling CControlBar
	int nHit = (int)CControlBar::OnToolHitTest(point, pTI);
	if (nHit != -1)
		return nHit;
	
	// now hit test against CToolBar buttons
	int nButtons = GetToolBarCtrl().GetButtonCount();
	for (int i = 0; i < nButtons; i++)
	{
		CRect rect;
		TBBUTTON button;
		if (GetToolBarCtrl().GetItemRect(i, &rect))
		{
			if (rect.PtInRect(point) &&
				GetToolBarCtrl().GetButton(i, &button) &&
				!(button.fsStyle & TBSTYLE_SEP))
			{
				int nHit = (int)GetItemID(i);
				if (pTI != NULL && pTI->cbSize >= 40/*sizeof(AFX_OLDTOOLINFO)*/)
				{
					pTI->hwnd = m_hWnd;
					pTI->rect = rect;
					pTI->uId = nHit;
					pTI->lpszText = LPSTR_TEXTCALLBACK;
				}
				// found matching rect, return the ID of the button
				return nHit != 0 ? nHit : -1;
			}
		}
	}
	return -1;
}

void CDynToolBar::OnToolBarGetButtonInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	TBNOTIFY* pTBntf = (TBNOTIFY *)pNMHDR;
	
	if((pTBntf->iItem>=0) && (pTBntf->iItem < GetToolBarCtrl().GetButtonCount()))
	{
		TBBUTTON button;
		GetToolBarCtrl().GetButton(pTBntf->iItem, &button);
		pTBntf->tbButton = button;
		CString str;
		str.LoadString(GetItemID(pTBntf->iItem));
		strcpy(pTBntf->pszText, str);
		
		*pResult = TRUE;
	}
	else
	{
		*pResult = FALSE;
	}
}

void CDynToolBar::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu menu;
	VERIFY( menu.CreatePopupMenu() );
	menu.AppendMenu(MF_STRING, 1, "Customize...");
	int nResult = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD,point.x,point.y,this);
	if (nResult==1)
		GetToolBarCtrl().Customize();
}

void CDynToolBar::OnCustomize()
{
	GetToolBarCtrl().Customize();
}

void CDynToolBar::OnToolBarQueryDelete(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = TRUE;
}

void CDynToolBar::OnToolBarQueryInsert(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = TRUE;
}

void CDynToolBar::OnToolBarChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	GetParentFrame()->RecalcLayout();
}

void CDynToolBar::OnToolBarEndAdjust(NMHDR* pNMHDR, LRESULT* pResult)
{
}

void CDynToolBar::OnToolBarBeginAdjust(NMHDR* pNMHDR, LRESULT* pResult)
{
}

void CDynToolBar::OnToolBarReset(NMHDR* pNMHDR, LRESULT* pResult)
{
}

void CDynToolBar::OnInitCustomize(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Hide the help button in the customize dialog
	*pResult = TBNRF_HIDEHELP;
}