/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_mutex.c
  �� �� ��   : ����
  ��    ��   : jiangkang
  ��������   : 2015��9��25��
  ����޸�   :
  ��������   : ϵͳ�߳�����ͬ���¼�������
  �����б�   :
              VOS_SM_Destroy
              VOS_SM_Init
              VOS_SM_P
              VOS_SM_V
              VOS_ThreadMutex_Destroy
              VOS_ThreadMutex_Init
              VOS_ThreadMutex_Lock
              VOS_ThreadMutex_UnLock
  �޸���ʷ   :
  1.��    ��   : 2015��9��25��
    ��    ��   : jiangkang
    �޸�����   : �����ļ�

******************************************************************************/
#include <vos_pub.h>


/*��ʼ���߳���*/
INT32 VOS_ThreadMutex_Init(VOS_RW_LOCK_S *pstMutex)
{
#if VOS_PLAT_WIN
    CRITICAL_SECTION *pstMutexLock = NULL;

    pstMutexLock = &pstMutex->stMutex;

    InitializeCriticalSection(&pstMutexLock);

#elif VOS_PLAT_LINUX
    pthread_mutex_t *pstMutexLock = NULL;

    pstMutexLock = &pstMutex->stMutex;

    pthread_mutex_init(pstMutexLock,NULL);
    
#elif VOS_PLAT_MAC


#endif


    return VOS_OK;
}

/*****************************************************************************
 *  ����: VOS_MmThreadMutex_Lock
 *  ˵��: �̼߳���
 *  *****************************************************************************/
INT32 VOS_ThreadMutex_Lock(VOS_RW_LOCK_S *pstMutex)
{
#if VOS_PLAT_WIN
        CRITICAL_SECTION *pstMutexLock = NULL;

        pstMutexLock = &pstMutex->stMutex;
        
        (VOID)EnterCriticalSection(pstMutexLock);
#elif VOS_PLAT_LINUX
        pthread_mutex_t *pstMutexLock = NULL;

        pstMutexLock = &pstMutex->stMutex;
    
        (VOID)pthread_mutex_lock(pstMutexLock);
#elif VOS_PLAT_MAC
    
#endif

    return VOS_OK;
}

/*****************************************************************************
 *  ����: VOS_ThreadMutex_UnLock
 *  ˵��: �߳̽���
 *  *****************************************************************************/
INT32 VOS_ThreadMutex_UnLock(VOS_RW_LOCK_S *pstMutex)
{
#if VOS_PLAT_WIN
            CRITICAL_SECTION *pstMutexLock = NULL;

            pstMutexLock = &pstMutex->stMutex;
            
            (VOID)LeaveCriticalSection(pstMutexLock);
#elif VOS_PLAT_LINUX
            pthread_mutex_t *pstMutexLock = NULL;

            pstMutexLock = &pstMutex->stMutex;
            
            (VOID)pthread_mutex_unlock(pstMutexLock);
#elif VOS_PLAT_MAC
        
#endif

    return VOS_OK;
}

/*****************************************************************************
 *  ����: VOS_ThreadMutex_UnLock
 *  ˵��: �߳����ͷ�
 *  *****************************************************************************/
INT32 VOS_ThreadMutex_Destroy(VOS_RW_LOCK_S *pstMutex)
{
#if VOS_PLAT_WIN
            CRITICAL_SECTION *pstMutexLock = NULL;

            pstMutexLock = &pstMutex->stMutex;
            (VOID)DeleteCriticalSection(pstMutexLock);
#elif VOS_PLAT_LINUX
            pthread_mutex_t *pstMutexLock = NULL;

            pstMutexLock = &pstMutex->stMutex;
            (VOID)pthread_mutex_destroy(pstMutexLock);
#elif VOS_PLAT_MAC
           //TODO: 
#endif

    return VOS_OK;
}

/*�ź�����ʼ��*/
INT32 VOS_SM_Init(VOS_SM_T *pstSem)
{
    INT32 uiErr = 0;
    if ( NULL == pstSem )
    {
        return VOS_ERR;
    }

    
#if VOS_PLAT_LINUX
    pthread_mutex_t *pstMutex = NULL;
    pthread_cond_t *pstCond = NULL;
    pthread_condattr_t attr;
    
    pstMutex = &pstSem->stMutex;
    pstCond = &pstSem->stCond;
    
    uiErr = pthread_mutex_init(pstMutex, NULL);
    if (uiErr != 0)
    {
        return VOS_ERR;
    }
    uiErr = pthread_condattr_init(&attr);
    if (uiErr != 0)
    {
        pthread_mutex_destroy(pstMutex);
        return VOS_ERR;
    }
    uiErr = pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
    if (uiErr != 0)
    {
        pthread_mutex_destroy(pstMutex);
        return VOS_ERR;
    }
    
    uiErr = pthread_cond_init(pstCond, &attr);
    if (uiErr != 0)
    {
        pthread_mutex_destroy(pstMutex);
        return VOS_ERR;
    }
    
#elif VOS_PLAT_WIN
   // CreateSemaphore();
#elif VOS_PLAT_MAC
             
#endif
    return VOS_OK;
}

/*�ź���P������ͬ������,������Ϊ�߳�ͬ�����*/
INT32 VOS_SM_P(VOS_SM_T *pstSem, UINT32 ulTimeOutMiliSec)
{
    INT32 uiErr = 0;
    if ( NULL == pstSem )
    {
        return VOS_ERR;
    }
#if VOS_PLAT_WIN
    
#elif VOS_PLAT_LINUX
    pthread_mutex_t *pstMutex = NULL;
    pthread_cond_t *pstCond = NULL;
    VOS_ABS_TIME_S stAbsTime = {{0}};
    
    pstMutex = &pstSem->stMutex;
    pstCond = &pstSem->stCond;
    //alway for wait
    if ( ulTimeOutMiliSec == 0)
    {
        uiErr = pthread_cond_wait(pstCond, pstMutex);
        if ( uiErr != 0 )
        {
            pthread_mutex_unlock(pstMutex);
            return VOS_ERR;
        }
        else
        {
            //OK
            pthread_mutex_unlock(pstMutex);
            return VOS_OK;
        }
    }
    else
    {
        VOS_GetAbsTime(&stAbsTime,ulTimeOutMiliSec);
        uiErr = pthread_cond_timedwait(pstCond, pstMutex, &(stAbsTime.stAbsTime));
        if(uiErr == 0)
        {
            //OK
            pthread_mutex_unlock(pstMutex);
        }
        else
        {
            //OK
            pthread_mutex_unlock(pstMutex);
            return VOS_SYS_SM_TIMEOUT;
        }

    }
#elif VOS_PLAT_MAC
                 
#endif
    return VOS_OK;
}

INT32 VOS_SM_V(VOS_SM_T *pstSem)
{
    INT32 uiErr = 0;
    if ( NULL == pstSem )
    {
        return VOS_ERR;
    }
#if VOS_PLAT_WIN
    
#elif VOS_PLAT_LINUX
    pthread_mutex_t *pstMutex = NULL;
    pthread_cond_t *pstCond = NULL;
    
    pstMutex = &pstSem->stMutex;
    pstCond = &pstSem->stCond;
    
    pthread_mutex_lock(pstMutex);
    uiErr = pthread_cond_signal(pstCond);
    if ( uiErr == 0 )
    {
        pthread_mutex_unlock(pstMutex);
    }
#elif VOS_PLAT_MAC
                 
#endif
    return VOS_OK;
}



INT32 VOS_SM_Destroy(VOS_SM_T *pstSem)
{
    if ( NULL == pstSem )
    {
        return VOS_ERR;
    }
    
#if VOS_PLAT_WIN
    
#elif VOS_PLAT_LINUX
    pthread_mutex_t *pstMutex = NULL;
    pthread_cond_t *pstCond = NULL;
    
    pstMutex = &pstSem->stMutex;
    pstCond = &pstSem->stCond;

    pthread_mutex_destroy(pstMutex);
    pthread_cond_destroy(pstCond);
#elif VOS_PLAT_MAC
                 
#endif

    return VOS_OK;
}



LONG VOS_InterlockedIncrement(ULONG *pulNums)
{
	
#if VOS_PLAT_LINUX
	*pulNums =*pulNums + 1;
#elif VOS_PLAT_WIN
	InterLockedIncrement(pulNums);
#endif

	return VOS_OK;
}
	

LONG VOS_InterlockedDecrement(ULONG *pulNums)
{
	
#if VOS_PLAT_LINUX
	*pulNums =*pulNums - 1;
#elif VOS_PLAT_WIN
	InterlockedDecrement(pulNums);
#endif

	return VOS_OK;
}





