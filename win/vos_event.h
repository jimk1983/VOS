/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : vos_event.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��10��17��
  ����޸�   :
  ��������   : �¼�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef _VOS_EVENT_H_
#define _VOS_EVENT_H_

#if VOS_PLAT_WIN32
/*�¼����*/
typedef HANDLE VOS_EVENT_T;
#elif VOS_PLAT_LINUX

#endif

VOS_EVENT_T VOS_EventCreate(const CHAR *pcName, BOOL bAuto);

VOS_EVENT_T VOS_EventOpen(CHAR *pcName);

VOID VOS_EventDelete(VOS_EVENT_T hEvent);

VOID VOS_EventReset(VOS_EVENT_T hEvent);

VOID VOS_EventNotify(VOS_EVENT_T hEvent);

INT32 VOS_EventWaiting(VOS_EVENT_T hEvent, UINT32 uiTimeout);

#endif


