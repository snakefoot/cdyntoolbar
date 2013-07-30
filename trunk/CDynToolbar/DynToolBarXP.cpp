#include "stdafx.h"
#include "DynToolBarXP.h"

BEGIN_MESSAGE_MAP(CDynToolBarXP, CDynToolBar)
	//{{AFX_MSG_MAP(CDynToolBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDynToolBarXP::OnNcPaint() 
{
	EraseNonClient();
}

void CDynToolBarXP::EraseNonClient()
{
	// Get window DC that is clipped to the non-client area.
	CWindowDC dc(this);
	CRect rectClient;
	GetClientRect(rectClient);
	CRect rectWindow;
	GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	rectClient.OffsetRect(-rectWindow.left, -rectWindow.top);
	dc.ExcludeClipRect(rectClient);

	// Draw the borders in the non-client area.
	rectWindow.OffsetRect(-rectWindow.left, -rectWindow.top);
	DrawBorders(&dc, rectWindow);

	// Erase the parts that are not drawn.
	dc.IntersectClipRect(rectWindow);
	SendMessage(WM_ERASEBKGND, (WPARAM)dc.m_hDC);

	// Draw the gripper in the non-client area.
	DrawGripper(&dc, rectWindow);
}

void CDynToolBarXP::DrawGripper(CDC* pDC, const CRect& rect)
{
	pDC->FillSolidRect( &rect, ::GetSysColor(COLOR_BTNFACE)); // Fill in the background.
	CDynToolBar::DrawGripper(pDC,rect);
}

void CDynToolBarXP::DoPaint(CDC* pDC)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	// Paint inside the client area.
	CRect rect;
	GetClientRect(rect);
	DrawBorders(pDC, rect);
	DrawGripper(pDC, rect);
}