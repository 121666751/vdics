/*
 ***************************************************************************
 * 作者   Nie
 * 网址： www.tchzt.com
 * 邮箱： niejianjun@tchzt.com
 *
 ***************************************************************************
 */
#ifndef __TRANSIMG_H__
#define __TRANSIMG_H__

#include "mem.h"

#ifdef __cplusplus
extern "C" {
#endif

	/**
	*函数ti_getfile()用于取图像文件  
	*@param filename:		文件名，输入参数
	*@param ip:			图像分发服务的ip地址,输入参数
	*@param imgFileData:	图像文件数据,输出参数
	*@return 0---成功，其他---失败
	*/
	int ti_getfile( const char* filename, const char* ip,int port, MemT& imgFileData);

	/**
	*函数ti_delfile()用于删除图像文件  
	*@param filename:		文件名，输入参数
	*@param ip:			图像分发服务的ip地址,输入参数
	*@return 0---成功，其他---失败
	*/
	int ti_delfile( const char* filename, const char* ip,int port);

	/**
	*函数ti_getfileformat()用于取指定格式的文件，目前只用于调取jpg文件
	*@param filename:		完整的文件名，输入参数
	*@param format:		文件格式，输入参数
	*@param rate:			压缩比，输入参数
	*@param imgFileData:	图像文件数据，输出参数
	*@return 0---成功，其他---失败
	*/
	int ti_getfileformat( const char* filename, const char* format, const char* ip, int port,int rate, MemT& imgFileData);
	/**
	*函数ti_filetoRGB()用于将图像文件数据转成通道图数据
	*@param imgFileData:	图像文件数据,输入参数
	*@param format:		图像格式（"bmp","jpg"等），输入参数
	*@param pRGB:			通道图数据，输出参数
	*@param sizeX:			图像宽度，输出参数
	*@param sizeY:			图像高度，输出参数
	*@param pannel:		通道数，输出参数
	*@return 0---成功，其他---失败
	*/
	int ti_filetoRGB(  MemT& imgFileData, const char* format, MemT& pRGB, int& sizeX, int& sizeY, int& pannel );
	/**
	*函数ti_getfileformat_ex()用于取指定格式的文件，目前只用于调取jpg文件
	*@param filename:		完整的bmp文件名，输入参数
	*@param format:		文件格式，输入参数
	*@param rate:			压缩比，输入参数
	*@param angel          旋转角度，输入参数
	*@param scale          缩放比例，输入参数
	*@param savename:	  图像文件数据，输出参数
	*@return 0---成功，其他---失败
	*/
	int ti_getfileformat_ex( const char* filename, const char* format, const char* ip,int port, int rate,int angel,double scale, const char * savename);

	/**
	*函数ti_getfileformat_ex2()用于取指定格式的文件，目前只用于调取jpg文件
	*@param filename:		完整的bmp文件名，输入参数
	*@param format:		文件格式，输入参数
	*@param rate:			压缩比，输入参数
	*@param angel          旋转角度，输入参数
	*@param scale          缩放比例，输入参数
	*@param imgFileData:	图像文件数据，输出参数
	*@return 0---成功，其他---失败
	*/
	int ti_getfileformat_ex2( const char* filename, const char* format, const char* ip,int port, int rate,int angel,double scale, MemT &imgData);
/**
 *函数ti-rotateimg()用于图像数据的旋转
 *@param image:		    通道图数据，既作输入也作输出
 *@param sizeX:			图像宽度，既作输入也作输出
 *@param sizeY:			图像高度，既作输入也作输出
 *@param pannel:		通道数，输入参数
 *@param angle:			旋转角度，输入参数
 *@return 0---成功，其他---失败
 */
int ti_rotateimg( MemT& image, int& sizeX, int& sizeY, int pannel, int angle);

/**
 *函数ti_getimage()调用以上三个函数，用于取图像文件并进行转通道图和旋转
 *@param filename:		文件名，输入参数
 *@param ip:			图像分发服务的ip地址,输入参数
 *@param angle			旋转角度，输入参数
 *@param pRGB:			旋转后的图像数据,输出参数
 *@param sizeX:			图像宽度,输出参数
 *@param sizeY:			图像高度,输出参数
 *@param pannel:		通道数,输出参数
 *@return 0---成功，其他---失败
 */
int ti_getimage( const char* filename, const char* ip,int port, int angle, 
			    MemT& pRGB, int& sizeX, int& sizeY, int& pannel );
/**
 *函数ti_getimagefast()用于调取jpg图像，输入扩展名为bmp的图像的文件名，在imgsrv端将bmp文件转成同名的jpg文件，发送过来，再进行转通道图和旋转
 *@param filename:		文件名,从数据库imgfile中取得，输入参数
 *@param ip:			图像分发服务的ip地址,输入参数
 *@param angle:			旋转角度，输入参数
 *@param pRGB:			旋转后的图像数据,输出参数
 *@param sizeX:			图像宽度,输出参数
 *@param sizeY:			图像高度,输出参数
 *@param pannel:		通道数,输出参数
 *@return 0---成功，其他---失败
 */
int ti_getimagefast(const char* filename, const char* ip, int port,int angle,
				  MemT& pRGB, int& sizeX, int& sizeY, int& pannel);


#ifdef __cplusplus
}
#endif

#endif
