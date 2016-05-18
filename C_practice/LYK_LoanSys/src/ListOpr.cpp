
#include "ListOpr.h"
#include "api.h"
#include "malloc.h"
#include "stdlib.h"

// 创建链表
LoanHistoryInfoNode* CreatList(void)
{
	LoanHistoryInfoNode *pHead = NULL;
	pHead = (LoanHistoryInfoNode *)malloc(sizeof(LoanHistoryInfoNode));
	if(NULL == pHead)
	{
        exit(0);
	}
	pHead ->pNext = NULL;
	return pHead;
}


// 删除链表
LoanHistoryInfoNode* RemoveList(LoanHistoryInfoNode *pHead)
{
	LoanHistoryInfoNode *pNext = NULL;
	LoanHistoryInfoNode *pNode = NULL;
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
LoanHistoryInfoNode* FindNodeInList(LoanHistoryInfoNode *pHandle, int iCradNo)
{
	LoanHistoryInfoNode * pNode = NULL;
	if (NULL == pHandle)
    {
        return NULL;
    }
	
	pNode = pHandle -> pNext;
	
	while(pNode != NULL)
    {
        if ((pNode->data).loanIdx == iCradNo)
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
LoanHistoryInfoNode* AddNodeInList(LoanHistoryInfoNode *pHead, LoanHistoryInfoItem *newNode)
{

	//ERROR
	if((NULL == pHead) ||(NULL == newNode))
	{
		return NULL;
	}

	LoanHistoryInfoNode *pTemp = NULL;
	LoanHistoryInfoNode *pNode = NULL;
	pNode = pHead;
	
	//pNode移到尾部
	while(pNode->pNext != NULL)
	{
		pNode =pNode->pNext;
	}
	
	pTemp = (LoanHistoryInfoNode *)malloc(sizeof(LoanHistoryInfoNode));
	if(NULL == pTemp)
	{
		exit(0);
	}
	//FIXME:拷贝方式
	pTemp->data = *newNode;
	pTemp->pNext = NULL;
	pNode->pNext = pTemp;
	return pHead;

}