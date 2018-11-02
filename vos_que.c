/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_que.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年5月22日
  最近修改   :
  功能描述   : 实现队列的几种方法
  函数列表   :
  修改历史   :
  1.日    期   : 2016年5月22日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
#include <vos_pub.h>

/*主要用于Rct架构中的消息，因此有个索引号*/
VOS_MSG_QUE_S *VOS_Rct_MsgQueCreate(UINT32 uiIndex)
{
    VOS_MSG_QUE_S *pstMsgQue = NULL;
    /*因为是初始化时候创建，因此直接用系统的*/
    pstMsgQue = (VOS_MSG_QUE_S *)malloc(sizeof(VOS_MSG_QUE_S));
    if ( NULL == pstMsgQue )
    {
        VOS_Printf("system error: Rct msg queue malloc error!");
        return pstMsgQue;
    }

    memset(pstMsgQue,0, sizeof(VOS_MSG_QUE_S));

    VOS_DLIST_INIT(&pstMsgQue->stList);

    VOS_RWLOCK_INIT(pstMsgQue->stLock);

    pstMsgQue->uiNum      =  0;
    pstMsgQue->uiIndex    =  uiIndex;

    return pstMsgQue;
}



/*****************************************************************************
 函 数 名  : VOS_Rct_MSGQue_Release
 功能描述  : 消息队列的释放
 输入参数  : VOS_MSG_QUE_S *pstMsgQue  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 

*****************************************************************************/
VOID VOS_Rct_MSGQue_Release(VOS_MSG_QUE_S *pstMsgQue)
{
    VOS_MSG_QUE_NODE_S *pstMsgNode = NULL;
    VOS_MSG_QUE_NODE_S *pstMsgTmp = NULL;

    if ( NULL != pstMsgQue )
    {
        /*如果有多余的节点，则遍历删除，避免内存泄漏*/
        if ( 0 < pstMsgQue->uiNum )
        {
            VOS_DLIST_FOR_EACH_ENTRY(pstMsgNode, &pstMsgQue->stList, VOS_MSG_QUE_NODE_S, stNode)
            {
                VOS_ASSERT(NULL != pstMsgNode);
                pstMsgTmp = VOS_DLIST_ENTRY(&pstMsgNode->stNode.prev, VOS_MSG_QUE_NODE_S, stNode);
                
                /*上面已经保存了，因此可以直接脱链*/
                VOS_Node_Remove((&pstMsgNode->stNode));
                /*消息节点使用VOS_Malloc()申请的，便于内存监听*/
                VOS_Free((CHAR *)pstMsgNode);
                
                pstMsgNode = pstMsgTmp;
            }
        }   

        VOS_RWLOCK_FREE(pstMsgQue->stLock);
        free(pstMsgQue);  
    }
}

/*****************************************************************************
 函 数 名  : VOS_Rct_MSGQueNode_Create
 功能描述  : 消息队列节点的创建
 输入参数  : 无
 输出参数  : 无
 返 回 值  : VOS_MSG_QUE_NODE_S
 调用函数  : 
 被调函数  : 
 *****************************************************************************/
VOS_MSG_QUE_NODE_S *VOS_Rct_MSGQueNode_Create()
{
    VOS_MSG_QUE_NODE_S *pstQueNode = NULL;

    pstQueNode = (VOS_MSG_QUE_NODE_S *)VOS_Malloc(VOS_MID, sizeof(VOS_MSG_QUE_NODE_S));
    if ( NULL == pstQueNode )
    {
        return NULL;
    }
    
    VOS_DLIST_INIT(&pstQueNode->stNode);
    
    VOS_Mem_Zero((CHAR *)pstQueNode->acMsgData, VOS_MSGQUE_DATA_LEN);
    
    return pstQueNode;
}


/*****************************************************************************
 函 数 名  : VOS_Rct_MSGQueNode_Release
 功能描述  : 消息队列节点的释放
 输入参数  : VOS_MSG_QUE_NODE_S *pstQueNode  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
*****************************************************************************/
VOID VOS_Rct_MSGQueNode_Release(VOS_MSG_QUE_NODE_S *pstQueNode)
{
    if ( NULL != pstQueNode )
    {
        VOS_Free((CHAR *)pstQueNode);
    }
}

/*****************************************************************************
 函 数 名  : VOS_Rct_MSGQue_IsEmpty
 功能描述  : 判断该消息队列是否为空
 输入参数  : VOS_MSG_QUE_S *pstMsgQue  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
*****************************************************************************/
BOOL VOS_Rct_MSGQue_IsEmpty(VOS_MSG_QUE_S *pstMsgQue)
{
    BOOL bRet = VOS_FALSE;

    if ( NULL == pstMsgQue )
    {
        VOS_Printf("param err!");
        return bRet;
    }
    
    VOS_RWLOCK_LOCK(pstMsgQue->stLock);
    
    if (0 == pstMsgQue->uiNum
        || VOS_OK == VOS_Node_IsEmpty(&pstMsgQue->stList))
    {
        /*不可能发生的，检查*/
        if(VOS_OK != VOS_Node_IsEmpty(&pstMsgQue->stList))
        {
            VOS_ASSERT(0);
            VOS_Printf("2016-10 [system error]: rct msg queue empty check error!!");
            
            VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);
            return bRet;
        }

        bRet = VOS_TRUE;
        
        VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);
        return bRet;
    }    
    VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);
    return bRet;
}


/*****************************************************************************
 函 数 名  : VOS_Rct_MSGQue_Push
 功能描述  : 将一个消息添加，先入先出，加入到队列尾部
 输入参数  : VOS_MSG_QUE_S *pstMsgQue        
             VOS_MSG_QUE_NODE_S *pstMsgNode  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
*****************************************************************************/
INT32 VOS_Rct_MSGQue_Push(VOS_MSG_QUE_S *pstMsgQue, VOS_MSG_QUE_NODE_S *pstMsgNode)
{
    
    if ( NULL == pstMsgQue
        || NULL == pstMsgNode )
    {
        VOS_Printf("VOS_Rct_MSGQue_Push, param error!pstMsgQue=%p, pstMsgNode=%p", pstMsgQue, pstMsgNode);
        return VOS_ERR;
    }

    /*加入队列*/
    VOS_RWLOCK_LOCK(pstMsgQue->stLock);
    VOS_DLIST_ADD_TAIL(&pstMsgQue->stList, &pstMsgNode->stNode);
    pstMsgQue->uiNum++;
    VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);

    return VOS_OK;
}


/*****************************************************************************
 函 数 名  : VOS_Rct_MSGQue_Pop
 功能描述  : 从队列中获取消息节点，先入先出，队列头取出
 输入参数  : VOS_MSG_QUE_S *pstMsgQue  
 输出参数  : 无
 返 回 值  : VOS_MSG_QUE_NODE_S
 调用函数  : 
 被调函数  : 
*****************************************************************************/
VOS_MSG_QUE_NODE_S * VOS_Rct_MSGQue_Pop(VOS_MSG_QUE_S *pstMsgQue)
{
    VOS_DLIST_NODE_S        *pstNode       = NULL;
    VOS_MSG_QUE_NODE_S   *pstMsgNode = NULL;
    if ( NULL == pstMsgQue )
    {
        VOS_Printf("VOS_Rct_MSGQue_Pop, param error!");
        return NULL;
    }

    VOS_RWLOCK_LOCK(pstMsgQue->stLock);

    /*空队列*/
    if ( 0 == pstMsgQue->uiNum )
    {
        /*不可能发生的，检查*/
        if(VOS_OK != VOS_Node_IsEmpty(&pstMsgQue->stList))
        {
            VOS_ASSERT(0);
            VOS_Printf("system error: rct msg queue POP check empty error!");
        }
        VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);
        return NULL;
    }
    /*取出队列头节点*/
    if(VOS_ERR == VOS_Node_HeadGet(&pstMsgQue->stList, &pstNode))
    {
        VOS_Printf("system error: rct msg queue get node error!");
    }
    
    pstMsgQue->uiNum--;
    
    VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);

    pstMsgNode = (VOS_MSG_QUE_NODE_S   *)pstNode;

    return pstMsgNode;
}





