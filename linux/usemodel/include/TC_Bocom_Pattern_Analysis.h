/////////////////////////////////////////////////////////////////////////////////
//
// 	 TC_Pattern_PrnOCR :	根据输入图像判别版式和识别磁码
//							设计需求由上海交行提交
//
// 	 北京天创征腾信息科技有限公司   2010年3月
//
// 	 设计者：邱锐 黄雅
// 	 编码者：Qiurui HuangYa
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef __TC_BOCOM_PATTERN_BOCOM_H__
#define __TC_BOCOM_PATTERN_BOCOM_H__

#if (defined WIN32 || defined _WIN32 || defined WIN64 || defined _WIN64)
	#ifdef TC_BOCOM_PATTERN_BOCOM_EXPORTS
	#define TC_BOCOM_PATTERN_BOCOM_API __declspec(dllexport)
	#else
	#define TC_BOCOM_PATTERN_BOCOM_API __declspec(dllimport)
	#endif
#else
	#define TC_BOCOM_PATTERN_BOCOM_API
#endif

//错误码定义
#define TC_PA_LOADIMAGE_ERROR 	-1
#define TC_PA_DPI_ERROR		-2
#define TC_PA_LOAD_CFG1_ERROR	-3
#define TC_PA_LOAD_CFG2_ERROR	-4
#define TC_PA_LOAD_CFG3_EEOR	-5
#define TC_PA_LOAD_SNCFG_ERROR	-6
#define TC_PA_SNCFG_ERROR		-7
#define TC_PA_RECOG_E13B_ERROR  -8
#define TC_PA_RECOG_NUM_ERROR   -9

#ifdef __cplusplus
extern "C"
{
#endif

struct TilOperationAndReturn
{
	int	recogPattern_type;	//版面识别参数。0：不识别；1：识别；2：细致识别；3：仅识别隔板.
	int	E13b_type;		//是否识别磁码。0：不识别；1：所有都识别；2：根据配置文件中的设置决定是否识别。
	int	OCR_type;			//是否识别凭证号码。0：不识别；1：所有都识别；2：根据配置文件中的设置决定是否识别。
	char	recog_cfg[512];	//识别配置文件的路径和名称。
	char	checkOCR_cfg[512];	//识别凭证号的配置文件的路径和名称。
	int	rst_pattern;		//（返回）票据类型编号。
	char	rst_E13b[256];		//（返回）磁码识别结果.如果recogPattern_type = 3,代表条码识别结果。
	char	rst_num[256];		//（返回）凭证号码识别结果.
};

/*
 作用：根据输入文件识别版面、读取帐号
 Input:
	pImportFile:	图像文件名。可以是jpg格式。
	dpi:			图像分辨率。要求在100~300之间。
	dpi:			图像分辨率。目前要求为200。
	trans:		传递参数和返回结果的结构体。
 Output:
	0:	sucessful
	others:	failure.定义参见“错误码定义”
*/
TC_BOCOM_PATTERN_BOCOM_API int TilPatternRecogClassify_F(char* pImportFile, int dpi, TilOperationAndReturn &trans);

/*
作用：根据输入文件识别版面、读取帐号
 Input:
	pImg_Ori:		待识别图像.rbg通道图.
	sizeX_Ori:		pImg_Ori sizeX.
	sizeY_Ori:		pImg_Ori sizeY.
	nChannel_Ori:	pImg_Ori's channels.目前规定为3.
	dpi:			图像分辨率。目前要求为200。
	trans:		传递参数和返回结果的结构体。
 Output:
	0:	sucessful
	others:	failure.定义参见“错误码定义”
*/
TC_BOCOM_PATTERN_BOCOM_API int  TilPatternRecogClassify(unsigned char *pImg_Ori, int sizeX_Ori, int sizeY_Ori, int nChannel_Ori,
						int dpi, TilOperationAndReturn &trans);

//创建颜色模板。
TC_BOCOM_PATTERN_BOCOM_API int TilCreatePatternRecogColorMod(char *imageName0, char *imageName1, int sn, 
											char *color_seg_cfgName, char *color_mod_cfgName);

//创建水平投影模板。
TC_BOCOM_PATTERN_BOCOM_API int TilCreateProjModUsingBImg(char *fName, char direct, int projThres, char *modName);

TC_BOCOM_PATTERN_BOCOM_API void TilGetPatternRecogVersion(char *version);

TC_BOCOM_PATTERN_BOCOM_API void TilCreatePatternRecogReadme();

#ifdef __cplusplus
}
#endif

#endif
