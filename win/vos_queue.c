/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : vos_queue.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年10月17日
  最近修改   :
  功能描述   : 多线程的队列操作
  函数列表   :
  修改历史   :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
    
#include "Common/common.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

#define VOS_MSGQUE_TIMEOUT     300

#define VOS_INFINITE			0xFFFFFFFF

/*直接信息类型*/
typedef enum
{
    VOS_QUEMSG_TYPE_UNKNOW=0,   /*未知类型*/
    VOS_QUEMSG_TYPE_CTRL,       /*控制类型*/
    VOS_QUEMSG_TYPE_DATA,       /*数据类型*/
    VOS_QUEMSG_TYPE_CMD_STOP,   /*队列停止命令*/
    VOS_QUEMSG_TYPE_USER,       /*用户自定义类型*/

    VOS_QUEMSG_TYPE_NUMS,
}VOS_QUEMSG_TYPE_E;


/*内部的消息节点*/
typedef struct tagVosMsgQueNode
{
    VOS_DLIST_NODE_S    stNode;
	UINT32              uiMsgCode;		/*内部控制的消息码*/
	UINT32              uiMsgLength;    /*数据传输*/
	VOID*               pvUserMsg;		/*用户数据保存, 当前采用指针形式*/
}VOS_MSGQUE_NODE_S, *PVOS_MSGQUE_NODE_S;

/*****************************************************************************
 函 数 名  : VOS_API_MsgQuePop
 功能描述  : 数据信息从数据链表中取出
 输入参数  : VOS_MSGQUE_S *pvMsgQue     --队列
			 VOID **ppvMsg          --用户的数据
			 INT32 *piMsgLen        --数据的长度
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年10月17日
	作    者   : 蒋康
	修改内容   : 新生成函数
*****************************************************************************/
INT32 VOS_API_MsgQuePop(VOS_MSGQUE_S *pstMsgQue, VOID **ppvMsg, INT32 *piMsgLen)
{
    PVOS_DLIST_NODE_S   pstEntry, pNextEntry, plistHead= NULL;
    PVOS_MSGQUE_NODE_S  pstMsgNode  = NULL;
    
	if (NULL == pstMsgQue
		|| NULL == ppvMsg
		|| NULL == piMsgLen)
	{
		return VOS_ERR;
	}

    VOS_RWLOCK_LOCK(pstMsgQue->stLock);

    if ( VOS_TRUE == VOS_List_IsEmpty(&pstMsgQue->stUsedList) )
    {
        *ppvMsg = NULL;
        VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);
        return VOS_OK;
    }
    
    plistHead = &pstMsgQue->stUsedList;
    for ( pstEntry = plistHead->prev;
          pstEntry != plistHead;
          pstEntry = pNextEntry)
    {
		pNextEntry = pstEntry->prev;
        pstMsgNode = VOS_DLIST_ENTRY(pstEntry, VOS_MSGQUE_NODE_S, stNode);
        if ( pstMsgNode->uiMsgCode != VOS_QUEMSG_TYPE_DATA )
        {
            continue;
        }
        
        VOS_DLIST_DEL(&pstMsgNode->stNode);
		pstMsgQue->iUsedNums--;
        break;
    }
          
    *ppvMsg= pstMsgNode->pvUserMsg;
    *piMsgLen = pstMsgNode->uiMsgLength;
    pstMsgNode->pvUserMsg = NULL;
    pstMsgNode->uiMsgCode = 0;
    pstMsgNode->uiMsgLength = 0;
    VOS_DLIST_ADD_TAIL(&pstMsgQue->stIdleList, &pstMsgNode->stNode);
          
    VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);
    
	return VOS_OK;
}


/*****************************************************************************
 函 数 名  : VOS_API_MsgQuePush
 功能描述  : 消息队列数据推送
 输入参数  : VOS_MSGQUE_S *pvMsgQue     ---队列
             VOID *pvMsg            ---用户推送的内容
             INT32 iMsgLen          ---用户推送的长度
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_API_MsgQuePush(VOS_MSGQUE_S *pstMsgQue, VOID *pvMsg, INT32 iMsgLen)
{
	PVOS_MSGQUE_NODE_S   pstMsgNode = NULL;
    PVOS_DLIST_NODE_S    pstEntry = NULL;

	if ( NULL == pstMsgQue
		|| NULL == pvMsg
		|| 0 >= iMsgLen )
	{
		return VOS_ERR;
	}

	VOS_RWLOCK_LOCK(pstMsgQue->stLock);
    if ( VOS_TRUE ==  VOS_List_IsEmpty(&pstMsgQue->stIdleList) )
    {
        //printf("VOS message idle list is Empty!idleNums=%d\n", pstMsgQue->iIdleNums);
	    VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);
        return VOS_ERR;
    }

    VOS_Node_HeadGet(&pstMsgQue->stIdleList, &pstEntry);
    if ( NULL == pstEntry )
    {
        printf("VOS message get idle node error!idleNums=%d\n", pstMsgQue->iIdleNums);
        VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);
        return VOS_ERR;
    }
	pstMsgQue->iIdleNums--;
	pstMsgNode = VOS_DLIST_ENTRY(pstEntry, VOS_MSGQUE_NODE_S, stNode);
	/*用户数据，暂时定义都是数据类型消息*/
	pstMsgNode->uiMsgCode = VOS_QUEMSG_TYPE_DATA;
	pstMsgNode->uiMsgLength = iMsgLen;
	pstMsgNode->pvUserMsg = pvMsg;

	VOS_DLIST_ADD_TAIL(&pstMsgQue->stUsedList, &pstMsgNode->stNode);
	pstMsgQue->iUsedNums++;
	VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);
    
    /*目前保证每个包实时通知*/
    VOS_EventNotify(pstMsgQue->hEventNotify);
    
    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : VOS_API_MsgQuePush
 功能描述  : 消息队列数据在头部插入
 输入参数  : VOS_MSGQUE_S *pvMsgQue     ---队列
             VOID *pvMsg            ---用户推送的内容
             INT32 iMsgLen          ---用户推送的长度
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_API_MsgQuePushHead(VOS_MSGQUE_S *pstMsgQue, VOID *pvMsg, INT32 iMsgLen)
{
    PVOS_MSGQUE_NODE_S   pstMsgNode = NULL;
    PVOS_DLIST_NODE_S    pstEntry = NULL;

    if (NULL == pstMsgQue
        || NULL == pvMsg
        || 0 >= iMsgLen)
    {
        return VOS_ERR;
    }

    VOS_RWLOCK_LOCK(pstMsgQue->stLock);
    if (VOS_TRUE == VOS_List_IsEmpty(&pstMsgQue->stIdleList))
    {
        //printf("VOS message idle list is Empty!idleNums=%d\n", pstMsgQue->iIdleNums);
        VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);
        return VOS_ERR;
    }

    VOS_Node_HeadGet(&pstMsgQue->stIdleList, &pstEntry);
    if (NULL == pstEntry)
    {
        printf("VOS message get idle node error!idleNums=%d\n", pstMsgQue->iIdleNums);
        VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);
        return VOS_ERR;
    }
    pstMsgQue->iIdleNums--;
    pstMsgNode = VOS_DLIST_ENTRY(pstEntry, VOS_MSGQUE_NODE_S, stNode);
    /*用户数据，暂时定义都是数据类型消息*/
    pstMsgNode->uiMsgCode = VOS_QUEMSG_TYPE_DATA;
    pstMsgNode->uiMsgLength = iMsgLen;
    pstMsgNode->pvUserMsg = pvMsg;

    VOS_DLIST_ADD_HEAD(&pstMsgQue->stUsedList, &pstMsgNode->stNode);
    pstMsgQue->iUsedNums++;
    VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);

    /*目前保证每个包实时通知*/
    VOS_EventNotify(pstMsgQue->hEventNotify);

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : VOS_API_MsgQueUserRegister
 功能描述  : 用户信息的注册
 输入参数  : PVOS_MSGQUE_S pstQueue                     --队列
           VOID *pvUserCtx                          --用户上下文
           pfVosQueUserHandlerCb pfUserHandleCb     --用户注册的消息处理函数
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月18日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_API_MsgQueUserRegister(PVOS_MSGQUE_S pstQueue, VOID *pvUserCtx, pfVosQueUserHandlerCb pfUserHandleCb)
{
    if ( NULL == pstQueue
        || NULL == pfUserHandleCb )
    {
        return VOS_ERR;
    }
    
    pstQueue->stUserCtx.pvUserCtx = pvUserCtx;
    pstQueue->stUserCtx.pfUserHandler = pfUserHandleCb;
    
    return VOS_OK;
}


/*****************************************************************************
 函 数 名  : VOS_MsgQue_Create
 功能描述  : 队列的创建
 输入参数  : INT32 iSize                                --队列的大小
           pfVosQueUserHandlerCb pfUserHandleCb     --队列的用户处理函数
           VOID *pvUserCtx                          --用户的上下文
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月18日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
PVOS_MSGQUE_S VOS_API_MsgQueCreate(INT32 iSize, pfVosQueUserHandlerCb pfUserHandleCb, VOID *pvUserCtx)
{
	PVOS_MSGQUE_S       pstQueue = NULL;
    PVOS_MSGQUE_NODE_S  pstEntry = NULL;
	int                 uiIndex  = 0; 
    
	if ( iSize > VOS_QUE_MAXSIZE-1 
        || NULL == pfUserHandleCb )
	{
		return NULL;
	}

	pstQueue = (PVOS_MSGQUE_S)malloc(sizeof(VOS_MSGQUE_S));
	if (NULL == pstQueue)
	{
		return pstQueue;
	}
	
	memset(pstQueue,0,sizeof(VOS_MSGQUE_S));
    
    VOS_DLIST_INIT(&pstQueue->stIdleList);
	pstQueue->iIdleNums = 0;
    VOS_DLIST_INIT(&pstQueue->stUsedList);
	pstQueue->iUsedNums = 0;
    VOS_RWLOCK_INIT(pstQueue->stLock);

    if ( VOS_ERR == VOS_UTILS_GenUUid(pstQueue->acQueGuid) )
    {
        printf("VOS Message queue create uuid error\n");
        return NULL;
    }    

    /*采用事件通知，频率不高*/
    pstQueue->hEventNotify = VOS_EventCreate(NULL, FALSE);
    if (VOS_INVALID_HANDLE == pstQueue->hEventNotify )
    {
        free(pstQueue);
        return NULL;
    }

    pstQueue->hStopWait = VOS_EventCreate(NULL, FALSE);
    if (VOS_INVALID_HANDLE == pstQueue->hStopWait)
    {
        free(pstQueue);
        return NULL;
    }
    
    pstQueue->stUserCtx.pvUserCtx       = pvUserCtx;
    pstQueue->stUserCtx.pfUserHandler   = pfUserHandleCb;

    /*开始新建消息节点*/
    for(uiIndex=0; uiIndex < iSize; uiIndex++ )
    {
        pstEntry = (PVOS_MSGQUE_NODE_S)malloc(sizeof(VOS_MSGQUE_NODE_S));
        if ( NULL == pstEntry )
        {   
            printf("VOS message queue create malloc node error!\n");
            VOS_API_MsgQueRelease(&pstQueue);
            return NULL;
        }

        VOS_DLIST_INIT(&pstEntry->stNode);
        VOS_DLIST_ADD_TAIL(&pstQueue->stIdleList, &pstEntry->stNode);
        pstQueue->iIdleNums++;
    }
    
	pstQueue->iSize = iSize;
    
	return pstQueue;
}



/*****************************************************************************
 函 数 名  : VOS_API_MsgQueRelease
 功能描述  : 队列的释放
 输入参数  : PVOS_MSGQUE_S pstQueue         --队列
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_API_MsgQueRelease(PVOS_MSGQUE_S *ppstQueue)
{
    PVOS_DLIST_NODE_S   pthisEntry, pNextEntry, plistHead;
    PVOS_MSGQUE_NODE_S  pstMsgNode  = NULL;
    PVOS_MSGQUE_S       pstQue      = NULL;
    INT32               iRet = 0;

    if ( NULL == ppstQueue )
    {
        return;
    }

    pstQue = *ppstQueue;

    VOS_API_MsgQueStopNotify(pstQue);
    
    iRet = VOS_EventWaiting(pstQue->hStopWait, 3000);
    if (VOS_ERR == iRet)
    {
        printf("VOS queue waiting for timeout!\n");
    }

    if ( NULL != pstQue )
    {   
        /* 空闲链表 */
        if ( VOS_TRUE != VOS_List_IsEmpty(&pstQue->stIdleList) )
        {
            plistHead = &pstQue->stIdleList;
            for ( pthisEntry = plistHead->prev;
                  pthisEntry != plistHead;
                  pthisEntry = pNextEntry)
            {
				pNextEntry = pthisEntry->prev;
                pstMsgNode = VOS_DLIST_ENTRY(pthisEntry, VOS_MSGQUE_NODE_S, stNode);
                VOS_DLIST_DEL(&pstMsgNode->stNode);
				free(pstMsgNode);
				pstQue->iIdleNums--;
                pthisEntry=plistHead->prev;
            }
        }

		/* 残留中的链表 */
		if (VOS_TRUE != VOS_List_IsEmpty(&pstQue->stUsedList))
		{
			plistHead = &pstQue->stUsedList;
			for (pthisEntry = plistHead->prev;
				pthisEntry != plistHead;
				pthisEntry = pNextEntry)
			{
				pNextEntry = pthisEntry->prev;
				pstMsgNode = VOS_DLIST_ENTRY(pthisEntry, VOS_MSGQUE_NODE_S, stNode);
				VOS_DLIST_DEL(&pstMsgNode->stNode);
				free(pstMsgNode);
				pstQue->iUsedNums--;
				pthisEntry = plistHead->prev;
			}
		}
        
        VOS_RWLOCK_FREE(pstQue->stLock);
        
        if ( VOS_INVALID_HANDLE != pstQue->hEventNotify  )
        {
            VOS_EventDelete(pstQue->hEventNotify);
            pstQue->hEventNotify = VOS_INVALID_HANDLE;
        }

        if (VOS_INVALID_HANDLE != pstQue->hStopWait)
        {
            VOS_EventDelete(pstQue->hStopWait);
            pstQue->hStopWait = VOS_INVALID_HANDLE;
        }
        
        free(pstQue);
        *ppstQueue = NULL;
    }
}


/*****************************************************************************
 函 数 名  : VOS_API_MsgQueWorkerWaitfor
 功能描述  : 消息队列启动消息接收
 输入参数  : PVOS_MSGQUE_S *pstQueue  --启动队列等待接收
 输出参数  : INT32 *piErrorCode         --队列结束的错误码
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月18日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_API_MsgQueWorkerWaitfor(PVOS_MSGQUE_S pstMsgQue, INT32 *piErrorCode)
{
    INT32               iRet        = 0;
    INT32               iStop       = 0;
    PVOS_DLIST_NODE_S   pstEntry    = NULL;
    PVOS_MSGQUE_NODE_S  pstMsgNode  = NULL;
    
    if ( NULL == pstMsgQue
		|| NULL == piErrorCode )
    {
        return VOS_ERR;
    }

    while(VOS_TRUE)
    {
        iRet = VOS_EventWaiting(pstMsgQue->hEventNotify, VOS_MSGQUE_TIMEOUT);
        if ( VOS_ERR == iRet )
        {
            printf("VOS Event waiting for error!\n");
            break;
        }

		/*避免下面复杂逻辑扩展后的形成死锁, 先加锁检查一次*/
		VOS_RWLOCK_LOCK(pstMsgQue->stLock);
        //printf("VOS_API_MsgQueWorkerWaitfor queue=[%s] iUsedNums=[%d], iIdleNums=[%d]\n",
        //    pstMsgQue->acQueGuid,pstMsgQue->iUsedNums, pstMsgQue->iIdleNums);
		if ( VOS_TRUE == VOS_List_IsEmpty(&pstMsgQue->stUsedList))
		{
			VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);
			continue;
		}
		VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);

		while (VOS_TRUE != VOS_List_IsEmpty(&pstMsgQue->stUsedList))
		{
			VOS_RWLOCK_LOCK(pstMsgQue->stLock);
			VOS_Node_HeadGet(&pstMsgQue->stUsedList, &pstEntry);
			pstMsgQue->iUsedNums--;
			VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);

			pstMsgNode = VOS_DLIST_ENTRY(pstEntry, VOS_MSGQUE_NODE_S, stNode);
			switch (pstMsgNode->uiMsgCode)
			{
			case VOS_QUEMSG_TYPE_CTRL:

				break;
			case VOS_QUEMSG_TYPE_DATA:
				if (NULL != pstMsgQue->stUserCtx.pfUserHandler)
				{
					(pstMsgQue->stUserCtx.pfUserHandler(pstMsgNode->pvUserMsg, pstMsgNode->uiMsgLength, pstMsgQue->stUserCtx.pvUserCtx));
				}
				break;
			case VOS_QUEMSG_TYPE_CMD_STOP:
				iStop = VOS_TRUE;
				break;
			default:
				break;
			}

			VOS_RWLOCK_LOCK(pstMsgQue->stLock);
			pstMsgNode->pvUserMsg = NULL;
			pstMsgNode->uiMsgCode = 0;
			pstMsgNode->uiMsgLength = 0;
			VOS_DLIST_ADD_TAIL(&pstMsgQue->stIdleList, &pstMsgNode->stNode);
			pstMsgQue->iIdleNums++;
			VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);

			if (iStop)
			{
				/*这种情况直接结束当前线程*/
				return VOS_OK;
			}
		}
    }

    VOS_EventNotify(pstMsgQue->hStopWait);

    return VOS_OK;
}


/*****************************************************************************
 函 数 名  : VOS_API_MsgQueStop
 功能描述  : 消息队列处理停止
 输入参数  : PVOS_MSGQUE_S *pstQueue  --停止的队列
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月18日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_API_MsgQueStopNotify(PVOS_MSGQUE_S pstMsgQue)
{
    PVOS_MSGQUE_NODE_S  pstMsgNode = NULL;
    PVOS_DLIST_NODE_S    pstEntry = NULL;
    
	if ( NULL == pstMsgQue )
	{
		return VOS_ERR;
	}

	VOS_RWLOCK_LOCK(pstMsgQue->stLock);
    if ( VOS_TRUE ==  VOS_List_IsEmpty(&pstMsgQue->stIdleList) )
    {
        //printf("VOS message idle list is Empty!idleNums=%d\n", pstMsgQue->iIdleNums);
	    VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);
        return VOS_ERR;
    }

    VOS_Node_HeadGet(&pstMsgQue->stIdleList, &pstEntry);
    if ( NULL == pstEntry )
    {
        printf("VOS message get idle node error!idleNums=%d\n", pstMsgQue->iIdleNums);
        VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);
        return VOS_ERR;
    }
    
    pstMsgNode = VOS_DLIST_ENTRY(pstEntry, VOS_MSGQUE_NODE_S, stNode);
    
    /*用户数据，暂时定义都是数据类型消息*/
    pstMsgNode->uiMsgCode   = VOS_QUEMSG_TYPE_CMD_STOP;
    
    VOS_DLIST_ADD_TAIL(&pstMsgQue->stUsedList, &pstMsgNode->stNode);
    pstMsgQue->iUsedNums++;
    
	VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);
    
    /*目前保证每个包实时通知*/
    VOS_EventNotify(pstMsgQue->hEventNotify);
    
    return VOS_OK;
}

