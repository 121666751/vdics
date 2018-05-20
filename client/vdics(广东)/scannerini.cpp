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
            MessageBox(FmIC->Handle,"读取配置文件错误!\t","提示",MB_OK|MB_ICONINFORMATION);
            return -1;
        }

 	return 0;
}

//##ModelId=3AF7B6DD006D
int ScannerIni::SaveIniData()
{
    if((fp=fopen(_iniFile,"w+"))==NULL)
    {
        MessageBox(FmIC->Handle,"读取配置文件错误!\t","提示",MB_OK|MB_ICONINFORMATION);
        return -1;
    }
    fprintf(fp,"#              批量扫描程序配置文件\n");
    fprintf(fp,"#*******************************************************\n");
    fprintf(fp,"# scannername:扫描仪名称\n");
    fprintf(fp,"scannername=%s\n\n",m_scannerInfo.scanner_name);

    fprintf(fp,"#******************************************************\n");
    fprintf(fp,"# serial:扫描仪序列号\n");
    fprintf(fp,"serial=%d\n",m_scannerInfo.scanner_sn);

    fprintf(fp,"#******************************************************\n");
    fprintf(fp,"# prefix:扫描仪标号，只有一个字母，用于区别不同的扫描仪,\n");
    fprintf(fp,"#        此项是区别于其他扫描仪的标志，千万不能和其他的\n");
    fprintf(fp,"#        扫描仪重复\n");
    fprintf(fp,"prefix=%c\n\n",m_scannerInfo.prefix[0]);

    fprintf(fp,"#********************************************************\n");
    fprintf(fp,"# path:图像保存路径\n");
    fprintf(fp,"path=%s\n\n",m_scannerInfo.path);

    fprintf(fp,"#********************************************************\n");
    fprintf(fp,"# offset:基准偏移，此项仅对扫描基准不在中心的扫描仪起作\n");
    fprintf(fp,"#        用，如富士通系列，取值一般为 dpi*2.5左右\n");
    fprintf(fp,"offset=%d\n\n",m_scannerInfo.offset);

    fprintf(fp,"#********************************************************\n");
    fprintf(fp,"# pkgflag :是否按交换号扫描\n");
    fprintf(fp,"pkgflag=%d\n\n",m_scannerInfo.pkgflag);

    fprintf(fp,"#********************************************************\n");
    fprintf(fp,"# needrotate :是否启用硬件旋转\n");
    fprintf(fp,"needrotate=%d\n\n",m_scannerInfo.needRotate);

    fprintf(fp,"#********************************************************\n");
    fprintf(fp,"# calx  :X方向校准值\n");
    fprintf(fp,"# caly  :Y方向校准值\n");
    fprintf(fp,"calx=%8.6f\n",m_scannerInfo.calx);
    fprintf(fp,"caly=%8.6f\n\n",m_scannerInfo.caly);


    fprintf(fp,"#******************************************************\n");
    fprintf(fp,"# dpi   :扫描图片精度,用于控制扫描不同DPI的图像\n");
    fprintf(fp,"#	 一般为200或300\n");
    fprintf(fp,"\n");

    fprintf(fp,"# format:扫描图片类型 0：bmp格式 1:jpg格式\n");
    fprintf(fp,"# 注意  :因为该扫描程序主要采用File模式扫描图像，一些扫\n");
    fprintf(fp,"#        描仪不支持File模式下扫描JPG图像功能，\n");
    fprintf(fp,"#        如panasonic kv-s2045\n");
    fprintf(fp,"\n");

    fprintf(fp,"# rotate:图片需要旋转的角度,用于图像分发程序中对图像进行\n");
    fprintf(fp,"#        旋转，实际角度为 rotate*90度,取值0，1，2，3\n");
    fprintf(fp,"\n");

    fprintf(fp,"# sideA :正面扫描控制 \n");
    fprintf(fp,"#     0 :不扫正面\n");
    fprintf(fp,"#     1 :扫正面\n");
    fprintf(fp,"\n");

    fprintf(fp,"# sideB :反面扫描控制 \n");
    fprintf(fp,"#     0 :不扫正面\n");
    fprintf(fp,"#     1 :扫正面\n");
    fprintf(fp,"\n");

    fprintf(fp,"# porcA :正面运算控制 \n");
    fprintf(fp,"#     0 :不参加运算\n");
    fprintf(fp,"#     1 :参加运算\n");
    fprintf(fp,"\n");

    fprintf(fp,"# porcA :反面运算控制 \n");
    fprintf(fp,"#     0 :不参加运算\n");
    fprintf(fp,"#     1 :参加运算\n");
    fprintf(fp,"\n");

    fprintf(fp,"# vchsizex:票据X方向扫描范围,单位英寸\n");
    fprintf(fp,"# vchsizey:票据Y方向扫描范围,单位英寸\n");
    fprintf(fp,"# 一般支票 vchsizex=3.2 vchsizey=6.7\n");
    fprintf(fp,"\n");

    fprintf(fp,"# colortype:扫描图像的颜色\n");
    fprintf(fp,"#         0:彩色(RGB24)\n");
    fprintf(fp,"#         1:灰色(GRAY256)\n");
    fprintf(fp,"#         2:黑白(BlackWhite)\n");
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
    fprintf(fp,"# vchcount   :当前配置文件中扫描凭证的种类数目\n");
    fprintf(fp,"vchcount=%d\n",i);

    fclose(fp);
  	return 0;
}

