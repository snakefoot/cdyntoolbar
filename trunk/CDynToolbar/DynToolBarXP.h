#pragma once

#include "DynToolBar.h"

// BUG: The gripper bar is drawn incorrectly when an MFC application uses Windows XP Visual Styles
//	- http://support.microsoft.com/kb/843490
class CDynToolBarXP : public CDynToolBar
{
public:
	void DrawGripper(CDC* pDC, const CRect& rect);
	void EraseNonClient();
	virtual void DoPaint(CDC* pDC);

protected:
	//{{AFX_MSG(CMyToolBar)
	afx_msg void OnNcPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};