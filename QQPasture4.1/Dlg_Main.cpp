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
//宏定义																//
//////////////////////////////////////////////////////////////////////////
#define WM_UPDATEDATA		WM_USER + 1		//刷新窗口数据
#define IDT_SERVERTIMER		1				//刷新服务器时间
#define IDT_SHOWMYRIPE		2				//刷新我的牧场成熟时间
#define IDT_SHOWFRIENDSRIPE 3				//刷新好友列表成熟时间
#define IDT_GETMYPASTURE	5				//刷新我的农场
#define IDT_GETFRIENDPASTURE 6				//刷新好友农场
#define IDT_RESETALLCTRL	7				//到凌晨时重置所有控制变量
#define IDT_WORKTIME		8				//工作计时器
#define IDT_SLEEPTIME		9				//休息计时器
#define IDT_CHECKSERVERTIMER 10				//校正服务器时间
//////////////////////////////////////////////////////////////////////////
//全局变量																//
//////////////////////////////////////////////////////////////////////////
//绑定窗口控件
CString						strName;		//昵称
CString						strLevel;		//等级
CString						strGold;		//金币
CString						strExp;			//经验
CString						strAnimalFood;	//狗
CString						strGuard;		//看守
CString						strUpLvlExp;	//升级所需经验
int							numMin,numMax,numPos;
//WorkThread事件控制
BOOL						bGetBlogData = TRUE;	//从BLOG获取必要数据
BOOL						bGetServerTime = TRUE;	//获取牧场服务器时间
BOOL						bGetXmlData	= TRUE;		//获取牧场资源数据
BOOL						bGetFriends = TRUE;		//获取好友列表
BOOL						bScanFriends = TRUE;	//扫描好友牧场
BOOL						bGetStatusFilter = FALSE; //过滤好友状态
BOOL						bWorking	= TRUE;		//工作状态
BOOL						bGetMyPasture = FALSE;	//刷新我的牧场
//HandlePasture事件控制
BOOL						bClearMosquito = TRUE;	//为好友拍蚊子
BOOL						bClearShit	= TRUE;		//为好友清粪便
BOOL						bAddFoodToFd = TRUE;	//为好友添加饲料

ST_USERLIST					m_SelfInfo;			//我的牧场,简易数据
std::vector<ST_USERLIST>	v_FriendsInfo;		//我的好友,简易数据
int							m_SelfLandId = 0;	//当前显示的ID
int							m_Pastures = 0;			//背包里牧草的数量
ST_GLOBALCONFIG				m_Config;			//全局配置

CString						m_gtk;				//Cookie Gtk

CString						strLogPath;
//////////////////////////////////////////////////////////////////////////
//前端定义																//
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
	//绑定控件文本
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
	//给牧场动物列表添加标头
	m_List_Lands.InsertColumn(0,"编号",NULL,40,-1);
	m_List_Lands.InsertColumn(1,"名称",NULL,100,-1);
	m_List_Lands.InsertColumn(2,"等级",NULL,40,-1);
	m_List_Lands.InsertColumn(3,"阶段",NULL,50,-1);
	m_List_Lands.InsertColumn(4,"距下一阶段",NULL,102,-1);
	//设置列表框整行选择和显示网格
	m_List_Lands.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	
	//给好友列表框添加标头
	m_List_Friends.InsertColumn(0,"编号",NULL,40,-1);
	m_List_Friends.InsertColumn(1,"昵称",NULL,90,-1);
	m_List_Friends.InsertColumn(2,"等级",NULL,40,-1);
	m_List_Friends.InsertColumn(3,"金币",NULL,70,-1);
	m_List_Friends.InsertColumn(4,"距下一阶段",NULL,110,-1);
	//设置列表框整行选择和显示网格
	m_List_Friends.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	
	//初始化经验对照表
	pInitExpCmpList();

	//显示QQ头像
	CC_CreateThread(pShowHeadPic,(LPVOID)atoi(GetQQInfo(GQI_NUM)),FALSE);
	
	//设置当前QQ的专用目录
	strSelfPath = GetAppPath(GAP_DIR) + GetQQInfo(GQI_NUM);
	CreateDirectory(strSelfPath,NULL);
	strAnimalsIni =		strSelfPath + "\\QQPasture_Animals.ini";
	strFriendsDataIni = strSelfPath + "\\QQPasture_FriendsData.ini";
	strFriendsListIni = strSelfPath + "\\QQPasture_FriendsList.ini";
	strAddonIni	=		strSelfPath + "\\QQPasture_Addon.ini";
	strBeastIni =		strSelfPath + "\\QQPasture_Beast.ini";
	strCrystalIni =		strSelfPath + "\\QQPasture_Crystal.ini";
	//设定存储XML版本的INI路径
	strQQPasture_ini = strSelfPath + "\\QQPasture_Config.ini";
	
	strLogPath =		strSelfPath + "\\QQPasture_PostRetStr.log";

	//检查版本数据,如果不同则执行更新处理
	CString DataVersion;
	GetPrivateProfileString("version","dataversion",NULL,DataVersion.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQPasture_ini);
	DataVersion.ReleaseBuffer();
	int nDataVersion = atoi(DataVersion);
	if (nDataVersion == 0)
	{
		//强制更新扩展数据
		WritePrivateProfileString("xmldata","config_url","",strQQPasture_ini);
		nDataVersion ++;
	}
	if (nDataVersion == 1)
	{
		//全局配置有变化,提示重新设置
		CC_PopTrayMsg("QQ牧场","全局配置有变动,请重新设置",PTM_INFO,2000);
		nDataVersion ++;
	}
	//写入当前版本数据
	WritePrivateProfileString("version","dataversion","2",strQQPasture_ini);
	
	//启动初始化和启动工作线程
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
	case IDT_SERVERTIMER:	//刷新服务器时间
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
		bClearMosquito = TRUE;		//继续为好友拍蚊子
		bClearShit = TRUE;			//继续为好友清粪便
		bAddFoodToFd = TRUE;		//继续为好友添加饲料
		bGetXmlData = TRUE;
		N_SetGlobalTimer(NULL);		//重新设置所有计时器
		break;
	case IDT_WORKTIME:
		CC_Addtolist("[QQ牧场]:工作时间结束,准备暂停",COLOR_INFO);
		bWorking = FALSE;
		KillTimer(IDT_WORKTIME);
		SetTimer(IDT_SLEEPTIME,60*60*1000,NULL);
		break;
	case IDT_SLEEPTIME:
		CC_Addtolist("[QQ牧场]:暂停时间结束,准备继续工作",COLOR_INFO);
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
		SetDlgItemText(IDC_MAIN_BTN_MYPASTURE,"我的牧场");
		ST_USERINFO pUserInfo;
		G_GetUserInfo(v_FriendsInfo[nMark].Id,pUserInfo);
		//显示QQ头像
		CC_CreateThread(pShowHeadPic,(LPVOID)pUserInfo.QQ,FALSE);
		//显示用户信息
		S_ShowUserInfo((LPVOID)pUserInfo.Id);
		//设置要显示的ID
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
		bGetMyPasture = TRUE;	//强制更新我的牧场
	}
	else
	{
		SetDlgItemText(IDC_MAIN_BTN_MYPASTURE,"刷新牧场");
		ST_USERINFO pUserInfo;
		G_GetUserInfo(m_SelfInfo.Id,pUserInfo);
		//显示QQ头像
		CC_CreateThread(pShowHeadPic,(LPVOID)pUserInfo.QQ,FALSE);
		//显示用户信息
		S_ShowUserInfo((LPVOID)m_SelfInfo.Id);
		//设置显示ID
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
//常用代码简化函数														//
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
	SetTimer(hWndMain,IDT_GETMYPASTURE,10800000,NULL);	//三小时刷新我的牧场
	
	KillTimer(hWndMain,IDT_GETFRIENDPASTURE);
	SetTimer(hWndMain,IDT_GETFRIENDPASTURE,43200000,NULL);	 //十二个小时刷新所有好友牧场
	
	CTime tNow;
	tNow = CTime::GetCurrentTime();
	int nH = tNow.GetHour();
	int nM = tNow.GetMinute();
	int nS = tNow.GetSecond();
	int nNow = (nH*60*60)+(nM*60)+(nS);	//当前时间
	nNow = 86400 - nNow + 400;
	nNow *= 1000;
	KillTimer(hWndMain,IDT_RESETALLCTRL);
	SetTimer(hWndMain,IDT_RESETALLCTRL,nNow,NULL);
	
	KillTimer(hWndMain,IDT_WORKTIME);
	KillTimer(hWndMain,IDT_SLEEPTIME);
	SetTimer(hWndMain,IDT_WORKTIME,m_Config.nWorkTime*60*1000,NULL);	//工作计时
	bWorking = TRUE;
	
	KillTimer(hWndMain,IDT_CHECKSERVERTIMER);
	SetTimer(hWndMain,IDT_CHECKSERVERTIMER,14400000,NULL);	//校验服务器时间
	return TRUE;
}
UINT H_CheckPost(CString &strHtml)
{
	if (strHtml.Find("\"ecode\"") != -1 || strHtml.Find("\"code\"") != -1)
	{
		if(strHtml.Find("请重新登录")!=-1)
			return 1;
		if(strHtml.Find("系统繁忙")!=-1)
			return 2;
		if(strHtml.Find("请输入验证码")!=-1)
			return 3;
	}
	return 0;
}
CString N_PasturePostUrl(CString strHost,CString strUrl,CString strPostData)
{
	CString strHtml;

	//获取gtk
	CC_GetGTK(m_gtk.GetBufferSetLength(SHORTTEXTLEN));
	m_gtk.ReleaseBuffer();

	//处理一下URL
	if (strUrl.Find("?") == -1)
		strUrl = strUrl + "?g_tk=" + m_gtk;
	else
		strUrl = strUrl + "&g_tk=" + m_gtk;

	BOOL bSuccess = FALSE;
	while(!bSuccess)
	{
		//以当前时间获取KEY密钥
		CString strVCode;	//验证码
M1:
		time_t time;
		CString strFarmKey = PastureTimeSha1PastureKey(time);
		CString szPostData;	//处理POST数据
		szPostData.Format("pastureKey=%s&farmTime=%d&%s",strFarmKey,(DWORD)time,strPostData);
		if (strVCode != "")	//如果验证码不为空,则添加到POST数据
		{
			szPostData = szPostData + "&validatemsg=" + strVCode;
			strVCode = "";		//清空验证码
		}
		//POST提交,等待返回文本
		strHtml = PostUrl(strHost,strUrl,strPostHeader,szPostData,TRUE);

		FILE *fp;
		fp = fopen(strLogPath.GetBuffer(strLogPath.GetLength()),"w");
		strLogPath.ReleaseBuffer();
		fprintf(fp,"http://%s%s\r\n%s\r\n\r\n%s",strHost,strUrl,strPostData,strHtml.GetBuffer(strHtml.GetLength()));
		strHtml.ReleaseBuffer();
		fclose(fp);

		switch (H_CheckPost(strHtml))	//检查错误,根据对应错误进行处理
		{
		case 1: //需要重新登录
			//MessageBox(hWndMain,"需要重新登录","提示",0);
			::SendMessage(CChWndMain,EVENT_ALL_RELOGIN,0,0);	
			goto M1;	//跳到M1流程再次执行
		case 2: //系统繁忙,稍候再试
			CC_Addtolist("[QQ牧场]:系统繁忙,获取数据失败,一分钟后重试.",COLOR_ERROR);
			Sleep(60000); //暂停一分钟
			goto M1;	//跳到M1流程再次执行
		case 3:	//需要输入验证码
			//MessageBox(hWndMain,"需要输入验证码","提示",0);
			::SendMessage(CChWndMain,EVENT_ALL_INPUTVERIFYCODE,(LONG)strVCode.GetBufferSetLength(QQVCLEN),0);
			strVCode.ReleaseBuffer();
			goto M1;	//跳到M1流程再次执行
		}
		bSuccess = TRUE;//如果没问题,则修改判定,跳出循环
	}
	return strHtml;
}

//此处执行所有初始化函数,并启动所有工作线程
UINT N_RootThread(LPVOID pParam)
{
	//设置全局计时器
	N_SetGlobalTimer(NULL);
	//创建工作线程
	while(!H_WorkThread(NULL)){Sleep(5000);};
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//初始化必要数据														//
//////////////////////////////////////////////////////////////////////////
//获取BLOG内容
UINT N_GetBlogData(LPVOID pParam)
{
	//获取BLOG网页数据
	CString szHtml = GetUrl("http://hi.baidu.com/stevety/blog/item/1635856283f42d780d33fa43.html",FALSE);
	int nPos1,nPos2;
	
	//查询牧场KEY
	nPos1 = szHtml.Find("[pasturekey]");
	nPos2 = szHtml.Find("[/pasturekey]",nPos1);
	if (nPos1 == -1 || nPos2 == 1)
		return FALSE;
	strQQPastureKey = szHtml.Mid(nPos1 + 12,nPos2 - nPos1 - 12);
	//解密牧场KEY
	strQQPastureKey=UncrypKey(strQQPastureKey,"");
	
	return TRUE;
}
//获取资源数据
UINT N_GetXmlData(LPVOID pParam)
{
	//获取主页面数据
	CString szHtml = GetUrl("http://mcbase.qzone.qq.com/cgi-bin/index/appbase_run_unity.cgi?appid=358&max=0&appparams=353%7C",TRUE);
	//查询最新版的XML
	CString	strConfig_url;
	CString	strConfig_data;
	if(FindMidStr(szHtml,0,"\"config_url_qz\":\"","\"",strConfig_url) == -1 ||
		FindMidStr(szHtml,0,"\"animalConfig_url_qz\":\"","\"",strConfig_data) == -1)
		return FALSE;

	//读取本地配置
	G_GetGlobalConfig(m_Config);

	//从配置读取记录的XML版本
	CString strConfig_url_ini;
	CString strConfig_data_ini;
	GetPrivateProfileString("xmldata","config_url",NULL,strConfig_url_ini.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQPasture_ini);
	strConfig_url_ini.ReleaseBuffer();
	GetPrivateProfileString("xmldata","config_data",NULL,strConfig_data_ini.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQPasture_ini);
	strConfig_data_ini.ReleaseBuffer();

	//将主页面XML版本和本地存储的XML版本进行对比,不同则更新SWF数据
	CString strMaster;
	CString strFarmui1;
	CString strFarmui2;
	CString strCommonui;
	if (strConfig_url != strConfig_url_ini)
	{
		CC_Addtolist("[QQ牧场]:发现有更新版本,开始解析最新协议",COLOR_INFO);
		CC_PopTrayMsg("[QQ牧场]","发现有更新版本,开始解析最新协议",PTM_INFO,2000);
		//下载版本XML和植物数据XML
		CString strConfig_url_xml = GetAppPath(GAP_DIR)+"mc_material.xml";
		if (!GetUrlToFile(strConfig_url,strConfig_url_xml,TRUE))
			return FALSE;
		//开始分析XML内容
		CMarkup xml;
		if (!xml.Load(strConfig_url_xml))			//加载XML
			return FALSE;
		xml.ResetMainPos();							//重置搜索位置,这个类的玩意
		xml.FindElem("main");						//向下查找main元素
		xml.IntoElem();								//进入元素内部
		xml.FindElem("moduleList");					//在main内部查找moduleList元素
		xml.IntoElem();
		xml.FindElem("module");						//在moduleList内查找module
		strMaster=xml.GetAttrib("url");				//获取module元素的url元件
		xml.IntoElem();
		xml.FindElem("material");
		strFarmui1 = xml.GetAttrib("url");
		xml.FindElem("material");
		strFarmui2 = xml.GetAttrib("url");
		xml.FindElem("material");
		strCommonui = xml.GetAttrib("url");
		xml.OutOfElem();
		xml.OutOfElem();

		//开始分析卡片游戏数据
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
		//签到奖励
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
		//下载野生动物数据
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
			//名字
			strcpy(pBeastInfo.Name,xml.GetAttrib("name"));
			//是否可以购买
			pBeastInfo.IsDisplay = atoi(xml.GetAttrib("isDisplay"));
			//销售价格
			pBeastInfo.SaleMoney = atoi(xml.GetAttrib("saleMoney"));
			//购买价格
			pBeastInfo.BuyPrice = atoi(xml.GetAttrib("price"));
			//可放养的次数
			pBeastInfo.RaiseTimes = atoi(xml.GetAttrib("raiseTimes"));
			//购买需要的人品
			pBeastInfo.BuyNeedExp = atoi(xml.GetAttrib("buyNeedExp"));
			//最大体力
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
			//名字
			strcpy(pCrystalInfo.Name,xml.GetAttrib("name"));
			//销售价格
			pCrystalInfo.SalePrice = atoi(xml.GetAttrib("sale"));
			G_WriteCrystalInfo(pCrystalInfo);
		}

		DeleteFile(strConfig_wild_xml);
		DeleteFile(strConfig_url_xml);				//删除XML文件
		WritePrivateProfileString("xmldata","config_url",strConfig_url,strQQPasture_ini);
		WritePrivateProfileString("xmldata","master",strMaster,strQQPasture_ini);
		WritePrivateProfileString("xmldata","farmui1",strFarmui1,strQQPasture_ini);
		WritePrivateProfileString("xmldata","farmui2",strFarmui2,strQQPasture_ini);
		WritePrivateProfileString("xmldata","commonui",strCommonui,strQQPasture_ini);
		WritePrivateProfileString("xmldata","config_wild",strConfig_wild,strQQPasture_ini);
	}
	else
	{
		//如果相同,则从配置中读取数据
		GetPrivateProfileString("xmldata","master",NULL,strMaster.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQPasture_ini);
		strMaster.ReleaseBuffer();
		GetPrivateProfileString("xmldata","farmui1",NULL,strFarmui1.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQPasture_ini);
		strFarmui1.ReleaseBuffer();
		GetPrivateProfileString("xmldata","farmui2",NULL,strFarmui2.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQPasture_ini);
		strFarmui2.ReleaseBuffer();
		GetPrivateProfileString("xmldata","commonui",NULL,strCommonui.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQPasture_ini);
		strCommonui.ReleaseBuffer();
	}
	//这个数据在牧场功能中,POST提交时必需的,需要将module元素取到的url加入其中
	strPostHeader = "Accept: */*\r\nReferer: " + strMaster + "\r\nx-flash-version: 10,2,152,32\r\nAccept-Language: zh-cn\r\nContent-Type: application/x-www-form-urlencoded\r\nAccept-Encoding\r\n";

M1:
	CString strPostData;	//处理POST数据
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

	//将返回文本交由指定函数处理,返回自定义结构的数据
	ST_USERINFO pUserInfo;
	H_GetPastureInfo(szHtml,pUserInfo);
	pUserInfo.QQ = atoi(GetQQInfo(GQI_NUM)); //获取一下QQ
	CC_GetQQInfo(pUserInfo.Name,GQI_NAME);	//返回的数据中取不到昵称,使用公共函数取一下
	pUserInfo.IsQzone = TRUE;

	G_WriteUserInfo(pUserInfo);				//存储用户数据到硬盘
	S_ShowUserInfo((LPVOID)pUserInfo.Id);	//显示用户信息到窗口

	//将必要数据写入简易用户数据结构,存储于内存中
	m_SelfInfo.Id = pUserInfo.Id;
	m_SelfInfo.Lvl = FarmExpToLevel(pUserInfo.Exp);
	m_SelfInfo.RipeTime = H_GetPastureLandRipe(pUserInfo);
	m_SelfInfo.CheckTime = dwServerTime;

	//将主页面XML版本和本地存储的XML版本进行对比,不同则更新动物数据
	if (strConfig_data != strConfig_data_ini)
	{
		CC_Addtolist("[QQ牧场]:动物数据有变化,开始下载最新数据",COLOR_INFO);
		//下载植物数据XML到本地
		CString strConfig_data_xml = GetAppPath(GAP_DIR)+"AnimalGrow.xml";
		if (!GetUrlToFile(strConfig_data,strConfig_data_xml,TRUE))
			return FALSE;
		CMarkup xml;	//开始加载XML
		if (!xml.Load(strConfig_data_xml))
			return FALSE;
		xml.ResetMainPos();		//重置搜索位置,这个类的玩意
		xml.FindElem("main");
		xml.IntoElem();
		xml.FindElem("animals");
		xml.IntoElem();

		Sleep(2000);
		//获取商店动物数据,完善XML缺少的数据
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

			//在商店数据中查找是否有该动物的数据
			nOffset = strHtml.Find("\"cId\":"+strBuf+",",0);
			if (nOffset != -1)
			{
				//获取该动物的信息
				int nPos1,nPos2;
				nPos2 = strHtml.Find('}',nOffset);
				strItem = strHtml.Mid(0,nPos2);
				nPos1 = strItem.ReverseFind('{');
				strItem = strItem.Mid(nPos1,nPos2 - nPos1);
				//等级
				FindMidStr(strItem,0,"\"cLevel\":",",",strBuf);
				pAnimalGrow.Level = atoi(strBuf);
				//类型
				FindMidStr(strItem,0,"\"cType\":",",",strBuf);
				pAnimalGrow.Type = atoi(strBuf);
				//产物价格
				FindMidStr(strItem,0,"\"byproductprice\":",",",strBuf);
				pAnimalGrow.byproductprice = atoi(strBuf);
				//产物经验
				FindMidStr(strItem,0,"\"harvestbExp\":",",",strBuf);
				pAnimalGrow.harvestbExp = atoi(strBuf);
				//动物价格
				FindMidStr(strItem,0,"\"productprice\":",",",strBuf);
				pAnimalGrow.productprice = atoi(strBuf);
				//动物经验
				FindMidStr(strItem,0,"\"harvestpExp\":",",",strBuf);
				pAnimalGrow.harvestpExp = atoi(strBuf);
				//购买价格
				FindMidStr(strItem,0,"\"price\":",",",strBuf);
				pAnimalGrow.buyprice = atoi(strBuf);
			}
			//名字
			strcpy(pAnimalGrow.Name,xml.GetAttrib("name"));
			//仔的名字
			strcpy(pAnimalGrow.Product,xml.GetAttrib("product"));
			//计量单位
			strcpy(pAnimalGrow.Unit,xml.GetAttrib("u"));
			//产量
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

		DeleteFile(strConfig_data_xml);		//删除XML文件
		WritePrivateProfileString("xmldata","config_data",strConfig_data,strQQPasture_ini);
	}

	m_SelfLandId = m_SelfInfo.Id;
	S_ShowMyRipeTime();
	//设置刷新我的牧场土地数据定时器
	SetTimer(hWndMain,IDT_SHOWMYRIPE,60000,NULL);
	//设置刷新我的好友列表即将成熟的定时器
	SetTimer(hWndMain,IDT_SHOWFRIENDSRIPE,60000,NULL);
	//处理一下牧场数据
	if (H_HandleMyPasture(NULL))
		S_ShowUserInfo((LPVOID)m_SelfLandId);

	if (szHtml.Find("\"d\":1,",0) != -1)	//有礼包可领取
	{
		strPostData.Format("farmKey=null&uIdx=%d",m_SelfInfo.Id);
		N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_accept_gift",strPostData);
		CC_Addtolist("[QQ牧场]:领取了每日礼包",COLOR_INFO);
	}

	strPostData.Format("farmKey=null&uinY=%s&uIdx=%d",GetQQInfo(GQI_NUM),pUserInfo.Id);
	szHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_pasture_login_home",strPostData);
	Sleep(1000);
	int Num = 0;
	FindMidStr(szHtml,0,"\"number\":",",",strBuf);
	//翻牌的次数
	Num = atoi(strBuf);
	//如果次数为零
	if (Num == 0)
	{
		szHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_pasture_login_click",strPostData);
		Sleep(1000);
		//额外的收获
		int nBonus = 0;
		//连续签到的天数
		int nDays = 0;
		//翻牌的次数
		int nNumber = 0;
		FindMidStr(szHtml,0,"\"bonus\":",",",strBuf);
		nBonus = atoi(strBuf);
		FindMidStr(szHtml,0,"\"days\":",",",strBuf);
		nDays = atoi(strBuf);
		FindMidStr(szHtml,0,"\"number\":",",",strBuf);
		nNumber = atoi(strBuf);
		//如果有额外的收获
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
				strPrintf.Format("[QQ牧场]:连续签到%d天,获得%d个%s",nDays,pCardsGame.Num,pCardsGame.Name);
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
				strPrintf.Format("[QQ牧场]:参加幸运九宫格,获得%d个%s",pCardsGame.Num,pCardsGame.Name);
				Addtolist(strPrintf,COLOR_INFO);
			}
		}
	}

	return TRUE;
}

//工作线程,主体操作都在这儿
UINT H_WorkThread(LPVOID pParam)
{
	try
	{
		while(TRUE)
		{
			if (bGetBlogData)
			{
				bGetBlogData = FALSE;
				//从BLOG获取KEY等内容
				CC_Addtolist("[QQ牧场]:获取牧场加密协议",COLOR_INFO);
				while(!N_GetBlogData(NULL)){Sleep(5000);};
				Sleep(3000);
			}
			if (bGetServerTime)
			{
				bGetServerTime = FALSE;
				//获取服务器时间
				CC_Addtolist("[QQ牧场]:获取牧场服务器时间",COLOR_INFO);
				while(!pGetServerTime()){Sleep(5000);};
				Sleep(3000);
			}
			if (bGetXmlData)
			{
				bGetXmlData = FALSE;
				//从牧场主页面获取各类资源
				CC_Addtolist("[QQ牧场]:检查动物数据是否需要更新",COLOR_INFO);
				while(!N_GetXmlData(NULL)){Sleep(5000);};
				Sleep(3000);
			}
			//获取好友列表
			if (bGetFriends)	
			{
				bGetFriends = FALSE;
				CC_Addtolist("[QQ牧场]:获取牧场好友列表",COLOR_INFO);
				F_GetFriends(NULL);
				hDlg_Main->m_List_Friends.DeleteAllItems();
				for (int i=0;i<v_FriendsInfo.size();i++)	//循环读取简易好友列表数组
				{
					ST_USERINFO pUserInfo;					//从INI文件中读出获取的数据
					G_GetUserInfo(v_FriendsInfo[i].Id,pUserInfo);
					//处理界面数据
					char szNum[SHORTTEXTLEN]={0};
					itoa(i+1,szNum,10);	
					//编号
					hDlg_Main->m_List_Friends.InsertItem(i,szNum);
					//昵称
					hDlg_Main->m_List_Friends.SetItemText(i,1,pUserInfo.Name);
					//等级
					ZeroMemory(szNum,SHORTTEXTLEN);
					itoa(FarmExpToLevel(pUserInfo.Exp),szNum,10);
					hDlg_Main->m_List_Friends.SetItemText(i,2,szNum);
					//金币
					ZeroMemory(szNum,SHORTTEXTLEN);
					itoa(pUserInfo.Money,szNum,10);
					hDlg_Main->m_List_Friends.SetItemText(i,3,szNum);
				}
				Sleep(3000);
			}
			//扫描好友牧场
			if (bScanFriends)	
			{
				bScanFriends = FALSE;
				CC_Addtolist("[QQ牧场]:开始分析好友的牧场数据",COLOR_INFO);
				for (int i=0;i<v_FriendsInfo.size();i++)	//循环读取简易好友列表数组 
				{
					if (v_FriendsInfo[i].Lvl < m_Config.nIgnoreFdLvl)
						continue;
					ST_USERINFO pUserInfo;					//从INI文件中读出获取的数据
					G_GetUserInfo(v_FriendsInfo[i].Id,pUserInfo);
					CString strText;
					strText.Format("正在分析好友[%s]的牧场",pUserInfo.Name);
					SetDlgItemText(hWndMain,IDC_MAIN_STATETEXT,strText);	//设置窗口状态标签
					strText.Format("%d/%d",i+1,v_FriendsInfo.size());		//设置进度
					SetDlgItemText(hWndMain,IDC_MAIN_SCANPOS,strText);
					BOOL bSleep = F_ScanPastureInfo((LPVOID)v_FriendsInfo[i].Id);	//获取好友牧场,会自动保存至INI文件,如果从INI中读的则返回FALSE
					if (bSleep)
						H_HandleFriendPasture((LPVOID)v_FriendsInfo[i].Id);		///如果是从网络读取则处理好友牧场数据
					G_GetUserInfo(v_FriendsInfo[i].Id,pUserInfo);			//从INI文件再次读出数据
					v_FriendsInfo[i].RipeTime = H_GetPastureLandRipe(pUserInfo); //获取牧场最近成熟的时间,没有则返回0
					v_FriendsInfo[i].CheckTime = dwServerTime;				//记录一下处理的时间
					G_WriteUserList(v_FriendsInfo[i]);						//写入INI文件
					S_ShowFriendsRipeTimeOnPos(i);							//显示一下距成熟还有多久,没有则空
					if (bSleep)												//如果是从网络读取则暂停一下下
						Sleep(2000);
				}
				SetDlgItemText(hWndMain,IDC_MAIN_STATETEXT,"");				//清空状态标签
				if (m_SelfLandId == m_SelfInfo.Id)
					S_ShowUserInfo((LPVOID)m_SelfLandId);
				Sleep(3000);
			}
			if (m_SelfInfo.RipeTime > 0 && 
				m_SelfInfo.RipeTime <= dwServerTime && 
				m_Config.bHarvest)
			{
				m_SelfInfo.RipeTime = 0;
				CC_Addtolist("[QQ牧场]:开始刷新我的牧场",COLOR_INFO);
				ST_USERINFO pUserInfo;	//从INI文件中读出获取的数据
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
				m_SelfInfo.CheckTime = dwServerTime;				//记录一下处理的时间
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
					ST_USERINFO pUserInfo;	//从INI文件中读出获取的数据
					G_GetUserInfo(v_FriendsInfo[i].Id,pUserInfo);
					if (H_CountPasture(pUserInfo))
					{
						F_GetPastureInfo((LPVOID)v_FriendsInfo[i].Id);
						Sleep(1000);
						H_HandleFriendPasture((LPVOID)v_FriendsInfo[i].Id);
					}
					G_GetUserInfo(v_FriendsInfo[i].Id,pUserInfo);
					v_FriendsInfo[i].RipeTime = H_GetPastureLandRipe(pUserInfo);
					v_FriendsInfo[i].CheckTime = dwServerTime;				//记录一下处理的时间
					G_WriteUserList(v_FriendsInfo[i]);
					S_ShowFriendsRipeTimeOnPos(i);							//显示一下距成熟还有多久,没有则空
					Sleep(3000);
				}
			}
			//如果暂停工作
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
//牧场功能函数															//
//////////////////////////////////////////////////////////////////////////
UINT H_GetPastureInfo(CString &strHtml,ST_USERINFO &pUserInfo)
{
	//清空一下数据先
	ST_USERINFO pClrUserInfo;
	memcpy(&pUserInfo.AnimalLand,&pClrUserInfo.AnimalLand,sizeof(pClrUserInfo.AnimalLand));
	
	CString strBuf;	//临时文本
	
		//查找UID
		if (FindMidStr(strHtml,0,"\"uId\":",",",strBuf) != -1)
		{
			pUserInfo.Id=atoi(strBuf);
			//查找QQ
			//if(FindMidStr(strHtml,0,"\"uin\":",",",strBuf) != -1)
			//pUserInfo.QQ=atoi(strBuf);
			//查找金币
			if(FindMidStr(strHtml,0,"\"money\":",",",strBuf) != -1 && strBuf != "0")
				pUserInfo.Money=atol(strBuf);
			//查找经验
			if(FindMidStr(strHtml,0,"\"exp\":",",",strBuf) != -1)
			pUserInfo.Exp=atol(strBuf);
		}
		//查找饲料
		if(FindMidStr(strHtml,0,"\"animalFood\":",",",strBuf) != -1)
		pUserInfo.animalFood=atoi(strBuf);
		//查找窝等级
		if(FindMidStr(strHtml,0,"\"id\":102,\"lv\":","}",strBuf) != -1)
		pUserInfo.wlvl=atoi(strBuf);
		//查找棚等级
		if(FindMidStr(strHtml,0,"\"id\":103,\"lv\":","}",strBuf) != -1)
		pUserInfo.plvl=atoi(strBuf);
	
		//查找黄钻等级
		/*if (if(FindMidStr(strHtml,0,"\"yellowlevel\":",",",strBuf) == 1)
			return FALSE;
		pUserInfo.YellowLevel=atoi(strBuf) != -1)
		//查找黄钻状态
		if (if(FindMidStr(strHtml,0,"\"yellowstatus\":",",",strBuf) == 1)
			return FALSE;
		pUserInfo.YellowStatus=atoi(strBuf) != -1)*/
	
		//查找看守
		if(FindMidStr(strHtml,0,"\"striketime\"","}",strBuf)==-1)
			pUserInfo.guard = -1;
		else
			pUserInfo.guard=atoi(strBuf);
	
		//偷取标志
		CString strStealFlags,strStealFlag;
		if(FindMidStr(strHtml,0,"\"stealflag\":{","}",strStealFlags) != -1)
			strStealFlags += ",";
	
		//清空动物数据
		int wNum,pNum;
		PastureHouseNum(pUserInfo.wlvl,pUserInfo.plvl,wNum,pNum);
		for (int i=0;i<wNum+pNum;i++)
		{
			pUserInfo.AnimalLand[i].Id = 0;
			pUserInfo.AnimalLand[i].changeTime = 0;
		}
		//获取动物数据
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
					//是否可偷取
					strStealFlag.Format("\"%s\":",strBuf);
					if (FindMidStr(strStealFlags,0,strStealFlag,",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].stealflag = atoi(strBuf);
					else
					pUserInfo.AnimalLand[nCount].stealflag = 3;
					//已花费的生长时间
					if (FindMidStr(strAnimal,0,"\"growTime\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].growTime = atoi(strBuf);
					//距离下一状态需要的时间
					if (FindMidStr(strAnimal,0,"\"growTimeNext\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].growTimeNext = atoi(strBuf);
					//是否饥饿
					if (FindMidStr(strAnimal,0,"\"hungry\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].hungry = atoi(strBuf);
					//当前状态
					if (FindMidStr(strAnimal,0,"\"status\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].status = atoi(strBuf);
					//下一状态
					if (FindMidStr(strAnimal,0,"\"statusNext\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].statusNext = atoi(strBuf);
					//产物数量
					if (FindMidStr(strAnimal,0,"\"totalCome\":","",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].totalCome = atoi(strBuf);
					//如果不饥饿,就计算一下成长时间
					if (pUserInfo.AnimalLand[nCount].hungry == FALSE && pUserInfo.AnimalLand[nCount].status < 6)
					{
						//计算进入下一阶段的时间
						pUserInfo.AnimalLand[nCount].changeTime = pUserInfo.AnimalLand[nCount].growTimeNext + dwServerTime;
					}
				}
							
				if (nOffset == -1)
					break;
				//进入下一次循环
				nOffset=FindMidStr(strAnimals,nOffset,"{","}",strAnimal);
			}
		}
	
		CString strBadInfos,strBadInfo;
		//查找害虫便便的数据
		if (FindMidStr(strHtml,0,"\"badinfo\":[","]",strBadInfos) != -1)
		{
			//获取蚊子数量
			nOffset=FindMidStr(strBadInfos,0,"{","}",strBadInfo);
			if (FindMidStr(strBadInfo,0,"\"num\":",",",strBuf) != -1)
			pUserInfo.mosquito = atoi(strBuf);
			//获取便便数量
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
				//主人QQ
				if (FindMidStr(strBeast,0,"\"fid\":",",",strBuf) != -1)
					pUserInfo.BeastLand[nCount].fId = atoi(strBuf);
				//动物ID
				if (FindMidStr(strBeast,0,"\"id\":",",",strBuf) != -1)
					pUserInfo.BeastLand[nCount].Id = atoi(strBuf);
				//动物类型
				if (FindMidStr(strBeast,0,"\"type\":","",strBuf) != -1)
					pUserInfo.BeastLand[nCount].Type = atoi(strBuf);
				//放养时间
				if (FindMidStr(strBeast,0,"\"raisetime\":",",",strBuf) != -1)
					pUserInfo.BeastLand[nCount].RaiseTime = atol(strBuf);
				//返回时间
				if (FindMidStr(strBeast,0,"\"returntime\":",",",strBuf) != -1)
					pUserInfo.BeastLand[nCount].ReturnTime = atol(strBuf);
				//动物体力
				if (FindMidStr(strBeast,0,"\"blood\":",",",strBuf) != -1)
					pUserInfo.BeastLand[nCount].Blood = atoi(strBuf);
	
				//是否可以攻击
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

//处理牧场动物状态
UINT H_CountPasture(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	int wNum,pNum;
	PastureHouseNum(pUserInfo.wlvl,pUserInfo.plvl,wNum,pNum);
	ST_ANIMALGROW pAnimalGrow;
	for (int i=0;i<wNum+pNum;i++)
	{
		if (pUserInfo.AnimalLand[i].Id > 0 &&	//ID不为零
			pUserInfo.AnimalLand[i].hungry == FALSE &&	//动物不饥饿
			pUserInfo.AnimalLand[i].changeTime > 0 &&	//成长时间不为零
			pUserInfo.AnimalLand[i].changeTime <= dwServerTime)	//时间已经过了
		{
			//读取动物数据
			if (pAnimalGrow.Id != pUserInfo.AnimalLand[i].Id)
				G_GetAnimalInfo(pUserInfo.AnimalLand[i].Id,pAnimalGrow);
			//是否已经到可收获期
			if (pUserInfo.AnimalLand[i].status < 6)
			{
				//动物成长到下一阶段
				pUserInfo.AnimalLand[i].status = pUserInfo.AnimalLand[i].statusNext;
				//增加动物已经存活的时间
				pUserInfo.AnimalLand[i].growTime += pUserInfo.AnimalLand[i].growTimeNext;
				//如果已经到成熟期
				if (pUserInfo.AnimalLand[i].status == 6 && pUserInfo.Id == m_SelfInfo.Id)
					nResult = TRUE;
				//设置下一阶段需要的生长时间
				pUserInfo.AnimalLand[i].growTimeNext = pAnimalGrow.cropvalue[pUserInfo.AnimalLand[i].status - 1] - 
					(dwServerTime - pUserInfo.AnimalLand[i].changeTime);
				
				//如果动物从幼崽到成长期,那下一阶段设置为生产期
				if (pUserInfo.AnimalLand[i].status == 2)
					pUserInfo.AnimalLand[i].statusNext = 3;
				//如果动物到了生产期,则返回TRUE
				if (pUserInfo.AnimalLand[i].status == 3)
					nResult = TRUE;
			}
		}
	}
	G_WriteUserInfo(pUserInfo);
	return nResult;
}

//添加饲料
UINT F_PastureAddFood(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	if (pUserInfo.animalFood < m_Config.nMinFood)
	{
		F_GetUserPackage(NULL);
		Sleep(2000);
		//需要增加多少牧草
		int nNeedNum = m_Config.nMaxFood - pUserInfo.animalFood;
		//背包里牧草的数量
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
		
		//从背包添加饲料
		if (nPackFood > 0)
		{
			CString strPostData;
			strPostData.Format("type=0&foodnum=%d&uIdx=%d&farmKey=null",nPackFood,m_SelfInfo.Id);
			CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_feed_food",strPostData);
			CString strBuf;
			//是否成功
			int nCode = 0;
			//成功添加的数量
			int nAdded = 0;
			FindMidStr(strHtml,0,"\"code\":",",",strBuf);
			nCode = atoi(strBuf);
			if (nCode == TRUE)
			{
				FindMidStr(strHtml,0,"\"added\":",",",strBuf);
				nAdded = atoi(strBuf);
				FindMidStr(strHtml,0,"\"total\":",",",strBuf);
				//写入新的饲料数量
				pUserInfo.animalFood = atoi(strBuf);
				CString strPrintf;
				strPrintf.Format("[QQ牧场]:从背包添加了%d棵牧草作为饲料",nAdded);
				Addtolist(strPrintf,COLOR_INFO);
				H_GetPastureInfo(strHtml,pUserInfo);
				nResult = TRUE;
			}
			Sleep(2000);
			m_Pastures -= nAdded;
		}
		//从商店购买饲料
		if (nBuyFood > 0)
		{
			CString strPostData;
			strPostData.Format("type=1&foodnum=%d&uIdx=%d&farmKey=null",nBuyFood,m_SelfInfo.Id);
			CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_feed_food",strPostData);
			CString strBuf;
			//成功添加的数量
			int nAdded = nBuyFood;
			//花费的金币
			int nMoney = 0;
			FindMidStr(strHtml,0,"\"money\":",",",strBuf);
			nMoney = atoi(strBuf);
			//减一下金币
			pUserInfo.Money -= nMoney;
			FindMidStr(strHtml,0,"\"total\":",",",strBuf);
			//写入新的饲料数量
			pUserInfo.animalFood = atoi(strBuf);
			CString strPrintf;
			strPrintf.Format("[QQ牧场]:购买了%d棵牧草作为饲料,共花费了%d个金币",nAdded,nMoney);
			Addtolist(strPrintf,COLOR_INFO);
			H_GetPastureInfo(strHtml,pUserInfo);
			nResult = TRUE;
			Sleep(2000);
		}
		G_WriteUserInfo(pUserInfo);
	}
	return nResult;
}
//收获生产的幼崽
UINT F_PastureHarvest1(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	std::vector<int> v_Steal;
		ST_ANIMALGROW pAnimalGrow;
		std::vector<LONG> v_CrowValue;
		//是否暂停15秒,等待生产结束
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
				//是否成功
				int nCode = 0;
				//增加的经验
				int nAddExp = 0;
				FindMidStr(strHtml,0,"\"code\":",",",strBuf);
				nCode = atoi(strBuf);
				FindMidStr(strHtml,0,"\"addExp\":",",",strBuf);
				nAddExp = atoi(strBuf);
				//阶段
				FindMidStr(strHtml,0,"\"status\":",",",strBuf);
				pUserInfo.AnimalLand[i].status = atoi(strBuf);
				//产量
				pUserInfo.AnimalLand[i].totalCome = 0;
				//存活时间
				FindMidStr(strHtml,0,"\"growTime\":",",",strBuf);
				pUserInfo.AnimalLand[i].growTime = atoi(strBuf);
				//下一阶段
				FindMidStr(strHtml,0,"\"statusNext\":",",",strBuf);
				pUserInfo.AnimalLand[i].statusNext = atoi(strBuf);
				//下一阶段时间
				FindMidStr(strHtml,0,"\"growTimeNext\":",",",strBuf);
				pUserInfo.AnimalLand[i].growTimeNext = atoi(strBuf);
				//是否饥饿
				FindMidStr(strHtml,0,"\"hungry\":",",",strBuf);
				pUserInfo.AnimalLand[i].hungry = atoi(strBuf);
				//修改偷取标志
				pUserInfo.AnimalLand[i].stealflag = 0;
				if (pUserInfo.AnimalLand[i].hungry == FALSE)
				{
					//计算进入下一阶段的时间
					pUserInfo.AnimalLand[i].changeTime = pUserInfo.AnimalLand[i].growTimeNext + dwServerTime;
				}
				//如果成功
				if (nCode == TRUE)
				{
					if (pAnimalGrow.Id != pUserInfo.AnimalLand[i].Id)
					{
						G_GetAnimalInfo(pUserInfo.AnimalLand[i].Id,pAnimalGrow);
						v_CrowValue.clear();	//清空动态数组
						v_CrowValue.push_back(pAnimalGrow.cropvalue[0]);	//幼崽
						v_CrowValue.push_back(pAnimalGrow.cropvalue[1]);	//成长期
						int nValue = pAnimalGrow.cropvalue[3] + pAnimalGrow.cropvalue[4];	//生产期间隔
						int nNum = pAnimalGrow.cropvalue[2] / nValue;		//生产次数
						for (int j=0;j<nNum;j++)
						{
							v_CrowValue.push_back(nValue);					//循环加入生产期时间
						}
					}
					CString strPrintf;
					strPrintf.Format("[QQ牧场]:将自己%d号位的%s赶去生产",pUserInfo.AnimalLand[i].serial,pAnimalGrow.Name);
					if (nAddExp > 0)
					{
						CString strAddExp;
						strAddExp.Format(",获得了%d点经验",nAddExp);
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
					v_CrowValue.clear();	//清空动态数组
					v_CrowValue.push_back(pAnimalGrow.cropvalue[0]);	//幼崽
					v_CrowValue.push_back(pAnimalGrow.cropvalue[1]);	//成长期
					int nValue = pAnimalGrow.cropvalue[3] + pAnimalGrow.cropvalue[4];	//生产期间隔
					int nNum = pAnimalGrow.cropvalue[2] / nValue;		//生产次数
					for (int j=0;j<nNum;j++)
					{
						v_CrowValue.push_back(nValue);					//循环加入生产期时间
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
					//计算进入下一阶段的时间
					pUserInfo.AnimalLand[i].changeTime = pUserInfo.AnimalLand[i].growTimeNext + dwServerTime;
				}
				bSleep = TRUE;
			}
	
			//将可收获的幼崽ID加入动态数组
			if (pUserInfo.AnimalLand[i].Id > 0 &&			//如果ID大于零
				pUserInfo.AnimalLand[i].totalCome > 0 &&	//产量大于零
				pUserInfo.AnimalLand[i].stealflag == 3)		//偷取标识为3
			{
				pUserInfo.AnimalLand[i].totalCome = 0;
				pUserInfo.AnimalLand[i].stealflag = 0;
				BOOL bSteal = FALSE;	//是否已存在收获ID数组中
				for (int j=0;j<v_Steal.size();j++)
				{
					if (v_Steal[j] == pUserInfo.AnimalLand[i].Id)	//循环查找数组中是否已存在相同ID
					{
						bSteal = TRUE;
						break;
					}
				}
				if (!bSteal)			//如果不存在则将ID加入数组
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
			//增加的经验
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
					strPrintf.Format("[QQ牧场]:收获了自己家的%s%s%s",strBuf,pAnimalGrow.Unit,pAnimalGrow.Product);
					Addtolist(strPrintf,COLOR_INFO);
				}
			}
			if (nAddExp > 0)
			{
				strPrintf.Format("[QQ牧场]:共获得%d点经验",nAddExp);
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
//收获已成熟的动物
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
		//增加的经验
		int nAddExp = 0;
		FindMidStr(strHtml,0,"[",",[",strBuf);
		nAddExp = atoi(strBuf);
		CString strPrintf;
		for (i=0;i<v_Steal.size();i++)
		{
			if (pAnimalGrow.Id != pUserInfo.AnimalLand[v_Steal[i]].Id)
				G_GetAnimalInfo(pUserInfo.AnimalLand[v_Steal[i]].Id,pAnimalGrow);
			strPrintf.Format("[QQ牧场]:收获了自己家%d号位的%s",pUserInfo.AnimalLand[v_Steal[i]].serial,pAnimalGrow.Name);
			Addtolist(strPrintf,COLOR_INFO);
			pUserInfo.AnimalLand[v_Steal[i]].Id = 0;
			pUserInfo.AnimalLand[v_Steal[i]].changeTime = 0;
			pUserInfo.AnimalLand[v_Steal[i]].status = 0;
		}
		if (nAddExp > 0)
		{
			strPrintf.Format("[QQ牧场]:共获得%d点经验",nAddExp);
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
//返回购买动物的ID
UINT H_GetBuyAnimalId(BOOL bWorP,BOOL &bIsRaise)
{
	UINT nAnimalId = 0;
	std::vector<ST_ANIMALGROW> v_Animals;
	std::vector<ST_ANIMALGROW> v_HaveAnimals;  //背包里有的动物
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
	std::sort(v_Animals.begin(),v_Animals.end(),C_AnimalsLvlCmp);	//根据等级排序
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
//获取用户动物数据
UINT H_GetAnimalsInfo(CString &strHtml,ST_USERINFO &pUserInfo)
{
	//获取动物数据
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
				//是否可偷取
				pUserInfo.AnimalLand[nCount].stealflag = 3;
				//已花费的生长时间
				if (FindMidStr(strAnimal,0,"\"growTime\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].growTime = atoi(strBuf);
				//距离下一状态需要的时间
				if (FindMidStr(strAnimal,0,"\"growTimeNext\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].growTimeNext = atoi(strBuf);
				//是否饥饿
				if (FindMidStr(strAnimal,0,"\"hungry\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].hungry = atoi(strBuf);
				//当前状态
				if (FindMidStr(strAnimal,0,"\"status\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].status = atoi(strBuf);
				//下一状态
				if (FindMidStr(strAnimal,0,"\"statusNext\":",",",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].statusNext = atoi(strBuf);
				//产物数量
				if (FindMidStr(strAnimal,0,"\"totalCome\":","",strBuf) != -1)
					pUserInfo.AnimalLand[nCount].totalCome = atoi(strBuf);
				//如果不饥饿,就计算一下成长时间
				if (pUserInfo.AnimalLand[nCount].hungry == FALSE && pUserInfo.AnimalLand[nCount].status < 6)
				{
					//计算进入下一阶段的时间
					pUserInfo.AnimalLand[nCount].changeTime = pUserInfo.AnimalLand[nCount].growTimeNext + dwServerTime;
				}
			}
			
			if (nOffset == -1)
				break;
			//进入下一次循环
			nOffset=FindMidStr(strAnimals,nOffset,"{","}",strAnimal);
		}
	}
	return TRUE;
}
//购买动物
UINT F_PastureBuyAnimal(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	if (m_Config.nRaiseAnimalType == 0)
		F_GetUserAnimals(NULL);
	//需要购买的动物数量
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
		//获取购买动物的ID
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
				//增加的经验
				int nAddExp = 0;
				FindMidStr(strHtml,0,"\"addExp\":",",",strBuf);
				nAddExp = atoi(strBuf);
				H_GetAnimalsInfo(strHtml,pUserInfo);
				CString strPrintf;
				strPrintf.Format("[QQ牧场]:饲养了1只%s(窝)",pAnimalGrow.Name);
				if (nAddExp > 0)
				{
					CString strAddExp;
					strAddExp.Format(",获得了%d点经验",nAddExp);
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
				//增加的经验
				int nAddExp = 0;
				FindMidStr(strHtml,0,"\"addExp\":",",",strBuf);
				nAddExp = atoi(strBuf);
				H_GetAnimalsInfo(strHtml,pUserInfo);
				//修改金币
				pUserInfo.Money -= pAnimalGrow.buyprice * nBuyWnum;
				CString strPrintf;
				strPrintf.Format("[QQ牧场]:购买了%d只%s(窝)",nBuyWnum,pAnimalGrow.Name);
				if (nAddExp > 0)
				{
					CString strAddExp;
					strAddExp.Format(",获得了%d点经验",nAddExp);
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
		//获取购买动物的ID
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
				//增加的经验
				int nAddExp = 0;
				FindMidStr(strHtml,0,"\"addExp\":",",",strBuf);
				nAddExp = atoi(strBuf);
				H_GetAnimalsInfo(strHtml,pUserInfo);
				//修改金币
				pUserInfo.Money -= pAnimalGrow.buyprice * nBuyPnum;
				CString strPrintf;
				strPrintf.Format("[QQ牧场]:饲养了1只%s(棚)",pAnimalGrow.Name);
				if (nAddExp > 0)
				{
					CString strAddExp;
					strAddExp.Format(",获得了%d点经验",nAddExp);
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
				//增加的经验
				int nAddExp = 0;
				FindMidStr(strHtml,0,"\"addExp\":",",",strBuf);
				nAddExp = atoi(strBuf);
				H_GetAnimalsInfo(strHtml,pUserInfo);
				//修改金币
				pUserInfo.Money -= pAnimalGrow.buyprice * nBuyPnum;
				CString strPrintf;
				strPrintf.Format("[QQ牧场]:购买了%d只%s(棚)",nBuyPnum,pAnimalGrow.Name);
				if (nAddExp > 0)
				{
					CString strAddExp;
					strAddExp.Format(",获得了%d点经验",nAddExp);
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
//消灭蚊子
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
			//增加的经验
			int nAddExp = 0;
			FindMidStr(strHtml,0,"\"addExp\":",",",strBuf);
			nAddExp = atoi(strBuf);
			CString strPrintf;
			if (pUserInfo.Id == m_SelfInfo.Id)
			{
				strPrintf.Format("[QQ牧场]:拍死了自己家的%d只蚊子,获得了%d点经验",pUserInfo.mosquito,nAddExp);
			}
			else
			{
				strPrintf.Format("[QQ牧场]:拍死了[%s]家的%d只蚊子,获得了%d点经验",pUserInfo.Name,pUserInfo.mosquito,nAddExp);
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
				strPrintf.Format("[QQ牧场]:拍死了自己家的%d只蚊子,获得了%d点经验",pUserInfo.mosquito,nAddExp);
			}
			else
			{
				strPrintf.Format("[QQ牧场]:拍死了[%s]家的%d只蚊子,获得了%d点经验",pUserInfo.Name,pUserInfo.mosquito,nAddExp);
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
			//增加的经验
			int nAddExp = 0;
			FindMidStr(strHtml,0,"\"addExp\":",",",strBuf);
			nAddExp = atoi(strBuf);
			CString strPrintf;
			if (pUserInfo.Id == m_SelfInfo.Id)
			{
				strPrintf.Format("[QQ牧场]:拍死了自己家的%d只蚊子,获得了%d点经验",pUserInfo.mosquito,nAddExp);
			}
			else
			{
				strPrintf.Format("[QQ牧场]:拍死了[%s]家的%d只蚊子,获得了%d点经验",pUserInfo.Name,pUserInfo.mosquito,nAddExp);
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
//清理粪便
UINT F_PastureClearShit(ST_USERINFO &pUserInfo)
{
	if (pUserInfo.shit > 0)
	{
		CString strPostData;
		strPostData.Format("pos=0&uId=%d&farmKey=null&num=%d&type=2&uIdx=%d",pUserInfo.Id,pUserInfo.shit,m_SelfInfo.Id);
		CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_help_pasture",strPostData);
		CString strBuf;
		//收集的便便
		int nRepNum = 0;
		FindMidStr(strHtml,0,"\"repNum\":",",",strBuf);
		nRepNum = atoi(strBuf);
		CString strPrintf;
		if (pUserInfo.Id == m_SelfInfo.Id)
		{
			strPrintf.Format("[QQ牧场]:收集了自己家的%d坨便便",nRepNum);
		}
		else
		{
			strPrintf.Format("[QQ牧场]:收集了[%s]家的%d坨便便",pUserInfo.Name,nRepNum);
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
//升级窝棚
UINT F_PastureUpHouse(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	CString strPostData;
	strPostData.Format("farmKey=null&act=query&level=%d&type=1&newitem=1&uIdx=%d",pUserInfo.wlvl,pUserInfo.Id);
	CString strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_up_animalhouse",strPostData);
	CString strBuf;
	//等级
	int nLevel = 0;
	//金币
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
		//是否成功
		int nCode = 0;
		FindMidStr(strHtml,0,"\"code\":",",",strBuf);
		nCode = atoi(strBuf);
		if (nCode == TRUE)
		{
			//查找窝等级
			if(FindMidStr(strHtml,0,"\"id\":102,\"lv\":","}",strBuf) != -1)
				pUserInfo.wlvl=atoi(strBuf);
			CString strPrintf;
			strPrintf.Format("[QQ牧场]:将窝升级到%d级,花费了%d个金币",pUserInfo.wlvl,nMoney);
			Addtolist(strPrintf,COLOR_INFO);
			nResult = TRUE;
		}
	}
	
	Sleep(2000);
	
	strPostData.Format("farmKey=null&act=query&level=%d&type=2&newitem=1&uIdx=%d",pUserInfo.plvl,pUserInfo.Id);
	strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_up_animalhouse",strPostData);
	
	//等级
	nLevel = 0;
	//金币
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
		//是否成功
		int nCode = 0;
		FindMidStr(strHtml,0,"\"code\":",",",strBuf);
		nCode = atoi(strBuf);
		if (nCode == TRUE)
		{
			//查找棚等级
			if(FindMidStr(strHtml,0,"\"id\":103,\"lv\":","}",strBuf) != -1)
				pUserInfo.plvl=atoi(strBuf);
			CString strPrintf;
			strPrintf.Format("[QQ牧场]:将棚升级到%d级,花费了%d个金币",pUserInfo.plvl,nMoney);
			Addtolist(strPrintf,COLOR_INFO);
			nResult = TRUE;
		}
	}
	if (nResult)
		G_WriteUserInfo(pUserInfo);
	return nResult;
}
//攻击野生动物
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
				//增加的人品值
				int nAddMoral = 0;
				FindMidStr(strHtml,0,"\"addmoral\":",",",strBuf);
				nAddMoral = atoi(strBuf);
				if (nAddMoral > 0)
				{
					//现有体力
					FindMidStr(strHtml,0,"\"leftblood\":",",",strBuf);
					pUserInfo.BeastLand[i].Blood = atoi(strBuf);
				}
				//返回结果
				int nResult = 0;
				FindMidStr(strHtml,0,"\"result\":",",",strBuf);
				nResult = atoi(strBuf);
				
				//返回时间
				FindMidStr(strHtml,0,"\"t\":","}",strBuf);
				pUserInfo.BeastLand[i].ReturnTime = atol(strBuf);
				
				ST_BEASTINFO pBeastInfo;
				G_GetBeastInfo(pUserInfo.BeastLand[i].Type,pBeastInfo);
	
				CString strUser;
				if (pUserInfo.Id == m_SelfInfo.Id)
					strUser = "自己";
				else
					strUser.Format("好友[%s]",pUserInfo.Name);
	
				//如果返回1,则击中
				if (nResult == 1)
				{
					CString strPrintf;
					strPrintf.Format("[QQ牧场]:击中了%s田里的%s(%d/%d),增加%d点人品",strUser,pBeastInfo.Name,pUserInfo.BeastLand[i].Blood,pBeastInfo.Blood,nAddMoral);
					Addtolist(strPrintf,COLOR_INFO);
				}else if (nResult == 2)
				{
					CString strPrintf;
					strPrintf.Format("[QQ牧场]:甩翻了%s田里的%s(%d/%d),增加%d点人品",strUser,pBeastInfo.Name,pUserInfo.BeastLand[i].Blood,pBeastInfo.Blood,nAddMoral);
					Addtolist(strPrintf,COLOR_INFO);
				}else if (nResult == 3)
				{
					CString strPrintf;
					strPrintf.Format("[QQ牧场]:攻击%s田里的%s(%d/%d)失败",strUser,pBeastInfo.Name,pUserInfo.BeastLand[i].Blood,pBeastInfo.Blood);
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
						//数量
						FindMidStr(strCrystal,0,"\"num\":",",",strBuf);
						nNum = atoi(strBuf);
						//时间
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
							strPrintf.Format("[QQ牧场]:捡到了%d块%s",nNum,pCrystalInfo.Name);
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
//处理我的牧场的数据
UINT H_HandleMyPasture(LPVOID pParam)
{
	ST_USERINFO pUserInfo;	//从配置文件中读取已经获取的数据
	G_GetUserInfo(m_SelfInfo.Id,pUserInfo);
	m_SelfInfo.Lvl = FarmExpToLevel(pUserInfo.Exp);	//更新一下我的等级

	UINT nAddFood = FALSE;	//是否添加饲料
	UINT nHarvest1 = FALSE; //是否收获幼崽
	UINT nHarvest2 = FALSE; //是否收获成熟的动物
	UINT nBuyAnimal = FALSE;//是否购买了动物
	UINT nClearMosquito = FALSE; //是否杀蚊子
	UINT nClearShit	= FALSE; //是否清理便便
	UINT nAttackBeast = FALSE;//是否有攻击野生动物

	//添加饲料
	if (m_Config.bAddFood)
	{
		nAddFood = F_PastureAddFood(pUserInfo);
		if (nAddFood)
			Sleep(2000);
	}
	//生产和收获幼崽
	if (m_Config.bHarvest)
	{
		nHarvest1 = F_PastureHarvest1(pUserInfo);
		if (nHarvest1)
			Sleep(2000);
		//收获成熟期的动物
		nHarvest2 = F_PastureHarvest2(pUserInfo);
		if (nHarvest2)
			Sleep(2000);

		m_SelfInfo.Lvl = FarmExpToLevel(pUserInfo.Exp);	//更新一下我的等级
	}
	//卖出仓库收获
	if (m_Config.bSaleAnimals && nHarvest2)
	{
		F_GetUserAnimals(NULL);
		Sleep(2000);
		F_SaleUserAnimals(NULL);
		Sleep(2000);
	}
	//升级窝棚
	if (m_Config.bUpHouse && nHarvest2)
	{
		F_PastureUpHouse(pUserInfo);
		Sleep(2000);
	}
	//购买动物
	if (m_Config.bBuyAnimal)
	{
		nBuyAnimal = F_PastureBuyAnimal(pUserInfo);
		if (nBuyAnimal)
			Sleep(2000);
	}
	//杀蚊子
	if (m_Config.bClearBad)
	{
		nClearMosquito = F_PastureClearMosquito(pUserInfo);
		if (nClearMosquito)
			Sleep(2000);
		//清便便
		nClearShit = F_PastureClearShit(pUserInfo);
		if (nClearShit)
			Sleep(2000);
	}

	nAttackBeast = F_PastureAttackBeast(pUserInfo); //攻击野生动物
	if (nAttackBeast)
		Sleep(2000);

	if (nHarvest1 || nHarvest2 || nBuyAnimal)
	{
		if (m_SelfLandId == m_SelfInfo.Id)
			S_ShowMyRipeTime();
	}

	return TRUE;
}
//偷取好友动物
UINT F_PastureSteal(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	std::vector<int> v_Steal;
		ST_ANIMALGROW pAnimalGrow;
		std::vector<LONG> v_CrowValue;
		//是否暂停15秒,等待生产结束
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
				//是否成功
				int nCode = 0;
				//增加的经验
				int nAddExp = 0;
				FindMidStr(strHtml,0,"\"code\":",",",strBuf);
				nCode = atoi(strBuf);
				FindMidStr(strHtml,0,"\"addExp\":",",",strBuf);
				nAddExp = atoi(strBuf);
				//阶段
				FindMidStr(strHtml,0,"\"status\":",",",strBuf);
				pUserInfo.AnimalLand[i].status = atoi(strBuf);
				//产量
				pUserInfo.AnimalLand[i].totalCome = 1;
				//存活时间
				FindMidStr(strHtml,0,"\"growTime\":",",",strBuf);
				pUserInfo.AnimalLand[i].growTime = atoi(strBuf);
				//下一阶段
				FindMidStr(strHtml,0,"\"statusNext\":",",",strBuf);
				pUserInfo.AnimalLand[i].statusNext = atoi(strBuf);
				//下一阶段时间
				FindMidStr(strHtml,0,"\"growTimeNext\":",",",strBuf);
				pUserInfo.AnimalLand[i].growTimeNext = atoi(strBuf);
				//是否饥饿
				FindMidStr(strHtml,0,"\"hungry\":",",",strBuf);
				pUserInfo.AnimalLand[i].hungry = atoi(strBuf);
				//修改偷取标志
				pUserInfo.AnimalLand[i].stealflag = 3;
				if (pUserInfo.AnimalLand[i].hungry == FALSE)
				{
					//计算进入下一阶段的时间
					pUserInfo.AnimalLand[i].changeTime = pUserInfo.AnimalLand[i].growTimeNext + dwServerTime;
				}
				//如果成功
				if (nCode == TRUE)
				{
					if (pAnimalGrow.Id != pUserInfo.AnimalLand[i].Id)
					{
						G_GetAnimalInfo(pUserInfo.AnimalLand[i].Id,pAnimalGrow);
						v_CrowValue.clear();	//清空动态数组
						v_CrowValue.push_back(pAnimalGrow.cropvalue[0]);	//幼崽
						v_CrowValue.push_back(pAnimalGrow.cropvalue[1]);	//成长期
						int nValue = pAnimalGrow.cropvalue[3] + pAnimalGrow.cropvalue[4];	//生产期间隔
						int nNum = pAnimalGrow.cropvalue[2] / nValue;		//生产次数
						for (int j=0;j<nNum;j++)	//nNum加1 是待收获期的时间
						{
							v_CrowValue.push_back(nValue);					//循环加入生产期时间
						}
					}
					CString strPrintf;
					strPrintf.Format("[QQ牧场]:将好友[%s]%d号位的%s赶去生产",pUserInfo.Name,pUserInfo.AnimalLand[i].serial,pAnimalGrow.Name);
					if (nAddExp > 0)
					{
						CString strAddExp;
						strAddExp.Format(",获得了%d点经验",nAddExp);
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
					v_CrowValue.clear();	//清空动态数组
					v_CrowValue.push_back(pAnimalGrow.cropvalue[0]);	//幼崽
					v_CrowValue.push_back(pAnimalGrow.cropvalue[1]);	//成长期
					int nValue = pAnimalGrow.cropvalue[3] + pAnimalGrow.cropvalue[4];	//生产期间隔
					int nNum = pAnimalGrow.cropvalue[2] / nValue;		//生产次数
					for (int j=0;j<nNum;j++)	//nNum加1 是待收获期的时间
					{
						v_CrowValue.push_back(nValue);					//循环加入生产期时间
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
					//计算进入下一阶段的时间
					pUserInfo.AnimalLand[i].changeTime = pUserInfo.AnimalLand[i].growTimeNext + dwServerTime;
				}
				bSleep = TRUE;
			}
	
			//将可收获的幼崽ID加入动态数组
			if (pUserInfo.AnimalLand[i].Id > 0 &&			//如果ID大于零
				pUserInfo.AnimalLand[i].totalCome > 0 &&	//产量大于零
				pUserInfo.AnimalLand[i].stealflag == 3)		//偷取标识为3
			{
				pUserInfo.AnimalLand[i].totalCome = 0;
				pUserInfo.AnimalLand[i].stealflag = 0;
				BOOL bSteal = FALSE;	//是否已存在收获ID数组中
				for (int j=0;j<v_Steal.size();j++)
				{
					if (v_Steal[j] == pUserInfo.AnimalLand[i].Id)	//循环查找数组中是否已存在相同ID
					{
						bSteal = TRUE;
						break;
					}
				}
				if (!bSteal)			//如果不存在则将ID加入数组
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
					strPrintf.Format("[QQ牧场]:偷取了好友[%s]的%s%s%s",pUserInfo.Name,strBuf,pAnimalGrow.Unit,pAnimalGrow.Product);
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
//给好友添加饲料
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
			//获取动物数据
			FindMidStr(strHtml,0,"\"addExp\":",",",strBuf);
			nAddExp = atoi(strBuf);
			if (nAddExp == -1 || nAddExp == 0)
				bAddFoodToFd = FALSE;
			strDirection = CFindMidStr(strHtml,"\"alert\":\"","\",");
			FindMidStr(strHtml,0,"\"total\":",",",strBuf);
			pUserInfo.animalFood = atoi(strBuf);
			
			CString strPrintf;
			strPrintf.Format("[QQ牧场]:为好友[%s]的牧场添加了饲料,获得了%d点经验",pUserInfo.Name,nAddExp);
			if (strDirection != "")
				strPrintf += "\r\n→" + N_HtmlToText(strDirection);
			Addtolist(strPrintf,COLOR_INFO);
			nResult = TRUE;
		}
	}
	if (nResult)
		G_WriteUserInfo(pUserInfo);
	return nResult;
}
//处理好友牧场数据
UINT H_HandleFriendPasture(LPVOID pParam)
{
	int nId = (int)pParam;
	ST_USERINFO pUserInfo;	//从配置文件中读取已经获取的数据
	G_GetUserInfo(nId,pUserInfo);

	UINT nSteal = FALSE;	//是否偷取了
	UINT nClearMosquito = FALSE; //是否杀蚊子
	UINT nClearShit	= FALSE; //是否清理便便
	UINT nAddFoodToFd = FALSE; //是否为好友添加饲料

	//为好友添加饲料
	if (bAddFoodToFd)
	{
		nAddFoodToFd = F_PastureAddFoodToFd(pUserInfo);
		if (nAddFoodToFd)
			Sleep(2000);
	}
	//偷取动物
	if (m_Config.bFdSteal)
	{
		if (m_Config.bFdSafety || pUserInfo.guard == -1 || pUserInfo.guard == 0)
		{
			nSteal = F_PastureSteal(pUserInfo);
			if (nSteal)
				Sleep(2000);
		}
	}
	//杀蚊子
	if (bClearMosquito && m_Config.bFdClearBad)
	{
		nClearMosquito = F_PastureClearMosquito(pUserInfo);
		if (nClearMosquito)
			Sleep(2000);
	}
	//清粪便
	if (bClearShit && m_Config.bFdClearBad)
	{
		nClearShit = F_PastureClearShit(pUserInfo);
		if (nClearShit)
			Sleep(2000);
	}
	
	if (F_PastureAttackBeast(pUserInfo) == TRUE)	//攻击野生动物
		Sleep(2000);
	
	return TRUE;
}
//扫描好友牧场数据,与下面的函数不同的是如果在四小时内,会自动读取INI中的数据
UINT F_ScanPastureInfo(LPVOID pParam)
{
	UINT nResult = FALSE;
	int nId = (int)pParam;	//获取ID
	ST_USERLIST pUserList;	//从配置文件中读取简易数据
	G_GetUserList(nId,pUserList);
	ST_USERINFO pUserInfo;	//从配置文件中读取已经获取的数据
	G_GetUserInfo(nId,pUserInfo);
	if (pUserList.CheckTime + 14400 < dwServerTime)
	{
		CString strHtml;
		CString strPostData;	//处理POST数据,访问好友牧场
		if (nId == m_SelfInfo.Id)	//如果是获取自己的牧场数据
			strPostData.Format("flag=1&uId=0&farmKey=null&newitem=2&uIdx=%d",nId);
		else
			strPostData.Format("farmKey=null&uId=%d&uIdx=%d&newitem=2&flag=1",nId,m_SelfInfo.Id);
		
		strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_enter",strPostData);
		
		H_GetPastureInfo(strHtml,pUserInfo);	//处理返回的牧场数据
		//将完整数据写入配置文件
		G_WriteUserInfo(pUserInfo);
		nResult = TRUE;
	}
	return nResult;
}
//获取指定用户的牧场数据
UINT F_GetPastureInfo(LPVOID pParam)
{
	int nId = (int)pParam;	//获取ID
	ST_USERINFO pUserInfo;	//从配置文件中读取已经获取的数据
	G_GetUserInfo(nId,pUserInfo);
	CString strHtml;
	CString strPostData;	//处理POST数据,访问好友牧场
	if (nId == m_SelfInfo.Id)	//如果是获取自己的牧场数据
		strPostData.Format("flag=1&uId=0&farmKey=null&newitem=2&uIdx=%d",nId);
	else
		strPostData.Format("farmKey=null&uId=%d&uIdx=%d&newitem=2&flag=1",nId,m_SelfInfo.Id);
	
	strHtml = N_PasturePostUrl("mc.qzone.qq.com","/cgi-bin/cgi_enter",strPostData);
	
	H_GetPastureInfo(strHtml,pUserInfo);	//处理返回的牧场数据
	//将完整数据写入配置文件
	G_WriteUserInfo(pUserInfo);
	return TRUE;
}

BOOL C_FriendsListCmp(ST_USERLIST pUserList1,ST_USERLIST pUserList2)
{
	return pUserList1.Lvl > pUserList2.Lvl;
}

//获取好友列表
UINT F_GetFriends(LPVOID pParam)
{
	CString strPostData;	//处理POST数据,获取所有好友
	strPostData.Format("farmKey=null&user=true&uIdx=%d",m_SelfInfo.Id);
	CString strHtml = N_PasturePostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_getFriendList?mod=friend",strPostData);
	
	if (!v_FriendsInfo.empty())	//简易好友列表是否为空
		v_FriendsInfo.clear();	//不是则清空
	
	CString strBuf;
	int nOffset = 0;
	nOffset = FindMidStr(strHtml,nOffset,"{","}",strBuf);
	while(nOffset!=-1)
	{
		CString strItem;
		CString strQQ;
		ST_USERINFO pUserInfo;
		//查找ID
		FindMidStr(strBuf,0,"\"uId\":",",",strItem);
		pUserInfo.Id = atoi(strItem);
		G_GetUserInfo(pUserInfo.Id,pUserInfo);
		//查找QQ
		FindMidStr(strBuf,0,"\"uin\":",",",strQQ);
		pUserInfo.QQ = atoi(strQQ);
		//查找经验
		FindMidStr(strBuf,0,"\"pastrueExp\":",",",strItem);
		pUserInfo.Exp = atol(strItem);
		//查找金币
		FindMidStr(strBuf,0,"\"money\":",",",strItem);
		pUserInfo.Money = atol(strItem);
		//是否开通牧场
		FindMidStr(strBuf,0,"\"pf\":",",",strItem);
		pUserInfo.pf = atoi(strItem);
		//查找昵称
		FindMidStr(strBuf,0,"\"userName\":\"","\"",strItem);
		strcpy(pUserInfo.Name,strItem.GetBuffer(strItem.GetLength()));
		strItem.ReleaseBuffer();
		if (nOffset == -1)
			break;
		if (pUserInfo.pf && strQQ != GetQQInfo(GQI_NUM))	//如果这个不是自己的ID才保存
		{
			//将牧场数据写入INI文件
			G_WriteUserInfo(pUserInfo);
			ST_USERLIST pUserList;		//简易数据
			G_GetUserList(pUserInfo.Id,pUserList);
			pUserList.Id = pUserInfo.Id;	//写入到好友列表配置文件
			pUserList.Lvl = FarmExpToLevel(pUserInfo.Exp);	//等级
			G_WriteUserList(pUserList);
			//将简易数据加入数组
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
	//取所有Section项,单个字符串以0结尾,整个字符串以00结尾
	GetPrivateProfileSectionNames(szAnimals,SHORTUSERLEN,strAnimalsIni);
	int nPos = 0;
	while(szAnimals[nPos] != 0)
	{
		strcpy(szAnimal,szAnimals+nPos);	//得到单个的Section
		ST_ANIMALGROW pAnimalGrow;
		G_GetAnimalInfo(atoi(szAnimal),pAnimalGrow);	//读取这个Section的数据
		pAnimalGrow.nPackage = 0;				//清空背包数量
		G_WriteAnimalInfo(pAnimalGrow);		//重新写入配置文件
		nPos += strlen(szAnimal) + 1;
	}
	m_Pastures = 0;		//清空我的背包牧草数量
	
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
				//数量
				FindMidStr(strBuf,0,"\"amount\":","",strItem);
				pAnimalGrow.nPackage = atoi(strItem);
				//等级
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
	//取所有Section项,单个字符串以0结尾,整个字符串以00结尾
	GetPrivateProfileSectionNames(szAnimals,SHORTUSERLEN,strAnimalsIni);
	int nPos = 0;
	while(szAnimals[nPos] != 0)
	{
		strcpy(szAnimal,szAnimals+nPos);	//得到单个的Section
		ST_ANIMALGROW pAnimalGrow;
		G_GetAnimalInfo(atoi(szAnimal),pAnimalGrow);	//读取这个Section的数据
		pAnimalGrow.nAnimals = 0;				//清空仓库数量
		pAnimalGrow.nWhelp = 0;
		G_WriteAnimalInfo(pAnimalGrow);		//重新写入配置文件
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
		BOOL bWhelp = TRUE;	//是幼崽还是成熟的动物
		int nId = 0;
		//获取ID
		if (FindMidStr(strBuf,0,"\"cId\":",",",strItem) != -1)
		{
			nId = atoi(strItem);
			if (nId > 10000)
			{
				bWhelp = FALSE;
				nId -= 10000;
			}
			//读取本地动物数据
			G_GetAnimalInfo(nId,pAnimalGrow);
			//数量
			FindMidStr(strBuf,0,"\"amount\":",",",strItem);
			if (bWhelp)		//根据类型写入不同的变量
				pAnimalGrow.nWhelp = atoi(strItem);
			else
				pAnimalGrow.nAnimals = atoi(strItem);
			//锁定状态
			FindMidStr(strBuf,0,"\"lock\":",",",strItem);
			pAnimalGrow.IsLock = atoi(strItem);
			//等级
			FindMidStr(strBuf,0,"\"lv\":",",",strItem);
			pAnimalGrow.Level = atoi(strItem);
			//销售价格
			FindMidStr(strBuf,0,"\"price\":",",",strItem);
			if (bWhelp)		//根据类型写入不同的变量
				pAnimalGrow.byproductprice = atoi(strItem);
			else
				pAnimalGrow.productprice = atoi(strItem);
			//类型
			FindMidStr(strBuf,0,"\"type\":","",strItem);
			pAnimalGrow.Type = atoi(strItem);
			//写入数据
			G_WriteAnimalInfo(pAnimalGrow);
		}
		
		if (nOffset == -1)
			break;
		
		nOffset = FindMidStr(strHtml,nOffset,"{","}",strBuf);
	}
	return TRUE;
}
//卖出用户仓库收获
UINT F_SaleUserAnimals(LPVOID pParam)
{
	//可卖出的数量
	int nNum = 0;
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
		//错误代码
		int nECode = 0;
		//收入金币
		int nMoney = 0;
		CString strDirection;	//提示文本
		
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
			CString strPrintf("[QQ牧场]:卖出仓库所有动物(未锁定的)");
			if (strDirection != "")
				strPrintf += "\r\n→" + N_HtmlToText(strDirection);
			Addtolist(strPrintf,COLOR_INFO);
		}
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//窗口显示函数															//
//////////////////////////////////////////////////////////////////////////
UINT S_ShowUserInfo(LPVOID pParam)
{
	int nId = (int)pParam;	//获取ID
	ST_USERINFO pUserInfo;	//从INI文件中读取指定ID的详细数据
	if (!G_GetUserInfo(nId,pUserInfo))
		return FALSE;
	strName = pUserInfo.Name;	//设置昵称
	int nLvl = FarmExpToLevel(pUserInfo.Exp);	//经验转换为等级
	strLevel.Format("%d",nLvl);					//设置等级
	strGold.Format("%d",pUserInfo.Money);		//设置金币
	strExp.Format("%d",pUserInfo.Exp);			//设置经验
	strAnimalFood.Format("%d",pUserInfo.animalFood);	//饲料
	strGuard = pUserInfo.guard != -1 ? "有":"无";	//守护
	int nNextLvlExp = nExpcmplist[nLvl + 1] - nExpcmplist[nLvl];	//剩余升级经验
	int nNeedExp = pUserInfo.Exp - nExpcmplist[nLvl];			
	strUpLvlExp.Format("%d/%d",nNeedExp,nNextLvlExp);	
	hDlg_Main->m_Pgs_UplvExp.SetRange(0,nNextLvlExp);	//设置到进度条
	hDlg_Main->m_Pgs_UplvExp.SetPos(nNeedExp);
	PostMessage(hWndMain,WM_UPDATEDATA,0,0);		//刷新窗口
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
		out_time.Format("%d小时%d分",nH,nM);
		hDlg_Main->m_List_Friends.SetItemText(nId,4,out_time);
	}
	else
	{
		hDlg_Main->m_List_Friends.SetItemText(nId,4,"");
	}
}
//显示好友牧场成熟时间
void S_ShowFriendsRipeTime()
{
	for (int i=0;i<v_FriendsInfo.size();i++)
	{
		S_ShowFriendsRipeTimeOnPos(i);		
	}
}
//显示我的牧场动物数据
void S_ShowMyRipeTime()
{
	hDlg_Main->m_List_Lands.DeleteAllItems();	//先清空列表
	ST_USERINFO pUserInfo;
	G_GetUserInfo(m_SelfLandId,pUserInfo);
	ST_ANIMALGROW pAnimalGrow;
	std::vector<LONG> v_CrowValue;
	int wNum,pNum;
	PastureHouseNum(pUserInfo.wlvl,pUserInfo.plvl,wNum,pNum);
	for (int i=0;i<wNum+pNum;i++)
	{
		int nId = pUserInfo.AnimalLand[i].Id;	//动物ID,0为空地
		if (nId > 0)
		{
			if (nId != pAnimalGrow.Id)	//如果动物数据不同,则读取新的
			{
				G_GetAnimalInfo(nId,pAnimalGrow);
				v_CrowValue.clear();	//清空动态数组
				v_CrowValue.push_back(pAnimalGrow.cropvalue[0]);	//幼崽
				v_CrowValue.push_back(pAnimalGrow.cropvalue[1]);	//成长期
				int nValue = pAnimalGrow.cropvalue[3] + pAnimalGrow.cropvalue[4];	//生产期间隔
				int nNum = pAnimalGrow.cropvalue[2] / nValue;		//生产次数
				for (int j=0;j<nNum;j++)
				{
					v_CrowValue.push_back(nValue);					//循环加入生产期时间
				}
			}

			char szNum[SHORTLEN]={0};
			itoa(pUserInfo.AnimalLand[i].serial,szNum,10);
			//编号
			hDlg_Main->m_List_Lands.InsertItem(i,szNum);
			//动物名称
			hDlg_Main->m_List_Lands.SetItemText(i,1,pAnimalGrow.Name);
			//等级
			ZeroMemory(szNum,SHORTLEN);
			itoa(pAnimalGrow.Level,szNum,10);
			hDlg_Main->m_List_Lands.SetItemText(i,2,szNum);
			//阶段
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
			//距下一状态的时间
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
					strBuf.Format("%d小时%d分",nH,nM);
					hDlg_Main->m_List_Lands.SetItemText(i,4,strBuf);
				}else if (nNum == 3)
				{
					hDlg_Main->m_List_Lands.SetItemText(i,4,"可生产");
				}else if (nNum == 4)
				{
					hDlg_Main->m_List_Lands.SetItemText(i,4,"生产中");
				}else if (nNum == 6)
				{
					hDlg_Main->m_List_Lands.SetItemText(i,4,"可收获");
				}
			}
			else
			{
				hDlg_Main->m_List_Lands.SetItemText(i,4,"饥饿");
			}
		}
		else
		{
			//编号
			hDlg_Main->m_List_Lands.InsertItem(i,"");
			//动物名称
			hDlg_Main->m_List_Lands.SetItemText(i,1,"未饲养");
		}
	}
	if (!v_CrowValue.empty())
		v_CrowValue.clear();
}

//////////////////////////////////////////////////////////////////////////
//数据存储函数															//
//////////////////////////////////////////////////////////////////////////
UINT G_GetUserInfo(int nId,ST_USERINFO &pUserInfo)
{
	char szuId[QQNUMLEN]={0};
	itoa(nId,szuId,10);
	return GetPrivateProfileStruct(szuId,"UserInfo",&pUserInfo,sizeof(ST_USERINFO),strFriendsDataIni);
}
UINT G_WriteUserInfo(ST_USERINFO &pUserInfo)
{
	char szuId[QQNUMLEN]={0};	//用户牧场ID
	itoa(pUserInfo.Id,szuId,10);	//转换ID为文本
	return WritePrivateProfileStruct(szuId,"UserInfo",&pUserInfo,sizeof(ST_USERINFO),strFriendsDataIni);
}
UINT G_GetUserList(int nId,ST_USERLIST &pUserList)
{
	char szuId[QQNUMLEN]={0};	//用户牧场ID
	itoa(nId,szuId,10);	//转换ID为文本
	return GetPrivateProfileStruct(szuId,"UserList",&pUserList,sizeof(ST_USERLIST),strFriendsListIni);
}
UINT G_WriteUserList(ST_USERLIST &pUserList)
{
	char szuId[QQNUMLEN]={0};	//用户牧场ID
	itoa(pUserList.Id,szuId,10);	//转换ID为文本	
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