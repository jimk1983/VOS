/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_malloc.c
  版 本 号   : 初稿
  作    者   : jiangkang
  生成日期   : 2015年9月25日
  最近修改   :
  功能描述   : 系统内存操作的适配
  函数列表   :
              VOS_Free
              VOS_Malloc
              VOS_Mem_Copy
              VOS_Mem_Zero
  修改历史   :
  1.日    期   : 2015年9月25日
    作    者   : jiangkang
    修改内容   : 创建文件

******************************************************************************/
#include <vos/vos_pub.h>


/*申请内存*/
CHAR *VOS_Memory(ULONG ulSize)
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

/*内存清零*/
VOID VOS_Mem_Zero(CHAR *pcBuf, ULONG ulSize)
{
    if ( (0 == ulSize) || (NULL == pcBuf) )
    {
        return;
    }

    memset(pcBuf,0,ulSize);
    
    return;
}

/*内存拷贝*/
VOID VOS_Mem_Copy(CHAR *pcDst, CHAR *pcSrc,ULONG ulSize)
{
    if ( (NULL == pcDst) || (NULL == pcSrc) || (0 == ulSize) )
    {
        return;
    }
    
    memset(pcDst,0,ulSize);
    
    memcpy(pcDst,pcSrc,ulSize);

    return;
}

VOID VOS_Mem_Copy_S(CHAR *pcDst, ULONG ulMaxLen, CHAR *pcSrc,ULONG ulSize)
{
    if ( (NULL == pcDst) || (NULL == pcSrc) || (0 == ulSize) )
    {
        return;
    }

    if ( ulMaxLen < ulSize )
    {
        return;
    }
    
    memset(pcDst,0,ulSize);
    
    memcpy(pcDst,pcSrc,ulSize);

    return;
}


/*内存安全释放*/
VOID VOS_MemFree(CHAR *ucPtr)
{
    if ( NULL == ucPtr )
    {
        return;
    }
 
    free(ucPtr);
 
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
  1.日    期   : 2016年6月24日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG VOS_MemCmp(CHAR *pcMem1, CHAR *pcMem2, UINT32 uiSize)
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



