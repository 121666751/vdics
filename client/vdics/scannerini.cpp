#include "ScannerIni.h"
#include <vcl.h>
#include <zcommon.h>
#include "ConfigFile.h"
#include "main.h"


ScannerIni::ScannerIni()
{
	memset(m_iniPath,0,sizeof(m_iniPath));
	strcpy(m_iniPath,FmIC->gPath.c_str());
//    GetCurrentDirectory(sizeof(m_iniPath),m_iniPath);
    strcat(m_iniPath,"\\config");
    debugout("m_iniPath = %s\n",m_iniPath);
    pList=new VchList;

}
ScannerIni::~ScannerIni()
{
  delete pList;

}
//##ModelId=3AD14CDC0066
int ScannerIni::LoadIniData(char * filename)
{
    memset(&m_scannerInfo,0,sizeof(m_scannerInfo));
    sprintf(_iniFile,"%s\\%s",m_iniPath,filename);
    String tmp;
    try
       {
          Config scanner(_iniFile);

          tmp=scanner.value("scannername",1);
          if(tmp.Trim().c_str()!=NULL)
           {
            strcpy(m_scannerInfo.scanner_name,tmp.Trim().c_str());
            debugout("scanner name:%s\n",tmp.c_str());
            }
          else
            strcpy(m_scannerInfo.scanner_name,"");

          tmp=scanner.value("serial",1);
          if(tmp.Trim().c_str()!=NULL)
          {
            m_scannerInfo.scanner_sn=tmp.Trim().ToInt();
            debugout("serial :%s\n",tmp.c_str());
            }
          else
            m_scannerInfo.scanner_sn=0;

          tmp=scanner.value("prefix",1);
          if(tmp.Trim().c_str()!=NULL)
           {

            strcpy(m_scannerInfo.prefix,tmp.Trim().c_str());

            debugout("perfix:%c\n",m_scannerInfo.prefix[0]);
            }
          else
            strcpy(m_scannerInfo.prefix,"A");

          tmp=scanner.value("offset",1);
          if(tmp.Trim().c_str()!=NULL)
           {
            m_scannerInfo.offset=tmp.Trim().ToInt();
            debugout("offet:%s\n",tmp.c_str());
            }
          else m_scannerInfo.offset=0;
          tmp=scanner.value("calx",1);
          if(tmp.Trim().c_str()!=NULL)
          {
            m_scannerInfo.calx=tmp.Trim().ToDouble();
            debugout("calx:%s\n",tmp.c_str());
            }
          else
            m_scannerInfo.calx=1.00;

          tmp=scanner.value("caly",1);
          if(tmp.Trim().c_str()!=NULL)
           {
            m_scannerInfo.caly=tmp.Trim().ToDouble();
            debugout("caly:%s\n",tmp.c_str());
            }
          else
            m_scannerInfo.caly=1.00;

          tmp=scanner.value("path",1);
          if(tmp.Trim().c_str()!=NULL)
          {
              strcpy(m_scannerInfo.path,tmp.Trim().c_str());
              debugout("path:%s\n",tmp.c_str());
          }
          else
            strcpy(m_scannerInfo.path,"c:\\");



          tmp=scanner.value("needrotate",1);
          if(tmp.Trim().c_str()!=NULL)
          {
            m_scannerInfo.needRotate=tmp.Trim().ToInt();
            debugout("needRotate:%s\n",tmp.c_str());
         }
         else m_scannerInfo.needRotate=0;

          tmp=scanner.value("pkgflag",1);
          if(tmp.Trim().c_str()!=NULL)
          {
            m_scannerInfo.pkgflag=tmp.Trim().ToInt();
            debugout("pkgflag:%s\n",tmp.c_str());
          }
          else
            m_scannerInfo.pkgflag=0;
		/*			
		tmp=scanner.value("SaveDate",1);
          if(tmp.Trim().c_str()!=NULL)
          {
			  debugout("SaveDate = %s",tmp.Trim().c_str());
            m_Date = tmp.Trim().ToInt();
            debugout("SaveData :%d\n",m_Date);
            }
          else
           */ m_Date=30;
					
          tmp=scanner.value("vchcount",1);
          if(tmp.Trim().c_str()!=NULL)
           {
            m_scannerInfo.vchtcount=tmp.Trim().ToInt();
            debugout("vchcount:%d\n",tmp.Trim().ToInt());
            }
          else
            m_scannerInfo.vchtcount=0;

           String block;
           pList->myList.clear();

           for(int i=0;i<m_scannerInfo.vchtcount;i++)
           {
                block.sprintf("VchType%02d",i+1);

                m_vchType.sid=i;

                tmp=scanner.value(block.c_str(),"vchname",1);
                if(tmp.Trim().c_str()!=NULL)
                {
                    strcpy(m_vchType.vchname,tmp.Trim().c_str());
                    debugout("vchname:%s\n",tmp.Trim().c_str());
                }
                else
                    strcpy(m_vchType.vchname,"vchname");

                tmp=scanner.value(block.c_str(),"dpi",1);
                if(tmp.Trim().c_str()!=NULL)
                {
                     m_vchType.dpi=tmp.Trim().ToInt();
                     debugout("dpi:%s\n",tmp.c_str());
                }
                else
                    m_vchType.dpi=0;

                tmp=scanner.value(block.c_str(),"format",1);
                if(tmp.Trim().c_str()!=NULL)
                {
                    m_vchType.format=tmp.Trim().ToInt();
                    debugout("format:%s\n",tmp.c_str());
                }
                else
                    m_vchType.format=0;

                tmp=scanner.value(block.c_str(),"rotate",1);
                if(tmp.Trim().c_str()!=NULL)
                {
                    m_vchType.rotate=tmp.Trim().ToInt();
                    debugout("rotate:%s\n",tmp.c_str());
                }
                else
                    m_vchType.rotate=0;

                tmp=scanner.value(block.c_str(),"sideA",1);
                if(tmp.Trim().c_str()!=NULL)
                {
                    m_vchType.sideA=tmp.Trim().ToInt();
                    debugout("sideA:%s\n",tmp.c_str());
                 }
                else
                    m_vchType.sideA=0;


                tmp=scanner.value(block.c_str(),"sideB",1);
                if(tmp.Trim().c_str()!=NULL)
                {
                    m_vchType.sideB=tmp.Trim().ToInt();
                    debugout("sideB:%s\n",tmp.c_str());
                }
                else
                    m_vchType.sideB=0;

                tmp=scanner.value(block.c_str(),"procA",1);
                if(tmp.Trim().c_str()!=NULL)
                {
                    m_vchType.procA=tmp.Trim().ToInt();
                    debugout("procA:%s\n",tmp.c_str());
                }
                else
                    m_vchType.procA=0;


                tmp=scanner.value(block.c_str(),"procB",1);
                if(tmp.Trim().c_str()!=NULL)
                {
                    m_vchType.procB=tmp.Trim().ToInt();
                    debugout("procB:%s\n",tmp.c_str());
                }
                else
                    m_vchType.procB=0;

                tmp=scanner.value(block.c_str(),"vchsizex",1);
                if(tmp.Trim().c_str()!=NULL)
                {
                    m_vchType.vch_sizex=tmp.Trim().ToDouble();
                    debugout("vchsizex:%s\n",tmp.c_str());
                }
                else
                    m_vchType.vch_sizex=3.2;

                tmp=scanner.value(block.c_str(),"vchsizey",1);
                if(tmp.Trim().c_str()!=NULL)
                {
                    m_vchType.vch_sizey=tmp.Trim().ToDouble();
                    debugout("vchsizey:%s\n",tmp.c_str());
                }
                else
                    m_vchType.vch_sizey=6.7;



                tmp=scanner.value(block.c_str(),"colortype",1);
                if(tmp.Trim().c_str()!=NULL)
                {
                    m_vchType.colortype=tmp.Trim().ToInt();
                    debugout("colortype :%s\n",tmp.c_str());
                }
                else
                    m_vchType.colortype=0;

                tmp=scanner.value(block.c_str(),"scantype",1);
                if(tmp.Trim().c_str()!=NULL)
                {
                    m_vchType.scantype=tmp.Trim().ToInt();
                    debugout("scantype :%s\n",tmp.c_str());
                }
                else
                    m_vchType.scantype=0;

                 if(i==0)  m_scannerInfo.onevch=m_vchType;
                 pList->myList.push_back(m_vchType);
                
             }


        }
        catch(...)
        {
            MessageBox(FmIC->Handle,"��ȡ�����ļ�����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
            return -1;
        }

 	return 0;
}

//##ModelId=3AF7B6DD006D
int ScannerIni::SaveIniData()
{
    if((fp=fopen(_iniFile,"w+"))==NULL)
    {
        MessageBox(FmIC->Handle,"��ȡ�����ļ�����!\t","��ʾ",MB_OK|MB_ICONINFORMATION);
        return -1;
    }
    fprintf(fp,"#              ����ɨ����������ļ�\n");
    fprintf(fp,"#*******************************************************\n");
    fprintf(fp,"# scannername:ɨ��������\n");
    fprintf(fp,"scannername=%s\n\n",m_scannerInfo.scanner_name);

    fprintf(fp,"#******************************************************\n");
    fprintf(fp,"# serial:ɨ�������к�\n");
    fprintf(fp,"serial=%d\n",m_scannerInfo.scanner_sn);

    fprintf(fp,"#******************************************************\n");
    fprintf(fp,"# prefix:ɨ���Ǳ�ţ�ֻ��һ����ĸ����������ͬ��ɨ����,\n");
    fprintf(fp,"#        ����������������ɨ���ǵı�־��ǧ���ܺ�������\n");
    fprintf(fp,"#        ɨ�����ظ�\n");
    fprintf(fp,"prefix=%c\n\n",m_scannerInfo.prefix[0]);

    fprintf(fp,"#********************************************************\n");
    fprintf(fp,"# path:ͼ�񱣴�·��\n");
    fprintf(fp,"path=%s\n\n",m_scannerInfo.path);

    fprintf(fp,"#********************************************************\n");
    fprintf(fp,"# offset:��׼ƫ�ƣ��������ɨ���׼�������ĵ�ɨ��������\n");
    fprintf(fp,"#        �ã��縻ʿͨϵ�У�ȡֵһ��Ϊ dpi*2.5����\n");
    fprintf(fp,"offset=%d\n\n",m_scannerInfo.offset);

    fprintf(fp,"#********************************************************\n");
    fprintf(fp,"# pkgflag :�Ƿ񰴽�����ɨ��\n");
    fprintf(fp,"pkgflag=%d\n\n",m_scannerInfo.pkgflag);

    fprintf(fp,"#********************************************************\n");
    fprintf(fp,"# needrotate :�Ƿ�����Ӳ����ת\n");
    fprintf(fp,"needrotate=%d\n\n",m_scannerInfo.needRotate);

    fprintf(fp,"#********************************************************\n");
    fprintf(fp,"# calx  :X����У׼ֵ\n");
    fprintf(fp,"# caly  :Y����У׼ֵ\n");
    fprintf(fp,"calx=%8.6f\n",m_scannerInfo.calx);
    fprintf(fp,"caly=%8.6f\n\n",m_scannerInfo.caly);


    fprintf(fp,"#******************************************************\n");
    fprintf(fp,"# dpi   :ɨ��ͼƬ����,���ڿ���ɨ�費ͬDPI��ͼ��\n");
    fprintf(fp,"#	 һ��Ϊ200��300\n");
    fprintf(fp,"\n");

    fprintf(fp,"# format:ɨ��ͼƬ���� 0��bmp��ʽ 1:jpg��ʽ\n");
    fprintf(fp,"# ע��  :��Ϊ��ɨ�������Ҫ����Fileģʽɨ��ͼ��һЩɨ\n");
    fprintf(fp,"#        ���ǲ�֧��Fileģʽ��ɨ��JPGͼ���ܣ�\n");
    fprintf(fp,"#        ��panasonic kv-s2045\n");
    fprintf(fp,"\n");

    fprintf(fp,"# rotate:ͼƬ��Ҫ��ת�ĽǶ�,����ͼ��ַ������ж�ͼ�����\n");
    fprintf(fp,"#        ��ת��ʵ�ʽǶ�Ϊ rotate*90��,ȡֵ0��1��2��3\n");
    fprintf(fp,"\n");

    fprintf(fp,"# sideA :����ɨ����� \n");
    fprintf(fp,"#     0 :��ɨ����\n");
    fprintf(fp,"#     1 :ɨ����\n");
    fprintf(fp,"\n");

    fprintf(fp,"# sideB :����ɨ����� \n");
    fprintf(fp,"#     0 :��ɨ����\n");
    fprintf(fp,"#     1 :ɨ����\n");
    fprintf(fp,"\n");

    fprintf(fp,"# porcA :����������� \n");
    fprintf(fp,"#     0 :���μ�����\n");
    fprintf(fp,"#     1 :�μ�����\n");
    fprintf(fp,"\n");

    fprintf(fp,"# porcA :����������� \n");
    fprintf(fp,"#     0 :���μ�����\n");
    fprintf(fp,"#     1 :�μ�����\n");
    fprintf(fp,"\n");

    fprintf(fp,"# vchsizex:Ʊ��X����ɨ�跶Χ,��λӢ��\n");
    fprintf(fp,"# vchsizey:Ʊ��Y����ɨ�跶Χ,��λӢ��\n");
    fprintf(fp,"# һ��֧Ʊ vchsizex=3.2 vchsizey=6.7\n");
    fprintf(fp,"\n");

    fprintf(fp,"# colortype:ɨ��ͼ�����ɫ\n");
    fprintf(fp,"#         0:��ɫ(RGB24)\n");
    fprintf(fp,"#         1:��ɫ(GRAY256)\n");
    fprintf(fp,"#         2:�ڰ�(BlackWhite)\n");
    fprintf(fp,"#********************************************************\n");


    VCHTYPELIST::iterator  pos= pList->myList.begin();
    int i=0;
    while( pos !=pList->myList.end())
	{
        i=i+1;
	    {
            fprintf(fp,"[VchType%02d]\n",i);
            fprintf(fp,"vchname=%s\n",pos->vchname);
            fprintf(fp,"dpi=%d\n",pos->dpi);
            fprintf(fp,"format=%d\n",pos->format);
            fprintf(fp,"rotate=%d\n",pos->rotate);
            fprintf(fp,"sideA=%d\n", pos->sideA);
            fprintf(fp,"sideB=%d\n", pos->sideB);
            fprintf(fp,"procA=%d\n", pos->procA);
            fprintf(fp,"procB=%d\n", pos->procB);
            fprintf(fp,"vchsizex=%8.6f\n",pos->vch_sizex);
            fprintf(fp,"vchsizey=%8.6f\n",pos->vch_sizey);
            fprintf(fp,"colortype=%d\n\n",  pos->colortype);
            fprintf(fp,"scantype=%d\n\n",  pos->scantype);

        }
        ++pos;
	}

    fprintf(fp,"#******************************************************\n");
    fprintf(fp,"# vchcount   :��ǰ�����ļ���ɨ��ƾ֤��������Ŀ\n");
    fprintf(fp,"vchcount=%d\n",i);

    fclose(fp);
  	return 0;
}

