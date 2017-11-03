// QQFarm.h : main header file for the QQFARM DLL
//

#if !defined(AFX_QQFARM_H__F7C77537_8744_4B80_B13C_5EB00C3DF8F9__INCLUDED_)
#define AFX_QQFARM_H__F7C77537_8744_4B80_B13C_5EB00C3DF8F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CQQFarmApp
// See QQFarm.cpp for the implementation of this class
//

class CQQFarmApp : public CWinApp
{
public:
	CQQFarmApp();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQQFarmApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CQQFarmApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QQFARM_H__F7C77537_8744_4B80_B13C_5EB00C3DF8F9__INCLUDED_)
