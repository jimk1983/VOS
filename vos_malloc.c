/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_malloc.c
  �� �� ��   : ����
  ��    ��   : jiangkang
  ��������   : 2015��9��25��
  ����޸�   :
  ��������   : ϵͳ�ڴ����������
  �����б�   :
              VOS_Free
              VOS_Malloc
              VOS_Mem_Copy
              VOS_Mem_Zero
  �޸���ʷ   :
  1.��    ��   : 2015��9��25��
    ��    ��   : jiangkang
    �޸�����   : �����ļ�

******************************************************************************/
#include <vos/vos_pub.h>


/*�����ڴ�*/
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

/*�ڴ�����*/
VOID VOS_Mem_Zero(CHAR *pcBuf, ULONG ulSize)
{
    if ( (0 == ulSize) || (NULL == pcBuf) )
    {
        return;
    }

    memset(pcBuf,0,ulSize);
    
    return;
}

/*�ڴ濽��*/
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


/*�ڴ氲ȫ�ͷ�*/
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
  1.��    ��   : 2016��6��24��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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



