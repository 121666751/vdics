/////////////////////////////////////////////////////////////////////////////////
//
// 	 til_calbarcode128 :	��������ͼ����128��ֵ
//							����������Ϻ�����POC�����ύ
//
// 	 �����촴������Ϣ�Ƽ����޹�˾   2010��3��
//
// 	 ����ߣ�����
// 	 �����ߣ�HuangYa
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef __TC__BARCODE128__H__
#define __TC__BARCODE128__H__

#ifdef TC_BARCODE128_EXPORTS
#define TC_BARCODE128_API __declspec(dllexport)
#else
#define TC_BARCODE128_API __declspec(dllimport)
#endif

/*Function:��������ͼ����128��ֵ��
Parameters:
	pImage:		��ʶ���ͼ��.
	sizeX:		��ʶ��ͼ���sizeX.
	sizeY:		��ʶ��ͼ���sizeY.
	nChannels:	ͨ����.
	dpi:		ɨ���dpi.
	rlt:		�����أ�ʶ������128��ֵ��
Return:
	0:	ʶ��ɹ�
	-1��ͼ��ߴ��С.
	-2:	����ͼ�����޷���128��������ͼ��
*/
TC_BARCODE128_API int til_calbarcode128(unsigned char* pImage, int sizeX, int sizeY, int nChannels, int dpi, char* rlt_str);

/*Function:��������ͼ����128��ֵ��
Parameters:
	pImportFile:	�ļ���
	dpi:		ɨ���dpi.
	rlt:		�����أ�ʶ������128��ֵ��
Return:
	0:	ʶ��ɹ�
	-1��ͼ��ߴ��С.
	-2:	����ͼ�����޷���128��������ͼ��
*/
TC_BARCODE128_API int til_calbarcode128_1(char *pImportFile, int dpi, char* rlt_str);

#endif