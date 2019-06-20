/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : vos_thread.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��10��16��
  ����޸�   :
  ��������   : ���̴߳�����ϵͳ����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��10��16��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/

#include "Common/common.h"

/*****************************************************************************
 �� �� ��  : VOS_Thread_Create
 ��������  : �̴߳���
 �������  : CHAR *pcName                       --������������, ����ΪNULL
           VOID(* pfvThread)(VOID *arg)     --�̴߳�����
           VOID *pvArg                      --�̲߳���
           INT32 iStacksize                 --�����̵߳�ջ��С��Ĭ����д0
           INT32 iPrio                      --�����̵߳����ȼ���Ĭ����д0
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��16��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
VOS_THREAD_T VOS_Thread_Create(const char *pcName, VOID(* pfvThread)(VOID *arg), VOID *pvArg, INT32 iStacksize, INT32 iPrio)
{
    VOS_THREAD_T handle = 0 ;
	
#if VOS_PLAT_LINUX
    LONG lRet = 0;
    lRet = pthread_create(&handle, NULL, (VOID *)pfvThread, pvArg);
    if ( 0 != lRet )
    {
        VOS_Printf("CreateThread failed!\n"); 
        return 0;
    }
#elif VOS_PLAT_WIN32
    handle = (VOS_THREAD_T)_beginthreadex(
        NULL,                            // security attributes
        iStacksize,                       // stack size  
        (_beginthreadex_proc_type)pfvThread,  // thread function name
        pvArg,                             // argument to thread function 
        0,                               // creation flags 
        NULL);                           // returns the thread identifier 

    if (handle == NULL) 
    {
        printf("CreateThread failed with %d.\n", GetLastError()); 
        return VOS_INVALID_HANDLE;
    }
    if (!SetThreadPriority(handle, iPrio))
		printf("SetThreadPriority failed with %d\n", GetLastError());
    
#elif VOS_PLAT_MAC

#endif

	UNREFERENCED_PARAMETER(pcName);
	return handle;
}

/*****************************************************************************
 �� �� ��  : VOS_GetLastError
 ��������  : ��ȡ���յĴ�����
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��16��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VOS_GetLastError()
{ 
#if VOS_PLAT_LINUX
    return errno;
#elif VOS_PLAT_WIN32
    return GetLastError();
#endif
}




