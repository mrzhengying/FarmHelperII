// Dlg_Main.cpp : implementation file
//

#include "stdafx.h"
#include "QQFarm.h"
#include "Dlg_Main.h"	//������
#include "Dlg_Config.h"	//���ô���
#include "Dlg_Seeds.h"	//��������
#include "Dlg_Crops.h"	//�ֿⴰ��


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//�궨��																//
//////////////////////////////////////////////////////////////////////////
#define WM_UPDATEDATA		WM_USER + 1		//ˢ�´�������
#define IDT_SERVERTIMER		1				//ˢ�·�����ʱ��
#define IDT_SHOWMYRIPE		2				//ˢ���ҵ�ũ������ʱ��
#define IDT_SHOWFRIENDSRIPE 3				//ˢ�º����б����ʱ��
#define IDT_GETSTATUSFILTER 4				//���˺���״̬
#define IDT_GETMYFARM		5				//ˢ���ҵ�ũ��
#define IDT_GETFRIENDFARM	6				//ˢ�º���ũ��
#define IDT_RESETALLCTRL	7				//���賿ʱ�������п��Ʊ���
#define IDT_WORKTIME		8				//������ʱ��
#define IDT_SLEEPTIME		9				//��Ϣ��ʱ��
#define IDT_CHECKSERVERTIMER 10				//У��������ʱ��
//////////////////////////////////////////////////////////////////////////
//ȫ�ֱ���																//
//////////////////////////////////////////////////////////////////////////
//�󶨴��ڿؼ�
CString						strName;		//�ǳ�
CString						strLevel;		//�ȼ�
CString						strGold;		//���
CString						strExp;			//����
CString						strDog;			//��
CString						strHealthMode;	//��ȫģʽ
CString						strUpLvlExp;	//�������辭��
int							numMin,numMax,numPos;
//WorkThread�¼�����
BOOL						bGetBlogData = TRUE;	//��BLOG��ȡ��Ҫ����
BOOL						bGetServerTime = TRUE;	//��ȡũ��������ʱ��
BOOL						bGetXmlData	= TRUE;		//��ȡũ����Դ����
BOOL						bGetFriends = TRUE;		//��ȡ�����б�
BOOL						bScanFriends = TRUE;	//ɨ�����ũ��
BOOL						bGetStatusFilter = FALSE; //���˺���״̬
BOOL						bWorking	= TRUE;		//����״̬
//HandleFarm�¼�����
BOOL						bWcsFriends = TRUE;		//Ϊ���ѳ���ɱ�潽ˮ

ST_USERLIST					m_SelfInfo;			//�ҵ�ũ��,��������
std::vector<ST_USERLIST>	v_FriendsInfo;		//�ҵĺ���,��������
int							m_SelfLandId = 0;	//��ǰ��ʾ��ID
ST_GLOBALCONFIG				m_Config;			//ȫ������
ST_MYBEASTINFO				m_MyBeast[3];		//�ҵ�Ұ����������

CString						m_gtk;				//Cookie Gtk
//////////////////////////////////////////////////////////////////////////
//ǰ�˶���																//
//////////////////////////////////////////////////////////////////////////
UINT N_RootThread(LPVOID pParam);		//������ʼ��,����,�߳�
UINT N_GetBlogData(LPVOID pParam);		//��BLOG��ȡ��Ҫ����
UINT N_GetXmlData(LPVOID pParam);		//��ȡ��Դ�汾,������Դ
UINT H_CheckPost(CString &strHtml);		//���POST���ص�����,�Ƿ������⴦��,����0:�޴���,����1:���µ�¼,����2:ϵͳ��æ,����3��֤��
UINT H_WorkThread(LPVOID pParam);		//ȫ�ֹ����߳�
UINT H_GetFarmInfo(CString &strHtml,ST_USERINFO &pUserInfo); //�����ҵ�ũ������
UINT H_GetCropsInfo(int nId,ST_CROPSGROW &pCropGrow);	//��INI�ļ���ȡָ��ֲ������
DWORD H_GetFarmLandRipe(ST_CROPLANDINFO pCropLand[24]);	//����ũ���������,�������һ���ճɵ�ʱ��
UINT H_HandleMyFarm(LPVOID pParam);		//�����ҵ�ũ��������,�����ճ��������ﴦ��
UINT H_HandleFriendFarm(LPVOID pParam);	//�������ũ��������,͵��������
UINT F_GetStatusFilter(LPVOID pParam);	//���˺���״̬
UINT F_ScanFarmInfo(LPVOID pParam);		//POST��ȡũ�������ı�,ʹ��H_GetFarmInfo���д���
UINT F_GetFarmInfo(LPVOID pParam);
UINT F_GetFriends(LPVOID pParam);		//��ȡ�����б�
UINT F_GetUserSeed(LPVOID pParam);		//��ȡ��������
UINT F_GetUserCrop(LPVOID pParam);		//��ȡ�ֿ�����
UINT S_ShowUserInfo(LPVOID pParam);		//��ʾ���ݵ�������
void S_ShowFriendsRipeTimeOnPos(int nId);	//��ʾָ������ũ������ʱ��
void S_ShowFriendsRipeTime();			//��ʾ���к���ũ������ʱ��
void S_ShowMyRipeTime();				//��ʾ�ҵ�ũ����������
UINT G_GetUserInfo(int nId,ST_USERINFO &pUserInfo);	//��INI�ļ���ȡ��ϸ�û�����
UINT G_WriteUserInfo(ST_USERINFO &pUserInfo);		//����ϸ�û�����д��INI
UINT G_GetUserList(int nId,ST_USERLIST &pUserList);	//��INI�ļ���ȡ�����û�����
UINT G_WriteUserList(ST_USERLIST &pUserList);		//�������û�����д��INI
UINT G_GetCropInfo(int nId,ST_CROPSGROW &pCropGrow);//��INI�ļ���ȡֲ������
UINT G_WriteCropInfo(ST_CROPSGROW &pCropGrow);		//��ֲ������д��INI�ļ�
UINT G_GetGlobalConfig(ST_GLOBALCONFIG &pConfig);
UINT G_WriteGlobalConfig(ST_GLOBALCONFIG &pConfig);
UINT G_GetCardsGame(int nId,ST_CARDSGAME &pCardsGame);
UINT G_WriteCardsGame(ST_CARDSGAME &pCardsGame);
UINT G_GetBeastInfo(int nId,ST_BEASTINFO &pBeastInfo);
UINT G_WriteBeastInfo(ST_BEASTINFO &pBeastInfo);
UINT G_GetCrystalInfo(int nId,ST_CRYSTALINFO &pCrystalInfo);
UINT G_WriteCrystalInfo(ST_CRYSTALINFO &pCrystalInfo);
UINT N_SetGlobalTimer(LPVOID pParam);				//����ȫ�ּ�ʱ��
/////////////////////////////////////////////////////////////////////////////
// CDlg_Main dialog


CDlg_Main::CDlg_Main(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Main::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlg_Main)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlg_Main::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//�󶨿ؼ���������
	DDX_Control(pDX,IDC_LIST_LANDS,m_List_Lands);
	DDX_Control(pDX,IDC_LIST_FRIENDS,m_List_Friends);
	DDX_Control(pDX,IDC_PGS_UPLVLEXP,m_Pgs_UplvExp);
	//�󶨿ؼ��ı�
	DDX_Text(pDX,IDC_MAIN_NAME,strName);
	DDX_Text(pDX,IDC_MAIN_LEVEL,strLevel);
	DDX_Text(pDX,IDC_MAIN_GOLD,strGold);
	DDX_Text(pDX,IDC_MAIN_EXP,strExp);
	DDX_Text(pDX,IDC_MAIN_DOG,strDog);
	DDX_Text(pDX,IDC_MAIN_SAFETY,strHealthMode);
	DDX_Text(pDX,IDC_MAIN_UPLVLEXP,strUpLvlExp);
}


BEGIN_MESSAGE_MAP(CDlg_Main, CDialog)
	//{{AFX_MSG_MAP(CDlg_Main)
	ON_WM_PAINT()
	ON_MESSAGE(WM_UPDATEDATA,OnUpdateData)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_LIST_FRIENDS, OnClickListFriends)
	ON_BN_CLICKED(IDC_MAIN_BTN_MYFARM, OnMainBtnMyfarm)
	ON_BN_CLICKED(IDC_MAIN_BTN_CONFIG, OnMainBtnConfig)
	ON_BN_CLICKED(IDC_MAIN_BTN_SHOWSEED, OnMainBtnShowseed)
	ON_BN_CLICKED(IDC_MAIN_BTN_SHOWCROP, OnMainBtnShowcrop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_Main message handlers

BOOL CDlg_Main::OnInitDialog()	//��ʼ������
{
	CDialog::OnInitDialog();	
	
	hWndMain = this->m_hWnd;
	//�������б����ӱ�ͷ
	m_List_Lands.InsertColumn(0,"���",NULL,40,-1);
	m_List_Lands.InsertColumn(1,"����",NULL,100,-1);
	m_List_Lands.InsertColumn(2,"�ȼ�",NULL,40,-1);
	m_List_Lands.InsertColumn(3,"��",NULL,40,-1);
	m_List_Lands.InsertColumn(4,"�����",NULL,112,-1);
	//�����б������ѡ�����ʾ����
	m_List_Lands.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	//�������б����ӱ�ͷ
	m_List_Friends.InsertColumn(0,"���",NULL,40,-1);
	m_List_Friends.InsertColumn(1,"�ǳ�",NULL,90,-1);
	m_List_Friends.InsertColumn(2,"�ȼ�",NULL,40,-1);
	m_List_Friends.InsertColumn(3,"���",NULL,70,-1);
	m_List_Friends.InsertColumn(4,"�����",NULL,110,-1);
	//�����б������ѡ�����ʾ����
	m_List_Friends.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	//��ʼ��������ձ�
	pInitExpCmpList();

	//��ʾQQͷ��
	CC_CreateThread(pShowHeadPic,(LPVOID)atoi(GetQQInfo(GQI_NUM)),FALSE);

	//���õ�ǰQQ��ר��Ŀ¼
	strSelfPath = GetAppPath(GAP_DIR) + GetQQInfo(GQI_NUM);
	CreateDirectory(strSelfPath,NULL);
	strCropsIni =		strSelfPath + "\\QQFarm_Crops.ini";
	strFriendsDataIni = strSelfPath + "\\QQFarm_FriendsData.ini";
	strFriendsListIni = strSelfPath + "\\QQFarm_FriendsList.ini";
	strQQFarm_ini =		strSelfPath + "\\QQFarm_Config.ini";
	strAddonIni	=		strSelfPath + "\\QQFarm_Addon.ini";
	strBeastIni =		strSelfPath + "\\QQFarm_Beast.ini";
	strCrystalIni =		strSelfPath + "\\QQFarm_Crystal.ini";

	//���汾����,�����ͬ��ִ�и��´���
	CString DataVersion;
	GetPrivateProfileString("version","dataversion",NULL,DataVersion.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQFarm_ini);
	DataVersion.ReleaseBuffer();
	int nDataVersion = atoi(DataVersion);
	if (nDataVersion == 0)
	{
		//ǿ�Ƹ�����չ����
		WritePrivateProfileString("xmldata","config_addon","",strQQFarm_ini);
		nDataVersion ++;
	}
	if (nDataVersion == 1)
	{
		//ȫ�������б仯,��ʾ��������
		CC_PopTrayMsg("QQũ��","ȫ�������б䶯,����������",PTM_INFO,2000);
		nDataVersion ++;
	}
	//д�뵱ǰ�汾����
	WritePrivateProfileString("version","dataversion","2",strQQFarm_ini);

	//������ʼ�������������߳�
	CC_CreateThread(N_RootThread,NULL,FALSE);

	return TRUE;
}

LRESULT CDlg_Main::OnUpdateData(WPARAM wParam,LPARAM lParam)
{
	return UpdateData((BOOL)wParam);
}

void CDlg_Main::OnPaint() 
{
	CPaintDC dc(this);
	if(HeadPic_pPic)
	{
		HeadPic_pPic->Render(dc.m_hDC, 12, 8, 50, 50, 0, HeadPic_nHeight, HeadPic_nWidth, -HeadPic_nHeight, NULL);
	}
}
void CDlg_Main::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case IDT_SERVERTIMER:	//ˢ�·�����ʱ��
		dwServerTime++;
		break;
	case IDT_SHOWMYRIPE:
		S_ShowMyRipeTime();
		break;
	case IDT_SHOWFRIENDSRIPE:
		S_ShowFriendsRipeTime();
		break;
	case IDT_GETMYFARM:
		m_SelfInfo.RipeTime = dwServerTime - 1;
		break;
	case IDT_GETSTATUSFILTER:
		bGetStatusFilter = TRUE;
		break;
	case IDT_GETFRIENDFARM:
		bScanFriends = TRUE;
		bGetXmlData = TRUE;
		bGetBlogData = TRUE;
		break;
	case IDT_RESETALLCTRL:
		bWcsFriends = TRUE;		//����Ϊ���ѳ���ɱ��
		bGetXmlData = TRUE;
		N_SetGlobalTimer(NULL);	//�����������м�ʱ��
		break;
	case IDT_WORKTIME:
		CC_Addtolist("[QQũ��]:����ʱ�����,׼����ͣ",COLOR_INFO);
		bWorking = FALSE;
		KillTimer(IDT_WORKTIME);
		SetTimer(IDT_SLEEPTIME,50*60*1000,NULL);
		break;
	case IDT_SLEEPTIME:
		CC_Addtolist("[QQũ��]:��ͣʱ�����,׼����������",COLOR_INFO);
		bWorking = TRUE;
		KillTimer(IDT_SLEEPTIME);
		SetTimer(IDT_WORKTIME,120*60*1000,NULL);
		break;
	case IDT_CHECKSERVERTIMER:
		bGetServerTime = TRUE;
		break;
	}
	CDialog::OnTimer(nIDEvent);
}
void CDlg_Main::OnClickListFriends(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (m_List_Friends.GetSelectedCount() > 0)
	{
		int nMark = m_List_Friends.GetSelectionMark();
		SetDlgItemText(IDC_MAIN_BTN_MYFARM,"�ҵ�ũ��");
		ST_USERINFO pUserInfo;
		G_GetUserInfo(v_FriendsInfo[nMark].Id,pUserInfo);
		//��ʾQQͷ��
		CC_CreateThread(pShowHeadPic,(LPVOID)pUserInfo.QQ,FALSE);
		//��ʾ�û���Ϣ
		S_ShowUserInfo((LPVOID)pUserInfo.Id);
		//����Ҫ��ʾ��ID
		m_SelfLandId = pUserInfo.Id;
		S_ShowMyRipeTime();
	}
	*pResult = 0;
}
void CDlg_Main::OnMainBtnMyfarm() 
{
	if (m_SelfLandId == m_SelfInfo.Id)
	{
		m_SelfInfo.RipeTime = dwServerTime - 1;
	}
	else
	{
		SetDlgItemText(IDC_MAIN_BTN_MYFARM,"ˢ��ũ��");
		ST_USERINFO pUserInfo;
		G_GetUserInfo(m_SelfInfo.Id,pUserInfo);
		//��ʾQQͷ��
		CC_CreateThread(pShowHeadPic,(LPVOID)pUserInfo.QQ,FALSE);
		//��ʾ�û���Ϣ
		S_ShowUserInfo((LPVOID)m_SelfInfo.Id);
		//������ʾID
		m_SelfLandId = m_SelfInfo.Id;
		S_ShowMyRipeTime();
	}
}
void CDlg_Main::OnMainBtnConfig() 
{
	CDlg_Config	hDlg_Config;
	hDlg_Config.DoModal();
}
void CDlg_Main::OnMainBtnShowseed() 
{
	CDlg_Seeds hDlg_Seeds;
	hDlg_Seeds.DoModal();
}
void CDlg_Main::OnMainBtnShowcrop() 
{
	CDlg_Crops hDlg_Crops;
	hDlg_Crops.DoModal();
}
//////////////////////////////////////////////////////////////////////////
//���ô���򻯺���														//
//////////////////////////////////////////////////////////////////////////
CString N_HtmlToText(CString strHtml)
{
	while(TRUE)
	{
		CString strBuf;
		int nPos1=0,nPos2=0;
		nPos1=strHtml.Find("<",0);
		nPos2=strHtml.Find(">",0);
		if (nPos1 == -1 || nPos2 == -1)
			break;
		strBuf = strHtml.Mid(nPos1,nPos2-nPos1+1);
		strHtml.Replace(strBuf,"");
	}
	return strHtml;
}
UINT H_CheckPost(CString &strHtml)
{
	if (strHtml.Find("\"ecode\"") != -1 || strHtml.Find("\"code\"") != -1)
	{
		if(strHtml.Find("�����µ�¼")!=-1)
			return 1;
		if(strHtml.Find("ϵͳ��æ")!=-1)
			return 2;
		if(strHtml.Find("��������֤��")!=-1)
			return 3;
	}
	return 0;
}
CString N_FarmPostUrl(CString strHost,CString strUrl,CString strPostData)
{
	CString strHtml;

	//��ȡgtk
	CC_GetGTK(m_gtk.GetBufferSetLength(SHORTTEXTLEN));
	m_gtk.ReleaseBuffer();
	
	//����һ��URL
	if (strUrl.Find("?") == -1)
		strUrl = strUrl + "?g_tk=" + m_gtk;
	else
		strUrl = strUrl + "&g_tk=" + m_gtk;

	BOOL bSuccess = FALSE;
	while(!bSuccess)
	{
		//����ѭ������
		int nErrorLoop = 0;
		//�Ե�ǰʱ���ȡKEY��Կ
		CString strVCode;	//��֤��
M1:
		if (nErrorLoop > 2)
		{
			strHtml = "";
			break;
		}
		time_t time;
		CString strFarmKey = FarmTimeMD5FarmKey(time);
		CString szPostData;	//����POST����
		szPostData.Format("farmKey=%s&farmTime=%d&%s",strFarmKey,(DWORD)time,strPostData);
		if (strVCode != "")	//�����֤�벻Ϊ��,����ӵ�POST����
		{
			szPostData = szPostData + "&validatemsg=" + strVCode;
			strVCode = "";		//�����֤��
		}
		//POST�ύ,�ȴ������ı�
		strHtml = PostUrl(strHost,strUrl,strPostHeader,szPostData,TRUE);
		switch (H_CheckPost(strHtml))	//������,���ݶ�Ӧ������д���
		{
		case 1: //��Ҫ���µ�¼
			//MessageBox(hWndMain,"��Ҫ���µ�¼","��ʾ",0);
			::SendMessage(CChWndMain,EVENT_ALL_RELOGIN,0,0);	
			goto M1;	//����M1�����ٴ�ִ��
		case 2: //ϵͳ��æ,�Ժ�����
			CC_Addtolist("[QQũ��]:ϵͳ��æ,��ȡ����ʧ��,һ���Ӻ�����.",COLOR_ERROR);
			Sleep(60000); //��ͣһ����
			nErrorLoop++;
			goto M1;	//����M1�����ٴ�ִ��
		case 3:	//��Ҫ������֤��
			//MessageBox(hWndMain,"��Ҫ������֤��","��ʾ",0);
			::SendMessage(CChWndMain,EVENT_ALL_INPUTVERIFYCODE,(LONG)strVCode.GetBufferSetLength(QQVCLEN),0);
			strVCode.ReleaseBuffer();
			goto M1;	//����M1�����ٴ�ִ��
		}
		bSuccess = TRUE;//���û����,���޸��ж�,����ѭ��
	}
	return strHtml;
}
UINT N_SetGlobalTimer(LPVOID pParam)
{
	KillTimer(hWndMain,IDT_GETMYFARM);
	SetTimer(hWndMain,IDT_GETMYFARM,7200000,NULL);	//��Сʱˢ���ҵ�ũ��

	KillTimer(hWndMain,IDT_GETSTATUSFILTER);
	SetTimer(hWndMain,IDT_GETSTATUSFILTER,7200000,NULL);  //����Сʱ����һ�º���״̬

	KillTimer(hWndMain,IDT_GETFRIENDFARM);
	SetTimer(hWndMain,IDT_GETFRIENDFARM,43200000,NULL);	 //ʮ����Сʱˢ�����к���ũ��

	CTime tNow;
	tNow = CTime::GetCurrentTime();
	int nH = tNow.GetHour();
	int nM = tNow.GetMinute();
	int nS = tNow.GetSecond();
	int nNow = (nH*60*60)+(nM*60)+(nS);	//��ǰʱ��
	nNow = 86400 - nNow + 300;
	nNow *= 1000;
	KillTimer(hWndMain,IDT_RESETALLCTRL);
	SetTimer(hWndMain,IDT_RESETALLCTRL,nNow,NULL);

	KillTimer(hWndMain,IDT_WORKTIME);
	KillTimer(hWndMain,IDT_SLEEPTIME);
	SetTimer(hWndMain,IDT_WORKTIME,m_Config.nWorkTime*60*1000,NULL);	//������ʱ
	bWorking = TRUE;

	KillTimer(hWndMain,IDT_CHECKSERVERTIMER);
	SetTimer(hWndMain,IDT_CHECKSERVERTIMER,14400000,NULL);	//У�������ʱ��
	return TRUE;
}
//�˴�ִ�����г�ʼ������,���������й����߳�
UINT N_RootThread(LPVOID pParam)
{
	//����ȫ�ּ�ʱ��
	N_SetGlobalTimer(NULL);
	//���������߳�
	while(!H_WorkThread(NULL)){Sleep(5000);};
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//��ʼ����Ҫ����														//
//////////////////////////////////////////////////////////////////////////
//��ȡBLOG����
UINT N_GetBlogData(LPVOID pParam)
{
	//��ȡBLOG��ҳ����
	CString szHtml = GetUrl("http://hi.baidu.com/stevety/blog/item/1635856283f42d780d33fa43.html",FALSE);
	int nPos1,nPos2;
	
	//��ѯũ��KEY
	nPos1 = szHtml.Find("[farmkey]");
	nPos2 = szHtml.Find("[/farmkey]",nPos1);
	if (nPos1 == -1 || nPos2 == 1)
		return FALSE;
	strQqFarmKey = szHtml.Mid(nPos1 + 9,nPos2 - nPos1 - 9);
	//����ũ��KEY
	strQqFarmKey=UncrypKey(strQqFarmKey,"");
	
	return TRUE;
}
//��ȡ��Դ����
UINT N_GetXmlData(LPVOID pParam)
{
	//��ȡ��ҳ������
	CString strUrl;
	strUrl.Format("http://appbase.qzone.qq.com/cgi-bin/index/appbase_run_unity.cgi?appid=353&max=0&uin=%s&keyname=353&params=&qz_style=100&canvastype=home",GetQQInfo(GQI_NUM));
	CString szHtml = GetUrl(strUrl,TRUE);
	//��ѯ���°��XML
	CString	strConfig_url;
	CString	strConfig_data;
	CString strConfig_addon;
	if(FindMidStr(szHtml,0,"\"config_url\":\"","\"",strConfig_url) == -1 ||
		FindMidStr(szHtml,0,"\"config_data\":\"","\"",strConfig_data) == -1 ||
		FindMidStr(szHtml,0,"\"config_addon\":\"","\"",strConfig_addon) == -1)
		return FALSE;

	//��ȡ��������
	G_GetGlobalConfig(m_Config);

	//�����ö�ȡ��¼��XML�汾
	CString strConfig_url_ini;
	CString strConfig_data_ini;
	CString strConfig_addon_ini;
	GetPrivateProfileString("xmldata","config_url",NULL,strConfig_url_ini.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQFarm_ini);
	strConfig_url_ini.ReleaseBuffer();
	GetPrivateProfileString("xmldata","config_data",NULL,strConfig_data_ini.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQFarm_ini);
	strConfig_data_ini.ReleaseBuffer();
	GetPrivateProfileString("xmldata","config_addon",NULL,strConfig_addon_ini.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQFarm_ini);
	strConfig_addon_ini.ReleaseBuffer();

	//����ҳ��XML�汾�ͱ��ش洢��XML�汾���жԱ�,��ͬ�����SWF����
	CString strModuleUrl;
	CString strLoadingUrl;
	if (strConfig_url != strConfig_url_ini)
	{
		CC_Addtolist("[QQũ��]:�����и��°汾,��ʼ��������Э��",COLOR_INFO);
		CC_PopTrayMsg("[QQũ��]","�����и��°汾,��ʼ��������Э��",PTM_INFO,2000);
		//���ذ汾XML��ֲ������XML
		CString strConfig_url_xml = GetAppPath(GAP_DIR)+"Load_Main.xml";
		if (!GetUrlToFile(strConfig_url,strConfig_url_xml,TRUE))
			return FALSE;
		//��ʼ����XML����
		CMarkup xml;
		if (!xml.Load(strConfig_url_xml))			//����XML
			return FALSE;
		xml.ResetMainPos();							//��������λ��,����������
		xml.FindElem("data");						//���²���dataԪ��
		xml.IntoElem();								//����Ԫ���ڲ�
		xml.FindElem("loading");					//��data�ڲ�����loadingԪ��
		strLoadingUrl=xml.GetAttrib("url");			//��ȡloadingԪ�ص�urlԪ��
		xml.FindElem("moduleList");					//��data�ڲ�����modulelistԪ��
		xml.IntoElem();								//����modulelistԪ���ڲ�
		xml.FindElem("module");						//���ڲ�����moduleԪ��
		strModuleUrl=xml.GetAttrib("url");			//��ȡmoduleԪ�ص�urlԪ��
		DeleteFile(strConfig_url_xml);				//ɾ��XML�ļ�
		WritePrivateProfileString("xmldata","config_url",strConfig_url,strQQFarm_ini);
		WritePrivateProfileString("xmldata","loading",strLoadingUrl,strQQFarm_ini);
		WritePrivateProfileString("xmldata","module",strModuleUrl,strQQFarm_ini);
	}
	else
	{
		//�����ͬ,��������ж�ȡ����
		GetPrivateProfileString("xmldata","loading",NULL,strLoadingUrl.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQFarm_ini);
		strLoadingUrl.ReleaseBuffer();
		GetPrivateProfileString("xmldata","module",NULL,strModuleUrl.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQFarm_ini);
		strModuleUrl.ReleaseBuffer();
	}
	//���������ũ��������,POST�ύʱ�����,��Ҫ��moduleԪ��ȡ����url��������
	strPostHeader = "Accept: */*\r\nReferer: " + strModuleUrl + "\r\nx-flash-version: 10,2,152,32\r\nAccept-Language: zh-cn\r\nContent-Type: application/x-www-form-urlencoded\r\nAccept-Encoding\r\n";
	
M1:
	CString strPostData;	//����POST����
	strPostData.Format("0=%s&uIdx=&uinY=",strLoadingUrl);
	szHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_index?mod=user&act=run",strPostData);

	if (szHtml.Find("\"uId\":",0) == -1)
	{
		Sleep(5000);
		goto M1;
	}
	
	CString strBuf;
	int nYellowLevel = 0,nYellowStatus = 0;
	FindMidStr(szHtml,0,"\"yellowlevel\":",",",strBuf);
	nYellowLevel = atoi(strBuf);
	FindMidStr(szHtml,0,"\"yellowstatus\":",",",strBuf);
	nYellowStatus = atoi(strBuf);

	//�������ı�����ָ����������,�����Զ���ṹ������
	ST_USERINFO pUserInfo;
	H_GetFarmInfo(szHtml,pUserInfo);
	CC_GetQQInfo(pUserInfo.Name,GQI_NAME);	//���ص�������ȡ�����ǳ�,ʹ�ù�������ȡһ��
	pUserInfo.IsQzone = TRUE;				//�ж�Ϊ�ռ�

	G_WriteUserInfo(pUserInfo);				//�洢�û����ݵ�Ӳ��
	S_ShowUserInfo((LPVOID)pUserInfo.Id);	//��ʾ�û���Ϣ������

	//����Ҫ����д������û����ݽṹ,�洢���ڴ���
	m_SelfInfo.Id = pUserInfo.Id;
	m_SelfInfo.Lvl = FarmExpToLevel(pUserInfo.Exp);
	m_SelfInfo.RipeTime = H_GetFarmLandRipe(pUserInfo.CropLand);
	m_SelfInfo.CheckTime = dwServerTime;

	//����ҳ��XML�汾�ͱ��ش洢��XML�汾���жԱ�,��ͬ�����ֲ������
	if (strConfig_data != strConfig_data_ini)
	{
		CC_Addtolist("[QQũ��]:ũ���������б仯,��ʼ������������",COLOR_INFO);
		//����ֲ������XML������
		CString strConfig_data_xml = GetAppPath(GAP_DIR)+"CropGrow.xml";
		if (!GetUrlToFile(strConfig_data,strConfig_data_xml,TRUE))
			return FALSE;
		CMarkup xml;	//��ʼ����XML
		if (!xml.Load(strConfig_data_xml))
			return FALSE;
		xml.ResetMainPos();		//��������λ��,����������
		xml.FindElem("data");	//����dataԪ��
		xml.IntoElem();			//����dataԪ���ڲ�
		xml.FindElem("crops");	//����cropsԪ��
		CString strCrops=xml.GetData();	//��ȡcropsԪ�ص�����

		Sleep(2000);
		//��ȡ�̵�ֲ������,����XMLȱ�ٵ�����
		strPostData = "";
		strPostData.Format("uIdx=%d&uinY=%s",m_SelfInfo.Id,GetQQInfo(GQI_NUM));
		CString strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_getseedinfo?mod=usertool&act=getSeedInfo",strPostData);

		//׼����������ֲ������
		int nOffset=0,nIndex=0;
		CString strBuf;
		
		nOffset=FindMidStr(strCrops,nOffset,"\"cropGrow\": \"","}",strBuf);
		while(nOffset!=-1)
		{
			ST_CROPSGROW st_Crops;
			CString strItem;
			FindMidStr(strBuf,0,"\"id\":",",",strItem);
			st_Crops.Id=atoi(strItem);

			G_GetCropInfo(st_Crops.Id,st_Crops);

			FindMidStr(strBuf,0,"\"type\":","",strItem);
			st_Crops.Type=atoi(strItem);
			FindMidStr(strBuf,0,"\"name\": \"","\"",strItem);
			strcpy(st_Crops.Name,strItem.GetBuffer(strItem.GetLength()));
			strItem.ReleaseBuffer();
			FindMidStr(strBuf,0,"\"harvestNum\":",",",strItem);
			st_Crops.HarvestNum=atoi(strItem);
			
			st_Crops.IsRed=FindMidStr(strBuf,0,"\"isRed\": \"","\"",strItem)==-1 ? 0 : 1 ;
			st_Crops.IsRestaurant=FindMidStr(strBuf,0,"\"isRestaurant\": \"","\"",strItem)==-1 ? 0 : 1 ;
			st_Crops.IsFood=FindMidStr(strBuf,0,"\"isFood\": \"","\"",strItem)==-1 ? 0 : 1 ;
			//st_Crops.IsAnimation=FindMidStr(strBuf,0,"\"isAnimation\": \"","\"",strItem)==-1 ? 0 : 1 ;
			st_Crops.IsFlower=FindMidStr(strBuf,0,"\"isFlower\": \"","\"",strItem)==-1 ? 0 : 1 ;
			st_Crops.IsMill=FindMidStr(strBuf,0,"\"isMill\": \"","\"",strItem)==-1 ? 0 : 1 ;
			//st_Crops.IsActivity=FindMidStr(strBuf,0,"\"isActivity\": \"","\"",strItem)==-1 ? 0 : 1 ;
			if (FindMidStr(strBuf,0,"\"isLock\": ","\"",strItem) != -1)
			{
				strItem.Replace("\"","");
				st_Crops.IsLock = atoi(strItem);
			}
			
			nIndex=0;
			nIndex=FindMidStr(strBuf,nIndex,"",",",strItem);
			st_Crops.CropGrow[0]=atol(strItem);
			nIndex=FindMidStr(strBuf,nIndex,"",",",strItem);
			st_Crops.CropGrow[1]=atol(strItem);
			nIndex=FindMidStr(strBuf,nIndex,"",",",strItem);
			st_Crops.CropGrow[2]=atol(strItem);
			nIndex=FindMidStr(strBuf,nIndex,"",",",strItem);
			st_Crops.CropGrow[3]=atol(strItem);
			nIndex=FindMidStr(strBuf,nIndex,"",",",strItem);
			st_Crops.CropGrow[4]=atol(strItem);
			nIndex=FindMidStr(strBuf,nIndex,"","\"",strItem);
			st_Crops.CropGrow[5]=atol(strItem);

			CString strId,strBuf2;
			strId.Format("{\"cId\":%d,",st_Crops.Id);
			if (FindMidStr(strHtml,0,strId,"}",strBuf2) != -1)
			{
				FindMidStr(strBuf2,0,"\"cLevel\":",",",strItem);
				st_Crops.Level=atoi(strItem);
				FindMidStr(strBuf2,0,"\"cropExp\":",",",strItem);
				st_Crops.CropExp=atoi(strItem);
				FindMidStr(strBuf2,0,"\"expect\":",",",strItem);
				st_Crops.Expect=atoi(strItem);
				FindMidStr(strBuf2,0,"\"growthCycle\":",",",strItem);
				st_Crops.GrowthCycle=atoi(strItem);
				FindMidStr(strBuf2,0,"\"output\":",",",strItem);
				st_Crops.Output=atoi(strItem);
				FindMidStr(strBuf2,0,"\"price\":",",",strItem);
				st_Crops.BuyPrice=atoi(strItem);
				FindMidStr(strBuf2,0,"\"sale\":","",strItem);
				st_Crops.SalePrice=atoi(strItem);
			}
			if (nOffset == -1)
				break;
			G_WriteCropInfo(st_Crops);
			nOffset=FindMidStr(strCrops,nOffset,"\"cropGrow\": \"","}",strBuf);
		}

		DeleteFile(strConfig_data_xml);		//ɾ��XML�ļ�
		WritePrivateProfileString("xmldata","config_data",strConfig_data,strQQFarm_ini);
	}

	//����ҳ��XML�汾�ͱ��ش洢��XML�汾���жԱ�,��ͬ�������չ����
	if (strConfig_addon != strConfig_addon_ini)
	{
		CC_Addtolist("[QQũ��]:��չ�����б仯,��ʼ������������",COLOR_INFO);
		//������չ����XML������
		CString strConfig_addon_xml = GetAppPath(GAP_DIR)+"addon.xml";
		if (!GetUrlToFile(strConfig_addon,strConfig_addon_xml,TRUE))
			return FALSE;
		CMarkup xml;	//��ʼ����XML
		if (!xml.Load(strConfig_addon_xml))
			return FALSE;
		xml.ResetMainPos();		//��������λ��,����������
		xml.FindElem("data");
		xml.IntoElem();
		xml.FindElem("vars");
		xml.IntoElem();
		//��ȡҰ������XML��URL
		xml.FindElem("wildXML");
		CString strConfig_wild;
		strConfig_wild = xml.GetData();
		xml.OutOfElem();
		xml.FindElem("cardsGameRewards");
		xml.IntoElem();
		while(xml.FindElem("cardsGameReward"))
		{
			ST_CARDSGAME pCardsGame;
			pCardsGame.Id = atoi(xml.GetAttrib("id"));
			strcpy(pCardsGame.Name,xml.GetAttrib("name"));
			pCardsGame.Num = atoi(xml.GetAttrib("num"));
			G_WriteCardsGame(pCardsGame);
		}
		//����Ұ����������
		CString strConfig_wild_xml = GetAppPath(GAP_DIR)+"wild.xml";
		if (!GetUrlToFile(strConfig_wild,strConfig_wild_xml,TRUE))
			return FALSE;
		if (!xml.Load(strConfig_wild_xml))
			return FALSE;
		xml.ResetMainPos();
		xml.FindElem("wild");
		xml.IntoElem();
		xml.FindElem("beast");
		xml.IntoElem();
		while(xml.FindElem("info"))
		{
			ST_BEASTINFO pBeastInfo;
			//id
			pBeastInfo.Id = atoi(xml.GetAttrib("id"));
			//����
			strcpy(pBeastInfo.Name,xml.GetAttrib("name"));
			//�Ƿ���Թ���
			pBeastInfo.IsDisplay = atoi(xml.GetAttrib("isDisplay"));
			//���ۼ۸�
			pBeastInfo.SaleMoney = atoi(xml.GetAttrib("saleMoney"));
			//����۸�
			pBeastInfo.BuyPrice = atoi(xml.GetAttrib("price"));
			//�ɷ����Ĵ���
			pBeastInfo.RaiseTimes = atoi(xml.GetAttrib("raiseTimes"));
			//������Ҫ����Ʒ
			pBeastInfo.BuyNeedExp = atoi(xml.GetAttrib("buyNeedExp"));
			//�������
			pBeastInfo.Blood = atoi(xml.GetAttrib("blood"));
			G_WriteBeastInfo(pBeastInfo);
		}
		xml.OutOfElem();
		xml.FindElem("crystal");
		xml.IntoElem();
		while(xml.FindElem("info"))
		{
			ST_CRYSTALINFO pCrystalInfo;
			//Id
			pCrystalInfo.Id = atoi(xml.GetAttrib("id"));
			//����
			strcpy(pCrystalInfo.Name,xml.GetAttrib("name"));
			//���ۼ۸�
			pCrystalInfo.SalePrice = atoi(xml.GetAttrib("sale"));
			G_WriteCrystalInfo(pCrystalInfo);
		}
		DeleteFile(strConfig_addon_xml);		//ɾ��XML�ļ�
		DeleteFile(strConfig_wild_xml);
		WritePrivateProfileString("xmldata","config_addon",strConfig_addon,strQQFarm_ini);
		WritePrivateProfileString("xmldata","config_wild",strConfig_wild,strQQFarm_ini);
	}

	m_SelfLandId = m_SelfInfo.Id;
	S_ShowMyRipeTime();
	//����ˢ���ҵ�ũ���������ݶ�ʱ��
	SetTimer(hWndMain,IDT_SHOWMYRIPE,60000,NULL);
	//����ˢ���ҵĺ����б�������Ķ�ʱ��
	SetTimer(hWndMain,IDT_SHOWFRIENDSRIPE,60000,NULL);
	//����һ��ũ������
	if (H_HandleMyFarm(NULL))
		S_ShowUserInfo((LPVOID)m_SelfLandId);

	if (szHtml.Find("\"d\":1,",0) != -1)	//���������ȡ
	{
		strPostData.Format("uIdx=%d&uinY=%s&uin=%s&uinX=",m_SelfInfo.Id,GetQQInfo(GQI_NUM),GetQQInfo(GQI_NUM));
		N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_getpackage_mini",strPostData);
		CC_Addtolist("[QQũ��]:��ȡ��ÿ�����",COLOR_INFO);
	}

	strPostData.Format("uinY=%s&uIdx=%d",GetQQInfo(GQI_NUM),pUserInfo.Id);
	szHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_login_home",strPostData);
	Sleep(1000);
	int Num = 0;
	FindMidStr(szHtml,0,"\"number\":",",",strBuf);
	//���ƵĴ���
	Num = atoi(strBuf);
	//�������Ϊ��
	if (Num == 0)
	{
		szHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_login_click",strPostData);
		Sleep(1000);
		//������ջ�
		int nBonus = 0;
		//����ǩ��������
		int nDays = 0;
		//���ƵĴ���
		int nNumber = 0;
		FindMidStr(szHtml,0,"\"bonus\":",",",strBuf);
		nBonus = atoi(strBuf);
		FindMidStr(szHtml,0,"\"days\":",",",strBuf);
		nDays = atoi(strBuf);
		FindMidStr(szHtml,0,"\"number\":",",",strBuf);
		nNumber = atoi(strBuf);
		//����ж�����ջ�
		if (nBonus == TRUE)
		{
			strPostData.Format("uinY=%s&days=%d&flag=1&uIdx=%d",GetQQInfo(GQI_NUM),nDays,pUserInfo.Id);
			szHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_pasture_signin",strPostData);
			Sleep(1000);
			int nCode = 0;
			FindMidStr(szHtml,0,"\"code\":",",",strBuf);
			nCode = atoi(strBuf);
			if (nCode == TRUE)
			{
				int nId = 0;
				FindMidStr(szHtml,0,"\"id\":",",",strBuf);
				nId = atoi(strBuf);
				ST_CARDSGAME pCardsGame;
				G_GetCardsGame(nId,pCardsGame);
				CString strPrintf;
				strPrintf.Format("[QQũ��]:����ǩ��%d��,���%d��%s",nDays,pCardsGame.Num,pCardsGame.Name);
				Addtolist(strPrintf,COLOR_INFO);
			}
		}
		int nNum = 1;
		if (nYellowLevel > 0)
			nNum++;
		for (int i=0;i<nNum-nNumber;i++)
		{
			strPostData.Format("pid=%d&flag=2&uIdx=%d&yellow=%d&uinY=%s",i,pUserInfo.Id,nYellowLevel>0 ? 1:0,GetQQInfo(GQI_NUM));
			szHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_pasture_signin",strPostData);
			Sleep(800);
			int nCode = 0;
			FindMidStr(szHtml,0,"\"code\":",",",strBuf);
			nCode = atoi(strBuf);
			if (nCode == TRUE)
			{
				int nId = 0;
				FindMidStr(szHtml,0,"\"id\":",",",strBuf);
				nId = atoi(strBuf);
				ST_CARDSGAME pCardsGame;
				G_GetCardsGame(nId,pCardsGame);
				CString strPrintf;
				strPrintf.Format("[QQũ��]:�μ����˾Ź���,���%d��%s",pCardsGame.Num,pCardsGame.Name);
				Addtolist(strPrintf,COLOR_INFO);
			}
		}
	}

	return TRUE;
}

//�����߳�,��������������
UINT H_WorkThread(LPVOID pParam)
{
	try
	{
		while(TRUE)
		{
			if (bGetBlogData)
			{
				bGetBlogData = FALSE;
				//��BLOG��ȡKEY������
				CC_Addtolist("[QQũ��]:��ȡũ������Э��",COLOR_INFO);
				while(!N_GetBlogData(NULL)){Sleep(5000);};
				Sleep(3000);
			}
			if (bGetServerTime)
			{
				bGetServerTime = FALSE;
				//��ȡ������ʱ��
				CC_Addtolist("[QQũ��]:��ȡũ��������ʱ��",COLOR_INFO);
				while(!pGetServerTime()){Sleep(5000);};
				Sleep(3000);
			}
			if (bGetXmlData)
			{
				bGetXmlData = FALSE;
				//��ũ����ҳ���ȡ������Դ
				CC_Addtolist("[QQũ��]:���ũ���������Ƿ���Ҫ����",COLOR_INFO);
				while(!N_GetXmlData(NULL)){Sleep(5000);};
				Sleep(3000);
			}
			//��ȡ�����б�
			if (bGetFriends)	
			{
				bGetFriends = FALSE;
				CC_Addtolist("[QQũ��]:��ȡũ�������б�",COLOR_INFO);
				F_GetFriends(NULL);
				//hDlg_Main->m_List_Friends.SetRedraw(FALSE);
				hDlg_Main->m_List_Friends.DeleteAllItems();
				for (int i=0;i<v_FriendsInfo.size();i++)	//ѭ����ȡ���׺����б�����
				{
					ST_USERINFO pUserInfo;					//��INI�ļ��ж�����ȡ������
					G_GetUserInfo(v_FriendsInfo[i].Id,pUserInfo);
					//�����������
					char szNum[SHORTTEXTLEN]={0};
					itoa(i+1,szNum,10);	
					//���
					hDlg_Main->m_List_Friends.InsertItem(i,szNum);
					//�ǳ�
					hDlg_Main->m_List_Friends.SetItemText(i,1,pUserInfo.Name);
					//�ȼ�
					ZeroMemory(szNum,SHORTTEXTLEN);
					itoa(FarmExpToLevel(pUserInfo.Exp),szNum,10);
					hDlg_Main->m_List_Friends.SetItemText(i,2,szNum);
					//���
					ZeroMemory(szNum,SHORTTEXTLEN);
					itoa(pUserInfo.Money,szNum,10);
					hDlg_Main->m_List_Friends.SetItemText(i,3,szNum);
				}
				//hDlg_Main->m_List_Friends.SetRedraw(TRUE);
				Sleep(3000);
			}
			//ɨ�����ũ��
			if (bScanFriends)
			{
				bScanFriends = FALSE;
				CC_Addtolist("[QQũ��]:��ʼ�������ѵ�ũ������",COLOR_INFO);
				for (int i=0;i<v_FriendsInfo.size();i++)	//ѭ����ȡ���׺����б����� 
				{
					if (v_FriendsInfo[i].Lvl < m_Config.nIgnoreFdLvl)
						continue;
					ST_USERINFO pUserInfo;					//��INI�ļ��ж�����ȡ������
					G_GetUserInfo(v_FriendsInfo[i].Id,pUserInfo);
					CString strText;
					strText.Format("���ڷ�������[%s]��ũ��",pUserInfo.Name);
					SetDlgItemText(hWndMain,IDC_MAIN_STATETEXT,strText);	//���ô���״̬��ǩ
					strText.Format("%d/%d",i+1,v_FriendsInfo.size());		//���ý���
					SetDlgItemText(hWndMain,IDC_MAIN_SCANPOS,strText);
					BOOL bSleep = F_ScanFarmInfo((LPVOID)v_FriendsInfo[i].Id);	//��ȡ����ũ��,���Զ�������INI�ļ�,�����INI�ж����򷵻�FALSE
					if (bSleep)
						H_HandleFriendFarm((LPVOID)v_FriendsInfo[i].Id);		///����Ǵ������ȡ�������ũ������
					G_GetUserInfo(v_FriendsInfo[i].Id,pUserInfo);			//��INI�ļ��ٴζ�������
					v_FriendsInfo[i].RipeTime = H_GetFarmLandRipe(pUserInfo.CropLand); //��ȡũ����������ʱ��,û���򷵻�0
					v_FriendsInfo[i].CheckTime = dwServerTime;				//��¼һ�´����ʱ��
					G_WriteUserList(v_FriendsInfo[i]);						//д��INI�ļ�
					S_ShowFriendsRipeTimeOnPos(i);							//��ʾһ�¾���컹�ж��,û�����
					if (bSleep)												//����Ǵ������ȡ����ͣһ����
						Sleep(2000);
				}
				SetDlgItemText(hWndMain,IDC_MAIN_STATETEXT,"");				//���״̬��ǩ
				if (m_SelfLandId == m_SelfInfo.Id)
					S_ShowUserInfo((LPVOID)m_SelfLandId);
				Sleep(3000);
			}
			if (m_SelfInfo.RipeTime > 0 && 
				m_SelfInfo.RipeTime <= dwServerTime &&
				m_Config.bSelfSc)
			{
				m_SelfInfo.RipeTime = 0;
				CC_Addtolist("[QQũ��]:��ʼˢ���ҵ�ũ��",COLOR_INFO);
				F_GetFarmInfo((LPVOID)m_SelfInfo.Id);
				Sleep(1000);
				H_HandleMyFarm(NULL);
				ST_USERINFO pUserInfo;	//��INI�ļ��ж�����ȡ������
				G_GetUserInfo(m_SelfInfo.Id,pUserInfo);
				m_SelfInfo.Lvl = FarmExpToLevel(pUserInfo.Exp);
				m_SelfInfo.RipeTime = H_GetFarmLandRipe(pUserInfo.CropLand);
				m_SelfInfo.CheckTime = dwServerTime;				//��¼һ�´����ʱ��
				if (m_SelfLandId == m_SelfInfo.Id)
					S_ShowUserInfo((LPVOID)m_SelfLandId);
				Sleep(3000);
			}
			for (int i=0;i<v_FriendsInfo.size();i++)
			{
				if (v_FriendsInfo[i].RipeTime > 0 && 
					v_FriendsInfo[i].RipeTime <= dwServerTime && 
					v_FriendsInfo[i].Id != m_SelfInfo.Id &&
					v_FriendsInfo[i].Lvl >= m_Config.nIgnoreFdLvl &&
					m_Config.bFdSc)
				{
					v_FriendsInfo[i].RipeTime = 0;
					if (m_Config.bFdSc || m_Config.bFdWcs)
					{
						F_GetFarmInfo((LPVOID)v_FriendsInfo[i].Id);
						Sleep(1000);
						H_HandleFriendFarm((LPVOID)v_FriendsInfo[i].Id);
						ST_USERINFO pUserInfo;	//��INI�ļ��ж�����ȡ������
						G_GetUserInfo(v_FriendsInfo[i].Id,pUserInfo);
						v_FriendsInfo[i].RipeTime = H_GetFarmLandRipe(pUserInfo.CropLand);
						v_FriendsInfo[i].CheckTime = dwServerTime;				//��¼һ�´����ʱ��
						G_WriteUserList(v_FriendsInfo[i]);						//д��INI�ļ�
						S_ShowFriendsRipeTimeOnPos(i);							//��ʾһ�¾���컹�ж��,û�����
						Sleep(3000);
					}
				}
			}
			//���˺���״̬
			if (bGetStatusFilter)
			{
				bGetStatusFilter = FALSE;
				if (m_Config.bFdWcs)
				{
					CC_Addtolist("[QQũ��]:�����Ҫ��æ����,ɱ��,��ˮ�ĺ���",COLOR_INFO);
					F_GetStatusFilter(NULL);
				}
			}
			//�����ͣ����
			while(!bWorking)
			{
				Sleep(6000);
			}
			Sleep(6000);
		}
	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//ũ�����ܺ���															//
//////////////////////////////////////////////////////////////////////////
BOOL C_RipeTimeCmp(DWORD dwMin,DWORD dwMax)
{
	return dwMin > dwMax;
}
DWORD H_GetFarmLandRipe(ST_CROPLANDINFO pCropLand[24])
{
	DWORD dwResultTime = 0;
	std::vector <DWORD> v_ripe;
	for (int i=0;i<24;i++)
	{
		if (pCropLand[i].a > 0 && pCropLand[i].n == TRUE && pCropLand[i].b < 6)	//������ǿյ�,����û��͵��,�����
		{
			ST_CROPSGROW pCropGrow;
			G_GetCropInfo(pCropLand[i].a,pCropGrow);		//��ȡ���ص�ֲ������
			DWORD dwCropTime = 0;
			dwCropTime = pCropLand[i].q + pCropGrow.CropGrow[4];
			if (dwCropTime > (dwServerTime - 14400))
				v_ripe.push_back(pCropLand[i].q + pCropGrow.CropGrow[4]);
		}
	}
	if (v_ripe.size() > 0)
	{
		std::sort(v_ripe.begin(),v_ripe.end(),C_RipeTimeCmp);
		dwResultTime = v_ripe[0];
		for (i=0;i<v_ripe.size();i++)
		{
			if (v_ripe[i] < dwResultTime && (dwResultTime - v_ripe[i]) > 60)
			{
				dwResultTime = v_ripe[i];
			}
			else if (v_ripe[i] > dwResultTime && (v_ripe[i] - dwResultTime) < 60)
			{
				dwResultTime = v_ripe[i];
			}
		}
		v_ripe.clear();
	}
	return dwResultTime;
}
UINT H_GetFarmInfo(CString &strHtml,ST_USERINFO &pUserInfo)
{
	CString strBuf;	//��ʱ�ı�
		CString strQQ;

		//����UID
		if (FindMidStr(strHtml,0,"\"uId\":",",",strBuf) != -1)
		pUserInfo.Id=atoi(strBuf);
		//����QQ
		if(FindMidStr(strHtml,0,"\"uinLogin\":",",",strQQ) != -1)
		pUserInfo.QQ=atoi(strQQ);
		//���ҽ��
		if(FindMidStr(strHtml,0,"\"money\":",",",strBuf) != -1)
		pUserInfo.Money=atol(strBuf);
		//���Ҿ���
		if(FindMidStr(strHtml,0,"\"exp\":",",",strBuf) != -1)
		pUserInfo.Exp=atol(strBuf);
		//�Ƿ�������
		if(FindMidStr(strHtml,0,"\"pf\":",",",strBuf) != -1)
		pUserInfo.pf=atoi(strBuf);
		//��Ʒֵ
		if(FindMidStr(strHtml,0,"\"moralexp\":",",",strBuf) != -1)
		pUserInfo.MoralExp=atoi(strBuf);
		//���һ���ȼ�
		/*if (if(FindMidStr(strHtml,0,"\"yellowlevel\":",",",strBuf) == 1)
			return FALSE;
		pUserInfo.YellowLevel=atoi(strBuf) != -1)
		//���һ���״̬
		if (if(FindMidStr(strHtml,0,"\"yellowstatus\":",",",strBuf) == 1)
			return FALSE;
		pUserInfo.YellowStatus=atoi(strBuf) != -1)*/
		//��ȫģʽ�Ƿ���
		if(FindMidStr(strHtml,0,"\"set\":",",",strBuf) != -1)
		pUserInfo.HealthMode=atoi(strBuf);
		//��ȫģʽ����ʱ��
		if(FindMidStr(strHtml,0,"\"beginTime\":",",",strBuf) != -1)
		pUserInfo.BeginTime=atol(strBuf);
		//��ȫģʽ����ʱ��
		if(FindMidStr(strHtml,0,"\"endTime\":","}",strBuf) != -1)
		pUserInfo.EndTime=atol(strBuf);
		//����״̬,-1û�й�,0û�й���,1�й���
		if(FindMidStr(strHtml,0,"\"isHungry\":","}",strBuf)==-1)
			pUserInfo.Dog = -1;
		else
			pUserInfo.Dog=atoi(strBuf)==0 ? 1:0;
	
		//��ȡ���ֲ������
		int nOffset = 0,nCount = 0;
		nOffset=strHtml.Find("[");
		if(nOffset!=-1)
		{
			nOffset=FindMidStr(strHtml,nOffset,"\"a\":",",",strBuf);
			while(nOffset!=-1)
			{
				pUserInfo.CropLand[nCount].a=atoi(strBuf);
				nOffset=FindMidStr(strHtml,nOffset,"\"b\":",",",strBuf);
				if (nOffset != -1)
				pUserInfo.CropLand[nCount].b=atoi(strBuf);
				nOffset=FindMidStr(strHtml,nOffset,"\"f\":",",",strBuf);
				if (nOffset != -1)
				pUserInfo.CropLand[nCount].f=atoi(strBuf);
				nOffset=FindMidStr(strHtml,nOffset,"\"g\":",",",strBuf);
				if (nOffset != -1)
				pUserInfo.CropLand[nCount].g=atoi(strBuf);
				nOffset=FindMidStr(strHtml,nOffset,"\"h\":",",",strBuf);
				if (nOffset != -1)
				pUserInfo.CropLand[nCount].h=atoi(strBuf);
				nOffset=FindMidStr(strHtml,nOffset,"\"i\":",",",strBuf);
				if (nOffset != -1)
				pUserInfo.CropLand[nCount].i=atoi(strBuf);
				nOffset=FindMidStr(strHtml,nOffset,"\"j\":",",",strBuf);
				if (nOffset != -1)
				pUserInfo.CropLand[nCount].j=atoi(strBuf);
				nOffset=FindMidStr(strHtml,nOffset,"\"k\":",",",strBuf);
				if (nOffset != -1)
				pUserInfo.CropLand[nCount].k=atoi(strBuf);
				nOffset=FindMidStr(strHtml,nOffset,"\"l\":",",",strBuf);
				if (nOffset != -1)
				pUserInfo.CropLand[nCount].l=atoi(strBuf);
				nOffset=FindMidStr(strHtml,nOffset,"\"m\":",",",strBuf);
				if (nOffset != -1)
				pUserInfo.CropLand[nCount].m=atoi(strBuf);
				nOffset=FindMidStr(strHtml,nOffset,"\"q\":",",",strBuf);
				if (nOffset != -1)
				pUserInfo.CropLand[nCount].q=atol(strBuf);
				pUserInfo.CropLand[nCount].r=dwServerTime;
				nOffset=FindMidStr(strHtml,nOffset,"\"bitmap\":",",",strBuf);
				pUserInfo.CropLand[nCount].IsRed=atoi(strBuf);
				if (strQQ != GetQQInfo(GQI_NUM))
				{
					nOffset=FindMidStr(strHtml,nOffset,"\"n\":{","}",strBuf);
					CString strId;
					strId.Format("%d",m_SelfInfo.Id);
					pUserInfo.CropLand[nCount].n = strBuf.Find(strId,0) == -1 ? 1:0;
					if (pUserInfo.CropLand[nCount].b == 6 && pUserInfo.CropLand[nCount].n == 1)
					{
						if (pUserInfo.CropLand[nCount].m <= pUserInfo.CropLand[nCount].l)
							pUserInfo.CropLand[nCount].n = 0;
					}
				}
				else
				{
					pUserInfo.CropLand[nCount].n = 1;
				}
				
				if (nOffset == -1)
					break;
				nCount++;
				//������һ��ѭ��
				nOffset=FindMidStr(strHtml,nOffset,"\"a\":",",",strBuf);
			}
		}
	
		for (int i=0;i<3;i++)
		{
			pUserInfo.BeastLand[i].fId = 0;
			pUserInfo.BeastLand[i].Type = 0;
		}
		nOffset = 0;
		CString strBeasts,strBeast;
		if (FindMidStr(strHtml,0,"\"beast\":{\"info\":",",\"drop\"",strBeasts) != -1)
		{
			nOffset=FindMidStr(strBeasts,nOffset,"{","],\"status\"",strBeast);
			while(nOffset != -1)
			{
				int nCount = -1;
				for (i=0;i<3;i++)
				{
					if (pUserInfo.BeastLand[i].fId == 0 &&
						pUserInfo.BeastLand[i].Type == 0)
					{
						nCount = i;
						break;
					}
				}
				if (nCount == -1)
					break;
				
				//����QQ
				if (FindMidStr(strBeast,0,"\"fid\":",",",strBuf) != -1)
					pUserInfo.BeastLand[nCount].fId = atoi(strBuf);
				//����ID
				if (FindMidStr(strBeast,0,"\"id\":",",",strBuf) != -1)
					pUserInfo.BeastLand[nCount].Id = atoi(strBuf);
				//��������
				if (FindMidStr(strBeast,0,"\"type\":",",",strBuf) != -1)
					pUserInfo.BeastLand[nCount].Type = atoi(strBuf);
				//����ʱ��
				if (FindMidStr(strBeast,0,"\"raisetime\":",",",strBuf) != -1)
					pUserInfo.BeastLand[nCount].RaiseTime = atol(strBuf);
				//����ʱ��
				if (FindMidStr(strBeast,0,"\"returntime\":",",",strBuf) != -1)
					pUserInfo.BeastLand[nCount].ReturnTime = atol(strBuf);
				//��������
				if (FindMidStr(strBeast,0,"\"blood\":",",",strBuf) != -1)
					pUserInfo.BeastLand[nCount].Blood = atoi(strBuf);
				//�Ƿ���Թ���
				CString strFindQQ;
				strFindQQ.Format("\"fid\":%s,",GetQQInfo(GQI_NUM));
				if (strBeast.Find(strFindQQ,0) == -1)
					pUserInfo.BeastLand[nCount].bAttack = TRUE;
	
				if (nOffset == -1)
					break;
				//������һ��ѭ��
				nOffset=FindMidStr(strBeasts,nOffset,"{","],\"status\"",strBeast);
			}
		}
		//������Լ���ũ��,���ȡһ��Ұ����������
		if (pUserInfo.QQ == atoi(GetQQInfo(GQI_NUM)) && pUserInfo.QQ !=0)
		{
			nOffset = strHtml.Find("\"beast\"",0);
			if (FindMidStr(strHtml,nOffset,"\"return\":[","]",strBeasts) != -1)
			{
				ST_BEASTINFO pBeastInfo;
				nOffset = 0;
				nOffset=FindMidStr(strBeasts,nOffset,"{","}",strBeast);
				while(nOffset != -1)
				{
					//λ��ID
					int nId = 0;
					//����ID
					int nType = 0;
	
					//λ��ID
					if (FindMidStr(strBeast,0,"\"id\":",",",strBuf) != -1)
					{
						nId = atoi(strBuf);
						m_MyBeast[nId].Id = nId;
					}
					//����ID
					if (FindMidStr(strBeast,0,"\"type\":",",",strBuf) != -1)
					{
						nType = atoi(strBuf);
						m_MyBeast[nId].Type = nType;
					}
					//��ȡҰ����������
					if (pBeastInfo.Id != nType)
						G_GetBeastInfo(nType,pBeastInfo);
					//ʣ���������
					if (FindMidStr(strBeast,0,"\"raisenum\":",",",strBuf) != -1)
						m_MyBeast[nId].LeftRaise = pBeastInfo.RaiseTimes - atoi(strBuf);
					//Ұ������״̬
					if (FindMidStr(strBeast,0,"\"status\":",",",strBuf) != -1)
						m_MyBeast[nId].Status = atoi(strBuf);
					//����ʱ��
					if (FindMidStr(strBeast,0,"\"adopttime\":",",",strBuf) != -1)
						m_MyBeast[nId].AdoptTime = atol(strBuf);
					//����ʱ��
					if (FindMidStr(strBeast,0,"\"returntime\":",",",strBuf) != -1)
						m_MyBeast[nId].ReturnTime = atol(strBuf);
	
					if (nOffset == -1)
						break;
					//������һ��ѭ��
					nOffset=FindMidStr(strBeasts,nOffset,"{","}",strBeast);
				}
			}
		}
	return TRUE;
}
//�ջ��Լ�ũ�����������
UINT F_FarmHarvest(ST_USERINFO &pUserInfo)
{
	std::vector<int> v_Land;
	
		for (int i=0;i<24;i++)
		{	//ѭ���鿴�Ƿ��п�͵ȡ��ֲ��
			if (pUserInfo.CropLand[i].a > 0 && pUserInfo.CropLand[i].b == 6 && pUserInfo.CropLand[i].n == 1)
			{
				v_Land.push_back(i);
			}
		}
		//���û�п�͵ȡ,�򷵻ؼ�
		if (v_Land.size() == 0)
			return FALSE;
	
		CString strPostData;
		strPostData.Format("uIdx=%d&ownerId=%d&uinY=%s&fName=%s&tName=%s&place=",
			pUserInfo.Id,pUserInfo.Id,GetQQInfo(GQI_NUM),strConvert(pUserInfo.Name,936,CP_UTF8),strConvert(pUserInfo.Name,936,CP_UTF8));
		for (i=0;i<v_Land.size();i++)
		{
			CString strPlace;
			strPlace.Format("%d",v_Land[i]);
			strPostData += strPlace;
			if ((i+1)<v_Land.size())
				strPostData += "%2C";
		}
		CString strHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_plant?mod=farmlandstatus&act=harvest",strPostData);
	
		ST_CROPSGROW pCropGrow;
		CString strSub;
		int nOffset = 0;
		nOffset=FindMidStr(strHtml,nOffset,"{","}}",strSub);
		while(nOffset!=-1)
		{
			CString strBuf;
	
			int nCode = 0;			//codeΪ1��ɹ�
			CString strDirection;	//��ʾ�ı�
			int nExp = 0;			//���ӵľ���
			int nIndex = 0;			//��������
			int nHarvest = 0;		//�ջ�����
			int ncId = 0;			//ֲ��ID
			int nCropStatus = 0;	//�µĽ׶�
			int nHarvestTimes = 0;	//�µļ���
			int nHealth = 0;		//�������
	
			int nHumidity = 0;		//ʪ��
			int nPest = 0;			//����
			int nWeed = 0;			//�Ӳ�
			DWORD dwPlantTime = 0;	//����ʱ��
			//int nThief = 0;			//�Ƿ��͵ȡ
			
			FindMidStr(strSub,0,"\"code\":",",",strBuf);	//codeΪ1��ɹ�		   
				nCode = atoi(strBuf);
			//FindMidStr(strSub,0,"\"direction\":\"","\"",strDirection); //��ʾ�ı�
			strDirection = CFindMidStr(strSub,"\"direction\":\"","\",");
	
			FindMidStr(strSub,0,"\"exp\":",",",strBuf);
				nExp = atoi(strBuf);
			FindMidStr(strSub,0,"\"farmlandIndex\":",",",strBuf);
				nIndex = atoi(strBuf);
			FindMidStr(strSub,0,"\"harvest\":",",",strBuf);
				nHarvest = atoi(strBuf);
			FindMidStr(strSub,0,"\"cId\":",",",strBuf);
				ncId = atoi(strBuf);
			FindMidStr(strSub,0,"\"cropStatus\":",",",strBuf);
				nCropStatus = atoi(strBuf);
			FindMidStr(strSub,0,"\"harvestTimes\":",",",strBuf);
				nHarvestTimes = atoi(strBuf);
			FindMidStr(strSub,0,"\"health\":",",",strBuf);
				nHealth = atoi(strBuf);
			FindMidStr(strSub,0,"\"humidity\":",",",strBuf);
				nHumidity = atoi(strBuf);
			FindMidStr(strSub,0,"\"pest\":",",",strBuf);
				nPest = atoi(strBuf);
			FindMidStr(strSub,0,"\"weed\":",",",strBuf);
				nWeed = atoi(strBuf);
			FindMidStr(strSub,0,"\"plantTime\":",",",strBuf);
				dwPlantTime = atol(strBuf);
			
			if (nIndex < 24)
			{
				if (nCode == TRUE)
				{
					pUserInfo.CropLand[nIndex].a = ncId;
					pUserInfo.CropLand[nIndex].b = nCropStatus;
					pUserInfo.CropLand[nIndex].j = nHarvestTimes;
					pUserInfo.CropLand[nIndex].i = nHealth;
					pUserInfo.CropLand[nIndex].h = nHumidity;
					pUserInfo.CropLand[nIndex].g = nPest;
					pUserInfo.CropLand[nIndex].f = nWeed;
					pUserInfo.CropLand[nIndex].q = dwPlantTime;
					pUserInfo.CropLand[nIndex].n = 1;
					pUserInfo.Exp += nExp;
	
					if (ncId != pCropGrow.Id)
						G_GetCropInfo(ncId,pCropGrow);
					CString strPrintf;
					strPrintf.Format("[QQũ��]:�ջ����Լ�%d�������%d��%s,������%d�㾭��",nIndex+1,nHarvest,pCropGrow.Name,nExp);
					if (strDirection != "")
						strPrintf += "\r\n��" + N_HtmlToText(strDirection);
					Addtolist(strPrintf,COLOR_INFO);
				}
			}
	
			if (nOffset == -1)
				break;
			//������һ��ѭ��
			nOffset=FindMidStr(strHtml,nOffset,"{","}}",strSub);
		}
		G_WriteUserInfo(pUserInfo);
		if (!v_Land.empty())
			v_Land.clear();
	return TRUE;
}
//�Կ�ή��ֲ����з���
UINT F_FarmScarify(ST_USERINFO &pUserInfo)
{
	std::vector<int> v_Land;
	
	for (int i=0;i<24;i++)
	{	//ѭ���鿴�Ƿ�����Ҫ��������
		if (pUserInfo.CropLand[i].a > 0 && pUserInfo.CropLand[i].b == 7)
		{
			v_Land.push_back(i);
		}
	}
	//���û�пɷ���,�򷵻ؼ�
	if (v_Land.size() == 0)
		return FALSE;
	
	for (i=0;i<v_Land.size();i++)
	{
		CString strPostData;
		strPostData.Format("uIdx=%d&ownerId=%d&uinY=%s&fName=%s&cropStatus=7&tName=%s&place=%d",
			pUserInfo.Id,pUserInfo.Id,GetQQInfo(GQI_NUM),strConvert(pUserInfo.Name,936,CP_UTF8),strConvert(pUserInfo.Name,936,CP_UTF8),v_Land[i]);
		CString strHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_plant?mod=farmlandstatus&act=scarify",strPostData);
		Sleep(800);
		
		CString strBuf;
		
		int nCode = 0;			//codeΪ1��ɹ�
		CString strDirection;	//��ʾ�ı�
		int nExp = 0;			//���ӵľ���
		int nIndex = 0;			//��������
		
		FindMidStr(strHtml,0,"\"code\":",",",strBuf);	//codeΪ1��ɹ�		   
		nCode = atoi(strBuf);
		//FindMidStr(strSub,0,"\"direction\":\"","\"",strDirection); //��ʾ�ı�
		strDirection = CFindMidStr(strHtml,"\"direction\":\"","\",");
		
		FindMidStr(strHtml,0,"\"exp\":",",",strBuf);
		nExp = atoi(strBuf);
		FindMidStr(strHtml,0,"\"farmlandIndex\":",",",strBuf);
		nIndex = atoi(strBuf);
		
		if (nIndex < 24)
		{			
			if (nCode == TRUE)
			{
				pUserInfo.CropLand[nIndex].a = 0;
				pUserInfo.CropLand[nIndex].b = 0;
				pUserInfo.CropLand[nIndex].f = 0;
				pUserInfo.CropLand[nIndex].g = 0;
				pUserInfo.CropLand[nIndex].h = 1;
				pUserInfo.CropLand[nIndex].i = 0;
				//pUserInfo.CropLand[nIndex].IsRed = 0;
				pUserInfo.CropLand[nIndex].j = 0;
				pUserInfo.CropLand[nIndex].k = 0;
				pUserInfo.CropLand[nIndex].l = 0;
				pUserInfo.CropLand[nIndex].m = 0;
				pUserInfo.CropLand[nIndex].q = 0;
				pUserInfo.CropLand[nIndex].n = 0;
				pUserInfo.CropLand[nIndex].r = 0;
				pUserInfo.Exp += nExp;
				
				CString strPrintf;
				strPrintf.Format("[QQũ��]:���Լ�%d��������˷���,�����%d�㾭��",nIndex+1,nExp);
				if (strDirection != "")
					strPrintf += "\r\n��" + N_HtmlToText(strDirection);
				Addtolist(strPrintf,COLOR_INFO);
			}
		}
	}
	
	G_WriteUserInfo(pUserInfo);
	if (!v_Land.empty())
		v_Land.clear();
	return TRUE;
}
//��������
UINT F_FarmBuySeed(int nId, int nNum)
{
	CString strPostData;
	strPostData.Format("uIdx=%d&uinY=%s&number=%d&cId=%d",m_SelfInfo.Id,GetQQInfo(GQI_NUM),nNum,nId);
	CString strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_buyseed?mod=repertory&act=buySeed",strPostData);
	CString strBuf;
	
	int nCode = 0;			//codeΪ1��ɹ�
	CString strDirection;	//��ʾ�ı�
	int nMoney = 0;			//���ٵĽ��
	CString strName;		//ֲ������
	
	FindMidStr(strHtml,0,"\"code\":",",",strBuf);	//codeΪ1��ɹ�		   
	nCode = atoi(strBuf);
	//FindMidStr(strSub,0,"\"direction\":\"","\"",strDirection); //��ʾ�ı�
	strDirection = CFindMidStr(strHtml,"\"direction\":\"","\",");
	
	FindMidStr(strHtml,0,"\"money\":",",",strBuf);
	nMoney = atoi(strBuf);
	FindMidStr(strHtml,0,"\"cName\":",",",strName);
	
	if (nCode == TRUE)
	{
		ST_USERINFO pUserInfo;	
		G_GetUserInfo(m_SelfInfo.Id,pUserInfo);
		pUserInfo.Money += nMoney;	//����Ҹ������Լ�������
		G_WriteUserInfo(pUserInfo);
		
		CString strPrintf;
		strPrintf.Format("[QQũ��]:������%d��%s������,������%d�����",nNum,strName,nMoney);
		if (strDirection != "")
			strPrintf += "\r\n��" + N_HtmlToText(strDirection);
		Addtolist(strPrintf,COLOR_INFO);
		return TRUE;
	}
	return FALSE;
}
BOOL C_CropsLvlCmp(ST_CROPSGROW pCropGrow1,ST_CROPSGROW pCropGrow2)
{
	return pCropGrow1.Level < pCropGrow2.Level;
}
//���ع����������ӵ�ID
UINT H_GetBuySeedId(BOOL bIsRed)
{
	//���շ��ص�����ID
	UINT nSeedId = 0;
	std::vector<ST_CROPSGROW> v_Seeds;	//�ɹ��������
	std::vector<ST_CROPSGROW> v_HaveSeeds;  //�������е�����
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
		if (pCropGrow.BuyPrice > 0 && pCropGrow.IsRed == bIsRed && pCropGrow.IsMill == FALSE)	//���������,����������������������
			v_Seeds.push_back(pCropGrow);
		if (pCropGrow.nSeedNum > 0 && (bIsRed || pCropGrow.IsRed == FALSE))
			v_HaveSeeds.push_back(pCropGrow);
		nPos += strlen(szCrop) + 1;
	}
	std::sort(v_Seeds.begin(),v_Seeds.end(),C_CropsLvlCmp);	//�������ӵȼ�����
	
	if (m_Config.nBuySeedType == 0)	//�������ߵȼ�������
	{
		if (m_SelfInfo.Lvl < v_Seeds[v_Seeds.size()-1].Level)
		{
			for (int i=0;i<v_Seeds.size();i++)
			{
				if (m_SelfInfo.Lvl == v_Seeds[i].Level)
				{
					nSeedId = v_Seeds[i].Id;
					break;
				}
				if (m_SelfInfo.Lvl < v_Seeds[i].Level)
				{
					nSeedId = v_Seeds[i-1].Id;
					break;
				}
			}
		}
		else
		{
			nSeedId = v_Seeds[v_Seeds.size()-1].Id;
		}
	}
	else
	{
		if (bIsRed)
			nSeedId = m_Config.nRedSeed;
		else
			nSeedId = m_Config.nBlackSeed;
	}
	
	if (m_Config.nPlantingType == 0 && v_HaveSeeds.size() > 0)
	{
		for (int i=0;i<v_HaveSeeds.size();i++)
		{
			nSeedId = v_HaveSeeds[i].Id;
			break;
		}
	}
	
	ST_CROPSGROW pCropGrow;
	G_GetCropInfo(nSeedId,pCropGrow);	
	if (pCropGrow.nSeedNum > 0)
	{
		pCropGrow.nSeedNum--;
		G_WriteCropInfo(pCropGrow);
	}
	else
	{
		if (F_FarmBuySeed(nSeedId,1) == FALSE)
			nSeedId = 0;
	}
	
	if (!v_Seeds.empty())
		v_Seeds.clear();
	if (!v_HaveSeeds.empty())
		v_HaveSeeds.clear();
	return nSeedId;
}
//�Կ����ؽ��в���
UINT F_FarmPlanting(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	if (m_Config.nPlantingType == 0)
		F_GetUserSeed(NULL);	//��ȡ��������
	for (int i=0;i<24;i++)
	{	//ѭ���鿴�Ƿ��пյ�����
		if (pUserInfo.CropLand[i].a == 0 && pUserInfo.CropLand[i].b == 0)
		{
			//��ȡ�������ӵ�ID
			UINT nSeedId = H_GetBuySeedId(pUserInfo.CropLand[i].IsRed);
			if (nSeedId != 0)	//���ID��Ϊ0,����в���
			{
				Sleep(500);
				CString strPostData;
				strPostData.Format("uIdx=%d&ownerId=%d&uinY=%s&fName=%s&tName=%s&place=%d&cId=%d",
					pUserInfo.Id,pUserInfo.Id,GetQQInfo(GQI_NUM),
					strConvert(pUserInfo.Name,936,CP_UTF8),strConvert(pUserInfo.Name,936,CP_UTF8),
					i,nSeedId);
				CString strHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_plant?mod=farmlandstatus&act=planting",strPostData);
				CString strBuf;
				
				int nCode = 0;			//codeΪ1��ɹ�
				CString strDirection;	//��ʾ�ı�
				int nExp = 0;			//���ӵľ���
				int nIndex = 0;			//��������
				
				FindMidStr(strHtml,0,"\"code\":",",",strBuf);	//codeΪ1��ɹ�		   
				nCode = atoi(strBuf);
				//FindMidStr(strSub,0,"\"direction\":\"","\"",strDirection); //��ʾ�ı�
				strDirection = CFindMidStr(strHtml,"\"direction\":\"","\",");
				
				FindMidStr(strHtml,0,"\"exp\":",",",strBuf);
				nExp = atoi(strBuf);
				FindMidStr(strHtml,0,"\"farmlandIndex\":",",",strBuf);
				nIndex = atoi(strBuf);
				
				if (nIndex < 24)
				{						
					if (nCode == TRUE)
					{
						pUserInfo.CropLand[nIndex].a = nSeedId;
						pUserInfo.CropLand[nIndex].b = 1;
						pUserInfo.CropLand[nIndex].f = 0;
						pUserInfo.CropLand[nIndex].g = 0;
						pUserInfo.CropLand[nIndex].h = 1;
						pUserInfo.CropLand[nIndex].q = dwServerTime;
						pUserInfo.CropLand[nIndex].n = 1;
						pUserInfo.CropLand[nIndex].r = dwServerTime;
						pUserInfo.Exp += nExp;
						
						ST_CROPSGROW pCropGrow;
						G_GetCropInfo(nSeedId,pCropGrow);
						CString strPrintf;
						strPrintf.Format("[QQũ��]:���Լ�%d�����ﲥ����%s������,�����%d�㾭��",nIndex+1,pCropGrow.Name,nExp);
						if (strDirection != "")
							strPrintf += "\r\n��" + N_HtmlToText(strDirection);
						Addtolist(strPrintf,COLOR_INFO);
					}
					nResult = TRUE;
					Sleep(1000);
				}
			}
		}
	}
	if (nResult)
		G_WriteUserInfo(pUserInfo);
	return nResult;
}
//����ũ�����Ӳ�
UINT F_FarmClearWeed(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	for (int i=0;i<24;i++)
	{
		if (pUserInfo.CropLand[i].a > 0 && pUserInfo.CropLand[i].f > 0)
		{
			CString strPostData;
			strPostData.Format("uIdx=%d&ownerId=%d&uinX=%d&uinY=%s&fName=%s&tName=%s&place=",
				m_SelfInfo.Id,pUserInfo.Id,pUserInfo.QQ,GetQQInfo(GQI_NUM),strConvert(GetQQInfo(GQI_NAME),936,CP_UTF8),strConvert(pUserInfo.Name,936,CP_UTF8));
			for (int j=0;j<pUserInfo.CropLand[i].f;j++)
			{
				CString strPlace;
				strPlace.Format("%d",i);
				strPostData += strPlace;
				if ((j+1)<pUserInfo.CropLand[i].f)
					strPostData += "%2C";
			}
			CString strHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_opt?mod=farmlandstatus&act=clearWeed",strPostData);
			CString strSub;
			int nOffset = 0;
			nOffset=FindMidStr(strHtml,nOffset,"{","}",strSub);
			while(nOffset!=-1)
			{
				CString strBuf;
				
				int nCode = 0;			//codeΪ1��ɹ�
				CString strDirection;	//��ʾ�ı�
				int nExp = 0;			//���ӵľ���
				int nMoney = 0;			//���ӵĽ��
				int nIndex = 0;			//��������
				int nWeed = 0;			//ʣ�µĲ�
				
				FindMidStr(strSub,0,"\"code\":",",",strBuf);	//codeΪ1��ɹ�		   
				nCode = atoi(strBuf);
				strDirection = CFindMidStr(strSub,"\"direction\":\"","\",");
				FindMidStr(strSub,0,"\"exp\":",",",strBuf);
				nExp = atoi(strBuf);
				FindMidStr(strSub,0,"\"money\":",",",strBuf);
				nMoney = atoi(strBuf);
				FindMidStr(strSub,0,"\"farmlandIndex\":",",",strBuf);
				nIndex = atoi(strBuf);
				FindMidStr(strSub,0,"\"weed\":",",",strBuf);
				nWeed = atoi(strBuf);
				
				if (nIndex < 24)
				{					
					if (nCode == TRUE)
					{
						pUserInfo.CropLand[nIndex].f = nWeed;
						pUserInfo.Exp += nExp;
						
						CString strPrintf;
						if (m_SelfInfo.Id == pUserInfo.Id)
							strPrintf.Format("[QQũ��]:������Լ�%d�����1���Ӳ�,�����%d�㾭��",nIndex+1,nExp);
						else
							strPrintf.Format("[QQũ��]:����˺���[%s]%d�����1���Ӳ�,�����%d�㾭��,%d�����",pUserInfo.Name,nIndex+1,nExp,nMoney);
						if (strDirection != "")
							strPrintf += "\r\n��" + N_HtmlToText(strDirection);
						Addtolist(strPrintf,COLOR_INFO);
						if (nExp == 0)	//����õ��ľ���Ϊ0,����ͣΪ���ѳ���
						{
							KillTimer(hWndMain,IDT_GETSTATUSFILTER);
							bWcsFriends = FALSE;
						}
					}
					nResult = TRUE;
				}
				
				if (nOffset == -1)
					break;
				//������һ��ѭ��
				nOffset=FindMidStr(strHtml,nOffset,"{","}",strSub);
			}
			Sleep(1000);
		}
	}
	G_WriteUserInfo(pUserInfo);
	return nResult;
}
//����ũ���ĺ���
UINT F_FarmSpraying(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	for (int i=0;i<24;i++)
	{
		if (pUserInfo.CropLand[i].a > 0 && pUserInfo.CropLand[i].g > 0)
		{
			CString strPostData;
			strPostData.Format("uIdx=%d&ownerId=%d&uinX=%d&uinY=%s&fName=%s&tName=%s&place=",
				m_SelfInfo.Id,pUserInfo.Id,pUserInfo.QQ,GetQQInfo(GQI_NUM),strConvert(GetQQInfo(GQI_NAME),936,CP_UTF8),strConvert(pUserInfo.Name,936,CP_UTF8));
			for (int j=0;j<pUserInfo.CropLand[i].g;j++)
			{
				CString strPlace;
				strPlace.Format("%d",i);
				strPostData += strPlace;
				if ((j+1)<pUserInfo.CropLand[i].g)
					strPostData += "%2C";
			}
			CString strHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_opt?mod=farmlandstatus&act=spraying",strPostData);
			CString strSub;
			int nOffset = 0;
			nOffset=FindMidStr(strHtml,nOffset,"{","}",strSub);
			while(nOffset!=-1)
			{
				CString strBuf;
				
				int nCode = 0;			//codeΪ1��ɹ�
				CString strDirection;	//��ʾ�ı�
				int nExp = 0;			//���ӵľ���
				int nMoney = 0;			//���ӵĽ��
				int nIndex = 0;			//��������
				int nPest = 0;			//ʣ�µĲ�
				
				FindMidStr(strSub,0,"\"code\":",",",strBuf);	//codeΪ1��ɹ�		   
				nCode = atoi(strBuf);
				strDirection = CFindMidStr(strSub,"\"direction\":\"","\",");
				FindMidStr(strSub,0,"\"exp\":",",",strBuf);
				nExp = atoi(strBuf);
				FindMidStr(strSub,0,"\"money\":",",",strBuf);
				nMoney = atoi(strBuf);
				FindMidStr(strSub,0,"\"farmlandIndex\":",",",strBuf);
				nIndex = atoi(strBuf);
				FindMidStr(strSub,0,"\"pest\":",",",strBuf);
				nPest = atoi(strBuf);
				
				if (nIndex < 24)
				{				
					if (nCode == TRUE)
					{
						pUserInfo.CropLand[nIndex].g = nPest;
						pUserInfo.Exp += nExp;
						
						CString strPrintf;
						if (m_SelfInfo.Id == pUserInfo.Id)
							strPrintf.Format("[QQũ��]:������Լ�%d�����1ֻ����,�����%d�㾭��",nIndex+1,nExp);
						else
							strPrintf.Format("[QQũ��]:����˺���[%s]%d�����1ֻ����,�����%d�㾭��,%d�����",pUserInfo.Name,nIndex+1,nExp,nMoney);
						if (strDirection != "")
							strPrintf += "\r\n��" + N_HtmlToText(strDirection);
						Addtolist(strPrintf,COLOR_INFO);
						if (nExp == 0)	//����õ��ľ���Ϊ0,����ͣΪ���ѳ���
						{
							KillTimer(hWndMain,IDT_GETSTATUSFILTER);
							bWcsFriends = FALSE;
						}
					}
					nResult = TRUE;
				}
				
				if (nOffset == -1)
					break;
				//������һ��ѭ��
				nOffset=FindMidStr(strHtml,nOffset,"{","}",strSub);
			}
			Sleep(1000);
		}
	}
	G_WriteUserInfo(pUserInfo);
	return nResult;
}
//Ϊ�ɺ������ؽ�ˮ
UINT F_FarmWatering(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	for (int i=0;i<24;i++)
	{
		if (pUserInfo.CropLand[i].a > 0 && pUserInfo.CropLand[i].h == 0)
		{
			CString strPostData;
			strPostData.Format("uIdx=%d&ownerId=%d&uinX=%d&uinY=%s&fName=%s&tName=%s&place=%d",
				m_SelfInfo.Id,pUserInfo.Id,pUserInfo.QQ,GetQQInfo(GQI_NUM),strConvert(GetQQInfo(GQI_NAME),936,CP_UTF8),strConvert(pUserInfo.Name,936,CP_UTF8),i);
			CString strHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_opt?mod=farmlandstatus&act=water",strPostData);
			CString strSub;
			int nOffset = 0;
			nOffset=FindMidStr(strHtml,nOffset,"{","}",strSub);
			while(nOffset!=-1)
			{
				CString strBuf;
				
				int nCode = 0;			//codeΪ1��ɹ�
				CString strDirection;	//��ʾ�ı�
				int nExp = 0;			//���ӵľ���
				int nMoney = 0;			//���ӵĽ��
				int nIndex = 0;			//��������
				
				FindMidStr(strSub,0,"\"code\":",",",strBuf);	//codeΪ1��ɹ�		   
				nCode = atoi(strBuf);
				strDirection = CFindMidStr(strSub,"\"direction\":\"","\",");
				FindMidStr(strSub,0,"\"exp\":",",",strBuf);
				nExp = atoi(strBuf);
				FindMidStr(strSub,0,"\"money\":",",",strBuf);
				nMoney = atoi(strBuf);
				FindMidStr(strSub,0,"\"farmlandIndex\":",",",strBuf);
				nIndex = atoi(strBuf);
				
				if (nIndex < 24)
				{					
					if (nCode == TRUE)
					{
						pUserInfo.CropLand[nIndex].h = 1;
						pUserInfo.Exp += nExp;
						
						CString strPrintf;
						if (m_SelfInfo.Id == pUserInfo.Id)
							strPrintf.Format("[QQũ��]:Ϊ�Լ�%d��������˽�ˮ,�����%d�㾭��",nIndex+1,nExp);
						else
							strPrintf.Format("[QQũ��]:Ϊ����[%s]%d��������˽�ˮ,�����%d�㾭��,%d�����",pUserInfo.Name,nIndex+1,nExp,nMoney);
						if (strDirection != "")
							strPrintf += "\r\n��" + N_HtmlToText(strDirection);
						Addtolist(strPrintf,COLOR_INFO);
						if (nExp == 0)	//����õ��ľ���Ϊ0,����ͣΪ���ѳ���
						{
							KillTimer(hWndMain,IDT_GETSTATUSFILTER);
							bWcsFriends = FALSE;
						}
					}
					nResult = TRUE;
				}
				
				if (nOffset == -1)
					break;
				//������һ��ѭ��
				nOffset=FindMidStr(strHtml,nOffset,"{","}",strSub);
			}
			Sleep(1000);
		}
	}
	G_WriteUserInfo(pUserInfo);
	return nResult;
}
UINT F_FarmReclaim(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	CString strPostData;
	strPostData.Format("uinY=%s&uIdx=%d",GetQQInfo(GQI_NUM),m_SelfInfo.Id);
	CString strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_reclaimpay?mod=user&act=reclaimPay",strPostData);
	
	CString strBuf;
	int nCode = 0;			//codeΪ1��ɹ�
	int nLevel = 0;			//���ӵľ���
	int nMoney = 0;			//���ӵĽ��
	
	FindMidStr(strHtml,0,"\"code\":",",",strBuf);	//codeΪ1��ɹ�		   
	nCode = atoi(strBuf);
	FindMidStr(strHtml,0,"\"level\":",",",strBuf);
	nLevel = atoi(strBuf);
	FindMidStr(strHtml,0,"\"money\":","}",strBuf);
	nMoney = atoi(strBuf);
	if (nCode == TRUE)
	{
		if (nLevel > 0 && nMoney > 0 && m_SelfInfo.Lvl >= nLevel && pUserInfo.Money > nMoney)
		{
			Sleep(2000);
			strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_reclaim?mod=user&act=reclaim",strPostData);
			nCode = 0;			//codeΪ1��ɹ�
			CString strDirection;	//��ʾ�ı�
			nMoney = 0;			//���ӵĽ��
			
			FindMidStr(strHtml,0,"\"code\":",",",strBuf);	//codeΪ1��ɹ�		   
			nCode = atoi(strBuf);
			strDirection = CFindMidStr(strHtml,"\"direction\":\"","\",");
			FindMidStr(strHtml,0,"\"money\":","}",strBuf);
			nMoney = atoi(strBuf);
			if (nCode == TRUE)
			{
				pUserInfo.Money += nMoney;
				for (int i=0;i<24;i++)
				{
					if (pUserInfo.CropLand[i].a == -1)
					{
						pUserInfo.CropLand[i].a = 0;
						break;
					}
				}
				CString strPrintf;
				strPrintf.Format("[QQũ��]:������һ������,������%d�����",nMoney);
				if (strDirection != "")
					strPrintf += "\r\n��" + N_HtmlToText(strDirection);
				Addtolist(strPrintf,COLOR_INFO);
				G_WriteUserInfo(pUserInfo);
				nResult = TRUE;
			}
		}
	}
	return nResult;
}
UINT F_FarmUpgrade(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	CString strPostData;
	strPostData.Format("uinY=%s&uIdx=%d",GetQQInfo(GQI_NUM),m_SelfInfo.Id);
	CString strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_upgrade",strPostData);
	
	CString strBuf;
	int nCode = 0;			//codeΪ1��ɹ�
	int nLevel = 0;			//���ӵľ���
	int nMoney = 0;			//���ӵĽ��
	
	FindMidStr(strHtml,0,"\"code\":",",",strBuf);	//codeΪ1��ɹ�		   
	nCode = atoi(strBuf);
	FindMidStr(strHtml,0,"\"level\":",",",strBuf);
	nLevel = atoi(strBuf);
	FindMidStr(strHtml,0,"\"money\":",",",strBuf);
	nMoney = atoi(strBuf);
	if (nCode == TRUE)
	{
		if (nLevel > 0 && nMoney > 0 && m_SelfInfo.Lvl >= nLevel && pUserInfo.Money > nMoney)
		{
			Sleep(2000);
			strPostData += "&confirm=1";
			strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_upgrade",strPostData);
			
			nCode = 0;			//codeΪ1��ɹ�
			CString strDirection;	//��ʾ�ı�
			nMoney = 0;			//���ӵĽ��
			
			FindMidStr(strHtml,0,"\"code\":",",",strBuf);	//codeΪ1��ɹ�		   
			nCode = atoi(strBuf);
			strDirection = CFindMidStr(strHtml,"\"direction\":\"","\",");
			FindMidStr(strHtml,0,"\"money\":",",",strBuf);
			nMoney = atoi(strBuf);
			
			pUserInfo.Money += nMoney;
			for (int i=0;i<24;i++)
			{
				if (pUserInfo.CropLand[i].IsRed == FALSE)
				{
					pUserInfo.CropLand[i].IsRed = TRUE;
					break;
				}
			}
			CString strPrintf;
			strPrintf.Format("[QQũ��]:������һ������Ϊ������,������%d�����",nMoney);
			if (strDirection != "")
				strPrintf += "\r\n��" + N_HtmlToText(strDirection);
			Addtolist(strPrintf,COLOR_INFO);
			G_WriteUserInfo(pUserInfo);
			nResult = TRUE;
		}
	}
	return nResult;
}
//����Ұ������
UINT F_FarmAttackBeast(ST_USERINFO &pUserInfo)
{
	UINT Result = FALSE;
	for (int i=0;i<3;i++)
		{
			if (pUserInfo.BeastLand[i].fId > 0 &&
				pUserInfo.BeastLand[i].Type > 0 &&
				pUserInfo.BeastLand[i].Blood > 0 &&
				pUserInfo.BeastLand[i].bAttack)
			{
				pUserInfo.BeastLand[i].bAttack = FALSE;
				CString strPostData;
				strPostData.Format("weapon=3&mode=0&uinY=%s&uinX=%d&isqz=1&ownerId=%d&flag=1&attackType=Fight&slotid=%d&isfarm=1&owner=%d&uIdx=%d&index=%d",GetQQInfo(GQI_NUM),pUserInfo.QQ,pUserInfo.Id,pUserInfo.BeastLand[i].Id,pUserInfo.BeastLand[i].fId,m_SelfInfo.Id,i);
				CString strHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_attack_beast",strPostData);
				Sleep(1000);
				CString strBuf;
				//���ӵ���Ʒֵ
				int nAddMoral = 0;
				FindMidStr(strHtml,0,"\"addmoral\":",",",strBuf);
				nAddMoral = atoi(strBuf);
				if (nAddMoral > 0)
				{
					ST_USERINFO mUserInfo;
					G_GetUserInfo(m_SelfInfo.Id,mUserInfo);
					mUserInfo.MoralExp += nAddMoral;
					G_WriteUserInfo(mUserInfo);

					//��������
					FindMidStr(strHtml,0,"\"leftblood\":",",",strBuf);
					pUserInfo.BeastLand[i].Blood = atoi(strBuf);
				}
				//���ؽ��
				int nResult = 0;
				FindMidStr(strHtml,0,"\"result\":",",",strBuf);
				nResult = atoi(strBuf);
				
				//����ʱ��
				FindMidStr(strHtml,0,"\"t\":","}",strBuf);
				pUserInfo.BeastLand[i].ReturnTime = atol(strBuf);
				
				ST_BEASTINFO pBeastInfo;
				G_GetBeastInfo(pUserInfo.BeastLand[i].Type,pBeastInfo);
	
				CString strUser;
				if (pUserInfo.Id == m_SelfInfo.Id)
					strUser = "�Լ�";
				else
					strUser.Format("����[%s]",pUserInfo.Name);
	
				//�������1,�����
				if (nResult == 1)
				{
					CString strPrintf;
					strPrintf.Format("[QQũ��]:������%s�����%s(%d/%d),����%d����Ʒ",strUser,pBeastInfo.Name,pUserInfo.BeastLand[i].Blood,pBeastInfo.Blood,nAddMoral);
					Addtolist(strPrintf,COLOR_INFO);
				}else if (nResult == 2)
				{
					CString strPrintf;
					strPrintf.Format("[QQũ��]:˦����%s�����%s(%d/%d),����%d����Ʒ",strUser,pBeastInfo.Name,pUserInfo.BeastLand[i].Blood,pBeastInfo.Blood,nAddMoral);
					Addtolist(strPrintf,COLOR_INFO);
				}else if (nResult == 3)
				{
					CString strPrintf;
					strPrintf.Format("[QQũ��]:����%s�����%s(%d/%d)ʧ��",strUser,pBeastInfo.Name,pUserInfo.BeastLand[i].Blood,pBeastInfo.Blood);
					Addtolist(strPrintf,COLOR_INFO);
				}
	
				int nOffset = 0,nCount = 0;
				CString strCrystals,strCrystal;
				if (FindMidStr(strHtml,0,"\"drop\":[","],",strCrystals) != -1)
				{
					nOffset = FindMidStr(strCrystals,nOffset,"{","}",strCrystal);
					while(nOffset != -1)
					{
						int nId;
						int nNum;
						DWORD dwTime;
						//id
						FindMidStr(strCrystal,0,"\"id\":",",",strBuf);
						nId = atoi(strBuf);
						//����
						FindMidStr(strCrystal,0,"\"num\":",",",strBuf);
						nNum = atoi(strBuf);
						//ʱ��
						FindMidStr(strCrystal,0,"\"time\":",",",strBuf);
						dwTime = atol(strBuf);
						
						strPostData.Format("farm=1&ownerId=%d&time=%d&index=%d&opt=1&uinY=%s&id=%d&uIdx=%d&uinX=%d",pUserInfo.Id,dwTime,nCount,GetQQInfo(GQI_NUM),nId,m_SelfInfo.Id,pUserInfo.QQ);
						strHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_pickup_crystal",strPostData);
						Sleep(800);
						
						int nCode = 0;
						FindMidStr(strHtml,0,"\"code\":",",",strBuf);
						nCode = atoi(strBuf);
						if (nCode == TRUE)
						{
							ST_CRYSTALINFO pCrystalInfo;
							G_GetCrystalInfo(nId,pCrystalInfo);
							CString strPrintf;
							strPrintf.Format("[QQũ��]:����%d��%s",nNum,pCrystalInfo.Name);
							Addtolist(strPrintf,COLOR_INFO);
						}
	
						if (nOffset == -1)
							break;
						nCount++;
						nOffset = FindMidStr(strCrystals,nOffset,"{","}",strCrystal);
					}
				}
				Result = TRUE;
			}
		}
		if (Result)
			G_WriteUserInfo(pUserInfo);
	return Result;
}
//����ҵ�Ұ����������
UINT F_GetMyBeastInfo(LPVOID pParam)
{
	CString strPostData;
	strPostData.Format("uIdx=%d&uinY=%s&ownerId=%d",m_SelfInfo.Id,GetQQInfo(GQI_NUM),m_SelfInfo.Id);
	CString strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_get_userbeast",strPostData);
	CString strBuf;
	//��ǰ�����Ĳ�λ����
	int nMaxSlotId = 0;
	FindMidStr(strHtml,0,"\"maxslotid\":","}",strBuf);
	nMaxSlotId = atoi(strBuf);
	//�滻һ������,����ѭ������
	strHtml.Replace("\"\",","{}");
	
	CString strBeasts,strBeast;
	if (FindMidStr(strHtml,0,"\"beasts\":[","]",strBeasts) != -1)
	{
		int nOffset = 0;
		for (int i=0;i<nMaxSlotId+1;i++)
		{
			nOffset=FindMidStr(strBeasts,nOffset,"{","}",strBeast);
			//λ��ID
			m_MyBeast[i].Id = i;
			//��������
			FindMidStr(strBeast,0,"\"curblood\":",",",strBuf);
			m_MyBeast[i].Blood = atoi(strBuf);
			//ʣ���������
			FindMidStr(strBeast,0,"\"leftraise\":",",",strBuf);
			m_MyBeast[i].LeftRaise = atoi(strBuf);
			//����״̬
			FindMidStr(strBeast,0,"\"status\":",",",strBuf);
			m_MyBeast[i].Status = atoi(strBuf);
			//����ID
			FindMidStr(strBeast,0,"\"type\":","",strBuf);
			m_MyBeast[i].Type = atoi(strBuf);
		}
	}
	return TRUE;
}
//Ұ�����ﷵ��
UINT F_FarmReturnBeast(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	ST_BEASTINFO pBeastInfo;
	//Ұ�����ﷵ��
	for (int i=0;i<3;i++)
	{
		if (m_MyBeast[i].Type > 0 && m_MyBeast[i].Status == 3)
		{
			CString strPostData;
			strPostData.Format("uIdx=%d&uinY=%s&slotid=%d&type=%d",m_SelfInfo.Id,GetQQInfo(GQI_NUM),i,m_MyBeast[i].Type);
			CString strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_reward_beast",strPostData);
			Sleep(800);
			CString strBuf;
			//�������
			int nECode = -1;
			//��õĽ��
			int nMoney = 0;
			//��õ���Ʒ
			int nMoralExp = 0;
			
			if (FindMidStr(strHtml,0,"\"ecode\":",",",strBuf) != -1)
				nECode = atoi(strBuf);
			if (FindMidStr(strHtml,0,"\"money\":",",",strBuf) != -1)
				nMoney = atoi(strBuf);
			if (FindMidStr(strHtml,0,"\"moralexp\":","}",strBuf) != -1)
				nMoralExp = atoi(strBuf);
			
			if (nECode == FALSE)
			{
				nResult = TRUE;
				pUserInfo.Money += nMoney;
				pUserInfo.MoralExp += nMoralExp;
				
				if (pBeastInfo.Id != m_MyBeast[i].Type)
					G_GetBeastInfo(m_MyBeast[i].Type,pBeastInfo);
				
				CString strPrintf;
				strPrintf.Format("[QQũ��]:%d�Ų�λ������%s������,�����%d�����,%d����Ʒֵ",i+1,pBeastInfo.Name,nMoney,nMoralExp);
				Addtolist(strPrintf,COLOR_INFO);
				
				int nOffset = 0;
				CString strCrystals,strCrystal;
				if (FindMidStr(strHtml,0,"\"drop\":[","],",strCrystals) != -1)
				{
					nOffset = FindMidStr(strCrystals,nOffset,"{","}",strCrystal);
					while(nOffset != -1)
					{
						int nId;
						int nNum;
						//id
						FindMidStr(strCrystal,0,"\"id\":",",",strBuf);
						nId = atoi(strBuf);
						//����
						FindMidStr(strCrystal,0,"\"num\":",",",strBuf);
						nNum = atoi(strBuf);
						
						ST_CRYSTALINFO pCrystalInfo;
						G_GetCrystalInfo(nId,pCrystalInfo);
						strPrintf.Format("[QQũ��]:�����%d��%s",nNum,pCrystalInfo.Name);
						Addtolist(strPrintf,COLOR_INFO);
						
						if (nOffset == -1)
							break;
						nOffset = FindMidStr(strCrystals,nOffset,"{","}",strCrystal);
					}
				}
			}
		}
	}
	return nResult;
}
//����Ұ�������λ
UINT F_FarmOpenSlot(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	//������λ
	for (int i=0;i<3;i++)
	{
		if (m_MyBeast[i].Id == -1)
		{
			//��һ����λ���Զ�������,���Ϊ-1,���ǻ�ȡʧ��
			if (i == 0)
				break;
			
			int nNeedMoney = 0;
			if (i == 1)
				nNeedMoney = 300000;
			if (i == 2)
				nNeedMoney = 600000;
			
			//�������㹻,������λ
			if (pUserInfo.Money > nNeedMoney + 50000)
			{
				CString strPostData;
				strPostData.Format("uIdx=%d&uinY=%s&slotid=%d",m_SelfInfo.Id,GetQQInfo(GQI_NUM),i);
				CString strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_open_slot",strPostData);
				Sleep(800);
				CString strBuf;
				
				//�������
				int nECode = -1;
				//���ٵĽ��
				int nMoney = 0;
				
				FindMidStr(strHtml,0,"\"ecode\":",",",strBuf);
				nECode = atoi(strBuf);
				FindMidStr(strHtml,0,"\"money\":","}",strBuf);
				nMoney = atoi(strBuf);
				
				if (nECode == FALSE)
				{
					nResult = TRUE;
					pUserInfo.Money += nMoney;
					
					m_MyBeast[i].Id = i;
					
					CString strPrintf;
					strPrintf.Format("[QQũ��]:������%d��Ұ�������λ,������%d�����",i+1,nMoney);
					Addtolist(strPrintf,COLOR_INFO);
				}
			}
		}
	}
	return nResult;
}
//����Ұ������
UINT F_FarmDonateBeast(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	ST_BEASTINFO pBeastInfo;
	//����Ұ������
	for (int i=0;i<3;i++)
	{
		if (m_MyBeast[i].Status == 5 && m_MyBeast[i].Type > 0)
		{
			CString strPostData;
			strPostData.Format("uIdx=%d&uinY=%s&slotid=%d&type=%d",m_SelfInfo.Id,GetQQInfo(GQI_NUM),i,m_MyBeast[i].Type);
			CString strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_donate_beast",strPostData);
			Sleep(800);
			CString strBuf;
			
			//�������
			int nECode = 0;
			//���ٵĽ��
			int nMoney = 0;
			
			FindMidStr(strHtml,0,"\"ecode\":",",",strBuf);
			nECode = atoi(strBuf);
			FindMidStr(strHtml,0,"\"money\":","}",strBuf);
			nMoney = atoi(strBuf);
			
			if (nECode == FALSE)
			{
				nResult = TRUE;
				pUserInfo.Money += nMoney;
				
				m_MyBeast[i].Type = 0;
				
				if (pBeastInfo.Id != m_MyBeast[i].Type)
					G_GetBeastInfo(m_MyBeast[i].Type,pBeastInfo);
				
				CString strPrintf;
				strPrintf.Format("[QQũ��]:������%d�Ų�λ��%s,�����%d�����",i+1,pBeastInfo.Name,nMoney);
				Addtolist(strPrintf,COLOR_INFO);
			}
		}
	}
	return nResult;
}
BOOL C_BeastsLvlCmp(ST_BEASTINFO pBeastInfo1,ST_BEASTINFO pBeastInfo2)
{
	return pBeastInfo1.BuyNeedExp < pBeastInfo2.BuyNeedExp;
}
//����Ұ������
UINT F_FarmAdoptBeast(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	int nAdoptNum = 0;
		for (int i=0;i<3;i++)
		{
			if (m_MyBeast[i].Id != -1 && m_MyBeast[i].Type == 0)
				nAdoptNum++;
		}
		if (nAdoptNum == 0)
			return nResult;
	
		std::vector<ST_BEASTINFO> v_Beasts;	//�������Ķ���
		char szBeasts[SHORTUSERLEN]={0};
		char szBeast[SHORTLEN]={0};
		//ȡ����Section��,�����ַ�����0��β,�����ַ�����00��β
		GetPrivateProfileSectionNames(szBeasts,SHORTUSERLEN,strBeastIni);
		int nPos = 0;
		while(szBeasts[nPos] != 0)
		{
			strcpy(szBeast,szBeasts+nPos);	//�õ�������Section
			ST_BEASTINFO pBeastInfo;
			G_GetBeastInfo(atoi(szBeast),pBeastInfo);	//��ȡ���Section������
			if (pBeastInfo.IsDisplay == TRUE && pBeastInfo.BuyNeedExp <= pUserInfo.MoralExp && pBeastInfo.BuyPrice < pUserInfo.Money)
				v_Beasts.push_back(pBeastInfo);
			nPos += strlen(szBeast) + 1;
		}
		std::sort(v_Beasts.begin(),v_Beasts.end(),C_BeastsLvlCmp);	//����
		if (v_Beasts.size() == 0)
			return nResult;
	
		for (i=0;i<3;i++)
		{
			if (m_MyBeast[i].Id != -1 && m_MyBeast[i].Type == 0)
			{
				//�Ȼ�ȡ��Ʒֵ
				CString strPostData;
				strPostData.Format("uIdx=%d&uinY=%s",m_SelfInfo.Id,GetQQInfo(GQI_NUM));
				CString strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_get_moralexp",strPostData);
				Sleep(800);
				CString strBuf;
				//�������
				int nECode = -1;
				//��Ʒֵ
				int nMoralExp = 0;
				FindMidStr(strHtml,0,"\"ecode\":",",",strBuf);
					nECode = atoi(strBuf);
				FindMidStr(strHtml,0,"\"moralexp\":",",",strBuf);
					nMoralExp = atoi(strBuf);
	
				if (nECode == FALSE)
				{
					pUserInfo.MoralExp = nMoralExp;
				}
	
				//��ʼ��ȡҰ��������
				strPostData.Format("uIdx=%d&uinY=%s&slotid=%d&type=%d",m_SelfInfo.Id,GetQQInfo(GQI_NUM),i,v_Beasts[v_Beasts.size()-1].Id);
				strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_adopt_beast",strPostData);
				//�������
				nECode = -1;
				//���ѵĽ��
				int nMoney = 0;
				FindMidStr(strHtml,0,"\"ecode\":",",",strBuf);
					nECode = atoi(strBuf);
				FindMidStr(strHtml,0,"\"money\":","}",strBuf);
					nMoney = atoi(strBuf);
				if (nECode == FALSE)
				{
					nResult = TRUE;
					pUserInfo.Money += nMoney;
					
					//λ��ID
					int nId = 0;
					//����ID
					int nType = 0;
					
					//λ��ID
					if (FindMidStr(strHtml,0,"\"id\":",",",strBuf) != -1)
					{
						nId = atoi(strBuf);
						m_MyBeast[nId].Id = nId;
					}
					//����ID
					if (FindMidStr(strHtml,0,"\"type\":","}",strBuf) != -1)
					{
						nType = atoi(strBuf);
						m_MyBeast[nId].Type = nType;
					}
	
					//ʣ���������
					if (FindMidStr(strHtml,0,"\"raisenum\":",",",strBuf) != -1)
						m_MyBeast[nId].LeftRaise = v_Beasts[v_Beasts.size()-1].RaiseTimes - atoi(strBuf);
					//Ұ������״̬
					if (FindMidStr(strHtml,0,"\"status\":",",",strBuf) != -1)
						m_MyBeast[nId].Status = atoi(strBuf);
					//����ʱ��
					if (FindMidStr(strHtml,0,"\"adopttime\":",",",strBuf) != -1)
						m_MyBeast[nId].AdoptTime = atol(strBuf);
					//����ʱ��
					if (FindMidStr(strHtml,0,"\"returntime\":",",",strBuf) != -1)
						m_MyBeast[nId].ReturnTime = atol(strBuf);
	
					CString strPrintf;
					strPrintf.Format("[QQũ��]:%d�Ų�λ������%s,������%d�����",i+1,v_Beasts[v_Beasts.size()-1].Name,nMoney);
					Addtolist(strPrintf,COLOR_INFO);
				}
			}
		}
		
		if (!v_Beasts.empty())
			v_Beasts.clear();
	return nResult;
}
//�����ҵ�Ұ������
UINT H_HandleMyBeast(ST_USERINFO &pUserInfo)
{
	//�Ƿ���Ұ�����ﷵ��
	UINT nReturnBeast = FALSE;
	//�Ƿ����˲�λ
	UINT nOpenSlot = FALSE;
	//�Ƿ������Ұ������
	UINT nDonateBeast = FALSE;
	//�Ƿ�������Ұ������
	UINT nAdoptBeast = FALSE;


	//Ұ�����ﷵ��
	nReturnBeast = F_FarmReturnBeast(pUserInfo);
	if (nReturnBeast)
		Sleep(2000);
	//����ҵ�Ұ����������
	F_GetMyBeastInfo(NULL);
	Sleep(1000);

	//������λ
	nOpenSlot = F_FarmOpenSlot(pUserInfo);
	if (nOpenSlot)
		Sleep(2000);
	//����Ұ������
	nDonateBeast = F_FarmDonateBeast(pUserInfo);
	if (nDonateBeast)
		Sleep(2000);
	//����Ұ������
	nAdoptBeast = F_FarmAdoptBeast(pUserInfo);
	if (nAdoptBeast)
		Sleep(2000);
	
	if (nReturnBeast || nOpenSlot || nDonateBeast || nAdoptBeast)
		G_WriteUserInfo(pUserInfo);
	return TRUE;
}
//�����ҵ�ũ��������,�����ճ��������ﴦ��
UINT H_HandleMyFarm(LPVOID pParam)
{
	ST_USERINFO pUserInfo;	//�������ļ��ж�ȡ�Ѿ���ȡ������
	G_GetUserInfo(m_SelfInfo.Id,pUserInfo);
	m_SelfInfo.Lvl = FarmExpToLevel(pUserInfo.Exp);	//����һ���ҵĵȼ�

	UINT nHarvest = FALSE;	//�Ƿ����ջ�
	UINT nScarify = FALSE;	//�Ƿ��з���
	UINT nPlanting = FALSE;	//�Ƿ��в���
	UINT nClearWeed = FALSE;//�Ƿ��г���
	UINT nSpraying	= FALSE;//�Ƿ���ɱ��
	UINT nWatering  = FALSE;//�Ƿ��н�ˮ
	UINT nAttackBeast = FALSE;//�Ƿ��й���Ұ������

	if (m_Config.bSelfSc)
	{
		nHarvest = F_FarmHarvest(pUserInfo);	//ִ���ջ����
		if (nHarvest)			//���������ͣ����
		{
			Sleep(2000);
			if (m_Config.bSaleCrop)
			{
				F_GetUserCrop(NULL);	//��ȡ�ֿ��ջ�
				Sleep(2000);
				F_SaleUserCrop(NULL);	//���������ջ�
				Sleep(2000);
			}
			if (m_Config.bReclaim)		//����
			{
				int nLand = 0;
				for (int i=0;i<24;i++)
				{
					if (pUserInfo.CropLand[i].a == -1)
						nLand++;
				}
				if (nLand > 0)
				{
					F_FarmReclaim(pUserInfo);
					Sleep(2000);
				}
			}
			if (m_Config.bUpRed && m_SelfInfo.Lvl > 27)		//����Ϊ������
			{
				int nRed = 0;
				for (int i=0;i<24;i++)
				{
					if (pUserInfo.CropLand[i].IsRed)
						nRed++;
				}
				int nLvl = m_SelfInfo.Lvl - 27;
				if (nLvl > nRed)
				{
					F_FarmUpgrade(pUserInfo);
					Sleep(2000);
				}
			}
		}
		
		nScarify = F_FarmScarify(pUserInfo);	//ִ�з�������
		if (nScarify)			//���������ͣ����
			Sleep(2000);
	}
	
	if (m_Config.bSelfBp)
	{
		nPlanting = F_FarmPlanting(pUserInfo);	//ִ�в��ֲ���
		if (nPlanting)			//���������ͣ����
			Sleep(2000);
	}

	if (m_Config.bSelfWcs)
	{
		nClearWeed = F_FarmClearWeed(pUserInfo);//ִ�г��ݲ���
		if (nClearWeed)			//���������ͣ����
			Sleep(2000);
		
		nSpraying = F_FarmSpraying(pUserInfo);	//ִ��ɱ�����
		if (nSpraying)
			Sleep(2000);
		
		nWatering = F_FarmWatering(pUserInfo);	//ִ�н�ˮ����
		if (nWatering)
			Sleep(2000);
	}

	nAttackBeast = F_FarmAttackBeast(pUserInfo); //����Ұ������
	if (nAttackBeast)
		Sleep(2000);
	//�����ҵ�Ұ������
	H_HandleMyBeast(pUserInfo);

	if (nHarvest||nScarify||nPlanting)
	{
		if (m_SelfLandId == m_SelfInfo.Id)
			S_ShowMyRipeTime();
		return TRUE;
	}
	if (nClearWeed||nSpraying||nWatering)
	{
		return TRUE;
	}
	return FALSE;
}
//͵ȡ���ѳ��������
UINT F_FarmScrounge(ST_USERINFO &pUserInfo)
{
	std::vector<int> v_Land;
		
		for (int i=0;i<24;i++)
		{	//ѭ���鿴�Ƿ��п�͵ȡ��ֲ��
			if (pUserInfo.CropLand[i].a > 0 && pUserInfo.CropLand[i].b == 6 && pUserInfo.CropLand[i].n == 1)
			{
				v_Land.push_back(i);
			}
		}
		//���û�п�͵ȡ,�򷵻ؼ�
		if (v_Land.size() == 0)
			return FALSE;
	
		CString strPostData;
		strPostData.Format("ownerId=%d&tName=%s&fName=%s&uinY=%s&uinX=%d&uIdx=%d&place=",
			pUserInfo.Id,strConvert(pUserInfo.Name,936,CP_UTF8),strConvert(GetQQInfo(GQI_NAME),936,CP_UTF8),GetQQInfo(GQI_NUM),pUserInfo.QQ,m_SelfInfo.Id);
		for (i=0;i<v_Land.size();i++)
		{
			CString strPlace;
			strPlace.Format("%d",v_Land[i]);
			strPostData += strPlace;
			if ((i+1)<v_Land.size())
				strPostData += "%2C";
		}
		CString strHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_steal_v2?mod=farmlandstatus&act=scrounge",strPostData);
		
		ST_CROPSGROW pCropGrow;
		CString strSub;
		int nOffset = 0;
		nOffset=FindMidStr(strHtml,nOffset,"{","}",strSub);
		while(nOffset!=-1)
		{
			CString strBuf;
			
			int nCode = 0;			//codeΪ1��ɹ�
			CString strDirection;	//��ʾ�ı�
			int nIndex = 0;			//��������
			//int nMoney = 0;			//�ջ�Ľ��
			int nHarvest = 0;		//�ջ�����
			int ncId = 0;			//ֲ��ID
			int nCropStatus = 0;	//�µĽ׶�
			int nHarvestTimes = 0;	//�µļ���
			int nHealth = 0;		//�������
			
			int nHumidity = 0;		//ʪ��
			int nPest = 0;			//����
			int nWeed = 0;			//�Ӳ�
			DWORD dwPlantTime = 0;	//����ʱ��
			//int nThief = 0;			//�Ƿ��͵ȡ
			
			FindMidStr(strSub,0,"\"code\":",",",strBuf);	//codeΪ1��ɹ�		   
			nCode = atoi(strBuf);
			//FindMidStr(strSub,0,"\"direction\":\"","\"",strDirection); //��ʾ�ı�
			strDirection = CFindMidStr(strSub,"\"direction\":\"","\",");
			if (strDirection == "")
				strDirection = CFindMidStr(strSub,"\"plus\":\"","\",");
			
			//FindMidStr(strSub,0,"\"money\":",",",strBuf);
			//nMoney = atoi(strBuf);
			FindMidStr(strSub,0,"\"farmlandIndex\":",",",strBuf);
			nIndex = atoi(strBuf);
			FindMidStr(strSub,0,"\"harvest\":",",",strBuf);
			nHarvest = atoi(strBuf);
			FindMidStr(strSub,0,"\"cId\":",",",strBuf);
			ncId = atoi(strBuf);
			FindMidStr(strSub,0,"\"cropStatus\":",",",strBuf);
			nCropStatus = atoi(strBuf);
			FindMidStr(strSub,0,"\"harvestTimes\":",",",strBuf);
			nHarvestTimes = atoi(strBuf);
			FindMidStr(strSub,0,"\"health\":",",",strBuf);
			nHealth = atoi(strBuf);
			FindMidStr(strSub,0,"\"humidity\":",",",strBuf);
			nHumidity = atoi(strBuf);
			FindMidStr(strSub,0,"\"pest\":",",",strBuf);
			nPest = atoi(strBuf);
			FindMidStr(strSub,0,"\"weed\":",",",strBuf);
			nWeed = atoi(strBuf);
			FindMidStr(strSub,0,"\"plantTime\":",",",strBuf);
			dwPlantTime = atol(strBuf);
			
			if (nIndex < 24)
			{			
				if (nCode == TRUE)
				{
					pUserInfo.CropLand[nIndex].a = ncId;
					pUserInfo.CropLand[nIndex].b = nCropStatus;
					pUserInfo.CropLand[nIndex].j = nHarvestTimes;
					pUserInfo.CropLand[nIndex].i = nHealth;
					pUserInfo.CropLand[nIndex].h = nHumidity;
					pUserInfo.CropLand[nIndex].g = nPest;
					pUserInfo.CropLand[nIndex].f = nWeed;
					pUserInfo.CropLand[nIndex].q = dwPlantTime;
					pUserInfo.CropLand[nIndex].n = 0;
	
					if (ncId != pCropGrow.Id)
						G_GetCropInfo(ncId,pCropGrow);
					CString strPrintf;
					strPrintf.Format("[QQũ��]:ժȡ�˺���[%s]%d�������%d��%s",pUserInfo.Name,nIndex+1,nHarvest,pCropGrow.Name);
					if (strDirection != "")
						strPrintf += "\r\n��" + N_HtmlToText(strDirection);
					Addtolist(strPrintf,COLOR_INFO);
				}
			}
			
			if (nOffset == -1)
				break;
			//������һ��ѭ��
			nOffset=FindMidStr(strHtml,nOffset,"{","}",strSub);
		}
		G_WriteUserInfo(pUserInfo);
	return TRUE;
}
//����Ұ������
UINT F_FarmRaiseBeast(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	//�Ƿ������
	BOOL IsRaised = FALSE;
	//�Ƿ��п�λ
	int  nSlots = 0;
	for (int i=0;i<3;i++)
	{
		if (pUserInfo.BeastLand[i].fId > 0 && pUserInfo.BeastLand[i].fId == atoi(GetQQInfo(GQI_NUM)))
			IsRaised = TRUE;
		if (pUserInfo.BeastLand[i].fId == 0 && pUserInfo.BeastLand[i].Type == 0)
			nSlots++;
	}
	
	if (IsRaised || nSlots == 0)
		return nResult;
	
	ST_BEASTINFO pBeastInfo;
	for (i=0;i<3;i++)
	{
		if (m_MyBeast[i].Id != -1 && m_MyBeast[i].Type > 0 && m_MyBeast[i].Status == 1)
		{
			CString strPostData;
			strPostData.Format("uinX=%d&uIdx=%d&slotid=%d&type=%d&uinY=%s&isfarm=1&ownerId=%d",pUserInfo.QQ,m_SelfInfo.Id,i,m_MyBeast[i].Type,GetQQInfo(GQI_NUM),pUserInfo.Id);
			CString strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_raise_beast",strPostData);
			Sleep(800);
			CString strBuf;
			//�������
			int nECode = 0;
			FindMidStr(strHtml,0,"\"ecode\":",",",strBuf);
			nECode = atoi(strBuf);
			//��Ʒֵ
			int nMoralExp = 0;
			FindMidStr(strHtml,0,"\"moralexp\":",",",strBuf);
			nMoralExp = atoi(strBuf);
			//����ʱ��
			FindMidStr(strHtml,0,"\"returntime\":",",",strBuf);
			m_MyBeast[i].ReturnTime = atol(strBuf);
			
			if (nECode == FALSE)
			{
				nResult = TRUE;
				m_MyBeast[i].Status = 2;
				
				if (pBeastInfo.Id != m_MyBeast[i].Type)
					G_GetBeastInfo(m_MyBeast[i].Type,pBeastInfo);
				
				CString strPrintf;
				strPrintf.Format("[QQũ��]:�ں���[%s]�ҷ�����%s,�����%d����Ʒ",pUserInfo.Name,pBeastInfo.Name,nMoralExp);
				Addtolist(strPrintf,COLOR_INFO);
				
				int nOffset = 0;
				CString strCrystals,strCrystal;
				if (FindMidStr(strHtml,0,",\"drop\":[","],",strCrystals) != -1)
				{
					nOffset = FindMidStr(strCrystals,nOffset,"{","}",strCrystal);
					while(nOffset != -1)
					{
						int nId;
						int nNum;
						//id
						FindMidStr(strCrystal,0,"\"id\":",",",strBuf);
						nId = atoi(strBuf);
						//����
						FindMidStr(strCrystal,0,"\"num\":",",",strBuf);
						nNum = atoi(strBuf);
						
						ST_CRYSTALINFO pCrystalInfo;
						G_GetCrystalInfo(nId,pCrystalInfo);
						strPrintf.Format("[QQũ��]:�����%d��%s",nNum,pCrystalInfo.Name);
						Addtolist(strPrintf,COLOR_INFO);
						
						if (nOffset == -1)
							break;
						nOffset = FindMidStr(strCrystals,nOffset,"{","}",strCrystal);
					}
				}
				
				CString strSteal;
				if (FindMidStr(strHtml,0,"\"steal\":","}",strSteal) != -1)
				{
					int nId;
					//id
					FindMidStr(strSteal,0,"\"cid\":",",",strBuf);
					nId = atoi(strBuf);
					
					ST_CROPSGROW pCropGrow;
					G_GetCropInfo(nId,pCropGrow);
					strPrintf.Format("[QQũ��]:%s͵����1��%s",pBeastInfo.Name,pCropGrow.Name);
					Addtolist(strPrintf,COLOR_INFO);
				}
				
			}
			break;
		}
	}
	return nResult;
}
//�������ũ��������,͵��������
UINT H_HandleFriendFarm(LPVOID pParam)
{
	int nId = (int)pParam;
	ST_USERINFO pUserInfo;	//�������ļ��ж�ȡ�Ѿ���ȡ������
	G_GetUserInfo(nId,pUserInfo);
	if (m_Config.bFdSc)
	{
		if (m_Config.bFdDog || !pUserInfo.Dog)
		{
			if (F_FarmScrounge(pUserInfo) == TRUE)
				Sleep(2000);
		}
	}
	if (bWcsFriends && m_Config.bFdWcs)	//������Գ���ɱ��Ŵ���
	{
		if (F_FarmClearWeed(pUserInfo) == TRUE)
			Sleep(2000);
		if (F_FarmSpraying(pUserInfo) == TRUE)
			Sleep(2000);
		if (F_FarmWatering(pUserInfo) == TRUE)
			Sleep(2000);
	}

	if (F_FarmAttackBeast(pUserInfo) == TRUE)	//����Ұ������
		Sleep(2000);
	F_FarmRaiseBeast(pUserInfo);  //����Ұ������

	return TRUE;
}
//���˺���״̬
UINT F_GetStatusFilter(LPVOID pParam)
{
	std::vector<int>	v_FriendsQQ;
	CString strUins;
	CString strUids;
	for (int i=0;i<v_FriendsInfo.size();i++)
	{
		ST_USERINFO pUserInfo;
		G_GetUserInfo(v_FriendsInfo[i].Id,pUserInfo);
		v_FriendsQQ.push_back(pUserInfo.QQ);
		CString strBuf;
		strBuf.Format("%d",pUserInfo.QQ);
		strUins += strBuf;
		strBuf.Format("%d",pUserInfo.Id);
		strUids += strBuf;
		if ((i+1)<v_FriendsInfo.size())
		{
			strUins += "%2C";
			strUids += "%2C";
		}
	}
	strUins = "&friend%5Fuins=" + strUins;
	strUids = "&friend%5Fuids=" + strUids;
	CString strPostData;
	strPostData.Format("uinY=%s&uIdx=%d&cmd=1%s%s",GetQQInfo(GQI_NUM),m_SelfInfo.Id,strUins,strUids);
	
	CString strHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_getstatus_filter?",strPostData);
	
	int nPos1,nPos2;
	nPos1 = strHtml.Find(":{\"",0);
	if (nPos1 != -1)
	{
		nPos2 = strHtml.Find("\"",nPos1 + 3);
		CString szQQ,szStatus;
		szQQ = strHtml.Mid(nPos1+3,nPos2 - nPos1 - 3);
		nPos1 = strHtml.Find("{",nPos2);
		nPos2 = strHtml.Find("}",nPos1);
		szStatus = strHtml.Mid(nPos1 + 1,nPos2 - nPos1 -1);
		if (szStatus.Find("\"2\"") != -1 || szStatus.Find("\"3\"") != -1 || szStatus.Find("\"4\"") != -1 || szStatus.Find("\"5\"") != -1)
		{
			for (int j=0;j<v_FriendsQQ.size();j++)
			{
				if (v_FriendsQQ[j] == atoi(szQQ))
				{
					F_GetFarmInfo((LPVOID)v_FriendsInfo[j].Id);
					H_HandleFriendFarm((LPVOID)v_FriendsInfo[j].Id);
					Sleep(2000);
				}
			}
		}
		while(strHtml.Find(",\"",nPos2) != -1)
		{
			nPos1 = strHtml.Find(",\"",nPos2);
			nPos2 = strHtml.Find("\"",nPos1 + 2);
			szQQ = strHtml.Mid(nPos1+2,nPos2 - nPos1 - 2);
			nPos1 = strHtml.Find("{",nPos2);
			nPos2 = strHtml.Find("}",nPos1);
			szStatus = strHtml.Mid(nPos1 + 1,nPos2 - nPos1 -1);
			if (szStatus.Find("\"2\"") != -1 || szStatus.Find("\"3\"") != -1 || szStatus.Find("\"4\"" || szStatus.Find("\"5\"") != -1) != -1)
			{
				for (int j=0;j<v_FriendsQQ.size();j++)
				{
					if (v_FriendsQQ[j] == atoi(szQQ))
					{
						F_GetFarmInfo((LPVOID)v_FriendsInfo[j].Id);
						H_HandleFriendFarm((LPVOID)v_FriendsInfo[j].Id);
						Sleep(2000);
					}
				}
			}
		}
	}
	return TRUE;
}
//ɨ�����ũ������,������ĺ�����ͬ�����������Сʱ��,���Զ���ȡINI�е�����
UINT F_ScanFarmInfo(LPVOID pParam)
{
	UINT nResult = FALSE;
	int nId = (int)pParam;	//��ȡID
	ST_USERLIST pUserList;	//�������ļ��ж�ȡ��������
	G_GetUserList(nId,pUserList);
	ST_USERINFO pUserInfo;	//�������ļ��ж�ȡ�Ѿ���ȡ������
	G_GetUserInfo(nId,pUserInfo);
	if (pUserList.CheckTime + 14400 < dwServerTime)
	{
		CString strHtml;
		CString strPostData;	//����POST����,���ʺ���ũ��
		if (nId == m_SelfInfo.Id)	//����ǻ�ȡ�Լ���ũ������
			strPostData.Format("uIdx=%d&flag=1&ownerId=0&uinY=%s",nId,GetQQInfo(GQI_NUM));
		else
			strPostData.Format("uinY=%s&ownerId=%d&uinX=%d&uIdx=%d",GetQQInfo(GQI_NUM),pUserInfo.Id,pUserInfo.QQ,m_SelfInfo.Id);
		
		strHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_index?mod=user&act=run",strPostData);
		
		H_GetFarmInfo(strHtml,pUserInfo);	//�����ص�ũ������
		//����������д�������ļ�
		G_WriteUserInfo(pUserInfo);
		nResult = TRUE;
	}
	return nResult;
}
//��ȡָ���û���ũ������
UINT F_GetFarmInfo(LPVOID pParam)
{
	int nId = (int)pParam;	//��ȡID
	ST_USERINFO pUserInfo;	//�������ļ��ж�ȡ�Ѿ���ȡ������
	G_GetUserInfo(nId,pUserInfo);
	CString strHtml;
	CString strPostData;	//����POST����,���ʺ���ũ��
	if (nId == m_SelfInfo.Id)	//����ǻ�ȡ�Լ���ũ������
		strPostData.Format("uIdx=%d&flag=1&ownerId=0&uinY=%s",nId,GetQQInfo(GQI_NUM));
	else
		strPostData.Format("uinY=%s&ownerId=%d&uinX=%d&uIdx=%d",GetQQInfo(GQI_NUM),pUserInfo.Id,pUserInfo.QQ,m_SelfInfo.Id);
	
	strHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_index?mod=user&act=run",strPostData);
	
	H_GetFarmInfo(strHtml,pUserInfo);	//�����ص�ũ������
	//����������д�������ļ�
	G_WriteUserInfo(pUserInfo);
	return TRUE;
}
BOOL C_FriendsListCmp(ST_USERLIST pUserList1,ST_USERLIST pUserList2)
{
	return pUserList1.Lvl > pUserList2.Lvl;
}
//��ȡ�����б�
UINT F_GetFriends(LPVOID pParam)
{
	CString strPostData;	//����POST����,��ȡ���к���
	strPostData.Format("uinY=%s&uIdx=%d",GetQQInfo(GQI_NUM),m_SelfInfo.Id);
	CString strHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_getFriendList?mod=friend",strPostData);
	
	if (!v_FriendsInfo.empty())	//���׺����б��Ƿ�Ϊ��
		v_FriendsInfo.clear();	//���������
	CString strBuf;
	int nOffset = 0;
	nOffset = FindMidStr(strHtml,nOffset,"{","}",strBuf);
	while(nOffset!=-1)
	{
		CString strItem;
		CString strQQ;
		ST_USERINFO pUserInfo;
		//����ID
		FindMidStr(strBuf,0,"\"uId\":",",",strItem);
		pUserInfo.Id = atoi(strItem);
		G_GetUserInfo(pUserInfo.Id,pUserInfo);
		//����QQ
		FindMidStr(strBuf,0,"\"uin\":",",",strQQ);
		pUserInfo.QQ = atoi(strQQ);
		//���Ҿ���
		FindMidStr(strBuf,0,"\"exp\":",",",strItem);
		pUserInfo.Exp = atol(strItem);
		//���ҽ��
		FindMidStr(strBuf,0,"\"money\":",",",strItem);
		pUserInfo.Money = atol(strItem);
		//�Ƿ�ͨ����
		FindMidStr(strBuf,0,"\"pf\":",",",strItem);
		pUserInfo.pf = atoi(strItem);
		//�����ǳ�
		FindMidStr(strBuf,0,"\"userName\":\"","\"",strItem);
		strcpy(pUserInfo.Name,strItem.GetBuffer(strItem.GetLength()));
		strItem.ReleaseBuffer();
		if (nOffset == -1)
			break;
		if (strQQ != GetQQInfo(GQI_NUM))	//�����������Լ���ID�ű���
		{
			//��ũ������д��INI�ļ�
			G_WriteUserInfo(pUserInfo);
			ST_USERLIST pUserList;		//��������
			G_GetUserList(pUserInfo.Id,pUserList);
			pUserList.Id = pUserInfo.Id;	//д�뵽�����б������ļ�
			pUserList.Lvl = FarmExpToLevel(pUserInfo.Exp);	//�ȼ�
			G_WriteUserList(pUserList);
			//���������ݼ�������
			v_FriendsInfo.push_back(pUserList);
		}
		nOffset = FindMidStr(strHtml,nOffset,"{","}",strBuf);
	}
	std::sort(v_FriendsInfo.begin(),v_FriendsInfo.end(),C_FriendsListCmp);
	return TRUE;
}
UINT F_GetUserSeed(LPVOID pParam)
{
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
		pCropGrow.nSeedNum = 0;			//�����������
		G_WriteCropInfo(pCropGrow);		//����д�������ļ�
		nPos += strlen(szCrop) + 1;
	}
	
	CString strPostData;
	strPostData.Format("uinY=%s&uIdx=%d",GetQQInfo(GQI_NUM),m_SelfInfo.Id);
	CString strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_getuserseed?mod=repertory&act=getUserSeed",strPostData);
	CString strBuf;
	int nOffset = 0;
	nOffset = FindMidStr(strHtml,nOffset,"{","}",strBuf);
	while(nOffset!=-1)
	{
		CString strItem;
		ST_CROPSGROW pCropGrow;
		//����ID
		if (FindMidStr(strBuf,0,"\"cId\":",",",strItem) != -1)
		{
			pCropGrow.Id = atoi(strItem);
			G_GetCropInfo(pCropGrow.Id,pCropGrow);
			//����
			FindMidStr(strBuf,0,"\"amount\":",",",strItem);
			pCropGrow.nSeedNum = atoi(strItem);
			//�ȼ�
			FindMidStr(strBuf,0,"\"level\":",",",strItem);
			pCropGrow.Level = atoi(strItem);
			G_WriteCropInfo(pCropGrow);
		}
		
		if (nOffset == -1)
			break;
		
		nOffset = FindMidStr(strHtml,nOffset,"{","}",strBuf);
	}
	return TRUE;
}
UINT F_GetUserCrop(LPVOID pParam)
{
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
		pCropGrow.nCropNum = 0;			//��ղֿ��ջ�����
		G_WriteCropInfo(pCropGrow);		//����д�������ļ�
		nPos += strlen(szCrop) + 1;
	}
	
	CString strPostData;
	strPostData.Format("uinY=%s&uIdx=%d",GetQQInfo(GQI_NUM),m_SelfInfo.Id);
	CString strHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_getusercrop?f=1",strPostData);
	int nOffset = 0;
	if ((nOffset=strHtml.Find("\"crop\":",0)) != -1)
	{
		CString strBuf;
		nOffset = FindMidStr(strHtml,nOffset,"{","}",strBuf);
		while(nOffset!=-1)
		{
			CString strItem;
			ST_CROPSGROW pCropGrow;
			//����ID
			if (FindMidStr(strBuf,0,"\"cId\":",",",strItem) != -1)
			{
				pCropGrow.Id = atoi(strItem);
				G_GetCropInfo(pCropGrow.Id,pCropGrow);
				//����
				FindMidStr(strBuf,0,"\"amount\":",",",strItem);
				pCropGrow.nCropNum = atoi(strItem);
				//����״̬
				if (!pCropGrow.IsLock)
				{
					FindMidStr(strBuf,0,"\"isLock\":",",",strItem);
					pCropGrow.IsLock = strItem == "1" ? TRUE:FALSE;
				}
				//���ۼ۸�
				FindMidStr(strBuf,0,"\"price\":",",",strItem);
				pCropGrow.SalePrice = atoi(strItem);
				//д�������ļ�
				G_WriteCropInfo(pCropGrow);
			}
			
			if (nOffset == -1)
				break;
			
			nOffset = FindMidStr(strHtml,nOffset,"{","}",strBuf);
		}
	}
	return TRUE;
}
UINT F_SaleUserCrop(LPVOID pParam)
{
	//������������
	int nNum = 0;
	CString strCrops;
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
		if (pCropGrow.nCropNum > 0 && !pCropGrow.IsLock && pCropGrow.Type == 1)
		{
			CString strId;
			strId.Format("%d",pCropGrow.Id);
			strCrops = strCrops + strId + "%2C";
			nNum += pCropGrow.nCropNum;
			pCropGrow.nCropNum = 0;
			G_WriteCropInfo(pCropGrow);
		}
		nPos += strlen(szCrop) + 1;
	}
	if (strCrops != "" && nNum >= m_Config.nSaleNum)
	{
		strCrops = strCrops.Left(strCrops.GetLength()-3);
		CString strPostData;
		strPostData.Format("onlineTime=0&uId=%d&cIds=%s&uinY=%s&uinX=%s&uIdx=%d",m_SelfInfo.Id,strCrops,GetQQInfo(GQI_NUM),GetQQInfo(GQI_NUM),m_SelfInfo.Id);
		CString strHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_saleall?mod=repertory&act=saleAll",strPostData);
		
		CString strBuf;
		int nCode = 0;			//codeΪ1��ɹ�
		CString strDirection;	//��ʾ�ı�
		int nMoney = 0;			//���ӵĽ��
		
		FindMidStr(strHtml,0,"\"code\":",",",strBuf);	//codeΪ1��ɹ�		   
		nCode = atoi(strBuf);
		strDirection = CFindMidStr(strHtml,"\"direction\":\"","\",");
		FindMidStr(strHtml,0,"\"money\":","}",strBuf);
		nMoney = atoi(strBuf);
		
		if (nCode == TRUE)
		{
			ST_USERINFO pUserInfo;
			G_GetUserInfo(m_SelfInfo.Id,pUserInfo);
			pUserInfo.Money += nMoney;
			G_WriteUserInfo(pUserInfo);
			CString strPrintf;
			strPrintf.Format("[QQũ��]:�����ֿ��ջ�ɹ�(δ������),�����%d�����",nMoney);
			Addtolist(strPrintf,COLOR_INFO);
			return TRUE;
		}
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////
//������ʾ����															//
//////////////////////////////////////////////////////////////////////////
UINT S_ShowUserInfo(LPVOID pParam)
{
	int nId = (int)pParam;	//��ȡID
	ST_USERINFO pUserInfo;	//��INI�ļ��ж�ȡָ��ID����ϸ����
	if (!G_GetUserInfo(nId,pUserInfo))
		return FALSE;
	strName = pUserInfo.Name;	//�����ǳ�
	int nLvl = FarmExpToLevel(pUserInfo.Exp);	//����ת��Ϊ�ȼ�
	strLevel.Format("%d",nLvl);					//���õȼ�
	strGold.Format("%d",pUserInfo.Money);		//���ý��
	strExp.Format("%d",pUserInfo.Exp);			//���þ���
	strDog = pUserInfo.Dog == 1 ? "��":"��";	//����״̬
	strHealthMode = IsHealthMode(pUserInfo.BeginTime,pUserInfo.EndTime) ? "��":"��";	//��ȫģʽ
	int nNextLvlExp = nExpcmplist[nLvl + 1] - nExpcmplist[nLvl];	//ʣ����������
	int nNeedExp = pUserInfo.Exp - nExpcmplist[nLvl];			
	strUpLvlExp.Format("%d/%d",nNeedExp,nNextLvlExp);	
	hDlg_Main->m_Pgs_UplvExp.SetRange(0,nNextLvlExp);	//���õ�������
	hDlg_Main->m_Pgs_UplvExp.SetPos(nNeedExp);
	PostMessage(hWndMain,WM_UPDATEDATA,0,0);		//ˢ�´���
	return TRUE;
}
void S_ShowFriendsRipeTimeOnPos(int nId)
{
	DWORD dwRipeTime = v_FriendsInfo[nId].RipeTime;
	if (dwRipeTime > dwServerTime)
	{
		/*CTime t1;
		t1 = dwRipeTime;
		CString out_time;
		out_time = t1.Format("%d %H-%M-%S");*/
		CTime t1,t2;
		CTimeSpan t3;
		int nD,nH,nM;
		t1 = dwRipeTime;
		t2 = dwServerTime;
		t3 = t1 - t2;
		nD = (int)t3.GetDays();
		nH = t3.GetHours();
		nM = t3.GetMinutes();
		if (nD > 0)
			nH += nD * 24;
		CString out_time;
		out_time.Format("%dСʱ%d��",nH,nM);
		//hDlg_Main->m_List_Friends.SetRedraw(FALSE);
		hDlg_Main->m_List_Friends.SetItemText(nId,4,out_time);
		//hDlg_Main->m_List_Friends.SetRedraw(TRUE);
	}
	else
	{
		hDlg_Main->m_List_Friends.SetItemText(nId,4,"");
	}
}
//��ʾ����ũ������ʱ��
void S_ShowFriendsRipeTime()
{
	for (int i=0;i<v_FriendsInfo.size();i++)
	{
		S_ShowFriendsRipeTimeOnPos(i);		
	}
}
//��ʾ�ҵ�ũ����������
void S_ShowMyRipeTime()
{
	hDlg_Main->m_List_Lands.DeleteAllItems();	//������б�
	ST_USERINFO pUserInfo;
	G_GetUserInfo(m_SelfLandId,pUserInfo);
	ST_CROPSGROW pCropGrow;	//ֲ������
	for (int i=0;i<24;i++)
	{
		int nId = pUserInfo.CropLand[i].a;	//����ֲ��ID,0Ϊ�յ�
		if (nId > 0)
		{
			if (nId != pCropGrow.Id)	//���ֲ�����ݲ�ͬ,���ȡ�µ�
				G_GetCropInfo(nId,pCropGrow);

			char szNum[SHORTLEN]={0};
			itoa(i+1,szNum,10);
			//���
			hDlg_Main->m_List_Lands.InsertItem(i,szNum);
			//ֲ������
			hDlg_Main->m_List_Lands.SetItemText(i,1,pCropGrow.Name);
			//�ȼ�
			ZeroMemory(szNum,SHORTLEN);
			itoa(pCropGrow.Level,szNum,10);
			hDlg_Main->m_List_Lands.SetItemText(i,2,szNum);
			//����
			CString strBuf;
			strBuf.Format("%d/%d",pUserInfo.CropLand[i].j+1,pCropGrow.HarvestNum);
			hDlg_Main->m_List_Lands.SetItemText(i,3,strBuf);
			//�����
			int nNum = pUserInfo.CropLand[i].b;
			if (nNum < 6)
			{
				CTime t1,t2;
				CTimeSpan t3;
				int nD,nH,nM;
				t1 = pUserInfo.CropLand[i].q + pCropGrow.CropGrow[4];
				t2 = dwServerTime;
				t3 = t1 - t2;
				nD = (int)t3.GetDays();
				nH = t3.GetHours();
				nM = t3.GetMinutes();
				if (nD > 0)
					nH += nD * 24;
				strBuf.Format("%dСʱ%d��",nH,nM);
				hDlg_Main->m_List_Lands.SetItemText(i,4,strBuf);
			} else if (nNum == 6)
			{
				hDlg_Main->m_List_Lands.SetItemText(i,4,"����");
			} else if (nNum == 7)
			{
				hDlg_Main->m_List_Lands.SetItemText(i,4,"�ѿ�ή");
			}
		}
		else if (nId == 0)
		{
			char szNum[SHORTLEN]={0};
			itoa(i+1,szNum,10);
			//���
			hDlg_Main->m_List_Lands.InsertItem(i,szNum);
			//ֲ������
			hDlg_Main->m_List_Lands.SetItemText(i,1,"�յ�");
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//���ݴ洢����															//
//////////////////////////////////////////////////////////////////////////
UINT G_GetUserInfo(int nId,ST_USERINFO &pUserInfo)
{
	char szuId[QQNUMLEN]={0};
	itoa(nId,szuId,10);
	return GetPrivateProfileStruct(szuId,"UserInfo",&pUserInfo,sizeof(ST_USERINFO),strFriendsDataIni);
}
UINT G_WriteUserInfo(ST_USERINFO &pUserInfo)
{
	char szuId[QQNUMLEN]={0};	//�û�ũ��ID
	itoa(pUserInfo.Id,szuId,10);	//ת��IDΪ�ı�
	return WritePrivateProfileStruct(szuId,"UserInfo",&pUserInfo,sizeof(ST_USERINFO),strFriendsDataIni);
}
UINT G_GetUserList(int nId,ST_USERLIST &pUserList)
{
	char szuId[QQNUMLEN]={0};	//�û�ũ��ID
	itoa(nId,szuId,10);	//ת��IDΪ�ı�
	return GetPrivateProfileStruct(szuId,"UserList",&pUserList,sizeof(ST_USERLIST),strFriendsListIni);
}
UINT G_WriteUserList(ST_USERLIST &pUserList)
{
	char szuId[QQNUMLEN]={0};	//�û�ũ��ID
	itoa(pUserList.Id,szuId,10);	//ת��IDΪ�ı�	
	return WritePrivateProfileStruct(szuId,"UserList",&pUserList,sizeof(ST_USERLIST),strFriendsListIni);
}
UINT G_GetCropInfo(int nId,ST_CROPSGROW &pCropGrow)
{
	char szuId[SHORTLEN]={0};
	itoa(nId,szuId,10);
	return GetPrivateProfileStruct(szuId,"CropInfo",&pCropGrow,sizeof(ST_CROPSGROW),strCropsIni);
}
UINT G_WriteCropInfo(ST_CROPSGROW &pCropGrow)
{
	char szuId[SHORTLEN]={0};
	itoa(pCropGrow.Id,szuId,10);
	return WritePrivateProfileStruct(szuId,"CropInfo",&pCropGrow,sizeof(ST_CROPSGROW),strCropsIni);
}
UINT G_GetGlobalConfig(ST_GLOBALCONFIG &pConfig)
{
	return GetPrivateProfileStruct("config","global",&pConfig,sizeof(ST_GLOBALCONFIG),strQQFarm_ini);
}
UINT G_WriteGlobalConfig(ST_GLOBALCONFIG &pConfig)
{
	return WritePrivateProfileStruct("config","global",&pConfig,sizeof(ST_GLOBALCONFIG),strQQFarm_ini);
}
UINT G_GetCardsGame(int nId,ST_CARDSGAME &pCardsGame)
{
	char szuId[SHORTLEN]={0};
	itoa(nId,szuId,10);
	return GetPrivateProfileStruct(szuId,"CardsGame",&pCardsGame,sizeof(ST_CARDSGAME),strAddonIni);
}
UINT G_WriteCardsGame(ST_CARDSGAME &pCardsGame)
{
	char szuId[SHORTLEN]={0};
	itoa(pCardsGame.Id,szuId,10);
	return WritePrivateProfileStruct(szuId,"CardsGame",&pCardsGame,sizeof(ST_CARDSGAME),strAddonIni);
}
UINT G_GetBeastInfo(int nId,ST_BEASTINFO &pBeastInfo)
{
	char szuId[SHORTLEN]={0};
	itoa(nId,szuId,10);
	return GetPrivateProfileStruct(szuId,"BeastInfo",&pBeastInfo,sizeof(ST_BEASTINFO),strBeastIni);
}
UINT G_WriteBeastInfo(ST_BEASTINFO &pBeastInfo)
{
	char szuId[SHORTLEN]={0};
	itoa(pBeastInfo.Id,szuId,10);
	return WritePrivateProfileStruct(szuId,"BeastInfo",&pBeastInfo,sizeof(ST_BEASTINFO),strBeastIni);
}
UINT G_GetCrystalInfo(int nId,ST_CRYSTALINFO &pCrystalInfo)
{
	char szuId[SHORTLEN]={0};
	itoa(nId,szuId,10);
	return GetPrivateProfileStruct(szuId,"CrystalInfo",&pCrystalInfo,sizeof(ST_CRYSTALINFO),strCrystalIni);
}
UINT G_WriteCrystalInfo(ST_CRYSTALINFO &pCrystalInfo)
{
	char szuId[SHORTLEN]={0};
	itoa(pCrystalInfo.Id,szuId,10);
	return WritePrivateProfileStruct(szuId,"CrystalInfo",&pCrystalInfo,sizeof(ST_CRYSTALINFO),strCrystalIni);
}
