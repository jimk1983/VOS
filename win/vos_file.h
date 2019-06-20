/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : vos_file.h
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

#ifndef _VOS_FILE_H_
#define _VOS_FILE_H_


/*最大的文件名长度*/
#define VOS_FILENAME_LEN        256
#define VOS_DIRSNAME_LEN        256

#if VOS_PLAT_WIN32
typedef HANDLE  VOS_FILE_T;
#define VOS_DIR_SEPARATOR						"\\"
#elif VOS_PLAT_LINUX
#define VOS_DIR_SEPARATOR						"/"
#endif


VOS_FILE_T VOS_FileOpen(CHAR *pcFilePath);

INT32   VOS_FileClose(VOS_FILE_T hFileHandle);

INT32   VOS_FileRead(VOS_FILE_T hFileHandle, CHAR *pcReadBuff, INT32 iLen);

INT32   VOS_FileWrite(VOS_FILE_T hFileHandle, CHAR *pcWriteBuff, INT32 iLen);

INT32   VOS_FileGetSize(CHAR *pcFilePath);

BOOL    VOS_FileFlush(VOS_FILE_T hFileHandle);

INT32   VOS_FileRemove(CHAR *pcFilePath);

VOS_FILE_T   VOS_FileCreate(CHAR *pcFilePath);

BOOL    VOS_FileIsExist(const CHAR *pcFilePath);

BOOL    VOS_DirIsExist(const CHAR *pcDirPath);

VOID    VOS_DirGetCurrentProcPath(CHAR *pcProcDirPath);

INT32   VOS_GetFilesMotifyTime(const char* pfilename, time_t* tmCreateTime);

#endif
