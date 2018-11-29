/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : dbg_log.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��2��14��
  ����޸�   :
  ��������   : Debug��ؼ��װ�
  �����б�   :
              DebugOutput
              WriteToLog
  �޸���ʷ   :
  1.��    ��   : 2016��2��14��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#include <vos_pub.h>
#include <default_def.h>

VOS_RW_LOCK_S   stLogLock;

VOS_RW_LOCK_S   stRctLogLock;

/*****************************************************************************
 �� �� ��  : VOS_PrintDebug
 ��������  : �����ڵĴ�ӡ
 �������  : const CHAR *pcformat  
             ...                   
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
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


/*���ͷ���*/
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

/*��16���ƴ�ӡ�ڴ�*/
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
 �� �� ��  : VOS_WriteSystemLog
 ��������  : �ļ���־
 �������  : const CHAR *pcformat  
             ...                   
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��23��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_WriteRctLog
 ��������  : RCT����־
 �������  : const CHAR *pcformat  
             ...                   
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��24��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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

