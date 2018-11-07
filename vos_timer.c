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
#include <vos_pub.h>

#define VOS_TIMER_STARTTIME	1000

#if VOS_PLAT_WIN
struct tagVosTimer
{
	HANDLE				hTimer;
	HANDLE				hQueue;
	pfVosTimerHandlerCb pfTimerCb;
	VOID*				pvCtx;
};

typedef VOID(NTAPI * WAITORTIMERCALLBACKFUNC) (PVOID, BOOLEAN);

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
/*参考: https://blog.csdn.net/leo9150285/article/details/8271910*/
/*Linux定时器, 精度可以达到纳秒*/
struct tagLinuxVosTimer
{
    pfVosTimerHandlerCb pfTimerCb;
	VOID*				pvCtx;
};


VOID VOS_TimerLinuxWorker(union sigval argv)
{
    VOS_TIMER_T hTimerHandle = NULL;
    
    if ( NULL == argv.sival_ptr )
    {
        return;
    }

    hTimerHandle = (VOS_TIMER_T)argv.sival_ptr;

    if (NULL != hTimerHandle->pfTimerCb)
    {
        hTimerHandle->pfTimerCb(hTimerHandle->pvCtx);
    }
}

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
    VOS_TIMER_T hTimerHandle = NULL;
    INT32       iRet = 0;
    
    if ( NULL == pfTimerHandleCb  )
    {
        return NULL;
    }
    
#if VOS_PLAT_WIN
	hTimerHandle = (VOS_TIMER_T)VOS_Malloc(0, sizeof(struct tagVosTimer));
	if (NULL == hTimerHandle)
	{
		return NULL;
	}


    hTimerHandle->hQueue = CreateTimerQueue();
    if ( NULL == hTimerHandle->hQueue )
    {
		VOS_FREE(hTimerHandle);
        return NULL;
    }

	if (NULL != pvCtx)
	{
		hTimerHandle->pvCtx = pvCtx;
	}

	hTimerHandle->pfTimerCb = pfTimerHandleCb;

	if (FALSE == CreateTimerQueueTimer(&hTimerHandle->hTimer, hTimerHandle->hQueue, VOS_TimerWorker, hTimerHandle, VOS_TIMER_STARTTIME, uiMilliTimes, 0))
	{
		DeleteTimerQueue(hTimerHandle->hQueue);
		VOS_FREE(hTimerHandle);
		return NULL;
	}

#elif VOS_PLAT_LINUX

    INT32      iSecs    = 0;
    INT32      iNsecs   = 0;
    struct itimerspec   stTimerSpc={0};
    struct sigevent     stSigEvt={0};   /*__SIGRTMIN --__SIGRTMAX之间*/
    timer_t             timerid = 0;

    hTimerHandle = (VOS_TIMER_T)VOS_Malloc(0, sizeof(struct tagLinuxVosTimer));
	if (NULL == hTimerHandle)
	{
		return NULL;
	}
#if 0
    hTimerHandle->stSigEvt.sigev_value.sival_ptr = &hTimerHandle->timer;
    hTimerHandle->stSigEvt.sigev_notify = SIGEV_SIGNAL;
    hTimerHandle->stSigEvt.sigev_signo  = SIGUSR1;
    
    signal(SIGUSR1, VOS_TimerWorker);
#endif
    VOS_Mem_Zero((CHAR *)&stSigEvt, sizeof(stSigEvt));

    /*线程派驻方式*/
    stSigEvt.sigev_value.sival_ptr = hTimerHandle;
    stSigEvt.sigev_notify = SIGEV_THREAD;
    stSigEvt.sigev_notify_function = VOS_TimerLinuxWorker;

	if (NULL != pvCtx)
	{
		hTimerHandle->pvCtx = pvCtx;
	}

	hTimerHandle->pfTimerCb = pfTimerHandleCb;
    
    /*创建定时器*/
    if (timer_create(CLOCK_REALTIME, &stSigEvt, &timerid) == -1)
    {
        VOS_Free((CHAR *)hTimerHandle);
        return NULL;
    }

    if ( uiMilliTimes >= 1000 )    
    {
        iSecs  = uiMilliTimes/1000;
    }
    
    iNsecs = (uiMilliTimes%1000) * 1000 ;
    
    stTimerSpc.it_interval.tv_sec = iSecs;
    stTimerSpc.it_interval.tv_nsec = iNsecs;
    
    stTimerSpc.it_value.tv_sec = iSecs;
    stTimerSpc.it_value.tv_nsec = iNsecs;

    iRet = timer_settime(timerid, 0, &stTimerSpc, NULL);
    if( iRet )
    {
        VOS_Free((CHAR *)hTimerHandle);
        return NULL;
    }
    
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
#if VOS_PLAT_WIN    
    if ( NULL != hTimerHandle->hQueue )
    {
        DeleteTimerQueue(hTimerHandle->hQueue);
        hTimerHandle->hQueue = NULL;
    }

#elif VOS_PLAT_LINUX
    

#endif
    VOS_Free((CHAR *)hTimerHandle);
    return;
}


