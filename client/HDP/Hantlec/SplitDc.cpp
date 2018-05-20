/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
 *
 ***************************************************************************
 */
#include <iostream>
#include "SplitDc.h"
#include <zcommon.h>
#include <vector>

SplitDc::SplitDc()
{

}

/**
    初始化各域分割符
*/
int SplitDc::init(DISTINFO &distinf)
{
    _typeMap.insert(value_t("<<", CT_VCHNO));


   _typeMap.insert(value_t("::", CT_EXCHNO));


	_typeMap.insert(value_t(":<", CT_ACCNO));

    _typeMap.insert(value_t("<;", CT_VCHTP));

    _typeMap.insert(value_t(";;", CT_MONEY));

}

/**
    切割
*/
int SplitDc::split(string code, DcInfo &result)
{
  /*	debugout("CODE:[%s]", code.c_str());

	if( code.length()< 3  ) return 0;

	if( !isSplitChar(code[0])) code = " "+code;
	if( !isSplitChar(code[code.length()-1])) code += " ";

	string index;
	index = code[0];

	int flag = -1;
	for( unsigned int i=1; i<code.length(); i++)
    {
		if( isSplitChar(code[i]) )
		{
			index += code[i];

			CodeMap::iterator t = _typeMap.find(index);
			if( t != _typeMap.end() )
            {
				CodeType ct = t->second;
				string val = code.substr(1, i-1);
				debugout("cut:[%s]" , val.c_str());

				setInfo(val,ct,result);
				flag = i;
				break;
			}
			else
            {
				index = code[i];
				continue;
			}
		}
	}

	if( flag == -1 ) return 0;

	code = code.substr(flag, code.length()-flag);
	cout << "rest:" << code << endl;

	split(code,result);
    */

    string s = code;
    vector<string> _session;

	if(s[0]=='D') s[0]=' ';

	// 将除'-'字符外的非数字字符替换为空格
	for(int i=0; i<s.length(); i++)
	{
		if(s[i] == 'D') s[i] = '-';
		
		if( (s[i]<'0' || s[i]>'9') && s[i]!='-' ) s[i] = ' ';
	}

    istringstream is(s);

	string str[5];
	str[4]="0";


	try{
		int i = 0;
		while( !is.eof() && i<5 )
		{
			string item;
			is >> item;
			if(i == 0 && item.length() == 1) continue;

			if( item.length()>1 ) str[i] = item;
			else continue;
			debugout("str [%d] [%s]\n",i,str[i].c_str());
			++i;
		}    

		if(i == 4)
        {
            if(str[0].length()>6)
            {
                debugout("凭证号缺失");
			    str[4]= str[3];
			    str[3]= str[2];
			    str[2]= str[1];
			    str[1]= str[0];
                str[0]= "000000";
            }
            else if(str[2].length()==2)
            {
                debugout("账号缺失");
                str[4]=str[3];
                str[3]=str[2];
			    str[2]="00000000";    
            }
            else
            {
                debugout("类型缺失");
                str[4]=str[3];
			    str[3]="00";      
            }
        }

        if(i == 3)
        {
            if(str[0].length()>6)
            {
                debugout("凭证号缺失");
                if( str[1].length()==2)
                {
                    debugout("账号缺失");
			        str[4]= str[2];
			        str[3]= str[1];
			        str[2]= "0000000000";
			        str[1]= str[0];                     
                    str[0]= "000000";
                }
                else
                {
                    debugout("类型缺失");
                    str[4]= str[2];
			        str[2]= str[1];
			        str[3]= "00";
			        str[1]= str[0];                    
                    str[0]= "000000";
                }
            }
            else
            {
                    debugout("账号缺失");
                    debugout("类型缺失");
                    
                    str[4]= str[2];
			        str[3]= "00";
			        str[2]= "0000000000";
            }
        }           
	
		for(int j=0; j<5; j++) _session.push_back(str[j]);    

		if( _session.size()<3 ) zreturn(-3);

		if( _session[1].length()<4 || _session[2].length()<8 )
			zreturn(-4);

	}
	catch(...)
    {
	} 

    result.vchno    = _session[0];
    result.exchno   = _session[1];
    result.accno    = _session[2];
    result.vchtp    = _session[3];
    result.money    = _session[4];   

	return 0;
}

/**
    判断是否是分割符
*/
bool SplitDc::isSplitChar(char ch)
{
	string split_chars = "<:;,/ ";
	int ret = split_chars.find(ch);

	cout << "Find: " << ret << " ";

	return (ret!=-1);
}

/**
    设置分割后的各域值
*/
void SplitDc::setInfo(string &val, CodeType ct, DcInfo &info)
{

	if( ct == CT_VCHNO ) {
		info.vchno = val;
	}
	else if( ct == CT_EXCHNO ) {
        if(val[0]==':' )
            val = val.c_str()+1;
		info.exchno = val;
	}
	else if( ct == CT_ACCNO ) {
		info.accno = val;
	}
	else if( ct == CT_MONEY ) {
        if(val.length() <8) return ;
		info.money = val;
	}

	else if( ct == CT_VCHTP ) {
		info.vchtp = val;
	}
}
