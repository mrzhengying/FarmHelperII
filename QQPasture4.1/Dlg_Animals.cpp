// Dlg_Animals.cpp : implementation file
//

#include "stdafx.h"
#include "QQPasture.h"
#include "Dlg_Animals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlg_Animals *hDlg_Animals;
/////////////////////////////////////////////////////////////////////////////
// CDlg_Animals dialog


CDlg_Animals::CDlg_Animals(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Animals::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_Animals)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_Animals::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_Animals)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX,IDC_ANIMALS_ANIMALS,m_List_Animals);
	DDX_Control(pDX,IDC_ANIMALS_SALEANIMALS,m_Btn_SaleAnimals);
}


BEGIN_MESSAGE_MAP(CDlg_Animals, CDialog)
	//{{AFX_MSG_MAP(CDlg_Animals)
	ON_BN_CLICKED(IDC_ANIMALS_SALEANIMALS, OnAnimalsSaleanimals)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_Animals message handlers

UINT pShowAnimals(LPVOID pParam)
{
	BOOL bGetNetData = (BOOL)pParam;
	hDlg_Animals->SetWindowText("仓库 - 正在刷新数据");
	if (bGetNetData)
		F_GetUserAnimals(NULL);

	hDlg_Animals->m_List_Animals.DeleteAllItems();
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
		if (pAnimalGrow.nAnimals > 0)
		{
			int nCount = hDlg_Animals->m_List_Animals.GetItemCount();
			CString strBuf;
			strBuf.Format("%d",pAnimalGrow.nAnimals);
			nCount = hDlg_Animals->m_List_Animals.InsertItem(nCount,strBuf);
			strBuf.Format("%d",pAnimalGrow.Id + 10000);
			hDlg_Animals->m_List_Animals.SetItemText(nCount,1,strBuf);
			hDlg_Animals->m_List_Animals.SetItemText(nCount,2,pAnimalGrow.Name);
			strBuf.Format("%d",pAnimalGrow.Level);
			hDlg_Animals->m_List_Animals.SetItemText(nCount,3,strBuf);
			strBuf.Format("%d",pAnimalGrow.nAnimals * pAnimalGrow.productprice);
			hDlg_Animals->m_List_Animals.SetItemText(nCount,4,strBuf);
		}
		if (pAnimalGrow.nWhelp > 0)
		{
			int nCount = hDlg_Animals->m_List_Animals.GetItemCount();
			CString strBuf;
			strBuf.Format("%d",pAnimalGrow.nWhelp);
			nCount = hDlg_Animals->m_List_Animals.InsertItem(nCount,strBuf);
			strBuf.Format("%d",pAnimalGrow.Id);
			hDlg_Animals->m_List_Animals.SetItemText(nCount,1,strBuf);
			hDlg_Animals->m_List_Animals.SetItemText(nCount,2,pAnimalGrow.Product);
			strBuf.Format("%d",pAnimalGrow.Level);
			hDlg_Animals->m_List_Animals.SetItemText(nCount,3,strBuf);
			strBuf.Format("%d",pAnimalGrow.nWhelp * pAnimalGrow.byproductprice);
			hDlg_Animals->m_List_Animals.SetItemText(nCount,4,strBuf);
		}
		nPos += strlen(szAnimal) + 1;
	}
	hDlg_Animals->SetWindowText("仓库");
	return TRUE;
}

CDlg_Animals::OnInitDialog()
{
	CDialog::OnInitDialog();

	hDlg_Animals = (CDlg_Animals *)this;

	//给列表添加标头
	m_List_Animals.InsertColumn(0,"数量",NULL,50,-1);
	m_List_Animals.InsertColumn(1,"ID",NULL,50,-1);
	m_List_Animals.InsertColumn(2,"名称",NULL,120,-1);
	m_List_Animals.InsertColumn(3,"等级",NULL,50,-1);
	m_List_Animals.InsertColumn(4,"价值",NULL,60,-1);
	m_List_Animals.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	CC_CreateThread(pShowAnimals,(LPVOID)TRUE,FALSE);

	return TRUE;
}

void CDlg_Animals::OnAnimalsSaleanimals() 
{
	m_Btn_SaleAnimals.EnableWindow(FALSE);
	CC_CreateThread(F_SaleUserAnimals,NULL,TRUE);
	//显示仓库植物
	CC_CreateThread(pShowAnimals,(LPVOID)FALSE,FALSE);
	m_Btn_SaleAnimals.EnableWindow(TRUE);
}
