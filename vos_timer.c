/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : vos_timer.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��10��22��
  ����޸�   :
  ��������   : VOS��ʱ���Ĺ��ܷ�װ
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��10��22��
    ��    ��   : ����
    �޸�����   : �����ļ�

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
/*�ο�: https://blog.csdn.net/leo9150285/article/details/8271910*/
/*Linux��ʱ��, ���ȿ��Դﵽ����*/
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
 �� �� ��  : VOS_TimerCreate
 ��������  :    ��ʱ���Ĵ���
 �������  :    INT32 iType                             ---����
            UINT32 uiMilliTimes,                    ---���뼶��Ķ�ʱʱ��
            pfVosTimerHandlerCb pfTimerHandleCb     ---��ʱ����
            VOID *pvCtx                             ---������
 �������  : ��
 �� �� ֵ  : ���ظö�ʱ���ľ��������ʱ����Ҫ
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��22��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
    struct sigevent     stSigEvt={0};   /*__SIGRTMIN --__SIGRTMAX֮��*/
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

    /*�߳���פ��ʽ*/
    stSigEvt.sigev_value.sival_ptr = hTimerHandle;
    stSigEvt.sigev_notify = SIGEV_THREAD;
    stSigEvt.sigev_notify_function = VOS_TimerLinuxWorker;

	if (NULL != pvCtx)
	{
		hTimerHandle->pvCtx = pvCtx;
	}

	hTimerHandle->pfTimerCb = pfTimerHandleCb;
    
    /*������ʱ��*/
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
 �� �� ��  : VOS_TimerDestory
 ��������  : ��ʱ������
 �������  : VOS_TIMER_T hTimerHandle           ---��ʱ�����
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��22��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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


