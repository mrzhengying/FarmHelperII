#pragma once
//////////////////////////////////////////////////////////////////////////
//�궨��																//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//���ȳ���
#define TEXTLEN         256			//���ı�
#define MENTEXTLEN		64			//�г��ı�
#define SHORTTEXTLEN	32			//���ı�
#define ARGLEN          1024		
#define USERLEN         4096
#define SHORTUSERLEN	2048  
#define SHORTLEN        8
#define QQNUMLEN		11			//QQ�ų���
#define QQPWLEN			17			//QQ���볤��
#define QQNAMELEN		25			//�û�������
#define	QQVCLEN			7			//��֤�볤��
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//��������
#define GAP_DIR			0			//ȡ��ǰĿ¼
#define GAP_PATH		1			//ȡ��������·��
#define GAP_Name		2			//ȡ�ļ���

#define TF_MDX			0
#define TF_YMDHMS		1			//ת��Ϊ������ʱ����
#define TF_TIMETONOW	2			//����ʱ�䵽��ǰʱ���ʱ��
#define TF_TIMEOVER		3			//ת��Ϊʱ����

#define SS_MS			0			//΢��
#define SS_MIS			1			//����
#define SS_S			2			//��
#define SS_M			3			//��
#define SS_H			4			//Сʱ
#define SS_D			5			//��

#define FIS_MID			0			//ȡ�ı��м�
#define FIS_REVMID		1			//��ȡ�ı��м�

#define GH_MAIN			0			//�����ھ��
#define GH_TABCTRL		1			//ѡ���(������ڸ���)
#define GH_LISTBOX		2			//�б��(�������)
#define GH_STATUS		3			//״̬��(�������)

#define GQI_NUM			0			//QQ����
#define GQI_NAME		1			//�ǳ�

#define PTM_NONE		0			//û��ͼ��
#define PTM_INFO		1			//��Ϣ
#define PTM_WARNING		2			//����
#define PTM_ERROR		3			//����

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//�Զ�����Ϣ
/*SendMessage(�����ھ��,EVENT_ALL_RELOGIN,0,0); COOKIESʧЧ,����Ϣʹ�����ڵ������µ�¼�Ĵ���*/
#define	EVENT_ALL_RELOGIN	WM_USER+1001		//���µ�¼

/*char szBuf[QQVCLEN] = {0};
  SendMessage(�����ھ��,EVENT_ALL_INPUTVERIFYCODE,(LONG)szBuf,0); ����Ϣʹ�����ڵ�����֤�����봰��,
  �뽫�ַ���������ָ�봫�ݸ�wParam*/
#define EVENT_ALL_INPUTVERIFYCODE	WM_USER+1002	//������֤��

/*������Ϣ�������ڷַ����������.�����Ҫ,���������Ӧ*/
#define EVENT_PL_LOGIN		WM_USER+1000		//�û���¼
#define EVENT_PL_PAUSE		WM_USER+1003		//��ͣ
#define EVENT_PL_CONTINUE	WM_USER+1004		//����
#define EVENT_PL_RESET		WM_USER+1005		//����
#define EVENT_PL_CLOSE		WM_USER+1006		//�ر�
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//���ں�����															//
//////////////////////////////////////////////////////////////////////////
extern "C"
{	
	/*�����ɫ�ı�,�ڶ�����������ʹ��RGB()����,֧��\r\n���з�*/
	UINT __stdcall CC_Addtolist(LPCTSTR pStr,COLORREF pColor);
		
	/*����������״̬���ϵ��ı�,������Ϊ��ʾ��ʱ��,5000Ϊ5��,-1��һֱ��ʾ,ֱ���������ٴ�ִ��*/
	UINT __stdcall CC_SetStsText(LPCTSTR pStr,int nTime);
	
	/*�ı����ܺ���,����һ:Դ�ı�,������:��Կ,������:���ؼ��ܺ���ı�. ���ܺ���ı������,��ע��Խ������*/
	UINT __stdcall CC_EncrypKey(char* strSrc, char* strKey, char* strResult);
	
	/*�ı����ܺ���,����һ:�����ı�,������,��Կ,������:���ؽ��ܺ���ı�.*/
	UINT __stdcall CC_UncrypKey(char* strSrc, char* strKey, char* strResult);

	/*����ı������а�,����һ:���ھ��,������:�ı�ָ��*/
	void __stdcall CC_WriteTextToClipboard(HWND hDlg,char* szBuf);

	/*�������ļ�,����һ:��·��,������:Դ·��*/
	BOOL __stdcall CC_ReNameFile(char* pTo,char* pFrom);

	/*�ļ��Ƿ����,����һ:�ļ�·��*/
	BOOL __stdcall CC_FileExteriorFile(char* FileName);
	
	/*ȡ·��,����һ:�����ַ���,������:��ο�����궨���к�������һ��GAP_��ͷ������*/
	void __stdcall CC_GetAppPath(char* sPath,int nType);
	
	/*�������ļ����浽����,����һ:URL��ַ,������:���·��*/
	BOOL __stdcall CC_GetUrlToFile(char* szUrl,char* strFilePath,BOOL bDontCache,LPBINDSTATUSCALLBACK callback);
	
	/*ȡ��ҳ�ı�,����:��ҳ�ı�ָ��;����һ:URL��ַ,������:����޷���ȡ����,������ΪTRUE,ת��һ��
	  ����ʹ���귵���ı���,ʹ��CC_FreeStr�ͷ��ڴ�*/
	char* __stdcall CC_GetUrl(char* strURL,BOOL bConvert);
	
	/*POST�ύ��ҳ����,����:��ҳ�ı�ָ��;����һ:����,����Ҫ��http,������:URL��ַ,������:ͷ������,������:�ύ����,
	  ������:����޷���ȡ����,������ΪTRUE,ת��һ��,
	  ����ʹ���귵���ı���,ʹ��CC_FreeStr�ͷ��ڴ�*/
	char* __stdcall CC_PostUrl(char* pHost,char* pStrObjectName,char* pHeadres,char* sPostData,BOOL bConvert);

	/*�ͷ��ڴ�,�����ͷ�GetUrl,PostUrl���ص�ָ��*/
	void __stdcall CC_FreeStr(char* str);
	
	/*���ַ�תANSI*/
	void __stdcall CC_StrConvert(char* str,int sourceCodepage,int targetCodepage);
	
	/*����ʱ��ת��,����һ:����ʱ��,������:����ʱ��,������:�����ַ�,������:��ο�TF_��ͷ�ĺ궨��*/
	void __stdcall CC_TimeFormat(DWORD dwtime,DWORD nowtime,char* out_time,int nType);
	
	/*������ʱ,����һ:�ȴ�ʱ��,������:ʱ������,�ο�SS_��ͷ�ĺ궨��.
	  �˺���������Sleep����,��Ҫ�������߳���,��ֹ��ʱʱ���ڿ���,�뾡��ֻ�����߳���ʹ��*/
	void __stdcall CC_SuperSleep(int dwWait,int dwType);
	
	/*�ȴ��߳̽���,����һ:�߳̾��,��Ҫ�������߳���,��ֹ�ȴ��߳�ʱ���ڿ���,�뾡��ֻ�����߳���ʹ��*/
	BOOL __stdcall CC_WaitForThreadToTerminate(HANDLE hThread);
	
	/*�����߳�,����һ:������ַ,������:����,������:�Ƿ�ʹ��CC_WaitForThreadToTerminate�ȴ��߳̽���*/
	void __stdcall CC_CreateThread(LPVOID pVoid,LPVOID pPrm,BOOL bWait);
	
	/*����ǲ��ǿ�ʹ�õĲ��,����һ:�����ļ�·��*/
	BOOL __stdcall CC_IsCCPlugin(char *szFileName);
	
	/*����QQ����,����һ:QQ����,������:��֤��,������:���ؽ��. QQ������ܲ�������MD5*/
	BOOL __stdcall CC_EncryptQQPW(char* szQQPw,char* szVCode,char* szResult);
	
	/*�ַ���������,����һ:Դ�ı�,������:��־һ,������:��־��,������:���ؽ��,������:��������,��ο�FIS_��ͷ�ĺ궨��*/
	UINT __stdcall CC_FindIdStr(char* szRes, char* szP1, char* szP2, char* szResult, int nType);

	/*ȡ�ļ�MD5ֵ,����һ:�ļ�·��,������:���ؽ��(32λ�ַ�)*/
	BOOL __stdcall CC_GetFileMd5(char* szPath,char* szMd5);

	/*ȡ�ַ���MD5ֵ,����һ:�ַ���ָ��,������:���ؽ��(32λ�ַ�)*/
	void __stdcall CC_GetStrMd5(char* szRes,char* szMd5);

	/*���̵���������ʾ,����һ:����,������:��ʾ�ı�,������:ͼ��(�����PTM_��ͷ�ĺ궨��),������:��ʾʱ��
	  ����Ƶ�����Ƶ��,��Ҫ�����Ļ����*/
	BOOL __stdcall CC_PopTrayMsg(LPCTSTR pTitle,LPCTSTR pStr,int nType,int nTime);

	/*ȡQQ��Ϣ,����һ:���ؽ��(��������ճ�����),������:�����GQI_��ͷ�ĺ궨��*/
	UINT __stdcall CC_GetQQInfo(char *szResult,int nType);

	/*ȡ�����ڿؼ����,����һ:�����GH_��ͷ�ĺ�*/
	HWND __stdcall CC_GethWnd(int nType);

	/*Ping����,�����Ĵ�Ping�ĳ�ʱƽ��ֵ*/
	UINT __stdcall CC_Ping(char* host);

	/*����Cookie��gtk*/
	BOOL __stdcall CC_GetGTK(char* szGTK);
}
