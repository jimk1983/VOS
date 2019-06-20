/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : vos_mutex.h
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年10月17日
  最近修改   :
  功能描述   : 线程锁以及信号量
  函数列表   :
  修改历史   :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/

#ifndef _VOS_MUTEX_H_
#define _VOS_MUTEX_H_


//SM_P(), SM_V(), the same as EventWait(), EventNotify()
#if VOS_PLAT_LINUX
typedef struct tagLinuxSem
{
    UINT32 V;
    pthread_mutex_t     stMutex;
    pthread_cond_t      stCond;
}LINUX_SEM_S;
typedef LINUX_SEM_S VOS_SM_T;
#elif VOS_PLAT_MAC
typedef struct tagMacOSSem
{
    UINT32 V;
    pthread_mutex_t     stMutex;
    pthread_cond_t      stCond;
}MACOS_SEM_S;
typedef MACOS_SEM_S VOS_SM_T;

#elif VOS_PLAT_WIN32
#ifndef HANDLE
//#define HANDLE (VOID *)
#endif
typedef HANDLE  VOS_SM_T;
#endif


/*读写共享锁*/
#if VOS_PLAT_LINUX
typedef struct tagLinuxRWLock
{
    pthread_mutex_t     stMutex;
    pthread_cond_t      stCond;
}LINUX_RW_LOCK_S;
typedef LINUX_RW_LOCK_S VOS_RW_LOCK_S;
#elif VOS_PLAT_WIN32
typedef struct tagWinRWLock
{
    CRITICAL_SECTION stMutex;
    INT32            stCond;
}WIN_RW_LOCK_S;
typedef WIN_RW_LOCK_S VOS_RW_LOCK_S;
#endif


/*初始化线程共享读写锁*/
#define VOS_RWLOCK_INIT(_rwlock)    VOS_ThreadMutex_Init(&(_rwlock))

/*释放共享锁*/
#define VOS_RWLOCK_FREE(_rwlock)    VOS_ThreadMutex_Destroy(&(_rwlock))

/*加锁*/
#define VOS_RWLOCK_LOCK(_rwLock)    VOS_ThreadMutex_Lock(&(_rwLock))

/*解锁*/
#define VOS_RWLOCK_UNLOCK(_rwLock)    VOS_ThreadMutex_UnLock(&(_rwLock))

/*指针类型的方式加锁*/
#define VOS_RWLOCK_PLOCK(_prwLock)    VOS_ThreadMutex_Lock(_prwLock)

/*指针类型的方式解锁*/
#define VOS_RWLOCK_UNPLOCK(_prwLock)    VOS_ThreadMutex_UnLock(_prwLock)


INT32 VOS_ThreadMutex_Init(VOS_RW_LOCK_S *pstMutex);

INT32 VOS_ThreadMutex_Lock(VOS_RW_LOCK_S *pstMutex);

INT32 VOS_ThreadMutex_UnLock(VOS_RW_LOCK_S *pstMutex);

INT32 VOS_ThreadMutex_Destroy(VOS_RW_LOCK_S *pstMutex);


INT32 VOS_SM_Init(VOS_SM_T *pstSem);
    
INT32 VOS_SM_P(VOS_SM_T *pstSem, UINT32 ulTimeOutMiliSec);

INT32 VOS_SM_V(VOS_SM_T *pstSem);

INT32 VOS_SM_Destroy(VOS_SM_T *pstSem);


LONG VOS_InterlockedIncrement(volatile LONG *plNums);

LONG VOS_InterlockedDecrement(volatile LONG *plNums);

LONG VOS_InterlockedExchange(volatile LONG *plNums, LONG Value);

#endif

