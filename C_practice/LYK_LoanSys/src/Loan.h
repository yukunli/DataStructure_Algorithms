/******************************************************************************

                  ��Ȩ���� (C), 2009-2009, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : api.h
  �� �� ��   : ����
  ��    ��   : 
  ��������   : 2011��8��18��
  ����޸�   :
  ��������   : api.cpp ��ͷ�ļ�
******************************************************************************/

#ifndef __HOSPITAL_H__
#define __HOSPITAL_H__


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */
// �����ŵı߽�ֵ
#define LOANID_MIN  1
#define LOANID_MAX  10

// ������ı߽�ֵ
// ���������Сֵ
#define  INCOME_LOW_MIN   1000
// �͵���������ֵ
#define  INCOME_LOW_MAX   5000
// �ߵ��������Сֵ
#define  INCOME_HIGH_MIN  5001
// �ߵ���������ֵ
#define  INCOME_HIGH_MAX   10000

// �������޵ı߽�ֵ
// ��Сֵ
#define  INSTALLMENTS_MIN   1
// ��������Ⱥ���޵����ֵ
#define  INSTALLMENTS_LOW_MAX   10
// ��������Ⱥ���޵����ֵ
#define  INSTALLMENTS_HIGH_MAX   20
// ���޵������ۿ۹յ�
#define  INSTALLMENTS_RATE_DISCOUNT   120

// ������ı߽�ֵ
// ��Сֵ
#define  AMOUNT_MIN   10
// ��������Ⱥ�������ֵ
#define  AMOUNT_LOW_MAX   50
// ��������Ⱥ�������ֵ
#define  AMOUNT_HIGH_MAX   100
// �����ۿ۵Ĺյ�
#define  AMOUNT_RATE_DISCOUNT   40

// ����ʱ��ı߽�ֵ
#define  APPLY_TIME_MIN   1
#define  APPLY_TIME_MAX   10

// ��ѯ�·ݵı߽�ֵ
#define  QUERY_MONTH_MIN   1
#define  QUERY_MONTH_MAX   240
//��ѯʱ��ı߽�ֵ
#define  QUERY_TIME_MIN   1
#define  QUERY_TIME_MAX   24

// �����ʵ�����
#define  LOAN_RATE_TYPE_0_24   0.024
#define  LOAN_RATE_TYPE_0_48   0.048

// ���ʵ��ۿ�����
#define  LOAD_RATE_DISCOUNT_TYPE_0_5   0.5

// ���еĴ����޶�
#define  BANK_LOAN_AMOUNT_LIMIT   300
// �����ܹ���������ķ�������
#define  BANK_LOAN_COUNT_LIMIT   5

//ס����������
#define LOAD_FUND_MIN   0
#define LOAD_FUND_MAX   100


/* -----------------------
�������Զ���ʵ�ֵĺ���
------------------------ */

/* ϵͳ��ʼ������ */
void CmdReset();

/* ������������ */
void ReqeustLoan(int loanIdx, float salary, int principal, int years, int reqTime);

/* ��ѯ */
void CmdLst(int loanIdx, int month, int fund, int lstTime);

/*��������ĺϷ��Լ��*/
int ReqeustLoanCheak(int loanIdx, float salary, int principal, int years, int reqTime);
/*��ѯ����ĺϷ��Լ��*/
int ListLoanCheak(int loanIdx, int month, int fund, int lstTime);

/*�������ŵ�״̬*/
LoanStatus CheckLoanStatus(LoanHistoryInfoNode *pHead, int loanIdNo);

/*��һ��t�ڸ���״̬*/
/*�������������������ŵ�״̬*/
void UpdateHistoryList(LoanHistoryInfoNode *pHead, int statuscurtime);

void LoanRate(LoanInfo *QueryLoadInfo, int fund);

#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __HOSPITAL_H__ */