/******************************************************************************

                  ��Ȩ���� (C), 2015-2025, ����Ƽ����޹�˾

 ******************************************************************************
  �� �� ��   : vos_hash.c
  �� �� ��   : ����
  ��    ��   : jimk
  ��������   : 2016��2��15��
  ����޸�   :
  ��������   : �򵥵�hashʵ��
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2016��2��15��
    ��    ��   : jimk
    �޸�����   : �����ļ�

******************************************************************************/
#include <vos_pub.h>
#include <vos_log.h>


/*��������*/
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


/*�����������С*/
#define VOS_HASHTBL_PRIME_SIZE    (sizeof(g_ulPrimeArry) / sizeof(ULONG))

/*Hash����������ѡ����������*/
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

    /*ѡȡ���������*/
    return ulPrime;
}

/*�ַ�����hash�㷨�� ���÷���ֵȡ%���ȿɻ�ȡ��Ӧ��Hashֵ*/
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

/*��������Ϊ0ʱ���ʾ��ͬ*/
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

/*ʹ��ULONG������Hashֵ*/
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

/*��������Ϊ0ʱ���ʾ��ͬ*/
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
 �� �� ��  : VOS_Hash_ULONGKeySaveData
 ��������  : ͨ��key�������õ����ݣ������Hash����
 �������  :  VOS_HASH_TABLE_S *pstHashTbl  
                           ULONG ulKey                                  ---��Ҫ��VIP����һ��ULONGֵ
                           VOID *pvData                  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
˵��:  ����������Hash���÷�����Ҫ����VIP��������ݱ��棬
             ʹ�����һ��Ҫ��֤ulKey��ϵͳ����Ψһ�ģ���VIP,
             ��Ϊ����Ľڵ��ͻ�ж�Ҳ���ø�ulKeyֵ���жϵ�

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

    /*�½�һ��Hash�ڵ�*/
    pstHashNode = (VOS_HASH_NODE_S *)VOS_Malloc(VOS_MID, sizeof(VOS_HASH_NODE_S));
    if ( NULL == pstHashNode )
    {
        VOS_Printf("malloc error");
        return VOS_ERR;
    }

    VOS_Mem_Zero((CHAR *)pstHashNode, sizeof(VOS_HASH_NODE_S));
    VOS_DLIST_INIT(&pstHashNode->stBlkList);
    
    /*��Hash�ڵ��ULONGֵ*/
    pstHashNode->ulKey   = ulKey;
    pstHashNode->pvData = pvData;

    /*�����Hashֵ*/
    ulKeyValue  = pstHashTbl->pfHashCode((VOID *)&ulKey);
    /*����ȡ��*/
    ulHashIndex = ulKeyValue % pstHashTbl->lPrime;
    
    if ( NULL == pstHashTbl->ppstArrayHashTbl[ulHashIndex] )
    {
        /*������ڿ��У���ֱ�Ӳ��뼴��*/
        pstHashTbl->ppstArrayHashTbl[ulHashIndex] = pstHashNode;
        return VOS_OK;
    }
    

    /*�ȱȽϵ�һ��ͷ�ڵ㣬�������޳�ͻ*/
    pstHashHead = VOS_DLIST_ENTRY(&(pstHashTbl->ppstArrayHashTbl[ulHashIndex]->stBlkList), VOS_HASH_NODE_S, stBlkList);
    if (VOS_OK == pstHashTbl->pfHashKeyCmp((VOID *)&ulKey, (VOID *)&pstHashHead->ulKey ))
    {
        VOS_Printf("the key has already exist!");
        /*�����ͻ�����ʾ����*/
        VOS_Free((CHAR *)pstHashNode);
        return VOS_ERR;
    }
        
    /*���Ŀǰֻ��һ���ڵ�, ��ô��ֱ�Ӽ����ͻ����*/
    if ( 0 == pstHashHead->ulBlkNum )
    {
        /*ֱ�Ӳ������*/
        VOS_DLIST_ADD_TAIL(&pstHashHead->stBlkList, &pstHashNode->stBlkList);
        pstHashHead->ulBlkNum++;
        return VOS_OK;
    }
    /*���ڶ���ڵ��ͻ�����*/
    else
    {
        /*ǰ���һ���ڵ��Ѿ��Ƚϣ���һ��next�ڵ㿪ʼ*/
        for( pstHashTmp = VOS_DLIST_ENTRY(pstHashHead->stBlkList.next, VOS_HASH_NODE_S, stBlkList);\
                &(pstHashTmp->stBlkList) != &(pstHashHead->stBlkList);\
               pstHashTmp= VOS_DLIST_ENTRY(pstHashTmp->stBlkList.next, VOS_HASH_NODE_S, stBlkList))
        {
             /*���������Ƿ��г�ͻ*/
             if (VOS_OK == pstHashTbl->pfHashKeyCmp((VOID *)&ulKey, (VOID *)&pstHashTmp->ulKey ))
            {
                VOS_Printf("the key has already exist!");
                /*�����ͻ�����ʾ����*/
                VOS_Free((CHAR *)pstHashNode);
                return VOS_ERR;
            }
        }
        /*ֱ�Ӳ������*/
        VOS_DLIST_ADD_TAIL(&pstHashHead->stBlkList, &pstHashNode->stBlkList);
        pstHashHead->ulBlkNum++;
    }
    
    return VOS_OK;
}


/*****************************************************************************
 �� �� ��  : VOS_Hash_ULONGKeyGetData
 ��������  : ��ȡHash������ݣ����Զ�д
 �������  : VOS_HASH_TABLE_S *pstHashTbl  
             ULONG ulKey                   
             VOID **pvData                 
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��11��14��
    ��    ��   : jimk
    �޸�����   : �����ɺ���
    
    ����ULONG ���͵�Hashֵ�����ֲ��뷨�ȽϽ�ʡ�ռ䣬����ͳһ�Բ�̫��
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

    /*�����Hashֵ*/
    ulKeyValue  = pstHashTbl->pfHashCode((VOID *)&ulKey);
    /*����ȡ��*/
    ulHashIndex = ulKeyValue % pstHashTbl->lPrime;

    /*û���ҵ�*/
    if ( NULL == pstHashTbl->ppstArrayHashTbl[ulHashIndex] )
    {
        return VOS_ERR;
    }

    pstHashHead = VOS_DLIST_ENTRY(&(pstHashTbl->ppstArrayHashTbl[ulHashIndex]->stBlkList), VOS_HASH_NODE_S, stBlkList);

    /*Ŀǰֻ��һ���ڵ�*/
    if ( 0 == pstHashHead->ulBlkNum )
    {
        /*��ǰһ���ڵ㣬��ô��ֱ�ӻ�ȡ��ǰ�Ľڵ�����*/
        *pvData = pstHashHead->pvData;
        
        return VOS_OK;
    }
    /*���ڶ���ڵ��ͻ�����*/
    else if ( 0 < pstHashHead->ulBlkNum )
    {
        /*�����Ƿ� ��ǰ��һ������ͬ��KEY*/
        if (VOS_OK == pstHashTbl->pfHashKeyCmp((VOID *)&ulKey, (VOID *)&pstHashHead->ulKey ))
        {
            /*��ͬKEYֱ�ӷ�������*/
            *pvData = pstHashTbl->ppstArrayHashTbl[ulHashIndex]->pvData;
    
            return VOS_OK;
        }
        
        /*ǰ���һ���ڵ��Ѿ��Ƚϣ���һ��next�ڵ㿪ʼ*/
        for( pstHashTmp = VOS_DLIST_ENTRY(pstHashHead->stBlkList.next, VOS_HASH_NODE_S, stBlkList);\
                &(pstHashTmp->stBlkList) != &(pstHashHead->stBlkList);\
               pstHashTmp= VOS_DLIST_ENTRY(pstHashTmp->stBlkList.next, VOS_HASH_NODE_S, stBlkList))
        {
             /*���������Ƿ�����ͬ��KEY*/
             if (VOS_OK == pstHashTbl->pfHashKeyCmp((VOID *)&ulKey, (VOID *)&pstHashTmp->ulKey ))
            {
                *pvData = pstHashTbl->ppstArrayHashTbl[ulHashIndex]->pvData;
        
                return VOS_OK;
            }
        }
    }
    
    return VOS_ERR;
}


/*ɾ���ڵ�*/
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
    
    /*�����Hashֵ*/
    ulKeyValue  = pstHashTbl->pfHashCode((VOID *)&ulKey);
    /*����ȡ��*/
    ulHashIndex = ulKeyValue % pstHashTbl->lPrime;

    pstHashHead = pstHashTbl->ppstArrayHashTbl[ulHashIndex];

    if ( NULL == pstHashHead )
    {
        VOS_Printf("hash table index[%d] is empty", ulHashIndex);
        /*����*/
        return VOS_ERR;
    }
    
    /*Ŀǰֻ��һ���ڵ�*/
    if ( 0 == pstHashHead->ulBlkNum )
    {
        /*�����Ƿ���ͬ*/
        if ( VOS_ERR == pstHashTbl->pfHashKeyCmp((VOID *)&ulKey, &pstHashHead->ulKey ) )
        {
            /*�����ܷ�����*/
            VOS_Printf("system error");
            return VOS_ERR;
        }
        /*�ҵ���ֱ��ɾ��*/
        VOS_Free((CHAR *)pstHashTbl->ppstArrayHashTbl[ulHashIndex]);
        pstHashTbl->ppstArrayHashTbl[ulHashIndex]  = NULL;

        return VOS_OK;
    }
    /*���ڶ���ڵ��ͻ�����*/
    else if ( 0 < pstHashHead->ulBlkNum )
    {
        /*�ȱȽϵ�һ��ͷ�ڵ㣬����������ͬ��KEY*/
        pstHashHead = VOS_DLIST_ENTRY(&(pstHashTbl->ppstArrayHashTbl[ulHashIndex]->stBlkList), VOS_HASH_NODE_S, stBlkList);
        /*����������ͷ�ڵ��Ƿ�Ϊ����ͬ��KEY*/
        if (VOS_OK == pstHashTbl->pfHashKeyCmp((VOID *)&ulKey, (VOID *)&pstHashHead->ulKey ))
        {
            /*�ȼ���һ��*/
            pstHashHead->ulBlkNum--;
            /*�ð���һ���ڵ�����Ϊͷ�ڵ�*/
            pstHashTmp = VOS_DLIST_ENTRY(pstHashHead->stBlkList.next, VOS_HASH_NODE_S, stBlkList);
            /*������һ���ڵ��ֵ����ֵ��δ����ͷ�ڵ�*/
            pstHashTmp->ulBlkNum = pstHashHead->ulBlkNum;
            /*���ú��������ڵ�Ϊ�µ�ͷ�ڵ�*/
            pstHashTbl->ppstArrayHashTbl[ulHashIndex] = pstHashTmp;
            /*���Լ�ɾ��*/
            VOS_DLIST_DEL(&pstHashHead->stBlkList);
            
            VOS_Free((CHAR *)pstHashTmp->pvData);
            VOS_Free((CHAR *)pstHashTmp);
            
            return VOS_OK;
        }
        
        /*ǰ���һ���ڵ��Ѿ��Ƚϣ�����һ���ڵ㿪ʼ*/
        for( pstHashTmp = VOS_DLIST_ENTRY(pstHashHead->stBlkList.next, VOS_HASH_NODE_S, stBlkList);\
                &(pstHashTmp->stBlkList) != &(pstHashHead->stBlkList);\
               pstHashTmp= VOS_DLIST_ENTRY(pstHashTmp->stBlkList.next, VOS_HASH_NODE_S, stBlkList))
        {
             /*���������Ƿ�����ͬ��KEY*/
             if (VOS_OK == pstHashTbl->pfHashKeyCmp((VOID *)&ulKey, (VOID *)&pstHashTmp->ulKey ))
            {
                /*��Ϊ��������ͷ�ڵ����ģ��ȼ���һ��,
                 ���Ҿ���ǰ����жϣ�ͷ�ڵ�û����ͬ��ɾ���Ĳ���ͷ�ڵ�*/
                pstHashHead->ulBlkNum--;
                /*���Խ��Լ�ɾ��*/
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

/*��ͨ�͵�Hash����*/
VOS_HASH_TABLE_S *VOS_Hash_TableCreate(ULONG ulSize, VOS_HASH_COMP_FUNC pfHashCmp, VOS_HASH_VALUE_FUNC pfHashVal)
{
    VOS_HASH_TABLE_S *pstHashTbl = NULL;
    ULONG                    ulIndex = 0;

    /*��ΪHash���ᾭ���������ͷţ�����ֱ��ʹ��ϵͳ��, ȷ���ڴ�ʹ�ð�ȫ*/
    pstHashTbl = (VOS_HASH_TABLE_S *)malloc(sizeof(VOS_HASH_TABLE_S));
    if ( NULL == pstHashTbl )
    {
        VOS_Printf("malloc hash table error");
        return NULL;
    }
    
    /*����ڴ���Ϊ����֮��Ͳ����ͷ�,���ֱ��ʹ��ϵͳ��*/
    memset(pstHashTbl, 0 , sizeof(VOS_HASH_TABLE_S));

    /*ѡȡ���ʵ�����*/
    pstHashTbl->lPrime = VOS_Hash_TablePrime(ulSize);
    if ( 0 == pstHashTbl->lPrime )
    {
        free(pstHashTbl);
        return NULL;
    }
    
    /*����hash���е�Hash�ڵ�, ͨ��ѡȡ�ĺ��ʵ����������������С*/
    pstHashTbl->ppstArrayHashTbl = (VOS_HASH_NODE_S **)malloc(sizeof(VOS_HASH_NODE_S *) * pstHashTbl->lPrime );
    if ( NULL == pstHashTbl->ppstArrayHashTbl )
    {
        free(pstHashTbl);
        return NULL;
    }

    /*��ʼ��ÿ��Hash�ڵ�*/
    for(;ulIndex<pstHashTbl->lPrime; ulIndex++)
    {
        pstHashTbl->ppstArrayHashTbl[ulIndex] = NULL;
    }

    /*Ĭ�ϴ����ַ��͵�Hash��*/
    pstHashTbl->pfHashKeyCmp = ((pfHashCmp == NULL) ? (VOS_HASH_COMP_FUNC)VOS_Hash_CmpString : pfHashCmp);
    pstHashTbl->pfHashCode = ((pfHashVal == NULL) ? (VOS_HASH_VALUE_FUNC)VOS_Hash_String : pfHashVal);
        
    return pstHashTbl;
}

/*Hash���ͷţ� ֱ��ȫ���ͷ�*/
VOID  VOS_Hash_TableRelease(VOS_HASH_TABLE_S *pstHashTbl)
{
    ULONG uiCount = 0;

    if ( NULL != pstHashTbl )
    {
        if (NULL != pstHashTbl->ppstArrayHashTbl)
        {
            for (uiCount = 0; uiCount< pstHashTbl->lPrime; uiCount++)
            {
                /*�����ݲ����ĳ�ͻ�����ͷ��ˣ����ܻ���С�����ڴ�й©
                ��Ϊ���õ���VOS�ڴ����, ͳһ��VOS�ͷţ�
                ������м��ͷ�*/
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

 
 