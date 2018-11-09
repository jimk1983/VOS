/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_string.c
  版 本 号   : 初稿
  作    者   : jiangkang
  生成日期   : 2015年9月25日
  最近修改   :
  功能描述   : 系统字符串的操作适配
  函数列表   :
                           
  修改历史   :
  1.日    期   : 2015年9月25日
    作    者   : jiangkang
    修改内容   : 创建文件

******************************************************************************/
#include <vos_pub.h>

/*计算字符串长度*/
INT32 VOS_StrLen(CHAR *pcString)
{
    INT32 ulStrlen =0;
    
    if ( NULL == pcString )
    {
        return ulStrlen;
    }

    ulStrlen = strlen(pcString);

    return ulStrlen;
}

/*比较两个字符串是否相同,相同返回0*/
INT32 VOS_StrCmp(CHAR *pcString1,CHAR *pcString2)
{
    INT32 lRet = -1;

    if ( NULL == pcString1 
        || NULL == pcString2 )
    {
        return lRet;
    }

    lRet = strcmp((CHAR *)pcString1,(CHAR *)pcString2);

    return lRet;
}

CHAR *VOS_StrCpy(CHAR *pcString1,CHAR *pcString2)
{
    if ( NULL == pcString1 
        || NULL == pcString2 )
    {
        return NULL;
    }

    return strcpy(pcString1,pcString2);
}

CHAR *VOS_StrCpy_S(CHAR *pcDst, ULONG ulMaxLen, CHAR *pcSrc)
{
    if ( NULL == pcDst 
        || NULL == pcSrc )
    {
        return NULL;
    }

    if ( strlen(pcSrc) > ulMaxLen )
    {
        return NULL;
    }

    return strcpy(pcDst,pcSrc);
}

CHAR *VOS_StrCat(CHAR *dest, const CHAR *src)
{
    CHAR *tmp = dest;
    
    while(*dest)dest++;
    
    while((*dest++ = *src++) != '\0');

    return tmp;
}


CHAR *VOS_StrNCat(CHAR *dest, const CHAR *src, INT32 iCount)
{
    CHAR *tmp = dest;

    if (iCount) 
    {
        while(*dest)dest++;
        
        while ((*dest++ = *src++) != 0) 
        {
            if (--iCount == 0)
            {
                *dest = '\0';
                break;
            }
        }
    }
    return tmp;
}


CHAR *VOS_StrNCpy_S(CHAR *pcDst, ULONG ulMaxLen,CHAR *pcSrc, ULONG ulLen)
{
    if ( NULL == pcDst
        || NULL == pcSrc )
    {
        return NULL;
    }

    if ( ulLen > ulMaxLen )
    {
        return NULL;
    }

    return strncpy(pcDst, pcSrc, ulLen);
}

/*打印函数, 限制字符串最大长度为16K*/
VOID VOS_Printf(CHAR *pcformat,...)
{
    va_list args_ptr;
    CHAR acbuf[VOS_MAXSTRLEN]= {0};   
    
    if(NULL == pcformat)
    {
        return;
    }
    
    va_start(args_ptr,pcformat);
    #if VOS_PLAT_LINUX
    (VOID)vsnprintf(acbuf,VOS_MAXSTRLEN-1,pcformat,args_ptr);
    #elif VOS_PLAT_WIN
    (VOID)vsprintf_s(acbuf, VOS_MAXSTRLEN-1, pcformat,args_ptr);
    #endif
    va_end(args_ptr);
    
    printf("%s\n", acbuf);     

    return;
}

/*拼接字符串*/
INT32 VOS_Snprintf(CHAR *pcBuf, UINT32 ulMaxLen, CHAR *pcFormat,...)
{
    va_list args_ptr;
    INT32 ulRet = 0;    
    
    if( NULL == pcBuf || NULL == pcFormat )
    { 
        return 0;
    }

    if ( VOS_MAXSTRLEN < ulMaxLen )
    {
        return 0;
    }

    va_start(args_ptr, pcFormat);
    
    #if VOS_PLAT_WIN 
    ulRet = vsprintf_s(pcBuf, ulMaxLen-1,pcFormat,args_ptr);
    #elif VOS_PLAT_LINUX
    ulRet = vsnprintf(pcBuf, ulMaxLen-1,pcFormat,args_ptr);
    #elif VOS_PLAT_MAC
    ulRet = vsnprintf(pcBuf, ulMaxLen-1,pcFormat,args_ptr);
    #endif
    va_end(args_ptr);
   
    return ulRet;
}

/* 从pcStr1中寻找pcStr2 */
CHAR *VOS_StrStr(const CHAR *pcStr1, const CHAR *pcStr2)
{
    CHAR *pcStr = NULL;

    if ( NULL == pcStr1 || NULL == pcStr2 )
    {
        return NULL;
    }

    pcStr = strstr(pcStr1, pcStr2);
    
    return pcStr;
}

/*查找最后一个字符的位置*/
CHAR *VOS_Strrchr(const CHAR *pcString, const CHAR chEndChr)
{
    CHAR *pcStr = NULL;

    if ( NULL == pcString  )
    {
        return NULL;
    }

    pcStr = strrchr((CHAR *)pcString, chEndChr);
    
    return pcStr;
}

/*查找首次出现的位置*/
CHAR *VOS_Strchr(const CHAR *pcString, const CHAR chStartChr)
{
    CHAR *pcStr = NULL;

    if ( NULL == pcString  )
    {
        return NULL;
    }

    pcStr = strchr(pcString, chStartChr);
    
    return pcStr;
}



