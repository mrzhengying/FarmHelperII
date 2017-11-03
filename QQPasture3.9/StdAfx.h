// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__C6A456DA_97A3_4F04_854E_5D1023989D5B__INCLUDED_)
#define AFX_STDAFX_H__C6A456DA_97A3_4F04_854E_5D1023989D5B__INCLUDED_

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
#define	COLOR_INFO	RGB(255,128,0)		//��Ϣ��ʾ��ɫ����ʾ��Ϣ

//////////////////////////////////////////////////////////////////////////
struct ST_GLOBALCONFIG
{
	//�������
	BOOL bAddFood;
	//������
	BOOL bBuyAnimal;
	//����,�ջ�
	BOOL bHarvest;
	//������,����
	BOOL bClearBad;
	//���ֿ��ջ�
	BOOL bSaleAnimals;
	//��������
	BOOL bUpHouse;
	//͵ȡ����
	BOOL bFdSteal;
	//������������,����
	BOOL bFdClearBad;
	//�п���Ҳ͵
	BOOL bFdSafety;
	//��������
	int  nBuyAnimalType;
	//ָ���Ѷ���ID
	int  nWAnimalId;
	//ָ���ﶯ��ID
	int  nPAnimalId;
	//������������
	int	 nRaiseAnimalType;
	//������������
	int  nMinFood;
	//�����������
	int  nMaxFood;
	//���������ﵽ���ٲ���
	int		nSaleNum;
	//���Ժ��ѵĵȼ�
	int		nIgnoreFdLvl;
	//͵ȡʱ��
	int		nWorkTime;			
	//��Ϣʱ��
	int		nSleepTime;
	ST_GLOBALCONFIG()
	{
		bAddFood = TRUE;
		bBuyAnimal = TRUE;
		bHarvest = TRUE;
		bClearBad = TRUE;
		bSaleAnimals = FALSE;
		bUpHouse = FALSE;
		bFdSteal = TRUE;
		bFdClearBad = TRUE;
		bFdSafety = TRUE;
		nBuyAnimalType = 0;
		nWAnimalId = 0;
		nPAnimalId = 0;
		nRaiseAnimalType = 0;
		nMinFood = 500;
		nMaxFood = 1000;
		nSaleNum = 200;
		nIgnoreFdLvl = 0;
		nWorkTime = 120;
		nSleepTime = 50;
	}
};
struct ST_ANIMALGROW
{
	//ID
	int Id;	
	//����
	char Name[32];
	//������
	char Product[32];	
	//��λ
	char Unit[8];		
	//���ﻹ����
	int house;			
	//����ȼ�
	int Level;			
	//����,��������ר��
	int Type;		
	//����۸�
	int byproductprice; 
	//���ﾭ��
	int harvestbExp;	
	//����۸�
	int productprice;	
	//���ﾭ��
	int harvestpExp;
	//����
	int output;			
	//����۸�
	int buyprice;		
	//�Ƿ�����
	int IsLock;		
	//����
	LONG cropvalue[6];
	//��������
	int nPackage;
	//�ֿ⶯������
	int nAnimals;
	//�ֿ⶯����������
	int nWhelp;
	ST_ANIMALGROW()
	{
		Id = 0;
		memset(Name,0,32);
		memset(Product,0,32);
		memset(Unit,0,8);
		house = 0;
		Level = 0;
		Type = 0;
		byproductprice = 0;
		harvestbExp = 0;
		productprice = 0;
		harvestpExp = 0;
		output = 0;
		buyprice = 0;
		IsLock = 0;
		memset(cropvalue,0,sizeof(LONG)*6);
		nPackage = 0;
		nAnimals = 0;
		nWhelp = 0;
	}
};

struct ST_ANIMALLAND_INFO 
{
	//ID
	int Id;		
	//����ʱ��
	int changeTime;
	//�Ѵ���ʱ��
	int growTime;
	//��һ�׶ε�ʱ��
	int growTimeNext;	
	//������
	int hungry;			
	//���
	int serial;	
	//�׶�
	int status;			
	//��һ�׶�
	int statusNext;
	//����������
	int totalCome;
	//͵ȡ��־
	int stealflag;
	ST_ANIMALLAND_INFO()
	{
		Id = -1;
		changeTime = 0;
		growTime = 0;
		growTimeNext = 0;
		hungry = 0;
		serial = 0;
		status = 0;
		statusNext = 0;
		totalCome = 0;
		stealflag = 0;
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
	//����id
	int		Id;		
	//����qq
	int		QQ;
	//�����ǳ�
	char	Name[QQNAMELEN];
	//int		yellowlevel;	//����ȼ�
	//int		yellowstatus;
	//����
	int		Exp;		
	//׬����Ǯ
	int		Money;			
	//����
	int		animalFood;
	//�ѵȼ�
	int		wlvl;			
	//��ȼ�
	int		plvl;			
	//����
	int		guard;
	//�ռ��У��
	BOOL	IsQzone;
	//��������
	ST_ANIMALLAND_INFO AnimalLand[20];	//����˵Ķ�����Ϣ
	//��������
	int		mosquito;
	//�������
	int		shit;
	// �Ƿ�ͨ����
	BOOL	pf;
	// Ұ������
	ST_BEASTLANDINFO BeastLand[3];
	ST_USERINFO()
	{
		Id = 0;
		QQ = 0;
		memset(Name,0,QQNAMELEN);
		wlvl = 0;
		plvl = 0;
		Exp = 0;
		Money = 0;
		guard = -1;
		IsQzone = FALSE;
		animalFood = 0;
		mosquito = 0;
		shit = 0;
		pf = FALSE;
	}
};

struct ST_USERLIST
{
	//uid
	int Id;
	//�û��ȼ�
	int Lvl;
	//����ɳ���ʱ��
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
extern ST_USERLIST	m_SelfInfo;		//�ҵ�ũ������
extern std::vector<ST_USERLIST>	v_FriendsInfo;  //��������
extern ST_GLOBALCONFIG	m_Config;	//ȫ������
//////////////////////////////////////////////////////////////////////////
UINT G_GetAnimalInfo(int nId,ST_ANIMALGROW &pAnimalGrow);
UINT G_WriteAnimalInfo(ST_ANIMALGROW &pAnimalGrow);
UINT G_GetGlobalConfig(ST_GLOBALCONFIG &pConfig);
UINT G_WriteGlobalConfig(ST_GLOBALCONFIG &pConfig);
CString N_PasturePostUrl(CString strHost,CString strUrl,CString strPostData);
UINT F_GetUserPackage(LPVOID pParam);
UINT F_GetUserAnimals(LPVOID pParam);
UINT F_SaleUserAnimals(LPVOID pParam);
BOOL C_AnimalsLvlCmp(ST_ANIMALGROW pAnimalGrow1,ST_ANIMALGROW pAnimalGrow2);
//////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__C6A456DA_97A3_4F04_854E_5D1023989D5B__INCLUDED_)
