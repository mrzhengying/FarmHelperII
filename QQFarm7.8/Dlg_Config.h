#if !defined(AFX_DLG_CONFIG_H__8CA6E25C_48DC_4755_9081_142B10925691__INCLUDED_)
#define AFX_DLG_CONFIG_H__8CA6E25C_48DC_4755_9081_142B10925691__INCLUDED_

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
	afx_msg void OnSelchangeCfgComboBlackseed();
	afx_msg void OnSelchangeCfgComboRedseed();
	afx_msg void OnCfgBtnOk();
	afx_msg void OnCfgBtnCancel();
	afx_msg void OnSelchangeCfgComboBlack2seed();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl		m_List_Crops;	//植物列表
	CComboBox		m_Combo_BlackSeed;	//普通种子
	CComboBox		m_Combo_BlackSeedId;//普通种子ID
	CComboBox		m_Combo_RedSeed;	//红土地种子
	CComboBox		m_Combo_RedSeedId;	//红土地种子ID
	CComboBox		m_Combo_Black2Seed;	//黑土地种子
	CComboBox		m_Combo_Black2SeedId;//黑土地种子ID
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLG_CONFIG_H__8CA6E25C_48DC_4755_9081_142B10925691__INCLUDED_)
