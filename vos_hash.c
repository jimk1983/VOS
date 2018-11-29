/******************************************************************************

                  版权所有 (C), 2015-2025, 网络科技有限公司

 ******************************************************************************
  文 件 名   : vos_hash.c
  版 本 号   : 初稿
  作    者   : jimk
  生成日期   : 2016年2月15日
  最近修改   :
  功能描述   : 简单的hash实现
  函数列表   :
  修改历史   :
  1.日    期   : 2016年2月15日
    作    者   : jimk
    修改内容   : 创建文件

******************************************************************************/
#include <vos_pub.h>
#include <vos_log.h>


/*素数数组*/
static ULONG g_ulPrimeArry[]={
53,
97,
193,   
389,  //3
769,  //4
1543,  
3079,
6151,
12289,
24593,
49157,
98317,
196613,
393241,
786433,
1572869,
3145739,
6291469,
12582917,
25165843,
50331653,
100663319,
201326611,
402653189,
805306457,
1610612741,
};


/*素数的数组大小*/
#define VOS_HASHTBL_PRIME_SIZE    (sizeof(g_ulPrimeArry) / sizeof(ULONG))

/*Hash的素数表中选择合理的素数*/
ULONG VOS_Hash_TablePrime(ULONG ulSize)
{
    ULONG ulCount = VOS_HASHTBL_PRIME_SIZE;
    ULONG ulIndex = 0;
    ULONG ulPrime = g_ulPrimeArry[0];
    double dmin = fabs(log((double)ulSize) - log((double)g_ulPrimeArry[0]));
    double temp;
    
    if ( (ulSize < VOS_HASHTABLE_SIZE_MIN)
        || (ulSize > VOS_HASHTABLE_SIZE_MAX) )
    {
        VOS_Printf("param error");
        return 0;
    }

    for(ulIndex = 0;ulIndex < ulCount; ulIndex++)
    {
        temp = fabs(log((double)ulSize) - log((double)g_ulPrimeArry[ulIndex]));
        if ( temp > dmin )
        {
            continue;
        }
        dmin = temp;
        
        ulPrime = g_ulPrimeArry[ulIndex];
    }

    /*选取的最佳素数*/
    return ulPrime;
}

/*字符串的hash算法， 将该返回值取%，既可获取对应的Hash值*/
ULONG VOS_Hash_String(const VOID *pcStr)
{
    ULONG ret = 0;
    LONG   n   = 0;
    ULONG v   = 0;
    INT32  r    = 0;
    CHAR *pcString = NULL;

    if ( NULL == pcStr
        || (*(CHAR *)pcStr == '\0') )
    {
        VOS_Printf("param error");
        return (ret);
    }
        
    pcString = (CHAR *)pcStr;
    
    n = 0x100;
    while (*pcString) {
        v = n | (*pcString);
        n += 0x100;
        r = (INT32)((v >> 2) ^ v) & 0x0F;
        ret = (ret << r) | (ret >> (32 - r));
        ret &= 0xFFFFFFFFL;
        ret ^= v * v;
        pcString++;
    }
    return ((ret >> 16) ^ ret);
}

/*仅仅返回为0时候表示相同*/
LONG VOS_Hash_CmpString(const VOID *pvArgv1, const VOID *pvArgv2)
{
    if ( NULL == pvArgv1 
        || NULL == pvArgv2 )
    {
        VOS_Printf("param error");
        return VOS_ERR;
    }

    return strcmp((CHAR *)pvArgv1, (CHAR *)pvArgv2);
}

/*使用ULONG创建的Hash值*/
ULONG VOS_Hash_ULONGValue(const VOID * pulKey)
{
    ULONG ulValue = 0;

    if ( NULL == pulKey )
    {
        VOS_Printf("param error");
        return 0;
    }

    ulValue = *(ULONG *)pulKey;

    return ulValue;
}

/*仅仅返回为0时候表示相同*/
LONG VOS_Hash_ULONGCmp(const VOID *pvArgv1, const VOID *pvArgv2)
{
    ULONG ulKey1 = 0;
    ULONG ulKey2 = 0;
    
    if ( NULL == pvArgv1
        || NULL == pvArgv2 )
    {
        VOS_Printf("param error");
        return VOS_ERR;    
    }
    
    ulKey1 = *(ULONG *)pvArgv1;
    ulKey2 = *(ULONG *)pvArgv2;

    if ( ulKey1 != ulKey2 )
    {
        VOS_Printf("The key has not equal");
        return VOS_ERR;
    }
    
    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : VOS_Hash_ULONGKeySaveData
 功能描述  : 通过key保存设置的数据，存放在Hash表中
 输入参数  :  VOS_HASH_TABLE_S *pstHashTbl  
                           ULONG ulKey                                  ---主要是VIP，是一个ULONG值
                           VOID *pvData                  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
说明:  这个是特殊的Hash表用法，主要用于VIP的相关数据保存，
             使用这个一定要保证ulKey在系统内是唯一的，如VIP,
             因为后面的节点冲突判断也是用该ulKey值来判断的

*****************************************************************************/
LONG VOS_Hash_ULONGKeySetData(VOS_HASH_TABLE_S *pstHashTbl, ULONG ulKey, VOID *pvData)
{
    ULONG ulHashIndex = 0;
    ULONG ulKeyValue   = 0;
    VOS_HASH_NODE_S *pstHashNode = NULL;
    VOS_HASH_NODE_S *pstHashHead = NULL;
    VOS_HASH_NODE_S *pstHashTmp  = NULL;
    
    if ( NULL == pstHashTbl 
        || NULL == pvData )
    {
        VOS_Printf("param error");
        return VOS_ERR;
    }

    /*新建一个Hash节点*/
    pstHashNode = (VOS_HASH_NODE_S *)VOS_Malloc(VOS_MID, sizeof(VOS_HASH_NODE_S));
    if ( NULL == pstHashNode )
    {
        VOS_Printf("malloc error");
        return VOS_ERR;
    }

    VOS_Mem_Zero((CHAR *)pstHashNode, sizeof(VOS_HASH_NODE_S));
    VOS_DLIST_INIT(&pstHashNode->stBlkList);
    
    /*该Hash节点的ULONG值*/
    pstHashNode->ulKey   = ulKey;
    pstHashNode->pvData = pvData;

    /*计算出Hash值*/
    ulKeyValue  = pstHashTbl->pfHashCode((VOID *)&ulKey);
    /*素数取余*/
    ulHashIndex = ulKeyValue % pstHashTbl->lPrime;
    
    if ( NULL == pstHashTbl->ppstArrayHashTbl[ulHashIndex] )
    {
        /*如果存在空闲，则直接插入即可*/
        pstHashTbl->ppstArrayHashTbl[ulHashIndex] = pstHashNode;
        return VOS_OK;
    }
    

    /*先比较第一个头节点，看看有无冲突*/
    pstHashHead = VOS_DLIST_ENTRY(&(pstHashTbl->ppstArrayHashTbl[ulHashIndex]->stBlkList), VOS_HASH_NODE_S, stBlkList);
    if (VOS_OK == pstHashTbl->pfHashKeyCmp((VOID *)&ulKey, (VOID *)&pstHashHead->ulKey ))
    {
        VOS_Printf("the key has already exist!");
        /*如果冲突，则表示出错*/
        VOS_Free((CHAR *)pstHashNode);
        return VOS_ERR;
    }
        
    /*如果目前只有一个节点, 那么就直接加入冲突链表*/
    if ( 0 == pstHashHead->ulBlkNum )
    {
        /*直接插入后面*/
        VOS_DLIST_ADD_TAIL(&pstHashHead->stBlkList, &pstHashNode->stBlkList);
        pstHashHead->ulBlkNum++;
        return VOS_OK;
    }
    /*存在多个节点冲突的情况*/
    else
    {
        /*前面第一个节点已经比较，下一个next节点开始*/
        for( pstHashTmp = VOS_DLIST_ENTRY(pstHashHead->stBlkList.next, VOS_HASH_NODE_S, stBlkList);\
                &(pstHashTmp->stBlkList) != &(pstHashHead->stBlkList);\
               pstHashTmp= VOS_DLIST_ENTRY(pstHashTmp->stBlkList.next, VOS_HASH_NODE_S, stBlkList))
        {
             /*遍历看看是否有冲突*/
             if (VOS_OK == pstHashTbl->pfHashKeyCmp((VOID *)&ulKey, (VOID *)&pstHashTmp->ulKey ))
            {
                VOS_Printf("the key has already exist!");
                /*如果冲突，则表示出错*/
                VOS_Free((CHAR *)pstHashNode);
                return VOS_ERR;
            }
        }
        /*直接插入后面*/
        VOS_DLIST_ADD_TAIL(&pstHashHead->stBlkList, &pstHashNode->stBlkList);
        pstHashHead->ulBlkNum++;
    }
    
    return VOS_OK;
}


/*****************************************************************************
 函 数 名  : VOS_Hash_ULONGKeyGetData
 功能描述  : 获取Hash表的数据，可以读写
 输入参数  : VOS_HASH_TABLE_S *pstHashTbl  
             ULONG ulKey                   
             VOID **pvData                 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年11月14日
    作    者   : jimk
    修改内容   : 新生成函数
    
    插入ULONG 类型的Hash值，这种插入法比较节省空间，但是统一性不太好
*****************************************************************************/
LONG VOS_Hash_ULONGKeyGetData(VOS_HASH_TABLE_S *pstHashTbl, ULONG ulKey, VOID **pvData)
{
    ULONG ulHashIndex = 0;
    ULONG ulKeyValue   = 0;
    VOS_HASH_NODE_S *pstHashHead = NULL;
    VOS_HASH_NODE_S *pstHashTmp  = NULL;
    
    if ( NULL == pstHashTbl 
        || NULL == pvData )
    {
        VOS_Printf("param error");
        return VOS_ERR;
    }

    /*计算出Hash值*/
    ulKeyValue  = pstHashTbl->pfHashCode((VOID *)&ulKey);
    /*素数取余*/
    ulHashIndex = ulKeyValue % pstHashTbl->lPrime;

    /*没有找到*/
    if ( NULL == pstHashTbl->ppstArrayHashTbl[ulHashIndex] )
    {
        return VOS_ERR;
    }

    pstHashHead = VOS_DLIST_ENTRY(&(pstHashTbl->ppstArrayHashTbl[ulHashIndex]->stBlkList), VOS_HASH_NODE_S, stBlkList);

    /*目前只有一个节点*/
    if ( 0 == pstHashHead->ulBlkNum )
    {
        /*当前一个节点，那么就直接获取当前的节点数据*/
        *pvData = pstHashHead->pvData;
        
        return VOS_OK;
    }
    /*存在多个节点冲突的情况*/
    else if ( 0 < pstHashHead->ulBlkNum )
    {
        /*看看是否 当前第一个有相同的KEY*/
        if (VOS_OK == pstHashTbl->pfHashKeyCmp((VOID *)&ulKey, (VOID *)&pstHashHead->ulKey ))
        {
            /*相同KEY直接返回数据*/
            *pvData = pstHashTbl->ppstArrayHashTbl[ulHashIndex]->pvData;
    
            return VOS_OK;
        }
        
        /*前面第一个节点已经比较，下一个next节点开始*/
        for( pstHashTmp = VOS_DLIST_ENTRY(pstHashHead->stBlkList.next, VOS_HASH_NODE_S, stBlkList);\
                &(pstHashTmp->stBlkList) != &(pstHashHead->stBlkList);\
               pstHashTmp= VOS_DLIST_ENTRY(pstHashTmp->stBlkList.next, VOS_HASH_NODE_S, stBlkList))
        {
             /*遍历看看是否有相同的KEY*/
             if (VOS_OK == pstHashTbl->pfHashKeyCmp((VOID *)&ulKey, (VOID *)&pstHashTmp->ulKey ))
            {
                *pvData = pstHashTbl->ppstArrayHashTbl[ulHashIndex]->pvData;
        
                return VOS_OK;
            }
        }
    }
    
    return VOS_ERR;
}


/*删除节点*/
LONG VOS_Hash_ULONGKeyRemove(VOS_HASH_TABLE_S *pstHashTbl, ULONG ulKey)
{
    ULONG ulHashIndex = 0;
    ULONG ulKeyValue   = 0;
    VOS_HASH_NODE_S *pstHashHead = NULL;
    VOS_HASH_NODE_S *pstHashTmp  = NULL;

    if ( NULL == pstHashTbl )
    {
        VOS_Printf("param error");
        return VOS_ERR;
    }
    
    /*计算出Hash值*/
    ulKeyValue  = pstHashTbl->pfHashCode((VOID *)&ulKey);
    /*素数取余*/
    ulHashIndex = ulKeyValue % pstHashTbl->lPrime;

    pstHashHead = pstHashTbl->ppstArrayHashTbl[ulHashIndex];

    if ( NULL == pstHashHead )
    {
        VOS_Printf("hash table index[%d] is empty", ulHashIndex);
        /*错误*/
        return VOS_ERR;
    }
    
    /*目前只有一个节点*/
    if ( 0 == pstHashHead->ulBlkNum )
    {
        /*看看是否相同*/
        if ( VOS_ERR == pstHashTbl->pfHashKeyCmp((VOID *)&ulKey, &pstHashHead->ulKey ) )
        {
            /*不可能发生的*/
            VOS_Printf("system error");
            return VOS_ERR;
        }
        /*找到，直接删除*/
        VOS_Free((CHAR *)pstHashTbl->ppstArrayHashTbl[ulHashIndex]);
        pstHashTbl->ppstArrayHashTbl[ulHashIndex]  = NULL;

        return VOS_OK;
    }
    /*存在多个节点冲突的情况*/
    else if ( 0 < pstHashHead->ulBlkNum )
    {
        /*先比较第一个头节点，看看有无相同的KEY*/
        pstHashHead = VOS_DLIST_ENTRY(&(pstHashTbl->ppstArrayHashTbl[ulHashIndex]->stBlkList), VOS_HASH_NODE_S, stBlkList);
        /*遍历看看该头节点是否即为该相同的KEY*/
        if (VOS_OK == pstHashTbl->pfHashKeyCmp((VOID *)&ulKey, (VOID *)&pstHashHead->ulKey ))
        {
            /*先减掉一个*/
            pstHashHead->ulBlkNum--;
            /*得把下一个节点设置为头节点*/
            pstHashTmp = VOS_DLIST_ENTRY(pstHashHead->stBlkList.next, VOS_HASH_NODE_S, stBlkList);
            /*将少了一个节点的值，赋值给未来的头节点*/
            pstHashTmp->ulBlkNum = pstHashHead->ulBlkNum;
            /*设置后面的这个节点为新的头节点*/
            pstHashTbl->ppstArrayHashTbl[ulHashIndex] = pstHashTmp;
            /*将自己删除*/
            VOS_DLIST_DEL(&pstHashHead->stBlkList);
            
            VOS_Free((CHAR *)pstHashTmp->pvData);
            VOS_Free((CHAR *)pstHashTmp);
            
            return VOS_OK;
        }
        
        /*前面第一个节点已经比较，从下一个节点开始*/
        for( pstHashTmp = VOS_DLIST_ENTRY(pstHashHead->stBlkList.next, VOS_HASH_NODE_S, stBlkList);\
                &(pstHashTmp->stBlkList) != &(pstHashHead->stBlkList);\
               pstHashTmp= VOS_DLIST_ENTRY(pstHashTmp->stBlkList.next, VOS_HASH_NODE_S, stBlkList))
        {
             /*遍历看看是否有相同的KEY*/
             if (VOS_OK == pstHashTbl->pfHashKeyCmp((VOID *)&ulKey, (VOID *)&pstHashTmp->ulKey ))
            {
                /*因为数量都是头节点管理的，先减掉一个,
                 而且经过前面的判断，头节点没有相同，删除的不是头节点*/
                pstHashHead->ulBlkNum--;
                /*可以将自己删除*/
                VOS_DLIST_DEL(&pstHashTmp->stBlkList);

                VOS_Free((CHAR *)pstHashTmp->pvData);
                VOS_Free((CHAR *)pstHashTmp);
                return VOS_OK;
            }
        }
    }
        
    return VOS_ERR;
}

LONG VOS_Hash_StringKeySetData(VOS_HASH_TABLE_S *pstHashTbl, CHAR *pcString, VOID *pvData)
{
    

    return VOS_OK;
}


LONG VOS_Hash_StringKeyRemove(VOS_HASH_TABLE_S *pstHashTbl, CHAR *pcString)
{

    return VOS_OK;
}

/*普通型的Hash表创建*/
VOS_HASH_TABLE_S *VOS_Hash_TableCreate(ULONG ulSize, VOS_HASH_COMP_FUNC pfHashCmp, VOS_HASH_VALUE_FUNC pfHashVal)
{
    VOS_HASH_TABLE_S *pstHashTbl = NULL;
    ULONG                    ulIndex = 0;

    /*因为Hash表不会经常创建和释放，所以直接使用系统的, 确保内存使用安全*/
    pstHashTbl = (VOS_HASH_TABLE_S *)malloc(sizeof(VOS_HASH_TABLE_S));
    if ( NULL == pstHashTbl )
    {
        VOS_Printf("malloc hash table error");
        return NULL;
    }
    
    /*这个内存因为申请之后就不再释放,因此直接使用系统的*/
    memset(pstHashTbl, 0 , sizeof(VOS_HASH_TABLE_S));

    /*选取合适的素数*/
    pstHashTbl->lPrime = VOS_Hash_TablePrime(ulSize);
    if ( 0 == pstHashTbl->lPrime )
    {
        free(pstHashTbl);
        return NULL;
    }
    
    /*申请hash表中的Hash节点, 通过选取的合适的素数来创建数组大小*/
    pstHashTbl->ppstArrayHashTbl = (VOS_HASH_NODE_S **)malloc(sizeof(VOS_HASH_NODE_S *) * pstHashTbl->lPrime );
    if ( NULL == pstHashTbl->ppstArrayHashTbl )
    {
        free(pstHashTbl);
        return NULL;
    }

    /*初始化每个Hash节点*/
    for(;ulIndex<pstHashTbl->lPrime; ulIndex++)
    {
        pstHashTbl->ppstArrayHashTbl[ulIndex] = NULL;
    }

    /*默认创建字符型的Hash表*/
    pstHashTbl->pfHashKeyCmp = ((pfHashCmp == NULL) ? (VOS_HASH_COMP_FUNC)VOS_Hash_CmpString : pfHashCmp);
    pstHashTbl->pfHashCode = ((pfHashVal == NULL) ? (VOS_HASH_VALUE_FUNC)VOS_Hash_String : pfHashVal);
        
    return pstHashTbl;
}

/*Hash表释放， 直接全部释放*/
VOID  VOS_Hash_TableRelease(VOS_HASH_TABLE_S *pstHashTbl)
{
    ULONG uiCount = 0;

    if ( NULL != pstHashTbl )
    {
        if (NULL != pstHashTbl->ppstArrayHashTbl)
        {
            for (uiCount = 0; uiCount< pstHashTbl->lPrime; uiCount++)
            {
                /*这里暂不关心冲突链的释放了，可能会有小部分内存泄漏
                因为采用的是VOS内存管理, 统一由VOS释放，
                这里进行简单释放*/
                if ( NULL != pstHashTbl->ppstArrayHashTbl[uiCount] )
                {
                    VOS_Free((CHAR *)pstHashTbl->ppstArrayHashTbl[uiCount]);
                }
            }
        
            free(pstHashTbl->ppstArrayHashTbl);
        }

        free(pstHashTbl);
    }
}

 
 