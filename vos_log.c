/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : dbg_log.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年2月14日
  最近修改   :
  功能描述   : Debug相关简易版
  函数列表   :
              DebugOutput
              WriteToLog
  修改历史   :
  1.日    期   : 2016年2月14日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
#include <vos_pub.h>
#include <default_def.h>

VOS_RW_LOCK_S   stLogLock;

VOS_RW_LOCK_S   stRctLogLock;

/*****************************************************************************
 函 数 名  : VOS_PrintDebug
 功能描述  : 带日期的打印
 输入参数  : const CHAR *pcformat  
             ...                   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
*****************************************************************************/
VOID VOS_PrintDebug(const CHAR *pcformat, ...)
{
    va_list args_ptr;
    CHAR acbuf[VOS_MAXSTRLEN]= {0};
    VOS_LOCAL_SYSTEM_TIME_S stTime = {0};

    if(NULL == pcformat)
    {
        return;
    }

    VOS_GetLocalSystemTime(&stTime);

    va_start(args_ptr,pcformat);
#if VOS_PLAT_LINUX
    (VOID)vsnprintf(acbuf,VOS_MAXSTRLEN-1,pcformat,args_ptr);
#elif VOS_PLAT_WIN
    (VOID)vsprintf_s(acbuf, VOS_MAXSTRLEN-1, pcformat,args_ptr);
#endif
    va_end(args_ptr);

    printf("[%d:%d:%d %d] %s\n",stTime.uiHour, stTime.uiMinute, stTime.uiSecond, stTime.uiMiliSec, acbuf);

    return; 

}


/*不释放了*/
LONG VOS_Log_Init()
{
    if ( VOS_ERR == VOS_FileCreate((CHAR *)DEFAULT_LOGFILE) )
    {
        VOS_Printf("Default vos logfile[%s] create error!", DEFAULT_LOGFILE);
        return VOS_ERR;
    }
    
    if ( VOS_ERR == VOS_FileCreate((CHAR *)DEFAULT_RCTLOGFILE) )
    {
        VOS_Printf("Default rct logfile[%s] create error!", DEFAULT_LOGFILE);
        return VOS_ERR;
    }

    VOS_RWLOCK_INIT(stLogLock);

    VOS_RWLOCK_INIT(stRctLogLock);

    return VOS_OK;
}

VOID VOS_Log_UnInit()
{

    VOS_RWLOCK_FREE(stLogLock);

    VOS_RWLOCK_FREE(stRctLogLock);


    return;
}

VOID VOS_DebugTraceOutput(const CHAR *pcformat, ...)
{
    va_list args_ptr;
    CHAR acbuf[VOS_MAXSTRLEN]= {0};
    VOS_LOCAL_SYSTEM_TIME_S stTime = {0};

    if(NULL == pcformat)
    {
        return;
    }

    VOS_GetLocalSystemTime(&stTime);

    va_start(args_ptr,pcformat);
#if VOS_PLAT_LINUX
    (VOID)vsnprintf(acbuf,VOS_MAXSTRLEN-1,pcformat,args_ptr);
#elif VOS_PLAT_WIN
    (VOID)vsprintf_s(acbuf, VOS_MAXSTRLEN-1, pcformat,args_ptr);
#endif
    va_end(args_ptr);

    printf("[%d:%d:%d--%d]%s\n",stTime.uiHour, stTime.uiMinute, stTime.uiSecond, stTime.uiMiliSec,acbuf);

    return; 
}

/*以16进制打印内存*/
VOID VOS_DebugBufHexOutput(CHAR *pcHeadDescript, CHAR *pcBuf, LONG ulLen)
{
    UINT32 ulIndex = 0;
    UINT32 ulCount = 0;
    UCHAR *pcTmp = NULL;
    

    if ( NULL == pcBuf )
    {
        return;
    }

    printf("%s\r\n0x0000: ", pcHeadDescript);

    pcTmp = (UCHAR *)pcBuf;
    
    for(ulIndex=0; ulIndex<ulLen; ulIndex++)
    {
        printf("%02x", pcTmp[ulIndex]);
        
        if ( (ulIndex+1) %4 == 0 )
        {
            printf(" ");
        }

        if ( (ulIndex+1) %16 == 0 && ulIndex <ulLen-1)
        {
            ulCount++;
            printf("\r\n0x%04x: ", (ulCount*16));
        }
    }

    printf("\n");
    return; 
}


/*****************************************************************************
 函 数 名  : VOS_WriteSystemLog
 功能描述  : 文件日志
 输入参数  : const CHAR *pcformat  
             ...                   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月23日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_WriteSystemLog(const CHAR *pcformat,...)
{
    va_list args_ptr;
    CHAR acBuf[VOS_MAXSTRLEN]= {0};
    CHAR acLog[VOS_MAXSTRLEN]= {0};
    FILE *pFile = NULL;
    VOS_LOCAL_SYSTEM_TIME_S stTime = {0};
    
    if(NULL == pcformat)
    {
        return;
    }
    
    VOS_RWLOCK_LOCK(stLogLock);
    
    va_start(args_ptr,pcformat);
    #if VOS_PLAT_LINUX
    (VOID)vsnprintf(acBuf,VOS_MAXSTRLEN-1,pcformat,args_ptr);
    #elif VOS_PLAT_WIN
    (VOID)vsprintf_s(acBuf, VOS_MAXSTRLEN-1, pcformat,args_ptr);
    #endif
    va_end(args_ptr);

    VOS_GetLocalSystemTime(&stTime);
    
    (VOID)snprintf(acLog, VOS_MAXSTRLEN-1,"[%d:%d:%d %d] %s\n",
            stTime.uiHour, stTime.uiMinute, stTime.uiSecond, stTime.uiMiliSec, acBuf);
    
    pFile = fopen(DEFAULT_LOGFILE, "a+");
    fprintf(pFile, "%s\n", acLog);
    fclose(pFile);
    VOS_RWLOCK_LOCK(stLogLock);
    return;
}



/*****************************************************************************
 函 数 名  : VOS_WriteRctLog
 功能描述  : RCT的日志
 输入参数  : const CHAR *pcformat  
             ...                   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月24日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_WriteRctLog(const CHAR *pcformat,...)
{
    va_list args_ptr;
    CHAR acBuf[VOS_MAXSTRLEN]= {0};
    CHAR acLog[VOS_MAXSTRLEN]= {0};
    FILE *pFile = NULL;
    VOS_LOCAL_SYSTEM_TIME_S stTime = {0};
    
    if(NULL == pcformat)
    {
        return;
    }
    
    VOS_RWLOCK_LOCK(stRctLogLock);
    
    va_start(args_ptr,pcformat);
    #if VOS_PLAT_LINUX
    (VOID)vsnprintf(acBuf,VOS_MAXSTRLEN-1,pcformat,args_ptr);
    #elif VOS_PLAT_WIN
    (VOID)vsprintf_s(acBuf, VOS_MAXSTRLEN-1, pcformat,args_ptr);
    #endif
    va_end(args_ptr);

    VOS_GetLocalSystemTime(&stTime);
    
    (VOID)snprintf(acLog, VOS_MAXSTRLEN-1,"[%d:%d:%d %d] %s\n",
            stTime.uiHour, stTime.uiMinute, stTime.uiSecond, stTime.uiMiliSec, acBuf);
    
    pFile = fopen(DEFAULT_RCTLOGFILE, "a+");
    fprintf(pFile, "%s\n", acLog);
    fclose(pFile);
    VOS_RWLOCK_LOCK(stRctLogLock);
    return;
}

