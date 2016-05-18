


#ifndef __LISTOPR_H__
#define __LISTOPR_H__

#include "api.h"


LoanHistoryInfoNode* CreatList(void);

LoanHistoryInfoNode* CreatList(LoanHistoryInfoNode *pHead);

//查找节点
LoanHistoryInfoNode* FindNodeInList(LoanHistoryInfoNode *pHandle, int iCradNo);

LoanHistoryInfoNode* AddNodeInList(LoanHistoryInfoNode *pHead, LoanHistoryInfoItem *newNode);

// 删除链表
LoanHistoryInfoNode* RemoveList(LoanHistoryInfoNode *pHead);
#endif