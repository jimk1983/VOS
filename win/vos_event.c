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

#include "Common/common.h"

/*****************************************************************************
 �� �� ��  : VOS_EventCreate
 ��������  : �����¼�	
 �������  : CHAR *pcName			---�¼�����  
             BOOL bManualReset		---�Ƿ��ֹ��Լ�Reset
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_EVENT_T VOS_EventCreate(const CHAR *pcName, BOOL bManualReset)
{
	VOS_EVENT_T hEvent = NULL;

#if VOS_PLAT_WIN32
	hEvent = CreateEventA(NULL, bManualReset, FALSE, pcName);
	if (NULL == hEvent)
	{
		return VOS_INVALID_HANDLE;
	}

	//ResetEvent(hEvent);
#elif VOS_PLAT_LINUX


#endif
	return hEvent;
}



/*****************************************************************************
 �� �� ��  : VOS_EventDelete
 ��������  : ɾ���¼�
 �������  : VOS_EVENT_T hEvent  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID VOS_EventDelete(VOS_EVENT_T hEvent)
{
	if (VOS_INVALID_HANDLE != hEvent)
	{
#if VOS_PLAT_WIN32
		CloseHandle(hEvent);
#elif VOS_PLAT_LINUX


#endif
	}
}

/*****************************************************************************
 �� �� ��  : VOS_EventNotify
 ��������  : �¼�֪ͨ
 �������  : VOS_EVENT_T hEvent  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID VOS_EventNotify(VOS_EVENT_T hEvent)
{
	if (VOS_INVALID_HANDLE != hEvent)
	{
#if VOS_PLAT_WIN32
		SetEvent(hEvent);
#elif VOS_PLAT_LINUX


#endif
	}
}

/*****************************************************************************
 �� �� ��  : VOS_EventReset
 ��������  : �����¼�
 �������  : VOS_EVENT_T hEvent  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��20��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID VOS_EventReset(VOS_EVENT_T hEvent)
{
    if ( VOS_INVALID_HANDLE != hEvent )
    {
       ResetEvent(hEvent);
    }
	
}
/*****************************************************************************
 �� �� ��  : VOS_EventOpen
 ��������  : �¼���
 �������  : char *pcName  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_EVENT_T VOS_EventOpen(CHAR *pcName)
{
	VOS_EVENT_T hEvent = NULL;
	if (NULL == pcName)
	{
		return NULL;
	}

#if VOS_PLAT_WIN32
	hEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, pcName);
	if (VOS_INVALID_HANDLE == hEvent)
	{
		return NULL;
	}
#elif VOS_PLAT_LINUX


#endif

	return hEvent;
}


/*****************************************************************************
 �� �� ��  : VOS_EventWaiting
 ��������  : �¼��ȴ�
 �������  : VOS_EVENT_T hEvent  
             UINT32 uiTimeout		---��ʱʱ�䣬�Ժ���Ϊ��λ
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32 VOS_EventWaiting(VOS_EVENT_T hEvent, UINT32 uiTimeout)
{
	if (VOS_INVALID_HANDLE == hEvent)
	{
		return VOS_ERR;
	}

#if VOS_PLAT_WIN32
	DWORD	dwRet = 0;
	DWORD   dwTimeout = 0;

	if (0 == uiTimeout)
	{
		dwTimeout = INFINITE;
	}
	else
	{
		dwTimeout = uiTimeout;
	}

	dwRet = WaitForSingleObject(hEvent, dwTimeout);
	switch (dwRet)
	{
		case WAIT_OBJECT_0:
            return VOS_OK;
		case WAIT_TIMEOUT:
			return VOS_SYS_TIMEOUT;
		default:
			break;
	}

#elif VOS_PLAT_LINUX


#endif
	return VOS_OK;
}