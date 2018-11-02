/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �������޹�˾

 ******************************************************************************
  �� �� ��   : vos_mpools.h
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��4��29��
  ����޸�   :
  ��������   : ��̬�ڴ�ص����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��4��29��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/
#include <vos_pub.h>




/*****************************************************************************
 �� �� ��  : VOS_LookSideListInit
 ��������  : ��ʼ��һ���ڴ��,Ĭ�ϴ���100���ڴ�
 �������  : VOS_PLookSideList **ppstLookSideList  
             UINT32 uiEntrySize                    
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��4��29��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VOS_LookSideListInit(VOS_PLookSideList **ppstLookSideList, UINT32 uiEntrySize)
{
    VOS_PLookSideList  *pstLooksidelist = NULL;
    VOS_DLIST_NODE_S   *pstEntry        = NULL;
    UINT32              uiIndex         = 0;
    
    if ( NULL == ppstLookSideList )
    {
        return VOS_ERR;
    }

    /*����VOS���ڴ���*/
    pstLooksidelist = (PVOS_PLookSideList)malloc(sizeof(VOS_PLookSideList));
    if ( NULL == pstLooksidelist )
    {
        VOS_Printf("vos lookside list malloc error!\n");
        return VOS_ERR;
    }
    memset(pstLooksidelist,0, sizeof(VOS_PLookSideList));
    
    VOS_RWLOCK_INIT(pstLooksidelist->stPoolListLock);
    VOS_DLIST_INIT(&pstLooksidelist->stPoolListFree);
    
    pstLooksidelist->uiEntrySize = uiEntrySize;

    /*��ʼ�½��ڴ��*/
    for(uiIndex=0; uiIndex<VOS_LOOKSIDE_NUMS; uiIndex++ )
    {
        /*ÿ��ͷ���ض���VOS_DLIST_NODE_S*/
        pstEntry = (VOS_DLIST_NODE_S *)malloc(sizeof(VOS_DLIST_NODE_S) + uiEntrySize);
        if ( NULL == pstEntry )
        {   
            VOS_Printf("vos lookside entry malloc error!\n");
            goto ErrHandle;
        }

        VOS_DLIST_INIT(pstEntry);
        VOS_DLIST_ADD_TAIL(&pstLooksidelist->stPoolListFree, pstEntry);
        pstLooksidelist->uiNums++;
    }

    *ppstLookSideList = pstLooksidelist;

    return VOS_OK;
    
ErrHandle:

    VOS_LookSideListUnInit(pstLooksidelist);
    return VOS_ERR;
}


/*****************************************************************************
 �� �� ��  : VOS_LookSideListUnInit
 ��������  : �ڴ�ص��ͷ�
 �������  : VOS_PLookSideList *pstLookSideList  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��4��29��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID VOS_LookSideListUnInit(VOS_PLookSideList *pstLookSideList)
{
    PVOS_DLIST_NODE_S   thisEntry, nextEntry, listHead;
    
    if ( NULL != pstLookSideList )
    {
        listHead = &pstLookSideList->stPoolListFree;
        for (   thisEntry = listHead->prev;
                thisEntry != listHead ;
                thisEntry = nextEntry)
        {
            nextEntry = thisEntry->prev;
            free(thisEntry);
            pstLookSideList->uiNums--;
        }

        free(pstLookSideList);
        pstLookSideList = NULL;
    }
    
    return;
}

/*****************************************************************************
 �� �� ��  : VOS_AllcateFromLookSideList
 ��������  : �����ڴ�
 �������  : VOS_PLookSideList *pstLookSideList  
             VOID *pvEntry                       
 �������  : ��
 �� �� ֵ  : VOID
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��4��29��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID *VOS_AllcateFromLookSideList(VOS_PLookSideList *pstLookSideList)
{
    VOS_DLIST_NODE_S *pstEntry  = NULL;
    VOID             *pvMem     = NULL;

    VOS_RWLOCK_LOCK(pstLookSideList->stPoolListLock);
    if ( VOS_OK != VOS_Node_IsEmpty(&pstLookSideList->stPoolListFree) )
    {
        pstEntry =  pstLookSideList->stPoolListFree.prev;
        VOS_DLIST_DEL(pstEntry);
        VOS_RWLOCK_UNLOCK(pstLookSideList->stPoolListLock);
        
        pvMem = (CHAR *)pstEntry + sizeof(VOS_DLIST_NODE_S);
        return pvMem;
    }
    VOS_RWLOCK_UNLOCK(pstLookSideList->stPoolListLock);

    /*ÿ��ͷ���ض���VOS_DLIST_NODE_S*/
    pstEntry = (VOS_DLIST_NODE_S *)malloc(sizeof(VOS_DLIST_NODE_S) + pstLookSideList->uiEntrySize);
    if ( NULL == pstEntry )
    {
        return NULL;
    }

    VOS_DLIST_INIT(pstEntry);
    pvMem = (CHAR *)pstEntry + sizeof(VOS_DLIST_NODE_S);
    pstLookSideList->uiNums++;
    
    
    return pvMem;
}



/*****************************************************************************
 �� �� ��  : VOS_FreeToLookSideList
 ��������  : �ͷ��ڴ�
 �������  : VOS_PLookSideList *pstLookSideList  
             VOID *pvEntry                       
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��4��29��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID  VOS_FreeToLookSideList(VOS_PLookSideList *pstLookSideList, VOID *pvEntry)
{
    VOS_DLIST_NODE_S *pstEntry  = NULL;

    if ( NULL != pvEntry )
    {
          pstEntry = (VOS_DLIST_NODE_S *)((CHAR *)pvEntry - sizeof(VOS_DLIST_NODE_S));
          VOS_DLIST_INIT(pstEntry);
          VOS_RWLOCK_LOCK(pstLookSideList->stPoolListLock);
          VOS_DLIST_ADD_TAIL(&pstLookSideList->stPoolListFree, pstEntry);
          VOS_RWLOCK_UNLOCK(pstLookSideList->stPoolListLock);
    }
    
    return;
}




