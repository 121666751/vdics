/////////////////////////////////////////////////////////////////////////////////
//
// 	 E13b OCR 识别函数(dll 导出函数)
//
// 	 北京天创征腾信息科技有限公司    2008年7月
//
// 	 设计者：邱锐
// 	 编码者：Qiurui
//
//////////////////////////////////////////////////////////////////////////////////
#ifndef __TC_E13B_OCR_API_H_
#define __TC_E13B_OCR_API_H_

#if (defined WIN32 || defined _WIN32 || defined WIN64 || defined _WIN64)
    #ifdef TC_E13BOCR_EXPORTS
    #define TC_E13BOCR_API __declspec(dllexport)
    #else
    #define TC_E13BOCR_API __declspec(dllimport)
    #endif
#else
	#define TC_E13BOCR_API
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*Function:识别图像的磁码信息。
    Parameters:
    img:    磁码图像。注意：一定要切合适尺寸的图片。以北京交行清分机为例，切割子图的各项参数为：
            OffX = 5;
            OffY = src_sizeY - 95;
            SizeX = src_sizeX - 10;
            SizeY = 90;
            nChannels = src_nChannels;
    sizeX:  img的sizex.
    sizeY:  img的sizeY.
    nChannels:  img的通道数.
    dpi:    扫描dpi.
    out:    （返回）识别出来的磁码结果.应分配保证的字符串（建议大于等于256个），送入前先初始化为全零。
Return:
    0:  正常识别。
*/
TC_E13BOCR_API int E13b_OCR(unsigned char *srcImage, int sizeX, int sizeY, int nchannels, int dpi,
                            char* out);

#ifdef __cplusplus
}
#endif

#endif
