/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : vos_time.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年10月17日
  最近修改   :
  功能描述   : 系统时间封装
  函数列表   :
  修改历史   :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
    
#include "Common/common.h"

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

#define VOS_TMSTR_LEN   32

/*****************************************************************************
 函 数 名  : VOS_GetCurrentTime
 功能描述  : 获取当前系统时间, 只能精确到秒
 输入参数  : CHAR *pcTimeStr  --时间信息，格式”2018-10-17 12:10:10“
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_GetCurrentSecTime(CHAR *pcTimeStr, INT32 iStrLen)
{
#if VOS_PLAT_WIN32
    struct tm cur_time;
	time_t time_seconds;

    time(&time_seconds);
    localtime_s(&cur_time, &time_seconds);

    memset(pcTimeStr, 0, iStrLen);
    sprintf_s(pcTimeStr, iStrLen-1,"%04d-%02d-%02d %02d:%02d:%02d", 
        cur_time.tm_year + 1900,
		cur_time.tm_mon + 1,
		cur_time.tm_mday,
		cur_time.tm_hour,
		cur_time.tm_min,
		cur_time.tm_sec);
    
#elif VOS_PLAT_LINUX


#endif
    
    return VOS_OK;
}


/*****************************************************************************
 函 数 名  : VOS_GetCurrentMillTime_S
 功能描述  : 获取当前毫秒时间
 输入参数  : CHAR *pcTimeStr  
             INT32 iStrLen    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_GetCurrentMillTime(CHAR *pcMTimeStr, INT32 iStrLen)
{
#if VOS_PLAT_WIN32
    SYSTEMTIME currentTime;
    //GetSystemTime(&currentTime);
    GetLocalTime(&currentTime);

    memset(pcMTimeStr, 0, iStrLen);
    sprintf_s(pcMTimeStr, iStrLen-1,"%04d-%02d-%02d %02d:%02d:%02d %03d", 
        currentTime.wYear,
        currentTime.wMonth,
        currentTime.wDay,
        currentTime.wHour,
        currentTime.wMinute,
        currentTime.wSecond,
        currentTime.wMilliseconds);
    
        //currentTime.wDayOfWeek
    
#elif VOS_PLAT_LINUX


#endif

    return VOS_OK;
}


/*****************************************************************************
 函 数 名  : VOS_GetSystemTime
 功能描述  : 获取系统时间
 输入参数  : UINT32 *puiYear        年
             UINT32 *puiMouth   月
             UINT32 *puiDay     日
             UINT32 *puiHour    时
             UINT32 *puiMiute   分
             UINT32 *puiSecond  秒
             UINT32 *puiMilliS  毫秒
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_GetSystemTime(UINT32 *puiYear, 
    UINT32 *puiMonth, UINT32 *puiDay, 
    UINT32 *puiHour, UINT32 *puiMinute, 
    UINT32 *puiSecond, 
    UINT32 *puiMilliS)
{
#if VOS_PLAT_WIN32
	SYSTEMTIME currentTime;
	GetLocalTime(&currentTime);

	if (NULL != puiYear )
	{
		*puiYear = currentTime.wYear;
	}

	if (NULL != puiMonth)
	{
		*puiMonth = currentTime.wMonth;
	}

	if (NULL != puiDay)
	{
		*puiDay = currentTime.wDay;
	}

	if (NULL != puiHour)
	{
		*puiHour = currentTime.wHour;
	}

	if (NULL != puiMinute)
	{
		*puiMinute = currentTime.wMinute;
	}

	if (NULL != puiSecond)
	{
		*puiSecond = currentTime.wSecond;
	}

	if (NULL != puiMilliS)
	{
		*puiMilliS = currentTime.wMilliseconds;
	}

#elif VOS_PLAT_LINUX


#endif

	return VOS_OK;
}



unsigned long VOS_GetTickCount()
{
    unsigned long ulRet = 0;
#if VOS_PLAT_WIN32
    ulRet = GetTickCount();
#elif VOS_PLAT_LINUX


#endif
    
    return ulRet;
}
