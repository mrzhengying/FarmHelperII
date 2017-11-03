#if !defined(AFX_DLG_MAIN_H__C2FF9D20_D114_4048_BA58_E08AD3CA8B50__INCLUDED_)
#define AFX_DLG_MAIN_H__C2FF9D20_D114_4048_BA58_E08AD3CA8B50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_Main.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CDlg_Main dialog

class CDlg_Main : public CDialog
{
// Construction
public:
	CDlg_Main(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg_Main)
	enum { IDD = IDD_DLG_MAIN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_Main)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();
	// Generated message map functions
	//{{AFX_MSG(CDlg_Main)
	afx_msg void OnPaint();
	afx_msg LRESULT OnUpdateData(WPARAM wParam,LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClickListFriends(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMainBtnMyfarm();
	afx_msg void OnMainBtnConfig();
	afx_msg void OnMainBtnShowseed();
	afx_msg void OnMainBtnShowcrop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl		m_List_Lands;	//土地列表框
	CListCtrl		m_List_Friends;	//好友列表框
	CProgressCtrl	m_Pgs_UplvExp;	//升级所需经验进度条
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_MAIN_H__C2FF9D20_D114_4048_BA58_E08AD3CA8B50__INCLUDED_)
