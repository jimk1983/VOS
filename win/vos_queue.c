/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : vos_queue.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��10��17��
  ����޸�   :
  ��������   : ���̵߳Ķ��в���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
    
#include "Common/common.h"

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

#define VOS_MSGQUE_TIMEOUT     300

#define VOS_INFINITE			0xFFFFFFFF

/*ֱ����Ϣ����*/
typedef enum
{
    VOS_QUEMSG_TYPE_UNKNOW=0,   /*δ֪����*/
    VOS_QUEMSG_TYPE_CTRL,       /*��������*/
    VOS_QUEMSG_TYPE_DATA,       /*��������*/
    VOS_QUEMSG_TYPE_CMD_STOP,   /*����ֹͣ����*/
    VOS_QUEMSG_TYPE_USER,       /*�û��Զ�������*/

    VOS_QUEMSG_TYPE_NUMS,
}VOS_QUEMSG_TYPE_E;


/*�ڲ�����Ϣ�ڵ�*/
typedef struct tagVosMsgQueNode
{
    VOS_DLIST_NODE_S    stNode;
	UINT32              uiMsgCode;		/*�ڲ����Ƶ���Ϣ��*/
	UINT32              uiMsgLength;    /*���ݴ���*/
	VOID*               pvUserMsg;		/*�û����ݱ���, ��ǰ����ָ����ʽ*/
}VOS_MSGQUE_NODE_S, *PVOS_MSGQUE_NODE_S;

/*****************************************************************************
 �� �� ��  : VOS_API_MsgQuePop
 ��������  : ������Ϣ������������ȡ��
 �������  : VOS_MSGQUE_S *pvMsgQue     --����
			 VOID **ppvMsg          --�û�������
			 INT32 *piMsgLen        --���ݵĳ���
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
	��    ��   : ����
	�޸�����   : �����ɺ���
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
 �� �� ��  : VOS_API_MsgQuePush
 ��������  : ��Ϣ������������
 �������  : VOS_MSGQUE_S *pvMsgQue     ---����
             VOID *pvMsg            ---�û����͵�����
             INT32 iMsgLen          ---�û����͵ĳ���
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
	/*�û����ݣ���ʱ���嶼������������Ϣ*/
	pstMsgNode->uiMsgCode = VOS_QUEMSG_TYPE_DATA;
	pstMsgNode->uiMsgLength = iMsgLen;
	pstMsgNode->pvUserMsg = pvMsg;

	VOS_DLIST_ADD_TAIL(&pstMsgQue->stUsedList, &pstMsgNode->stNode);
	pstMsgQue->iUsedNums++;
	VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);
    
    /*Ŀǰ��֤ÿ����ʵʱ֪ͨ*/
    VOS_EventNotify(pstMsgQue->hEventNotify);
    
    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : VOS_API_MsgQuePush
 ��������  : ��Ϣ����������ͷ������
 �������  : VOS_MSGQUE_S *pvMsgQue     ---����
             VOID *pvMsg            ---�û����͵�����
             INT32 iMsgLen          ---�û����͵ĳ���
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
    /*�û����ݣ���ʱ���嶼������������Ϣ*/
    pstMsgNode->uiMsgCode = VOS_QUEMSG_TYPE_DATA;
    pstMsgNode->uiMsgLength = iMsgLen;
    pstMsgNode->pvUserMsg = pvMsg;

    VOS_DLIST_ADD_HEAD(&pstMsgQue->stUsedList, &pstMsgNode->stNode);
    pstMsgQue->iUsedNums++;
    VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);

    /*Ŀǰ��֤ÿ����ʵʱ֪ͨ*/
    VOS_EventNotify(pstMsgQue->hEventNotify);

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : VOS_API_MsgQueUserRegister
 ��������  : �û���Ϣ��ע��
 �������  : PVOS_MSGQUE_S pstQueue                     --����
           VOID *pvUserCtx                          --�û�������
           pfVosQueUserHandlerCb pfUserHandleCb     --�û�ע�����Ϣ������
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��18��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_MsgQue_Create
 ��������  : ���еĴ���
 �������  : INT32 iSize                                --���еĴ�С
           pfVosQueUserHandlerCb pfUserHandleCb     --���е��û�������
           VOID *pvUserCtx                          --�û���������
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��18��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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

    /*�����¼�֪ͨ��Ƶ�ʲ���*/
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

    /*��ʼ�½���Ϣ�ڵ�*/
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
 �� �� ��  : VOS_API_MsgQueRelease
 ��������  : ���е��ͷ�
 �������  : PVOS_MSGQUE_S pstQueue         --����
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
        /* �������� */
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

		/* �����е����� */
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
 �� �� ��  : VOS_API_MsgQueWorkerWaitfor
 ��������  : ��Ϣ����������Ϣ����
 �������  : PVOS_MSGQUE_S *pstQueue  --�������еȴ�����
 �������  : INT32 *piErrorCode         --���н����Ĵ�����
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��18��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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

		/*�������渴���߼���չ����γ�����, �ȼ������һ��*/
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
				/*�������ֱ�ӽ�����ǰ�߳�*/
				return VOS_OK;
			}
		}
    }

    VOS_EventNotify(pstMsgQue->hStopWait);

    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : VOS_API_MsgQueStop
 ��������  : ��Ϣ���д���ֹͣ
 �������  : PVOS_MSGQUE_S *pstQueue  --ֹͣ�Ķ���
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��18��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
    
    /*�û����ݣ���ʱ���嶼������������Ϣ*/
    pstMsgNode->uiMsgCode   = VOS_QUEMSG_TYPE_CMD_STOP;
    
    VOS_DLIST_ADD_TAIL(&pstMsgQue->stUsedList, &pstMsgNode->stNode);
    pstMsgQue->iUsedNums++;
    
	VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);
    
    /*Ŀǰ��֤ÿ����ʵʱ֪ͨ*/
    VOS_EventNotify(pstMsgQue->hEventNotify);
    
    return VOS_OK;
}

