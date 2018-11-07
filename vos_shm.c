/******************************************************************************

                  版权所有 (C), 2018-2028, 技术有限公司

 ******************************************************************************
  文 件 名   : vos_shm.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年11月7日
  最近修改   :
  功能描述   : 共享内存的适配
  函数列表   :
  修改历史   :
  1.日    期   : 2018年11月7日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/

#include <vos_pub.h>
#include <vos_log.h>


/*****************************************************************************
 函 数 名  : VOS_SHMemCreate
 功能描述  : 创建共享内存，不提供权限
 输入参数  : UINT32 uiSize  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月7日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID *VOS_SHMemCreate(VOS_SHM_T *pstHandle, CHAR *pcKeyString, UINT32 uiSize)
{
    VOID*           pvMem = NULL;
    
    if ( NULL == pstHandle
        || NULL == pcKeyString 
        || uiSize > VOS_SHMEM_SIZE )
    {
        return NULL;
    }

#if VOS_PLAT_LINUX
    pthread_mutexattr_t attr;
     
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    
    pstHandle->key = VOS_Hash_String(pcKeyString);
    
    pstHandle->shmid = shmget(pstHandle->key, sizeof(LINUX_MEM_UNIT), IPC_CREAT | IPC_EXCL | 0666);
    if ( -1 == pstHandle->shmid )
    {
        return NULL;
    }

    pvMem = shmat(pstHandle->shmid, NULL, 0);
    if ( pvMem == (VOID *)-1 )
    {
        (VOID)shmctl(pstHandle->shmid, IPC_RMID, NULL);
        return NULL;
    }
    
    pstHandle->pstMem = (PLINUX_MEM_UNIT)pvMem;
    
    pthread_mutex_init(&pstHandle->pstMem->stProcMutex, &attr);
    VOS_Mem_Zero(pstHandle->pstMem->acMapMem, VOS_SHMEM_SIZE);
    
#elif VOS_PLAT_WIN
    
#endif

    return pstHandle->pstMem->acMapMem;
}

/*****************************************************************************
 函 数 名  : VOS_SHMemOpen
 功能描述  : 打开共享内存
 输入参数  : VOS_SHM_T *pstHandle  
             CHAR *pcKeyString     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月7日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID *VOS_SHMemOpen(VOS_SHM_T *pstHandle, CHAR *pcKeyString)
{
    VOID*           pvMem = NULL;
    
    if ( NULL == pstHandle 
        || NULL == pcKeyString )
    {
        return NULL;
    }
        
#if VOS_PLAT_LINUX

    pstHandle->key = VOS_Hash_String(pcKeyString);

    pstHandle->shmid = shmget(pstHandle->key, 0, 0);
    if ( -1 == pstHandle->shmid  )
    {
        return NULL;
    }
    
    pvMem = shmat(pstHandle->shmid, NULL, 0);
    if ( pvMem == (VOID *)-1 )
    {
        return NULL;
    }
    
    pstHandle->pstMem = (PLINUX_MEM_UNIT)pvMem;
    
#elif VOS_PLAT_WIN
    
#endif

    return pstHandle->pstMem->acMapMem;
}

/*****************************************************************************
 函 数 名  : VOS_SHMemLock
 功能描述  : 共享内存锁
 输入参数  : VOS_SHM_T *pstHandle  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月7日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_SHMemLock(VOS_SHM_T *pstHandle)
{
    if ( NULL == pstHandle 
        || NULL == pstHandle->pstMem )
    {
        return;
    }
#if VOS_PLAT_LINUX
    
    pthread_mutex_lock(&pstHandle->pstMem->stProcMutex);
    
#elif VOS_PLAT_WIN
    
#endif

}

/*****************************************************************************
 函 数 名  : VOS_SHMemUnLock
 功能描述  : 共享内存的解锁
 输入参数  : VOS_SHM_T *pstHandle  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月7日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_SHMemUnLock(VOS_SHM_T *pstHandle)
{
    if ( NULL == pstHandle 
        || NULL == pstHandle->pstMem )
    {
        return;
    }
#if VOS_PLAT_LINUX
    pthread_mutex_unlock(&pstHandle->pstMem->stProcMutex);
#elif VOS_PLAT_WIN
    
#endif


}


/*****************************************************************************
 函 数 名  : VOS_SHMemClose
 功能描述  : 关闭共享内存
 输入参数  : VOS_SHM_T *pstHandle  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月7日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_SHMemClose(VOS_SHM_T *pstHandle)
{
#if VOS_PLAT_LINUX
    if ( NULL != pstHandle )
    {
        (VOID)pthread_mutex_destroy(&pstHandle->pstMem->stProcMutex);
            
        (VOID)shmctl(pstHandle->shmid, IPC_RMID, NULL);
    }
#elif VOS_PLAT_WIN

#endif

}



