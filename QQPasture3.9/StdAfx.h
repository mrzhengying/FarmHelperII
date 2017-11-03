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
extern HWND			CChWndMain;			//父窗口句柄
extern HWND			hWndMain;			//窗口句柄

#define	COLOR_ERROR	RGB(255,0,0)	//信息显示颜色：错误信息
#define	COLOR_INFO	RGB(255,128,0)		//信息显示颜色：提示信息

//////////////////////////////////////////////////////////////////////////
struct ST_GLOBALCONFIG
{
	//添加饲料
	BOOL bAddFood;
	//购买动物
	BOOL bBuyAnimal;
	//生产,收获
	BOOL bHarvest;
	//拍蚊子,清粪便
	BOOL bClearBad;
	//卖仓库收获
	BOOL bSaleAnimals;
	//升级窝棚
	BOOL bUpHouse;
	//偷取好友
	BOOL bFdSteal;
	//给好友拍蚊子,清粪便
	BOOL bFdClearBad;
	//有看守也偷
	BOOL bFdSafety;
	//买动物类型
	int  nBuyAnimalType;
	//指定窝动物ID
	int  nWAnimalId;
	//指定棚动物ID
	int  nPAnimalId;
	//饲养动物类型
	int	 nRaiseAnimalType;
	//最少饲料数量
	int  nMinFood;
	//最大饲料数量
	int  nMaxFood;
	//动物数量达到多少才卖
	int		nSaleNum;
	//忽略好友的等级
	int		nIgnoreFdLvl;
	//偷取时间
	int		nWorkTime;			
	//休息时间
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
	//名字
	char Name[32];
	//仔名字
	char Product[32];	
	//单位
	char Unit[8];		
	//是棚还是窝
	int house;			
	//购买等级
	int Level;			
	//类型,常规或餐厅专供
	int Type;		
	//产物价格
	int byproductprice; 
	//产物经验
	int harvestbExp;	
	//动物价格
	int productprice;	
	//动物经验
	int harvestpExp;
	//产量
	int output;			
	//购买价格
	int buyprice;		
	//是否锁定
	int IsLock;		
	//周期
	LONG cropvalue[6];
	//背包数量
	int nPackage;
	//仓库动物数量
	int nAnimals;
	//仓库动物幼崽数量
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
	//生长时间
	int changeTime;
	//已存活的时间
	int growTime;
	//下一阶段的时间
	int growTimeNext;	
	//饿不饿
	int hungry;			
	//编号
	int serial;	
	//阶段
	int status;			
	//下一阶段
	int statusNext;
	//已生产数量
	int totalCome;
	//偷取标志
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
	//名字
	char Name[SHORTTEXTLEN];
	//isDisplay 是否可以购买?
	int IsDisplay;
	//击败后获得的人品
	//int FinalAttackExp;
	//攻击成功获得的人品
	//int AttackExp;
	//StayExp 还没搞清楚
	//int StayExp;
	//放养获得的人品
	//int RaiseExp;
	//卖出后的金币
	int SaleMoney;
	//购买价格
	int BuyPrice;
	//可放养的次数
	int RaiseTimes;
	//购买需要的人品
	int BuyNeedExp;
	//最大体力值
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
	//主人QQ
	int fId;
	//id 不知道是什么
	int Id;
	//野生动物id
	int Type;
	//放养时间
	DWORD RaiseTime;
	//返回时间
	DWORD ReturnTime;
	//好像是收获的什么数量
	//int InCome;
	//动物的体力
	int Blood;
	//是否攻击过了
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
	//好友id
	int		Id;		
	//好友qq
	int		QQ;
	//好友昵称
	char	Name[QQNAMELEN];
	//int		yellowlevel;	//黄钻等级
	//int		yellowstatus;
	//经验
	int		Exp;		
	//赚到的钱
	int		Money;			
	//饲料
	int		animalFood;
	//窝等级
	int		wlvl;			
	//棚等级
	int		plvl;			
	//看守
	int		guard;
	//空间或校友
	BOOL	IsQzone;
	//动物数据
	ST_ANIMALLAND_INFO AnimalLand[20];	//这个人的动物信息
	//蚊子数量
	int		mosquito;
	//粪便数量
	int		shit;
	// 是否开通牧场
	BOOL	pf;
	// 野生动物
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
	//用户等级
	int Lvl;
	//最近成长的时间
	DWORD RipeTime;
	//检查的时间
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
	//名字
	char Name[SHORTTEXTLEN];
	//数量
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
	//名字
	char Name[SHORTTEXTLEN];
	//销售价格
	int SalePrice;
	ST_CRYSTALINFO()
	{
		Id = 0;
		memset(Name,0,SHORTTEXTLEN);
		SalePrice = 0;
	}
};
//////////////////////////////////////////////////////////////////////////
extern ST_USERLIST	m_SelfInfo;		//我的农场数据
extern std::vector<ST_USERLIST>	v_FriendsInfo;  //好友数据
extern ST_GLOBALCONFIG	m_Config;	//全局配置
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
