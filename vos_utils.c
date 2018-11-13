/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_utils.c
  �� �� ��   : ����
  ��    ��   : jiangkang
  ��������   : 2015��9��25��
  ����޸�   :
  ��������   : ϵͳ��������ʹ������
  �����б�   :
                           
  �޸���ʷ   :
  1.��    ��   : 2015��9��25��
    ��    ��   : jiangkang
    �޸�����   : �����ļ�

******************************************************************************/
#include <vos_pub.h>


/*���ַ�������ǰ��ո�����ַ���ת��Ϊ����*/
UINT32 VOS_atoi(CHAR *pcStr)
{
    UINT32 i = VOS_INVALID_VAL32;
    
    if ( NULL == pcStr )
    {
        return i;
    }

    i = atoi(pcStr);
       
    return i;
}

/*��������ת��Ϊ�ַ���*/
VOID VOS_itoa(CHAR *pcStr, UINT32 iVal)
{
    CHAR acBuf[VOS_INT32STR_MAXLEN] = {0};

    if ( NULL == pcStr )
    {
        return;
    }

    #if VOS_PLAT_LINUX
    snprintf(acBuf, VOS_INT32STR_MAXLEN-1,"%d", iVal);
    #elif VOS_PLAT_WIN
    sprintf_s(acBuf, VOS_INT32STR_MAXLEN-1,"%d", iVal);
    #endif

    memcpy(pcStr, acBuf, strlen(acBuf));
    
    return;
}

/*��ʮ����ת��Ϊ�ַ���*/
VOID VOS_hextoa(CHAR *pcStr, CHAR iVal)
{
    CHAR acBuf[VOS_INT32STR_MAXLEN] = {0};

    if ( NULL == pcStr )
    {
        return;
    }

    #if VOS_PLAT_LINUX
    snprintf(acBuf, VOS_INT32STR_MAXLEN-1,"%02x", iVal);
    #elif VOS_PLAT_WIN
    sprintf_s(acBuf, VOS_INT32STR_MAXLEN-1,"%02x", iVal);
    #endif

    memcpy(pcStr, acBuf, strlen(acBuf));
    
    return;
}



/*��˷���1, С�˷���0*/
BOOL VOS_CpuIsBigEndian()
{
    /*С�˴��:�ӵ͵�ַ���ߵ�ַ0x01, 0x00, 0x00, 0x00*/
    INT32 i = 0x00000001;
    /*ȡһ�����ֽ�*/
    CHAR c = *(CHAR *)&i;

    /*�͵�ַ�ֽ�, ΪС��*/
    if ( 0x01 == c )
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

/*��ȡ���߳�ID��*/
ULONG VOS_GetSelfPId()
{
  ULONG ulpthreadId;

#if VOS_PLAT_LINUX
  ulpthreadId = pthread_self();
#endif
  
  return ulpthreadId;
}


/*����һ��16�ֽڵ������*/
USHORT VOS_Random16()
{
    USHORT usRandom = 0;  

#if VOS_PLAT_LINUX
    srand((UINT32)time(NULL));
    usRandom = rand() & 0x0000FFFF;
#endif

    
    return usRandom;
}

/*����һ��32�ֽڵ������*/
UINT32 VOS_Random32()
{
    UINT32 usRandom = 0;

#if VOS_PLAT_LINUX
    srand((UINT32)time(NULL));
    usRandom = rand() & 0xFFFFFFFF;
#endif
    
    return usRandom;
}

