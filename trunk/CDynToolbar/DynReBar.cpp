#include "StdAfx.h"
#include "DynReBar.h"

#include "ViewConfigSection.h"

BEGIN_MESSAGE_MAP(CDynReBar, CReBar)
	//{{AFX_MSG_MAP(CDynReBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDynReBar::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu menu;
	VERIFY( menu.CreatePopupMenu() );

	TCHAR pText[256] = {0};
	REBARBANDINFO rbbi = {0};
	rbbi.cbSize = sizeof( rbbi );
	rbbi.fMask  = RBBIM_TEXT | RBBIM_ID | RBBIM_STYLE;
	rbbi.lpText = pText;
	rbbi.cch = _countof(pText)-1;

	int nCount = GetReBarCtrl().GetBandCount();
	for ( int nBand = 0; nBand < nCount; nBand++ )
	{
		VERIFY( GetReBarCtrl().GetBandInfo( nBand, &rbbi ) );

		UINT nFlags = MF_STRING;
		if (rbbi.fStyle & RBBS_HIDDEN)
			nFlags |= MF_UNCHECKED;
		else
			nFlags |= MF_CHECKED;
		menu.AppendMenu(nFlags, nBand+1, rbbi.lpText);
	}
	int nResult = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD,point.x,point.y,this);
	if (nResult > 0)
	{
		int nBand = nResult-1;

		VERIFY( GetReBarCtrl().GetBandInfo( nBand, &rbbi ) );
		if (rbbi.fStyle & RBBS_HIDDEN)
		{
			GetReBarCtrl().ShowBand(nBand, TRUE);
		}
		else
		{
			GetReBarCtrl().ShowBand(nBand, FALSE);
		}
	}
}

BOOL CDynReBar::AddBar(CWnd* pBar, LPCTSTR pszText, CBitmap* pbmp, DWORD dwStyle)
{
	if (!CReBar::AddBar(pBar, pszText, pbmp, dwStyle))
		return FALSE;

	// Give the toolbar a unique id (Order of how they are added)
	REBARBANDINFO rbbi = {0};
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_ID;
	rbbi.wID = GetReBarCtrl().GetBandCount()-1;
	VERIFY( GetReBarCtrl().SetBandInfo(GetReBarCtrl().GetBandCount()-1, &rbbi) );
	return TRUE;
}

BOOL CDynReBar::AddBar(CWnd* pBar, COLORREF clrFore, COLORREF clrBack, LPCTSTR pszText, DWORD dwStyle)
{
	if (!CReBar::AddBar(pBar, clrFore, clrBack, pszText, dwStyle))
		return FALSE;

	// Give the toolbar a unique id (Order of how they are added)
	REBARBANDINFO rbbi = {0};
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_ID;
	rbbi.wID = GetReBarCtrl().GetBandCount()-1;
	VERIFY( GetReBarCtrl().SetBandInfo(GetReBarCtrl().GetBandCount()-1, &rbbi) );
	return TRUE;
}

CToolBar* CDynReBar::GetToolBar(int nBand)
{
	REBARBANDINFO rbbi = {0};
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_CHILD|RBBIM_STYLE|RBBIM_SIZE;

	VERIFY( GetReBarCtrl().GetBandInfo( nBand, &rbbi ) );

	return DYNAMIC_DOWNCAST(CToolBar, CWnd::FromHandlePermanent(rbbi.hwndChild));
}

void CDynReBar::LoadState( CViewConfigSectionDefault& config )
{
	if (!config.HasDefaultConfig())
		SaveState(config.GetDefaultConfig());

	// This function restores index, width and style for each band in the rebar.
	int nConfigCount = config.GetIntSetting(_T("BandCount"));
	if (nConfigCount > 0)
	{
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof( rbbi );
		rbbi.fMask  = RBBIM_STYLE | RBBIM_SIZE | RBBIM_ID | RBBIM_CHILD;

		int nCount = GetReBarCtrl().GetBandCount();
		for ( int nBand = 0; nBand < nCount; nBand++ )
		{
			CString bandIdSetting;
			bandIdSetting.Format(_T("BandId_%d"), nBand);
			int nBandId = config.GetIntSetting(bandIdSetting);

			CString bandPosSetting;
			bandPosSetting.Format(_T("BandPosition_%d"), nBand);
			int nBandPos = config.GetIntSetting(bandPosSetting);

			CString bandStyleSetting;
			bandStyleSetting.Format(_T("BandStyle_%d"), nBand);
			int nBandStyle = config.GetIntSetting(bandStyleSetting);

			GetReBarCtrl().MoveBand( GetReBarCtrl().IDToIndex(nBandId), nBand );
			VERIFY( GetReBarCtrl().GetBandInfo( nBand, &rbbi ) );
			rbbi.cx     = nBandPos;
			rbbi.fStyle = ( rbbi.fStyle & ~( RBBS_HIDDEN | RBBS_BREAK ) ) | nBandStyle;
			VERIFY( GetReBarCtrl().SetBandInfo( nBand, &rbbi ) );

			CWnd* pBandWnd = CWnd::FromHandlePermanent(rbbi.hwndChild);
			if (pBandWnd && (nBandStyle & RBBS_HIDDEN))
			{
				pBandWnd->ShowWindow(SW_HIDE);
			}
		}
	}
}

void CDynReBar::SaveState( CViewConfigSection& config )
{
	// This function saves index, width and style for each band in the rebar,
	// so that it could be possible to restore all these settings when the
	// user runs the program next time.

	REBARBANDINFO rbbi = {0};
	rbbi.cbSize = sizeof( rbbi );
	rbbi.fMask  = RBBIM_STYLE | RBBIM_SIZE | RBBIM_ID;

	int nCount = GetReBarCtrl().GetBandCount();

	config.SetIntSetting(_T("BandCount"), nCount);

	for ( int nBand = 0; nBand < nCount; nBand++ )
	{
		VERIFY( GetReBarCtrl().GetBandInfo( nBand, &rbbi ) );

		CString bandIdSetting;
		bandIdSetting.Format(_T("BandId_%d"), nBand);
		config.SetIntSetting(bandIdSetting, rbbi.wID);

		CString bandPosSetting;
		bandPosSetting.Format(_T("BandPosition_%d"), nBand);
		config.SetIntSetting(bandPosSetting, rbbi.cx);

		CString bandStyleSetting;
		bandStyleSetting.Format(_T("BandStyle_%d"), nBand);
		config.SetIntSetting(bandStyleSetting, rbbi.fStyle);
	}
}

CSize CDynReBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	RecalcLayout();	// Synchronize with min-size of bands added
	return CReBar::CalcFixedLayout(bStretch, bHorz);
}

void CDynReBar::RecalcLayout()
{
	REBARBANDINFO rbbi = {0};
	rbbi.cbSize = sizeof( rbbi );
	rbbi.fMask  = RBBIM_ID | RBBIM_CHILDSIZE | RBBIM_CHILD | RBBIM_SIZE;

	int nCount = GetReBarCtrl().GetBandCount();

	for ( int nBand = 0; nBand < nCount; nBand++ )
	{
		VERIFY( GetReBarCtrl().GetBandInfo( nBand, &rbbi ) );

		CSize size;
		CControlBar* pPar = DYNAMIC_DOWNCAST(CControlBar, CWnd::FromHandlePermanent(rbbi.hwndChild));
		if (pPar != NULL)
		{
			size = pPar->CalcFixedLayout(FALSE, m_dwStyle & CBRS_ORIENT_HORZ);   
			if (size.cx != rbbi.cxMinChild || size.cy != rbbi.cyMinChild)
			{
				rbbi.cxMinChild = size.cx;
				rbbi.cyMinChild = size.cy;
				
				if (rbbi.cxMinChild > rbbi.cx)
					rbbi.cx = rbbi.cxMinChild;

				VERIFY( GetReBarCtrl().SetBandInfo( nBand, &rbbi ) );
			}
		}
	}
}