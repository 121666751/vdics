/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
 *
 ***************************************************************************
 */
#ifndef __ZTDEFS_H__
#define __ZTDEFS_H__
#include <string.h> 
#include <map>
#include <vector>
#include <iostream>

using namespace std;


enum 
{
	UNKNOW_DEAL_CODE  		= -100,
	
	XML_STAMP_ERROR 			= -1000,
	XML_IMAGE_ERROR 			= -1001,
	XML_DPI_ERROR   			= -1002,
	XML_SIZEX_ERROR   		= -1003,
	XML_SIZEY_ERROR   		= -1004,
	XML_BANDS_ERROR				= -1005
};


inline char* ErrorDecode(int errNo)
{
	switch(errNo) 
	{
		case UNKNOW_DEAL_CODE:		return "未知交易!";
		case XML_STAMP_ERROR:			return "无印鉴库!";
		case XML_IMAGE_ERROR: 		return "无验印图像!";		
		case XML_DPI_ERROR:		    return "验印图像DPI不对!";
		case XML_SIZEX_ERROR:			return "验印图像sizex不对!";
		case XML_SIZEY_ERROR:			return "验印图像sizey不对!";
		case XML_BANDS_ERROR:			return "验印图像bands不对!";
		default:									return "未知错误!";
	}
}

#endif

