#pragma once

class CViewConfigSection;
class CViewConfigSectionDefault;

class CDynReBar : public CReBar
{
public:
	BOOL AddBar(CWnd* pBar, LPCTSTR pszText = NULL, CBitmap* pbmp = NULL,
		DWORD dwStyle = RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP);
	BOOL AddBar(CWnd* pBar, COLORREF clrFore, COLORREF clrBack,
		LPCTSTR pszText = NULL, DWORD dwStyle = RBBS_GRIPPERALWAYS);

	CToolBar* GetToolBar(int nBand);
	void LoadState( CViewConfigSectionDefault& config );
	void SaveState( CViewConfigSection& config );
};