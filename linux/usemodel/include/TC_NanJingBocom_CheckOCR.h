/////////////////////////////////////////////////////////////////////////////////
//
// 	 TC_NanJingBocom_CheckOCR:
//      南京流程银行项目，支票号码OCR识别.
//
// 	 北京天创征腾信息科技有限公司   2010年6月
//
// 	 设计者：黄雅,邱锐
// 	 编码者：HuangYa, QiuRui
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __TC_NANJING_CHECKOCR_H__
#define __TC_NANJING_CHECKOCR_H__

#if (defined WIN32 || defined _WIN32 || defined WIN64 || defined _WIN64)
	#ifdef TC_NANJINGBOCOM_CHECKOCR_EXPORTS
	#define TC_NANJINGBOCOM_CHECKOCR_API __declspec(dllexport)
	#else
	#define TC_NANJINGBOCOM_CHECKOCR_API __declspec(dllimport)
	#endif
#else
	#define TC_NANJINGBOCOM_CHECKOCR_API
#endif

#ifdef __cplusplus
extern "C"
{
#endif
/*Function:南京交行支票号码识别。
Parameters:
    srcImage:   待处理的图像。可以为灰度图像和彩色图像。
                但因号码区域经常出现红色印章重叠的情况，所以如果采用灰度图像将会降低准确识别率。
    sizeX:      srcImage的sizeX.在配置文件中对其的最小值有所限制。
    sizeY:      srcImage的sizeY.在配置文件中对其的最小值有所限制。
    nChannels:  srcImage的sizeY.只能填1或3。
    dpi:        srcImage的扫描dpi。处理范围：[150, 300]
    type:       配置文件编号。南京交行支票填1
    rlt_no:     (返回)识别出来的号码。
Return:
    0:          成功识别。
    -1:         配置文件无法读取。
    -2:         送入图像的dpi不在处理范围。
    -3:         送入图像的大小不合适。
    -4:         无法正确切割号码子图。原因：1、配置文件出错；2、送入图像不正常。
    -11:		(OCR)读取配置文件有误
	-12:		(OCR)文件的dpi有误.dpi允许的范围:30~300
	-13:		(OCR)送入的图像大小比模板还小,有误!
	-14:		(OCR)读取垂直模板有误.
	-15:		(OCR)计算每个字符的分割x方向坐标时有误.
	-16:		(OCR)读取垂直模板有误.
	-17:		(OCR)计算每个字符的分割y方向坐标时有误.
	-18:		(OCR)初始化识别数字模板有误.有可能是模板无法读取.
	-19:		(OCR)有效数字设置有误.
*/
TC_NANJINGBOCOM_CHECKOCR_API int TC_NanjingBocom_CheckOCR(unsigned char *srcImage, int sizeX, int sizeY,
                                                        int nChannels, int dpi, int type, char *rlt_no);


#ifdef __cplusplus
}
#endif

#endif
