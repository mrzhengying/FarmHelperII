#include "StdAfx.h"
//��ʾͷ��
extern IStream*		HeadPic_pStm;
extern IPicture*	HeadPic_pPic;
extern long			HeadPic_nWidth,HeadPic_nHeight;
extern SIZE			HeadPic_sz;
extern RECT			HeadPic_Rect;

extern CString		strQQPastureKey;//������Կ
extern CString		strPostHeader;	//POST��Ҫ����
extern DWORD		dwServerTime;	//������ʱ��
extern CString		strSelfPath;	//����Ŀ¼ + QQ�ļ���
extern CString		strQQPasture_ini;
extern CString		strAnimalsIni;	//�洢ֲ�����ݵ�INI
extern CString		strAddonIni;	//�洢��չ����INI
extern CString		strBeastIni;	//�洢Ұ����������INI
extern CString		strCrystalIni;	//�洢ˮ������INI
extern CString		strFriendsDataIni;	//��¼�������ݵ�INI
extern CString		strFriendsListIni;	//��¼�����б��INI
extern int			nExpcmplist[120]; //����Աȱ�


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