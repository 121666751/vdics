/*
 ***************************************************************************
 * ����   Nie
 * ��ַ�� www.tchzt.com
 * ���䣺 niejianjun@tchzt.com
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
		case UNKNOW_DEAL_CODE:		return "δ֪����!";
		case XML_STAMP_ERROR:			return "��ӡ����!";
		case XML_IMAGE_ERROR: 		return "����ӡͼ��!";		
		case XML_DPI_ERROR:		    return "��ӡͼ��DPI����!";
		case XML_SIZEX_ERROR:			return "��ӡͼ��sizex����!";
		case XML_SIZEY_ERROR:			return "��ӡͼ��sizey����!";
		case XML_BANDS_ERROR:			return "��ӡͼ��bands����!";
		default:									return "δ֪����!";
	}
}

#endif

