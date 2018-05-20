#include "package.h"

//获取包类型
int get_pack_type(const char *buf, char *pack_type)
{
	try
	{
		strncpy(pack_type, buf, PKG_TYPE_LEN);
	}
	catch(...)
	{
		return -1;
	}
	return 0;
}

//解出交易码
int get_pack_tradecode(const char *buf, char *trade_code)
{
	try
	{
		strncpy(trade_code, buf+PKG_TYPE_LEN, PKG_TRADECODE_LEN);
	}
	catch(...)
	{
		return -1;
	}	
	return 0;
}
