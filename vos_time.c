/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_time.c
  版 本 号   : 初稿
  作    者   : jiangkang
  生成日期   : 2015年9月25日
  最近修改   :
  功能描述   : 系统时间的适配
  函数列表   :
              VOS_GetAbsTime
  修改历史   :
  1.日    期   : 2015年9月25日
    作    者   : jiangkang
    修改内容   : 创建文件

******************************************************************************/
#include <vos_pub.h>



/*获取绝对时间*/
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


/*获取系统时间*/
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

    /*获取年月日,时分秒*/
    pstTime  = localtime(&t);


    pstSysTime->uiYear       =  pstTime->tm_year + 1900;
    pstSysTime->uiMoth      =  pstTime->tm_mon + 1;
    pstSysTime->uiDay        =  pstTime->tm_mday;

    pstSysTime->uiHour      =  pstTime->tm_hour;
    pstSysTime->uiMinute   =  pstTime->tm_min;
    pstSysTime->uiSecond   =  pstTime->tm_sec;
    
    /*获取毫秒时间*/
    gettimeofday(&tv, NULL);
    
    pstSysTime->uiMiliSec   =  tv.tv_usec;

    return;
}

