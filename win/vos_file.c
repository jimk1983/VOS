/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : vos_file.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��10��17��
  ����޸�   :
  ��������   : �ļ�����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/
#include "Common/common.h"


/*****************************************************************************
 �� �� ��  : VOS_FileOpen
 ��������  : ���ļ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : �ɹ�������Ч���ļ����
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_FileClose
 ��������  : �ļ��ر�
 �������  : VOS_FILE_T hFileHandle     ----���رյ��ļ����
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_FileRead
 ��������  : �ļ���ȡ
 �������  : VOS_FILE_T hFileHandle    ---�ļ����
           CHAR *pcReadBuff        ---��ȡ����
           INT32 iLen              ---����
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_FileWrite
 ��������  : �ļ�д��,׷�ӵ�ĩβ
 �������  : VOS_FILE_T hFileHandle  --�ļ����
             CHAR *pcWriteBuff       --д����
             INT32 iLen				 --����
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_FILEGetSize
 ��������  : ��ȡ�ļ���С
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_FileRemove
 ��������  : �ļ�ɾ��
 �������  : CHAR *pcFilePath  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_FileCreate
 ��������  : �ļ�����
 �������  : CHAR *pcFilePath  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_FileFlush
 ��������  : ˢ���ļ�
 �������  : VOS_FILE_T hFileHandle
 �������  : 
 �� �� ֵ  : INT32
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2019��04��27��
    ��    ��   : �Ƴ���
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_FileIsExist
 ��������  : �ļ��Ƿ����
 �������  : const CHAR *pcFilePath  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_DirIsExist
 ��������  : �ļ����Ƿ����
 �������  : const CHAR *pcDirPath
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : �Ƴ���
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_GetCurrentProcDirPath
 ��������  : ��ȡ��ǰ����·��
 �������  : CHAR *pcProcDirPath  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_GetFilesMotifyTime
 ��������  :
 �������  : const char* pfilename
 �������  : time_t* tmCreateTime
 �������  :
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��11��9��
    ��    ��   : yanchaodong
    �޸�����   :��ȡ�ļ��޸�ʱ��

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









