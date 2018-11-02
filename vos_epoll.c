/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_epoll.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��1��12��
  ����޸�   :
  ��������   : EPOLLʵ��
  �����б�   :
              VOS_EPOLL_Create
              VOS_EPOLL_Ctrl
              VOS_EPOLL_Release
              VOS_EPOLL_Wait
  �޸���ʷ   :
  1.��    ��   : 2016��1��12��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#include <vos/vos_pub.h>



/*����һ��EPOLL ID, ��EPOLL����ͬʱ����ulEpollNum���¼�*/
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

/*�ر��ͷ�EPOLL*/
VOID VOS_EPOLL_Release(INT32 lEpollId)
{
    if ( lEpollId == VOS_EPOLL_INVALID )
    {
        return;
    }
    
    close(lEpollId);
    
    return;
}

/*���Ʊ�EPOLLID�ϵ�socket fd ���ͺͽ��տ���*/
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

/*�ȴ��¼�����*/
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
    /*ÿ�α�ʾ���»�ȡ*/
    VOS_Mem_Zero((CHAR *)pstWaitEvent, sizeof(VOS_EPOLL_WAIT_EVT_S));
    
    /*�ȴ�ÿ���¼�����*/
    lRet = epoll_wait(lEpollFd, astEvents, VOS_EPOLL_EVTWAIT_NUM, lTimeout);
    
    if (0 < lRet )
    {
        for(ulIndex=0;ulIndex < lRet; ulIndex++)
        {
            /*��ȡ��Ӧ���¼�socket fd*/
            pstWaitEvent->astEevent[ulIndex].lSockFd = astEvents[ulIndex].data.fd;
            if( astEvents[ulIndex].events & EPOLLIN)
            {
                /*���ö�Ӧ���¼�����*/
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
        /*��ʾ��ʱ�˳��������������������κ�����*/
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




