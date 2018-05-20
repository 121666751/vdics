
#include <string.h>
#include <sstream>
#include <mem.h>
#include <transimg.h>
#include <ZTime.hpp>

#include "SendVouchers.h"

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

//////////////////////////////////////////////////////////////////////////
// 上传类实现

// -----------------------------------------------------------------------
static int task_count = 0;
CSENDV::CSENDV()
{
	bzero(TaskIp,sizeof(TaskIp));
	bzero(DbHost,sizeof(DbHost));
	bzero(DbUser,sizeof(DbUser));
	bzero(DbPswd,sizeof(DbPswd));
	bzero(DbName,sizeof(DbName));
	try
	{
		LogPrint( VOUCHER_SEND_NODE, GSIP_VERSION);

		Config mycfg("./config/config.cfg");

		string szTmp;
		szTmp=mycfg.value("task_ip",1);
      	if(szTmp.size()>0)
      	{
         	strcpy(TaskIp,szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(VOUCHER_SEND_NODE, "读取调度服务IP错误!");
      		printf("读调度服务IP错误!");
      		exit(-1);
      	}
      	szTmp=mycfg.value("task_port",1);
      	if(szTmp.size()>0)
      	{
      		TaskPort = stoi(szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(VOUCHER_SEND_NODE, "读取调度服务端口错误!");
      		printf("读调度服务端口错误!");
      		exit(-1);
      	}
      
      	szTmp=mycfg.value("db_host",1);
      	if(szTmp.size()>0)
      	{
      		strcpy(DbHost,szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(VOUCHER_SEND_NODE, "读数据库地址错误!");
      		printf("读数据库地址错误!");
      		exit(-1);
      	}
      
     		szTmp=mycfg.value("db_uid",1);
      	if(szTmp.size()>0)
      	{
           strcpy(DbUser,szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(VOUCHER_SEND_NODE, "读数据库用户名错误!");
      		printf("读数据库用户名错误!");
      		exit(-1);
      	}
      
      	szTmp=mycfg.value("db_pwd",1);
      	if(szTmp.size()>0)
      	{
      		strcpy(DbPswd,szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(VOUCHER_SEND_NODE, "读数据库用户密码错误!");
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
      		LogPrint(VOUCHER_SEND_NODE, "读数据库实例错误!");
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
      		LogPrint(VOUCHER_SEND_NODE, "读图像存放目录错误!");
      		printf("读图像存放目录错误!");
      		exit(-1);
      	}
    }
    catch(...)
    {
   		LogPrint(VOUCHER_SEND_NODE, "读图像存放目录错误!");
   		printf("读图像存放目录错误!");
      exit(-1);
    }     
}

// -----------------------------------------------------------------------
CSENDV::~CSENDV()
{

}

int verbose;

// -----------------------------------------------------------------------
int CSENDV::getTaskId()
{
	memset(TaskId,0,sizeof(TaskId));

	if(verbose)
		LogPrint(VOUCHER_SEND_NODE, "发送上传任务请求!");
		
	ZSocket so;
	int ret = so.Connect(TaskIp,TaskPort);
	if(ret)
	{	
		LogPrint(VOUCHER_SEND_NODE, "连接调度任务出错[%s][%d]!",TaskIp,TaskPort);
		return ret;
	}
	ret = so.Send((void*)"00000006031004",14);
  	if(ret<=0)
	{
		LogPrint(VOUCHER_SEND_NODE, "发送上传任务请求失败!");
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
		if(ret==0)
		{
			LogPrint(VOUCHER_SEND_NODE, "错误!服务器关闭连接!");
		}
		else
		{
			LogPrint(VOUCHER_SEND_NODE, "接收任务请求失败!");
		}
		sleep(3);

  		return -1;
  	}
  	
  	int len = atoi(cmd);
  	memset(cmd,0,sizeof(cmd));
  	ret =  so.RecvEx(cmd,len);
  	if(ret!=len)
  	{
  		LogPrint(VOUCHER_SEND_NODE, "接收任务请求失败!");
		sleep(3);
  		return -1;
  	}
  
	if(task_count%20==0)
		LogPrint(VOUCHER_SEND_NODE, "收到消息[%d][%s]!",len,cmd);

	strncpy(str,cmd+PKG_TYPE_LEN+PKG_TRADECODE_LEN,PKG_RESULTCODE_LEN);
	
	if(strncmp(str,"0000",4) == 0)
	{
		strcpy(TaskId,cmd+PKG_TYPE_LEN+PKG_TRADECODE_LEN+PKG_RESULTCODE_LEN);
	}
	else
	{
		if(strncmp(str,TASK_RESULT_NONE,4) == 0)  
		{
			//LogPrint(VOUCHER_SEND_NODE, "无上传任务!");
		}
		else
		{
			LogPrint(VOUCHER_SEND_NODE, "获取上传任务失败[%s]!",str);
			sleep(3);
		}		
		
		return -1;
	}	
	
	return 0;
}
// 开始上传
void CSENDV::Start(int debug)
{
	verbose = debug;
	
	if(verbose)
    	{ 
	        LogPrint(VOUCHER_SEND_NODE, "./svsrv -v");
	}   
	else
	{   
	        LogPrint(VOUCHER_SEND_NODE, "./svsrv");
			init_dameon();
	}  

	printf("version %sn", GSIP_VERSION);
	LogPrint(VOUCHER_SEND_NODE, "version %s\n", GSIP_VERSION);
	LogPrint(VOUCHER_SEND_NODE, "开始处理...");

	char fname[80];
	char bname[80];
	char newfname[128];
    	char newbname[128];
	char send_err[120];
    
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
		LogPrint(VOUCHER_SEND_NODE, "数据库连接出错[%d]!",ret);	
		return ;
	}			

	//获取配置信息	
	ret = get_config();
	if(ret)
	{
		LogPrint(VOUCHER_SEND_NODE, "get_config [%d]",ret);	
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
		
				LogPrint(VOUCHER_SEND_NODE, "get_config [%d]",ret);	
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
		ret = m_db.dbo_get_sv_tasks(TaskId,_scanNode);
		if(ret)
		{
			LogPrint(VOUCHER_SEND_NODE, "查询任务明细错误[%s]",TaskId);
			m_db.dbo_update_sv_state(TaskId,3);
			continue;
		}
		
		//判断影响是否成功上传过
   		if(strlen(_scanNode.imageid)<10)
   		{
			//获取图像
			sprintf(fname,"%s//%02d//%05d//%s",_scanNode.date,atoi(_scanNode.sess),atoi(_scanNode.pkgno),_scanNode.fname);
			sprintf(bname,"%s//%02d//%05d//%s",_scanNode.date,atoi(_scanNode.sess),atoi(_scanNode.pkgno),_scanNode.bname);
			sprintf(newfname,"SVF%d.jpg",_scanNode.sid);
			sprintf(newbname,"SVB%d.jpg",_scanNode.sid);
			
			ret = ti_getfileformat_ex2(fname,"jpg",_scanNode.ipaddr,3210,99,_scanNode.rotate,1,imageF);
	        	if(ret)
	        	{
		       		LogPrint(VOUCHER_SEND_NODE, "获取正面图像错误[%s][%d]!",fname,ret);	
		       		m_db.dbo_update_sv_state_desc(TaskId,3,"获取图像错误", -1);
				sleep(5);
	            		continue;
	        	}
	         
	        	ret = ti_getfileformat_ex2(bname,"jpg",_scanNode.ipaddr,3210,80,_scanNode.rotate,1,imageB);
	        	if(ret)
	        	{
	         		LogPrint(VOUCHER_SEND_NODE, "获取反面图像错误[%s][%d]!",bname,ret);	
	            		m_db.dbo_update_sv_state_desc(TaskId, 3, "获取图像错误", -1);
	            		continue;
	        	}
			LogPrint(VOUCHER_SEND_NODE, "获取图像成功");	

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
	         
	       		LogPrint(VOUCHER_SEND_NODE, "图像大小[%d]",size);	
	        	
	        	gloabXml["TotalSize"] = itos(size);
	        	ZDate dt;
	        	dt.Now();
	        	string dd = dt.BankDateString().c_str();
	        	string hh = dt.TimeString().c_str();
	        	string id = itos(_scanNode.sid);
	        
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

			//sesion补成3位
			sprintf(_scanNode.sess, "%03d", atoi(_scanNode.sess));

	        	gloabXml["ExchangeScene"]   =  _scanNode.sess;;
	        	gloabXml["EventID"] = (dd+hh+id+"SCN001").c_str();
	        	gloabXml["CaseID"] = (dd+hh+id+"SCN001").c_str();
	        
	       	 	gloabXml["SubBankNo"] = _cfgMap["brno"]; 
	        	gloabXml["TellerNo"] = _scanNode.clkno;
			string pk = _exnoMap[_scanNode.exchno].c_str();
			if(pk.size()<4)
				pk = _scanNode.exchno;
	        
	
			int n_zero = 12-pk.size();
			char tmp_pkgno[13];
			bzero(tmp_pkgno, sizeof(tmp_pkgno));
			while(n_zero>0)
			{   
	    			strcat( tmp_pkgno,"0"); 
	    			n_zero--;
			}   
			
			strcat( tmp_pkgno,pk.c_str());
	        	gloabXml["ExchangeNo"] = tmp_pkgno;
	
	        	gloabXml["AttachType"] = "1";
	        	gloabXml["ExchangeArea"] = _scanNode.area;

			if(strlen(_scanNode.banktype)<2)
		    	strcpy(_scanNode.banktype, "000");
							
			LogPrint(VOUCHER_SEND_NODE, "vchtype->[%s] area->[%s] banktype->[%s] session->[%s]", _scanNode.vchtype , _scanNode.area, _scanNode.banktype, _scanNode.sess);
		
	        	gloabXml["BillType"] =  _scanNode.banktype;
   
	        	string xmlstr = makeXml(gloabXml,t);
	        	memset(Results,0,sizeof(Results));       
	        	MemT xmlm;
	       		xmlm.assign((unsigned char*)xmlstr.c_str(),xmlstr.size());
     	
	       		//ECM通讯
		       	
			ret = myECM(xmlm, _cfgMap["ecmip"].c_str(),_cfgMap["ecmport"].c_str(),Results);		
			strncpy(send_err, Results, 100);
		
			deleteFiles(t);
	        	if(ret<0)
	        	{
	        		LogPrint(VOUCHER_SEND_NODE, "ECM上传图像失败[%d]!",ret);	
				if(ret==-4)
				{
					LogPrint(VOUCHER_SEND_NODE, "ECM应答超时![%d]!",ret);	
					strcpy(send_err, "ECM应答超时");
				}
				else if(ret==-3)
				{
					LogPrint(VOUCHER_SEND_NODE, "ECM连接失败![%d]!",ret);	
					strcpy(send_err, "ECM连接失败");
				}
				else
				{
					LogPrint(VOUCHER_SEND_NODE, "ECM影像上传错误![%d]!",ret);	
					strcpy(send_err, "ECM影像上传错误");
				}
					
				m_db.dbo_update_sv_state_desc(TaskId, VOUCHERS_SENDSTATE_ECM_FAIL, send_err, ret);

				sleep(3);
	           		continue;
	        	}
		
	        	KeyMap  outmap;
	        
	        	//解析结果
	        	outmap = parseOutXml(Results);
			memset(send_err, '\0', sizeof(send_err));
			strncpy(send_err, outmap["MsgInfo"].c_str(), 100);

	        	if( (outmap["MsgCode"]!="000000" &&  outmap["MsgCode"]!="0")||(outmap["CaseID"]!= gloabXml["CaseID"]) )
	        	{
		       		LogPrint(VOUCHER_SEND_NODE, "ECM上传图像失败,原因[%s]",outmap["MsgInfo"].c_str());
	        		m_db.dbo_update_sv_state_desc(TaskId,VOUCHERS_SENDSTATE_ECM_FAIL, send_err, -1);
				sleep(3);
	            		continue;
	         	}
	
	        	memset(ImageID,0,sizeof(ImageID));
	        	strcpy(ImageID,outmap["DocumentID"].c_str());
	        	if( strlen(ImageID)<8)
	        	{
	        		LogPrint(VOUCHER_SEND_NODE, "ECM上传图像失败,原因影像ID错误");
				strcpy(send_err, "影像ID错误");
	          		m_db.dbo_update_sv_state_desc(TaskId, VOUCHERS_SENDSTATE_ECM_FAIL, send_err, -1);
				sleep(3);
	        		continue;
	        	}       
				 
			//保存影像ID
			ret = m_db.dbo_save_sv_imageid(TaskId,ImageID);
			if(ret)
			{
				LogPrint(VOUCHER_SEND_NODE, "更新影像ID失败[%d]!",ret);	
	          		m_db.dbo_update_sv_state(TaskId,VOUCHERS_SENDSTATE_ECM_FAIL);
				sleep(3);
	           		continue;
			} 
		
			LogPrint(VOUCHER_SEND_NODE, "ECM上传图像成功[%s]",ImageID);
	   	}
		else
		{
			if(strlen(_scanNode.banktype)<2)
				strcpy(_scanNode.banktype, "000");

			if(task_count%20==0)
				LogPrint(VOUCHER_SEND_NODE, "vchtype->[%s] area->[%s] banktype->[%s] imageid->[%s]", _scanNode.vchtype , _scanNode.area, _scanNode.banktype, _scanNode.imageid);
			
			strcpy(ImageID, _scanNode.imageid);
		}

		memset(&disttask,0,sizeof(disttask));
        	memset(&distdata,0,sizeof(distdata));

		strncpy(disttask.orgcode,_cfgMap["brno"].c_str(),6);

		strcpy(disttask.clkno,_scanNode.clkno);
		 LogPrint(VOUCHER_SEND_NODE,"clkno = %s,%s",_scanNode.clkno,disttask.clkno);

        	strcpy(disttask.imageid,ImageID);
       	 	strcpy(disttask.pageno,"2");
		strcpy(disttask.brno,_cfgMap["brno"].c_str());
        	strcpy(disttask.accdate,_scanNode.date);
        	strcpy(disttask.areano,_scanNode.area);

		string pkn = _exnoMap[_scanNode.exchno].c_str();
		if(pkn.size()<5) 
			pkn = _scanNode.exchno;
        	strcpy(disttask.exchno,pkn.c_str());

		LogPrint(VOUCHER_SEND_NODE, "disttask.excho->[%s] _scanNode.pkgno->[%s]", disttask.exchno, _scanNode.pkgno);

        	strcpy(disttask.session, _scanNode.sess);
        	strcpy(disttask.modeflag, _scanNode.modeflag);

        	// strcpy(disttask.imgtype, gloabXml["BillType"].c_str());
      		strcpy(disttask.imgtype,_scanNode.banktype);
		 
        	strcpy(disttask.ccycode,"CNY");		
		    
		string headstr, taskstr;

		//北京分行
		if(!strcmp(disttask.brno, "110999"))
		{
			if( atoi(_scanNode.vchtype) < 50)
			{
				headstr = makeHead((char*)_cfgMap["msgtype"].c_str());
				LogPrint(VOUCHER_SEND_NODE,"brno->[%s] exchno->[%s] accno->[%s], vchno->[%s], amount->[%s]", disttask.brno, disttask.exchno, _scanNode.accno, _scanNode.vchno, _scanNode.amount);
				taskstr = makeTaskBodyEx(disttask,_scanNode.accno,_scanNode.vchno,_scanNode.amount);       
			}
			else
			{
				//北京硬联进帐单vchtype<50的, 走不带帐号金额的111流程
				headstr = makeHead("451600");

				strcpy(disttask.imgtype, "111");
				taskstr = makeTaskBody(disttask);
			}
		}
		//非北京分行
		else
		{
			headstr = makeHead((char*)_cfgMap["msgtype"].c_str());

			if(!strcmp((char *)_cfgMap["msgtype"].c_str(), "451601"))
			{
				LogPrint(VOUCHER_SEND_NODE,"brno->[%s] exchno->[%s] accno->[%s], vchno->[%s], amount->[%s]", disttask.brno, _scanNode.exchno, _scanNode.accno, _scanNode.vchno, _scanNode.amount);
				taskstr = makeTaskBodyEx(disttask,_scanNode.accno,_scanNode.vchno,_scanNode.amount);   
			}
			else if(!strcmp((char *)_cfgMap["msgtype"].c_str(), "451600"))
			{
				taskstr = makeTaskBody(disttask);
			}
			else
			{
				LogPrint( VOUCHER_SEND_NODE, "任务中断！ 错误的交易类型: [%s]", (char *)_cfgMap["msgtype"].c_str());
				continue;	
			}
		}
        
		headstr.assign("02");
		string send = headstr+taskstr;
        	//string send = taskstr;

		memset(cmd,0,sizeof(cmd));
        	memset(cmdstr,0,sizeof(cmdstr));
        
        	//流程交易,cmdstr返回的报文
        	ret = doTrade(send.c_str(), _cfgMap["flowip"].c_str(),_cfgMap["flowport"].c_str(),result,cmd,cmdstr);

		memset(send_err, '\0', sizeof(send_err));

		strncpy(send_err, cmdstr, 100);
        	if(ret!=0)
        	{
        		LogPrint(VOUCHER_SEND_NODE, "启动流程任务失败[%d]",ret);
			if(ret==-4)
			{
				strcpy( send_err, "流程应答超时");
				LogPrint(VOUCHER_SEND_NODE, "流程应答超时!");
				m_db.dbo_update_sv_state_desc(TaskId, VOUCHERS_SENDSTATE_TOUCH_FAIL, send_err, result);
			}
			else if(ret==-3)
			{	
				strcpy( send_err, "流程连接失败");
				LogPrint(VOUCHER_SEND_NODE, "流程连接失败!");
				m_db.dbo_update_sv_state_desc(TaskId, VOUCHERS_SENDSTATE_FAIL, send_err, result);
			}
			else
			{
				strcpy( send_err, "流程失败");
				LogPrint(VOUCHER_SEND_NODE, "流程失败!");
				m_db.dbo_update_sv_state_desc(TaskId, VOUCHERS_SENDSTATE_FAIL, send_err, result);
			}

			sleep(3);
            		continue;
        	}

       		if(result!=0)
        	{
            		if(strncmp(send_err,"SCD044516030001该影像处理流程已启动",35)!=0 )
            		{
          			LogPrint(VOUCHER_SEND_NODE, "启动流程任务失败[%s][%s]...",cmd,send_err);
			    	//存储交易返回码和返回字符串
				if(strlen(send_err)==0)
					strcpy( send_err, "启动流程失败");

          			m_db.dbo_update_sv_state_desc(TaskId, VOUCHERS_SENDSTATE_FAIL, send_err, result);
				sleep(3);
            			continue;
            		}
       		}
       	
       		LogPrint(VOUCHER_SEND_NODE, "启动流程任务成功 tradetype->[%s]", headstr.c_str());

		memset(send_err, '\0', sizeof(send_err));	
       		m_db.dbo_update_sv_state_desc(TaskId,1, send_err, 0);
			
	}
	return;
}

int CSENDV::get_config()
{
	//查询配置
	_cfgMap.clear();
	int ret = m_db.dbo_get_config(_cfgMap);
	if(ret)
	{
		LogPrint(VOUCHER_SEND_NODE, "查询数据错误[%d]!",ret);	
		return ret;
	}		
	
	if(task_count%20==0)
	{
		LogPrint(VOUCHER_SEND_NODE, "ECM[%s][%s]!",_cfgMap["ecmip"].c_str(),_cfgMap["ecmport"].c_str());	
		LogPrint(VOUCHER_SEND_NODE, "FLOW[%s][%s]!",_cfgMap["flowip"].c_str(),_cfgMap["flowport"].c_str());	
	}
	
	//查询凭证类型
	_vtMap.clear();
	_exnoMap.clear();
	ret = m_db.dbo_get_vchtype(_vtMap);
	if(ret)
	{
		LogPrint(VOUCHER_SEND_NODE, "查询数据错误[%d]!",ret);	
		return ret;
	}	
	ret = m_db.dbo_get_exchno(_exnoMap);
	if(ret)
	{
		LogPrint(VOUCHER_SEND_NODE,"get exno list error",ret);
		return ret;
	}

	return 0;
}
