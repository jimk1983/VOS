/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : default_def.h
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 
  最近修改   :
  功能描述   : 默认创建的一个网关的配置
  函数列表   :
  修改历史   :

******************************************************************************/
    
/**************************************************************************************/
/* 云主机上可以为同一个网卡 */
/*该网关的默认的外网访问入口网卡接口名称*/
#define DEFAULT_GATEWAY_IFNAME                      "eth0"
/*系统启动会默认创建一个网关*/
#define DEFAULT_GWTEWAY_SSLPORT                     443
#define DEFAULT_GWTEWAY_UDPSPORT                    443
/*该网关的默认的名称*/
#define DEFAULT_GATEWAY_NAME                        "defaultfm"

#define DEFAULT_CERT_ROOTCA                         "/usr/local/cert/RootCA.crt"        /*默认的客户端验证CA证书*/
#define DEFAULT_CERT_SERVERC_CERT                   "/usr/local/cert/server1.crt"       /*默认服务器证书*/
#define DEFAULT_CERT_SERVERC_KEY                    "/usr/local/cert/server1.pem"       /*默认服务器私钥*/
#define DEFAULT_LOGFILE                             "/usr/local/log/syslogfile"         /*默认的系统日志文件*/
#define DEFAULT_RCTLOGFILE                          "/usr/local/log/rctlogfile"         /*默认的RCT 日志文件*/


/*创建默认的家庭信息, 不能重复*/
#define DEFAULT_GWFMINFO_DEFNAME                    "dfltfm.com"

/********************************翻墙模式************************************/
/*是否采用旁路挂载模式，即是否内外网都使用一块网卡
该模式一般用于NAT防火墙映射服务器模式
或者是用于内网的测试模式*/
#define DEFAULT_GATEWAY_ACCESS_INNER_BYPASS     0


/*用于给客户端设置的DNS服务器地址,(Windows 7之前的采用虚拟网卡版本，需要使用)*/
#define DEFAULT_GATEWAY_DNSPRIM                       "10.143.23.42"
#define DEFAULT_GATEWAY_DNSSEC                        "100.100.2.138"


/******************  系统启动时默认的虚拟网卡配置                  *********************/
/*配置创建的虚拟网卡名称*/
#define DEFAULT_GATEWAY_VTAPNAME                    "tunbridges"
/*配置创建的虚拟网卡的地址，该地址用于转发客户端报文
翻墙模式下，该虚拟网卡设置为客户端的网关地址*/
/*虚拟网卡的地址, 用来做流量转发的网关地址，
如果客户端是虚拟网卡的模式，那么直接设置到虚拟网卡上，作为网关地址使用，
如果客户端是SDK的模式，那么需要将客户端的真实地址，映射为这个地址段上的地址，作为源IP地址进行SNAT映射
例如： 192.168.1.11:339 ---SNAT-->10.10.10.11:339, --->Gw写入，如果和网关对应了，那就不用映射了 */

/*如果当前版本不想做客户端的SNAT,那么SDK下其实和用户内网真实网关相同网段即可*/
#define DEFAULT_GATEWAY_VTAPIP                      "192.168.3.1"    
#define DEFAULT_GATEWAY_VTAPIPMASK                  "255.255.255.0"

//需要命令: -- 设置一个nat映射，虚拟地址映射为eth0出口地址
//iptables -t nat -A POSTROUTING -s 192.168.3.0/24 -o eth0 -j MSAQUERADE
//还有一个设置: /etc/sysctl.conf 中的net_ipv4_forward=1, 执行sysctl -p即可。

/*默认配置的网络地址池起始地址*/
#define DEFAULT_GATEWAY_NETPOOL_START               "192.168.3.100"
#define DEFAULT_GATEWAY_NETPOOL_END                 "192.168.3.200"

/********************************云主机网卡默认设置************************************/
/*该默认网关的内网网卡接口名称*/
#define DEFAULT_GATEWAY_INNER_IFNAME                "eth0"



