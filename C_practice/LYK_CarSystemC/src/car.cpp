#include <string.h>
#include "stdio.h"
#include <malloc.h>
#include "api.h"
#include "car.h"
#include "ListOpr.h"

/**************************************************/
/*ȫ�ֱ���*/
/**************************************************/
QueryResultNode * gQueryResultHisttoryList = NULL;
SystemInfo gSystemInitInfo = {0};

/**************************************************/
void main(int argc, char* argv[])
{
    /* ����Socket��������5555�˿�(sapi_server_start������lib����ʵ��)��
     * lib����ʵ�ִ�Socket���յ��ַ����������ַ�����
     * ����ֻ��Ҫʵ�ַַ���ĸ�����ɡ�
    */
    api_server_start(argc, argv);
	// �˴�����ִ�е���ע�ⲻҪ�ڴ˴���Ӵ���
}

/*****************************************************************************
 �� �� ��  : opInit
 ��������  : ������Ҫʵ�ֵĽӿ�
             ��ɡ�ϵͳ��ʼ��������
             ����ʵ����i
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2010��1��21��
    ��    ��   :
    �޸�����   : �����ɺ���

*****************************************************************************/
void opInit(void)
{
	
	/*ɾ����ʷ��Ϣ����*/
	gQueryResultHisttoryList = RemoveList(gQueryResultHisttoryList);

	/*�����µ���ʷ����*/
	gQueryResultHisttoryList = CreatList();

	/*��ʼ��ϵͳ��Ϣ*/
	gSystemInitInfo.SystemInitFalg = 1;
	gSystemInitInfo.RemainScore = MAX_SCORE_VALUE;
	gSystemInitInfo.HistoryVioNo = 0;
	gSystemInitInfo.ViolatNo  = 0;
	gSystemInitInfo.SystemTime = 0;
	gSystemInitInfo.RemainFee = 0;
	
	api_print_result(S001); // ��ʼ���ɹ�

}

/*****************************************************************************
 �� �� ��  : opRecord
 ��������  : ������Ҫʵ�ֵĽӿ�
             ��ɡ�¼��Υ���¼������
             ����ʵ����r 0-1
 �������  : int Peccancy  :    Υ������
             int Days      :    ʱ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :
*****************************************************************************/
void opRecord(int PeccancyType, int Days)
{
	
	QueryResult NewQueryResult = {0};
	/*�����Ϸ��Լ��*/
	if (1 != gSystemInitInfo.SystemInitFalg)
	{
		api_print_result(E001);		//ϵͳδ��ʼ��
		return;
	}
	if (1 > Days || MAX_DAYS_VALUE < Days)
	{
		api_print_result(E002);		//����ʱ�䳬��ϵͳ����Χ
		return;
	}
	if (Days < gSystemInitInfo.SystemTime)
	{
		api_print_result(E008);		//ʱ�����С���ϴβ���ʱ��
		return;
	}
	if (0 > PeccancyType || 4 < PeccancyType)
	{
		api_print_result(E003);		//����Υ�����ͳ���ϵͳ����Χ	
		return;
	}
	/*��¼�Ƿ�����*/
	if (MAX_PECCANCY_RECORD_NUM < (gSystemInitInfo.ViolatNo + 1))
	{
		api_print_result(E009);			//Υ���¼����
		return;
	}
	/*�����Ƿ��ѿ���*/
	if (0 >= gSystemInitInfo.RemainScore)
	{
		api_print_result(E004);			//�����ѵ���������ʧ��
		return;
	}
	
	GetFineAndSCore(PeccancyType, &NewQueryResult.Fee, &NewQueryResult.Score);
	gSystemInitInfo.SystemTime = Days;
	gSystemInitInfo.HistoryVioNo ++;
	gSystemInitInfo.ViolatNo ++;
	gSystemInitInfo.RemainScore = NewQueryResult.Score;
	gSystemInitInfo.RemainFee += NewQueryResult.Fee;

	NewQueryResult.Index = gSystemInitInfo.HistoryVioNo;
	NewQueryResult.stCarSysInfo.Time = Days;
	NewQueryResult.stCarSysInfo.Reason = PeccancyType;
	NewQueryResult.PayFlag  = STAT_NO_PAY;
	//������ʷ����
	gQueryResultHisttoryList = AddNodeInList(gQueryResultHisttoryList, &NewQueryResult);
	////¼��Υ���¼�ɹ�
	api_print_result(S002);
	return;
}

/*****************************************************************************
 �� �� ��  : GetFineSCore
 ��������  : ���㷣��Ϳ۷�
 �������  : int Peccancy  : Υ������
             int *oneFine  : Ӧ�۽��
			 int *oneScore : ʣ�����
 �������  : int *oneFine
			 int *oneScore
 �� �� ֵ  : ��
 ******************************************************************************/
void GetFineAndSCore(int PeccancyType, int *oneFine, int *oneScore)
{
	
	if (PECCANCY_TYPE_0 == PeccancyType)
	{
		*oneFine = 100;
		*oneScore = gSystemInitInfo.RemainScore - 1;
		return;
	}
	if (PECCANCY_TYPE_1 == PeccancyType)
	{
		*oneFine = 200;
		*oneScore = gSystemInitInfo.RemainScore - 2;
		return;
	}
	if (PECCANCY_TYPE_2 == PeccancyType)
	{
		*oneFine = 300;
		*oneScore = gSystemInitInfo.RemainScore - 3;
		return;
	}
	if (PECCANCY_TYPE_3 == PeccancyType)
	{
		*oneFine = 400;
		*oneScore = gSystemInitInfo.RemainScore - 4;
		return;
	}
	if (PECCANCY_TYPE_4 == PeccancyType)
	{
		*oneFine = 500;
		*oneScore = gSystemInitInfo.RemainScore - 5;
	}
	return;
}

/*****************************************************************************
 �� �� ��  : opQuery
 ��������  : ������Ҫʵ�ֵĽӿ�
             ��ɲ�ѯΥ���¼����
             ����ʵ����q
 �������  : ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :
*****************************************************************************/
void opQuery(void)
{
	
	QueryResultNode *pNode = NULL;
	/*�Ϸ��Լ��*/
	if (1 != gSystemInitInfo.SystemInitFalg)
	{
		api_print_result(E001);		//ϵͳδ��ʼ��
		return;
	}
	if (0 >= gSystemInitInfo.RemainScore)
	{
		api_print_result(E004);			//�����ѵ���������ʧ��
		return;
	}
	
	pNode = gQueryResultHisttoryList->pNext;
	if (NULL == pNode)
	{
		api_print_result(E010);		//��Υ���¼
		return;
	}
	while(NULL != pNode)
	{
		api_print_query_info(&(pNode->data));
		pNode = pNode->pNext;
	}

	return;
}

/*****************************************************************************
 �� �� ��  : opPay
 ��������  : ������Ҫʵ�ֵĽӿ�
             ��ɽ���Υ�淣�����
             ����ʵ����p 100
 �������  : int  Money       :    ���ɽ��
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  :
 ��������  :
*****************************************************************************/
void opPay(int Money)
{
	QueryResultNode *CopyQueryResultNodeList = NULL;
	QueryResultNode *TempQueryResultNode = NULL;
	QueryResultNode *ChangeQueryNode = NULL;
	QueryResult TempQuery = {0};
	PayResult   StrPayResult = {0};
	int Datasize = 0;
	int DeductionNO = 0;
	int TempMoney = Money;

	/*�Ϸ��Լ��*/
	if (1 != gSystemInitInfo.SystemInitFalg)
	{
		api_print_result(E001);		//ϵͳδ��ʼ��
		return;
	}
	if (0 >= gSystemInitInfo.RemainScore)
	{
		api_print_result(E004);		//�����ѵ���������ʧ��
		return;
	}
	if (1 > Money || MAX_MONEY_VALUE < Money)
	{
		api_print_result(E005);		//������ɽ���ϵͳ����Χ
		return;
	}
	if (0 == gSystemInitInfo.ViolatNo)
	{
		api_print_result(E007);		//��Υ���¼������Ҫ���ɷ���
		return;
	}
	/*���н���Υ��*/

	CopyQueryResultNodeList = CreatList();
	CopyList(CopyQueryResultNodeList, gQueryResultHisttoryList);
	CopyQueryResultNodeList = GetPayCostList(CopyQueryResultNodeList);
	TempQueryResultNode = CopyQueryResultNodeList->pNext;
	while(NULL != TempQueryResultNode)
	{
		TempQuery = TempQueryResultNode->data;
		if (TempQuery.Fee <= TempMoney)
		{
			//��ɿ۷ѹ���
			DeductionNO = TempQuery.Index;
			TempMoney = TempMoney - TempQuery.Fee; 
			ChangeQueryNode = FindNodeInList(gQueryResultHisttoryList, DeductionNO);
			ChangeQueryNode->data.PayFlag = STAT_HAVE_PAY;   //���Ϊ�Ѿ��۷�
			gSystemInitInfo.ViolatNo--;
			gSystemInitInfo.RemainFee = gSystemInitInfo.RemainFee - TempQuery.Fee; 
		}
		TempQueryResultNode = TempQueryResultNode->pNext;
	}
	if (TempMoney == Money)
	{
		api_print_result(E006);		//E006: ���ɶ�����Ч
		free(CopyQueryResultNodeList);
		return;
	}
	
	StrPayResult.Fee = gSystemInitInfo.RemainFee;
	StrPayResult.PeccancyNo = gSystemInitInfo.ViolatNo;
	StrPayResult.ReturnMoney = TempMoney;
	StrPayResult.Score = gSystemInitInfo.RemainScore;
	api_print_pay_info(&StrPayResult);
	free(CopyQueryResultNodeList);
	return;
}

/*****************************************************************************
 �� �� ��  : GetPayCostList
 ��������  : ��Υ����ʷ��¼�Ŀ���������������
 �������  : QueryResultNode* pHead  ��ʷ����
 �������  : ��
 �� �� ֵ  : ����õ�����
 ���ú���  :
 ��������  :
*****************************************************************************/
QueryResultNode * GetPayCostList(QueryResultNode* pHead)
{
	QueryResultNode *pNode = NULL;
	QueryResultNode *preNode = NULL;
	QueryResult TempQuery = {0};
	int loop = 0;
	int Num = 0;
	int flag = 0;

	if (NULL == pHead)
	{
		api_print_result(E999);
		return NULL;
	}
	preNode = pHead;
	pNode = preNode->pNext;
	while(NULL != pNode)		//ɾ���Ѿ����ɷ���Ľڵ�
	{
		TempQuery = pNode->data;
		if (STAT_HAVE_PAY == TempQuery.PayFlag)
		{
			if (pNode == preNode)     //���Ҫɾ���Ľڵ��ǵ�һ���ڵ�  
			{  
				pHead = pHead->pNext;
			}  
			else
			{  
				preNode->pNext = pNode->pNext;
			}  
			
			free (pNode);      //�ͷŵ�ǰ�ڵ�  
			pNode = NULL;

		}
		preNode = preNode->pNext; 
		pNode = preNode->pNext;
	}
	
	for (loop = 1; loop <= gSystemInitInfo.SystemTime; loop++)  //���ı�ʱ�����������°��ս���С��������
	{	
		Num = 0;
		pNode = pHead->pNext;
		flag = 0;
		while((NULL != pNode) &&(loop == pNode->data.stCarSysInfo.Time))
		{
			Num++;
			if (1 == Num)
			{
				preNode = pNode;
			}
			pNode = pNode->pNext;	
		}
		if (1 < Num)
		{
			Sortfee(preNode, Num);
		}	
	}
	return pHead;
}


/*��������*/
void Sortfee(QueryResultNode * pHead, int astNum)
{
	int i = 0;
	int j = 0;

	QueryResultNode *pstItem1 = NULL;
	QueryResultNode *pstItem2 = NULL;
	QueryResultNode *pstTemp = NULL;

	pstTemp = CreatList();
	
	while(NULL != pHead)
	{
		pstItem1 = pHead;
		i++;
		for(j = 0; j < astNum-i; j++)
		{
			pstItem2 = pstItem1->pNext;
			if (pstItem1->data.Fee < pstItem2->data.Fee)
			{
				pstTemp->data = pstItem1->data;
				pstItem1->data = pstItem2->data;
				pstItem2->data = pstTemp->data;	
			}
			pstItem2 = pstItem2->pNext;
		}
		pHead = pHead->pNext;
	}
	return;
}