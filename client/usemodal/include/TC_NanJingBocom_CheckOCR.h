/////////////////////////////////////////////////////////////////////////////////
//
// 	 TC_NanJingBocom_CheckOCR:
//      �Ͼ�����������Ŀ��֧Ʊ����OCRʶ��.
//
// 	 �����촴������Ϣ�Ƽ����޹�˾   2010��6��
//
// 	 ����ߣ�����,����
// 	 �����ߣ�HuangYa, QiuRui
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
/*Function:�Ͼ�����֧Ʊ����ʶ��
Parameters:
    srcImage:   �������ͼ�񡣿���Ϊ�Ҷ�ͼ��Ͳ�ɫͼ��
                ����������򾭳����ֺ�ɫӡ���ص������������������ûҶ�ͼ�񽫻ή��׼ȷʶ���ʡ�
    sizeX:      srcImage��sizeX.�������ļ��ж������Сֵ�������ơ�
    sizeY:      srcImage��sizeY.�������ļ��ж������Сֵ�������ơ�
    nChannels:  srcImage��sizeY.ֻ����1��3��
    dpi:        srcImage��ɨ��dpi������Χ��[150, 300]
    type:       �����ļ���š��Ͼ�����֧Ʊ��1
    rlt_no:     (����)ʶ������ĺ��롣
Return:
    0:          �ɹ�ʶ��
    -1:         �����ļ��޷���ȡ��
    -2:         ����ͼ���dpi���ڴ���Χ��
    -3:         ����ͼ��Ĵ�С�����ʡ�
    -4:         �޷���ȷ�и������ͼ��ԭ��1�������ļ�����2������ͼ��������
    -11:		(OCR)��ȡ�����ļ�����
	-12:		(OCR)�ļ���dpi����.dpi����ķ�Χ:30~300
	-13:		(OCR)�����ͼ���С��ģ�廹С,����!
	-14:		(OCR)��ȡ��ֱģ������.
	-15:		(OCR)����ÿ���ַ��ķָ�x��������ʱ����.
	-16:		(OCR)��ȡ��ֱģ������.
	-17:		(OCR)����ÿ���ַ��ķָ�y��������ʱ����.
	-18:		(OCR)��ʼ��ʶ������ģ������.�п�����ģ���޷���ȡ.
	-19:		(OCR)��Ч������������.
*/
TC_NANJINGBOCOM_CHECKOCR_API int TC_NanjingBocom_CheckOCR(unsigned char *srcImage, int sizeX, int sizeY,
                                                        int nChannels, int dpi, int type, char *rlt_no);


#ifdef __cplusplus
}
#endif

#endif
