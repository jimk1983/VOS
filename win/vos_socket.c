/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : vos_socket.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��10��17��
  ����޸�   :
  ��������   : socket��װ
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/
    
#include "Common/common.h"

#include <WS2tcpip.h>


/*****************************************************************************
 �� �� ��  : VOS_SOCK_Init
 ��������  : Socket������ʼ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��29��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VOS_SOCK_Init()
{
#if VOS_PLAT_WIN32

    WORD      wVersionRequested;
    WSADATA wsaData;
    INT32       err;

    /*����2.2�汾��winsock��*/
    wVersionRequested = MAKEWORD(2,2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if ( 0 != err )
    {
        return VOS_ERR;
    }
    
#endif    
    
    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : VOS_SOCK_UnInit
 ��������  : Socket�Ļ���ȥ��ʼ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��29��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VOS_SOCK_UnInit()
{
#if VOS_PLAT_WIN32
    WSACleanup();
#endif

    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : VOS_SOCK_Create
 ��������  : ����
 �������  : ULONG ulType  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��29��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VOS_SOCK_Create(ULONG ulType)
{
    LONG lSockfd = VOS_SOCKET_INVALID;

#if VOS_PLAT_WIN32
	if (VOS_SOCK_TYPE_TCP == ulType)
	{
		lSockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	else if (VOS_SOCK_TYPE_UDP == ulType)
	{
		lSockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}
	else
	{
		lSockfd = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
	}
    
#elif VOS_PLAT_LINUX

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
#endif
    if ( VOS_SOCKET_INVALID > lSockfd )
    {
        return VOS_SOCKET_INVALID;
    }

    return lSockfd;
}


/*****************************************************************************
 �� �� ��  : VOS_SOCK_Close
 ��������  : �ر�
 �������  : LONG lSockfd  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��29��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VOS_SOCK_Close(LONG lSockfd)
{
    if ( VOS_SOCKET_INVALID == lSockfd )
    {
        return VOS_ERR;
    }
    
#if VOS_PLAT_WIN32
    closesocket(lSockfd);
#elif VOS_PLAT_LINUX
    close(lSockfd);
#endif

    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : VOS_SOCK_Shutdown
 ��������  : �ر�
 �������  : LONG lSockfd  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��29��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VOS_SOCK_Shutdown(LONG lSockfd)
{
    if ( VOS_SOCKET_INVALID == lSockfd )
    {
        return VOS_ERR;
    }
    
#if VOS_PLAT_LINUX
    shutdown(lSockfd, SHUT_RDWR); //SHUT_RDWR(2), SHUT_RD(0), SHUT_WR(1)
#elif VOS_PLAT_WIN32
    shutdown(lSockfd, SD_BOTH);
#endif

    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : VOS_SOCK_Send
 ��������  : ���ͽӿ�
 �������  : LONG lSockfd         
             CHAR *pcBuf          
             ULONG ulBufLen       
             LONG *plErrorStatus  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��29��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
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


/*****************************************************************************
 �� �� ��  : VOS_SOCK_Recv
 ��������  : �ӿڽӿ�
 �������  : LONG lSockfd         
             CHAR *pcBuf          
             ULONG ulBufLen       
             LONG *plErrorStatus  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��29��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VOS_SOCK_Recv(LONG lSockfd, CHAR *pcBuf, ULONG ulBufLen, LONG *plErrorStatus)
{
    LONG lRet = 0;
    
    if ( NULL == pcBuf)
    {
        return VOS_ERR;
    }
#if VOS_PLAT_WIN32
    lRet = recv(lSockfd, pcBuf, ulBufLen, 0);
    if( lRet < 0 )
    {
        if( GetLastError() == 10035 )
        {
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
            return VOS_ERR;
        }
        else
        {
            (*plErrorStatus) = VOS_SOCK_ERROR;
			printf("VOS_SOCK_Recv error=%d\n", GetLastError());
            
            return VOS_ERR;
        }
    }
    else if ( lRet == 0)
    {
        if ( 0 != GetLastError() )
        {
            (*plErrorStatus) = VOS_SOCK_FINISH;
        }
        else
        {
            (*plErrorStatus) = VOS_SOCK_EWOULDBLOCK;
        }
        
        return VOS_ERR;
	}
#endif
    
    return lRet;
}

/*****************************************************************************
 �� �� ��  : VOS_SOCK_Bind
 ��������  : ��
 �������  : LONG lSockfd          
             CHAR *pcLocalAddress  
             ULONG ulPort          
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��29��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VOS_SOCK_Bind(LONG lSockfd, CHAR *pcLocalAddress, ULONG ulPort)
{
    struct  sockaddr_in stLocalAddr = {0};

    if(NULL == pcLocalAddress
    || 0 == ulPort
    || 0>= lSockfd)
    {
        return VOS_ERR;
    }

	inet_pton(AF_INET, pcLocalAddress, &stLocalAddr.sin_addr);
    stLocalAddr.sin_family = AF_INET;
    //stLocalAddr.sin_addr.s_addr = inet_addr(pcLocalAddress);
    stLocalAddr.sin_port = htons((SHORT)ulPort);
    
    if(-1 == bind(lSockfd, (struct sockaddr *)&stLocalAddr, sizeof(stLocalAddr)))
    {

        printf("bind socket error!(errno=%d)",VOS_GetLastError());
        return VOS_ERR;
    }
    
    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : VOS_SOCK_Listen
 ��������  : ����
 �������  : LONG lSockfd  
             ULONG ulNum   
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��29��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
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


/*****************************************************************************
 �� �� ��  : VOS_SOCK_Accept
 ��������  : ����
 �������  : LONG lListenfd         
             CHAR *pcClientAddr     
             USHORT *pusClientPort  
             LONG *plErrorStatus    
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��29��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VOS_SOCK_Accept(LONG lListenfd, CHAR *pcClientAddr, USHORT *pusClientPort, LONG *plErrorStatus)
{
    LONG lClientFd = 0;
    struct sockaddr_in stClientAddr = {0};
    socklen_t stSerLen = sizeof(stClientAddr);
	CHAR acAddr[VOS_SOCK_IPADDR_LEN] = {0};
    
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
    
	inet_ntop(AF_INET, &stClientAddr.sin_addr, acAddr, VOS_SOCK_IPADDR_LEN);
	VOS_Mem_Copy_S(pcClientAddr, VOS_SOCK_IPADDR_LEN , acAddr, VOS_SOCK_IPADDR_LEN);

    //VOS_Mem_Copy(pcClientAddr,inet_ntoa(stClientAddr.sin_addr), strlen(inet_ntoa(stClientAddr.sin_addr)));
    
    return lClientFd;
}


/*****************************************************************************
 �� �� ��  : VOS_SOCK_Connect
 ��������  : ����
 �������  : LONG lSockfd         
             CHAR *pcServAddr     
             USHORT usServPort    
             LONG *plErrorStatus  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��29��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
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

	inet_pton(AF_INET, pcServAddr, &stServAddr.sin_addr);
	
    //stServAddr.sin_addr.s_addr = inet_addr(pcServAddr);
    
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


/*****************************************************************************
 �� �� ��:  VOS_SOCK_SetOption
 ��������  :    Ĭ�϶�����Ϊ������
 �������  :    LONG lSockfd        ---Socket�ķ�����
 �������  :    ��
 �� �� ֵ: 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��30��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VOS_SOCK_SetOption(LONG lSockfd)
{
  LONG lRet =0;
  LONG lnonBlockflag = 0; //1;

  LONG lSndBufsize = VOS_SOCK_SNDWINSIZE;
  LONG lRcvBufsize = VOS_SOCK_MSGUDP_RCVWINSIZE;

#if VOS_PLAT_LINUX
  lRet = ioctl(lSockfd, FIONBIO, &lnonBlockflag);
  if ( 0 != lRet )
  {
     return VOS_ERR;
  }
#elif VOS_PLAT_WIN32
  lRet = ioctlsocket(lSockfd, FIONBIO, (unsigned long *)&lnonBlockflag);
  if ( -1 == lRet )
  {
     return VOS_ERR;
  }
#endif
  
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

  return VOS_OK;
}


