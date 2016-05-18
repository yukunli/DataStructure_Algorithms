


#ifndef __LISTOPR_H__
#define __LISTOPR_H__

#include "api.h"


QueryResultNode* CreatList(void);

QueryResultNode* CreatList(QueryResultNode *pHead);

//查找节点
QueryResultNode* FindNodeInList(QueryResultNode *pHandle, int iCradNo);

QueryResultNode* AddNodeInList(QueryResultNode *pHead, QueryResult *newNode);

// 删除链表
QueryResultNode* RemoveList(QueryResultNode *pHead);
// 链表拷贝
void CopyList(QueryResultNode *pHead1, QueryResultNode *pHead2);
#endif