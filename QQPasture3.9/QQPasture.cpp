// QQPasture.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "QQPasture.h"
#include "Dlg_Main.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
//全局变量																//
//////////////////////////////////////////////////////////////////////////
CDlg_Main		*hDlg_Main = new CDlg_Main;
HWND			CChWndMain = 0;			//父窗口句柄
HWND			hWndMain = 0;			//窗口句柄

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CQQPastureApp

BEGIN_MESSAGE_MAP(CQQPastureApp, CWinApp)
	//{{AFX_MSG_MAP(CQQPastureApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQQPastureApp construction

CQQPastureApp::CQQPastureApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CQQPastureApp object

CQQPastureApp theApp;


/*插件内容(必需), OUT名字,OUT关于,OUT版本*/
extern "C" UINT __declspec(dllexport) __stdcall CC_Plugindata(char shortname[32],char aboutme[256],char myversion[32])
{
	strcpy(shortname,"QQ牧场助手");
	strcpy(aboutme,"全自动化管理您的牧场");
	strcpy(myversion,"3.9");
	return 1;
}

/*插件窗口初始化(可空),IN主程序版本,IN主程序窗口句柄,IN主程序HINST,OUT插件窗口句柄,OUT图标(可写NULL),OUT标题*/
extern "C" UINT __declspec(dllexport) __stdcall CC_Plugininit(char CCVersion[32],HWND CChWnd,HINSTANCE hInst,HWND &CplhWnd,HICON &CplhIcon,char shortname[32])
{
	CChWndMain = CChWnd;
	//切换MFC模块状态(动态链接到MFC库),如果是静态链接就不需要这行
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	hDlg_Main->Create(IDD_DLG_MAIN,CWnd::FromHandle(CChWnd));
	hDlg_Main->ShowWindow(SW_SHOW);
	CplhWnd = hDlg_Main->GetSafeHwnd();
	hWndMain = CplhWnd;
	CplhIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_ICON1));
	strcpy(shortname,"QQ牧场助手");
	return 1;
}

/*插件菜单初始化(可空),OUT菜单内容
  格式为-1 or 0 标题(首项,-1为新建,0为插入到功能菜单),1 标题(子项)|-2 -(这是分隔符)|2 标题(子项)*/
extern "C" UINT __declspec(dllexport) __stdcall CC_Pluginmenu(char data[4096])
{
	strcpy(data,"-1 QQ牧场助手,1 刷新我的牧场|2 查看我的仓库|3 查看我的背包|-2 -|4 刷牧场经验|-2 -|5 牧场全局配置");
	return 1;
}

/*菜单回调*/
extern "C" void __declspec(dllexport) __stdcall CC_Pluginaction(int action)
{
	switch(action)
	{
	case 1:
		hDlg_Main->SendMessage(WM_COMMAND,IDC_MAIN_BTN_MYPASTURE,0);
		break;
	case 2:
		hDlg_Main->SendMessage(WM_COMMAND,IDC_MAIN_BTN_SHOWANIMALS,0);
		break;
	case 3:
		hDlg_Main->SendMessage(WM_COMMAND,IDC_MAIN_BTN_SHOWPACKAGE,0);
		break;
	case 5:
		hDlg_Main->SendMessage(WM_COMMAND,IDC_MAIN_BTN_CONFIG,0);
		break;
	}
}
