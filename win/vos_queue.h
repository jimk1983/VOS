/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : vos_queue.h
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年10月17日
  最近修改   :
  功能描述   : 根据当前的采集终端的业务设计的线程队列
  函数列表   :
  修改历史   :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/
#ifndef _VOS_QUEUE_H_
#define _VOS_QUEUE_H_


/*队列的规格，设计上考虑实际的终端的内存大小来设置即可*/
#define		VOS_QUE_MAXSIZE			100000

/*未知错误*/
#define     VOS_QUEERR_UNKNOW   -1
/*正常的停止*/
#define     VOS_QUEERR_CMDSTOP  -2



typedef struct tagVosMsgQueue VOS_MSGQUE_S, *PVOS_MSGQUE_S;

typedef INT32(*pfVosQueUserHandlerCb)(VOID *pvMsg, INT32 iMsgLen, VOID *pvUserCtx);

/*用户消息*/
typedef struct tagVosMsgUserCtx
{    
    VOID*                   pvUserCtx;                 /*用户队列中的的上下文信息*/
    pfVosQueUserHandlerCb   pfUserHandler;             /*用户的处理*/
}VOS_MSGUSR_CTX_S, *PVOS_MSGUSR_CTX_S;


/*内存传输方式的业务队列模式*/
struct tagVosMsgQueue
{
	CHAR	acQueGuid[VOS_GUID_LEN];		/*该队列的唯一ID*/
	INT32	iSize;							/*该队列的大小*/
	

	VOS_DLIST_NODE_S		stIdleList;		/*空闲链表*/
    INT32	                iIdleNums;		/*队列空闲节点*/
	VOS_DLIST_NODE_S		stUsedList;		/*使用链表*/
    INT32	                iUsedNums;		/*队列有效节点*/
	VOS_RW_LOCK_S			stLock;			/*队列锁*/
	VOS_EVENT_T				hEventNotify;	/*事件通知*/
    VOS_EVENT_T				hStopWait;	    /*事件通知*/

    VOS_MSGUSR_CTX_S        stUserCtx;      /*用户使用的相关信息保存*/
};


/*****************************************************************************
 函 数 名  : VOS_MsgQue_Create
 功能描述  : 队列的创建
 输入参数  : INT32 iSize                                --队列的大小，最大为VOS_QUE_MAXSIZE
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
PVOS_MSGQUE_S VOS_API_MsgQueCreate(INT32 iSize, pfVosQueUserHandlerCb pfUserHandleCb, VOID *pvUserCtx);



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
VOID VOS_API_MsgQueRelease(PVOS_MSGQUE_S *ppstQueue);


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
INT32 VOS_API_MsgQuePush(VOS_MSGQUE_S *pstMsgQue, VOID *pvMsg, INT32 iMsgLen);

/*****************************************************************************
 函 数 名  : VOS_API_MsgQuePush
 功能描述  : 消息队列数据从队列头插入
 输入参数  : VOS_MSGQUE_S *pvMsgQue     ---队列
             VOID *pvMsg            ---用户推送的内容
             INT32 iMsgLen          ---用户推送的长度
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2019年4月22日
    作    者   : 闫朝东
    修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_API_MsgQuePushHead(VOS_MSGQUE_S *pstMsgQue, VOID *pvMsg, INT32 iMsgLen);
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
INT32 VOS_API_MsgQuePop(VOS_MSGQUE_S *pstMsgQue, VOID **ppvMsg, INT32 *piMsgLen);


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
INT32 VOS_API_MsgQueWorkerWaitfor(PVOS_MSGQUE_S pstMsgQue, INT32 *piErrorCode);

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
INT32 VOS_API_MsgQueStopNotify(PVOS_MSGQUE_S pstMsgQue);

#endif

