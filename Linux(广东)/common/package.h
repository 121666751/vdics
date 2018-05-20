#ifndef PACKAGE_H
#define PACKAGE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define PKG_VOUCHERS_LEN	26
#define PKG_VOUCHERS_REQ_LEN	10


//报文包中各个字段长度
#define PKG_LEN_LEN			8
#define PKG_TYPE_LEN 		2
#define PKG_TRADECODE_LEN 	4
#define PKG_RESULTCODE_LEN 	4
#define PKG_TASKID_LEN 		12

int get_pack_type(const char *buf, char *pkg_type);
int get_pack_tradecode(const char *buf, char *trade_code);

#endif
