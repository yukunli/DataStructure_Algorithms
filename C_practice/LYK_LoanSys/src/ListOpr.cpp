
#include "ListOpr.h"
#include "api.h"
#include "malloc.h"
#include "stdlib.h"

// ��������
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


// ɾ������
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


//���ҽڵ�
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

//������β����ڵ�
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
	
	//pNode�Ƶ�β��
	while(pNode->pNext != NULL)
	{
		pNode =pNode->pNext;
	}
	
	pTemp = (LoanHistoryInfoNode *)malloc(sizeof(LoanHistoryInfoNode));
	if(NULL == pTemp)
	{
		exit(0);
	}
	//FIXME:������ʽ
	pTemp->data = *newNode;
	pTemp->pNext = NULL;
	pNode->pNext = pTemp;
	return pHead;

}