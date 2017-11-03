#include "StdAfx.h"
#include "common.h"
#include "sha1.h"

#import "msxml3.dll"
using namespace MSXML2;


IStream*		HeadPic_pStm = NULL;
IPicture*		HeadPic_pPic = NULL;
long			HeadPic_nWidth,HeadPic_nHeight;
SIZE			HeadPic_sz;
RECT			HeadPic_Rect = {0,0,0,0};
CString			strQQPastureKey;
CString			strPostHeader;
DWORD			dwServerTime;
CString			strSelfPath;
CString			strQQPasture_ini;
CString			strAnimalsIni;
CString			strAddonIni;
CString			strBeastIni;
CString			strCrystalIni;
CString			strFriendsDataIni;
CString			strFriendsListIni;
int				nExpcmplist[120];


CString GetUrl(CString strURL, BOOL bConvert)
{
	char* pchHtml=CC_GetUrl(strURL.GetBuffer(strURL.GetLength()),bConvert);
	strURL.ReleaseBuffer();
	CString strHtml(pchHtml);
	CC_FreeStr(pchHtml);
	
	return strHtml;
}

CString PostUrl(CString strHost, CString StrObjectName, CString strHeadres, CString strPostData, BOOL bConvert)
{
	char* pchHtml=CC_PostUrl(strHost.GetBuffer(strHost.GetLength()),StrObjectName.GetBuffer(StrObjectName.GetLength()),
		strHeadres.GetBuffer(strHeadres.GetLength()),strPostData.GetBuffer(strPostData.GetLength()),bConvert);
	strHost.ReleaseBuffer();
	StrObjectName.ReleaseBuffer();
	strHeadres.ReleaseBuffer();
	strPostData.ReleaseBuffer();
	CString strHtml(pchHtml);
	CC_FreeStr(pchHtml);
	
	return strHtml;
}

CString strConvert(CString pStr,int sourceCodepage,int targetCodepage)
{
	char str[TEXTLEN]={0};
	strcpy(str,pStr.GetBuffer(pStr.GetLength()));
	pStr.ReleaseBuffer();
	CC_StrConvert(str,sourceCodepage,targetCodepage);
	pStr = str;
	return pStr;
}

CString CFindMidStr(CString strHtml,CString strP1,CString strP2)
{
	CString str;
	int nPos1=0,nPos2=0;
	nPos1=strHtml.Find(strP1,0);
	nPos2=strHtml.Find(strP2,nPos1+strP1.GetLength());
	if (nPos1 != -1 && nPos2 != -1)
	{
		str = strHtml.Mid(nPos1+strP1.GetLength(),nPos2-nPos1-strP1.GetLength());
	}
	return str;
}

UINT Addtolist(CString &pStr,COLORREF pColor)
{
	UINT n = CC_Addtolist(pStr.GetBuffer(pStr.GetLength()),pColor);
	pStr.ReleaseBuffer();
	return n;
}

CString GetFileMd5(CString strPath)
{
	char chBuf[34];
	CC_GetFileMd5(strPath.GetBuffer(strPath.GetLength()),chBuf);
	strPath.ReleaseBuffer();
	return CString(chBuf);
}

BOOL GetUrlToFile(CString strUrl, CString strFilePath, BOOL bDontCache)
{
	BOOL bOK=CC_GetUrlToFile(strUrl.GetBuffer(strUrl.GetLength()),strFilePath.GetBuffer(strFilePath.GetLength()),bDontCache,NULL);
	strUrl.ReleaseBuffer();
	strFilePath.ReleaseBuffer();
	return bOK;
}

CString GetQQInfo(int nType)
{
	char chBuf[32];
	CC_GetQQInfo(chBuf,nType);
	return CString(chBuf);
}

CString GetAppPath(int nType)
{
	char chBuf[MAX_PATH];
	CC_GetAppPath(chBuf,nType);
	return CString(chBuf);
}

CString UncrypKey(CString &strSrc, CString strKey)
{
	char chBuf[ARGLEN] = {0};
	CC_UncrypKey(strSrc.GetBuffer(strSrc.GetLength()),strKey.GetBuffer(strKey.GetLength()),chBuf);
	strSrc.ReleaseBuffer();
	strKey.ReleaseBuffer();
	return CString(chBuf);
}

BOOL FileExteriorFile(CString &strFileName)
{
	BOOL bOK=CC_FileExteriorFile(strFileName.GetBuffer(strFileName.GetLength()));
	strFileName.ReleaseBuffer();
	return bOK;
}

CString PastureTimeSha1PastureKey(time_t &time)
{
	time = ::time(0);
	CString strTime,strTemp;
	strTime.Format("%d",time);
	strTemp.Format("%s%s",strTime,strQQPastureKey.Right(strQQPastureKey.GetLength() - time%10));
	CSHA1 pSha1Hash;
	pSha1Hash.Update((unsigned __int8 *)strTemp.GetBuffer(strTemp.GetLength()),strTemp.GetLength());
	pSha1Hash.Final();
	char szInfoHash[128];
	memset(szInfoHash,0,128);
	pSha1Hash.ReportHash(szInfoHash);
	strTemp.ReleaseBuffer();
	return CString(szInfoHash);
}

void PastureHouseNum(int wLvl,int pLvl,int &wNum,int &pNum)
{
	switch (wLvl)
	{
	case 0:
		wNum = 0;
		break;
	case 1:
		wNum = 2;
		break;
	case 2:
		wNum = 3;
		break;
	case 3:
		wNum = 5;
		break;
	case 4:
		wNum = 6;
		break;
	case 5:
		wNum = 7;
		break;
	case 6:
		wNum = 8;
		break;
	case 7:
		wNum = 9;
		break;
	case 8:
		wNum = 10;
		break;
	}
	switch (pLvl)
	{
	case 0:
		pNum = 0;
		break;
	case 1:
		pNum = 3;
		break;
	case 2:
		pNum = 4;
		break;
	case 3:
		pNum = 5;
		break;
	case 4:
		pNum = 6;
		break;
	case 5:
		pNum = 7;
		break;
	case 6:
		pNum = 8;
		break;
	case 7:
		pNum = 9;
		break;
	case 8:
		pNum = 10;
		break;
	}
}

BOOL IsHealthMode(DWORD dwBeginTime,DWORD dwEndTime)
{
	if (dwServerTime > dwEndTime || dwServerTime < dwBeginTime)
	{
		return FALSE;
	}
	CTime t1 = dwBeginTime;
	CTime t2 = dwServerTime;
	int nt1Hour = t1.GetHour();
	int nt1Minute = t1.GetMinute();
	int nt2Hour = t2.GetHour();
	int nt2Minute = t2.GetMinute();
	if (nt2Hour < nt1Hour || nt2Hour > nt1Hour + 8)
	{
		return FALSE;
	}
	if (nt2Hour == nt1Hour && nt2Minute < nt1Minute)
	{
		return FALSE;
	}
	if (nt2Hour == nt1Hour + 8 && nt2Minute > nt1Minute)
	{
		return FALSE;
	}
	return TRUE;
}

int FindMidStr(const CString &strIn, int nOffset, const CString strP1, const CString strP2, CString &strOut)
{
	strOut="";
	
	if(strIn.GetLength()<=0)	return -1;
	if(strP1.GetLength()<=0 && strP2.GetLength()<=0)	return -1;
	
	int nIndex1=nOffset;
	if(strP1.GetLength()>0)
	{
		nIndex1=strIn.Find(strP1,nOffset);
		if(nIndex1==-1)	return -1;
	}
	
	strOut=strIn.Right(strIn.GetLength()-nIndex1-strP1.GetLength());
	
	int nIndex2=strOut.GetLength();
	if(strP2.GetLength()>0)
	{
		nIndex2=strOut.Find(strP2);
		if(nIndex2==-1)	return -1;
		
		strOut=strOut.Left(nIndex2);
	}
	
	strOut.TrimLeft();
	strOut.TrimRight();
	
	return nIndex1+strP1.GetLength()+strOut.GetLength()+strP2.GetLength();
}

UINT pShowHeadPic(LPVOID param)		//ÏÔÊ¾Í·Ïñ
{
	int nQQ = (int)param;
	if (!nQQ)
		return FALSE;	
	CString strUrl;
	strUrl.Format("http://r.qzone.qq.com/fcg-bin/cgi_get_score.fcg?uins=%d&mask=7",nQQ);
	CString szHtml = GetUrl(strUrl,FALSE);

	int nPos1,nPos2;
	nPos1 = szHtml.Find(":[\"",0);
	if (nPos1 != -1)
	{
		nPos2 = szHtml.Find("\",",nPos1);
		if (nPos2 != -1)
			strUrl = szHtml.Mid(nPos1+3,nPos2-nPos1-3);
		else
			return FALSE;
	}
	else
		return FALSE;

	DWORD dwLen = 0;
	::CoInitialize(NULL);
	char szTempPath[MAX_PATH] = {0};
	GetTempPath(MAX_PATH,szTempPath);
	char szFilePath[MAX_PATH] = {0};
	srand((unsigned)time(NULL));
	int	nRand1 = (int)(((double) rand() / (double) RAND_MAX) * 900000 + 100000);
	sprintf(szFilePath,"%s%d.jpg",szTempPath,nRand1);	
	if (GetUrlToFile(strUrl,szFilePath,FALSE))
	{
		FILE *file;
		file = fopen(szFilePath,"rb");
		if (file)
		{
			fseek(file,0,SEEK_END);
			dwLen = ftell(file);
			HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwLen);         
			LPVOID pvData = NULL;          
			if (hGlobal != NULL)         
			{             
				pvData = GlobalLock(hGlobal);             
				if (pvData != NULL)            
				{
					fseek(file,0,SEEK_SET);
					fread(pvData,dwLen,1,file);
					GlobalUnlock(hGlobal);
					
					if(HeadPic_pStm) HeadPic_pStm->Release();
					CreateStreamOnHGlobal(hGlobal, TRUE, &HeadPic_pStm);      
				}      
			}  
			fclose(file);
			DeleteFile(szFilePath);
			if(HeadPic_pPic) HeadPic_pPic->Release();
			if(HeadPic_pStm && SUCCEEDED(OleLoadPicture(HeadPic_pStm, dwLen, TRUE,IID_IPicture, (LPVOID*)&HeadPic_pPic)))    
			{     
				HeadPic_pPic->get_Width(&HeadPic_nWidth);
				HeadPic_pPic->get_Height(&HeadPic_nHeight);
				
				HeadPic_Rect.left = 12;
				HeadPic_Rect.top = 8;
				HeadPic_Rect.right = 12 + 50;
				HeadPic_Rect.bottom = 8 + 50;
			}
			//RedrawWindow(hWndMain,&HeadPic_Rect,NULL,RDW_INTERNALPAINT|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE);
			hDlg_Main->RedrawWindow(&HeadPic_Rect,NULL,RDW_INVALIDATE);
		}
	}
	::CoUninitialize();
	return TRUE;
}

void pInitExpCmpList()
{
	for (int i=1;i<120;i++)
	{
		nExpcmplist[i]=i * (i + 1) * 100;
	}
}

int FarmExpToLevel(int nExp)
{
	for (int i=1;i<120;i++)
	{
		if (nExp < nExpcmplist[i])
		{
			return i - 1;
		}
	}
	return -1;
}

UINT pGetServerTime()
{
	::CoInitialize(NULL);
	IXMLHTTPRequestPtr pIXMLHTTPRequest = NULL;
	BSTR bstrString = NULL;
	HRESULT hr;
	try {
		hr=pIXMLHTTPRequest.CreateInstance("Msxml2.XMLHTTP.3.0");
		SUCCEEDED(hr) ? 0 : throw hr;		
		hr=pIXMLHTTPRequest->open("GET", "http://nc.qzone.qq.com/", false);
		SUCCEEDED(hr) ? 0 : throw hr;	
		hr=pIXMLHTTPRequest->send();
		SUCCEEDED(hr) ? 0 : throw hr;		
		bstrString=pIXMLHTTPRequest->getResponseHeader("Date");
		SetTimer(hWndMain,1,1000,NULL);
		
		CString strString = (LPCTSTR)_bstr_t(bstrString);
		if (strString == "")
			return FALSE;

		int nOffset = 0;
		CString strD;
		nOffset = FindMidStr(strString,nOffset," "," ",strD);
		nOffset--;
		CString strM;
		nOffset = FindMidStr(strString,nOffset," "," ",strM);
		nOffset--;
		if (strM == "Jan")
			strM = "01";
		else if (strM == "Feb")
			strM = "02";
		else if (strM == "Mar")
			strM = "03";
		else if (strM == "Apr")
			strM = "04";
		else if (strM == "May")
			strM = "05";
		else if (strM == "Jun")
			strM = "06";
		else if (strM == "Jul")
			strM = "07";
		else if (strM == "Aug")
			strM = "08";
		else if (strM == "Sep")
			strM = "09";
		else if (strM == "Oct")
			strM = "10";
		else if (strM == "Nov")
			strM = "11";
		else if (strM == "Dec")
			strM = "12";
		else if (strM == "")
			return FALSE;
		//Sun, 13 Mar 2011 17:32:23 GMT
		CString strY;
		nOffset = FindMidStr(strString,nOffset," "," ",strY);
		nOffset--;
		CString strHMS;
		FindMidStr(strString,nOffset," "," ",strHMS);
		CString strTime;
		strTime.Format("%s-%s-%s %s",strY,strM,strD,strHMS);

		COleDateTime t_time;
		t_time=(_variant_t)(_bstr_t)strTime;
		SYSTEMTIME systime;
		VariantTimeToSystemTime(t_time, &systime);
		CTime tm(systime);
		dwServerTime = (DWORD)tm.GetTime() + 28800;
		
		//if(bstrString)
		//{
		//	::SysFreeString(bstrString);
		//	bstrString = NULL;
		//}
		
	} catch (...) 
	{
		//if(bstrString)
		//	::SysFreeString(bstrString);
		return FALSE;
	}
	//::CoUninitialize();
	return TRUE;
}