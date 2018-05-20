#include "pubfun.h"
#include <zsocket.hpp>
#include <AnsiMemfile.h>
#include <zcommon.h>
#include <utility/xmlpp.h>
using namespace xml;

char* myTrim(char* s)
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
int parsecd(char* vt)
{
    return  (vt[0]-'0')%2;
}

char* pvb(int vt,char* bt)
{
    switch(vt)
    {
        /*case 12:
            strcpy(bt ,"115");
            break;
        case 62:
            strcpy(bt ,"123");
            break;
        case 13:
            strcpy(bt ,"117");
            break;
        case 14:
            strcpy(bt ,"129");
            break;
        case 15:
            strcpy(bt ,"XX1");
            break;
        case 65:
            strcpy(bt ,"000");
            break;
        case 99:
            strcpy(bt ,"XX2");
            break;
        default :
            strcpy(bt ,"000");
            break;
        */
        case 00:
            strcpy(bt ,"115");
            break;
        case 01:
            strcpy(bt ,"123");
            break;
        case 02:
            strcpy(bt ,"117");
            break;
        case 16:
            strcpy(bt ,"XX1");
            break;
        case 25:
            strcpy(bt ,"000");
            break;
        case 99:
            strcpy(bt ,"XX2");
            break;
        default :
            strcpy(bt ,"000");
            break;
    }
    return bt;
}
String makeHead(char* mode)
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

String makeTaskBody(distTaskT body,String modeType)
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
    memFile.Write(modeType.c_str(),1,1,false);
    memFile.Write("\0", 1, 1, false);
    return (char*)memFile.GetPtr();
}

String makeQueryDs(const char* orgCode,const char* optCode,const char* brno,const char*date,const char*session,const char*area,const char*exchno)
{

	CAnsiMemFile memFile;
	memFile.Write(orgCode, 11, strlen(orgCode), false);
	memFile.Write(optCode, 7, strlen(optCode), true,' ');
	memFile.Write(brno, 11, strlen(brno), false);
	memFile.Write(date,8,strlen(date),false);
	memFile.Write(area,6,strlen(area),false);
	//memFile.Write(exchno,12,strlen(exchno),true,'0');
	memFile.Write(session,3,strlen(session),true,'0');
	memFile.Write("\0", 1, 1, false);
	return (char*)memFile.GetPtr();
	/*
    CAnsiMemFile memFile;
    memFile.Write(" ", 4, 1, false);
    memFile.Write(ZM_DIST_CHECK, 6, 6, false);
    memFile.Write(" ", 40, 1, false);
    memFile.Write("T00", 3, 3, false);
    memFile.Write(" ", 118, 1, false);
    memFile.Write(date,8,strlen(date),false);
    memFile.Write(area,6,strlen(area),false);
    memFile.Write(exchno,12,strlen(exchno),true,'0');
    memFile.Write(session,3,strlen(session),true,'0');
    memFile.Write("\0", 1, 1, false);
    return (char*)memFile.GetPtr();
	*/
}

String makeTaskBodyEx(distTaskT body,String acc,String vchno,String amt,String modeType)
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
    memFile.Write(acc.c_str(),21,acc.Length(),true);
    memFile.Write(vchno.c_str(),30,vchno.Length(),false);
    memFile.Write(amt.c_str(),15,amt.Length(),true,'0');
    memFile.Write("N",1,1,false);
    memFile.Write(modeType.c_str(), 1, 1, false);
    return (char*)memFile.GetPtr();
}


int myECM(MemT in,char *ip,int port,char*pout)
{
    char lenstr[12];

    memset(lenstr,0,sizeof(lenstr));

    ZSocket so;
    sprintf(lenstr,"%08d",in.size());
        
    String sendstr;
    sendstr.sprintf("%08d%s",in.size(),(char*)in.data());

    int iRstLen =so.Connect(ip,port);
    if(iRstLen!= 0 )
    {
        debugout("Socket连接失败[%s][%d]\n", ip,port);
        return iRstLen;
    }      

    iRstLen = so.Send(sendstr.c_str(),in.size()+8);
    if(iRstLen< 0 )
    {
        debugout("发送数据失败[%d]\n", iRstLen);
        so.Close();
        return iRstLen;
    }

    char out[8896];
    memset(out,0,sizeof(out));
    iRstLen = so.RecvEx(out,8);
    if(iRstLen< 0 )
    {
        debugout("接收长度失败[%d]\n", iRstLen);
        so.Close();
        return iRstLen;
    }

    
    int len = atoi(out);

    memset(out,0,sizeof(out));
    iRstLen = so.RecvEx(out,len);
    if(iRstLen< 0 )
    {
        debugout("接收数据失败[%d]\n", iRstLen);
        so.Close();
        return iRstLen;
    }

    so.Close();
    debugout("接收数据成功[%d]\n", iRstLen);
    strncpy(pout,out,iRstLen);
    return 0;
}
int doTrade(char* sends,char *ip,int port,int  &result,char*response,char*outstr)
{
    char send[1024];
    memset(send,0,sizeof(send));
    strcpy(send,sends);

    ZSocket so;
    sprintf(send,"%08d%s",strlen(sends),sends);
    int iRstLen =so.Connect(ip,port);
    if(iRstLen!= 0 )
    {
        debugout("Socket连接失败[%s][%d]\n", ip,port);
        return iRstLen;
    }

    
    iRstLen = so.Send(send,strlen(send));
    if(iRstLen<=0 )
    {
        debugout("发送数据失败[%d]\n", iRstLen);
        so.Close();
        return -1;
    }

    char out[512];
    memset(out,0,sizeof(out));
    iRstLen = so.RecvEx(out,512);
    if(iRstLen<=0 )
    {
        debugout("接收数据失败[%s][%d][%s][%d]\n", ip,port,send,iRstLen);
        so.Close();
        return -1;
    }

    debugout("接收数据[%s]\n", out);
    so.Close();

    char r  = out[11];
    if( r=='N' ) result = 0;
    else if( r == 'A' ) result = 1;
    else if( r == 'C' ) result = 2;
    else    result = -1;

    strncpy(response,out+12,6);
    if(strlen(out)>126)
    {
        strcpy(outstr,out+126);
    }

    debugout("报文[%s]\n", outstr);
    so.Close();
    return 0;
}

String makeDataBody(distDataT body)
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
    memFile.Write(body.vchno,30,strlen(body.vchno),true);
    memFile.Write(body.actamt,15,strlen(body.actamt),true,'0');
    memFile.Write(body.payact,21,strlen(body.payact),true);
    memFile.Write(" ",60,1,false);
    memFile.Write("\0", 1, 1, false);
    return (char*)memFile.GetPtr();
}
    

XmlMap parseOutXml(String xmlstr)
{
    XmlMap outxml;
    DocT doc; //定义解析对象
    if(doc.parseMemory(xmlstr.c_str()))
    { 
        return outxml;
    }

    NodeT node = doc.getRoot();	// 获取root节点
	if(node.name()!="Transaction")
    {
        return outxml;
    }

    node.child();	// 移动到子节点


	while( node.fetch() )
    {
        if(node.name()=="ESBHead")
        {
             NodeT node2 = node;
             node2.child();
             while(node2.fetch())
             {
                if(node2.name() == "text")
                {
                    continue;
                }
                else
                {
                    outxml[node2.name().c_str()] = node2.value().c_str();
                }
             }
        }

        if(node.name()=="ESBBody")
        {
             NodeT node2 = node;
             node2.child();
             while(node2.fetch())
             {
                if(node2.name() == "text")
                {
                    continue;
                }
                if( node2.name() == "AppResponse")
                {
                     NodeT node3 = node2;
                     node3.child();


                     while(node3.fetch())
                     {

                        if(node3.name()=="text")
                        {
                            continue;
                        }
                        else if (node3.name()=="AppRspHead")
                        {
                            NodeT node4 = node3;
                            node4.child();
                            while(node4.fetch())
                            {
                                if(node4.name()=="text")
                                {
                                    continue;
                                }
                                else if(node4.name() == "StatusInfo" )
                                {
                                    NodeT node5 = node4;
                                    node5.child();
                                    while(node5.fetch())
                                    {
                                        if(node5.name()=="text")
                                        {
                                            continue;
                                        }
                                        else
                                            outxml[node5.name().c_str()] = node5.value().c_str();
                                    }
                                }
                                else
                                {
                                    outxml[node4.name().c_str()] = node4.value().c_str();
                                }
                            }

                        }
                        else if (node3.name()=="AppRspBody")
                        {
                            NodeT node4 = node3;
                            node4.child();
                            while(node4.fetch())
                            {
                                if(node4.name()=="text")
                                {
                                    continue;
                                }
                                else if(node4.name() == "CommonRsp" || node4.name() == "BatchIndex")
                                {
                                    NodeT node5 = node4;
                                    node5.child();
                                    while(node5.fetch())
                                    {
                                        if(node5.name()=="text")
                                        {
                                            continue;
                                        }
                                        outxml[node5.name().c_str()] = node5.value().c_str();
                                    }
                                }
                                else if(node4.name() == "DocumentList" )
                                {
                                    NodeT node5 = node4;
                                    node5.child();
                                    while(node5.fetch())
                                    {
                                        if(node5.name()=="text")
                                        {
                                            continue;
                                        }

                                        else if(node5.name()== "Document")
                                        {
                                            NodeT node6 = node5;
                                            node6.child();
                                            while(node6.fetch())
                                            {
                                                if(node6.name()=="text")
                                                {
                                                    continue;
                                                }
                                                else if(node6.name() == "DocumentIndex")
                                                {
                                                    NodeT node7 = node6;
                                                    node7.child();
                                                    while(node7.fetch())
                                                    {
                                                        if(node7.name()=="text")
                                                        {
                                                            continue;
                                                        }
                                                        else
                                                            outxml[node7.name().c_str()] = node7.value().c_str();

                                                    }
                                                }

                                                else 
                                                {
                                                    outxml[node6.name().c_str()] = node6.value().c_str();
                                                }
                                            }
                                        }
                                    }
                                }

                            }
                        }
                        
                    }                   
                }
             }
         }
    }

    XmlMap::iterator cur = outxml.begin();
    while (cur!=outxml.end())
    {
        String key = cur->first;
        cur++;
	}
    return outxml;
}


int WriteToLogFile(char*clkno,char *msgtype,int id,const char* msg)
{
    debugout("save log ...");
	FILE* file = fopen("vdics.log", "a+");
	if (!file)
    {
        debugout("open file vdics.log error ...");
		return -1;
    }
    ZDate dt;
    dt.Now();
    String dd = dt.BankDateString().c_str();
    String hh = dt.TimeString().c_str();

    String dh = dd+hh;

    String show ;
    String cmsg;

    show = "时间["+dh+"]柜员["+clkno+"]交易["+msgtype+"]流水["+id+"]\n报文["+msg+"]\n";
	int ret = 0;
	if (fwrite(show.c_str(), show.Length(), 1, file) != strlen(msg))
		ret = -1;
	fwrite("\r\n", strlen("\r\n"), 1, file);
	
	fclose(file);

	return ret;
}
int deleteFiles(FileVector Files)
{
    FileVector::iterator cur=Files.begin();
    while(cur!= Files.end())
    {
        _unlink(cur->fileFullName);
        cur++;
    }
}

String makeUpdateXml(XmlMap keys,FileVector Files)
{
    String ObjectsList;

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
        <<"                 <TradeCode>UPDDOC</TradeCode>"<<endl
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
        <<"                 <DocumentList>"<<endl
        <<"                     <Document>"<<endl
        <<"                         <DocumentID>"<<keys["DocumentID"].c_str()<<"</DocumentID>"<<endl
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
String makeXml(XmlMap keys,FileVector Files)
{
    String ObjectsList;

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
