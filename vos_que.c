/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_que.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��5��22��
  ����޸�   :
  ��������   : ʵ�ֶ��еļ��ַ���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��5��22��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#include <vos_pub.h>

/*��Ҫ����Rct�ܹ��е���Ϣ������и�������*/
VOS_MSG_QUE_S *VOS_Rct_MsgQueCreate(UINT32 uiIndex)
{
    VOS_MSG_QUE_S *pstMsgQue = NULL;
    /*��Ϊ�ǳ�ʼ��ʱ�򴴽������ֱ����ϵͳ��*/
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
 �� �� ��  : VOS_Rct_MSGQue_Release
 ��������  : ��Ϣ���е��ͷ�
 �������  : VOS_MSG_QUE_S *pstMsgQue  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 

*****************************************************************************/
VOID VOS_Rct_MSGQue_Release(VOS_MSG_QUE_S *pstMsgQue)
{
    VOS_MSG_QUE_NODE_S *pstMsgNode = NULL;
    VOS_MSG_QUE_NODE_S *pstMsgTmp = NULL;

    if ( NULL != pstMsgQue )
    {
        /*����ж���Ľڵ㣬�����ɾ���������ڴ�й©*/
        if ( 0 < pstMsgQue->uiNum )
        {
            VOS_DLIST_FOR_EACH_ENTRY(pstMsgNode, &pstMsgQue->stList, VOS_MSG_QUE_NODE_S, stNode)
            {
                VOS_ASSERT(NULL != pstMsgNode);
                pstMsgTmp = VOS_DLIST_ENTRY(&pstMsgNode->stNode.prev, VOS_MSG_QUE_NODE_S, stNode);
                
                /*�����Ѿ������ˣ���˿���ֱ������*/
                VOS_Node_Remove((&pstMsgNode->stNode));
                /*��Ϣ�ڵ�ʹ��VOS_Malloc()����ģ������ڴ����*/
                VOS_Free((CHAR *)pstMsgNode);
                
                pstMsgNode = pstMsgTmp;
            }
        }   

        VOS_RWLOCK_FREE(pstMsgQue->stLock);
        free(pstMsgQue);  
    }
}

/*****************************************************************************
 �� �� ��  : VOS_Rct_MSGQueNode_Create
 ��������  : ��Ϣ���нڵ�Ĵ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : VOS_MSG_QUE_NODE_S
 ���ú���  : 
 ��������  : 
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
 �� �� ��  : VOS_Rct_MSGQueNode_Release
 ��������  : ��Ϣ���нڵ���ͷ�
 �������  : VOS_MSG_QUE_NODE_S *pstQueNode  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
*****************************************************************************/
VOID VOS_Rct_MSGQueNode_Release(VOS_MSG_QUE_NODE_S *pstQueNode)
{
    if ( NULL != pstQueNode )
    {
        VOS_Free((CHAR *)pstQueNode);
    }
}

/*****************************************************************************
 �� �� ��  : VOS_Rct_MSGQue_IsEmpty
 ��������  : �жϸ���Ϣ�����Ƿ�Ϊ��
 �������  : VOS_MSG_QUE_S *pstMsgQue  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
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
        /*�����ܷ����ģ����*/
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
 �� �� ��  : VOS_Rct_MSGQue_Push
 ��������  : ��һ����Ϣ��ӣ������ȳ������뵽����β��
 �������  : VOS_MSG_QUE_S *pstMsgQue        
             VOS_MSG_QUE_NODE_S *pstMsgNode  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
*****************************************************************************/
INT32 VOS_Rct_MSGQue_Push(VOS_MSG_QUE_S *pstMsgQue, VOS_MSG_QUE_NODE_S *pstMsgNode)
{
    
    if ( NULL == pstMsgQue
        || NULL == pstMsgNode )
    {
        VOS_Printf("VOS_Rct_MSGQue_Push, param error!pstMsgQue=%p, pstMsgNode=%p", pstMsgQue, pstMsgNode);
        return VOS_ERR;
    }

    /*�������*/
    VOS_RWLOCK_LOCK(pstMsgQue->stLock);
    VOS_DLIST_ADD_TAIL(&pstMsgQue->stList, &pstMsgNode->stNode);
    pstMsgQue->uiNum++;
    VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);

    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : VOS_Rct_MSGQue_Pop
 ��������  : �Ӷ����л�ȡ��Ϣ�ڵ㣬�����ȳ�������ͷȡ��
 �������  : VOS_MSG_QUE_S *pstMsgQue  
 �������  : ��
 �� �� ֵ  : VOS_MSG_QUE_NODE_S
 ���ú���  : 
 ��������  : 
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

    /*�ն���*/
    if ( 0 == pstMsgQue->uiNum )
    {
        /*�����ܷ����ģ����*/
        if(VOS_OK != VOS_Node_IsEmpty(&pstMsgQue->stList))
        {
            VOS_ASSERT(0);
            VOS_Printf("system error: rct msg queue POP check empty error!");
        }
        VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);
        return NULL;
    }
    /*ȡ������ͷ�ڵ�*/
    if(VOS_ERR == VOS_Node_HeadGet(&pstMsgQue->stList, &pstNode))
    {
        VOS_Printf("system error: rct msg queue get node error!");
    }
    
    pstMsgQue->uiNum--;
    
    VOS_RWLOCK_UNLOCK(pstMsgQue->stLock);

    pstMsgNode = (VOS_MSG_QUE_NODE_S   *)pstNode;

    return pstMsgNode;
}





