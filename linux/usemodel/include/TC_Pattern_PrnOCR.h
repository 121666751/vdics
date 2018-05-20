
#ifndef __TC_PATTER_PRNOCR_H__
#define __TC_PATTER_PRNOCR_H__

#ifdef TC_PATTERN_PRNOCR_EXPORTS
#define TC_PATTERN_PRNOCR_API __declspec(dllexport)
#else
#define TC_PATTERN_PRNOCR_API __declspec(dllimport)
#endif

/*
 ���ã����������ļ�ʶ����桢��ȡ�ʺ�
 Input:
	pImportFile:ͼ���ļ�����������jpg��ʽ��
	dpi:		ͼ��ֱ��ʡ�Ҫ����100~300֮�䡣
	type:		�������͡�1:ʶ�����; 2:ʶ�����,����� 8λƾ֤��;����:������.
	vt:			�洢���������������ص�Ʊ������: 0:--; 1:�жһ�Ʊ; 2:�ŷ�; 3:����ƾ֤; 4:��Ʊ; 5:֧Ʊ
	vchno:		ʶ������ʺŵĽ��.
 Output:
	0:	sucessful
	1:	reading image error! maybe pImportFile is not existed!
	2:	the pattern of image is not one of we kowned.
	3:	the size of image is too small.
	4:	recognize number failure!
*/

//TC_PATTERN_PRNOCR_API int  image_process(MIL_ID* pMilSystem, char* pImportFile, int dpi, int type, int &vt, char* vchno);
TC_PATTERN_PRNOCR_API int  image_process(void*pSystem, char* pImportFile, int dpi, int type, int &vt, char* vchno);

#endif
