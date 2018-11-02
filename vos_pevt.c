/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_pevt.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年11月24日
  最近修改   :
  功能描述   : VOS_Pthread event线程的时间队列通知机制
                            仿照EPOLL的实现
                            本质上是一个消息队列
  函数列表   :
  修改历史   :
  1.日    期   : 2016年11月24日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/

#include <vos_pub.h>


/*****************************************************************************
 函 数 名  : VOS_PthEvtGetQue
 功能描述  : 获取事件队列
 输入参数  : UINT32 uiQueID  
 输出参数  : 无
 返 回 值  : VOS_PTHEVT_QUE_S
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年11月24日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOS_PTHEVT_QUE_S *VOS_PthEvt_GetQue(UINT32 uiQueID)
{
    VOS_PTHEVT_QUE_S *pstEvtQue = NULL;


    return pstEvtQue;
}



/*****************************************************************************
 函 数 名  : VOS_PthEvt_Notify
 功能描述  : 事件队列写入
 输入参数  : UINT32 uiQueID   
             UINT32 uiEvtype  
             VOID *pcData     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年11月24日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG VOS_PthEvt_Notify(UINT32 uiQueID, UINT32 uiEvtype, VOID *pcData)
{
    VOS_PTHEVT_QUE_S *pstEvtQue = VOS_PthEvt_GetQue(uiQueID);

    if ( NULL == pstEvtQue )
    {
        return VOS_ERR;
    }

#if VOS_PLAT_WIN
    if( pstEvtQue->uiFreeFlag == VOS_TRUE )
    {
        return VOS_ERR;
    }
    /*TODO1: 查看队列空闲标记，是否队列长度满，等待read在处理中,
                   如果不空闲，表示长度满，不能入队列，返回失败*/
    /*TODO2: 否则新建节点*/
    /*TODO3: 入队列链表中, 判断是否有超过队列最大数,
                   如果队列超过长度，设置不空闲*/
    if( pstEvtQue->uiEventNums > VOS_PTHEVT_DPTCH_NUMS )
    {
        pstEvtQue->uiFreeFlag = VOS_TRUE;
    }
    
    SetEvent(pstEvtQue->stEventHandle);


#elif  VOS_PLAT_MAC

#elif VOS_PLAT_LINUX

#endif

    return VOS_OK;
}


/*****************************************************************************
 函 数 名  : VOS_PthEvt_WaitForDispatch
 功能描述  : 事件队列读取链表所有事件
 输入参数  : UINT32 uiQueID             
             VOS_DLIST_NODE_S *pstEvts  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年11月24日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG VOS_PthEvt_WaitForDispatch(UINT32 uiQueID)
{    
    VOS_PTHEVT_QUE_S   *pstEvtQue = VOS_PthEvt_GetQue(uiQueID);
    VOS_DLIST_NODE_S     stEvtList = {0};
    VOS_DLIST_NODE_S     *pstEvtList = NULL;
    
        
    if ( NULL == pstEvtQue )
    {
        return VOS_ERR;
    }

    pstEvtList = &stEvtList;

    VOS_DLIST_INIT(pstEvtList);

#if VOS_PLAT_WIN
    WaitForSingleObject(pstEvtQue->stEventHandle, INFINITE);
    /*TODO1: 将链表头切换到本stEvtList*/
    /*TODO: for循环，处理所有的事件，根据事件的注册函数进行处理*/

    /*处理完要打开空闲标记*/
    pstEvtQue->uiFreeFlag = VOS_FALSE;
    
#elif  VOS_PLAT_MAC
    
#elif VOS_PLAT_LINUX
    
#endif

    return VOS_OK;
}




