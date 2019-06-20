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
 �� �� ��  : VOS_TimerParamsFree
 ��������  : �����ڴ���ͷ�(������ʱ��ʱ����Ĳ���)
 �������  : VOS_TIMER_T hTimerHandle           ---��ʱ�����
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��26��
    ��    ��   : yifanKong
    �޸�����   : �����ɺ���

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



