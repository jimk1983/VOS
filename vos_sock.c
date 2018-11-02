/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_sock.c
  �� �� ��   : ����
  ��    ��   : jiangkang
  ��������   : 2015��9��25��
  ����޸�   :
  ��������   : socket�ĺ�����װ
  �����б�   :
              VOS_SOCK_Accept
              VOS_SOCK_Bind
              VOS_SOCK_Close
              VOS_SOCK_Connect
              VOS_SOCK_Create
              VOS_SOCK_GetHostAddress
              VOS_SOCK_GetPeerAddress
              VOS_SOCK_Listen
              VOS_SOCK_Recv
              VOS_SOCK_Recvfrom
              VOS_SOCK_Send
              VOS_SOCK_SendTo
              VOS_SOCK_SetOption
  �޸���ʷ   :
  1.��    ��   : 2015��9��25��
    ��    ��   : jiangkang
    �޸�����   : �����ļ�

******************************************************************************/

#include <vos_pub.h>


LONG VOS_SOCK_Init()
{
#if VOS_PLAT_WIN

    WORD      wVersionRequested;
    WSADATA wsaData;
    INT32       err;

    /*����1.1�汾��winsock��*/
    wVersionRequested = MAKEWORD(1,1);
    err = WSAStartup(wVersionRequested, &wsaData);
    if ( 0 != err )
    {
        return VOS_ERR;
    }
    
#endif    
    
    return VOS_OK;
}

LONG VOS_SOCK_UnInit()
{
#if VOS_PLAT_WIN
    WSACleanup();
#endif

    return VOS_OK;
}


LONG VOS_SOCK_Send(LONG lSockfd, CHAR *pcBuf, ULONG ulBufLen, LONG *plErrorStatus)
{
    LONG lRet = 0;
    
    if ( NULL == pcBuf)
    {
        return VOS_ERR;
    }

    lRet = send(lSockfd, pcBuf, ulBufLen, 0);
    if( lRet < 0 )
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK )
        {
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_ERR;
        }
        else
        {
            (*plErrorStatus) = VOS_SOCK_ERROR;
            return VOS_ERR;
        }
    }
    
    return lRet;
}

LONG VOS_SOCK_RAW_SendTo(LONG lSockfd, CHAR *pcBuf, ULONG ulBufLen, CHAR *acIfName, LONG *plErrorStatus)
{
    LONG lRet = 0;
    struct sockaddr sa;
    
    if ( NULL == pcBuf
        || NULL == acIfName )
    {
        return VOS_ERR;
    }

    VOS_Mem_Zero((CHAR *)&sa, sizeof(sa));
    VOS_StrCpy((CHAR *)sa.sa_data, acIfName);
    
    lRet = sendto(lSockfd, pcBuf, ulBufLen, 0, &sa, sizeof(sa));
    if( lRet < 0 )
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK )
        {
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_ERR;
        }
        else
        {
            VOS_Printf("vos raw socket send error, errno=%d", errno);
            (*plErrorStatus) = VOS_SOCK_ERROR;
            return VOS_ERR;
        }
    }        

    return lRet;
    
}

LONG VOS_SOCK_RAW_Recvfrom(LONG lSockfd, CHAR *pcBuf, ULONG ulBufLen, LONG *plErrorStatus)
{
    LONG lRet = 0;
    socklen_t lLen = 0;
    if ( NULL == pcBuf)
    {
        return VOS_ERR;
    }

    memset(pcBuf, 0, ulBufLen);
    
    lRet = recvfrom(lSockfd, pcBuf, ulBufLen, 0, NULL, &lLen);
    if( lRet < 0 )
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK )
        {
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_ERR;
        }
        else
        {
            (*plErrorStatus) = VOS_SOCK_ERROR;
            
            return VOS_ERR;
        }
    }
    else if ( lRet == 0)
    {
        (*plErrorStatus) = VOS_SOCK_FINISH;
        
        return VOS_ERR;
    }
    
    return lRet;
}


LONG VOS_SOCK_SendTo(LONG lSockfd, CHAR *pcBuf, ULONG ulBufLen, CHAR *pcPeerAddress, ULONG ulPeerPort, LONG *plErrorStatus)
{
    LONG lRet = 0;
    struct sockaddr_in stPeerAddr = {0};
    
    if ( NULL == pcBuf
        || NULL == pcPeerAddress )
    {
        return VOS_ERR;
    }
    stPeerAddr.sin_family = AF_INET;
    stPeerAddr.sin_addr.s_addr = inet_addr(pcPeerAddress);
    stPeerAddr.sin_port = htons(ulPeerPort);
    
    lRet = sendto(lSockfd, pcBuf, ulBufLen, 0, (struct sockaddr *)&stPeerAddr, sizeof(stPeerAddr));
    if( lRet < 0 )
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK )
        {
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_ERR;
        }
        else
        {
            (*plErrorStatus) = VOS_SOCK_ERROR;
            return VOS_ERR;
        }
    }        

    return lRet;
}

LONG VOS_SOCK_SendToN(LONG lSockfd, CHAR *pcBuf, ULONG ulBufLen, VOS_IPADDR_T uiPeerAddress, ULONG ulPeerPort, LONG *plErrorStatus)
{
    LONG lRet = 0;
    struct sockaddr_in stPeerAddr = {0};
    
    if ( NULL == pcBuf )
    {
        return VOS_ERR;
    }
    stPeerAddr.sin_family = AF_INET;
    stPeerAddr.sin_addr.s_addr = uiPeerAddress;
    stPeerAddr.sin_port = htons(ulPeerPort);
    
    lRet = sendto(lSockfd, pcBuf, ulBufLen, 0, (struct sockaddr *)&stPeerAddr, sizeof(stPeerAddr));
    if( lRet < 0 )
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK )
        {
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_ERR;
        }
        else
        {
            (*plErrorStatus) = VOS_SOCK_ERROR;
            return VOS_ERR;
        }
    }        

    return lRet;
}


LONG VOS_SOCK_Recv(LONG lSockfd, CHAR *pcBuf, ULONG ulBufLen, LONG *plErrorStatus)
{
    LONG lRet = 0;
    
    if ( NULL == pcBuf)
    {
        return VOS_ERR;
    }

    lRet = recv(lSockfd, pcBuf, ulBufLen, 0);
    if( lRet < 0 )
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK )
        {
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_ERR;
        }
        else
        {
            (*plErrorStatus) = errno;
            
            return VOS_ERR;
        }
    }
    else if ( lRet == 0)
    {
        (*plErrorStatus) = VOS_SOCK_FINISH;
        
        return VOS_ERR;
    }
    
    return lRet;
}


LONG VOS_SOCK_Recvfrom(LONG lSockfd, CHAR *pcBuf, ULONG ulBufLen, CHAR *pcPeerAddress, ULONG *pulPeerPort, LONG *plErrorStatus)
{
    LONG lRet = 0;
    struct sockaddr_in stPeerAddr = {0};
    socklen_t lSize = sizeof(stPeerAddr);
    
    if ( NULL == pcBuf  || NULL == plErrorStatus)
    {
        return VOS_ERR;
    }
        
    lRet = recvfrom(lSockfd, pcBuf, ulBufLen, 0, (struct sockaddr *)&stPeerAddr, &lSize);
    if( lRet <= 0 )
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK )
        {
            //VOS_Printf("VOS_SOCK_Recvfrom error!errno=%d", errno);
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_ERR;
        }
        else
        {
            (*plErrorStatus) = VOS_SOCK_ERROR;
            return VOS_ERR;
        }
    }   
    
    if ( NULL != pcPeerAddress )
    {
        VOS_Mem_Copy(pcPeerAddress, inet_ntoa(stPeerAddr.sin_addr),VOS_SOCK_IPADDR_LEN);
    }
    
    if ( NULL != pulPeerPort )
    {
        (*pulPeerPort) = ntohs(stPeerAddr.sin_port);
    }
    
    return lRet;
}


LONG VOS_SOCK_RecvfromN(LONG lSockfd, CHAR *pcBuf, ULONG ulBufLen, VOS_IPADDR_T *puiPeerAddress, ULONG *pulPeerPort, LONG *plErrorStatus)
{
    LONG lRet = 0;
    struct sockaddr_in stPeerAddr = {0};
    socklen_t lSize = sizeof(stPeerAddr);
    
    if ( NULL == pcBuf  || NULL == plErrorStatus)
    {
        return VOS_ERR;
    }
        
    lRet = recvfrom(lSockfd, pcBuf, ulBufLen, 0, (struct sockaddr *)&stPeerAddr, &lSize);
    if( lRet <= 0 )
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK )
        {
            //VOS_Printf("VOS_SOCK_Recvfrom error!errno=%d", errno);
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_ERR;
        }
        else
        {
            (*plErrorStatus) = VOS_SOCK_ERROR;
        }
        return VOS_ERR;
    }   
    
    if ( NULL != puiPeerAddress )
    {
        *puiPeerAddress = stPeerAddr.sin_addr.s_addr;
        VOS_Printf("recvfrom the addr=%08x\n", stPeerAddr.sin_addr.s_addr);
    }
    
    if ( NULL != pulPeerPort )
    {
        (*pulPeerPort) = ntohs(stPeerAddr.sin_port);
    }
    
    return lRet;
}


/*****************************************************************************
 �� �� ��  : VOS_SOCK_Bind
 ��������  : �󶨾���ĵ�ַ�Ͷ˿�
 �������  : LONG lSockfd          
             CHAR *pcLocalAddress  
             ULONG ulPort          
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��7��30��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VOS_SOCK_Bind(LONG lSockfd, CHAR *pcLocalAddress, ULONG ulPort)
{
    struct sockaddr_in stLocalAddr = {0};
    
    if(NULL == pcLocalAddress
    || 0 == ulPort
    || 0>= lSockfd)
    {
        return VOS_ERR;
    }

    stLocalAddr.sin_family = AF_INET;
    stLocalAddr.sin_addr.s_addr = inet_addr(pcLocalAddress);
    stLocalAddr.sin_port = htons(ulPort);
    
    if(-1 == bind(lSockfd, (struct sockaddr *)&stLocalAddr, sizeof(stLocalAddr)))
    {
        VOS_Printf("bind socket error!(errno=%d)",errno);
        return VOS_ERR;
    }
    
    return VOS_OK;
}

LONG VOS_SOCK_Listen(LONG lSockfd, ULONG ulNum)
{
    if ( 0 >= lSockfd )
    {
        return VOS_ERR;
    }
    
    if( -1 == listen(lSockfd,ulNum) )
    {
        return VOS_ERR;
    }
    
    return VOS_OK;
}

LONG VOS_SOCK_Accept(LONG lListenfd, CHAR *pcClientAddr, USHORT *pusClientPort, LONG *plErrorStatus)
{
    LONG lClientFd = 0;
    struct sockaddr_in stClientAddr = {0};
    socklen_t stSerLen = sizeof(stClientAddr);

    
    if ( 0 >= lListenfd 
        || NULL == pcClientAddr
        || NULL == pusClientPort)
    {
        (*plErrorStatus) = VOS_SOCK_ERROR;
        return VOS_SOCK_INVALID;
    }
    lClientFd = accept(lListenfd, (struct sockaddr *)&stClientAddr, &stSerLen);
    if( lClientFd < 0 )
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK )
        {
            usleep(1000);
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_SOCK_INVALID;
        }
        else
        {
            (*plErrorStatus) = VOS_SOCK_ERROR;
            return VOS_SOCK_INVALID;
        }
    }

    (*pusClientPort) = ntohs(stClientAddr.sin_port);
    
    VOS_Mem_Copy(pcClientAddr,inet_ntoa(stClientAddr.sin_addr), VOS_StrLen(inet_ntoa(stClientAddr.sin_addr)));
    
    return lClientFd;
}

LONG VOS_SOCK_Connect(LONG lSockfd, CHAR *pcServAddr, USHORT usServPort, LONG *plErrorStatus)
{
    struct sockaddr_in stServAddr = {0};
    LONG lRet =0;

    if ( 0 >= lSockfd 
        || NULL == pcServAddr
        || NULL == plErrorStatus)
    {
        (*plErrorStatus) = VOS_SOCK_ERROR;
        return VOS_ERR;
    }

    stServAddr.sin_family = AF_INET;
    stServAddr.sin_port = htons(usServPort);
    stServAddr.sin_addr.s_addr = inet_addr(pcServAddr);
    
    lRet = connect(lSockfd, (struct sockaddr *)&stServAddr, sizeof(stServAddr));
    if( lRet < 0 )
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK )
        {
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_OK;
        }
        else
        {
            (*plErrorStatus) = VOS_SOCK_ERROR;
            return VOS_ERR;
        }
    }
    
    (*plErrorStatus) = VOS_SOCK_OK;
    return VOS_OK;
}



LONG VOS_SOCK_ConnectNetAddr(LONG lSockfd, VOS_IPADDR_T uiIpaddr, USHORT usServPort, LONG *plErrorStatus)
{
    struct sockaddr_in stServAddr = {0};
    LONG lRet =0;

    if ( 0 >= lSockfd 
        || NULL == plErrorStatus)
    {
        (*plErrorStatus) = VOS_SOCK_ERROR;
        return VOS_ERR;
    }

    stServAddr.sin_family = AF_INET;
    stServAddr.sin_port = htons(usServPort);
    stServAddr.sin_addr.s_addr = uiIpaddr;
    
    lRet = connect(lSockfd, (struct sockaddr *)&stServAddr, sizeof(stServAddr));
    if( lRet < 0 )
    {
        /*115: operation now in progress, ����*/
        if(errno == EAGAIN || errno == EWOULDBLOCK || errno == EINPROGRESS )
        {
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_OK;
        }
        else
        {
            (*plErrorStatus) = VOS_SOCK_ERROR;
            return VOS_ERR;
        }
    }

    (*plErrorStatus) = VOS_SOCK_OK;
    
    return VOS_OK;
}


LONG VOS_SOCK_GetHostSockAddr(INT32 lSockfd, CHAR *pcHostAddress, UINT32 *pulPort)
{
    struct sockaddr_in stHostAddr = {0};
    socklen_t stSerLen = sizeof(stHostAddr);
    
    if ( 0 >= lSockfd 
        || NULL == pcHostAddress)
    {
        return VOS_ERR;
    }
    
    if(-1 == getsockname(lSockfd, (struct sockaddr *)&stHostAddr, &stSerLen))
    {
        return VOS_ERR;
    }
    
    VOS_Mem_Copy(pcHostAddress,inet_ntoa(stHostAddr.sin_addr),VOS_SOCK_IPADDR_LEN);
    (*pulPort) = stHostAddr.sin_port;
    
    return VOS_OK;
}



LONG VOS_SOCK_GetPeerSockAddr(LONG lSockfd, CHAR *pcPeerAddress, UINT32 *pulPort)
{
    struct sockaddr_in stPeerAddr = {0};
    socklen_t ulSerLen = sizeof(stPeerAddr);
    
    if ( 0 >= lSockfd 
        || NULL == pcPeerAddress)
    {
        return VOS_ERR;
    }
    
    if( -1 == getpeername((INT32)lSockfd, (struct sockaddr *)&stPeerAddr, &ulSerLen))
    {
        return VOS_ERR;
    }

    VOS_Mem_Copy(pcPeerAddress,inet_ntoa(stPeerAddr.sin_addr), VOS_SOCK_IPADDR_LEN);
    (*pulPort) = stPeerAddr.sin_port;
        
    return VOS_OK;
}


LONG VOS_SOCK_Create(ULONG ulType)
{
    LONG lSockfd = VOS_SOCKET_INVALID;

    if ( VOS_SOCK_TYPE_TCP == ulType )
    {
        lSockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    }
    else if ( VOS_SOCK_TYPE_UDP == ulType )
    {
        lSockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }
    else if (VOS_SOCK_TYPE_ARP == ulType)
    {
        /*ע�⣬��ģʽ��Ҫ����������Ϊ����ģʽ?
        ifconfig eth0 promisc ���û���
        ifconfig eth0 -promisc��ȡ������*/
        lSockfd = socket ( PF_INET,SOCK_PACKET, VOS_htons( ETH_P_ARP));
    }
    else if (VOS_SOCK_TYPE_RAW == ulType)
    {
        /*ע�⣬��ģʽ��Ҫ����������Ϊ����ģʽ?
        ifconfig eth0 promisc ���û���
        ifconfig eth0 -promisc��ȡ������*/
        //--fedora���湤���ܺã�����centos�¾ͻ���ֿ�����������
        //lSockfd = socket ( PF_INET,SOCK_PACKET, VOS_htons( ETH_P_IP));   

        //--centos������տ��Թ��������Ͳ���
        //lSockfd = socket (PF_PACKET, SOCK_RAW, VOS_htons(ETH_P_IP)); 
        lSockfd = socket (PF_PACKET, SOCK_PACKET, VOS_htons(ETH_P_IP)); 
    }
    else 
    {
        lSockfd = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    }

    if ( VOS_SOCKET_INVALID > lSockfd )
    {
        return VOS_SOCKET_INVALID;
    }

    return lSockfd;
}

LONG VOS_SOCK_Close(LONG lSockfd)
{
    if ( VOS_SOCKET_INVALID == lSockfd )
    {
        return VOS_ERR;
    }

    close(lSockfd);

    return VOS_OK;
}

LONG VOS_SOCK_Shutdown(LONG lSockfd)
{
    if ( VOS_SOCKET_INVALID == lSockfd )
    {
        return VOS_ERR;
    }

    shutdown(lSockfd, SHUT_RDWR); //SHUT_RDWR(2), SHUT_RD(0), SHUT_WR(1)

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : VOS_SOCK_SetBPFOption
 ��������  : ����rawsocket��BPF���˹���
 �������  : LONG lSockfd  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��9��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VOS_SOCK_SetBPFOption(LONG lSockfd)
{
    struct sock_filter BPF_code[6]= {
        { 0x20, 0, 0, 0x00000002 },
        { 0x15, 0, 3, 0x29f4cd78 },
        { 0x28, 0, 0, 0x00000000 },
        { 0x15, 0, 1, 0x0000000c },
        { 0x6, 0, 0, 0x00000060 },
        { 0x6, 0, 0, 0x00000000 }
    };                           
    struct sock_fprog Filter;

    Filter.len = 6;
    Filter.filter = BPF_code;

    /*BPF���˹���*/
    if(setsockopt(lSockfd, SOL_SOCKET, SO_ATTACH_FILTER,&Filter, sizeof(Filter))<0)
    {
        return VOS_ERR;
    }
    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : VOS_SOCK_SetProMiscMode
 ��������  : ���û���ģʽ
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��12��10��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VOS_SOCK_SetProMiscMode(LONG lSockfd, CHAR *pcIfName)
{
#if VOS_PLAT_LINUX

    struct ifreq ethreq;
    
    if ( NULL == pcIfName )
    {
        return VOS_ERR;
    }

    strncpy(ethreq.ifr_name,pcIfName,IFNAMSIZ);
    
    if (ioctl(lSockfd,SIOCGIFFLAGS, &ethreq)==-1) {
        return VOS_ERR;
    }
    
    ethreq.ifr_flags |= IFF_PROMISC;
    
    if (ioctl(lSockfd,SIOCSIFFLAGS,&ethreq)==-1) {
        return VOS_ERR;
    }
#endif
    return VOS_OK;
}

/*Ĭ�϶�����Ϊ������*/
LONG VOS_SOCK_SetOption(LONG lSockfd)
{
  LONG lRet =0;
  LONG lnonBlockflag = 1;

  //LONG lOn = 1;

  LONG lSndBufsize = VOS_SOCK_SNDWINSIZE;
  LONG lRcvBufsize = VOS_SOCK_RCVWINSIZE;

  struct linger so_linger = {0};

  /*�������ֽڶ���,ֱ�ӹر�*/
  so_linger.l_onoff = 1;
  so_linger.l_linger = 0;


  if( VOS_SOCKET_INVALID == lSockfd )
  {
        return VOS_ERR;
  }

  lRet = ioctl(lSockfd, FIONBIO, &lnonBlockflag);
  if ( 0 != lRet )
  {
        return VOS_ERR;
  }
  
  lRet = setsockopt(lSockfd, SOL_SOCKET, SO_SNDBUF,(const CHAR *)&lSndBufsize, sizeof(LONG) );
  if ( 0 != lRet )
  {
        return VOS_ERR;
  }
  
  lRet = setsockopt(lSockfd, SOL_SOCKET, SO_RCVBUF,(const CHAR *)&lRcvBufsize, sizeof(LONG) );
  if ( 0 != lRet )
  {
        return VOS_ERR;
  }

  /*�ر�linger*/
  lRet = setsockopt(lSockfd, SOL_SOCKET, SO_LINGER,(const CHAR *)&so_linger, sizeof(so_linger) );
  if ( 0 != lRet )
  {
        return VOS_ERR;
  }
#if 0
    /*�رս��յ�send��Σ�(SO_NOSIGPIPE-IOS/MacOS)linux���ܵ�sigPipe, ����
    linux: signal(SIGPIPE, SIG_IGN) */
    lRet = setsockopt(lSockfd, SOL_SOCKET, MSG_NOSIGNAL,(const CHAR *)&lOn, sizeof(LONG) );
    if ( 0 != lRet )
    {
        return VOS_ERR;
    }
#endif
    
    return VOS_OK;
}



/*Ĭ�϶�����Ϊ������*/
LONG VOS_SOCK_UDPSetOption(LONG lUdpSockfd)
{
  LONG lRet =0;
  LONG lnonBlockflag = 1;

  LONG lSndBufsize = VOS_SOCK_SNDWINSIZE;
  LONG lRcvBufsize = VOS_SOCK_MSGUDP_RCVWINSIZE;

  lRet = ioctl(lUdpSockfd, FIONBIO, &lnonBlockflag);
  if ( 0 != lRet )
  {
        return VOS_ERR;
  }
  
  lRet = setsockopt(lUdpSockfd, SOL_SOCKET, SO_SNDBUF,(const CHAR *)&lSndBufsize, sizeof(LONG) );
  if ( 0 != lRet )
  {
        return VOS_ERR;
  }
  
  lRet = setsockopt(lUdpSockfd, SOL_SOCKET, SO_RCVBUF,(const CHAR *)&lRcvBufsize, sizeof(LONG) );
  if ( 0 != lRet )
  {
        return VOS_ERR;
  }

  return VOS_OK;
}



VOS_IPADDR_T  VOS_SOCK_GetHostIpByName(CHAR *acDomain)
{
    VOS_IPADDR_T iRet  = VOS_INVALID_VAL32;
    struct hostent *    psthostent = NULL;
    struct in_addr **   ppstaddr_list = NULL;
    struct in_addr InIp = {0};
    
    if ( NULL == acDomain )
    {
        return VOS_INVALID_VAL32;
    }
    
    if ((psthostent = gethostbyname(acDomain)) == NULL) {  
        return VOS_INVALID_VAL32;
    }

    ppstaddr_list = (struct in_addr **)psthostent->h_addr_list;

    /*���ܴ��ڶ��IP*/
    //for(i = 0; ppstaddr_list[i] != NULL; i++) {
        //printf("%s ", inet_ntoa(*ppstaddr_list[i]));
    //}
    
    /*Ŀǰ��ȡ��һ��*/
    if ( NULL != ppstaddr_list[0]  )
    {
        InIp = *ppstaddr_list[0];
        iRet= InIp.s_addr;
    }
        
    return iRet;
}




