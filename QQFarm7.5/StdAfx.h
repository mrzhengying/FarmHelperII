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
extern HWND			CChWndMain;			//父窗口句柄
extern HWND			hWndMain;			//窗口句柄

#define	COLOR_ERROR	RGB(255,0,0)	//信息显示颜色：错误信息
#define	COLOR_INFO	RGB(255,0,128)		//信息显示颜色：提示信息

//////////////////////////////////////////////////////////////////////////
struct ST_GLOBALCONFIG
{
	// 对自己 买种子,播种
	BOOL	bSelfBp;
	// 对自己 收获,翻地
	BOOL	bSelfSc;
	// 对自己 浇水锄草杀虫
	BOOL	bSelfWcs;			
	// 卖果实
	BOOL	bSaleCrop;	
	// 扩地
	BOOL	bReclaim;
	//升级为红土地
	BOOL	bUpRed;
	// 对好友 偷取
	BOOL	bFdSc;
	// 对好友 浇水锄草杀虫
	BOOL	bFdWcs;			
	// 对好友 有狗粮也偷
	BOOL	bFdDog;	
	// 买种子类型 按等级或指定
	int		nBuySeedType;	
	// 黑土地 指定的种子
	int		nBlackSeed;			
	// 红土地 指定的种子
	int		nRedSeed;
	// 果实数量达到多少才卖
	int		nSaleNum;
	// 忽略好友的等级
	int		nIgnoreFdLvl;
	// 偷取时间
	int		nWorkTime;			
	// 休息时间
	int		nSleepTime;	
	// 播种类型 全部或指定
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
	// 名字
	char	Name[SHORTTEXTLEN];	
	// 季度
	int		HarvestNum;			
	// 等级
	int		Level;				
	// 购买价格
	int		BuyPrice;			
	// 销售价格
	int		SalePrice;			
	// 预计产量
	int		Output;				
	// 预计收益
	int		Expect;				
	// 经验
	int		CropExp;		
	// 成熟时间
	int		GrowthCycle;		
	// 是不是红土地植物
	BOOL	IsRed;				
	// 是否可用于餐馆
	BOOL	IsRestaurant;		
	// 是否可用于牧场
	BOOL	IsFood;				
	// 是否锁定
	BOOL	IsLock;				
	// 广告神马的
	//BOOL		IsAnimation;	
	// 是不是花
	BOOL	IsFlower;
	// 是否可用于加工坊
	BOOL	IsMill;				
	//BOOL	IsActivity;			
	int		Type;
	// 生长周期
	LONG	CropGrow[6];
	// 种子数量
	int		nSeedNum;
	// 仓库数量
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
	// 种子的编号
	int a;					
	// 阶段
	int b;	
	// 是否为红土地
	BOOL IsRed;					
	// 曾经是否有草
	//int c;						
	// 曾经是否有虫子
	//int d;						
	// 曾经是否干旱
	//int e;						
	// 大于0有草
	int f;					
	// 大于0有虫子
	int g;						
	// 等于0干旱
	int h;					
	// 优秀程度
	int i;						
	// 采摘的次数 (季度)
	int j;						
	// 产量
	int k;				
	// 最低收获
	int l;					
	// 剩余
	int m;						
	// 偷过我果实的好友uid列表 (此次用于记录是否可偷取)
	int n;						
	//int o;					// 施肥的次数
	//int p;					// 动作？
	// 作物播种时间点
	DWORD q;				
	// 更新时间点
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

struct ST_MYBEASTINFO 
{
	//位置id
	int Id;
	//剩余放养次数
	int LeftRaise;
	//当前状态
	int Status;
	//野生动物id
	int Type;
	//领养时间
	DWORD AdoptTime;
	//返回时间
	DWORD ReturnTime;
	//好像是收获的什么数量
	//int InCome;
	//动物的体力
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
	// uid
	int		Id;					
	// qq
	int		QQ;					
	// 昵称
	char	Name[QQNAMELEN];	
	//int		YellowLevel;		// 黄钻等级
	//int		YellowStatus;		// 黄钻状态
	// 经验值
	LONG	Exp;				
	// 金币
	LONG	Money;				
	// 狗
	int		Dog;				
	// 是否开通牧场
	BOOL	pf;					
	// 安全模式
	BOOL	HealthMode;			
	// 安全模式开始时间
	LONG	BeginTime;			
	// 安全模式结束时间
	LONG	EndTime;			
	// 空间或校友
	BOOL	IsQzone;			
	// 土地数据
	ST_CROPLANDINFO CropLand[24];
	// 人品值
	int		MoralExp;
	// 野生动物
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
	//用户等级
	int Lvl;
	//最近成熟的时间
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
UINT G_GetCropInfo(int nId,ST_CROPSGROW &pCropGrow);
UINT G_WriteCropInfo(ST_CROPSGROW &pCropGrow);
BOOL C_CropsLvlCmp(ST_CROPSGROW pCropGrow1,ST_CROPSGROW pCropGrow2);
UINT G_GetGlobalConfig(ST_GLOBALCONFIG &pConfig);
UINT G_WriteGlobalConfig(ST_GLOBALCONFIG &pConfig);
CString N_FarmPostUrl(CString strHost,CString strUrl,CString strPostData);
UINT F_GetUserSeed(LPVOID pParam);		//获取背包数据
UINT F_GetUserCrop(LPVOID pParam);		//获取仓库数据
UINT F_SaleUserCrop(LPVOID pParam);
//////////////////////////////////////////////////////////////////////////

extern ST_USERLIST	m_SelfInfo;		//我的农场数据
extern std::vector<ST_USERLIST>	v_FriendsInfo;  //好友数据
extern ST_GLOBALCONFIG	m_Config;	//全局配置

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__95FAEDB7_C3AD_4889_9A12_45F369430AA8__INCLUDED_)
