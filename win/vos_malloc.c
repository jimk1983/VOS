/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : vos_malloc.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年10月17日
  最近修改   :
  功能描述   : 内存操作
  函数列表   :
  修改历史   :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/



#include "Common/common.h"

/*内存分配*/
volatile LONG  g_ulMallocNums = 0;

/*****************************************************************************
 函 数 名  : VOS_MemMalloc
 功能描述  : 申请内存
 输入参数  : ULONG ulSize  
 输出参数  : 无
 返 回 值  : CHAR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
CHAR *VOS_MemMalloc(ULONG ulSize)
{
    CHAR *ptr = NULL;

    if ( ulSize == 0 )
    {
        return NULL;
    }
    
    ptr = (CHAR *)malloc(sizeof(CHAR) * ulSize);
    if (NULL != ptr)
    {
       return ptr;
    }
    
    memset(ptr, 0, ulSize);
    
    return NULL;
}

/*****************************************************************************
 函 数 名  : VOS_Mem_Zero
 功能描述  : 内存清零
 输入参数  : CHAR *pcBuf   
             ULONG ulSize  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_Mem_Zero(CHAR *pcBuf, ULONG ulSize)
{
    if ( (0 == ulSize) || (NULL == pcBuf) )
    {
        return;
    }

    memset(pcBuf,0,ulSize);
    
    return;
}


/*****************************************************************************
 函 数 名  : VOS_Mem_Copy
 功能描述  : 内存拷贝
 输入参数  : CHAR *pcDst   
             CHAR *pcSrc   
             ULONG ulSize  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_Mem_Copy(VOID *pcDst, const VOID *pcSrc,ULONG ulSize)
{
    if ( (NULL == pcDst) || (NULL == pcSrc) || (0 == ulSize) )
    {
        return;
    }
    
    memset(pcDst,0,ulSize);
    
    memcpy(pcDst,pcSrc,ulSize);

    return;
}


/*****************************************************************************
 函 数 名  : VOS_Mem_Copy_S
 功能描述  : 安全拷贝
 输入参数  : CHAR *pcDst     
             ULONG ulMaxLen  
             CHAR *pcSrc     
             ULONG ulSize    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_Mem_Copy_S(VOID *pcDst, ULONG ulMaxLen, const VOID *pcSrc,ULONG ulSize)
{
    if ( (NULL == pcDst) || (NULL == pcSrc) || (0 == ulSize) )
    {
        return;
    }

    if ( ulMaxLen < ulSize )
    {
        return;
    }
    
    memset(pcDst,0,ulMaxLen);
    
    memcpy(pcDst,pcSrc,ulSize);

    return;
}

/*****************************************************************************
 函 数 名  : VOS_MemFree
 功能描述  : 内存安全释放
 输入参数  : CHAR *ucPtr  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_MemFree(VOID *ucPtr)
{
    if ( NULL == ucPtr )
    {
        return;
    }
    
    free(ucPtr);

    VOS_InterlockedDecrement(&g_ulMallocNums);

    CLOG_writelog_level("VOS", CLOG_LEVEL_INFO, "#####>>>>VOS_MemFree nums=%d", g_ulMallocNums);
    return;
}

/*****************************************************************************
 函 数 名  : VOS_MemCmp
 功能描述  : 内存比较
 输入参数  : CHAR *pcMem1   
             CHAR *pcMem2   
             UINT32 uiSize  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
LONG VOS_MemCmp(VOID *pcMem1, VOID *pcMem2, UINT32 uiSize)
{
    LONG lRet = VOS_ERR;

    if ( NULL == pcMem1
        || NULL == pcMem2 )
    {
        return VOS_ERR;
    }

    lRet = memcmp(pcMem1, pcMem2, uiSize);
    if ( 0 != lRet )
    {
        return VOS_ERR;
    }
    
    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : VOS_MemCreate_X
 功能描述  : 申请内存
 输入参数  : ULONG ulSize           ----申请内存大小
 输出参数  : 无
 返 回 值  : 正确返回申请的地址
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
CHAR *VOS_MemCreate_X(ULONG ulMID, ULONG ulSize, const CHAR *pcFileName, ULONG ulLine)
{
    CHAR *ptr = NULL;

    if ( ulSize == 0
        || 0xFFFFFFFF == ulMID
        || NULL == pcFileName
        || 0 == ulLine )
    {
        return NULL;
    }


    VOS_InterlockedIncrement(&g_ulMallocNums);

    CLOG_writelog_level("VOS", CLOG_LEVEL_INFO, "<<<<<#####VOS_MemCreate nums=%d, [%s, %d]", g_ulMallocNums, pcFileName, ulLine);

    ptr = (CHAR *)malloc(sizeof(CHAR) * ulSize);
    if (NULL != ptr)
    {
        memset(ptr, 0, ulSize);
        return ptr;
    }

    return NULL;
}
















