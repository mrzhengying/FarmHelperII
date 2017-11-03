// QQFarm.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "QQFarm.h"
#include "Dlg_Main.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
//ȫ�ֱ���																//
//////////////////////////////////////////////////////////////////////////
CDlg_Main		*hDlg_Main = new CDlg_Main;
HWND			CChWndMain = 0;			//�����ھ��
HWND			hWndMain = 0;			//���ھ��

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
// CQQFarmApp

BEGIN_MESSAGE_MAP(CQQFarmApp, CWinApp)
	//{{AFX_MSG_MAP(CQQFarmApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQQFarmApp construction

CQQFarmApp::CQQFarmApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CQQFarmApp object

CQQFarmApp theApp;


/*�������(����), OUT����,OUT����,OUT�汾*/
extern "C" UINT __declspec(dllexport) __stdcall CC_Plugindata(char shortname[32],char aboutme[256],char myversion[32])
{
	strcpy(shortname,"QQũ������");
	strcpy(aboutme,"ȫ�Զ�����������ũ��");
	strcpy(myversion,"7.8");
	return 1;
}

/*������ڳ�ʼ��(�ɿ�),IN������汾,IN�����򴰿ھ��,IN������HINST,OUT������ھ��,OUTͼ��(��дNULL),OUT����*/
extern "C" UINT __declspec(dllexport) __stdcall CC_Plugininit(char CCVersion[32],HWND CChWnd,HINSTANCE hInst,HWND &CplhWnd,HICON &CplhIcon,char shortname[32])
{
	CChWndMain = CChWnd;
	//�л�MFCģ��״̬(��̬���ӵ�MFC��),����Ǿ�̬���ӾͲ���Ҫ����
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	hDlg_Main->Create(IDD_DLG_MAIN,CWnd::FromHandle(CChWnd));
	hDlg_Main->ShowWindow(SW_SHOW);
	CplhWnd = hDlg_Main->GetSafeHwnd();
	hWndMain = CplhWnd;
	CplhIcon = LoadIcon(AfxGetResourceHandle(),MAKEINTRESOURCE(IDI_ICON1));
	strcpy(shortname,"QQũ������");
	return 1;
}

/*����˵���ʼ��(�ɿ�),OUT�˵�����
  ��ʽΪ-1 or 0 ����(����,-1Ϊ�½�,0Ϊ���뵽���ܲ˵�),1 ����(����)|-2 -(���Ƿָ���)|2 ����(����)*/
extern "C" UINT __declspec(dllexport) __stdcall CC_Pluginmenu(char data[4096])
{
	strcpy(data,"-1 QQũ������,1 ˢ���ҵ�ũ��|2 �鿴�ҵĲֿ�|3 �鿴�ҵı���|-2 -|4 ˢũ������|-2 -|5 ũ��ȫ������");
	return 1;
}

/*�˵��ص�*/
extern "C" void __declspec(dllexport) __stdcall CC_Pluginaction(int action)
{
	switch (action)
	{
	case 1:
		hDlg_Main->SendMessage(WM_COMMAND,IDC_MAIN_BTN_MYFARM,0);
		break;
	case 2:
		hDlg_Main->SendMessage(WM_COMMAND,IDC_MAIN_BTN_SHOWCROP,0);
		break;
	case 3:
		hDlg_Main->SendMessage(WM_COMMAND,IDC_MAIN_BTN_SHOWSEED,0);
		break;
	case 4:
		MessageBox(hWndMain,"��������˹���,��һ��ض����ֲ����,��ȡ��ֲ�ľ���","��ʾ",MB_OK|MB_ICONINFORMATION);
		break;
	case 5:
		hDlg_Main->SendMessage(WM_COMMAND,IDC_MAIN_BTN_CONFIG,0);
		break;
	}
}