/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : vos_system.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��10��25��
  ����޸�   :
  ��������   : VOSϵͳ����ʵ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��10��25��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/

#include "Common/common.h"


#if VOS_PLAT_WIN32
#include <tlhelp32.h>	/*���̿���ͷ�ļ�*/
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib,"Ole32.Lib")

#endif


/*****************************************************************************
 �� �� ��  : VOS_API_CreateProcess
 ��������  :    �������̣������ɹ��󣬻�����ס��ǰ���߳�
 �������  :    CHAR *pcProcPath  --���̾���·��,ʾ��: "C:\\Program Files\\AAA.exe"
            CHAR *pcCmdArgv   --�������
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��25��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
BOOL    VOS_API_CreateProcess(CHAR *pcProcPath, CHAR *pcCmdArgv, BOOL bAsynchronous)
{
    BOOL    bRet = VOS_FALSE;
    
    if ( NULL == pcProcPath )
    {
        return VOS_FALSE;
    }

#if VOS_PLAT_WIN32
    STARTUPINFOA            stStartUpInfo;
    PROCESS_INFORMATION     stProcessInfo;
    DWORD	                dwRet = 0;
    DWORD                   dwErrCode = 0;
	//HDESK					hDesk;
	//hDesk = CreateDesktopA("MyDesk", NULL, NULL, 0, GENERIC_ALL, NULL);
	//ASSERT(hDesk);

    VOS_Mem_Zero((CHAR *)&stStartUpInfo, sizeof(stStartUpInfo));
    VOS_Mem_Zero((CHAR *)&stProcessInfo, sizeof(stProcessInfo));

    stStartUpInfo.cb = sizeof(stStartUpInfo);
	//stStartUpInfo.lpDesktop = _T("MyDesk");
    //cjwang delete begin on 2018/11/22 
    stStartUpInfo.dwFlags = STARTF_USESHOWWINDOW;
    stStartUpInfo.wShowWindow = SW_HIDE;
    //cjwang delete end on 2018/11/22 

	if ( NULL == pcCmdArgv )
	{
		bRet = CreateProcessA(
			pcProcPath,
			NULL,
			NULL,
			NULL,
			FALSE,
            NORMAL_PRIORITY_CLASS|CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&stStartUpInfo,
			&stProcessInfo);
		if (bRet)
		{
			/*TODO: ��Ҫ���Ǿ���رս��̵��������ʱ���������ⲿ���д����߳�*/
            if (FALSE == bAsynchronous)
            {
                dwRet = WaitForSingleObject(stProcessInfo.hProcess, INFINITE);
            }
			//
			CloseHandle(stProcessInfo.hProcess);
			CloseHandle(stProcessInfo.hThread);

			return VOS_TRUE;
		}
		else
		{
			dwErrCode = VOS_GetLastError();
		}
	}
	else
	{
		CHAR acCmdLine[MAX_PATH] = {0};

		VOS_Snprintf(acCmdLine,MAX_PATH, "\"%s\" %s", pcProcPath, pcCmdArgv);
		bRet = CreateProcessA(
			NULL,
			acCmdLine,
			NULL,
			NULL,
			FALSE,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&stStartUpInfo,
			&stProcessInfo);
		if (bRet)
		{
			/*TODO: ��Ҫ���Ǿ���رս��̵��������ʱ���������ⲿ���д����߳�*/
            if (FALSE == bAsynchronous)
            {
                dwRet = WaitForSingleObject(stProcessInfo.hProcess, INFINITE);
            }
			CloseHandle(stProcessInfo.hProcess);
			CloseHandle(stProcessInfo.hThread);

			return VOS_TRUE;
		}
		else
		{
			dwErrCode = VOS_GetLastError();
		}
	}
#elif 



#endif
    

    return VOS_FALSE;
}



/*****************************************************************************
 �� �� ��  : VOS_API_KillProcess
 ��������  : ǿɱ����
 �������  : CHAR *pcProcName	---ǿɱ���̵ľ���·������
 �������  : ��
 �� �� ֵ  : INT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
	��    ��   : ����
	�޸�����   : �����ɺ���

*****************************************************************************/
INT32 VOS_API_KillProcess(CHAR *pcProcName)
{
#if VOS_PLAT_WIN32 
	HANDLE hSnapshot;
	PROCESSENTRY32 lppe;
	UINT32 uiProcMatch = 0;
	WCHAR	wcProcName[MAX_PATH] = {0};

	(VOID)VOS_UTILS_AscIItoUnicode(pcProcName, wcProcName);

	//����ϵͳ���� 
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == NULL)
	{
		return VOS_ERR;
	}

	//��ʼ�� lppe �Ĵ�С 
	lppe.dwSize = sizeof(lppe);

	//���ҵ�һ������ 
	if (!Process32First(hSnapshot, &lppe))
		return VOS_ERR;
    if (NULL == pcProcName)
    {
        return VOS_ERR;
    }
	do
	{
        if (0 == _wcsicmp(lppe.szExeFile, wcProcName))
        {
            uiProcMatch = 1;
        }
		if (1 == uiProcMatch)
		{
			DWORD xCode;
			HANDLE hProc;
			hProc = OpenProcess(PROCESS_TERMINATE, FALSE, lppe.th32ProcessID);
			if (!GetExitCodeProcess(hProc, &xCode))
			{
				TerminateProcess(hProc, xCode);
			}

			uiProcMatch = 0;
		}
	}while (Process32Next(hSnapshot, &lppe)); //������һ������ 

	if (!CloseHandle(hSnapshot))
		return VOS_ERR;
#endif
	return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : VOS_API_ProcessIsExist
 ��������  : �����Ƿ����
 �������  : CHAR *pcProcName	---���ҽ��̵ľ���·������
 �������  : ��
 �� �� ֵ  : INT32
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��11��15��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32 VOS_API_ProcessIsExist(CHAR *pcProcName)
{
    UINT32 uiProcMatch = 0;
#if VOS_PLAT_WIN32 
    HANDLE hSnapshot;
    PROCESSENTRY32 lppe;
    WCHAR   wcProcName[MAX_PATH] = { 0 };

    (VOID)VOS_UTILS_AscIItoUnicode(pcProcName, wcProcName);

    //����ϵͳ���� 
    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if ( hSnapshot == NULL )
    {
        return VOS_ERR;
    }

    //��ʼ�� lppe �Ĵ�С 
    lppe.dwSize = sizeof(lppe);

    //���ҵ�һ������ 
    if (!Process32First(hSnapshot, &lppe))
        return VOS_ERR;

    if (NULL == pcProcName)
    {
        return VOS_ERR;
    }
    do
    {
        if (0 == _wcsicmp(lppe.szExeFile, wcProcName))
        {
            uiProcMatch = 1;
            break;
        }
    } while (Process32Next(hSnapshot, &lppe)); //������һ������ 

    if (!CloseHandle(hSnapshot))
    {
        return VOS_ERR;
    }
#endif
    if ( 0 == uiProcMatch )
    {
        return VOS_ERR;
    }

    return VOS_OK;
}
