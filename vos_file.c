/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_file.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年10月22日
  最近修改   :
  功能描述   : 判断目录是否存在
  函数列表   :
              VOS_DirIsExist
  修改历史   :
  1.日    期   : 2016年10月22日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
#include <vos_pub.h>

/*LINUX下的errno含义*/
#define VOS_ERRNO_FILEORDIRNOTEXIST     2               /*创建的目录或者文件有级数不存在，无法创建*/
#define VOS_ERRNO_FILEISEXIST                   17             /*该文件已经存在*/

/*检查文件是否存在*/
#define VOS_ERR_ACCESS_ISEXIST                0

/*设置权限*/
#define VOS_FILEPRIV_ALL                            0777
/*****************************************************************************
 函 数 名  : VOS_DirIsExist
 功能描述  : 判断目录是否存在
 输入参数  : const CHAR *pcDirPath  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月22日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG VOS_DirIsExist(const CHAR *pcDirPath)
{
    if ( NULL == pcDirPath )    
    {
        VOS_Printf("VOS_DirIsExist param error!");
        return VOS_ERR;
    }
#if VOS_PLAT_LINUX

    DIR    *dir = NULL;

    dir = opendir(pcDirPath);
    if ( dir == NULL  )
    {   
        //VOS_Printf("open dir=[%s] error,dir is not exist!", pcDirPath);
        return VOS_ERR;
    }
    closedir(dir);
#endif
    /*打开成功表示存在*/
    return VOS_OK;
}


/*****************************************************************************
 函 数 名  : VOS_DirGetCurrentPath
 功能描述  : 获取当前的目录
 输入参数  : CHAR *pcDirPath  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月9日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
LONG VOS_DirGetCurrentPath(CHAR *pcDirPath, INT32 iMaxLen)
{
    INT32 iRet  = 0;
    INT32 index = 0;
    
#if VOS_PLAT_LINUX
    iRet = readlink("/proc/self/exe", pcDirPath, iMaxLen);
    if (iRet < 0 || iRet >= iMaxLen)
    {
        return VOS_ERR;
    }

    //获取当前目录绝对路径，即去掉程序名
    for (index = iRet; index >=0; --index)
    {
        if ( pcDirPath[index] == '/')
        {
            pcDirPath[index+1] = '\0';

            break;
        }
    }
#endif

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : VOS_DirsCreate
 功能描述  : 创建多级目录
 输入参数  : CHAR *pcDirPath  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月22日
    作    者   : jimk
    修改内容   : 新生成函数

int   access(const   char   *filename,   int   amode); 
amode参数为0时表示检查文件的存在性，如果文件存在，返回0，不存在，返回-1。 
这个函数还可以检查其它文件属性： 
06     检查读写权限 
04     检查读权限 
02     检查写权限 
01     检查执行权限 
00     检查文件的存在性
而这个就算这个文件没有读权限，也可以判断这个文件存在于否
存在返回0，不存在返回-1

*****************************************************************************/
LONG VOS_DirsCreate(CHAR *pcDirPath)
{
    INT32 iPos = 0;
    INT32 iLen = 0;
    INT32 iRet = 0;
    CHAR acTmp[VOS_FILENAME_LEN + 1]={0};
    
    if ( NULL == pcDirPath )    
    {
        VOS_Printf("VOS_DirsCreate param error!");
        return VOS_ERR;
    }
#if VOS_PLAT_LINUX
    (VOID)VOS_StrCpy_S(acTmp, VOS_FILENAME_LEN, pcDirPath);
    iLen = VOS_StrLen(pcDirPath);

    /*从1开始，0为根目录的'/' */
    for ( iPos = 1; iPos < iLen; iPos++)
    {
        if (acTmp[iPos] == '/' )
        {
            acTmp[iPos] = '\0';
            
            if ( VOS_DirIsExist(acTmp) != VOS_OK )
            {
                /*创建并设置权限*/
                iRet = mkdir(acTmp, VOS_FILEPRIV_ALL);
                if ( 0 != iRet )
                {
                    VOS_Printf("mkdir error!");
                    return VOS_ERR;
                }
                else
                {
                    acTmp[iPos] = '/';
                }
            }
            else
            {
                acTmp[iPos] = '/';
            }
        }
    }

    if ( iLen > 0 
        && (VOS_DirIsExist(acTmp) != VOS_OK)  )
    {
        iRet = mkdir(acTmp, VOS_FILEPRIV_ALL);
        if ( 0 != iRet )
        {
            VOS_Printf("mkdir last error!");
            return VOS_ERR;
        }
    }
    
#endif
    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : VOS_DIrsCurrentRemove
 功能描述  : 删除本目录的所有文件夹和文件
 输入参数  : CHAR *pcDir  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月23日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG VOS_DirsCurrentRemove(CHAR *pcDir)
{
#if VOS_PLAT_LINUX

    DIR               *pstCurDir = opendir(".");
    struct dirent  *pstDirEnt  = NULL;
    struct stat     stStat = {0};
    
    if ( NULL == pstCurDir
        || NULL == pcDir )
    {
        return VOS_ERR;
    }

    /*遍历当前所有目录*/
    while ((pstDirEnt = readdir(pstCurDir)) != NULL)
    {
        stat(pstDirEnt->d_name, &stStat);
        
        if (strcmp(pstDirEnt->d_name, ".") == 0 || strcmp(pstDirEnt->d_name, "..") == 0)
        {
            continue;
        }
        
        if (S_ISDIR(stStat.st_mode))
        {   
            /*循环删除*/
            chdir(pstDirEnt->d_name);
            VOS_DirsCurrentRemove(pstDirEnt->d_name);
            chdir("..");
        }
        /*一律删除*/
        remove(pstDirEnt->d_name);
    }
    closedir(pstCurDir);
    
#endif
    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : VOS_DirsRemove
 功能描述  : 删除一个目录，包含该目录下的所有文件和文件夹
 输入参数  : CHAR *pcDirPath  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月23日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG VOS_DirsRemove(CHAR *pcDirPath)
{
    CHAR  acOldPath[VOS_DIRSNAME_LEN] = {0};
 #if VOS_PLAT_LINUX
    if ( NULL == pcDirPath )
    {
        return VOS_ERR;
    }
 
    getcwd(acOldPath, VOS_DIRSNAME_LEN);

    /*切换到删除的目录*/
    if (chdir(pcDirPath) == -1)
    {
        return VOS_ERR;
    }
 
    (VOID)VOS_DirsCurrentRemove(pcDirPath);
    /*切换回工作目录*/
    chdir(acOldPath);
 
    /*
    如果你想删除该目录本身的话,取消本注释
    unlink(acOldPath); 
    */
#endif
    
    return VOS_OK;
}


/*需要通过后面的删除*/
VOS_DIRS_FILESINFO_S *VOS_DirsGetFilesInfo(CHAR *pcDir)
{
    VOS_DIRS_FILESINFO_S *pstDirFileInfoHead= NULL;
    VOS_DIRS_FILESINFO_S *pstDirFileInfoTmp = NULL;
    VOS_DIRS_FILESINFO_S *pstDirFileInfoNext = NULL;
    CHAR  acOldPath[VOS_DIRSNAME_LEN] = {0};
    DIR               *pstCurDir = NULL;
    struct dirent  *pstDirEnt  = NULL;
    struct stat     stStat = {0};
    if ( NULL  == pcDir )
    {
        return NULL;
    }
    
#if VOS_PLAT_LINUX
    /*获取当前目录*/
    getcwd(acOldPath, VOS_DIRSNAME_LEN);

    /*切换到遍历的目录*/
    if (chdir(pcDir) == -1)
    {
        return NULL;
    }

    pstCurDir = opendir(".");
    if ( NULL == pstCurDir )
    {
        return NULL;
    }

    pstDirFileInfoHead = (VOS_DIRS_FILESINFO_S *)VOS_Malloc(VOS_MID, sizeof(VOS_DIRS_FILESINFO_S));
    VOS_Mem_Zero((CHAR *)pstDirFileInfoHead, sizeof(VOS_DIRS_FILESINFO_S));

    /*遍历当前所有目录*/
    while ((pstDirEnt = readdir(pstCurDir)) != NULL)
    {
        stat(pstDirEnt->d_name, &stStat);
        
        if (strcmp(pstDirEnt->d_name, ".") == 0 || strcmp(pstDirEnt->d_name, "..") == 0)
        {
            continue;
        }
        
        if (S_ISDIR(stStat.st_mode))
        {   
            /*循环删除*/
            chdir(pstDirEnt->d_name);
            VOS_DirsCurrentRemove(pstDirEnt->d_name);
            chdir("..");
        }
        else
        {
            if (  NULL == pstDirFileInfoHead->next )
            {
                /*给下次申请一个内存*/                
                VOS_StrCpy_S(pstDirFileInfoHead->acFileName, VOS_FILENAME_LEN, pstDirEnt->d_name);
                /*先赋值给自己，这个就第一次会进入*/
                pstDirFileInfoHead->next = pstDirFileInfoHead;
            }
            else
            {   
                /*第二次进入*/
                if ( pstDirFileInfoHead == pstDirFileInfoHead->next )  
                {
                    pstDirFileInfoTmp = (VOS_DIRS_FILESINFO_S *)VOS_Malloc(VOS_MID, sizeof(VOS_DIRS_FILESINFO_S));
                    VOS_Mem_Zero((CHAR *)pstDirFileInfoTmp, sizeof(VOS_DIRS_FILESINFO_S));
                    
                    VOS_StrCpy_S(pstDirFileInfoTmp->acFileName, VOS_FILENAME_LEN, pstDirEnt->d_name);
                    pstDirFileInfoHead->next = pstDirFileInfoTmp;    
                    
                }
                else
                {
                    /*将上一次申请的赋值给next保存*/
                    pstDirFileInfoNext = pstDirFileInfoTmp;

                    /*然后再申请tmp*/
                    pstDirFileInfoTmp = (VOS_DIRS_FILESINFO_S *)VOS_Malloc(VOS_MID, sizeof(VOS_DIRS_FILESINFO_S));
                    VOS_Mem_Zero((CHAR *)pstDirFileInfoTmp, sizeof(VOS_DIRS_FILESINFO_S));
                    VOS_StrCpy_S(pstDirFileInfoTmp->acFileName, VOS_FILENAME_LEN, pstDirEnt->d_name);
                    
                    pstDirFileInfoNext->next = pstDirFileInfoTmp;
                }
            }

        }
    }
    closedir(pstCurDir);
    
    /*切换回工作目录*/
    chdir(acOldPath);
#endif
    return pstDirFileInfoHead;
}

/*释放目录信息*/
VOID VOS_DirsFileInfoFree(VOS_DIRS_FILESINFO_S *pstDirFileInfo)
{
    VOS_DIRS_FILESINFO_S *pstDirFileInfoNext = pstDirFileInfo;
    VOS_DIRS_FILESINFO_S *pstDirFileInfoTmp = pstDirFileInfo;

    if ( NULL == pstDirFileInfoTmp )
    {
        return;
    }
    
    while(pstDirFileInfoNext != NULL)
    {   
        pstDirFileInfoTmp = pstDirFileInfoNext;
    
        pstDirFileInfoNext = pstDirFileInfoTmp->next;
        
        VOS_Free((CHAR *)pstDirFileInfoTmp);
    }
}

/*判断该文件是否存在*/
LONG VOS_FileIsExist(const CHAR *pcFilePath)
{
    INT32         iRet = 0;

    if ( NULL == pcFilePath )    
    {
        VOS_Printf("VOS_FileIsExist param error!");
        return VOS_ERR;
    }
#if VOS_PLAT_LINUX
    /*不等于0，表示不存在*/
    iRet = access(pcFilePath, VOS_ERR_ACCESS_ISEXIST);
    if ( 0 != iRet )
    {
       VOS_Printf("access check file=[%s] is not exist!", pcFilePath);
        return VOS_ERR;
    }
#endif

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : VOS_FileCreate
 功能描述  : 创建一个文件
 输入参数  : CHAR *pcFilePath  ---注意该路径一定要为绝对路径
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月23日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG VOS_FileCreate(CHAR *pcFilePath)
{
    CHAR acDirPath[VOS_FILENAME_LEN] ={0};
    CHAR *pcChr = NULL;
    INT32  iPos = 0;
    
    if ( NULL == pcFilePath )    
    {
        VOS_Printf("VOS_FileCreate param error!");
        return VOS_ERR;
    }
#if VOS_PLAT_LINUX 
    FILE *pstFile = NULL;
    
    /*文件存在直接返回VOS_OK, 不用再创建了*/
    if ( VOS_OK == VOS_FileIsExist(pcFilePath) )
    {
        //VOS_Printf("file is exist , return OK!");
        return VOS_OK;
    }
    
    VOS_StrCpy_S(acDirPath, VOS_FILENAME_LEN, pcFilePath);
    
    /*查找最后出现的位置*/
    pcChr = strrchr(acDirPath, '/');
    if ( NULL == pcChr )
    {
        VOS_Printf("not found the  '/' error!");
        return VOS_ERR;
    }

    iPos = pcChr - acDirPath;

    acDirPath[iPos] = '\0';

    /*先创建目录*/
    if ( VOS_ERR == VOS_DirIsExist(acDirPath) )
    {
        /*创建目录*/
        if( VOS_OK != VOS_DirsCreate(acDirPath) )
        {
            VOS_Printf("create the dir path=[%s] error!", acDirPath);
            return VOS_ERR;
        }
    }

    pstFile = fopen(pcFilePath, "a+");
    if ( NULL == pstFile )
    {
        VOS_Printf("Create the file path=[%s] error!", pcFilePath);
        return VOS_ERR;
    }
    
    fclose(pstFile);
#endif
    
    return VOS_OK;
}


/*****************************************************************************
 函 数 名  : VOS_FileCreate
 功能描述  :  某个文件删除
 输入参数  : CHAR *pcFilePath  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月23日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG VOS_FileRemove(CHAR *pcFilePath)
{
    if ( NULL == pcFilePath )    
    {
        return VOS_ERR;
    }
#if VOS_PLAT_LINUX 
    if ( VOS_ERR == VOS_FileIsExist(pcFilePath))
    {
        return VOS_OK;
    }

    remove(pcFilePath);
#endif

    return VOS_OK;
}


/*****************************************************************************
 函 数 名  : VOS_FileGetSize
 功能描述  : 获取文件大小
 输入参数  : CHAR *pcFullName  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月10日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
UINT32 VOS_FileGetSize(CHAR *pcFullName)
{
    UINT32 ulSize = 0;

    if ( NULL == pcFullName )
    {
        return 0;
    }
    
#if VOS_PLAT_LINUX
    off_t file_size;
    struct stat stbuf;
    INT32 iFd = 0;
      
    iFd = open(pcFullName, O_RDONLY);
    if (iFd == -1) 
    {
      return 0;
    }
      
    if ((fstat(iFd, &stbuf) != 0) || (!S_ISREG(stbuf.st_mode))) 
    {
      close(iFd);
      return 0;
    }
      
    file_size = stbuf.st_size;
    ulSize = (UINT32)file_size;
#endif

    return ulSize;
}


/*****************************************************************************
 函 数 名  : VOS_FileRead
 功能描述  : 读取文件内容
 输入参数  : CHAR *pcFilename      ---文件名
             INT32 *pcLen      ---文件长度大小
             CHAR **pcData     ---申请出文件的内存，需要外部进行释放，否则内存泄漏
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年11月12日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
LONG VOS_FileRead(CHAR *pcFilename, INT32 *pcLen, UCHAR **ppucData )
{
    int iStep = 16000;
    int i = 0;
    int cur_len = 0;
    int iFile = 0;
    
#if VOS_PLAT_LINUX
    iFile = open(pcFilename, O_RDONLY);
    if ( iFile == -1 )
        return VOS_ERR;

    while (VOS_TRUE)
    {
        *ppucData = realloc( *ppucData, iStep*(i+1));
        if ( ppucData == NULL )
        {
            close(iFile);
            return VOS_ERR;
        }

        cur_len = read( iFile, *ppucData+(iStep*i), iStep);
        if ( cur_len == 0 )
            break;
        else
            *pcLen += cur_len;
        i++;
    }

    close( iFile );
#endif

    return VOS_OK;
}





/*****************************************************************************
 函 数 名  : VOS_EventfdCreate
 功能描述  : Linux下的EventFd的创建, 可以复用网络事件的EPOLL
 输入参数  : LONG lFlags  ---EFD_NONBLOCK | EFD_CLOEXEC ---类似这样的,Linux 26.26以后
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年4月19日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
LONG VOS_EventfdCreate(LONG lFlags)
{
    LONG lEventFd = -1;

    lEventFd = eventfd(0, lFlags);
    if ( lEventFd < 0 )
    {
        return VOS_ERR;
    }

    return lEventFd;
}

/*****************************************************************************
 函 数 名  : VOS_EventfdClose
 功能描述  : 关闭句柄
 输入参数  : LONG lEventFd  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年4月19日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_EventfdClose(LONG lEventFd)
{
    close(lEventFd);
}

/*****************************************************************************
 函 数 名  : VOS_EventfdRead
 功能描述  :    eventfd事件读数据功能
 输入参数  :    LONG lEventFd  
             CHAR *pcBuf    
             UINT32 uiSize  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年4月19日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
LONG VOS_EventfdRead(LONG lEventFd, VOID *pcBuf, UINT32 uiSize)
{
    LONG lRet = VOS_ERR;
    //ULONG ulVal = 1;
    
    //lRet = read(lEventFd, &ulVal, sizeof(ULONG));
    lRet = read(lEventFd, pcBuf, uiSize);
    if ( lRet < 0 )
    {
        VOS_Printf("VOS_Eventfd read error=%d,(%d)", lRet, errno);
    }
    
    return lRet;
}

/*****************************************************************************
 函 数 名  : VOS_EventfdWrite
 功能描述  : eventfd写事功能
 输入参数  : LONG lEventFd  
             CHAR *pcBuf    
             UINT32 uiSize  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年4月19日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
LONG VOS_EventfdWrite(LONG lEventFd, VOID *pcBuf, UINT32 uiSize)
{
    LONG lRet = VOS_ERR;
    //ULONG ulVal = 1;
    //lRet = write(lEventFd, &ulVal, sizeof(ULONG));
    lRet = write(lEventFd, pcBuf, uiSize);
    if ( lRet < 0 )
    {
        VOS_Printf("VOS_EventfdWrite error=%d,(%d)", lRet, errno);
    }
    
    return lRet;
}


