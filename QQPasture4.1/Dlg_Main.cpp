// Dlg_Main.cpp : implementation file
//

#include "stdafx.h"
#include "QQPasture.h"
#include "Dlg_Main.h"
#include "Dlg_Config.h"
#include "Dlg_Package.h"
#include "Dlg_Animals.h"

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
#define IDT_SHOWMYRIPE		2				//ˢ���ҵ���������ʱ��
#define IDT_SHOWFRIENDSRIPE 3				//ˢ�º����б����ʱ��
#define IDT_GETMYPASTURE	5				//ˢ���ҵ�ũ��
#define IDT_GETFRIENDPASTURE 6				//ˢ�º���ũ��
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
CString						strAnimalFood;	//��
CString						strGuard;		//����
CString						strUpLvlExp;	//�������辭��
int							numMin,numMax,numPos;
//WorkThread�¼�����
BOOL						bGetBlogData = TRUE;	//��BLOG��ȡ��Ҫ����
BOOL						bGetServerTime = TRUE;	//��ȡ����������ʱ��
BOOL						bGetXmlData	= TRUE;		//��ȡ������Դ����
BOOL						bGetFriends = TRUE;		//��ȡ�����б�
BOOL						bScanFriends = TRUE;	//ɨ���������
BOOL						bGetStatusFilter = FALSE; //���˺���״̬
BOOL						bWorking	= TRUE;		//����״̬
BOOL						bGetMyPasture = FALSE;	//ˢ���ҵ�����
//HandlePasture�¼�����
BOOL						bClearMosquito = TRUE;	//Ϊ����������
BOOL						bClearShit	= TRUE;		//Ϊ��������
BOOL						bAddFoodToFd = TRUE;	//Ϊ�����������

ST_USERLIST					m_SelfInfo;			//�ҵ�����,��������
std::vector<ST_USERLIST>	v_FriendsInfo;		//�ҵĺ���,��������
int							m_SelfLandId = 0;	//��ǰ��ʾ��ID
int							m_Pastures = 0;			//���������ݵ�����
ST_GLOBALCONFIG				m_Config;			//ȫ������

CString						m_gtk;				//Cookie Gtk

CString						strLogPath;
//////////////////////////////////////////////////////////////////////////
//ǰ�˶���																//
//////////////////////////////////////////////////////////////////////////
UINT N_RootThread(LPVOID pParam);
UINT H_WorkThread(LPVOID pParam);
UINT N_SetGlobalTimer(LPVOID pParam);

UINT H_GetPastureInfo(CString &strHtml,ST_USERINFO &pUserInfo);
DWORD H_GetPastureLandRipe(ST_USERINFO &pUserInfo);
UINT H_CountPasture(ST_USERINFO &pUserInfo);
UINT H_HandleMyPasture(LPVOID pParam);
UINT H_HandleFriendPasture(LPVOID pParam);
UINT F_ScanPastureInfo(LPVOID pParam);
UINT F_GetPastureInfo(LPVOID pParam);
UINT F_GetFriends(LPVOID pParam);
UINT F_GetUserPackage(LPVOID pParam);
UINT F_GetUserAnimals(LPVOID pParam);
UINT F_SaleUserAnimals(LPVOID pParam);
UINT F_ScanPastureInfo(LPVOID pParam);
UINT F_GetPastureInfo(LPVOID pParam);

UINT S_ShowUserInfo(LPVOID pParam);
void S_ShowMyRipeTime();
void S_ShowFriendsRipeTimeOnPos(int nId);
void S_ShowFriendsRipeTime();

UINT G_GetUserInfo(int nId,ST_USERINFO &pUserInfo);
UINT G_WriteUserInfo(ST_USERINFO &pUserInfo);
UINT G_GetUserList(int nId,ST_USERLIST &pUserList);
UINT G_WriteUserList(ST_USERLIST &pUserList);
UINT G_GetAnimalInfo(int nId,ST_ANIMALGROW &pAnimalGrow);
UINT G_WriteAnimalInfo(ST_ANIMALGROW &pAnimalGrow);
UINT G_GetCardsGame(int nId,ST_CARDSGAME &pCardsGame);
UINT G_WriteCardsGame(ST_CARDSGAME &pCardsGame);
UINT G_GetBeastInfo(int nId,ST_BEASTINFO &pBeastInfo);
UINT G_WriteBeastInfo(ST_BEASTINFO &pBeastInfo);
UINT G_GetCrystalInfo(int nId,ST_CRYSTALINFO &pCrystalInfo);
UINT G_WriteCrystalInfo(ST_CRYSTALINFO &pCrystalInfo);

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
	//{{AFX_DATA_MAP(CDlg_Main)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX,IDC_MAIN_LANDS,m_List_Lands);
	DDX_Control(pDX,IDC_MAIN_FRIENDS,m_List_Friends);
	DDX_Control(pDX,IDC_PGS_UPLVLEXP,m_Pgs_UplvExp);
	//�󶨿ؼ��ı�
	DDX_Text(pDX,IDC_MAIN_NAME,strName);
	DDX_Text(pDX,IDC_MAIN_LEVEL,strLevel);
	DDX_Text(pDX,IDC_MAIN_GOLD,strGold);
	DDX_Text(pDX,IDC_MAIN_EXP,strExp);
	DDX_Text(pDX,IDC_MAIN_ANIMALFOOD,strAnimalFood);
	DDX_Text(pDX,IDC_MAIN_GUARD,strGuard);
	DDX_Text(pDX,IDC_MAIN_UPLVLEXP,strUpLvlExp);
}


BEGIN_MESSAGE_MAP(CDlg_Main, CDialog)
	//{{AFX_MSG_MAP(CDlg_Main)
	ON_WM_PAINT()
	ON_MESSAGE(WM_UPDATEDATA,OnUpdateData)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_MAIN_FRIENDS, OnClickMainFriends)
	ON_BN_CLICKED(IDC_MAIN_BTN_MYPASTURE, OnMainBtnMypasture)
	ON_BN_CLICKED(IDC_MAIN_BTN_SHOWPACKAGE, OnMainBtnShowpackage)
	ON_BN_CLICKED(IDC_MAIN_BTN_SHOWANIMALS, OnMainBtnShowanimals)
	ON_BN_CLICKED(IDC_MAIN_BTN_CONFIG, OnMainBtnConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_Main message handlers
CDlg_Main::OnInitDialog()
{
	CDialog::OnInitDialog();
	hWndMain = this->m_hWnd;
	//�����������б���ӱ�ͷ
	m_List_Lands.InsertColumn(0,"���",NULL,40,-1);
	m_List_Lands.InsertColumn(1,"����",NULL,100,-1);
	m_List_Lands.InsertColumn(2,"�ȼ�",NULL,40,-1);
	m_List_Lands.InsertColumn(3,"�׶�",NULL,50,-1);
	m_List_Lands.InsertColumn(4,"����һ�׶�",NULL,102,-1);
	//�����б������ѡ�����ʾ����
	m_List_Lands.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	
	//�������б����ӱ�ͷ
	m_List_Friends.InsertColumn(0,"���",NULL,40,-1);
	m_List_Friends.InsertColumn(1,"�ǳ�",NULL,90,-1);
	m_List_Friends.InsertColumn(2,"�ȼ�",NULL,40,-1);
	m_List_Friends.InsertColumn(3,"���",NULL,70,-1);
	m_List_Friends.InsertColumn(4,"����һ�׶�",NULL,110,-1);
	//�����б������ѡ�����ʾ����
	m_List_Friends.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	
	//��ʼ��������ձ�
	pInitExpCmpList();

	//��ʾQQͷ��
	CC_CreateThread(pShowHeadPic,(LPVOID)atoi(GetQQInfo(GQI_NUM)),FALSE);
	
	//���õ�ǰQQ��ר��Ŀ¼
	strSelfPath = GetAppPath(GAP_DIR) + GetQQInfo(GQI_NUM);
	CreateDirectory(strSelfPath,NULL);
	strAnimalsIni =		strSelfPath + "\\QQPasture_Animals.ini";
	strFriendsDataIni = strSelfPath + "\\QQPasture_FriendsData.ini";
	strFriendsListIni = strSelfPath + "\\QQPasture_FriendsList.ini";
	strAddonIni	=		strSelfPath + "\\QQPasture_Addon.ini";
	strBeastIni =		strSelfPath + "\\QQPasture_Beast.ini";
	strCrystalIni =		strSelfPath + "\\QQPasture_Crystal.ini";
	//�趨�洢XML�汾��INI·��
	strQQPasture_ini = strSelfPath + "\\QQPasture_Config.ini";
	
	strLogPath =		strSelfPath + "\\QQPasture_PostRetStr.log";

	//���汾����,�����ͬ��ִ�и��´���
	CString DataVersion;
	GetPrivateProfileString("version","dataversion",NULL,DataVersion.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQPasture_ini);
	DataVersion.ReleaseBuffer();
	int nDataVersion = atoi(DataVersion);
	if (nDataVersion == 0)
	{
		//ǿ�Ƹ�����չ����
		WritePrivateProfileString("xmldata","config_url","",strQQPasture_ini);
		nDataVersion ++;
	}
	if (nDataVersion == 1)
	{
		//ȫ�������б仯,��ʾ��������
		CC_PopTrayMsg("QQ����","ȫ�������б䶯,����������",PTM_INFO,2000);
		nDataVersion ++;
	}
	//д�뵱ǰ�汾����
	WritePrivateProfileString("version","dataversion","2",strQQPasture_ini);
	
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
	case IDT_GETMYPASTURE:
		m_SelfInfo.RipeTime = dwServerTime - 1;
		bGetMyPasture = TRUE;
		break;
	case IDT_GETFRIENDPASTURE:
		bScanFriends = TRUE;
		bGetXmlData = TRUE;
		bGetBlogData = TRUE;
		break;
	case IDT_RESETALLCTRL:
		bClearMosquito = TRUE;		//����Ϊ����������
		bClearShit = TRUE;			//����Ϊ��������
		bAddFoodToFd = TRUE;		//����Ϊ�����������
		bGetXmlData = TRUE;
		N_SetGlobalTimer(NULL);		//�����������м�ʱ��
		break;
	case IDT_WORKTIME:
		CC_Addtolist("[QQ����]:����ʱ�����,׼����ͣ",COLOR_INFO);
		bWorking = FALSE;
		KillTimer(IDT_WORKTIME);
		SetTimer(IDT_SLEEPTIME,60*60*1000,NULL);
		break;
	case IDT_SLEEPTIME:
		CC_Addtolist("[QQ����]:��ͣʱ�����,׼����������",COLOR_INFO);
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

void CDlg_Main::OnClickMainFriends(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (m_List_Friends.GetSelectedCount() > 0)
	{
		int nMark = m_List_Friends.GetSelectionMark();
		SetDlgItemText(IDC_MAIN_BTN_MYPASTURE,"�ҵ�����");
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

void CDlg_Main::OnMainBtnMypasture() 
{
	if (m_SelfLandId == m_SelfInfo.Id)
	{
		m_SelfInfo.RipeTime = dwServerTime - 1;
		bGetMyPasture = TRUE;	//ǿ�Ƹ����ҵ�����
	}
	else
	{
		SetDlgItemText(IDC_MAIN_BTN_MYPASTURE,"ˢ������");
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

void CDlg_Main::OnMainBtnShowpackage() 
{
	CDlg_Package hDlg_Package;
	hDlg_Package.DoModal();
}

void CDlg_Main::OnMainBtnShowanimals() 
{
	CDlg_Animals hDlg_Animals;
	hDlg_Animals.DoModal();
}

void CDlg_Main::OnMainBtnConfig() 
{
	CDlg_Config hDlg_Config;
	hDlg_Config.DoModal();
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
UINT N_SetGlobalTimer(LPVOID pParam)
{
	KillTimer(hWndMain,IDT_GETMYPASTURE);
	SetTimer(hWndMain,IDT_GETMYPASTURE,10800000,NULL);	//��Сʱˢ���ҵ�����
	
	KillTimer(hWndMain,IDT_GETFRIENDPASTURE);
	SetTimer(hWndMain,IDT_GETFRIENDPASTURE,43200000,NULL);	 //ʮ����Сʱˢ�����к�������
	
	CTime tNow;
	tNow = CTime::GetCurrentTime();
	int nH = tNow.GetHour();
	int nM = tNow.GetMinute();
	int nS = tNow.GetSecond();
	int nNow = (nH*60*60)+(nM*60)+(nS);	//��ǰʱ��
	nNow = 86400 - nNow + 400;
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
CString N_PasturePostUrl(CString strHost,CString strUrl,CString strPostData)
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
		//�Ե�ǰʱ���ȡKEY��Կ
		CString strVCode;	//��֤��
M1:
		time_t time;
		CString strFarmKey = PastureTimeSha1PastureKey(time);
		CString szPostData;	//����POST����
		szPostData.Format("pastureKey=%s&farmTime=%d&%s",strFarmKey,(DWORD)time,strPostData);
		if (strVCode != "")	//�����֤�벻Ϊ��,����ӵ�POST����
		{
			szPostData = szPostData + "&validatemsg=" + strVCode;
			strVCode = "";		//�����֤��
		}
		//POST�ύ,�ȴ������ı�
		strHtml = PostUrl(strHost,strUrl,strPostHeader,szPostData,TRUE);

		FILE *fp;
		fp = fopen(strLogPath.GetBuffer(strLogPath.GetLength()),"w");
		strLogPath.ReleaseBuffer();
		fprintf(fp,"http://%s%s\r\n%s\r\n\r\n%s",strHost,strUrl,strPostData,strHtml.GetBuffer(strHtml.GetLength()));
		strHtml.ReleaseBuffer();
		fclose(fp);

		switch (H_CheckPost(strHtml))	//������,���ݶ�Ӧ������д���
		{
		case 1: //��Ҫ���µ�¼
			//MessageBox(hWndMain,"��Ҫ���µ�¼","��ʾ",0);
			::SendMessage(CChWndMain,EVENT_ALL_RELOGIN,0,0);	
			goto M1;	//����M1�����ٴ�ִ��
		case 2: //ϵͳ��æ,�Ժ�����
			CC_Addtolist("[QQ����]:ϵͳ��æ,��ȡ����ʧ��,һ���Ӻ�����.",COLOR_ERROR);
			Sleep(60000); //��ͣһ����
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
	
	//��ѯ����KEY
	nPos1 = szHtml.Find("[pasturekey]");
	nPos2 = szHtml.Find("[/pasturekey]",nPos1);
	if (nPos1 == -1 || nPos2 == 1)
		return FALSE;
	strQQPastureKey = szHtml.Mid(nPos1 + 12,nPos2 - nPos1 - 12);
	//��������KEY
	strQQPastureKey=UncrypKey(strQQPastureKey,"");
	
	return TRUE;
}
//��ȡ��Դ����
UINT N_GetXmlData(LPVOID pParam)
{
	//��ȡ��ҳ������
	CString szHtml = GetUrl("http://mcbase.qzone.qq.com/cgi-bin/index/appbase_run_unity.cgi?appid=358&max=0&appparams=353%7C",TRUE);
	//��ѯ���°��XML
	CString	strConfig_url;
	CString	strConfig_data;
	if(FindMidStr(szHtml,0,"\"config_url_qz\":\"","\"",strConfig_url) == -1 ||
		FindMidStr(szHtml,0,"\"animalConfig_url_qz\":\"","\"",strConfig_data) == -1)
		return FALSE;

	//��ȡ��������
	G_GetGlobalConfig(m_Config);

	//�����ö�ȡ��¼��XML�汾
	CString strConfig_url_ini;
	CString strConfig_data_ini;
	GetPrivateProfileString("xmldata","config_url",NULL,strConfig_url_ini.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQPasture_ini);
	strConfig_url_ini.ReleaseBuffer();
	GetPrivateProfileString("xmldata","config_data",NULL,strConfig_data_ini.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQPasture_ini);
	strConfig_data_ini.ReleaseBuffer();

	//����ҳ��XML�汾�ͱ��ش洢��XML�汾���жԱ�,��ͬ�����SWF����
	CString strMaster;
	CString strFarmui1;
	CString strFarmui2;
	CString strCommonui;
	if (strConfig_url != strConfig_url_ini)
	{
		CC_Addtolist("[QQ����]:�����и��°汾,��ʼ��������Э��",COLOR_INFO);
		CC_PopTrayMsg("[QQ����]","�����и��°汾,��ʼ��������Э��",PTM_INFO,2000);
		//���ذ汾XML��ֲ������XML
		CString strConfig_url_xml = GetAppPath(GAP_DIR)+"mc_material.xml";
		if (!GetUrlToFile(strConfig_url,strConfig_url_xml,TRUE))
			return FALSE;
		//��ʼ����XML����
		CMarkup xml;
		if (!xml.Load(strConfig_url_xml))			//����XML
			return FALSE;
		xml.ResetMainPos();							//��������λ��,����������
		xml.FindElem("main");						//���²���mainԪ��
		xml.IntoElem();								//����Ԫ���ڲ�
		xml.FindElem("moduleList");					//��main�ڲ�����moduleListԪ��
		xml.IntoElem();
		xml.FindElem("module");						//��moduleList�ڲ���module
		strMaster=xml.GetAttrib("url");				//��ȡmoduleԪ�ص�urlԪ��
		xml.IntoElem();
		xml.FindElem("material");
		strFarmui1 = xml.GetAttrib("url");
		xml.FindElem("material");
		strFarmui2 = xml.GetAttrib("url");
		xml.FindElem("material");
		strCommonui = xml.GetAttrib("url");
		xml.OutOfElem();
		xml.OutOfElem();

		//��ʼ������Ƭ��Ϸ����
		xml.FindElem("flopCards");
		xml.IntoElem();
		while(xml.FindElem("gift"))
		{
			ST_CARDSGAME pCardsGame;
			pCardsGame.Id = atoi(xml.GetAttrib("sid"));
			strcpy(pCardsGame.Name,xml.GetAttrib("name"));
			pCardsGame.Num = atoi(xml.GetAttrib("num"));
			G_WriteCardsGame(pCardsGame);
		}
		//ǩ������
		xml.OutOfElem();
		xml.FindElem("flopGifts");
		xml.IntoElem();
		while(xml.FindElem("gift"))
		{
			ST_CARDSGAME pCardsGame;
			pCardsGame.Id = atoi(xml.GetAttrib("id"));
			strcpy(pCardsGame.Name,xml.GetAttrib("name"));
			pCardsGame.Num = atoi(xml.GetAttrib("num"));
			G_WriteCardsGame(pCardsGame);
		}
		xml.OutOfElem();
		xml.FindElem("wild");	
		CString strConfig_wild;
		strConfig_wild = xml.GetAttrib("url");
		//����Ұ����������
		CString strConfig_wild_xml = GetAppPath(GAP_DIR)+"mcwild.xml";
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

		DeleteFile(strConfig_wild_xml);
		DeleteFile(strConfig_url_xml);				//ɾ��XML�ļ�
		WritePrivateProfileString("xmldata","config_url",strConfig_url,strQQPasture_ini);
		WritePrivateProfileString("xmldata","master",strMaster,strQQPasture_ini);
		WritePrivateProfileString("xmldata","farmui1",strFarmui1,strQQPasture_ini);
		WritePrivateProfileString("xmldata","farmui2",strFarmui2,strQQPasture_ini);
		WritePrivateProfileString("xmldata","commonui",strCommonui,strQQPasture_ini);
		WritePrivateProfileString("xmldata","config_wild",strConfig_wild,strQQPasture_ini);
	}
	else
	{
		//�����ͬ,��������ж�ȡ����
		GetPrivateProfileString("xmldata","master",NULL,strMaster.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQPasture_ini);
		strMaster.ReleaseBuffer();
		GetPrivateProfileString("xmldata","farmui1",NULL,strFarmui1.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQPasture_ini);
		strFarmui1.ReleaseBuffer();
		GetPrivateProfileString("xmldata","farmui2",NULL,strFarmui2.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQPasture_ini);
		strFarmui2.ReleaseBuffer();
		GetPrivateProfileString("xmldata","commonui",NULL,strCommonui.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQPasture_ini);
		strCommonui.ReleaseBuffer();
	}
	//�������������������,POST�ύʱ�����,��Ҫ��moduleԪ��ȡ����url��������
	strPostHeader = "Accept: */*\r\nReferer: " + strMaster + "\r\nx-flash-version: 10,2,152,32\r\nAccept-Language: zh-cn\r\nContent-Type: application/x-www-form-urlencoded\r\nAccept-Encoding\r\n";

M1:
	CString strPostData;	//����POST����
	strPostData.Format("0=%s&1=%s&2=%s&3=%s&newitem=2&farmKey=null&uIdx=",strMaster,strFarmui1,strFarmui2,strCommonui);
	szHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_enter",strPostData);
	
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
	H_GetPastureInfo(szHtml,pUserInfo);
	pUserInfo.QQ = atoi(GetQQInfo(GQI_NUM)); //��ȡһ��QQ
	CC_GetQQInfo(pUserInfo.Name,GQI_NAME);	//���ص�������ȡ�����ǳ�,ʹ�ù�������ȡһ��
	pUserInfo.IsQzone = TRUE;

	G_WriteUserInfo(pUserInfo);				//�洢�û����ݵ�Ӳ��
	S_ShowUserInfo((LPVOID)pUserInfo.Id);	//��ʾ�û���Ϣ������

	//����Ҫ����д������û����ݽṹ,�洢���ڴ���
	m_SelfInfo.Id = pUserInfo.Id;
	m_SelfInfo.Lvl = FarmExpToLevel(pUserInfo.Exp);
	m_SelfInfo.RipeTime = H_GetPastureLandRipe(pUserInfo);
	m_SelfInfo.CheckTime = dwServerTime;

	//����ҳ��XML�汾�ͱ��ش洢��XML�汾���жԱ�,��ͬ����¶�������
	if (strConfig_data != strConfig_data_ini)
	{
		CC_Addtolist("[QQ����]:���������б仯,��ʼ������������",COLOR_INFO);
		//����ֲ������XML������
		CString strConfig_data_xml = GetAppPath(GAP_DIR)+"AnimalGrow.xml";
		if (!GetUrlToFile(strConfig_data,strConfig_data_xml,TRUE))
			return FALSE;
		CMarkup xml;	//��ʼ����XML
		if (!xml.Load(strConfig_data_xml))
			return FALSE;
		xml.ResetMainPos();		//��������λ��,����������
		xml.FindElem("main");
		xml.IntoElem();
		xml.FindElem("animals");
		xml.IntoElem();

		Sleep(2000);
		//��ȡ�̵궯������,����XMLȱ�ٵ�����
		strPostData = "";
		strPostData.Format("uIdx=%d&farmKey=null",m_SelfInfo.Id);
		CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_get_animals",strPostData);

		int nOffset = 0,nIndex = 0;
		CString strBuf;

		while(xml.FindElem("animal"))
		{
			CString strItem;
			ST_ANIMALGROW pAnimalGrow;
			strBuf = xml.GetAttrib("id");
			pAnimalGrow.Id = atoi(strBuf);
			G_GetAnimalInfo(pAnimalGrow.Id,pAnimalGrow);

			//���̵������в����Ƿ��иö��������
			nOffset = strHtml.Find("\"cId\":"+strBuf+",",0);
			if (nOffset != -1)
			{
				//��ȡ�ö������Ϣ
				int nPos1,nPos2;
				nPos2 = strHtml.Find('}',nOffset);
				strItem = strHtml.Mid(0,nPos2);
				nPos1 = strItem.ReverseFind('{');
				strItem = strItem.Mid(nPos1,nPos2 - nPos1);
				//�ȼ�
				FindMidStr(strItem,0,"\"cLevel\":",",",strBuf);
				pAnimalGrow.Level = atoi(strBuf);
				//����
				FindMidStr(strItem,0,"\"cType\":",",",strBuf);
				pAnimalGrow.Type = atoi(strBuf);
				//����۸�
				FindMidStr(strItem,0,"\"byproductprice\":",",",strBuf);
				pAnimalGrow.byproductprice = atoi(strBuf);
				//���ﾭ��
				FindMidStr(strItem,0,"\"harvestbExp\":",",",strBuf);
				pAnimalGrow.harvestbExp = atoi(strBuf);
				//����۸�
				FindMidStr(strItem,0,"\"productprice\":",",",strBuf);
				pAnimalGrow.productprice = atoi(strBuf);
				//���ﾭ��
				FindMidStr(strItem,0,"\"harvestpExp\":",",",strBuf);
				pAnimalGrow.harvestpExp = atoi(strBuf);
				//����۸�
				FindMidStr(strItem,0,"\"price\":",",",strBuf);
				pAnimalGrow.buyprice = atoi(strBuf);
			}
			//����
			strcpy(pAnimalGrow.Name,xml.GetAttrib("name"));
			//�е�����
			strcpy(pAnimalGrow.Product,xml.GetAttrib("product"));
			//������λ
			strcpy(pAnimalGrow.Unit,xml.GetAttrib("u"));
			//����
			pAnimalGrow.output = atoi(xml.GetAttrib("childNum"));

			xml.IntoElem();
			xml.FindElem("nextTime");
			strItem = "";
			strItem = xml.GetAttrib("value");
			strItem += "\"";

			nIndex=0;
			nIndex=FindMidStr(strItem,nIndex,"",",",strBuf);
			pAnimalGrow.cropvalue[0]=atol(strBuf);
			nIndex=FindMidStr(strItem,nIndex,"",",",strBuf);
			pAnimalGrow.cropvalue[1]=atol(strBuf);
			nIndex=FindMidStr(strItem,nIndex,"",",",strBuf);
			pAnimalGrow.cropvalue[2]=atol(strBuf);
			nIndex=FindMidStr(strItem,nIndex,"",",",strBuf);
			pAnimalGrow.cropvalue[3]=atol(strBuf);
			nIndex=FindMidStr(strItem,nIndex,"",",",strBuf);
			pAnimalGrow.cropvalue[4]=atol(strBuf);
			nIndex=FindMidStr(strItem,nIndex,"","\"",strBuf);
			pAnimalGrow.cropvalue[5]=atol(strBuf);
			xml.OutOfElem();

			G_WriteAnimalInfo(pAnimalGrow);
		}

		DeleteFile(strConfig_data_xml);		//ɾ��XML�ļ�
		WritePrivateProfileString("xmldata","config_data",strConfig_data,strQQPasture_ini);
	}

	m_SelfLandId = m_SelfInfo.Id;
	S_ShowMyRipeTime();
	//����ˢ���ҵ������������ݶ�ʱ��
	SetTimer(hWndMain,IDT_SHOWMYRIPE,60000,NULL);
	//����ˢ���ҵĺ����б�������Ķ�ʱ��
	SetTimer(hWndMain,IDT_SHOWFRIENDSRIPE,60000,NULL);
	//����һ����������
	if (H_HandleMyPasture(NULL))
		S_ShowUserInfo((LPVOID)m_SelfLandId);

	if (szHtml.Find("\"d\":1,",0) != -1)	//���������ȡ
	{
		strPostData.Format("farmKey=null&uIdx=%d",m_SelfInfo.Id);
		N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_accept_gift",strPostData);
		CC_Addtolist("[QQ����]:��ȡ��ÿ�����",COLOR_INFO);
	}

	strPostData.Format("farmKey=null&uinY=%s&uIdx=%d",GetQQInfo(GQI_NUM),pUserInfo.Id);
	szHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_pasture_login_home",strPostData);
	Sleep(1000);
	int Num = 0;
	FindMidStr(szHtml,0,"\"number\":",",",strBuf);
	//���ƵĴ���
	Num = atoi(strBuf);
	//�������Ϊ��
	if (Num == 0)
	{
		szHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_pasture_login_click",strPostData);
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
			strPostData.Format("farmKey=null&uinY=%s&days=%d&flag=1&uIdx=%d",GetQQInfo(GQI_NUM),nDays,pUserInfo.Id);
			szHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_signin",strPostData);
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
				strPrintf.Format("[QQ����]:����ǩ��%d��,���%d��%s",nDays,pCardsGame.Num,pCardsGame.Name);
				Addtolist(strPrintf,COLOR_INFO);
			}
		}
		int nNum = 1;
		if (nYellowLevel > 0)
			nNum++;
		for (int i=0;i<nNum-nNumber;i++)
		{
			strPostData.Format("pid=%d&flag=2&uIdx=%d&yellow=%d&uinY=%s",i,pUserInfo.Id,nYellowLevel>0 ? 1:0,GetQQInfo(GQI_NUM));
			szHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_signin",strPostData);
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
				strPrintf.Format("[QQ����]:�μ����˾Ź���,���%d��%s",pCardsGame.Num,pCardsGame.Name);
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
				CC_Addtolist("[QQ����]:��ȡ��������Э��",COLOR_INFO);
				while(!N_GetBlogData(NULL)){Sleep(5000);};
				Sleep(3000);
			}
			if (bGetServerTime)
			{
				bGetServerTime = FALSE;
				//��ȡ������ʱ��
				CC_Addtolist("[QQ����]:��ȡ����������ʱ��",COLOR_INFO);
				while(!pGetServerTime()){Sleep(5000);};
				Sleep(3000);
			}
			if (bGetXmlData)
			{
				bGetXmlData = FALSE;
				//��������ҳ���ȡ������Դ
				CC_Addtolist("[QQ����]:��鶯�������Ƿ���Ҫ����",COLOR_INFO);
				while(!N_GetXmlData(NULL)){Sleep(5000);};
				Sleep(3000);
			}
			//��ȡ�����б�
			if (bGetFriends)	
			{
				bGetFriends = FALSE;
				CC_Addtolist("[QQ����]:��ȡ���������б�",COLOR_INFO);
				F_GetFriends(NULL);
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
				Sleep(3000);
			}
			//ɨ���������
			if (bScanFriends)	
			{
				bScanFriends = FALSE;
				CC_Addtolist("[QQ����]:��ʼ�������ѵ���������",COLOR_INFO);
				for (int i=0;i<v_FriendsInfo.size();i++)	//ѭ����ȡ���׺����б����� 
				{
					if (v_FriendsInfo[i].Lvl < m_Config.nIgnoreFdLvl)
						continue;
					ST_USERINFO pUserInfo;					//��INI�ļ��ж�����ȡ������
					G_GetUserInfo(v_FriendsInfo[i].Id,pUserInfo);
					CString strText;
					strText.Format("���ڷ�������[%s]������",pUserInfo.Name);
					SetDlgItemText(hWndMain,IDC_MAIN_STATETEXT,strText);	//���ô���״̬��ǩ
					strText.Format("%d/%d",i+1,v_FriendsInfo.size());		//���ý���
					SetDlgItemText(hWndMain,IDC_MAIN_SCANPOS,strText);
					BOOL bSleep = F_ScanPastureInfo((LPVOID)v_FriendsInfo[i].Id);	//��ȡ��������,���Զ�������INI�ļ�,�����INI�ж����򷵻�FALSE
					if (bSleep)
						H_HandleFriendPasture((LPVOID)v_FriendsInfo[i].Id);		///����Ǵ������ȡ���������������
					G_GetUserInfo(v_FriendsInfo[i].Id,pUserInfo);			//��INI�ļ��ٴζ�������
					v_FriendsInfo[i].RipeTime = H_GetPastureLandRipe(pUserInfo); //��ȡ������������ʱ��,û���򷵻�0
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
				m_Config.bHarvest)
			{
				m_SelfInfo.RipeTime = 0;
				CC_Addtolist("[QQ����]:��ʼˢ���ҵ�����",COLOR_INFO);
				ST_USERINFO pUserInfo;	//��INI�ļ��ж�����ȡ������
				G_GetUserInfo(m_SelfInfo.Id,pUserInfo);
				if (H_CountPasture(pUserInfo) || bGetMyPasture)
				{
					bGetMyPasture = FALSE;
					F_GetPastureInfo((LPVOID)m_SelfInfo.Id);
					Sleep(1000);
					H_HandleMyPasture(NULL);
				}
				G_GetUserInfo(m_SelfInfo.Id,pUserInfo);
				m_SelfInfo.Lvl = FarmExpToLevel(pUserInfo.Exp);
				m_SelfInfo.RipeTime = H_GetPastureLandRipe(pUserInfo);
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
					m_Config.bFdSteal)
				{
					v_FriendsInfo[i].RipeTime = 0;
					ST_USERINFO pUserInfo;	//��INI�ļ��ж�����ȡ������
					G_GetUserInfo(v_FriendsInfo[i].Id,pUserInfo);
					if (H_CountPasture(pUserInfo))
					{
						F_GetPastureInfo((LPVOID)v_FriendsInfo[i].Id);
						Sleep(1000);
						H_HandleFriendPasture((LPVOID)v_FriendsInfo[i].Id);
					}
					G_GetUserInfo(v_FriendsInfo[i].Id,pUserInfo);
					v_FriendsInfo[i].RipeTime = H_GetPastureLandRipe(pUserInfo);
					v_FriendsInfo[i].CheckTime = dwServerTime;				//��¼һ�´����ʱ��
					G_WriteUserList(v_FriendsInfo[i]);
					S_ShowFriendsRipeTimeOnPos(i);							//��ʾһ�¾���컹�ж��,û�����
					Sleep(3000);
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
//�������ܺ���															//
//////////////////////////////////////////////////////////////////////////
UINT H_GetPastureInfo(CString &strHtml,ST_USERINFO &pUserInfo)
{
	//���һ��������
	ST_USERINFO pClrUserInfo;
	memcpy(&pUserInfo.AnimalLand,&pClrUserInfo.AnimalLand,sizeof(pClrUserInfo.AnimalLand));
	
	CString strBuf;	//��ʱ�ı�
	
		//����UID
		if (FindMidStr(strHtml,0,"\"uId\":",",",strBuf) != -1)
		{
			pUserInfo.Id=atoi(strBuf);
			//����QQ
			//if(FindMidStr(strHtml,0,"\"uin\":",",",strBuf) != -1)
			//pUserInfo.QQ=atoi(strBuf);
			//���ҽ��
			if(FindMidStr(strHtml,0,"\"money\":",",",strBuf) != -1 && strBuf != "0")
				pUserInfo.Money=atol(strBuf);
			//���Ҿ���
			if(FindMidStr(strHtml,0,"\"exp\":",",",strBuf) != -1)
			pUserInfo.Exp=atol(strBuf);
		}
		//��������
		if(FindMidStr(strHtml,0,"\"animalFood\":",",",strBuf) != -1)
		pUserInfo.animalFood=atoi(strBuf);
		//�����ѵȼ�
		if(FindMidStr(strHtml,0,"\"id\":102,\"lv\":","}",strBuf) != -1)
		pUserInfo.wlvl=atoi(strBuf);
		//������ȼ�
		if(FindMidStr(strHtml,0,"\"id\":103,\"lv\":","}",strBuf) != -1)
		pUserInfo.plvl=atoi(strBuf);
	
		//���һ���ȼ�
		/*if (if(FindMidStr(strHtml,0,"\"yellowlevel\":",",",strBuf) == 1)
			return FALSE;
		pUserInfo.YellowLevel=atoi(strBuf) != -1)
		//���һ���״̬
		if (if(FindMidStr(strHtml,0,"\"yellowstatus\":",",",strBuf) == 1)
			return FALSE;
		pUserInfo.YellowStatus=atoi(strBuf) != -1)*/
	
		//���ҿ���
		if(FindMidStr(strHtml,0,"\"striketime\"","}",strBuf)==-1)
			pUserInfo.guard = -1;
		else
			pUserInfo.guard=atoi(strBuf);
	
		//͵ȡ��־
		CString strStealFlags,strStealFlag;
		if(FindMidStr(strHtml,0,"\"stealflag\":{","}",strStealFlags) != -1)
			strStealFlags += ",";
	
		//��ն�������
		int wNum,pNum;
		PastureHouseNum(pUserInfo.wlvl,pUserInfo.plvl,wNum,pNum);
		for (int i=0;i<wNum+pNum;i++)
		{
			pUserInfo.AnimalLand[i].Id = 0;
			pUserInfo.AnimalLand[i].changeTime = 0;
		}
		//��ȡ��������
		int nOffset = 0;
		CString strAnimals,strAnimal;
		if(FindMidStr(strHtml,0,"\"animal\":[","]",strAnimals) != -1)
		{
			nOffset=FindMidStr(strAnimals,nOffset,"{","}",strAnimal);
			while(nOffset!=-1)
			{
				if (FindMidStr(strAnimal,0,"\"serial\":",",",strBuf) != -1)
				{
					int nCount = -1;
					for (int i=0;i<20;i++)
					{
						if (pUserInfo.AnimalLand[i].Id == 0)
						{
							nCount = i;
							break;
						}
					}
					if (nCount == -1)
						break;
					pUserInfo.AnimalLand[nCount].serial = atoi(strBuf);
					
					//ID
					if (FindMidStr(strAnimal,0,"\"cId\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].Id = atoi(strBuf);
					//�Ƿ��͵ȡ
					strStealFlag.Format("\"%s\":",strBuf);
					if (FindMidStr(strStealFlags,0,strStealFlag,",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].stealflag = atoi(strBuf);
					else
					pUserInfo.AnimalLand[nCount].stealflag = 3;
					//�ѻ��ѵ�����ʱ��
					if (FindMidStr(strAnimal,0,"\"growTime\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].growTime = atoi(strBuf);
					//������һ״̬��Ҫ��ʱ��
					if (FindMidStr(strAnimal,0,"\"growTimeNext\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].growTimeNext = atoi(strBuf);
					//�Ƿ񼢶�
					if (FindMidStr(strAnimal,0,"\"hungry\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].hungry = atoi(strBuf);
					//��ǰ״̬
					if (FindMidStr(strAnimal,0,"\"status\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].status = atoi(strBuf);
					//��һ״̬
					if (FindMidStr(strAnimal,0,"\"statusNext\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].statusNext = atoi(strBuf);
					//��������
					if (FindMidStr(strAnimal,0,"\"totalCome\":","",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].totalCome = atoi(strBuf);
					//���������,�ͼ���һ�³ɳ�ʱ��
					if (pUserInfo.AnimalLand[nCount].hungry == FALSE && pUserInfo.AnimalLand[nCount].status < 6)
					{
						//���������һ�׶ε�ʱ��
						pUserInfo.AnimalLand[nCount].changeTime = pUserInfo.AnimalLand[nCount].growTimeNext + dwServerTime;
					}
				}
							
				if (nOffset == -1)
					break;
				//������һ��ѭ��
				nOffset=FindMidStr(strAnimals,nOffset,"{","}",strAnimal);
			}
		}
	
		CString strBadInfos,strBadInfo;
		//���Һ����������
		if (FindMidStr(strHtml,0,"\"badinfo\":[","]",strBadInfos) != -1)
		{
			//��ȡ��������
			nOffset=FindMidStr(strBadInfos,0,"{","}",strBadInfo);
			if (FindMidStr(strBadInfo,0,"\"num\":",",",strBuf) != -1)
			pUserInfo.mosquito = atoi(strBuf);
			//��ȡ�������
			nOffset=FindMidStr(strBadInfos,nOffset,"{","}",strBadInfo);
			if (FindMidStr(strBadInfo,0,"\"num\":",",",strBuf) != -1)
			pUserInfo.shit = atoi(strBuf);
		}
	
		for (i=0;i<3;i++)
		{
			pUserInfo.BeastLand[i].fId = 0;
			pUserInfo.BeastLand[i].Type = 0;
		}
		nOffset = 0;
		CString strBeasts,strBeast,strAttack;
		if (FindMidStr(strHtml,0,"\"beast\":","]}",strBeasts) != -1)
		{
			while((nOffset = FindMidStr(strBeasts,nOffset,"\"attack\":[","]",strAttack)) != -1)
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
	
				nOffset = FindMidStr(strBeasts,nOffset,",","}",strBeast);
				//����QQ
				if (FindMidStr(strBeast,0,"\"fid\":",",",strBuf) != -1)
					pUserInfo.BeastLand[nCount].fId = atoi(strBuf);
				//����ID
				if (FindMidStr(strBeast,0,"\"id\":",",",strBuf) != -1)
					pUserInfo.BeastLand[nCount].Id = atoi(strBuf);
				//��������
				if (FindMidStr(strBeast,0,"\"type\":","",strBuf) != -1)
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
				if (strAttack.Find(strFindQQ,0) == -1 && pUserInfo.BeastLand[nCount].fId != atoi(GetQQInfo(GQI_NUM)))
					pUserInfo.BeastLand[nCount].bAttack = TRUE;
				
				if (nOffset == -1)
					break;
			}
		}
	return TRUE;
}

BOOL C_RipeTimeCmp(DWORD dwMin,DWORD dwMax)
{
	return dwMin > dwMax;
}
DWORD H_GetPastureLandRipe(ST_USERINFO &pUserInfo)
{
	DWORD dwResultTime = 0;
	std::vector <DWORD> v_ripe;
	for (int i=0;i<20;i++)
	{
		if (pUserInfo.AnimalLand[i].Id > 0 && pUserInfo.AnimalLand[i].status < 6 && pUserInfo.AnimalLand[i].hungry == FALSE && pUserInfo.AnimalLand[i].changeTime > 0)
		{
			v_ripe.push_back(pUserInfo.AnimalLand[i].changeTime);
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

//������������״̬
UINT H_CountPasture(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	int wNum,pNum;
	PastureHouseNum(pUserInfo.wlvl,pUserInfo.plvl,wNum,pNum);
	ST_ANIMALGROW pAnimalGrow;
	for (int i=0;i<wNum+pNum;i++)
	{
		if (pUserInfo.AnimalLand[i].Id > 0 &&	//ID��Ϊ��
			pUserInfo.AnimalLand[i].hungry == FALSE &&	//���ﲻ����
			pUserInfo.AnimalLand[i].changeTime > 0 &&	//�ɳ�ʱ�䲻Ϊ��
			pUserInfo.AnimalLand[i].changeTime <= dwServerTime)	//ʱ���Ѿ�����
		{
			//��ȡ��������
			if (pAnimalGrow.Id != pUserInfo.AnimalLand[i].Id)
				G_GetAnimalInfo(pUserInfo.AnimalLand[i].Id,pAnimalGrow);
			//�Ƿ��Ѿ������ջ���
			if (pUserInfo.AnimalLand[i].status < 6)
			{
				//����ɳ�����һ�׶�
				pUserInfo.AnimalLand[i].status = pUserInfo.AnimalLand[i].statusNext;
				//���Ӷ����Ѿ�����ʱ��
				pUserInfo.AnimalLand[i].growTime += pUserInfo.AnimalLand[i].growTimeNext;
				//����Ѿ���������
				if (pUserInfo.AnimalLand[i].status == 6 && pUserInfo.Id == m_SelfInfo.Id)
					nResult = TRUE;
				//������һ�׶���Ҫ������ʱ��
				pUserInfo.AnimalLand[i].growTimeNext = pAnimalGrow.cropvalue[pUserInfo.AnimalLand[i].status - 1] - 
					(dwServerTime - pUserInfo.AnimalLand[i].changeTime);
				
				//�����������̵��ɳ���,����һ�׶�����Ϊ������
				if (pUserInfo.AnimalLand[i].status == 2)
					pUserInfo.AnimalLand[i].statusNext = 3;
				//������ﵽ��������,�򷵻�TRUE
				if (pUserInfo.AnimalLand[i].status == 3)
					nResult = TRUE;
			}
		}
	}
	G_WriteUserInfo(pUserInfo);
	return nResult;
}

//�������
UINT F_PastureAddFood(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	if (pUserInfo.animalFood < m_Config.nMinFood)
	{
		F_GetUserPackage(NULL);
		Sleep(2000);
		//��Ҫ���Ӷ�������
		int nNeedNum = m_Config.nMaxFood - pUserInfo.animalFood;
		//���������ݵ�����
		int nPackFood = 0;
		int nBuyFood = 0;
		
		if (m_Pastures >= nNeedNum)
		{
			nPackFood = nNeedNum;
		}
		else
		{
			if (m_Pastures > 0)
			{
				nPackFood = m_Pastures;
				nNeedNum -= nPackFood;
			}
			while((nNeedNum*60) > pUserInfo.Money)
				nNeedNum--;
			nBuyFood = nNeedNum;
		}
		
		//�ӱ����������
		if (nPackFood > 0)
		{
			CString strPostData;
			strPostData.Format("type=0&foodnum=%d&uIdx=%d&farmKey=null",nPackFood,m_SelfInfo.Id);
			CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_feed_food",strPostData);
			CString strBuf;
			//�Ƿ�ɹ�
			int nCode = 0;
			//�ɹ���ӵ�����
			int nAdded = 0;
			FindMidStr(strHtml,0,"\"code\":",",",strBuf);
			nCode = atoi(strBuf);
			if (nCode == TRUE)
			{
				FindMidStr(strHtml,0,"\"added\":",",",strBuf);
				nAdded = atoi(strBuf);
				FindMidStr(strHtml,0,"\"total\":",",",strBuf);
				//д���µ���������
				pUserInfo.animalFood = atoi(strBuf);
				CString strPrintf;
				strPrintf.Format("[QQ����]:�ӱ��������%d��������Ϊ����",nAdded);
				Addtolist(strPrintf,COLOR_INFO);
				H_GetPastureInfo(strHtml,pUserInfo);
				nResult = TRUE;
			}
			Sleep(2000);
			m_Pastures -= nAdded;
		}
		//���̵깺������
		if (nBuyFood > 0)
		{
			CString strPostData;
			strPostData.Format("type=1&foodnum=%d&uIdx=%d&farmKey=null",nBuyFood,m_SelfInfo.Id);
			CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_feed_food",strPostData);
			CString strBuf;
			//�ɹ���ӵ�����
			int nAdded = nBuyFood;
			//���ѵĽ��
			int nMoney = 0;
			FindMidStr(strHtml,0,"\"money\":",",",strBuf);
			nMoney = atoi(strBuf);
			//��һ�½��
			pUserInfo.Money -= nMoney;
			FindMidStr(strHtml,0,"\"total\":",",",strBuf);
			//д���µ���������
			pUserInfo.animalFood = atoi(strBuf);
			CString strPrintf;
			strPrintf.Format("[QQ����]:������%d��������Ϊ����,��������%d�����",nAdded,nMoney);
			Addtolist(strPrintf,COLOR_INFO);
			H_GetPastureInfo(strHtml,pUserInfo);
			nResult = TRUE;
			Sleep(2000);
		}
		G_WriteUserInfo(pUserInfo);
	}
	return nResult;
}
//�ջ�����������
UINT F_PastureHarvest1(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	std::vector<int> v_Steal;
		ST_ANIMALGROW pAnimalGrow;
		std::vector<LONG> v_CrowValue;
		//�Ƿ���ͣ15��,�ȴ���������
		BOOL bSleep = FALSE;
		int wNum,pNum;
		PastureHouseNum(pUserInfo.wlvl,pUserInfo.plvl,wNum,pNum);
		for (int i=0;i<wNum+pNum;i++)
		{
			if (pUserInfo.AnimalLand[i].Id > 0 &&
				pUserInfo.AnimalLand[i].hungry == FALSE &&
				pUserInfo.AnimalLand[i].status == 3)
			{
				CString strPostData;
				strPostData.Format("farmKey=null&uIdx=%d&serial=%d",m_SelfInfo.Id,pUserInfo.AnimalLand[i].serial);
				CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_post_product",strPostData);
				CString strBuf;
				//�Ƿ�ɹ�
				int nCode = 0;
				//���ӵľ���
				int nAddExp = 0;
				FindMidStr(strHtml,0,"\"code\":",",",strBuf);
				nCode = atoi(strBuf);
				FindMidStr(strHtml,0,"\"addExp\":",",",strBuf);
				nAddExp = atoi(strBuf);
				//�׶�
				FindMidStr(strHtml,0,"\"status\":",",",strBuf);
				pUserInfo.AnimalLand[i].status = atoi(strBuf);
				//����
				pUserInfo.AnimalLand[i].totalCome = 0;
				//���ʱ��
				FindMidStr(strHtml,0,"\"growTime\":",",",strBuf);
				pUserInfo.AnimalLand[i].growTime = atoi(strBuf);
				//��һ�׶�
				FindMidStr(strHtml,0,"\"statusNext\":",",",strBuf);
				pUserInfo.AnimalLand[i].statusNext = atoi(strBuf);
				//��һ�׶�ʱ��
				FindMidStr(strHtml,0,"\"growTimeNext\":",",",strBuf);
				pUserInfo.AnimalLand[i].growTimeNext = atoi(strBuf);
				//�Ƿ񼢶�
				FindMidStr(strHtml,0,"\"hungry\":",",",strBuf);
				pUserInfo.AnimalLand[i].hungry = atoi(strBuf);
				//�޸�͵ȡ��־
				pUserInfo.AnimalLand[i].stealflag = 0;
				if (pUserInfo.AnimalLand[i].hungry == FALSE)
				{
					//���������һ�׶ε�ʱ��
					pUserInfo.AnimalLand[i].changeTime = pUserInfo.AnimalLand[i].growTimeNext + dwServerTime;
				}
				//����ɹ�
				if (nCode == TRUE)
				{
					if (pAnimalGrow.Id != pUserInfo.AnimalLand[i].Id)
					{
						G_GetAnimalInfo(pUserInfo.AnimalLand[i].Id,pAnimalGrow);
						v_CrowValue.clear();	//��ն�̬����
						v_CrowValue.push_back(pAnimalGrow.cropvalue[0]);	//����
						v_CrowValue.push_back(pAnimalGrow.cropvalue[1]);	//�ɳ���
						int nValue = pAnimalGrow.cropvalue[3] + pAnimalGrow.cropvalue[4];	//�����ڼ��
						int nNum = pAnimalGrow.cropvalue[2] / nValue;		//��������
						for (int j=0;j<nNum;j++)
						{
							v_CrowValue.push_back(nValue);					//ѭ������������ʱ��
						}
					}
					CString strPrintf;
					strPrintf.Format("[QQ����]:���Լ�%d��λ��%s��ȥ����",pUserInfo.AnimalLand[i].serial,pAnimalGrow.Name);
					if (nAddExp > 0)
					{
						CString strAddExp;
						strAddExp.Format(",�����%d�㾭��",nAddExp);
						strPrintf += strAddExp;
					}
					Addtolist(strPrintf,COLOR_INFO);
					bSleep = TRUE;
				}
				Sleep(600);
			}
	
			if (pUserInfo.AnimalLand[i].Id > 0 &&
				pUserInfo.AnimalLand[i].status == 4)
			{
				pUserInfo.AnimalLand[i].totalCome = 1;
				pUserInfo.AnimalLand[i].stealflag = 3;
				pUserInfo.AnimalLand[i].status = 5;
				if (pAnimalGrow.Id != pUserInfo.AnimalLand[i].Id)
				{
					G_GetAnimalInfo(pUserInfo.AnimalLand[i].Id,pAnimalGrow);
					v_CrowValue.clear();	//��ն�̬����
					v_CrowValue.push_back(pAnimalGrow.cropvalue[0]);	//����
					v_CrowValue.push_back(pAnimalGrow.cropvalue[1]);	//�ɳ���
					int nValue = pAnimalGrow.cropvalue[3] + pAnimalGrow.cropvalue[4];	//�����ڼ��
					int nNum = pAnimalGrow.cropvalue[2] / nValue;		//��������
					for (int j=0;j<nNum;j++)
					{
						v_CrowValue.push_back(nValue);					//ѭ������������ʱ��
					}
				}
				int nValue = 0;
				int nStatus = 0;
				for (int j=0;j<v_CrowValue.size();j++)
				{
					nValue += v_CrowValue[j];
					if (pUserInfo.AnimalLand[i].growTime < nValue)
					{
						nStatus = j + 1;
						break;
					}
				}
				if (nStatus == v_CrowValue.size())
				{
					pUserInfo.AnimalLand[i].statusNext = 6;
					pUserInfo.AnimalLand[i].growTimeNext = pAnimalGrow.cropvalue[5] - pUserInfo.AnimalLand[i].growTime;
				}
				else
				{
					pUserInfo.AnimalLand[i].statusNext = 3;
					pUserInfo.AnimalLand[i].growTimeNext = pAnimalGrow.cropvalue[4] + pAnimalGrow.cropvalue[3];
				}
				if (pUserInfo.AnimalLand[i].hungry == FALSE)
				{
					//���������һ�׶ε�ʱ��
					pUserInfo.AnimalLand[i].changeTime = pUserInfo.AnimalLand[i].growTimeNext + dwServerTime;
				}
				bSleep = TRUE;
			}
	
			//�����ջ������ID���붯̬����
			if (pUserInfo.AnimalLand[i].Id > 0 &&			//���ID������
				pUserInfo.AnimalLand[i].totalCome > 0 &&	//����������
				pUserInfo.AnimalLand[i].stealflag == 3)		//͵ȡ��ʶΪ3
			{
				pUserInfo.AnimalLand[i].totalCome = 0;
				pUserInfo.AnimalLand[i].stealflag = 0;
				BOOL bSteal = FALSE;	//�Ƿ��Ѵ����ջ�ID������
				for (int j=0;j<v_Steal.size();j++)
				{
					if (v_Steal[j] == pUserInfo.AnimalLand[i].Id)	//ѭ�������������Ƿ��Ѵ�����ͬID
					{
						bSteal = TRUE;
						break;
					}
				}
				if (!bSteal)			//�����������ID��������
					v_Steal.push_back(pUserInfo.AnimalLand[i].Id);
			}
		}
	
		if (bSleep)
			Sleep(16000);
	
		if (v_Steal.size() > 0)
		{
			CString strPostData;
			strPostData.Format("version=1&harvesttype=1&farmKey=null&uIdx=%d&type=",m_SelfInfo.Id);
			strPostData += "%2D1";
			CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_harvest_product",strPostData);
			CString strBuf;
			//���ӵľ���
			int nAddExp = 0;
			FindMidStr(strHtml,0,"[",",[",strBuf);
			nAddExp = atoi(strBuf);
			CString strPrintf;
			for (i=0;i<v_Steal.size();i++)
			{
				CString strFind;
				strFind.Format("[%d,",v_Steal[i]);
				if (FindMidStr(strHtml,0,strFind,"]",strBuf) != -1)
				{
					if (pAnimalGrow.Id != v_Steal[i])
						G_GetAnimalInfo(v_Steal[i],pAnimalGrow);
					strPrintf.Format("[QQ����]:�ջ����Լ��ҵ�%s%s%s",strBuf,pAnimalGrow.Unit,pAnimalGrow.Product);
					Addtolist(strPrintf,COLOR_INFO);
				}
			}
			if (nAddExp > 0)
			{
				strPrintf.Format("[QQ����]:�����%d�㾭��",nAddExp);
				Addtolist(strPrintf,COLOR_INFO);
				pUserInfo.Exp += nAddExp;
			}
			nResult = TRUE;
		}
		if (!v_Steal.empty())
			v_Steal.clear();
		if (!v_CrowValue.empty())
			v_Steal.clear();
	G_WriteUserInfo(pUserInfo);
	return nResult;
}
//�ջ��ѳ���Ķ���
UINT F_PastureHarvest2(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	std::vector<int> v_Steal;
	ST_ANIMALGROW pAnimalGrow;
	int wNum,pNum;
	PastureHouseNum(pUserInfo.wlvl,pUserInfo.plvl,wNum,pNum);
	for (int i=0;i<wNum+pNum;i++)
	{
		if (pUserInfo.AnimalLand[i].Id > 0 &&
			pUserInfo.AnimalLand[i].status == 6)
		{
			v_Steal.push_back(i);
		}
	}
	if (v_Steal.size() > 0)
	{
		CString strPostData;
		strPostData.Format("version=1&harvesttype=2&serialIndex=%d&farmKey=null&uIdx=%d&serial=",v_Steal[0]+1,m_SelfInfo.Id);
		strPostData += "%2D1";
		CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_harvest_product",strPostData);
		CString strBuf;
		//���ӵľ���
		int nAddExp = 0;
		FindMidStr(strHtml,0,"[",",[",strBuf);
		nAddExp = atoi(strBuf);
		CString strPrintf;
		for (i=0;i<v_Steal.size();i++)
		{
			if (pAnimalGrow.Id != pUserInfo.AnimalLand[v_Steal[i]].Id)
				G_GetAnimalInfo(pUserInfo.AnimalLand[v_Steal[i]].Id,pAnimalGrow);
			strPrintf.Format("[QQ����]:�ջ����Լ���%d��λ��%s",pUserInfo.AnimalLand[v_Steal[i]].serial,pAnimalGrow.Name);
			Addtolist(strPrintf,COLOR_INFO);
			pUserInfo.AnimalLand[v_Steal[i]].Id = 0;
			pUserInfo.AnimalLand[v_Steal[i]].changeTime = 0;
			pUserInfo.AnimalLand[v_Steal[i]].status = 0;
		}
		if (nAddExp > 0)
		{
			strPrintf.Format("[QQ����]:�����%d�㾭��",nAddExp);
			Addtolist(strPrintf,COLOR_INFO);
			pUserInfo.Exp += nAddExp;
		}
		G_WriteUserInfo(pUserInfo);
		nResult = TRUE;
	}
	if (!v_Steal.empty())
		v_Steal.clear();
	return nResult;
}
BOOL C_AnimalsLvlCmp(ST_ANIMALGROW pAnimalGrow1,ST_ANIMALGROW pAnimalGrow2)
{
	return pAnimalGrow1.Level < pAnimalGrow2.Level;
}
//���ع������ID
UINT H_GetBuyAnimalId(BOOL bWorP,BOOL &bIsRaise)
{
	UINT nAnimalId = 0;
	std::vector<ST_ANIMALGROW> v_Animals;
	std::vector<ST_ANIMALGROW> v_HaveAnimals;  //�������еĶ���
	char szAnimals[SHORTUSERLEN]={0};
	char szAnimal[SHORTLEN]={0};
	//ȡ����Section��,�����ַ�����0��β,�����ַ�����00��β
	GetPrivateProfileSectionNames(szAnimals,SHORTUSERLEN,strAnimalsIni);
	int nPos = 0;
	while(szAnimals[nPos] != 0)
	{
		strcpy(szAnimal,szAnimals+nPos);	//�õ�������Section
		ST_ANIMALGROW pAnimalGrow;
		G_GetAnimalInfo(atoi(szAnimal),pAnimalGrow);	//��ȡ���Section������
		if (pAnimalGrow.buyprice > 0)
		{
			if (bWorP)
			{
				if (pAnimalGrow.Id < 1500)
					v_Animals.push_back(pAnimalGrow);
			}
			else
			{
				if (pAnimalGrow.Id > 1500)
					v_Animals.push_back(pAnimalGrow);
			}
		}
		if (pAnimalGrow.nPackage > 0)
		{
			if (bWorP)
			{
				if (pAnimalGrow.Id < 1500)
					v_HaveAnimals.push_back(pAnimalGrow);
			}
			else
			{
				if (pAnimalGrow.Id > 1500)
					v_HaveAnimals.push_back(pAnimalGrow);
			}
		}
		nPos += strlen(szAnimal) + 1;
	}
	std::sort(v_Animals.begin(),v_Animals.end(),C_AnimalsLvlCmp);	//���ݵȼ�����
	if (m_Config.nBuyAnimalType == 0)
	{
		if (m_SelfInfo.Lvl < v_Animals[v_Animals.size()-1].Level)
		{
			for (int i=0;i<v_Animals.size();i++)
			{
				if (m_SelfInfo.Lvl == v_Animals[i].Level)
				{
					nAnimalId = v_Animals[i].Id;
					break;
				}
				if (m_SelfInfo.Lvl < v_Animals[i].Level)
				{
					nAnimalId = v_Animals[i-1].Id;
					break;
				}
			}
		}
		else
		{
			nAnimalId = v_Animals[v_Animals.size()-1].Id;
		}
	}
	else
	{
		if (bWorP)
			nAnimalId = m_Config.nWAnimalId;
		else
			nAnimalId = m_Config.nPAnimalId;
	}

	if (m_Config.nRaiseAnimalType == 0 && v_HaveAnimals.size() > 0)
	{
		for (int i=0;i<v_HaveAnimals.size();i++)
		{
			nAnimalId = v_HaveAnimals[i].Id;
			break;
		}
		bIsRaise = TRUE;
	}
	else
	{
		bIsRaise = FALSE;
	}

	ST_ANIMALGROW pAnimalGrow;
	G_GetAnimalInfo(nAnimalId,pAnimalGrow);
	if (pAnimalGrow.nPackage > 0)
	{
		pAnimalGrow.nPackage--;
		G_WriteAnimalInfo(pAnimalGrow);
	}

	if (!v_Animals.empty())
		v_Animals.clear();
	if (!v_HaveAnimals.empty())
		v_HaveAnimals.clear();

	return nAnimalId;
}
//��ȡ�û���������
UINT H_GetAnimalsInfo(CString &strHtml,ST_USERINFO &pUserInfo)
{
	//��ȡ��������
	int nOffset = 0;
	CString strAnimals,strAnimal,strBuf;
	if(FindMidStr(strHtml,0,"\"animal\":[","]",strAnimals) != -1)
	{
		nOffset=FindMidStr(strAnimals,nOffset,"{","}",strAnimal);
		while(nOffset!=-1)
		{
			if (FindMidStr(strAnimal,0,"\"serial\":",",",strBuf) != -1)
			{
				int nCount = -1;
				for (int i=0;i<20;i++)
				{
					if (pUserInfo.AnimalLand[i].Id == 0)
					{
						nCount = i;
						break;
					}
				}
				if (nCount == -1)
					break;
				pUserInfo.AnimalLand[nCount].serial = atoi(strBuf);
				
				//ID
				if (FindMidStr(strAnimal,0,"\"cId\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].Id = atoi(strBuf);
				//�Ƿ��͵ȡ
				pUserInfo.AnimalLand[nCount].stealflag = 3;
				//�ѻ��ѵ�����ʱ��
				if (FindMidStr(strAnimal,0,"\"growTime\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].growTime = atoi(strBuf);
				//������һ״̬��Ҫ��ʱ��
				if (FindMidStr(strAnimal,0,"\"growTimeNext\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].growTimeNext = atoi(strBuf);
				//�Ƿ񼢶�
				if (FindMidStr(strAnimal,0,"\"hungry\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].hungry = atoi(strBuf);
				//��ǰ״̬
				if (FindMidStr(strAnimal,0,"\"status\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].status = atoi(strBuf);
				//��һ״̬
				if (FindMidStr(strAnimal,0,"\"statusNext\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].statusNext = atoi(strBuf);
				//��������
				if (FindMidStr(strAnimal,0,"\"totalCome\":","",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].totalCome = atoi(strBuf);
				//���������,�ͼ���һ�³ɳ�ʱ��
				if (pUserInfo.AnimalLand[nCount].hungry == FALSE && pUserInfo.AnimalLand[nCount].status < 6)
				{
					//���������һ�׶ε�ʱ��
					pUserInfo.AnimalLand[nCount].changeTime = pUserInfo.AnimalLand[nCount].growTimeNext + dwServerTime;
				}
			}
			
			if (nOffset == -1)
				break;
			//������һ��ѭ��
			nOffset=FindMidStr(strAnimals,nOffset,"{","}",strAnimal);
		}
	}
	return TRUE;
}
//������
UINT F_PastureBuyAnimal(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	if (m_Config.nRaiseAnimalType == 0)
		F_GetUserAnimals(NULL);
	//��Ҫ����Ķ�������
	int wNum,pNum;
	PastureHouseNum(pUserInfo.wlvl,pUserInfo.plvl,wNum,pNum);
	int nBuyWnum = wNum,nBuyPnum = pNum;
	for (int i=0;i<wNum+pNum;i++)
	{
		if (pUserInfo.AnimalLand[i].Id > 0 && pUserInfo.AnimalLand[i].Id < 1500)
			nBuyWnum--;
		if (pUserInfo.AnimalLand[i].Id > 0 && pUserInfo.AnimalLand[i].Id > 1500)
			nBuyPnum--;
	}
	if (nBuyWnum > 0)
	{
M1:
		//��ȡ�������ID
		int nBuyId = 0;
		BOOL bIsRaise = FALSE;
		nBuyId = H_GetBuyAnimalId(TRUE,bIsRaise);
		if (nBuyId > 0)
		{
			ST_ANIMALGROW pAnimalGrow;
			G_GetAnimalInfo(nBuyId,pAnimalGrow);
			if (bIsRaise == TRUE)
			{
				CString strPostData;
				strPostData.Format("number=1&farmKey=null&type=%d&uIdx=%d",nBuyId,m_SelfInfo.Id);
				CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_raise_cub",strPostData);
				CString strBuf;
				//���ӵľ���
				int nAddExp = 0;
				FindMidStr(strHtml,0,"\"addExp\":",",",strBuf);
				nAddExp = atoi(strBuf);
				H_GetAnimalsInfo(strHtml,pUserInfo);
				CString strPrintf;
				strPrintf.Format("[QQ����]:������1ֻ%s(��)",pAnimalGrow.Name);
				if (nAddExp > 0)
				{
					CString strAddExp;
					strAddExp.Format(",�����%d�㾭��",nAddExp);
					strPrintf += strAddExp;
				}
				Addtolist(strPrintf,COLOR_INFO);
				nResult = TRUE;
				Sleep(1000);
				nBuyWnum--;
				if (nBuyWnum > 0)
					goto M1;
			} else if (bIsRaise == FALSE && pAnimalGrow.buyprice * nBuyWnum < pUserInfo.Money)
			{
				CString strPostData;
				strPostData.Format("cId=%d&number=%d&farmKey=null&uIdx=%d",nBuyId,nBuyWnum,m_SelfInfo.Id);
				CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_buy_animal",strPostData);
				CString strBuf;
				//���ӵľ���
				int nAddExp = 0;
				FindMidStr(strHtml,0,"\"addExp\":",",",strBuf);
				nAddExp = atoi(strBuf);
				H_GetAnimalsInfo(strHtml,pUserInfo);
				//�޸Ľ��
				pUserInfo.Money -= pAnimalGrow.buyprice * nBuyWnum;
				CString strPrintf;
				strPrintf.Format("[QQ����]:������%dֻ%s(��)",nBuyWnum,pAnimalGrow.Name);
				if (nAddExp > 0)
				{
					CString strAddExp;
					strAddExp.Format(",�����%d�㾭��",nAddExp);
					strPrintf += strAddExp;
				}
				Addtolist(strPrintf,COLOR_INFO);
				nResult = TRUE;
				Sleep(1000);
			}			
		}
	}
	if (nBuyPnum > 0)
	{
M2:
		//��ȡ�������ID
		int nBuyId = 0;
		BOOL bIsRaise = FALSE;
		nBuyId = H_GetBuyAnimalId(FALSE,bIsRaise);
		if (nBuyId > 0)
		{
			ST_ANIMALGROW pAnimalGrow;
			G_GetAnimalInfo(nBuyId,pAnimalGrow);
			if (bIsRaise == TRUE)
			{
				CString strPostData;
				strPostData.Format("number=1&farmKey=null&type=%d&uIdx=%d",nBuyId,m_SelfInfo.Id);
				CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_raise_cub",strPostData);
				CString strBuf;
				//���ӵľ���
				int nAddExp = 0;
				FindMidStr(strHtml,0,"\"addExp\":",",",strBuf);
				nAddExp = atoi(strBuf);
				H_GetAnimalsInfo(strHtml,pUserInfo);
				//�޸Ľ��
				pUserInfo.Money -= pAnimalGrow.buyprice * nBuyPnum;
				CString strPrintf;
				strPrintf.Format("[QQ����]:������1ֻ%s(��)",pAnimalGrow.Name);
				if (nAddExp > 0)
				{
					CString strAddExp;
					strAddExp.Format(",�����%d�㾭��",nAddExp);
					strPrintf += strAddExp;
				}
				Addtolist(strPrintf,COLOR_INFO);
				nResult = TRUE;
				Sleep(1000);
				nBuyPnum--;
				if (nBuyPnum > 0)
					goto M2;
			} else if (bIsRaise == FALSE && pAnimalGrow.buyprice * nBuyPnum < pUserInfo.Money)
			{
				CString strPostData;
				strPostData.Format("cId=%d&number=%d&farmKey=null&uIdx=%d",nBuyId,nBuyPnum,m_SelfInfo.Id);
				CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_buy_animal",strPostData);
				CString strBuf;
				//���ӵľ���
				int nAddExp = 0;
				FindMidStr(strHtml,0,"\"addExp\":",",",strBuf);
				nAddExp = atoi(strBuf);
				H_GetAnimalsInfo(strHtml,pUserInfo);
				//�޸Ľ��
				pUserInfo.Money -= pAnimalGrow.buyprice * nBuyPnum;
				CString strPrintf;
				strPrintf.Format("[QQ����]:������%dֻ%s(��)",nBuyPnum,pAnimalGrow.Name);
				if (nAddExp > 0)
				{
					CString strAddExp;
					strAddExp.Format(",�����%d�㾭��",nAddExp);
					strPrintf += strAddExp;
				}
				Addtolist(strPrintf,COLOR_INFO);
				nResult = TRUE;
				Sleep(1000);
			}
		}
	}
	if (nResult)
		G_WriteUserInfo(pUserInfo);
	return nResult;
}
//��������
UINT F_PastureClearMosquito(ST_USERINFO &pUserInfo)
{
	if (pUserInfo.mosquito > 0)
	{
		if (pUserInfo.mosquito > 1)
		{
			BOOL bIsDouble = FALSE;
			if (pUserInfo.mosquito%2 == 0)
				bIsDouble = TRUE;
			
			if (bIsDouble == FALSE)
				pUserInfo.mosquito++;
			
			pUserInfo.mosquito = pUserInfo.mosquito/2;
			
			CString strPostData;
			strPostData.Format("pos=1&uId=%d&farmKey=null&num=%d&type=1&uIdx=%d",pUserInfo.Id,pUserInfo.mosquito,m_SelfInfo.Id);
			CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_help_pasture",strPostData);
			CString strBuf;
			//���ӵľ���
			int nAddExp = 0;
			FindMidStr(strHtml,0,"\"addExp\":",",",strBuf);
			nAddExp = atoi(strBuf);
			CString strPrintf;
			if (pUserInfo.Id == m_SelfInfo.Id)
			{
				strPrintf.Format("[QQ����]:�������Լ��ҵ�%dֻ����,�����%d�㾭��",pUserInfo.mosquito,nAddExp);
			}
			else
			{
				strPrintf.Format("[QQ����]:������[%s]�ҵ�%dֻ����,�����%d�㾭��",pUserInfo.Name,pUserInfo.mosquito,nAddExp);
				if (nAddExp == 0)
					bClearMosquito = FALSE;
			}
			Addtolist(strPrintf,COLOR_INFO);
			
			if (bIsDouble == FALSE)
				pUserInfo.mosquito--;
			
			Sleep(600);
			
			strPostData.Format("pos=2&uId=%d&farmKey=null&num=%d&type=1&uIdx=%d",pUserInfo.Id,pUserInfo.mosquito,m_SelfInfo.Id);
			strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_help_pasture",strPostData);
			FindMidStr(strHtml,0,"\"addExp\":",",",strBuf);
			nAddExp = atoi(strBuf);
			if (pUserInfo.Id == m_SelfInfo.Id)
			{
				strPrintf.Format("[QQ����]:�������Լ��ҵ�%dֻ����,�����%d�㾭��",pUserInfo.mosquito,nAddExp);
			}
			else
			{
				strPrintf.Format("[QQ����]:������[%s]�ҵ�%dֻ����,�����%d�㾭��",pUserInfo.Name,pUserInfo.mosquito,nAddExp);
				if (nAddExp == 0)
					bClearMosquito = FALSE;
			}
			Addtolist(strPrintf,COLOR_INFO);
		}
		else
		{
			CString strPostData;
			strPostData.Format("pos=1&uId=%d&farmKey=null&num=%d&type=1&uIdx=%d",pUserInfo.Id,pUserInfo.mosquito,m_SelfInfo.Id);
			CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_help_pasture",strPostData);
			CString strBuf;
			//���ӵľ���
			int nAddExp = 0;
			FindMidStr(strHtml,0,"\"addExp\":",",",strBuf);
			nAddExp = atoi(strBuf);
			CString strPrintf;
			if (pUserInfo.Id == m_SelfInfo.Id)
			{
				strPrintf.Format("[QQ����]:�������Լ��ҵ�%dֻ����,�����%d�㾭��",pUserInfo.mosquito,nAddExp);
			}
			else
			{
				strPrintf.Format("[QQ����]:������[%s]�ҵ�%dֻ����,�����%d�㾭��",pUserInfo.Name,pUserInfo.mosquito,nAddExp);
				if (nAddExp == 0)
					bClearMosquito = FALSE;
			}
			Addtolist(strPrintf,COLOR_INFO);
		}
		pUserInfo.mosquito = 0;
		G_WriteUserInfo(pUserInfo);
		return TRUE;
	}
	return FALSE;
}
//������
UINT F_PastureClearShit(ST_USERINFO &pUserInfo)
{
	if (pUserInfo.shit > 0)
	{
		CString strPostData;
		strPostData.Format("pos=0&uId=%d&farmKey=null&num=%d&type=2&uIdx=%d",pUserInfo.Id,pUserInfo.shit,m_SelfInfo.Id);
		CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_help_pasture",strPostData);
		CString strBuf;
		//�ռ��ı��
		int nRepNum = 0;
		FindMidStr(strHtml,0,"\"repNum\":",",",strBuf);
		nRepNum = atoi(strBuf);
		CString strPrintf;
		if (pUserInfo.Id == m_SelfInfo.Id)
		{
			strPrintf.Format("[QQ����]:�ռ����Լ��ҵ�%d����",nRepNum);
		}
		else
		{
			strPrintf.Format("[QQ����]:�ռ���[%s]�ҵ�%d����",pUserInfo.Name,nRepNum);
			if (nRepNum == 0)
				bClearShit = FALSE;
		}
		Addtolist(strPrintf,COLOR_INFO);
		pUserInfo.shit = 0;
		G_WriteUserInfo(pUserInfo);
		return TRUE;
	}
	return FALSE;
}
//��������
UINT F_PastureUpHouse(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	CString strPostData;
	strPostData.Format("farmKey=null&act=query&level=%d&type=1&newitem=1&uIdx=%d",pUserInfo.wlvl,pUserInfo.Id);
	CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_up_animalhouse",strPostData);
	CString strBuf;
	//�ȼ�
	int nLevel = 0;
	//���
	int nMoney = 0;
	FindMidStr(strHtml,0,"\"level\":",",",strBuf);
	nLevel = atoi(strBuf);
	FindMidStr(strHtml,0,"\"money\":","}",strBuf);
	nMoney = atoi(strBuf);
	if (nLevel != 0 &&
		nMoney != 0 &&
		nLevel <= m_SelfInfo.Lvl &&
		nMoney < pUserInfo.Money)
	{
		Sleep(2000);
		strPostData.Format("type=1&newitem=1&farmKey=null&uIdx=%d",pUserInfo.Id);
		strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_up_animalhouse",strPostData);
		//�Ƿ�ɹ�
		int nCode = 0;
		FindMidStr(strHtml,0,"\"code\":",",",strBuf);
		nCode = atoi(strBuf);
		if (nCode == TRUE)
		{
			//�����ѵȼ�
			if(FindMidStr(strHtml,0,"\"id\":102,\"lv\":","}",strBuf) != -1)
				pUserInfo.wlvl=atoi(strBuf);
			CString strPrintf;
			strPrintf.Format("[QQ����]:����������%d��,������%d�����",pUserInfo.wlvl,nMoney);
			Addtolist(strPrintf,COLOR_INFO);
			nResult = TRUE;
		}
	}
	
	Sleep(2000);
	
	strPostData.Format("farmKey=null&act=query&level=%d&type=2&newitem=1&uIdx=%d",pUserInfo.plvl,pUserInfo.Id);
	strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_up_animalhouse",strPostData);
	
	//�ȼ�
	nLevel = 0;
	//���
	nMoney = 0;
	FindMidStr(strHtml,0,"\"level\":",",",strBuf);
	nLevel = atoi(strBuf);
	FindMidStr(strHtml,0,"\"money\":","}",strBuf);
	nMoney = atoi(strBuf);
	if (nLevel != 0 &&
		nMoney != 0 &&
		nLevel <= m_SelfInfo.Lvl &&
		nMoney < pUserInfo.Money)
	{
		Sleep(2000);
		strPostData.Format("type=2&newitem=1&farmKey=null&uIdx=%d",pUserInfo.Id);
		strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_up_animalhouse",strPostData);
		//�Ƿ�ɹ�
		int nCode = 0;
		FindMidStr(strHtml,0,"\"code\":",",",strBuf);
		nCode = atoi(strBuf);
		if (nCode == TRUE)
		{
			//������ȼ�
			if(FindMidStr(strHtml,0,"\"id\":103,\"lv\":","}",strBuf) != -1)
				pUserInfo.plvl=atoi(strBuf);
			CString strPrintf;
			strPrintf.Format("[QQ����]:����������%d��,������%d�����",pUserInfo.plvl,nMoney);
			Addtolist(strPrintf,COLOR_INFO);
			nResult = TRUE;
		}
	}
	if (nResult)
		G_WriteUserInfo(pUserInfo);
	return nResult;
}
//����Ұ������
UINT F_PastureAttackBeast(ST_USERINFO &pUserInfo)
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
				strPostData.Format("weapon=3&mode=0&uinY=%s&uinX=%d&isqz=1&ownerId=%d&flag=1&attackType=Fight&slotid=%d&isfarm=0&owner=%d&uIdx=%d&farmKey=null&index=%d",GetQQInfo(GQI_NUM),pUserInfo.QQ,pUserInfo.Id,pUserInfo.BeastLand[i].Id,pUserInfo.BeastLand[i].fId,m_SelfInfo.Id,i);
				CString strHtml = N_PasturePostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_attack_beast",strPostData);
				Sleep(1000);
				CString strBuf;
				//���ӵ���Ʒֵ
				int nAddMoral = 0;
				FindMidStr(strHtml,0,"\"addmoral\":",",",strBuf);
				nAddMoral = atoi(strBuf);
				if (nAddMoral > 0)
				{
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
					strPrintf.Format("[QQ����]:������%s�����%s(%d/%d),����%d����Ʒ",strUser,pBeastInfo.Name,pUserInfo.BeastLand[i].Blood,pBeastInfo.Blood,nAddMoral);
					Addtolist(strPrintf,COLOR_INFO);
				}else if (nResult == 2)
				{
					CString strPrintf;
					strPrintf.Format("[QQ����]:˦����%s�����%s(%d/%d),����%d����Ʒ",strUser,pBeastInfo.Name,pUserInfo.BeastLand[i].Blood,pBeastInfo.Blood,nAddMoral);
					Addtolist(strPrintf,COLOR_INFO);
				}else if (nResult == 3)
				{
					CString strPrintf;
					strPrintf.Format("[QQ����]:����%s�����%s(%d/%d)ʧ��",strUser,pBeastInfo.Name,pUserInfo.BeastLand[i].Blood,pBeastInfo.Blood);
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
						
						strPostData.Format("farmKey=null&farm=0&ownerId=%d&time=%d&index=%d&opt=1&uinY=%s&id=%d&uIdx=%d&uinX=%d",pUserInfo.Id,dwTime,nCount,GetQQInfo(GQI_NUM),nId,m_SelfInfo.Id,pUserInfo.QQ);
						strHtml = N_PasturePostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_pickup_crystal",strPostData);
						Sleep(800);
						
						int nCode = 0;
						FindMidStr(strHtml,0,"\"code\":",",",strBuf);
						nCode = atoi(strBuf);
						if (nCode == TRUE)
						{
							ST_CRYSTALINFO pCrystalInfo;
							G_GetCrystalInfo(nId,pCrystalInfo);
							CString strPrintf;
							strPrintf.Format("[QQ����]:����%d��%s",nNum,pCrystalInfo.Name);
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
//�����ҵ�����������
UINT H_HandleMyPasture(LPVOID pParam)
{
	ST_USERINFO pUserInfo;	//�������ļ��ж�ȡ�Ѿ���ȡ������
	G_GetUserInfo(m_SelfInfo.Id,pUserInfo);
	m_SelfInfo.Lvl = FarmExpToLevel(pUserInfo.Exp);	//����һ���ҵĵȼ�

	UINT nAddFood = FALSE;	//�Ƿ��������
	UINT nHarvest1 = FALSE; //�Ƿ��ջ�����
	UINT nHarvest2 = FALSE; //�Ƿ��ջ����Ķ���
	UINT nBuyAnimal = FALSE;//�Ƿ����˶���
	UINT nClearMosquito = FALSE; //�Ƿ�ɱ����
	UINT nClearShit	= FALSE; //�Ƿ�������
	UINT nAttackBeast = FALSE;//�Ƿ��й���Ұ������

	//�������
	if (m_Config.bAddFood)
	{
		nAddFood = F_PastureAddFood(pUserInfo);
		if (nAddFood)
			Sleep(2000);
	}
	//�������ջ�����
	if (m_Config.bHarvest)
	{
		nHarvest1 = F_PastureHarvest1(pUserInfo);
		if (nHarvest1)
			Sleep(2000);
		//�ջ�����ڵĶ���
		nHarvest2 = F_PastureHarvest2(pUserInfo);
		if (nHarvest2)
			Sleep(2000);

		m_SelfInfo.Lvl = FarmExpToLevel(pUserInfo.Exp);	//����һ���ҵĵȼ�
	}
	//�����ֿ��ջ�
	if (m_Config.bSaleAnimals && nHarvest2)
	{
		F_GetUserAnimals(NULL);
		Sleep(2000);
		F_SaleUserAnimals(NULL);
		Sleep(2000);
	}
	//��������
	if (m_Config.bUpHouse && nHarvest2)
	{
		F_PastureUpHouse(pUserInfo);
		Sleep(2000);
	}
	//������
	if (m_Config.bBuyAnimal)
	{
		nBuyAnimal = F_PastureBuyAnimal(pUserInfo);
		if (nBuyAnimal)
			Sleep(2000);
	}
	//ɱ����
	if (m_Config.bClearBad)
	{
		nClearMosquito = F_PastureClearMosquito(pUserInfo);
		if (nClearMosquito)
			Sleep(2000);
		//����
		nClearShit = F_PastureClearShit(pUserInfo);
		if (nClearShit)
			Sleep(2000);
	}

	nAttackBeast = F_PastureAttackBeast(pUserInfo); //����Ұ������
	if (nAttackBeast)
		Sleep(2000);

	if (nHarvest1 || nHarvest2 || nBuyAnimal)
	{
		if (m_SelfLandId == m_SelfInfo.Id)
			S_ShowMyRipeTime();
	}

	return TRUE;
}
//͵ȡ���Ѷ���
UINT F_PastureSteal(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	std::vector<int> v_Steal;
		ST_ANIMALGROW pAnimalGrow;
		std::vector<LONG> v_CrowValue;
		//�Ƿ���ͣ15��,�ȴ���������
		BOOL bSleep = FALSE;
		int wNum,pNum;
		PastureHouseNum(pUserInfo.wlvl,pUserInfo.plvl,wNum,pNum);
		for (int i=0;i<wNum+pNum;i++)
		{
			if (pUserInfo.AnimalLand[i].Id > 0 &&
				pUserInfo.AnimalLand[i].hungry == FALSE &&
				pUserInfo.AnimalLand[i].status == 3)
			{
				CString strPostData;
				strPostData.Format("nick=%s&uId=%d&farmKey=null&uIdx=%d&serial=%d",strConvert(GetQQInfo(GQI_NAME),936,CP_UTF8),pUserInfo.Id,m_SelfInfo.Id,pUserInfo.AnimalLand[i].serial);
				CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_post_product",strPostData);
				CString strBuf;
				//�Ƿ�ɹ�
				int nCode = 0;
				//���ӵľ���
				int nAddExp = 0;
				FindMidStr(strHtml,0,"\"code\":",",",strBuf);
				nCode = atoi(strBuf);
				FindMidStr(strHtml,0,"\"addExp\":",",",strBuf);
				nAddExp = atoi(strBuf);
				//�׶�
				FindMidStr(strHtml,0,"\"status\":",",",strBuf);
				pUserInfo.AnimalLand[i].status = atoi(strBuf);
				//����
				pUserInfo.AnimalLand[i].totalCome = 1;
				//���ʱ��
				FindMidStr(strHtml,0,"\"growTime\":",",",strBuf);
				pUserInfo.AnimalLand[i].growTime = atoi(strBuf);
				//��һ�׶�
				FindMidStr(strHtml,0,"\"statusNext\":",",",strBuf);
				pUserInfo.AnimalLand[i].statusNext = atoi(strBuf);
				//��һ�׶�ʱ��
				FindMidStr(strHtml,0,"\"growTimeNext\":",",",strBuf);
				pUserInfo.AnimalLand[i].growTimeNext = atoi(strBuf);
				//�Ƿ񼢶�
				FindMidStr(strHtml,0,"\"hungry\":",",",strBuf);
				pUserInfo.AnimalLand[i].hungry = atoi(strBuf);
				//�޸�͵ȡ��־
				pUserInfo.AnimalLand[i].stealflag = 3;
				if (pUserInfo.AnimalLand[i].hungry == FALSE)
				{
					//���������һ�׶ε�ʱ��
					pUserInfo.AnimalLand[i].changeTime = pUserInfo.AnimalLand[i].growTimeNext + dwServerTime;
				}
				//����ɹ�
				if (nCode == TRUE)
				{
					if (pAnimalGrow.Id != pUserInfo.AnimalLand[i].Id)
					{
						G_GetAnimalInfo(pUserInfo.AnimalLand[i].Id,pAnimalGrow);
						v_CrowValue.clear();	//��ն�̬����
						v_CrowValue.push_back(pAnimalGrow.cropvalue[0]);	//����
						v_CrowValue.push_back(pAnimalGrow.cropvalue[1]);	//�ɳ���
						int nValue = pAnimalGrow.cropvalue[3] + pAnimalGrow.cropvalue[4];	//�����ڼ��
						int nNum = pAnimalGrow.cropvalue[2] / nValue;		//��������
						for (int j=0;j<nNum;j++)	//nNum��1 �Ǵ��ջ��ڵ�ʱ��
						{
							v_CrowValue.push_back(nValue);					//ѭ������������ʱ��
						}
					}
					CString strPrintf;
					strPrintf.Format("[QQ����]:������[%s]%d��λ��%s��ȥ����",pUserInfo.Name,pUserInfo.AnimalLand[i].serial,pAnimalGrow.Name);
					if (nAddExp > 0)
					{
						CString strAddExp;
						strAddExp.Format(",�����%d�㾭��",nAddExp);
						strPrintf += strAddExp;
					}
					Addtolist(strPrintf,COLOR_INFO);
					bSleep = TRUE;
				}
				Sleep(600);
			}
	
			if (pUserInfo.AnimalLand[i].Id > 0 &&
				pUserInfo.AnimalLand[i].status == 4)
			{
				pUserInfo.AnimalLand[i].totalCome = 1;
				pUserInfo.AnimalLand[i].stealflag = 3;
				pUserInfo.AnimalLand[i].status = 5;
				if (pAnimalGrow.Id != pUserInfo.AnimalLand[i].Id)
				{
					G_GetAnimalInfo(pUserInfo.AnimalLand[i].Id,pAnimalGrow);
					v_CrowValue.clear();	//��ն�̬����
					v_CrowValue.push_back(pAnimalGrow.cropvalue[0]);	//����
					v_CrowValue.push_back(pAnimalGrow.cropvalue[1]);	//�ɳ���
					int nValue = pAnimalGrow.cropvalue[3] + pAnimalGrow.cropvalue[4];	//�����ڼ��
					int nNum = pAnimalGrow.cropvalue[2] / nValue;		//��������
					for (int j=0;j<nNum;j++)	//nNum��1 �Ǵ��ջ��ڵ�ʱ��
					{
						v_CrowValue.push_back(nValue);					//ѭ������������ʱ��
					}
				}
				int nValue = 0;
				int nStatus = 0;
				for (int j=0;j<v_CrowValue.size();j++)
				{
					nValue += v_CrowValue[j];
					if (pUserInfo.AnimalLand[i].growTime < nValue)
					{
						nStatus = j + 1;
						break;
					}
				}
				if (nStatus == v_CrowValue.size())
				{
					pUserInfo.AnimalLand[i].statusNext = 6;
					pUserInfo.AnimalLand[i].growTimeNext = pAnimalGrow.cropvalue[5] - pUserInfo.AnimalLand[i].growTime;
				}
				else
				{
					pUserInfo.AnimalLand[i].statusNext = 3;
					pUserInfo.AnimalLand[i].growTimeNext = pAnimalGrow.cropvalue[4] + pAnimalGrow.cropvalue[3];
				}
				if (pUserInfo.AnimalLand[i].hungry == FALSE)
				{
					//���������һ�׶ε�ʱ��
					pUserInfo.AnimalLand[i].changeTime = pUserInfo.AnimalLand[i].growTimeNext + dwServerTime;
				}
				bSleep = TRUE;
			}
	
			//�����ջ������ID���붯̬����
			if (pUserInfo.AnimalLand[i].Id > 0 &&			//���ID������
				pUserInfo.AnimalLand[i].totalCome > 0 &&	//����������
				pUserInfo.AnimalLand[i].stealflag == 3)		//͵ȡ��ʶΪ3
			{
				pUserInfo.AnimalLand[i].totalCome = 0;
				pUserInfo.AnimalLand[i].stealflag = 0;
				BOOL bSteal = FALSE;	//�Ƿ��Ѵ����ջ�ID������
				for (int j=0;j<v_Steal.size();j++)
				{
					if (v_Steal[j] == pUserInfo.AnimalLand[i].Id)	//ѭ�������������Ƿ��Ѵ�����ͬID
					{
						bSteal = TRUE;
						break;
					}
				}
				if (!bSteal)			//�����������ID��������
					v_Steal.push_back(pUserInfo.AnimalLand[i].Id);
			}
		}
	
		if (bSleep)
			Sleep(16000);
	
		if (v_Steal.size() > 0)
		{
			CString strPostData;
			strPostData.Format("nick=%s&uId=%d&version=1&harvesttype=1&farmKey=null&uIdx=%d&type=",strConvert(GetQQInfo(GQI_NAME),936,CP_UTF8),pUserInfo.Id,m_SelfInfo.Id);
			strPostData += "%2D1";
			CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_steal_product",strPostData);
			CString strBuf;
			CString strPrintf;
			for (i=0;i<v_Steal.size();i++)
			{
				CString strFind;
				strFind.Format("[%d,",v_Steal[i]);
				if (FindMidStr(strHtml,0,strFind,"]",strBuf) != -1)
				{
					if (pAnimalGrow.Id != v_Steal[i])
						G_GetAnimalInfo(v_Steal[i],pAnimalGrow);
					strPrintf.Format("[QQ����]:͵ȡ�˺���[%s]��%s%s%s",pUserInfo.Name,strBuf,pAnimalGrow.Unit,pAnimalGrow.Product);
					Addtolist(strPrintf,COLOR_INFO);
				}
			}
			nResult = TRUE;
		}
		G_WriteUserInfo(pUserInfo);
		if (!v_Steal.empty())
			v_Steal.clear();
		if (!v_CrowValue.empty())
			v_CrowValue.clear();
	return nResult;
}
//�������������
UINT F_PastureAddFoodToFd(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	if (pUserInfo.animalFood < 991 && bAddFoodToFd)
	{
		CString strPostData;
		strPostData.Format("type=2&foodnum=10&uId=%d&farmKey=null&uIdx=%d&nick=%s",
						pUserInfo.Id,m_SelfInfo.Id,strConvert(GetQQInfo(GQI_NAME),936,CP_UTF8));
		CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_feed_food",strPostData);

		CString strBuf;
		int nAddExp = 0;
		int nMoney = 0;
		CString strDirection;
		FindMidStr(strHtml,0,"\"money\":",",",strBuf);
		nMoney = atoi(strBuf);
		if (nMoney > 0)
		{
			//��ȡ��������
			FindMidStr(strHtml,0,"\"addExp\":",",",strBuf);
			nAddExp = atoi(strBuf);
			if (nAddExp == -1 || nAddExp == 0)
				bAddFoodToFd = FALSE;
			strDirection = CFindMidStr(strHtml,"\"alert\":\"","\",");
			FindMidStr(strHtml,0,"\"total\":",",",strBuf);
			pUserInfo.animalFood = atoi(strBuf);
			
			CString strPrintf;
			strPrintf.Format("[QQ����]:Ϊ����[%s]���������������,�����%d�㾭��",pUserInfo.Name,nAddExp);
			if (strDirection != "")
				strPrintf += "\r\n��" + N_HtmlToText(strDirection);
			Addtolist(strPrintf,COLOR_INFO);
			nResult = TRUE;
		}
	}
	if (nResult)
		G_WriteUserInfo(pUserInfo);
	return nResult;
}
//���������������
UINT H_HandleFriendPasture(LPVOID pParam)
{
	int nId = (int)pParam;
	ST_USERINFO pUserInfo;	//�������ļ��ж�ȡ�Ѿ���ȡ������
	G_GetUserInfo(nId,pUserInfo);

	UINT nSteal = FALSE;	//�Ƿ�͵ȡ��
	UINT nClearMosquito = FALSE; //�Ƿ�ɱ����
	UINT nClearShit	= FALSE; //�Ƿ�������
	UINT nAddFoodToFd = FALSE; //�Ƿ�Ϊ�����������

	//Ϊ�����������
	if (bAddFoodToFd)
	{
		nAddFoodToFd = F_PastureAddFoodToFd(pUserInfo);
		if (nAddFoodToFd)
			Sleep(2000);
	}
	//͵ȡ����
	if (m_Config.bFdSteal)
	{
		if (m_Config.bFdSafety || pUserInfo.guard == -1 || pUserInfo.guard == 0)
		{
			nSteal = F_PastureSteal(pUserInfo);
			if (nSteal)
				Sleep(2000);
		}
	}
	//ɱ����
	if (bClearMosquito && m_Config.bFdClearBad)
	{
		nClearMosquito = F_PastureClearMosquito(pUserInfo);
		if (nClearMosquito)
			Sleep(2000);
	}
	//����
	if (bClearShit && m_Config.bFdClearBad)
	{
		nClearShit = F_PastureClearShit(pUserInfo);
		if (nClearShit)
			Sleep(2000);
	}
	
	if (F_PastureAttackBeast(pUserInfo) == TRUE)	//����Ұ������
		Sleep(2000);
	
	return TRUE;
}
//ɨ�������������,������ĺ�����ͬ�����������Сʱ��,���Զ���ȡINI�е�����
UINT F_ScanPastureInfo(LPVOID pParam)
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
		CString strPostData;	//����POST����,���ʺ�������
		if (nId == m_SelfInfo.Id)	//����ǻ�ȡ�Լ�����������
			strPostData.Format("flag=1&uId=0&farmKey=null&newitem=2&uIdx=%d",nId);
		else
			strPostData.Format("farmKey=null&uId=%d&uIdx=%d&newitem=2&flag=1",nId,m_SelfInfo.Id);
		
		strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_enter",strPostData);
		
		H_GetPastureInfo(strHtml,pUserInfo);	//�����ص���������
		//����������д�������ļ�
		G_WriteUserInfo(pUserInfo);
		nResult = TRUE;
	}
	return nResult;
}
//��ȡָ���û�����������
UINT F_GetPastureInfo(LPVOID pParam)
{
	int nId = (int)pParam;	//��ȡID
	ST_USERINFO pUserInfo;	//�������ļ��ж�ȡ�Ѿ���ȡ������
	G_GetUserInfo(nId,pUserInfo);
	CString strHtml;
	CString strPostData;	//����POST����,���ʺ�������
	if (nId == m_SelfInfo.Id)	//����ǻ�ȡ�Լ�����������
		strPostData.Format("flag=1&uId=0&farmKey=null&newitem=2&uIdx=%d",nId);
	else
		strPostData.Format("farmKey=null&uId=%d&uIdx=%d&newitem=2&flag=1",nId,m_SelfInfo.Id);
	
	strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_enter",strPostData);
	
	H_GetPastureInfo(strHtml,pUserInfo);	//�����ص���������
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
	strPostData.Format("farmKey=null&user=true&uIdx=%d",m_SelfInfo.Id);
	CString strHtml = N_PasturePostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_getFriendList?mod=friend",strPostData);
	
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
		FindMidStr(strBuf,0,"\"pastrueExp\":",",",strItem);
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
		if (pUserInfo.pf && strQQ != GetQQInfo(GQI_NUM))	//�����������Լ���ID�ű���
		{
			//����������д��INI�ļ�
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

UINT F_GetUserPackage(LPVOID pParam)
{
	char szAnimals[SHORTUSERLEN]={0};
	char szAnimal[SHORTLEN]={0};
	//ȡ����Section��,�����ַ�����0��β,�����ַ�����00��β
	GetPrivateProfileSectionNames(szAnimals,SHORTUSERLEN,strAnimalsIni);
	int nPos = 0;
	while(szAnimals[nPos] != 0)
	{
		strcpy(szAnimal,szAnimals+nPos);	//�õ�������Section
		ST_ANIMALGROW pAnimalGrow;
		G_GetAnimalInfo(atoi(szAnimal),pAnimalGrow);	//��ȡ���Section������
		pAnimalGrow.nPackage = 0;				//��ձ�������
		G_WriteAnimalInfo(pAnimalGrow);		//����д�������ļ�
		nPos += strlen(szAnimal) + 1;
	}
	m_Pastures = 0;		//����ҵı�����������
	
	CString strPostData;
	strPostData.Format("farmKey=null&uIdx=%d",m_SelfInfo.Id);
	CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_get_package",strPostData);
	CString strBuf;
	int nOffset = 0;
	nOffset = FindMidStr(strHtml,nOffset,"{","}",strBuf);
	while(nOffset!=-1)
	{
		CString strItem;
		ST_ANIMALGROW pAnimalGrow;
		if (FindMidStr(strBuf,0,"\"type\":",",",strItem) != -1)
		{
			int nType = atoi(strItem);
			if (nType == 9)
			{
				FindMidStr(strBuf,0,"\"tId\":",",",strItem);
				pAnimalGrow.Id = atoi(strItem);
				G_GetAnimalInfo(pAnimalGrow.Id,pAnimalGrow);
				//����
				FindMidStr(strBuf,0,"\"amount\":","",strItem);
				pAnimalGrow.nPackage = atoi(strItem);
				//�ȼ�
				FindMidStr(strBuf,0,"\"tDesc\":\"","\"",strItem);
				pAnimalGrow.Level = atoi(strItem);
				G_WriteAnimalInfo(pAnimalGrow);
			}
			else if (nType == 4)
			{
				FindMidStr(strBuf,0,"\"tId\":",",",strItem);
				if (atoi(strItem) == 40)
				{
					FindMidStr(strBuf,0,"\"amount\":","",strItem);
					m_Pastures = atoi(strItem);
				}
			}
		}
		
		if (nOffset == -1)
			break;
		
		nOffset = FindMidStr(strHtml,nOffset,"{","}",strBuf);
	}
	return TRUE;
}

UINT F_GetUserAnimals(LPVOID pParam)
{
	char szAnimals[SHORTUSERLEN]={0};
	char szAnimal[SHORTLEN]={0};
	//ȡ����Section��,�����ַ�����0��β,�����ַ�����00��β
	GetPrivateProfileSectionNames(szAnimals,SHORTUSERLEN,strAnimalsIni);
	int nPos = 0;
	while(szAnimals[nPos] != 0)
	{
		strcpy(szAnimal,szAnimals+nPos);	//�õ�������Section
		ST_ANIMALGROW pAnimalGrow;
		G_GetAnimalInfo(atoi(szAnimal),pAnimalGrow);	//��ȡ���Section������
		pAnimalGrow.nAnimals = 0;				//��ղֿ�����
		pAnimalGrow.nWhelp = 0;
		G_WriteAnimalInfo(pAnimalGrow);		//����д�������ļ�
		nPos += strlen(szAnimal) + 1;
	}
	
	CString strPostData;
	strPostData.Format("farmKey=null&uIdx=%d",m_SelfInfo.Id);
	CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_get_repertory?target=animal",strPostData);
	CString strBuf;
	int nOffset = 0;
	nOffset = FindMidStr(strHtml,nOffset,"{","}",strBuf);
	while(nOffset!=-1)
	{
		CString strItem;
		ST_ANIMALGROW pAnimalGrow;
		BOOL bWhelp = TRUE;	//�����̻��ǳ���Ķ���
		int nId = 0;
		//��ȡID
		if (FindMidStr(strBuf,0,"\"cId\":",",",strItem) != -1)
		{
			nId = atoi(strItem);
			if (nId > 10000)
			{
				bWhelp = FALSE;
				nId -= 10000;
			}
			//��ȡ���ض�������
			G_GetAnimalInfo(nId,pAnimalGrow);
			//����
			FindMidStr(strBuf,0,"\"amount\":",",",strItem);
			if (bWhelp)		//��������д�벻ͬ�ı���
				pAnimalGrow.nWhelp = atoi(strItem);
			else
				pAnimalGrow.nAnimals = atoi(strItem);
			//����״̬
			FindMidStr(strBuf,0,"\"lock\":",",",strItem);
			pAnimalGrow.IsLock = atoi(strItem);
			//�ȼ�
			FindMidStr(strBuf,0,"\"lv\":",",",strItem);
			pAnimalGrow.Level = atoi(strItem);
			//���ۼ۸�
			FindMidStr(strBuf,0,"\"price\":",",",strItem);
			if (bWhelp)		//��������д�벻ͬ�ı���
				pAnimalGrow.byproductprice = atoi(strItem);
			else
				pAnimalGrow.productprice = atoi(strItem);
			//����
			FindMidStr(strBuf,0,"\"type\":","",strItem);
			pAnimalGrow.Type = atoi(strItem);
			//д������
			G_WriteAnimalInfo(pAnimalGrow);
		}
		
		if (nOffset == -1)
			break;
		
		nOffset = FindMidStr(strHtml,nOffset,"{","}",strBuf);
	}
	return TRUE;
}
//�����û��ֿ��ջ�
UINT F_SaleUserAnimals(LPVOID pParam)
{
	//������������
	int nNum = 0;
	char szAnimals[SHORTUSERLEN]={0};
	char szAnimal[SHORTLEN]={0};
	//ȡ����Section��,�����ַ�����0��β,�����ַ�����00��β
	GetPrivateProfileSectionNames(szAnimals,SHORTUSERLEN,strAnimalsIni);
	int nPos = 0;
	while(szAnimals[nPos] != 0)
	{
		strcpy(szAnimal,szAnimals+nPos);	//�õ�������Section
		ST_ANIMALGROW pAnimalGrow;
		G_GetAnimalInfo(atoi(szAnimal),pAnimalGrow);	//��ȡ���Section������
		if ((pAnimalGrow.nAnimals > 0 || pAnimalGrow.nWhelp > 0) &&
			!pAnimalGrow.IsLock && pAnimalGrow.Type != 4)
		{
			nNum += pAnimalGrow.nAnimals;
			nNum += pAnimalGrow.nWhelp;
			pAnimalGrow.nAnimals = 0;
			pAnimalGrow.nWhelp = 0;
			G_WriteAnimalInfo(pAnimalGrow);
		}
		nPos += strlen(szAnimal) + 1;
	}
	
	if (nNum >= m_Config.nSaleNum)
	{
		CString strPostData;
		strPostData.Format("farmKey=null&saleAll=1&uIdx=%d",m_SelfInfo.Id);
		CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_sale_product",strPostData);
		CString strBuf;
		//�������
		int nECode = 0;
		//������
		int nMoney = 0;
		CString strDirection;	//��ʾ�ı�
		
		FindMidStr(strHtml,0,"\"ecode\":",",",strBuf);
		strDirection = CFindMidStr(strHtml,"\"direction\":\"","\",");
		FindMidStr(strHtml,0,"\"money\":","}",strBuf);
		nMoney = atoi(strBuf);
		if (nECode == 0)
		{
			ST_USERINFO pUserInfo;
			G_GetUserInfo(m_SelfInfo.Id,pUserInfo);
			pUserInfo.Money += nMoney;
			G_WriteUserInfo(pUserInfo);
			CString strPrintf("[QQ����]:�����ֿ����ж���(δ������)");
			if (strDirection != "")
				strPrintf += "\r\n��" + N_HtmlToText(strDirection);
			Addtolist(strPrintf,COLOR_INFO);
		}
	}
	return TRUE;
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
	strAnimalFood.Format("%d",pUserInfo.animalFood);	//����
	strGuard = pUserInfo.guard != -1 ? "��":"��";	//�ػ�
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
		hDlg_Main->m_List_Friends.SetItemText(nId,4,out_time);
	}
	else
	{
		hDlg_Main->m_List_Friends.SetItemText(nId,4,"");
	}
}
//��ʾ������������ʱ��
void S_ShowFriendsRipeTime()
{
	for (int i=0;i<v_FriendsInfo.size();i++)
	{
		S_ShowFriendsRipeTimeOnPos(i);		
	}
}
//��ʾ�ҵ�������������
void S_ShowMyRipeTime()
{
	hDlg_Main->m_List_Lands.DeleteAllItems();	//������б�
	ST_USERINFO pUserInfo;
	G_GetUserInfo(m_SelfLandId,pUserInfo);
	ST_ANIMALGROW pAnimalGrow;
	std::vector<LONG> v_CrowValue;
	int wNum,pNum;
	PastureHouseNum(pUserInfo.wlvl,pUserInfo.plvl,wNum,pNum);
	for (int i=0;i<wNum+pNum;i++)
	{
		int nId = pUserInfo.AnimalLand[i].Id;	//����ID,0Ϊ�յ�
		if (nId > 0)
		{
			if (nId != pAnimalGrow.Id)	//����������ݲ�ͬ,���ȡ�µ�
			{
				G_GetAnimalInfo(nId,pAnimalGrow);
				v_CrowValue.clear();	//��ն�̬����
				v_CrowValue.push_back(pAnimalGrow.cropvalue[0]);	//����
				v_CrowValue.push_back(pAnimalGrow.cropvalue[1]);	//�ɳ���
				int nValue = pAnimalGrow.cropvalue[3] + pAnimalGrow.cropvalue[4];	//�����ڼ��
				int nNum = pAnimalGrow.cropvalue[2] / nValue;		//��������
				for (int j=0;j<nNum;j++)
				{
					v_CrowValue.push_back(nValue);					//ѭ������������ʱ��
				}
			}

			char szNum[SHORTLEN]={0};
			itoa(pUserInfo.AnimalLand[i].serial,szNum,10);
			//���
			hDlg_Main->m_List_Lands.InsertItem(i,szNum);
			//��������
			hDlg_Main->m_List_Lands.SetItemText(i,1,pAnimalGrow.Name);
			//�ȼ�
			ZeroMemory(szNum,SHORTLEN);
			itoa(pAnimalGrow.Level,szNum,10);
			hDlg_Main->m_List_Lands.SetItemText(i,2,szNum);
			//�׶�
			int nValue = 0;
			int nStatus = v_CrowValue.size();
			for (int j=0;j<v_CrowValue.size();j++)
			{
				nValue += v_CrowValue[j];
				if (pUserInfo.AnimalLand[i].growTime < nValue)
				{
					nStatus = j + 1;
					break;
				}
			}
			CString strBuf;
			strBuf.Format("%d/%d",nStatus,v_CrowValue.size());
			hDlg_Main->m_List_Lands.SetItemText(i,3,strBuf);
			//����һ״̬��ʱ��
			if (pUserInfo.AnimalLand[i].hungry == FALSE)
			{
				int nNum = pUserInfo.AnimalLand[i].status;
				if (nNum < 6 && nNum != 3 && nNum != 4)
				{
					CTime t1,t2;
					CTimeSpan t3;
					int nD,nH,nM;
					t1 = pUserInfo.AnimalLand[i].changeTime;
					t2 = dwServerTime;
					t3 = t1 - t2;
					nD = (int)t3.GetDays();
					nH = t3.GetHours();
					nM = t3.GetMinutes();
					if (nD > 0)
						nH += nD * 24;
					strBuf.Format("%dСʱ%d��",nH,nM);
					hDlg_Main->m_List_Lands.SetItemText(i,4,strBuf);
				}else if (nNum == 3)
				{
					hDlg_Main->m_List_Lands.SetItemText(i,4,"������");
				}else if (nNum == 4)
				{
					hDlg_Main->m_List_Lands.SetItemText(i,4,"������");
				}else if (nNum == 6)
				{
					hDlg_Main->m_List_Lands.SetItemText(i,4,"���ջ�");
				}
			}
			else
			{
				hDlg_Main->m_List_Lands.SetItemText(i,4,"����");
			}
		}
		else
		{
			//���
			hDlg_Main->m_List_Lands.InsertItem(i,"");
			//��������
			hDlg_Main->m_List_Lands.SetItemText(i,1,"δ����");
		}
	}
	if (!v_CrowValue.empty())
		v_CrowValue.clear();
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
	char szuId[QQNUMLEN]={0};	//�û�����ID
	itoa(pUserInfo.Id,szuId,10);	//ת��IDΪ�ı�
	return WritePrivateProfileStruct(szuId,"UserInfo",&pUserInfo,sizeof(ST_USERINFO),strFriendsDataIni);
}
UINT G_GetUserList(int nId,ST_USERLIST &pUserList)
{
	char szuId[QQNUMLEN]={0};	//�û�����ID
	itoa(nId,szuId,10);	//ת��IDΪ�ı�
	return GetPrivateProfileStruct(szuId,"UserList",&pUserList,sizeof(ST_USERLIST),strFriendsListIni);
}
UINT G_WriteUserList(ST_USERLIST &pUserList)
{
	char szuId[QQNUMLEN]={0};	//�û�����ID
	itoa(pUserList.Id,szuId,10);	//ת��IDΪ�ı�	
	return WritePrivateProfileStruct(szuId,"UserList",&pUserList,sizeof(ST_USERLIST),strFriendsListIni);
}
UINT G_GetAnimalInfo(int nId,ST_ANIMALGROW &pAnimalGrow)
{
	char szuId[SHORTLEN]={0};
	itoa(nId,szuId,10);
	return GetPrivateProfileStruct(szuId,"AnimalInfo",&pAnimalGrow,sizeof(ST_ANIMALGROW),strAnimalsIni);
}
UINT G_WriteAnimalInfo(ST_ANIMALGROW &pAnimalGrow)
{
	char szuId[SHORTLEN]={0};
	itoa(pAnimalGrow.Id,szuId,10);
	return WritePrivateProfileStruct(szuId,"AnimalInfo",&pAnimalGrow,sizeof(ST_ANIMALGROW),strAnimalsIni);
}
UINT G_GetGlobalConfig(ST_GLOBALCONFIG &pConfig)
{
	return GetPrivateProfileStruct("config","global",&pConfig,sizeof(ST_GLOBALCONFIG),strQQPasture_ini);
}
UINT G_WriteGlobalConfig(ST_GLOBALCONFIG &pConfig)
{
	return WritePrivateProfileStruct("config","global",&pConfig,sizeof(ST_GLOBALCONFIG),strQQPasture_ini);
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