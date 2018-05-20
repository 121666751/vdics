/////////////////////////////////////////////////////////////////////////////////
//
// 	 E13b OCR ʶ����(dll ��������)
//
// 	 �����촴������Ϣ�Ƽ����޹�˾    2008��7��
//
// 	 ����ߣ�����
// 	 �����ߣ�Qiurui
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

/*Function:ʶ��ͼ��Ĵ�����Ϣ��
    Parameters:
    img:    ����ͼ��ע�⣺һ��Ҫ�к��ʳߴ��ͼƬ���Ա���������ֻ�Ϊ�����и���ͼ�ĸ������Ϊ��
            OffX = 5;
            OffY = src_sizeY - 95;
            SizeX = src_sizeX - 10;
            SizeY = 90;
            nChannels = src_nChannels;
    sizeX:  img��sizex.
    sizeY:  img��sizeY.
    nChannels:  img��ͨ����.
    dpi:    ɨ��dpi.
    out:    �����أ�ʶ������Ĵ�����.Ӧ���䱣֤���ַ�����������ڵ���256����������ǰ�ȳ�ʼ��Ϊȫ�㡣
Return:
    0:  ����ʶ��
*/
TC_E13BOCR_API int E13b_OCR(unsigned char *srcImage, int sizeX, int sizeY, int nchannels, int dpi,
                            char* out);

#ifdef __cplusplus
}
#endif

#endif
