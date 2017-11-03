#if !defined(AFX_DLG_SEEDS_H__41F88683_E457_49FA_A6ED_298937F522F5__INCLUDED_)
#define AFX_DLG_SEEDS_H__41F88683_E457_49FA_A6ED_298937F522F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_Seeds.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_Seeds dialog

class CDlg_Seeds : public CDialog
{
// Construction
public:
	CDlg_Seeds(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg_Seeds)
	enum { IDD = IDD_DLG_SEEDS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_Seeds)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();
	// Generated message map functions
	//{{AFX_MSG(CDlg_Seeds)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl		m_List_Crops;		//种子列表
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_SEEDS_H__41F88683_E457_49FA_A6ED_298937F522F5__INCLUDED_)
