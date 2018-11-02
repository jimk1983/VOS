/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_sys.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��5��23��
  ����޸�   :
  ��������   : ϵͳ���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��5��23��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#include <vos/vos_pub.h>

VOS_THREAD_T VOS_Thread_Create(CHAR *pcName, VOID(* pfvThread)(VOID *arg), VOID *pvArg, INT32 iStacksize, INT32 iPrio)
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
#elif VOS_PLAT_WIN
    handle = (VOS_THREAD_T)_beginthreadex( 
        NULL,                            // security attributes
        iStacksize,                       // stack size  
        (LPTHREAD_START_ROUTINE)pfvThread,  // thread function name
        pvArg,                             // argument to thread function 
        0,                               // creation flags 
        NULL);                           // returns the thread identifier 

    if (handle == NULL) 
    {
        VOS_Printf("CreateThread failed with %d.\n", GetLastError()); 
        return 0;
    }
    if (!SetThreadPriority(handle, iPrio))
        VOS_Printf("SetThreadPriority failed with %d\n", GetLastError());
    
#elif VOS_PLAT_MAC

#endif

  return handle;
}


LONG VOS_GetLastError()
{
    return errno;
}

CHAR *VOS_GetStrLastError()
{
    return strerror(errno);
}

