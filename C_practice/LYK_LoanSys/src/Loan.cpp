#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "api.h"
#include "Loan.h"
#include "ListOpr.h"

/*************************************************************************
ȫ�ֱ���
**************************************************************************/
LoanSystemInfo gLoanSystemInfo = {0,0,0};
LoanHistoryInfoNode *gpstLoanHistoryInfo = NULL;


void main(int argc, char* argv[])
{
    CmdReset(); 
    /* 
    ����Socket��������5555�˿�(sapi_server_start������lib����ʵ��)��
    lib����ʵ�ִ�Socket���յ��ַ����������ַ�����
    */
    api_server_start(argc, argv);
    // �˴�����ִ�е���ע�ⲻҪ�ڴ˴���Ӵ���
}

/* -----------------------
�������Զ���ʵ�ֵĺ���
------------------------ */

/*****************************************************************************
 �� �� ��  : CmdReset
 ��������  : ��������ʵ�֣�ʵ��ϵͳ��ʼ��
 ��    ��  : ������
 ʱ    ��  : 2016.5.15
*****************************************************************************/
void CmdReset()
{
	/*���ԭ��ϵͳ����*/
	gLoanSystemInfo.iSystemTime = 0;
	gLoanSystemInfo.liDoneLoanMoney =0;
	gLoanSystemInfo.iDoneLoanNum =0;
    /*���ԭ�б����ʷ����*/
	gpstLoanHistoryInfo = RemoveList(gpstLoanHistoryInfo);

	/*��������*/
	gpstLoanHistoryInfo = CreatList();

	/*��ʼ��ϵͳʱ��*/
	gLoanSystemInfo.iSystemTime = 1;
	api_loansys_ret(OP_E_INIT_SUCCESS); //E001:��ʼ���ɹ�
	return;
}

/*****************************************************************************
 �� �� ��  : ReqeustLoan
 ��������  : ��������ʵ�֣�ʵ�ִ�����������
 �������  : loanIdx:������
             salary:������������
             principal:�����ܶ�
			 reqTime:����ʱ��
*****************************************************************************/
void ReqeustLoan(int loanIdx, float salary, int principal, int years, int reqTime)
{
    
	int statustime = 1;
	LoanHistoryInfoItem strNewLoanItem = {0};
	LoanHistoryInfoItem * pstrNewItem = &strNewLoanItem;
	LoanHistoryInfoNode *CheckingLoanInfo = NULL;

	/*������������ĺϷ��Լ��*/
	if (0 == ReqeustLoanCheak(loanIdx, salary, principal, years, reqTime))
	{
		return;	   //��������ʧ��
	}
	
	if(reqTime > gLoanSystemInfo.iSystemTime)	//����ϵͳʱ��
	{
		for (statustime = gLoanSystemInfo.iSystemTime+1; statustime <= reqTime; statustime++)
		{
			//���θ���״̬
			UpdateHistoryList(gpstLoanHistoryInfo, statustime);
		}
		gLoanSystemInfo.iSystemTime = reqTime;
	}
	
	//����������ı�� ����������


	CheckingLoanInfo = FindNodeInList(gpstLoanHistoryInfo, loanIdx);
	if (NULL != CheckingLoanInfo)
	{
		return;
	}
	else
	{
		if (RE_LOAN == CheckingLoanInfo->data.status)
		{
			//ɾ���ڵ㣻
		}
	}
	
	strNewLoanItem.applytime = reqTime;
	strNewLoanItem.income = salary;
	strNewLoanItem.loanIdx  = loanIdx;
	strNewLoanItem.monthAll = years*12;
	strNewLoanItem.principal = principal;
	strNewLoanItem.status = WAIT_CHECK;		//�����״̬
	strNewLoanItem.againtime = 0;
	strNewLoanItem.grandtime = 0;
	strNewLoanItem.waitgrandtime = 0;
	gpstLoanHistoryInfo = AddNodeInList(gpstLoanHistoryInfo, pstrNewItem);  //�����µı�ŵ�����β
	
	api_loansys_ret(OP_E_SUCCESS); //E009:��������ɹ�

	return;
}

/*****************************************************************************
 �� �� ��  : ReqeustLoanCheak
 ��������  : ��������ĺϷ��Լ��
 �������  : loanIdx:������
             salary:������������
             principal:�����ܶ�
			 reqTime:����ʱ��
*****************************************************************************/
int ReqeustLoanCheak(int loanIdx, float salary, int principal, int years, int reqTime)
{
	LoanStatus Applustatus = ON_THING;
	if(LOANID_MIN > loanIdx || LOANID_MAX < loanIdx)
	{
		api_loansys_ret(OP_E_IDX); //E002:�������������
		return 0;
	}
	if(INCOME_LOW_MIN > salary || INCOME_HIGH_MAX < salary)
	{
		api_loansys_ret(OP_E_SALARY); //E003:�������������������
		return 0;
	}
	if(AMOUNT_MIN > principal || AMOUNT_HIGH_MAX < principal)
	{
		api_loansys_ret(OP_E_PRINCIPAL); //E004:������������  
		return 0;
	}
	if(INSTALLMENTS_MIN > years || INSTALLMENTS_HIGH_MAX < years)
	{
		api_loansys_ret(OP_E_YEAR); //E005:���������������
		return 0;
	}
	if (APPLY_TIME_MIN  > reqTime || APPLY_TIME_MAX < reqTime)
	{
		api_loansys_ret(OP_E_APPLY_TIME); //E006:����ʱ���������
		return 0;
	}
	if (reqTime < gLoanSystemInfo.iSystemTime)
	{
		api_loansys_ret(OP_E_APPLYTIME_SUB); //E007:����ʱ�����ݼ�
		return 0;
	}
	/*��Եڣ�7��������,����Ƿ��ڡ��Ǵ��������롯״̬*/
	Applustatus = CheckLoanStatus(gpstLoanHistoryInfo, loanIdx);
	if (RE_LOAN != Applustatus && ON_THING != Applustatus)
	{
		api_loansys_ret(OP_E_MODIFY); //E008:���������,�������޸ĳ�ʼ��Ϣ
		return 0;
	}
	
	return 1;
}

/*�������ŵ�״̬*/
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
 �� �� ��  : ReqeustLoanCheak
 ��������  : ���������Լ����
 �������  : LoanHistoryInfoItem  ��������ʷ�ڵ���Ϣ
 �������  : ��
 ����ֵ    : ���ͨ�� 1
			 ��鲻ͨ�� 0
*****************************************************************************/
int onProReview(LoanHistoryInfoItem *WaitCheckNode)
{
	char personType = 0;

	if (RE_LOAN == WaitCheckNode->status) //���ڴ����������ID�������
	{
		return 0;
	}
	
	if (AMOUNT_LOW_MAX >= WaitCheckNode->income)
	{
		personType = 0; //��������
		if(AMOUNT_LOW_MAX < WaitCheckNode->principal)
		{
			//����𳬳���Χ,������ʧ��
			WaitCheckNode->reason = OVER_PRINCIPAL;
			return 0;
		}
		if(APPLY_TIME_MAX < WaitCheckNode->monthAll)
		{
			//�������޳�����Χ,������ʧ��
			WaitCheckNode->reason = OVER_YEAR;
			return 0;
		}
	}
	return 1;
}

/*****************************************************************************
 �� �� ��  : onPrORelease
 ��������  : ���Ŵ������Լ��
 �������  : LoanHistoryInfoItem  ��������ʷ�ڵ���Ϣ
 �������  : ��
 ����ֵ    : ����ͨ�� 1
			 ���Ų�ͨ�� 0
*****************************************************************************/
int onPrORelease(LoanHistoryInfoItem *WaitGrantNode)
{
	if (WAIT_CHECK == WaitGrantNode->status ) //���ڴ���������ID�������
	{
		return 0;
	}
	if (BANK_LOAN_COUNT_LIMIT < gLoanSystemInfo.iDoneLoanNum)
	{
		//���Ŵ���ϵͳ��������,���Ŵ���ʧ��
		WaitGrantNode->reason = SYS_DISABLE;
		return 0;
	}
	if (BANK_LOAN_AMOUNT_LIMIT < gLoanSystemInfo.liDoneLoanMoney + WaitGrantNode->principal)
	{
		//�����޿��ô���,���Ŵ���ʧ��
		WaitGrantNode->reason = BANK_NOMOMEY;
		return 0;
	}
	return 1;
}

/*��һ��t�ڸ���״̬*/
/*�������������������ŵ�״̬*/
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
		//�����ŵı��
		if ((WAIT_GRANT == pNode->data.status) && (1 == updateGrantFlag)) 
		{
			updateItem = &(pNode->data);
			if (0 != onPrORelease(updateItem))  //���ż��ͨ��
			{
				updateItem->grandtime = statuscurtime;
				updateItem->status  = SUCC_GRANT; // ״̬����Ϊ ���ųɹ�
			}
			else
			{
				updateItem->againtime = statuscurtime;
				updateItem->status  = RE_LOAN; // ���ż�鲻ͨ���������Ϊ ����������
			}
			updateGrantFlag = 0;
		}
		//������ı��
		if ((WAIT_CHECK == pNode->data.status) && (1 == updateCheckFlag)) 
		{
			updateItem = &(pNode->data);
			if (0 != onProReview(updateItem))
			{
				updateItem->waitgrandtime = statuscurtime;
				updateItem->status  = WAIT_GRANT; // ״̬����Ϊ ������
			}
			else
			{
				updateItem->againtime = statuscurtime;
				updateItem->status  = WAIT_CHECK; // ״̬����Ϊ ����������
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
 �� �� ��  : CmdLst
 ��������  : ��������ʵ�֣�ʵ�ִ�����Ϣ��ѯ
 �������  : loanIdx:������
             month:��ѯ���·�
             fund:�����𻹿��
			 lstTime:��ѯʱ��
*****************************************************************************/
void CmdLst(int loanIdx, int month, int fund, int lstTime)
{
    LoanHistoryInfoNode  *LoanNode = NULL;
	LoanInfo CheckedLoanInfo = {0};
 
	if (0 == ListLoanCheak(loanIdx, month, fund, lstTime))
	{
		return;  //��ѯ����Ϸ�
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
 �� �� ��  : ListLoanCheak
 ��������  : ��ѯ����ĺϷ��Լ��
 �������  : loanIdx:������
             month:��ѯ���·�
			 fund:�����𻹿��
			 lstTime:��ѯʱ��
�������   ���ô����ŵ���ʷ��Ϣ�ڵ�
����ֵ     ��0 ��ѯ����Ϸ�/1 ��ѯ����Ϸ�
*****************************************************************************/
int ListLoanCheak(int loanIdx, int month, int fund, int lstTime)
{
	LoanHistoryInfoNode *LoanInfoNode = NULL;
	if(LOANID_MIN > loanIdx || LOANID_MAX < loanIdx)
	{
		api_loansys_ret(OP_E_IDX); //E002:�������������
		return 0;
	}
	
	LoanInfoNode = FindNodeInList(gpstLoanHistoryInfo, loanIdx);
	if (NULL == LoanInfoNode)
	{
		api_loansys_ret(OP_E_NO_VALIDAPPLY);   //E010:����Ч�Ĵ�������
		return 0; 
	}
	
	if (QUERY_MONTH_MIN > month || QUERY_MONTH_MAX < month)
	{
		api_loansys_ret(OP_E_MONTH); //E011:�����·���Ч  E012:ס�������𻹿�����Ч
		return 0;
	}
	if (LOAD_FUND_MIN > fund || LOAD_FUND_MAX < fund)
	{
		api_loansys_ret(OP_E_FUND_MONEY);	//E012:ס�������𻹿�����Ч
		return 0;
	}
	if (QUERY_TIME_MIN > lstTime || QUERY_TIME_MAX < lstTime)
	{
		api_loansys_ret(OP_E_QRY_TIME);	//E013:��ѯʱ���������
		return 0;
	}
	/*��ѯʱ��С������ʱ��*/	
	if ((LoanInfoNode->data.applytime) > lstTime)
	{
		api_loansys_ret(OP_E_QRYTIME_SUB);	//E014:��ѯʱ�����ݼ�
		return 0;
	}
	return 1;
}
/*****************************************************************************
�� �� ��  : LoanRate
��������  : �������ʺͻ�����ʾ��
�������  : QueryLoadInfo������Ϣ�е�monthAll  : �ܻ�������
principal	: �����ܶ�(��Ԫ)
monthCur	: ��x����
fund		: �����𻹿��(��Ԫ)

�������  : QueryLoadInfo������Ϣ�е�rate			: ʵ��������(ǧ��֮)
monthCurMoney : ��x���·ݵĻ��Ϣ�ܽ��(Ԫ)
allMoney      : �軹�Ϣ�ܽ��(Ԫ)
ownMoney		: ����˻��Ϣ�ܽ��(Ԫ)
*****************************************************************************/
void LoanRate(LoanInfo *QueryLoadInfo, int fund)
{
	int YearRate = 0;		/* ������(ǧ��֮) */
	int Year = 0;			/* �������� */
	int MonthPrincipal = 0;	/* ��̯��ÿ���µı���(Ԫ) */
	int MonthInterest = 0;	/* ������Ϣ(Ԫ) */
	int AllInterest = 0;	/* ����Ϣ(Ԫ) */
	
	/* ---------- 1.���ݸ��ۿۻ��Ƽ���������� ----------*/
	Year = QueryLoadInfo->monthAll / 12;
	
	/* ��������[1,10],��ʼ������:2.4%����������(10,20],��ʼ������:4.8% */
	YearRate = (1 <= Year && 10 >= Year) ? 24 : 48;	/* ��λΪǧ��֮���ֵΪ24��48 */
	
	/* ����� [10,40],�������ۿ�:100%�������(40,100],�������ۿ�:50% */
    YearRate = ((10 <= QueryLoadInfo->principal) && ( 40 >= QueryLoadInfo->principal)) ? YearRate : ( YearRate / 2 );
	
	/* ������ת���������� */
    QueryLoadInfo->rate = YearRate / 12;
	
	/* ---------- 2.���ݵ�ǰ�·������㵱�»����ܽ�� ----------*/
	/* ÿ�»����=�����/�������� */
	MonthPrincipal = QueryLoadInfo->principal * 10000 / QueryLoadInfo->monthAll;
	
	/* ��x���»�����Ϣ=ÿ�»����*(��������-x+1)*ʵ�������� */
	MonthInterest = MonthPrincipal * (QueryLoadInfo->monthAll - QueryLoadInfo->monthCur + 1) * QueryLoadInfo->rate / 1000;
	
	/* ���»����ܽ��Ϊ:��̯��ÿ���µı��� + ÿ���µ���Ϣ */
	QueryLoadInfo->monthCurMoney = MonthPrincipal + MonthInterest;
	
	
	
	/* ---------- 3.���㱾Ϣ�ܽ�� ----------*/
	/* ��������Ϣ=�����*ʵ��������*(��������+1)/2 */
	AllInterest = QueryLoadInfo->principal * 10000 * (QueryLoadInfo->rate / 1000) * (QueryLoadInfo->monthAll + 1) / 2;
	
	/* ���Ϣ�ܽ��=�����+��������Ϣ */
	QueryLoadInfo->allMoney = QueryLoadInfo->principal * 10000 + AllInterest;
	
	
	/* ---------- 4.������˻��Ϣ�ܽ�� ----------*/
	/* ���˻��Ϣ�ܽ��=���Ϣ�ܽ��-ס�������𻹿���*50% */
	QueryLoadInfo->ownMoney = QueryLoadInfo->allMoney - (fund * 10000 / 2);
}

