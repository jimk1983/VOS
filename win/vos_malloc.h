/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : vos_malloc.h
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
#ifndef _VOS_MALLOC_H_
#define _VOS_MALLOC_H_

/*�õ�����ֽ�λ��ֵ*/
#define MEM_GET_BYTE(x)     (*((BYTE *)(x)))
#define MEM_GET_INT32(x)    (*((INT32 *)(x)))


#define VOS_MALLOC(ulMid, ulSize)  VOS_MemCreate_X(ulMid, ulSize,__FILE__,__LINE__)

#define VOS_FREE(pcMemory) VOS_MemFree(pcMemory)

/*��չ*/
CHAR *VOS_MemCreate_X(ULONG ulMID, ULONG ulSize, const CHAR *pcFileName, ULONG ulLine);

/*�ڴ�����*/
VOID VOS_Mem_Zero(CHAR *pcBuf, ULONG ulSize);

/*�ڴ氲ȫ�ͷ�*/
VOID VOS_MemFree(VOID *ucPtr);

/*�ڴ濽��*/
VOID VOS_Mem_Copy(VOID *pcDst, const VOID *pcSrc,ULONG ulSize);

/*�ڴ氲ȫ����*/
VOID VOS_Mem_Copy_S(VOID *pcDst, ULONG ulMaxLen, const VOID *pcSrc,ULONG ulSize);

/*�ڴ�Ƚ�*/
LONG VOS_MemCmp(VOID *pcMem1, VOID *pcMem2, UINT32 uiSize);



#endif

