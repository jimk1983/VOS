/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : vos_socket.h
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
#ifndef _VOS_SOCK_H_
#define _VOS_SOCK_H_

//Create TCP type socket , AF_INET
#define VOS_SOCK_TYPE_TCP   0 
//Create UDP type socket , AF_INET
#define VOS_SOCK_TYPE_UDP   1
//Create RAW type socket , AF_INET
#define VOS_SOCK_TYPE_RAW   2
//Create ARP type socket , PF_INET
#define VOS_SOCK_TYPE_ARP   3

//INVALID Socket defined
#define VOS_SOCKET_INVALID  0

//Socket Status
#define VOS_SOCK_OK                 0
#define VOS_SOCK_ERROR              -1
#define VOS_SOCK_EWOULDBLOCK        -2
#define VOS_SOCK_TCP_CONNECTING     -3
#define VOS_SOCK_TCP_CONNECTED      -4
#define VOS_SOCK_SSL_CONNECTING     -5
#define VOS_SOCK_SSL_ACCEPTING      -6
#define VOS_SOCK_SSL_ACCEPTED       -7
#define VOS_SOCK_SSL_EWOULDBLOCK    -8
#define VOS_SOCK_SSL_RECONNECTING   -9
#define VOS_SOCK_SSL_ERROR          -10
#define VOS_SOCK_FINISH             -11
#define VOS_SOCK_SSL_CONNECTED      -12

//Socket windows size
#define VOS_SOCK_SNDWINSIZE         65535
#define VOS_SOCK_RCVWINSIZE         65535

#define VOS_SOCK_MSGUDP_RCVWINSIZE  6*1024*1024   

#define VOS_SOCK_IPADDR_LEN         16

/*����ϵͳsocket�ţ�linux�Ǵ�С�����������ӵ�*/
#define VOS_SOCK_FDSIZE_MAX         20000

#define VOS_SOCK_LOCALHOST_STR      "127.0.0.1"

#define VOS_SOCK_INVALID            -1


LONG VOS_SOCK_Init();

LONG VOS_SOCK_UnInit();

LONG VOS_SOCK_Create(ULONG ulType);

LONG VOS_SOCK_Close(LONG lSockfd);

LONG VOS_SOCK_Shutdown(LONG lSockfd);

LONG VOS_SOCK_Send(LONG lSockfd, CHAR *pcBuf, ULONG ulBufLen, LONG *plErrorStatus);

LONG VOS_SOCK_Recv(LONG lSockfd, CHAR *pcBuf, ULONG ulBufLen, LONG *plErrorStatus);

LONG VOS_SOCK_Bind(LONG lSockfd, CHAR *pcLocalAddress, ULONG ulPort);

LONG VOS_SOCK_Listen(LONG lSockfd, ULONG ulNum);

LONG VOS_SOCK_Accept(LONG lListenfd, CHAR *pcClientAddr, USHORT *pusClientPort, LONG *plErrorStatus);

LONG VOS_SOCK_Connect(LONG lSockfd, CHAR *pcServAddr, USHORT usServPort, LONG *plErrorStatus);

LONG VOS_SOCK_SetOption(LONG lSockfd);


#endif