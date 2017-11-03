// Dlg_Seeds.cpp : implementation file
//

#include "stdafx.h"
#include "QQFarm.h"
#include "Dlg_Seeds.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlg_Seeds		*hDlg_Seeds;

/////////////////////////////////////////////////////////////////////////////
// CDlg_Seeds dialog


CDlg_Seeds::CDlg_Seeds(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Seeds::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_Seeds)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_Seeds::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_Seeds)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX,IDC_SED_CROPS,m_List_Crops);
}


BEGIN_MESSAGE_MAP(CDlg_Seeds, CDialog)
	//{{AFX_MSG_MAP(CDlg_Seeds)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_Seeds message handlers

UINT pShowSeeds(LPVOID pParam)
{
	BOOL bGetNetData = (BOOL)pParam;
	hDlg_Seeds->SetWindowText("背包 - 正在刷新数据");
	if (bGetNetData)
		F_GetUserSeed(NULL);

	char szCrops[SHORTUSERLEN]={0};
	char szCrop[SHORTLEN]={0};
	//取所有Section项,单个字符串以0结尾,整个字符串以00结尾
	GetPrivateProfileSectionNames(szCrops,SHORTUSERLEN,strCropsIni);
	int nPos = 0;
	while(szCrops[nPos] != 0)
	{
		strcpy(szCrop,szCrops+nPos);	//得到单个的Section
		ST_CROPSGROW pCropGrow;
		G_GetCropInfo(atoi(szCrop),pCropGrow);	//读取这个Section的数据
		if (pCropGrow.nSeedNum > 0)
		{
			int nCount = hDlg_Seeds->m_List_Crops.GetItemCount();
			CString strBuf;
			strBuf.Format("%d",pCropGrow.nSeedNum);
			nCount = hDlg_Seeds->m_List_Crops.InsertItem(nCount,strBuf);
			strBuf.Format("%d",pCropGrow.Id);
			hDlg_Seeds->m_List_Crops.SetItemText(nCount,1,strBuf);
			hDlg_Seeds->m_List_Crops.SetItemText(nCount,2,pCropGrow.Name);
			strBuf.Format("%d",pCropGrow.Level);
			hDlg_Seeds->m_List_Crops.SetItemText(nCount,3,strBuf);
		}
		nPos += strlen(szCrop) + 1;
	}
	hDlg_Seeds->SetWindowText("背包");
	return TRUE;
}

CDlg_Seeds::OnInitDialog()
{
	CDialog::OnInitDialog();

	hDlg_Seeds = (CDlg_Seeds *)this;

	//给种子列表添加标头
	m_List_Crops.InsertColumn(0,"数量",NULL,50,-1);
	m_List_Crops.InsertColumn(1,"ID",NULL,50,-1);
	m_List_Crops.InsertColumn(2,"名称",NULL,120,-1);
	m_List_Crops.InsertColumn(3,"等级",NULL,50,-1);
	m_List_Crops.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	//显示我的种子
	CC_CreateThread(pShowSeeds,(LPVOID)TRUE,FALSE);

	return TRUE;
}
