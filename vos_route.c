/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_route.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年6月28日
  最近修改   :
  功能描述   : 系统路由相关
  函数列表   :
  修改历史   :
  1.日    期   : 2016年6月28日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/


/*****************************************************************************
 函 数 名  : VOS_GetCurrentRouteTable
 功能描述  : 获取当前系统的路由表
 输入参数  : VOS_SYS_ROUTE_TBL_S *pstRouteTbl  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月28日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
#include <vos_pub.h>
#include <vos_log.h>


/*获取系统所有的路由表信息
注意 : 有一种情况，可能同一个路由有多条，就是优先级不同
那么还要判断谁的优先级最小，越小的优先级越高*/
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

    /*获取首行*/
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
 函 数 名  : VOS_GetDefaultRouteEntry
 功能描述  : 获取默认路由
 输入参数  : VOS_ROUTE_ENTRY_S *pstRouteEntry  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月29日
    作    者   : jimk
    修改内容   : 新生成函数

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

    /*获取首行*/
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
        
        /*0.0.0.0 目的地址为默认路由*/
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
 函 数 名  : VOS_GetARPTable
 功能描述  : 获取本机ARP内网表项
 输入参数  : VOS_ROUTE_ENTRY_S *pstRouteEntry  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月29日
    作    者   : jimk
    修改内容   : 新生成函数

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

    /*获取首行*/
    fgets(acBuf, 256, fp);

    while(fgets(acBuf, 256, fp))
    {

        /*注意: 一定要08x, 否则IP字符串出错*/
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
 函 数 名  : VOS_GetInnerIPMacAddr
 功能描述  : 通过IP获取Mac信息
 输入参数  : VOS_IPADDR_T uiIPAddr  
             UCHAR *pcMacAddr       
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年1月11日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
INT32   VOS_GetInnerIPMacAddr(VOS_IPADDR_T uiIPAddr, UCHAR *pcMacAddr)
{
    FILE *fp = NULL;
    CHAR acBuf[256] = {0};
    UINT32 uiCount = 0;
    VOS_SYS_ARP_TBL_S   stArpTable = {{{{0}}}};
    VOS_SYS_ARP_TBL_S *pstArpTbl   =  &stArpTable;    /*消除部分linux告警写法*/
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

    /*获取首行*/
    fgets(acBuf, 256, fp);

    while(fgets(acBuf, 256, fp))
    {

        /*注意: 一定要08x, 否则IP字符串出错*/
        sscanf(acBuf, "%s%08x%08x%s%s%s",
            pstArpTbl->astArrayArpTbl[uiCount].acIpAddr,
            &pstArpTbl->astArrayArpTbl[uiCount].uiHwType,
            &pstArpTbl->astArrayArpTbl[uiCount].uiFlags,
            pstArpTbl->astArrayArpTbl[uiCount].acMacAddr,
            acMask,
            pstArpTbl->astArrayArpTbl[uiCount].acDevName);

         /*通过地址判断是否相等*/
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
 函 数 名  : VOS_GetDefaultGateway
 功能描述  : 获取默认网关地址
 输入参数  : VOS_IPADDR_T *puiDeaultGateway  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月29日
    作    者   : jimk
    修改内容   : 新生成函数

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

    /*获取首行*/
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
        
        /*0.0.0.0 目的地址为默认路由*/
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
 函 数 名  : VOS_GetDstIpBestRoute
 功能描述  : 获取目的地址的最佳路由
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月30日
    作    者   : jimk
    修改内容   : 新生成函数

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

    /*获取首行*/
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

        /*找到接口路由*/
        if ( (uiDstIp & pstRouteTbl->astArrayRouteTbl[uiCount].uiMask) == 
            ( pstRouteTbl->astArrayRouteTbl[uiCount].uiDstip & pstRouteTbl->astArrayRouteTbl[uiCount].uiMask) )
        {
            uiFound = 1;

            /*判断路由表中路由网关，如果是0，表示也采用默认网关*/
            if ( 0 == pstRouteTbl->astArrayRouteTbl[uiCount].uiGateway )
            {
                VOS_GetDefaultGatewayRouteEntry(pstRouteEntry);
                break;
            }
            
            /*拷贝表项*/
            VOS_Mem_Copy_S((CHAR *)pstRouteEntry, 
                        sizeof(VOS_ROUTE_ENTRY_S), 
                        (CHAR *)&pstRouteTbl->astArrayRouteTbl[uiCount], 
                        sizeof(VOS_ROUTE_ENTRY_S));

            break;
        }
        
        uiCount++;
        
    }

    /*没有发现就获取默认路由*/
    if ( 0 ==  uiFound )
    {
          VOS_GetDefaultGatewayRouteEntry(pstRouteEntry);
    }
    
    fclose(fp);
#endif
    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : VOS_AddRouteEntry
 功能描述  : 添加路由
 输入参数  :   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月30日
    作    者   : jimk
    修改内容   : 新生成函数

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

    /*设置目的地址*/
    pstSockaddr = (struct sockaddr_in *)(&(stRouteEntry.rt_dst));
    pstSockaddr->sin_family = AF_INET;
    pstSockaddr->sin_port = 0;
    pstSockaddr->sin_addr.s_addr = uiDstip;

    /*设置掩码
     注意: 默认路由下，不设置Host标记*/
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

    /*设置网关*/
    if ( 0 != uiGateway  )
    {
        pstSockaddr = (struct sockaddr_in *)(&(stRouteEntry.rt_gateway));
        pstSockaddr->sin_family = AF_INET;
        pstSockaddr->sin_port = 0;
        pstSockaddr->sin_addr.s_addr = uiGateway;
        stRouteEntry.rt_flags |= RTF_GATEWAY;
    }

    /*设置MTU*/
    /*stRouteEntry.rt_flags |= RTF_MTU;
       stRouteEntry.rt_mtu = uiMtu;
   */

    /*设置优先级*/
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
 函 数 名  : VOS_DelRouteEntry
 功能描述  : 删除某条路由
 输入参数  : VOS_IPADDR_T uiDstip    
             VOS_IPADDR_T uiMask     
             VOS_IPADDR_T uiGateway  
             UINT32   uiMetric       
             CHAR *pcifName          
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月30日
    作    者   : jimk
    修改内容   : 新生成函数

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

    /*设置目的地址*/
    pstSockaddr = (struct sockaddr_in *)(&(stRouteEntry.rt_dst));
    pstSockaddr->sin_family = AF_INET;
    pstSockaddr->sin_port = 0;
    pstSockaddr->sin_addr.s_addr = uiDstip;

    /*设置掩码
     注意: 默认路由下，不设置Host标记*/
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

    /*设置网关*/
    if ( 0 != uiGateway  )
    {
        pstSockaddr = (struct sockaddr_in *)(&(stRouteEntry.rt_gateway));
        pstSockaddr->sin_family = AF_INET;
        pstSockaddr->sin_port = 0;
        pstSockaddr->sin_addr.s_addr = uiGateway;
        stRouteEntry.rt_flags |= RTF_GATEWAY;
    }

    /*删除时候，优先级设置为0*/
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


