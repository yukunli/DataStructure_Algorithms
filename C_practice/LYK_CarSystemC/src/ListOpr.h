


#ifndef __LISTOPR_H__
#define __LISTOPR_H__

#include "api.h"


QueryResultNode* CreatList(void);

QueryResultNode* CreatList(QueryResultNode *pHead);

//���ҽڵ�
QueryResultNode* FindNodeInList(QueryResultNode *pHandle, int iCradNo);

QueryResultNode* AddNodeInList(QueryResultNode *pHead, QueryResult *newNode);

// ɾ������
QueryResultNode* RemoveList(QueryResultNode *pHead);
// ������
void CopyList(QueryResultNode *pHead1, QueryResultNode *pHead2);
#endif