/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_ipcmsg.c
  �� �� ��   : ����
  ��    ��   : jiangkang
  ��������   : 2015��9��25��
  ����޸�   :
  ��������   : ϵͳ�����̼߳���Ϣ
  �����б�   :
              VOS_GetPhreadSelfId
  �޸���ʷ   :
  1.��    ��   : 2015��9��25��
    ��    ��   : jiangkang
    �޸�����   : �����ļ�

******************************************************************************/
#include <vos_pub.h>

/*��ȡ�Լ���ǰ���߳�ID*/
INT32 VOS_GetSelfPhreadID()
{
  INT32 ulpthreadId;

#if VOS_PLAT_LINUX
  ulpthreadId = pthread_self();
#endif
  
  return ulpthreadId;
}

/*����һ����Ϣ���У�ͨ��KeyID����ȡ����Ϣ,���ض��е�ID*/
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

/*�ͷ�һ����Ϣ���У�ͨ��KeyID��ȷ��*/
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

/*�ж���Ϣ�Ƿ����*/
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

/*�̼߳�ͬ����Ϣ���ͣ�����ͬ��������������
  * iMsgPtr--���ڷ����̼߳�ĵ�ַ
  * �ĸ���ַ������
*/
INT32 VOS_MSGSend(INT32 ulMsgQueId, INTPTR iMsgPtr[4])
{
    VOS_MSG_S stQueMsg= {0};
    INT32 iRet = VOS_ERR;
#if VOS_PLAT_LINUX

    stQueMsg.lMsgType= 1;
    /*���ڴ���ָ��*/
    stQueMsg.iMsgPtr[0] = (INTPTR)iMsgPtr[0];
        
    iRet = msgsnd(ulMsgQueId, (VOID *)&stQueMsg, 4 * sizeof(INTPTR), 0);
    if ( iRet == -1 )
    {
        return VOS_ERR;
    }
#endif

    return VOS_OK;
}

/*�����̼߳��ͬ�����͵���Ϣ��ͬһ�����̣������ַ����ͬ��
  ��˿��Դ����ַ*/
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

//���̼�Ŀ�����δ�õ����Ȳ�֧��
#if 0
/*���̼���Ϣ,��Ϣ��Ҫ����*/
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
    
    /*lMsgType����0 �򷵻ض��е������һ����Ϣ��
       lMsgType����0���򷵻�������Ϊmtype�ĵ�һ����Ϣ��
       lMsgTypeС��0,�򷵻�������С�ڻ����mtype�����ľ���ֵ����С��һ����Ϣ��
       */
    pstMsgData->lMsgType= 1;
    //pstMsgData->stMsgTxt.ulDataLen = ulSndSize;
    //pstMsgData->stMsgTxt.pcPthreadData = pcSndBuf;
    /*���̼����ʹ�ÿ���,��Ϊ��ַ�ռ䲻ͬ*/
    //VOS_Mem_Copy(stMsgData.stMsgTxt.acQueBuf, pcSndBuf,ulSndSize);

    /*���ڱ�����ʹ�õ����̣߳��ռ乲������ʹ�ø�ֵ
    �����Ƚ��˷���Դ*/
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
    /*���̼����ʹ�ÿ���,��Ϊ��ַ�ռ䲻ͬ*/
    //VOS_Mem_Copy(pcSndBuf, stMsgData.stMsgTxt.acQueBuf, stMsgData.stMsgTxt.ulDataLen);

    /*���ڱ�����ʹ�õ����̣߳��ռ乲������ʹ�ø�ֵ
    �����Ƚ��˷���Դ*/
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
    /*lMsgType����0 �򷵻ض��е������һ����Ϣ��
       lMsgType����0���򷵻�������Ϊmtype�ĵ�һ����Ϣ��
       lMsgTypeС��0,�򷵻�������С�ڻ����mtype�����ľ���ֵ����С��һ����Ϣ��
       */
    stMsgData.lMsgType= 1;
    /*���̼����ʹ�ÿ���,��Ϊ��ַ�ռ䲻ͬ*/
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
    
    /*���̼����ʹ�ÿ���,��Ϊ��ַ�ռ䲻ͬ*/
    VOS_Mem_Copy(pcRcvBuf, stMsg.acMsgBuf, VOS_MSG_BUF_SIZE);
    
    return VOS_OK;
}
#endif

