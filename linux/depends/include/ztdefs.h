/*
 ***************************************************************************
 *
 * Copyright (c) 2006-2008 北京天创征腾信息科技有限公司    版权所有
 *
 * 此文件为天创征腾软件产品的一部分。
 * 天创征腾软件产品的所有部分,包括但不限于软件产品中所含的任何图象、照片、
 * 动画、文字和附加程序（applets）、加密卡、随附的印刷材料、源代码文件及
 * 本软件产品的任何副本的一切所有权和知识产权，均归版权人天创征腾拥有。
 *
 * 文件：	$Id$
 * 版本：	$Revision$
 * 网址：   www.tchzt.com
 * 邮箱：   support@tchzt.com
 *
 ***************************************************************************
 */
#ifndef __ZTDEFS_H__
#define __ZTDEFS_H__
#include <string.h> 
#include <map>
#include <vector>
#include <iostream>
#include <zstructs.h>

using namespace std;

#define ZTS_SERVER		"ZTS_SERVER"
typedef vector<string>	MISS;
typedef map<string,string> StrDict;


#define SRVPORT     "SRVPORT"       		/* 服务端口*/
#define CENTERID    "CENTERID"      		/* 中心电话号*/
#define CENTERID2    "CENTERID2"      	/* 中心电话号*/
#define CSPADDR     "CSPADDR"       		/* CSP地址 */   
#define CSPPORT     "CSPPORT"       		/* CSP端口 */   
#define NASADDR     "NASADDR"       		/* NAS地址 */   
#define NASPORT     "NASPORT"       		/* NAS端口 */  
#define ATMADDR     "ATMADDR"       		/* ATMP地址*/ 
#define ATMPORT     "ATMPORT"       		/* ATMP端口*/         
#define MONADDR     "MONADDR"       		/* 交易监控机地址 */    
#define MONPORT     "MONPORT"       		/* 交易监控机端口 */      
#define PHOADDR     "PHOADDR"						/* 语音平台地址 */ 
#define PHOPORT     "PHOPORT"						/* 语音平台端口 */       
#define HSMADDR     "HSMADDR"						/* 加密机地址 */  
#define HSMPORT     "HSMPORT"						/* 加密机服务端口 */  
#define MACIDX      "MACIDX"						/*终端计算MAC 主密钥索引 */       
#define PINIDX      "PINIDX"						/*终端加密pin 主密钥索引 */  
//#define BLKIDX      "BLKIDX"        	/*银行密钥索引(传输密钥) ,加密TMK*/       		 
//#define BTMKIDX     "BTMKIDX"       	/*银行密钥索引(传输密钥) ,加密TPK*/       	
//#define BATMIDX     "BATMIDX"       	/*银行密钥索引(ATM KEY) ,密文*/    
//#define BTPKIDX     "BTPKIDX"       	/*银行密钥索引(PIN KEY) ,加密PIN用*/   
#define BMACIDX     "BTMACIDX"        	/*银行密钥索引(MAC KEY) ,计算MAC用*/  
//#define BTMKIDX2    "BTMKIDX2"       	/*银行密钥索引(传输密钥),加密TPK， LoadKey 临时存放*/       	
//#define BATMIDX2    "BATMIDX2"       	/*银行密钥索引(ATM KEY) ,密文， LoadKey 临时存放*/    
//#define BPINIDX2    "BTPKIDX2"       	/*银行密钥索引(PIN KEY) ,加密PIN用， LoadKey 临时存放*/
#define CUSTOMID    "CUSTOMID"     			/*客户编号*/
#define SAVETRACE   "SAVETRACE"      		/*是否记录上送记录*/
#define LOCALADDR   "LOCALADDR"      		/*本机ip地址*/
#define RBSNETNO		"RBSNETNO"				/*RBS网点号*/
#define NASNETNO		"NASNETNO"				/*RBS网点号*/
#define RBSTERMNO		"RBSTERMNO"				/*RBS终端号*/ 
#define SUBJECT_TRANS	"SUBJECT"					/*转账科目*/
#define SUBJECT_NAS   "NASSUBJECT"			/*NAS交易 行内入账科目*/
#define SUBJECT_FEE		"FEESUBJECT"			/*手续费科目*/	
   	
enum ZTS_ST
{
	
	
	TM_QUERY_AMT		= 110,	/* 查余额*/
	TM_TRANS_AMT    = 111,  /* 转帐*/
	TM_REMIT_AMT    = 112,  /* 汇款*/
	TM_QUERY_HIS		= 113,  /* 查询末笔交易*/ 
	BK_TRANS_AMT		= 300, 
	BK_LOAD_KEY     = 400,	/* 下载KEY */
};


enum ZTS_CARD_TYPE
{	
	CARD_PRO				= 0,	/* 省内卡*/
	CARD_BRO        = 1,  /* 省外卡*/
	CARD_OTH        = 2,  /* 他行卡*/
};

enum ZTS_CHARGE_TYPE
{	
	CARD_OUTER			= 0,	/* 外扣,扣持卡人*/
	CARD_INNER      = 1,  /* 内扣,扣商户*/
};


inline char* ErrorDecode(int errNo)
{
	switch(errNo) 
	{
		case UNKNOW_DEAL_CODE:		return "未知交易!";
		case NEXT_DEAL_CODE:			return "暂不支持的交易!";
		case DEC_MSG_ERROR: 			return "终端报文解析错!";		
		case DEC_ACCOUNT_ERROR:		return "解帐号错!";
		case CAL_MAC_ERROR:				return "计算MAC错!";
		case VER_MAC_ERROR:				return "校验MAC错!";
		case TRS_PIN_ERROR:				return "转加密错!";
		case HSM_CONNECT_ERROR:		return "加密机连接错!";		
    case HSM_SEND_ERROR:			return "加密机通讯错!";
    case HSM_RECV_ERROR:			return "加密机接收错!";
    case HSM_DSWK_ERROR:			return "离散密钥失败!";
    case HSM_MAC_ERROR:				return "计算MAC失败!";
    case HSM_DES_ERROR:				return "解密失败!";
    case HSM_CPIN_ERROR:			return "PIN转换失败!";
    case HSM_CKEY_ERROR:			return "密钥转换失败";
    case HSM_GKEY_ERROR:			return "取密钥失败";
                            	
    case DB_OPEN_ERROR:				return "数据库连接错!";	
    case DB_SQL_ERROR:				return "数据库查询错!";	
    case DB_QUERY_EMPTY:			return "查询结果为空!";	
    case DB_KEYS_EMPTY:				return "查询密钥错误!";	
                            	
    case TM_TELEID_ERROR:			return "终端电话未申请!";
    case TM_TERMID_ERROR:			return "电话号码与终端不匹配!";
    case TM_PSAMID_ERROR:			return "电话号码与PSAM卡不匹配!";
    case TM_ACCNO_ERROR:			return "帐号绑定无效!";
    case TM_STATE_ERROR:			return "终端状态不正确!";
    case TM_TRANSQ_ERROR:			return "转帐交易查询无记录或超时!";
    case TM_INOUT_ERROR:			return "不允许同一张卡之间交易!";
    case TM_HIS_ERROR:				return "末笔交易不是在本终端上进行的操作!";
    case TM_ENABLE_ERROR:			return "终端尚未启用!";
    case TM_DISABLE_ERROR:		return "终端已停用!";
    case TM_MAXPAY_ERROR:			return "单笔交易额超限!";    
    case TM_CHARGE_ERROR:		  return "手续费查询出错!";
    case TM_CARD_ERROR:			  return "不支持的卡类型!";	
    case TM_CARD_BIND:			  return "汇出卡与绑定卡不一致!";	
		case CSP_CONNECT_ERROR:		return "CSP服务器连接错!";
		case CSP_SEND_ERROR:     	return "CSP服务器通讯错!";
		case CSP_RECV_ERROR:     	return "CSP服务器接收超时!";
		case CSP_TANS_RECV_ERROR:	return "CSP服务器接收超时,请通过末笔交易重印或余额查询确认交易结果!";
		
		case NAS_CONNECT_ERROR:		return "NAS服务器连接错!";
		case NAS_SEND_ERROR:     	return "NAS服务器通讯错!";
		case NAS_RECV_ERROR:     	return "NAS服务器接收超时!";
		case NAS_TANS_RECV_ERROR:	return "NAS服务器接收超时,请通过末笔交易重印或余额查询确认交易结果!";	
		
		case CSP_KEY_ERROR:		    return "Load Key 错误!";
		case CHECK_PSW_ERROR:			return "密码验证错误!";
		case MALLOC_ERROR:				return "分配内存空间出错"	;
		case RBS_REVERSE_ERROR:   return "交易失败,冲正失败";
		case RBS_REVERSE_SUCCESS: return "交易失败,冲正成功";
		default:									return "未知错误!";
	}
}

inline char* cspErrMsg(char* cspno)
{
	if(strcmp(cspno,"0000") == 0 )	return "交易成功";
	else if( strcmp(cspno,"SP01")== 0 || strcmp(cspno,"sp01")== 0 ) 		 return "CSP系统错误";
	else if ( strcmp(cspno,"SP02")== 0 || strcmp(cspno,"sp02")== 0 ) return "系统未知错误";
	else if ( strcmp(cspno,"SP03")== 0 || strcmp(cspno,"sp03")== 0 ) return "数据库操作失败";
	
	else if ( strcmp(cspno,"SP04")== 0 || strcmp(cspno,"sp04")== 0 ) return "记录未发现";
	
	else if ( strcmp(cspno,"SP05")== 0 || strcmp(cspno,"sp05")== 0 ) return "接收前端失败";
	else if ( strcmp(cspno,"SP06")== 0 || strcmp(cspno,"sp06")== 0 ) return "返回前端失败";
	
	else if ( strcmp(cspno,"SP07")== 0 || strcmp(cspno,"sp07")== 0 ) return "接收外系统失败";
	else if ( strcmp(cspno,"SP08")== 0 || strcmp(cspno,"sp08")== 0 ) return "返回外系统失败";
	
	else if ( strcmp(cspno,"SP09")== 0 || strcmp(cspno,"sp09")== 0 ) return "余额不足";
	else if ( strcmp(cspno,"SP10")== 0 || strcmp(cspno,"sp10")== 0 ) return "功能码错";
	
	else if ( strcmp(cspno,"SP11")== 0 || strcmp(cspno,"sp11")== 0 ) return "记日志出错";
	else if ( strcmp(cspno,"SP12")== 0 || strcmp(cspno,"sp12")== 0 ) return "数据格式错";
	
	else if ( strcmp(cspno,"SP13")== 0 || strcmp(cspno,"sp13")== 0 ) return "会员用户已被使用";
	else if ( strcmp(cspno,"SP14")== 0 || strcmp(cspno,"sp14")== 0 ) return "密码错误";
	else if ( strcmp(cspno,"SP15")== 0 || strcmp(cspno,"sp15")== 0 ) return "密码错误次数超限";
	
	else if ( strcmp(cspno,"SP16")== 0 || strcmp(cspno,"sp16")== 0 ) return "积分余额不足";
	
	else if ( strcmp(cspno,"SP17")== 0 || strcmp(cspno,"sp17")== 0 ) return "该卡号已登记";
	
	else if ( strcmp(cspno,"SP18")== 0 || strcmp(cspno,"sp18")== 0 ) return "手机号码未登记";
	else if ( strcmp(cspno,"SP19")== 0 || strcmp(cspno,"sp19")== 0 ) return "手机号码与登记不一致";
	else if ( strcmp(cspno,"SP20")== 0 || strcmp(cspno,"sp20")== 0 ) return "用户不存在";
	
	else if ( strcmp(cspno,"SP21")== 0 || strcmp(cspno,"sp21")== 0 ) return "积分兑换要求输入已登记的手机号";
	else if ( strcmp(cspno,"SP22")== 0 || strcmp(cspno,"sp22")== 0 ) return "未知卡类型";
	
	else if ( strcmp(cspno,"SP23")== 0 || strcmp(cspno,"sp23")== 0 ) return "未开通自助终端转帐服务";
	else if ( strcmp(cspno,"SP24")== 0 || strcmp(cspno,"sp24")== 0 ) return "收款帐号非省内中行帐号";
	
	else if ( strcmp(cspno,"SP25")== 0 || strcmp(cspno,"sp25")== 0 ) return "收款帐号只能是存折帐号或信用卡号";
	else if ( strcmp(cspno,"SP26")== 0 || strcmp(cspno,"sp26")== 0 ) return "找不到付款帐号的所属行";
	
	else if ( strcmp(cspno,"SP27")== 0 || strcmp(cspno,"sp27")== 0 ) return "付款帐号只能是借记卡或者信用卡";
	else if ( strcmp(cspno,"SP28")== 0 || strcmp(cspno,"sp28")== 0 ) return "暂不支持这两种账户之间转帐";
	
	else if ( strcmp(cspno,"SP29")== 0 || strcmp(cspno,"sp29")== 0 ) return "信用卡不能转外地";
	
	else if ( strcmp(cspno,"SP30")== 0 || strcmp(cspno,"sp30")== 0 ) return "非系统工作时间，转账只允许在7:30-19:30进行";
	else if ( strcmp(cspno,"SP31")== 0 || strcmp(cspno,"sp31")== 0 ) return "非系统工作时间，转账只允许在7:30-19:30进行";

	else if ( strcmp(cspno,"SP32")== 0 || strcmp(cspno,"sp32")== 0 ) return "校验卡密码失败";
	else if ( strcmp(cspno,"SP33")== 0 || strcmp(cspno,"sp33")== 0 ) return "单笔交易金额超限";
	
	else if ( strcmp(cspno,"SP34")== 0 || strcmp(cspno,"sp34")== 0 ) return "当日总交易金额超限";
	else if ( strcmp(cspno,"SP35")== 0 || strcmp(cspno,"sp35")== 0 ) return "取收款帐户户名失败";
	
	else if ( strcmp(cspno,"SP36")== 0 || strcmp(cspno,"sp36")== 0 ) return "非系统工作时间，转账只允许在7:30-19:30进行";
	else if ( strcmp(cspno,"SP37")== 0 || strcmp(cspno,"sp37")== 0 ) return "借记卡对应的存折号失败";
	
	else if ( strcmp(cspno,"SP38")== 0 || strcmp(cspno,"sp38")== 0 ) return "转账失败";
	else if ( strcmp(cspno,"SP39")== 0 || strcmp(cspno,"sp39")== 0 ) return "非本地卡";
	
	else if ( strcmp(cspno,"SP40")== 0 || strcmp(cspno,"sp40")== 0 ) return "不存在对应交易检查";
	else if ( strcmp(cspno,"SP41")== 0 || strcmp(cspno,"sp41")== 0 ) return "密码错";
	
	else if ( strcmp(cspno,"SP42")== 0 || strcmp(cspno,"sp42")== 0 ) return "ATM密码未修改，发卡后需要修改密码";
	else if ( strcmp(cspno,"SP43")== 0 || strcmp(cspno,"sp43")== 0 ) return "ATM验密错";
	
	else if ( strcmp(cspno,"SP44")== 0 || strcmp(cspno,"sp44")== 0 ) return "ATM验密错";
	else if ( strcmp(cspno,"SP45")== 0 || strcmp(cspno,"sp45")== 0 ) return "省内转账不支持信用卡转账到信用卡";
	
	else if ( strcmp(cspno,"SP46")== 0 || strcmp(cspno,"sp46")== 0 ) return "跨行或跨省转账收款帐号是省辖中行的帐号";
	else if ( strcmp(cspno,"SP47")== 0 || strcmp(cspno,"sp47")== 0 ) return "检查节假日失败";
	
	else if ( strcmp(cspno,"SP48")== 0 || strcmp(cspno,"sp48")== 0 ) return "节假日不可跨省或者跨行转账";
	else if ( strcmp(cspno,"SP49")== 0 || strcmp(cspno,"sp49")== 0 ) return "CISS返回错误";
	
	else if ( strcmp(cspno,"SP50")== 0 || strcmp(cspno,"sp50")== 0 ) return "无法取得本次交易积分";
	else if ( strcmp(cspno,"SP51")== 0 || strcmp(cspno,"sp51")== 0 ) return "非交易时间";
	
	else if ( strcmp(cspno,"SP52")== 0 || strcmp(cspno,"sp52")== 0 ) return "取LPAR出错";
	else if ( strcmp(cspno,"SP53")== 0 || strcmp(cspno,"sp53")== 0 ) return "基金认购和申购金额单笔不能超过20万元";
	
	else if ( strcmp(cspno,"SP54")== 0 || strcmp(cspno,"sp54")== 0 ) return "基金认购和申购金额当天累计不能超过50万元";
	else if ( strcmp(cspno,"SP55")== 0 || strcmp(cspno,"sp55")== 0 ) return "信用卡不可以办理基金业务";
	
	else if ( strcmp(cspno,"SP56")== 0 || strcmp(cspno,"sp56")== 0 ) return "交易失败";
	else if ( strcmp(cspno,"SP57")== 0 || strcmp(cspno,"sp57")== 0 ) return "MAC校验失败";
	
	else if ( strcmp(cspno,"SP58")== 0 || strcmp(cspno,"sp58")== 0 ) return "未知错误";
	else if ( strcmp(cspno,"SP59")== 0 || strcmp(cspno,"sp59")== 0 ) return "未知错误";
	
	else if ( strcmp(cspno,"SP60")== 0 || strcmp(cspno,"sp60")== 0 ) return "输入的收款帐号不是中行帐号";
	else if ( strcmp(cspno,"SP61")== 0 || strcmp(cspno,"sp61")== 0 ) return "非法网点号";
	
	else if ( strcmp(cspno,"SP62")== 0 || strcmp(cspno,"sp62")== 0 ) return "解密失败";
	else if ( strcmp(cspno,"SP63")== 0 || strcmp(cspno,"sp63")== 0 ) return "帐户类型错";
	
	else if ( strcmp(cspno,"SP64")== 0 || strcmp(cspno,"sp64")== 0 ) return "付款帐号与监控帐号不一致";
	else if ( strcmp(cspno,"SP65")== 0 || strcmp(cspno,"sp65")== 0 ) return "签约纪录已存在";
	
	else if ( strcmp(cspno,"SP66")== 0 || strcmp(cspno,"sp66")== 0 ) return "RTS返回错误";
	else if ( strcmp(cspno,"SP67")== 0 || strcmp(cspno,"sp67")== 0 ) return "CSP带信息返回";
	
	else if ( strcmp(cspno,"SP68")== 0 || strcmp(cspno,"sp68")== 0 ) return "取帐户名错";
	else if ( strcmp(cspno,"SP69")== 0 || strcmp(cspno,"sp69")== 0 ) return "该商户暂不支持信用卡缴费";
	
	else if ( strcmp(cspno,"SP70")== 0 || strcmp(cspno,"sp70")== 0 ) return "未知错误";
	else if ( strcmp(cspno,"SP71")== 0 || strcmp(cspno,"sp71")== 0 ) return "查积分失败";	
	else if ( strcmp(cspno,"SP72")== 0 || strcmp(cspno,"sp72")== 0 ) return "全年累计金额超限";
	
	else if ( strcmp(cspno,"NS01")== 0 ) 	return "请与银行联系";
	else if ( strcmp(cspno,"NS03")== 0 ) 	return "无效商户";
	else if ( strcmp(cspno,"NS04")== 0 ) 	return "没收";
	else if ( strcmp(cspno,"NS05")== 0 ) 	return "拒绝";
	else if ( strcmp(cspno,"NS12")== 0 )	return "交易不存在";
	else if ( strcmp(cspno,"NS13")== 0 )	return "金额无效";
	else if ( strcmp(cspno,"NS14")== 0 )	return "无效卡号";	
	else if ( strcmp(cspno,"NS15")== 0 )	return "无此发卡行";
	else if ( strcmp(cspno,"NS19")== 0 )	return "拒绝";
	else if ( strcmp(cspno,"NS23")== 0 )	return "无效金额";
	else if ( strcmp(cspno,"NS25")== 0 )	return "不能更新记录";
	else if ( strcmp(cspno,"NS30")== 0 )	return "传送格式错误";
	else if ( strcmp(cspno,"NS31")== 0 ) 	return "发卡银行未联网";
	else if ( strcmp(cspno,"NS33")== 0 )	return "没收";
	else if ( strcmp(cspno,"NS34")== 0 )	return "没收";
	else if ( strcmp(cspno,"NS36")== 0 )	return "没收";
	else if ( strcmp(cspno,"NS40")== 0 )	return "无此交易";
	else if ( strcmp(cspno,"NS41")== 0 )	return "遗失卡，没收";
	else if ( strcmp(cspno,"NS43")== 0 )	return "被盗卡，没收";
	else if ( strcmp(cspno,"NS51")== 0 )	return "拒绝";
	else if ( strcmp(cspno,"NS54")== 0 )	return "过期卡";
	else if ( strcmp(cspno,"NS55")== 0 )	return "密码错误";
	else if ( strcmp(cspno,"NS56")== 0 )	return "无卡记录";
	else if ( strcmp(cspno,"NS57")== 0 )	return "无效交易";
	else if ( strcmp(cspno,"NS58")== 0 )	return "交易不支持";
	else if ( strcmp(cspno,"NS61")== 0 )	return "超交易限额，拒绝";
	else if ( strcmp(cspno,"NS63")== 0 )	return "MAC错";
	else if ( strcmp(cspno,"NS64")== 0 )	return "原交易金额错";
	else if ( strcmp(cspno,"NS65")== 0 )	return "超交易限次，拒绝";
	else if ( strcmp(cspno,"NS68")== 0 )	return "接收超时";
	else if ( strcmp(cspno,"NS75")== 0 )	return "密码输入超限次，拒绝";
	else if ( strcmp(cspno,"NS76")== 0 )	return "产品代码错";
	else if ( strcmp(cspno,"NS78")== 0 )	return "流水号错误";
	else if ( strcmp(cspno,"NS79")== 0 )	return "交换密钥错误";
	else if ( strcmp(cspno,"NS80")== 0 )	return "批次号错误";
	else if ( strcmp(cspno,"NS88")== 0 )	return "网络失败";
	else if ( strcmp(cspno,"NS89")== 0 )	return "无效POS";
	else if ( strcmp(cspno,"NS90")== 0 )	return "系统暂停";
	else if ( strcmp(cspno,"NS91")== 0 )	return "通讯失败";
	else if ( strcmp(cspno,"NS92")== 0 )	return "路由错";
	else if ( strcmp(cspno,"NS94")== 0 )	return "重复传送";
	else if ( strcmp(cspno,"NS95")== 0 )	return "交易上送，请等待";
	else if ( strcmp(cspno,"NS96")== 0 )	return "系统故障";
	
	else if ( strncmp(cspno,"AT96",2)== 0  ) return "密码错误或后台处理失败";
	else if ( strncmp(cspno,"AT55",2)== 0  ) return "密码错误";
	else if ( strncmp(cspno,"AT34",2)== 0  ) return "CVV错误";
	else if ( strncmp(cspno,"AT43",2)== 0  ) return "CVV错误已冻结";
	else if ( strcmp(cspno,"S001") == 0) return "账户已销户";
	else if ( strcmp(cspno,"S002") == 0) return "账户已止付";
	else if ( strcmp(cspno,"S656") == 0) return "卡状态不正常";
	else if ( strcmp(cspno,"S182") == 0) return "账户余额不足";
	else if ( strcmp(cspno,"A116") == 0) return "科目号不存在";
	else if ( cspno[0]=='A' || cspno[0]=='E' || cspno[0]== 'S' ) return "后台系统错误";	
	else return "未知错误";	
}

inline char* rbsErrMsg(char* rbsno)
{
	if (strcmp(rbsno,"0000") == 0 )	return "交易成功";
	else if (strcmp(rbsno,"A001") == 0) return "A001-CICS HANDLE ERROR-NOT READY";
	else if (strcmp(rbsno,"A002") == 0) return "A002-AP PROGRAM SEND MSG ERROR";
	else if (strcmp(rbsno,"A003") == 0) return "A003-NB TRAILER ERROR";
	else if (strcmp(rbsno,"A004") == 0) return "A004-NB POST FLAG ERROR";
	else if (strcmp(rbsno,"A005") == 0) return "A005-SP TRAILER DATA ERROR";
	else if (strcmp(rbsno,"A006") == 0) return "A006-GP TRAILER ERROR";
	else if (strcmp(rbsno,"A007") == 0) return "A007-NB TRAILER DUPLICATE";
	else if (strcmp(rbsno,"A008") == 0) return "A008-SP TRAILER ERROR";
	else if (strcmp(rbsno,"A009") == 0) return "A009-GP TRAILER ERROR";
	else if (strcmp(rbsno,"A010") == 0) return "A010-BAL NOT MATCH AFTER POSTED";
	else if (strcmp(rbsno,"A011") == 0) return "A011-SP TR MR NOT MATCH";
	else if (strcmp(rbsno,"A012") == 0) return "A012-NB TRAILER POINT DUPLICATE";
	else if (strcmp(rbsno,"A013") == 0) return "A013-ITTP REVERSE ERROR";
	else if (strcmp(rbsno,"A020") == 0) return "A020-DBS ACCESS SEQUENCE ERROR";
	else if (strcmp(rbsno,"A021") == 0) return "A021-DBS ACCESS FUNCTION CODE ERROR";
	else if (strcmp(rbsno,"A022") == 0) return "A022-DBS READ/WRITE ERROR";
	else if (strcmp(rbsno,"A023") == 0) return "A023-DBS SERVICE ERROR";
	else if (strcmp(rbsno,"A024") == 0) return "A024-DBS RETURN CODE ERROR";
	else if (strcmp(rbsno,"A025") == 0) return "A025-DBS NOT FOUND";
	else if (strcmp(rbsno,"A026") == 0) return "A026-DBS TR DUPLICATE";
	else if (strcmp(rbsno,"A027") == 0) return "A027-READ OR WRITE FAIL";
	else if (strcmp(rbsno,"A030") == 0) return "A030-BIFM UPDATE ERROR";
	else if (strcmp(rbsno,"A031") == 0) return "A031-BIFM ACC CODE NOT EXIST";
	else if (strcmp(rbsno,"A032") == 0) return "A032-BIFM READ/WRITE ERROR";
	else if (strcmp(rbsno,"A033") == 0) return "A033-BIF ACTION CODE ERROR";
	else if (strcmp(rbsno,"A034") == 0) return "A034-MAIN PROG SEND DATA ERROR";
	else if (strcmp(rbsno,"A035") == 0) return "A035-BIF DUPLICATE KEY";
	else if (strcmp(rbsno,"A036") == 0) return "A036-BIF RECORD ALREADY CLOSED";
	else if (strcmp(rbsno,"A041") == 0) return "A041-ACCM-LAST-TX-GT-TX-DATE";
	else if (strcmp(rbsno,"A042") == 0) return "A042-ACCM-EX-DATE-GT-TX-DATE";
	else if (strcmp(rbsno,"A043") == 0) return "A043-ACCM-JOB-CODE-ERR";
	else if (strcmp(rbsno,"A044") == 0) return "A044-ACCM-CMOSRATE-ERR";
	else if (strcmp(rbsno,"A045") == 0) return "A045-ACCM-CMOSDATE-ERR";
	else if (strcmp(rbsno,"A046") == 0) return "A046-INT CODE ERROR";
	else if (strcmp(rbsno,"A047") == 0) return "A047-ACCM-CHARC-CODE-ERR";
	else if (strcmp(rbsno,"A048") == 0) return "A048-ACCM-MINUS-ACCM-ERR";
	else if (strcmp(rbsno,"A049") == 0) return "A049-ACCM-MINUS-ACCM-ERR";
	else if (strcmp(rbsno,"A050") == 0) return "A050-ACCM-PERIOD-MONTH-ERR";
	else if (strcmp(rbsno,"A051") == 0) return "A051-ACCM-OTHER-ERROR";
	else if (strcmp(rbsno,"A052") == 0) return "A052-REPEAT TXN AMT TYPE ERROR";
	else if (strcmp(rbsno,"A053") == 0) return "A053-LOAN TXN OVER 90% PRINCIPAL";
	else if (strcmp(rbsno,"A054") == 0) return "A054-RE-TXN ERR OF CS-TXN FREX A/C";
	else if (strcmp(rbsno,"A055") == 0) return "A055-REPEAT TXN MUST HAVE SUP CODE";
	else if (strcmp(rbsno,"A056") == 0) return "A056-REPEAT TXN INSUFFICIENT FUND";
	else if (strcmp(rbsno,"A057") == 0) return "A057-REPEAT TXN INSUFFICIENT INT.";
	else if (strcmp(rbsno,"A058") == 0) return "A058-RRN NOT FOUND";
	else if (strcmp(rbsno,"A059") == 0) return "A059-SCMTAX SUB-TP ERROR";
	else if (strcmp(rbsno,"A060") == 0) return "A060-ITTP SYSTEM ID ERROR";
	else if (strcmp(rbsno,"A061") == 0) return "A061-TEXT ID ERR FOR INTER TP";
	else if (strcmp(rbsno,"A062") == 0) return "A062-AC-TRANS RECORD NOT FOUND";
	else if (strcmp(rbsno,"A063") == 0) return "A063-AC-TABLE RECORD NOT FOUND";
	else if (strcmp(rbsno,"A064") == 0) return "A064-AC-TRANS I/O ERROR";
	else if (strcmp(rbsno,"A065") == 0) return "A065-AC-TABLE I/O ERROR";
	else if (strcmp(rbsno,"A066") == 0) return "A066-AC-TRANS RECORD NOT FOUND";
	else if (strcmp(rbsno,"A067") == 0) return "A067-AC-TRANS RECORD NOT FOUND";
	else if (strcmp(rbsno,"A068") == 0) return "A068-AC-TRANS RECORD NOT FOUND";
	else if (strcmp(rbsno,"A069") == 0) return "A069-AC-TRANS RECORD NOT FOUND";
	else if (strcmp(rbsno,"A070") == 0) return "A070-NOT FOUND ANY CT A/C";
	else if (strcmp(rbsno,"A071") == 0) return "A071-CMPSCIF SUB-TP UPDATE ERR";
	else if (strcmp(rbsno,"A072") == 0) return "A072-BIOM READ/WRITE ERROR";
	else if (strcmp(rbsno,"A073") == 0) return "A073-BIOM DATA NOT EXIST";
	else if (strcmp(rbsno,"A074") == 0) return "A074-BIOM DATA DUPLICATE";
	else if (strcmp(rbsno,"A083") == 0) return "A083-NOT ENOUGH CASH !";
	else if (strcmp(rbsno,"A084") == 0) return "A084-NO SUCH RECORD IN FILE";
	else if (strcmp(rbsno,"A085") == 0) return "A085-TELLER CASH AMT OVER LIMIT";
	else if (strcmp(rbsno,"A086") == 0) return "A086-TELLER CASH TRANSFER AMT OVER LIMIT";
	else if (strcmp(rbsno,"A087") == 0) return "A087-CAHT AMT TYPE ERROR";
	else if (strcmp(rbsno,"A088") == 0) return "A088-MUST DO EXT-BAG TRANSACTION!!";
	else if (strcmp(rbsno,"A089") == 0) return "A089-THE TELLER MUST DO 0011 REC-CASH!!";
	else if (strcmp(rbsno,"A091") == 0) return "A091-FDOSINT ERROR";
	else if (strcmp(rbsno,"A092") == 0) return "A092-FOREIGN-FEXC ERROR";
	else if (strcmp(rbsno,"A093") == 0) return "A093-THIS CUSTOMER HAS BEEN OPEN ACT IN THIS SDN";
	else if (strcmp(rbsno,"A094") == 0) return "A094-ACT SEQ NO ERROR";
	else if (strcmp(rbsno,"A095") == 0) return "A095-:CT-JOB  ERROR";
	else if (strcmp(rbsno,"A096") == 0) return "A096-DUE-DATE OR END-DATE ERROR";
	else if (strcmp(rbsno,"A097") == 0) return "A097-COUNT INTEREST ERROR";
	else if (strcmp(rbsno,"A098") == 0) return "A098-PERIOD MONTH COUNT ERROR";
	else if (strcmp(rbsno,"A099") == 0) return "A099-NOT LAST LOG FOR CANCEL ACT TXN";
	else if (strcmp(rbsno,"A101") == 0) return "A101-MULTI TX PROCESSING ERR";
	else if (strcmp(rbsno,"A102") == 0) return "A102-PROCESS ERROR CODE NOT FOUND";
	else if (strcmp(rbsno,"A103") == 0) return "A103-ILLEGAL TX LOGGING FOUND -> CHECK AP TXLOG SEQ.";
	else if (strcmp(rbsno,"A104") == 0) return "A104-ILLEGAL TX REJECT CODE FOUND";
	else if (strcmp(rbsno,"A105") == 0) return "A105-TX CODE ERROR (TCSTXLOG)";
	else if (strcmp(rbsno,"A106") == 0) return "A106-TX LOG AREA LENGTH ERR (TCSTXLOG)";
	else if (strcmp(rbsno,"A108") == 0) return "A108-THE BAL NOT ENOUGH TO WITHDRAW AMT";
	else if (strcmp(rbsno,"A109") == 0) return "A109-";
	else if (strcmp(rbsno,"A110") == 0) return "A110-";
	else if (strcmp(rbsno,"A111") == 0) return "A111-CANCEL ACT AMT ERROR";
	else if (strcmp(rbsno,"A112") == 0) return "A112-ITEM-FILE HAVE NOT THIS ITEM";
	else if (strcmp(rbsno,"A113") == 0) return "A113-ITEM-BAL ERROR";
	else if (strcmp(rbsno,"A114") == 0) return "A114-ITEM-FILE CR/DR/N ERROR";
	else if (strcmp(rbsno,"A115") == 0) return "A115-AMT TYPE ERROR";
	else if (strcmp(rbsno,"A116") == 0) return "A116-ACC CODE NOT FOUND IN CMFACCM";
	else if (strcmp(rbsno,"A201") == 0) return "A201-RECEIVE MESSAGE ERROR";
	else if (strcmp(rbsno,"A202") == 0) return "A202-COMMUNICATION ERROR";
	else if (strcmp(rbsno,"A203") == 0) return "A203-DB OPERATE ERROR";
	else if (strcmp(rbsno,"A204") == 0) return "A204-UNTIE PACKAGE ERROR";
	else if (strcmp(rbsno,"A205") == 0) return "A205-OPERATE CODE ERROR";
	else if (strcmp(rbsno,"A206") == 0) return "A206-FUNCTION CODE ERROR";
	else if (strcmp(rbsno,"A211") == 0) return "A211-RECEIVE MESSAGE ERROR";
	else if (strcmp(rbsno,"A212") == 0) return "A212-COMMUNICATION ERROR";
	else if (strcmp(rbsno,"A213") == 0) return "A213-DB OPERATE ERROR";
	else if (strcmp(rbsno,"A214") == 0) return "A214-UNTIE PACKAGE ERROR";
	else if (strcmp(rbsno,"A215") == 0) return "A215-OPERATE CODE ERROR";
	else if (strcmp(rbsno,"A216") == 0) return "A216-FUNCTION CODE ERROR";
	else if (strcmp(rbsno,"A217") == 0) return "A217-THIS ACNO HAS NOT SINGED ON AGENT";
	else if (strcmp(rbsno,"A218") == 0) return "A218-THIS SERVICE HAS BEEN APPLIED";
	else if (strcmp(rbsno,"A219") == 0) return "A219-THIS SERVICE HAS NOT BEEN APPLIED";
	else if (strcmp(rbsno,"A220") == 0) return "A220-                    ";
	else if (strcmp(rbsno,"A221") == 0) return "A221-                    ";
	else if (strcmp(rbsno,"A222") == 0) return "A222-                    ";
	else if (strcmp(rbsno,"A223") == 0) return "A223-                    ";
	else if (strcmp(rbsno,"A224") == 0) return "A224-                    ";
	else if (strcmp(rbsno,"A225") == 0) return "A225-CIF PASSWORD ERROR  ";
	else if (strcmp(rbsno,"A300") == 0) return "A300-CMOSDATE ERROR";
	else if (strcmp(rbsno,"A301") == 0) return "A301-";
	else if (strcmp(rbsno,"A302") == 0) return "A302-";
	else if (strcmp(rbsno,"A800") == 0) return "A800-TXN TYPE ERROR";
	else if (strcmp(rbsno,"A801") == 0) return "A801-CHFT BAL < 0";
	else if (strcmp(rbsno,"A802") == 0) return "A802-CAN NOT MODIFY SVFMSTM";
	else if (strcmp(rbsno,"A803") == 0) return "A803-TIA TOT LEN TOO LARGE";
	else if (strcmp(rbsno,"A804") == 0) return "A804-BAL AMT NOT EQUAL";
	else if (strcmp(rbsno,"A805") == 0) return "A805-CHFT STATUS NOT NORMAL";
	else if (strcmp(rbsno,"A806") == 0) return "A806-CHFT NOT STOP PAY YET";
	else if (strcmp(rbsno,"A807") == 0) return "A807-NO DB CR CODE";
	else if (strcmp(rbsno,"A808") == 0) return "A808-TXN AMT NOT NUMERIC";
	else if (strcmp(rbsno,"A809") == 0) return "A809-TRANSFER ACT NOT NUMERIC";
	else if (strcmp(rbsno,"A810") == 0) return "A810-JOB CODE NOT = 01";
	else if (strcmp(rbsno,"A811") == 0) return "A811-RRN LOOP WHEN REVERSE";
	else if (strcmp(rbsno,"A812") == 0) return "A812-ALREADY REVERSE";
	else if (strcmp(rbsno,"A813") == 0) return "A813-ACT OFFSET ERROR";
	else if (strcmp(rbsno,"A814") == 0) return "A814-INVALID TXN AT CUT OFF";
	else if (strcmp(rbsno,"A815") == 0) return "A815-IET DEFINE ERROR";
	else if (strcmp(rbsno,"A816") == 0) return "A816-FIELD LEN ERROR";
	else if (strcmp(rbsno,"A901") == 0) return "A901-BCSUVSAM:FILE ALREADY EXIT";
	else if (strcmp(rbsno,"A902") == 0) return "A902-BCSUVSAM:DATA OR FILE NOTFND";
	else if (strcmp(rbsno,"A903") == 0) return "A903-BCSUVSAM:FILE DUPLICATE";
	else if (strcmp(rbsno,"A904") == 0) return "A904-BCSUVSAM:HAS WARNING MESSAGE";
	else if (strcmp(rbsno,"A905") == 0) return "A905-BCSUVSAM:END OF FILE";
	else if (strcmp(rbsno,"A906") == 0) return "A906-BCSUVSAM:INVALID FUNCTION";
	else if (strcmp(rbsno,"A907") == 0) return "A907-BCSUVSAM:OTHER ERRORS HAPPENED";
	else if (strcmp(rbsno,"A908") == 0) return "A908-BCSUVSAM:DATABASE HAS ERROR";
	else if (strcmp(rbsno,"A909") == 0) return "A909-BCSUVSAM:KEY HAS NON-NUMERNIC DATA";
	else if (strcmp(rbsno,"A910") == 0) return "A910-BCSUVSAM:LENGTH ERROR";
	else if (strcmp(rbsno,"A911") == 0) return "A911-BCSUVSAM:DBS RETURN CODE INVALID";
	else if (strcmp(rbsno,"A912") == 0) return "A912-";
	else if (strcmp(rbsno,"A999") == 0) return "A999-END OF A-MST";
	else if (strcmp(rbsno,"E001") == 0) return "E001-B.C.S NOT START YET!";
	else if (strcmp(rbsno,"E002") == 0) return "E002-RESTART SYSTEM ERROR";
	else if (strcmp(rbsno,"E003") == 0) return "E003-TM TYPE NOT IN SFCVT";
	else if (strcmp(rbsno,"E004") == 0) return "E004-THIS PROGRAM CAN NOT USED";
	else if (strcmp(rbsno,"E005") == 0) return "E005-ERR ON STD FORMAT CONCT";
	else if (strcmp(rbsno,"E006") == 0) return "E006-??????????";
	else if (strcmp(rbsno,"E007") == 0) return "E007-SYSTEM ID ERROR";
	else if (strcmp(rbsno,"E008") == 0) return "E008-TXN ID ERROR";
	else if (strcmp(rbsno,"E009") == 0) return "E009-1ST LIT-TX ERR";
	else if (strcmp(rbsno,"E010") == 0) return "E010-ERR ON DISCOMPOSE";
	else if (strcmp(rbsno,"E011") == 0) return "E011-THIS FIELD MUST INPUT   !";
	else if (strcmp(rbsno,"E012") == 0) return "E012-ATTRB OF DIDT ERR";
	else if (strcmp(rbsno,"E013") == 0) return "E013-DATA LEN EXCEED IETDID";
	else if (strcmp(rbsno,"E014") == 0) return "E014-INPUT DATA NOT NUMERIC  !";
	else if (strcmp(rbsno,"E015") == 0) return "E015-INPUT DATA LENGTH ERROR";
	else if (strcmp(rbsno,"E017") == 0) return "E017-TMID NOT FOUND IN BIT";
	else if (strcmp(rbsno,"E018") == 0) return "E018-TM IS CLOSED";
	else if (strcmp(rbsno,"E019") == 0) return "E019-ACCT DISABLED";
	else if (strcmp(rbsno,"E020") == 0) return "E020-TERM BACKED";
	else if (strcmp(rbsno,"E021") == 0) return "E021-TERM UNAUTHORIZED";
	else if (strcmp(rbsno,"E022") == 0) return "E022-BR-ID NOT FOUND IN BIT";
	else if (strcmp(rbsno,"E023") == 0) return "E023-THIS ACT NOT BELONG TO TRAINING SDN";
	else if (strcmp(rbsno,"E024") == 0) return "E024-THIS SDN NOT IN TRAINING STATUS";
	else if (strcmp(rbsno,"E025") == 0) return "E025-SYSTEM ALREADY START";
	else if (strcmp(rbsno,"E026") == 0) return "E026-SYSTEM NEED FORCE END";
	else if (strcmp(rbsno,"E030") == 0) return "E030-NOW IS NOT ONBATCH MODE";
	else if (strcmp(rbsno,"E031") == 0) return "E031-CWTXMODE ERROR";
	else if (strcmp(rbsno,"E032") == 0) return "E032-CLIENT TXN,BUT IN BATCH MODE";
	else if (strcmp(rbsno,"E033") == 0) return "E033-FUNCTION ERROR";
	else if (strcmp(rbsno,"E034") == 0) return "E034-OB SYSTEM ERROR";
	else if (strcmp(rbsno,"E035") == 0) return "E035-INVALID VERSION";
	else if (strcmp(rbsno,"E036") == 0) return "E036-NOT ON-BATCH TRM";
	else if (strcmp(rbsno,"E037") == 0) return "E037-NO SYSTEM ADDRESS";
	else if (strcmp(rbsno,"E038") == 0) return "E038-THIS SYSTEM IS INVALID";
	else if (strcmp(rbsno,"E039") == 0) return "E039-THIS SYSTEM IS DISABLE";
	else if (strcmp(rbsno,"E040") == 0) return "E040-SEQ NO TOO LONG";
	else if (strcmp(rbsno,"E041") == 0) return "E041-LTID NOT MATCH";
	else if (strcmp(rbsno,"E042") == 0) return "E042-LOG NOT FOUND";
	else if (strcmp(rbsno,"E050") == 0) return "E050-PROGRAM ID ERROR";
	else if (strcmp(rbsno,"E051") == 0) return "E051-SYSTEM MEMORY NOT ENOUGH";
	else if (strcmp(rbsno,"E052") == 0) return "E052-FILE ID ERROR";
	else if (strcmp(rbsno,"E053") == 0) return "E053-CICS ERROR";
	else if (strcmp(rbsno,"E054") == 0) return "E054-READ/WRITE ERROR";
	else if (strcmp(rbsno,"E055") == 0) return "E055-READ/WRITE LENGTH ERROR";
	else if (strcmp(rbsno,"E056") == 0) return "E056-LOGICAL ERROR";
	else if (strcmp(rbsno,"E057") == 0) return "E057-DATA DUPLICATE";
	else if (strcmp(rbsno,"E060") == 0) return "E060-TX LOG NOT A BIT BK REC";
	else if (strcmp(rbsno,"E061") == 0) return "E061-CWA SYSTEM DATE NOT MATCH WITH LOG DATE";
	else if (strcmp(rbsno,"E062") == 0) return "E062-FINAL RRN ERR";
	else if (strcmp(rbsno,"E063") == 0) return "E063-TERMINAL ID NOT EQUAL TO BIT";
	else if (strcmp(rbsno,"E064") == 0) return "E064-TERMINAL NOT FOUND IN BIT";
	else if (strcmp(rbsno,"E065") == 0) return "E065-NO WAY TO BACKWARD";
	else if (strcmp(rbsno,"E066") == 0) return "E066-MUST BE BATCH TERMINAL";
	else if (strcmp(rbsno,"E067") == 0) return "E067-RRN ERROR";
	else if (strcmp(rbsno,"E068") == 0) return "E068-DETAIL DATA RECORD NO ERROR";
	else if (strcmp(rbsno,"E069") == 0) return "E069-DBS ACCESS RETURN CODE ERROR";
	else if (strcmp(rbsno,"E071") == 0) return "E071-INIT NOT I OR R";
	else if (strcmp(rbsno,"E072") == 0) return "E072-TOO MANY INT RATE IN ACIA FILE !";
	else if (strcmp(rbsno,"E073") == 0) return "E073-TAX REC PTR ERR";
	else if (strcmp(rbsno,"E074") == 0) return "E074-FAVOR TAB PTR ERR";
	else if (strcmp(rbsno,"E075") == 0) return "E075-INIT FILE DATE ERR";
	else if (strcmp(rbsno,"E076") == 0) return "E076-INIT INPUT DATE ERR";
	else if (strcmp(rbsno,"E077") == 0) return "E077-ACIA INT TAB ERR";
	else if (strcmp(rbsno,"E078") == 0) return "E078-CALL DATE SUBPROGRAM ERROR";
	else if (strcmp(rbsno,"E079") == 0) return "E079-LOG POINTER ERR";
	else if (strcmp(rbsno,"E080") == 0) return "E080-INVALID REVERSE TX-ID";
	else if (strcmp(rbsno,"E081") == 0) return "E081-TMNORCNT > TWNORCNT";
	else if (strcmp(rbsno,"E082") == 0) return "E082-TMSPECNT > TWSPECNT";
	else if (strcmp(rbsno,"E083") == 0) return "E083-TMBTCNT > TWBTCNT";
	else if (strcmp(rbsno,"E084") == 0) return "E084-TMSPECNT < 90000";
	else if (strcmp(rbsno,"E085") == 0) return "E085-ATM PROCESSING ERROR FOUND";
	else if (strcmp(rbsno,"E086") == 0) return "E086-INVALID ATM PIN KEY";
	else if (strcmp(rbsno,"E087") == 0) return "E087-OPEN FILE ERROR";
	else if (strcmp(rbsno,"E088") == 0) return "E088-FILE SPACE NOT ENOUGH";
	else if (strcmp(rbsno,"E089") == 0) return "E089-HANDLE CONDITION ERR";
	else if (strcmp(rbsno,"E090") == 0) return "E090-CAN NOT READ FLAG=1 FX-RATE RECORD";
	else if (strcmp(rbsno,"E091") == 0) return "E091-NO INT DATA LOAD IN ACIA";
	else if (strcmp(rbsno,"E092") == 0) return "E092-ACC CODE NOTFND IN CMFACCM";
	else if (strcmp(rbsno,"E093") == 0) return "E093-REVERSE TXN AGAIN";
	else if (strcmp(rbsno,"E094") == 0) return "E094-????";
	else if (strcmp(rbsno,"E095") == 0) return "E095-????";
	else if (strcmp(rbsno,"E096") == 0) return "E096-NOT JUST CAN RUN AUTO TXN";
	else if (strcmp(rbsno,"E097") == 0) return "E097-NOT CAN NOT RUN AUTO TXN";
	else if (strcmp(rbsno,"E098") == 0) return "E098-THIS SDN ALREADY CLOSE";
	else if (strcmp(rbsno,"E099") == 0) return "E099-CICS INSTRUCTION ERROR";
	else if (strcmp(rbsno,"E100") == 0) return "E100-THIS FORM NOT FOUND IN SOCVT";
	else if (strcmp(rbsno,"E101") == 0) return "E101-24 HOURS SYSTEM NOT START";
	else if (strcmp(rbsno,"E901") == 0) return "E901-TCTUA LEN < 64";
	else if (strcmp(rbsno,"E902") == 0) return "E902-TMID NOT IN BIT";
	else if (strcmp(rbsno,"E903") == 0) return "E903-PLS. OUT ALL HOLDING MSG";
	else if (strcmp(rbsno,"E904") == 0) return "E904-UNAUTHORIZED MSG HOLDING REQUEST";
	else if (strcmp(rbsno,"E999") == 0) return "E999-END OF E-MST";
	else if (strcmp(rbsno,"S001") == 0) return "S001-HAS CLOSED";
	else if (strcmp(rbsno,"S002") == 0) return "S002-HAS STOP PAY";
	else if (strcmp(rbsno,"S003") == 0) return "S003-HAS TRANSFERED  !";
	else if (strcmp(rbsno,"S004") == 0) return "S004-MEMO MUST BE 1 OR 2!";
	else if (strcmp(rbsno,"S005") == 0) return "S005-ACC-CODE NOT FOUND !";
	else if (strcmp(rbsno,"S006") == 0) return "S006-ACT STATUS ERROR";
	else if (strcmp(rbsno,"S007") == 0) return "S007-HAS WITHDRAWN ALL !";
	else if (strcmp(rbsno,"S008") == 0) return "S008-HAS WITHDRAWN IN PART !";
	else if (strcmp(rbsno,"S009") == 0) return "S009-AMT-INT HAS DEPOSITED IN ANOTHER ACT-NO !";
	else if (strcmp(rbsno,"S010") == 0) return "S010-REC-PAY ERROR";
	else if (strcmp(rbsno,"S011") == 0) return "S011-JOB CODE ERROR.MUST BE INPUT";
	else if (strcmp(rbsno,"S012") == 0) return "S012-AMT MUST GREAT THAN ZEROS";
	else if (strcmp(rbsno,"S013") == 0) return "S013-AMT HAS DEPOSITED IN ANOTHER ACT-NO !";
	else if (strcmp(rbsno,"S014") == 0) return "S014-AMT-INT + CASH HAS DEPOSITED IN ANOTHER ACT-NO !";
	else if (strcmp(rbsno,"S015") == 0) return "S015-CIF NOT CREATED !";
	else if (strcmp(rbsno,"S016") == 0) return "S016-CIF NOT MATCHED !";
	else if (strcmp(rbsno,"S017") == 0) return "S017-HAS BEEN OPENED !";
	else if (strcmp(rbsno,"S018") == 0) return "S018-ACT STATUS ERROR !";
	else if (strcmp(rbsno,"S019") == 0) return "S019-HAS NOT OPEN !";
	else if (strcmp(rbsno,"S020") == 0) return "S020-HAS NOT DUMMY !";
	else if (strcmp(rbsno,"S021") == 0) return "S021-DUMMY RECORD";
	else if (strcmp(rbsno,"S022") == 0) return "S022-CHANGE CT OR PB !";
	else if (strcmp(rbsno,"S023") == 0) return "S023-MEMO  MUST BE 2 !!!";
	else if (strcmp(rbsno,"S024") == 0) return "S024-THIS ACCT. IS NOT ACCT. OF TARGET";
	else if (strcmp(rbsno,"S025") == 0) return "S025-PLS. AUTHORIZE FOR INQUIRY";
	else if (strcmp(rbsno,"S026") == 0) return "S026-ACCT. NOT FOUND IN FDFMSTM";
	else if (strcmp(rbsno,"S027") == 0) return "S027-READ FDFMSTM ERROR";
	else if (strcmp(rbsno,"S028") == 0) return "S028-THIS CT HAVE REVERSE  .";
	else if (strcmp(rbsno,"S029") == 0) return "S029-DETAIL/TOTAL MUST BE 0 OR 1";
	else if (strcmp(rbsno,"S030") == 0) return "S030-DIS/PRT STATUS MUST BE 0 OR 1";
	else if (strcmp(rbsno,"S031") == 0) return "S031-INPUT CUU OR JOB OR CHAR CODE ERROR";
	else if (strcmp(rbsno,"S032") == 0) return "S032-MUST INPUT OFFICE CODE";
	else if (strcmp(rbsno,"S033") == 0) return "S033-MUST INPUT PAY ITEM";
	else if (strcmp(rbsno,"S034") == 0) return "S034-DESIGN CODE MUST BE 0 OR 1";
	else if (strcmp(rbsno,"S035") == 0) return "S035-VALID DATE IS INVALID";
	else if (strcmp(rbsno,"S036") == 0) return "S036-MUST HAVE SUPERVISOR KEY";
	else if (strcmp(rbsno,"S037") == 0) return "S037-THIS NET CANNOT PROCESS THE OFFICE--PAY-CODE";
	else if (strcmp(rbsno,"S038") == 0) return "S038-CUU WRONG----THIS TXN ALLOW CASH ACCOUNT!";
	else if (strcmp(rbsno,"S039") == 0) return "S039-MUST  INPUT BUY/SEL AMT!";
	else if (strcmp(rbsno,"S040") == 0) return "S040-BUY AMT . SELL AMT ONLY ALLOW INPUT ONE!";
	else if (strcmp(rbsno,"S041") == 0) return "S041-CUU NOT FOUND!";
	else if (strcmp(rbsno,"S042") == 0) return "S042-THE  BIO FILE HAS NOT DATA FOR THIS FEE";
	else if (strcmp(rbsno,"S043") == 0) return "S043-WK-ACC-CUU-CODE NOT MATCH WITH ACC CODE";
	else if (strcmp(rbsno,"S044") == 0) return "S044-THIS ACC CODE IS HEAD TYPE";
	else if (strcmp(rbsno,"S045") == 0) return "S045-NEW A/C DEPOSIT DATE ERROR";
	else if (strcmp(rbsno,"S046") == 0) return "S046-OBJP0004 FAIL, CAN NOT CHANGE CODE";
	else if (strcmp(rbsno,"S047") == 0) return "S047-VALID DATE BE MUST GREAT THAN BUSINESS DATE";
	else if (strcmp(rbsno,"S048") == 0) return "S048-OFFICE = SPACE, PAY-ITEM NOT = 0 !";
	else if (strcmp(rbsno,"S049") == 0) return "S049-IF HAS ACT-NO, MUST INPUT OFFICE,ITEM!";
	else if (strcmp(rbsno,"S050") == 0) return "S050-NORMAL CT , CAN NOT REVERSE";
	else if (strcmp(rbsno,"S051") == 0) return "S051-JOB CODE ERROR.MUST BE 50 OR 51";
	else if (strcmp(rbsno,"S052") == 0) return "S052-NO REVERSE ON SP-LOANING";
	else if (strcmp(rbsno,"S053") == 0) return "S053-UNSUCCESSFUL WRITE OR READ";
	else if (strcmp(rbsno,"S054") == 0) return "S054-IF BEFORE INT,AMT-TYPEMUST BE 2";
	else if (strcmp(rbsno,"S055") == 0) return "S055-BUY CUU AND SELL CUU MUST DIFFERENT!";
	else if (strcmp(rbsno,"S056") == 0) return "S056-THIS ACC ALREADY CLOSE";
	else if (strcmp(rbsno,"S057") == 0) return "S057-";
	else if (strcmp(rbsno,"S058") == 0) return "S058-";
	else if (strcmp(rbsno,"S059") == 0) return "S059-";
	else if (strcmp(rbsno,"S060") == 0) return "S060-MUST INPUT AMT-TYPE";
	else if (strcmp(rbsno,"S061") == 0) return "S061-ENT-AMT IS INCORRECT,PLS CHECK";
	else if (strcmp(rbsno,"S062") == 0) return "S062-EXT-AMT IS INCORRECT,PLS CHECK";
	else if (strcmp(rbsno,"S063") == 0) return "S063-TELLER  SHOULD DO 0011 REC-AMT  !!";
	else if (strcmp(rbsno,"S064") == 0) return "S064-";
	else if (strcmp(rbsno,"S065") == 0) return "S065-HAVE TELLER HAS NOT ENT-BAG!!!";
	else if (strcmp(rbsno,"S066") == 0) return "S066-INTERBR TXN EXCEED LIMIT";
	else if (strcmp(rbsno,"S067") == 0) return "S067-DO 1229 TXN PLS.";
	else if (strcmp(rbsno,"S068") == 0) return "S068-NO INTER-BRANCH BAD CHECK";
	else if (strcmp(rbsno,"S069") == 0) return "S069-RELOAN AMT > SUM OF LOAN";
	else if (strcmp(rbsno,"S070") == 0) return "S070-NO TXN THIS PERIOD";
	else if (strcmp(rbsno,"S071") == 0) return "S071-";
	else if (strcmp(rbsno,"S072") == 0) return "S072-NO DATA FOUND ";
	else if (strcmp(rbsno,"S073") == 0) return "S073-";
	else if (strcmp(rbsno,"S074") == 0) return "S074-";
	else if (strcmp(rbsno,"S075") == 0) return "S075-";
	else if (strcmp(rbsno,"S076") == 0) return "S076-";
	else if (strcmp(rbsno,"S077") == 0) return "S077-";
	else if (strcmp(rbsno,"S078") == 0) return "S078-CHINESE NAME AND ENGLISH NAME ALL SPACES";
	else if (strcmp(rbsno,"S079") == 0) return "S079-THIS ACT-NO NOT FOUND !!!";
	else if (strcmp(rbsno,"S080") == 0) return "S080-ONLY OPEN ORIGNAL RMB PASSBK";
	else if (strcmp(rbsno,"S081") == 0) return "S081-CHAR-CODE=11 21, AMT-TYPE MUST 01";
	else if (strcmp(rbsno,"S082") == 0) return "S082-LOST PERIOD < 7, CANNOT ISSUING";
	else if (strcmp(rbsno,"S083") == 0) return "S083-STOP PAY IN LAW, SUP-NO MUST 59 60";
	else if (strcmp(rbsno,"S084") == 0) return "S084-TELLER-ID CAN NOT 99";
	else if (strcmp(rbsno,"S085") == 0) return "S085-CUU-CODE=15 20 28 35, CHAR-CODE MUST=10 20";
	else if (strcmp(rbsno,"S086") == 0) return "S086-JOB-CODE HAVE INPUT,TELLER-ID NOT 99";
	else if (strcmp(rbsno,"S087") == 0) return "S087-THIS TXN SEL-CUU MUST BE NOT 01";
	else if (strcmp(rbsno,"S088") == 0) return "S088-THIS TXN BUY-CUU MUST BE NOT 01";
	else if (strcmp(rbsno,"S089") == 0) return "S089-START SEQ NO > END SEQ NO";
	else if (strcmp(rbsno,"S090") == 0) return "S090-ORGINAL TID NOT FOUND";
	else if (strcmp(rbsno,"S091") == 0) return "S091-ORGINAL LOG REC NOT FOUND";
	else if (strcmp(rbsno,"S092") == 0) return "S092-ACT NOT MATCH ";
	else if (strcmp(rbsno,"S093") == 0) return "S093-REVERSE TXN AGAIN";
	else if (strcmp(rbsno,"S094") == 0) return "S094-ENTRY DATA IN PROCESSING NOW";
	else if (strcmp(rbsno,"S095") == 0) return "S095-PLEASE USE  0015  TO INQUIRE BUY-AMT !";
	else if (strcmp(rbsno,"S096") == 0) return "S096-NO-REPEAT ON INTER-BRANCH";
	else if (strcmp(rbsno,"S097") == 0) return "S097-ACT-NO NOT NOMAL,IF INQU NEED SUPERVIDOR";
	else if (strcmp(rbsno,"S098") == 0) return "S098-NOT HAVE THIS TRM-ID !";
	else if (strcmp(rbsno,"S099") == 0) return "S099-MR-LOG-TXN-RRN=0,RECORD ERROR !";
	else if (strcmp(rbsno,"S100") == 0) return "S100-RELEASE CODE WRONG  !";
	else if (strcmp(rbsno,"S101") == 0) return "S101-TERMINAL TYPE ERROR !";
	else if (strcmp(rbsno,"S102") == 0) return "S102-EC AMT NOT MATCH !";
	else if (strcmp(rbsno,"S103") == 0) return "S103-CROSS SDN MUST ON-LINE !";
	else if (strcmp(rbsno,"S104") == 0) return "S104-CAN NOT CROSS SDN !";
	else if (strcmp(rbsno,"S105") == 0) return "S105-MUST HAVE PASSBOOK !";
	else if (strcmp(rbsno,"S106") == 0) return "S106-MUST HAVE SUPERVISOR KEY !";
	else if (strcmp(rbsno,"S107") == 0) return "S107-AMT TYPE MUST = 2 OR 99 !";
	else if (strcmp(rbsno,"S108") == 0) return "S108-EX INT DATE ERROR !";
	else if (strcmp(rbsno,"S109") == 0) return "S109-DATA NOT FOUND !";
	else if (strcmp(rbsno,"S110") == 0) return "S110-OLD BAL NOT MATCH !";
	else if (strcmp(rbsno,"S111") == 0) return "S111-PASSBOOK-NO NOT MATCH !";
	else if (strcmp(rbsno,"S112") == 0) return "S112-BOOK HAS BEEN LOST !";
	else if (strcmp(rbsno,"S113") == 0) return "S113-AMT TYPE ERROR !";
	else if (strcmp(rbsno,"S114") == 0) return "S114-AVAIL BAL NOT ENOUGH !";
	else if (strcmp(rbsno,"S115") == 0) return "S115-SDN NOT MATCH !";
	else if (strcmp(rbsno,"S116") == 0) return "S116-MUST OPERATE ON-LINE !";
	else if (strcmp(rbsno,"S117") == 0) return "S117-CUU CODE ERROR !";
	else if (strcmp(rbsno,"S118") == 0) return "S118-JOB CODE ERROR !";
	else if (strcmp(rbsno,"S119") == 0) return "S119-CHARC CODE ERROR !";
	else if (strcmp(rbsno,"S120") == 0) return "S120-OPEN ACT AMT NOT ENOUGH !";
	else if (strcmp(rbsno,"S121") == 0) return "S121-ACCESS METHOD ERROR !";
	else if (strcmp(rbsno,"S122") == 0) return "S122-PLEASE INPUT PASSWORD !";
	else if (strcmp(rbsno,"S123") == 0) return "S123-ACT EXIST !";
	else if (strcmp(rbsno,"S124") == 0) return "S124-ACT NOT FOUND !";
	else if (strcmp(rbsno,"S125") == 0) return "S125-MAY NOT HAVE PASSBOOK !";
	else if (strcmp(rbsno,"S126") == 0) return "S126-MR OR ACT NOT NORMAL !";
	else if (strcmp(rbsno,"S127") == 0) return "S127-CAN NOT EC !";
	else if (strcmp(rbsno,"S128") == 0) return "S128-OPERATE MUST IN OPEN SDN !";
	else if (strcmp(rbsno,"S129") == 0) return "S129-STOP PAY CODE ERROR !";
	else if (strcmp(rbsno,"S130") == 0) return "S130-STOP/RELEASE CODE ERROR !";
	else if (strcmp(rbsno,"S131") == 0) return "S131-NO STOP PAY DATA, CANNOT RELEASE !";
	else if (strcmp(rbsno,"S132") == 0) return "S132-CAN NOT STOP PAY AGAIN !";
	else if (strcmp(rbsno,"S133") == 0) return "S133-APPLY CODE ERROR !";
	else if (strcmp(rbsno,"S134") == 0) return "S134-APPLY/CANCEL CODE ERROR !";
	else if (strcmp(rbsno,"S135") == 0) return "S135-NO TXN NEED TO POST !";
	else if (strcmp(rbsno,"S136") == 0) return "S136-AGAIN ERROR !";
	else if (strcmp(rbsno,"S137") == 0) return "S137-NEVER APPLY, CAN NOT CANCEL !";
	else if (strcmp(rbsno,"S138") == 0) return "S138-CAN NOT APPLY AGAIN !";
	else if (strcmp(rbsno,"S139") == 0) return "S139-BIO DATE ERROR";
	else if (strcmp(rbsno,"S140") == 0) return "S140-MR CODE ERROR";
	else if (strcmp(rbsno,"S141") == 0) return "S141-HAVE NO BOOK REFRENCE !";
	else if (strcmp(rbsno,"S142") == 0) return "S142-PASSBOOK LINE NOT = MASTER FILE !";
	else if (strcmp(rbsno,"S143") == 0) return "S143-NOT FOUND RATE CODE !";
	else if (strcmp(rbsno,"S144") == 0) return "S144-JOB CODE WRONG  !";
	else if (strcmp(rbsno,"S145") == 0) return "S145-HAVE TRANSACTION AFTER OPEN TODAY!";
	else if (strcmp(rbsno,"S146") == 0) return "S146-WITHDRAW DATE NOT RIGHT !";
	else if (strcmp(rbsno,"S147") == 0) return "S147-PLEASE INPUT PASSWORD  !";
	else if (strcmp(rbsno,"S148") == 0) return "S148-ACC CODE ERRO!";
	else if (strcmp(rbsno,"S149") == 0) return "S149-NOT THIS SDN BIO PAY ITEM";
	else if (strcmp(rbsno,"S150") == 0) return "S150-OLD PASSWORD NOT EXIST  !";
	else if (strcmp(rbsno,"S151") == 0) return "S151-AMT-TYPE  MUST  =  02 !!!";
	else if (strcmp(rbsno,"S152") == 0) return "S152-CUU,JOB,CHAR MUST SAME !!";
	else if (strcmp(rbsno,"S153") == 0) return "S153-THE BIO-TXN HAS BEEN CANCELED";
	else if (strcmp(rbsno,"S154") == 0) return "S154-AFTER VAL DATE SET,CANNOT DO 1120 1220 ";
	else if (strcmp(rbsno,"S155") == 0) return "S155-PASSWORD ERROR !";
	else if (strcmp(rbsno,"S156") == 0) return "S156-ACCT  NOT  CLOSED,  CAN  NOT  REVERSE  !";
	else if (strcmp(rbsno,"S157") == 0) return "S157-INPUT AMT  WRONG  !";
	else if (strcmp(rbsno,"S158") == 0) return "S158-MAY   NOT  HAVE  BIO-TXN  !";
	else if (strcmp(rbsno,"S159") == 0) return "S159-NEW  ACCT,  DO  REVERSE-TXN  TO CLOSE IT !";
	else if (strcmp(rbsno,"S160") == 0) return "S160-THIS PAY ID HAS DB AMT , CAN NOT DO 1120 !";
	else if (strcmp(rbsno,"S161") == 0) return "S161-THIS PAY ID HAS CR AMT , CAN NOT DO 1220 !";
	else if (strcmp(rbsno,"S162") == 0) return "S162-ACT-NO IS NOT FOR RMB,THIS TXN FIT TO RMB BUSN.";
	else if (strcmp(rbsno,"S163") == 0) return "S163-FOREIGN CUU, JOB-CODE ONLY BE 02.";
	else if (strcmp(rbsno,"S164") == 0) return "S164-RMB CHAR-CODE ONLY BE 00, 01";
	else if (strcmp(rbsno,"S165") == 0) return "S165-FRG CGAR-CODE ONLY BE 10 11 20 21";
	else if (strcmp(rbsno,"S166") == 0) return "S166-WHEN REVERSE, AMT-TYPE MUST BE SAME WITH OLD ";
	else if (strcmp(rbsno,"S167") == 0) return "S167-WHEN REVERSE, MSTR-FIL-STAS NOT NORMAL ";
	else if (strcmp(rbsno,"S168") == 0) return "S168-WNEN REVERSE. AVAIL BAL NOT MATCH WITH EC-AMT";
	else if (strcmp(rbsno,"S169") == 0) return "S169-WNEN CLOSE, PASSBOOK-BAL NOT RIGHT";
	else if (strcmp(rbsno,"S170") == 0) return "S170-WNEN CLOSE, PLS INPUT THE PSWD OF OPEN ACT-NO.";
	else if (strcmp(rbsno,"S171") == 0) return "S171-WNEN REVERSE AND NOT ITTP: AMT ERROR.";
	else if (strcmp(rbsno,"S172") == 0) return "S172-WNEN REVERSE AND NOT ITTP: AMT-TYPE ERROR.";
	else if (strcmp(rbsno,"S173") == 0) return "S173-PASSBOOK BAL NOT = FILE BAL. PLS CHECK";
	else if (strcmp(rbsno,"S174") == 0) return "S174-PASSBOOK NO  NOT = FILE NO.PLS CHECK";
	else if (strcmp(rbsno,"S175") == 0) return "S175-IN THIS CASE MUST HAVE PASSBOOK";
	else if (strcmp(rbsno,"S176") == 0) return "S176-EX-DT MUST > LST-CMP-INT-DT, < TCIA-BUSN-DT";
	else if (strcmp(rbsno,"S177") == 0) return "S177-DO TXN-1100 AND NO ITTP, AMT-TYPE IS 1-2";
	else if (strcmp(rbsno,"S178") == 0) return "S178-CANCEL AMT NOT =  BIO AMT";
	else if (strcmp(rbsno,"S179") == 0) return "S179-OPENING ACT BANK OFFLINE, CANNOT DO 1200";
	else if (strcmp(rbsno,"S180") == 0) return "S180-NO BOOK AND WITHDRAW,NEED PSWD";
	else if (strcmp(rbsno,"S181") == 0) return "S181-DO 1200 MUST HAVE PSWD OR SEAL";
	else if (strcmp(rbsno,"S182") == 0) return "S182-THE BAL NOT ENOUGH TO WITHDRAW AMT";
	else if (strcmp(rbsno,"S183") == 0) return "S183-WHEN ONLINE,MUST HAVE PASSBOOK";
	else if (strcmp(rbsno,"S184") == 0) return "S184-WHEN NO PASSBOOK,MUST SUPERVISER";
	else if (strcmp(rbsno,"S185") == 0) return "S185-ONLY OPENING ACT BANK CAN RELEASE";
	else if (strcmp(rbsno,"S186") == 0) return "S186-WHEN RELEASE NEED SUPERVISOR";
	else if (strcmp(rbsno,"S187") == 0) return "S187-WHEN STOP CAN NOT OFFLINE";
	else if (strcmp(rbsno,"S188") == 0) return "S188-NEW AND OLD PASSWORD MUST INPUT";
	else if (strcmp(rbsno,"S189") == 0) return "S189-NO PWD IN FLIE, CANNOT CHANGE";
	else if (strcmp(rbsno,"S190") == 0) return "S190-AGAIN-CODE =0,1 MUST HAVE PASSBOOK";
	else if (strcmp(rbsno,"S191") == 0) return "S191-AGAIN-CODE =3,  NOT NEED PASSBOOK";
	else if (strcmp(rbsno,"S192") == 0) return "S192-BOOK STOPPED, AGAIN-CD CANOT BE 0 1 3";
	else if (strcmp(rbsno,"S193") == 0) return "S193-FIRST 1300,THEN DO 1720 FOR AGAIN-CODE=2";
	else if (strcmp(rbsno,"S194") == 0) return "S194-BOOK NOT LOST,CANNOT DO LOSTLY REISS";
	else if (strcmp(rbsno,"S195") == 0) return "S195-WIDRAW-CD=3, DO NOT INPUT PASSWORD";
	else if (strcmp(rbsno,"S196") == 0) return "S196-TRANS-AMT VERY LARGE,REJECT TO DO";
	else if (strcmp(rbsno,"S197") == 0) return "S197-OPEN REVERSE,CANOT HAS TRAN.";
	else if (strcmp(rbsno,"S198") == 0) return "S198-CASH ACT CAN NOT TRANS AMT TO NON-CASH ACT";
	else if (strcmp(rbsno,"S199") == 0) return "S199-TWO ACT DUPLICATE";
	else if (strcmp(rbsno,"S200") == 0) return "S200-BLACK CARD";
	else if (strcmp(rbsno,"S201") == 0) return "S201-THE CARD IS INVALID";
	else if (strcmp(rbsno,"S202") == 0) return "S202-THE BAL NOT ENOUGH TO WITHDRAW ";
	else if (strcmp(rbsno,"S203") == 0) return "S203-CONNECT TO BRANCH ISSU THIS CARD";
	else if (strcmp(rbsno,"S204") == 0) return "S204-TXN CODE ERROR";
	else if (strcmp(rbsno,"S209") == 0) return "S209-INVALAID BUSINESS NO";
	else if (strcmp(rbsno,"S210") == 0) return "S210-INVALAID AMOUNT";
	else if (strcmp(rbsno,"S211") == 0) return "S211-INVALAID CARD NO";
	else if (strcmp(rbsno,"S216") == 0) return "S216-NOT ENOUGH AMOUNT OF MONEY";
	else if (strcmp(rbsno,"S222") == 0) return "S222-GO TO CLOSED NET";
	else if (strcmp(rbsno,"S223") == 0) return "S223-SUCESS THE RESTRACT OF WITHDRAW TIMES";
	else if (strcmp(rbsno,"S224") == 0) return "S224-ONLY IN OPEN SDN OR STOP SDN";
	else if (strcmp(rbsno,"S230") == 0) return "S230-STATUS OF CARD IS NOT NORMAL";
	else if (strcmp(rbsno,"S231") == 0) return "S231-AUTH NUMNER IS NOT RIGHT";
	else if (strcmp(rbsno,"S232") == 0) return "S232-PLEASE USE 4001";
	else if (strcmp(rbsno,"S233") == 0) return "S233-THE TRANSACTION IS NOT EXIST";
	else if (strcmp(rbsno,"S234") == 0) return "S234-THE AUTH IS NOT ACT";
	else if (strcmp(rbsno,"S235") == 0) return "S235-CDP STATUS ERROR";
	else if (strcmp(rbsno,"S236") == 0) return "S236-CDP HAS USED";
	else if (strcmp(rbsno,"S237") == 0) return "S237-LAST ACT-NO";
	else if (strcmp(rbsno,"S223") == 0) return "S222-SYSTEM ERROR";
	else if (strcmp(rbsno,"S240") == 0) return "S240-THE CHECK NOT BELONG TO THE ACT";
	else if (strcmp(rbsno,"S241") == 0) return "S241-THE CHECK HAS STOP PAY";
	else if (strcmp(rbsno,"S242") == 0) return "S242-THE CHECK SALED";
	else if (strcmp(rbsno,"S243") == 0) return "S243-THE CHECK ALREADY RELEASE";
	else if (strcmp(rbsno,"S244") == 0) return "S244-THE CHECK ALREADY WITHDRAWN";
	else if (strcmp(rbsno,"S245") == 0) return "S245-THE CHECK ALREADY TAKEN BACK ";
	else if (strcmp(rbsno,"S246") == 0) return "S246-SIGN DATE GREAT 10 DAYS";
	else if (strcmp(rbsno,"S247") == 0) return "S247-TXN-AMT ERROR";
	else if (strcmp(rbsno,"S248") == 0) return "S248-THE CHEQUE IS NOT STOP-PAYED";
	else if (strcmp(rbsno,"S249") == 0) return "S249-THE RECORD IS NOT FOUND";
	else if (strcmp(rbsno,"S250") == 0) return "S250-MUST INPUT THE CHEQUE-NO";
	else if (strcmp(rbsno,"S251") == 0) return "S251-";
	else if (strcmp(rbsno,"S252") == 0) return "S252-CHECK TXN AMT <100";
	else if (strcmp(rbsno,"S253") == 0) return "S253-    ";
	else if (strcmp(rbsno,"S254") == 0) return "S254-    ";
	else if (strcmp(rbsno,"S255") == 0) return "S255-    ";
	else if (strcmp(rbsno,"S256") == 0) return "S256-    ";
	else if (strcmp(rbsno,"S257") == 0) return "S257-    ";
	else if (strcmp(rbsno,"S258") == 0) return "S258-    ";
	else if (strcmp(rbsno,"S259") == 0) return "S259-THE INFORMATION OF THIS CUSTOMER NOT EXIST ";
	else if (strcmp(rbsno,"S261") == 0) return "S261-IDNO NO MATCH ";
	else if (strcmp(rbsno,"S262") == 0) return "S262-AMT < 100000,CAN NOT USE SPECIAL CT";
	else if (strcmp(rbsno,"S263") == 0) return "S263-AMT NOT  < 100000, MUST USE SPECIAL CT";
	else if (strcmp(rbsno,"S300") == 0) return "S300-WITHDRAW-TIMES WRONG  !";
	else if (strcmp(rbsno,"S301") == 0) return "S301-TRANS-ID  MUST BE 0-2 ";
	else if (strcmp(rbsno,"S302") == 0) return "S302-AUTO-TRAN, JOB-CODE MUST BE  10  OR 40  ";
	else if (strcmp(rbsno,"S303") == 0) return "S303-AUTO-TRAN-INT, JOB-CODE MUST BE  10 ";
	else if (strcmp(rbsno,"S304") == 0) return "S304-TRANSF-ACT-NO  WRONG ";
	else if (strcmp(rbsno,"S305") == 0) return "S305-PERIOD MONTH MUST >=  3";
	else if (strcmp(rbsno,"S306") == 0) return "S306-PERIOD MONTH INVALID";
	else if (strcmp(rbsno,"S307") == 0) return "S307-PLEASE INPUT TRANSF-ACT  !";
	else if (strcmp(rbsno,"S308") == 0) return "S308-WIDRAW INT METHOD IS 1-2 !";
	else if (strcmp(rbsno,"S309") == 0) return "S309-WIDRAW INT PERIOD WRONG  !";
	else if (strcmp(rbsno,"S310") == 0) return "S310-WIDRAW INT PERIOD MUST BE ZERO";
	else if (strcmp(rbsno,"S311") == 0) return "S311-WITHDRAW-INT-CODE IS INT-END !";
	else if (strcmp(rbsno,"S312") == 0) return "S312-NO INT TO  WITHDRAW !";
	else if (strcmp(rbsno,"S313") == 0) return "S313-INVAID JOB CODE !!";
	else if (strcmp(rbsno,"S314") == 0) return "S314-DEPOSIT AMT ERROR   !!";
	else if (strcmp(rbsno,"S315") == 0) return "S315-PLS INPUT CT-NO   !!";
	else if (strcmp(rbsno,"S316") == 0) return "S316-THIS ACCT. IS NOT ACCT. OF TARGET";
	else if (strcmp(rbsno,"S317") == 0) return "S317-PLS. AUTHORIZE      !!";
	else if (strcmp(rbsno,"S318") == 0) return "S318-NOT FOUND IN FDFMSTM !";
	else if (strcmp(rbsno,"S319") == 0) return "S319-READ FDFMSTM ERROR  !!";
	else if (strcmp(rbsno,"S320") == 0) return "S320-NOT FOUND IN FDFDTLT !";
	else if (strcmp(rbsno,"S321") == 0) return "S321-READ FDFDTLT ERROR  !!";
	else if (strcmp(rbsno,"S322") == 0) return "S322-PLEASE CLOSE YOUR ACT  !";
	else if (strcmp(rbsno,"S323") == 0) return "S323-AHEAD WITHDRAW      !!";
	else if (strcmp(rbsno,"S324") == 0) return "S324-NORMAL    CLOSE     !!";
	else if (strcmp(rbsno,"S325") == 0) return "S325-PLS. CHECK STATUS OF THIS ACCT.!";
	else if (strcmp(rbsno,"S326") == 0) return "S326-DATE OF CLOSING CAN NOT LESS  DATE OF OPENNING";
	else if (strcmp(rbsno,"S327") == 0) return "S327-INPUT DATE WRONG !!";
	else if (strcmp(rbsno,"S328") == 0) return "S328-CERT ALREADY CONVERTED";
	else if (strcmp(rbsno,"S329") == 0) return "S329-ALREADY CLOSED !!";
	else if (strcmp(rbsno,"S330") == 0) return "S330-AHEAD PART-WITHDRAW !!";
	else if (strcmp(rbsno,"S331") == 0) return "S331-ALREADY STOP-PAY OR DISABLE";
	else if (strcmp(rbsno,"S332") == 0) return "S332-LOU-COUNT1 NOT ONE TIME ,DONT DO IT AGAIN!!!";
	else if (strcmp(rbsno,"S333") == 0) return "S333-CMOSDATE  ERROR  !  ";
	else if (strcmp(rbsno,"S334") == 0) return "S334-AMT AND CNT NOT  ZEROS !  ";
	else if (strcmp(rbsno,"S335") == 0) return "S335-THIS TRAN IS NON-AMT-TRAN , CAN NOT REVERSE";
	else if (strcmp(rbsno,"S336") == 0) return "S336-OPEN NOT ONLINE-OFF";
	else if (strcmp(rbsno,"S337") == 0) return "S337-AMT < MIN-AMT";
	else if (strcmp(rbsno,"S338") == 0) return "S338-AMT < MONTH-AMT";
	else if (strcmp(rbsno,"S339") == 0) return "S339-BEG-INT-DATE ERR";
	else if (strcmp(rbsno,"S340") == 0) return "S340-BEG-INT MUST TRANSF";
	else if (strcmp(rbsno,"S341") == 0) return "S341-MUST HAVE SUPPERVISOR !";
	else if (strcmp(rbsno,"S342") == 0) return "S342-TRANSF ACNO SATUS ERROR";
	else if (strcmp(rbsno,"S343") == 0) return "S343-ACNO EXIT, NOT OPEN";
	else if (strcmp(rbsno,"S344") == 0) return "S344-ACNO NOT EXIT, CAN NOT REVERSE";
	else if (strcmp(rbsno,"S345") == 0) return "S345-ACNO SATUS ERROR";
	else if (strcmp(rbsno,"S346") == 0) return "S346-TRANSF ACNO NOT EXIT";
	else if (strcmp(rbsno,"S347") == 0) return "S347-MUST INPUT NEW PASSBOOK-NO";
	else if (strcmp(rbsno,"S348") == 0) return "S348-MUST HAVE OLD PASSBOOK-NO";
	else if (strcmp(rbsno,"S349") == 0) return "S349-REISSUE TYPE MUST MB 1-3";
	else if (strcmp(rbsno,"S350") == 0) return "S350-INT-AMT-TYPE ERROR !";
	else if (strcmp(rbsno,"S351") == 0) return "S351-YI ZHONG HUI HU CAN NOT DO 3091";
	else if (strcmp(rbsno,"S352") == 0) return "S352-STOP PAY! CAN NOT WITHDRAW !";
	else if (strcmp(rbsno,"S353") == 0) return "S353-PLS FIRST DO 3300 TRAN.";
	else if (strcmp(rbsno,"S354") == 0) return "S354-PLS FIRST DO 3095 TRAN.";
	else if (strcmp(rbsno,"S355") == 0) return "S355-THIS ACT-NO IS NOT FOR PASSBOOK";
	else if (strcmp(rbsno,"S356") == 0) return "S356-FDTL-TRANS-NO IS CHANGE TO NEXT!";
	else if (strcmp(rbsno,"S357") == 0) return "S357-DUCUMENT NOT HAS STOP ,CAN NOT GIVE NEW ONE";
	else if (strcmp(rbsno,"S358") == 0) return "S358-PLEASE CHECK TXNAMT AND FIXAMT !";
	else if (strcmp(rbsno,"S359") == 0) return "S359-SEARCH RATE NOT NORMAL !";
	else if (strcmp(rbsno,"S360") == 0) return "S360-NEW  CT-NO  = OLD  CT-NO  !";
	else if (strcmp(rbsno,"S361") == 0) return "S361-FDFDTLT-FILE HAS PROBLEM";
	else if (strcmp(rbsno,"S362") == 0) return "S362-WITHDRAW-AMT + NEW-CT-AMT NOT = OLD-CT-AMT!";
	else if (strcmp(rbsno,"S363") == 0) return "S363-CAN NOT DO UNDUE-WIDRAW-TXN AGAIN !";
	else if (strcmp(rbsno,"S364") == 0) return "S364-NEW CT , OR HAS TRANSFERED .  !";
	else if (strcmp(rbsno,"S365") == 0) return "S365-ONLY RMB CAN OPEN LING ZHENG !";
	else if (strcmp(rbsno,"S366") == 0) return "S366-ACT  NEED  REBOOK  !";
	else if (strcmp(rbsno,"S367") == 0) return "S367-DUCUMENT IS NOT NORMAL CANT GIVE NEW";
	else if (strcmp(rbsno,"S368") == 0) return "S368-DUCUMENT IS NOT CLOSED CANT GIVE NEW";
	else if (strcmp(rbsno,"S369") == 0) return "S369-TODAY OPEN CANT DO THIS TXN.  !";
	else if (strcmp(rbsno,"S370") == 0) return "S370-WIDRAW-INT-METHOD MUST BE 1 OR 2";
	else if (strcmp(rbsno,"S371") == 0) return "S371-INVAID CUU-CODE";
	else if (strcmp(rbsno,"S372") == 0) return "S372-INVAID JOB-CODE";
	else if (strcmp(rbsno,"S373") == 0) return "S373-INVAID CHAR-CODE";
	else if (strcmp(rbsno,"S374") == 0) return "S374-FORG CUU-CODE NOT MATCH JOB-CODE";
	else if (strcmp(rbsno,"S375") == 0) return "S375-FORG CUU-CODE NOT MATCH CHAR-CODE";
	else if (strcmp(rbsno,"S376") == 0) return "S376-RMB  CUU-CODE NOT MATCH CHAR-CODE";
	else if (strcmp(rbsno,"S377") == 0) return "S377-PLEASE CHECK CT-AMT   ";
	else if (strcmp(rbsno,"S378") == 0) return "S378-3092 CAN NOT DO UNDUE-WIDRAW-TXN ! ";
	else if (strcmp(rbsno,"S379") == 0) return "S379-CHANGE/GIVE CERT OR PASSBOOK ";
	else if (strcmp(rbsno,"S380") == 0) return "S380-THIS CT NOT OPEN ";
	else if (strcmp(rbsno,"S381") == 0) return "S381-THIS CT SATUS ERROR ";
	else if (strcmp(rbsno,"S382") == 0) return "S382-THIS CT HAVE NOT STOP ";
	else if (strcmp(rbsno,"S383") == 0) return "S383-THIS CT HAVE NOT STOP, PLS CHK";
	else if (strcmp(rbsno,"S384") == 0) return "S384-TRANS-CD CANNOT BE 1-2 WHEN JOB-CODE=40 OR 41";
	else if (strcmp(rbsno,"S385") == 0) return "S385-WIDRAW-INT-METHOD MUST BE 2 IN THIS CASE";
	else if (strcmp(rbsno,"S386") == 0) return "S386-WIDRAW-INT-METHOD MUST BE 1 IN THIS CASE";
	else if (strcmp(rbsno,"S387") == 0) return "S387-CT DUE-TO-DATE, DO 3090 OR 3092 TXN !";
	else if (strcmp(rbsno,"S388") == 0) return "S388-FDFTLT ACT-STATUS NOT NORMAL !";
	else if (strcmp(rbsno,"S389") == 0) return "S389-ACT-NO NOT FIND IN FDFSTPT  !";
	else if (strcmp(rbsno,"S390") == 0) return "S390-THIS PASSBOOK ALREADY DUE  !";
	else if (strcmp(rbsno,"S391") == 0) return "S391-DEPOSIT PERIOD  WRONG  FOR COMMON-CUSTOMER";
	else if (strcmp(rbsno,"S392") == 0) return "S392-YI ZHONG HUI HU TRANSF, CHAR-CODE MUST = ";
	else if (strcmp(rbsno,"S393") == 0) return "S393-COMPUTE INTER IS WRONG !";
	else if (strcmp(rbsno,"S394") == 0) return "S394-ELO AND ADD-FLAG WRONG !";
	else if (strcmp(rbsno,"S395") == 0) return "S395-YI ZHONG HUI HU QU XIAN,INCLUDE CT-AMT INT-AMT";
	else if (strcmp(rbsno,"S396") == 0) return "S396-PLEASE CHECK TOTAL-DEPOST!";
	else if (strcmp(rbsno,"S397") == 0) return "S397-LZ ONLY ELO ONE TIMES  !";
	else if (strcmp(rbsno,"S398") == 0) return "S398-NO LOU CUN CANOT BU CUN !";
	else if (strcmp(rbsno,"S399") == 0) return "S399-LZ ELO ONLY ONE TIME AND NO ELO BEFORE!";
	else if (strcmp(rbsno,"S400") == 0) return "S400-DEPOSIT-TYPE ERROR!";
	else if (strcmp(rbsno,"S401") == 0) return "S401-LZ ADD ONLY ONE TIME AND NO ADD BEFORE!";
	else if (strcmp(rbsno,"S402") == 0) return "S402-LZ ELO/ADD TXN-AMT MUST >2*MON-AMT< 3*MON AMT!";
	else if (strcmp(rbsno,"S403") == 0) return "S403-LZ ELO/ADD TXN AMT < 2*MONTH-AMT !";
	else if (strcmp(rbsno,"S404") == 0) return "S404-LZ CUN RU TXN-AMT MUST = MONTH-AMT!";
	else if (strcmp(rbsno,"S405") == 0) return "S405-ALREADY CUNRU COMMER THIS MON NOT CUNRU AGAIN!";
	else if (strcmp(rbsno,"S406") == 0) return "S406-INPUT AMT NOT = BALANCE !";
	else if (strcmp(rbsno,"S407") == 0) return "S407-CHECK THE DATE   ";
	else if (strcmp(rbsno,"S408") == 0) return "S408-CHECK THE RATE   ";
	else if (strcmp(rbsno,"S409") == 0) return "S409-CHECK DB  OR   CR";
	else if (strcmp(rbsno,"S410") == 0) return "S410-CT HAS  DELETED  ";
	else if (strcmp(rbsno,"S411") == 0) return "S411-CT HAS  NOT DELETED  ";
	else if (strcmp(rbsno,"S412") == 0) return "S412-CHECK THE PERIOD-MONTH";
	else if (strcmp(rbsno,"S413") == 0) return "S413-NO SUCH CUU BRIEF";
	else if (strcmp(rbsno,"S414") == 0) return "S414-INPUT ACCITEM AMT TYPE MUST 02";
	else if (strcmp(rbsno,"S415") == 0) return "S415-INPUT CUU MUST BE THE SAME AS THE CT";
	else if (strcmp(rbsno,"S416") == 0) return "S416-CT IS DUE PLEASE DO (4090) OR (4092) TXN";
	else if (strcmp(rbsno,"S417") == 0) return "S417-(4092) CAN NOT DO PREWITHDRAW";
	else if (strcmp(rbsno,"S418") == 0) return "S418-THE NUL ACT NUMBER ERROR";
	else if (strcmp(rbsno,"S419") == 0) return "S419-THE NUL ACT NUMBER > NORMAL";
	else if (strcmp(rbsno,"S420") == 0) return "S420-DEPOSIT-TYPE FOR FOREIGN CUU ERROR!";
	else if (strcmp(rbsno,"S421") == 0) return "S421-RBS-ATMP PROCESS PROGRAM  ERROR!";
	else if (strcmp(rbsno,"S422") == 0) return "S422-PERIOD FOR DEPOSIT-TYPE-2 ERROR!";
	else if (strcmp(rbsno,"S423") == 0) return "S423-PERIOD FOR DEPOSIT-TYPE-3 ERROR!";
	else if (strcmp(rbsno,"S424") == 0) return "S424-PERIOD FOR DEPOSIT-TYPE-4 ERROR!";
	else if (strcmp(rbsno,"S425") == 0) return "S425-WHEN DEPOSIT-TYPE-2,CANNOT BAL-INT-AUTO-TRANS!";
	else if (strcmp(rbsno,"S426") == 0) return "S426-WHEN PASSBOOK STOPPED,CANNOT RELEASE ONE CT!";
	else if (strcmp(rbsno,"S427") == 0) return "S427-CANNOT MODIFY ID-NO!";
	else if (strcmp(rbsno,"S428") == 0) return "S428-CANNOT HAND OVER CASH FOR OTHERS!";
	else if (strcmp(rbsno,"S429") == 0) return "S429-PLEASE POST PASSBOOK";
	else if (strcmp(rbsno,"S430") == 0) return "S430-SHOULD NOT BE EUR-CUU";
	else if (strcmp(rbsno,"S431") == 0) return "S431-SHOULD BE EUR-CUU";
	else if (strcmp(rbsno,"S433") == 0) return "S433-MUST INPUT RELEASE NO";
	else if (strcmp(rbsno,"S434") == 0) return "S434-PART-STOP MUST INPUT CUU/CHAR";
	else if (strcmp(rbsno,"S435") == 0) return "S435-ALREADY RELEASE";
	else if (strcmp(rbsno,"S450") == 0) return "S450-PLEASE REPLACE THE PASSBOOK";
	else if (strcmp(rbsno,"S451") == 0) return "S451- CT-NO NOT > 60 NOT REPLACE PASSBOOK FOR #1";
	else if (strcmp(rbsno,"S452") == 0) return "S452- CT-NO > 60 REPLACE PASSBOOK FOR #1";
	else if (strcmp(rbsno,"S460") == 0) return "S460-HOT CARD!!!";
	else if (strcmp(rbsno,"S461") == 0) return "S461-INVALID DATE!";
	else if (strcmp(rbsno,"S463") == 0) return "S463-CONNECT WHITH CARD ISSU BANK!!";
	else if (strcmp(rbsno,"S469") == 0) return "S469-INVALID BUSSINO!!";
	else if (strcmp(rbsno,"S470") == 0) return "S470-INVALID AMOUNT!!";
	else if (strcmp(rbsno,"S471") == 0) return "S471-INVALID CARD NO!";
	else if (strcmp(rbsno,"S472") == 0) return "S472- CARD MARK ERROR";
	else if (strcmp(rbsno,"S476") == 0) return "S476-NOT ENOUGH BALANCE!";
	else if (strcmp(rbsno,"S483") == 0) return "S483-OVER WITHDRAW TIMES!!";
	else if (strcmp(rbsno,"S490") == 0) return "S490-CARD /ACCOUNT STATUS NOT RIGHT!!";
	else if (strcmp(rbsno,"S491") == 0) return "S491-AUTH NO ERROR!!";
	else if (strcmp(rbsno,"S492") == 0) return "S492-CARD SYSTEM CUT OFF!";
	else if (strcmp(rbsno,"S493") == 0) return "S493-NOT HAVE THIS TRANSACTION!!";
	else if (strcmp(rbsno,"S494") == 0) return "S494-AUTH HAVENT ANSWER!!";
	else if (strcmp(rbsno,"S495") == 0) return "S495-INVALID TRANSFER DATA!!";
	else if (strcmp(rbsno,"S496") == 0) return "S496-AS/400 SERVICE ERROR !!";
	else if (strcmp(rbsno,"S497") == 0) return "S497-STREAM ERROR!!";
	else if (strcmp(rbsno,"S498") == 0) return "S498 SYSTEM ERRO!";
	else if (strcmp(rbsno,"S499") == 0) return "S499- AMT<USD 20000 CAN NOT DO PFXC TXN";
	else if (strcmp(rbsno,"S501") == 0) return "S501-";
	else if (strcmp(rbsno,"S502") == 0) return "S502-";
	else if (strcmp(rbsno,"S503") == 0) return "S503-";
	else if (strcmp(rbsno,"S504") == 0) return "S504-CAN NOT GREAT 4 !";
	else if (strcmp(rbsno,"S506") == 0) return "S506-INVALID CARD NO!";
	else if (strcmp(rbsno,"S510") == 0) return "S510-FEN!";
	else if (strcmp(rbsno,"S511") == 0) return "S511-FEN!";
	else if (strcmp(rbsno,"S512") == 0) return "S512-FEN!";
	else if (strcmp(rbsno,"S513") == 0) return "S513-THIS ACT ALREADY LOAN";
	else if (strcmp(rbsno,"S514") == 0) return "S514-NOT MAIN ACT,NOT ALLOW LOAN";
	else if (strcmp(rbsno,"S515") == 0) return "S515-CHINESE MUST 20 21,FOREIGN MUST 10 11";
	else if (strcmp(rbsno,"S516") == 0) return "S516-TOTAL BALANCE LESS THAN LOAN AMOUNT TOTAL";
	else if (strcmp(rbsno,"S520") == 0) return "S520-STOCT SYSTEM IS ERROR";
	else if (strcmp(rbsno,"S521") == 0) return "S521-STOCT SYSTEM IS BUSSY";
	else if (strcmp(rbsno,"S522") == 0) return "S522-STOC SDN NOT EXIST";
	else if (strcmp(rbsno,"S523") == 0) return "S523-BILL HAS PRINTED";
	else if (strcmp(rbsno,"S524") == 0) return "S524-LICENSE CAN NOT DOUBLE";
	else if (strcmp(rbsno,"S525") == 0) return "S525-LICENSE NOT RIGHT";
	else if (strcmp(rbsno,"S526") == 0) return "S526-NO LUQIAO CARD";
	else if (strcmp(rbsno,"S527") == 0) return "S527-COMPANY A/C NOT EXIST";
	else if (strcmp(rbsno,"S528") == 0) return "S528-PRIVATE A/C NOT EXIST";
	else if (strcmp(rbsno,"S529") == 0) return "S529-LUQIAO CIF CAN NOT MODIY IF NOT OPEN DAY";
	else if (strcmp(rbsno,"S530") == 0) return "S530-TBS-PSW MUST INPUT ENOUGH!";
	else if (strcmp(rbsno,"S531") == 0) return "S531-CHECK ID-NO ERROR!";
	else if (strcmp(rbsno,"S532") == 0) return "S532-NO ID-NO IN CIFMSTM!";
	else if (strcmp(rbsno,"S533") == 0) return "S533-ALREADY OPEN!";
	else if (strcmp(rbsno,"S534") == 0) return "S534-NO OPEN!";
	else if (strcmp(rbsno,"S535") == 0) return "S535-NO REPORT THE LOSS!";
	else if (strcmp(rbsno,"S536") == 0) return "S536-REPORT THE LOSS LESS ONE WEEK!";
	else if (strcmp(rbsno,"S537") == 0) return "S537-OTHERS!";
	else if (strcmp(rbsno,"S540") == 0) return "S540-TBS-DO NOT HAVE THIS UNIT CODE !";
	else if (strcmp(rbsno,"S541") == 0) return "S541-TBS-DO NOT HAVE THIS USER CODE !";
	else if (strcmp(rbsno,"S542") == 0) return "S542-TBS-AMT NOT MATCH !";
	else if (strcmp(rbsno,"S543") == 0) return "S543-TBS-LATE FEE NOT MATCH !";
	else if (strcmp(rbsno,"S544") == 0) return "S544-TBS-OUT OF PAY DATE !";
	else if (strcmp(rbsno,"S545") == 0) return "S545-TBS-ALREADY PAY FOR IT !";
	else if (strcmp(rbsno,"S546") == 0) return "S546-TBS-ALREADY PRINT RECEIPT !";
	else if (strcmp(rbsno,"S547") == 0) return "S547-TBS-TBS SYSTEM ERROR !";
	else if (strcmp(rbsno,"S548") == 0) return "S548-TBS-HAS NOT PAY !";
	else if (strcmp(rbsno,"S551") == 0) return "S551-SYSTEN ERROR";
	else if (strcmp(rbsno,"S552") == 0) return "S552-STOCK IS NULL";
	else if (strcmp(rbsno,"S553") == 0) return "S553-LENGTH OF ID IS ERROR";
	else if (strcmp(rbsno,"S554") == 0) return "S554-LENGTH OF STOCK ACCOUNT IS ERROR";
	else if (strcmp(rbsno,"S555") == 0) return "S555-STOCK ACCOUNT IS OPEN";
	else if (strcmp(rbsno,"S556") == 0) return "S556-STATUS OF FUND IS ERROR";
	else if (strcmp(rbsno,"S557") == 0) return "S557-ID IS ERROR";
	else if (strcmp(rbsno,"S558") == 0) return "S558-ACCOUNT IS NOT OPEN YQT";
	else if (strcmp(rbsno,"S559") == 0) return "S559-OTHER ERROR";
	else if (strcmp(rbsno,"S560") == 0) return "S560-SYSTEM IS OUT";
	else if (strcmp(rbsno,"S561") == 0) return "S561-STOCK HAS NO XW NO";
	else if (strcmp(rbsno,"S562") == 0) return "S562-OPEN STOCK ACCOUNT ERROR";
	else if (strcmp(rbsno,"S563") == 0) return "S563-CLOSE ACCOUNT ERROR";
	else if (strcmp(rbsno,"S564") == 0) return "S564-INVALID     CODE";
	else if (strcmp(rbsno,"S565") == 0) return "S565-NOT WITHDRAW THE POINT";
	else if (strcmp(rbsno,"S566") == 0) return "S566-STATUS OF CHANGE STOCK HOLDER";
	else if (strcmp(rbsno,"S567") == 0) return "S567-STOCK ACCOUNT IS STOP";
	else if (strcmp(rbsno,"S568") == 0) return "S568-STOCK ACCOUNT IS NOT STOP";
	else if (strcmp(rbsno,"S569") == 0) return "S569-NOT IN THE ACCURATE TIME";
	else if (strcmp(rbsno,"S570") == 0) return "S570-POINT OR WITHDRAW POINT TXN IS ERROR";
	else if (strcmp(rbsno,"S571") == 0) return "S571-CHANGE TRUSTEESHIP IS ERROR";
	else if (strcmp(rbsno,"S572") == 0) return "S572-STATUS OF STOCK ACCOUNT IS ERROR";
	else if (strcmp(rbsno,"S573") == 0) return "S573-BAL OF AVAILABLE STOCK IS NOT ENOUGH";
	else if (strcmp(rbsno,"S574") == 0) return "S574-NO STOCK CODE      ";
	else if (strcmp(rbsno,"S575") == 0) return "S575-CLIENT HAS OPEN STOCK ";
	else if (strcmp(rbsno,"S576") == 0) return "S576-COUNTRY CODE ERROR!   ";
	else if (strcmp(rbsno,"S577") == 0) return "S577-WILL CREATE LOAN  !   ";
	else if (strcmp(rbsno,"S578") == 0) return "S578-ALLIED CARD NOT ALLOWED WITHDRAW IN CASH !";
	else if (strcmp(rbsno,"S600") == 0) return "S600-TAX COUNTRY CODE ERROR!";
	else if (strcmp(rbsno,"S601") == 0) return "S601-COMM WITH RS6000 ERROR!";
	else if (strcmp(rbsno,"S602") == 0) return "S602-INQIURY OPEN CARD BANK!";
	else if (strcmp(rbsno,"S603") == 0) return "S603-NO CREDIT!";
	else if (strcmp(rbsno,"S604") == 0) return "S604-INVALID INFORMATION!";
	else if (strcmp(rbsno,"S605") == 0) return "S605-ACTION IS NOT START!";
	else if (strcmp(rbsno,"S606") == 0) return "S606-CAN NOT UPDATE FILE!";
	else if (strcmp(rbsno,"S607") == 0) return "S607-INVALID ACCEPT ORGID!";
	else if (strcmp(rbsno,"S608") == 0) return "S608-THE TIME OF INPUT PIN IS MORE!";
	else if (strcmp(rbsno,"S609") == 0) return "S609-ERROR FUNCTION!";
	else if (strcmp(rbsno,"S610") == 0) return "S610-THE CARD HAS LOST!";
	else if (strcmp(rbsno,"S611") == 0) return "S611-PIN INPUT ERROR!";
	else if (strcmp(rbsno,"S612") == 0) return "S612-AMT > 20000 AND HAVE DUE CAN NOT  PART WITHDRAW!";
	else if (strcmp(rbsno,"S613") == 0) return "S613-CARD IS NOT IN FILE!";
	else if (strcmp(rbsno,"S614") == 0) return "S614-CARD IS LIMITED!";
	else if (strcmp(rbsno,"S615") == 0) return "S615-INVALID MAC!";
	else if (strcmp(rbsno,"S616") == 0) return "S616-INVALID INIT AMT!";
	else if (strcmp(rbsno,"S617") == 0) return "S617-UNCONFIRMED TXN EXISTS,CAN NOT CANCEL ACCOUNT!";
	else if (strcmp(rbsno,"S618") == 0) return "S618-FUND IS NOT OPEN!";
	else if (strcmp(rbsno,"S619") == 0) return "S619-FUND CODE NOT EXIST!";
	else if (strcmp(rbsno,"S620") == 0) return "S620-THE LOSS OF ACCOUNT HAS BEEN REPORTED!";
	else if (strcmp(rbsno,"S621") == 0) return "S621-FUND NOT EXIST IN ACCOUNT!";
	else if (strcmp(rbsno,"S622") == 0) return "S622-NOT FIND FROZEN OR IMPAWNED FUND!";
	else if (strcmp(rbsno,"S623") == 0) return "S623-ACCOUNT NOT PERFORMED APPOINTED TXN!";
	else if (strcmp(rbsno,"S624") == 0) return "S624-TXN NOT EXIST!";
	else if (strcmp(rbsno,"S625") == 0) return "S625-TXN IS NOT CONFIRMED!";
	else if (strcmp(rbsno,"S626") == 0) return "S626-PERSON ID AND DEBT CARD NUMBER NOT EXIST!";
	else if (strcmp(rbsno,"S627") == 0) return "S627-TXN CAN NOT CANEL BILL!";
	else if (strcmp(rbsno,"S628") == 0) return "S628-TXN OUT TIME!";
	else if (strcmp(rbsno,"S629") == 0) return "S629-NOT FIND ISSUE CARD BANK!";
	else if (strcmp(rbsno,"S630") == 0) return "S630-TXN MAY BE REPEAT!";
	else if (strcmp(rbsno,"S631") == 0) return "S631-IN CHANGE ERROR!";
	else if (strcmp(rbsno,"S632") == 0) return "S632-PASSWORD CHANGEING IS DOING!";
	else if (strcmp(rbsno,"S633") == 0) return "S633-CHANGE CENTER FILL START BANK STOP!";
	else if (strcmp(rbsno,"S634") == 0) return "S634-TXN IS NOT MAICH!";
	else if (strcmp(rbsno,"S635") == 0) return "S635-TXN IS NOT MAICH,BUT VALID!";
	else if (strcmp(rbsno,"S636") == 0) return "S636-VALID MONTH IS NOT MAICH!";
	else if (strcmp(rbsno,"S637") == 0) return "S637-CVV!";
	else if (strcmp(rbsno,"S638") == 0) return "S638-FUND ACCOUNT IS NOT FOUND!";
	else if (strcmp(rbsno,"S639") == 0) return "S639-PASSWORD IS NOT CORRECT!";
	else if (strcmp(rbsno,"S640") == 0) return "S640-AMOUNT OR FACE IS TOO LOW !";
	else if (strcmp(rbsno,"S641") == 0) return "S641-DB SYSTEM NOT START!(DB QUEUE ERR)";
	else if (strcmp(rbsno,"S642") == 0) return "S642-REVERSE DATE-TIME ERR!";
	else if (strcmp(rbsno,"S643") == 0) return "S643-REVERSE ACQ-INST-ID ERR!";
	else if (strcmp(rbsno,"S644") == 0) return "S644-NO FUND CHANGING TXN !";
	else if (strcmp(rbsno,"S645") == 0) return "S645-TXN NOT PERMIT !";
	else if (strcmp(rbsno,"S646") == 0) return "S646-NO FUND FREEZE  TXN !";
	else if (strcmp(rbsno,"S647") == 0) return "S647-AMOUNT OR FACE IS TOO HIGH !";
	else if (strcmp(rbsno,"S648") == 0) return "S648-AMOUNT OR FACE IS TOO LOW !";
	else if (strcmp(rbsno,"S649") == 0) return "S649-NO FUND UNFREEZE TXN !";
	else if (strcmp(rbsno,"S650") == 0) return "S650-TBS HAVE OPEN !";
	else if (strcmp(rbsno,"S651") == 0) return "S651-TBS HAVE NOT OPEN !";
	else if (strcmp(rbsno,"S652") == 0) return "S652-CARD TYPE MUST BE 0 OR 1 !";
	else if (strcmp(rbsno,"S653") == 0) return "S653-THE CARD HAVE ISSUM !";
	else if (strcmp(rbsno,"S654") == 0) return "S654-WRITE CIFMSTM ERROR !";
	else if (strcmp(rbsno,"S655") == 0) return "S655-APLLY CARD NUMBER MUST < 50 !";
	else if (strcmp(rbsno,"S656") == 0) return "S656-CARD STATUS IS NOT NORMAL !";
	else if (strcmp(rbsno,"S657") == 0) return "S657-THE CARD HAVE BE STOP PAY !";
	else if (strcmp(rbsno,"S658") == 0) return "S658-THE CARD IS BLACK !";
	else if (strcmp(rbsno,"S659") == 0) return "S659-CARD SEQ ERROR !";
	else if (strcmp(rbsno,"S660") == 0) return "S660-ACT NO BOOK STATUS ERROR !";
	else if (strcmp(rbsno,"S661") == 0) return "S661-CLOSE SV BY CARD MUST BE NOBOOK ACT !";
	else if (strcmp(rbsno,"S662") == 0) return "S662-THE ACT HAS ONE VALID DB CARD LINKED !";
	else if (strcmp(rbsno,"S663") == 0) return "S663-THE ACT HAVE BE DELETED !";
	else if (strcmp(rbsno,"S664") == 0) return "S664-THE ACT NOT MATCH !";
	else if (strcmp(rbsno,"S665") == 0) return "S665-PLEASE USE MAIN CARD!";
	else if (strcmp(rbsno,"S666") == 0) return "S666-MUST BE INTERNATIONAL DB CARD !";
	else if (strcmp(rbsno,"S667") == 0) return "S667-TX DATE IS GREATER THAN EXPIRE DATE !";
	else if (strcmp(rbsno,"S668") == 0) return "S668-THIS TX CAN NOT SETTLE BASIC ACCOUNT !";
	else if (strcmp(rbsno,"S669") == 0) return "S669-NO  FUND BNSMOD CHOOSE !";
	else if (strcmp(rbsno,"S670") == 0) return "S670-FUND AMMAUNT IS TOO LITTLE!";
	else if (strcmp(rbsno,"S671") == 0) return "S671-HAVE AVALUE BOND , CAN NOT CLOSE !";
	else if (strcmp(rbsno,"S672") == 0) return "S672-ID NO ERROR !";
	else if (strcmp(rbsno,"S673") == 0) return "S673-ACT STATUS NOT NORMAL, REFUSE TXN !";
	else if (strcmp(rbsno,"S674") == 0) return "S674-AVALUE BAL NOT ENOUGH !";
	else if (strcmp(rbsno,"S675") == 0) return "S675-ACT HAS CLOSED !";
	else if (strcmp(rbsno,"S676") == 0) return "S676-ACT NOT LOST , CAN NOT RELEASE !";
	else if (strcmp(rbsno,"S677") == 0) return "S677-ACT NOT STOP , CAN NOT RELEASE !";
	else if (strcmp(rbsno,"S678") == 0) return "S678-NOT FOUND STOP RECORD !";
	else if (strcmp(rbsno,"S679") == 0) return "S679-FUND ACT ERROR , REFUSE TXN !";
	else if (strcmp(rbsno,"S680") == 0) return "S680-POSITION NOT ENOUGH !";
	else if (strcmp(rbsno,"S681") == 0) return "S681-ACT EMPUTY !";
	else if (strcmp(rbsno,"S682") == 0) return "S682-THE BOND IS NOT NEW !";
	else if (strcmp(rbsno,"S683") == 0) return "S683-THE DATE IS NOT VALUE FOR TXN !";
	else if (strcmp(rbsno,"S684") == 0) return "S684-BOND RATE ERROR !";
	else if (strcmp(rbsno,"S685") == 0) return "S685-ONE PEPOLE ONLY HAVE ONE ACT !";
	else if (strcmp(rbsno,"S686") == 0) return "S686-THE TXN HAVE EXIT!";
	else if (strcmp(rbsno,"S687") == 0) return "S687-THE TXN HAVE ACCEPT!";
	else if (strcmp(rbsno,"S688") == 0) return "S688-DO NOT TXN IN TODAY!";
	else if (strcmp(rbsno,"S689") == 0) return "S686-TRNSIN BOND ACT ERROR!";
	else if (strcmp(rbsno,"S691") == 0) return "S691-BOND SYSTEM NOT READY !";
	else if (strcmp(rbsno,"S692") == 0) return "S692-BOND SYSTEM TXN CODE ERROR !";
	else if (strcmp(rbsno,"S693") == 0) return "S693-BOND SYSTEM FILE NOT OPEN !";
	else if (strcmp(rbsno,"S694") == 0) return "S694-BOND SYSTEM RECORD NOT FIND !";
	else if (strcmp(rbsno,"S695") == 0) return "S695-BOND SYSTEM CAN NOT REVERSE !";
	else if (strcmp(rbsno,"S696") == 0) return "S696-SDN CAN NOT DO BOND TXN !";
	else if (strcmp(rbsno,"S697") == 0) return "S697-PLEASE USE 6240        !";
	else if (strcmp(rbsno,"S698") == 0) return "S698-FACE NOT ENOUGH        !";
	else if (strcmp(rbsno,"S699") == 0) return "S699-AMT MUST BE COUNTS OF ADD AMT !";
	else if (strcmp(rbsno,"S701") == 0) return "S701-MAIN CARD MUST HAVE ACT !";
	else if (strcmp(rbsno,"S702") == 0) return "S702-SUB CARD MUST NOT HANVE ACT !";
	else if (strcmp(rbsno,"S703") == 0) return "S703-CUU AND AMT MUST BE INPUTED !";
	else if (strcmp(rbsno,"S790") == 0) return "S790-SDN AUTHOR DISMATCH!";
	else if (strcmp(rbsno,"S791") == 0) return "S791-CENTER SDN RECORD NOT EXIST IN FDFBOND!";
	else if (strcmp(rbsno,"S792") == 0) return "S792-AMOUNT LIMIT !";
	else if (strcmp(rbsno,"S793") == 0) return "S793-FENPEI  RECORD EXIST ALREADY!";
	else if (strcmp(rbsno,"S794") == 0) return "S794-GKQ YEAR MUST GREAT 2001!";
	else if (strcmp(rbsno,"S795") == 0) return "S795-TIAOPEI RECORD NOT EXIST!";
	else if (strcmp(rbsno,"S796") == 0) return "S796-AMOUNT IS BOYOND!";
	else if (strcmp(rbsno,"S797") == 0) return "S797-JOB-CODE IS NOT 35,NOT NEED GKQ-RATE-FLAG!";
	else if (strcmp(rbsno,"S801") == 0) return "S801-JOB-CODE MUST BE 10 !";
	else if (strcmp(rbsno,"S802") == 0) return "S802-JOB-CODE MUST BE 10,11 !";
	else if (strcmp(rbsno,"S803") == 0) return "S803-CHAR-CODE MUST BE 00,01 !";
	else if (strcmp(rbsno,"S804") == 0) return "S804-CHAR-CODE MUST BE 10,11,20,21 !";
	else if (strcmp(rbsno,"S805") == 0) return "S805-TXN-AMT MUST = 500,1000,5000,10000 !";
	else if (strcmp(rbsno,"S806") == 0) return "S806-TXN-AMT MUST = 10,20,30,50,100 !";
	else if (strcmp(rbsno,"S807") == 0) return "S807-TXN-AMT MUST > 1000.00 !";
	else if (strcmp(rbsno,"S808") == 0) return "S808-PERIOD-MONTH MUST BE 3,6,12,24 !";
	else if (strcmp(rbsno,"S809") == 0) return "S809-PERIOD-MONTH MUST BE 12,36,60 !";
	else if (strcmp(rbsno,"S810") == 0) return "S810-PERIOD-MONTH MUST BE 36,60 !";
	else if (strcmp(rbsno,"S811") == 0) return "S811-PERIOD-MONTH MUST BE 3,6,9,12,24 !";
	else if (strcmp(rbsno,"S812") == 0) return "S812-PERIOD-MONTH MUST BE 6,12,24 !";
	else if (strcmp(rbsno,"S813") == 0) return "S813-PERIOD-MONTH MUST BE 12 !";
	else if (strcmp(rbsno,"S814") == 0) return "S814-PERIOD-MONTH MUST BE 00 !";
	else if (strcmp(rbsno,"S815") == 0) return "S815-NEW-CT-AMT CAN NOT  =  ZEROS !";
	else if (strcmp(rbsno,"S816") == 0) return "S816-NEW-CT-AMT NOT RIGHT ! ";
	else if (strcmp(rbsno,"S817") == 0) return "S817-FDOSINF-RETURN ERROR ! ";
	else if (strcmp(rbsno,"S818") == 0) return "S818-PLEASE OPEN CT AT YOURSELF BRANCH!";
	else if (strcmp(rbsno,"S819") == 0) return "S819-60 CANT DO AUTO TRANINT !";
	else if (strcmp(rbsno,"S820") == 0) return "S820-ALREADY CHANGE NEW PASSBOOK.";
	else if (strcmp(rbsno,"S821") == 0) return "S821-AMT-TYPE MUST BE 01     !";
	else if (strcmp(rbsno,"S822") == 0) return "S822-JOB-CODE CANNT BE 50 OR 51!";
	else if (strcmp(rbsno,"S824") == 0) return "S824-CHAR-CODE MUST BE 00 OR 01!";
	else if (strcmp(rbsno,"S825") == 0) return "S825-CHAR-CODE MUST BE WAIHUIHU!";
	else if (strcmp(rbsno,"S826") == 0) return "S826-CHAR-CODE MUST BE 10 OR 20!";
	else if (strcmp(rbsno,"S827") == 0) return "S827-CHAR-CODE MUST BE 11 OR 21!";
	else if (strcmp(rbsno,"S828") == 0) return "S828-3095 MUST HAVE PASSBOOK   !";
	else if (strcmp(rbsno,"S829") == 0) return "S829-CUU-CODE MUST BE 00 01    !";
	else if (strcmp(rbsno,"S830") == 0) return "S830-PLEASE CHECK YOUR CT-NO !";
	else if (strcmp(rbsno,"S831") == 0) return "S831-PLEASE CHECK YOUR MEMO  !";
	else if (strcmp(rbsno,"S832") == 0) return "S832-DA E KE ZHUAN RANG CAN NOT UNDUE WITHDRAW!";
	else if (strcmp(rbsno,"S833") == 0) return "S832-DA E KE ZHUAN RANG CAN NOT UNDUE WITHDRAW!";
	else if (strcmp(rbsno,"S834") == 0) return "S834-AMT-TYPE MUST = 02 OR 99!";
	else if (strcmp(rbsno,"S836") == 0) return "S836-NOT-UNACTIVE CAN NOT BE EREASED!";
	else if (strcmp(rbsno,"S837") == 0) return "S837-PLEASE  DO  0000  TO  DELETE  THIS  CT !";
	else if (strcmp(rbsno,"S838") == 0) return "S838-CAN NOT DO  701  ACC-CODE   !";
	else if (strcmp(rbsno,"S839") == 0) return "S839-PERIOD-MONTH  WRONG  !";
	else if (strcmp(rbsno,"S840") == 0) return "S840-ACC-INT  HAVE  NOT  JOB  OR CHAR-CODE !";
	else if (strcmp(rbsno,"S841") == 0) return "S841-AMT > 1000   MUST   HAVE  SUP-NO  !";
	else if (strcmp(rbsno,"S842") == 0) return "S842-IN THIS MONTH ONLY DEPOSIT ONE TIME";
	else if (strcmp(rbsno,"S843") == 0) return "S843-TXN-AMT CANNOT GT THE HALF OF BALANCE";
	else if (strcmp(rbsno,"S850") == 0) return "S850-PERIOD MUST 36";
	else if (strcmp(rbsno,"S851") == 0) return "S851-END DATE < GUOKUJUAN END DATE !!";
	else if (strcmp(rbsno,"S852") == 0) return "S852-NO THIS GUOKUJUAN !!";
	else if (strcmp(rbsno,"S853") == 0) return "S853-THIS GUOKUJUAN IS END!!";
	else if (strcmp(rbsno,"S854") == 0) return "S854-THIS CT IS UNDUE PLEASE CHECK !!";
	else if (strcmp(rbsno,"S901") == 0) return "S901-BCSUVSAM:FILE ALREADY EXIT";
	else if (strcmp(rbsno,"S902") == 0) return "S902-BCSUVSAM:FILE NOTFND";
	else if (strcmp(rbsno,"S903") == 0) return "S903-BCSUVSAM:FILE DUPLICATE";
	else if (strcmp(rbsno,"S904") == 0) return "S904-BCSUVSAM:HAS WARNING MESSAGE";
	else if (strcmp(rbsno,"S905") == 0) return "S905-BCSUVSAM:END OF FILE";
	else if (strcmp(rbsno,"S906") == 0) return "S906-BCSUVSAM:INVALID FUNCTION";
	else if (strcmp(rbsno,"S907") == 0) return "S907-BCSUVSAM:OTHER ERRORS HAPPENED";
	else if (strcmp(rbsno,"S908") == 0) return "S908-BCSUVSAM:DATABASE HAS ERROR";
	else if (strcmp(rbsno,"S909") == 0) return "S909-BCSUVSAM:KEY HAS NON-NUMERNIC DATA";
	else if (strcmp(rbsno,"S910") == 0) return "S910-BCSUVSAM:LENGTH ERROR";
	else if (strcmp(rbsno,"S911") == 0) return "S911-BCSUVSAM:DBS RETURN CODE INVALID";
	else if (strcmp(rbsno,"S990") == 0) return "没有定义项目代号和单位代码";
	else if (strcmp(rbsno,"S996") == 0) return "S996-NOT LAST LOG FOR CANCEL ACT TXN";
	else if (strcmp(rbsno,"S997") == 0) return "S997-EC MUST THE SAME TELLER";
	else if (strcmp(rbsno,"S998") == 0) return "S998-NOT FIRST ITTP REC";
	else if (strcmp(rbsno,"S999") == 0) return "S999-TXN AMT IS EQUAL TO THIS CUU-BAL !";
	else if (strcmp(rbsno,"W010") == 0) return "W010-NO AP TXN !";
	else if (strcmp(rbsno,"W011") == 0) return "W011-ACT NOT EQUAL";
	else if (strcmp(rbsno,"W012") == 0) return "W012-NO NORMAL OR REJ OR INQUIREN TXN";
	else if (strcmp(rbsno,"W013") == 0) return "W013-NOT  SV  OR  FD  TRANSACTION !";
	else if (strcmp(rbsno,"W015") == 0) return "W015-TELLER CASH HAVE BEEN > 100000.00";
	else if (strcmp(rbsno,"W101") == 0) return "W101-PLEASE CHANGE NEW PASSBOOK !";
	else if (strcmp(rbsno,"W102") == 0) return "W102-MAY BE NOT INPUT ID-NO !";
	else if (strcmp(rbsno,"W103") == 0) return "W103-MUST SAME SDN !";
	else if (strcmp(rbsno,"W104") == 0) return "W104-ACT RETREIVE ERROR !";
	else if (strcmp(rbsno,"W105") == 0) return "W105-CIF ERROR !";
	else if (strcmp(rbsno,"W106") == 0) return "W106-WRITE CIF ERROR !";
	else if (strcmp(rbsno,"Z000") == 0) return "Z000-SUCCESSFUL";		
	else return "未知错误";	
}


#endif

