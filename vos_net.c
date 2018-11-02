/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_ifnet.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年4月17日
  最近修改   :
  功能描述   : 网络接口
  函数列表   :
  修改历史   :
  1.日    期   : 2016年4月17日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
#include <vos/vos_pub.h>


typedef struct {
# define IPH_GET_VER(v) (((v) >> 4) & 0x0F)
# define IPH_GET_LEN(v) (((v) & 0x0F) << 2)
    UCHAR    version_len;

    UCHAR    tos;
    USHORT   tot_len;
    USHORT   id;

# define IPH_IP_OFFMASK 0x1fff
    USHORT   frag_off;

    UCHAR    ttl;

# define IPH_IPPROTO_UDP  17  /* UDP protocol */
# define IPH_IPPROTO_TCP   6  /* TCP protocol */
# define IPH_IPPROTO_ICMP  1  /* ICMP protocol */
# define IPH_IPPROTO_IGMP  2  /* IGMP protocol */
    UCHAR    protocol;

    USHORT   check;
    UINT32    saddr;
    UINT32    daddr;
/* The options start here. */
} IPHDR;

/*****************************************************************************
 函 数 名  : VOS_GetMACAddrByIfName
 功能描述  : 根据网卡名称获取网卡的Mac地址
 输入参数  : CHAR *pcIfName    
             UCHAR *pcMacAddr  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年10月23日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG VOS_GetMACAddrByIfName(CHAR *pcIfName, UCHAR *pcMacAddr)
{
    LONG    lSockfd     = 0;
#if VOS_PLAT_LINUX

    struct   ifreq          ifReq;

    if ( NULL == pcIfName 
        || NULL == pcMacAddr )
    {
        VOS_Printf("VOS_GetMACAddrByIfName param error!");
        return VOS_ERR;
    }

    if ( (lSockfd =socket(PF_INET, SOCK_DGRAM,0) ) == -1)
    {
        VOS_Printf("Ifnet name[%s] create socket error!", pcIfName);
        return VOS_ERR;
    }

    memset(&ifReq, 0, sizeof(ifReq));

    strcpy(ifReq.ifr_name, (const CHAR *)pcIfName);

    if ( ioctl(lSockfd, SIOCGIFHWADDR, (CHAR *)&ifReq) == -1)
    {
        VOS_Printf("Ifnet name[%s] get hwaddr error!", pcIfName);
        close(lSockfd);
        return VOS_ERR;
    }

    memcpy(pcMacAddr, ifReq.ifr_hwaddr.sa_data, VOS_IPV4MACADDR_LEN);
    
    close(lSockfd);
#endif
    return VOS_OK;
}


LONG VOS_GetIPMaskByIfName(CHAR *pcIfName, VOS_IPADDR_T *piIPMask)
{
    LONG    lSockfd     = 0;
#if VOS_PLAT_LINUX
    struct   ifreq          ifReq;
    struct   sockaddr_in *sin;

    if ( NULL == pcIfName 
        || NULL == piIPMask )
    {
        VOS_Printf("VOS_GetIPMaskByIfName param error!");
        return VOS_ERR;
    }

    if ( (lSockfd =socket(PF_INET, SOCK_DGRAM,0) ) == -1)
    {
        VOS_Printf("Ifnet name[%s] create socket error!", pcIfName);
        return VOS_ERR;
    }

    memset(&ifReq, 0, sizeof(ifReq));

    strcpy(ifReq.ifr_name, (const CHAR *)pcIfName);

    ifReq.ifr_addr.sa_family = PF_INET;
    if ( ioctl ( lSockfd,SIOCGIFNETMASK, ( char * ) &ifReq ) ==-1 )
    {
        VOS_Printf ("VOS_GetIPMaskByIfName-->ioctl SIOCGIFNETMASK:[%s], ifname=[%s]",strerror ( errno ), pcIfName);
        close ( lSockfd );
        return VOS_ERR;
    }
    
    sin = ( struct sockaddr_in * ) &ifReq.ifr_addr;
    memcpy ( piIPMask, ( char * ) &sin->sin_addr,  VOS_IPV4ADDR_LEN);
    
    close(lSockfd);
#endif

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : VOS_GetIndexByIfName
 功能描述  : 获取接口索引号
 输入参数  : CHAR *pcIfName          
             VOS_IPADDR_T *piIPMask  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月29日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG VOS_GetIndexByIfName(CHAR *pcIfName, UINT32 *puiIndex)
{
    LONG    lSockfd     = 0;
#if VOS_PLAT_LINUX
    struct   ifreq          ifReq;

    if ( NULL == pcIfName  )
    {
        VOS_Printf("VOS_GetIndexByIfName param error!");
        return VOS_ERR;
    }

    if ( (lSockfd =socket(PF_INET, SOCK_DGRAM,0) ) == -1)
    {
        VOS_Printf("Ifnet name[%s] create socket error!", pcIfName);
        return VOS_ERR;
    }

    memset(&ifReq, 0, sizeof(ifReq));

    strcpy(ifReq.ifr_name, (const CHAR *)pcIfName);

    ifReq.ifr_addr.sa_family = PF_INET;
    if ( ioctl ( lSockfd,SIOCGIFINDEX, ( char * ) &ifReq ) ==-1 )
    {
        VOS_Printf ("VOS_GetIndexByIfName-->ioctl SIOCGIFINDEX:[%s], ifname:[%s]\n",strerror ( errno ), pcIfName);
        close ( lSockfd );
        return VOS_ERR;
    }

    *puiIndex = ifReq.ifr_ifindex;
    
    close(lSockfd);
#endif
    return VOS_OK;
}

/*最多获取4个*/
LONG VOS_GetDNSServerIp(VOS_IPADDR_T uiDNSSev[4])
{
    FILE *fp = NULL;
    CHAR acLine[256]={0};
    CHAR *ptr = NULL;
    UINT32 uiCount = 0;
    
#if VOS_PLAT_LINUX
    if ( (fp  = fopen("/etc/resolv.conf", "r")) == NULL )
    {
        uiDNSSev[0] = 0;
        uiDNSSev[1] = 0;
        uiDNSSev[2] = 0;
        uiDNSSev[3] = 0;
        
        return VOS_ERR;
    }

    /*循环获取行内容*/
    while(fgets(acLine, 256, fp ))
    {
        /*跳过注释行*/
        if ( acLine[0] == '#' )
        {
            continue;
        }

        if ( strncmp(acLine, "nameserver", 10) == 0  )
        {
            /*将空格之前的字符串指向ptr， 即提取空格之前的字符串*/
            ptr = strtok(acLine, " ");
            if ( NULL != ptr )
            {
                /*将空格之后的字符串指向ptr*/
                ptr = strtok(NULL, " ");
                if ( NULL != ptr )
                {
                    uiDNSSev[uiCount] = VOS_IPV4StrToN(ptr);
                    uiCount++;
                }
            }
        }

        /*就记录前面4个*/
        if ( uiCount >= 4 )
        {
            break;
        }
    }

#endif
    return VOS_OK;
}


LONG VOS_GetIPAddrByIfName(CHAR *pcIfName, VOS_IPADDR_T *piIPAddr)
{
    LONG    lSockfd     = 0;
#if VOS_PLAT_LINUX

    struct   ifreq          ifReq;
    struct   sockaddr_in *sin;

    if ( NULL == pcIfName 
        || NULL == piIPAddr )
    {
        VOS_Printf("VOS_GetIPAddrByIfName param error!");
        return VOS_ERR;
    }

    if ( (lSockfd =socket(PF_INET, SOCK_DGRAM,0) ) == -1)
    {
        VOS_Printf("Ifnet name[%s] create socket error!", pcIfName);
        return VOS_ERR;
    }

    memset(&ifReq, 0, sizeof(ifReq));

    strcpy(ifReq.ifr_name, (const CHAR *)pcIfName);

    ifReq.ifr_addr.sa_family = PF_INET;
    if ( ioctl ( lSockfd,SIOCGIFADDR, ( char * ) &ifReq ) ==-1 )
    {
        VOS_Printf ("VOS_GetIPAddrByIfName-->ioctl SIOCGIFADDR:[%s], ifname:[%s]\n",strerror ( errno ), pcIfName);
        close ( lSockfd );
        return VOS_ERR;
    }
    
    sin = ( struct sockaddr_in * ) &ifReq.ifr_addr;
    memcpy ( piIPAddr, ( char * ) &sin->sin_addr,  VOS_IPV4ADDR_LEN);
    
    close(lSockfd);
#endif
    return VOS_OK;
}


LONG VOS_GetSystemInterfaceNums()
{
    LONG lRet = 0;
#if VOS_PLAT_LINUX
        struct ifreq ifr;
        struct ifconf ifc;
        CHAR acBuf[2048] = {0};
       
        int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
        if (sock == -1)
        {
            return -1;
        }

        ifc.ifc_len = sizeof(acBuf);
        ifc.ifc_buf = acBuf;
        if( ioctl(sock, SIOCGIFCONF, &ifc) == -1)
        {
            return -1;
        }

        struct ifreq *it = ifc.ifc_req;
        const struct ifreq * const end = it + (ifc.ifc_len / sizeof(struct ifreq));
        char szMac[64];
        int count = 0;
        for (; it != end; ++it) {
            strcpy(ifr.ifr_name, it->ifr_name);
            if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
                if (! (ifr.ifr_flags & IFF_LOOPBACK)) { // don't count loopback
                    if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
                        count ++ ;
                        unsigned char * ptr ;
                        ptr = (unsigned char  *)&ifr.ifr_ifru.ifru_hwaddr.sa_data[0];
                        snprintf(szMac,64,"%02X:%02X:%02X:%02X:%02X:%02X",*ptr,*(ptr+1),*(ptr+2),*(ptr+3),*(ptr+4),*(ptr+5));
                        //printf("%d,Interface name : %s , Mac address : %s \n",count,ifr.ifr_name, szMac);
                        lRet++;
                    }
                }
            }else{
                //printf("get mac info error\n");
                return -1;
            }
        }
#elif VOS_PLAT_WIN
    
#endif

    return lRet;
}



// Calculate the checksum, 网络序
USHORT VOS_IPV4Checksum(void *buf, UINT32 size)
{
	int sum = 0;
	USHORT *addr = (USHORT *)buf;
	int len = (int)size;
	USHORT *w = addr;
	int nleft = len;
	USHORT answer = 0;

	while (nleft > 1)
	{
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1)
	{
		*(UCHAR *)(&answer) = *(UCHAR *)w;
		sum += answer;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);

	answer = (USHORT)~sum;

	return answer;
}

/*****************************************************************************
 函 数 名  : VOS_IPCheckSumCalc
 功能描述  : checksum具体算法
 输入参数  : VOID *dataptr  
             UINT32 len     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年1月11日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
USHORT VOS_IPCheckSumCalc(VOID *dataptr, UINT32 len)
{
    unsigned int    acc;
    unsigned short  src;
    unsigned char   *octetptr;

    acc = 0;
    /* dataptr may be at odd or even addresses */
    octetptr = (unsigned char *)dataptr;
    
    while (len > 1) {
        /* declare first octet as most significant
        *        thus assume network order, ignoring host order */
        src = (*octetptr) << 8;
        octetptr++;
        /* declare second octet as least significant */
        src |= (*octetptr);
        octetptr++;
        acc += src;
        len -= 2;
    }
    if (len > 0) {
        /* accumulate remaining octet */
        src = (*octetptr) << 8;
        acc += src;
    }
    /* add deferred carry bits */
    acc = (acc >> 16) + (acc & 0x0000ffffUL);
    if ((acc & 0xffff0000UL) != 0) {
        acc = (acc >> 16) + (acc & 0x0000ffffUL);
    }
    /* This maybe a little confusing: reorder sum using htons()
    *      instead of ntohs() since it has a little less call overhead.
    *           The caller must invert bits for Internet sum ! */
    return htons((unsigned short)acc);
}

/*****************************************************************************
 函 数 名  : VOS_InetIPCheckSum
 功能描述  : 计算该包的checksum
 输入参数  : CHAR *pcPacket  
             UINT32 iLen     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年1月11日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
USHORT VOS_InetCheckSum(CHAR *pcPacket, UINT32 iLen)
{
    USHORT uiResult = 0;
    
    uiResult = ~VOS_IPCheckSumCalc((void *)pcPacket, iLen);

    return uiResult;
}

/*****************************************************************************
 函 数 名  : VOS_IPCheckSum
 功能描述  : 计算该IP头的checksum
 输入参数  :  CHAR *pcPacket  ---NAT将一个IP头传入，得出该IP的校验和
                           UINT32 iLen     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年1月11日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
USHORT VOS_IPCheckSum(CHAR *pcPacket, UINT32 iLen)
{
    IPHDR *pstIpHdr = NULL;
    IPHDR  stIP = {0};
    USHORT usResult = 0;
    
    pstIpHdr = (IPHDR *)pcPacket;

    VOS_Mem_Copy((CHAR *)&stIP, (CHAR *)pstIpHdr,  sizeof(IPHDR));

    /*IP报文要先将校验和清零*/
    stIP.check = 0;
    /*计算IP头部校验和*/
    usResult = VOS_InetCheckSum((CHAR *)&stIP, sizeof(IPHDR));

    return usResult;
}

/*****************************************************************************
 函 数 名  : VOS_GetSystemInterfaceName
 功能描述  : 获取系统的网卡信息名称
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年6月19日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
LONG VOS_GetSystemInterfaceName(CHAR acIfName[8][32], LONG *plNums)
{
#if VOS_PLAT_LINUX
    struct ifreq ifr;
    struct ifconf ifc;
    CHAR acBuf[2048] = {0};

    if ( plNums == NULL)
    {
        return -1;
    }
   
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock == -1)
    {
        return -1;
    }

    ifc.ifc_len = sizeof(acBuf);
    ifc.ifc_buf = acBuf;
    if( ioctl(sock, SIOCGIFCONF, &ifc) == -1)
    {
        return -1;
    }

    struct ifreq *it = ifc.ifc_req;
    const struct ifreq * const end = it + (ifc.ifc_len / sizeof(struct ifreq));
    char szMac[64];
    int count = 0;
    for (; it != end; ++it) {
        strcpy(ifr.ifr_name, it->ifr_name);
        if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
            if (! (ifr.ifr_flags & IFF_LOOPBACK)) { // don't count loopback
                if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
                    unsigned char * ptr ;
                    ptr = (unsigned char  *)&ifr.ifr_ifru.ifru_hwaddr.sa_data[0];
                    snprintf(szMac,64,"%02X:%02X:%02X:%02X:%02X:%02X",*ptr,*(ptr+1),*(ptr+2),*(ptr+3),*(ptr+4),*(ptr+5));
                    //printf("%d,Interface name : %s , Mac address : %s \n",count,ifr.ifr_name, szMac);
                    memcpy(acIfName[count], ifr.ifr_name,  strlen(ifr.ifr_name));
                    
                    count ++ ;
                    if ( count > 3  )
                    {
                        *plNums = count;
                        return VOS_OK;
                    }
                }
            }
        }else{
            //printf("get mac info error\n");
            *plNums = 0;
            return -1;
        }
    }
#elif VOS_PLAT_WIN
        
#endif
    *plNums = count;
    return VOS_OK;
}

#define BUFSIZE 8192 

struct route_info{   
 u_int dstAddr;   
 u_int srcAddr;   
 u_int gateWay;   
 char ifName[IF_NAMESIZE];   
};   

int readNlSock(int sockFd, char *bufPtr, int seqNum, int pId)   
{   
  int readLen = 0, msgLen = 0;   
#if VOS_PLAT_LINUX

  struct nlmsghdr *nlHdr;   
  do{   
    //收到内核的应答   
    if((readLen = recv(sockFd, bufPtr, BUFSIZE - msgLen, 0)) < 0)   
    {   
      perror("SOCK READ: ");   
      return -1;   
    }   
      
    nlHdr = (struct nlmsghdr *)bufPtr;   
    //检查header是否有效   
    if((NLMSG_OK(nlHdr, readLen) == 0) || (nlHdr->nlmsg_type == NLMSG_ERROR))   
    {   
      perror("Error in recieved packet");   
      return -1;   
    }   
      
       
    if(nlHdr->nlmsg_type == NLMSG_DONE)    
    {   
      break;   
    }   
    else   
    {   
         
      bufPtr += readLen;   
      msgLen += readLen;   
    }   
      
       
    if((nlHdr->nlmsg_flags & NLM_F_MULTI) == 0)    
    {   
         
     break;   
    }   
  } while((nlHdr->nlmsg_seq != seqNum) || (nlHdr->nlmsg_pid != pId));   
  #endif
  return msgLen;   
}   

#if 0
//分析返回的路由信息   
void parseRoutes(struct nlmsghdr *nlHdr, struct route_info *rtInfo,char *gateway)   
{   
#if VOS_PLAT_LINUX

  struct rtmsg *rtMsg;   
  struct rtattr *rtAttr;   
  int rtLen;   
  char *tempBuf = NULL;   
  struct in_addr dst;
  
  struct in_addr uigateway;   

     
  tempBuf = (char *)malloc(100);   
  rtMsg = (struct rtmsg *)NLMSG_DATA(nlHdr);   
  // If the route is not for AF_INET or does not belong to main routing table   
  //then return.    
  if((rtMsg->rtm_family != AF_INET) || (rtMsg->rtm_table != RT_TABLE_MAIN))   
  return;   
     
  rtAttr = (struct rtattr *)RTM_RTA(rtMsg);   
  rtLen = RTM_PAYLOAD(nlHdr);   
  for(;RTA_OK(rtAttr,rtLen);rtAttr = RTA_NEXT(rtAttr,rtLen)){   
   switch(rtAttr->rta_type) {   
   case RTA_OIF:   
    if_indextoname(*(int *)RTA_DATA(rtAttr), rtInfo->ifName);   
    break;   
   case RTA_GATEWAY:   
    rtInfo->gateWay = *(u_int *)RTA_DATA(rtAttr);   
    break;   
   case RTA_PREFSRC:   
    rtInfo->srcAddr = *(u_int *)RTA_DATA(rtAttr);   
    break;   
   case RTA_DST:   
    rtInfo->dstAddr = *(u_int *)RTA_DATA(rtAttr);   
    break;   
   }   
  }   
  dst.s_addr = rtInfo->dstAddr;   
  if (strstr((char *)inet_ntoa(dst), "0.0.0.0"))   
  {   
    //printf("oif:%s",rtInfo->ifName);   
    uigateway.s_addr = rtInfo->gateWay;   
//    sprintf(gateway, (char *)inet_ntoa(gate));   
    //printf("%s\n",gateway);   
    uigateway.s_addr = rtInfo->srcAddr;   
    //printf("src:%s\n",(char *)inet_ntoa(gate));   
    uigateway.s_addr = rtInfo->dstAddr;   
    //printf("dst:%s\n",(char *)inet_ntoa(gate));    
  }   
  free(tempBuf);  
#endif
  return;   
}   
#endif



   
/*将字符串形式的地址转化为网络序*/
INT32 VOS_IPV4StrToN(CHAR *pcStr)
{
   INT32 iRet = 0;
   CHAR acStr[VOS_IPV4STR_MAXLEN] = {0};
   CHAR *pcTmp = NULL;
   INT32 iCount = 0;
   INT32 iOffset = 0;
   INT32 iAddr = 0;
   INT32 iResult = 0;
   
   if ( NULL == pcStr )
   {
       return iRet;
   }

   if ( strlen(pcStr) > VOS_IPV4STR_MAXLEN )
   {
       return iRet;
   }

   memcpy(acStr, pcStr, strlen(pcStr));
   pcTmp = acStr;
   
   while(1)
   {
       if ( acStr[iCount] == '.' )
       {
           acStr[iCount] ='\0';
           iAddr = atoi(pcTmp);
           if ( iAddr > 255  )
           {
               return 0;
           }
           iRet =  iAddr << iOffset;
           iResult |= iRet;
           iOffset+=8;
           pcTmp = acStr + iCount + 1;
       }
       
       iCount++;
       
       if ( acStr[iCount] == '\0' )
       {
           iAddr = atoi(pcTmp);
           if ( iAddr > 255  )
           {
               return 0;
           }
           iRet =  iAddr << iOffset;
           iResult |= iRet;
           break;
       }
   }

   if ( iOffset != 24 )
   {
       return 0;
   }

   return iResult;
}

/*将网络序地址转换为字符串*/
INT32 VOS_NToIPV4Str(UINT32 iNAddr, CHAR *pcStr)
{
   INT32 iRet = 0;
   CHAR acStr[VOS_IPV4STR_MAXLEN] = {0};
   CHAR acAddr[VOS_IPV4STR_MAXLEN] = {0};
   INT32 iCount = 0;
   INT32 iOffset = 0;
   INT32 iAddr = 0;
   INT32 iIndex = 0;
   
   if ( NULL == pcStr 
       || 0 == iNAddr )
   {
       return VOS_ERR;
   }

   while(1)
   {
       iAddr = ( iNAddr >> iOffset ) & ( 0x000000ff );
#if VOS_PLAT_LINUX
       snprintf(acAddr, VOS_INT32STR_MAXLEN-1,"%d", iAddr);
       iRet = snprintf(acStr+iIndex, VOS_INT32STR_MAXLEN-1,"%s.", acAddr);
#elif VOS_PLAT_WIN
       sprintf_s(acAddr, VOS_INT32STR_MAXLEN-1,"%d", iAddr);
       iRet = snprintf(acStr+iIndex, VOS_INT32STR_MAXLEN-1,"%s.", acAddr);
#endif
       iIndex +=iRet;
       memset(acAddr,0,VOS_INT32STR_MAXLEN);
       iOffset+=8;
       iCount++;

       /*IPV4地址偏移3次*/
       if ( iCount >= 3 )
       {
           break;
       }
   }
   iAddr = ( iNAddr >> iOffset ) & ( 0x000000ff );
#if VOS_PLAT_LINUX
   snprintf(acAddr, VOS_INT32STR_MAXLEN-1,"%d", iAddr);
   iRet = snprintf(acStr+iIndex, VOS_INT32STR_MAXLEN-1,"%s", acAddr);
#elif VOS_PLAT_WIN
   sprintf_s(acAddr, VOS_INT32STR_MAXLEN-1,"%d", iAddr);
   iRet = snprintf(acStr+iIndex, VOS_INT32STR_MAXLEN-1,"%s.", acAddr);
#endif

   memset(pcStr, 0, VOS_IPV4STR_MAXLEN-1);
   memcpy(pcStr, acStr, strlen(acStr));
   
   return VOS_OK;
}


/*使用系统函数进行转化*/
INT32 VOS_IPAddrToN(CHAR *pcStr)
{
   INT32 iRet = 0;

   if ( NULL == pcStr )
   {
       return iRet;
   }
   iRet = inet_addr(pcStr);
       
   return iRet;
}

#if 0
LONG VOS_GetGateway(CHAR *pcGateway)   
{   
#if VOS_PLAT_LINUX

    struct nlmsghdr *nlMsg;   
    //struct rtmsg *rtMsg;   
    struct route_info *rtInfo;   
    char msgBuf[BUFSIZE];   

    int sock, len, msgSeq = 0;   

    if((sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE)) < 0)   
    {   
        //perror("Socket Creation: ");   
        return VOS_ERR;   
    }   


    memset(msgBuf, 0, BUFSIZE);   


    nlMsg = (struct nlmsghdr *)msgBuf;   
   // rtMsg = (struct rtmsg *)NLMSG_DATA(nlMsg);   


    nlMsg->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg)); // Length of message.   
    nlMsg->nlmsg_type = RTM_GETROUTE; // Get the routes from kernel routing table .   

    nlMsg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST; // The message is a request for dump.   
    nlMsg->nlmsg_seq = msgSeq++; // Sequence of the message packet.   
    nlMsg->nlmsg_pid = getpid(); // PID of process sending the request.   


    if(send(sock, nlMsg, nlMsg->nlmsg_len, 0) < 0){   
        printf("Write To Socket Failed…\n");   
        return VOS_ERR;   
    }   


    if((len = readNlSock(sock, msgBuf, msgSeq, getpid())) < 0) {   
    printf("Read From Socket Failed…\n");   
    return VOS_ERR;   
    }   

    rtInfo = (struct route_info *)malloc(sizeof(struct route_info));   
    for(;NLMSG_OK(nlMsg,len);nlMsg = NLMSG_NEXT(nlMsg,len)){   
    memset(rtInfo, 0, sizeof(struct route_info));   
    //parseRoutes(nlMsg, rtInfo, pcGateway);   
    }   
    free(rtInfo);   
    close(sock);   
    #endif
    return VOS_OK;
}   
   #endif
   
/*****************************************************************************
函 数 名  : VOS_atom
功能描述  : 将字符串的MAC地址，转换为网络序中的MAC字节
                         例如: "01:23:45:67:98:0a" --->0x01,0x23, 0x45...
输入参数  : CHAR *pcMacStr    
                         MACADDR *MacAddr  
输出参数  : 无
返 回 值  : 
调用函数  : 
被调函数  : 

修改历史      :
 1.日    期   : 2017年1月12日
   作    者   : jimk
   修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_atom(CHAR *pcMacStr, CHAR *MacAddr)
{
   INT32 i = 0;
   INT32 len = 0;
   INT32 index = 0;
   CHAR *pStart = NULL;
   CHAR *pTmp = NULL;
   UINT32 uiHex = 0;
   
   if ( NULL == pcMacStr 
        || NULL == MacAddr )
   {
       return;
   }
   len = VOS_StrLen(pcMacStr);
   
   pStart = pcMacStr;
   pTmp =  pcMacStr;
   
   for(i=0; i<len;i++)
   {
       pTmp++;
       
       /*找到冒号*/
       if ( *pTmp == ':' )
       {
           /*先截断，此时pStart就是二进制字符串*/
           *pTmp = '\0';

           /*转换，注意atoi只能转换10进制的字符串，不能转换16进制的字符串*/
           //uiHex = atoi(pStart);
           sscanf(pStart, "%02x", &uiHex);
           MacAddr[index] = uiHex;
           index++;
           printf("hex=%02x\n", uiHex);
           pStart = pTmp+1;
           /*将字符串还原*/
           *pTmp=':';
           pTmp++;
       }
   }

   if ( pTmp > pStart )
   {
       uiHex = atoi(pStart);
       MacAddr[index] = uiHex;
   }
   
   return;
}

/*将Mac地址转换为字符串00:00:00:00:00:00格式*/
CHAR *VOS_mtoa (const UCHAR *pucMacAddr)
{
    static CHAR pcBuffer[18] = {0};
    
    VOS_Mem_Zero(pcBuffer, sizeof (pcBuffer));
    
    VOS_Snprintf (pcBuffer, 18, "%02X:%02X:%02X:%02X:%02X:%02X",
              pucMacAddr[0],pucMacAddr[1],pucMacAddr[2],pucMacAddr[3],pucMacAddr[4],pucMacAddr[5]);
    
    return pcBuffer;
}

USHORT VOS_htons(USHORT usHValue)
{
    return htons(usHValue);
}

USHORT VOS_ntohs(USHORT usHValue)
{
    return ntohs(usHValue);
}

UINT32  VOS_htonl(UINT32 ulHValue)
{
    return htonl(ulHValue);
}

UINT32  VOS_ntohl(UINT32 ulHValue)
{
    return ntohl(ulHValue);
}

/*使用系统函数进行转化*/
UINT32 VOS_aton_s(CHAR *pcStr)
{
    UINT32 iRet = 0;

    if ( NULL == pcStr )
    {
        return iRet;
    }
    iRet = inet_addr(pcStr);
        
    return iRet;
}


/*将字符串形式的地址转化为网络序*/
UINT32 VOS_aton(const CHAR *pcStr)
{
    UINT32 iRet = 0;
    CHAR acStr[VOS_IPV4STR_MAXLEN] = {0};
    CHAR *pcTmp = NULL;
    UINT32 iCount = 0;
    UINT32 iOffset = 0;
    UINT32 iAddr = 0;
    UINT32 iResult = 0;
    
    if ( NULL == pcStr )
    {
        return iRet;
    }

    if ( strlen(pcStr) > VOS_IPV4STR_MAXLEN )
    {
        return iRet;
    }

    memcpy(acStr, pcStr, strlen(pcStr));
    pcTmp = acStr;
    
    while(1)
    {
        if ( acStr[iCount] == '.' )
        {
            acStr[iCount] ='\0';
            iAddr = atoi(pcTmp);
            if ( iAddr > 255  )
            {
                return 0;
            }
            iRet =  iAddr << iOffset;
            iResult |= iRet;
            iOffset+=8;
            pcTmp = acStr + iCount + 1;
        }
        
        iCount++;
        
        if ( acStr[iCount] == '\0' )
        {
            iAddr = atoi(pcTmp);
            if ( iAddr > 255  )
            {
                return 0;
            }
            iRet =  iAddr << iOffset;
            iResult |= iRet;
            break;
        }
    }

    if ( iOffset != 24 )
    {
        return 0;
    }

    return iResult;
}

/*将网络序地址转换为字符串*/
UINT32 VOS_ntoa(UINT32 iNAddr, CHAR *pcStr)
{
    UINT32 iRet = 0;
    CHAR acStr[VOS_IPV4STR_MAXLEN] = {0};
    CHAR acAddr[VOS_IPV4STR_MAXLEN] = {0};
    UINT32 iCount = 0;
    UINT32 iOffset = 0;
    UINT32 iAddr = 0;
    UINT32 iIndex = 0;
    
    if ( NULL == pcStr 
        || 0 == iNAddr )
    {
        return VOS_ERR;
    }

    while(1)
    {
        iAddr = ( iNAddr >> iOffset ) & ( 0x000000ff );
#if VOS_PLAT_LINUX
        snprintf(acAddr, VOS_INT32STR_MAXLEN-1,"%d", iAddr);
        iRet = snprintf(acStr+iIndex, VOS_INT32STR_MAXLEN-1,"%s.", acAddr);
#elif VOS_PLAT_WIN
        sprintf_s(acAddr, VOS_INT32STR_MAXLEN-1,"%d", iAddr);
        iRet = snprintf(acStr+iIndex, VOS_INT32STR_MAXLEN-1,"%s.", acAddr);
#endif
        iIndex +=iRet;
        memset(acAddr,0,VOS_INT32STR_MAXLEN);
        iOffset+=8;
        iCount++;

        /*IPV4地址偏移3次*/
        if ( iCount >= 3 )
        {
            break;
        }
    }
    iAddr = ( iNAddr >> iOffset ) & ( 0x000000ff );
#if VOS_PLAT_LINUX
    snprintf(acAddr, VOS_INT32STR_MAXLEN-1,"%d", iAddr);
    iRet = snprintf(acStr+iIndex, VOS_INT32STR_MAXLEN-1,"%s", acAddr);
#elif VOS_PLAT_WIN
    sprintf_s(acAddr, VOS_INT32STR_MAXLEN-1,"%d", iAddr);
    iRet = snprintf(acStr+iIndex, VOS_INT32STR_MAXLEN-1,"%s.", acAddr);
#endif

    memset(pcStr, 0, VOS_IPV4STR_MAXLEN-1);
    memcpy(pcStr, acStr, strlen(acStr));
    
    return VOS_OK;
}


