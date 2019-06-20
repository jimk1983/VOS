/******************************************************************************

                  版权所有 (C), 2018-2028, 杭州友机技术有限公司

 ******************************************************************************
  文 件 名   : vos_utils.c
  版 本 号   : 初稿
  作    者   : 蒋康
  生成日期   : 2018年10月17日
  最近修改   :
  功能描述   : VOS内部使用的Utls函数
  函数列表   :
  修改历史   :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 创建文件

******************************************************************************/

/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
    
#include "Common/common.h"


/*----------------------------------------------*
 * 外部变量说明                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 外部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 内部函数原型说明                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 全局变量                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 模块级变量                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

/*****************************************************************************
 函 数 名  : VOS_UTLS_GetCurrentDirectory
 功能描述  : 获取当前目录
 输入参数  : char *pcCurDir  
 输出参数  : 无
 返 回 值  : int
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_UTLS_GetCurrentDirectory(char *pcCurDir)
{
    char acCurDir[MAX_PATH] = {0};

    if (NULL == pcCurDir)
    {
		return VOS_ERR;
    }

#if VOS_PLAT_WIN32 
    (VOID)GetCurrentDirectoryA(MAX_PATH, acCurDir);
#elif VOS_PLAT_LINUX

#endif

    strcpy_s(pcCurDir, MAX_PATH-1, acCurDir);

	return VOS_OK;
}


/*****************************************************************************
 函 数 名  : VOS_UTILS_GetUUID
 功能描述  : 获取UUD唯一标识码   
 输入参数  : CHAR *pcuuid           --UUID字符串,格式"xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxx"
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月17日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_UTILS_GenUUid(CHAR *pcUUid)
{
    CHAR  acBuffer[VOS_GUID_LEN] = {0};
    INT32 iLen = 0;
    
#if VOS_PLAT_WIN32
    GUID guid;

    if ( CoCreateGuid(&guid) )
    {
        fprintf(stderr, "create guid error\n");
        return VOS_ERR;
    }
    else
    {
        iLen = _snprintf_s(acBuffer, VOS_GUID_LEN-1, VOS_GUID_LEN, 
            "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
            guid.Data1, guid.Data2, guid.Data3,
            guid.Data4[0], guid.Data4[1], guid.Data4[2],
            guid.Data4[3], guid.Data4[4], guid.Data4[5],
            guid.Data4[6], guid.Data4[7]);

            if ( NULL != pcUUid )
            {
                memcpy_s(pcUUid, VOS_GUID_LEN-1, acBuffer, iLen);
            }
    }

#elif VOS_PLAT_LINUX

#endif

    return VOS_OK;
}



/*****************************************************************************
 函 数 名  : VOS_UTILS_GenUuidData
 功能描述  : 产生的GUID的数据，直接规定为8个长度
 输入参数  : CHAR acUUid[8]  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年10月18日
    作    者   : 蒋康
    修改内容   : 新生成函数

*****************************************************************************/
INT32 VOS_UTILS_GenUUidData(CHAR *pcUUid)
{
#if VOS_PLAT_WIN32
    GUID guid;

    if ( CoCreateGuid(&guid) )
    {
        fprintf(stderr, "create guid error\n");
        return VOS_ERR;
    }
    else
    {
		if (NULL != pcUUid)
		{
			memcpy_s(pcUUid, VOS_GUIDDATA_LEN, &guid, VOS_GUIDDATA_LEN);
		}
    }

#elif VOS_PLAT_LINUX
    
#endif

    return VOS_OK;
}


#if VOS_PLAT_WIN32

INT32 VOS_UTILS_AscIItoUnicode(CHAR *pcStr, WCHAR *plwstr)
{
	//wchar_t pwStr[MAX_PATH * sizeof(wchar_t)] = { 0 };;
	int wcharlen = 0;

	wcharlen = MultiByteToWideChar(CP_ACP, 0, pcStr, -1, NULL, 0);

	MultiByteToWideChar(CP_ACP, 0, pcStr, -1, (LPWSTR)plwstr, wcharlen);

	//memcpy_s(plwstr, wcharlen, pwStr, MAX_PATH * sizeof(wchar_t));

	return 0;
}

INT32 VOS_UTILS_UnicodeToAscII(WCHAR *plwstr, CHAR *pcStr)
{
	char acStr[MAX_PATH] = { 0 };
	int ilen = 0;

	ilen = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)plwstr, -1, NULL, 0, NULL, NULL);

	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)plwstr, -1, acStr, ilen, NULL, NULL);

	memcpy_s(pcStr, ilen, acStr, MAX_PATH);

	return 0;
}

#endif