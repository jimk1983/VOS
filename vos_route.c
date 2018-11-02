/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_route.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��6��28��
  ����޸�   :
  ��������   : ϵͳ·�����
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��6��28��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/


/*****************************************************************************
 �� �� ��  : VOS_GetCurrentRouteTable
 ��������  : ��ȡ��ǰϵͳ��·�ɱ�
 �������  : VOS_SYS_ROUTE_TBL_S *pstRouteTbl  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��6��28��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
#include <vos_pub.h>
#include <vos_log.h>


/*��ȡϵͳ���е�·�ɱ���Ϣ
ע�� : ��һ�����������ͬһ��·���ж������������ȼ���ͬ
��ô��Ҫ�ж�˭�����ȼ���С��ԽС�����ȼ�Խ��*/
INT32   VOS_GetCurrentRouteTable(VOS_SYS_ROUTE_TBL_S *pstRouteTbl)
{
    FILE *fp = NULL;
    CHAR acBuf[256] = {0};
    UINT32 uiCount = 0;
    
    UINT32 uiRef = 0;
    UINT32 uiUse = 0;
    UINT32 uiWindow =0;
    UINT32 uiIrtt = 0;
#if VOS_PLAT_LINUX

    
    if ( NULL == pstRouteTbl )
    {
        return VOS_ERR;
    }
    
    fp = fopen("/proc/net/route", "r");
    if ( NULL == fp )
    {
        return VOS_ERR;
    }

    /*��ȡ����*/
    fgets(acBuf, 256, fp);

    while(fgets(acBuf, 256, fp))
    {
        
        sscanf(acBuf, "%s%08x%08x%04x%04x%02x%02x%08x%02x%02x%02x",
            pstRouteTbl->astArrayRouteTbl[uiCount].acDevName,
            &pstRouteTbl->astArrayRouteTbl[uiCount].uiDstip,
            &pstRouteTbl->astArrayRouteTbl[uiCount].uiGateway,
            &pstRouteTbl->astArrayRouteTbl[uiCount].ulFlags,
            &uiRef,
            &uiUse,
            &pstRouteTbl->astArrayRouteTbl[uiCount].uiMetric,
            &pstRouteTbl->astArrayRouteTbl[uiCount].uiMask,
            &pstRouteTbl->astArrayRouteTbl[uiCount].uiMtu,
            &uiWindow,
            &uiIrtt);
        //printf("%s ", pstRouteTbl->astArrayRouteTbl[uiCount].acDevName);
        //printf("%08x ", pstRouteTbl->astArrayRouteTbl[uiCount].uiDstip);
        //printf("%08x ", pstRouteTbl->astArrayRouteTbl[uiCount].uiGateway);
        //printf("%08x\n", pstRouteTbl->astArrayRouteTbl[uiCount].uiMask);
        
        uiCount++;
        
    }
    
    fclose(fp);
#endif
    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : VOS_GetDefaultRouteEntry
 ��������  : ��ȡĬ��·��
 �������  : VOS_ROUTE_ENTRY_S *pstRouteEntry  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��6��29��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32   VOS_GetDefaultGatewayRouteEntry(VOS_ROUTE_ENTRY_S *pstRouteEntry)
{
    FILE *fp = NULL;
    CHAR acBuf[256] = {0};
    UINT32 uiCount = 0;
    UINT32 uiRef = 0;
    UINT32 uiUse = 0;
    UINT32 uiWindow =0;
    UINT32 uiIrtt = 0;
    VOS_SYS_ROUTE_TBL_S   stRouteTable ={{{0}}};
    VOS_SYS_ROUTE_TBL_S *pstRouteTbl =  &stRouteTable;
#if VOS_PLAT_LINUX

    if ( NULL == pstRouteEntry )
    {
        return VOS_ERR;
    }
    
    fp = fopen("/proc/net/route", "r");
    if ( NULL == fp )
    {
        return VOS_ERR;
    }

    /*��ȡ����*/
    fgets(acBuf, 256, fp);

    while(fgets(acBuf, 256, fp))
    {
        
        sscanf(acBuf, "%s%08x%08x%04x%04x%02x%02x%08x%02x%02x%02x",
            pstRouteTbl->astArrayRouteTbl[uiCount].acDevName,
            &pstRouteTbl->astArrayRouteTbl[uiCount].uiDstip,
            &pstRouteTbl->astArrayRouteTbl[uiCount].uiGateway,
            &pstRouteTbl->astArrayRouteTbl[uiCount].ulFlags,
            &uiRef,
            &uiUse,
            &pstRouteTbl->astArrayRouteTbl[uiCount].uiMetric,
            &pstRouteTbl->astArrayRouteTbl[uiCount].uiMask,
            &pstRouteTbl->astArrayRouteTbl[uiCount].uiMtu,
            &uiWindow,
            &uiIrtt);
        
        /*0.0.0.0 Ŀ�ĵ�ַΪĬ��·��*/
        if (  pstRouteTbl->astArrayRouteTbl[uiCount].uiDstip == 0 )
        {
            VOS_Mem_Copy_S((CHAR *)pstRouteEntry, sizeof(VOS_ROUTE_ENTRY_S),  (CHAR *)&pstRouteTbl->astArrayRouteTbl[uiCount], sizeof(VOS_ROUTE_ENTRY_S));
            
            fclose(fp);
            return VOS_OK;
        }
        
        uiCount++;
    }
    
    fclose(fp);
#endif
    return VOS_ERR;
}




/*****************************************************************************
 �� �� ��  : VOS_GetARPTable
 ��������  : ��ȡ����ARP��������
 �������  : VOS_ROUTE_ENTRY_S *pstRouteEntry  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��6��29��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32   VOS_GetCurrentARPTable(VOS_SYS_ARP_TBL_S *pstArpTbl)
{
    FILE *fp = NULL;
    CHAR acBuf[256] = {0};
    UINT32 uiCount = 0;
    
    CHAR   acMask[VOS_IPV4STR_MAXLEN]={0};
    
#if VOS_PLAT_LINUX

    if ( NULL == pstArpTbl )
    {
        return VOS_ERR;
    }
    
    fp = fopen("/proc/net/arp", "r");
    if ( NULL == fp )
    {
        return VOS_ERR;
    }

    /*��ȡ����*/
    fgets(acBuf, 256, fp);

    while(fgets(acBuf, 256, fp))
    {

        /*ע��: һ��Ҫ08x, ����IP�ַ�������*/
        sscanf(acBuf, "%s%08x%08x%s%s%s",
            pstArpTbl->astArrayArpTbl[uiCount].acIpAddr,
            &pstArpTbl->astArrayArpTbl[uiCount].uiHwType,
            &pstArpTbl->astArrayArpTbl[uiCount].uiFlags,
            pstArpTbl->astArrayArpTbl[uiCount].acMacAddr,
            acMask,
            pstArpTbl->astArrayArpTbl[uiCount].acDevName);
    }
    
    fclose(fp);
#endif
    return VOS_ERR;
}


/*****************************************************************************
 �� �� ��  : VOS_GetInnerIPMacAddr
 ��������  : ͨ��IP��ȡMac��Ϣ
 �������  : VOS_IPADDR_T uiIPAddr  
             UCHAR *pcMacAddr       
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2017��1��11��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32   VOS_GetInnerIPMacAddr(VOS_IPADDR_T uiIPAddr, UCHAR *pcMacAddr)
{
    FILE *fp = NULL;
    CHAR acBuf[256] = {0};
    UINT32 uiCount = 0;
    VOS_SYS_ARP_TBL_S   stArpTable = {{{{0}}}};
    VOS_SYS_ARP_TBL_S *pstArpTbl   =  &stArpTable;    /*��������linux�澯д��*/
    CHAR   acMask[VOS_IPV4STR_MAXLEN]={0};
    struct in_addr stIpaddr = {0};
    
#if VOS_PLAT_LINUX

    if ( NULL == pcMacAddr )
    {
        return VOS_ERR;
    }
    
    fp = fopen("/proc/net/arp", "r");
    if ( NULL == fp )
    {
        return VOS_ERR;
    }

    /*��ȡ����*/
    fgets(acBuf, 256, fp);

    while(fgets(acBuf, 256, fp))
    {

        /*ע��: һ��Ҫ08x, ����IP�ַ�������*/
        sscanf(acBuf, "%s%08x%08x%s%s%s",
            pstArpTbl->astArrayArpTbl[uiCount].acIpAddr,
            &pstArpTbl->astArrayArpTbl[uiCount].uiHwType,
            &pstArpTbl->astArrayArpTbl[uiCount].uiFlags,
            pstArpTbl->astArrayArpTbl[uiCount].acMacAddr,
            acMask,
            pstArpTbl->astArrayArpTbl[uiCount].acDevName);

         /*ͨ����ַ�ж��Ƿ����*/
        if (  inet_aton(pstArpTbl->astArrayArpTbl[uiCount].acIpAddr, &stIpaddr) == uiIPAddr )
        {
            VOS_Mem_Copy_S((CHAR *)pcMacAddr, 
                    32, 
                    pstArpTbl->astArrayArpTbl[uiCount].acMacAddr, 
                    VOS_StrLen(pstArpTbl->astArrayArpTbl[uiCount].acMacAddr));
            
            fclose(fp);
            return VOS_OK;
        }
        
    }
    
    fclose(fp);
#endif
    return VOS_ERR;
}

/*****************************************************************************
 �� �� ��  : VOS_GetDefaultGateway
 ��������  : ��ȡĬ�����ص�ַ
 �������  : VOS_IPADDR_T *puiDeaultGateway  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��6��29��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
INT32   VOS_GetDefaultGateway(VOS_IPADDR_T *puiDeaultGateway)
{
    FILE *fp = NULL;
    CHAR acBuf[256] = {0};
    UINT32 uiCount = 0;
    UINT32 uiRef = 0;
    UINT32 uiUse = 0;
    UINT32 uiWindow =0;
    UINT32 uiIrtt = 0;
    VOS_SYS_ROUTE_TBL_S   stRouteTable ={{{0}}};
    VOS_SYS_ROUTE_TBL_S *pstRouteTbl =  &stRouteTable;
#if VOS_PLAT_LINUX

    if ( NULL == puiDeaultGateway )
    {
        return VOS_ERR;
    }
    
    fp = fopen("/proc/net/route", "r");
    if ( NULL == fp )
    {
        return VOS_ERR;
    }

    /*��ȡ����*/
    fgets(acBuf, 256, fp);

    while(fgets(acBuf, 256, fp))
    {
        
        sscanf(acBuf, "%s%08x%08x%04x%04x%02x%02x%08x%02x%02x%02x",
            pstRouteTbl->astArrayRouteTbl[uiCount].acDevName,
            &pstRouteTbl->astArrayRouteTbl[uiCount].uiDstip,
            &pstRouteTbl->astArrayRouteTbl[uiCount].uiGateway,
            &pstRouteTbl->astArrayRouteTbl[uiCount].ulFlags,
            &uiRef,
            &uiUse,
            &pstRouteTbl->astArrayRouteTbl[uiCount].uiMetric,
            &pstRouteTbl->astArrayRouteTbl[uiCount].uiMask,
            &pstRouteTbl->astArrayRouteTbl[uiCount].uiMtu,
            &uiWindow,
            &uiIrtt);
        
        /*0.0.0.0 Ŀ�ĵ�ַΪĬ��·��*/
        if (  pstRouteTbl->astArrayRouteTbl[uiCount].uiDstip == 0 )
        {
            *puiDeaultGateway = pstRouteTbl->astArrayRouteTbl[uiCount].uiGateway;
            fclose(fp);
            return VOS_OK;
        }
        
        uiCount++;
    }
    
    fclose(fp);
    #endif
    return VOS_ERR;
}


/*****************************************************************************
 �� �� ��  : VOS_GetDstIpBestRoute
 ��������  : ��ȡĿ�ĵ�ַ�����·��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��6��30��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VOS_GetDstIpBestRoute(VOS_IPADDR_T uiDstIp, VOS_ROUTE_ENTRY_S *pstRouteEntry)
{
    FILE *fp = NULL;
    CHAR acBuf[256] = {0};
    UINT32 uiCount = 0;
    UINT32 uiFound = 0;
    UINT32 uiRef = 0;
    UINT32 uiUse = 0;
    UINT32 uiWindow =0;
    UINT32 uiIrtt = 0;
    VOS_SYS_ROUTE_TBL_S stRouteTbl = {{{0}}};
    VOS_SYS_ROUTE_TBL_S *pstRouteTbl = &stRouteTbl;
#if VOS_PLAT_LINUX

    if ( NULL == pstRouteEntry )
    {
        return VOS_ERR;
    }
    
    fp = fopen("/proc/net/route", "r");
    if ( NULL == fp )
    {
        return VOS_ERR;
    }

    /*��ȡ����*/
    fgets(acBuf, 256, fp);

    while(fgets(acBuf, 256, fp))
    {
        
        sscanf(acBuf, "%s%08x%08x%04x%04x%02x%02x%08x%02x%02x%02x",
            pstRouteTbl->astArrayRouteTbl[uiCount].acDevName,
            &pstRouteTbl->astArrayRouteTbl[uiCount].uiDstip,
            &pstRouteTbl->astArrayRouteTbl[uiCount].uiGateway,
            &pstRouteTbl->astArrayRouteTbl[uiCount].ulFlags,
            &uiRef,
            &uiUse,
            &pstRouteTbl->astArrayRouteTbl[uiCount].uiMetric,
            &pstRouteTbl->astArrayRouteTbl[uiCount].uiMask,
            &pstRouteTbl->astArrayRouteTbl[uiCount].uiMtu,
            &uiWindow,
            &uiIrtt);

        /*�ҵ��ӿ�·��*/
        if ( (uiDstIp & pstRouteTbl->astArrayRouteTbl[uiCount].uiMask) == 
            ( pstRouteTbl->astArrayRouteTbl[uiCount].uiDstip & pstRouteTbl->astArrayRouteTbl[uiCount].uiMask) )
        {
            uiFound = 1;

            /*�ж�·�ɱ���·�����أ������0����ʾҲ����Ĭ������*/
            if ( 0 == pstRouteTbl->astArrayRouteTbl[uiCount].uiGateway )
            {
                VOS_GetDefaultGatewayRouteEntry(pstRouteEntry);
                break;
            }
            
            /*��������*/
            VOS_Mem_Copy_S((CHAR *)pstRouteEntry, 
                        sizeof(VOS_ROUTE_ENTRY_S), 
                        (CHAR *)&pstRouteTbl->astArrayRouteTbl[uiCount], 
                        sizeof(VOS_ROUTE_ENTRY_S));

            break;
        }
        
        uiCount++;
        
    }

    /*û�з��־ͻ�ȡĬ��·��*/
    if ( 0 ==  uiFound )
    {
          VOS_GetDefaultGatewayRouteEntry(pstRouteEntry);
    }
    
    fclose(fp);
#endif
    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : VOS_AddRouteEntry
 ��������  : ���·��
 �������  :   
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��6��30��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VOS_RouteEntryAdd( VOS_IPADDR_T uiDstip,
                                                   VOS_IPADDR_T uiMask,
                                                   VOS_IPADDR_T uiGateway,
                                                   UINT32   uiMetric,
                                                   CHAR *pcifName)
{
#if VOS_PLAT_LINUX

    struct  sockaddr_in *pstSockaddr = NULL;
    struct  rtentry         stRouteEntry = {0};
    INT32   iSockfd = -1;

    if ( pcifName == NULL  )
    {
        return VOS_ERR;
    }
    
    iSockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if ( 0 >= iSockfd )
    {
        return VOS_ERR;
    }

    stRouteEntry.rt_flags = RTF_UP;

    /*����Ŀ�ĵ�ַ*/
    pstSockaddr = (struct sockaddr_in *)(&(stRouteEntry.rt_dst));
    pstSockaddr->sin_family = AF_INET;
    pstSockaddr->sin_port = 0;
    pstSockaddr->sin_addr.s_addr = uiDstip;

    /*��������
     ע��: Ĭ��·���£�������Host���*/
    if ( (0 == uiDstip )
        || ((0 != uiMask) && (0xFFFFFFFF != uiMask)) )
    {
        pstSockaddr = (struct sockaddr_in *)(&(stRouteEntry.rt_genmask));
        pstSockaddr->sin_family = AF_INET;
        pstSockaddr->sin_port = 0;
        pstSockaddr->sin_addr.s_addr = uiMask;
    }
    else
    {
        stRouteEntry.rt_flags |= RTF_HOST;
    }

    /*��������*/
    if ( 0 != uiGateway  )
    {
        pstSockaddr = (struct sockaddr_in *)(&(stRouteEntry.rt_gateway));
        pstSockaddr->sin_family = AF_INET;
        pstSockaddr->sin_port = 0;
        pstSockaddr->sin_addr.s_addr = uiGateway;
        stRouteEntry.rt_flags |= RTF_GATEWAY;
    }

    /*����MTU*/
    /*stRouteEntry.rt_flags |= RTF_MTU;
       stRouteEntry.rt_mtu = uiMtu;
   */

    /*�������ȼ�*/
    if ( uiMetric > 0)
    {
        stRouteEntry.rt_metric = uiMetric;
    }

    if ( VOS_StrLen(pcifName) > 0)
    {
        stRouteEntry.rt_dev = pcifName;
    }

    if ( 0 != ioctl(iSockfd, SIOCADDRT, (void *)&stRouteEntry) )
    {
        close(iSockfd);
        return VOS_ERR;
    }

    close(iSockfd);
#endif
    return VOS_OK;
}

/*****************************************************************************
 �� �� ��  : VOS_DelRouteEntry
 ��������  : ɾ��ĳ��·��
 �������  : VOS_IPADDR_T uiDstip    
             VOS_IPADDR_T uiMask     
             VOS_IPADDR_T uiGateway  
             UINT32   uiMetric       
             CHAR *pcifName          
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��6��30��
    ��    ��   : jimk
    �޸�����   : �����ɺ���

*****************************************************************************/
LONG VOS_RouteEntryDel( VOS_IPADDR_T uiDstip,
                                                   VOS_IPADDR_T uiMask,
                                                   VOS_IPADDR_T uiGateway)
{
#if VOS_PLAT_LINUX

    struct  sockaddr_in *pstSockaddr = NULL;
    struct  rtentry         stRouteEntry = {0};
    INT32   iSockfd = -1;

    
    iSockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if ( 0 >= iSockfd )
    {
        return VOS_ERR;
    }

    stRouteEntry.rt_flags = RTF_UP;

    /*����Ŀ�ĵ�ַ*/
    pstSockaddr = (struct sockaddr_in *)(&(stRouteEntry.rt_dst));
    pstSockaddr->sin_family = AF_INET;
    pstSockaddr->sin_port = 0;
    pstSockaddr->sin_addr.s_addr = uiDstip;

    /*��������
     ע��: Ĭ��·���£�������Host���*/
    if ( (0 == uiDstip )
        || ((0 != uiMask) && (0xFFFFFFFF != uiMask)) )
    {
        pstSockaddr = (struct sockaddr_in *)(&(stRouteEntry.rt_genmask));
        pstSockaddr->sin_family = AF_INET;
        pstSockaddr->sin_port = 0;
        pstSockaddr->sin_addr.s_addr = uiMask;
    }
    else
    {
        stRouteEntry.rt_flags |= RTF_HOST;
    }

    /*��������*/
    if ( 0 != uiGateway  )
    {
        pstSockaddr = (struct sockaddr_in *)(&(stRouteEntry.rt_gateway));
        pstSockaddr->sin_family = AF_INET;
        pstSockaddr->sin_port = 0;
        pstSockaddr->sin_addr.s_addr = uiGateway;
        stRouteEntry.rt_flags |= RTF_GATEWAY;
    }

    /*ɾ��ʱ�����ȼ�����Ϊ0*/
    stRouteEntry.rt_metric = 0;
   
    if ( 0 != ioctl(iSockfd, SIOCDELRT, (void *)&stRouteEntry) )
    {
        close(iSockfd);
        return VOS_ERR;
    }

    close(iSockfd);
#endif
    return VOS_OK;
}


