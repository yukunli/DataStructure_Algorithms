


#ifndef __LISTOPR_H__
#define __LISTOPR_H__

#include "api.h"


LoanHistoryInfoNode* CreatList(void);

LoanHistoryInfoNode* CreatList(LoanHistoryInfoNode *pHead);

//���ҽڵ�
LoanHistoryInfoNode* FindNodeInList(LoanHistoryInfoNode *pHandle, int iCradNo);

LoanHistoryInfoNode* AddNodeInList(LoanHistoryInfoNode *pHead, LoanHistoryInfoItem *newNode);

// ɾ������
LoanHistoryInfoNode* RemoveList(LoanHistoryInfoNode *pHead);
#endif