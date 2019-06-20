/******************************************************************************

                  ��Ȩ���� (C), 2018-2028, �����ѻ��������޹�˾

 ******************************************************************************
  �� �� ��   : vos_string.c
  �� �� ��   : ����
  ��    ��   : ����
  ��������   : 2018��10��17��
  ����޸�   :
  ��������   : �ַ�������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ļ�

******************************************************************************/

#include "Common/common.h"



/*****************************************************************************
 �� �� ��  : VOS_Snprintf
 ��������  : �ַ���ƴ��
 �������  : CHAR *pcBuf      
             UINT32 ulMaxLen  
             CHAR *pcFormat   
             ...              
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��17��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32 VOS_Snprintf(CHAR *pcBuf, INT32 ulMaxLen, const CHAR *pcFormat,...)
{
    va_list args_ptr;
    INT32 ulRet = 0;    
    
    if( NULL == pcBuf || NULL == pcFormat )
    { 
        return 0;
    }

    va_start(args_ptr, pcFormat);
    
    #if VOS_PLAT_WIN32
	
	ulRet = _vscprintf_l(pcFormat, 0, args_ptr) + 1;
	if (ulRet >= ulMaxLen-1 )
	{
		return 0;
	}

    ulRet = vsprintf_s(pcBuf, ulMaxLen-1,pcFormat,args_ptr);
    #elif VOS_PLAT_LINUX
    ulRet = vsnprintf(pcBuf, ulMaxLen-1,pcFormat,args_ptr);
    #elif VOS_PLAT_MAC
    ulRet = vsnprintf(pcBuf, ulMaxLen-1,pcFormat,args_ptr);
    #endif
    va_end(args_ptr);
   
    return ulRet;
}




/*****************************************************************************
 �� �� ��  : VOS_StrCpy_S
 ��������  : �ַ�������
 �������  : CHAR *pcDst     
             ULONG ulMaxLen  
             CHAR *pcSrc     
 �������  : ��
 �� �� ֵ  : CHAR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��10��31��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID VOS_StrCpy_S(CHAR *pcDst, ULONG ulMaxLen, CHAR *pcSrc)
{
    if ( NULL == pcDst 
        || NULL == pcSrc )
    {
        return;
    }

    strcpy_s(pcDst, ulMaxLen-1, pcSrc);
        
    return;
}








