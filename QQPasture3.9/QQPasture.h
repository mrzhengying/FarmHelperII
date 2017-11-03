// QQPasture.h : main header file for the QQPASTURE DLL
//

#if !defined(AFX_QQPASTURE_H__A86EBE80_5DA3_45F5_AA07_3EAEF6E65F7B__INCLUDED_)
#define AFX_QQPASTURE_H__A86EBE80_5DA3_45F5_AA07_3EAEF6E65F7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CQQPastureApp
// See QQPasture.cpp for the implementation of this class
//

class CQQPastureApp : public CWinApp
{
public:
	CQQPastureApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQQPastureApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CQQPastureApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QQPASTURE_H__A86EBE80_5DA3_45F5_AA07_3EAEF6E65F7B__INCLUDED_)
