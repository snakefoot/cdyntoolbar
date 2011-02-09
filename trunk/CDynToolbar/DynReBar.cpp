#include "StdAfx.h"
#include "DynReBar.h"

#include "ViewConfigSection.h"

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
		rbbi.fMask  = RBBIM_STYLE | RBBIM_SIZE | RBBIM_ID;

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

			CToolBar* pToolBar = GetToolBar(nBand);

			if (pToolBar && (nBandStyle & RBBS_HIDDEN))
			{
				pToolBar->ShowWindow(SW_HIDE);
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
