/////////////////////////////////////////////////////////////////////////////////
//
// 	 til_calbarcode128 :	根据输入图像求128码值
//							设计需求由上海交行POC测试提交
//
// 	 北京天创征腾信息科技有限公司   2010年3月
//
// 	 设计者：黄雅
// 	 编码者：HuangYa
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef __TC__BARCODE128__H__
#define __TC__BARCODE128__H__

#ifdef TC_BARCODE128_EXPORTS
#define TC_BARCODE128_API __declspec(dllexport)
#else
#define TC_BARCODE128_API __declspec(dllimport)
#endif

/*Function:根据输入图像求128码值。
Parameters:
	pImage:		待识别的图像.
	sizeX:		待识别图像的sizeX.
	sizeY:		待识别图像的sizeY.
	nChannels:	通道数.
	dpi:		扫描的dpi.
	rlt:		（返回）识别结果。128码值。
Return:
	0:	识别成功
	-1：图像尺寸过小.
	-2:	输入图像中无符合128码条件的图像。
*/
TC_BARCODE128_API int til_calbarcode128(unsigned char* pImage, int sizeX, int sizeY, int nChannels, int dpi, char* rlt_str);

/*Function:根据输入图像求128码值。
Parameters:
	pImportFile:	文件名
	dpi:		扫描的dpi.
	rlt:		（返回）识别结果。128码值。
Return:
	0:	识别成功
	-1：图像尺寸过小.
	-2:	输入图像中无符合128码条件的图像。
*/
TC_BARCODE128_API int til_calbarcode128_1(char *pImportFile, int dpi, char* rlt_str);

#endif