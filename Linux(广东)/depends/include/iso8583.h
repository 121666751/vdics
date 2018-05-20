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
* 网址：   www.tchzt.com.cn
* 邮箱：   support@tchzt.com.cn
*
***************************************************************************
*/

#ifndef __ISO_8583_H__
#define __ISO_8583_H__

#include <map>
#include <string>
using namespace std;

#include <math.h>
#include <assert.h>

#define HEADER  8

typedef map<int, string>	ISOMAP;
typedef unsigned char		BYTE;
typedef unsigned long		DWORD;
typedef char				TCHAR;

typedef struct
{
    int        bit_flag;			  /*域数据类型2 -- string, 1 -- int, 0 -- binary*/
    char       *data_name;		  /*域名*/
    int        length;				  /*数据域长度*/
    int        length_in_byte;  /*实际长度（如果是变长）*/
    int        variable_flag;		/*是否变长标志0：否 2：2位变长, 3：3位变长*/
    int        datatype;			  /*0 -- string, 1 -- int, 2 -- binary*/
    char	     *data;				    /*存放具体值*/
    int        attribute;			  /*保留*/
} ISO8583Field; 

typedef struct {
    int len;
    BYTE *data;
} ISO8583FieldData;

static ISO8583Field Tbl8583[128] = 
{ 
 /* FLD 01 */ {0,"BIT MAP,EXTENDED", 8, 0, 0, 2, NULL,0}, 
 /* FLD 02 */ {0,"PRIMARY ACCOUNT NUMBER", 22, 0, 2, 0, NULL,0}, 
 /* FLD 03 */ {0,"PROCESSING CODE", 6, 0, 0, 0, NULL,0}, 
 /* FLD 04 */ {0,"AMOUNT, TRANSACTION", 12, 0, 0, 1, NULL,0}, 
 /* FLD 05 */ {0,"NO USE", 12, 0, 0, 1, NULL,0}, 
 /* FLD 06 */ {0,"II_AMOUNT_OUT", 12, 0, 0, 0, NULL,0}, 
 /* FLD 07 */ {0,"TRANSACTION DATE AND TIME", 10, 0, 0, 0, NULL,0}, 
 /* FLD 08 */ {0,"TCHZT_DEP_CODE", 8, 0, 0, 0, NULL,0}, 
 /* FLD 09 */ {2,"TCHZT_RET_CODE", 8, 0, 0, 0, NULL,0}, 
 /* FLD 10 */ {2,"TCHZT_SUBJECT", 8, 0, 0, 0, NULL,0}, 
 /* FLD 11 */ {0,"SYSTEM TRACE AUDIT NUMBER", 6, 0, 0, 1, NULL,0}, 
 /* FLD 12 */ {0,"TIME, LOCAL TRANSACTION", 6, 0, 0, 0, NULL,0}, 
 /* FLD 13 */ {0,"DATE, LOCAL TRANSACTION", 4, 0, 0, 0, NULL,0}, 
 /* FLD 14 */ {0,"DATE, EXPIRATION", 4, 0, 0, 0, NULL,0}, 
 /* FLD 15 */ {0,"DATE, SETTLEMENT", 4, 0, 0, 0, NULL,0}, 
 /* FLD 16 */ {0,"NO USE", 4, 0, 0, 0, NULL,0}, 
 /* FLD 17 */ {0,"DATE, CAPTURE", 4, 0, 0, 0, NULL,0}, 
 /* FLD 18 */ {0,"MERCHANT'S TYPE", 4, 0, 0, 0, NULL,0}, 
 /* FLD 19 */ {2,"TCHZT_CARD_TYPE", 3, 0, 0, 0, NULL,0}, 
 /* FLD 20 */ {0,"TCHZT_ACT_TYPE", 3, 0, 0, 0, NULL,0}, 
 /* FLD 21 */ {0,"NO USE", 3, 0, 0, 0, NULL,0}, 
 /* FLD 22 */ {0,"POINT OF SERVICE ENTRY MODE", 3, 0, 0, 0, NULL,0}, 
 /* FLD 23 */ {0,"NO USE", 3, 0, 0, 0, NULL,0}, 
 /* FLD 24 */ {0,"NO USE", 3, 0, 0, 0, NULL,0}, 
 /* FLD 25 */ {0,"POINT OF SERVICE CONDITION CODE", 2, 0, 0, 0, NULL,0}, 
 /* FLD 26 */ {0,"NO USE", 2, 0, 0, 0, NULL,0}, 
 /* FLD 27 */ {0,"NO USE", 1, 0, 0, 0, NULL,0}, 
 /* FLD 28 */ {0,"field27", 6, 0, 0, 0, NULL,0}, 
 /* FLD 29 */ {0,"NO USE", 8, 0, 1, 0, NULL,0}, 
 /* FLD 30 */ {0,"NO USE", 8, 0, 1, 0, NULL,0}, 
 /* FLD 31 */ {0,"NO USE", 8, 0, 1, 0, NULL,0}, 
 /* FLD 32 */ {0,"ACQUIRER INSTITUTION ID. CODE", 11, 0, 2, 0, NULL,0}, 
 /* FLD 33 */ {0,"FORWARDING INSTITUTION ID. CODE", 11, 0, 2, 0, NULL,0}, 
 /* FLD 34 */ {0,"TCHZT_ACCNO_BIND", 28, 0, 0, 0, NULL,0}, 
 /* FLD 35 */ {0,"TRACK 2 DATA", 37, 0, 2, 0, NULL,0}, 
 /* FLD 36 */ {0,"TRACK 3 DATA",104, 0, 3, 0, NULL,0}, 
 /* FLD 37 */ {2,"RETRIEVAL REFERENCE NUMBER", 12, 0, 0, 0, NULL,0}, 
 /* FLD 38 */ {1,"AUTH. IDENTIFICATION RESPONSE", 6, 0, 0, 0, NULL,0}, 
 /* FLD 39 */ {2,"RESPONSE CODE", 2, 0, 0, 0, NULL,0}, 
 /* FLD 40 */ {0,"NO USE", 3, 0, 0, 0, NULL,0}, 
 /* FLD 41 */ {1,"CARD ACCEPTOR TERMINAL ID.", 8, 0, 0, 0, NULL,0}, 
 /* FLD 42 */ {1,"CARD ACCEPTOR IDENTIFICATION CODE", 15, 0, 0, 0, NULL,0}, 
 /* FLD 43 */ {0,"CARD ACCEPTOR NAME LOCATION", 40, 0, 0, 0, NULL,0}, 
 /* FLD 44 */ {0,"ADDITIONAL RESPONSE DATA", 25, 0, 2, 0, NULL,0}, 
 /* FLD 45 */ {0,"TCHZT_IN_REVERSE_SEQ", 14, 0, 2, 0, NULL,0}, 
 /* FLD 46 */ {0,"TCHZT_OUT_REVERSE_SEQ", 14, 0, 3, 0, NULL,0}, 
 /* FLD 47 */ {2,"TCHZT_ACCNAME", 20, 0, 0, 0, NULL,0}, 
 /* FLD 48 */ {2,"ADDITIONAL DATA --- PRIVATE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 49 */ {2,"CURRENCY CODE,TRANSACTION", 3, 0, 0, 0, NULL,0}, 
 /* FLD 50 */ {0,"CURRENCY CODE,SETTLEMENT", 3, 0, 0, 0, NULL,0}, 
 /* FLD 51 */ {0,"NO USE", 3, 0, 0, 0, NULL,0}, 
 /* FLD 52 */ {0,"PERSONAL IDENTIFICATION NUMBER DATA", 16, 0, 0, 2, NULL,0}, 
 /* FLD 53 */ {0,"SECURITY RELATED CONTROL INformATION", 16, 0, 0, 0, NULL,0}, 
 /* FLD 54 */ {0,"ADDITIONAL AMOUNTS",120, 0, 3, 0, NULL,0}, 
 /* FLD 55 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 56 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 57 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 58 */ {1,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 59 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 60 */ {1,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 61 */ {2,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 62 */ {1,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 63 */ {1,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 64 */ {0,"MESSAGE AUTHENTICATION CODE FIELD", 8, 0, 0, 2, NULL,0}, 
 /* FLD 65 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 66 */ {0,"NO USE", 1, 0, 0, 0, NULL,0}, 
 /* FLD 67 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 68 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 69 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 70 */ {0,"SYSTEM MANAGEMENT INformATION CODE", 3, 0, 0, 0, NULL,0}, 
 /* FLD 71 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 72 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 73 */ {0,"NO USE", 6, 0, 0, 0, NULL,0}, 
 /* FLD 74 */ {0,"NUMBER OF CREDITS", 10, 0, 0, 0, NULL,0}, 
 /* FLD 75 */ {0,"REVERSAL NUMBER OF CREDITS", 10, 0, 0, 0, NULL,0}, 
 /* FLD 76 */ {0,"NUMBER OF DEBITS", 10, 0, 0, 0, NULL,0}, 
 /* FLD 77 */ {0,"REVERSAL NUMBER OF DEBITS", 10, 0, 0, 0, NULL,0}, 
 /* FLD 78 */ {0,"NUMBER OF TRANSFER", 10, 0, 0, 0, NULL,0}, 
 /* FLD 79 */ {0,"REVERSAL NUMBER OF TRANSFER", 10, 0, 0, 0, NULL,0}, 
 /* FLD 80 */ {0,"NUMBER OF INQUIRS", 10, 0, 0, 0, NULL,0}, 
 /* FLD 81 */ {0,"AUTHORIZATION NUMBER", 10, 0, 0, 0, NULL,0}, 
 /* FLD 82 */ {0,"NO USE", 12, 0, 0, 0, NULL,0}, 
 /* FLD 83 */ {0,"CREDITS,TRANSCATION FEEAMOUNT", 12, 0, 0, 0, NULL,0}, 
 /* FLD 84 */ {0,"NO USE", 12, 0, 0, 0, NULL,0}, 
 /* FLD 85 */ {0,"DEBITS,TRANSCATION FEEAMOUNT", 12, 0, 0, 0, NULL,0}, 
 /* FLD 86 */ {0,"AMOUNT OF CREDITS", 16, 0, 0, 0, NULL,0}, 
 /* FLD 87 */ {0,"REVERSAL AMOUNT OF CREDITS", 16, 0, 0, 0, NULL,0}, 
 /* FLD 88 */ {0,"AMOUNT OF DEBITS", 16, 0, 0, 0, NULL,0}, 
 /* FLD 89 */ {0,"REVERSAL AMOUNT OF DEBITS", 16, 0, 0, 0, NULL,0}, 
 /* FLD 90 */ {0,"ORIGINAL DATA ELEMENTS", 42, 0, 0, 0, NULL,0}, 
 /* FLD 91 */ {0,"FILE UPDATE CODE", 1, 0, 0, 0, NULL,0}, 
 /* FLD 92 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 93 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 94 */ {0,"SERVICE INDICATOR", 7, 0, 0, 0, NULL,0}, 
 /* FLD 95 */ {0,"REPLACEMENT AMOUNTS", 42, 0, 0, 0, NULL,0}, 
 /* FLD 96 */ {0,"NO USE", 8, 0, 0, 0, NULL,0}, 
 /* FLD 97 */ {0,"AMOUNT OF NET SETTLEMENT", 16, 0, 0, 0, NULL,0}, 
 /* FLD 98 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 99 */ {0,"SETTLEMENT INSTITUTION ID", 11, 0, 2, 0, NULL,0}, 
 /* FLD 100 */ {0,"RECVEING INSTITUTION ID", 11, 0, 2, 0, NULL,0}, 
 /* FLD 101 */ {0,"FILENAME", 17, 0, 2, 0, NULL,0}, 
 /* FLD 102 */ {0,"ACCOUNT IDENTIFICATION1", 28, 0, 2, 0, NULL,0}, 
 /* FLD 103 */ {0,"ACCOUNT IDENTIFICATION2", 28, 0, 2, 0, NULL,0}, 
 /* FLD 104 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 105 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 106 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 107 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 108 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 109 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 110 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 111 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 112 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 113 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 114 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 115 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 116 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 117 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 118 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 119 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 120 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 121 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 122 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 123 */ {0,"NEW PIN DATA", 8, 0, 3, 2, NULL,0}, 
 /* FLD 124 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 125 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 126 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 127 */ {0,"NO USE", 999, 0, 3, 0, NULL,0}, 
 /* FLD 128 */ {0,"MESSAGE AUTHENTICATION CODE FIELD", 8, 0, 0, 2, NULL,0}, 
}; 

class CISO8583
{
public:
    enum FileType
    {
        XML,
        BIN,
        HEX
    };

    /* Get message length in bytes */
    int GetMessageLength()
	{
		int len = 0;
		map<int, ISO8583FieldData>::iterator it = m_fields.begin();
		while (it != m_fields.end()) 
		{
			if (Tbl8583[it->first - 1].bit_flag == 0)
				len = len + (it->second.len + 1) / 2 + (Tbl8583[it->first - 1].variable_flag+1)/2;
			else
				len = len + it->second.len + (Tbl8583[it->first - 1].variable_flag+1)/2;
			it++;
		}
		
		return len + HEADER;
	}
	
    /* Write message out in xml format */
    virtual bool WriteToFile(char* pszFile, FileType type)
	{
		if (type == CISO8583::XML)
			return writeXML(pszFile);
		else if (type == CISO8583::BIN)
			return writeBIN(pszFile);
		else if (type == CISO8583::HEX)
			return writeHEX(pszFile);
		
		return false;
	}

protected:
	/* Write message in xml format */
    bool writeXML(char* pszXMLFile)
	{
		int i = 0;
		BYTE bitmap[HEADER];
		char sBitMap[33] = {'\0'};
		TCHAR *szFieldType[3] = {"string", "int", "binary"};
		FILE *fp = fopen(pszXMLFile, "w");
		if (NULL == fp) 
			return false;
		
		/* write bitmap */
		memset(bitmap, 0, sizeof(bitmap));
		map<int, ISO8583FieldData>::iterator it = m_fields.begin();
		while (it != m_fields.end()) {
			setBit(bitmap, it->first);
			it++;
		}
		
		for (i = 0; i < HEADER; i++) 
		{
			unsigned char h[8];
			memset(h,0,sizeof(h));
			IntToBCD(bitmap[i],h,2);
			//sprintf(h,"%02x", bitmap[i]);
			memcpy(sBitMap + 2 * i,h,2);
		}
		fprintf(fp, "<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n");
		fprintf(fp, "<ISO8583 fieldscount=\"%d\" totalbytes=\"%d\" bitmap=\"%s\">", m_fields.size(), this->GetMessageLength(), sBitMap);
		
		/* write fields */
		it = m_fields.begin();
		while (it != m_fields.end()) {
			fprintf(fp, "\r\n\t<Field no=\"%d\" type=\"%s\" fieldlength=\"%d\" datalength=\"%d\">\n", it->first, szFieldType[Tbl8583[it->first - 1].datatype], Tbl8583[it->first - 1].length, it->second.len);
			if (Tbl8583[it->first - 1].datatype == 1)
				fprintf(fp, "\t\t%s", (char *)it->second.data);
			else {
				fprintf(fp, "\t\t");
				for (i = 0; i < it->second.len; i++)
					fprintf(fp, "%02X", (BYTE)it->second.data[i]);
			}
            
			fprintf(fp, "\n\t</Field>");
			it++;
		}
		
		fprintf(fp, "\n</ISO8583>");
		fclose(fp);
		return true;
	}

    bool writeBIN(char* pszBINFile)
	{
		int i = 0;
		BYTE bitmap[HEADER];
		FILE *fp = fopen(pszBINFile, "wb");
		if (NULL == fp) 
			return false;
		
		/* write bitmap */
		memset(bitmap, 0, sizeof(bitmap));
		map<int, ISO8583FieldData>::iterator it = m_fields.begin();
		while (it != m_fields.end()) {
			setBit(bitmap, it->first);
			it++;
		}
		
		fwrite(bitmap, 1, sizeof(bitmap), fp);
		/* write fields */
		it = m_fields.begin();
		int len = 0;
		while (it != m_fields.end()) {
			if (Tbl8583[it->first - 1].bit_flag == 0)
				len = (it->second.len + 1) / 2;
			else
				len = it->second.len;
			
			if (Tbl8583[it->first - 1].variable_flag == 0)
				fwrite(it->second.data, 1, len, fp);
			else if (Tbl8583[it->first - 1].variable_flag == 2) {
				fprintf(fp, "%02d", it->second.len);
				fwrite(it->second.data, 1, len, fp);
			}
			else if (Tbl8583[it->first - 1].variable_flag == 3) {
				fprintf(fp, "%03d", it->second.len);
				fwrite(it->second.data, 1, len, fp);
			}
			
			it++;
		}
		
		fclose(fp);
		return true;
	}

    bool writeHEX(char* pszHEXFile)
	{
		int i = 0, j = 0, round = 0, length = this->GetMessageLength();
		FILE *fp = fopen(pszHEXFile, "wb");
		if (NULL == fp) 
			return false;
		
		BYTE *pBytes = new BYTE[length];
		BYTE *p = pBytes;
		if (NULL == pBytes)
			return false;
		
		memset(pBytes, 0, length);
		/* write bitmap */
		map<int, ISO8583FieldData>::iterator it = m_fields.begin();
		while (it != m_fields.end()) {
			setBit(pBytes, it->first);
			it++;
		}
		
		p += HEADER;
		/* write fields */
		it = m_fields.begin();
		int len = 0;
		while (it != m_fields.end()) {
			if (Tbl8583[it->first - 1].bit_flag == 0)
				len = (it->second.len + 1) / 2;
			else
				len = it->second.len;
			
			if (Tbl8583[it->first - 1].variable_flag == 0) {
				memcpy(p, it->second.data, len);
			}
			else if (Tbl8583[it->first - 1].variable_flag == 2) {
				
				unsigned char h[8];
				memset(h,0,sizeof(h));
				IntToBCD( Tbl8583[it->first - 1].length,h,2);
				
				memcpy(p,h,1);
				p += 1;
				memcpy(p, it->second.data, len);
			}
			else if (Tbl8583[it->first - 1].variable_flag == 3) {
				unsigned char h[8];
				memset(h,0,sizeof(h));
				IntToBCD( Tbl8583[it->first - 1].length,h,3);
				memcpy(p,h,2);
				p += 2;
				memcpy(p, it->second.data, len);
			}
			
			p += len;
			it++;
		}
		
		/* write to file in ultraedit hex-view format */
		for (i = 0; i < length; i++) {
			if (i == 0) 
				fprintf(fp, "%08Xh:", round);
			
			fprintf(fp, " %02X", pBytes[i]);
			
			if (++round % 16 == 0 && i != length - 1) {
				fwrite(" ; ", 1, 3, fp);
				p = pBytes + i - 15;
				for (j = 0; j < 16; j++) 
					fprintf(fp, "%c", (p[j] == 0 || p[j] == '\r' || p[j] == '\n')? '.' : p[j]);
				
				fprintf(fp, "\r\n%08Xh:", round);
			}
		}
		
		if ((round = length % 16) != 0) {
			for (i = 0; i < 16 - round; i++) 
				fwrite("   ", 1, 3, fp);
			fwrite(" ; ", 1, 3, fp);
			p = pBytes + length - round;
			for (i = 0; i < round; i++)
				fprintf(fp, "%c", (p[i] == 0 || p[i] == '\r' || p[i] == '\n') ? '.' : p[i]);
		}
		
		delete pBytes;
		fclose(fp);
		return true;
	}

	unsigned char* CharNToBCD(const char* cn,unsigned char* out,int cnt,bool bLeftSpace = true)
	{
		int i;
		unsigned char *nchar = new unsigned char[cnt+2];
		
		
		memset(nchar,0,sizeof(nchar));
		if(cnt%2==0)
		{		
			memcpy(nchar,cn,cnt);
		}
		else
		{	
			if (bLeftSpace)
			{
				// 左补0
				nchar[0] = '0';
				memcpy(nchar + 1, cn, cnt);
			}
			else
			{
				// 右补0 
				memcpy(nchar,cn,cnt);
				nchar[cnt]='0';
			}
		}
		
		char uu[3];
		for(i=0;i<(cnt+1)/2;i++)
		{
			//out[i]=((nchar[i*2]-0x30)<<4)|((nchar[i*2+1]-0x30)&0x0f);
			uu[0] = nchar[i * 2];
			uu[1] = nchar[i * 2 + 1];
			if (uu[1] == ' ')
				uu[1] = '0';
			
			uu[2] = '\0';
			
			out[i] = strtol(uu, (char**)NULL, 16);
		}
		
		if(nchar!=NULL) delete []nchar;
		return out;
	}

	unsigned char* IntToBCD(int val,unsigned char* out,int size)
	{
		char cval[20];
		switch(size)
		{
		case 1:
		case 2:
			sprintf(cval,"%02d",val);
			break;
		case 3:
			sprintf(cval,"%03d",val);
			break;			
		case 4:
			sprintf(cval,"%04d",val);
			break;
		case 5:
		case 6:
			sprintf(cval,"%06d",val);
			break;
		default:
			sprintf(cval,"%d",val);
			break;
		}
		return CharNToBCD(cval,out,strlen(cval));
		//	return (unsigned char *)cval;	
	}

	int Bcd2ToInt(unsigned char* bcd)
	{
		unsigned int res;
		unsigned int a,b,c,d;
		a=((unsigned char)bcd[0]>>4)*1000;
		b=((unsigned char)bcd[0]&0x0F)*100;
		c=((unsigned char)bcd[1]>>4)*10;
		d=(unsigned char)bcd[1]&0x0F;
		res=a+b+c+d;
		return res;
	}

	// 将一个BCD字符转换为整型数字
	int BcdToInt(unsigned char* bcd)
	{
		unsigned int res;
		unsigned int c,d;
		c=((unsigned char)bcd[0]>>4)*10;
		d=(unsigned char)bcd[0]&0x0F;
		res=c+d;
		return res;		
	}

	char *BinToHex(char* in,int inlen,char* out)
	{
		//out[0]=0;
		int i;
		for (i = 0; i < inlen; i++)
			sprintf(out + i * 2, "%02x", (unsigned char)(in[i]));
		return out;
	}

	/* Set bit in specified position */
    void setBit(BYTE *pBytes, int nPos)
	{
		if (nPos % 8 == 0)
			pBytes[nPos / 8 - 1] |= 0x01;
		else
			pBytes[(unsigned int)floor( (double)(nPos / 8))] |= (0x80 >> (nPos % 8 - 1));
		
		if (nPos > 64)
			setBit(pBytes, 1);
	}

	/* Test to see if bit position of p is set */
    bool testBit(const BYTE *pBytes, int nPos)
	{
		unsigned int nOffset;
		BYTE nMask;
		nOffset = (nPos % 8 == 0) ? nPos / 8 - 1 : (unsigned int)floor((double)(nPos / 8));
		nMask = nPos % 8 == 0 ? 0x01 : (0x80 >> (nPos % 8 - 1));
		
		return (pBytes[nOffset] & nMask) == nMask;
	}

    map<int, ISO8583FieldData> m_fields;
};

class CISO8583Writer : public CISO8583
{
public:
    CISO8583Writer()
	{
	}

    ~CISO8583Writer()
	{
		map<int, ISO8583FieldData>::iterator it = m_fields.begin();
		while (it != m_fields.end()) {
			delete it->second.data;
			it++;
		}
	}

	// 清除指定field内容，nField = -1清除所有内容
	void ClearField(int nField = -1)
	{
		if (nField == -1)
		{
			map<int, ISO8583FieldData>::iterator it = m_fields.begin();
		
			for (; it != m_fields.end(); it++)
			{
				delete it->second.data;
				m_fields.erase(it);
			}
		}
		else
		{
			//WriteField(nField, 0, NULL);
			map<int, ISO8583FieldData>::iterator it = m_fields.find(nField);
		/* delete the specified field if it exists */
		if (it != m_fields.end()) {
			delete it->second.data;
			m_fields.erase(it);
		}	
		}
	}
	
	// Build body
	int WriteBody(ISOMAP& isomap)
	{
		map<int, string>::iterator it = isomap.begin();
		
		for (; it != isomap.end(); it++)
		{
			WriteField(it->first, it->second.length(), (char*)it->second.c_str());
		}
		
		return 0;
	}

  /* Copy message out */
  int CopyOut(BYTE *pOutBuf, int nBufLen)
	{
		int offset = HEADER;
		if (NULL == pOutBuf || nBufLen < GetMessageLength())
			return -1;
		
		/* clear bitmap */
		memset((void *)pOutBuf, 0, HEADER);
		map<int, ISO8583FieldData>::iterator it = m_fields.begin();
		while (it != m_fields.end()) {
			/* set bitmap */
			setBit(pOutBuf, it->first);
			
			/* copy data */
			if (Tbl8583[it->first - 1].variable_flag == 0)  //固定长度
			{
				if (Tbl8583[it->first - 1].bit_flag == 0 )     //压缩
				{
					memcpy(pOutBuf + offset, it->second.data, (it->second.len+1)/2);
					offset += (it->second.len+1)/2;
				}
				else
				{
					memcpy(pOutBuf + offset, it->second.data, it->second.len);
					offset += it->second.len;
				}
			}
			else if (Tbl8583[it->first - 1].variable_flag == 2)  //2位可变长
				{
				unsigned char h[8];
				memset(h,0,sizeof(h));
				IntToBCD( it->second.len,h,2);
				
				memcpy((char *)(pOutBuf + offset),h,1);
				offset += 1;
				if (Tbl8583[it->first - 1].bit_flag == 0 )				//压缩
				{
					memcpy(pOutBuf + offset, it->second.data, (it->second.len + 1)/2);
					offset = offset + (it->second.len+1)/2 ;
				}
				else
				{
					memcpy(pOutBuf + offset, it->second.data, it->second.len );
					offset = offset + it->second.len ;
				}
			}
			else if (Tbl8583[it->first - 1].variable_flag == 3) {
				unsigned char h[8];
				memset(h,0,sizeof(h));
				IntToBCD( it->second.len,h,3);
				
				memcpy((char *)(pOutBuf + offset),h,2);
				offset += 2;
				if (Tbl8583[it->first - 1].bit_flag == 0 )
				{
					memcpy(pOutBuf + offset , it->second.data, (it->second.len + 1)/2);
					offset = offset + (it->second.len+1)/2 ;
				}
				else
				{
					memcpy(pOutBuf + offset , it->second.data, it->second.len);
					offset = offset + it->second.len ;
				}
			}
			it++;
		}
		
		return offset;
	}

protected:
    /* write specified field, if pFieldBuf is NULL then this field will be cleared */
	void WriteField(int nField, int nFieldLen,  char *pFieldBuf)
	{
		char str[200];
		memset(str,0,sizeof(str));
		strncpy(str,pFieldBuf,nFieldLen);
		if (Tbl8583[nField-1].variable_flag == 0 && Tbl8583[nField - 1].bit_flag != 0)  //固定长度
		{
			if(nFieldLen < Tbl8583[nField-1].length)
			{				
				strncpy(str+nFieldLen,"          ",Tbl8583[nField-1].length-nFieldLen);
				nFieldLen = Tbl8583[nField-1].length;
			}
		}

		
		if (Tbl8583[nField - 1].bit_flag == 0 ) 
		{
			unsigned char out[100];
			memset(out,0,sizeof(out));
			if(nField == 4)							//金额补足12位
			{
				char amt[16];
				memset(amt,0,sizeof(amt));
				if(nFieldLen<12)
				{
					strncpy(amt,"00000000000",12-nFieldLen);
					strcat(amt,str);
				}
				else
				{
					strncpy(amt,str+nFieldLen-12,12);
				}
				nFieldLen = 12;
				WriteFieldB(nField,nFieldLen,(unsigned char*)CharNToBCD(amt,out,nFieldLen,true));			
			}			
			else	if( Tbl8583[nField - 1].variable_flag == 0 )		//固定长度左补0
			{
				WriteFieldB(nField,nFieldLen,(unsigned char*)CharNToBCD(str,out,nFieldLen,true));		
			}
			else
			{
				WriteFieldB(nField,nFieldLen,(unsigned char*)CharNToBCD(str,out,nFieldLen,false));		
			}
		}
		else
			WriteFieldB(nField,nFieldLen,(unsigned char*)str);
	}

  const BYTE *WriteFieldB(int nField, int nFieldLen, BYTE *pFieldBuf)
	{
		if (nField > 128 || nField < 1)
			throw "field should between 1 and 128";
		
		map<int, ISO8583FieldData>::iterator it = m_fields.find(nField);
		/* delete the specified field if it exists */
		if (it != m_fields.end()) {
			delete it->second.data;
			m_fields.erase(it);
		}
		
		/* clear the specified field and return if pFieldBuf is NULL */
		if (NULL == pFieldBuf) 
			return NULL;
		
		
		/* copy data */
		ISO8583FieldData field;
		field.len = nFieldLen;
		int tlen  = nFieldLen;
		if (Tbl8583[nField - 1].bit_flag == 0 ) 
		{
			tlen = (field.len+1)/2;
		}
		
		field.data = new BYTE[tlen+1];
		memset(field.data,0,tlen+1);
		if (NULL == field.data)
			return NULL;
		
		memcpy(field.data, pFieldBuf, tlen);
		/* add field */
		m_fields[nField] = field;
		
		return field.data;
	}
};

class CISO8583Reader : public CISO8583
{
public:
    CISO8583Reader()
	{
	}

    ~CISO8583Reader()
	{
		m_fields.clear();
	}

    /* Read the message */
    bool ReadMessage(const BYTE *pMessage)
	{
		try {
			return parseFields(pMessage);
		}
		catch (...) {
			return false;
		}
	}

	int GetFields(ISOMAP& isomap)
	{
		isomap.clear();
		
		int fl = FieldsCount();
		
		for (int index = 2; index <= 64; index++)
		{
			if (!FieldExist(index))
				continue;
			
			int flen = FieldLength(index);
			unsigned char* accno = new unsigned char[flen + 1];
			
			memset(accno, 0, flen + 1);
			int s = ReadField(index, accno, flen);
			
			char* out = new char[s * 2 + 2];
			memset(out, 0, s * 2 + 2);
			
			memset(out, 0, sizeof(out));
			BinToHex((char*)accno, s, out);
			
			isomap[index] = out;
			
			delete []accno;
			delete []out;
		}
		
		return 0;
	}

    /* Get fields number that is set in the bitmap, count should large than value return by FieldsCount() */
    int GetFields(int *fields, int count)
	{
		int fcount = m_fields.size() > count ? count : m_fields.size();
		map<int, ISO8583FieldData>::iterator it = m_fields.begin();
		for (int i = 0; i < fcount; i++) {
			fields[i] = it->first;
			it++;
		}
		
		return fcount;
	}

    /* Findout whether specified field exists */
    bool FieldExist(int nField)
	{
		return m_fields.find(nField) != m_fields.end();
	}

    /* Findout whether specified fields exist. */
    int FieldsExist(int *fields, int nCount)
	{
		for (int i = 0; i < nCount; i++) 
		{
			if (!FieldExist(fields[i]))
				return fields[i];
		}
		
		return 0;
	}

    /* Get fields total count */
    int FieldsCount()
	{
		return m_fields.size();
	}

    /* Get specified field length */
    int FieldLength(int nField)
	{
		if (nField > 128) return -1;
		map<int, ISO8583FieldData>::iterator it = m_fields.find(nField);
		if (it == m_fields.end())
			return -1;
		
		/* substract the LLL or LL length */
	  //	if (Tbl8583[nField - 1].bit_flag == 0)
		//	return (it->second.len+1)/2;
		//else
		return it->second.len;//- Tbl8583[nField - 1].variable_flag;
	}

    /* Get specified field data */
    int ReadField(int nField, BYTE *pOutBuf, int nBufLen)
	{
		map<int, ISO8583FieldData>::iterator it = m_fields.find(nField);
		if (it == m_fields.end())
			return -1;
		
		int nFieldLen = it->second.len;// - Tbl8583[nField - 1].variable_flag;
		
		if (Tbl8583[nField - 1].bit_flag == 0)
			nFieldLen = ( nFieldLen+1)/2;	
		if (nBufLen >nFieldLen)
			return -1;
		
		memcpy(pOutBuf, it->second.data, nFieldLen);
		return nFieldLen;
	}

protected:

    bool parseFields(const BYTE *pMessage)
	{
		//bool extensionMode = testBit(pMessage, 1); /* Test bit pos 2 - 128 or 2 - 64 */
		DWORD dwOffset = HEADER;
		BYTE len[4];
		
		for (int i = 2; i <= 64 ; i++) {
			if (!testBit(pMessage, i)) continue;
			
			/* find data position and length for each field */
			ISO8583FieldData field;
			if (Tbl8583[i - 1].variable_flag == 0) 
			{
				field.len = Tbl8583[i - 1].length;
				if (Tbl8583[i - 1].bit_flag == 0 )   field.len =  (field.len+1)/2;
				field.data = (BYTE *)pMessage + dwOffset;
				
				dwOffset += field.len;
				field.len = Tbl8583[i - 1].length;
			}
			else if(Tbl8583[i - 1].variable_flag == 2) 
			{
				memset(len, 0, sizeof(len));
				memcpy((void *)len, (void *)(pMessage + dwOffset), 1);
				
				field.len = BcdToInt(len);
				if (Tbl8583[i - 1].bit_flag == 0 )   
					field.len =  (field.len+1)/2;
				field.data = (BYTE *)(pMessage + dwOffset + 1);
				dwOffset = dwOffset + field.len + 1;
				field.len = BcdToInt(len);//atoi((char*)len); //Bcd2ToInt(len);
			}
			else
			{
				memset(len, 0, sizeof(len));
				memcpy((void *)len, (void *)(pMessage + dwOffset), 2);
				char out[24];
				memset(out,0,sizeof(out));
				BinToHex((char*)len, 2,out);
				
				field.len = Bcd2ToInt(len);
				
				if (Tbl8583[i - 1].bit_flag == 0 )   field.len =  (field.len+1)/2;
				field.data = (BYTE *)(pMessage + dwOffset + 2);
				dwOffset = dwOffset + field.len + 2;
				field.len = Bcd2ToInt(len);
				
			}
			
			m_fields[i] = field;
		}
		
		return true;
	}

};

#endif
