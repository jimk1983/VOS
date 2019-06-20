/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : vos_dlist.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年10月17日
  最近修改   :
  功能描述   : 链表
  函数列表   :
  修改历史   :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/
#ifndef _VOS_DLIST_H_
#define _VOS_DLIST_H_
    
typedef struct tagVosDlistNode
{
    struct tagVosDlistNode *prev;
    struct tagVosDlistNode *next;
}VOS_DLIST_NODE_S, *PVOS_DLIST_NODE_S;

#ifndef container_of
#define container_of(ptr, type, member) \
            (type *)((char *)(ptr) - (char *) &((type *)0)->member)
#endif
    
#define VOS_DLIST_ENTRY(pnode,type,membernode) \
             container_of(pnode,type,membernode)
         
#define VOS_DLIST_FOR_EACH_ENTRY(pos, pstList, type, stnode)\
         for(pos = VOS_DLIST_ENTRY((pstList)->next, type, stnode);\
             &pos->stnode != (pstList);\
             pos =VOS_DLIST_ENTRY(pos->stnode.next, type, stnode))\
    
#define VOS_DLIST_FOR_EACH_DEL(pos, pstList, type, stnode)\
              for(pos = VOS_DLIST_ENTRY((pstList)->next, type, stnode);\
                  &pos->stnode != (pstList);\
                  pos =VOS_DLIST_ENTRY((pstList)->next, type, stnode))\
    
#define VOS_DLIST_INIT(pstNode) \
    do{\
        if ( NULL != (pstNode) )\
        {\
             (pstNode)->prev = (pstNode);\
             (pstNode)->next = (pstNode);\
        }\
    }while(0)

        
#define VOS_DLIST_ADD_HEAD(pstList, pstNode)\
     {\
         if( (NULL != (pstList)) && (NULL != (pstNode)) )\
         {\
              (pstNode)->next = (pstList)->next;\
              (pstNode)->prev = (pstList);\
              (pstList)->next->prev = (pstNode);\
              (pstList)->next = (pstNode);\
         }\
     }
      
#define VOS_DLIST_ADD_TAIL(pstList,pstNode)\
         {\
             if( (NULL != (pstList)) && (NULL != (pstNode)) )\
             {\
                 (pstNode)->next = (pstList);\
                 (pstNode)->prev = (pstList)->prev;\
                 (pstList)->prev->next = (pstNode);\
                 (pstList)->prev = (pstNode);\
             }\
         }
              
#define VOS_DLIST_INSERT(pstPrev,pstNext,pstNode)\
         {\
             if ( (NULL != (pstPrev)) \
                 && (NULL != (pstNext))\
                 && (NULL != (pstNode)))\
             {\
                 (pstPrev)->next = (pstNode);\
                 (pstNext)->prev = (pstNode);\
                 (pstNode)->prev = (pstPrev);\
                 (pstNode)->next = (pstNext);\
             }\
         }

#define VOS_DLIST_DEL(pstNode)\
         {\
             if(NULL != (pstNode))\
             {\
                 (pstNode)->next->prev = (pstNode)->prev;\
                 (pstNode)->prev->next = (pstNode)->next;\
                 (pstNode)->next = (pstNode);\
                 (pstNode)->prev = (pstNode);\
             }\
         }
              
#define VOS_DLIST_HEAD_MOVE(pstListFrom,pstListTo)\
         {\
             if ( (NULL != (pstListFrom)) && (NULL != (pstListTo)) )\
             {\
                 (pstListTo)->next = (pstListFrom)->next;\
                 (pstListFrom)->next->prev = (pstListTo);\
                 (pstListFrom)->prev->next = (pstListTo);\
                 (pstListTo)->prev = (pstListFrom)->prev;\
                 (pstListFrom)->next = (pstListFrom);\
                 (pstListFrom)->prev = (pstListFrom);\
             }\
         }
    
    
INT32 VOS_Node_Init(VOS_DLIST_NODE_S *pstNode);

INT32 VOS_Node_HeadAdd(VOS_DLIST_NODE_S *pstListHead, VOS_DLIST_NODE_S *pstNode);

INT32 VOS_Node_TailAdd(VOS_DLIST_NODE_S *pstListHead, VOS_DLIST_NODE_S *pstNode);

INT32 VOS_Node_IsEmpty(VOS_DLIST_NODE_S *pstList);

INT32 VOS_Node_Remove(VOS_DLIST_NODE_S *pstNode);

INT32 VOS_Node_MoveList(VOS_DLIST_NODE_S *pstListFrom, VOS_DLIST_NODE_S *pstListTo);

INT32 VOS_Node_HeadGet(VOS_DLIST_NODE_S *pstListHead, VOS_DLIST_NODE_S **ppstNode);

INT32 VOS_Node_TailGet(VOS_DLIST_NODE_S *pstListHead, VOS_DLIST_NODE_S **ppstNode);
    
BOOL  VOS_List_IsEmpty(VOS_DLIST_NODE_S *pstList);

#endif











