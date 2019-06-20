/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : vos_timer.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年10月22日
  最近修改   :
  功能描述   : VOS定时器的功能封装
  函数列表   :
  修改历史   :
  1.日    期   : 2018年10月22日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/
    
#include "Common/common.h"

#define VOS_TIMER_STARTTIME	1000

#if VOS_PLAT_WIN32
struct tagVosTimer
{
	HANDLE				hTimer;
	HANDLE				hQueue;
	pfVosTimerHandlerCb pfTimerCb;
	VOID*				pvCtx;
    MMRESULT            hEventTimer;
};

typedef VOID(NTAPI * WAITORTIMERCALLBACKFUNC) (PVOID, BOOLEAN);

void CALLBACK VOS_EventTimerWorker(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
    VOS_TIMER_T hTimerHandle = NULL;

    if ( 0 == dwUser)
    {
        return;
    }

    hTimerHandle = (VOS_TIMER_T)dwUser;

    if (NULL != hTimerHandle->pfTimerCb)
    {
        hTimerHandle->pfTimerCb(hTimerHandle->pvCtx);
    }

    UNREFERENCED_PARAMETER(uTimerID);
    UNREFERENCED_PARAMETER(uMsg);
    UNREFERENCED_PARAMETER(dw1);
    UNREFERENCED_PARAMETER(dw2);
}

VOID NTAPI VOS_TimerWorker(PVOID pvArgv, BOOLEAN bIsTimeout)
{
	VOS_TIMER_T hTimerHandle = NULL;

	if (NULL == pvArgv)
	{
		return;
	}

	hTimerHandle = (VOS_TIMER_T)pvArgv;

	if (NULL != hTimerHandle->pfTimerCb)
	{
		hTimerHandle->pfTimerCb(hTimerHandle->pvCtx);
	}

	UNREFERENCED_PARAMETER(bIsTimeout);
}

#elif VOS_PLAT_LINUX

#endif

/*****************************************************************************
 函 数 名  : VOS_TimerCreate
 功能描述  :    定时器的创建
 输入参数  :    INT32 iType                             ---类型
            UINT32 uiMilliTimes,                    ---毫秒级别的定时时间
            pfVosTimerHandlerCb pfTimerHandleCb     ---定时处理
            VOID *pvCtx                             ---上下文
 输出参数  : 无
 返 回 值  : 返回该定时器的句柄，销毁时候需要
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月22日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOS_TIMER_T VOS_TimerCreate(UINT32 uiMilliTimes, pfVosTimerHandlerCb pfTimerHandleCb, VOID *pvCtx)

{
    VOS_TIMER_T hTimerHandle = VOS_INVALID_HANDLE;
    
    if ( NULL == pfTimerHandleCb  )
    {
        return NULL;
    }
    
#if VOS_PLAT_WIN32
	hTimerHandle = (VOS_TIMER_T)VOS_MALLOC(0, sizeof(struct tagVosTimer));
	if (NULL == hTimerHandle)
	{
		return NULL;
	}

    if (NULL != pvCtx)
    {
        hTimerHandle->pvCtx = pvCtx;
    }

    hTimerHandle->pfTimerCb = pfTimerHandleCb;
    hTimerHandle->hQueue = NULL;
    hTimerHandle->hEventTimer = 0;

#if 0
    hTimerHandle->hQueue = CreateTimerQueue();
    if ( NULL == hTimerHandle->hQueue )
    {
		VOS_FREE(hTimerHandle);
        return NULL;
    }

	if (FALSE == CreateTimerQueueTimer(&hTimerHandle->hTimer, hTimerHandle->hQueue, VOS_TimerWorker, hTimerHandle, VOS_TIMER_STARTTIME, uiMilliTimes, 0))
	{
		DeleteTimerQueue(hTimerHandle->hQueue);
		VOS_FREE(hTimerHandle);
		return NULL;
	}
#else
    hTimerHandle->hEventTimer = timeSetEvent(uiMilliTimes, 0, (LPTIMECALLBACK)VOS_EventTimerWorker,(DWORD_PTR)hTimerHandle, TIME_PERIODIC);
    if ( 0 == hTimerHandle->hEventTimer )
    {
        VOS_FREE(hTimerHandle);
        return NULL;
    }
#endif
#elif VOS_PLAT_LINUX


#endif
    return hTimerHandle;
}


/*****************************************************************************
 函 数 名  : VOS_TimerDestory
 功能描述  : 定时器销毁
 输入参数  : VOS_TIMER_T hTimerHandle           ---定时器句柄
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月22日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_TimerDestory(VOS_TIMER_T hTimerHandle)
{
    if ( NULL == hTimerHandle )
    {
        return;
    }
    
    if ( NULL != hTimerHandle->hQueue )
    {
        DeleteTimerQueue(hTimerHandle->hQueue);
        hTimerHandle->hQueue = NULL;
    }

    if ( 0 != hTimerHandle->hEventTimer )
    {
        timeKillEvent(hTimerHandle->hEventTimer);
        hTimerHandle->hEventTimer = 0;
    }

	free(hTimerHandle);

    return;
}

/*****************************************************************************
 函 数 名  : VOS_TimerParamsFree
 功能描述  : 参数内存的释放(启动定时器时传入的参数)
 输入参数  : VOS_TIMER_T hTimerHandle           ---定时器句柄
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月26日
    作    者   : yifanKong
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_TimerParamsFree(VOS_TIMER_T hTimerHandle)
{
    if ( NULL == hTimerHandle )
    {
        return;
    }
    
	if (NULL!=hTimerHandle->pvCtx){
		VOS_FREE(hTimerHandle->pvCtx);
	} 

    return;
}



