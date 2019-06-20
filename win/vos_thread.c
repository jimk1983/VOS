/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : vos_thread.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年10月16日
  最近修改   :
  功能描述   : 对线程创建的系统适配
  函数列表   :
  修改历史   :
  1.日    期   : 2018年10月16日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/

#include "Common/common.h"

/*****************************************************************************
 函 数 名  : VOS_Thread_Create
 功能描述  : 线程创建
 输入参数  : CHAR *pcName                       --保存句柄的名称, 可以为NULL
           VOID(* pfvThread)(VOID *arg)     --线程处理函数
           VOID *pvArg                      --线程参数
           INT32 iStacksize                 --设置线程的栈大小，默认填写0
           INT32 iPrio                      --设置线程的优先级，默认填写0
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月16日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOS_THREAD_T VOS_Thread_Create(const char *pcName, VOID(* pfvThread)(VOID *arg), VOID *pvArg, INT32 iStacksize, INT32 iPrio)
{
    VOS_THREAD_T handle = 0 ;
	
#if VOS_PLAT_LINUX
    LONG lRet = 0;
    lRet = pthread_create(&handle, NULL, (VOID *)pfvThread, pvArg);
    if ( 0 != lRet )
    {
        VOS_Printf("CreateThread failed!\n"); 
        return 0;
    }
#elif VOS_PLAT_WIN32
    handle = (VOS_THREAD_T)_beginthreadex(
        NULL,                            // security attributes
        iStacksize,                       // stack size  
        (_beginthreadex_proc_type)pfvThread,  // thread function name
        pvArg,                             // argument to thread function 
        0,                               // creation flags 
        NULL);                           // returns the thread identifier 

    if (handle == NULL) 
    {
        printf("CreateThread failed with %d.\n", GetLastError()); 
        return VOS_INVALID_HANDLE;
    }
    if (!SetThreadPriority(handle, iPrio))
		printf("SetThreadPriority failed with %d\n", GetLastError());
    
#elif VOS_PLAT_MAC

#endif

	UNREFERENCED_PARAMETER(pcName);
	return handle;
}

/*****************************************************************************
 函 数 名  : VOS_GetLastError
 功能描述  : 获取最终的错误码
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月16日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
LONG VOS_GetLastError()
{ 
#if VOS_PLAT_LINUX
    return errno;
#elif VOS_PLAT_WIN32
    return GetLastError();
#endif
}




