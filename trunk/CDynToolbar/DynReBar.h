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

	BOOL ShowBand(int nBand, BOOL bShow);

	CToolBar* GetToolBar(int nBand);
	void LoadState( CViewConfigSectionDefault& config );
	void SaveState( CViewConfigSection& config );

protected:
	//{{AFX_MSG(CDynReBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

private:

	DECLARE_MESSAGE_MAP()
};