/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : vos_reactor.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��10��30��
  ����޸�   :
  ��������   : �����������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��10��30��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/

#include "Common/common.h"


#if VOS_PLAT_WIN32
/*****************************************************************************
 �� �� ��  : RCT_SockEventsAdd
 ��������  : ��ӵ�ǰ��socket�¼�
 �������  : PVOS_RCT_MGR_S *pstRctMgr          ---�����¼�
             SOCKET sSocketFd               
             UINT32 uiSockType          
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��30��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32   RCT_API_SockEventsAdd(PVOS_RCT_MGR_S pstRctMgr, SOCKET sSocketFd)
{
    INT32       iIndex = 0;
    WSAEVENT    wnewEvent = NULL;
    
    if ( NULL == pstRctMgr 
        || INVALID_SOCKET == sSocketFd )
    {
        return VOS_ERR;
    }

    wnewEvent = WSACreateEvent();
    if ( NULL == wnewEvent )
    {
        return VOS_ERR;
    }
    
    for (iIndex = 0; iIndex < WSAEVT_NUMS-1; iIndex++)
    {
        if ( VOS_SOCKET_INVALID == pstRctMgr->stArrySockEvt.arrSocket[iIndex] )
        {
            pstRctMgr->stArrySockEvt.arrSocket[iIndex]  = sSocketFd;
            pstRctMgr->stArrySockEvt.arrWSAEvts[iIndex] = wnewEvent;

            /*��Ҫ���Ǻ�����չ����ʹ��*/
            pstRctMgr->stArrySockInfo[iIndex].iIndex  = iIndex;
            pstRctMgr->stArrySockInfo[iIndex].sSockfd  = sSocketFd;
            pstRctMgr->ulSockNums++;
            return iIndex;
        }
    }

    return VOS_ERR;
}


/*****************************************************************************
 �� �� ��  : RCT_SockEventsDelete
 ��������  : ɾ��
 �������  : PVOS_RCT_MGR_S *pstRctMgr  ---�����¼�
             UINT32 uiEvtIndex          
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��30��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID   RCT_API_SockEventsDel(PVOS_RCT_MGR_S pstRctMgr, SOCKET sSockfd)
{
	INT32       iIndex = 0;

    if ( NULL == pstRctMgr
        || sSockfd == INVALID_SOCKET )
    {
        return;
    }

	/*V0.7�ȶ�Ϊ����������ȷ��Ŀǰ����Ƶ���Ͽ�������������ʧ������Linux����Epollʵ�ּ���*/
	for (iIndex = 0; iIndex < WSAEVT_NUMS - 1; iIndex++)
	{
		if ( sSockfd == pstRctMgr->stArrySockEvt.arrSocket[iIndex]
			&& VOS_SOCK_INVALID != sSockfd )
		{
			VOS_SOCK_Close(sSockfd);
			WSACloseEvent(pstRctMgr->stArrySockEvt.arrWSAEvts[iIndex]);

			pstRctMgr->stArrySockEvt.arrSocket[iIndex] = VOS_SOCKET_INVALID;
			pstRctMgr->stArrySockEvt.arrWSAEvts[iIndex]= NULL;

			pstRctMgr->stArrySockInfo[iIndex].bIsRegisted = VOS_FALSE;
			pstRctMgr->stArrySockInfo[iIndex].eSockType = VOS_PROCTYPE_UNKNOW;
			pstRctMgr->stArrySockInfo[iIndex].iIndex = -1;
			pstRctMgr->stArrySockInfo[iIndex].iClientfd = 0;
            pstRctMgr->stArrySockInfo[iIndex].sSockfd  = VOS_SOCKET_INVALID;
			VOS_Mem_Zero(pstRctMgr->stArrySockInfo[iIndex].acUuid, VOS_GUID_LEN);
			pstRctMgr->ulSockNums--;
			return;
		}
	}
    
    return;
}

/*****************************************************************************
 �� �� ��  : RCT_API_RctMgrCreate
 ��������  : ����������
 �������  : USHORT usMgrPort  
 �������  : ��
 �� �� ֵ  : VOS_RCT_MGR_S
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��30��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_RCT_MGR_S *RCT_API_RctMgrCreate(USHORT usMgrPort, pfMainWorker pfWorker)
{
    PVOS_RCT_MGR_S  pstMgrCtx   = NULL;
    SOCKET          sSocket     = VOS_SOCKET_INVALID;
    INT32           iIndex      = 0;
#if 0
	if ( NULL == pfWorker )
	{
		return NULL;
	}
#endif

    pstMgrCtx = (VOS_RCT_MGR_S *)VOS_MALLOC(0, sizeof(VOS_RCT_MGR_S));
    if ( NULL == pstMgrCtx )
    {
        return NULL;
    }
	
	pstMgrCtx->pfWorkerCb = pfWorker;

    sSocket = VOS_SOCK_Create(VOS_SOCK_TYPE_TCP);
    if ( VOS_SOCKET_INVALID == sSocket )
    {
        VOS_FREE(pstMgrCtx);
        return NULL;
    }

    if ( VOS_ERR == VOS_SOCK_Bind(sSocket, VOS_SOCK_LOCALHOST_STR, usMgrPort) )
    {
        VOS_SOCK_Close(sSocket);
        VOS_FREE(pstMgrCtx);
        return NULL;
    }

    if ( VOS_ERR == VOS_SOCK_Listen(sSocket, LISTEN_NUMS) )
    {
        VOS_SOCK_Close(sSocket);
        VOS_FREE(pstMgrCtx);
        return NULL;
    }

#if 0
    if ( VOS_ERR == VOS_SOCK_SetOption(sSocket) )
    {
        VOS_SOCK_Close(sSocket);
        VOS_FREE(pstMgrCtx);
        return NULL;
    }
#endif

    for (iIndex =0; iIndex < WSAEVT_NUMS-1; iIndex++ )
    {
        pstMgrCtx->stArrySockEvt.arrSocket[iIndex]  = VOS_SOCKET_INVALID;
        pstMgrCtx->stArrySockEvt.arrWSAEvts[iIndex] = NULL;
        
        pstMgrCtx->stArrySockInfo[iIndex].eSockType = VOS_PROCTYPE_UNKNOW;
        pstMgrCtx->stArrySockInfo[iIndex].iIndex = 0;
        pstMgrCtx->stArrySockInfo[iIndex].bIsRegisted = VOS_FALSE;
        pstMgrCtx->stArrySockInfo[iIndex].sSockfd = VOS_SOCKET_INVALID;
        VOS_DLIST_INIT(&pstMgrCtx->stArrySockInfo[iIndex].stNode);
        VOS_RWLOCK_INIT(pstMgrCtx->stArrySockInfo[iIndex].stMutex);
    }
    pstMgrCtx->sMgrSocket = sSocket;

    VOS_RWLOCK_INIT(pstMgrCtx->stLock);
    VOS_DLIST_INIT(&pstMgrCtx->stArrySockInfo[0].stNode);
#if 0
    /*������ʱ�򣬵�WSAEVT_NUMS-1���͸���ǰ��Acceptʹ�ã����Ч��*/
    pstMgrCtx->stArrySockEvt.arrSocket[0]	= sSocket;
    pstMgrCtx->stArrySockEvt.arrWSAEvts[0]  = WSACreateEvent();

    
    pstMgrCtx->stArrySockInfo[0].eSockType = VOS_PROCTYPE_TRASITSERV;
    pstMgrCtx->stArrySockInfo[0].sSockfd   = sSocket;

	WSAEventSelect(sSocket, pstMgrCtx->stArrySockEvt.arrWSAEvts[0], FD_ACCEPT | FD_CLOSE);
#endif

    pstMgrCtx->usMgrPort = usMgrPort;
    pstMgrCtx->ulSockNums++;

#if 0
    pstMgrCtx->hThreadHandle = VOS_Thread_Create(NULL, pfWorker, pstMgrCtx, 0, 0);
    if ( VOS_INVALID_HANDLE == pstMgrCtx->hThreadHandle )
    {
        WSACloseEvent(pstMgrCtx->stArrySockEvt.arrWSAEvts[0]);
        VOS_SOCK_Close(sSocket);
        VOS_FREE(pstMgrCtx);
        return NULL;
    }
#endif

    return pstMgrCtx;
}

/*****************************************************************************
 �� �� ��  : RCT_API_Win32RctMgrRelease
 ��������  : �ͷŹ�����
 �������  : VOS_RCT_MGR_S *pstMgrCtx  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��30��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID RCT_API_RctMgrRelease(VOS_RCT_MGR_S **ppstMgrCtx)
{
    VOS_RCT_MGR_S *pstMgrCtx = NULL;
    INT32       iIndex = 0;

    if ( NULL == ppstMgrCtx )
    {
        return;
    }

    pstMgrCtx= *ppstMgrCtx;

    if ( NULL != pstMgrCtx )
    {
        for (iIndex = 0; iIndex < WSAEVT_NUMS - 1; iIndex++)
        {
            VOS_RWLOCK_FREE(pstMgrCtx->stArrySockInfo[iIndex].stMutex);
        }
        VOS_RWLOCK_FREE(pstMgrCtx->stLock);
        VOS_FREE(pstMgrCtx);

        *ppstMgrCtx = NULL;
    }
}


#elif VOS_PLAT_LINUX


#endif






