#include "StdAfx.h"
//显示头像
extern IStream*		HeadPic_pStm;
extern IPicture*	HeadPic_pPic;
extern long			HeadPic_nWidth,HeadPic_nHeight;
extern SIZE			HeadPic_sz;
extern RECT			HeadPic_Rect;

extern CString		strQQPastureKey;//牧场密钥
extern CString		strPostHeader;	//POST必要数据
extern DWORD		dwServerTime;	//服务器时间
extern CString		strSelfPath;	//运行目录 + QQ文件夹
extern CString		strQQPasture_ini;
extern CString		strAnimalsIni;	//存储植物数据的INI
extern CString		strAddonIni;	//存储扩展数据INI
extern CString		strBeastIni;	//存储野生动物数据INI
extern CString		strCrystalIni;	//存储水晶数据INI
extern CString		strFriendsDataIni;	//记录好友数据的INI
extern CString		strFriendsListIni;	//记录好友列表的INI
extern int			nExpcmplist[120]; //经验对比表


CString GetUrl(CString strURL, BOOL bConvert);
CString PostUrl(CString strHost, CString StrObjectName, CString strHeadres, CString strPostData, BOOL bConvert);
CString strConvert(CString pStr,int sourceCodepage,int targetCodepage);
CString CFindMidStr(CString strHtml,CString strP1,CString strP2);
UINT	Addtolist(CString &pStr,COLORREF pColor);
CString GetFileMd5(CString strPath);
BOOL	GetUrlToFile(CString strUrl, CString strFilePath, BOOL bDontCache);
CString GetQQInfo(int nType);
CString GetAppPath(int nType);
CString UncrypKey(CString &strSrc, CString strKey);
BOOL	FileExteriorFile(CString strFileName);
CString PastureTimeSha1PastureKey(time_t &time);
void	PastureHouseNum(int wLvl,int pLvl,int &wNum,int &pNum);
BOOL	IsHealthMode(DWORD dwBeginTime,DWORD dwEndTime);
int		FindMidStr(const CString &strIn, int nOffset, const CString strP1, const CString strP2, CString &strOut);
UINT	pShowHeadPic(LPVOID param);
void	pInitExpCmpList();
int		FarmExpToLevel(int nExp);
UINT	pGetServerTime();