/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_file.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��10��22��
  ����޸�   :
  ��������   : �ж�Ŀ¼�Ƿ����
  �����б�   :
              VOS_DirIsExist
  �޸���ʷ   :
  1.��    ��   : 2016��10��22��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#include <vos_pub.h>

/*LINUX�µ�errno����*/
#define VOS_ERRNO_FILEORDIRNOTEXIST     2               /*������Ŀ¼�����ļ��м��������ڣ��޷�����*/
#define VOS_ERRNO_FILEISEXIST                   17             /*���ļ��Ѿ�����*/

/*����ļ��Ƿ����*/
#define VOS_ERR_ACCESS_ISEXIST                0

/*����Ȩ��*/
#define VOS_FILEPRIV_ALL                            0777
/*****************************************************************************
 �� �� ��  : VOS_DirIsExist
 ��������  : �ж�Ŀ¼�Ƿ����
 �������  : const CHAR *pcDirPath  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��22��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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
    /*�򿪳ɹ���ʾ����*/
    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : VOS_DirGetCurrentPath
 ��������  : ��ȡ��ǰ��Ŀ¼
 �������  : CHAR *pcDirPath  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��9��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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

    //��ȡ��ǰĿ¼����·������ȥ��������
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
 �� �� ��  : VOS_DirsCreate
 ��������  : �����༶Ŀ¼
 �������  : CHAR *pcDirPath  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��22��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

int   access(const   char   *filename,   int   amode); 
amode����Ϊ0ʱ��ʾ����ļ��Ĵ����ԣ�����ļ����ڣ�����0�������ڣ�����-1�� 
������������Լ�������ļ����ԣ� 
06     ����дȨ�� 
04     ����Ȩ�� 
02     ���дȨ�� 
01     ���ִ��Ȩ�� 
00     ����ļ��Ĵ�����
�������������ļ�û�ж�Ȩ�ޣ�Ҳ�����ж�����ļ������ڷ�
���ڷ���0�������ڷ���-1

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

    /*��1��ʼ��0Ϊ��Ŀ¼��'/' */
    for ( iPos = 1; iPos < iLen; iPos++)
    {
        if (acTmp[iPos] == '/' )
        {
            acTmp[iPos] = '\0';
            
            if ( VOS_DirIsExist(acTmp) != VOS_OK )
            {
                /*����������Ȩ��*/
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
 �� �� ��  : VOS_DIrsCurrentRemove
 ��������  : ɾ����Ŀ¼�������ļ��к��ļ�
 �������  : CHAR *pcDir  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��23��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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

    /*������ǰ����Ŀ¼*/
    while ((pstDirEnt = readdir(pstCurDir)) != NULL)
    {
        stat(pstDirEnt->d_name, &stStat);
        
        if (strcmp(pstDirEnt->d_name, ".") == 0 || strcmp(pstDirEnt->d_name, "..") == 0)
        {
            continue;
        }
        
        if (S_ISDIR(stStat.st_mode))
        {   
            /*ѭ��ɾ��*/
            chdir(pstDirEnt->d_name);
            VOS_DirsCurrentRemove(pstDirEnt->d_name);
            chdir("..");
        }
        /*һ��ɾ��*/
        remove(pstDirEnt->d_name);
    }
    closedir(pstCurDir);
    
#endif
    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : VOS_DirsRemove
 ��������  : ɾ��һ��Ŀ¼��������Ŀ¼�µ������ļ����ļ���
 �������  : CHAR *pcDirPath  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��23��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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

    /*�л���ɾ����Ŀ¼*/
    if (chdir(pcDirPath) == -1)
    {
        return VOS_ERR;
    }
 
    (VOID)VOS_DirsCurrentRemove(pcDirPath);
    /*�л��ع���Ŀ¼*/
    chdir(acOldPath);
 
    /*
    �������ɾ����Ŀ¼����Ļ�,ȡ����ע��
    unlink(acOldPath); 
    */
#endif
    
    return VOS_OK;
}


/*��Ҫͨ�������ɾ��*/
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
    /*��ȡ��ǰĿ¼*/
    getcwd(acOldPath, VOS_DIRSNAME_LEN);

    /*�л���������Ŀ¼*/
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

    /*������ǰ����Ŀ¼*/
    while ((pstDirEnt = readdir(pstCurDir)) != NULL)
    {
        stat(pstDirEnt->d_name, &stStat);
        
        if (strcmp(pstDirEnt->d_name, ".") == 0 || strcmp(pstDirEnt->d_name, "..") == 0)
        {
            continue;
        }
        
        if (S_ISDIR(stStat.st_mode))
        {   
            /*ѭ��ɾ��*/
            chdir(pstDirEnt->d_name);
            VOS_DirsCurrentRemove(pstDirEnt->d_name);
            chdir("..");
        }
        else
        {
            if (  NULL == pstDirFileInfoHead->next )
            {
                /*���´�����һ���ڴ�*/                
                VOS_StrCpy_S(pstDirFileInfoHead->acFileName, VOS_FILENAME_LEN, pstDirEnt->d_name);
                /*�ȸ�ֵ���Լ�������͵�һ�λ����*/
                pstDirFileInfoHead->next = pstDirFileInfoHead;
            }
            else
            {   
                /*�ڶ��ν���*/
                if ( pstDirFileInfoHead == pstDirFileInfoHead->next )  
                {
                    pstDirFileInfoTmp = (VOS_DIRS_FILESINFO_S *)VOS_Malloc(VOS_MID, sizeof(VOS_DIRS_FILESINFO_S));
                    VOS_Mem_Zero((CHAR *)pstDirFileInfoTmp, sizeof(VOS_DIRS_FILESINFO_S));
                    
                    VOS_StrCpy_S(pstDirFileInfoTmp->acFileName, VOS_FILENAME_LEN, pstDirEnt->d_name);
                    pstDirFileInfoHead->next = pstDirFileInfoTmp;    
                    
                }
                else
                {
                    /*����һ������ĸ�ֵ��next����*/
                    pstDirFileInfoNext = pstDirFileInfoTmp;

                    /*Ȼ��������tmp*/
                    pstDirFileInfoTmp = (VOS_DIRS_FILESINFO_S *)VOS_Malloc(VOS_MID, sizeof(VOS_DIRS_FILESINFO_S));
                    VOS_Mem_Zero((CHAR *)pstDirFileInfoTmp, sizeof(VOS_DIRS_FILESINFO_S));
                    VOS_StrCpy_S(pstDirFileInfoTmp->acFileName, VOS_FILENAME_LEN, pstDirEnt->d_name);
                    
                    pstDirFileInfoNext->next = pstDirFileInfoTmp;
                }
            }

        }
    }
    closedir(pstCurDir);
    
    /*�л��ع���Ŀ¼*/
    chdir(acOldPath);
#endif
    return pstDirFileInfoHead;
}

/*�ͷ�Ŀ¼��Ϣ*/
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

/*�жϸ��ļ��Ƿ����*/
LONG VOS_FileIsExist(const CHAR *pcFilePath)
{
    INT32         iRet = 0;

    if ( NULL == pcFilePath )    
    {
        VOS_Printf("VOS_FileIsExist param error!");
        return VOS_ERR;
    }
#if VOS_PLAT_LINUX
    /*������0����ʾ������*/
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
 �� �� ��  : VOS_FileCreate
 ��������  : ����һ���ļ�
 �������  : CHAR *pcFilePath  ---ע���·��һ��ҪΪ����·��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��23��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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
    
    /*�ļ�����ֱ�ӷ���VOS_OK, �����ٴ�����*/
    if ( VOS_OK == VOS_FileIsExist(pcFilePath) )
    {
        //VOS_Printf("file is exist , return OK!");
        return VOS_OK;
    }
    
    VOS_StrCpy_S(acDirPath, VOS_FILENAME_LEN, pcFilePath);
    
    /*���������ֵ�λ��*/
    pcChr = strrchr(acDirPath, '/');
    if ( NULL == pcChr )
    {
        VOS_Printf("not found the  '/' error!");
        return VOS_ERR;
    }

    iPos = pcChr - acDirPath;

    acDirPath[iPos] = '\0';

    /*�ȴ���Ŀ¼*/
    if ( VOS_ERR == VOS_DirIsExist(acDirPath) )
    {
        /*����Ŀ¼*/
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
 �� �� ��  : VOS_FileCreate
 ��������  :  ĳ���ļ�ɾ��
 �������  : CHAR *pcFilePath  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��10��23��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_FileGetSize
 ��������  : ��ȡ�ļ���С
 �������  : CHAR *pcFullName  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��10��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_FileRead
 ��������  : ��ȡ�ļ�����
 �������  : CHAR *pcFilename      ---�ļ���
             INT32 *pcLen      ---�ļ����ȴ�С
             CHAR **pcData     ---������ļ����ڴ棬��Ҫ�ⲿ�����ͷţ������ڴ�й©
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��11��12��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_EventfdCreate
 ��������  : Linux�µ�EventFd�Ĵ���, ���Ը��������¼���EPOLL
 �������  : LONG lFlags  ---EFD_NONBLOCK | EFD_CLOEXEC ---����������,Linux 26.26�Ժ�
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��4��19��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_EventfdClose
 ��������  : �رվ��
 �������  : LONG lEventFd  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��4��19��
    ��    ��   : ����
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID VOS_EventfdClose(LONG lEventFd)
{
    close(lEventFd);
}

/*****************************************************************************
 �� �� ��  : VOS_EventfdRead
 ��������  :    eventfd�¼������ݹ���
 �������  :    LONG lEventFd  
             CHAR *pcBuf    
             UINT32 uiSize  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��4��19��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
 �� �� ��  : VOS_EventfdWrite
 ��������  : eventfdд�¹���
 �������  : LONG lEventFd  
             CHAR *pcBuf    
             UINT32 uiSize  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��4��19��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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


