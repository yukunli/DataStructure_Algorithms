#include <string.h>
#include "stdio.h"
#include <malloc.h>
#include "api.h"
#include "car.h"
#include "ListOpr.h"

/**************************************************/
/*全局变量*/
/**************************************************/
QueryResultNode * gQueryResultHisttoryList = NULL;
SystemInfo gSystemInitInfo = {0};

/**************************************************/
void main(int argc, char* argv[])
{
    /* 启动Socket服务侦听5555端口(sapi_server_start函数在lib库已实现)。
     * lib库已实现从Socket接收到字符串后的命令分发处理；
     * 考生只需要实现分发后的各命令即可。
    */
    api_server_start(argc, argv);
	// 此处不会执行到，注意不要在此处添加代码
}

/*****************************************************************************
 函 数 名  : opInit
 功能描述  : 考生需要实现的接口
             完成“系统初始化”操作
             命令实例：i
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2010年1月21日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
void opInit(void)
{
	
	/*删除历史信息链表*/
	gQueryResultHisttoryList = RemoveList(gQueryResultHisttoryList);

	/*创建新的历史链表*/
	gQueryResultHisttoryList = CreatList();

	/*初始化系统信息*/
	gSystemInitInfo.SystemInitFalg = 1;
	gSystemInitInfo.RemainScore = MAX_SCORE_VALUE;
	gSystemInitInfo.HistoryVioNo = 0;
	gSystemInitInfo.ViolatNo  = 0;
	gSystemInitInfo.SystemTime = 0;
	gSystemInitInfo.RemainFee = 0;
	
	api_print_result(S001); // 初始化成功

}

/*****************************************************************************
 函 数 名  : opRecord
 功能描述  : 考生需要实现的接口
             完成“录入违规记录”操作
             命令实例：r 0-1
 输入参数  : int Peccancy  :    违规类型
             int Days      :    时间
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :
*****************************************************************************/
void opRecord(int PeccancyType, int Days)
{
	
	QueryResult NewQueryResult = {0};
	/*参数合法性检查*/
	if (1 != gSystemInitInfo.SystemInitFalg)
	{
		api_print_result(E001);		//系统未初始化
		return;
	}
	if (1 > Days || MAX_DAYS_VALUE < Days)
	{
		api_print_result(E002);		//输入时间超出系统允许范围
		return;
	}
	if (Days < gSystemInitInfo.SystemTime)
	{
		api_print_result(E008);		//时间参数小于上次操作时间
		return;
	}
	if (0 > PeccancyType || 4 < PeccancyType)
	{
		api_print_result(E003);		//输入违规类型超出系统允许范围	
		return;
	}
	/*记录是否已满*/
	if (MAX_PECCANCY_RECORD_NUM < (gSystemInitInfo.ViolatNo + 1))
	{
		api_print_result(E009);			//违规记录已满
		return;
	}
	/*积分是否已扣完*/
	if (0 >= gSystemInitInfo.RemainScore)
	{
		api_print_result(E004);			//车牌已吊销，操作失败
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
	//插入历史链表
	gQueryResultHisttoryList = AddNodeInList(gQueryResultHisttoryList, &NewQueryResult);
	////录入违规记录成功
	api_print_result(S002);
	return;
}

/*****************************************************************************
 函 数 名  : GetFineSCore
 功能描述  : 计算罚款和扣分
 输入参数  : int Peccancy  : 违规类型
             int *oneFine  : 应扣金额
			 int *oneScore : 剩余积分
 输出参数  : int *oneFine
			 int *oneScore
 返 回 值  : 无
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
 函 数 名  : opQuery
 功能描述  : 考生需要实现的接口
             完成查询违规记录操作
             命令实例：q
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :
*****************************************************************************/
void opQuery(void)
{
	
	QueryResultNode *pNode = NULL;
	/*合法性检查*/
	if (1 != gSystemInitInfo.SystemInitFalg)
	{
		api_print_result(E001);		//系统未初始化
		return;
	}
	if (0 >= gSystemInitInfo.RemainScore)
	{
		api_print_result(E004);			//车牌已吊销，操作失败
		return;
	}
	
	pNode = gQueryResultHisttoryList->pNext;
	if (NULL == pNode)
	{
		api_print_result(E010);		//无违规记录
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
 函 数 名  : opPay
 功能描述  : 考生需要实现的接口
             完成缴纳违规罚款操作
             命令实例：p 100
 输入参数  : int  Money       :    缴纳金额
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :
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

	/*合法性检查*/
	if (1 != gSystemInitInfo.SystemInitFalg)
	{
		api_print_result(E001);		//系统未初始化
		return;
	}
	if (0 >= gSystemInitInfo.RemainScore)
	{
		api_print_result(E004);		//车牌已吊销，操作失败
		return;
	}
	if (1 > Money || MAX_MONEY_VALUE < Money)
	{
		api_print_result(E005);		//输入缴纳金额超出系统允许范围
		return;
	}
	if (0 == gSystemInitInfo.ViolatNo)
	{
		api_print_result(E007);		//无违规记录，不需要缴纳罚款
		return;
	}
	/*进行缴纳违规*/

	CopyQueryResultNodeList = CreatList();
	CopyList(CopyQueryResultNodeList, gQueryResultHisttoryList);
	CopyQueryResultNodeList = GetPayCostList(CopyQueryResultNodeList);
	TempQueryResultNode = CopyQueryResultNodeList->pNext;
	while(NULL != TempQueryResultNode)
	{
		TempQuery = TempQueryResultNode->data;
		if (TempQuery.Fee <= TempMoney)
		{
			//完成扣费过程
			DeductionNO = TempQuery.Index;
			TempMoney = TempMoney - TempQuery.Fee; 
			ChangeQueryNode = FindNodeInList(gQueryResultHisttoryList, DeductionNO);
			ChangeQueryNode->data.PayFlag = STAT_HAVE_PAY;   //标记为已经扣费
			gSystemInitInfo.ViolatNo--;
			gSystemInitInfo.RemainFee = gSystemInitInfo.RemainFee - TempQuery.Fee; 
		}
		TempQueryResultNode = TempQueryResultNode->pNext;
	}
	if (TempMoney == Money)
	{
		api_print_result(E006);		//E006: 缴纳动作无效
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
 函 数 名  : GetPayCostList
 功能描述  : 对违规历史记录的拷贝进行排序整理
 输入参数  : QueryResultNode* pHead  历史链表
 输出参数  : 无
 返 回 值  : 整理好的链表
 调用函数  :
 被调函数  :
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
	while(NULL != pNode)		//删掉已经缴纳罚款的节点
	{
		TempQuery = pNode->data;
		if (STAT_HAVE_PAY == TempQuery.PayFlag)
		{
			if (pNode == preNode)     //如果要删除的节点是第一个节点  
			{  
				pHead = pHead->pNext;
			}  
			else
			{  
				preNode->pNext = pNode->pNext;
			}  
			
			free (pNode);      //释放当前节点  
			pNode = NULL;

		}
		preNode = preNode->pNext; 
		pNode = preNode->pNext;
	}
	
	for (loop = 1; loop <= gSystemInitInfo.SystemTime; loop++)  //不改变时间排序的情况下按照金额大小进行排序
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


/*降序排序*/
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