/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : vos_utils.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��10��17��
  ����޸�   :
  ��������   : VOS�ڲ�ʹ�õ�Utls����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/

/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
    
#include "Common/common.h"


/*----------------------------------------------*
 * �ⲿ����˵��                                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ȫ�ֱ���                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ģ�鼶����                                   *
 *----------------------------------------------*/

/*----------------------------------------------*
 * ��������                                     *
 *----------------------------------------------*/

/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/

/*****************************************************************************
 �� �� ��  : VOS_UTLS_GetCurrentDirectory
 ��������  : ��ȡ��ǰĿ¼
 �������  : char *pcCurDir  
 �������  : ��
 �� �� ֵ  : int
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_UTILS_GetUUID
 ��������  : ��ȡUUDΨһ��ʶ��   
 �������  : CHAR *pcuuid           --UUID�ַ���,��ʽ"xxxxxxxx-xxxx-xxxx-xxxxxxxxxxxxxx"
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_UTILS_GenUuidData
 ��������  : ������GUID�����ݣ�ֱ�ӹ涨Ϊ8������
 �������  : CHAR acUUid[8]  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��18��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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