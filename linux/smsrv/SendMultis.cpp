#include <string.h>
#include <sstream>
#include <mem.h>
#include <transimg.h>
#include <ZTime.hpp>


#include "ConfigFile.h"
#include "PubFun.h"
#include "ExportLog.h"
#include "Dboperate.h"
#include "gsipcom.h"
#include "package.h"
#include "gsip_util.h"
#include <zsocket.hpp>
#include <sys/time.h>

#include "SendMultis.h"

#define TASK_COUNT 100

//////////////////////////////////////////////////////////////////////////
// 上传类实现

int verbose;

static int task_count = 0;
// -----------------------------------------------------------------------
CSENDM::CSENDM()
{
	bzero(TaskIp,sizeof(TaskIp));
	bzero(DbHost,sizeof(DbHost));
	bzero(DbUser,sizeof(DbUser));
	bzero(DbPswd,sizeof(DbPswd));
	bzero(DbName,sizeof(DbName));
	try
	{
		LogPrint( MULTIS_SEND_NODE, GSIP_VERSION);

		Config mycfg("./config/config.cfg");

		string szTmp;
		szTmp=mycfg.value("task_ip",1);
      	if(szTmp.size()>0)
      	{
         	strcpy(TaskIp,szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(MULTIS_SEND_NODE, "读取调度服务IP错误!");
			printf("读取调度服务IP错误!");
			exit(-1);
      	}
      	szTmp=mycfg.value("task_port",1);
      	if(szTmp.size()>0)
      	{
      		TaskPort = stoi(szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(MULTIS_SEND_NODE, "读取调度服务端口错误!");
			printf("读取调度端口错误!");
			exit(-1);
      	}
      
      	szTmp=mycfg.value("db_host",1);
      	if(szTmp.size()>0)
      	{
      		strcpy(DbHost,szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(MULTIS_SEND_NODE, "读数据库地址错误!");
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
      		LogPrint(MULTIS_SEND_NODE, "读数据库用户名错误!");
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
      		LogPrint(MULTIS_SEND_NODE, "读数据库用户密码错误!");
			printf("读取数据库用户密码错误!");
			exit(-1);
      	}
      
      	szTmp=mycfg.value("db_dbAlias",1);
      	if(szTmp.size()>0)
      	{
           	strcpy(DbName,szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(MULTIS_SEND_NODE, "读数据库实例错误!");
			printf("读取数据库实例错误!");
			exit(-1);
      	}
      	
      	szTmp=mycfg.value("imagepath",1);
      	if(szTmp.size()>0)
      	{
           	strcpy(ImagePath,szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(MULTIS_SEND_NODE, "读图像存放目录错误!");
			printf("读图像存放目录错误!");
			exit(-1);
      	}
    }
    catch(...)
    {
   		LogPrint(MULTIS_SEND_NODE, "读配置错误!");
   		printf("读配置错误!");
		exit(-1);
    }     
}

// -----------------------------------------------------------------------
CSENDM::~CSENDM()
{

}

// -----------------------------------------------------------------------
int CSENDM::getTaskId()
{
	memset(TaskId,0,sizeof(TaskId));

	if(verbose)
		LogPrint(MULTIS_SEND_NODE, "发送上传任务请求!");
	
	ZSocket so;
	int ret = so.Connect(TaskIp,TaskPort);
	if(ret)
	{	
		LogPrint(MULTIS_SEND_NODE, "连接调度任务出错[%s][%d]!",TaskIp,TaskPort);
		sleep(3);
		return ret;
	}
	ret = so.Send((void*)"00000006031006",14);
  	if(ret<=0)
	{
		if(verbose)
			LogPrint(MULTIS_SEND_NODE, "发送上传任务请求失败!");
		sleep(3);
		return -1;
	}
	
  	char cmd[512];
  	char str[8];
  
  	memset(cmd,0,sizeof(cmd));
  	memset(str,0,sizeof(str));
  
  	if((ret =  so.RecvEx(cmd,8)) < 0)
	{
		if(ret==0)
		{
			LogPrint(MULTIS_SEND_NODE, "错误！调度服务关闭连接!");
		}
		else
		{
			LogPrint(MULTIS_SEND_NODE, "等待任务请求超时!");
		}
		sleep(3);

  		return -1;
	}

  	if(ret!=8)
  	{
  		LogPrint(MULTIS_SEND_NODE, "接收任务请求失败!");
		sleep(3);
  		return -1;
  	}
  	
  	int len = atoi(cmd);
  	memset(cmd,0,sizeof(cmd));
  	ret =  so.RecvEx(cmd,len);
  	if(ret!=len)
  	{
  		LogPrint(MULTIS_SEND_NODE, "接收任务请求失败!");
		sleep(3);
  		return -1;
  	}
 
	if(task_count%20==0)
		LogPrint(MULTIS_SEND_NODE, "收到消息[%d][%s]!",len,cmd);

	strncpy(str,cmd+PKG_TYPE_LEN+PKG_TRADECODE_LEN,PKG_RESULTCODE_LEN);
	
	
	if(strncmp(str,"0000",4) == 0)
	{
		strcpy(TaskId,cmd+PKG_TYPE_LEN+PKG_TRADECODE_LEN+PKG_RESULTCODE_LEN);
	}
	else
	{
		if(strncmp(str,TASK_RESULT_NONE,4) == 0)  
		{
			//LogPrint(MULTIS_SEND_NODE, "无上传任务!");
		}
		else
		{
			LogPrint(MULTIS_SEND_NODE, "获取上传任务失败[%s]!",str);
			sleep(3);
		}		
		
		return -1;
	}	
	
	return 0;
}
// 开始上传
void CSENDM::Start(int debug)
{
	verbose = debug;
	if(verbose)
	{
		LogPrint(MULTIS_SEND_NODE, "./smsrv -v");
	}
	else
	{
		LogPrint(MULTIS_SEND_NODE, "./smsrv");
		init_dameon();
	}

	LogPrint(MULTIS_SEND_NODE, "开始处理...");

	printf("version %s", GSIP_VERSION);
	LogPrint(MULTIS_SEND_NODE, "%s", GSIP_VERSION);
		
	char fname[80];
	char bname[80];
	char newfname[128];
    char newbname[128];
	char senddesc[120];
    
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
		LogPrint(MULTIS_SEND_NODE, "数据库连接出错[%d]!",ret);	
		return ;
	}			

	//获取配置参数	
	ret = get_config();
	if(ret)
	{
		LogPrint(MULTIS_SEND_NODE, "获取配置参数出错[%d]!",ret);	
		return ;
	}
		
	while(true)
	{	
		//获取配置参数
		if( ++task_count > TASK_COUNT)
		{
			task_count = 0;

			if(get_config())
			{
				LogPrint(MULTIS_SEND_NODE, "获取配置参数出错[%d]!",ret);	
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
		vector<string> vc_fname;
		vector<string> vc_bname;
		ret = m_db.dbo_get_sm_tasks(TaskId, _scanNode, vc_fname, vc_bname);

		if(ret)
		{
			LogPrint(MULTIS_SEND_NODE, "查询任务明细错误[%s]",TaskId);
			m_db.dbo_update_sm_state(TaskId, MULTIS_SENDSTATE_FAIL);
			continue;
		}

		if(vc_fname.size()>0)
		{
			LogPrint(MULTIS_SEND_NODE,	"获取明细成功!图像数量[%d]", vc_fname.size());
		}

		//如果未上传过
		if(strlen(_scanNode.imageid)<10)
		{

			//获取图像
			//------------------------------主件+附件+信封------------------------------------------
			//_scanNode, <vector><string>vc
			FileVector t;
			int count = 0;
			
			while(vc_fname.size() > 0)
			{
        	//LogPrint(MULTIS_SEND_NODE, "count=[%d]!", count);	

			sprintf(fname,"%s//%02d//%05d//%s",_scanNode.date,atoi(_scanNode.sess),atoi(_scanNode.exchno),vc_fname.at(0).c_str());
			sprintf(bname,"%s//%02d//%05d//%s",_scanNode.date,atoi(_scanNode.sess),atoi(_scanNode.exchno),vc_bname.at(0).c_str());
			sprintf(newfname,"SMF%d_%d.jpg",_scanNode.sid, count);
			sprintf(newbname,"SMB%d_%d.jpg",_scanNode.sid, count);

			count++;
		
			ret = ti_getfileformat_ex2(fname,"jpg",_scanNode.ipaddr,3210,99,_scanNode.rotate,1,imageF);
        	if(ret)
        	{
        		LogPrint(MULTIS_SEND_NODE, "获取正面图像错误[%s][%d]!",fname,ret);	
        		m_db.dbo_update_sm_state_desc(TaskId,3, "取图失败", -1);
				sleep(5);
            	break;
        	}
         
       	 	ret = ti_getfileformat_ex2(bname,"jpg",_scanNode.ipaddr,3210,80,_scanNode.rotate,1,imageB);
        	if(ret)
        	{
         		LogPrint(MULTIS_SEND_NODE, "获取反面图像错误[%s][%d]!",bname,ret);	
            	m_db.dbo_update_sm_state_desc(TaskId,3, "取图失败", -1);
            	break;
        	}
		
//			LogPrint(MULTIS_SEND_NODE, "获取图像成功 fname->[%s]", fname);	

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
        	size = size + imageF.size()+imageB.size();        

        	imageB.saveToFile(node.fileFullName);

  
      		LogPrint(MULTIS_SEND_NODE, "图像大小[%d]",size);	
        	vc_fname.erase(vc_fname.begin());
			vc_bname.erase(vc_bname.begin());
		}
		//-----------------------------------结束-------------------------------------------------
		//只要有任意一张图没取到，取消此笔业务
		if(vc_bname.size()>0 || vc_fname.size() >0)
		{
			deleteFiles(t);
			continue;		
		}

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

		sprintf(_scanNode.sess, "%03d", atoi(_scanNode.sess));

        gloabXml["ExchangeScene"]   =  _scanNode.sess;;
        gloabXml["EventID"] = (dd+hh+id+"SM001").c_str();
        gloabXml["CaseID"] = (dd+hh+id+"SM001").c_str();
        
        gloabXml["SubBankNo"] = _cfgMap["brno"]; 
        gloabXml["TellerNo"] = _scanNode.clkno;

		//交换号为12位        
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


		//banktype
		if(strlen(_scanNode.banktype)<2)
        	strcpy(_scanNode.banktype, "000");

		LogPrint( MULTIS_SEND_NODE, "vchtype->[%s] area->[%s] banktype->[%s] session->[%s]", _scanNode.vchtype, _scanNode.area, _scanNode.banktype, _scanNode.sess);

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
        	LogPrint(MULTIS_SEND_NODE, "ECM上传图像失败[%d]!",ret);	
			if(ret==-4)
			{
				LogPrint(MULTIS_SEND_NODE, "ECM应答超时!");	
				strcpy(senddesc, "ECM应答超时");
			}
			else if(ret==-3)
			{
				LogPrint(MULTIS_SEND_NODE, "ECM连接失败!");	
				strcpy(senddesc, "ECM连接失败");
			}
			else
			{
				LogPrint(MULTIS_SEND_NODE, "ECM影像上传错误!");	
				strcpy(senddesc, "ECM影像上传错误");
			}
          	m_db.dbo_update_sm_state_desc(TaskId, MULTIS_SENDSTATE_ECM_FAIL, senddesc, ret);

			sleep(3);
           	continue;
        }

        KeyMap  outmap;
        
        //解析结果
        outmap = parseOutXml(Results);
		bzero(senddesc, sizeof(senddesc));
		strncpy(senddesc, outmap["MsgInfo"].c_str(), 100);

        if( (outmap["MsgCode"]!="000000" &&  outmap["MsgCode"]!="0")||(outmap["CaseID"]!= gloabXml["CaseID"]) )
        {
	       	LogPrint(MULTIS_SEND_NODE, "ECM上传图像失败,原因[%s]",outmap["MsgInfo"].c_str());
        	m_db.dbo_update_sm_state_desc(TaskId, MULTIS_SENDSTATE_ECM_FAIL, senddesc, -1);
			sleep(3);
            continue;
         }

        memset(ImageID,0,sizeof(ImageID));
        strcpy(ImageID,outmap["DocumentID"].c_str());
        if( strlen(ImageID)<8)
        {
        	LogPrint(MULTIS_SEND_NODE, "ECM上传图像失败,原因影像ID错误");
			strcpy(senddesc, "影响ID错误");
        	m_db.dbo_update_sm_state_desc(TaskId, MULTIS_SENDSTATE_ECM_FAIL, senddesc, -1);
			sleep(3);
            continue;
        }       
		 
		//保存影像ID
		ret = m_db.dbo_save_sm_imageid(TaskId,ImageID);
		if(ret)
		{
			LogPrint(MULTIS_SEND_NODE, "更新影像ID失败[%d]!",ret);	
           	m_db.dbo_update_sm_state(TaskId, MULTIS_SENDSTATE_ECM_FAIL);
			sleep(3);
           	continue;
		} 
		
		LogPrint(MULTIS_SEND_NODE, "ECM上传图像成功[%s]",ImageID);
		}
		else
		{

			if(strlen(_scanNode.banktype)<2)
				strcpy(_scanNode.banktype, "000");

			strcpy(ImageID, _scanNode.imageid);

			LogPrint( MULTIS_SEND_NODE, "vchtype->[%s] area->[%s] banktype->[%s] imageid->[%s]", _scanNode.vchtype, _scanNode.area, _scanNode.banktype, _scanNode.imageid);
		}
		
		memset(&disttask,0,sizeof(disttask));
        memset(&distdata,0,sizeof(distdata));
   
	strncpy(disttask.orgcode,_cfgMap["brno"].c_str(),6);
	strcpy(disttask.clkno,_scanNode.clkno);
        strcpy(disttask.imageid,ImageID);
	strcpy(disttask.pageno,"2");
        strcpy(disttask.brno,_cfgMap["brno"].c_str());
        strcpy(disttask.accdate,_scanNode.date);
        strcpy(disttask.areano,_scanNode.area);
        strcpy(disttask.exchno,_scanNode.pkgno);
        strcpy(disttask.session,_scanNode.sess);
        strcpy(disttask.modeflag,_scanNode.modeflag);

        //strcpy(disttask.imgtype, gloabXml["BillType"].c_str());
       
		strcpy(disttask.imgtype,_scanNode.banktype);
        strcpy(disttask.ccycode,"CNY");

        char amt[16];
        memset(amt,0,sizeof(amt));
        if(  _cfgMap["amount"] == "1" )
        {
              strcpy( amt,"000");
        }
        else
        {
             strcpy(amt,_scanNode.amount);
        }

		//string headstr = makeHead((char*)_cfgMap["msgtype"].c_str());
		string headstr = makeHead("451600");

		//2011-01-25修改
        //string taskstr = makeTaskBodyEx(disttask," "," "," ");
        string taskstr = makeTaskBody(disttask);

	headstr.assign("02");

        string send = headstr+taskstr;
        memset(cmd,0,sizeof(cmd));
        memset(cmdstr,0,sizeof(cmdstr));
        
        //流程交易
        ret = doTrade(send.c_str(), _cfgMap["flowip"].c_str(),_cfgMap["flowport"].c_str(),result,cmd,cmdstr);
		memset(senddesc, '\0', sizeof(senddesc));
		strncpy(senddesc, cmdstr, 100);

        if(ret!=0)
        {
        	LogPrint(MULTIS_SEND_NODE, "启动流程任务失败[%d]!",ret);
			//范朋飞修改 把触发流程中网络中断的归类为sendstate=4
			if(ret==-4)
			{
				LogPrint(MULTIS_SEND_NODE, "流程应答超时!");
				strcpy(senddesc, "流程应答超时");
				m_db.dbo_update_sm_state_desc(TaskId, MULTIS_SENDSTATE_TOUCH_FAIL, senddesc, result);
			}
			else if(ret==-3)
			{
				LogPrint(MULTIS_SEND_NODE, "流程连接失败!");
				strcpy(senddesc, "流程连接失败");
				m_db.dbo_update_sm_state_desc(TaskId, MULTIS_SENDSTATE_FAIL, senddesc, result);
			}
			else
			{
				LogPrint(MULTIS_SEND_NODE, "流程失败!");
				strcpy(senddesc, "流程失败");
				m_db.dbo_update_sm_state_desc(TaskId, MULTIS_SENDSTATE_FAIL, senddesc, result);
			}

			sleep(3);
            continue;
        }

        if(result!=0)
        {
            if(strncmp(cmdstr,"SCD044516030001该影像处理流程已启动",35)!=0 )
            {
          		LogPrint(MULTIS_SEND_NODE, "启动流程任务失败[%s][%s]...",cmd,cmdstr);
				if(strlen(senddesc)==0)
					strcpy( senddesc, "启动流程失败");

          		m_db.dbo_update_sm_state_desc(TaskId, MULTIS_SENDSTATE_FAIL, senddesc, result);
				sleep(3);
            	continue;
            }
       	}
       	
       	LogPrint(MULTIS_SEND_NODE, "启动流程任务成功!");

		memset(senddesc,'\0', sizeof(senddesc));
       	m_db.dbo_update_sm_state_desc(TaskId,1, senddesc, 0);
	}
	return;
}

//获取配置
int CSENDM::get_config()
{
	//查询配置
	_cfgMap.clear();
	int ret = m_db.dbo_get_config(_cfgMap);
	if(ret)
	{
		LogPrint(MULTIS_SEND_NODE, "查询数据错误[%d]!",ret);	
		return ret;
	}		

	if(task_count%20==0)
	{
		LogPrint(MULTIS_SEND_NODE, "ECM[%s][%s]!",_cfgMap["ecmip"].c_str(),_cfgMap["ecmport"].c_str());	
		LogPrint(MULTIS_SEND_NODE, "FLOW[%s][%s]!",_cfgMap["flowip"].c_str(),_cfgMap["flowport"].c_str());	
	}

	//查询凭证类型
	_vtMap.clear();
	ret = m_db.dbo_get_vchtype(_vtMap);
	if(ret)
	{
		LogPrint(MULTIS_SEND_NODE, "查询数据错误[%d]!",ret);	
		return ret;
	}		

	ret = m_db.dbo_get_area(_areaMap);
	_areaMap.clear();
	if(ret)
	{
		LogPrint(MULTIS_SEND_NODE,"查询交换区域出错[%d]",ret);
		return ret;
	}

	return 0;
}
