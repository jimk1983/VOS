/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : vos_time.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��10��17��
  ����޸�   :
  ��������   : ϵͳʱ���װ
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
    
#include "Common/common.h"

/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

#define VOS_TMSTR_LEN   32

/*****************************************************************************
 �� �� ��  : VOS_GetCurrentTime
 ��������  : ��ȡ��ǰϵͳʱ��, ֻ�ܾ�ȷ����
 �������  : CHAR *pcTimeStr  --ʱ����Ϣ����ʽ��2018-10-17 12:10:10��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_GetCurrentMillTime_S
 ��������  : ��ȡ��ǰ����ʱ��
 �������  : CHAR *pcTimeStr  
             INT32 iStrLen    
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_GetSystemTime
 ��������  : ��ȡϵͳʱ��
 �������  : UINT32 *puiYear        ��
             UINT32 *puiMouth   ��
             UINT32 *puiDay     ��
             UINT32 *puiHour    ʱ
             UINT32 *puiMiute   ��
             UINT32 *puiSecond  ��
             UINT32 *puiMilliS  ����
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
