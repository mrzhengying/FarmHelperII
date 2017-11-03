#if !defined(AFX_DLG_CONFIG_H__1C602461_9C2C_4622_AB0C_983DD3811232__INCLUDED_)
#define AFX_DLG_CONFIG_H__1C602461_9C2C_4622_AB0C_983DD3811232__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dlg_Config.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_Config dialog

class CDlg_Config : public CDialog
{
// Construction
public:
	CDlg_Config(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlg_Config)
	enum { IDD = IDD_DLG_CONFIG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_Config)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL OnInitDialog();
	// Generated message map functions
	//{{AFX_MSG(CDlg_Config)
	afx_msg void OnSelchangeCfgComboWanimals();
	afx_msg void OnSelchangeCfgComboPanimals();
	afx_msg void OnCfgBtnOk();
	afx_msg void OnCfgBtnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl		m_List_Animals;		//动物列表
	CComboBox		m_Combo_WAnimals;
	CComboBox		m_Combo_WAnimalId;
	CComboBox		m_Combo_PAnimals;
	CComboBox		m_Combo_PAnimalId;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_CONFIG_H__1C602461_9C2C_4622_AB0C_983DD3811232__INCLUDED_)
