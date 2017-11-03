// Dlg_Main.cpp : implementation file
//

#include "stdafx.h"
#include "QQFarm.h"
#include "Dlg_Main.h"	//主窗口
#include "Dlg_Config.h"	//配置窗口
#include "Dlg_Seeds.h"	//背包窗口
#include "Dlg_Crops.h"	//仓库窗口


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
#define IDT_SHOWMYRIPE		2				//刷新我的农场成熟时间
#define IDT_SHOWFRIENDSRIPE 3				//刷新好友列表成熟时间
#define IDT_GETSTATUSFILTER 4				//过滤好友状态
#define IDT_GETMYFARM		5				//刷新我的农场
#define IDT_GETFRIENDFARM	6				//刷新好友农场
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
CString						strDog;			//狗
CString						strHealthMode;	//安全模式
CString						strUpLvlExp;	//升级所需经验
int							numMin,numMax,numPos;
//WorkThread事件控制
BOOL						bGetBlogData = TRUE;	//从BLOG获取必要数据
BOOL						bGetServerTime = TRUE;	//获取农场服务器时间
BOOL						bGetXmlData	= TRUE;		//获取农场资源数据
BOOL						bGetFriends = TRUE;		//获取好友列表
BOOL						bScanFriends = TRUE;	//扫描好友农场
BOOL						bGetStatusFilter = FALSE; //过滤好友状态
BOOL						bWorking	= TRUE;		//工作状态
//HandleFarm事件控制
BOOL						bWcsFriends = TRUE;		//为好友除草杀虫浇水

ST_USERLIST					m_SelfInfo;			//我的农场,简易数据
std::vector<ST_USERLIST>	v_FriendsInfo;		//我的好友,简易数据
int							m_SelfLandId = 0;	//当前显示的ID
ST_GLOBALCONFIG				m_Config;			//全局配置
ST_MYBEASTINFO				m_MyBeast[3];		//我的野生动物数据

CString						m_gtk;				//Cookie Gtk
//////////////////////////////////////////////////////////////////////////
//前端定义																//
//////////////////////////////////////////////////////////////////////////
UINT N_RootThread(LPVOID pParam);		//启动初始化,工作,线程
UINT N_GetBlogData(LPVOID pParam);		//从BLOG获取必要数据
UINT N_GetXmlData(LPVOID pParam);		//获取资源版本,更新资源
UINT H_CheckPost(CString &strHtml);		//检查POST返回的数据,是否需特殊处理,返回0:无错误,返回1:重新登录,返回2:系统繁忙,返回3验证码
UINT H_WorkThread(LPVOID pParam);		//全局工作线程
UINT H_GetFarmInfo(CString &strHtml,ST_USERINFO &pUserInfo); //处理我的农场数据
UINT H_GetCropsInfo(int nId,ST_CROPSGROW &pCropGrow);	//从INI文件读取指定植物数据
DWORD H_GetFarmLandRipe(ST_CROPLANDINFO pCropLand[24]);	//处理农场田地数据,返回最近一次收成的时间
UINT H_HandleMyFarm(LPVOID pParam);		//处理我的农场的数据,播种收除都在这里处理
UINT H_HandleFriendFarm(LPVOID pParam);	//处理好友农场的数据,偷除在这里
UINT F_GetStatusFilter(LPVOID pParam);	//过滤好友状态
UINT F_ScanFarmInfo(LPVOID pParam);		//POST获取农场数据文本,使用H_GetFarmInfo进行处理
UINT F_GetFarmInfo(LPVOID pParam);
UINT F_GetFriends(LPVOID pParam);		//获取好友列表
UINT F_GetUserSeed(LPVOID pParam);		//获取背包数据
UINT F_GetUserCrop(LPVOID pParam);		//获取仓库数据
UINT S_ShowUserInfo(LPVOID pParam);		//显示数据到窗口上
void S_ShowFriendsRipeTimeOnPos(int nId);	//显示指定好友农场成熟时间
void S_ShowFriendsRipeTime();			//显示所有好友农场成熟时间
void S_ShowMyRipeTime();				//显示我的农场土地数据
UINT G_GetUserInfo(int nId,ST_USERINFO &pUserInfo);	//从INI文件读取详细用户数据
UINT G_WriteUserInfo(ST_USERINFO &pUserInfo);		//将详细用户数据写入INI
UINT G_GetUserList(int nId,ST_USERLIST &pUserList);	//从INI文件读取简易用户数据
UINT G_WriteUserList(ST_USERLIST &pUserList);		//将简易用户数据写入INI
UINT G_GetCropInfo(int nId,ST_CROPSGROW &pCropGrow);//从INI文件读取植物数据
UINT G_WriteCropInfo(ST_CROPSGROW &pCropGrow);		//将植物数据写入INI文件
UINT G_GetGlobalConfig(ST_GLOBALCONFIG &pConfig);
UINT G_WriteGlobalConfig(ST_GLOBALCONFIG &pConfig);
UINT G_GetCardsGame(int nId,ST_CARDSGAME &pCardsGame);
UINT G_WriteCardsGame(ST_CARDSGAME &pCardsGame);
UINT G_GetBeastInfo(int nId,ST_BEASTINFO &pBeastInfo);
UINT G_WriteBeastInfo(ST_BEASTINFO &pBeastInfo);
UINT G_GetCrystalInfo(int nId,ST_CRYSTALINFO &pCrystalInfo);
UINT G_WriteCrystalInfo(ST_CRYSTALINFO &pCrystalInfo);
UINT N_SetGlobalTimer(LPVOID pParam);				//设置全局计时器
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
	//绑定控件至控制器
	DDX_Control(pDX,IDC_LIST_LANDS,m_List_Lands);
	DDX_Control(pDX,IDC_LIST_FRIENDS,m_List_Friends);
	DDX_Control(pDX,IDC_PGS_UPLVLEXP,m_Pgs_UplvExp);
	//绑定控件文本
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

BOOL CDlg_Main::OnInitDialog()	//初始化窗口
{
	CDialog::OnInitDialog();	
	
	hWndMain = this->m_hWnd;
	//给土地列表框添加标头
	m_List_Lands.InsertColumn(0,"编号",NULL,40,-1);
	m_List_Lands.InsertColumn(1,"名称",NULL,100,-1);
	m_List_Lands.InsertColumn(2,"等级",NULL,40,-1);
	m_List_Lands.InsertColumn(3,"季",NULL,40,-1);
	m_List_Lands.InsertColumn(4,"距成熟",NULL,112,-1);
	//设置列表框整行选择和显示网格
	m_List_Lands.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	//给好友列表框添加标头
	m_List_Friends.InsertColumn(0,"编号",NULL,40,-1);
	m_List_Friends.InsertColumn(1,"昵称",NULL,90,-1);
	m_List_Friends.InsertColumn(2,"等级",NULL,40,-1);
	m_List_Friends.InsertColumn(3,"金币",NULL,70,-1);
	m_List_Friends.InsertColumn(4,"距成熟",NULL,110,-1);
	//设置列表框整行选择和显示网格
	m_List_Friends.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	//初始化经验对照表
	pInitExpCmpList();

	//显示QQ头像
	CC_CreateThread(pShowHeadPic,(LPVOID)atoi(GetQQInfo(GQI_NUM)),FALSE);

	//设置当前QQ的专用目录
	strSelfPath = GetAppPath(GAP_DIR) + GetQQInfo(GQI_NUM);
	CreateDirectory(strSelfPath,NULL);
	strCropsIni =		strSelfPath + "\\QQFarm_Crops.ini";
	strFriendsDataIni = strSelfPath + "\\QQFarm_FriendsData.ini";
	strFriendsListIni = strSelfPath + "\\QQFarm_FriendsList.ini";
	strQQFarm_ini =		strSelfPath + "\\QQFarm_Config.ini";
	strAddonIni	=		strSelfPath + "\\QQFarm_Addon.ini";
	strBeastIni =		strSelfPath + "\\QQFarm_Beast.ini";
	strCrystalIni =		strSelfPath + "\\QQFarm_Crystal.ini";

	//检查版本数据,如果不同则执行更新处理
	CString DataVersion;
	GetPrivateProfileString("version","dataversion",NULL,DataVersion.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQFarm_ini);
	DataVersion.ReleaseBuffer();
	int nDataVersion = atoi(DataVersion);
	if (nDataVersion == 0)
	{
		//强制更新扩展数据
		WritePrivateProfileString("xmldata","config_addon","",strQQFarm_ini);
		nDataVersion ++;
	}
	if (nDataVersion == 1)
	{
		//全局配置有变化,提示重新设置
		CC_PopTrayMsg("QQ农场","全局配置有变动,请重新设置",PTM_INFO,2000);
		nDataVersion ++;
	}
	//写入当前版本数据
	WritePrivateProfileString("version","dataversion","2",strQQFarm_ini);

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
		bWcsFriends = TRUE;		//继续为好友除草杀虫
		bGetXmlData = TRUE;
		N_SetGlobalTimer(NULL);	//重新设置所有计时器
		break;
	case IDT_WORKTIME:
		CC_Addtolist("[QQ农场]:工作时间结束,准备暂停",COLOR_INFO);
		bWorking = FALSE;
		KillTimer(IDT_WORKTIME);
		SetTimer(IDT_SLEEPTIME,50*60*1000,NULL);
		break;
	case IDT_SLEEPTIME:
		CC_Addtolist("[QQ农场]:暂停时间结束,准备继续工作",COLOR_INFO);
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
		SetDlgItemText(IDC_MAIN_BTN_MYFARM,"我的农场");
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
void CDlg_Main::OnMainBtnMyfarm() 
{
	if (m_SelfLandId == m_SelfInfo.Id)
	{
		m_SelfInfo.RipeTime = dwServerTime - 1;
	}
	else
	{
		SetDlgItemText(IDC_MAIN_BTN_MYFARM,"刷新农场");
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
CString N_FarmPostUrl(CString strHost,CString strUrl,CString strPostData)
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
		//错误循环次数
		int nErrorLoop = 0;
		//以当前时间获取KEY密钥
		CString strVCode;	//验证码
M1:
		if (nErrorLoop > 2)
		{
			strHtml = "";
			break;
		}
		time_t time;
		CString strFarmKey = FarmTimeMD5FarmKey(time);
		CString szPostData;	//处理POST数据
		szPostData.Format("farmKey=%s&farmTime=%d&%s",strFarmKey,(DWORD)time,strPostData);
		if (strVCode != "")	//如果验证码不为空,则添加到POST数据
		{
			szPostData = szPostData + "&validatemsg=" + strVCode;
			strVCode = "";		//清空验证码
		}
		//POST提交,等待返回文本
		strHtml = PostUrl(strHost,strUrl,strPostHeader,szPostData,TRUE);
		switch (H_CheckPost(strHtml))	//检查错误,根据对应错误进行处理
		{
		case 1: //需要重新登录
			//MessageBox(hWndMain,"需要重新登录","提示",0);
			::SendMessage(CChWndMain,EVENT_ALL_RELOGIN,0,0);	
			goto M1;	//跳到M1流程再次执行
		case 2: //系统繁忙,稍候再试
			CC_Addtolist("[QQ农场]:系统繁忙,获取数据失败,一分钟后重试.",COLOR_ERROR);
			Sleep(60000); //暂停一分钟
			nErrorLoop++;
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
UINT N_SetGlobalTimer(LPVOID pParam)
{
	KillTimer(hWndMain,IDT_GETMYFARM);
	SetTimer(hWndMain,IDT_GETMYFARM,7200000,NULL);	//两小时刷新我的农场

	KillTimer(hWndMain,IDT_GETSTATUSFILTER);
	SetTimer(hWndMain,IDT_GETSTATUSFILTER,7200000,NULL);  //两个小时过滤一下好友状态

	KillTimer(hWndMain,IDT_GETFRIENDFARM);
	SetTimer(hWndMain,IDT_GETFRIENDFARM,43200000,NULL);	 //十二个小时刷新所有好友农场

	CTime tNow;
	tNow = CTime::GetCurrentTime();
	int nH = tNow.GetHour();
	int nM = tNow.GetMinute();
	int nS = tNow.GetSecond();
	int nNow = (nH*60*60)+(nM*60)+(nS);	//当前时间
	nNow = 86400 - nNow + 300;
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
	
	//查询农场KEY
	nPos1 = szHtml.Find("[farmkey]");
	nPos2 = szHtml.Find("[/farmkey]",nPos1);
	if (nPos1 == -1 || nPos2 == 1)
		return FALSE;
	strQqFarmKey = szHtml.Mid(nPos1 + 9,nPos2 - nPos1 - 9);
	//解密农场KEY
	strQqFarmKey=UncrypKey(strQqFarmKey,"");
	
	return TRUE;
}
//获取资源数据
UINT N_GetXmlData(LPVOID pParam)
{
	//获取主页面数据
	CString strUrl;
	strUrl.Format("http://appbase.qzone.qq.com/cgi-bin/index/appbase_run_unity.cgi?appid=353&max=0&uin=%s&keyname=353&params=&qz_style=100&canvastype=home",GetQQInfo(GQI_NUM));
	CString szHtml = GetUrl(strUrl,TRUE);
	//查询最新版的XML
	CString	strConfig_url;
	CString	strConfig_data;
	CString strConfig_addon;
	if(FindMidStr(szHtml,0,"\"config_url\":\"","\"",strConfig_url) == -1 ||
		FindMidStr(szHtml,0,"\"config_data\":\"","\"",strConfig_data) == -1 ||
		FindMidStr(szHtml,0,"\"config_addon\":\"","\"",strConfig_addon) == -1)
		return FALSE;

	//读取本地配置
	G_GetGlobalConfig(m_Config);

	//从配置读取记录的XML版本
	CString strConfig_url_ini;
	CString strConfig_data_ini;
	CString strConfig_addon_ini;
	GetPrivateProfileString("xmldata","config_url",NULL,strConfig_url_ini.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQFarm_ini);
	strConfig_url_ini.ReleaseBuffer();
	GetPrivateProfileString("xmldata","config_data",NULL,strConfig_data_ini.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQFarm_ini);
	strConfig_data_ini.ReleaseBuffer();
	GetPrivateProfileString("xmldata","config_addon",NULL,strConfig_addon_ini.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQFarm_ini);
	strConfig_addon_ini.ReleaseBuffer();

	//将主页面XML版本和本地存储的XML版本进行对比,不同则更新SWF数据
	CString strModuleUrl;
	CString strLoadingUrl;
	if (strConfig_url != strConfig_url_ini)
	{
		CC_Addtolist("[QQ农场]:发现有更新版本,开始解析最新协议",COLOR_INFO);
		CC_PopTrayMsg("[QQ农场]","发现有更新版本,开始解析最新协议",PTM_INFO,2000);
		//下载版本XML和植物数据XML
		CString strConfig_url_xml = GetAppPath(GAP_DIR)+"Load_Main.xml";
		if (!GetUrlToFile(strConfig_url,strConfig_url_xml,TRUE))
			return FALSE;
		//开始分析XML内容
		CMarkup xml;
		if (!xml.Load(strConfig_url_xml))			//加载XML
			return FALSE;
		xml.ResetMainPos();							//重置搜索位置,这个类的玩意
		xml.FindElem("data");						//向下查找data元素
		xml.IntoElem();								//进入元素内部
		xml.FindElem("loading");					//在data内部查找loading元素
		strLoadingUrl=xml.GetAttrib("url");			//获取loading元素的url元件
		xml.FindElem("moduleList");					//在data内部查找modulelist元素
		xml.IntoElem();								//进入modulelist元素内部
		xml.FindElem("module");						//在内部查找module元素
		strModuleUrl=xml.GetAttrib("url");			//获取module元素的url元件
		DeleteFile(strConfig_url_xml);				//删除XML文件
		WritePrivateProfileString("xmldata","config_url",strConfig_url,strQQFarm_ini);
		WritePrivateProfileString("xmldata","loading",strLoadingUrl,strQQFarm_ini);
		WritePrivateProfileString("xmldata","module",strModuleUrl,strQQFarm_ini);
	}
	else
	{
		//如果相同,则从配置中读取数据
		GetPrivateProfileString("xmldata","loading",NULL,strLoadingUrl.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQFarm_ini);
		strLoadingUrl.ReleaseBuffer();
		GetPrivateProfileString("xmldata","module",NULL,strModuleUrl.GetBufferSetLength(MAX_PATH),MAX_PATH,strQQFarm_ini);
		strModuleUrl.ReleaseBuffer();
	}
	//这个数据在农场功能中,POST提交时必需的,需要将module元素取到的url加入其中
	strPostHeader = "Accept: */*\r\nReferer: " + strModuleUrl + "\r\nx-flash-version: 10,2,152,32\r\nAccept-Language: zh-cn\r\nContent-Type: application/x-www-form-urlencoded\r\nAccept-Encoding\r\n";
	
M1:
	CString strPostData;	//处理POST数据
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

	//将返回文本交由指定函数处理,返回自定义结构的数据
	ST_USERINFO pUserInfo;
	H_GetFarmInfo(szHtml,pUserInfo);
	CC_GetQQInfo(pUserInfo.Name,GQI_NAME);	//返回的数据中取不到昵称,使用公共函数取一下
	pUserInfo.IsQzone = TRUE;				//判定为空间

	G_WriteUserInfo(pUserInfo);				//存储用户数据到硬盘
	S_ShowUserInfo((LPVOID)pUserInfo.Id);	//显示用户信息到窗口

	//将必要数据写入简易用户数据结构,存储于内存中
	m_SelfInfo.Id = pUserInfo.Id;
	m_SelfInfo.Lvl = FarmExpToLevel(pUserInfo.Exp);
	m_SelfInfo.RipeTime = H_GetFarmLandRipe(pUserInfo.CropLand);
	m_SelfInfo.CheckTime = dwServerTime;

	//将主页面XML版本和本地存储的XML版本进行对比,不同则更新植物数据
	if (strConfig_data != strConfig_data_ini)
	{
		CC_Addtolist("[QQ农场]:农作物数据有变化,开始下载最新数据",COLOR_INFO);
		//下载植物数据XML到本地
		CString strConfig_data_xml = GetAppPath(GAP_DIR)+"CropGrow.xml";
		if (!GetUrlToFile(strConfig_data,strConfig_data_xml,TRUE))
			return FALSE;
		CMarkup xml;	//开始加载XML
		if (!xml.Load(strConfig_data_xml))
			return FALSE;
		xml.ResetMainPos();		//重置搜索位置,这个类的玩意
		xml.FindElem("data");	//查找data元素
		xml.IntoElem();			//进入data元素内部
		xml.FindElem("crops");	//查找crops元素
		CString strCrops=xml.GetData();	//获取crops元素的内容

		Sleep(2000);
		//获取商店植物数据,完善XML缺少的数据
		strPostData = "";
		strPostData.Format("uIdx=%d&uinY=%s",m_SelfInfo.Id,GetQQInfo(GQI_NUM));
		CString strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_getseedinfo?mod=usertool&act=getSeedInfo",strPostData);

		//准备处理所有植物数据
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

		DeleteFile(strConfig_data_xml);		//删除XML文件
		WritePrivateProfileString("xmldata","config_data",strConfig_data,strQQFarm_ini);
	}

	//将主页面XML版本和本地存储的XML版本进行对比,不同则更新扩展数据
	if (strConfig_addon != strConfig_addon_ini)
	{
		CC_Addtolist("[QQ农场]:扩展数据有变化,开始下载最新数据",COLOR_INFO);
		//下载扩展数据XML到本地
		CString strConfig_addon_xml = GetAppPath(GAP_DIR)+"addon.xml";
		if (!GetUrlToFile(strConfig_addon,strConfig_addon_xml,TRUE))
			return FALSE;
		CMarkup xml;	//开始加载XML
		if (!xml.Load(strConfig_addon_xml))
			return FALSE;
		xml.ResetMainPos();		//重置搜索位置,这个类的玩意
		xml.FindElem("data");
		xml.IntoElem();
		xml.FindElem("vars");
		xml.IntoElem();
		//获取野生动物XML的URL
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
		//下载野生动物数据
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
		DeleteFile(strConfig_addon_xml);		//删除XML文件
		DeleteFile(strConfig_wild_xml);
		WritePrivateProfileString("xmldata","config_addon",strConfig_addon,strQQFarm_ini);
		WritePrivateProfileString("xmldata","config_wild",strConfig_wild,strQQFarm_ini);
	}

	m_SelfLandId = m_SelfInfo.Id;
	S_ShowMyRipeTime();
	//设置刷新我的农场土地数据定时器
	SetTimer(hWndMain,IDT_SHOWMYRIPE,60000,NULL);
	//设置刷新我的好友列表即将成熟的定时器
	SetTimer(hWndMain,IDT_SHOWFRIENDSRIPE,60000,NULL);
	//处理一下农场数据
	if (H_HandleMyFarm(NULL))
		S_ShowUserInfo((LPVOID)m_SelfLandId);

	if (szHtml.Find("\"d\":1,",0) != -1)	//有礼包可领取
	{
		strPostData.Format("uIdx=%d&uinY=%s&uin=%s&uinX=",m_SelfInfo.Id,GetQQInfo(GQI_NUM),GetQQInfo(GQI_NUM));
		N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_getpackage_mini",strPostData);
		CC_Addtolist("[QQ农场]:领取了每日礼包",COLOR_INFO);
	}

	strPostData.Format("uinY=%s&uIdx=%d",GetQQInfo(GQI_NUM),pUserInfo.Id);
	szHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_login_home",strPostData);
	Sleep(1000);
	int Num = 0;
	FindMidStr(szHtml,0,"\"number\":",",",strBuf);
	//翻牌的次数
	Num = atoi(strBuf);
	//如果次数为零
	if (Num == 0)
	{
		szHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_login_click",strPostData);
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
				strPrintf.Format("[QQ农场]:连续签到%d天,获得%d个%s",nDays,pCardsGame.Num,pCardsGame.Name);
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
				strPrintf.Format("[QQ农场]:参加幸运九宫格,获得%d个%s",pCardsGame.Num,pCardsGame.Name);
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
				CC_Addtolist("[QQ农场]:获取农场加密协议",COLOR_INFO);
				while(!N_GetBlogData(NULL)){Sleep(5000);};
				Sleep(3000);
			}
			if (bGetServerTime)
			{
				bGetServerTime = FALSE;
				//获取服务器时间
				CC_Addtolist("[QQ农场]:获取农场服务器时间",COLOR_INFO);
				while(!pGetServerTime()){Sleep(5000);};
				Sleep(3000);
			}
			if (bGetXmlData)
			{
				bGetXmlData = FALSE;
				//从农场主页面获取各类资源
				CC_Addtolist("[QQ农场]:检查农作物数据是否需要更新",COLOR_INFO);
				while(!N_GetXmlData(NULL)){Sleep(5000);};
				Sleep(3000);
			}
			//获取好友列表
			if (bGetFriends)	
			{
				bGetFriends = FALSE;
				CC_Addtolist("[QQ农场]:获取农场好友列表",COLOR_INFO);
				F_GetFriends(NULL);
				//hDlg_Main->m_List_Friends.SetRedraw(FALSE);
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
				//hDlg_Main->m_List_Friends.SetRedraw(TRUE);
				Sleep(3000);
			}
			//扫描好友农场
			if (bScanFriends)
			{
				bScanFriends = FALSE;
				CC_Addtolist("[QQ农场]:开始分析好友的农场数据",COLOR_INFO);
				for (int i=0;i<v_FriendsInfo.size();i++)	//循环读取简易好友列表数组 
				{
					if (v_FriendsInfo[i].Lvl < m_Config.nIgnoreFdLvl)
						continue;
					ST_USERINFO pUserInfo;					//从INI文件中读出获取的数据
					G_GetUserInfo(v_FriendsInfo[i].Id,pUserInfo);
					CString strText;
					strText.Format("正在分析好友[%s]的农场",pUserInfo.Name);
					SetDlgItemText(hWndMain,IDC_MAIN_STATETEXT,strText);	//设置窗口状态标签
					strText.Format("%d/%d",i+1,v_FriendsInfo.size());		//设置进度
					SetDlgItemText(hWndMain,IDC_MAIN_SCANPOS,strText);
					BOOL bSleep = F_ScanFarmInfo((LPVOID)v_FriendsInfo[i].Id);	//获取好友农场,会自动保存至INI文件,如果从INI中读的则返回FALSE
					if (bSleep)
						H_HandleFriendFarm((LPVOID)v_FriendsInfo[i].Id);		///如果是从网络读取则处理好友农场数据
					G_GetUserInfo(v_FriendsInfo[i].Id,pUserInfo);			//从INI文件再次读出数据
					v_FriendsInfo[i].RipeTime = H_GetFarmLandRipe(pUserInfo.CropLand); //获取农场最近成熟的时间,没有则返回0
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
				m_Config.bSelfSc)
			{
				m_SelfInfo.RipeTime = 0;
				CC_Addtolist("[QQ农场]:开始刷新我的农场",COLOR_INFO);
				F_GetFarmInfo((LPVOID)m_SelfInfo.Id);
				Sleep(1000);
				H_HandleMyFarm(NULL);
				ST_USERINFO pUserInfo;	//从INI文件中读出获取的数据
				G_GetUserInfo(m_SelfInfo.Id,pUserInfo);
				m_SelfInfo.Lvl = FarmExpToLevel(pUserInfo.Exp);
				m_SelfInfo.RipeTime = H_GetFarmLandRipe(pUserInfo.CropLand);
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
					m_Config.bFdSc)
				{
					v_FriendsInfo[i].RipeTime = 0;
					if (m_Config.bFdSc || m_Config.bFdWcs)
					{
						F_GetFarmInfo((LPVOID)v_FriendsInfo[i].Id);
						Sleep(1000);
						H_HandleFriendFarm((LPVOID)v_FriendsInfo[i].Id);
						ST_USERINFO pUserInfo;	//从INI文件中读出获取的数据
						G_GetUserInfo(v_FriendsInfo[i].Id,pUserInfo);
						v_FriendsInfo[i].RipeTime = H_GetFarmLandRipe(pUserInfo.CropLand);
						v_FriendsInfo[i].CheckTime = dwServerTime;				//记录一下处理的时间
						G_WriteUserList(v_FriendsInfo[i]);						//写入INI文件
						S_ShowFriendsRipeTimeOnPos(i);							//显示一下距成熟还有多久,没有则空
						Sleep(3000);
					}
				}
			}
			//过滤好友状态
			if (bGetStatusFilter)
			{
				bGetStatusFilter = FALSE;
				if (m_Config.bFdWcs)
				{
					CC_Addtolist("[QQ农场]:检查需要帮忙除草,杀虫,浇水的好友",COLOR_INFO);
					F_GetStatusFilter(NULL);
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
//农场功能函数															//
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
		if (pCropLand[i].a > 0 && pCropLand[i].n == TRUE && pCropLand[i].b < 6)	//如果不是空地,而且没有偷过,则进入
		{
			ST_CROPSGROW pCropGrow;
			G_GetCropInfo(pCropLand[i].a,pCropGrow);		//获取这块地的植物数据
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
	CString strBuf;	//临时文本
		CString strQQ;

		//查找UID
		if (FindMidStr(strHtml,0,"\"uId\":",",",strBuf) != -1)
		pUserInfo.Id=atoi(strBuf);
		//查找QQ
		if(FindMidStr(strHtml,0,"\"uinLogin\":",",",strQQ) != -1)
		pUserInfo.QQ=atoi(strQQ);
		//查找金币
		if(FindMidStr(strHtml,0,"\"money\":",",",strBuf) != -1)
		pUserInfo.Money=atol(strBuf);
		//查找经验
		if(FindMidStr(strHtml,0,"\"exp\":",",",strBuf) != -1)
		pUserInfo.Exp=atol(strBuf);
		//是否开启牧场
		if(FindMidStr(strHtml,0,"\"pf\":",",",strBuf) != -1)
		pUserInfo.pf=atoi(strBuf);
		//人品值
		if(FindMidStr(strHtml,0,"\"moralexp\":",",",strBuf) != -1)
		pUserInfo.MoralExp=atoi(strBuf);
		//查找黄钻等级
		/*if (if(FindMidStr(strHtml,0,"\"yellowlevel\":",",",strBuf) == 1)
			return FALSE;
		pUserInfo.YellowLevel=atoi(strBuf) != -1)
		//查找黄钻状态
		if (if(FindMidStr(strHtml,0,"\"yellowstatus\":",",",strBuf) == 1)
			return FALSE;
		pUserInfo.YellowStatus=atoi(strBuf) != -1)*/
		//安全模式是否开启
		if(FindMidStr(strHtml,0,"\"set\":",",",strBuf) != -1)
		pUserInfo.HealthMode=atoi(strBuf);
		//安全模式开启时间
		if(FindMidStr(strHtml,0,"\"beginTime\":",",",strBuf) != -1)
		pUserInfo.BeginTime=atol(strBuf);
		//安全模式结束时间
		if(FindMidStr(strHtml,0,"\"endTime\":","}",strBuf) != -1)
		pUserInfo.EndTime=atol(strBuf);
		//狗的状态,-1没有狗,0没有狗粮,1有狗粮
		if(FindMidStr(strHtml,0,"\"isHungry\":","}",strBuf)==-1)
			pUserInfo.Dog = -1;
		else
			pUserInfo.Dog=atoi(strBuf)==0 ? 1:0;
	
		//获取田地植物数据
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
				//进入下一次循环
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
				
				//主人QQ
				if (FindMidStr(strBeast,0,"\"fid\":",",",strBuf) != -1)
					pUserInfo.BeastLand[nCount].fId = atoi(strBuf);
				//动物ID
				if (FindMidStr(strBeast,0,"\"id\":",",",strBuf) != -1)
					pUserInfo.BeastLand[nCount].Id = atoi(strBuf);
				//动物类型
				if (FindMidStr(strBeast,0,"\"type\":",",",strBuf) != -1)
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
				if (strBeast.Find(strFindQQ,0) == -1)
					pUserInfo.BeastLand[nCount].bAttack = TRUE;
	
				if (nOffset == -1)
					break;
				//进入下一次循环
				nOffset=FindMidStr(strBeasts,nOffset,"{","],\"status\"",strBeast);
			}
		}
		//如果是自己的农场,则获取一下野生动物数据
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
					//位置ID
					int nId = 0;
					//动物ID
					int nType = 0;
	
					//位置ID
					if (FindMidStr(strBeast,0,"\"id\":",",",strBuf) != -1)
					{
						nId = atoi(strBuf);
						m_MyBeast[nId].Id = nId;
					}
					//动物ID
					if (FindMidStr(strBeast,0,"\"type\":",",",strBuf) != -1)
					{
						nType = atoi(strBuf);
						m_MyBeast[nId].Type = nType;
					}
					//读取野生动物数据
					if (pBeastInfo.Id != nType)
						G_GetBeastInfo(nType,pBeastInfo);
					//剩余放养次数
					if (FindMidStr(strBeast,0,"\"raisenum\":",",",strBuf) != -1)
						m_MyBeast[nId].LeftRaise = pBeastInfo.RaiseTimes - atoi(strBuf);
					//野生动物状态
					if (FindMidStr(strBeast,0,"\"status\":",",",strBuf) != -1)
						m_MyBeast[nId].Status = atoi(strBuf);
					//领养时间
					if (FindMidStr(strBeast,0,"\"adopttime\":",",",strBuf) != -1)
						m_MyBeast[nId].AdoptTime = atol(strBuf);
					//返回时间
					if (FindMidStr(strBeast,0,"\"returntime\":",",",strBuf) != -1)
						m_MyBeast[nId].ReturnTime = atol(strBuf);
	
					if (nOffset == -1)
						break;
					//进入下一次循环
					nOffset=FindMidStr(strBeasts,nOffset,"{","}",strBeast);
				}
			}
		}
	return TRUE;
}
//收获自己农场成熟的作物
UINT F_FarmHarvest(ST_USERINFO &pUserInfo)
{
	std::vector<int> v_Land;
	
		for (int i=0;i<24;i++)
		{	//循环查看是否有可偷取的植物
			if (pUserInfo.CropLand[i].a > 0 && pUserInfo.CropLand[i].b == 6 && pUserInfo.CropLand[i].n == 1)
			{
				v_Land.push_back(i);
			}
		}
		//如果没有可偷取,则返回假
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
	
			int nCode = 0;			//code为1则成功
			CString strDirection;	//提示文本
			int nExp = 0;			//增加的经验
			int nIndex = 0;			//土地索引
			int nHarvest = 0;		//收获数量
			int ncId = 0;			//植物ID
			int nCropStatus = 0;	//新的阶段
			int nHarvestTimes = 0;	//新的季度
			int nHealth = 0;		//优秀程序
	
			int nHumidity = 0;		//湿度
			int nPest = 0;			//害虫
			int nWeed = 0;			//杂草
			DWORD dwPlantTime = 0;	//播种时间
			//int nThief = 0;			//是否可偷取
			
			FindMidStr(strSub,0,"\"code\":",",",strBuf);	//code为1则成功		   
				nCode = atoi(strBuf);
			//FindMidStr(strSub,0,"\"direction\":\"","\"",strDirection); //提示文本
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
					strPrintf.Format("[QQ农场]:收获了自己%d号田里的%d个%s,增加了%d点经验",nIndex+1,nHarvest,pCropGrow.Name,nExp);
					if (strDirection != "")
						strPrintf += "\r\n→" + N_HtmlToText(strDirection);
					Addtolist(strPrintf,COLOR_INFO);
				}
			}
	
			if (nOffset == -1)
				break;
			//进入下一次循环
			nOffset=FindMidStr(strHtml,nOffset,"{","}}",strSub);
		}
		G_WriteUserInfo(pUserInfo);
		if (!v_Land.empty())
			v_Land.clear();
	return TRUE;
}
//对枯萎的植物进行翻土
UINT F_FarmScarify(ST_USERINFO &pUserInfo)
{
	std::vector<int> v_Land;
	
	for (int i=0;i<24;i++)
	{	//循环查看是否有需要翻土的田
		if (pUserInfo.CropLand[i].a > 0 && pUserInfo.CropLand[i].b == 7)
		{
			v_Land.push_back(i);
		}
	}
	//如果没有可翻土,则返回假
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
		
		int nCode = 0;			//code为1则成功
		CString strDirection;	//提示文本
		int nExp = 0;			//增加的经验
		int nIndex = 0;			//土地索引
		
		FindMidStr(strHtml,0,"\"code\":",",",strBuf);	//code为1则成功		   
		nCode = atoi(strBuf);
		//FindMidStr(strSub,0,"\"direction\":\"","\"",strDirection); //提示文本
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
				strPrintf.Format("[QQ农场]:对自己%d号田进行了翻土,获得了%d点经验",nIndex+1,nExp);
				if (strDirection != "")
					strPrintf += "\r\n→" + N_HtmlToText(strDirection);
				Addtolist(strPrintf,COLOR_INFO);
			}
		}
	}
	
	G_WriteUserInfo(pUserInfo);
	if (!v_Land.empty())
		v_Land.clear();
	return TRUE;
}
//购买种子
UINT F_FarmBuySeed(int nId, int nNum)
{
	CString strPostData;
	strPostData.Format("uIdx=%d&uinY=%s&number=%d&cId=%d",m_SelfInfo.Id,GetQQInfo(GQI_NUM),nNum,nId);
	CString strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_buyseed?mod=repertory&act=buySeed",strPostData);
	CString strBuf;
	
	int nCode = 0;			//code为1则成功
	CString strDirection;	//提示文本
	int nMoney = 0;			//减少的金币
	CString strName;		//植物名称
	
	FindMidStr(strHtml,0,"\"code\":",",",strBuf);	//code为1则成功		   
	nCode = atoi(strBuf);
	//FindMidStr(strSub,0,"\"direction\":\"","\"",strDirection); //提示文本
	strDirection = CFindMidStr(strHtml,"\"direction\":\"","\",");
	
	FindMidStr(strHtml,0,"\"money\":",",",strBuf);
	nMoney = atoi(strBuf);
	FindMidStr(strHtml,0,"\"cName\":",",",strName);
	
	if (nCode == TRUE)
	{
		ST_USERINFO pUserInfo;	
		G_GetUserInfo(m_SelfInfo.Id,pUserInfo);
		pUserInfo.Money += nMoney;	//将金币更新至自己的数据
		G_WriteUserInfo(pUserInfo);
		
		CString strPrintf;
		strPrintf.Format("[QQ农场]:购买了%d粒%s的种子,花费了%d个金币",nNum,strName,nMoney);
		if (strDirection != "")
			strPrintf += "\r\n→" + N_HtmlToText(strDirection);
		Addtolist(strPrintf,COLOR_INFO);
		return TRUE;
	}
	return FALSE;
}
BOOL C_CropsLvlCmp(ST_CROPSGROW pCropGrow1,ST_CROPSGROW pCropGrow2)
{
	return pCropGrow1.Level < pCropGrow2.Level;
}
//返回购买哪种种子的ID
UINT H_GetBuySeedId(BOOL bIsRed)
{
	//最终返回的种子ID
	UINT nSeedId = 0;
	std::vector<ST_CROPSGROW> v_Seeds;	//可购买的种子
	std::vector<ST_CROPSGROW> v_HaveSeeds;  //背包里有的种子
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
		if (pCropGrow.BuyPrice > 0 && pCropGrow.IsRed == bIsRed && pCropGrow.IsMill == FALSE)	//如果是种子,且种子类型相符则加入数组
			v_Seeds.push_back(pCropGrow);
		if (pCropGrow.nSeedNum > 0 && (bIsRed || pCropGrow.IsRed == FALSE))
			v_HaveSeeds.push_back(pCropGrow);
		nPos += strlen(szCrop) + 1;
	}
	std::sort(v_Seeds.begin(),v_Seeds.end(),C_CropsLvlCmp);	//根据种子等级排序
	
	if (m_Config.nBuySeedType == 0)	//如果按最高等级来购买
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
//对空土地进行播种
UINT F_FarmPlanting(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	if (m_Config.nPlantingType == 0)
		F_GetUserSeed(NULL);	//读取背包数据
	for (int i=0;i<24;i++)
	{	//循环查看是否有空的土地
		if (pUserInfo.CropLand[i].a == 0 && pUserInfo.CropLand[i].b == 0)
		{
			//获取购买种子的ID
			UINT nSeedId = H_GetBuySeedId(pUserInfo.CropLand[i].IsRed);
			if (nSeedId != 0)	//如果ID不为0,则进行播种
			{
				Sleep(500);
				CString strPostData;
				strPostData.Format("uIdx=%d&ownerId=%d&uinY=%s&fName=%s&tName=%s&place=%d&cId=%d",
					pUserInfo.Id,pUserInfo.Id,GetQQInfo(GQI_NUM),
					strConvert(pUserInfo.Name,936,CP_UTF8),strConvert(pUserInfo.Name,936,CP_UTF8),
					i,nSeedId);
				CString strHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_plant?mod=farmlandstatus&act=planting",strPostData);
				CString strBuf;
				
				int nCode = 0;			//code为1则成功
				CString strDirection;	//提示文本
				int nExp = 0;			//增加的经验
				int nIndex = 0;			//土地索引
				
				FindMidStr(strHtml,0,"\"code\":",",",strBuf);	//code为1则成功		   
				nCode = atoi(strBuf);
				//FindMidStr(strSub,0,"\"direction\":\"","\"",strDirection); //提示文本
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
						strPrintf.Format("[QQ农场]:在自己%d号田里播种了%s的种子,获得了%d点经验",nIndex+1,pCropGrow.Name,nExp);
						if (strDirection != "")
							strPrintf += "\r\n→" + N_HtmlToText(strDirection);
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
//清理农场的杂草
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
				
				int nCode = 0;			//code为1则成功
				CString strDirection;	//提示文本
				int nExp = 0;			//增加的经验
				int nMoney = 0;			//增加的金币
				int nIndex = 0;			//土地索引
				int nWeed = 0;			//剩下的草
				
				FindMidStr(strSub,0,"\"code\":",",",strBuf);	//code为1则成功		   
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
							strPrintf.Format("[QQ农场]:清除了自己%d号田的1棵杂草,获得了%d点经验",nIndex+1,nExp);
						else
							strPrintf.Format("[QQ农场]:清除了好友[%s]%d号田的1棵杂草,获得了%d点经验,%d个金币",pUserInfo.Name,nIndex+1,nExp,nMoney);
						if (strDirection != "")
							strPrintf += "\r\n→" + N_HtmlToText(strDirection);
						Addtolist(strPrintf,COLOR_INFO);
						if (nExp == 0)	//如果得到的经验为0,就暂停为好友除草
						{
							KillTimer(hWndMain,IDT_GETSTATUSFILTER);
							bWcsFriends = FALSE;
						}
					}
					nResult = TRUE;
				}
				
				if (nOffset == -1)
					break;
				//进入下一次循环
				nOffset=FindMidStr(strHtml,nOffset,"{","}",strSub);
			}
			Sleep(1000);
		}
	}
	G_WriteUserInfo(pUserInfo);
	return nResult;
}
//清理农场的害虫
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
				
				int nCode = 0;			//code为1则成功
				CString strDirection;	//提示文本
				int nExp = 0;			//增加的经验
				int nMoney = 0;			//增加的金币
				int nIndex = 0;			//土地索引
				int nPest = 0;			//剩下的草
				
				FindMidStr(strSub,0,"\"code\":",",",strBuf);	//code为1则成功		   
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
							strPrintf.Format("[QQ农场]:清除了自己%d号田的1只害虫,获得了%d点经验",nIndex+1,nExp);
						else
							strPrintf.Format("[QQ农场]:清除了好友[%s]%d号田的1只害虫,获得了%d点经验,%d个金币",pUserInfo.Name,nIndex+1,nExp,nMoney);
						if (strDirection != "")
							strPrintf += "\r\n→" + N_HtmlToText(strDirection);
						Addtolist(strPrintf,COLOR_INFO);
						if (nExp == 0)	//如果得到的经验为0,就暂停为好友除草
						{
							KillTimer(hWndMain,IDT_GETSTATUSFILTER);
							bWcsFriends = FALSE;
						}
					}
					nResult = TRUE;
				}
				
				if (nOffset == -1)
					break;
				//进入下一次循环
				nOffset=FindMidStr(strHtml,nOffset,"{","}",strSub);
			}
			Sleep(1000);
		}
	}
	G_WriteUserInfo(pUserInfo);
	return nResult;
}
//为干旱的土地浇水
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
				
				int nCode = 0;			//code为1则成功
				CString strDirection;	//提示文本
				int nExp = 0;			//增加的经验
				int nMoney = 0;			//增加的金币
				int nIndex = 0;			//土地索引
				
				FindMidStr(strSub,0,"\"code\":",",",strBuf);	//code为1则成功		   
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
							strPrintf.Format("[QQ农场]:为自己%d号田进行了浇水,获得了%d点经验",nIndex+1,nExp);
						else
							strPrintf.Format("[QQ农场]:为好友[%s]%d号田进行了浇水,获得了%d点经验,%d个金币",pUserInfo.Name,nIndex+1,nExp,nMoney);
						if (strDirection != "")
							strPrintf += "\r\n→" + N_HtmlToText(strDirection);
						Addtolist(strPrintf,COLOR_INFO);
						if (nExp == 0)	//如果得到的经验为0,就暂停为好友除草
						{
							KillTimer(hWndMain,IDT_GETSTATUSFILTER);
							bWcsFriends = FALSE;
						}
					}
					nResult = TRUE;
				}
				
				if (nOffset == -1)
					break;
				//进入下一次循环
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
	int nCode = 0;			//code为1则成功
	int nLevel = 0;			//增加的经验
	int nMoney = 0;			//增加的金币
	
	FindMidStr(strHtml,0,"\"code\":",",",strBuf);	//code为1则成功		   
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
			nCode = 0;			//code为1则成功
			CString strDirection;	//提示文本
			nMoney = 0;			//增加的金币
			
			FindMidStr(strHtml,0,"\"code\":",",",strBuf);	//code为1则成功		   
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
				strPrintf.Format("[QQ农场]:开垦了一块土地,花费了%d个金币",nMoney);
				if (strDirection != "")
					strPrintf += "\r\n→" + N_HtmlToText(strDirection);
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
	int nCode = 0;			//code为1则成功
	int nLevel = 0;			//增加的经验
	int nMoney = 0;			//增加的金币
	
	FindMidStr(strHtml,0,"\"code\":",",",strBuf);	//code为1则成功		   
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
			
			nCode = 0;			//code为1则成功
			CString strDirection;	//提示文本
			nMoney = 0;			//增加的金币
			
			FindMidStr(strHtml,0,"\"code\":",",",strBuf);	//code为1则成功		   
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
			strPrintf.Format("[QQ农场]:升级了一块土地为红土地,花费了%d个金币",nMoney);
			if (strDirection != "")
				strPrintf += "\r\n→" + N_HtmlToText(strDirection);
			Addtolist(strPrintf,COLOR_INFO);
			G_WriteUserInfo(pUserInfo);
			nResult = TRUE;
		}
	}
	return nResult;
}
//攻击野生动物
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
				//增加的人品值
				int nAddMoral = 0;
				FindMidStr(strHtml,0,"\"addmoral\":",",",strBuf);
				nAddMoral = atoi(strBuf);
				if (nAddMoral > 0)
				{
					ST_USERINFO mUserInfo;
					G_GetUserInfo(m_SelfInfo.Id,mUserInfo);
					mUserInfo.MoralExp += nAddMoral;
					G_WriteUserInfo(mUserInfo);

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
					strPrintf.Format("[QQ农场]:击中了%s田里的%s(%d/%d),增加%d点人品",strUser,pBeastInfo.Name,pUserInfo.BeastLand[i].Blood,pBeastInfo.Blood,nAddMoral);
					Addtolist(strPrintf,COLOR_INFO);
				}else if (nResult == 2)
				{
					CString strPrintf;
					strPrintf.Format("[QQ农场]:甩翻了%s田里的%s(%d/%d),增加%d点人品",strUser,pBeastInfo.Name,pUserInfo.BeastLand[i].Blood,pBeastInfo.Blood,nAddMoral);
					Addtolist(strPrintf,COLOR_INFO);
				}else if (nResult == 3)
				{
					CString strPrintf;
					strPrintf.Format("[QQ农场]:攻击%s田里的%s(%d/%d)失败",strUser,pBeastInfo.Name,pUserInfo.BeastLand[i].Blood,pBeastInfo.Blood);
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
							strPrintf.Format("[QQ农场]:捡到了%d块%s",nNum,pCrystalInfo.Name);
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
//获得我的野生动物数据
UINT F_GetMyBeastInfo(LPVOID pParam)
{
	CString strPostData;
	strPostData.Format("uIdx=%d&uinY=%s&ownerId=%d",m_SelfInfo.Id,GetQQInfo(GQI_NUM),m_SelfInfo.Id);
	CString strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_get_userbeast",strPostData);
	CString strBuf;
	//当前开启的槽位数量
	int nMaxSlotId = 0;
	FindMidStr(strHtml,0,"\"maxslotid\":","}",strBuf);
	nMaxSlotId = atoi(strBuf);
	//替换一次内容,方便循环查找
	strHtml.Replace("\"\",","{}");
	
	CString strBeasts,strBeast;
	if (FindMidStr(strHtml,0,"\"beasts\":[","]",strBeasts) != -1)
	{
		int nOffset = 0;
		for (int i=0;i<nMaxSlotId+1;i++)
		{
			nOffset=FindMidStr(strBeasts,nOffset,"{","}",strBeast);
			//位置ID
			m_MyBeast[i].Id = i;
			//动物体力
			FindMidStr(strBeast,0,"\"curblood\":",",",strBuf);
			m_MyBeast[i].Blood = atoi(strBuf);
			//剩余放养次数
			FindMidStr(strBeast,0,"\"leftraise\":",",",strBuf);
			m_MyBeast[i].LeftRaise = atoi(strBuf);
			//动物状态
			FindMidStr(strBeast,0,"\"status\":",",",strBuf);
			m_MyBeast[i].Status = atoi(strBuf);
			//动物ID
			FindMidStr(strBeast,0,"\"type\":","",strBuf);
			m_MyBeast[i].Type = atoi(strBuf);
		}
	}
	return TRUE;
}
//野生动物返回
UINT F_FarmReturnBeast(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	ST_BEASTINFO pBeastInfo;
	//野生动物返回
	for (int i=0;i<3;i++)
	{
		if (m_MyBeast[i].Type > 0 && m_MyBeast[i].Status == 3)
		{
			CString strPostData;
			strPostData.Format("uIdx=%d&uinY=%s&slotid=%d&type=%d",m_SelfInfo.Id,GetQQInfo(GQI_NUM),i,m_MyBeast[i].Type);
			CString strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_reward_beast",strPostData);
			Sleep(800);
			CString strBuf;
			//错误代码
			int nECode = -1;
			//获得的金币
			int nMoney = 0;
			//获得的人品
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
				strPrintf.Format("[QQ农场]:%d号槽位放养的%s返回了,获得了%d个金币,%d点人品值",i+1,pBeastInfo.Name,nMoney,nMoralExp);
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
						//数量
						FindMidStr(strCrystal,0,"\"num\":",",",strBuf);
						nNum = atoi(strBuf);
						
						ST_CRYSTALINFO pCrystalInfo;
						G_GetCrystalInfo(nId,pCrystalInfo);
						strPrintf.Format("[QQ农场]:获得了%d块%s",nNum,pCrystalInfo.Name);
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
//开启野生动物槽位
UINT F_FarmOpenSlot(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	//开启槽位
	for (int i=0;i<3;i++)
	{
		if (m_MyBeast[i].Id == -1)
		{
			//第一个槽位是自动开启的,如果为-1,则是获取失败
			if (i == 0)
				break;
			
			int nNeedMoney = 0;
			if (i == 1)
				nNeedMoney = 300000;
			if (i == 2)
				nNeedMoney = 600000;
			
			//如果金币足够,则开启槽位
			if (pUserInfo.Money > nNeedMoney + 50000)
			{
				CString strPostData;
				strPostData.Format("uIdx=%d&uinY=%s&slotid=%d",m_SelfInfo.Id,GetQQInfo(GQI_NUM),i);
				CString strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_open_slot",strPostData);
				Sleep(800);
				CString strBuf;
				
				//错误代码
				int nECode = -1;
				//减少的金币
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
					strPrintf.Format("[QQ农场]:开启了%d号野生动物槽位,花费了%d个金币",i+1,nMoney);
					Addtolist(strPrintf,COLOR_INFO);
				}
			}
		}
	}
	return nResult;
}
//捐赠野生动物
UINT F_FarmDonateBeast(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	ST_BEASTINFO pBeastInfo;
	//捐赠野生动物
	for (int i=0;i<3;i++)
	{
		if (m_MyBeast[i].Status == 5 && m_MyBeast[i].Type > 0)
		{
			CString strPostData;
			strPostData.Format("uIdx=%d&uinY=%s&slotid=%d&type=%d",m_SelfInfo.Id,GetQQInfo(GQI_NUM),i,m_MyBeast[i].Type);
			CString strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_donate_beast",strPostData);
			Sleep(800);
			CString strBuf;
			
			//错误代码
			int nECode = 0;
			//减少的金币
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
				strPrintf.Format("[QQ农场]:捐赠了%d号槽位的%s,获得了%d个金币",i+1,pBeastInfo.Name,nMoney);
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
//领养野生动物
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
	
		std::vector<ST_BEASTINFO> v_Beasts;	//可领养的动物
		char szBeasts[SHORTUSERLEN]={0};
		char szBeast[SHORTLEN]={0};
		//取所有Section项,单个字符串以0结尾,整个字符串以00结尾
		GetPrivateProfileSectionNames(szBeasts,SHORTUSERLEN,strBeastIni);
		int nPos = 0;
		while(szBeasts[nPos] != 0)
		{
			strcpy(szBeast,szBeasts+nPos);	//得到单个的Section
			ST_BEASTINFO pBeastInfo;
			G_GetBeastInfo(atoi(szBeast),pBeastInfo);	//读取这个Section的数据
			if (pBeastInfo.IsDisplay == TRUE && pBeastInfo.BuyNeedExp <= pUserInfo.MoralExp && pBeastInfo.BuyPrice < pUserInfo.Money)
				v_Beasts.push_back(pBeastInfo);
			nPos += strlen(szBeast) + 1;
		}
		std::sort(v_Beasts.begin(),v_Beasts.end(),C_BeastsLvlCmp);	//排序
		if (v_Beasts.size() == 0)
			return nResult;
	
		for (i=0;i<3;i++)
		{
			if (m_MyBeast[i].Id != -1 && m_MyBeast[i].Type == 0)
			{
				//先获取人品值
				CString strPostData;
				strPostData.Format("uIdx=%d&uinY=%s",m_SelfInfo.Id,GetQQInfo(GQI_NUM));
				CString strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_get_moralexp",strPostData);
				Sleep(800);
				CString strBuf;
				//错误代码
				int nECode = -1;
				//人品值
				int nMoralExp = 0;
				FindMidStr(strHtml,0,"\"ecode\":",",",strBuf);
					nECode = atoi(strBuf);
				FindMidStr(strHtml,0,"\"moralexp\":",",",strBuf);
					nMoralExp = atoi(strBuf);
	
				if (nECode == FALSE)
				{
					pUserInfo.MoralExp = nMoralExp;
				}
	
				//开始领取野生动物了
				strPostData.Format("uIdx=%d&uinY=%s&slotid=%d&type=%d",m_SelfInfo.Id,GetQQInfo(GQI_NUM),i,v_Beasts[v_Beasts.size()-1].Id);
				strHtml = N_FarmPostUrl("farm.qzone.qq.com","/cgi-bin/cgi_farm_adopt_beast",strPostData);
				//错误代码
				nECode = -1;
				//花费的金币
				int nMoney = 0;
				FindMidStr(strHtml,0,"\"ecode\":",",",strBuf);
					nECode = atoi(strBuf);
				FindMidStr(strHtml,0,"\"money\":","}",strBuf);
					nMoney = atoi(strBuf);
				if (nECode == FALSE)
				{
					nResult = TRUE;
					pUserInfo.Money += nMoney;
					
					//位置ID
					int nId = 0;
					//动物ID
					int nType = 0;
					
					//位置ID
					if (FindMidStr(strHtml,0,"\"id\":",",",strBuf) != -1)
					{
						nId = atoi(strBuf);
						m_MyBeast[nId].Id = nId;
					}
					//动物ID
					if (FindMidStr(strHtml,0,"\"type\":","}",strBuf) != -1)
					{
						nType = atoi(strBuf);
						m_MyBeast[nId].Type = nType;
					}
	
					//剩余放养次数
					if (FindMidStr(strHtml,0,"\"raisenum\":",",",strBuf) != -1)
						m_MyBeast[nId].LeftRaise = v_Beasts[v_Beasts.size()-1].RaiseTimes - atoi(strBuf);
					//野生动物状态
					if (FindMidStr(strHtml,0,"\"status\":",",",strBuf) != -1)
						m_MyBeast[nId].Status = atoi(strBuf);
					//领养时间
					if (FindMidStr(strHtml,0,"\"adopttime\":",",",strBuf) != -1)
						m_MyBeast[nId].AdoptTime = atol(strBuf);
					//返回时间
					if (FindMidStr(strHtml,0,"\"returntime\":",",",strBuf) != -1)
						m_MyBeast[nId].ReturnTime = atol(strBuf);
	
					CString strPrintf;
					strPrintf.Format("[QQ农场]:%d号槽位领养了%s,花费了%d个金币",i+1,v_Beasts[v_Beasts.size()-1].Name,nMoney);
					Addtolist(strPrintf,COLOR_INFO);
				}
			}
		}
		
		if (!v_Beasts.empty())
			v_Beasts.clear();
	return nResult;
}
//处理我的野生动物
UINT H_HandleMyBeast(ST_USERINFO &pUserInfo)
{
	//是否有野生动物返回
	UINT nReturnBeast = FALSE;
	//是否开启了槽位
	UINT nOpenSlot = FALSE;
	//是否捐赠了野生动物
	UINT nDonateBeast = FALSE;
	//是否领养了野生动物
	UINT nAdoptBeast = FALSE;


	//野生动物返回
	nReturnBeast = F_FarmReturnBeast(pUserInfo);
	if (nReturnBeast)
		Sleep(2000);
	//获得我的野生动物数据
	F_GetMyBeastInfo(NULL);
	Sleep(1000);

	//开启槽位
	nOpenSlot = F_FarmOpenSlot(pUserInfo);
	if (nOpenSlot)
		Sleep(2000);
	//捐赠野生动物
	nDonateBeast = F_FarmDonateBeast(pUserInfo);
	if (nDonateBeast)
		Sleep(2000);
	//领养野生动物
	nAdoptBeast = F_FarmAdoptBeast(pUserInfo);
	if (nAdoptBeast)
		Sleep(2000);
	
	if (nReturnBeast || nOpenSlot || nDonateBeast || nAdoptBeast)
		G_WriteUserInfo(pUserInfo);
	return TRUE;
}
//处理我的农场的数据,播种收除都在这里处理
UINT H_HandleMyFarm(LPVOID pParam)
{
	ST_USERINFO pUserInfo;	//从配置文件中读取已经获取的数据
	G_GetUserInfo(m_SelfInfo.Id,pUserInfo);
	m_SelfInfo.Lvl = FarmExpToLevel(pUserInfo.Exp);	//更新一下我的等级

	UINT nHarvest = FALSE;	//是否有收获
	UINT nScarify = FALSE;	//是否有翻土
	UINT nPlanting = FALSE;	//是否有播种
	UINT nClearWeed = FALSE;//是否有除草
	UINT nSpraying	= FALSE;//是否有杀虫
	UINT nWatering  = FALSE;//是否有浇水
	UINT nAttackBeast = FALSE;//是否有攻击野生动物

	if (m_Config.bSelfSc)
	{
		nHarvest = F_FarmHarvest(pUserInfo);	//执行收获操作
		if (nHarvest)			//如果有则暂停两秒
		{
			Sleep(2000);
			if (m_Config.bSaleCrop)
			{
				F_GetUserCrop(NULL);	//获取仓库收获
				Sleep(2000);
				F_SaleUserCrop(NULL);	//卖出所有收获
				Sleep(2000);
			}
			if (m_Config.bReclaim)		//扩地
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
			if (m_Config.bUpRed && m_SelfInfo.Lvl > 27)		//升级为红土地
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
		
		nScarify = F_FarmScarify(pUserInfo);	//执行翻土操作
		if (nScarify)			//如果有则暂停两秒
			Sleep(2000);
	}
	
	if (m_Config.bSelfBp)
	{
		nPlanting = F_FarmPlanting(pUserInfo);	//执行播种操作
		if (nPlanting)			//如果有则暂停两秒
			Sleep(2000);
	}

	if (m_Config.bSelfWcs)
	{
		nClearWeed = F_FarmClearWeed(pUserInfo);//执行除草操作
		if (nClearWeed)			//如果有则暂停两秒
			Sleep(2000);
		
		nSpraying = F_FarmSpraying(pUserInfo);	//执行杀虫操作
		if (nSpraying)
			Sleep(2000);
		
		nWatering = F_FarmWatering(pUserInfo);	//执行浇水操作
		if (nWatering)
			Sleep(2000);
	}

	nAttackBeast = F_FarmAttackBeast(pUserInfo); //攻击野生动物
	if (nAttackBeast)
		Sleep(2000);
	//处理我的野生动物
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
//偷取好友成熟的作物
UINT F_FarmScrounge(ST_USERINFO &pUserInfo)
{
	std::vector<int> v_Land;
		
		for (int i=0;i<24;i++)
		{	//循环查看是否有可偷取的植物
			if (pUserInfo.CropLand[i].a > 0 && pUserInfo.CropLand[i].b == 6 && pUserInfo.CropLand[i].n == 1)
			{
				v_Land.push_back(i);
			}
		}
		//如果没有可偷取,则返回假
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
			
			int nCode = 0;			//code为1则成功
			CString strDirection;	//提示文本
			int nIndex = 0;			//土地索引
			//int nMoney = 0;			//收获的金币
			int nHarvest = 0;		//收获数量
			int ncId = 0;			//植物ID
			int nCropStatus = 0;	//新的阶段
			int nHarvestTimes = 0;	//新的季度
			int nHealth = 0;		//优秀程序
			
			int nHumidity = 0;		//湿度
			int nPest = 0;			//害虫
			int nWeed = 0;			//杂草
			DWORD dwPlantTime = 0;	//播种时间
			//int nThief = 0;			//是否可偷取
			
			FindMidStr(strSub,0,"\"code\":",",",strBuf);	//code为1则成功		   
			nCode = atoi(strBuf);
			//FindMidStr(strSub,0,"\"direction\":\"","\"",strDirection); //提示文本
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
					strPrintf.Format("[QQ农场]:摘取了好友[%s]%d号田里的%d个%s",pUserInfo.Name,nIndex+1,nHarvest,pCropGrow.Name);
					if (strDirection != "")
						strPrintf += "\r\n→" + N_HtmlToText(strDirection);
					Addtolist(strPrintf,COLOR_INFO);
				}
			}
			
			if (nOffset == -1)
				break;
			//进入下一次循环
			nOffset=FindMidStr(strHtml,nOffset,"{","}",strSub);
		}
		G_WriteUserInfo(pUserInfo);
	return TRUE;
}
//放养野生动物
UINT F_FarmRaiseBeast(ST_USERINFO &pUserInfo)
{
	UINT nResult = FALSE;
	//是否放养过
	BOOL IsRaised = FALSE;
	//是否有空位
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
			//错误代码
			int nECode = 0;
			FindMidStr(strHtml,0,"\"ecode\":",",",strBuf);
			nECode = atoi(strBuf);
			//人品值
			int nMoralExp = 0;
			FindMidStr(strHtml,0,"\"moralexp\":",",",strBuf);
			nMoralExp = atoi(strBuf);
			//返回时间
			FindMidStr(strHtml,0,"\"returntime\":",",",strBuf);
			m_MyBeast[i].ReturnTime = atol(strBuf);
			
			if (nECode == FALSE)
			{
				nResult = TRUE;
				m_MyBeast[i].Status = 2;
				
				if (pBeastInfo.Id != m_MyBeast[i].Type)
					G_GetBeastInfo(m_MyBeast[i].Type,pBeastInfo);
				
				CString strPrintf;
				strPrintf.Format("[QQ农场]:在好友[%s]家放养了%s,获得了%d点人品",pUserInfo.Name,pBeastInfo.Name,nMoralExp);
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
						//数量
						FindMidStr(strCrystal,0,"\"num\":",",",strBuf);
						nNum = atoi(strBuf);
						
						ST_CRYSTALINFO pCrystalInfo;
						G_GetCrystalInfo(nId,pCrystalInfo);
						strPrintf.Format("[QQ农场]:获得了%d块%s",nNum,pCrystalInfo.Name);
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
					strPrintf.Format("[QQ农场]:%s偷走了1个%s",pBeastInfo.Name,pCropGrow.Name);
					Addtolist(strPrintf,COLOR_INFO);
				}
				
			}
			break;
		}
	}
	return nResult;
}
//处理好友农场的数据,偷除在这里
UINT H_HandleFriendFarm(LPVOID pParam)
{
	int nId = (int)pParam;
	ST_USERINFO pUserInfo;	//从配置文件中读取已经获取的数据
	G_GetUserInfo(nId,pUserInfo);
	if (m_Config.bFdSc)
	{
		if (m_Config.bFdDog || !pUserInfo.Dog)
		{
			if (F_FarmScrounge(pUserInfo) == TRUE)
				Sleep(2000);
		}
	}
	if (bWcsFriends && m_Config.bFdWcs)	//如果可以除草杀虫才处理
	{
		if (F_FarmClearWeed(pUserInfo) == TRUE)
			Sleep(2000);
		if (F_FarmSpraying(pUserInfo) == TRUE)
			Sleep(2000);
		if (F_FarmWatering(pUserInfo) == TRUE)
			Sleep(2000);
	}

	if (F_FarmAttackBeast(pUserInfo) == TRUE)	//攻击野生动物
		Sleep(2000);
	F_FarmRaiseBeast(pUserInfo);  //放养野生动物

	return TRUE;
}
//过滤好友状态
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
//扫描好友农场数据,与下面的函数不同的是如果在四小时内,会自动读取INI中的数据
UINT F_ScanFarmInfo(LPVOID pParam)
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
		CString strPostData;	//处理POST数据,访问好友农场
		if (nId == m_SelfInfo.Id)	//如果是获取自己的农场数据
			strPostData.Format("uIdx=%d&flag=1&ownerId=0&uinY=%s",nId,GetQQInfo(GQI_NUM));
		else
			strPostData.Format("uinY=%s&ownerId=%d&uinX=%d&uIdx=%d",GetQQInfo(GQI_NUM),pUserInfo.Id,pUserInfo.QQ,m_SelfInfo.Id);
		
		strHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_index?mod=user&act=run",strPostData);
		
		H_GetFarmInfo(strHtml,pUserInfo);	//处理返回的农场数据
		//将完整数据写入配置文件
		G_WriteUserInfo(pUserInfo);
		nResult = TRUE;
	}
	return nResult;
}
//获取指定用户的农场数据
UINT F_GetFarmInfo(LPVOID pParam)
{
	int nId = (int)pParam;	//获取ID
	ST_USERINFO pUserInfo;	//从配置文件中读取已经获取的数据
	G_GetUserInfo(nId,pUserInfo);
	CString strHtml;
	CString strPostData;	//处理POST数据,访问好友农场
	if (nId == m_SelfInfo.Id)	//如果是获取自己的农场数据
		strPostData.Format("uIdx=%d&flag=1&ownerId=0&uinY=%s",nId,GetQQInfo(GQI_NUM));
	else
		strPostData.Format("uinY=%s&ownerId=%d&uinX=%d&uIdx=%d",GetQQInfo(GQI_NUM),pUserInfo.Id,pUserInfo.QQ,m_SelfInfo.Id);
	
	strHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_index?mod=user&act=run",strPostData);
	
	H_GetFarmInfo(strHtml,pUserInfo);	//处理返回的农场数据
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
	strPostData.Format("uinY=%s&uIdx=%d",GetQQInfo(GQI_NUM),m_SelfInfo.Id);
	CString strHtml = N_FarmPostUrl("nc.qzone.qq.com","/cgi-bin/cgi_farm_getFriendList?mod=friend",strPostData);
	
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
		FindMidStr(strBuf,0,"\"exp\":",",",strItem);
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
		if (strQQ != GetQQInfo(GQI_NUM))	//如果这个不是自己的ID才保存
		{
			//将农场数据写入INI文件
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
UINT F_GetUserSeed(LPVOID pParam)
{
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
		pCropGrow.nSeedNum = 0;			//清空种子数量
		G_WriteCropInfo(pCropGrow);		//重新写入配置文件
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
		//查找ID
		if (FindMidStr(strBuf,0,"\"cId\":",",",strItem) != -1)
		{
			pCropGrow.Id = atoi(strItem);
			G_GetCropInfo(pCropGrow.Id,pCropGrow);
			//数量
			FindMidStr(strBuf,0,"\"amount\":",",",strItem);
			pCropGrow.nSeedNum = atoi(strItem);
			//等级
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
	//取所有Section项,单个字符串以0结尾,整个字符串以00结尾
	GetPrivateProfileSectionNames(szCrops,SHORTUSERLEN,strCropsIni);
	int nPos = 0;
	while(szCrops[nPos] != 0)
	{
		strcpy(szCrop,szCrops+nPos);	//得到单个的Section
		ST_CROPSGROW pCropGrow;
		G_GetCropInfo(atoi(szCrop),pCropGrow);	//读取这个Section的数据
		pCropGrow.nCropNum = 0;			//清空仓库收获数量
		G_WriteCropInfo(pCropGrow);		//重新写入配置文件
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
			//查找ID
			if (FindMidStr(strBuf,0,"\"cId\":",",",strItem) != -1)
			{
				pCropGrow.Id = atoi(strItem);
				G_GetCropInfo(pCropGrow.Id,pCropGrow);
				//数量
				FindMidStr(strBuf,0,"\"amount\":",",",strItem);
				pCropGrow.nCropNum = atoi(strItem);
				//锁定状态
				if (!pCropGrow.IsLock)
				{
					FindMidStr(strBuf,0,"\"isLock\":",",",strItem);
					pCropGrow.IsLock = strItem == "1" ? TRUE:FALSE;
				}
				//销售价格
				FindMidStr(strBuf,0,"\"price\":",",",strItem);
				pCropGrow.SalePrice = atoi(strItem);
				//写入配置文件
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
	//可卖出的数量
	int nNum = 0;
	CString strCrops;
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
		int nCode = 0;			//code为1则成功
		CString strDirection;	//提示文本
		int nMoney = 0;			//增加的金币
		
		FindMidStr(strHtml,0,"\"code\":",",",strBuf);	//code为1则成功		   
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
			strPrintf.Format("[QQ农场]:卖出仓库收获成功(未锁定的),获得了%d个金币",nMoney);
			Addtolist(strPrintf,COLOR_INFO);
			return TRUE;
		}
	}
	return FALSE;
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
	strDog = pUserInfo.Dog == 1 ? "有":"无";	//狗的状态
	strHealthMode = IsHealthMode(pUserInfo.BeginTime,pUserInfo.EndTime) ? "是":"无";	//安全模式
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
		out_time.Format("%d小时%d分",nH,nM);
		//hDlg_Main->m_List_Friends.SetRedraw(FALSE);
		hDlg_Main->m_List_Friends.SetItemText(nId,4,out_time);
		//hDlg_Main->m_List_Friends.SetRedraw(TRUE);
	}
	else
	{
		hDlg_Main->m_List_Friends.SetItemText(nId,4,"");
	}
}
//显示好友农场成熟时间
void S_ShowFriendsRipeTime()
{
	for (int i=0;i<v_FriendsInfo.size();i++)
	{
		S_ShowFriendsRipeTimeOnPos(i);		
	}
}
//显示我的农场土地数据
void S_ShowMyRipeTime()
{
	hDlg_Main->m_List_Lands.DeleteAllItems();	//先清空列表
	ST_USERINFO pUserInfo;
	G_GetUserInfo(m_SelfLandId,pUserInfo);
	ST_CROPSGROW pCropGrow;	//植物数据
	for (int i=0;i<24;i++)
	{
		int nId = pUserInfo.CropLand[i].a;	//土地植物ID,0为空地
		if (nId > 0)
		{
			if (nId != pCropGrow.Id)	//如果植物数据不同,则读取新的
				G_GetCropInfo(nId,pCropGrow);

			char szNum[SHORTLEN]={0};
			itoa(i+1,szNum,10);
			//编号
			hDlg_Main->m_List_Lands.InsertItem(i,szNum);
			//植物名称
			hDlg_Main->m_List_Lands.SetItemText(i,1,pCropGrow.Name);
			//等级
			ZeroMemory(szNum,SHORTLEN);
			itoa(pCropGrow.Level,szNum,10);
			hDlg_Main->m_List_Lands.SetItemText(i,2,szNum);
			//季度
			CString strBuf;
			strBuf.Format("%d/%d",pUserInfo.CropLand[i].j+1,pCropGrow.HarvestNum);
			hDlg_Main->m_List_Lands.SetItemText(i,3,strBuf);
			//距成熟
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
				strBuf.Format("%d小时%d分",nH,nM);
				hDlg_Main->m_List_Lands.SetItemText(i,4,strBuf);
			} else if (nNum == 6)
			{
				hDlg_Main->m_List_Lands.SetItemText(i,4,"成熟");
			} else if (nNum == 7)
			{
				hDlg_Main->m_List_Lands.SetItemText(i,4,"已枯萎");
			}
		}
		else if (nId == 0)
		{
			char szNum[SHORTLEN]={0};
			itoa(i+1,szNum,10);
			//编号
			hDlg_Main->m_List_Lands.InsertItem(i,szNum);
			//植物名称
			hDlg_Main->m_List_Lands.SetItemText(i,1,"空地");
		}
	}
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
	char szuId[QQNUMLEN]={0};	//用户农场ID
	itoa(pUserInfo.Id,szuId,10);	//转换ID为文本
	return WritePrivateProfileStruct(szuId,"UserInfo",&pUserInfo,sizeof(ST_USERINFO),strFriendsDataIni);
}
UINT G_GetUserList(int nId,ST_USERLIST &pUserList)
{
	char szuId[QQNUMLEN]={0};	//用户农场ID
	itoa(nId,szuId,10);	//转换ID为文本
	return GetPrivateProfileStruct(szuId,"UserList",&pUserList,sizeof(ST_USERLIST),strFriendsListIni);
}
UINT G_WriteUserList(ST_USERLIST &pUserList)
{
	char szuId[QQNUMLEN]={0};	//用户农场ID
	itoa(pUserList.Id,szuId,10);	//转换ID为文本	
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
