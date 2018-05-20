#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <zsocket.hpp>
#include <AnsiMemfile.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h> 
#include <stdint.h>
#include "ExportLog.h"
#include "PubFun.h"
#include "mtype.h"

#ifdef __OS_LINUX_VER
	#include <sys/sysinfo.h>	
	#include <sys/wait.h>
	#include <dirent.h>
#endif

//字符编码转换函数
int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	/*iconv_t cd;
	int rc;
	char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset, from_charset);

	if (cd == 0)
	return 1;

	memset(outbuf, 0, outlen);

	if (iconv(cd, pin, (size_t *)&inlen, pout, (size_t *)&outlen) == -1)
		return 1;

	iconv_close(cd);
	*/
	
	strcpy(outbuf,inbuf);
	outlen = inlen;
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// 函数实现

// -----------------------------------------------------------------------
// 删除字符串左右空格
char* Trim(char* s)
{
	if (s == NULL)
		return NULL;

	// 删除左边的空格
	int i, j, k = 0;
	i = strlen(s) + 1;
	
	for (j = 0; j < i; j++)
	{
		if (s[j] != ' ' && s[j] != 0x0A) 
			break;
	}
	
	for (k = 0; j < i; k++, j++) 
		s[k] = s[j];
	
	// 删除右边的空格
	i = strlen(s) - 1;
	for (j = i; j > -1; j--) 
	{
		if (s[j] != ' ' && s[j] != 0x0A) 
			break;
	}
	
	s[j + 1] = 0;
	
	return s;
}
bool isNumeric(char * str)
{
    //TODO: Add your source code here
    string tmpstr = str;
    int Len =  tmpstr.size();      
    for(int i=0;i<Len;i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

// -----------------------------------------------------------------------
// 删除字符串中的括号
char* DelBracket(char* s)
{
	int len = strlen(s);
	int i, j;
	
	for (i = 0, j = 0; i < len; ++i)
	{   
		if (s[i] != '(' && s[i] != ')' && i >= j)
		{   
			s[j++] = s[i];   
		}   
	}   
    
	s[j] = '\0';
	
	return s;
}

// -----------------------------------------------------------------------
// 字符串转大写
char* to_upper(char* str)
{
	char *temp = str;
	
	for (; (*str) != 0x00; str++)
	{
		if (*str >= 'a' && *str <= 'z')
			*str -= 32;
	}
	
	return temp;
}

// -----------------------------------------------------------------------
// 字符串转小写
char* to_lower(char* str)
{
	char *temp = str;
	
	for (; (*str) != 0x00; str++)
	{
		if (*str >= 'A' && *str <= 'Z')
			*str += 32;
	}
	
	return temp;
}


long gettickcount()
{
#ifndef __OS_LINUX_VER
	return GetTickCount();
#else
	struct sysinfo info;
	
	sysinfo(&info);
	
	return info.uptime;
#endif
}

void init_dameon()
{
//#ifdef __OS_LINUX_VER
	int pid;
	int i;
	if(pid=fork())
		exit(0);//是父进程，结束父进程
	else if(pid< 0)
		exit(1);//fork失败，退出

	//是第一子进程，后台继续执行
	setsid();//第一子进程成为新的会话组长和进程组长

//	for(i=0;i< NOFILE;++i)//关闭打开的文件描述符
//		close(i);
	
	//chdir("/tmp");//改变工作目录到/tmp
	umask(0);//重设文件创建掩模

	close(0);
	open("/dev/null", O_RDWR);
	dup2(0, 1);
	dup2(0, 2);

//	signal(SIGCHLD,SIG_IGN); 

	return;
//#endif
}

#define READ_BUF_SIZE 50

int find_pid_by_name(char* pidName, int &first_pid)
{
#ifdef __OS_LINUX_VER
    DIR *dir;
    struct dirent *next;
    long* pidList=NULL;
    int i=0;

    dir = opendir("/proc");
    if (!dir)
    {
		fprintf(stderr, "Cannot open /proc\n");
		exit(1);
    }
        	              	            
    while ((next = readdir(dir)) != NULL)
    {
		FILE *status;
        char filename[READ_BUF_SIZE];      
		char buffer[READ_BUF_SIZE];
        char name[READ_BUF_SIZE];

		// Must skip ".." since that is outside /proc
	    if (strcmp(next->d_name, "..") == 0)
	    	continue;

		// If it isn't a number, we don't want it
        if (!isdigit(*next->d_name))
	    	continue;

        sprintf(filename, "/proc/%s/status", next->d_name);
	    if (! (status = fopen(filename, "r")))
			continue;

		// Read first line in /proc/?pid?/status
        if (fgets(buffer, READ_BUF_SIZE-1, status) == NULL)
        {
			fclose(status);
		    continue;
	    }
		fclose(status);

		// Buffer should contain a string like "Name:   binary_name"
        sscanf(buffer, "%*s %s", name);
	    if (strcmp(name, pidName) == 0)
	    {
	    	if (i == 0)
	    		first_pid = strtol(next->d_name, NULL, 0);
	    		
	    	i++;
		}
     }
	closedir(dir);

    return i;
#else
	return 0;
#endif
}

string makeHead(char* mode)
{
    CAnsiMemFile memFile;
    memFile.Write(" ", 4, 1, false);
    memFile.Write(mode, 6, strlen(mode), false);
    memFile.Write(" ", 40, 1, false);
    memFile.Write("T00", 3, 3, false);
    memFile.Write(" ", 118, 1, false);
    memFile.Write("\0", 1, 1, false);
    return (char*)memFile.GetPtr();
}


//451600交易
string makeTaskBody(distTaskT body)
{
    CAnsiMemFile memFile;
    memFile.Write(body.orgcode,6,strlen(body.orgcode),false);
    memFile.Write(body.clkno,7,strlen(body.clkno),false);
    memFile.Write(body.imageid,32,strlen(body.imageid),true,' ');
    memFile.Write(body.pageno,10,strlen(body.pageno),true,' ');
    memFile.Write(body.brno,11,strlen(body.brno),true,' ');
    memFile.Write(body.accdate,8,strlen(body.accdate),false);
    memFile.Write(body.areano,6,strlen(body.areano),false);
    memFile.Write(body.exchno,12,strlen(body.exchno),true,'0');
    memFile.Write(body.session,3,strlen(body.session),true,'0');
    memFile.Write(body.ccycode,3,strlen(body.ccycode),false);
    memFile.Write(body.imgtype,3,strlen(body.imgtype),false);
 //   memFile.Write(body.modeflag,1,strlen(body.modeflag),true,'0');
    if(strcmp(body.imgtype,"123")!=0)
    	memFile.Write("N",1,1,true,'0');
    else
	memFile.Write("O",1,1,true,'0');
    memFile.Write("\0", 1, 1, false);
    return (char*)memFile.GetPtr();
}

//451601交易
string makeTaskBodyEx(distTaskT body,string acc,string vchno,string amt)
{
    CAnsiMemFile memFile;
    memFile.Write(body.imageid,27,strlen(body.imageid),false);
    memFile.Write(body.brno,6,strlen(body.brno),false);
    memFile.Write(body.accdate,8,strlen(body.accdate),false);
    memFile.Write(body.areano,6,strlen(body.areano),false);
    memFile.Write(body.exchno,12,strlen(body.exchno),true,'0');
    memFile.Write(body.session,3,strlen(body.session),true,'0');
    memFile.Write(body.ccycode,3,strlen(body.ccycode),false);
    memFile.Write(body.imgtype,3,strlen(body.imgtype),false);
    memFile.Write(acc.c_str(),21,acc.size(),true);
    memFile.Write(vchno.c_str(),30,vchno.size(),false,' ');
    memFile.Write(amt.c_str(),15,amt.size(),true,'0');
//    memFile.Write(body.modeflag,1,strlen(body.modeflag),true,'0');
    if(strcmp(body.imgtype,"123")!=0)
    	memFile.Write("N",1,1,true,'0');
    else
	memFile.Write("O",1,1,true,'0');
    memFile.Write("\0", 1, 1, false);
    return (char*)memFile.GetPtr();
}

//556572交易 特色业务信封提交
string makeTaskBodyEx1(distTaskT body, string banktype, string elpId, string uplCnt,string uplAmt)
{
    CAnsiMemFile memFile;
    memFile.Write(body.accdate,8,strlen(body.accdate),false);
    memFile.Write(body.areano,6,strlen(body.areano),false);
    memFile.Write(body.exchno,12,strlen(body.exchno),true,'0');
    memFile.Write(body.session,3,strlen(body.session),true,'0');
    memFile.Write(banktype.c_str(),3,banktype.size(),true,'0');
    memFile.Write(elpId.c_str(),20,elpId.size(),true,'0');
    memFile.Write(uplCnt.c_str(),6,uplCnt.size(),true,'0');
    memFile.Write(uplAmt.c_str(),15,uplAmt.size(),true,'0');
    memFile.Write("\0", 1, 1, false);
    return (char*)memFile.GetPtr();
}

//556573交易 特色业务票据提交
string makeTaskBodyEx2(distTaskT body, string elpId, string imgId)
{
    CAnsiMemFile memFile;
    memFile.Write(body.accdate,8,strlen(body.accdate),false);
    memFile.Write(body.areano,6,strlen(body.areano),false);
    memFile.Write(body.exchno,12,strlen(body.exchno),true,'0');
    memFile.Write(body.session,3,strlen(body.session),true,'0');
    memFile.Write(elpId.c_str(),20,elpId.size(),true, '0');
    memFile.Write(imgId.c_str(),27,imgId.size(),false);
    memFile.Write("\0", 1, 1, false);
    return (char*)memFile.GetPtr();
}

int myECM(MemT in,const char *ip,const char* port,char*pout)
{
    char lenstr[12];
  
    memset(lenstr,0,sizeof(lenstr));

    ZSocket so;
    sprintf(lenstr,"%08d",in.size());
        
   
    CAnsiMemFile memFile;
	memFile.Write(lenstr, 8, 8, false);
    memFile.Write((char*)in.data(), in.size(), in.size(), false);
    memFile.Write("\0", 1, 1, false);
    
    

    int iRstLen =so.Connect(ip,atoi(port));
    if(iRstLen!= 0 )
    {     
        //LogPrint(VOUCHER_SEND_NODE, "Socket连接失败[%s][%s]\n", ip,port);
        return iRstLen;
    }      

    iRstLen = so.Send((void*)memFile.GetPtr(),memFile.GetLength());
    if(iRstLen< 0 )
    {
        //LogPrint(VOUCHER_SEND_NODE, "发送数据失败[%d]\n", iRstLen);
        so.Close();
        return iRstLen;
    }

    char out[8896];
    memset(out,0,sizeof(out));
	//RecvEx超过50秒 返回超时
    iRstLen = so.RecvEx(out,8);
    if(iRstLen< 0 )
    {
     	//LogPrint(VOUCHER_SEND_NODE, "接收长度失败[%d]\n", iRstLen);
        so.Close();
        return iRstLen;
    }
    
    int len = atoi(out);

    memset(out,0,sizeof(out));
    iRstLen = so.RecvEx(out,len);
    if(iRstLen< 0 )
    {
        //LogPrint(VOUCHER_SEND_NODE,"接收数据失败[%d]\n", iRstLen);
        so.Close();
        return iRstLen;
    }

    so.Close();
    strncpy(pout,out,iRstLen);
    return 0;
}
int doTrade(const char* sends,const char *ip,const char* port,int  &result,char*response,char*outstr)
{
	
    char send[1024];
    memset(send,0,sizeof(send));
    strcpy(send,sends);

    ZSocket so;
    sprintf(send,"%08d%s",strlen(sends),sends);
   
    int iRstLen =so.Connect(ip,atoi(port));
    if(iRstLen!= 0 )
    {
        LogPrint(VOUCHER_SEND_NODE, "Socket连接失败[%s][%s]\n", ip,port);
        return iRstLen;
    }

    iRstLen = so.Send(send,strlen(send));

	LogPrint(VOUCHER_SEND_NODE, "Send->[%s]\n", send);

    if(iRstLen<0 )
    {
        LogPrint(VOUCHER_SEND_NODE, "发送数据失败[%s][%d]\n",send, iRstLen);
        so.Close();
        return iRstLen;
    }

    char out[512];
    memset(out,0,sizeof(out));
	//RecvEx, 50秒超时
    iRstLen = so.RecvEx(out,512);
    if(iRstLen<= 0 )
    {
        LogPrint(VOUCHER_SEND_NODE, "接收数据失败[%s][%d]\n",send, iRstLen);
        so.Close();
        return -1;
    }
    so.Close();

    char r  = out[11];
    if( r=='N' ) result = 0;
    else if( r == 'A' ) result = 1;
    else if( r == 'C' ) result = 2;
    else    result = -1;

    strncpy(response,out+12,6);
    if(r!='N' && strlen(out)>126)
    {
        strncpy(outstr,out+126,100);
    }
    so.Close();

    return 0;
}

string makeDataBody(distDataT body)
{
    CAnsiMemFile memFile;
    memFile.Write(body.imageid,27,strlen(body.imageid),false);
    memFile.Write(body.accdate,8,strlen(body.accdate),false);
    memFile.Write(body.areano,6,strlen(body.areano),false);
    memFile.Write(body.exchno,12,strlen(body.exchno),true,'0');
    memFile.Write(body.session,3,strlen(body.session),true,'0');
    memFile.Write(body.ccycode,3,strlen(body.ccycode),false);
    memFile.Write(body.ovchtype,3,strlen(body.ovchtype),false);
    memFile.Write(body.brno,6,strlen(body.brno),false);
    memFile.Write(body.rpflg,1,strlen(body.rpflg),false);
    memFile.Write(body.vchtype,3,strlen(body.vchtype),false);
    memFile.Write(body.vchno,8,strlen(body.vchno),true);
    memFile.Write(body.actamt,15,strlen(body.actamt),true,'0');
    memFile.Write(body.payact,21,strlen(body.payact),true);
    memFile.Write(" ",60,1,false);
    memFile.Write("\0", 1, 1, false);
    return (char*)memFile.GetPtr();
}

int get_xml_values(xmlDocPtr doc,xmlNodePtr root,xmlChar *childname,xmlChar *columnname,xmlChar **col_value)
{
	xmlNodePtr cur=root,temp;
	cur=cur->xmlChildrenNode;
	while(cur!=NULL)
	{
		temp=cur;
		if(!xmlStrcmp(cur->name,childname))
		{
			cur=cur->xmlChildrenNode;
			while(cur!=NULL)
			{
				if(!xmlStrcmp(cur->name,columnname))
				{
					*col_value=xmlNodeListGetString(doc,cur->xmlChildrenNode,1);
					return 0;
				}
				cur=cur->next;
			}			
		}
		cur=temp->next;
	}
	return -3;
}
//----------------------------------------------------------------------------------------
int get_subs_from_xml(string& buf,const char* element, string& substr)
{
	string b,e;
	b = string("<")+element+">";
	e = string("</")+element+">";
	substr = "";

	int ret;

	if( (ret = buf.find(b)) == -1 ) {
		fprintf(stderr,"ERR: %s:%d",__FILE__,__LINE__);
		//LogPrint(VOUCHER_SEND_NODE, "错误的xml");
		return -1;
	}

	int start = ret+b.length();
	if( (ret = buf.find(e)) == -1 || ret < start ) {
		//LogPrint(VOUCHER_SEND_NODE, "错误的xml");
		return -1;
	}

	if( ret > start ) substr = buf.substr(start, ret-start);

	return 0;
}
 
KeyMap parseOutXml(string xmlstr)
{
    KeyMap outxml;
	xmlDocPtr doc;
	xmlNodePtr cur;
	xmlChar *col_value;
	
	string errstr;
	int err_no = 0;
	
	char *xmldata;
	char *xml;
	//LogPrint(VOUCHER_SEND_NODE, "解析xml");
	
	string stpstr;
	string imgstr;
	xml=strstr((const char*)xmlstr.c_str(),"<?xml");
	if(xml==NULL) xml=(char *)xmlstr.c_str();
	
	doc=xmlParseMemory(xml,strlen(xml));
	if(doc==NULL)
	{
		//LogPrint(VOUCHER_SEND_NODE, "错误的xml");
		xmlFree(doc);
		return outxml;
	}	
	cur=xmlDocGetRootElement(doc);
	if(cur==NULL)
	{
		//LogPrint(VOUCHER_SEND_NODE, "空xml");
		xmlFree(doc);
		return outxml;
	}


	xmlNodePtr Father;
	cur=cur->xmlChildrenNode;
	

	while(cur!=NULL)
	{
		Father =cur;		

		if(!xmlStrcmp(cur->name,(xmlChar *)"ESBHead"))
		{
			cur=cur->xmlChildrenNode;
			while(cur!=NULL)
			{
				if(xmlStrcmp(cur->name,(xmlChar *)"text") && xmlStrcmp(cur->name,(xmlChar *)"comment") )
				{		
					  col_value = xmlNodeListGetString(doc,cur->xmlChildrenNode,1);
					  if(col_value!=NULL) 
					  	outxml[(char*)cur->name] = (char*)col_value;
					  else
					  	outxml[(char*)cur->name] = "";
					  xmlFree(col_value);
				}
				
				cur=cur->next;
			}			
		}
		else if(!xmlStrcmp(cur->name,(xmlChar *)"ESBBody"))
		{	
			cur=cur->xmlChildrenNode;
			
			while(cur!=NULL)
			{	
				xmlNodePtr Father2 = cur;
				if(!xmlStrcmp(cur->name,(xmlChar *)"AppResponse"))	
				{
					xmlNodePtr Father3 = cur;	
					cur=cur->xmlChildrenNode;
					while(cur!=NULL)
					{
						xmlNodePtr Father3 = cur;
						if(!xmlStrcmp(cur->name,(xmlChar *)"AppRspHead"))
						{									
							cur=cur->xmlChildrenNode;
							while(cur!=NULL)
							{
								xmlNodePtr Father4 = cur;
								if(!xmlStrcmp(cur->name,(xmlChar *)"StatusInfo"))	
								{
									
									cur=cur->xmlChildrenNode;	
									while(cur!=NULL)
									{
										xmlNodePtr Father5 = cur;	
										if(xmlStrcmp(cur->name,(xmlChar *)"text") && xmlStrcmp(cur->name,(xmlChar *)"comment") )
										{
											col_value = xmlNodeListGetString(doc,cur->xmlChildrenNode,1);
					  						if(col_value!=NULL) 
					  							outxml[(char*)cur->name] = (char*)col_value;
					  						else
					  							outxml[(char*)cur->name] = "";
					  						xmlFree(col_value);
										}
										cur = Father5->next;				
									}
								}
								else if(xmlStrcmp(cur->name,(xmlChar *)"text") && xmlStrcmp(cur->name,(xmlChar *)"comment") )
								{
									col_value = xmlNodeListGetString(doc,cur->xmlChildrenNode,1);
					  				//LogPrint(VOUCHER_SEND_NODE, "[%s][%s]",(char*)cur->name,col_value);
					  				if(col_value!=NULL) 
					  					outxml[(char*)cur->name] = (char*)col_value;
					  				else
					  					outxml[(char*)cur->name] = "";
					  				xmlFree(col_value);
								}
								cur = Father4->next;
							}
							
						}
						else if(!xmlStrcmp(cur->name,(xmlChar *)"AppRspBody"))
						{
							
							cur=cur->xmlChildrenNode;
							while(cur!=NULL)
							{
								xmlNodePtr Father4 = cur;	
								if(!xmlStrcmp(cur->name,(xmlChar *)"CommonRsp") || !xmlStrcmp(cur->name,(xmlChar *)"BatchIndex"))
								{
									
									cur=cur->xmlChildrenNode;	
									while(cur!=NULL)
									{
										xmlNodePtr Father5 = cur;	
										if(xmlStrcmp(cur->name,(xmlChar *)"text") && xmlStrcmp(cur->name,(xmlChar *)"comment") )
										{
											col_value = xmlNodeListGetString(doc,cur->xmlChildrenNode,1);
					  						if(col_value!=NULL) 
					  							outxml[(char*)cur->name] = (char*)col_value;
					  						else
					  							outxml[(char*)cur->name] = "";
					  						xmlFree(col_value);
										}
										cur = cur->next;				
									}
								}
								else if(!xmlStrcmp(cur->name,(xmlChar *)"DocumentList"))
								{
										
									cur=cur->xmlChildrenNode;	
									while(cur!=NULL)
									{
										xmlNodePtr Father5 = cur;
										if( !xmlStrcmp(cur->name,(xmlChar *)"Document"))
										{
												
											cur=cur->xmlChildrenNode;	
											while(cur!=NULL)
											{												
												xmlNodePtr Father6 = cur;	
												if( !xmlStrcmp(cur->name,(xmlChar *)"DocumentIndex"))
												{
													cur=cur->xmlChildrenNode;
													while(cur!=NULL)
													{
														xmlNodePtr Father7 = cur;	
														if(xmlStrcmp(cur->name,(xmlChar *)"text") && xmlStrcmp(cur->name,(xmlChar *)"comment") )
														{
															col_value = xmlNodeListGetString(doc,cur->xmlChildrenNode,1);
					  										if(col_value!=NULL) 
					  											outxml[(char*)cur->name] = (char*)col_value;
					  										else
					  											outxml[(char*)cur->name] = "";
					  										xmlFree(col_value);
														}
														cur = Father7->next;				
													}		
												}
												else if(xmlStrcmp(cur->name,(xmlChar *)"text") && xmlStrcmp(cur->name,(xmlChar *)"comment") )
												{
													col_value = xmlNodeListGetString(doc,cur->xmlChildrenNode,1);
					  								if(col_value!=NULL) 
					  									outxml[(char*)cur->name] = (char*)col_value;
					  								else
					  									outxml[(char*)cur->name] = "";
					  								xmlFree(col_value);
												}
												cur = Father6->next;	
											}
												
										}
										cur = Father5->next;				
									}											
								}
								cur = Father4->next;
							}
							
						}
						cur = Father3->next;
					}	
				}
			
				cur=Father2->next;
			}
			
		}
		cur=Father->next;
	}


	/*KeyMap::iterator b = outxml.begin();
    while (b!=outxml.end())
    {
        string key = b->first;
        //LogPrint(VOUCHER_SEND_NODE, "key[%s] value[%s]",key.c_str(),outxml[key].c_str());
        b++;
        
	}*/
    return outxml;
}
 
int deleteFiles(FileVector Files)
{
    FileVector::iterator cur=Files.begin();
    while(cur!= Files.end())
    {
        remove(cur->fileFullName);
        cur++;
    }
}
string makeXml(KeyMap keys,FileVector Files)
{
    string ObjectsList;

    FileVector::iterator cur=Files.begin();
    while(cur!= Files.end())
    {
        ObjectsList=ObjectsList+"<ResourceObjectContent>\n";
        ObjectsList=ObjectsList+"                               <ResourceObjectName>"+ cur->fileName + "</ResourceObjectName>\n";
        ObjectsList=ObjectsList+"                               <ResourceObjectFullName>"+ cur->fileFullName + "</ResourceObjectFullName>\n";
        ObjectsList=ObjectsList+"                             </ResourceObjectContent>";
        cur++;
    }

    ostringstream os;
    os  <<"<?xml version='1.0' encoding='GBK'?>"<<endl
        <<"<Transaction>"<<endl
        <<"     <ESBHead>"<<endl
        <<"         <RequesterId>DST001</RequesterId>"<<endl
        <<"         <ESBServiceId>ECM001</ESBServiceId>"<<endl
        <<"         <MessageType>1</MessageType>"<<endl
        <<"         <ESBReqTimestamp>"<<keys["ESBReqTimestamp"].c_str()<<"</ESBReqTimestamp>"<<endl
        <<"         <ESBRspTimestamp></ESBRspTimestamp>"<<endl
        <<"         <Version>1.0</Version>"<<endl
        <<"         <Reserved></Reserved>"<<endl
        <<"     </ESBHead>"<<endl
        <<"     <ESBBody>"<<endl
        <<"         <AppRequest>"<<endl
        <<"             <AppReqHead>"<<endl
        <<"                 <TradeCode>CREATE</TradeCode>"<<endl
        <<"                 <ReqSerialNo></ReqSerialNo>"<<endl
        <<"                 <TradeDate>"<<keys["TradeDate"].c_str()<<"</TradeDate>"<<endl
        <<"                 <TradeTime>"<<keys["TradeTime"].c_str()<<"</TradeTime>"<<endl
        <<"                 <TradeSource>"<<endl
        <<"                     <EnterpriseId>BOCOM</EnterpriseId>"<<endl
        <<"                     <TradeChannel>9</TradeChannel>"<<endl
        <<"                     <SubBankNo>"<<keys["SubBankNo"].c_str()<<"</SubBankNo>"<<endl
        <<"                     <OrganNo></OrganNo>"<<endl
        <<"                     <TellerNo>"<<keys["TellerNo"].c_str()<<"</TellerNo>"<<endl
        <<"                     <TermNo>DST001</TermNo>"<<endl
        <<"                 </TradeSource>"<<endl
        <<"                 <AuthInfo>"<<endl
        <<"                     <AuthLevel></AuthLevel>"<<endl
        <<"                     <Supervisor1></Supervisor1>"<<endl
        <<"                     <Supervisor2></Supervisor2>"<<endl
        <<"                     <Supv1Passwd></Supv1Passwd>"<<endl
        <<"                     <Supv2Passwd></Supv2Passwd>"<<endl
        <<"                     <AuthReason></AuthReason>"<<endl
        <<"                 </AuthInfo>"<<endl
        <<"                 <Reserved></Reserved>"<<endl
        <<"             </AppReqHead>"<<endl
        <<"             <AppReqBody>"<<endl
        <<"                 <CommonReq>"<<endl
        <<"                     <RequestLevel>1</RequestLevel>"<<endl
        <<"                     <TotalSize>"<<keys["TotalSize"].c_str()<<"</TotalSize>"<<endl
        <<"                     <EventID>"<<keys["EventID"].c_str()<<"</EventID>"<<endl
        <<"                     <EventType>1</EventType>"<<endl
        <<"                 </CommonReq>"<<endl
        <<"                 <BatchIndex>"<<endl
        <<"                     <AccountDate>"<<keys["AccountDate"].c_str()<<"</AccountDate>"<<endl
        <<"                     <ExchangeArea>"<<keys["ExchangeArea"].c_str()<<"</ExchangeArea>"<<endl
        <<"                     <ExchangeNo>"<<keys["ExchangeNo"].c_str()<<"</ExchangeNo>"<<endl
        <<"                     <ExchangeScene>"<<keys["ExchangeScene"].c_str()<<"</ExchangeScene>"<<endl
        <<"                     <Currency>CNY</Currency>"<<endl
        <<"                     <AttachType>"<<keys["AttachType"].c_str()<<"</AttachType>"<<endl
        <<"                 </BatchIndex>"<<endl
        <<"                 <DocumentList>"<<endl
        <<"                     <Document>"<<endl
        <<"                         <DocumentID></DocumentID>"<<endl
        <<"                         <DocumentType>01</DocumentType>"<<endl
        <<"                         <BizType>002</BizType>"<<endl
        <<"                         <CaseID>"<<keys["CaseID"].c_str()<<"</CaseID>"<<endl
        <<"                         <AttrType>3</AttrType>"<<endl
        <<"                         <DocumentIndex>"<<endl
        <<"                             <BillType>"<<keys["BillType"].c_str()<<"</BillType>"<<endl
        <<"                         </DocumentIndex>"<<endl
        <<"                         <ResourceObjectList>"<<endl
        <<"                             "<<ObjectsList.c_str()<<endl
        <<"                         </ResourceObjectList>"<<endl 
        <<"                     </Document>"<<endl
        <<"                 </DocumentList>"<<endl
        <<"             </AppReqBody>"<<endl
        <<"         </AppRequest>"<<endl
        <<"     </ESBBody>"<<endl
        <<" </Transaction>"<<endl;

        return os.str().c_str();
}

