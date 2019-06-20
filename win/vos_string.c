/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : vos_string.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年10月17日
  最近修改   :
  功能描述   : 字符串操作
  函数列表   :
  修改历史   :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/

#include "Common/common.h"



/*****************************************************************************
 函 数 名  : VOS_Snprintf
 功能描述  : 字符串拼接
 输入参数  : CHAR *pcBuf      
             UINT32 ulMaxLen  
             CHAR *pcFormat   
             ...              
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_Snprintf(CHAR *pcBuf, INT32 ulMaxLen, const CHAR *pcFormat,...)
{
    va_list args_ptr;
    INT32 ulRet = 0;    
    
    if( NULL == pcBuf || NULL == pcFormat )
    { 
        return 0;
    }

    va_start(args_ptr, pcFormat);
    
    #if VOS_PLAT_WIN32
	
	ulRet = _vscprintf_l(pcFormat, 0, args_ptr) + 1;
	if (ulRet >= ulMaxLen-1 )
	{
		return 0;
	}

    ulRet = vsprintf_s(pcBuf, ulMaxLen-1,pcFormat,args_ptr);
    #elif VOS_PLAT_LINUX
    ulRet = vsnprintf(pcBuf, ulMaxLen-1,pcFormat,args_ptr);
    #elif VOS_PLAT_MAC
    ulRet = vsnprintf(pcBuf, ulMaxLen-1,pcFormat,args_ptr);
    #endif
    va_end(args_ptr);
   
    return ulRet;
}




/*****************************************************************************
 函 数 名  : VOS_StrCpy_S
 功能描述  : 字符串拷贝
 输入参数  : CHAR *pcDst     
             ULONG ulMaxLen  
             CHAR *pcSrc     
 输出参数  : 无
 返 回 值  : CHAR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月31日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_StrCpy_S(CHAR *pcDst, ULONG ulMaxLen, CHAR *pcSrc)
{
    if ( NULL == pcDst 
        || NULL == pcSrc )
    {
        return;
    }

    strcpy_s(pcDst, ulMaxLen-1, pcSrc);
        
    return;
}








