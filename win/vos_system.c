/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : vos_system.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年10月25日
  最近修改   :
  功能描述   : VOS系统功能实现
  函数列表   :
  修改历史   :
  1.日    期   : 2018年10月25日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/

#include "Common/common.h"


#if VOS_PLAT_WIN32
#include <tlhelp32.h>	/*进程快照头文件*/
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib,"Ole32.Lib")

#endif


/*****************************************************************************
 函 数 名  : VOS_API_CreateProcess
 功能描述  :    创建进程，创建成功后，会阻塞住当前的线程
 输入参数  :    CHAR *pcProcPath  --进程绝对路径,示例: "C:\\Program Files\\AAA.exe"
            CHAR *pcCmdArgv   --命令参数
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月25日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
BOOL    VOS_API_CreateProcess(CHAR *pcProcPath, CHAR *pcCmdArgv, BOOL bAsynchronous)
{
    BOOL    bRet = VOS_FALSE;
    
    if ( NULL == pcProcPath )
    {
        return VOS_FALSE;
    }

#if VOS_PLAT_WIN32
    STARTUPINFOA            stStartUpInfo;
    PROCESS_INFORMATION     stProcessInfo;
    DWORD	                dwRet = 0;
    DWORD                   dwErrCode = 0;
	//HDESK					hDesk;
	//hDesk = CreateDesktopA("MyDesk", NULL, NULL, 0, GENERIC_ALL, NULL);
	//ASSERT(hDesk);

    VOS_Mem_Zero((CHAR *)&stStartUpInfo, sizeof(stStartUpInfo));
    VOS_Mem_Zero((CHAR *)&stProcessInfo, sizeof(stProcessInfo));

    stStartUpInfo.cb = sizeof(stStartUpInfo);
	//stStartUpInfo.lpDesktop = _T("MyDesk");
    //cjwang delete begin on 2018/11/22 
    stStartUpInfo.dwFlags = STARTF_USESHOWWINDOW;
    stStartUpInfo.wShowWindow = SW_HIDE;
    //cjwang delete end on 2018/11/22 

	if ( NULL == pcCmdArgv )
	{
		bRet = CreateProcessA(
			pcProcPath,
			NULL,
			NULL,
			NULL,
			FALSE,
            NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&stStartUpInfo,
			&stProcessInfo);
		if (bRet)
		{
			/*TODO: 需要考虑句柄关闭进程的情况，暂时先阻塞，外部自行创建线程*/
            if (FALSE == bAsynchronous)
            {
                dwRet = WaitForSingleObject(stProcessInfo.hProcess, INFINITE);
            }
			//
			CloseHandle(stProcessInfo.hProcess);
			CloseHandle(stProcessInfo.hThread);

			return VOS_TRUE;
		}
		else
		{
			dwErrCode = VOS_GetLastError();
		}
	}
	else
	{
		CHAR acCmdLine[MAX_PATH] = {0};

		VOS_Snprintf(acCmdLine,MAX_PATH, "\"%s\" %s", pcProcPath, pcCmdArgv);
		bRet = CreateProcessA(
			NULL,
			acCmdLine,
			NULL,
			NULL,
			FALSE,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&stStartUpInfo,
			&stProcessInfo);
		if (bRet)
		{
			/*TODO: 需要考虑句柄关闭进程的情况，暂时先阻塞，外部自行创建线程*/
            if (FALSE == bAsynchronous)
            {
                dwRet = WaitForSingleObject(stProcessInfo.hProcess, INFINITE);
            }
			CloseHandle(stProcessInfo.hProcess);
			CloseHandle(stProcessInfo.hThread);

			return VOS_TRUE;
		}
		else
		{
			dwErrCode = VOS_GetLastError();
		}
	}
#elif 



#endif
    

    return VOS_FALSE;
}



/*****************************************************************************
 函 数 名  : VOS_API_KillProcess
 功能描述  : 强杀进程
 输入参数  : CHAR *pcProcName	---强杀进程的绝对路径名称
 输出参数  : 无
 返 回 值  : INT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年10月17日
	作    者   : 蒋康
	修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_API_KillProcess(CHAR *pcProcName)
{
#if VOS_PLAT_WIN32 
	HANDLE hSnapshot;
	PROCESSENTRY32 lppe;
	UINT32 uiProcMatch = 0;
	WCHAR	wcProcName[MAX_PATH] = {0};

	(VOID)VOS_UTILS_AscIItoUnicode(pcProcName, wcProcName);

	//创建系统快照 
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == NULL)
	{
		return VOS_ERR;
	}

	//初始化 lppe 的大小 
	lppe.dwSize = sizeof(lppe);

	//查找第一个进程 
	if (!Process32First(hSnapshot, &lppe))
		return VOS_ERR;
    if (NULL == pcProcName)
    {
        return VOS_ERR;
    }
	do
	{
        if (0 == _wcsicmp(lppe.szExeFile, wcProcName))
        {
            uiProcMatch = 1;
        }
		if (1 == uiProcMatch)
		{
			DWORD xCode;
			HANDLE hProc;
			hProc = OpenProcess(PROCESS_TERMINATE, FALSE, lppe.th32ProcessID);
			if (!GetExitCodeProcess(hProc, &xCode))
			{
				TerminateProcess(hProc, xCode);
			}

			uiProcMatch = 0;
		}
	}while (Process32Next(hSnapshot, &lppe)); //查找下一个进程 

	if (!CloseHandle(hSnapshot))
		return VOS_ERR;
#endif
	return VOS_OK;
}

/*****************************************************************************
 函 数 名  : VOS_API_ProcessIsExist
 功能描述  : 进程是否存在
 输入参数  : CHAR *pcProcName	---查找进程的绝对路径名称
 输出参数  : 无
 返 回 值  : INT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年11月15日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_API_ProcessIsExist(CHAR *pcProcName)
{
    UINT32 uiProcMatch = 0;
#if VOS_PLAT_WIN32 
    HANDLE hSnapshot;
    PROCESSENTRY32 lppe;
    WCHAR   wcProcName[MAX_PATH] = { 0 };

    (VOID)VOS_UTILS_AscIItoUnicode(pcProcName, wcProcName);

    //创建系统快照 
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if ( hSnapshot == NULL )
    {
        return VOS_ERR;
    }

    //初始化 lppe 的大小 
    lppe.dwSize = sizeof(lppe);

    //查找第一个进程 
    if (!Process32First(hSnapshot, &lppe))
        return VOS_ERR;

    if (NULL == pcProcName)
    {
        return VOS_ERR;
    }
    do
    {
        if (0 == _wcsicmp(lppe.szExeFile, wcProcName))
        {
            uiProcMatch = 1;
            break;
        }
    } while (Process32Next(hSnapshot, &lppe)); //查找下一个进程 

    if (!CloseHandle(hSnapshot))
    {
        return VOS_ERR;
    }
#endif
    if ( 0 == uiProcMatch )
    {
        return VOS_ERR;
    }

    return VOS_OK;
}
