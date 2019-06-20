/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : vos_utils.h
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年10月17日
  最近修改   :
  功能描述   : VOS内部使用的Utls函数
  函数列表   :
  修改历史   :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/
#ifndef _VOS_UTILS_H_
#define _VOS_UTILS_H_
/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/
INT32 VOS_UTLS_GetCurrentDirectory(char *pcCurDir);

INT32 VOS_UTILS_GenUUid(CHAR *pcUUid);

INT32 VOS_UTILS_GenUUidData(CHAR *pcUUid);


#if VOS_PLAT_WIN32

INT32 VOS_UTILS_AscIItoUnicode(CHAR *pcStr, WCHAR *plwstr);

INT32 VOS_UTILS_UnicodeToAscII(WCHAR *plwstr, CHAR *pcStr);

#endif
/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/


#endif
