#if !defined(AFX_DLG_ANIMALS_H__857E18C9_ED22_4B28_A0A1_EE0A7E02D41E__INCLUDED_)
#define AFX_DLG_ANIMALS_H__857E18C9_ED22_4B28_A0A1_EE0A7E02D41E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_Animals.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_Animals dialog

class CDlg_Animals : public CDialog
{
// Construction
public:
	CDlg_Animals(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg_Animals)
	enum { IDD = IDD_DLG_ANIMALS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_Animals)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();
	// Generated message map functions
	//{{AFX_MSG(CDlg_Animals)
	afx_msg void OnAnimalsSaleanimals();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl	m_List_Animals;
	CButton		m_Btn_SaleAnimals;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_ANIMALS_H__857E18C9_ED22_4B28_A0A1_EE0A7E02D41E__INCLUDED_)
