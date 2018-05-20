#ifndef _LINUX_GSIP_UTIL_H
#define _LINUX_GSIP_UTIL_H

#include <stdio.h>
#include <string.h>
#include <map>
#include <string>
#include <TC_Bocom_Pattern_Analysis.h>

#define GSIP_VERSION "version 1.3.7"

//渠道信息， CHNNEL_REQ-请求信息， CHANNEL-RES--应答信息
#define CHANNEL_REQ				1
#define CHANNEL_RES				2

//数据包类型，分为系统报文和用户报文，心跳数据是系统报文， 其他应用类报文为用户报文
#define PKG_SYS_REQ "01"
#define PKG_SYS_RES "02"
#define PKG_USR_REQ "03"
#define PKG_USR_RES "04"

//后台服务任务调度交易码
#define VOUCHERS_TRADECODE_REQ  	"1000"
#define VOUCHERS_TRADECODE_RES  	"1001"
#define MULTIS_TRADECODE_REQ  		"1002"
#define MULTIS_TRADECODE_RES  		"1003"
#define VOUCHERS_TRADECODE_UP_REQ  	"1004"
#define VOUCHERS_TRADECODE_UP_RES  	"1005"
#define MULTIS_TRADECODE_UP_REQ  	"1006"
#define MULTIS_TRADECODE_UP_RES  	"1007"

//人行数据导入导出任务调度交易码
#define PBC_TRADECODE_IM_REQ  		"1008"
#define PBC_TRADECODE_IM_RES  		"1009"
#define PBC_TRADECODE_EX_REQ  		"1010"
#define PBC_TRADECODE_EX_RES  		"1011"

//委收两种业务
#define AUTHINPUT_TRADECODE_REQ		"1012"
#define AUTHINPUT_TRADECODE_RES		"1013"
#define AUTHINPUT_TRADECODE_UP_REQ		"1014"
#define AUTHINPUT_TRADECODE_UP_RES		"1015"

//上海进账单两种业务
#define CINPUT_TRADECODE_REQ		"1016"
#define CINPUT_TRADECODE_RES		"1017"
#define CINPUT_TRADECODE_UP_REQ		"1018"
#define CINPUT_TRADECODE_UP_RES		"1019"

#define PBC_CTRL_LOAD_REQ           "2000"
#define PBC_CTRL_LOAD_RES			"2001"
//任务返回结果
#define TASK_RESULT_SUCC			"0000"
#define TASK_RESULT_FAIL			"0001"
//查无任务
#define TASK_RESULT_NONE			"0002"
//SQL 查询错误
#define TASK_SQL_ERROR				"0003"


//单票表状态信息
#define VOUCHERS_STATE_NONE			0
#define VOUCHERS_STATE_SUCC			1
#define VOUCHERS_STATE_DOING		2
//取图或处理失败
#define VOUCHERS_STATE_FAIL			3
//版面校验失败
#define VOUCHERS_STATE_MATCH_FAIL	4
//版面识别失败
#define VOUCHERS_STATE_REC_FAIL		5

//单票表发送状态信息
#define VOUCHERS_SENDSTATE_NONE			0
#define VOUCHERS_SENDSTATE_SUCC			1
#define VOUCHERS_SENDSTATE_DOING		2
//一般错误，如获取明细、图像错误
#define VOUCHERS_SENDSTATE_FAIL			3
//流程应答超时
#define VOUCHERS_SENDSTATE_TOUCH_FAIL	5
//ECM应答超时
#define VOUCHERS_SENDSTATE_ECM_FAIL	4

//信封表状态信息
#define MULTIS_STATE_NONE			0
#define MULTIS_STATE_SUCC			1
#define MULTIS_STATE_DOING			2
//取图或处理失败
#define MULTIS_STATE_FAIL			3
//版面校验失败
#define MULTIS_STATE_MATCH_FAIL		4
//版面识别失败
#define MULTIS_STATE_REC_FAIL		5

//信封表发送状态信息
#define MULTIS_SENDSTATE_NONE			0
#define MULTIS_SENDSTATE_SUCC			1
#define MULTIS_SENDSTATE_DOING			2
#define MULTIS_SENDSTATE_FAIL			3
//ECM应答超时
#define MULTIS_SENDSTATE_TOUCH_FAIL		5
//流程应答超时
#define MULTIS_SENDSTATE_ECM_FAIL		4

#define  PBC_FILE_TYPE_QS 0 
#define  PBC_FILE_TYPE_MX 1


//委收表状态信息===============================
#define AUTHINPUT_STATE_NONE			0
#define AUTHINPUT_STATE_SUCC			1
#define AUTHINPUT_STATE_DOING			2
//取图或处理失败
#define AUTHINPUT_STATE_FAIL			3
//版面校验失败
#define AUTHINPUT_STATE_MATCH_FAIL		4
//版面识别失败
#define AUTHINPUT_STATE_REC_FAIL		5

//委收表发送状态信息
#define AUTHINPUT_SENDSTATE_NONE			0
#define AUTHINPUT_SENDSTATE_SUCC			1
#define AUTHINPUT_SENDSTATE_DOING			2
#define AUTHINPUT_SENDSTATE_FAIL			3
//ECM应答超时
#define AUTHINPUT_SENDSTATE_ECM_FAIL		4
//流程应答超时
#define AUTHINPUT_SENDSTATE_TOUCH_FAIL		5
//此笔业务未完成状态
#define AUTHINPUT_SENDSTATE_BILL_NONE		6
//此笔业务处理中锁定状态
#define AUTHINPUT_SENDSTATE_BILL_DOING		7
//此笔业务处理完成状态
#define AUTHINPUT_SENDSTATE_BILL_SUCC		8
//此笔业务触发流程超时，不会被回收
#define AUTHINPUT_SENDSTATE_BILL_OUTTIME	9

//上海进帐单
#define CINPUT_STATE_NONE			0
#define CINPUT_STATE_SUCC			1
#define CINPUT_STATE_DOING			2
//取图或处理失败
#define CINPUT_STATE_FAIL			3
//版面校验失败
#define CINPUT_STATE_MATCH_FAIL		4
//版面识别失败
#define CINPUT_STATE_REC_FAIL		5

#define CINPUT_SENDSTATE_NONE		0
#define CINPUT_SENDSTATE_SUCC		1
#define CINPUT_SENDSTATE_DOING		2
//一般错误，如获取明细、图像错误
#define CINPUT_SENDSTATE_FAIL		3
//流程应答超时
#define CINPUT_SENDSTATE_TOUCH_FAIL	5
//ECM应答超时
#define CINPUT_SENDSTATE_ECM_FAIL	4

//币种
#define PBC_COTYPE_VAL  "CNY"

//服务器连接超时
#define CONN_SERVER_ERR				1

using namespace std;
typedef map<string,string> StrDict;

//是否是数字
int isnum(char c);

//数字的位数
int int_bit_num(const unsigned int tmp);

//左补字符
int left_add_str(char *in, const char *str, const unsigned int num);

//获取数据包
int get_pkg(char *pkg, const char *channel, const char *type);

//发送心跳数据
int heart_request(int fd);

//心跳应答
int heart_response(int fd);

#endif
