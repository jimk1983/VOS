/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : vos_reactor.h
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��10��30��
  ����޸�   :
  ��������   : ������ܼ����䣬 Ŀǰ��Ҫ���Windows���¼�ʵ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��10��30��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/

#if VOS_PLAT_WIN32

/*�ð汾��֧��һ�Զ�, �����������໥ͨ��*/
typedef enum
{
	VOS_PROCTYPE_UNKNOW = 0,/*��ǰ�ķ������*/
    VOS_PROCTYPE_TRASITSERV,/*ת������*/
	VOS_PROCTYPE_TCAPP,		/*�ն˲ɼ�����*/
	VOS_PROCTYPE_DEAMON,	/*�ػ�����*/
	VOS_PROCTYPE_MAINTAIN,  /*����ά������*/
	VOS_PROCTYPE_TOOL,		/*���߽���*/
	VOS_PROCTYPE_SEVER,		/*�������*/
	VOS_PROCTYPE_UIAPP,		/*UIAPP�������*/
	VOS_PROCTYPE_MONITOR,	/*MONITOR����*/
    VOS_PROCTYPE_UPGRADE,  /*��������*/

	VOS_PROCTYPE_NUMS
}RCT_PROCTYPE_E;

#define VOS_PROTYPE_MAXNUMS     128

/*�¼�������*/
typedef enum
{
    VOS_CTRL_UNKNOW=0,
    SOCKCTRL_CLOSE_RECV,    /*�رս���*/
    SOCKCTRL_CLOSE_SEND,    /*�رշ���*/
    SOCKCTRL_OPEN_RECV,     /*�򿪽���*/
    SOCKCTRL_OPEN_SEND,     /*�رշ���*/
    SOCKCTRL_SHUTDOWN,      /*�ر�����*/

    SOCKCTRL_NUMS
}RCT_SOCKCTRL_CODE_E;
    
#define		WSAEVT_NUMS		            MAXIMUM_WAIT_OBJECTS
#define     RCT_BUFSIZE                 128*1024
#define     LISTEN_NUMS                 10

typedef struct tagRctEventSockMgr	VOS_RCT_MGR_S, *PVOS_RCT_MGR_S;

typedef VOID(*pfMainWorker)(VOID *argv);

typedef struct tagSockNetworkEvent
{
	WSAEVENT		arrWSAEvts[WSAEVT_NUMS];			/*��ǰ��socket�¼�����*/
	SOCKET			arrSocket[WSAEVT_NUMS];			    /*�¼���Ӧ��socket����*/
}SOCK_NEVET_S, *PSOCK_NEVET_S;

/*ÿ�����̵Ĵ�����Ϣ�ĺ���*/
typedef struct tagRctSockInfo
{
	VOS_DLIST_NODE_S			stNode;					/*�ڵ�*/
    BOOL                        bIsRegisted;            /*�Ƿ��Ѿ�ע��*/
	RCT_PROCTYPE_E				eSockType;				/*Socketҵ������*/
    INT32		                iIndex; 			    /*��������*/
    INT32		                iClientfd;			    /*�ͻ���FD*/
    SOCKET		                sSockfd;			    /*SocketFD*/
    CHAR                        acUuid[VOS_GUID_LEN];   /*UUID*/
    CHAR                        acClientAddr[VOS_SOCK_IPADDR_LEN];
    USHORT                      usClientPort;
    CHAR                        acRecvBuff[RCT_BUFSIZE];    /*���ջ���*/
    INT32                       iRecvOffset;
    VOS_RW_LOCK_S               stMutex;
}RCT_SOCKINFO_S,*PRCT_SOCKINFO_S;

/*WSAEvent�̵߳�socket��������Worker��Handler����ʹ��*/
/*ÿ���̶߳��иù�����*/
struct tagRctEventSockMgr
{
    VOS_THREAD_T		hThreadHandle;						/*�߳̾��*/
    SOCKET              sMgrSocket;
    USHORT              usMgrPort;                          /*����˿�*/
    RCT_SOCKINFO_S	    stArrySockInfo[WSAEVT_NUMS];		/*�����Socket��Ϣ�� ͨ���������������Ϳ��Ժ������¼�������ͬ��������߲���Ч��*/
    SOCK_NEVET_S        stArrySockEvt;                      /*�¼�������*/
	ULONG				ulSockNums;							/*��ǰ��û�г���64-4���Ϳ�����, һ������ͨ����, Ȼ�󱣳ֳɶ�*/
	pfMainWorker		pfWorkerCb;							/*��������*/
	CHAR				acRecvBuff[RCT_BUFSIZE];			/*���ջ���*/
    VOS_RW_LOCK_S       stLock;                             /*��*/
};

INT32   RCT_API_SockEventsAdd(PVOS_RCT_MGR_S pstRctMgr, SOCKET sSocketFd);

VOID    RCT_API_SockEventsDel(PVOS_RCT_MGR_S pstRctMgr, UINT32 uiEvtIndex);

VOS_RCT_MGR_S *RCT_API_RctMgrCreate(USHORT usMgrPort, pfMainWorker pfWorker);

VOID RCT_API_RctMgrRelease(VOS_RCT_MGR_S **ppstMgrCtx);

#elif VOS_PLAT_LINUX





#endif











