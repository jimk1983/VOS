/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_epoll.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年1月12日
  最近修改   :
  功能描述   : EPOLL实现
  函数列表   :
              VOS_EPOLL_Create
              VOS_EPOLL_Ctrl
              VOS_EPOLL_Release
              VOS_EPOLL_Wait
  修改历史   :
  1.日    期   : 2016年1月12日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
#include <vos/vos_pub.h>



/*创建一个EPOLL ID, 该EPOLL可以同时处理ulEpollNum个事件*/
INT32 VOS_EPOLL_Create(UINT32 ulEpollNum)
{
    INT32 lEpollId = VOS_EPOLL_INVALID;
    
    if ( ulEpollNum > VOS_EPOLL_EVENT_MAXNUM )
    {
        return VOS_EPOLL_INVALID;
    }
    
    lEpollId = epoll_create(ulEpollNum);
    if( 0 >=  lEpollId )
    {
        VOS_Printf("epoll create error, lEpollId =%d", lEpollId );
        return VOS_EPOLL_INVALID;
    }
    
    return lEpollId;
}

/*关闭释放EPOLL*/
VOID VOS_EPOLL_Release(INT32 lEpollId)
{
    if ( lEpollId == VOS_EPOLL_INVALID )
    {
        return;
    }
    
    close(lEpollId);
    
    return;
}

/*控制本EPOLLID上的socket fd 发送和接收开关*/
INT32 VOS_EPOLL_Ctrl(INT32 lEpollFd, INT32 lSockfd, UINT32 ulEpollCtl, UINT32 ulEpollMask)
{
    struct epoll_event stEvent = {0};
    LONG lRet = 0;

    if ( lSockfd == VOS_SOCKET_INVALID || lEpollFd == VOS_EPOLL_INVALID )
    {
        VOS_Printf("param error");
        return VOS_EPOLL_ERR;
    }

    stEvent.data.fd = lSockfd;  
    stEvent.events = 0;

    switch(ulEpollCtl)
    {
        case VOS_EPOLL_CTL_ADD:
            if (ulEpollMask & VOS_EPOLL_MASK_POLLIN)
            {
               stEvent.events |= EPOLLIN;
             }
             
             if (ulEpollMask & VOS_EPOLL_MASK_POLLOUT)
             {
               stEvent.events |= EPOLLOUT;
             }
             
             lRet = epoll_ctl(lEpollFd, EPOLL_CTL_ADD, lSockfd, &stEvent);
            if( 0 != lRet )
            {   
                VOS_Printf("epoll_ctl add error, lRet=%d,sockfd=%d, epollfd=%d, errno=%d", lRet, lSockfd, lEpollFd, errno);
                return VOS_EPOLL_ERR;
            }
          break;
        case VOS_EPOLL_CTL_MOD:
             if (ulEpollMask & VOS_EPOLL_MASK_POLLIN)
             {
                stEvent.events |= EPOLLIN;
             }
             else
             {
                stEvent.events &= ~(EPOLLIN);
             }

             if (ulEpollMask & VOS_EPOLL_MASK_POLLOUT)
             {
                stEvent.events |= EPOLLOUT;
             }
             else
             {
                stEvent.events &= ~(EPOLLOUT);
             }
             lRet = epoll_ctl(lEpollFd, EPOLL_CTL_MOD, lSockfd, &stEvent);
             if( 0 != lRet )
             {
                VOS_Printf("epoll_ctl mod error, lRet=%d,sockfd=%d, epollfd=%d, errno=%d", lRet, lSockfd, lEpollFd, errno);
                return VOS_EPOLL_ERR;
             }
          break;
        case VOS_EPOLL_CTL_DEL:
             lRet = epoll_ctl(lEpollFd, EPOLL_CTL_DEL, lSockfd, &stEvent);
             if( 0 != lRet )
             {
                VOS_Printf("epoll_ctl del error, lRet=%d,sockfd=%d, epollfd=%d, errno=%d", lRet, lSockfd, lEpollFd, errno);
                return VOS_EPOLL_ERR;
             }
          break;
        default:
          break;
    }     
    
    return VOS_EPOLL_OK;
}

/*等待事件发生*/
INT32 VOS_EPOLL_Wait(INT32 lEpollFd, VOS_EPOLL_WAIT_EVT_S *pstWaitEvent, INT32 lTimeout)
{
    INT32 lRet = VOS_EPOLL_ERR; 
    UINT32 ulIndex = 0;
    struct epoll_event astEvents[VOS_EPOLL_EVTWAIT_NUM]; 

    if ( NULL == pstWaitEvent )
    {
        return VOS_EPOLL_ERR;
    }

    VOS_Mem_Zero((CHAR *)&astEvents, sizeof(struct epoll_event) * VOS_EPOLL_EVTWAIT_NUM );
    /*每次表示重新获取*/
    VOS_Mem_Zero((CHAR *)pstWaitEvent, sizeof(VOS_EPOLL_WAIT_EVT_S));
    
    /*等待每次事件发生*/
    lRet = epoll_wait(lEpollFd, astEvents, VOS_EPOLL_EVTWAIT_NUM, lTimeout);
    
    if (0 < lRet )
    {
        for(ulIndex=0;ulIndex < lRet; ulIndex++)
        {
            /*获取对应的事件socket fd*/
            pstWaitEvent->astEevent[ulIndex].lSockFd = astEvents[ulIndex].data.fd;
            if( astEvents[ulIndex].events & EPOLLIN)
            {
                /*设置对应的事件类型*/
                pstWaitEvent->astEevent[ulIndex].ulEventMask |= VOS_EPOLL_MASK_POLLIN;
            }

            if(astEvents[ulIndex].events & EPOLLOUT)
            {
                pstWaitEvent->astEevent[ulIndex].ulEventMask |= VOS_EPOLL_MASK_POLLOUT;   
            }  
        }
        pstWaitEvent->ulEvtNum = lRet;
    }
    else if(lRet == 0)
    {
        /*表示超时退出，返回正常，不处理任何事情*/
        return VOS_EPOLL_OK;
    }
    else
    {
        if ( errno == 4)
        {
            return VOS_EPOLL_OK;
        }

        VOS_Printf("VOS_EPOLL_Wait error, lRet=%d,sockfd=%d, epollfd=%d", lRet, lEpollFd);
        return VOS_EPOLL_ERR;
    }  
    return lRet;
}




