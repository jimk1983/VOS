/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : vos_event.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年10月17日
  最近修改   :
  功能描述   : 事件
  函数列表   :
  修改历史   :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/

#include "Common/common.h"

/*****************************************************************************
 函 数 名  : VOS_EventCreate
 功能描述  : 创建事件	
 输入参数  : CHAR *pcName			---事件名称  
             BOOL bManualReset		---是否手工自己Reset
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOS_EVENT_T VOS_EventCreate(const CHAR *pcName, BOOL bManualReset)
{
	VOS_EVENT_T hEvent = NULL;

#if VOS_PLAT_WIN32
	hEvent = CreateEventA(NULL, bManualReset, FALSE, pcName);
	if (NULL == hEvent)
	{
		return VOS_INVALID_HANDLE;
	}

	//ResetEvent(hEvent);
#elif VOS_PLAT_LINUX


#endif
	return hEvent;
}



/*****************************************************************************
 函 数 名  : VOS_EventDelete
 功能描述  : 删除事件
 输入参数  : VOS_EVENT_T hEvent  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_EventDelete(VOS_EVENT_T hEvent)
{
	if (VOS_INVALID_HANDLE != hEvent)
	{
#if VOS_PLAT_WIN32
		CloseHandle(hEvent);
#elif VOS_PLAT_LINUX


#endif
	}
}

/*****************************************************************************
 函 数 名  : VOS_EventNotify
 功能描述  : 事件通知
 输入参数  : VOS_EVENT_T hEvent  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_EventNotify(VOS_EVENT_T hEvent)
{
	if (VOS_INVALID_HANDLE != hEvent)
	{
#if VOS_PLAT_WIN32
		SetEvent(hEvent);
#elif VOS_PLAT_LINUX


#endif
	}
}

/*****************************************************************************
 函 数 名  : VOS_EventReset
 功能描述  : 重置事件
 输入参数  : VOS_EVENT_T hEvent  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月20日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_EventReset(VOS_EVENT_T hEvent)
{
    if ( VOS_INVALID_HANDLE != hEvent )
    {
       ResetEvent(hEvent);
    }
	
}
/*****************************************************************************
 函 数 名  : VOS_EventOpen
 功能描述  : 事件打开
 输入参数  : char *pcName  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOS_EVENT_T VOS_EventOpen(CHAR *pcName)
{
	VOS_EVENT_T hEvent = NULL;
	if (NULL == pcName)
	{
		return NULL;
	}

#if VOS_PLAT_WIN32
	hEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, pcName);
	if (VOS_INVALID_HANDLE == hEvent)
	{
		return NULL;
	}
#elif VOS_PLAT_LINUX


#endif

	return hEvent;
}


/*****************************************************************************
 函 数 名  : VOS_EventWaiting
 功能描述  : 事件等待
 输入参数  : VOS_EVENT_T hEvent  
             UINT32 uiTimeout		---超时时间，以毫秒为单位
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_EventWaiting(VOS_EVENT_T hEvent, UINT32 uiTimeout)
{
	if (VOS_INVALID_HANDLE == hEvent)
	{
		return VOS_ERR;
	}

#if VOS_PLAT_WIN32
	DWORD	dwRet = 0;
	DWORD   dwTimeout = 0;

	if (0 == uiTimeout)
	{
		dwTimeout = INFINITE;
	}
	else
	{
		dwTimeout = uiTimeout;
	}

	dwRet = WaitForSingleObject(hEvent, dwTimeout);
	switch (dwRet)
	{
		case WAIT_OBJECT_0:
            return VOS_OK;
		case WAIT_TIMEOUT:
			return VOS_SYS_TIMEOUT;
		default:
			break;
	}

#elif VOS_PLAT_LINUX


#endif
	return VOS_OK;
}