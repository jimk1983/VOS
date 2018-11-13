/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_utils.c
  版 本 号   : 初稿
  作    者   : jiangkang
  生成日期   : 2015年9月25日
  最近修改   :
  功能描述   : 系统常见公共使用适配
  函数列表   :
                           
  修改历史   :
  1.日    期   : 2015年9月25日
    作    者   : jiangkang
    修改内容   : 创建文件

******************************************************************************/
#include <vos_pub.h>


/*将字符串跳过前面空格进行字符串转换为数字*/
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

/*将整形数转换为字符串*/
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

/*将十六数转换为字符串*/
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



/*大端返回1, 小端返回0*/
BOOL VOS_CpuIsBigEndian()
{
    /*小端存放:从低地址到高地址0x01, 0x00, 0x00, 0x00*/
    INT32 i = 0x00000001;
    /*取一个低字节*/
    CHAR c = *(CHAR *)&i;

    /*低地址字节, 为小端*/
    if ( 0x01 == c )
    {
        return VOS_FALSE;
    }

    return VOS_TRUE;
}

/*获取本线程ID号*/
ULONG VOS_GetSelfPId()
{
  ULONG ulpthreadId;

#if VOS_PLAT_LINUX
  ulpthreadId = pthread_self();
#endif
  
  return ulpthreadId;
}


/*返回一个16字节的随机数*/
USHORT VOS_Random16()
{
    USHORT usRandom = 0;  

#if VOS_PLAT_LINUX
    srand((UINT32)time(NULL));
    usRandom = rand() & 0x0000FFFF;
#endif

    
    return usRandom;
}

/*返回一个32字节的随机数*/
UINT32 VOS_Random32()
{
    UINT32 usRandom = 0;

#if VOS_PLAT_LINUX
    srand((UINT32)time(NULL));
    usRandom = rand() & 0xFFFFFFFF;
#endif
    
    return usRandom;
}

