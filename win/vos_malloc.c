/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : vos_malloc.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��10��17��
  ����޸�   :
  ��������   : �ڴ����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/



#include "Common/common.h"

/*�ڴ����*/
volatile LONG  g_ulMallocNums = 0;

/*****************************************************************************
 �� �� ��  : VOS_MemMalloc
 ��������  : �����ڴ�
 �������  : ULONG ulSize  
 �������  : ��
 �� �� ֵ  : CHAR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_Mem_Zero
 ��������  : �ڴ�����
 �������  : CHAR *pcBuf   
             ULONG ulSize  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_Mem_Copy
 ��������  : �ڴ濽��
 �������  : CHAR *pcDst   
             CHAR *pcSrc   
             ULONG ulSize  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_Mem_Copy_S
 ��������  : ��ȫ����
 �������  : CHAR *pcDst     
             ULONG ulMaxLen  
             CHAR *pcSrc     
             ULONG ulSize    
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_MemFree
 ��������  : �ڴ氲ȫ�ͷ�
 �������  : CHAR *ucPtr  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_MemCmp
 ��������  : �ڴ�Ƚ�
 �������  : CHAR *pcMem1   
             CHAR *pcMem2   
             UINT32 uiSize  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_MemCreate_X
 ��������  : �����ڴ�
 �������  : ULONG ulSize           ----�����ڴ��С
 �������  : ��
 �� �� ֵ  : ��ȷ��������ĵ�ַ
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
















