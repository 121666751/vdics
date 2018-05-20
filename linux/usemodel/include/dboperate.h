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
 * 文件：	$Id: dboperate.h,v 1.1 2008/03/30 10:05:04 Wu Exp $
 * 版本：	$Revision: 1.1 $
 * 网址：   www.tchzt.com.cn
 * 邮箱：   support@tchzt.com.cn
 *
 ***************************************************************************
 */

 #ifndef __dboperate_h_
#define __dboperate_h_

#include <libmem.h>
#include <STRING>
#include <zcommon.h>
#include <vchproc.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
	dboperate.dll 提供自动入账系统对数据库访问的功能
*/


struct idres_node_t{
	int sid;					/**<流水号*/
	char accno[30];				/**<账号*/
	char vchno[20];				/**<支票号*/
	char exchno[20];			/**<交换号*/
	char amount[20];			/**<金额*/
	int sysidres;				/**<自动识别结果*/
	int manidres;				/**<手动识别结果*/
	int state;					/**<记录状态*/
	char vchtype[5];			/**<凭证类型*/
	int  modifyno;				/**<更改号*/
	char pkgno[20];				/**<包号*/
	char clerk[20];				/**<干预柜员*/
	char chkclk[20];			/**<审票柜员*/
	char extflag[16];			/**<附加信息*/
	char reverse[512];			/**<保留字段*/
};

struct idres_detail_t{
	int sid;					/**<流水号*/
	char accno[30];				/**<帐号*/
	char vchno[10];				/**<支票号*/
	char vchtype[5];			/**<支票类型*/
	char moneytype[5];			/**<货币种类*/
	char exchno[20];			/**<交换号*/
	char amount[20];			/**<金额*/
	int sysidres;				/**<自动识别结果*/
	int manidres;				/**<手动识别结果*/
	char clerk[20];				/**<干预操作柜员号*/
	int state;					/**<状态位：0-待干预 1-已提交 2-正在干预 3-干预完成*/
	char sysstpres[20];			/**<自动每个章识别结果*/
	char manstpres[20];			/**<手动每个章识别结果*/
	unsigned char *resimg;		/**<匹配图*/
	long imglen;				/**<匹配图长度*/
	int modifyno;				/**<更改号*/
	char e13b[60];				/**<清分码*/
	char fname[40];				/**<图像文件的文件名*/
	char pkgno[20];				/**<包号*/
	char idtime[25];			/**<识别时间*/
	char extflag[16];			/**<附加信息*/
	char tel[40];				/**<联系电话*/
	char checkclk[20];			/**<审票柜员*/
	char warrantclk[20];		/**<票面审核授权柜员*/
	char warrantidres[20];      /**<验印大额授权柜员*/
	char memo[200];				/**<客户信息*/
        char idmemo[100];           /**<识别信息*/	
	char companyname[64];       /**<公司名称*/
	char vsacc[32];             /**<收款人帐号*/
	char vsname[64];            /**<收款人户名*/
	char date[12];              /**<日期*/
	char session[4];            /**<场次*/
        char money_e13b[20];		/**< 原始磁码金额 */
	char money_digit[20];		/**< 原始小写金额 */
	char money_cn[20]; 			/**< 原始大写金额 */
	char vchdate[12];			/**< 凭证日期 */
	char vchno_e13b[10];        /**<清分码 支票号*/
	char vchno_digit[10];       /**<小写凭证号*/ 
	char reverse[156];			/**<预留字段*/
};

struct imgfile_node_t{
	char fname[40];				/**<文件名*/	
	int dpi;					/**<dpi值*/
	double calx;				/**<扫描仪x轴校准值*/
	double caly;				/**<扫描仪y轴校准值*/
	int rotate;					/**<旋转角度*/
	char pkgno[20];				/**<批次号*/
	int pkgflag;				/**<包的类型标志位：0-包号 1-交换号*/
	char pkgdate[10];			/**<包的日期*/
	int pkgsess;				/**<场次*/
	char prefix;				/**<图像文件名中的前缀，用于区分不同台扫描仪*/
	char side;					/**<区分正反面：A-正面 B-反面*/
	int fcount;					/**<图像文件名中的数字部分*/
	char ip[20];				/**<文件所在计算机ip地址*/
	int state;					/**<运算状态*/
	char colortype[12];			/**<颜色类型*/
	char clerk[20];				/**<操作柜员号*/
	int proctype;				/**<处理类型:0-不运算 1-运算*/
	char reserve[512];			/**<附加信息*/
};

struct pkgno_t{
	char pkgno[20];				/**<包号*/
	char reserve[512];
};

struct idmemo_t{
	char idmemo[98];			/**<识别信息*/
	char reserve[512];
};

struct at_exchst_t
{
	char pkgno[20];
	char state[20];
	char reserved[100];
};


/**
	*提供数据库操作句柄,若操作失败返回NULL
*/

HANDLE dbo_init();


/**
	*提供数据库操作句柄,若操作失败返回NULL
*/

HANDLE dbo_init_ex(const char *database);

/**
	*获得所有的验印结果	
	*@dbh:		数据库操作句柄 
	*@nodelist:	[out]节点信息列表。
	*@count：	[out]返回的节点信息个数,resnode需用mem_free()释放
	*@return:	0-成功，否则失败，错误码见<zerrno.h>
*/

int dbo_loadalldata(HANDLE dbh,idres_node_t *&resnode,int &count);


/**
	*删除数据库操作句柄
	*@dbh: 数据库操作句柄
*/

void dbo_close(HANDLE dbh);



/**
	*获得所有比modifyno大的节点信息	
	*@dbh:		数据库操作句柄 
	*@modifyno: 更改号
	*@nodelist:	[out]节点信息列表。
	*@count:	[out]返回的节点信息个数,resnode需用mem_free()释放
	*@return:	0-成功，否则失败，错误码见<zerrno.h>
*/
int dbo_loadnewdata(HANDLE dbh,int modifyno,idres_node_t *&resnode,int &count);


/**
	*获得某条流水号所对应的识别信息的详细信息	
	*@dbh:			数据库操作句柄 
	*@sid:			流水号
	*@resdetail:	[out] 详细信息结构体,resdetail.resimg需用mem_free()释放
	*@return:		0-成功，否则失败，错误码见<zerrno.h>
*/
int dbo_loaddetail(HANDLE dbh,int sid,idres_detail_t &resdetail);

/**
	*用于获得baseno表中最大记录号
	*@dbh:		数据库操作句柄 
	*@flag:		系统标志位 0-自动入账
	*@modifyno: [out]最大记录号
	*@return:	0-成功，否则失败，错误码见<zerrno.h>
*/
int dbo_getmodifyno(HANDLE dbh,int flag,int &modifyno);

/**
	*用于记录某条流水号所对应的识别信息的人工干预结果
	*@dbh:		数据库操作句柄 
	*@detail:	详细信息
	*@imgflag:	0 存图像 1 不存图像
	*@return:	0-成功，否则失败，错误码见<zerrno.h>
*/
int dbo_updateidres(HANDLE dbh,const idres_detail_t *detail,int imgflag);

/**
	*用于设置idres表中sid所对应的state的值
	*@dbh:		数据库操作句柄
	*sid:		流水号
	*state:		印章的干预结果 0-带干预 1-已提交 2-正在干预 3-干预完成
*/
int dbo_setidres(HANDLE dbh,int sid,int state);

/**
	*用于将待干预记录的状态设置为正在干预
	*@dbh:		数据库操作句柄 
	*@sid:		流水号
	*@return:	0-成功，否则失败，错误码见<zerrno.h>
*/
int dbo_lockidres(HANDLE dbh,int sid);

/**
   *用于获得识别信息
   *@dbh:	 数据库操作句柄 
   *@mem:	 识别信息
   *@count:  总数
   *@return: 0-成功 其他:失败
*/
int dbo_loadidmemo(HANDLE dbh,MemT &mem,int &count);

/**
   *用于插入识别信息
   *@dbh:		数据库操作句柄 
   *@memolist:	识别信息
   *@count:     effect rows
   *@return: 0-成功 其他:失败
*/
int dbo_insertidmemo(HANDLE dbh,const char* memo,int &count);

/**
	*用于获得某状态所对应的所有包号
	*@dbh:		数据库操作句柄 
	*@flag: 	状态信息,0-未提交 1-已提交
	*@pkgno:	[out]包号信息结构体内存块。需用mem_free()释放
	*@count:	[out]返回的结构体数量
	*@return:	0-成功，否则失败，错误码见<zerrno.h>
*/
int dbo_loadpkgno(HANDLE dbh, int flag,pkgno_t *&pkgno,int &count,const char*date,const char*session);

/**
	*用于获得某批次所对应的某状态的所有节点信息
	*@dbh:		数据库操作句柄 
	*@pkgno:	批次号
	*@flag: 	状态信息，0-未提交 1-已提交
	*@reslist:	[out]节点信息结构体内存块。
	*@count:	[out]返回的结构体数量
	*@date:     日期
	*@session:  场次
	*@return:	0-成功，否则失败，错误码见<zerrno.h>
*/
int dbo_loadpkg(HANDLE dbh,const char *pkgno,int flag,idres_node_t *&reslist,int &count,const char*date,const char*session);

/**
	*用于提交某批次的运算结果
	*@dbh:		数据库操作句柄 
	*@pkgno:	批次号
	*@clkno:    提交柜员 
	*@date:     日期
	*@session:  场次
	*@return:	0-成功，否则失败，错误码见<zerrno.h>	
*/
int dbo_submit(HANDLE dbh,const char *pkgno,const char*date,const char*session,const char* clkno);

/**
	*用于在提交前检查是否可以提交
	*@dbh:		数据库操作句柄 
	*@pkgno:	批次号
	*@date:     日期
	*@session:  场次
	*@return:	0-成功，否则失败，错误码见<zerrno.h>
*/
int dbo_presubmit(HANDLE dbh,const char *pkgno,const char*date,const char*session);

/**
	*用于返回imgfile表中某前缀所对应最大的fcount数值
	*@dbh:		数据库操作句柄
	*@prefix:	前缀
	*@fcount:	[out]用于返回最大的fcount,如果没有符合记录的信息返回-1
	*@return:	0-成功，否则失败，错误码见<zerrno.h>
*/
int dbo_image_maxcount(HANDLE dbh,char prefix,long &fcount);

/**
	*用于更新imgfile表中某前缀所对应最大的fcount数值
	*@dbh:  	数据库操作句柄
	*@prefix:	前缀
	*@fcount:	[in]最大的fcount
	*@return:	0-成功，否则失败，错误码见<zerrno.h>
*/

int dbo_image_updatemax(HANDLE dbh,char prefix,long &fcount);

/**
	*用于在imgfile表中插入节点信息。
	*@dbh:		数据库操作句柄
	*@node:		要插入的节点信息的结果体
	*@return:	0-成功，否则失败，错误码见<zerrno.h>
*/
int dbo_image_insert(HANDLE dbh,const imgfile_node_t &node);

/**
	*函数用于查询at_exchst表中某批次所对应的扫描仪的状态。
	*@dbh:		数据库操作句柄
	*@pkgno:	包号
	*@state:	[out]用于返回状态信息,若无批次号所对应记录返回-1
	*@return:	0-成功，否则失败，错误码见<zerrno.h>
*/
int dbo_image_scaninfo(HANDLE dbh,const char *pkgno,int &state);

/**
	*用于向at_exchst表中更新或插入某批次所对应的扫描仪的状态
	*@dbh:		数据库操作句柄
	*@pkgno:	包号
	*@state:	状态信息
	*@return:	0-成功，否则失败，错误码见<zerrno.h>
*/
int dbo_image_update(HANDLE dbh,const char *pkgno,int state);

/**
	*函数用于从imgfile表中得到未运算且fcount值最小的文件信息或fname所对应信息。
	*@dbh:		数据库操作句柄
	*@fname:	文件名。如果为NULL返回未运算且fcount值最小的文件信息，否则返回文件名所对应信息
	*@imgnode：	[out]imgfile_node_t结构体，用于返回文件信息
	*@return:	0-成功，否则失败，错误码见<zerrno.h>
*/
int dbo_getimgfile(HANDLE dbh,const char *fname,imgfile_node_t &imgnode);

/**
	*函数用于设置imgfile表中state的值
	*@dbh:		数据库操作句柄
	*@fname:	文件名。
	*@state:	运算的状态。0-未运算 1-运算完成 2-正在运算
*/
int dbo_setimgfile(HANDLE dbh,const char *fname,int state);

/**
	函数用于保存运算结果信息。
	*@dbh:		数据库操作句柄
	*@resnode:	idres_allnode_t结构体用于存储运算结果信息
	*@imgflag:	0 存图像 1 不存图像
	*@return:	0-成功，否则失败，错误码见<zerrno.h>
*/
int dbo_saveidres(HANDLE dbh,const idres_detail_t &resnode,int imgflag);

/**
	函数用于提取config表中，name字段所对应的value的值
	*@:dbh:		数据库操作句柄
	*@:name:	预查询的name的值
	*@:value:	[out]用于返回value的值
	*@return:	0-成功，1-无纪录 否则失败
*/
int dbo_getcfg(HANDLE dbh,const char *name,char *value);


/**
	函数用于向config表中，插入name字段及value的字段
	*@:dbh:		数据库操作句柄
	*@:name:	预插入的name的值
	*@:value:	预插入的value的值
*/
int dbo_setcfg(HANDLE dbh,const char *name,const char *value);

/************************************************************************/
/*	函数用于设置某批次号的不参加运算的票据，在at_exchst表中验印位的状态	*/
/************************************************************************/
int dbo_set_cridstate(HANDLE dbh,const char *pkgno,int state);

/**
	*读取全部网点完整状态
*/
int dbo_allexchst(HANDLE dbh,MemT& mem, int& count,const char*date,const char*session);


/**
	*读取某批次完整状态
*/
int dbo_getexchst(HANDLE dbh,const char* pkgno, char* state,const char*date,const char*session);

/**
	*设置某批次完整状态
*/
int dbo_setexchst(HANDLE dbh,const char* pkgno, char* state,const char*date,const char*session);

/*
 *得到一个要记帐的网点的交换号和状态，并把该网点修改为记帐中
 *@param dbh: 数据库句柄
 *@param state: 状态
 *@param exchno: 交换号
 *@return:  0-正确，其他-不正确
 */
int dbo_gettallyexno(HANDLE dbh,char* pkgno, char* state);

/*
 *检查金额是否正确
 *@param dbh: 数据库句柄
 *@param amount: 金额
 *@param exchno: 交换号
 *@return:  0-正确，其他-不正确
 */
int dbo_checkamount(HANDLE dbh,const char* amount,const char* exchno,const char*date,const char*session);

/*
 *	查找贵宾帐号
 *@param dbh: 数据库句柄
 *@param accno: 帐号
 *@reutrn: 0-不是 1-是 其他：查询错误
 */
int dbo_checkvip(HANDLE dbh,const char* accno);

/*
 *	是否存在没有识别完的票据
 *@param dbh:    数据库句柄
 *@param exchno: 交换号
 *@reutrn:       0-完成了, 1-未完成, 其他：查询错误
 */
int dbo_isfinished(HANDLE dbh,const char* exchno,const char*date,const char*session);

/*
 *	修改状态为未识别
 *@param dbh:     数据库句柄
 *@param exchno:  交换号
 *@reutrn:        0-不是, 其他：查询错误
 */
int dbo_reidentify(HANDLE dbh,const char* exchno,const char*date,const char*session);

/*
 * 删除一个批次的纪录并重新运算	
 *@param dbh:     数据库句柄
 *@param exchno:  交换号
 *@reutrn:        0-不是, 其他：查询错误
 */
int dbo_deletepkg(HANDLE dbh,const char* exchno,const char*date,const char*session);

/*
 *@param dbh: 数据库句柄
 *@param exchno:    交换号
 *@param value:     总金额
 *@reutrn:          0-不是, 其他：查询错误
 */
int dbo_totalmoney(HANDLE dbh,const char* exchno,char* value,const char*date,const char*session);

/*
 *@param dbh:   数据库句柄
 *@param sid:   统一序号
 *@reutrn:      0-不是, 其他：查询错误	
 */
int dbo_processone(HANDLE dbh,int sid);

/*
 *删除一条记录
 *@param dbh: 数据库句柄
 *@param sid: 统一序号
 *@reutrn:    0-不是, 其他：查询错误	
 */
int dbo_deleteone(HANDLE dbh,int sid);



/*
 *从清分表中调取图像名为fname的帐号,凭证号等信息
 *@param dbh:   数据库句柄
 *@param fname: 图像名
 *@outInfo:     输出结构体
 */

int dbo_getVchInfo(HANDLE dbh,const char*fname,VCHOCRRESULT *outInfo);

/*
 *强制更新验印结果信息
 *@param dbh:   数据库句柄
 *@param sid:   序号
 *@result:      结果
 *@idclkno:     干预柜员
 *@warclkno:    授权柜员
 */

int dbo_forceupdate(HANDLE dbh,int sid,int result,const char* idclkno,const char* warclkno);

/*
 *根据文件名得到日期场次
 *@param dbh :数据库句柄
 *@fname     :文件名
 *@date      :日期
 *@session   :场次
 */
int dbo_getds(HANDLE dbh,const char*fname,char *date,char*session );

#ifdef __cplusplus
}
#endif

#endif