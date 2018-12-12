/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_time.c
  �� �� ��   : ����
  ��    ��   : jiangkang
  ��������   : 2015��9��25��
  ����޸�   :
  ��������   : ϵͳʱ�������
  �����б�   :
              VOS_GetAbsTime
  �޸���ʷ   :
  1.��    ��   : 2015��9��25��
    ��    ��   : jiangkang
    �޸�����   : �����ļ�

******************************************************************************/
#include <vos_pub.h>



/*��ȡ����ʱ��*/
VOID VOS_GetAbsTime(VOS_ABS_TIME_S *pstAbsTime, UINT32 ulTimeMiliSec)
{
    ULONG ulSec     =   0;
    ULONG ulNSec    =   0;
    ULONG ulSum     =   0;
    struct timespec *pstAbsT = NULL;
    
    if ( NULL == pstAbsTime )
    {
        return;
    }

    pstAbsT = &(pstAbsTime->stAbsTime);
    #if 1
        struct timespec currentTime;
        (VOID)clock_gettime(CLOCK_MONOTONIC, &currentTime);
        
        ulNSec = (ULONG)currentTime.tv_nsec;
        ulSum = (ulTimeMiliSec / 1000);
        ulNSec += ulSum * 1000000;
        pstAbsT->tv_nsec = ulNSec % 1000000000;
        ulSec = (ULONG)currentTime.tv_sec;
        ulSec += ulTimeMiliSec / 1000;        
        ulSec += ulNSec /1000000000;
        pstAbsT->tv_sec = (LONG)ulSec;
    #else
        struct timeval currentTime;
        gettimeofday(&currentTime, NULL);
        ulNSec = (ULONG)currentTime.tv_usec * 1000;
        ulNSec += (ulTimeMiliSec%1000) * 1000000;
        pstAbsT->tv_nsec = ulNSec / 1000000000;
        ulSec = (ULONG)currentTime.tv_sec;
        ulSec += ulTimeMiliSec %1000;
        ulSec += ulNSec /1000000000;
        pstAbsT->tv_sec = (LONG)ulSec;
    #endif

    return;
}


VOID VOS_Sleep(ULONG ulSeconds)
{
    sleep(ulSeconds);

    return;
}

VOID VOS_MSleep(ULONG ulMSleep)
{
    usleep(ulMSleep*1000);
    return;
}

VOID VOS_USleep(ULONG ulUSleep)
{
    usleep(ulUSleep);
    return;
}


/*��ȡϵͳʱ��*/
VOID VOS_GetLocalSystemTime(VOS_LOCAL_SYSTEM_TIME_S *pstSysTime)
{
    struct tm *pstTime = NULL;
    struct timeval tv;;
    time_t  t=time(NULL);
    
    if ( NULL == pstSysTime )
    {
        return;
    }

    VOS_Mem_Zero((CHAR *)pstSysTime , sizeof(VOS_LOCAL_SYSTEM_TIME_S));

    /*��ȡ������,ʱ����*/
    pstTime  = localtime(&t);


    pstSysTime->uiYear       =  pstTime->tm_year + 1900;
    pstSysTime->uiMoth      =  pstTime->tm_mon + 1;
    pstSysTime->uiDay        =  pstTime->tm_mday;

    pstSysTime->uiHour      =  pstTime->tm_hour;
    pstSysTime->uiMinute   =  pstTime->tm_min;
    pstSysTime->uiSecond   =  pstTime->tm_sec;
    
    /*��ȡ����ʱ��*/
    gettimeofday(&tv, NULL);
    
    pstSysTime->uiMiliSec   =  tv.tv_usec;

    return;
}

