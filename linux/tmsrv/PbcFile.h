#ifndef PBCFILE_H
#define PBCFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <gsip_util.h>
#include <package.h>
#include <ExportLog.h>
#include <db_common.h>
#include <gsipcom.h>

#define  PBC_FILE_TYPE_QS 0
#define  PBC_FILE_TYPE_MX 1

#define PBC_HEADER_DATE_LEN 		8
#define PBC_HEADER_AREA_LEN 		6
#define PBC_HEADER_SESSION_LEN 		3
#define PBC_HEADER_BRNO_LEN 		6

//币种
#define PBC_COINTYPE	"CNY"
	
//处理人行数据
int proc_pbc_data(const char *pkg_in);

//人行数据导入
int pbc_import( pbc_header_t *h, const char *fname_qs, const char *fname_mx);

//人行数据导出
int pbc_export( pbc_header_t *h, const char *fname_qs, const char *fname_mx);

/*人行明细数据导入
@fname: 输入文件名
return：成功返回0 ， 失败返回-1
**/
int pbc_mx_import( pbc_header_t *h, const char *fname_mx);

/*人行清算数据导入数据库
@fname: 文件名
**/
int pbc_qs_import( pbc_header_t *h, const char *fname_qs);

/*获取文件名
@type: 0-清算文件, 1-明细文件
return: 成功-返回文件名， 失败返回NULL
*/
char *get_pbc_filename(int type, char *fname, const char *date, const char *area, const char *session);

/*分解清算数据包
@line: 输入数据包
@qs:   返回清算结构体
return: 返回清算结构体
**/
pbc_qs_t * spart_pbc_qs(const char *line, pbc_qs_t *qs);

//分解明细数据
pbc_mx_t * spart_pbc_mx(const char *line, pbc_mx_t *mx);

/*解包获取请求信息
@line: 		输入数据包
@h: 		返回header结构体 
return: 	成功返回结构体指针， 失败返回NULL
**/
pbc_header_t * spart_pbc_header(char *line, pbc_header_t *h);

/*人行清算数据从数据库导出 
**/
int pbc_qs_export( pbc_header_t *h, const char *fname);

/*人行明细数据导出
@fname: 文件名
**/
int pbc_mx_export( pbc_header_t *h, const char *fname);

//解包头
int resolve_pbc_pkg( const char *pkg_in, pbc_header_t *h);

#endif
