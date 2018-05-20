/////////////////////////////////////////////////////////////////////////////////
//
// 	 ImageBasicProcess :	�����unsigned char*�洢��ͼ����в���.
//							�ܶ�����ǻ���opencv��.
//
// 	 �����촴������Ϣ�Ƽ����޹�˾   2008��11��
//
// 	 ����ߣ����񣬻���
// 	 �����ߣ�QuiRui,HuangYa
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

//���������ź����Ĳ���
#define	TIL_RESIZE_METHOD1	1		//�ٶȿ죬��Ч���
#define	TIL_RESIZE_METHOD2	2		//�Ŵ�ʱʹ�ô˷���Ч������
#define	TIL_RESIZE_METHOD3	3		//�������Сʱʹ�ô˷���,�Ŵ��Ч��ͬ����һ��
#define	TIL_RESIZE_METHOD4	4		//�Ŵ�Ч����ѣ����ٶ�����
#define	TIL_RESIZE_METHOD5	5		//��Ԥ��ӡ�����غ�ͼ������С�������㷨

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
Function:	��ͼ����д�������ϵ���С�������⴦��ĺ������Ը�����ʾΪĿ�ġ�HD��high definition
			����ʱʹ�������š�ͼ����ǿ��ƽ�����ȱ�׼���Ĵ������ű�����ԭͼ��Ŀ��ͼ�Ĵ�Сȷ������Լ��������.
			Note��1.ԭͼ��srcImage����Ŀ��ͼ��dstImage����ͨ����������ͬ��
				  2.Ŀ��ͼ��dstImage�����ں��������õġ�
Parameters:
	srcImage:	�����ŵ�ԭͼ
	src_SizeX:	srcImage��SizeX��
	src_SizeY:	srcImage��SizeY��
	dstImage:	���Ž����
	dst_SizeX:	dstImage��SizeX��
	dst_SizeY:	dstImage��SizeY��
	nChannels:	ͨ������ע�⣬srcImage��dstImage��ͨ����������ͬ��
return:
	0:	sucessful.
*/
TC_IMAGELIB_API	int tilImageHDShow(unsigned char *srcImage, int src_SizeX, int src_SizeY,
								   unsigned char *dstImage, int dst_SizeX, int dst_SizeY,
								   int nChannels);
/*
Function:	��ͼ����д����(index < 0.25)���ϵ���С�������⴦��ĺ������Ը�����ʾΪĿ�ġ�HD��high definition
			Note��1.ԭͼ��srcImage����Ŀ��ͼ��dstImage����ͨ������ͬ��
				  2.Ŀ��ͼ��dstImage�����ں����ڷ���õģ�ʹ�������tilImageRelease�����ͷš�
Parameters:
	srcImage:	�����ŵ�ԭͼ
	src_SizeX:	srcImage��SizeX��
	src_SizeY:	srcImage��SizeY��
	dstImage:	���Ž����
	dst_SizeX:	dstImage��SizeX������ֵ����
	dst_SizeY:	dstImage��SizeY������ֵ����
	nChannels:	ͨ������
	index:		���ű�����
return:
	0:	sucessful.
*/
TC_IMAGELIB_API int tilImageHDShowIndex(unsigned char *srcImage, int src_SizeX, int src_SizeY,
						unsigned char *&dstImage, int &dst_SizeX, int &dst_SizeY,
						int nChannels, double index);

//Function:	�ͷ�ͼ��
TC_IMAGELIB_API void tilImageRelease(unsigned char *image);

//�洢ͼ��
TC_IMAGELIB_API void tilSaveImage(unsigned char *pSrcImg, int sizeX, int sizeY, int nChannels, char *FileName);

/*
Function:	��ͼ���������
Parameters:
	srcImage:	�����ŵ�ԭͼ
	src_SizeX:	srcImage��SizeX��
	src_SizeY:	srcImage��SizeY��
	dstImage:	���Ž����
	dst_SizeX:	dstImage��SizeX��
	dst_SizeY:	dstImage��SizeY��
	nChannels:	ͨ������ע�⣬srcImage��dstImage��ͨ����������ͬ��
	method:		���ŵķ������μ����ļ�
				TIL_RESIZE_METHOD1:	�ٶȿ죬��Ч���������ڲ�ֵ����
				TIL_RESIZE_METHOD2:	�Ŵ�ʱʹ�ô˷���Ч��������˫���Բ�ֵ����ȱʡʹ�ã�
				TIL_RESIZE_METHOD3:	�������Сʱʹ�ô˷���,�Ŵ��Ч��ͬ����һ�������ع�ϵ�ز�����
				TIL_RESIZE_METHOD4:	�Ŵ�Ч����ѣ����ٶ�������3��������ֵ����
return:
	0:	sucessful.
*/
TC_IMAGELIB_API int tilImageResize(unsigned char *srcImage, int src_SizeX, int src_SizeY,
								   unsigned char *dstImage, int dst_SizeX, int dst_SizeY,
								   int nChannels, int method);

/*Function:	��CISɨ���ͼ�����У��
Parameters:
	srcImage:	ɨ���ͼ�񡣣�ע�⣺������3ͨ��ͼ���洢��ʽ��rrrgggbbb��
	dstImage:	У���Ľ������ע�⣺��С������srcImageһ�£�
	sizeX:	srcImage��SizeX��
	sizeY:	srcImage��SizeY��
return:
	0:	sucessful.
*/
TC_IMAGELIB_API	int	tilCISColorCalibrate(unsigned char *srcImage, unsigned char *dstImage,
										 int sizeX, int sizeY);

/*Function: ��ͼ���ļ���
            ע�⣺imageʹ�ù���һ��Ҫ��tilImageRelease�����ͷţ�����
Parameters:
    fName:  ͼ���ļ�������ע�⣺Ӧ����·���ͺ�׺��
    image:  ͼ�����ݣ��ڱ������з���ռ䣩��
    sizeX:  image��sizeX��
    sizeY:  image��sizeY��
    nChannels:  image��nChannels��
Return :
    0   �ɹ���
    ����  ʧ�ܡ�
*/
TC_IMAGELIB_API	int	tilLoadImageFile(char *fName, unsigned char *&image, int& sizeX, int& sizeY,
                                int& nChannels);

#ifdef __cplusplus
}
#endif

#endif
