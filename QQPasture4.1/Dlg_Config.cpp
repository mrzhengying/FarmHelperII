// Dlg_Config.cpp : implementation file
//

#include "stdafx.h"
#include "QQPasture.h"
#include "Dlg_Config.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_Config dialog


CDlg_Config::CDlg_Config(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Config::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_Config)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_Config::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_Config)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX,IDC_CFG_LIST_ANIMALS,m_List_Animals);
	DDX_Control(pDX,IDC_CFG_COMBO_WANIMALS,m_Combo_WAnimals);
	DDX_Control(pDX,IDC_CFG_COMBO_WANIMALID,m_Combo_WAnimalId);
	DDX_Control(pDX,IDC_CFG_COMBO_PANIMALS,m_Combo_PAnimals);
	DDX_Control(pDX,IDC_CFG_COMBO_PANIMALID,m_Combo_PAnimalId);

	DDX_Check(pDX,IDC_CFG_CHK_ADDFOOD,m_Config.bAddFood);
	DDX_Check(pDX,IDC_CFG_CHK_BUYANIMAL,m_Config.bBuyAnimal);
	DDX_Check(pDX,IDC_CFG_CHK_CLEARBAD,m_Config.bClearBad);
	DDX_Check(pDX,IDC_CFG_CHK_HARVEST,m_Config.bHarvest);
	DDX_Check(pDX,IDC_CFG_CHK_UPHOUSE,m_Config.bUpHouse);
	DDX_Check(pDX,IDC_CFG_CHK_SALEANIMALS,m_Config.bSaleAnimals);

	DDX_Check(pDX,IDC_CFG_CHK_FRIEND_STEAL,m_Config.bFdSteal);
	DDX_Check(pDX,IDC_CFG_CHK_FRIEND_CLEARBAD,m_Config.bFdClearBad);
	DDX_Check(pDX,IDC_CHK_CFG_FRIEND_SAFETY,m_Config.bFdSafety);

	DDX_Radio(pDX,IDC_CFG_RAD_BUYANIMAL_HIGHER,m_Config.nBuyAnimalType);
	DDX_Text(pDX,IDC_CFG_COMBO_WANIMALID,m_Config.nWAnimalId);
	DDX_Text(pDX,IDC_CFG_COMBO_PANIMALID,m_Config.nPAnimalId);
	DDX_Radio(pDX,IDC_CFG_RAD_RAISE_ALLANIMAL,m_Config.nRaiseAnimalType);
	DDX_Text(pDX,IDC_CFG_EDIT_MINFOOD,m_Config.nMinFood);
	DDX_Text(pDX,IDC_CFG_EDIT_MAXFOOD,m_Config.nMaxFood);
	DDX_Text(pDX,IDC_CFG_EDIT_IGNORE,m_Config.nIgnoreFdLvl);
	DDX_Text(pDX,IDC_CFG_EDIT_WORKTIME,m_Config.nWorkTime);
	DDX_Text(pDX,IDC_CFG_EDIT_SLEEPTIME,m_Config.nSleepTime);
	DDX_Text(pDX,IDC_CFG_EDIT_SALENUM,m_Config.nSaleNum);
}


BEGIN_MESSAGE_MAP(CDlg_Config, CDialog)
	//{{AFX_MSG_MAP(CDlg_Config)
	ON_CBN_SELCHANGE(IDC_CFG_COMBO_WANIMALS, OnSelchangeCfgComboWanimals)
	ON_CBN_SELCHANGE(IDC_CFG_COMBO_PANIMALS, OnSelchangeCfgComboPanimals)
	ON_BN_CLICKED(IDC_CFG_BTN_OK, OnCfgBtnOk)
	ON_BN_CLICKED(IDC_CFG_BTN_CANCEL, OnCfgBtnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_Config message handlers

CDlg_Config::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	//给卖出规则的列表添加标头
	m_List_Animals.InsertColumn(0,"锁定",NULL,40,-1);
	m_List_Animals.InsertColumn(1,"ID",NULL,50,-1);
	m_List_Animals.InsertColumn(2,"名称",NULL,140,-1);
	m_List_Animals.InsertColumn(3,"等级",NULL,50,-1);
	//设置列表框整行选择,显示网格,选择框
	m_List_Animals.SetExtendedStyle(LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	//读取本地配置
	G_GetGlobalConfig(m_Config);

	std::vector<ST_ANIMALGROW> v_Animals;
	char szAnimals[SHORTUSERLEN]={0};
	char szAnimal[SHORTLEN]={0};
	//取所有Section项,单个字符串以0结尾,整个字符串以00结尾
	GetPrivateProfileSectionNames(szAnimals,SHORTUSERLEN,strAnimalsIni);
	int nPos = 0;
	while(szAnimals[nPos] != 0)
	{
		strcpy(szAnimal,szAnimals+nPos);	//得到单个的Section
		ST_ANIMALGROW pAnimalGrow;
		G_GetAnimalInfo(atoi(szAnimal),pAnimalGrow);	//读取这个Section的数据
		v_Animals.push_back(pAnimalGrow);
		nPos += strlen(szAnimal) + 1;
	}
	std::sort(v_Animals.begin(),v_Animals.end(),C_AnimalsLvlCmp);	//根据等级排序

	int nWSel = 0,nPSel = 0;
	for (int i=0;i<v_Animals.size();i++)
	{
		m_List_Animals.InsertItem(i,"");
		CString strBuf;
		strBuf.Format("%d",v_Animals[i].Id);	//ID
		m_List_Animals.SetItemText(i,1,strBuf);
		m_List_Animals.SetItemText(i,2,v_Animals[i].Product);	//名称
		strBuf.Format("%d",v_Animals[i].Level);			//等级
		m_List_Animals.SetItemText(i,3,strBuf);
		if (v_Animals[i].IsLock || v_Animals[i].Type == 4)
			m_List_Animals.SetCheck(i,TRUE);

		if (v_Animals[i].buyprice > 0 && v_Animals[i].Level <= m_SelfInfo.Lvl)
		{
			CString strName,strId;
			strName.Format("%s(%d级)",v_Animals[i].Name,v_Animals[i].Level);	//名字(等级)
			strId.Format("%d",v_Animals[i].Id);								//ID
			if (v_Animals[i].Id < 1500)
			{
				int nCount = m_Combo_WAnimals.AddString(strName);
				m_Combo_WAnimalId.AddString(strId);
				if (v_Animals[i].Id == m_Config.nWAnimalId)
					nWSel = nCount;
			}
			else
			{
				int nCount = m_Combo_PAnimals.AddString(strName);
				m_Combo_PAnimalId.AddString(strId);
				if (v_Animals[i].Id == m_Config.nPAnimalId)
					nPSel = nCount;
			}
		}
	}
	m_Combo_WAnimals.SetCurSel(nWSel);
	m_Combo_WAnimalId.SetCurSel(nWSel);
	m_Combo_PAnimals.SetCurSel(nPSel);
	m_Combo_PAnimalId.SetCurSel(nPSel);

	UpdateData(FALSE);
	return TRUE;
}

void CDlg_Config::OnSelchangeCfgComboWanimals() 
{
	m_Combo_WAnimalId.SetCurSel(m_Combo_WAnimals.GetCurSel());
}

void CDlg_Config::OnSelchangeCfgComboPanimals() 
{
	m_Combo_PAnimalId.SetCurSel(m_Combo_PAnimals.GetCurSel());
}

UINT pCfgLockAnimal(LPVOID pParam)
{
	CDlg_Config *hDlg_Config = (CDlg_Config *)pParam;
	for (int i=0;i<hDlg_Config->m_List_Animals.GetItemCount();i++)
	{
		char szBuf[8]={0};
		hDlg_Config->m_List_Animals.GetItemText(i,1,szBuf,8);
		int nId = atoi(szBuf);
		BOOL bIsLock = hDlg_Config->m_List_Animals.GetCheck(i);
		ST_ANIMALGROW pAnimalGrow;
		G_GetAnimalInfo(nId,pAnimalGrow);
		if (pAnimalGrow.IsLock != bIsLock)
		{
			pAnimalGrow.IsLock = bIsLock;
			G_WriteAnimalInfo(pAnimalGrow);
			CString strPostData;
			strPostData.Format("cId=%d&uIdx=%d&farmKey=null&target=%s",pAnimalGrow.Id,m_SelfInfo.Id,bIsLock? "lock":"unlock");
			CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_get_repertory",strPostData);
			CString strPrintf;
			strPrintf.Format("%s了仓库中的%s",bIsLock? "锁定":"解锁",pAnimalGrow.Product);
			hDlg_Config->SetDlgItemText(IDC_CFG_TEXT_HINT,strPrintf);
			Sleep(600);
		}
	}
	return TRUE;
}

void CDlg_Config::OnCfgBtnOk() 
{
	UpdateData(TRUE);
	G_WriteGlobalConfig(m_Config);
	CC_CreateThread(pCfgLockAnimal,this,TRUE);
	OnCancel();
}

void CDlg_Config::OnCfgBtnCancel() 
{
	OnCancel();
}
