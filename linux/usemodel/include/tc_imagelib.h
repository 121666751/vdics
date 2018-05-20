/////////////////////////////////////////////////////////////////////////////////
//
// 	 ImageBasicProcess :	针对以unsigned char*存储的图像进行操作.
//							很多操作是基于opencv的.
//
// 	 北京天创征腾信息科技有限公司   2008年11月
//
// 	 设计者：邱锐，黄雅
// 	 编码者：QuiRui,HuangYa
//
/////////////////////////////////////////////////////////////////////////////////

#ifndef __TC_IMAGELIB_H__
#define __TC_IMAGELIB_H__

#if (defined WIN32 || defined _WIN32 || defined WIN64 || defined _WIN64)
    #ifdef TC_IMAGELIB_EXPORTS
    #define TC_IMAGELIB_API __declspec(dllexport)
    #else
    #define TC_IMAGELIB_API __declspec(dllimport)
    #endif
#else
    #define TC_IMAGELIB_API
#endif

//以下是缩放函数的参数
#define	TIL_RESIZE_METHOD1	1		//速度快，但效果差。
#define	TIL_RESIZE_METHOD2	2		//放大时使用此方法效果不错。
#define	TIL_RESIZE_METHOD3	3		//大比例缩小时使用此方法,放大的效果同方法一。
#define	TIL_RESIZE_METHOD4	4		//放大效果最佳，但速度慢。
#define	TIL_RESIZE_METHOD5	5		//对预留印鉴和重合图进行缩小的特殊算法

#define	TORECOG_VALUE	45
#define	PRIRECORD_VALUE	55
#define	OVER_VALUE	100
#define	FLAGREC_VALUE	90
#define	PRIRECORD_VALUE2 255

#ifdef __cplusplus
extern "C"
{
#endif

/*
Function:	对图像进行大比例以上的缩小进行特殊处理的函数，以高清显示为目的。HD：high definition
			处理时使用了缩放、图像增强和平均亮度标准化的处理。缩放比例由原图和目标图的大小确定（不约束比例）.
			Note：1.原图（srcImage）和目标图（dstImage）的通道数必须相同。
				  2.目标图（dstImage）是在函数外分配好的。
Parameters:
	srcImage:	待缩放的原图
	src_SizeX:	srcImage的SizeX。
	src_SizeY:	srcImage的SizeY。
	dstImage:	缩放结果。
	dst_SizeX:	dstImage的SizeX。
	dst_SizeY:	dstImage的SizeY。
	nChannels:	通道数。注意，srcImage和dstImage的通道数必须相同。
return:
	0:	sucessful.
*/
TC_IMAGELIB_API	int tilImageHDShow(unsigned char *srcImage, int src_SizeX, int src_SizeY,
								   unsigned char *dstImage, int dst_SizeX, int dst_SizeY,
								   int nChannels);
/*
Function:	对图像进行大比例(index < 0.25)以上的缩小进行特殊处理的函数，以高清显示为目的。HD：high definition
			Note：1.原图（srcImage）和目标图（dstImage）的通道数相同。
				  2.目标图（dstImage）是在函数内分配好的，使用完后用tilImageRelease进行释放。
Parameters:
	srcImage:	待缩放的原图
	src_SizeX:	srcImage的SizeX。
	src_SizeY:	srcImage的SizeY。
	dstImage:	缩放结果。
	dst_SizeX:	dstImage的SizeX（返回值）。
	dst_SizeY:	dstImage的SizeY（返回值）。
	nChannels:	通道数。
	index:		缩放比例。
return:
	0:	sucessful.
*/
TC_IMAGELIB_API int tilImageHDShowIndex(unsigned char *srcImage, int src_SizeX, int src_SizeY,
						unsigned char *&dstImage, int &dst_SizeX, int &dst_SizeY,
						int nChannels, double index);

//Function:	释放图像。
TC_IMAGELIB_API void tilImageRelease(unsigned char *image);

//存储图像
TC_IMAGELIB_API void tilSaveImage(unsigned char *pSrcImg, int sizeX, int sizeY, int nChannels, char *FileName);

/*
Function:	对图像进行缩放
Parameters:
	srcImage:	待缩放的原图
	src_SizeX:	srcImage的SizeX。
	src_SizeY:	srcImage的SizeY。
	dstImage:	缩放结果。
	dst_SizeX:	dstImage的SizeX。
	dst_SizeY:	dstImage的SizeY。
	nChannels:	通道数。注意，srcImage和dstImage的通道数必须相同。
	method:		缩放的方法。参见本文件
				TIL_RESIZE_METHOD1:	速度快，但效果差。（最近邻插值法）
				TIL_RESIZE_METHOD2:	放大时使用此方法效果不错。（双线性插值法，缺省使用）
				TIL_RESIZE_METHOD3:	大比例缩小时使用此方法,放大的效果同方法一。（象素关系重采样）
				TIL_RESIZE_METHOD4:	放大效果最佳，但速度慢。（3次样条插值法）
return:
	0:	sucessful.
*/
TC_IMAGELIB_API int tilImageResize(unsigned char *srcImage, int src_SizeX, int src_SizeY,
								   unsigned char *dstImage, int dst_SizeX, int dst_SizeY,
								   int nChannels, int method);

/*Function:	对CIS扫描的图像进行校正
Parameters:
	srcImage:	扫描的图像。（注意：必须是3通道图。存储格式：rrrgggbbb）
	dstImage:	校正的结果。（注意：大小必须与srcImage一致）
	sizeX:	srcImage的SizeX。
	sizeY:	srcImage的SizeY。
return:
	0:	sucessful.
*/
TC_IMAGELIB_API	int	tilCISColorCalibrate(unsigned char *srcImage, unsigned char *dstImage,
										 int sizeX, int sizeY);

/*Function: 读图像文件。
            注意：image使用过后一定要用tilImageRelease函数释放！！！
Parameters:
    fName:  图像文件名。（注意：应包含路径和后缀）
    image:  图像数据（在本函数中分配空间）。
    sizeX:  image的sizeX。
    sizeY:  image的sizeY。
    nChannels:  image的nChannels。
Return :
    0   成功。
    其它  失败。
*/
TC_IMAGELIB_API	int	tilLoadImageFile(char *fName, unsigned char *&image, int& sizeX, int& sizeY,
                                int& nChannels);

#ifdef __cplusplus
}
#endif

#endif
