/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_mm.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年1月17日
  最近修改   :
  功能描述   : 内存管理
  函数列表   :
  修改历史   :
  1.日    期   : 2016年1月17日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/

#include <vos_pub.h>

/*----------------------------------------------*
 * 常量定义                                     *
 *----------------------------------------------*/
    
/*内存单元的大小*/
#define VOS_MEM_UNIT_SIZE       sizeof(VOS_MEMBUF_UNIT_S)
    
    /*根据节点获取成员地址*/
#define VOS_MEM_UNIT_ENTRY(plist,type,membernode) \
        container_of(plist,type,membernode)

/*内存管理器*/
typedef struct tagVOSMemMgmt    VOS_MEMBUF_MGMT_S;

typedef struct tagVosMemUnit      VOS_MEMBUF_UNIT_S;

/*踩内存检测魔数*/
#define VOS_MGMT_MAGIC_HEAD1        0xAACCBBDD
#define VOS_MGMT_MAGIC_HEAD2        0xEEBBFFAA

#define VOS_MEM_TYPE_32          1
#define VOS_MEM_TYPE_64          2
#define VOS_MEM_TYPE_128        3
#define VOS_MEM_TYPE_256        4
#define VOS_MEM_TYPE_512        5
#define VOS_MEM_TYPE_1K         6
#define VOS_MEM_TYPE_2K         7
#define VOS_MEM_TYPE_4K         8
#define VOS_MEM_TYPE_8K         9
#define VOS_MEM_TYPE_16K       10
#define VOS_MEM_TYPE_32K       11
#define VOS_MEM_TYPE_64K       12
#define VOS_MEM_TYPE_128K      13
#define VOS_MEM_TYPE_256K      14
#define VOS_MEM_TYPE_512K      15

#define VOS_MEM_TYPE_SYS       16



#define VOS_MEM_SIZE_32         32
#define VOS_MEM_SIZE_64         64
#define VOS_MEM_SIZE_128        128
#define VOS_MEM_SIZE_256        256
#define VOS_MEM_SIZE_512        512
#define VOS_MEM_SIZE_1K         1024
#define VOS_MEM_SIZE_2K         2048
#define VOS_MEM_SIZE_4K         4096
#define VOS_MEM_SIZE_8K         8192    
#define VOS_MEM_SIZE_16K        16384+256
#define VOS_MEM_SIZE_32K        32768
#define VOS_MEM_SIZE_64K        65536
#define VOS_MEM_SIZE_128K       131072
#define VOS_MEM_SIZE_256K       262144
#define VOS_MEM_SIZE_512K       524288



/*可以分配两个大内存块进行管理*/
struct tagVOSMemMgmt
{
    /*初始大内存块*/
    CHAR *pcBlk1;

    /*初始大内存块1*/
    CHAR *pcBlk2;

    /*内存的线程锁*/
    VOID *pvThreadLock;

    /*定义32大小的空闲链表*/
    VOS_DLIST_NODE_S stMMListF_32;
    /*定义32大小的使用链表*/
    VOS_DLIST_NODE_S stMMListU_32;
    UINT32 ulFNum_32;    /*定义空闲的内存数*/
    UINT32 ulUNum_32;    /*定义已经使用的内存数*/

    /*定义64大小的空闲链表*/
    VOS_DLIST_NODE_S stMMListF_64;
    /*定义64大小的使用链表*/
    VOS_DLIST_NODE_S stMMListU_64;
    UINT32 ulFNum_64;    /*定义空闲的内存数*/
    UINT32 ulUNum_64;    /*定义已经使用的内存数*/

    /*定义128大小的空闲链表*/
    VOS_DLIST_NODE_S stMMListF_128;
    /*定义128大小的使用链表*/
    VOS_DLIST_NODE_S stMMListU_128;
    UINT32 ulFNum_128;    /*定义空闲的内存数*/
    UINT32 ulUNum_128;    /*定义已经使用的内存数*/

    /*定义256大小的空闲链表*/
    VOS_DLIST_NODE_S stMMListF_256;
    /*定义256大小的使用链表*/
    VOS_DLIST_NODE_S stMMListU_256;
    UINT32 ulFNum_256;    /*定义空闲的内存数*/
    UINT32 ulUNum_256;    /*定义已经使用的内存数*/

    /*定义512大小的空闲链表*/
    VOS_DLIST_NODE_S stMMListF_512;
    /*定义512大小的使用链表*/
    VOS_DLIST_NODE_S stMMListU_512;
    UINT32 ulFNum_512;    /*定义空闲的内存数*/
    UINT32 ulUNum_512;    /*定义已经使用的内存数*/


    /*定义1024大小的空闲链表*/
    VOS_DLIST_NODE_S stMMListF_1K;
    /*定义1024大小的使用链表*/
    VOS_DLIST_NODE_S stMMListU_1K;
    UINT32 ulFNum_1K;    /*定义空闲的内存数*/
    UINT32 ulUNum_1K;    /*定义已经使用的内存数*/

    /*定义2048大小的空闲链表*/
    VOS_DLIST_NODE_S stMMListF_2K;
    /*定义2048大小的使用链表*/
    VOS_DLIST_NODE_S stMMListU_2K;
    UINT32 ulFNum_2K;    /*定义空闲的内存数*/
    UINT32 ulUNum_2K;    /*定义已经使用的内存数*/


    /*定义4096大小的空闲链表*/
    VOS_DLIST_NODE_S stMMListF_4K;
    /*定义4096大小的使用链表*/
    VOS_DLIST_NODE_S stMMListU_4K;
    UINT32 ulFNum_4K;    /*定义空闲的内存数*/
    UINT32 ulUNum_4K;    /*定义已经使用的内存数*/

    /*定义8192大小的空闲链表*/
    VOS_DLIST_NODE_S stMMListF_8K;
    /*定义8192大小的使用链表*/
    VOS_DLIST_NODE_S stMMListU_8K;
    UINT32 ulFNum_8K;    /*定义空闲的内存数*/
    UINT32 ulUNum_8K;    /*定义已经使用的内存数*/

    /*定义16384大小的空闲链表*/
    VOS_DLIST_NODE_S stMMListF_16K;
    /*定义16384大小的使用链表*/
    VOS_DLIST_NODE_S stMMListU_16K;
    UINT32 ulFNum_16K;    /*定义空闲的内存数*/
    UINT32 ulUNum_16K;    /*定义已经使用的内存数*/

    /*定义32K大小的空闲链表*/
    VOS_DLIST_NODE_S stMMListF_32K;
    /*定义32K大小的使用链表*/
    VOS_DLIST_NODE_S stMMListU_32K;
    UINT32 ulFNum_32K;    /*定义空闲的内存数*/
    UINT32 ulUNum_32K;    /*定义已经使用的内存数*/

    /*定义64K大小的空闲链表*/
    VOS_DLIST_NODE_S stMMListF_64K;
    /*定义64K大小的使用链表*/
    VOS_DLIST_NODE_S stMMListU_64K;
    UINT32 ulFNum_64K;    /*定义空闲的内存数*/
    UINT32 ulUNum_64K;    /*定义已经使用的内存数*/
    
    /*定义128K大小的空闲链表*/
    VOS_DLIST_NODE_S stMMListF_128K;
    /*定义128K大小的使用链表*/
    VOS_DLIST_NODE_S stMMListU_128K;
    UINT32 ulFNum_128K;    /*定义空闲的内存数*/
    UINT32 ulUNum_128K;    /*定义已经使用的内存数*/

    /*超过128K的系统链表, 使用系统的malloc创建的*/
    VOS_DLIST_NODE_S stMMList_system;
    UINT32 ulUNum_system;    /*定义已经使用的内存数*/
};

/*基本的结构体*/
struct tagVosMemUnit
{
    VOS_DLIST_NODE_S stNode;
    UINT32 ulMagicHead1;          /*魔数头1，防止内存越界*/
    UINT32 ulMagicHead2;          /*魔数头2，防止内存越界*/
    UINT32 ulMid;                      /*模块的module ID号，方便进行内存泄露/使用情况定位*/
    UINT32 ulIdle;                      /*是否已经处于空闲,0:空闲1:非空闲*/
    CHAR    acFileName[VOS_FILEPATH_MAXLEN];            /*该内存所属文件*/
    UINT32 ulLineNum;              /*该内存申请所属文件行号*/
    UINT32 ulBufType;              /*释放内存的时候需要用到，查链表用*/
    CHAR    ucmmbuf[0];           /*本内存数据的首地址*/
};

/*定义内存管理器*/
static VOS_MEMBUF_MGMT_S  g_stmmMgmt;


/*定义内存锁，因为是从同一块内存分配的连续内存*/
VOS_RW_LOCK_S g_stmmLock32;
VOS_RW_LOCK_S g_stmmLock64;
VOS_RW_LOCK_S g_stmmLock128;
VOS_RW_LOCK_S g_stmmLock256;
VOS_RW_LOCK_S g_stmmLock512;
VOS_RW_LOCK_S g_stmmLock1K;
VOS_RW_LOCK_S g_stmmLock2K;
VOS_RW_LOCK_S g_stmmLock4K;
VOS_RW_LOCK_S g_stmmLock8K;
VOS_RW_LOCK_S g_stmmLock16K;
VOS_RW_LOCK_S g_stmmLock32K;
VOS_RW_LOCK_S g_stmmLock64K;
VOS_RW_LOCK_S g_stmmLock128K;

/*额外保留的1M大小的内存,防止初始化内存出错*/
#define VOS_MEMORY_RESV      1024 * 1024

/*内存管理器初始化*/
INT32 VOS_MemMgmt_Init()
{  
    /*根据情况，一块大内存也可以，两块的话，保证两块差不多即可
     主要可能是一次性申请大块内存可能会失败*/
    UINT32 ulBufSum1    = 0;
    UINT32 ulBufSum2    = 0;
    CHAR   *pcMmOffset = NULL;
    UINT32 ulIndex        = 0;
    UINT32 ulSizeChar    = VOS_CHAR_SIZE;
    //CHAR   *pcTmpOff    = NULL;
    VOS_MEMBUF_UNIT_S *pstMmEntry = NULL;

    /*小字节数量比较多, 放在第一块内存中*/
    ulBufSum1  =  VOS_MEM_32_NUM   * (  (VOS_MEM_SIZE_32+1)  * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum1 += VOS_MEM_64_NUM   * (  (VOS_MEM_SIZE_64+1)  * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum1 += VOS_MEM_128_NUM * (  (VOS_MEM_SIZE_128+1) * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum1 += VOS_MEM_256_NUM * (  (VOS_MEM_SIZE_256+1) * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum1 += VOS_MEM_512_NUM * (  (VOS_MEM_SIZE_512+1) * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);

    /*1K以后的放在另外一块*/
    ulBufSum2 =    VOS_MEM_1K_NUM * (  (VOS_MEM_SIZE_1K+1) * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum2 +=  VOS_MEM_2K_NUM * (  (VOS_MEM_SIZE_2K+1) * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum2 +=  VOS_MEM_4K_NUM * (  (VOS_MEM_SIZE_4K+1) * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum2 +=  VOS_MEM_8K_NUM * (  (VOS_MEM_SIZE_8K+1) * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum2 +=  VOS_MEM_16K_NUM * ( (VOS_MEM_SIZE_16K+1) * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum2 +=  VOS_MEM_32K_NUM * ( (VOS_MEM_SIZE_32K+1) * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum2 +=  VOS_MEM_64K_NUM * ( (VOS_MEM_SIZE_64K+1) * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);
    ulBufSum2 +=  VOS_MEM_128K_NUM *( (VOS_MEM_SIZE_128K+1) * VOS_CHAR_SIZE + VOS_MEM_UNIT_SIZE);

    
    /*多线程内存锁初始化*/
    VOS_ThreadMutex_Init(&g_stmmLock32);
    VOS_ThreadMutex_Init(&g_stmmLock64);
    VOS_ThreadMutex_Init(&g_stmmLock128);
    VOS_ThreadMutex_Init(&g_stmmLock256);
    VOS_ThreadMutex_Init(&g_stmmLock512);
    VOS_ThreadMutex_Init(&g_stmmLock1K);
    VOS_ThreadMutex_Init(&g_stmmLock2K);
    VOS_ThreadMutex_Init(&g_stmmLock4K);
    VOS_ThreadMutex_Init(&g_stmmLock8K);
    VOS_ThreadMutex_Init(&g_stmmLock16K);
    VOS_ThreadMutex_Init(&g_stmmLock32K);
    VOS_ThreadMutex_Init(&g_stmmLock64K);
    VOS_ThreadMutex_Init(&g_stmmLock128K);
    
    /*内存管理器*/
    VOS_Mem_Zero((CHAR *)&g_stmmMgmt, sizeof(g_stmmMgmt));
    
    /*1.申请内存*/
    g_stmmMgmt.pcBlk1 = (CHAR *)VOS_Memory(ulBufSum1 + VOS_MEMORY_RESV);
    if ( NULL == g_stmmMgmt.pcBlk1 )
    {
        VOS_Printf("g_stmmMgmt.ucBlk1 malloc error!ulBufSum1=%d\n", ulBufSum1);
        goto ErrHandle;
    }
    else
    {
        VOS_Mem_Zero(g_stmmMgmt.pcBlk1, ulBufSum1 );
    }
    
    g_stmmMgmt.pcBlk2 = (CHAR *)VOS_Memory(ulBufSum2 + VOS_MEMORY_RESV);
    if ( NULL == g_stmmMgmt.pcBlk2 )
    {
        VOS_Printf("g_stmmMgmt.ucBlk2  malloc error!\n");
        goto ErrHandle;
    }
    else
    {
        VOS_Mem_Zero(g_stmmMgmt.pcBlk2, ulBufSum2 );
    }

    /*管理内存链表初始化*/
    VOS_Node_Init(&g_stmmMgmt.stMMListF_32);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_32);
    g_stmmMgmt.ulFNum_32 = 0;
    g_stmmMgmt.ulUNum_32 = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_64);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_64);
    g_stmmMgmt.ulFNum_64 = 0;
    g_stmmMgmt.ulUNum_64 = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_128);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_128);
    g_stmmMgmt.ulFNum_128 = 0;
    g_stmmMgmt.ulUNum_128 = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_256);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_256);
    g_stmmMgmt.ulFNum_256 = 0;
    g_stmmMgmt.ulUNum_256 = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_512);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_512);
    g_stmmMgmt.ulFNum_512 = 0;
    g_stmmMgmt.ulUNum_512 = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_1K);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_1K);
    g_stmmMgmt.ulFNum_1K = 0;
    g_stmmMgmt.ulUNum_1K = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_2K);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_2K);
    g_stmmMgmt.ulFNum_2K = 0;
    g_stmmMgmt.ulUNum_2K = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_4K);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_4K);
    g_stmmMgmt.ulFNum_4K = 0;
    g_stmmMgmt.ulUNum_4K = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_8K);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_8K);
    g_stmmMgmt.ulFNum_8K = 0;
    g_stmmMgmt.ulUNum_8K = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_16K);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_16K);
    g_stmmMgmt.ulFNum_16K = 0;
    g_stmmMgmt.ulUNum_16K = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_32K);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_32K);
    g_stmmMgmt.ulFNum_32K = 0;
    g_stmmMgmt.ulUNum_32K = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_64K);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_64K);
    g_stmmMgmt.ulFNum_64K = 0;
    g_stmmMgmt.ulUNum_64K = 0;
    VOS_Node_Init(&g_stmmMgmt.stMMListF_128K);
    VOS_Node_Init(&g_stmmMgmt.stMMListU_128K);
    g_stmmMgmt.ulFNum_128K = 0;
    g_stmmMgmt.ulUNum_128K = 0;


    /*小字节的内存块1进行赋值*/
    pcMmOffset = g_stmmMgmt.pcBlk1;
    /*1. 32字节的内存*/
    for(ulIndex = 0;ulIndex < VOS_MEM_32_NUM;ulIndex++)
    {
        /*赋值内存的偏移地址*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*将该内存进行赋值*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_32;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*最后一个是内存的使用地址，
           从初始地址偏移即可*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*数据内存开始的地方*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*将内存加入到空闲的链表中*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_32, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_32++;
        /*切换到下一片的内存, 偏移大小是32字节+ 1结束位
           防止拷贝越界*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + 32 + ulSizeChar;
    }


    /*64字节*/
    for(ulIndex = 0;ulIndex < VOS_MEM_64_NUM;ulIndex++)
    {
        /*赋值内存的偏移地址*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*将该内存进行赋值*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_64;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*最后一个是内存的使用地址，
           从初始地址偏移即可*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*数据内存开始的地方*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*将内存加入到空闲的链表中*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_64, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_64++;
        /*切换到下一片的内存, 偏移大小是32字节+ 1结束位
           防止拷贝越界*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + 64 + ulSizeChar;
    }

    
    /*128字节*/
    for(ulIndex = 0;ulIndex < VOS_MEM_128_NUM;ulIndex++)
    {
        /*赋值内存的偏移地址*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*将该内存进行赋值*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_128;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*最后一个是内存的使用地址，
           从初始地址偏移即可*/
       //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*数据内存开始的地方*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*将内存加入到空闲的链表中*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_128, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_128++;
        /*切换到下一片的内存, 偏移大小是32字节+ 1结束位
           防止拷贝越界*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + 128 + ulSizeChar;
    }

    /*256字节*/
    for(ulIndex = 0;ulIndex < VOS_MEM_256_NUM;ulIndex++)
    {
        /*赋值内存的偏移地址*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*将该内存进行赋值*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_256;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*最后一个是内存的使用地址，
           从初始地址偏移即可*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*数据内存开始的地方*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*将内存加入到空闲的链表中*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_256, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_256++;
        /*切换到下一片的内存, 偏移大小是32字节+ 1结束位
           防止拷贝越界*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + 256 + ulSizeChar;
    }

    /*512字节*/
    for(ulIndex = 0;ulIndex < VOS_MEM_512_NUM;ulIndex++)
    {
        /*赋值内存的偏移地址*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*将该内存进行赋值*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_512;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*最后一个是内存的使用地址，
           从初始地址偏移即可*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*数据内存开始的地方*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*将内存加入到空闲的链表中*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_512, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_512++;
        /*切换到下一片的内存, 偏移大小是32字节+ 1结束位
           防止拷贝越界*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + 512 + ulSizeChar;
    }


    /*2. 大字节的内存块1进行赋值*/
    pcMmOffset = g_stmmMgmt.pcBlk2;
    /*. 1K字节的内存*/
    for(ulIndex = 0;ulIndex < VOS_MEM_1K_NUM;ulIndex++)
    {
        /*赋值内存的偏移地址*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*将该内存进行赋值*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_1K;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*最后一个是内存的使用地址，
           从初始地址偏移即可*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*数据内存开始的地方*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*将内存加入到空闲的链表中*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_1K, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_1K++;
        /*切换到下一片的内存, 偏移大小是1024字节+ 1结束位
           防止拷贝越界*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + 1024 + ulSizeChar;
    }


    /*2K字节*/
    for(ulIndex = 0;ulIndex < VOS_MEM_2K_NUM;ulIndex++)
    {
        /*赋值内存的偏移地址*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*将该内存进行赋值*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_2K;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*最后一个是内存的使用地址，
           从初始地址偏移即可*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*数据内存开始的地方*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*将内存加入到空闲的链表中*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_2K, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_2K++;
        /*切换到下一片的内存, 偏移大小是2048字节+ 1结束位
           防止拷贝越界*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + 2048 + ulSizeChar;
    }

    
    /*4096字节*/
    for(ulIndex = 0;ulIndex < VOS_MEM_4K_NUM;ulIndex++)
    {
        /*赋值内存的偏移地址*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*将该内存进行赋值*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_4K;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*最后一个是内存的使用地址，
           从初始地址偏移即可*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*数据内存开始的地方*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*将内存加入到空闲的链表中*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_4K, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_4K++;
        /*切换到下一片的内存, 偏移大小是32字节+ 1结束位
           防止拷贝越界*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + 4096 + ulSizeChar;
    }

    /*8192字节*/
    for(ulIndex = 0;ulIndex < VOS_MEM_8K_NUM;ulIndex++)
    {
        /*赋值内存的偏移地址*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*将该内存进行赋值*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_8K;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*最后一个是内存的使用地址，
           从初始地址偏移即可*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*数据内存开始的地方*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*将内存加入到空闲的链表中*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_8K, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_8K++;
        /*切换到下一片的内存, 偏移大小是32字节+ 1结束位
           防止拷贝越界*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + 8192 + ulSizeChar;
    }

    /*16384字节*/
    for(ulIndex = 0;ulIndex < VOS_MEM_16K_NUM;ulIndex++)
    {
        /*赋值内存的偏移地址*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*将该内存进行赋值*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_16K;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*最后一个是内存的使用地址，
           从初始地址偏移即可*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*数据内存开始的地方*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*将内存加入到空闲的链表中*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_16K, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_16K++;
        /*切换到下一片的内存, 偏移大小是16384字节+ 1结束位
           防止拷贝越界*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + 16384 + ulSizeChar;
    }

    /*32768字节*/
    for(ulIndex = 0;ulIndex < VOS_MEM_32K_NUM;ulIndex++)
    {
        /*赋值内存的偏移地址*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*将该内存进行赋值*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_32K;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*最后一个是内存的使用地址，
           从初始地址偏移即可*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*数据内存开始的地方*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*将内存加入到空闲的链表中*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_32K, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_32K++;
        /*切换到下一片的内存, 偏移大小是32768字节+ 1结束位
           防止拷贝越界*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + 32768 + ulSizeChar;
    }

    /*65536字节*/
    for(ulIndex = 0;ulIndex < VOS_MEM_64K_NUM;ulIndex++)
    {
        /*赋值内存的偏移地址*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*将该内存进行赋值*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_64K;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*最后一个是内存的使用地址，
           从初始地址偏移即可*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*数据内存开始的地方*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*将内存加入到空闲的链表中*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_64K, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_64K++;
        /*切换到下一片的内存, 偏移大小是32768字节+ 1结束位
           防止拷贝越界*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + 65536 + ulSizeChar;
    }

    /*131072字节*/
    for(ulIndex = 0;ulIndex < VOS_MEM_128K_NUM;ulIndex++)
    {
        /*赋值内存的偏移地址*/
        pstMmEntry = (VOS_MEMBUF_UNIT_S *)pcMmOffset;

        /*将该内存进行赋值*/
        pstMmEntry->ulMagicHead1 = VOS_MGMT_MAGIC_HEAD1;
        pstMmEntry->ulMagicHead2 = VOS_MGMT_MAGIC_HEAD2;
        VOS_Node_Init(&pstMmEntry->stNode);
        pstMmEntry->ulLineNum =  0;
        pstMmEntry->ulBufType  =  VOS_MEM_TYPE_128K;
        pstMmEntry->ulIdle        =  VOS_MEM_FREED;

        /*最后一个是内存的使用地址，
           从初始地址偏移即可*/
        //pcTmpOff = &pstMmEntry->ucmmbuf[0];

        /*数据内存开始的地方*/
        //pcTmpOff = (CHAR *)(pcMmOffset)+ VOS_MEM_UNIT_SIZE;
        
        /*将内存加入到空闲的链表中*/
        VOS_Node_HeadAdd(&g_stmmMgmt.stMMListF_128K, &pstMmEntry->stNode);
        g_stmmMgmt.ulFNum_128K++;
        /*切换到下一片的内存, 偏移大小是32768字节+ 1结束位
           防止拷贝越界*/
        pcMmOffset += VOS_MEM_UNIT_SIZE + 131072 + ulSizeChar;
    }
    
    return VOS_OK;
    
ErrHandle:
    VOS_MemFree(g_stmmMgmt.pcBlk1);
    VOS_MemFree(g_stmmMgmt.pcBlk2);
    
    VOS_ThreadMutex_Destroy(&g_stmmLock32);
    VOS_ThreadMutex_Destroy(&g_stmmLock64);
    VOS_ThreadMutex_Destroy(&g_stmmLock128);
    VOS_ThreadMutex_Destroy(&g_stmmLock256);
    VOS_ThreadMutex_Destroy(&g_stmmLock512);
    VOS_ThreadMutex_Destroy(&g_stmmLock1K);
    VOS_ThreadMutex_Destroy(&g_stmmLock2K);
    VOS_ThreadMutex_Destroy(&g_stmmLock4K);
    VOS_ThreadMutex_Destroy(&g_stmmLock8K);
    VOS_ThreadMutex_Destroy(&g_stmmLock16K);
    VOS_ThreadMutex_Destroy(&g_stmmLock32K);
    VOS_ThreadMutex_Destroy(&g_stmmLock64K);
    VOS_ThreadMutex_Destroy(&g_stmmLock128K);
    
    return VOS_ERR;
}

/*内存管理器去初始化*/
VOID VOS_MemMgmt_UnInit()
{  
    
    VOS_MemFree(g_stmmMgmt.pcBlk1);

    VOS_MemFree(g_stmmMgmt.pcBlk2);

    VOS_ThreadMutex_Destroy(&g_stmmLock32);
    VOS_ThreadMutex_Destroy(&g_stmmLock64);
    VOS_ThreadMutex_Destroy(&g_stmmLock128);
    VOS_ThreadMutex_Destroy(&g_stmmLock256);
    VOS_ThreadMutex_Destroy(&g_stmmLock512);
    VOS_ThreadMutex_Destroy(&g_stmmLock1K);
    VOS_ThreadMutex_Destroy(&g_stmmLock2K);
    VOS_ThreadMutex_Destroy(&g_stmmLock4K);
    VOS_ThreadMutex_Destroy(&g_stmmLock8K);
    VOS_ThreadMutex_Destroy(&g_stmmLock16K);
    VOS_ThreadMutex_Destroy(&g_stmmLock32K);
    VOS_ThreadMutex_Destroy(&g_stmmLock64K);
    VOS_ThreadMutex_Destroy(&g_stmmLock128K);
    
    return;
}

/*****************************************************************************
 函 数 名  : VOS_MemCreate_X
 功能描述  : 申请内存，记录了该内存的相关文件和行号信息
 输入参数  :   ULONG ulMid       ---模块信息
                            ULONG ulBufSize       --申请内存大小         
                            CHAR *pcFileName   ---申请内存的文件名称
                            ULONG ulLine        ---申请内存的行号
 输出参数  : 无
 返 回 值  : CHAR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月26日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
CHAR *VOS_MemCreate_X(ULONG ulMid, ULONG ulBufSize, CHAR *pcFileName, ULONG ulLine)
{   
    ULONG ulSysBufSize       = 0;
    CHAR *pcMemBuf          = NULL;
    VOS_MEMBUF_UNIT_S *pstMem = NULL;
    VOS_DLIST_NODE_S   *pstNode = NULL;
    
    /*支持多线程内存分配，需要添加线程锁*/
    if ( ulBufSize <= 32 )
    {
        VOS_RWLOCK_LOCK(g_stmmLock32);
         /*首先需要看是不是空的，从不为空的获取
           *(VOS_TRUE != VOS_Node_IsEmpty(&g_stmmMgmt.stMMListF_32) )
           *提高效率,目前不对该链表判断,对数目进行判断*/
        if ( g_stmmMgmt.ulFNum_32 >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_32,&pstNode);

            if ( NULL != pstNode )
            {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock32);
                    return NULL;
                }
                
                VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, pcFileName, VOS_StrLen(pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_32--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_32, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_32++;
            }
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock32);
    }
    else if ( ( ulBufSize > 32 ) && ( ulBufSize <= 64 ) )
    {
        VOS_RWLOCK_LOCK(g_stmmLock64);
        
        if ( g_stmmMgmt.ulFNum_64 >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_64,&pstNode);
            if ( NULL != pstNode )
            {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock64);
                    return NULL;
                }
                
                VOS_Mem_Copy(pstMem->acFileName, pcFileName, VOS_StrLen(pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_64--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_64, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_64++;
           }
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock64);
    }
    else if ( ( ulBufSize > 64 ) && ( ulBufSize <= 128 ) )
    {
        VOS_RWLOCK_LOCK(g_stmmLock128);
        
        if ( g_stmmMgmt.ulFNum_128 >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_128, &pstNode);
            if ( NULL != pstNode )
                {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock128);
                    return NULL;
                }
                
                VOS_Mem_Copy(pstMem->acFileName, pcFileName, VOS_StrLen(pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_128--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_128, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_128++;
            }
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock128);
    }
    else if ( ( ulBufSize > 128 ) && ( ulBufSize <= 256 ) )
    {
        VOS_RWLOCK_LOCK(g_stmmLock256);
        
        if ( g_stmmMgmt.ulFNum_256 >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_256,&pstNode);
            if ( NULL != pstNode )
            {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock256);
                    return NULL;
                }
                
                VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN,pcFileName, VOS_StrLen(pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_256--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_256, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_256++;
            }
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock256);
    }
    else if ( ( ulBufSize > 256 ) && ( ulBufSize <= 512 ) )
    {
        VOS_RWLOCK_LOCK(g_stmmLock512);
        
        if ( g_stmmMgmt.ulFNum_512 >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_512,&pstNode);
            if ( NULL != pstNode )
            {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock512);
                    return NULL;
                }
                
                VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, pcFileName, VOS_StrLen(pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_512--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_512, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_512++;
           }
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock512);
    }
    else if ( ( ulBufSize > 512 ) && ( ulBufSize <= 1024 ) )
    {
        VOS_RWLOCK_LOCK(g_stmmLock1K);
        
        if ( g_stmmMgmt.ulFNum_1K >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_1K,&pstNode);
            if ( NULL != pstNode )
            {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock1K);
                    return NULL;
                }
                
                VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, pcFileName, VOS_StrLen(pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_1K--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_1K, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_1K++;
            }
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock1K);
    }
    else if ( ( ulBufSize > 1024 ) && ( ulBufSize <= 2048 ) )
    {
        VOS_RWLOCK_LOCK(g_stmmLock2K);
        
        if ( g_stmmMgmt.ulFNum_2K >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_2K,&pstNode);
            if ( NULL != pstNode )
            {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock2K);
                    return NULL;
                }
                
                VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, pcFileName, VOS_StrLen(pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_2K--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_2K, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_2K++;
            }
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock2K);
    }
    else if ( ( ulBufSize > 2048 ) && ( ulBufSize <= 4096 ) )
    {
        VOS_RWLOCK_LOCK(g_stmmLock4K);
        
        if ( g_stmmMgmt.ulFNum_4K >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_4K,&pstNode);
            if ( NULL != pstNode )
            {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock4K);
                    return NULL;
                }
                
                VOS_Mem_Copy_S(pstMem->acFileName,VOS_FILEPATH_MAXLEN, pcFileName, VOS_StrLen(pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_4K--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_4K, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_4K++;
            }                
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock4K);
    }
    else if ( ( ulBufSize > 4096 ) && ( ulBufSize <= 8192 ) )
    {
        VOS_RWLOCK_LOCK(g_stmmLock8K);
        
        if ( g_stmmMgmt.ulFNum_8K >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_8K,&pstNode);
            if ( NULL != pstNode )
            {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode, VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock8K);
                    return NULL;
                }
                
                VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, pcFileName, VOS_StrLen(pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_8K--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_8K, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_8K++;
                }                
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock8K);
    }
    else if ( ( ulBufSize > 8192 ) && ( ulBufSize <= 16384 ) )
    {
        VOS_RWLOCK_LOCK(g_stmmLock16K);
        
        if ( g_stmmMgmt.ulFNum_16K >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_16K,&pstNode);
            if ( NULL != pstNode )
            {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock16K);
                    return NULL;
                }
                
                VOS_Mem_Copy_S(pstMem->acFileName,VOS_FILEPATH_MAXLEN, pcFileName, VOS_StrLen(pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_16K--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_16K, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_16K++;
            }                
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock16K);
    }
    else if ( ( ulBufSize > 16384 ) && ( ulBufSize <= 32768 ) )
    {
        VOS_RWLOCK_LOCK(g_stmmLock32K);
        
        if ( g_stmmMgmt.ulFNum_32K >0 )
        {
            VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_32K,&pstNode);
            if ( NULL != pstNode )
            {
                pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
                if ( NULL == pstMem )
                {
                    VOS_RWLOCK_UNLOCK(g_stmmLock32K);
                    return NULL;
                }
                
                VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN,pcFileName, VOS_StrLen(pcFileName));
                pstMem->ulLineNum =  ulLine;
                pstMem->ulIdle        =  VOS_MEM_USED;
                pstMem->ulMid        =  ulMid;
                pcMemBuf                =  pstMem->ucmmbuf;
                
                g_stmmMgmt.ulFNum_32K--;
                VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_32K, &(pstMem->stNode));
                g_stmmMgmt.ulUNum_32K++;
            }                
        }
        VOS_RWLOCK_UNLOCK(g_stmmLock32K);
    }
    else if ( ( ulBufSize > 32768 ) && ( ulBufSize <= 65536 ) )
    {
       VOS_RWLOCK_LOCK(g_stmmLock64K);
       
       if ( g_stmmMgmt.ulFNum_64K >0 )
       {
           VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_64K, &pstNode);
           if ( NULL != pstNode )
           {
               pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
               if ( NULL == pstMem )
               {
                   VOS_RWLOCK_UNLOCK(g_stmmLock64K);
                   return NULL;
               }
               
               VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, pcFileName, VOS_StrLen(pcFileName));
               pstMem->ulLineNum =  ulLine;
               pstMem->ulIdle        =  VOS_MEM_USED;
               pstMem->ulMid        =  ulMid;
               pcMemBuf                =  pstMem->ucmmbuf;
               
               g_stmmMgmt.ulFNum_64K--;
               VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_64K, &(pstMem->stNode));
               g_stmmMgmt.ulUNum_64K++;
            }               
       }
       VOS_RWLOCK_UNLOCK(g_stmmLock64K);
    }
    else if ( ( ulBufSize > 65536 ) && ( ulBufSize <= 131072 ) )
    {
       VOS_RWLOCK_LOCK(g_stmmLock128K);
       
       if ( g_stmmMgmt.ulFNum_128K >0 )
       {
           VOS_Node_HeadGet(&g_stmmMgmt.stMMListF_128K,&pstNode);
            if ( NULL != pstNode )
            {
               pstMem = VOS_MEM_UNIT_ENTRY(pstNode,VOS_MEMBUF_UNIT_S, stNode);
               if ( NULL == pstMem )
               {
                   VOS_RWLOCK_UNLOCK(g_stmmLock128K);
                   return NULL;
               }
               
               VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, pcFileName, VOS_StrLen(pcFileName));
               pstMem->ulLineNum =  ulLine;
               pstMem->ulIdle        =  VOS_MEM_USED;
               pstMem->ulMid        =  ulMid;
               pcMemBuf                =  pstMem->ucmmbuf;
               
               g_stmmMgmt.ulFNum_128K--;
               VOS_Node_TailAdd(&g_stmmMgmt.stMMListU_128K, &(pstMem->stNode));
               g_stmmMgmt.ulUNum_128K++;
            }               
       }
       VOS_RWLOCK_UNLOCK(g_stmmLock128K);
    }
    else
    {       
            /*字节对齐??*/
            ulSysBufSize = sizeof(VOS_MEMBUF_UNIT_S) + ulBufSize;
            pcMemBuf = (CHAR *)malloc(ulSysBufSize);
            pstMem = (VOS_MEMBUF_UNIT_S *)pcMemBuf;
            VOS_Mem_Copy_S(pstMem->acFileName, VOS_FILEPATH_MAXLEN, pcFileName, VOS_StrLen(pcFileName));
            pstMem->ulLineNum   =  ulLine;
            pstMem->ulIdle      =  VOS_MEM_USED;
            pstMem->ulMid       =  ulMid;
            pstMem->ulBufType   =  VOS_MEM_TYPE_SYS;
            pcMemBuf            =  pstMem->ucmmbuf;

            VOS_Printf("/****BigMemory Warning!:VOS_Malloc() used the system malloc(), [%s:%d]=[%p], memsize=%d",
                pcFileName, ulLine, pstMem, ulBufSize);
            
    }

    return pcMemBuf;
}


/*归还内存入链表*/
VOID VOS_MemFree_X(CHAR *pcMemBuf)
{   
    VOS_MEMBUF_UNIT_S *pstMem = NULL;

    if ( NULL == pcMemBuf )
    {
        return;
    }

    pstMem = VOS_MEM_UNIT_ENTRY(pcMemBuf, VOS_MEMBUF_UNIT_S, ucmmbuf);
    if ( NULL == pstMem )
    {
        return;
    }
    
    VOS_Mem_Zero(pstMem->acFileName, VOS_FILEPATH_MAXLEN);
    pstMem->ulIdle = VOS_MEM_FREED;
    pstMem->ulMid  = 0;
    pstMem->ulLineNum = 0;
    
    if ( VOS_MEM_TYPE_32 == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock32);
        
        VOS_Node_Remove(&pstMem->stNode);
        g_stmmMgmt.ulUNum_32--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_32, &(pstMem->stNode));
        g_stmmMgmt.ulFNum_32++;
        VOS_RWLOCK_UNLOCK(g_stmmLock32);
    }
    else if ( VOS_MEM_TYPE_64 == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock64);
        VOS_Node_Remove(&pstMem->stNode);
        g_stmmMgmt.ulUNum_64--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_64, &(pstMem->stNode));
        g_stmmMgmt.ulFNum_64++;
        VOS_RWLOCK_UNLOCK(g_stmmLock64);
    }
    else if ( VOS_MEM_TYPE_128 == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock128);
        
        VOS_Node_Remove(&pstMem->stNode);
        g_stmmMgmt.ulUNum_128--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_128, &(pstMem->stNode));
        g_stmmMgmt.ulFNum_128++;
        VOS_RWLOCK_UNLOCK(g_stmmLock128);
    }
    else if ( VOS_MEM_TYPE_256 == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock256);
        VOS_Node_Remove(&pstMem->stNode);
        g_stmmMgmt.ulUNum_256--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_256, &(pstMem->stNode));
        g_stmmMgmt.ulFNum_256++;
        VOS_RWLOCK_UNLOCK(g_stmmLock256);
    }
    else if ( VOS_MEM_TYPE_512 == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock512);
        VOS_Node_Remove(&pstMem->stNode);
        g_stmmMgmt.ulUNum_512--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_512, &(pstMem->stNode));
        g_stmmMgmt.ulFNum_512++;
        VOS_RWLOCK_UNLOCK(g_stmmLock512);
    }
    else if ( VOS_MEM_TYPE_1K == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock1K);
        VOS_Node_Remove(&pstMem->stNode);
        g_stmmMgmt.ulUNum_1K--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_1K, &(pstMem->stNode));
        g_stmmMgmt.ulFNum_1K++;
        VOS_RWLOCK_UNLOCK(g_stmmLock1K);
    }
    else if ( VOS_MEM_TYPE_2K == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock2K);
        VOS_Node_Remove(&pstMem->stNode);
        g_stmmMgmt.ulUNum_2K--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_2K, &(pstMem->stNode));
        g_stmmMgmt.ulFNum_2K++;
        VOS_RWLOCK_UNLOCK(g_stmmLock2K);
    }
    else if ( VOS_MEM_TYPE_4K == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock4K);
        VOS_Node_Remove(&pstMem->stNode);
        g_stmmMgmt.ulUNum_4K--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_4K, &(pstMem->stNode));
        g_stmmMgmt.ulFNum_4K++;
        VOS_RWLOCK_UNLOCK(g_stmmLock4K);
    }
    else if ( VOS_MEM_TYPE_8K == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock8K);
        VOS_Node_Remove(&pstMem->stNode);
        g_stmmMgmt.ulUNum_8K--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_8K, &(pstMem->stNode));
        g_stmmMgmt.ulFNum_8K++;            
        VOS_RWLOCK_UNLOCK(g_stmmLock8K);
    }
    else if ( VOS_MEM_TYPE_16K == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock16K);        
        VOS_Node_Remove(&pstMem->stNode);        
        g_stmmMgmt.ulUNum_16K--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_16K, &(pstMem->stNode));        
        g_stmmMgmt.ulFNum_16K++;
        VOS_RWLOCK_UNLOCK(g_stmmLock16K);
    }
    else if ( VOS_MEM_TYPE_32K == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock32K);        
        VOS_Node_Remove(&pstMem->stNode);        
        g_stmmMgmt.ulUNum_32K--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_32K, &(pstMem->stNode));        
        g_stmmMgmt.ulFNum_32K++;
        VOS_RWLOCK_UNLOCK(g_stmmLock32K);
    }
    else if ( VOS_MEM_TYPE_64K == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock64K);
        VOS_Node_Remove(&pstMem->stNode);
        g_stmmMgmt.ulUNum_64K--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_64K, &(pstMem->stNode));
        g_stmmMgmt.ulFNum_64K++;
        VOS_RWLOCK_UNLOCK(g_stmmLock64K);
    }
    else if ( VOS_MEM_TYPE_128K == pstMem->ulBufType )
    {
        VOS_RWLOCK_LOCK(g_stmmLock128K);        
        VOS_Node_Remove(&pstMem->stNode);        
        g_stmmMgmt.ulUNum_128K--;
        VOS_Node_TailAdd(&g_stmmMgmt.stMMListF_128K, &(pstMem->stNode));        
        g_stmmMgmt.ulFNum_128K++;
        VOS_RWLOCK_UNLOCK(g_stmmLock128K);
    }
    else
    {
        VOS_Printf("/****BigMemory Warning!:VOS_MemFree() used the system free(), [%s:%d]=[%p]",
                pstMem->acFileName, pstMem->ulLineNum, pstMem);
        free(pstMem);
    }
    return;
}

/*****************************************************************************
 函 数 名  : VOS_MM_TypeShow
 功能描述  : 内存显示
 输入参数  : UINT32 uiType  
             UINT32 uiMid   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月26日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_MM_MidTypeShow(UINT32 uiType,  UINT32 uiMid)
{
    VOS_DLIST_NODE_S   *pstDList     = NULL;
    VOS_RW_LOCK_S       *pstMutex   = NULL;
    VOS_MEMBUF_UNIT_S *pstMmBuf  = NULL;   
    ULONG                       ulCount      = 0;
    ULONG                       ulFreedNum= 0 ;
    ULONG                       ulUsedNum = 0;

    switch(uiType)
    {
        case VOS_MEM_TYPE_32:
            pstDList = &g_stmmMgmt.stMMListU_32;
            pstMutex = &g_stmmLock32;
            ulFreedNum = g_stmmMgmt.ulFNum_32;
            ulUsedNum = g_stmmMgmt.ulUNum_32;
            break;
        case VOS_MEM_TYPE_64:
            pstDList = &g_stmmMgmt.stMMListU_64;
            pstMutex = &g_stmmLock64;
            ulFreedNum = g_stmmMgmt.ulFNum_64;
            ulUsedNum = g_stmmMgmt.ulUNum_64;
            break;
        case VOS_MEM_TYPE_128:
            pstDList = &g_stmmMgmt.stMMListU_128;
            pstMutex = &g_stmmLock128;
            ulFreedNum = g_stmmMgmt.ulFNum_128;
            ulUsedNum = g_stmmMgmt.ulUNum_128;
            break;
        case VOS_MEM_TYPE_256:
            pstDList = &g_stmmMgmt.stMMListU_256;
            pstMutex = &g_stmmLock256;
            ulFreedNum = g_stmmMgmt.ulFNum_256;
            ulUsedNum = g_stmmMgmt.ulUNum_256;
            break;
        case VOS_MEM_TYPE_512:
            pstDList = &g_stmmMgmt.stMMListU_512;
            pstMutex = &g_stmmLock512;
            ulFreedNum = g_stmmMgmt.ulFNum_512;
            ulUsedNum = g_stmmMgmt.ulUNum_512;
            break;
        case VOS_MEM_TYPE_1K:
            pstDList = &g_stmmMgmt.stMMListU_1K;
            pstMutex = &g_stmmLock1K;
            ulFreedNum = g_stmmMgmt.ulFNum_1K;
            ulUsedNum = g_stmmMgmt.ulUNum_1K;
            break;
        case VOS_MEM_TYPE_2K:
            pstDList = &g_stmmMgmt.stMMListU_2K;
            pstMutex = &g_stmmLock2K;
            ulFreedNum = g_stmmMgmt.ulFNum_2K;
            ulUsedNum = g_stmmMgmt.ulUNum_2K;
            break;
        case VOS_MEM_TYPE_4K:
            pstDList = &g_stmmMgmt.stMMListU_4K;
            pstMutex = &g_stmmLock4K;
            ulFreedNum = g_stmmMgmt.ulFNum_4K;
            ulUsedNum = g_stmmMgmt.ulUNum_4K;
            break;
        case VOS_MEM_TYPE_8K:
            pstDList = &g_stmmMgmt.stMMListU_8K;
            pstMutex = &g_stmmLock8K;
            ulFreedNum = g_stmmMgmt.ulFNum_8K;
            ulUsedNum = g_stmmMgmt.ulUNum_8K;
            break;
        case VOS_MEM_TYPE_16K:
            pstDList = &g_stmmMgmt.stMMListU_16K;
            pstMutex = &g_stmmLock16K;
            ulFreedNum = g_stmmMgmt.ulFNum_16K;
            ulUsedNum = g_stmmMgmt.ulUNum_16K;
            break;
        case VOS_MEM_TYPE_32K:
            pstDList = &g_stmmMgmt.stMMListU_32K;
            pstMutex = &g_stmmLock32K;
            ulFreedNum = g_stmmMgmt.ulFNum_32K;
            ulUsedNum = g_stmmMgmt.ulUNum_32K;
            break;
        case VOS_MEM_TYPE_64K:
            pstDList = &g_stmmMgmt.stMMListU_64K;
            pstMutex = &g_stmmLock64K;
            ulFreedNum = g_stmmMgmt.ulFNum_64K;
            ulUsedNum = g_stmmMgmt.ulUNum_64K;
            break;
        case VOS_MEM_TYPE_128K:
            pstDList = &g_stmmMgmt.stMMListU_128K;
            pstMutex = &g_stmmLock128K;
            ulFreedNum = g_stmmMgmt.ulFNum_128K;
            ulUsedNum = g_stmmMgmt.ulUNum_128K;
            break;
        default:
            return;
    }

    if ( NULL == pstDList
        || NULL == pstMutex )
    {
        return;
    }

    
    VOS_RWLOCK_PLOCK(pstMutex);
    
    if ( VOS_OK != VOS_Node_IsEmpty(pstDList)  )
    {
        /* 50Line */
        VOS_Printf("########### Type:[%08x],FreeNum=%d,UsedNum=%d##########",
                    uiType, ulFreedNum, ulUsedNum);

        pstMmBuf = VOS_DLIST_ENTRY(pstDList->next, VOS_MEMBUF_UNIT_S, stNode);
        /*匹配或者0:表示全部显示*/
        if ( uiMid == pstMmBuf->ulMid )
        {
            ulCount++;
            VOS_Printf("***********MID:[%08x],File:[%s],Line:[%d]", 
                pstMmBuf->ulMid,pstMmBuf->acFileName, pstMmBuf->ulLineNum);
        }
        
        for(;pstMmBuf->stNode.next != pstDList;\
            pstMmBuf=VOS_DLIST_ENTRY(pstMmBuf->stNode.next,VOS_MEMBUF_UNIT_S, stNode))
        {
            ulCount++;
            
            if ( NULL == pstMmBuf )
            {
                break;
            }
            if ( uiMid == pstMmBuf->ulMid )
            {
                VOS_Printf("***********MID:[%08x],File:[%s],Line:[%d]", 
                    pstMmBuf->ulMid,pstMmBuf->acFileName, pstMmBuf->ulLineNum );
            }
        }
        
        VOS_Printf("################Count=%d#####################", ulCount);
    }

    VOS_RWLOCK_UNPLOCK(pstMutex);
        
   return;
}


/*****************************************************************************
 函 数 名  : VOS_MM_TypeShow
 功能描述  : 显示某一类型的内存
 输入参数  : UINT32 uiType  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年7月26日
    作    者   : jimk
    修改内容   : 新生成函数

*****************************************************************************/
VOID VOS_MM_TypeShow(UINT32 uiType)
{
    VOS_DLIST_NODE_S   *pstDList     = NULL;
    VOS_RW_LOCK_S       *pstMutex   = NULL;
    VOS_MEMBUF_UNIT_S *pstMmBuf  = NULL;   
    ULONG                       ulCount      = 0;
    ULONG                       ulFreedNum= 0 ;
    ULONG                       ulUsedNum = 0;

    switch(uiType)
    {
        case VOS_MEM_TYPE_32:
            pstDList = &g_stmmMgmt.stMMListU_32;
            pstMutex = &g_stmmLock32;
            ulFreedNum = g_stmmMgmt.ulFNum_32;
            ulUsedNum = g_stmmMgmt.ulUNum_32;
            break;
        case VOS_MEM_TYPE_64:
            pstDList = &g_stmmMgmt.stMMListU_64;
            pstMutex = &g_stmmLock64;
            ulFreedNum = g_stmmMgmt.ulFNum_64;
            ulUsedNum = g_stmmMgmt.ulUNum_64;
            break;
        case VOS_MEM_TYPE_128:
            pstDList = &g_stmmMgmt.stMMListU_128;
            pstMutex = &g_stmmLock128;
            ulFreedNum = g_stmmMgmt.ulFNum_128;
            ulUsedNum = g_stmmMgmt.ulUNum_128;
            break;
        case VOS_MEM_TYPE_256:
            pstDList = &g_stmmMgmt.stMMListU_256;
            pstMutex = &g_stmmLock256;
            ulFreedNum = g_stmmMgmt.ulFNum_256;
            ulUsedNum = g_stmmMgmt.ulUNum_256;
            break;
        case VOS_MEM_TYPE_512:
            pstDList = &g_stmmMgmt.stMMListU_512;
            pstMutex = &g_stmmLock512;
            ulFreedNum = g_stmmMgmt.ulFNum_512;
            ulUsedNum = g_stmmMgmt.ulUNum_512;
            break;
        case VOS_MEM_TYPE_1K:
            pstDList = &g_stmmMgmt.stMMListU_1K;
            pstMutex = &g_stmmLock1K;
            ulFreedNum = g_stmmMgmt.ulFNum_1K;
            ulUsedNum = g_stmmMgmt.ulUNum_1K;
            break;
        case VOS_MEM_TYPE_2K:
            pstDList = &g_stmmMgmt.stMMListU_2K;
            pstMutex = &g_stmmLock2K;
            ulFreedNum = g_stmmMgmt.ulFNum_2K;
            ulUsedNum = g_stmmMgmt.ulUNum_2K;
            break;
        case VOS_MEM_TYPE_4K:
            pstDList = &g_stmmMgmt.stMMListU_4K;
            pstMutex = &g_stmmLock4K;
            ulFreedNum = g_stmmMgmt.ulFNum_4K;
            ulUsedNum = g_stmmMgmt.ulUNum_4K;
            break;
        case VOS_MEM_TYPE_8K:
            pstDList = &g_stmmMgmt.stMMListU_8K;
            pstMutex = &g_stmmLock8K;
            ulFreedNum = g_stmmMgmt.ulFNum_8K;
            ulUsedNum = g_stmmMgmt.ulUNum_8K;
            break;
        case VOS_MEM_TYPE_16K:
            pstDList = &g_stmmMgmt.stMMListU_16K;
            pstMutex = &g_stmmLock16K;
            ulFreedNum = g_stmmMgmt.ulFNum_16K;
            ulUsedNum = g_stmmMgmt.ulUNum_16K;
            break;
        case VOS_MEM_TYPE_32K:
            pstDList = &g_stmmMgmt.stMMListU_32K;
            pstMutex = &g_stmmLock32K;
            ulFreedNum = g_stmmMgmt.ulFNum_32K;
            ulUsedNum = g_stmmMgmt.ulUNum_32K;
            break;
        case VOS_MEM_TYPE_64K:
            pstDList = &g_stmmMgmt.stMMListU_64K;
            pstMutex = &g_stmmLock64K;
            ulFreedNum = g_stmmMgmt.ulFNum_64K;
            ulUsedNum = g_stmmMgmt.ulUNum_64K;
            break;
        case VOS_MEM_TYPE_128K:
            pstDList = &g_stmmMgmt.stMMListU_128K;
            pstMutex = &g_stmmLock128K;
            ulFreedNum = g_stmmMgmt.ulFNum_128K;
            ulUsedNum = g_stmmMgmt.ulUNum_128K;
            break;
        default:
            return;
    }

    if ( NULL == pstDList
        || NULL == pstMutex )
    {
        return;
    }
    
    VOS_RWLOCK_PLOCK(pstMutex);
    
    if ( VOS_OK != VOS_Node_IsEmpty(pstDList)  )
    {
        VOS_Printf("########### Type:[%08x],FreeNum=%d,UsedNum=%d##########",
                    uiType, ulFreedNum, ulUsedNum);

        pstMmBuf = VOS_DLIST_ENTRY(pstDList->next, VOS_MEMBUF_UNIT_S, stNode);

        ulCount++;
        VOS_Printf("***********MID:[%08x], File:[%s],Line:[%d]", 
                pstMmBuf->ulMid, pstMmBuf->acFileName, pstMmBuf->ulLineNum);
        
        
        for(;pstMmBuf->stNode.next != pstDList;\
            pstMmBuf=VOS_DLIST_ENTRY(pstMmBuf->stNode.next,VOS_MEMBUF_UNIT_S, stNode))
        {
            ulCount++;
            
            if ( NULL == pstMmBuf )
            {
                break;
            }

            VOS_Printf("***********MID:[%08x], File:[%s],Line:[%d]", 
                    pstMmBuf->ulMid, pstMmBuf->acFileName, pstMmBuf->ulLineNum);
            
        }
        
        VOS_Printf("################Count=%d#####################", ulCount);
    }

    VOS_RWLOCK_UNPLOCK(pstMutex);
        
   return;
}



VOID VOS_MM_Show()
{
    UINT32 ulIndex = 0;

    for (ulIndex = VOS_MEM_TYPE_32; ulIndex <= VOS_MEM_TYPE_128K; ulIndex++)
    {
        VOS_MM_TypeShow(ulIndex);
    }
    
    return;
}
