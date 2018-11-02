/******************************************************************************

                  版权所有 (C), 2018-2028, 网络有限公司

 ******************************************************************************
  文 件 名   : vos_mpools.h
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年4月29日
  最近修改   :
  功能描述   : 动态内存池的设计
  函数列表   :
  修改历史   :
  1.日    期   : 2018年4月29日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/
#include <vos_pub.h>




/*****************************************************************************
 函 数 名  : VOS_LookSideListInit
 功能描述  : 初始化一个内存池,默认创建100个内存
 输入参数  : VOS_PLookSideList **ppstLookSideList  
             UINT32 uiEntrySize                    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年4月29日
    作    者   : 蒋康
    修改内容   : 新生成函数

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

    /*不在VOS的内存中*/
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

    /*开始新建内存池*/
    for(uiIndex=0; uiIndex<VOS_LOOKSIDE_NUMS; uiIndex++ )
    {
        /*每个头部必定是VOS_DLIST_NODE_S*/
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
 函 数 名  : VOS_LookSideListUnInit
 功能描述  : 内存池的释放
 输入参数  : VOS_PLookSideList *pstLookSideList  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年4月29日
    作    者   : 蒋康
    修改内容   : 新生成函数

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
 函 数 名  : VOS_AllcateFromLookSideList
 功能描述  : 申请内存
 输入参数  : VOS_PLookSideList *pstLookSideList  
             VOID *pvEntry                       
 输出参数  : 无
 返 回 值  : VOID
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年4月29日
    作    者   : 蒋康
    修改内容   : 新生成函数

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

    /*每个头部必定是VOS_DLIST_NODE_S*/
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
 函 数 名  : VOS_FreeToLookSideList
 功能描述  : 释放内存
 输入参数  : VOS_PLookSideList *pstLookSideList  
             VOID *pvEntry                       
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年4月29日
    作    者   : 蒋康
    修改内容   : 新生成函数

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




