/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : vos_file.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年10月17日
  最近修改   :
  功能描述   : 文件操作
  函数列表   :
  修改历史   :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/
#include "Common/common.h"


/*****************************************************************************
 函 数 名  : VOS_FileOpen
 功能描述  : 打开文件
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 成功返回有效的文件句柄
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOS_FILE_T VOS_FileOpen(CHAR *pcFilePath)
{
    VOS_FILE_T hFileHandle = VOS_INVALID_HANDLE;

#if VOS_PLAT_WIN32
	hFileHandle = CreateFileA(pcFilePath,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		0);
	if ( INVALID_HANDLE_VALUE == hFileHandle)
	{
		return NULL;
	}

#elif VOS_PLAT_LINUX


#endif

    return hFileHandle;
}


/*****************************************************************************
 函 数 名  : VOS_FileClose
 功能描述  : 文件关闭
 输入参数  : VOS_FILE_T hFileHandle     ----待关闭的文件句柄
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_FileClose(VOS_FILE_T hFileHandle)
{
#if VOS_PLAT_WIN32
	if ( hFileHandle != VOS_INVALID_HANDLE )
	{
		if (!CloseHandle(hFileHandle))
		{
            return VOS_ERR;
		}
	}
#elif VOS_PLAT_LINUX

#endif
    return VOS_OK;
}


/*****************************************************************************
 函 数 名  : VOS_FileRead
 功能描述  : 文件读取
 输入参数  : VOS_FILE_T hFileHandle    ---文件句柄
           CHAR *pcReadBuff        ---读取缓存
           INT32 iLen              ---长度
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_FileRead(VOS_FILE_T hFileHandle, CHAR *pcReadBuff, INT32 iLen)
{
	INT32 iRet = VOS_ERR;

#if VOS_PLAT_WIN32
	BOOL  bRet = FALSE;
	DWORD dwSize = 0;

	bRet = ReadFile(hFileHandle, pcReadBuff, iLen, &dwSize, NULL);
	if (TRUE == bRet)
	{
		iRet = dwSize;
	}
#elif VOS_PLAT_LINUX


#endif
	
    return iRet;
}


/*****************************************************************************
 函 数 名  : VOS_FileWrite
 功能描述  : 文件写入,追加到末尾
 输入参数  : VOS_FILE_T hFileHandle  --文件句柄
             CHAR *pcWriteBuff       --写缓存
             INT32 iLen				 --长度
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_FileWrite(VOS_FILE_T hFileHandle, CHAR *pcWriteBuff, INT32 iLen)
{
	INT32 iRet = VOS_ERR;

#if VOS_PLAT_WIN32
	BOOL  bRet = FALSE;
	DWORD dwSize = 0;

	bRet = WriteFile(hFileHandle, pcWriteBuff, iLen, &dwSize, NULL);
	if (TRUE == bRet)
	{
		iRet = dwSize;
	}
    //printf("error=%d\n", GetLastError());
#elif VOS_PLAT_LINUX


#endif
    return iRet;
}

/*****************************************************************************
 函 数 名  : VOS_FILEGetSize
 功能描述  : 获取文件大小
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_FileGetSize(CHAR *pcFilePath)
{
#if VOS_PLAT_WIN32
    HANDLE hFile = NULL;
    DWORD  dwFileSize = 0;
    INT32  iRet = 0;

    hFile = CreateFileA(pcFilePath,
        GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        0);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        return VOS_ERR;
    }

    iRet = GetFileSize(hFile, &dwFileSize);
    if (0 == iRet)
    {
        CloseHandle(hFile);
        return VOS_ERR;
    }
    //else
    //{
    //    iRet = dwFileSize<<32| iRet;
    //}

    CloseHandle(hFile);
#elif VOS_PLAT_LINUX
    
    
#endif

    return iRet;
}

/*****************************************************************************
 函 数 名  : VOS_FileRemove
 功能描述  : 文件删除
 输入参数  : CHAR *pcFilePath  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_FileRemove(CHAR *pcFilePath)
{
#if VOS_PLAT_WIN32 
	BOOL  bRet = FALSE;

	bRet = DeleteFileA(pcFilePath);
	if (TRUE == bRet)
	{
		return VOS_OK;
	}

#elif VOS_PLAT_LINUX

#endif

    return VOS_ERR;
}


/*****************************************************************************
 函 数 名  : VOS_FileCreate
 功能描述  : 文件创建
 输入参数  : CHAR *pcFilePath  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOS_FILE_T VOS_FileCreate(CHAR *pcFilePath)
{
    VOS_FILE_T hFileHandle = VOS_INVALID_HANDLE;
#if VOS_PLAT_WIN32
	DWORD dwFileSize = 0;

    hFileHandle = CreateFileA(pcFilePath,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		0,
		0);
	if (INVALID_HANDLE_VALUE == hFileHandle)
	{
		return NULL;
	}

	if (0 != GetFileSize(hFileHandle, &dwFileSize))
	{
		CloseHandle(hFileHandle);
		return NULL;
	}

	CloseHandle(hFileHandle);
#elif VOS_PLAT_LINUX


#endif
    return hFileHandle;
}

/*****************************************************************************
 函 数 名  : VOS_FileFlush
 功能描述  : 刷新文件
 输入参数  : VOS_FILE_T hFileHandle
 输出参数  : 
 返 回 值  : INT32
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2019年04月27日
    作    者   : 闫朝东
    修改内容   : 新生成函数

*****************************************************************************/
BOOL    VOS_FileFlush(VOS_FILE_T hFileHandle)
{
#if VOS_PLAT_WIN32
    BOOL  bRet = FALSE;

    bRet = FlushFileBuffers(hFileHandle);
    if (FALSE == bRet)
    {
        return VOS_FALSE;
    }
    //printf("error=%d\n", GetLastError());
#elif VOS_PLAT_LINUX

    fflush(hFileHandle);
#endif

    return VOS_TRUE;
}
/*****************************************************************************
 函 数 名  : VOS_FileIsExist
 功能描述  : 文件是否存在
 输入参数  : const CHAR *pcFilePath  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
BOOL VOS_FileIsExist(const CHAR *pcFilePath)
{
#if VOS_PLAT_WIN32
    WIN32_FIND_DATAA wfd;
    HANDLE hFile = FindFirstFileA(pcFilePath, &wfd);

    if ((hFile != INVALID_HANDLE_VALUE)
        && !(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
        FindClose(hFile);
        return VOS_TRUE;
    }

    FindClose(hFile);
#elif VOS_PLAT_LINUX

    
#endif

    return VOS_FALSE;
}

/*****************************************************************************
 函 数 名  : VOS_DirIsExist
 功能描述  : 文件夹是否存在
 输入参数  : const CHAR *pcDirPath
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 闫朝东
    修改内容   : 新生成函数

*****************************************************************************/
BOOL    VOS_DirIsExist(const CHAR *pcDirPath)
{
#if VOS_PLAT_WIN32
    WIN32_FIND_DATAA wfd;
    HANDLE hFile = FindFirstFileA(pcDirPath, &wfd);

    if ((hFile != INVALID_HANDLE_VALUE)
        && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
    {
        FindClose(hFile);
        return VOS_TRUE;
    }

    FindClose(hFile);
#elif VOS_PLAT_LINUX


#endif
    return VOS_FALSE;
}
/*****************************************************************************
 函 数 名  : VOS_GetCurrentProcDirPath
 功能描述  : 获取当前进程路径
 输入参数  : CHAR *pcProcDirPath  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_DirGetCurrentProcPath(CHAR *pcProcDirPath)
{
	char *pcStr = NULL;
	if (NULL == pcProcDirPath)
	{
		return;
	}

#if VOS_PLAT_WIN32
	GetModuleFileNameA(NULL, pcProcDirPath, MAX_PATH);
	pcStr = strrchr(pcProcDirPath, ('\\'));
	if (NULL != pcStr)
	{
		pcStr[1] = 0;
	}
#elif VOS_PLAT_LINUX

#endif

	return;
}

/*****************************************************************************
 函 数 名  : VOS_GetFilesMotifyTime
 功能描述  :
 输入参数  : const char* pfilename
 输入参数  : time_t* tmCreateTime
 输出参数  :
 返 回 值  : void
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年11月9日
    作    者   : yanchaodong
    修改内容   :获取文件修改时间

*****************************************************************************/
INT32 VOS_GetFilesMotifyTime(const char* pfilename, time_t* tmCreateTime)
{
#if VOS_PLAT_WIN32
    FILETIME ftCreate, ftAccess, ftModify;
    FILETIME stLocalTime;
    SYSTEMTIME stLocal;
    HANDLE hFile = CreateFileA(pfilename, GENERIC_READ,          // open for reading
        FILE_SHARE_READ,       // share for reading
        NULL,                            // default security
        OPEN_EXISTING,          // existing file only
        FILE_FLAG_BACKUP_SEMANTICS, // normal file
        NULL);


    if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftModify))
    {
        CloseHandle(hFile);
        return VOS_ERR;//continue;
    }

    ZeroMemory(&stLocal, sizeof(SYSTEMTIME));
    FileTimeToLocalFileTime(&ftModify, &stLocalTime);
    FileTimeToSystemTime(&stLocalTime, &stLocal);

    struct tm _tm;
    _tm.tm_year = stLocal.wYear - 1900;
    _tm.tm_mon = stLocal.wMonth - 1;
    _tm.tm_mday = stLocal.wDay;
    _tm.tm_hour = stLocal.wHour;
    _tm.tm_min = stLocal.wMinute;
    _tm.tm_sec = stLocal.wSecond;

    *tmCreateTime = mktime(&_tm);

    CloseHandle(hFile);
#elif VOS_PLAT_LINUX
    struct stat ss;
    int nRet = stat(filename.c_str(), &ss);
    if (nRet != VOS_OK)
    {
        CLOG_writelog_level("DBM", CLOG_LEVEL_ERROR, "get file informations error:%d", errno);
        return VOS_ERR;
    }
    else
    {
        tmCreateTime = ss.st_ctime;
    }
#endif
    return VOS_OK;
}









