/////////////////////////////////////////////////////////////////////////////////
//
// 	 TC_Pattern_PrnOCR :	根据输入图像判别版式和识别磁码
//							设计需求由上海交行提交
//
// 	 北京天创征腾信息科技有限公司   2008年11月
//
// 	 设计者：黄雅
// 	 编码者：HuangYa
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef __TC_PATTER_E13B_SCAN_H__
#define __TC_PATTER_E13B_SCAN_H__

#ifdef TC_PATTERN_E13B_SCAN_EXPORTS
#define TC_PATTERN_E13B_SCAN_API __declspec(dllexport)
#else
#define TC_PATTERN_E13B_SCAN_API __declspec(dllimport)
#endif

/*
 作用：根据输入文件识别版面、读取帐号
 Input:
	pImportFile:图像文件名。可以是jpg格式。
	dpi:		图像分辨率。要求在100~300之间。
	type:		操作类型。1:识别版面; 2:识别版面,并获得磁码号;其它:不操作
	vt:			存储版面分析结果。返回的票据类型: 0:未知版面; 1:承兑汇票; 2:信封; 3:贷记凭证; 4:本票; 5:支票;6:代收;7:代付
	vchno:		识别出的磁码结果(当type=2,且为信封或代收/代付时才进行操作).如果不需要进行磁码操作,则返回空.
	cfg_no:		配置文件的编号.本工程对应的配置文件名为:pattern_bocomm_scan**.cfg
 Output:
	0:	sucessful
	-1:	输入的cfg_no值不一样
	-2: 读取模板数据出错
	-3: 读图有误
	1:	识别版面出错。
	2:	求出的磁码区域出错！
	-6:	识别磁码出错!
*/

//TC_PATTERN_E13B_SCAN_API int  image_process_scan(void*pSystem, char* pImportFile, int dpi, int type, int &vt, char* vchno,
//								  int cfg_no);

/*
作用：根据输入文件识别版面、读取帐号
 Input:
	pSystem:		系统指针.
	pImg_Ori:		待识别图像.rbg通道图.
	sizeX_Ori:		pImg_Ori sizeX.
	sizeY_Ori:		pImg_Ori sizeY.
	nChannel_Ori:	pImg_Ori's channels.目前规定为3.
	dpi:			图像分辨率。目前要求为200。
	type:			操作类型。1:识别版面; 2:识别版面,并获得磁码号;其它:不操作
	vt:				存储版面分析结果。返回的票据类型: 0:未知版面; 1:承兑汇票; 2:信封; 3:贷记凭证; 4:本票; 5:支票;6:代收;7:代付
	vchno:			识别出的磁码结果(当type=2,且为信封或代收/代付时才进行操作).如果不需要进行磁码操作,则返回空.
	cfg_no:			配置文件的编号.本工程对应的配置文件名为:pattern_bocomm_scan**.cfg
 Output:
	0:	sucessful
	-1:	输入的cfg_no值不一样
	-2: 读取模板数据出错
	-3: 通道数有误
	1:	识别版面出错。
	2:	求出的磁码区域出错！
	-6:	识别磁码出错!
*/
TC_PATTERN_E13B_SCAN_API int  image_process_scan_1(void*pSystem, unsigned char *pImg_Ori, int sizeX_Ori, int sizeY_Ori, 
												 int nChannel_Ori, int dpi, int type, int &vt, char* vchno, int cfg_no);

#endif
