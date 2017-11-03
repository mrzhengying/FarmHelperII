// Dlg_Package.cpp : implementation file
//

#include "stdafx.h"
#include "QQPasture.h"
#include "Dlg_Package.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlg_Package *hDlg_Package;
/////////////////////////////////////////////////////////////////////////////
// CDlg_Package dialog


CDlg_Package::CDlg_Package(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Package::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_Package)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_Package::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_Package)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX,IDC_PACK_ANIMALS,m_List_Animals);
}


BEGIN_MESSAGE_MAP(CDlg_Package, CDialog)
	//{{AFX_MSG_MAP(CDlg_Package)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_Package message handlers

UINT pShowPackage(LPVOID pParam)
{
	BOOL bGetNetData = (BOOL)pParam;
	hDlg_Package->SetWindowText("背包 - 正在刷新数据");
	if (bGetNetData)
		F_GetUserPackage(NULL);

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
		if (pAnimalGrow.nPackage > 0)
		{
			int nCount = hDlg_Package->m_List_Animals.GetItemCount();
			CString strBuf;
			strBuf.Format("%d",pAnimalGrow.nPackage);
			nCount = hDlg_Package->m_List_Animals.InsertItem(nCount,strBuf);
			strBuf.Format("%d",pAnimalGrow.Id);
			hDlg_Package->m_List_Animals.SetItemText(nCount,1,strBuf);
			hDlg_Package->m_List_Animals.SetItemText(nCount,2,pAnimalGrow.Name);
			strBuf.Format("%d",pAnimalGrow.Level);
			hDlg_Package->m_List_Animals.SetItemText(nCount,3,strBuf);
		}
		nPos += strlen(szAnimal) + 1;
	}
	hDlg_Package->SetWindowText("背包");
	return TRUE;
}

CDlg_Package::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	hDlg_Package = (CDlg_Package *)this;

	//给列表添加标头
	m_List_Animals.InsertColumn(0,"数量",NULL,50,-1);
	m_List_Animals.InsertColumn(1,"ID",NULL,50,-1);
	m_List_Animals.InsertColumn(2,"名称",NULL,120,-1);
	m_List_Animals.InsertColumn(3,"等级",NULL,50,-1);
	m_List_Animals.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	//显示我的种子
	CC_CreateThread(pShowPackage,(LPVOID)TRUE,FALSE);

	return TRUE;
}