/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : vos_malloc.h
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
#ifndef _VOS_MALLOC_H_
#define _VOS_MALLOC_H_

/*得到最低字节位的值*/
#define MEM_GET_BYTE(x)     (*((BYTE *)(x)))
#define MEM_GET_INT32(x)    (*((INT32 *)(x)))


#define VOS_MALLOC(ulMid, ulSize)  VOS_MemCreate_X(ulMid, ulSize,__FILE__,__LINE__)

#define VOS_FREE(pcMemory) VOS_MemFree(pcMemory)

/*扩展*/
CHAR *VOS_MemCreate_X(ULONG ulMID, ULONG ulSize, const CHAR *pcFileName, ULONG ulLine);

/*内存清零*/
VOID VOS_Mem_Zero(CHAR *pcBuf, ULONG ulSize);

/*内存安全释放*/
VOID VOS_MemFree(VOID *ucPtr);

/*内存拷贝*/
VOID VOS_Mem_Copy(VOID *pcDst, const VOID *pcSrc,ULONG ulSize);

/*内存安全拷贝*/
VOID VOS_Mem_Copy_S(VOID *pcDst, ULONG ulMaxLen, const VOID *pcSrc,ULONG ulSize);

/*内存比较*/
LONG VOS_MemCmp(VOID *pcMem1, VOID *pcMem2, UINT32 uiSize);



#endif

