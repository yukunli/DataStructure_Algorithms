
#include "ListOpr.h"
#include "api.h"
#include "malloc.h"
#include "stdlib.h"

// 创建链表
QueryResultNode* CreatList(void)
{
	QueryResultNode *pHead = NULL;
	pHead = (QueryResultNode *)malloc(sizeof(QueryResultNode));
	if(NULL == pHead)
	{
        api_print_result(E999);
		exit(0);
	}
	pHead ->pNext = NULL;
	return pHead;
}


// 删除链表
QueryResultNode* RemoveList(QueryResultNode *pHead)
{
	QueryResultNode *pNext = NULL;
	QueryResultNode *pNode = NULL;
	pNode = pHead;
	while(pNode)
	{
		pNext = pNode->pNext;
		free(pNode);
		pNode = pNext;
	}
	pHead = NULL;
	pNext = NULL;
	pNode = NULL;
	return pHead;
}


//查找节点
QueryResultNode* FindNodeInList(QueryResultNode *pHandle, int iCradNo)
{
	QueryResultNode * pNode = NULL;
	if (NULL == pHandle)
    {
        return NULL;
    }
	
	pNode = pHandle -> pNext;
	
	while(pNode != NULL)
    {
        if ((pNode->data).Index  == iCradNo)
        {
            return pNode;
        }
        else
        {
            pNode = pNode -> pNext;
        }
    }
    return NULL;
}

//在链表尾加入节点
QueryResultNode* AddNodeInList(QueryResultNode *pHead, QueryResult *newNode)
{

	//ERROR
	if((NULL == pHead) ||(NULL == newNode))
	{
		return NULL;
	}

	QueryResultNode *pTemp = NULL;
	QueryResultNode *pNode = NULL;
	pNode = pHead;
	
	//pNode移到尾部
	while(pNode->pNext != NULL)
	{
		pNode =pNode->pNext;
	}
	
	pTemp = (QueryResultNode *)malloc(sizeof(QueryResultNode));
	if(NULL == pTemp)
	{
		api_print_result(E999);
		exit(0);
	}
	//FIXME:拷贝方式
	pTemp->data = *newNode;
	pTemp->pNext = NULL;
	pNode->pNext = pTemp;
	return pHead;
}
//拷贝链表
void CopyList(QueryResultNode *pHeada, QueryResultNode *pHeadb)
{
	QueryResultNode *pNode = NULL;

	if ( NULL == pHeadb || NULL == pHeada)
	{
		return;
	}
	pHeadb = pHeadb->pNext;
	while(NULL != pHeadb)
	{
		pNode = (QueryResultNode *)malloc(sizeof(QueryResultNode));
		if (NULL == pNode)
		{
			api_print_result(E999);
			return;
		}
		pNode->data = pHeadb->data;
		pNode->pNext = NULL;
		pHeada->pNext = pNode;
		pNode = NULL;

		pHeada = pHeada->pNext;
		pHeadb = pHeadb->pNext;
		
	}
	return;
}