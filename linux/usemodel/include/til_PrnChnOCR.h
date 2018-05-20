///////////////////////////////////////////////////////////////////////////////////
//
//			天创征腾(TM)印刷汉字OCR处理函数库  头文件
//
//			北京天创征腾信息科技有限公司		2010年9月
//
//			设计者:邱锐  黄雅 李华龙
//
//			版权所有(2010)
//
////////////////////////////////////////////////////////////////////////////////////

#ifndef	__TIL_PRNCHNOCR__H__
#define	__TIL_PRNCHNOCR__H__

//#include "til_segment.h"

#define VOUCHTYPE_WEISHOU	1
#define VOUCHTYPE_FEISHUI	2

//返回码
#define	RETURN_READCONFIG_ERROR		-1
#define	RETURN_IMAGESIZE_ERROR		-2
#define	RETURN_PREIMAGESIZE_ERROR	-3
#define	RETURN_CALLINE_ERROR			-4
#define	RETURN_SEGAMOUNTSIZE_ERROR	-5
#define	RETURN_EXACTCALPOS_ERROR		-6
#define	RETURN_SEGAMOUNTEXACT_ERROR	-7
#define	RETURN_LOADMOD_ERROR		-8

/*根据票据种类，完成对打印汉字金额串的分割和识别。（接口函数）
输入：
	pSrcImg		待识别的含有打印汉字金额字符串的图像(全幅面图像）
	sizeX			源图像宽度
	sizeY			源图像高度
	nChannels:		pSrcImg的通道数。（只允许输入1或3）
	dpi:			pSrcImg的dpi。（应大于等于200dpi）
	vouchType		票据种类（图像二值化分割依据）
	recogRlt:		（返回）识别结果（转换为数字的）
	vouchSn:		（返回）票据的联数。
输出：
0 - 成功；其他 - 失败
*/
int Til_SegmentPrnChnChar(unsigned char *pSrcImg, int sizeX, int sizeY, int nChannels, int dpi, int vouchType, double &recogRlt, int &vouchSn);

void  Til_SegmentPrnChnCharVersion(char *version);

void Til_SegmentPrnChnCharCreateReadme();
#endif
