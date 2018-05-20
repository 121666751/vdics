/////////////////////////////////////////////////////////////////////////////////
//
// 	 TC_Pattern_PrnOCR :	��������ͼ���б��ʽ��ʶ�����
//							����������Ϻ������ύ
//
// 	 �����촴������Ϣ�Ƽ����޹�˾   2010��3��
//
// 	 ����ߣ����� ����
// 	 �����ߣ�Qiurui HuangYa
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef __TC_BOCOM_PATTERN_BOCOM_H__
#define __TC_BOCOM_PATTERN_BOCOM_H__

#ifdef TC_BOCOM_PATTERN_BOCOM_EXPORTS
#define TC_BOCOM_PATTERN_BOCOM_API __declspec(dllexport)
#else
#define TC_BOCOM_PATTERN_BOCOM_API __declspec(dllimport)
#endif

/*
 ���ã����������ļ�ʶ����桢��ȡ�ʺ�
 Input:
	pImportFile:ͼ���ļ�����������jpg��ʽ��
	dpi:		ͼ��ֱ��ʡ�Ҫ����100~300֮�䡣
	type:		�������͡�1:ʶ�����; 2:ʶ�����,����ô����;����:������
	vt:			�洢���������������ص�Ʊ������: 0:δ֪����; 1:�жһ�Ʊ; 2:�ŷ�; 3:����ƾ֤; 4:��Ʊ; 5:֧Ʊ;6:����;7:����
	vchno:		ʶ����Ĵ�����(��type=2,��Ϊ�ŷ�����/����ʱ�Ž��в���).�������Ҫ���д������,�򷵻ؿ�.
	cfg_no:		�����ļ��ı��.�����̶�Ӧ�������ļ���Ϊ:pattern_bocomm_scan**.cfg
 Output:
	0:	sucessful
	-1:	�����cfg_noֵ��һ��
	-2: ��ȡģ�����ݳ���
	-3: ��ͼ����
	1:	ʶ��������
	2:	����Ĵ����������
	-6:	ʶ��������!
*/
TC_BOCOM_PATTERN_BOCOM_API int  image_process_scan(void*pSystem, char* pImportFile, int dpi, int type, int &vt, 
												 char* vchno, int cfg_no);

/*
���ã����������ļ�ʶ����桢��ȡ�ʺ�
 Input:
	pSystem:		ϵͳָ��.
	pImg_Ori:		��ʶ��ͼ��.rbgͨ��ͼ.
	sizeX_Ori:		pImg_Ori sizeX.
	sizeY_Ori:		pImg_Ori sizeY.
	nChannel_Ori:	pImg_Ori's channels.Ŀǰ�涨Ϊ3.
	dpi:			ͼ��ֱ��ʡ�ĿǰҪ��Ϊ200��
	type:			�������͡�1:ʶ�����; 2:ʶ�����,����ô����;����:������
	vt:				�洢���������������ص�Ʊ������: 
					0	δ֪����; 
					1	�жһ�Ʊ; 
					2	�ŷ�; 
					3	����ƾ֤; 
					4	��Ʊ; 
					5	֧Ʊ;
					6	����;
					7	����;
					8	��ƱII;
					9	������Ʊ����	
					10	������Ʊ����	
					11	������Ʊ����	
					12	������Ʊ����	
					13	������Ʊũ��	
					14	������Ʊ�ַ���	
					15	������Ʊ�Ϻ�ũ����	
					16	������Ʊ�Ϻ�����						
					17	������Ʊ��������
					18	������Ʊ��������
					19	������Ʊ�й�����
					20	������Ʊ�������
					21	������Ʊ��ҵ����
	vchno:			���á�
	cfg_no:			�����ļ��ı��.�����̶�Ӧ�������ļ���Ϊ:pattern_bocomm_scan**.cfg
 Output:
	0:	sucessful
	-1:	�����cfg_noֵ��һ��
	-2: ��ȡģ�����ݳ���
	-3: ͨ��������
	1:	ʶ��������
	2:	����Ĵ����������
	-6:	ʶ��������!
*/
TC_BOCOM_PATTERN_BOCOM_API int  image_process_scan_1(void*pSystem, unsigned char *pImg_Ori, int sizeX_Ori, int sizeY_Ori, 
												 int nChannel_Ori, int dpi, int type, int &vt, char* vchno, int cfg_no);

#endif
