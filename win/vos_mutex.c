/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : vos_mutex.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年10月17日
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
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/
#include "Common/common.h"


/*****************************************************************************
 函 数 名  : VOS_ThreadMutex_Init
 功能描述  : 初始化线程锁
 输入参数  : VOS_RW_LOCK_S *pstMutex  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

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
 函 数 名  : VOS_ThreadMutex_Lock
 功能描述  : 线程加锁
 输入参数  : VOS_RW_LOCK_S *pstMutex  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

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
 函 数 名  : VOS_ThreadMutex_UnLock
 功能描述  : 线程解锁
 输入参数  : VOS_RW_LOCK_S *pstMutex  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

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
 函 数 名  : VOS_ThreadMutex_Destroy
 功能描述  : 线程锁释放
 输入参数  : VOS_RW_LOCK_S *pstMutex  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

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
 函 数 名  : VOS_SM_Init
 功能描述  : 信号量初始化
 输入参数  : VOS_SM_T *pstSem  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

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
 函 数 名  : VOS_SM_P
 功能描述  : 信号量P操作，同步阻塞,可以作为线程同步?-
                 ?计
 输入参数  : VOS_SM_T *pstSem         
             UINT32 ulTimeOutMiliSec  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

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
 函 数 名  : VOS_SM_V
 功能描述  : 信号量V操作
 输入参数  : VOS_SM_T *pstSem  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

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
 函 数 名  : VOS_SM_Destroy
 功能描述  : 信号量销毁
 输入参数  : VOS_SM_T *pstSem  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

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
 函 数 名  : VOS_InterlockedIncrement
 功能描述  : 原子加操作
 输入参数  : volatile LONG *plNums  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

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
 函 数 名  : VOS_InterlockedDecrement
 功能描述  : 原子减操作
 输入参数  : volatile LONG *plNums  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

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
 函 数 名  : VOS_InterlockedExchange
 功能描述  : 原子赋值操作
 输入参数  : 
 输出参数  : 
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月14日
    作    者   : 王春军
    修改内容   : 新生成函数

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





