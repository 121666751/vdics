#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../common/ConfigFile.h"
#include "../common/PubFun.h"
#include "../common/ExportLog.h"	
#include <zsocket.hpp>


int verbose=0;

static void usage(void)
{
	printf("Usage: ./gsipsrv or ./gsipsrv -v\n");
}

//???к?
string brno;

int stopsrv(char*svname)
{
	int n=1,pid=-1;
	char cmd[128];

	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "pkill %s", svname);
	printf("%s\n", cmd);
	system(cmd);					

	/*
	while(n>0)
	{	
		char cmd[64];
		n = find_pid_by_name(svname, pid);	
		if(n>0)
		{
			memset(cmd, 0, sizeof(cmd));
			sprintf(cmd, "kill -9 %d", pid);
			printf("%s\n", cmd);
			system(cmd);					
			usleep(200);
		}
		else
			break;
	}	
	*/
	
	return 0;
}
int main(int argc,char*argv[])
{	
	
	int ch=0, verbose=0;
    
  while (argc > 1 && ((ch = getopt(argc, argv, "vn:")) != -1))
  {   
      switch (ch)
      {
          case 'v':
              verbose = 1;
              break;  
          case 'n':
              usage();
              return -1; 
      }
  }   
    
	int n = 0,ret,pid, i;
			 	
 	int 	svcount=0;
 	int 	smcount=0;
 	int 	pvcount=0;
 	int 	pmcount=0;
 	int 	tmcount=0;
 	int 	pacount=0;
 	int 	sacount=0;
 	int 	picount=0;
 	int 	sicount=0;
 	char	tkaddr[24];
 	int		tkport;
	try
	{
		Config mycfg("./config/config.cfg");
		
		brno=mycfg.value("brno",1);
		if(brno.size()<=0)
		{
			LogPrint( TASK_NODE, "brno cann't be null.\n");
			printf( "brno cann't be null.", brno.c_str());
			return -1;
		}
		else if(brno.size()>6)
		{
			LogPrint( TASK_NODE, "brno error.\n");
			return -1;
		}

		printf("brno...->[%s]...\n", brno.c_str());

			
		if(argc == 2)
		{
			if(strcmp(argv[1],"stop")==0)
			{
				stopsrv("tmsrv");
			 	stopsrv("pmsrv");
			 	stopsrv("pvsrv");
			 	stopsrv("svsrv");
			 	stopsrv("smsrv");
			 	stopsrv("pisrv");
			 	stopsrv("sisrv");

				LogPrint(TASK_NODE, "gsipsrv stop complete.\n");
				printf("gsipsrv stop complete.\n");
				return 0;
			}
		}

		
		bzero(tkaddr,sizeof(tkaddr));
		  
		string szTmp;
		szTmp=mycfg.value("svcount",1);
      	if(szTmp.size()>0)
      	{
         	svcount = stoi(szTmp);
      	}
      	else
      	{
      		svcount = 0;
      	}
      	/*
	szTmp=mycfg.value("sicount",1);
      	if(szTmp.size()>0)
      	{
         	sicount = stoi(szTmp);
      	}
      	else
      	{
      		sicount = 0;
      	}
      	*/
      	szTmp=mycfg.value("smcount",1);
      	if(szTmp.size()>0)
      	{
         	smcount = stoi(szTmp);
      	}
      	else
      	{
      		smcount = 0;
      	}
      	szTmp=mycfg.value("pvcount",1);
      	if(szTmp.size()>0)
      	{
         	pvcount = stoi(szTmp);
      	}
      	else
      	{
      		pvcount = 0;
      	}
      	
      	szTmp=mycfg.value("pmcount",1);
      	if(szTmp.size()>0)
      	{
         	pmcount = stoi(szTmp);
      	}
      	else
      	{
      		pmcount = 0;
      	}
    	/*
      	szTmp=mycfg.value("picount",1);
      	if(szTmp.size()>0)
      	{
         	picount = stoi(szTmp);
      	}
      	else
      	{
      		picount = 0;
      	}
      	*/
      	szTmp=mycfg.value("tmcount",1);
      	if(szTmp.size()>0)
      	{
         	tmcount = stoi(szTmp);
      	}
      	else
      	{
      		tmcount = 0;
      	}
      	
    
      	
      	szTmp=mycfg.value("task_ip",1);
      	if(szTmp.size()>0)
      	{
         	strcpy(tkaddr,szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(TASK_NODE, "task ip cann't be null!\n");
      	}
      	szTmp=mycfg.value("task_port",1);
      	if(szTmp.size()>0)
      	{
      		tkport = stoi(szTmp.c_str());
      	}
      	else
      	{
      		LogPrint(TASK_NODE, "task port cann't be null!\n");
      	}
    }
    catch(...)
    {
    	LogPrint(TASK_NODE, "error!\n");	
    }
   
    LogPrint(TASK_NODE,"begin to start server..."); 
    if(tmcount>0)
		{
			LogPrint(TASK_NODE, "start tmsrv");
			ZSocket so;
			ret = so.Connect(tkaddr,tkport);
			if(ret)
			{	
				LogPrint(TASK_NODE, "Start TaskManager[%s][%d]!\n",tkaddr,tkaddr);
				{
					LogPrint(TASK_NODE, "Starting TaskManager.!\n");
					if(!verbose)
						system("./tmsrv 2>1 1>/dev/null &");
					else
						system("./tmsrv 2>1 1>/dev/null -v &");		
				}
			}
			else
			{
				char cmd[24];
				ret = so.Send((void*)"00000006010000",14);
	  			if(ret<=0)
				{	
					LogPrint(TASK_NODE, "Shutdown TaskManager!\n");
					n = find_pid_by_name("tmsrv", pid);	
					if(n>0)
					{
						memset(cmd, 0, sizeof(cmd));
						sprintf(cmd, "kill -9 %d", pid);
						system(cmd);					
					}
					LogPrint(TASK_NODE, "restarting TaskManager!\n");
					system("./tmsrv &");	
				}	
				else
				{
				
					memset(cmd,0,sizeof(cmd));
				
					ret = so.RecvEx(cmd,sizeof(cmd));
					if(ret<=0)
					{
						
						LogPrint(TASK_NODE, "Send heart-data!\n");
						n = find_pid_by_name("tmsrv", pid);	
						if(n>0)
						{
							memset(cmd, 0, sizeof(cmd));
							sprintf(cmd, "kill -9 %d", pid);
							system(cmd);					
						}
						LogPrint(TASK_NODE, "start tmsrv!\n");
						if(!verbose)
							system("./tmsrv &");
						else
							system("./tmsrv -v &");			
					}	
					else
					{
						LogPrint(TASK_NODE, "Recv heart-data[%s]!\n",cmd);	
					}
		  		}
			}
			
			sleep(2);
		}
		n = find_pid_by_name("svsrv", pid);	
		for (int i = 0; i < svcount - n; i++)
		{
			LogPrint(TASK_NODE, "start svsrv!\n");
			if(!verbose)
				system("./svsrv 2>1 1>/dev/null &");
			else
				system("./svsrv 2>1 1>/dev/null -v &");
		}
		n = find_pid_by_name("smsrv", pid);	
		for (int i = 0; i < smcount - n; i++)
		{
			LogPrint(TASK_NODE, "start smsrv!\n");
			if(!verbose)
				system("./smsrv 2>1 1>/dev/null &");
			else
				system("./smsrv 2>1 1>/dev/null -v &");
		}
		n = find_pid_by_name("pvsrv", pid);	
		for (int i = 0; i < pvcount - n; i++)
		{
			LogPrint(TASK_NODE, "start pvsrv!\n");
			if(!verbose)
				system("./pvsrv 2>1 1>/dev/null &");
			else
				system("./pvsrv 2>1 1>/dev/null -v &");
		}	
		n = find_pid_by_name("pmsrv", pid);	
		for (int i = 0; i < pmcount - n; i++)
		{
			LogPrint(TASK_NODE, "start pmsrv!\n");
			if(!verbose)
				system("./pmsrv 2>1 1>/dev/null &");
			else
				system("./pmsrv 2>1 1>/dev/null -v &");
		}
		
		n = find_pid_by_name("pisrv", pid);	
		for (int i = 0; i < picount - n; i++)
		{
			LogPrint(TASK_NODE, "start pisrv!\n");
			if(!verbose)
				system("./pisrv 2>1 1>/dev/null &");
			else
				system("./pisrv 2>1 1>/dev/null -v &");
		}
		
		n = find_pid_by_name("sisrv", pid);	
		for (int i = 0; i < sicount - n; i++)
		{
			LogPrint(TASK_NODE, "start sisrv!\n");
			if(!verbose)
				system("./sisrv 2>1 1>/dev/null &");
			else
				system("./sisrv 2>1 1>/dev/null -v &");
		}

   		LogPrint(TASK_NODE,"gsipsrv start complete.\n");
		printf("gsipsrv start complete.\n");
		
    exit(0);
}
