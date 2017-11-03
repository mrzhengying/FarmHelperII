// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__95FAEDB7_C3AD_4889_9A12_45F369430AA8__INCLUDED_)
#define AFX_STDAFX_H__95FAEDB7_C3AD_4889_9A12_45F369430AA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <vector>
#include <algorithm>
#include "Public.h"
#pragma comment (lib,"Public.lib")
#include "resource.h"
#include "Markup.h"
#include "common.h"
#include "Dlg_Main.h"


extern CDlg_Main	*hDlg_Main;
extern HWND			CChWndMain;			//�����ھ��
extern HWND			hWndMain;			//���ھ��

#define	COLOR_ERROR	RGB(255,0,0)	//��Ϣ��ʾ��ɫ��������Ϣ
#define	COLOR_INFO	RGB(255,0,128)		//��Ϣ��ʾ��ɫ����ʾ��Ϣ

//////////////////////////////////////////////////////////////////////////
struct ST_GLOBALCONFIG
{
	// ���Լ� ������,����
	BOOL	bSelfBp;
	// ���Լ� �ջ�,����
	BOOL	bSelfSc;
	// ���Լ� ��ˮ����ɱ��
	BOOL	bSelfWcs;			
	// ����ʵ
	BOOL	bSaleCrop;	
	// ����
	BOOL	bReclaim;
	//����Ϊ������
	BOOL	bUpRed;
	// �Ժ��� ͵ȡ
	BOOL	bFdSc;
	// �Ժ��� ��ˮ����ɱ��
	BOOL	bFdWcs;			
	// �Ժ��� �й���Ҳ͵
	BOOL	bFdDog;	
	// ���������� ���ȼ���ָ��
	int		nBuySeedType;	
	// ������ ָ��������
	int		nBlackSeed;			
	// ������ ָ��������
	int		nRedSeed;
	// ��ʵ�����ﵽ���ٲ���
	int		nSaleNum;
	// ���Ժ��ѵĵȼ�
	int		nIgnoreFdLvl;
	// ͵ȡʱ��
	int		nWorkTime;			
	// ��Ϣʱ��
	int		nSleepTime;	
	// �������� ȫ����ָ��
	int		nPlantingType;
	ST_GLOBALCONFIG()
	{
		bSelfBp = TRUE;
		bSelfSc = TRUE;
		bSelfWcs = TRUE;
		bSaleCrop = FALSE;
		bReclaim = FALSE;
		bUpRed = FALSE;
		bFdSc = TRUE;
		bFdWcs = TRUE;
		bFdDog = TRUE;
		nBuySeedType = 0;
		nBlackSeed = 0;
		nRedSeed = 0;
		nSaleNum = 200;
		nIgnoreFdLvl = 0;
		nWorkTime = 120;
		nSleepTime = 30;
		nPlantingType = 0;
	}
};

struct ST_CROPSGROW
{
	// ID
	int		Id;					
	// ����
	char	Name[SHORTTEXTLEN];	
	// ����
	int		HarvestNum;			
	// �ȼ�
	int		Level;				
	// ����۸�
	int		BuyPrice;			
	// ���ۼ۸�
	int		SalePrice;			
	// Ԥ�Ʋ���
	int		Output;				
	// Ԥ������
	int		Expect;				
	// ����
	int		CropExp;		
	// ����ʱ��
	int		GrowthCycle;		
	// �ǲ��Ǻ�����ֲ��
	BOOL	IsRed;				
	// �Ƿ�����ڲ͹�
	BOOL	IsRestaurant;		
	// �Ƿ����������
	BOOL	IsFood;				
	// �Ƿ�����
	BOOL	IsLock;				
	// ��������
	//BOOL		IsAnimation;	
	// �ǲ��ǻ�
	BOOL	IsFlower;
	// �Ƿ�����ڼӹ���
	BOOL	IsMill;				
	//BOOL	IsActivity;			
	int		Type;
	// ��������
	LONG	CropGrow[6];
	// ��������
	int		nSeedNum;
	// �ֿ�����
	int		nCropNum;
	ST_CROPSGROW()
	{
		Id = 0;
		memset(Name,0,SHORTTEXTLEN);
		HarvestNum = 0;
		Level = 0;
		BuyPrice = 0;
		SalePrice = 0;
		Output = 0;
		Expect = 0;
		CropExp = 0;
		GrowthCycle = 0;
		IsRed = FALSE;
		IsRestaurant = FALSE;
		IsFood = FALSE;
		IsLock = FALSE;
		IsFlower = FALSE;
		IsMill = FALSE;
		Type = 0;
		memset(CropGrow,0,sizeof(long)*6);
		nSeedNum = 0;
		nCropNum = 0;
	}
};

struct ST_CROPLANDINFO
{
	// ���ӵı��
	int a;					
	// �׶�
	int b;	
	// �Ƿ�Ϊ������
	BOOL IsRed;					
	// �����Ƿ��в�
	//int c;						
	// �����Ƿ��г���
	//int d;						
	// �����Ƿ�ɺ�
	//int e;						
	// ����0�в�
	int f;					
	// ����0�г���
	int g;						
	// ����0�ɺ�
	int h;					
	// ����̶�
	int i;						
	// ��ժ�Ĵ��� (����)
	int j;						
	// ����
	int k;				
	// ����ջ�
	int l;					
	// ʣ��
	int m;						
	// ͵���ҹ�ʵ�ĺ���uid�б� (�˴����ڼ�¼�Ƿ��͵ȡ)
	int n;						
	//int o;					// ʩ�ʵĴ���
	//int p;					// ������
	// ���ﲥ��ʱ���
	DWORD q;				
	// ����ʱ���
	DWORD r;					
	ST_CROPLANDINFO()
	{
		a = -1;
		b = 0;
		IsRed = FALSE;
		//c = 0;
		//d = 0;
		//e = 0;
		f = 0;
		g = 0;
		h = 1;
		i = 0;
		j = 0;
		k = 0;
		l = 0;
		m = 0;
		q = 0;
		r = 0;
	}
};

struct ST_BEASTINFO 
{
	//id
	int Id;
	//����
	char Name[SHORTTEXTLEN];
	//isDisplay �Ƿ���Թ���?
	int IsDisplay;
	//���ܺ��õ���Ʒ
	//int FinalAttackExp;
	//�����ɹ���õ���Ʒ
	//int AttackExp;
	//StayExp ��û�����
	//int StayExp;
	//������õ���Ʒ
	//int RaiseExp;
	//������Ľ��
	int SaleMoney;
	//����۸�
	int BuyPrice;
	//�ɷ����Ĵ���
	int RaiseTimes;
	//������Ҫ����Ʒ
	int BuyNeedExp;
	//�������ֵ
	int Blood;
	ST_BEASTINFO()
	{
		Id = 0;
		memset(Name,0,SHORTTEXTLEN);
		IsDisplay = 0;
		SaleMoney = 0;
		BuyPrice = 0;
		RaiseTimes = 0;
		BuyNeedExp = 0;
		Blood = 0;
	}
};

struct ST_MYBEASTINFO 
{
	//λ��id
	int Id;
	//ʣ���������
	int LeftRaise;
	//��ǰ״̬
	int Status;
	//Ұ������id
	int Type;
	//����ʱ��
	DWORD AdoptTime;
	//����ʱ��
	DWORD ReturnTime;
	//�������ջ��ʲô����
	//int InCome;
	//���������
	int Blood;
	ST_MYBEASTINFO()
	{
		Id = -1;
		LeftRaise = 0;
		Status = 0;
		Type = 0;
		AdoptTime = 0;
		ReturnTime = 0;
		Blood = 0;
	}
};

struct ST_BEASTLANDINFO
{
	//����QQ
	int fId;
	//id ��֪����ʲô
	int Id;
	//Ұ������id
	int Type;
	//����ʱ��
	DWORD RaiseTime;
	//����ʱ��
	DWORD ReturnTime;
	//�������ջ��ʲô����
	//int InCome;
	//���������
	int Blood;
	//�Ƿ񹥻�����
	BOOL bAttack;
	ST_BEASTLANDINFO()
	{
		fId = 0;
		Type = 0;
		RaiseTime = 0;
		ReturnTime = 0;
		Blood = 0;
		bAttack = FALSE;
	}
};

struct ST_USERINFO
{
	// uid
	int		Id;					
	// qq
	int		QQ;					
	// �ǳ�
	char	Name[QQNAMELEN];	
	//int		YellowLevel;		// ����ȼ�
	//int		YellowStatus;		// ����״̬
	// ����ֵ
	LONG	Exp;				
	// ���
	LONG	Money;				
	// ��
	int		Dog;				
	// �Ƿ�ͨ����
	BOOL	pf;					
	// ��ȫģʽ
	BOOL	HealthMode;			
	// ��ȫģʽ��ʼʱ��
	LONG	BeginTime;			
	// ��ȫģʽ����ʱ��
	LONG	EndTime;			
	// �ռ��У��
	BOOL	IsQzone;			
	// ��������
	ST_CROPLANDINFO CropLand[24];
	// ��Ʒֵ
	int		MoralExp;
	// Ұ������
	ST_BEASTLANDINFO BeastLand[3];
	ST_USERINFO()
	{
		Id = 0;
		QQ = 0;
		memset(Name,0,QQNAMELEN);
		//YellowLevel = 0;
		//YellowStatus = 0;
		Exp = 0;
		Money = 0;
		Dog = -1;
		pf = FALSE;
		HealthMode = FALSE;
		BeginTime = 0;
		EndTime = 0;
		IsQzone = FALSE;
		MoralExp = 0;
	}
};
struct ST_USERLIST
{
	//uid
	int Id;
	//�û��ȼ�
	int Lvl;
	//��������ʱ��
	DWORD RipeTime;
	//����ʱ��
	DWORD CheckTime;
	ST_USERLIST()
	{
		Id = 0;
		Lvl = 0;
		RipeTime = 0;
		CheckTime = 0;
	}
};

struct ST_CARDSGAME
{
	//uid
	int Id;
	//����
	char Name[SHORTTEXTLEN];
	//����
	int Num;
	ST_CARDSGAME()
	{
		Id = 0;
		memset(Name,0,SHORTTEXTLEN);
		Num = 0;
	}
};

struct ST_CRYSTALINFO 
{
	//Id
	int Id;
	//����
	char Name[SHORTTEXTLEN];
	//���ۼ۸�
	int SalePrice;
	ST_CRYSTALINFO()
	{
		Id = 0;
		memset(Name,0,SHORTTEXTLEN);
		SalePrice = 0;
	}
};
//////////////////////////////////////////////////////////////////////////
UINT G_GetCropInfo(int nId,ST_CROPSGROW &pCropGrow);
UINT G_WriteCropInfo(ST_CROPSGROW &pCropGrow);
BOOL C_CropsLvlCmp(ST_CROPSGROW pCropGrow1,ST_CROPSGROW pCropGrow2);
UINT G_GetGlobalConfig(ST_GLOBALCONFIG &pConfig);
UINT G_WriteGlobalConfig(ST_GLOBALCONFIG &pConfig);
CString N_FarmPostUrl(CString strHost,CString strUrl,CString strPostData);
UINT F_GetUserSeed(LPVOID pParam);		//��ȡ��������
UINT F_GetUserCrop(LPVOID pParam);		//��ȡ�ֿ�����
UINT F_SaleUserCrop(LPVOID pParam);
//////////////////////////////////////////////////////////////////////////

extern ST_USERLIST	m_SelfInfo;		//�ҵ�ũ������
extern std::vector<ST_USERLIST>	v_FriendsInfo;  //��������
extern ST_GLOBALCONFIG	m_Config;	//ȫ������

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__95FAEDB7_C3AD_4889_9A12_45F369430AA8__INCLUDED_)
