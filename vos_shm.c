/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �������޹�˾

 ******************************************************************************
  �� �� ��   : vos_shm.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��11��7��
  ����޸�   :
  ��������   : �����ڴ������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��11��7��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/

#include <vos_pub.h>
#include <vos_log.h>


/*****************************************************************************
 �� �� ��  : VOS_SHMemCreate
 ��������  : ���������ڴ棬���ṩȨ��
 �������  : UINT32 uiSize  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_SHMemOpen
 ��������  : �򿪹����ڴ�
 �������  : VOS_SHM_T *pstHandle  
             CHAR *pcKeyString     
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_SHMemLock
 ��������  : �����ڴ���
 �������  : VOS_SHM_T *pstHandle  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_SHMemUnLock
 ��������  : �����ڴ�Ľ���
 �������  : VOS_SHM_T *pstHandle  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_SHMemClose
 ��������  : �رչ����ڴ�
 �������  : VOS_SHM_T *pstHandle  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��7��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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



