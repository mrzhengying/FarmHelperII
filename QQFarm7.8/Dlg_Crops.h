#if !defined(AFX_DLG_CROPS_H__7DD42111_9ADE_418C_BAAB_20EDE665AC05__INCLUDED_)
#define AFX_DLG_CROPS_H__7DD42111_9ADE_418C_BAAB_20EDE665AC05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_Crops.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_Crops dialog

class CDlg_Crops : public CDialog
{
// Construction
public:
	CDlg_Crops(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg_Crops)
	enum { IDD = IDD_DLG_CROPS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_Crops)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();
	// Generated message map functions
	//{{AFX_MSG(CDlg_Crops)
	afx_msg void OnCrpSalecrop();
	afx_msg void OnCrpSavelock();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl		m_List_Crops;	//仓库植物
	CButton			m_Btn_SaleCrop;	//卖出收获按钮
	CButton			m_Btn_SaveLock;	//保存锁定按钮
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_CROPS_H__7DD42111_9ADE_418C_BAAB_20EDE665AC05__INCLUDED_)
