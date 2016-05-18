#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "api.h"
#include "Loan.h"
#include "ListOpr.h"

/*************************************************************************
全局变量
**************************************************************************/
LoanSystemInfo gLoanSystemInfo = {0,0,0};
LoanHistoryInfoNode *gpstLoanHistoryInfo = NULL;


void main(int argc, char* argv[])
{
    CmdReset(); 
    /* 
    启动Socket服务侦听5555端口(sapi_server_start函数在lib库已实现)。
    lib库已实现从Socket接收到字符串后的命令分发处理；
    */
    api_server_start(argc, argv);
    // 此处不会执行到，注意不要在此处添加代码
}

/* -----------------------
供考生自定义实现的函数
------------------------ */

/*****************************************************************************
 函 数 名  : CmdReset
 功能描述  : 考生自行实现，实现系统初始化
 作    者  : 李宇坤
 时    间  : 2016.5.15
*****************************************************************************/
void CmdReset()
{
	/*清空原有系统数据*/
	gLoanSystemInfo.iSystemTime = 0;
	gLoanSystemInfo.liDoneLoanMoney =0;
	gLoanSystemInfo.iDoneLoanNum =0;
    /*清空原有编号历史数据*/
	gpstLoanHistoryInfo = RemoveList(gpstLoanHistoryInfo);

	/*创建链表*/
	gpstLoanHistoryInfo = CreatList();

	/*初始化系统时间*/
	gLoanSystemInfo.iSystemTime = 1;
	api_loansys_ret(OP_E_INIT_SUCCESS); //E001:初始化成功
	return;
}

/*****************************************************************************
 函 数 名  : ReqeustLoan
 功能描述  : 考生自行实现，实现贷款申请命令
 输入参数  : loanIdx:贷款编号
             salary:贷款人月收入
             principal:本金总额
			 reqTime:申请时间
*****************************************************************************/
void ReqeustLoan(int loanIdx, float salary, int principal, int years, int reqTime)
{
    
	int statustime = 1;
	LoanHistoryInfoItem strNewLoanItem = {0};
	LoanHistoryInfoItem * pstrNewItem = &strNewLoanItem;
	LoanHistoryInfoNode *CheckingLoanInfo = NULL;

	/*申请命令操作的合法性检查*/
	if (0 == ReqeustLoanCheak(loanIdx, salary, principal, years, reqTime))
	{
		return;	   //贷款申请失败
	}
	
	if(reqTime > gLoanSystemInfo.iSystemTime)	//更新系统时间
	{
		for (statustime = gLoanSystemInfo.iSystemTime+1; statustime <= reqTime; statustime++)
		{
			//依次更新状态
			UpdateHistoryList(gpstLoanHistoryInfo, statustime);
		}
		gLoanSystemInfo.iSystemTime = reqTime;
	}
	
	//待重新申请的编号 进行重申请


	CheckingLoanInfo = FindNodeInList(gpstLoanHistoryInfo, loanIdx);
	if (NULL != CheckingLoanInfo)
	{
		return;
	}
	else
	{
		if (RE_LOAN == CheckingLoanInfo->data.status)
		{
			//删除节点；
		}
	}
	
	strNewLoanItem.applytime = reqTime;
	strNewLoanItem.income = salary;
	strNewLoanItem.loanIdx  = loanIdx;
	strNewLoanItem.monthAll = years*12;
	strNewLoanItem.principal = principal;
	strNewLoanItem.status = WAIT_CHECK;		//待审查状态
	strNewLoanItem.againtime = 0;
	strNewLoanItem.grandtime = 0;
	strNewLoanItem.waitgrandtime = 0;
	gpstLoanHistoryInfo = AddNodeInList(gpstLoanHistoryInfo, pstrNewItem);  //加入新的编号到链表尾
	
	api_loansys_ret(OP_E_SUCCESS); //E009:贷款申请成功

	return;
}

/*****************************************************************************
 函 数 名  : ReqeustLoanCheak
 功能描述  : 申请命令的合法性检查
 输入参数  : loanIdx:贷款编号
             salary:贷款人月收入
             principal:本金总额
			 reqTime:申请时间
*****************************************************************************/
int ReqeustLoanCheak(int loanIdx, float salary, int principal, int years, int reqTime)
{
	LoanStatus Applustatus = ON_THING;
	if(LOANID_MIN > loanIdx || LOANID_MAX < loanIdx)
	{
		api_loansys_ret(OP_E_IDX); //E002:贷款编号输入错误
		return 0;
	}
	if(INCOME_LOW_MIN > salary || INCOME_HIGH_MAX < salary)
	{
		api_loansys_ret(OP_E_SALARY); //E003:贷款人月收入输入错误
		return 0;
	}
	if(AMOUNT_MIN > principal || AMOUNT_HIGH_MAX < principal)
	{
		api_loansys_ret(OP_E_PRINCIPAL); //E004:贷款本金输入错误  
		return 0;
	}
	if(INSTALLMENTS_MIN > years || INSTALLMENTS_HIGH_MAX < years)
	{
		api_loansys_ret(OP_E_YEAR); //E005:贷款年限输入错误
		return 0;
	}
	if (APPLY_TIME_MIN  > reqTime || APPLY_TIME_MAX < reqTime)
	{
		api_loansys_ret(OP_E_APPLY_TIME); //E006:申请时间输入错误
		return 0;
	}
	if (reqTime < gLoanSystemInfo.iSystemTime)
	{
		api_loansys_ret(OP_E_APPLYTIME_SUB); //E007:申请时间错误递减
		return 0;
	}
	/*针对第（7）个规则,检查是否在‘非待重新申请’状态*/
	Applustatus = CheckLoanStatus(gpstLoanHistoryInfo, loanIdx);
	if (RE_LOAN != Applustatus && ON_THING != Applustatus)
	{
		api_loansys_ret(OP_E_MODIFY); //E008:贷款处理正常,不允许修改初始信息
		return 0;
	}
	
	return 1;
}

/*检查贷款编号的状态*/
LoanStatus CheckLoanStatus(LoanHistoryInfoNode *pHead, int loanIdNo)
{
	LoanHistoryInfoNode *CheckingLoanInfo = NULL;
	if (NULL == pHead)
	{	
		exit(0);
	} 
	CheckingLoanInfo = FindNodeInList(pHead, loanIdNo);
	if (NULL != CheckingLoanInfo)
	{
		return CheckingLoanInfo->data.status;
	}
	else
	{
		return ON_THING;
	}
	return ON_THING;
}

/*****************************************************************************
 函 数 名  : ReqeustLoanCheak
 功能描述  : 审查贷款审查约束查
 输入参数  : LoanHistoryInfoItem  贷款编号历史节点信息
 输出参数  : 无
 返回值    : 审查通过 1
			 审查不通过 0
*****************************************************************************/
int onProReview(LoanHistoryInfoItem *WaitCheckNode)
{
	char personType = 0;

	if (RE_LOAN == WaitCheckNode->status) //对于待重新申请的ID不做审查
	{
		return 0;
	}
	
	if (AMOUNT_LOW_MAX >= WaitCheckNode->income)
	{
		personType = 0; //低收入人
		if(AMOUNT_LOW_MAX < WaitCheckNode->principal)
		{
			//贷款本金超出范围,审查贷款失败
			WaitCheckNode->reason = OVER_PRINCIPAL;
			return 0;
		}
		if(APPLY_TIME_MAX < WaitCheckNode->monthAll)
		{
			//贷款年限超出范围,审查贷款失败
			WaitCheckNode->reason = OVER_YEAR;
			return 0;
		}
	}
	return 1;
}

/*****************************************************************************
 函 数 名  : onPrORelease
 功能描述  : 发放贷款审查约束
 输入参数  : LoanHistoryInfoItem  贷款编号历史节点信息
 输出参数  : 无
 返回值    : 发放通过 1
			 发放不通过 0
*****************************************************************************/
int onPrORelease(LoanHistoryInfoItem *WaitGrantNode)
{
	if (WAIT_CHECK == WaitGrantNode->status ) //对于待重新申请ID不做审查
	{
		return 0;
	}
	if (BANK_LOAN_COUNT_LIMIT < gLoanSystemInfo.iDoneLoanNum)
	{
		//发放贷款系统能力不足,发放贷款失败
		WaitGrantNode->reason = SYS_DISABLE;
		return 0;
	}
	if (BANK_LOAN_AMOUNT_LIMIT < gLoanSystemInfo.liDoneLoanMoney + WaitGrantNode->principal)
	{
		//银行无可用贷款,发放贷款失败
		WaitGrantNode->reason = BANK_NOMOMEY;
		return 0;
	}
	return 1;
}

/*在一个t内更新状态*/
/*遍历整个链表，更新相信的状态*/
void UpdateHistoryList(LoanHistoryInfoNode *pHead,  int statuscurtime)
{
	LoanHistoryInfoItem *updateItem = NULL;
	char updateGrantFlag = 1;
	char updateCheckFlag = 1;
	
	if((NULL == pHead) || (NULL == statuscurtime))
	{
		return;
	}
	LoanHistoryInfoNode *pNode = pHead;

	while(NULL != pNode)
	{
		//待发放的编号
		if ((WAIT_GRANT == pNode->data.status) && (1 == updateGrantFlag)) 
		{
			updateItem = &(pNode->data);
			if (0 != onPrORelease(updateItem))  //发放检查通过
			{
				updateItem->grandtime = statuscurtime;
				updateItem->status  = SUCC_GRANT; // 状态更新为 发放成功
			}
			else
			{
				updateItem->againtime = statuscurtime;
				updateItem->status  = RE_LOAN; // 发放检查不通过，则更新为 待重新申请
			}
			updateGrantFlag = 0;
		}
		//待申请的编号
		if ((WAIT_CHECK == pNode->data.status) && (1 == updateCheckFlag)) 
		{
			updateItem = &(pNode->data);
			if (0 != onProReview(updateItem))
			{
				updateItem->waitgrandtime = statuscurtime;
				updateItem->status  = WAIT_GRANT; // 状态更新为 待发放
			}
			else
			{
				updateItem->againtime = statuscurtime;
				updateItem->status  = WAIT_CHECK; // 状态更新为 待重新申请
			}
			updateCheckFlag = 0;
			pNode = pNode->pNext;
			break;
		}
		pNode = pNode->pNext;
	}
	return;
}

/*****************************************************************************
 函 数 名  : CmdLst
 功能描述  : 考生自行实现，实现贷款信息查询
 输入参数  : loanIdx:贷款编号
             month:查询的月份
             fund:公积金还款额
			 lstTime:查询时间
*****************************************************************************/
void CmdLst(int loanIdx, int month, int fund, int lstTime)
{
    LoanHistoryInfoNode  *LoanNode = NULL;
	LoanInfo CheckedLoanInfo = {0};
 
	if (0 == ListLoanCheak(loanIdx, month, fund, lstTime))
	{
		return;  //查询命令不合法
	}
	
	LoanNode = FindNodeInList(gpstLoanHistoryInfo, loanIdx);
	CheckedLoanInfo.loanIdx = loanIdx;
	CheckedLoanInfo.principal = LoanNode->data.principal;
	CheckedLoanInfo.monthAll = LoanNode->data.monthAll;
	CheckedLoanInfo.reason = LoanNode->data.reason;
	CheckedLoanInfo.monthCur = month;
	if (lstTime >= gLoanSystemInfo.iSystemTime || (RE_LOAN == LoanNode->data.status))
	{
		CheckedLoanInfo.status = LoanNode->data.status;
	}
	else
	{
		if (LoanNode->data.applytime <= lstTime < LoanNode->data.waitgrandtime)
		{
			CheckedLoanInfo.status = WAIT_CHECK;
		}
		if (LoanNode->data.waitgrandtime <= lstTime < LoanNode->data.grandtime)
		{
			CheckedLoanInfo.status = WAIT_GRANT;
		}
		if ( lstTime >= LoanNode->data.grandtime)
		{
			CheckedLoanInfo.status = SUCC_GRANT;
		}
	}

	LoanRate(&CheckedLoanInfo, fund);
	api_loansys_loaninfo(CheckedLoanInfo);
	return;
}

/*****************************************************************************
 函 数 名  : ListLoanCheak
 功能描述  : 查询命令的合法性检查
 输入参数  : loanIdx:贷款编号
             month:查询的月份
			 fund:公积金还款额
			 lstTime:查询时间
输出参数   ：该贷款编号的历史信息节点
返回值     ：0 查询命令不合法/1 查询命令合法
*****************************************************************************/
int ListLoanCheak(int loanIdx, int month, int fund, int lstTime)
{
	LoanHistoryInfoNode *LoanInfoNode = NULL;
	if(LOANID_MIN > loanIdx || LOANID_MAX < loanIdx)
	{
		api_loansys_ret(OP_E_IDX); //E002:贷款编号输入错误
		return 0;
	}
	
	LoanInfoNode = FindNodeInList(gpstLoanHistoryInfo, loanIdx);
	if (NULL == LoanInfoNode)
	{
		api_loansys_ret(OP_E_NO_VALIDAPPLY);   //E010:无有效的贷款申请
		return 0; 
	}
	
	if (QUERY_MONTH_MIN > month || QUERY_MONTH_MAX < month)
	{
		api_loansys_ret(OP_E_MONTH); //E011:还款月份无效  E012:住房公积金还款金额无效
		return 0;
	}
	if (LOAD_FUND_MIN > fund || LOAD_FUND_MAX < fund)
	{
		api_loansys_ret(OP_E_FUND_MONEY);	//E012:住房公积金还款金额无效
		return 0;
	}
	if (QUERY_TIME_MIN > lstTime || QUERY_TIME_MAX < lstTime)
	{
		api_loansys_ret(OP_E_QRY_TIME);	//E013:查询时间输入错误
		return 0;
	}
	/*查询时间小于申请时间*/	
	if ((LoanInfoNode->data.applytime) > lstTime)
	{
		api_loansys_ret(OP_E_QRYTIME_SUB);	//E014:查询时间错误递减
		return 0;
	}
	return 1;
}
/*****************************************************************************
函 数 名  : LoanRate
功能描述  : 计算利率和还款金额示例
输入参数  : QueryLoadInfo贷款信息中的monthAll  : 总还款期数
principal	: 本金总额(万元)
monthCur	: 第x个月
fund		: 公积金还款额(万元)

输出参数  : QueryLoadInfo贷款信息中的rate			: 实际月利率(千分之)
monthCurMoney : 第x个月份的还款本息总金额(元)
allMoney      : 需还款本息总金额(元)
ownMoney		: 需个人还款本息总金额(元)
*****************************************************************************/
void LoanRate(LoanInfo *QueryLoadInfo, int fund)
{
	int YearRate = 0;		/* 年利率(千分之) */
	int Year = 0;			/* 贷款年限 */
	int MonthPrincipal = 0;	/* 分摊到每个月的本金(元) */
	int MonthInterest = 0;	/* 当月利息(元) */
	int AllInterest = 0;	/* 总利息(元) */
	
	/* ---------- 1.根据各折扣机制计算出月利率 ----------*/
	Year = QueryLoadInfo->monthAll / 12;
	
	/* 贷款年限[1,10],初始年利率:2.4%，贷款年限(10,20],初始年利率:4.8% */
	YearRate = (1 <= Year && 10 >= Year) ? 24 : 48;	/* 单位为千分之因此值为24、48 */
	
	/* 贷款本金 [10,40],年利率折扣:100%，贷款本金(40,100],年利率折扣:50% */
    YearRate = ((10 <= QueryLoadInfo->principal) && ( 40 >= QueryLoadInfo->principal)) ? YearRate : ( YearRate / 2 );
	
	/* 年利率转换成月利率 */
    QueryLoadInfo->rate = YearRate / 12;
	
	/* ---------- 2.根据当前月份数计算当月还款总金额 ----------*/
	/* 每月还款本金=贷款本金/还款期数 */
	MonthPrincipal = QueryLoadInfo->principal * 10000 / QueryLoadInfo->monthAll;
	
	/* 第x个月还款利息=每月还款本金*(还款期数-x+1)*实际月利率 */
	MonthInterest = MonthPrincipal * (QueryLoadInfo->monthAll - QueryLoadInfo->monthCur + 1) * QueryLoadInfo->rate / 1000;
	
	/* 当月还款总金额为:分摊到每个月的本金 + 每个月的利息 */
	QueryLoadInfo->monthCurMoney = MonthPrincipal + MonthInterest;
	
	
	
	/* ---------- 3.计算本息总金额 ----------*/
	/* 还款总利息=贷款本金*实际月利率*(还款期数+1)/2 */
	AllInterest = QueryLoadInfo->principal * 10000 * (QueryLoadInfo->rate / 1000) * (QueryLoadInfo->monthAll + 1) / 2;
	
	/* 还款本息总金额=贷款本金+还款总利息 */
	QueryLoadInfo->allMoney = QueryLoadInfo->principal * 10000 + AllInterest;
	
	
	/* ---------- 4.计算个人还款本息总金额 ----------*/
	/* 个人还款本息总金额=还款本息总金额-住房公积金还款金额*50% */
	QueryLoadInfo->ownMoney = QueryLoadInfo->allMoney - (fund * 10000 / 2);
}

