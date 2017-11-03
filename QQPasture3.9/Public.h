#pragma once
//////////////////////////////////////////////////////////////////////////
//宏定义																//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//长度常量
#define TEXTLEN         256			//长文本
#define MENTEXTLEN		64			//中长文本
#define SHORTTEXTLEN	32			//短文本
#define ARGLEN          1024		
#define USERLEN         4096
#define SHORTUSERLEN	2048  
#define SHORTLEN        8
#define QQNUMLEN		11			//QQ号长度
#define QQPWLEN			17			//QQ密码长度
#define QQNAMELEN		25			//用户名长度
#define	QQVCLEN			7			//验证码长度
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//函数参数
#define GAP_DIR			0			//取当前目录
#define GAP_PATH		1			//取进程完整路径
#define GAP_Name		2			//取文件名

#define TF_MDX			0
#define TF_YMDHMS		1			//转换为年月日时分秒
#define TF_TIMETONOW	2			//给定时间到当前时间的时差
#define TF_TIMEOVER		3			//转换为时分秒

#define SS_MS			0			//微秒
#define SS_MIS			1			//毫秒
#define SS_S			2			//秒
#define SS_M			3			//分
#define SS_H			4			//小时
#define SS_D			5			//天

#define FIS_MID			0			//取文本中间
#define FIS_REVMID		1			//倒取文本中间

#define GH_MAIN			0			//主窗口句柄
#define GH_TABCTRL		1			//选择夹(插件窗口父级)
#define GH_LISTBOX		2			//列表框(文字输出)
#define GH_STATUS		3			//状态栏(文字输出)

#define GQI_NUM			0			//QQ号码
#define GQI_NAME		1			//昵称

#define PTM_NONE		0			//没有图标
#define PTM_INFO		1			//信息
#define PTM_WARNING		2			//警告
#define PTM_ERROR		3			//错误

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//自定义消息
/*SendMessage(主窗口句柄,EVENT_ALL_RELOGIN,0,0); COOKIES失效,此消息使主窗口弹出重新登录的窗口*/
#define	EVENT_ALL_RELOGIN	WM_USER+1001		//重新登录

/*char szBuf[QQVCLEN] = {0};
  SendMessage(主窗口句柄,EVENT_ALL_INPUTVERIFYCODE,(LONG)szBuf,0); 此消息使主窗口弹出验证码输入窗口,
  请将字符串变量的指针传递给wParam*/
#define EVENT_ALL_INPUTVERIFYCODE	WM_USER+1002	//输入验证码

/*以下消息由主窗口分发至插件窗口.如果需要,则请添加响应*/
#define EVENT_PL_LOGIN		WM_USER+1000		//用户登录
#define EVENT_PL_PAUSE		WM_USER+1003		//暂停
#define EVENT_PL_CONTINUE	WM_USER+1004		//继续
#define EVENT_PL_RESET		WM_USER+1005		//重启
#define EVENT_PL_CLOSE		WM_USER+1006		//关闭
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//公众函数集															//
//////////////////////////////////////////////////////////////////////////
extern "C"
{	
	/*输出彩色文本,第二个参数可以使用RGB()函数,支持\r\n换行符*/
	UINT __stdcall CC_Addtolist(LPCTSTR pStr,COLORREF pColor);
		
	/*设置主窗口状态栏上的文本,参数二为显示的时间,5000为5秒,-1则一直显示,直到函数被再次执行*/
	UINT __stdcall CC_SetStsText(LPCTSTR pStr,int nTime);
	
	/*文本加密函数,参数一:源文本,参数二:密钥,参数三:返回加密后的文本. 加密后的文本会更长,请注意越界问题*/
	UINT __stdcall CC_EncrypKey(char* strSrc, char* strKey, char* strResult);
	
	/*文本解密函数,参数一:加密文本,参数二,密钥,参数三:返回解密后的文本.*/
	UINT __stdcall CC_UncrypKey(char* strSrc, char* strKey, char* strResult);

	/*输出文本到剪切板,参数一:窗口句柄,参数二:文本指针*/
	void __stdcall CC_WriteTextToClipboard(HWND hDlg,char* szBuf);

	/*重命名文件,参数一:新路径,参数二:源路径*/
	BOOL __stdcall CC_ReNameFile(char* pTo,char* pFrom);

	/*文件是否存在,参数一:文件路径*/
	BOOL __stdcall CC_FileExteriorFile(char* FileName);
	
	/*取路径,参数一:返回字符串,参数二:请参考上面宏定义中函数参数一栏GAP_开头的内容*/
	void __stdcall CC_GetAppPath(char* sPath,int nType);
	
	/*将网络文件保存到本地,参数一:URL地址,参数二:存放路径*/
	BOOL __stdcall CC_GetUrlToFile(char* szUrl,char* strFilePath,BOOL bDontCache,LPBINDSTATUSCALLBACK callback);
	
	/*取网页文本,返回:网页文本指针;参数一:URL地址,参数二:如果无法获取中文,请设置为TRUE,转换一下
	  请在使用完返回文本后,使用CC_FreeStr释放内存*/
	char* __stdcall CC_GetUrl(char* strURL,BOOL bConvert);
	
	/*POST提交网页数据,返回:网页文本指针;参数一:域名,不需要带http,参数二:URL地址,参数三:头部参数,参数四:提交数据,
	  参数五:如果无法获取中文,请设置为TRUE,转换一下,
	  请在使用完返回文本后,使用CC_FreeStr释放内存*/
	char* __stdcall CC_PostUrl(char* pHost,char* pStrObjectName,char* pHeadres,char* sPostData,BOOL bConvert);

	/*释放内存,用于释放GetUrl,PostUrl返回的指针*/
	void __stdcall CC_FreeStr(char* str);
	
	/*宽字符转ANSI*/
	void __stdcall CC_StrConvert(char* str,int sourceCodepage,int targetCodepage);
	
	/*格林时间转换,参数一:格林时间,参数二:现行时间,参数三:返回字符,参数四:请参考TF_开头的宏定义*/
	void __stdcall CC_TimeFormat(DWORD dwtime,DWORD nowtime,char* out_time,int nType);
	
	/*超级延时,参数一:等待时间,参数二:时间类型,参考SS_开头的宏定义.
	  此函数功能与Sleep相似,主要用于主线程中,防止延时时窗口卡死,请尽量只在主线程中使用*/
	void __stdcall CC_SuperSleep(int dwWait,int dwType);
	
	/*等待线程结束,参数一:线程句柄,主要用于主线程中,防止等待线程时窗口卡死,请尽量只在主线程中使用*/
	BOOL __stdcall CC_WaitForThreadToTerminate(HANDLE hThread);
	
	/*创建线程,参数一:函数地址,参数二:参数,参数三:是否使用CC_WaitForThreadToTerminate等待线程结束*/
	void __stdcall CC_CreateThread(LPVOID pVoid,LPVOID pPrm,BOOL bWait);
	
	/*检查是不是可使用的插件,参数一:完整文件路径*/
	BOOL __stdcall CC_IsCCPlugin(char *szFileName);
	
	/*加密QQ密码,参数一:QQ密码,参数二:验证码,参数三:返回结果. QQ密码加密采用特殊MD5*/
	BOOL __stdcall CC_EncryptQQPW(char* szQQPw,char* szVCode,char* szResult);
	
	/*字符串处理函数,参数一:源文本,参数二:标志一,参数三:标志二,参数四:返回结果,参数五:处理类型,请参考FIS_开头的宏定义*/
	UINT __stdcall CC_FindIdStr(char* szRes, char* szP1, char* szP2, char* szResult, int nType);

	/*取文件MD5值,参数一:文件路径,参数二:返回结果(32位字符)*/
	BOOL __stdcall CC_GetFileMd5(char* szPath,char* szMd5);

	/*取字符串MD5值,参数一:字符串指针,参数二:返回结果(32位字符)*/
	void __stdcall CC_GetStrMd5(char* szRes,char* szMd5);

	/*托盘弹出气泡提示,参数一:标题,参数二:提示文本,参数三:图标(请参照PTM_开头的宏定义),参数四:显示时间
	  请控制弹出的频率,不要造成屏幕垃圾*/
	BOOL __stdcall CC_PopTrayMsg(LPCTSTR pTitle,LPCTSTR pStr,int nType,int nTime);

	/*取QQ信息,参数一:返回结果(长度请参照常量表),参数二:请参照GQI_开头的宏定义*/
	UINT __stdcall CC_GetQQInfo(char *szResult,int nType);

	/*取主窗口控件句柄,参数一:请参照GH_开头的宏*/
	HWND __stdcall CC_GethWnd(int nType);

	/*Ping功能,返回四次Ping的超时平均值*/
	UINT __stdcall CC_Ping(char* host);

	/*返回Cookie的gtk*/
	BOOL __stdcall CC_GetGTK(char* szGTK);
}
