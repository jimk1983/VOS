/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : vos_dlist.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��10��17��
  ����޸�   :
  ��������   : ����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/


#include "Common/common.h"


#define DLIST_HEAD_INIT(name) { &(name), &(name) }
#define DLIST_HEAD_NEW(name) \
              struct VOS_DLIST_NODE_S name = DLIST_HEAD_INIT(name);


/*****************************************************************************
 �� �� ��  : VOS_Node_Init
 ��������  : ��ʼ���ڵ�
 �������  : VOS_DLIST_NODE_S *pstNode  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32 VOS_Node_Init(VOS_DLIST_NODE_S *pstNode)
{
    if (NULL == pstNode)
    {
        return VOS_ERR;
    }

    VOS_DLIST_INIT(pstNode);

    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : VOS_Node_HeadAdd
 ��������  : ���ڵ�pstNode���뵽����ͷpstListHead����, ���׸�
 �������  : VOS_DLIST_NODE_S *pstListHead  
             VOS_DLIST_NODE_S *pstNode      
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
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



/*****************************************************************************
 �� �� ��  : VOS_Node_TailAdd
 ��������  : ���ڵ�pstNode���뵽����pstListHead��ĩβ
 �������  : VOS_DLIST_NODE_S *pstListHead  
             VOS_DLIST_NODE_S *pstNode      
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
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

/*****************************************************************************
 �� �� ��  : VOS_Node_IsEmpty
 ��������  : �ж������Ƿ�Ϊ��
 �������  : VOS_DLIST_NODE_S *pstList  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
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


/*****************************************************************************
 �� �� ��  : VOS_Node_Remove
 ��������  : ��������ժ�����ڵ�
 �������  : VOS_DLIST_NODE_S *pstNode  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
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


/*****************************************************************************
 �� �� ��  : VOS_Node_MoveList
 ��������  : �����ƶ�����From��To������
 �������  : VOS_DLIST_NODE_S *pstListFrom  
             VOS_DLIST_NODE_S *pstListTo    
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32 VOS_Node_MoveList(VOS_DLIST_NODE_S *pstListFrom, VOS_DLIST_NODE_S *pstListTo)
{
    if ( (NULL == pstListFrom) || (NULL == pstListTo) )
    {
        return VOS_ERR;
    }
    
    VOS_DLIST_HEAD_MOVE(pstListFrom,pstListTo);
    
    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : VOS_Node_HeadGet
 ��������  : ��ȡ����ĵ�һ���ڵ�
 �������  : VOS_DLIST_NODE_S *pstListHead  
             VOS_DLIST_NODE_S **ppstNode    
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
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


/*****************************************************************************
 �� �� ��  : VOS_Node_TailGet
 ��������  : ��ȡ�����δ�ڵ�
 �������  : VOS_DLIST_NODE_S *pstListHead  
             VOS_DLIST_NODE_S **ppstNode    
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
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



/*****************************************************************************
 �� �� ��  : VOS_List_IsEmpty
 ��������  : �ж������Ƿ�Ϊ��
 �������  : VOS_DLIST_NODE_S *pstList  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
BOOL VOS_List_IsEmpty(VOS_DLIST_NODE_S *pstList)
{
    if ( (pstList->next == pstList) 
        && (pstList->prev == pstList) )
    {
        return VOS_TRUE;   
    }

    return VOS_FALSE;
}

