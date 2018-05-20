
#ifndef __TC_PATTER_PRNOCR_SCAN_H__
#define __TC_PATTER_PRNOCR_SCAN_H__

#ifdef TC_PATTERN_PRNOCR_EXPORTS
#define TC_PATTERN_PRNOCR_API __declspec(dllexport)
#else
#define TC_PATTERN_PRNOCR_API __declspec(dllimport)
#endif

/*
 作用：根据输入文件识别版面、读取帐号
 Input:
	pImportFile:图像文件名。可以是jpg格式。
	dpi:		图像分辨率。要求在100~300之间。
	type:		操作类型。1:识别版面; 2:识别版面,并获得 磁码号;其它:不操作.
	vt:			存储版面分析结果。返回的票据类型: 0:未知版面; 1:承兑汇票; 2:信封; 3:贷记凭证; 4:本票; 5:支票;6:代收;7:代付
	vchno:		识别出的帐号的结果.
 Output:
	0:	sucessful
	1:	reading image error! maybe pImportFile is not existed!
	2:	the pattern of image is not one of we kowned.
	3:	the size of image is too small.
	4:	recognize number failure!
*/

//TC_PATTERN_PRNOCR_API int  image_process(MIL_ID* pMilSystem, char* pImportFile, int dpi, int type, int &vt, char* vchno);
TC_PATTERN_PRNOCR_API int  image_process_scan(void*pSystem, char* pImportFile, int dpi, int type, int &vt, char* vchno,
								  int cfg_no);

#endif
