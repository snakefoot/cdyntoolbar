#include "StdAfx.h"
#include "DynReBar.h"

const LPCTSTR m_lpszStateInfoFormat = _T("wID=%04X,cx=%d,fStyle=%08X");

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

void CDynReBar::LoadState( LPCTSTR lpszProfileName )
{
	// This function restores index, width and style from the registry for
	// each band in the rebar.
	CString strValue = AfxGetApp()->GetProfileString( lpszProfileName, _T("CDynReBar") );
	if ( !strValue.IsEmpty() )
	{
		REBARBANDINFO rbbi = {0};
		rbbi.cbSize = sizeof( rbbi );
		rbbi.fMask  = RBBIM_STYLE | RBBIM_SIZE | RBBIM_ID;

		int nCount = GetReBarCtrl().GetBandCount();
		for ( int nBand = 0; nBand < nCount; nBand++ )
		{
			CString strBandState;
			VERIFY( AfxExtractSubString( strBandState, strValue, nBand, _T('\n') ) );

			UINT nID, cx, nStyle;
			int nResult = sscanf(strBandState, m_lpszStateInfoFormat, &nID, &cx, &nStyle );
			ASSERT( nResult == 3 );

			GetReBarCtrl().MoveBand( GetReBarCtrl().IDToIndex(nID), nBand );
			VERIFY( GetReBarCtrl().GetBandInfo( nBand, &rbbi ) );
			rbbi.cx     = cx;
			rbbi.fStyle = ( rbbi.fStyle & ~( RBBS_HIDDEN | RBBS_BREAK ) ) | nStyle;
			VERIFY( GetReBarCtrl().SetBandInfo( nBand, &rbbi ) );

			CToolBar* pToolBar = GetToolBar(nBand);

			if (pToolBar && (nStyle & RBBS_HIDDEN))
			{
				pToolBar->ShowWindow(SW_HIDE);
			}
		}
	}
}

void CDynReBar::SaveState( LPCTSTR lpszProfileName )
{
	// This function saves index, width and style in the registry for each
	// band in the rebar, so that it could be possible to restore all these
	// settings when the user runs the program next time.

	CString strValue;

	REBARBANDINFO rbbi = {0};
	rbbi.cbSize = sizeof( rbbi );
	rbbi.fMask  = RBBIM_STYLE | RBBIM_SIZE | RBBIM_ID;

	int nCount = GetReBarCtrl().GetBandCount();
	for ( int nBand = 0; nBand < nCount; nBand++ )
	{
		VERIFY( GetReBarCtrl().GetBandInfo( nBand, &rbbi ) );

		CString strBandState;
		strBandState.Format( m_lpszStateInfoFormat, rbbi.wID, rbbi.cx, rbbi.fStyle );
		strValue += ( strValue.IsEmpty() ? _T("") : _T("\n") ) + strBandState;
	}

	VERIFY( AfxGetApp()->WriteProfileString( lpszProfileName,
		_T("CDynReBar"), strValue ) );
}
