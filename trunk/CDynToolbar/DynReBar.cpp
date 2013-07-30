#include "StdAfx.h"
#include "DynReBar.h"

#include "ViewConfigSection.h"

BEGIN_MESSAGE_MAP(CDynReBar, CReBar)
	//{{AFX_MSG_MAP(CDynReBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_CONTEXTMENU()
	ON_NOTIFY_REFLECT_EX(RBN_CHEVRONPUSHED, OnChevronPushed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define REBARBANDINFO_SIZE CCSIZEOF_STRUCT(REBARBANDINFO, cxHeader)	// Use V5 size even if compiling for Vista/Win7

BOOL CDynReBar::AddBar(CWnd* pBar, LPCTSTR pszText, CBitmap* pbmp, DWORD dwStyle)
{
	if (!CReBar::AddBar(pBar, pszText, pbmp, dwStyle))
		return FALSE;

	// Give the toolbar a unique id (Order of how they are added)
	REBARBANDINFO rbbi = {0};
	rbbi.cbSize = REBARBANDINFO_SIZE;
	rbbi.fMask = RBBIM_ID;
	if (dwStyle & RBBS_USECHEVRON)
	{
		rbbi.fMask |= RBBIM_CHILDSIZE | RBBIM_IDEALSIZE;
		VERIFY( GetReBarCtrl().GetBandInfo(GetReBarCtrl().GetBandCount()-1, &rbbi) );
		rbbi.cxIdeal = rbbi.cxMinChild;
		rbbi.cxMinChild = 0;
	}
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
	rbbi.cbSize = REBARBANDINFO_SIZE;
	rbbi.fMask = RBBIM_ID;
	if (dwStyle & RBBS_USECHEVRON)
	{
		rbbi.fMask |= RBBIM_CHILDSIZE | RBBIM_IDEALSIZE;
		VERIFY( GetReBarCtrl().GetBandInfo(GetReBarCtrl().GetBandCount()-1, &rbbi) );
		rbbi.cxIdeal = rbbi.cxMinChild;
		rbbi.cxMinChild = 0;
	}
	rbbi.wID = GetReBarCtrl().GetBandCount()-1;
	VERIFY( GetReBarCtrl().SetBandInfo(GetReBarCtrl().GetBandCount()-1, &rbbi) );
	return TRUE;
}

CToolBar* CDynReBar::GetToolBar(int nBand)
{
	REBARBANDINFO rbbi = {0};
	rbbi.cbSize = REBARBANDINFO_SIZE;
	rbbi.fMask = RBBIM_CHILD|RBBIM_STYLE|RBBIM_SIZE;

	VERIFY( GetReBarCtrl().GetBandInfo( nBand, &rbbi ) );

	return DYNAMIC_DOWNCAST(CToolBar, CWnd::FromHandlePermanent(rbbi.hwndChild));
}

void CDynReBar::Lock( BOOL bLock)
{
	REBARBANDINFO rbbi = {0};
	rbbi.cbSize = REBARBANDINFO_SIZE;
	rbbi.fMask  = RBBIM_STYLE | RBBIM_ID;
	
	int nCount = GetReBarCtrl().GetBandCount();
	for ( int nBand = 0; nBand < nCount; nBand++ )
	{
		VERIFY( GetReBarCtrl().GetBandInfo( nBand, &rbbi ) );

		if ( bLock )
		{
			rbbi.fStyle &= ~RBBS_GRIPPERALWAYS;
			rbbi.fStyle |= RBBS_NOGRIPPER;
		}
		else
		{
			rbbi.fStyle |= RBBS_GRIPPERALWAYS;
			rbbi.fStyle &= ~RBBS_NOGRIPPER;
		}

		VERIFY( GetReBarCtrl().SetBandInfo( nBand, &rbbi ) );
	}

	RecalcLayout();
}

BOOL CDynReBar::IsLocked() const
{
	REBARBANDINFO rbbi = {0};
	rbbi.cbSize = REBARBANDINFO_SIZE;
	rbbi.fMask  = RBBIM_STYLE | RBBIM_ID;
	
	int nCount = GetReBarCtrl().GetBandCount();
	for ( int nBand = 0; nBand < nCount; nBand++ )
	{
		VERIFY( GetReBarCtrl().GetBandInfo( nBand, &rbbi ) );

		if ((rbbi.fStyle & RBBS_NOGRIPPER) == RBBS_NOGRIPPER && (rbbi.fStyle & RBBS_GRIPPERALWAYS) != RBBS_GRIPPERALWAYS)
		{
			return TRUE;
		}
	}
	return FALSE;
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
		rbbi.cbSize = REBARBANDINFO_SIZE;
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

			if (nBandStyle & RBBS_HIDDEN)
				GetReBarCtrl().ShowBand(nBand, FALSE);
		}
	}
}

void CDynReBar::SaveState( CViewConfigSection& config )
{
	// This function saves index, width and style for each band in the rebar,
	// so that it could be possible to restore all these settings when the
	// user runs the program next time.

	REBARBANDINFO rbbi = {0};
	rbbi.cbSize = REBARBANDINFO_SIZE;
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
	rbbi.cbSize = REBARBANDINFO_SIZE;
	rbbi.fMask  = RBBIM_ID | RBBIM_CHILDSIZE | RBBIM_CHILD | RBBIM_SIZE | RBBIM_STYLE | RBBIM_IDEALSIZE;

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
				if (rbbi.fStyle & RBBS_USECHEVRON)
				{
					rbbi.cxIdeal = size.cx;
				}
				else
				{
					rbbi.cxMinChild = size.cx;
					rbbi.cyMinChild = size.cy;

					if (rbbi.cxMinChild > rbbi.cx)
						rbbi.cx = rbbi.cxMinChild;
				}

				VERIFY( GetReBarCtrl().SetBandInfo( nBand, &rbbi ) );
			}
		}
	}
}

void CDynReBar::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu menu;
	VERIFY( menu.CreatePopupMenu() );

	TCHAR pText[256] = {0};
	REBARBANDINFO rbbi = {0};
	rbbi.cbSize = REBARBANDINFO_SIZE;
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
		VERIFY( menu.AppendMenu(nFlags, nBand+1, rbbi.lpText) );
	}

	VERIFY( menu.AppendMenu(MF_SEPARATOR) );

	if (IsLocked())
		VERIFY( menu.AppendMenu(MF_STRING | MF_CHECKED, nCount+2, _T("Locked")) );
	else
		VERIFY( menu.AppendMenu(MF_STRING | MF_UNCHECKED, nCount+1, _T("Locked")) );

	int nResult = menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD,point.x,point.y,this);
	if (nResult > 0)
	{
		int nBand = nResult-1;
		if (nBand < nCount)
		{
			VERIFY( GetReBarCtrl().GetBandInfo( nBand, &rbbi ) );
			if (rbbi.fStyle & RBBS_HIDDEN)
			{
				VERIFY( GetReBarCtrl().ShowBand(nBand, TRUE) );
			}
			else
			{
				VERIFY( GetReBarCtrl().ShowBand(nBand, FALSE) );
			}
		}
		else
		{
			if (nResult==nCount+1)
				Lock();
			else
				Lock(FALSE);
		}
	}
}

BOOL CDynReBar::OnChevronPushed(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMREBARCHEVRON* pRebarChevron = reinterpret_cast<NMREBARCHEVRON*>(pNMHDR);

	CToolBar* pToolBar = GetToolBar(pRebarChevron->uBand);
	if (pToolBar==NULL)
		return FALSE;

	// Display a vertical floating toolbar with text-labels
	//	- Allows reuse of bitmaps
	//	- Allows click-events to flow to the main window
	//	- Let the toolbar close itself when loosing focus
	//  - Copy CToolBarPopup from SizeableRebar_demo

	return FALSE;	// Let parent-dialog get chance
}