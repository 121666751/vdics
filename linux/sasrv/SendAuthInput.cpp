
#include <string.h>
#include <sstream>
#include <mem.h>
#include <transimg.h>
#include <ZTime.hpp>

#include "SendAuthInput.h"

#include <ConfigFile.h>
#include <PubFun.h>
#include <ExportLog.h>
#include <Dboperate.h>
#include <gsipcom.h>
#include <package.h>
#include <gsip_util.h>
#include <zsocket.hpp>
#include <sys/time.h>

#define TASK_COUNT 100

int verbose;
static int task_count = 0;
//////////////////////////////////////////////////////////////////////////
// 上传类实现

// -----------------------------------------------------------------------
CSENDA::CSENDA()
{
	bzero(TaskIp,sizeof(TaskIp));
	bzero(DbHost,sizeof(DbHost));
	bzero(DbUser,sizeof(DbUser));
	bzero(DbPswd,sizeof(DbPswd));
	bzero(DbName,sizeof(DbName));
	try
	{
		LogPrint( AUTHINPUT_SEND_NODE, GSIP_VERSION);		
		Config mycfg("./config/config.cfg");
		Config bj_bank("./config/bj_bank.cfg");

		string szTmp;
		szTmp=mycfg.value("task_ip",1);
      	if(szTmp.size()>0)
      	{
         	strcpy(TaskIp,szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(AUTHINPUT_SEND_NODE, "读取调度服务IP错误!");
      		printf( "读取调度服务IP错误!");
			exit(-1);
      	}
      	szTmp=mycfg.value("task_port",1);
      	if(szTmp.size()>0)
      	{
      		TaskPort = stoi(szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(AUTHINPUT_SEND_NODE, "读取调度服务端口错误!");
			printf("读取调度服务端口错误！");
			exit(-1);
      	}
      
      	szTmp=mycfg.value("db_host",1);
      	if(szTmp.size()>0)
      	{
      		strcpy(DbHost,szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(AUTHINPUT_SEND_NODE, "读数据库地址错误!");
			printf("读取数据库地址错误!");
			exit(-1);
      	}
      
     	szTmp=mycfg.value("db_uid",1);
      	if(szTmp.size()>0)
      	{
           strcpy(DbUser,szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(AUTHINPUT_SEND_NODE, "读数据库用户名错误!");
			printf("读取数据库用户名错误!");
			exit(-1);
      	}
      
      	szTmp=mycfg.value("db_pwd",1);
      	if(szTmp.size()>0)
      	{
      		strcpy(DbPswd,szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(AUTHINPUT_SEND_NODE, "读数据库用户密码错误!");
			printf("读数据库用户密码错误!");
			exit(-1);
      	}
      
      	szTmp=mycfg.value("db_dbAlias",1);
      	if(szTmp.size()>0)
      	{
           	strcpy(DbName,szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(AUTHINPUT_SEND_NODE, "读数据库实例错误!");
			printf("读数据库实例错误!");
			exit(-1);
      	}
      	
      	szTmp=mycfg.value("imagepath",1);
      	if(szTmp.size()>0)
      	{
           	strcpy(ImagePath,szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(AUTHINPUT_SEND_NODE, "读图像存放目录错误!");
			printf("读图像存放目录错误!");
			exit(-1);
      	}


		szTmp=bj_bank.value("banktype_fs",1);
      	if(szTmp.size()>0)
      	{
           	strcpy(banktype_fs,szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(AUTHINPUT_SEND_NODE, "读取非税配置参数错误!");
			printf("读取非税配置参数错误!");
			exit(-1);
      	}

		szTmp=bj_bank.value("banktype_ws",1);
      	if(szTmp.size()>0)
      	{
           	strcpy(banktype_ws,szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(AUTHINPUT_SEND_NODE, "读取委收配置参数错误!");
			printf("读取委收配置参数错误!");
			exit(-1);
      	}
    }
    catch(...)
    {
   		LogPrint(AUTHINPUT_SEND_NODE, "读配置错误!");
		printf("读取配置文件错误!");
		exit(-1);
    }     
}

// -----------------------------------------------------------------------
CSENDA::~CSENDA()
{

}

// -----------------------------------------------------------------------
int CSENDA::getTaskId()
{
	memset(TaskId,'\0',sizeof(TaskId));

	if(verbose)
		LogPrint(AUTHINPUT_SEND_NODE, "发送上传任务请求!");
	
	ZSocket so;
	int ret = so.Connect(TaskIp,TaskPort);
	if(ret)
	{	
		LogPrint(AUTHINPUT_SEND_NODE, "连接调度任务出错[%s][%d]!",TaskIp,TaskPort);
		return ret;
	}
	//委收上传 1004/1005
	ret = so.Send((void*)"00000006031014",14);
  	if(ret<=0)
	{
		LogPrint(AUTHINPUT_SEND_NODE, "发送上传任务请求失败!");
		sleep(3);
		return -1;
	}
	
  	char cmd[512];
  	char str[8];
  
  	memset(cmd,0,sizeof(cmd));
  	memset(str,0,sizeof(str));
  
  
  	ret =  so.RecvEx(cmd,8);
  	if(ret!=8)
  	{
  		LogPrint(AUTHINPUT_SEND_NODE, "接收任务请求失败!");
		sleep(3);
  		return -1;
  	}
  	
  	int len = atoi(cmd);
  	memset(cmd,0,sizeof(cmd));
  	ret =  so.RecvEx(cmd,len);
  	if(ret!=len)
  	{
  		LogPrint(AUTHINPUT_SEND_NODE, "接收任务请求失败!");
		sleep(3);
  		return -1;
  	}
 
	if(task_count%20==0)
		LogPrint(AUTHINPUT_SEND_NODE, "收到消息[%d][%s]!",len,cmd);

	strncpy(str,cmd+PKG_TYPE_LEN+PKG_TRADECODE_LEN,PKG_RESULTCODE_LEN);
	
	
	if(strncmp(str,"0000",4) == 0)
	{
		strcpy(TaskId,cmd+PKG_TYPE_LEN+PKG_TRADECODE_LEN+PKG_RESULTCODE_LEN);
	}
	else
	{
		if(strncmp(str,TASK_RESULT_NONE,4) == 0)  
		{
			if(verbose)
				LogPrint(AUTHINPUT_SEND_NODE, "无上传任务!");
		}
		else
		{
			LogPrint(AUTHINPUT_SEND_NODE, "获取上传任务失败[%s]!",str);
			sleep(3);
		}		
		
		return -1;
	}	
	
	return 0;
}
// 开始上传
void CSENDA::Start(int debug)
{
	verbose = debug;

	if(verbose)
	{
		LogPrint(AUTHINPUT_SEND_NODE, "./sasrv -v");
	}
	else
	{
		LogPrint(AUTHINPUT_SEND_NODE, "./sasrv");
		init_dameon();
	}

	LogPrint(AUTHINPUT_SEND_NODE, "开始处理...");
	printf("version %s\n", GSIP_VERSION);
	LogPrint(AUTHINPUT_SEND_NODE, "version %s", GSIP_VERSION);
		 
	char fname[80];
	char bname[80];
	char newfname[128];
    char newbname[128];
	char senddesc[120];

	vector<string> va_sid;
	vector<string> va_fname;
	vector<string> va_bname;
	vector<string> va_imgid;
    
    KeyMap gloabXml;
	int     result;
    char Results[8896];
    
    char    cmd[8];
    char    cmdstr[512];
    
    int size = 0;   
    
    MemT imageF;
    MemT imageB;
	
	int ret = m_db.dbo_open(DbHost,DbUser,DbPswd,DbName);
	if(ret)
	{
		LogPrint(AUTHINPUT_SEND_NODE, "数据库连接出错[%d]!",ret);	
		return ;
	}			

	//获取配置信息	
	ret = get_config();
	if(ret)
	{
		LogPrint(AUTHINPUT_SEND_NODE, "get_config [%d]",ret);	
		return ;
	}

	while(true)
	{	
		//任务计数器		
		if( ++task_count > TASK_COUNT)
		{
			if(get_config())
			{
				task_count = 0;

				LogPrint(AUTHINPUT_SEND_NODE, "get_config [%d]",ret);	
				return ;
			}
		}

		memset(&_scanNode,0,sizeof(_scanNode));
		memset(fname,0,sizeof(fname));
		memset(bname,0,sizeof(bname));
		memset(newfname,0,sizeof(newfname));
		memset(newbname,0,sizeof(newbname));
		size = 0;   
		
		//获取任务
		ret = getTaskId();
		if(ret)
		{
			sleep(3);	
			continue;
		}		
		
		//获取明细
		va_fname.clear();
		va_bname.clear();
		va_imgid.clear();
		va_sid.clear();

		ret = m_db.dbo_get_sa_tasks(TaskId,_scanNode, va_fname, va_bname, va_imgid, va_sid);
		if(ret)
		{
			LogPrint(AUTHINPUT_SEND_NODE, "查询任务明细错误[%s]",TaskId);
			m_db.dbo_update_sa_state(TaskId,3);
			continue;
		}

		//委收
		if(_scanNode.multiflag==1)
		{
			strcpy(_scanNode.banktype,  banktype_ws);
		}
		//非税
		else if(_scanNode.multiflag==3)
		{
			strcpy(_scanNode.banktype,  banktype_fs);
		}	
		else
		{
			strcpy(_scanNode.banktype, "000");
		}	

		if(strlen(_scanNode.banktype)<2)
       		strcpy( _scanNode.banktype, "000");
        	
	int ncount;
	//取信封里的附件
	for( ncount=1; ncount < va_fname.size(); ncount++)
	{
		//根据影像ID长度判断是否上传过
		if( ( (va_imgid.at(ncount)).size() )<8 )
		{
			LogPrint( AUTHINPUT_SEND_NODE, "TaskId[%s] 票据ID[%s] 上传数量[%d] 第[%d]传 fname->[%s] imageid->[%s]", TaskId, va_sid.at(ncount).c_str(), va_fname.size()-1, ncount, va_fname.at(ncount).c_str(), va_imgid.at(ncount).c_str());

			//获取图像
			sprintf(fname,"%s//%02d//%05d//%s",_scanNode.date,atoi(_scanNode.sess),atoi(_scanNode.exchno),va_fname.at(0).c_str());
			sprintf(bname,"%s//%02d//%05d//%s",_scanNode.date,atoi(_scanNode.sess),atoi(_scanNode.exchno),va_bname.at(0).c_str());
			sprintf(newfname,"SAF%d.jpg",va_sid.at(ncount).c_str());
			sprintf(newbname,"SAB%d.jpg",va_sid.at(ncount).c_str());
				
			ret = ti_getfileformat_ex2(fname,"jpg",_scanNode.ipaddr,3210,99,_scanNode.rotate,1,imageF);
		    if(ret)
       	 	{
        		LogPrint(AUTHINPUT_SEND_NODE, "获取正面图像错误[%s][%d]!",fname,ret);	
        		m_db.dbo_update_sa_state_desc(va_sid.at(ncount).c_str(),3,"取图失败", -1, TaskId);
				sleep(5);
            	continue;
        	}
         
        	ret = ti_getfileformat_ex2(bname,"jpg",_scanNode.ipaddr,3210,80,_scanNode.rotate,1,imageB);
        	if(ret)
        	{
         		LogPrint(AUTHINPUT_SEND_NODE, "获取反面图像错误[%s][%d]!",bname,ret);	
            	m_db.dbo_update_sa_state_desc(va_sid.at(ncount).c_str(),3,"取图失败", -1, TaskId);
            	continue;
        	}
			LogPrint(AUTHINPUT_SEND_NODE, "获取图像成功");	
     
        	//组包XML
        	FileVector t;
        	distFileList node;
        	memset(&node,0,sizeof(node));
        	strcpy(node.fileName,newfname);
        	sprintf(node.fileFullName,"%s/%s",ImagePath,newfname);
	        t.push_back(node);
		
			imageF.saveToFile(node.fileFullName);
       	 	memset(&node,0,sizeof(node));
        	strcpy(node.fileName,newbname);
        	sprintf(node.fileFullName,"%s/%s",ImagePath,newbname);
        	t.push_back(node);
        
        	imageB.saveToFile(node.fileFullName);
        	size = size + imageF.size()+imageB.size();        
         
        	LogPrint(AUTHINPUT_SEND_NODE, "图像大小[%d]",size);	
        		
        	gloabXml["TotalSize"] = itos(size);
        	ZDate dt;
        	dt.Now();
        	string dd = dt.BankDateString().c_str();
        	string hh = dt.TimeString().c_str();
        	string id = va_sid.at(ncount).c_str();
        
        	char dh[12];
        	memset(dh,0,sizeof(dh));
        	strcpy(dh,hh.c_str());
        	strncpy(dh+2,hh.c_str()+3,2);
        	strncpy(dh+4,hh.c_str()+6,2);
        
        	hh = dh;

        	gloabXml["TradeDate"] = dd.c_str();
        	gloabXml["TradeTime"] = hh.c_str();
        	gloabXml["ESBReqTimestamp"] = (dd+hh).c_str();
        	gloabXml["AccountDate"]     = _scanNode.date;
			sprintf(_scanNode.sess, "%03d", atoi(_scanNode.sess));
        	gloabXml["ExchangeScene"]   =  _scanNode.sess;;
        	gloabXml["EventID"] = (dd+hh+id+"SAN001").c_str();
        	gloabXml["CaseID"] = (dd+hh+id+"SAN001").c_str();
        	gloabXml["SubBankNo"] = _cfgMap["brno"]; 
        	gloabXml["TellerNo"] = _scanNode.clkno;

			//exchno长度为12位
        	int n_zero = 12-strlen(_scanNode.pkgno);
			char tmp_pkgno[13];
			bzero(tmp_pkgno, sizeof(tmp_pkgno));
			while(n_zero>0)
			{   
    			strcat( tmp_pkgno,"0"); 
    			n_zero--;
			}   
			strcat( tmp_pkgno, _scanNode.pkgno); 
        	gloabXml["ExchangeNo"] = tmp_pkgno;
        	gloabXml["AttachType"] = "1";
        	gloabXml["ExchangeArea"] = _scanNode.area;


			gloabXml["BillType"] =  _scanNode.banktype;

        	string xmlstr = makeXml(gloabXml,t);
        	memset(Results,0,sizeof(Results));       
        	MemT xmlm;
       		xmlm.assign((unsigned char*)xmlstr.c_str(),xmlstr.size());
       	
       		//ECM通讯
			ret = myECM(xmlm, _cfgMap["ecmip"].c_str(),_cfgMap["ecmport"].c_str(),Results);		
			strncpy(senddesc, Results, 100);
		
			deleteFiles(t);
        	if(ret<0)
        	{
        		LogPrint(AUTHINPUT_SEND_NODE, "ECM上传图像失败[%d]!",ret);	
				if(ret==-4)
				{
					LogPrint(AUTHINPUT_SEND_NODE, "ECM应答超时");	
					strcpy(senddesc, "ECM应答超时");
				}
				else if(ret==-3)
				{
					LogPrint(AUTHINPUT_SEND_NODE, "ECM连接失败!");	
					strcpy(senddesc, "ECM连接失败");
				}
				else
				{
					LogPrint(AUTHINPUT_SEND_NODE, "ECM失败!");	
					strcpy(senddesc, "ECM失败");
				}

           		m_db.dbo_update_sa_state_desc(va_sid.at(ncount).c_str(), AUTHINPUT_SENDSTATE_ECM_FAIL, senddesc, ret, TaskId);
				sleep(3);
           		continue;
        	}

        	KeyMap  outmap;
        
        	//解析结果
        	outmap = parseOutXml(Results);
			memset(senddesc, '\0', sizeof(senddesc));
			strncpy(senddesc, Results, 100);

        	if( (outmap["MsgCode"]!="000000" &&  outmap["MsgCode"]!="0")||(outmap["CaseID"]!= gloabXml["CaseID"]) )
        	{
	       		LogPrint(AUTHINPUT_SEND_NODE, "ECM上传图像失败,原因[%s]",outmap["MsgInfo"].c_str());
        		m_db.dbo_update_sa_state_desc(va_sid.at(ncount).c_str(), AUTHINPUT_SENDSTATE_ECM_FAIL, senddesc, -1, TaskId);
				sleep(3);
            	continue;
         	}

        	memset(ImageID,0,sizeof(ImageID));
        	strcpy(ImageID,outmap["DocumentID"].c_str());
        	if( strlen(ImageID)<8)
        	{
        		LogPrint(AUTHINPUT_SEND_NODE, "ECM上传图像失败,原因影像ID错误");
				strcpy(senddesc, "影像ID错误");
          		m_db.dbo_update_sa_state_desc(va_sid.at(ncount).c_str(), AUTHINPUT_SENDSTATE_ECM_FAIL, senddesc, -1, TaskId);
				sleep(3);
            	continue;
        	}       
		 
			//保存影像ID
			ret = m_db.dbo_save_sa_imageid(TaskId,ImageID);

			//影像ID加入缓存
			va_imgid.at(ncount) = ImageID;

			if(ret)
			{
				LogPrint(AUTHINPUT_SEND_NODE, "更新影像ID失败[%d]!",ret);	
		       	m_db.dbo_update_sa_state(TaskId, AUTHINPUT_SENDSTATE_ECM_FAIL);
				sleep(3);
		       	continue;
			} 
				
			LogPrint(AUTHINPUT_SEND_NODE, "ECM上传图像成功[%s]",ImageID);
		
		}
		//上传过影像
		else
		{
			strcpy(ImageID, _scanNode.imageid);

			LogPrint(AUTHINPUT_SEND_NODE, "vchtype->[%s] area->[%s] session[%s] banktype->[%s] imageid->[%s]", _scanNode.vchtype , _scanNode.area,_scanNode.sess, _scanNode.banktype, _scanNode.imageid);
		}
	}

	LogPrint(AUTHINPUT_SEND_NODE, "上传ECM结束，启动流程...");

	//循环触发流程
	for(ncount=0; ncount < va_sid.size(); ncount++)
	{
		if(va_sid.size()<2)
				break;
				
		string headstr, taskstr, send, elpId, uplCnt; 
		char tmp_elpId[64], tmp_uplCnt[64];
		int nfails = 0;

		memset(&disttask,0,sizeof(disttask));
		memset(tmp_elpId, '\0', sizeof(tmp_elpId));
		memset(tmp_uplCnt, '\0', sizeof(tmp_uplCnt));

    strcpy(disttask.accdate,_scanNode.date);
    strcpy(disttask.areano,_scanNode.area);
    strcpy(disttask.exchno,_scanNode.pkgno);
    strcpy(disttask.session,_scanNode.sess);
		strcpy(disttask.imgtype,_scanNode.banktype);

		sprintf( tmp_uplCnt, "%d", (va_sid.size()-1));

		elpId = TaskId;
		uplCnt = tmp_uplCnt;

		LogPrint(AUTHINPUT_SEND_NODE, "信封流水[%s] 上传笔数[%d]", elpId.c_str() , va_sid.size());

		//第一个任务 发送信封
		if(ncount==0)
		{
			//信封已上传过不再上传
			if(_scanNode.sendstate > 5)
			{
				LogPrint(AUTHINPUT_SEND_NODE, "信封[%s]已上传过，跳过！", elpId.c_str());
				continue;
			}
				
			headstr = makeHead((char*)_cfgMap["msgtypeex1"].c_str());
			
			//556572交易 特色业务信封提交 AccDat,AreNo,Scene,ElpType,ElpID,UplCnt,UplAmt
			taskstr = makeTaskBodyEx1(disttask, _scanNode.banktype, elpId, uplCnt, _scanNode.amount);

			LogPrint(AUTHINPUT_SEND_NODE, "发送信封! 交易码[%s] 信封ID[%d] 上传票据数[%d] 报文体[%s]",  (char *)_cfgMap["msgtypeex1"].c_str(), atoi(elpId.c_str()), va_sid.size(), taskstr.c_str() );
		}
		//发送票据,影像ID有效
		else if(ncount>0 && va_imgid.at(ncount).size()>10 )
		{
			headstr = makeHead((char*)_cfgMap["msgtypeex2"].c_str());

			//556573交易 特色业务票据提交
			//AccDat,AreNo,ExcNo,Scene,ElpId,ImgId
			taskstr = makeTaskBodyEx2(disttask, elpId, va_imgid.at(ncount));

			LogPrint(AUTHINPUT_SEND_NODE, "信封ID[%s] 发送票据! 交易码[%s] 报文体[%s]", elpId.c_str(), (char *)_cfgMap["msgtypeex2"].c_str(), taskstr.c_str());
		}
		//影像ID无效
		else
		{
			nfails++;
			LogPrint(AUTHINPUT_SEND_NODE, "票据[%s] 无影像ID, 跳过...", va_sid.at(ncount).c_str());
			continue;
		}

    send = headstr+taskstr;
		LogPrint(AUTHINPUT_SEND_NODE, "信封ID[%s] 发送报文! [%s]", elpId.c_str(), send.c_str());

    memset(cmd,0,sizeof(cmd));
    memset(cmdstr,0,sizeof(cmdstr));
   
    //流程交易
    ret = doTrade(send.c_str(), _cfgMap["flowip"].c_str(),_cfgMap["flowport"].c_str(),result,cmd,cmdstr);
		memset(senddesc, '\0', sizeof(senddesc));
		strncpy(senddesc, cmdstr, 100);

        if(ret!=0)
        {
						nfails++;
	
	        	LogPrint(AUTHINPUT_SEND_NODE, "启动流程任务失败[%d] cmd->[%s] msgtypeex1->[%s]",ret, cmd, _cfgMap["msgtypeex1"].c_str());
						if(ret==-4)
						{
								nfails++;
								strcpy(senddesc, "流程应答超时");
								LogPrint(AUTHINPUT_SEND_NODE, "%s", senddesc);
				
								//触发流程超时，则此笔业务跳出, 且不会被系统超时回收
								if(ncount==0)
								{
									LogPrint(AUTHINPUT_SEND_NODE, "错误: 信封[%s]触发流程超时，此笔业务终止", elpId.c_str());
									sprintf(senddesc, "错误: 信封[%s]触发流程超时，此笔业务终止", elpId.c_str());
									LogPrint(AUTHINPUT_SEND_NODE, "%s", senddesc);
									m_db.dbo_update_sa_state_desc((char *)elpId.c_str(), AUTHINPUT_SENDSTATE_BILL_OUTTIME, senddesc, result,(char *)elpId.c_str());
									break;
								}
								else
								{
									LogPrint(AUTHINPUT_SEND_NODE, "错误: 票据[%s]触发流程超时，此笔业务终止", va_sid.at(ncount).c_str());
									m_db.dbo_update_sa_state_desc(va_sid.at(ncount).c_str(), AUTHINPUT_SENDSTATE_TOUCH_FAIL, senddesc, result, TaskId);
									continue;
								}
						}
						else if(ret==-3)
						{
								strcpy(senddesc, "流程连接失败");
								LogPrint(AUTHINPUT_SEND_NODE, "%s", senddesc);
								//信封流程连接失败
								nfails++;
								if(ncount==0)
								{
										LogPrint(AUTHINPUT_SEND_NODE, "错误: 信封[%s]连接流程银行失败", elpId.c_str());
										m_db.dbo_update_sa_state_desc(va_sid.at(ncount).c_str(), AUTHINPUT_SENDSTATE_FAIL, senddesc, result,TaskId);
										break;
								}
								else
								{
										LogPrint(AUTHINPUT_SEND_NODE, "错误: 票据[%s]连接流程银行失败", va_sid.at(ncount).c_str());
										m_db.dbo_update_sa_state_desc(va_sid.at(ncount).c_str(), AUTHINPUT_SENDSTATE_FAIL, senddesc, result,TaskId);
										continue;
								}
						}
						else if(ret==-1)
						{
								nfails++;
								LogPrint(AUTHINPUT_SEND_NODE, "接收数据失败: [%s]连接流程银行失败", va_sid.at(ncount).c_str());
								if(ncount==0)
								{
									LogPrint(AUTHINPUT_SEND_NODE, "错误: 信封[%s]连接流程银行失败", va_sid.at(ncount).c_str());
									m_db.dbo_update_sa_state_desc(va_sid.at(ncount).c_str(), AUTHINPUT_SENDSTATE_FAIL, senddesc, result,TaskId);
									break;
								}
								else
								{
									LogPrint(AUTHINPUT_SEND_NODE, "错误: 票据[%s]连接流程银行失败", va_sid.at(ncount).c_str());
									m_db.dbo_update_sa_state_desc(va_sid.at(ncount).c_str(), AUTHINPUT_SENDSTATE_FAIL, senddesc, result,TaskId);
									continue;
								}
						}
						else
						{
								nfails++;
								if(ncount==0)
								{
									LogPrint(AUTHINPUT_SEND_NODE, "网络错误: 信封[%s]连接流程银行失败", va_sid.at(ncount).c_str());
									m_db.dbo_update_sa_state_desc(va_sid.at(ncount).c_str(), AUTHINPUT_SENDSTATE_FAIL, senddesc, result,TaskId);
									break;
								}
								else
								{
									LogPrint(AUTHINPUT_SEND_NODE, "网络错误: 票据[%s]连接流程银行失败", va_sid.at(ncount).c_str());
									m_db.dbo_update_sa_state_desc(va_sid.at(ncount).c_str(), AUTHINPUT_SENDSTATE_FAIL, senddesc, result,TaskId);
									continue;
								}
						}
        }

        if(result!=0)
        {
            if(strncmp(cmdstr,"SCD044536030001该影像处理流程已启动",35)!=0 )
            {
          		LogPrint(AUTHINPUT_SEND_NODE, "启动流程任务失败[%s][%s]...",cmd,cmdstr);
          		m_db.dbo_update_sa_state_desc(va_sid.at(ncount).c_str(), AUTHINPUT_SENDSTATE_FAIL, senddesc, result, TaskId);
							sleep(3);
							//如果第一笔信封触发流程失败，则取消此笔业务上传
							if(ncount==0)
								break;
							else
								continue;
            }
       	}

				//信封触发流程成功
				if( ncount == 0)
				{
						m_db.dbo_update_sa_state_desc(va_sid.at(ncount).c_str(), AUTHINPUT_SENDSTATE_BILL_DOING, "", 0, TaskId);
				}
				//最后一张票据更新此笔业务状态
				else if( (ncount+1) == va_fname.size() )
				{
						if(nfails == 0)
						{
							m_db.dbo_update_sa_state_desc(va_sid.at(ncount).c_str(), AUTHINPUT_SENDSTATE_BILL_SUCC, "", 0, TaskId);
						}
				}
				else
				{
						m_db.dbo_update_sa_state_desc(va_sid.at(ncount).c_str(), AUTHINPUT_SENDSTATE_SUCC, "", 0, TaskId);
				}

       	LogPrint(AUTHINPUT_SEND_NODE, "信封ID[%s] 触发流程结束! 交易码[%s]", elpId.c_str(), _cfgMap["msgtypeex1"].c_str() );

				memset(senddesc, '\0', sizeof(senddesc));
	}
	
	
	}
	
	return;
}

int CSENDA::get_config()
{
	//查询配置表
	_cfgMap.clear();
	int ret = m_db.dbo_get_config(_cfgMap);
	if(ret)
	{
		LogPrint(AUTHINPUT_SEND_NODE, "查询数据错误[%d]!",ret);	
		return ret;
	}		

	if(task_count%20==0)
	{
		LogPrint(AUTHINPUT_SEND_NODE, "ECM[%s][%s]!",_cfgMap["ecmip"].c_str(),_cfgMap["ecmport"].c_str());	
		LogPrint(AUTHINPUT_SEND_NODE, "FLOW[%s][%s]!",_cfgMap["flowip"].c_str(),_cfgMap["flowport"].c_str());	
	}

	//查询凭证类型
	_vtMap.clear();
	ret = m_db.dbo_get_vchtype(_vtMap);
	if(ret)
	{
		LogPrint(AUTHINPUT_SEND_NODE, "查询数据错误[%d]!",ret);	
		return ret;
	}	

	return 0;
}

