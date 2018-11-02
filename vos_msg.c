/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_ipcmsg.c
  版 本 号   : 初稿
  作    者   : jiangkang
  生成日期   : 2015年9月25日
  最近修改   :
  功能描述   : 系统适配线程间消息
  函数列表   :
              VOS_GetPhreadSelfId
  修改历史   :
  1.日    期   : 2015年9月25日
    作    者   : jiangkang
    修改内容   : 创建文件

******************************************************************************/
#include <vos_pub.h>

/*获取自己当前的线程ID*/
INT32 VOS_GetSelfPhreadID()
{
  INT32 ulpthreadId;

#if VOS_PLAT_LINUX
  ulpthreadId = pthread_self();
#endif
  
  return ulpthreadId;
}

/*创建一个消息队列，通过KeyID来获取该消息,返回队列的ID*/
INT32 VOS_MSGCreateByKeyID(INT32 ulMsgKeyId)
{
    INT32 iMsgQueId = VOS_ERR;
#if VOS_PLAT_LINUX

    iMsgQueId = msgget((key_t)ulMsgKeyId, 0666 | IPC_CREAT);
    if ( iMsgQueId == -1 )
    {
        return VOS_ERR;
    }
#endif
    return iMsgQueId;
}

/*释放一个消息队列，通过KeyID来确认*/
VOID VOS_MSGReleaseByKeyID(INT32 ulMsgKeyId)
{
    INT32 iRet = VOS_ERR;
    INT32 iMsgQueId = VOS_ERR;
#if VOS_PLAT_LINUX

    iMsgQueId = msgget((key_t)ulMsgKeyId, 0666 | IPC_CREAT);
    if ( iMsgQueId == -1 )
    {
        return;
    }

    iRet=msgctl(iMsgQueId,IPC_RMID,0);
    if ( iRet == -1 )
    {
        return;
    }
#endif

    return;
}

/*判断消息是否存在*/
INT32 VOS_MSGKeyIDIsExist(INT32 ulMsgKeyId)
{
    INT32 iMsgQueId = VOS_ERR;
#if VOS_PLAT_LINUX

    iMsgQueId = msgget((key_t)ulMsgKeyId, 0666 | IPC_CREAT | IPC_EXCL);
    /*It`s found the queue*/
    if ( iMsgQueId == -1 )
    {
        return VOS_OK;
    }
    
    /*if create it , then del it
    iRet=msgctl(iMsgQueId,IPC_RMID,0);
    if ( iRet == -1 )
    {
        return VOS_ERR;
    }
    */
#endif

    return VOS_ERR;
}

/*线程间同步消息发送，由于同步，处理不能阻塞
  * iMsgPtr--用于发送线程间的地址
  * 四个地址，可以
*/
INT32 VOS_MSGSend(INT32 ulMsgQueId, INTPTR iMsgPtr[4])
{
    VOS_MSG_S stQueMsg= {0};
    INT32 iRet = VOS_ERR;
#if VOS_PLAT_LINUX

    stQueMsg.lMsgType= 1;
    /*用于传输指针*/
    stQueMsg.iMsgPtr[0] = (INTPTR)iMsgPtr[0];
        
    iRet = msgsnd(ulMsgQueId, (VOID *)&stQueMsg, 4 * sizeof(INTPTR), 0);
    if ( iRet == -1 )
    {
        return VOS_ERR;
    }
#endif

    return VOS_OK;
}

/*接收线程间的同步发送的消息，同一个进程，虚拟地址是相同的
  因此可以传输地址*/
INT32 VOS_MSGRecv(INT32 ulMsgQueId, INTPTR *iMsgPtr)
{
    VOS_MSG_S stQueMsg= {0};
    INT32 iRet = VOS_ERR;
    INT32 lMsgtype = 0;    
#if VOS_PLAT_LINUX
    iRet = msgrcv(ulMsgQueId, (VOID *)&stQueMsg, 4 * sizeof(INTPTR), lMsgtype, 0);
    if ( iRet == -1 )
    {
        return VOS_ERR;
    }
    
    iMsgPtr[0] = stQueMsg.iMsgPtr[0];
    iMsgPtr[1] = stQueMsg.iMsgPtr[1];
    iMsgPtr[2] = stQueMsg.iMsgPtr[2];
    iMsgPtr[3] = stQueMsg.iMsgPtr[3];
#endif    
    return VOS_OK;
}

//进程间的拷贝暂未用到，先不支持
#if 0
/*进程间消息,消息需要拷贝*/
INT32 VOS_MsgQueSend(INT32 ulMsgQueId, CHAR *pcSndBuf, INT32 ulSndSize)
{
    LONG iRet = VOS_ERR;
    VOS_MSG_QUEUE_S *pstMsgData = {0};
    VOS_MSG_S  stMsg= {0};

    if ( ulSndSize > VOS_QUEUE_BUF_SIZE
        || NULL == pcSndBuf )
    {
        return VOS_ERR;
    }
    
    /*lMsgType等于0 则返回队列的最早的一个消息。
       lMsgType大于0，则返回其类型为mtype的第一个消息。
       lMsgType小于0,则返回其类型小于或等于mtype参数的绝对值的最小的一个消息。
       */
    pstMsgData->lMsgType= 1;
    //pstMsgData->stMsgTxt.ulDataLen = ulSndSize;
    //pstMsgData->stMsgTxt.pcPthreadData = pcSndBuf;
    /*进程间必须使用拷贝,因为地址空间不同*/
    //VOS_Mem_Copy(stMsgData.stMsgTxt.acQueBuf, pcSndBuf,ulSndSize);

    /*由于本程序使用的是线程，空间共享，所以使用赋值
    拷贝比较浪费资源*/
    stMsg.lMsgType = 1;
    VOS_Mem_Copy((CHAR *)stMsg.acMsgBuf, (CHAR *)pstMsgData, sizeof(VOS_MSG_QUEUE_S));
    
    iRet = msgsnd(ulMsgQueId, (VOID *)&stMsg, VOS_MSG_BUF_SIZE, 0);
    if ( iRet == -1 )
    {
        VOS_Printf("msgsnd error!");
        return VOS_ERR;
    }
    else
    {
        VOS_Printf("msgsnd ok!iRet=%d",  iRet);
    }

    return VOS_OK;
}


LONG VOS_MsgQueRecv(ULONG ulMsgQueId, CHAR *pcRcvBuf, ULONG *pulRcvSize)
{
    LONG iRet = VOS_ERR;
    VOS_MSG_QUEUE_S *pstMsgData = {0};
    VOS_MSG_S  stMsg= {0};
    LONG                   lMsgtype = 0;
    
    if ( NULL == pulRcvSize )
    {
        return VOS_ERR;
    }

    iRet = msgrcv(ulMsgQueId, (VOID *)&stMsg, VOS_MSG_BUF_SIZE, lMsgtype, 0);
    if ( iRet == -1 )
    {
        VOS_Printf("msgrcv error!");
        return VOS_ERR;
    }

    pstMsgData = (VOS_MSG_QUEUE_S *)stMsg.acMsgBuf;
    /*进程间必须使用拷贝,因为地址空间不同*/
    //VOS_Mem_Copy(pcSndBuf, stMsgData.stMsgTxt.acQueBuf, stMsgData.stMsgTxt.ulDataLen);

    /*由于本程序使用的是线程，空间共享，所以使用赋值
    拷贝比较浪费资源*/
    pcRcvBuf = pstMsgData->stMsgTxt.pcPthreadData;
    (*pulRcvSize) = pstMsgData->stMsgTxt.ulDataLen;
        
    return VOS_OK;
}


LONG VOS_MsgQueSendCpy(ULONG ulMsgQueId, CHAR *pcSndBuf)
{
    LONG iRet = VOS_ERR;
    VOS_MSG_S stMsgData = {0};

    if ( NULL == pcSndBuf )
    {
        return VOS_ERR;
    }
    /*lMsgType等于0 则返回队列的最早的一个消息。
       lMsgType大于0，则返回其类型为mtype的第一个消息。
       lMsgType小于0,则返回其类型小于或等于mtype参数的绝对值的最小的一个消息。
       */
    stMsgData.lMsgType= 1;
    /*进程间必须使用拷贝,因为地址空间不同*/
    VOS_Mem_Copy(stMsgData.acMsgBuf, pcSndBuf, VOS_MSG_BUF_SIZE);
    
    iRet = msgsnd(ulMsgQueId, (VOID *)&stMsgData, sizeof(stMsgData), 0);
    if ( iRet == -1 )
    {
        VOS_Printf("msgsnd error!");
        return VOS_ERR;
    }

    return VOS_OK;
}


LONG VOS_MsgQueRecvCpy(ULONG ulMsgQueId, CHAR *pcRcvBuf)
{
    LONG iRet = VOS_ERR;
    VOS_MSG_S  stMsg= {0};
    LONG           lMsgtype = 0;
    if ( NULL == pcRcvBuf )
    {
        return VOS_ERR;
    }

    iRet = msgrcv(ulMsgQueId, (VOID *)&stMsg, VOS_MSG_BUF_SIZE, lMsgtype, 0);
    if ( iRet == -1 )
    {
        VOS_Printf("msgrcv error!");
        return VOS_ERR;
    }
    
    /*进程间必须使用拷贝,因为地址空间不同*/
    VOS_Mem_Copy(pcRcvBuf, stMsg.acMsgBuf, VOS_MSG_BUF_SIZE);
    
    return VOS_OK;
}
#endif

