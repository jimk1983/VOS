/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : vos_mutex.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��10��17��
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
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/
#include "Common/common.h"


/*****************************************************************************
 �� �� ��  : VOS_ThreadMutex_Init
 ��������  : ��ʼ���߳���
 �������  : VOS_RW_LOCK_S *pstMutex  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32 VOS_ThreadMutex_Init(VOS_RW_LOCK_S *pstMutex)
{
#if VOS_PLAT_WIN32
    CRITICAL_SECTION *pstMutexLock = NULL;

    pstMutexLock = &pstMutex->stMutex;

    InitializeCriticalSection(pstMutexLock);

#elif VOS_PLAT_LINUX
    pthread_mutex_t *pstMutexLock = NULL;

    pstMutexLock = &pstMutex->stMutex;

    pthread_mutex_init(pstMutexLock,NULL);
    
#elif VOS_PLAT_MAC


#endif


    return VOS_OK;
}



/*****************************************************************************
 �� �� ��  : VOS_ThreadMutex_Lock
 ��������  : �̼߳���
 �������  : VOS_RW_LOCK_S *pstMutex  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32 VOS_ThreadMutex_Lock(VOS_RW_LOCK_S *pstMutex)
{
#if VOS_PLAT_WIN32
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
 �� �� ��  : VOS_ThreadMutex_UnLock
 ��������  : �߳̽���
 �������  : VOS_RW_LOCK_S *pstMutex  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32 VOS_ThreadMutex_UnLock(VOS_RW_LOCK_S *pstMutex)
{
#if VOS_PLAT_WIN32
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
 �� �� ��  : VOS_ThreadMutex_Destroy
 ��������  : �߳����ͷ�
 �������  : VOS_RW_LOCK_S *pstMutex  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32 VOS_ThreadMutex_Destroy(VOS_RW_LOCK_S *pstMutex)
{
#if VOS_PLAT_WIN32
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



/*****************************************************************************
 �� �� ��  : VOS_SM_Init
 ��������  : �ź�����ʼ��
 �������  : VOS_SM_T *pstSem  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
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
    
#elif VOS_PLAT_WIN32
   // CreateSemaphore();

	UNREFERENCED_PARAMETER(uiErr);
#elif VOS_PLAT_MAC
             
#endif
    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : VOS_SM_P
 ��������  : �ź���P������ͬ������,������Ϊ�߳�ͬ��?-
                 ?��
 �������  : VOS_SM_T *pstSem         
             UINT32 ulTimeOutMiliSec  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32 VOS_SM_P(VOS_SM_T *pstSem, UINT32 ulTimeOutMiliSec)
{
    INT32 uiErr = 0;
    if ( NULL == pstSem )
    {
        return VOS_ERR;
    }
#if VOS_PLAT_WIN32
	UNREFERENCED_PARAMETER(uiErr);
	UNREFERENCED_PARAMETER(ulTimeOutMiliSec);
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



/*****************************************************************************
 �� �� ��  : VOS_SM_V
 ��������  : �ź���V����
 �������  : VOS_SM_T *pstSem  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32 VOS_SM_V(VOS_SM_T *pstSem)
{
    INT32 uiErr = 0;
    if ( NULL == pstSem )
    {
        return VOS_ERR;
    }
#if VOS_PLAT_WIN32
	UNREFERENCED_PARAMETER(uiErr);
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


/*****************************************************************************
 �� �� ��  : VOS_SM_Destroy
 ��������  : �ź�������
 �������  : VOS_SM_T *pstSem  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32 VOS_SM_Destroy(VOS_SM_T *pstSem)
{
    if ( NULL == pstSem )
    {
        return VOS_ERR;
    }
    
#if VOS_PLAT_WIN32
    
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


/*****************************************************************************
 �� �� ��  : VOS_InterlockedIncrement
 ��������  : ԭ�ӼӲ���
 �������  : volatile LONG *plNums  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VOS_InterlockedIncrement(volatile LONG *plNums)
{
	
#if VOS_PLAT_LINUX
	*pulNums =*pulNums + 1;
#elif VOS_PLAT_WIN32
	InterlockedIncrement(plNums);
#endif

	return VOS_OK;
}
	

/*****************************************************************************
 �� �� ��  : VOS_InterlockedDecrement
 ��������  : ԭ�Ӽ�����
 �������  : volatile LONG *plNums  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VOS_InterlockedDecrement(volatile LONG *plNums)
{
#if VOS_PLAT_LINUX
	*pulNums =*pulNums - 1;
#elif VOS_PLAT_WIN32
	InterlockedDecrement(plNums);
#endif

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : VOS_InterlockedExchange
 ��������  : ԭ�Ӹ�ֵ����
 �������  : 
 �������  : 
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��14��
    ��    ��   : ������
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VOS_InterlockedExchange(volatile LONG *plNums, LONG Value)
{
#if VOS_PLAT_LINUX
    *pulNums = Value;
#elif VOS_PLAT_WIN32
    InterlockedExchange(plNums, Value);
#endif

    return VOS_OK;
}





