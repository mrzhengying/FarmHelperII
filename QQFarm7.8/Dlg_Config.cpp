// Dlg_Config.cpp : implementation file
//

#include "stdafx.h"
#include "QQFarm.h"
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
	DDX_Control(pDX,IDC_CFG_LIST_CROPS,m_List_Crops);
	DDX_Control(pDX,IDC_CFG_COMBO_BLACKSEED,m_Combo_BlackSeed);
	DDX_Control(pDX,IDC_CFG_COMBO_BLACKSEEDID,m_Combo_BlackSeedId);
	DDX_Control(pDX,IDC_CFG_COMBO_REDSEED,m_Combo_RedSeed);
	DDX_Control(pDX,IDC_CFG_COMBO_REDSEEDID,m_Combo_RedSeedId);
	DDX_Control(pDX,IDC_CFG_COMBO_BLACK2SEED,m_Combo_Black2Seed);
	DDX_Control(pDX,IDC_CFG_COMBO_BLACK2SEEDID,m_Combo_Black2SeedId);

	DDX_Check(pDX,IDC_CFG_CHK_BP,m_Config.bSelfBp);
	DDX_Check(pDX,IDC_CFG_CHK_SC,m_Config.bSelfSc);
	DDX_Check(pDX,IDC_CFG_CHK_WCS,m_Config.bSelfWcs);
	DDX_Check(pDX,IDC_CFG_CHK_SALECROP,m_Config.bSaleCrop);
	DDX_Check(pDX,IDC_CFG_CHK_RECLAIM,m_Config.bReclaim);
	DDX_Check(pDX,IDC_CFG_CHK_UPRED,m_Config.bUpRed);
	DDX_Check(pDX,IDC_CFG_CHK_UPBLACK2,m_Config.bUpBlack2);
	DDX_Check(pDX,IDC_CFG_CHK_BPFISH,m_Config.bSelfBpFish);
	DDX_Check(pDX,IDC_CFG_CHK_SCFISH,m_Config.bSelfScFish);
	DDX_Check(pDX,IDC_CFG_CHK_FD_SC,m_Config.bFdSc);
	DDX_Check(pDX,IDC_CFG_CHK_FD_WCS,m_Config.bFdWcs);
	DDX_Check(pDX,IDC_CFG_CHK_FD_DOG,m_Config.bFdDog);
	DDX_Check(pDX,IDC_CFG_CHK_FD_SCFISH,m_Config.bFdScFish);
	DDX_Radio(pDX,IDC_CFG_RAD_BUYSEED_HIGHER,m_Config.nBuySeedType);
	DDX_Text(pDX,IDC_CFG_COMBO_BLACKSEEDID,m_Config.nBlackSeed);
	DDX_Text(pDX,IDC_CFG_COMBO_REDSEEDID,m_Config.nRedSeed);
	DDX_Text(pDX,IDC_CFG_COMBO_BLACK2SEEDID,m_Config.nBlack2Seed);
	DDX_Radio(pDX,IDC_CFG_RAD_PLANTING_ALLSEED,m_Config.nPlantingType);
	DDX_Text(pDX,IDC_CFG_EDIT_IGNORE,m_Config.nIgnoreFdLvl);
	DDX_Text(pDX,IDC_CFG_EDIT_WORKTIME,m_Config.nWorkTime);
	DDX_Text(pDX,IDC_CFG_EDIT_SLEEPTIME,m_Config.nSleepTime);
	DDX_Text(pDX,IDC_CFG_EDIT_SALENUM,m_Config.nSaleNum);
}


BEGIN_MESSAGE_MAP(CDlg_Config, CDialog)
	//{{AFX_MSG_MAP(CDlg_Config)
	ON_CBN_SELCHANGE(IDC_CFG_COMBO_BLACKSEED, OnSelchangeCfgComboBlackseed)
	ON_CBN_SELCHANGE(IDC_CFG_COMBO_REDSEED, OnSelchangeCfgComboRedseed)
	ON_BN_CLICKED(IDC_CFG_BTN_OK, OnCfgBtnOk)
	ON_BN_CLICKED(IDC_CFG_BTN_CANCEL, OnCfgBtnCancel)
	ON_CBN_SELCHANGE(IDC_CFG_COMBO_BLACK2SEED, OnSelchangeCfgComboBlack2seed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_Config message handlers

CDlg_Config::OnInitDialog()	//��ʼ��ȫ�����ô���
{
	CDialog::OnInitDialog();

	//������������б���ӱ�ͷ
	m_List_Crops.InsertColumn(0,"����",NULL,40,-1);
	m_List_Crops.InsertColumn(1,"ID",NULL,50,-1);
	m_List_Crops.InsertColumn(2,"����",NULL,140,-1);
	m_List_Crops.InsertColumn(3,"�ȼ�",NULL,50,-1);
	//�����б������ѡ��,��ʾ����,ѡ���
	m_List_Crops.SetExtendedStyle(LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	//��ȡ��������
	G_GetGlobalConfig(m_Config);

	std::vector<ST_CROPSGROW> v_Seeds;
	
	char szCrops[SHORTUSERLEN]={0};
	char szCrop[SHORTLEN]={0};
	//ȡ����Section��,�����ַ�����0��β,�����ַ�����00��β
	GetPrivateProfileSectionNames(szCrops,SHORTUSERLEN,strCropsIni);
	int nPos = 0;
	while(szCrops[nPos] != 0)
	{
		strcpy(szCrop,szCrops+nPos);	//�õ�������Section
		ST_CROPSGROW pCropGrow;
		G_GetCropInfo(atoi(szCrop),pCropGrow);	//��ȡ���Section������
		v_Seeds.push_back(pCropGrow);
		nPos += strlen(szCrop) + 1;
	}
	std::sort(v_Seeds.begin(),v_Seeds.end(),C_CropsLvlCmp);	//�������ӵȼ�����

	int nBlackSel = 0,nRedSel = 0,nBlack2Sel = 0;
	for (int i=0;i<v_Seeds.size();i++)
	{
		m_List_Crops.InsertItem(i,"");

		CString strBuf;
		strBuf.Format("%d",v_Seeds[i].Id);	//����ID
		m_List_Crops.SetItemText(i,1,strBuf);
		m_List_Crops.SetItemText(i,2,v_Seeds[i].Name);	//��������
		strBuf.Format("%d",v_Seeds[i].Level);			//���ӵȼ�
		m_List_Crops.SetItemText(i,3,strBuf);
		if (v_Seeds[i].IsLock || v_Seeds[i].Type == 11)
			m_List_Crops.SetCheck(i,TRUE);

		//�������۸�Ϊ0,�������ڼӹ���,���ӵȼ����ڻ�С���Լ��ĵȼ�,���������ѡ���
		if (v_Seeds[i].BuyPrice > 0 && v_Seeds[i].IsMill == FALSE && v_Seeds[i].IsVip == FALSE && v_Seeds[i].Level <= m_SelfInfo.Lvl)
		{
			CString strName,strId;
			strName.Format("%s(%d��)",v_Seeds[i].Name,v_Seeds[i].Level);	//����(�ȼ�)
			strId.Format("%d",v_Seeds[i].Id);								//ID
			if (v_Seeds[i].IsRed == 0)						//������Ǻ����������������ͨ����ѡ���
			{
				int nCount = m_Combo_BlackSeed.AddString(strName);
				m_Combo_BlackSeedId.AddString(strId);
				if (v_Seeds[i].Id == m_Config.nBlackSeed)
					nBlackSel = nCount;
			}
			if (v_Seeds[i].IsRed == 0 || v_Seeds[i].IsRed == 1)
			{
				if (v_Seeds[i].IsRed == 1)
					strName = "(��)" + strName;				//�����,���һ������
				int nCount = m_Combo_RedSeed.AddString(strName);
				m_Combo_RedSeedId.AddString(strId);
				if (v_Seeds[i].Id == m_Config.nRedSeed)
					nRedSel = nCount;
			}
			if (v_Seeds[i].IsRed == 0 || v_Seeds[i].IsRed == 1 || v_Seeds[i].IsRed == 2)
			{
				if (v_Seeds[i].IsRed == 2)
					strName = "(��)" + strName;				//�����,���һ������
				int nCount = m_Combo_Black2Seed.AddString(strName);
				m_Combo_Black2SeedId.AddString(strId);
				if (v_Seeds[i].Id == m_Config.nBlack2Seed)
					nBlack2Sel = nCount;
			}
		}
	}
	m_Combo_BlackSeed.SetCurSel(nBlackSel);
	m_Combo_BlackSeedId.SetCurSel(nBlackSel);
	m_Combo_RedSeed.SetCurSel(nRedSel);
	m_Combo_RedSeedId.SetCurSel(nRedSel);
	m_Combo_Black2Seed.SetCurSel(nBlack2Sel);
	m_Combo_Black2SeedId.SetCurSel(nBlack2Sel);

	UpdateData(FALSE);
	return TRUE;
}

void CDlg_Config::OnSelchangeCfgComboBlackseed() 
{
	m_Combo_BlackSeedId.SetCurSel(m_Combo_BlackSeed.GetCurSel());
}

void CDlg_Config::OnSelchangeCfgComboRedseed() 
{
	m_Combo_RedSeedId.SetCurSel(m_Combo_RedSeed.GetCurSel());
}

void CDlg_Config::OnSelchangeCfgComboBlack2seed() 
{
	m_Combo_Black2SeedId.SetCurSel(m_Combo_Black2Seed.GetCurSel());
}

UINT pCfgLockCrop(LPVOID pParam)
{
	CDlg_Config *hDlg_Config = (CDlg_Config *)pParam;
	CString strCrops;
	for (int i=0;i<hDlg_Config->m_List_Crops.GetItemCount();i++)
	{
		char szBuf[8]={0};
		hDlg_Config->m_List_Crops.GetItemText(i,1,szBuf,8);
		int nId = atoi(szBuf);
		BOOL bIsLock = hDlg_Config->m_List_Crops.GetCheck(i);
		ST_CROPSGROW pCropGrow;
		G_GetCropInfo(nId,pCropGrow);
		if (pCropGrow.IsLock != bIsLock)
		{
			CString strBuf;
			strBuf.Format("%d",nId);
			strBuf += "%3A";
			strBuf += bIsLock ? "1":"2";
			strBuf += "%2C";
			strCrops += strBuf;
			pCropGrow.IsLock = bIsLock;
			G_WriteCropInfo(pCropGrow);
		}
	}
	if (strCrops != "")
	{
		strCrops = strCrops.Left(strCrops.GetLength()-3);
		CString strPostData;
		strPostData.Format("uIdx=%d&uinY=%s&crop=%s",m_SelfInfo.Id,GetQQInfo(GQI_NUM),strCrops);
		CString strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_set_lock",strPostData);
	}
	return TRUE;
}
//��������
void CDlg_Config::OnCfgBtnOk()
{
	UpdateData(TRUE);
	G_WriteGlobalConfig(m_Config);
	CC_CreateThread(pCfgLockCrop,this,TRUE);
	OnCancel();
}

void CDlg_Config::OnCfgBtnCancel() 
{
	OnCancel();
}
