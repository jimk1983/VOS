/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_mutex.c
  版 本 号   : 初稿
  作    者   : jiangkang
  生成日期   : 2015年9月25日
  最近修改   :
  功能描述   : 系统线程锁和同步事件的适配
  函数列表   :
              VOS_SM_Destroy
              VOS_SM_Init
              VOS_SM_P
              VOS_SM_V
              VOS_ThreadMutex_Destroy
              VOS_ThreadMutex_Init
              VOS_ThreadMutex_Lock
              VOS_ThreadMutex_UnLock
  修改历史   :
  1.日    期   : 2015年9月25日
    作    者   : jiangkang
    修改内容   : 创建文件

******************************************************************************/
#include <vos_pub.h>


/*初始化线程锁*/
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
 *  函数: VOS_MmThreadMutex_Lock
 *  说明: 线程加锁
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
 *  函数: VOS_ThreadMutex_UnLock
 *  说明: 线程解锁
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
 *  函数: VOS_ThreadMutex_UnLock
 *  说明: 线程锁释放
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

/*信号量初始化*/
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

/*信号量P操作，同步阻塞,可以作为线程同步设计*/
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





