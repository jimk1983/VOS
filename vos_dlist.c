/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_dlist.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年1月10日
  最近修改   :
  功能描述   : VOS的DLIST操作
  函数列表   :
  修改历史   :
  1.日    期   : 2016年1月10日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
#include <vos_pub.h>
#include <vos_log.h>


#define DLIST_HEAD_INIT(name) { &(name), &(name) }
#define DLIST_HEAD_NEW(name) \
              struct VOS_DLIST_NODE_S name = DLIST_HEAD_INIT(name);

			  
			  
			  
			  
     
/*初始化节点*/
INT32 VOS_Node_Init(VOS_DLIST_NODE_S *pstNode)
{
    if (NULL == pstNode)
    {
        return VOS_ERR;
    }

    VOS_DLIST_INIT(pstNode);

    return VOS_OK;
}


/*将节点pstNode插入到链表头pstListHead后面, 即首个*/
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

/*将节点pstNode插入到链表pstListHead的末尾*/
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

/*判断链表是否为空
VOS_OK--表示链表是空的*/
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

/*在链表中摘除本节点*/
INT32 VOS_Node_Remove(VOS_DLIST_NODE_S *pstNode)
{
    if ( NULL == pstNode )
    {
        return VOS_ERR;
    }

    /*摘除并重新初始化*/
    VOS_DLIST_DEL(pstNode);

    return VOS_OK;
}

/*链表移动，从From到To的链表*/
INT32 VOS_Node_MoveList(VOS_DLIST_NODE_S *pstListFrom, VOS_DLIST_NODE_S *pstListTo)
{
    if ( (NULL == pstListFrom) || (NULL == pstListTo) )
    {
        return VOS_ERR;
    }
    
    VOS_DLIST_HEAD_MOVE(pstListFrom,pstListTo);
    
    return VOS_OK;
}

/*获取链表的第一个节点*/
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

/*获取链表的未节点*/
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


