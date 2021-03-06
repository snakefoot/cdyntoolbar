#include "stdafx.h"
#include "DynToolBar.h"

#include "ViewConfigSection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynToolBar

CDynToolBar::CDynToolBar()
{
	m_bHideChildWndOnVertical = TRUE;
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
    // Saving and restoring toolbar
    ON_NOTIFY_REFLECT( TBN_SAVE, OnSave )
    ON_NOTIFY_REFLECT( TBN_RESTORE, OnRestore )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

bool CDynToolBar::AddSeparator()
{
	return AddButton(0, (HICON)NULL);
}

bool CDynToolBar::AddButton(int nIdCommand, HICON hIcon)
{
	TBBUTTON tbb = {0};
	tbb.idCommand = nIdCommand;
	tbb.iString = -1;	// Text drawn next to the button
	tbb.fsState = TBSTATE_ENABLED;
	tbb.fsStyle = nIdCommand==0 ? TBSTYLE_SEP : TBSTYLE_BUTTON;
	return AddButton(tbb, hIcon);
}

bool CDynToolBar::AddButton(int nIdCommand, UINT nIDResource)
{
	//load the icon from the resources
	if (nIDResource != NULL)
	{
		HINSTANCE hInst = 
			AfxFindResourceHandle(MAKEINTRESOURCE(nIDResource),RT_GROUP_ICON);
		HICON hIcon = (HICON)LoadImage(hInst,MAKEINTRESOURCE(nIDResource),
			IMAGE_ICON,32,32,LR_DEFAULTCOLOR);
		return AddButton(nIdCommand, hIcon);
	}
	else
	{
		return AddButton(nIdCommand, (HICON)NULL);
	}
}

bool CDynToolBar::AddButton(int nIdCommand, LPCTSTR lpszResourceName)
{
	//load the icon from the resources
	if (lpszResourceName != NULL)
	{
		HINSTANCE hInst = 
			AfxFindResourceHandle(MAKEINTRESOURCE(lpszResourceName),RT_GROUP_ICON);
		HICON hIcon = (HICON)LoadImage(hInst,MAKEINTRESOURCE(lpszResourceName),
			IMAGE_ICON,32,32,LR_DEFAULTCOLOR);
		return AddButton(nIdCommand, hIcon);
	}
	else
	{
		return AddButton(nIdCommand, (HICON)NULL);
	}
}

namespace
{
	// http://www.codeproject.com/KB/graphics/Create_GrayscaleIcon.aspx
	HICON CreateGrayscaleIcon( HICON hIcon, COLORREF* pPalette )
	{
		if (hIcon == NULL)
		{
			return NULL;
		}
	 
		HDC hdc = ::GetDC(NULL);
	 
		HICON      hGrayIcon      = NULL;
		ICONINFO   icInfo         = { 0 };
		ICONINFO   icGrayInfo     = { 0 };
		LPDWORD    lpBits         = NULL;
		LPBYTE     lpBitsPtr      = NULL;
		SIZE sz;
		DWORD c1 = 0;
		BITMAPINFO bmpInfo        = { 0 };
		bmpInfo.bmiHeader.biSize  = sizeof(BITMAPINFOHEADER);
	 
		if (::GetIconInfo(hIcon, &icInfo))
		{
			if (::GetDIBits(hdc, icInfo.hbmColor, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS) != 0)
			{
				bmpInfo.bmiHeader.biCompression = BI_RGB;
				bmpInfo.bmiHeader.biPlanes = 1;
				bmpInfo.bmiHeader.biBitCount = 32;
				bmpInfo.bmiHeader.biSizeImage = bmpInfo.bmiHeader.biWidth * 4 * bmpInfo.bmiHeader.biHeight;
				bmpInfo.bmiHeader.biClrUsed = 0;
				bmpInfo.bmiHeader.biClrImportant = 0;
	 
				const int size = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * bmpInfo.bmiHeader.biClrUsed;
				BITMAPINFO* pBmpInfo = (BITMAPINFO*)new BYTE[size];
				memset (pBmpInfo, 0, size);
				pBmpInfo->bmiHeader = bmpInfo.bmiHeader;
				pBmpInfo->bmiHeader.biCompression = BI_RGB;
	 
				sz.cx = bmpInfo.bmiHeader.biWidth;
				sz.cy = bmpInfo.bmiHeader.biHeight;
				c1 = sz.cx * sz.cy;
	 
				lpBits = (LPDWORD)::GlobalAlloc(GMEM_FIXED, (c1) * 4);
	 
				if (lpBits && ::GetDIBits(hdc, icInfo.hbmColor, 0, bmpInfo.bmiHeader.biHeight, lpBits, pBmpInfo, DIB_RGB_COLORS) != 0)
				{
					lpBitsPtr     = (LPBYTE)lpBits;
					UINT off      = 0;
	 
					for (UINT i = 0; i < c1; i++)
					{
						off = (UINT)( 255 - (( lpBitsPtr[0] + lpBitsPtr[1] + lpBitsPtr[2] ) / 3) );
	 
						if (lpBitsPtr[3] != 0 || off != 255)
						{
							if (off == 0)
							{
								off = 1;
							}
	 
							lpBits[i] = pPalette[off] | ( lpBitsPtr[3] << 24 );
						}
	 
						lpBitsPtr += 4;
					}
	 
					icGrayInfo.hbmColor = ::CreateCompatibleBitmap(hdc, sz.cx, sz.cy);
	 
					if (icGrayInfo.hbmColor != NULL)
					{
						::SetDIBits(hdc, icGrayInfo.hbmColor, 0, sz.cy, lpBits, &bmpInfo, DIB_RGB_COLORS);
	 
						icGrayInfo.hbmMask = icInfo.hbmMask;
						icGrayInfo.fIcon   = TRUE;
	 
						hGrayIcon = ::CreateIconIndirect(&icGrayInfo);
	 
						::DeleteObject(icGrayInfo.hbmColor);
					}
	 
					::GlobalFree(lpBits);
					lpBits = NULL;
				}
			}
	 
			::DeleteObject(icInfo.hbmColor);
			::DeleteObject(icInfo.hbmMask);
		}
	 
		::ReleaseDC(NULL,hdc);
	 
		return hGrayIcon;
	}
	 
	HICON CreateGrayscaleIcon( HICON hIcon )
	{
		if (hIcon == NULL)
		{
			return NULL;
		}
	 
		COLORREF defaultGrayPalette[256];
		BOOL bGrayPaletteSet = FALSE;
		if (!bGrayPaletteSet)
		{
			for(int i = 0; i < 256; i++)
			{
				defaultGrayPalette[i] = RGB(255-i, 255-i, 255-i);
			}
	 
			bGrayPaletteSet = TRUE;
		}
	 
		return CreateGrayscaleIcon(hIcon, defaultGrayPalette);
	}

}

bool CDynToolBar::AddButton(TBBUTTON& tbb, HICON hIcon)
{
	if (hIcon != 0)
	{
		// Add the icon to the imagelist's
		if (!m_ImageList)
		{
			SetSizes( CSize( 20 + 7, 20 + 8 ), CSize( 20, 20 ) );

			m_ImageList.Create(20, 20, ILC_COLOR32 | ILC_MASK, 0, 0);
			GetToolBarCtrl().SetImageList(&m_ImageList);

			m_DisabledImageList.Create(20, 20, ILC_COLOR32 | ILC_MASK, 0, 0);
			GetToolBarCtrl().SetDisabledImageList(&m_DisabledImageList);
		}

		tbb.iBitmap = m_ImageList.Add( hIcon );
		m_DisabledImageList.Add( CreateGrayscaleIcon(hIcon) );
	}
	else
	{
		tbb.iBitmap = I_IMAGENONE;
	}

    return GetToolBarCtrl().AddButtons( 1, &tbb )!=0;
}

void CDynToolBar::ReplaceButton(CWnd& wndCtrl, UINT nID)
{
	ASSERT(IsWindow(wndCtrl.m_hWnd));

	int nIndex = CommandToIndex( nID ) ;
	ASSERT(nIndex >= 0);

	if (nIndex < 0)
		return;

	// Set the button style for a seperator with a certain width
	wndCtrl.SetFont( GetFont());
	CRect rect(0,0,0,0);
	wndCtrl.GetWindowRect(rect);
	SetButtonInfo( nIndex, nID, TBBS_SEPARATOR, rect.Width());

	ModifyStyle(0,WS_CLIPCHILDREN);

	ButtonControl btnCtrl = { nID, &wndCtrl };
	m_AllButtonsCtrls.Add(btnCtrl);

	RecalcControlPosition(nID);
}

void CDynToolBar::RecalcControlPosition(UINT nID)
{
	CWnd* pButtonCtrl = NULL;
	for(int i = 0; i < m_AllButtonsCtrls.GetSize(); ++i)
	{
		if (m_AllButtonsCtrls[i].m_ButtonCmdId == nID)
		{
			pButtonCtrl = m_AllButtonsCtrls[i].m_pButtonWnd;
			break;
		}
	}

	ASSERT(pButtonCtrl != NULL);
	if (pButtonCtrl == NULL)
		return;

	int nIndex = CommandToIndex(nID);
	if (nIndex < 0)
	{
		pButtonCtrl->ShowWindow(SW_HIDE);
	}
	else
	{
		// Stolen from http://www.codeproject.com/KB/toolbars/toolbarex.aspx by Deepak Khajuria
		CRect itemRect;
		GetItemRect( nIndex, &itemRect );
		CRect rt;
		pButtonCtrl->GetWindowRect(&rt);
		itemRect.top+=max((itemRect.Height()-rt.Height())/2,0);	 //move to middle
		pButtonCtrl->SetWindowPos(0, itemRect.left, itemRect.top, 0, 0,
			SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOCOPYBITS );

		BOOL bVert = IsVertDocked(); //(m_dwStyle & CBRS_ORIENT_VERT) != 0;

		if (bVert && m_bHideChildWndOnVertical)
		{
	   		int nState=GetToolBarCtrl().GetState(nIndex);
	   		GetToolBarCtrl().SetState(nID,(nState | TBSTATE_HIDDEN));
			pButtonCtrl->ShowWindow( SW_HIDE );
		}
		else
		{
	   		int nState=GetToolBarCtrl().GetState(nIndex);
	   		GetToolBarCtrl().SetState(nIndex,(nState & ~TBSTATE_HIDDEN));
			pButtonCtrl->ShowWindow( SW_SHOW );
		}
	}
}

void CDynToolBar::RecalcControlPositions()
{
	for(int i = 0; i < m_AllButtonsCtrls.GetSize(); ++i)
	{
		RecalcControlPosition(m_AllButtonsCtrls[i].m_ButtonCmdId);
	}
}

BOOL CDynToolBar::IsVertDocked()
{
	UINT nID =GetParent()->GetDlgCtrlID();
	return ( (nID ==AFX_IDW_DOCKBAR_LEFT) || (nID== AFX_IDW_DOCKBAR_RIGHT));
}

void CDynToolBar::SaveState(CViewConfigSection& config)
{
	config.RemoveCurrentConfig();	// Reset the existing config

	// Skal kunne huske id for de knapper som er vist
	int nButtons = GetToolBarCtrl().GetButtonCount();

	config.SetIntSetting(_T("ButtonCount"), nButtons);

	for (int i = 0; i < nButtons; i++)
	{
		TBBUTTON button = {0};
		GetToolBarCtrl().GetButton(i, &button);

		CString btnSetting;
		btnSetting.Format(_T("Button_%d"), i);
		config.SetIntSetting(btnSetting, button.idCommand);
	}
}

void CDynToolBar::LoadState(const CViewConfigSection& config)
{
	SaveDefaultState();

	int nSelectedButtons = config.GetIntSetting(_T("ButtonCount"));

	CSimpleArray<int> selectedButtons;

	for(int i = 0; i < nSelectedButtons; ++i)
	{
		CString btnSetting;
		btnSetting.Format(_T("Button_%d"), i);
		int idCommand = config.GetIntSetting(btnSetting);

		for(int j = 0; j < m_AllButtons.GetSize(); ++j)
		{
			bool alreadySelected = false;
			for(int n = 0; n < selectedButtons.GetSize(); ++n)
				if (selectedButtons[n]==j)
					alreadySelected = true;

			if (alreadySelected)
				continue;

			if (m_AllButtons[j].m_ButtonInfo.idCommand == idCommand)
			{
				selectedButtons.Add(j);
				break;
			}
		}
	}

	if (selectedButtons.GetSize()==0)
	{
		// If no buttons selected, then display all buttons
		for(int j = 0; j < m_AllButtons.GetSize(); ++j)
			selectedButtons.Add(j);
	}

	int nButtons = GetToolBarCtrl().GetButtonCount();
	for(int k = nButtons-1; k >= 0; --k)
		GetToolBarCtrl().DeleteButton(k);

	for(int n = 0; n < selectedButtons.GetSize(); ++n)
		GetToolBarCtrl().AddButtons(1, &m_AllButtons[ selectedButtons[n] ].m_ButtonInfo);

	RecalcControlPositions();
	if (GetParentFrame())
		GetParentFrame()->RecalcLayout();

	// Check if any buttons in the state are available
	//	- If none then don't load the state
	//  - How to remove buttons without loosing custom controls ?
	//		- We need to hide / show the controls depending on their corresponding button is visible
	//		- We need a map between button-id and control-wnd
	//			- They should not be saved in the config class
}

void CDynToolBar::SaveDefaultState()
{
	if (m_AllButtons.GetSize()==0)
	{
		int nCount = GetToolBarCtrl().GetButtonCount();
		for (int i=0;i<nCount;i++)
		{
			ButtonDetails btnDetails = {0};
			GetToolBarCtrl().GetButton(i,&btnDetails.m_ButtonInfo);
			CString str;
			str.LoadString(btnDetails.m_ButtonInfo.idCommand);
			int nPos= str.ReverseFind(_T('\n'));
			btnDetails.m_ButtonText=str.Right(str.GetLength()-nPos-1);
			btnDetails.m_DefaultVisible=TRUE;

			m_AllButtons.Add(btnDetails);
		}

		for(int j=0;j< m_AllButtonsCtrls.GetSize(); ++j)
		{
			for(int k = 0; k < m_AllButtons.GetSize(); ++k)
			{
				if (m_AllButtons[k].m_ButtonInfo.idCommand == m_AllButtonsCtrls[j].m_ButtonCmdId)
				{
					if (m_AllButtons[k].m_pButtonWnd == NULL)
					{
						m_AllButtons[k].m_pButtonWnd = m_AllButtonsCtrls[j].m_pButtonWnd;
						break;
					}
				}
			}
		}
	}
}

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
#if defined(_WIN64)
	INT_PTR nHit = (int)CControlBar::OnToolHitTest(point, pTI);
#else
	int nHit = (int)CControlBar::OnToolHitTest(point, pTI);
#endif
	if (nHit != -1)
		return nHit;
	
	// now hit test against CToolBar buttons
	int nButtons = GetToolBarCtrl().GetButtonCount();
	for (int i = 0; i < nButtons; i++)
	{
		CRect rect;
		TBBUTTON button = {0};
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
	SaveDefaultState();

	TBNOTIFY* pTBntf = (TBNOTIFY *)pNMHDR;

	// if the index is valid
	if ((0 <= pTBntf->iItem) && (pTBntf->iItem < m_AllButtons.GetSize()))
	{
		// copy the stored button structure
		pTBntf->tbButton = m_AllButtons[pTBntf->iItem].m_ButtonInfo;

		// copy the text for the button label in the dialog
		_tcscpy(pTBntf->pszText, m_AllButtons[pTBntf->iItem].m_ButtonText);

		// indicate valid data was sent
		*pResult = TRUE;
	}
	else  	// else there is no button for this index
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
	TBNOTIFY* pTBntf = (TBNOTIFY *)pNMHDR;

	// do not allow hidden button to be deleted as they just do not go
	// to the Add listbox.
	if ((pTBntf->tbButton.idCommand) && 
		GetToolBarCtrl().IsButtonHidden(pTBntf->tbButton.idCommand))
			*pResult = FALSE;
	else					 
			*pResult = TRUE;  
}

void CDynToolBar::OnToolBarQueryInsert(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = TRUE;
}

void CDynToolBar::OnToolBarChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	RecalcControlPositions();
	if (GetParentFrame())
		GetParentFrame()->RecalcLayout();
}

void CDynToolBar::OnToolBarEndAdjust(NMHDR* pNMHDR, LRESULT* pResult)
{
	RecalcControlPositions();
	if (GetParentFrame())
		GetParentFrame()->RecalcLayout();
}

void CDynToolBar::OnToolBarBeginAdjust(NMHDR* pNMHDR, LRESULT* pResult)
{
}

void CDynToolBar::OnToolBarReset(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Remove all of the existing buttons 
	int nButtons = GetToolBarCtrl().GetButtonCount();
	for(int k = nButtons-1; k >= 0; --k)
		GetToolBarCtrl().DeleteButton(k);

	// Restore the buttons that were saved.
	for(int n = 0; n < m_AllButtons.GetSize(); ++n)
		GetToolBarCtrl().AddButtons(1, &m_AllButtons[ n ].m_ButtonInfo);

	RecalcControlPositions();
	if (GetParentFrame())
		GetParentFrame()->RecalcLayout();
}

void CDynToolBar::OnInitCustomize(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Hide the help button in the customize dialog
	*pResult = TBNRF_HIDEHELP;
}

void CDynToolBar::OnSave( NMHDR* pNMHDR, LRESULT* pResult )
{
	// Must add some dummy data, or else the toolbar will reset if restoring and only one button (DWORD) left
    NMTBSAVE* lpnmtb = ( NMTBSAVE* )pNMHDR;
    if ( lpnmtb->iItem == -1 )
    {
		lpnmtb->cbData  += sizeof( DWORD );
		lpnmtb->pData    = ( LPDWORD )::GlobalAlloc( GMEM_FIXED, lpnmtb->cbData );
        lpnmtb->pCurrent = lpnmtb->pData;

        *lpnmtb->pCurrent++ = (DWORD)0;
    }

    *pResult = 0;
}

void CDynToolBar::OnRestore( NMHDR* pNMHDR, LRESULT* pResult )
{
	SaveDefaultState();

    NMTBRESTORE* lpnmtb = ( NMTBRESTORE* )pNMHDR;

    if ( lpnmtb->iItem == -1 )
    {
		lpnmtb->cButtons = lpnmtb->cbData / lpnmtb->cbBytesPerRecord;
        lpnmtb->pCurrent = lpnmtb->pData;

        lpnmtb->pCurrent++;
    }

    *pResult = 0;
}
