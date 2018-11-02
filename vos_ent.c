/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_ent.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年10月23日
  最近修改   :
  功能描述   : VOS库使用的环境初始化
  函数列表   :
  修改历史   :
  1.日    期   : 2016年10月23日
    作    者   : jimk
    修改内容   : 创建文件

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

