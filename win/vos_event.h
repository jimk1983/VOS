/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : vos_event.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年10月17日
  最近修改   :
  功能描述   : 事件
  函数列表   :
  修改历史   :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/

#ifndef _VOS_EVENT_H_
#define _VOS_EVENT_H_

#if VOS_PLAT_WIN32
/*事件句柄*/
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


