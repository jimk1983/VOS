/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_ent.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��10��23��
  ����޸�   :
  ��������   : VOS��ʹ�õĻ�����ʼ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��10��23��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/

#include <vos/vos_pub.h>


LONG VOS_EnvInit()
{    
    if ( VOS_ERR == VOS_MemMgmt_Init() )
    {
        VOS_Printf("vos Memory Mgmt init error!");
        return VOS_ERR;
    }

     if ( VOS_ERR == VOS_Log_Init() )
    {
        VOS_Printf("vos log init error!");
        VOS_MemMgmt_UnInit();
        return VOS_ERR;
    }
    
     return VOS_OK;
}


VOID VOS_EnvUnInit()
{
    VOS_MemMgmt_UnInit();
    VOS_Log_UnInit();
}

