// A bug fix of MFC CToolBar that may cause incorrect tooltip be
// shown for a toolbar button
// 
// Implemented by Mingliang Zhu
// mailto:perryzh#msn.com
//

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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynToolBar message handlers
int CDynToolBar::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));
	
	// check child windows first by calling CControlBar
	int nHit = (int)CControlBar::OnToolHitTest(point, pTI);
	if (nHit != -1)
		return nHit;
	
	// now hit test against CToolBar buttons
	CDynToolBar* pBar = (CDynToolBar*)this;
	int nButtons = (int)pBar->DefWindowProc(TB_BUTTONCOUNT, 0, 0);
	for (int i = 0; i < nButtons; i++)
	{
		CRect rect;
		TBBUTTON button;
		if (pBar->DefWindowProc(TB_GETITEMRECT, i, (LPARAM)&rect))
		{
// 			++rect.bottom;
// 			++rect.right;
			if (rect.PtInRect(point) &&
				pBar->DefWindowProc(TB_GETBUTTON, i, (LPARAM)&button) &&
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