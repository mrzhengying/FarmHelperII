#if !defined(AFX_DLG_PACKAGE_H__78D7D0ED_8C4D_4684_A5A9_E287918BB666__INCLUDED_)
#define AFX_DLG_PACKAGE_H__78D7D0ED_8C4D_4684_A5A9_E287918BB666__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_Package.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_Package dialog

class CDlg_Package : public CDialog
{
// Construction
public:
	CDlg_Package(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg_Package)
	enum { IDD = IDD_DLG_PACKAGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_Package)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();
	// Generated message map functions
	//{{AFX_MSG(CDlg_Package)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl	m_List_Animals;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_PACKAGE_H__78D7D0ED_8C4D_4684_A5A9_E287918BB666__INCLUDED_)
