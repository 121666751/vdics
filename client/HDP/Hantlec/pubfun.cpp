#include "pubfun.h"
#include <zsocket.hpp>
#include <AnsiMemfile.h>
#include <zcommon.h>
#include <utility/xmlpp.h>
using namespace xml;


int parsecd(char* vt)
{
    return  (vt[0]-'0')%2;
}

char* pvb(int vt,char* bt)
{
    switch(vt)
    {
        case 12:
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
            strcpy(bt ,"XX1");
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

String makeTaskBody(distTaskT body)
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
    memFile.Write("\0", 1, 1, false);
    return (char*)memFile.GetPtr();
}

String makeSignBody(char*brno,char*areano,char*accdate,char*session)
{
    CAnsiMemFile memFile;
    memFile.Write(brno,6,strlen(brno),false);
    memFile.Write(accdate,8,strlen(accdate),false);
    memFile.Write(areano,6,strlen(areano),false);
    memFile.Write(session,3,strlen(session),true,'0');
    memFile.Write("\0", 1, 1, false);
    return (char*)memFile.GetPtr();
}

int doTrade(char* sends,char *ip,int port,int  &result,char*response,char*outstr)
{
    char send[1024];
    memset(send,0,sizeof(send));
    strcpy(send,sends);

    ZSocket so;
    sprintf(send,"%08d%s",strlen(send),sends);
    int iRstLen =so.Connect(ip,port);
    if(iRstLen!= 0 )
    {
        debugout("Socket连接失败[%s][%d]\n", ip,port);
        return iRstLen;
    }

    iRstLen = so.Send(send,strlen(send));
    if(iRstLen< 0 )
    {
        debugout("发送数据失败[%d]\n", iRstLen);
        return iRstLen;
    }

    debugout("send ok [%d][%s] ...",iRstLen,send);
    char out[512];
    memset(out,0,sizeof(out));
    iRstLen = so.Recv(out,512);
    if(iRstLen< 0 )
    {
        debugout("接收数据失败[%d]\n", iRstLen);
        return iRstLen;
    }
    so.Close();
    debugout("socket recive...[%s]\n",out);

    char r  = out[11];
    if( r=='N' ) result = 0;
    else if( r == 'A' ) result = 1;
    else if( r == 'C' ) result = 2;
    else    result = -1;

    strncpy(response,out+12,6);
    if(r!='N' && strlen(out)>126)
    {
        strcpy(outstr,out+126);
    }
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
    memFile.Write(body.vchno,8,strlen(body.vchno),true);
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
        debugout("key [%s] value[%s]\n",key.c_str(),outxml[key.c_str()]);
        cur++;
	}
    return outxml;
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
