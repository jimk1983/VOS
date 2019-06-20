/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : vos_reactor.h
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年10月30日
  最近修改   :
  功能描述   : 触发框架简单适配， 目前主要针对Windows的事件实现
  函数列表   :
  修改历史   :
  1.日    期   : 2018年10月30日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/

#if VOS_PLAT_WIN32

/*该版本不支持一对多, 仅单个进程相互通信*/
typedef enum
{
	VOS_PROCTYPE_UNKNOW = 0,/*当前的服务进程*/
    VOS_PROCTYPE_TRASITSERV,/*转发进程*/
	VOS_PROCTYPE_TCAPP,		/*终端采集进程*/
	VOS_PROCTYPE_DEAMON,	/*守护进程*/
	VOS_PROCTYPE_MAINTAIN,  /*升级维护进程*/
	VOS_PROCTYPE_TOOL,		/*工具进程*/
	VOS_PROCTYPE_SEVER,		/*服务进程*/
	VOS_PROCTYPE_UIAPP,		/*UIAPP界面进程*/
	VOS_PROCTYPE_MONITOR,	/*MONITOR进程*/
    VOS_PROCTYPE_UPGRADE,  /*升级进程*/

	VOS_PROCTYPE_NUMS
}RCT_PROCTYPE_E;

#define VOS_PROTYPE_MAXNUMS     128

/*事件控制码*/
typedef enum
{
    VOS_CTRL_UNKNOW=0,
    SOCKCTRL_CLOSE_RECV,    /*关闭接收*/
    SOCKCTRL_CLOSE_SEND,    /*关闭发送*/
    SOCKCTRL_OPEN_RECV,     /*打开接收*/
    SOCKCTRL_OPEN_SEND,     /*关闭发送*/
    SOCKCTRL_SHUTDOWN,      /*关闭连接*/

    SOCKCTRL_NUMS
}RCT_SOCKCTRL_CODE_E;
    
#define		WSAEVT_NUMS		            MAXIMUM_WAIT_OBJECTS
#define     RCT_BUFSIZE                 128*1024
#define     LISTEN_NUMS                 10

typedef struct tagRctEventSockMgr	VOS_RCT_MGR_S, *PVOS_RCT_MGR_S;

typedef VOID(*pfMainWorker)(VOID *argv);

typedef struct tagSockNetworkEvent
{
	WSAEVENT		arrWSAEvts[WSAEVT_NUMS];			/*当前的socket事件数组*/
	SOCKET			arrSocket[WSAEVT_NUMS];			    /*事件对应的socket索引*/
}SOCK_NEVET_S, *PSOCK_NEVET_S;

/*每个进程的处理消息的函数*/
typedef struct tagRctSockInfo
{
	VOS_DLIST_NODE_S			stNode;					/*节点*/
    BOOL                        bIsRegisted;            /*是否已经注册*/
	RCT_PROCTYPE_E				eSockType;				/*Socket业务类型*/
    INT32		                iIndex; 			    /*数组索引*/
    INT32		                iClientfd;			    /*客户端FD*/
    SOCKET		                sSockfd;			    /*SocketFD*/
    CHAR                        acUuid[VOS_GUID_LEN];   /*UUID*/
    CHAR                        acClientAddr[VOS_SOCK_IPADDR_LEN];
    USHORT                      usClientPort;
    CHAR                        acRecvBuff[RCT_BUFSIZE];    /*接收缓存*/
    INT32                       iRecvOffset;
    VOS_RW_LOCK_S               stMutex;
}RCT_SOCKINFO_S,*PRCT_SOCKINFO_S;

/*WSAEvent线程的socket管理器，Worker和Handler都会使用*/
/*每个线程都有该管理器*/
struct tagRctEventSockMgr
{
    VOS_THREAD_T		hThreadHandle;						/*线程句柄*/
    SOCKET              sMgrSocket;
    USHORT              usMgrPort;                          /*管理端口*/
    RCT_SOCKINFO_S	    stArrySockInfo[WSAEVT_NUMS];		/*管理的Socket信息， 通过索引管理，这样就可以和网络事件复用相同索引，提高查找效率*/
    SOCK_NEVET_S        stArrySockEvt;                      /*事件触发器*/
	ULONG				ulSockNums;							/*当前有没有超过64-4个就可以了, 一个用作通信了, 然后保持成对*/
	pfMainWorker		pfWorkerCb;							/*工作处理*/
	CHAR				acRecvBuff[RCT_BUFSIZE];			/*接收缓存*/
    VOS_RW_LOCK_S       stLock;                             /*锁*/
};

INT32   RCT_API_SockEventsAdd(PVOS_RCT_MGR_S pstRctMgr, SOCKET sSocketFd);

VOID    RCT_API_SockEventsDel(PVOS_RCT_MGR_S pstRctMgr, UINT32 uiEvtIndex);

VOS_RCT_MGR_S *RCT_API_RctMgrCreate(USHORT usMgrPort, pfMainWorker pfWorker);

VOID RCT_API_RctMgrRelease(VOS_RCT_MGR_S **ppstMgrCtx);

#elif VOS_PLAT_LINUX





#endif











