/******************************************************************************

                  版权所有 (C), 2009-2009, 华为技术有限公司

 ******************************************************************************
  文 件 名   : api.h
  版 本 号   : 初稿
  作    者   :
  生成日期   : 2009年9月9日
  最近修改   :
  功能描述   : api.cpp 的头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2009年9月9日
    作    者   :
    修改内容   : 创建文件

******************************************************************************/

#ifndef __API_H__
#define __API_H__


#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */


/** 本文件中的枚举、结构、API接口等供考生使用，但不允许修改 **/
//返回操作信息
enum OP_RST_INFO
{
    S001,        //初始化成功1
    S002,        //录入违规记录成功 1
    E001,        //系统未初始化1
    E002,        //输入时间超出系统允许范围1
    E003,        //输入违规类型超出系统允许范围1
    E004,        //车牌已吊销，操作失败1
    E005,        //输入缴纳金额超出系统允许范围1
    E006,        //缴纳动作无效1
    E007,        //无违规记录，不需要缴纳罚款1
    E008,        //时间参数小于上次操作时间 1
    E009,        //违规记录已满1
    E010,        //无违规记录1
    E999         //系统内部错误1
};

//车辆违规信息
typedef struct CAR_SYS_INFO
{
    int     Time;                                      //违规时间
    int     Reason;                                    //违规原因
}CarSysInfo;

//历史违规信息
typedef struct QUERY_RESULT
{
    int             Index;                             //历史违规序号
    CarSysInfo      stCarSysInfo;                      //车辆违规信息
	int				PayFlag;
    int             Fee;                               //总罚款金额
    int             Score;                             //剩余积分
}QueryResult;

//缴纳罚款输出信息
typedef struct PAY_RESULT
{
    int     ReturnMoney;                               //找零
    int     Score;                                     //剩余积分
    int     PeccancyNo;                                //剩余违规记录数
    int     Fee;                                       //剩余罚款金额
}PayResult;

//历史违规信息链表
typedef struct QueryResultNode
{
	QueryResult data;
	QueryResultNode *pNext;
}QueryResultNode;

//系统初始信息
typedef struct SystemInfo
{
	int SystemInitFalg;		//系统初始化标志
	int	RemainScore;		//车辆剩余的积分
	int	ViolatNo;			//违规记录数
	int HistoryVioNo;		//历史违规数
	int SystemTime;			//系统的时间
	int RemainFee;			//剩余罚款金额
}SystemInfo;
/*****************************************************************************
 函 数 名  : api_print_result
 功能描述  : 打印返回码信息到socket工具窗口
 输入参数  : enum OP_RST_INFO enResultCode
 输出参数  : 无
 返 回 值  : extern void
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2010年1月21日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
void api_print_result(enum OP_RST_INFO enResultCode);


/*****************************************************************************
 函 数 名  : api_print_query_info
 功能描述  : 打印查询信息到socket工具窗口
 输入参数  : QueryResult *pResult
 输出参数  : 无
 返 回 值  : extern void
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2010年1月21日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
void api_print_query_info(QueryResult *pResult);


/*****************************************************************************
 函 数 名  : api_print_pay_info
 功能描述  : 打印缴纳信息到socket工具窗口
 输入参数  : PayResult  *pResult
 输出参数  : 无
 返 回 值  : extern void
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2010年1月21日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
void api_print_pay_info(PayResult  *pResult);


/*****************************************************************************
 函 数 名  : api_server_start
 功能描述  : 启动socket服务端，以阻塞的方式接受用户输入
 输入参数  : 无
 输出参数  : 无
 返 回 值  : extern void
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2010年1月21日
    作    者   :
    修改内容   : 新生成函数

*****************************************************************************/
void api_server_start(int argc, char* argv[]);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __API_H__ */
