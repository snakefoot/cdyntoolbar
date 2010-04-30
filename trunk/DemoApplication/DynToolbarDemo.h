// DynToolbarDemo.h : main header file for the DynTOOLBARDEMO application
//

#if !defined(AFX_DynTOOLBARDEMO_H__2686E78D_A5B1_497B_BAE6_13BA37ED565D__INCLUDED_)
#define AFX_DynTOOLBARDEMO_H__2686E78D_A5B1_497B_BAE6_13BA37ED565D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CDynToolbarDemoApp:
// See DynToolbarDemo.cpp for the implementation of this class
//

class CDynToolbarDemoApp : public CWinApp
{
public:
	CDynToolbarDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynToolbarDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDynToolbarDemoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DynTOOLBARDEMO_H__2686E78D_A5B1_497B_BAE6_13BA37ED565D__INCLUDED_)
