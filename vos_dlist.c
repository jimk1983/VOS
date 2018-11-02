/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_dlist.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��1��10��
  ����޸�   :
  ��������   : VOS��DLIST����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��1��10��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#include <vos_pub.h>
#include <vos_log.h>


#define DLIST_HEAD_INIT(name) { &(name), &(name) }
#define DLIST_HEAD_NEW(name) \
              struct VOS_DLIST_NODE_S name = DLIST_HEAD_INIT(name);

			  
			  
			  
			  
     
/*��ʼ���ڵ�*/
INT32 VOS_Node_Init(VOS_DLIST_NODE_S *pstNode)
{
    if (NULL == pstNode)
    {
        return VOS_ERR;
    }

    VOS_DLIST_INIT(pstNode);

    return VOS_OK;
}


/*���ڵ�pstNode���뵽����ͷpstListHead����, ���׸�*/
INT32 VOS_Node_HeadAdd(VOS_DLIST_NODE_S *pstListHead, VOS_DLIST_NODE_S *pstNode)
{
    if ( (NULL == pstListHead) || 
         (NULL == pstNode))
    {
        return VOS_ERR;
    }
    
    VOS_DLIST_ADD_HEAD(pstListHead,pstNode);

    return VOS_OK;
}

/*���ڵ�pstNode���뵽����pstListHead��ĩβ*/
INT32 VOS_Node_TailAdd(VOS_DLIST_NODE_S *pstListHead, VOS_DLIST_NODE_S *pstNode)
{
    if ( (NULL == pstListHead) || 
         (NULL == pstNode))
    {
        return VOS_ERR;
    }
    
    VOS_DLIST_ADD_TAIL(pstListHead, pstNode);

    return VOS_OK;
}

/*�ж������Ƿ�Ϊ��
VOS_OK--��ʾ�����ǿյ�*/
INT32 VOS_Node_IsEmpty(VOS_DLIST_NODE_S *pstList)
{
    //if ( NULL == pstList )
    //{
    //    return VOS_ERR;
    //}

    if ( (pstList->next == pstList) 
        && (pstList->prev == pstList) )
    {
        return VOS_OK;   
    }

    return VOS_ERR;
}

/*��������ժ�����ڵ�*/
INT32 VOS_Node_Remove(VOS_DLIST_NODE_S *pstNode)
{
    if ( NULL == pstNode )
    {
        return VOS_ERR;
    }

    /*ժ�������³�ʼ��*/
    VOS_DLIST_DEL(pstNode);

    return VOS_OK;
}

/*�����ƶ�����From��To������*/
INT32 VOS_Node_MoveList(VOS_DLIST_NODE_S *pstListFrom, VOS_DLIST_NODE_S *pstListTo)
{
    if ( (NULL == pstListFrom) || (NULL == pstListTo) )
    {
        return VOS_ERR;
    }
    
    VOS_DLIST_HEAD_MOVE(pstListFrom,pstListTo);
    
    return VOS_OK;
}

/*��ȡ����ĵ�һ���ڵ�*/
INT32 VOS_Node_HeadGet(VOS_DLIST_NODE_S *pstListHead, VOS_DLIST_NODE_S **ppstNode)
{
    VOS_DLIST_NODE_S *pstTmp = NULL;
    
    if ( (NULL == pstListHead) || (NULL == ppstNode))
    {
        return VOS_ERR;
    }
    
    if ( VOS_OK != VOS_Node_IsEmpty(pstListHead) )
    {
        pstTmp = pstListHead->next;        
        VOS_DLIST_DEL(pstTmp);
        *ppstNode = pstTmp;        
    }
        
    return VOS_OK;
}

/*��ȡ�����δ�ڵ�*/
INT32 VOS_Node_TailGet(VOS_DLIST_NODE_S *pstListHead, VOS_DLIST_NODE_S **ppstNode)
{
    VOS_DLIST_NODE_S * pstTmp = NULL;

    if ( (NULL == pstListHead) || (NULL == ppstNode))
    {
        return VOS_ERR;
    }
    
    if ( VOS_OK != VOS_Node_IsEmpty(pstListHead) )
    {
        pstTmp = pstListHead->prev;        
        VOS_DLIST_DEL(pstTmp);
    }
    
    return VOS_OK;    
}


