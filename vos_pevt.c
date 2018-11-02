/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_pevt.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��11��24��
  ����޸�   :
  ��������   : VOS_Pthread event�̵߳�ʱ�����֪ͨ����
                            ����EPOLL��ʵ��
                            ��������һ����Ϣ����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��11��24��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/

#include <vos_pub.h>


/*****************************************************************************
 �� �� ��  : VOS_PthEvtGetQue
 ��������  : ��ȡ�¼�����
 �������  : UINT32 uiQueID  
 �������  : ��
 �� �� ֵ  : VOS_PTHEVT_QUE_S
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��11��24��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_PTHEVT_QUE_S *VOS_PthEvt_GetQue(UINT32 uiQueID)
{
    VOS_PTHEVT_QUE_S *pstEvtQue = NULL;


    return pstEvtQue;
}



/*****************************************************************************
 �� �� ��  : VOS_PthEvt_Notify
 ��������  : �¼�����д��
 �������  : UINT32 uiQueID   
             UINT32 uiEvtype  
             VOID *pcData     
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��11��24��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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
    /*TODO1: �鿴���п��б�ǣ��Ƿ���г��������ȴ�read�ڴ�����,
                   ��������У���ʾ����������������У�����ʧ��*/
    /*TODO2: �����½��ڵ�*/
    /*TODO3: �����������, �ж��Ƿ��г������������,
                   ������г������ȣ����ò�����*/
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
 �� �� ��  : VOS_PthEvt_WaitForDispatch
 ��������  : �¼����ж�ȡ���������¼�
 �������  : UINT32 uiQueID             
             VOS_DLIST_NODE_S *pstEvts  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��11��24��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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
    /*TODO1: ������ͷ�л�����stEvtList*/
    /*TODO: forѭ�����������е��¼��������¼���ע�ắ�����д���*/

    /*������Ҫ�򿪿��б��*/
    pstEvtQue->uiFreeFlag = VOS_FALSE;
    
#elif  VOS_PLAT_MAC
    
#elif VOS_PLAT_LINUX
    
#endif

    return VOS_OK;
}




