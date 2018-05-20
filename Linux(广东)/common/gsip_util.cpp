#include "gsip_util.h"
#include <AnsiMemfile.h>
#include <mem.h>
#include <package.h>
#include <db_common.h>

#define MAX_TMP_LEN 4096


/*是否是数字
@c: 判断字符
return :是数字返回1， 不是数字返回0  */
int isnum(char c)
{
	return (c>=48 && c<=57);
}

/*判断数字的位数
@tmp: 数字
return : 返回数字的位数 */
int int_bit_num(const unsigned int tmp)
{
	char buf[MAX_TMP_LEN];
	memset(buf, '\0', sizeof(buf));
	sprintf(buf, "%d", tmp);

	return strlen(buf);	
}


/*字符串左补任意字符或子字符串
@in : 输入字符串
@str: 要补的字符或字符串
@num: 字符的数目
return : 成功-0 ，失败-其他  */
int left_add_str(char *in, const char *str, const unsigned int num)
{
	if(num <0)
		return -1;

	int i;
	if((strlen(in) + num) > MAX_TMP_LEN)
		return -1;

	char tmp[MAX_TMP_LEN];
	memset(tmp, '\0', sizeof(tmp));
	
	for(i=0; i<num; i++)
		strcat(tmp, str);

	strcat(tmp, in);
	memset(in, '\0', strlen(in));
	strcpy(in, tmp);	

	return 0;
}

/*获取数据包
@pkg: 数据包存放的缓冲区
@channa: 渠道类型
@pkg_type :消息类型  
return ; 成功-0， 失败-其他  */
int get_pkg(char *pkg, const char *channel, const char *pkg_type)
{
	CAnsiMemFile m_cm;
	int len = 0, ret = 0;

	if(!strcmp(channel, PKG_USR_REQ))
	{

		m_cm.Write(" ", PKG_LEN_LEN, 1);

		//单票交易
		if(!strcmp(pkg_type, VOUCHERS_TRADECODE_REQ))
		{
				m_cm.Write(PKG_USR_REQ, PKG_TYPE_LEN, strlen(PKG_USR_REQ), true, '0');

				m_cm.Write(VOUCHERS_TRADECODE_REQ, PKG_TRADECODE_LEN, strlen(VOUCHERS_TRADECODE_REQ), true, '0');
			
				m_cm.Seek(0, CAnsiMemFile::begin);

				len = m_cm.GetLength() - PKG_LEN_LEN;

				m_cm.Write(itos(len).c_str(), PKG_LEN_LEN, itos(len).size(), true, '0');
			
				strncpy(pkg, (char *)m_cm.GetPtr(), m_cm.GetLength());
		}
		else if(!strcmp(pkg_type,VOUCHERS_TRADECODE_UP_REQ))
		{
				m_cm.Write(PKG_USR_REQ, PKG_TYPE_LEN, strlen(PKG_USR_REQ), true, '0');

				m_cm.Write(VOUCHERS_TRADECODE_UP_REQ, PKG_TRADECODE_LEN, strlen(VOUCHERS_TRADECODE_UP_REQ), true, '0');

				m_cm.Seek(0, CAnsiMemFile::begin);

				len = m_cm.GetLength() - PKG_LEN_LEN;

				m_cm.Write(itos(len).c_str(), PKG_LEN_LEN, itos(len).size(), true, '0');
				
				strncpy(pkg, (char *)m_cm.GetPtr(), m_cm.GetLength());
		}

		//信封交易
		else if(!strcmp(pkg_type, MULTIS_TRADECODE_REQ))
		{
				m_cm.Write(PKG_USR_REQ, PKG_TYPE_LEN, strlen(PKG_USR_REQ), true, '0');

				m_cm.Write(MULTIS_TRADECODE_REQ, PKG_TRADECODE_LEN, strlen(MULTIS_TRADECODE_REQ), true, '0');

				m_cm.Seek(0, CAnsiMemFile::begin);

				len = m_cm.GetLength() - PKG_LEN_LEN;

				m_cm.Write(itos(len).c_str(), PKG_LEN_LEN, itos(len).size(), true, '0');

				strncpy(pkg, (char *)m_cm.GetPtr(), m_cm.GetLength());
		}
		else if(!strcmp(pkg_type, MULTIS_TRADECODE_UP_REQ))
		{
				m_cm.Write(PKG_USR_REQ, PKG_TYPE_LEN, strlen(PKG_USR_REQ), true, '0');

				m_cm.Write(MULTIS_TRADECODE_UP_REQ, PKG_TRADECODE_LEN, strlen(MULTIS_TRADECODE_UP_REQ), true, '0');

				m_cm.Seek(0, CAnsiMemFile::begin);

				len = m_cm.GetLength() - PKG_LEN_LEN;

				m_cm.Write(itos(len).c_str(), PKG_LEN_LEN, itos(len).size(), true, '0');
			
				strncpy(pkg, (char *)m_cm.GetPtr(), m_cm.GetLength());
		}

		//委收交易
		else if(!strcmp(pkg_type, AUTHINPUT_TRADECODE_REQ))
		{
				m_cm.Write(PKG_USR_REQ, PKG_TYPE_LEN, strlen(PKG_USR_REQ), true, '0');

				m_cm.Write(AUTHINPUT_TRADECODE_REQ, PKG_TRADECODE_LEN, strlen(AUTHINPUT_TRADECODE_REQ), true, '0');

				m_cm.Seek(0, CAnsiMemFile::begin);

				len = m_cm.GetLength() - PKG_LEN_LEN;

				m_cm.Write(itos(len).c_str(), PKG_LEN_LEN, itos(len).size(), true, '0');

				strncpy(pkg, (char *)m_cm.GetPtr(), m_cm.GetLength());
		}
		else if(!strcmp(pkg_type, AUTHINPUT_TRADECODE_UP_REQ))
		{
				m_cm.Write(PKG_USR_REQ, PKG_TYPE_LEN, strlen(PKG_USR_REQ), true, '0');

				m_cm.Write(AUTHINPUT_TRADECODE_UP_REQ, PKG_TRADECODE_LEN, strlen(AUTHINPUT_TRADECODE_UP_REQ), true, '0');

				m_cm.Seek(0, CAnsiMemFile::begin);

				len = m_cm.GetLength() - PKG_LEN_LEN;

				m_cm.Write(itos(len).c_str(), PKG_LEN_LEN, itos(len).size(), true, '0');
			
				strncpy(pkg, (char *)m_cm.GetPtr(), m_cm.GetLength());
		}

		//上海进帐单交易
		if(!strcmp(pkg_type, CINPUT_TRADECODE_REQ))
		{
				m_cm.Write(PKG_USR_REQ, PKG_TYPE_LEN, strlen(PKG_USR_REQ), true, '0');

				m_cm.Write(CINPUT_TRADECODE_REQ, PKG_TRADECODE_LEN, strlen(CINPUT_TRADECODE_REQ), true, '0');
			
				m_cm.Seek(0, CAnsiMemFile::begin);

				len = m_cm.GetLength() - PKG_LEN_LEN;

				m_cm.Write(itos(len).c_str(), PKG_LEN_LEN, itos(len).size(), true, '0');
			
				strncpy(pkg, (char *)m_cm.GetPtr(), m_cm.GetLength());
		}
		else if(!strcmp(pkg_type,CINPUT_TRADECODE_UP_REQ))
		{
				m_cm.Write(PKG_USR_REQ, PKG_TYPE_LEN, strlen(PKG_USR_REQ), true, '0');

				m_cm.Write(CINPUT_TRADECODE_UP_REQ, PKG_TRADECODE_LEN, strlen(CINPUT_TRADECODE_UP_REQ), true, '0');

				m_cm.Seek(0, CAnsiMemFile::begin);

				len = m_cm.GetLength() - PKG_LEN_LEN;

				m_cm.Write(itos(len).c_str(), PKG_LEN_LEN, itos(len).size(), true, '0');
				
				strncpy(pkg, (char *)m_cm.GetPtr(), m_cm.GetLength());
		}
	}
	else
		ret = -1;

	return ret;
}

//心跳发送
int heart_request(int fd)
{
	CAnsiMemFile m_cm;

	m_cm.Write(" ", PKG_LEN_LEN, 1);
	m_cm.Write(PKG_SYS_REQ, PKG_TYPE_LEN, strlen(PKG_SYS_REQ), true, '0');

	m_cm.Seek(0, CAnsiMemFile::begin);

	int len = m_cm.GetLength() - PKG_LEN_LEN;

	m_cm.Write(itos(len).c_str(), PKG_LEN_LEN, itos(len).size(), true, '0');

	if(write(fd, m_cm.GetPtr(), m_cm.GetLength()) != m_cm.GetLength())
		return -1;
	
	return 0;
}

//心跳应答
int heart_response(int fd)
{
	CAnsiMemFile m_cm;

	m_cm.Write(" ", PKG_LEN_LEN, 1);
	m_cm.Write(PKG_SYS_RES, PKG_TYPE_LEN, strlen(PKG_SYS_RES), true, '0');

	int len = m_cm.GetLength() - PKG_LEN_LEN;

	m_cm.Seek(0, CAnsiMemFile::begin);

	m_cm.Write(itos(len).c_str(), PKG_LEN_LEN, itos(len).size(), true, '0');

	if(write(fd, m_cm.GetPtr(), m_cm.GetLength()) != m_cm.GetLength())
		return -1;
	
	return 0;
}
