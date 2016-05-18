
#include "ListOpr.h"
#include "api.h"
#include "malloc.h"
#include "stdlib.h"

// ��������
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


// ɾ������
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


//���ҽڵ�
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

//������β����ڵ�
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
	
	//pNode�Ƶ�β��
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
	//FIXME:������ʽ
	pTemp->data = *newNode;
	pTemp->pNext = NULL;
	pNode->pNext = pTemp;
	return pHead;
}
//��������
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