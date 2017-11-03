// Dlg_Crops.cpp : implementation file
//

#include "stdafx.h"
#include "QQFarm.h"
#include "Dlg_Crops.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlg_Crops				*hDlg_Crops;

/////////////////////////////////////////////////////////////////////////////
// CDlg_Crops dialog


CDlg_Crops::CDlg_Crops(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Crops::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_Crops)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_Crops::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_Crops)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX,IDC_CRP_CROPS,m_List_Crops);
	DDX_Control(pDX,IDC_CRP_SALECROP,m_Btn_SaleCrop);
	DDX_Control(pDX,IDC_CRP_SAVELOCK,m_Btn_SaveLock);
}


BEGIN_MESSAGE_MAP(CDlg_Crops, CDialog)
	//{{AFX_MSG_MAP(CDlg_Crops)
	ON_BN_CLICKED(IDC_CRP_SALECROP, OnCrpSalecrop)
	ON_BN_CLICKED(IDC_CRP_SAVELOCK, OnCrpSavelock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_Crops message handlers

UINT pShowCrops(LPVOID pParam)
{
	BOOL bGetNetData = (BOOL)pParam;
	hDlg_Crops->SetWindowText("�ֿ� - ����ˢ������");
	if (bGetNetData)
		F_GetUserCrop(NULL);
	
	hDlg_Crops->m_List_Crops.DeleteAllItems();
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
		if (pCropGrow.nCropNum > 0)
		{
			int nCount = hDlg_Crops->m_List_Crops.GetItemCount();
			nCount = hDlg_Crops->m_List_Crops.InsertItem(nCount,"");
			if (pCropGrow.IsLock || pCropGrow.Type != 1)
				hDlg_Crops->m_List_Crops.SetCheck(nCount,TRUE);
			CString strBuf;
			strBuf.Format("%d",pCropGrow.nCropNum);
			hDlg_Crops->m_List_Crops.SetItemText(nCount,1,strBuf);
			strBuf.Format("%d",pCropGrow.Id);
			hDlg_Crops->m_List_Crops.SetItemText(nCount,2,strBuf);
			hDlg_Crops->m_List_Crops.SetItemText(nCount,3,pCropGrow.Name);
			strBuf.Format("%d",pCropGrow.nCropNum * pCropGrow.SalePrice);
			hDlg_Crops->m_List_Crops.SetItemText(nCount,4,strBuf);
		}
		nPos += strlen(szCrop) + 1;
	}
	hDlg_Crops->SetWindowText("�ֿ�");
	return TRUE;
}

CDlg_Crops::OnInitDialog()
{
	CDialog::OnInitDialog();

	hDlg_Crops = (CDlg_Crops *)this;

	//��ֲ���б���ӱ�ͷ
	m_List_Crops.InsertColumn(0,"����",NULL,50,-1);
	m_List_Crops.InsertColumn(1,"����",NULL,50,-1);
	m_List_Crops.InsertColumn(2,"ID",NULL,50,-1);
	m_List_Crops.InsertColumn(3,"����",NULL,120,-1);
	m_List_Crops.InsertColumn(4,"��ֵ",NULL,60,-1);
	m_List_Crops.SetExtendedStyle(LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	//��ʾ�ֿ�ֲ��
	CC_CreateThread(pShowCrops,(LPVOID)TRUE,FALSE);

	return TRUE;
}

void CDlg_Crops::OnCrpSalecrop() 
{
	m_Btn_SaveLock.EnableWindow(FALSE);
	m_Btn_SaleCrop.EnableWindow(FALSE);
	CC_CreateThread(F_SaleUserCrop,NULL,TRUE);
	//��ʾ�ֿ�ֲ��
	CC_CreateThread(pShowCrops,(LPVOID)FALSE,FALSE);
	m_Btn_SaveLock.EnableWindow(TRUE);
	m_Btn_SaleCrop.EnableWindow(TRUE);
}

UINT pLockCrop(LPVOID pParam)
{
	CString strCrops;
	for (int i=0;i<hDlg_Crops->m_List_Crops.GetItemCount();i++)
	{
		char szBuf[8]={0};
		hDlg_Crops->m_List_Crops.GetItemText(i,2,szBuf,8);
		int nId = atoi(szBuf);
		BOOL bIsLock = hDlg_Crops->m_List_Crops.GetCheck(i);
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

void CDlg_Crops::OnCrpSavelock() 
{
	m_Btn_SaveLock.EnableWindow(FALSE);
	m_Btn_SaleCrop.EnableWindow(FALSE);
	//�޸�����״̬
	CC_CreateThread(pLockCrop,NULL,TRUE);
	m_Btn_SaveLock.EnableWindow(TRUE);
	m_Btn_SaleCrop.EnableWindow(TRUE);
}
