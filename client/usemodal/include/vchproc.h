#ifndef __VCHPROC_H__
#define __VCHPROC_H__

#include <wtypes.h>

struct VCHOCRRESULT
{
	char accno[40];			/**< �ʺ� */
	char vchno[16];			/**< ƾ֤�� */
	char vchtype[8];		/**< ƾ֤���� */
	char money[20];			/**< ��� */
	char exchno[20];		/**< ������ */
	char vchtypename[40];	/**< ƾ֤���� */
	char orginE13b[60];		/**< ԭʼe13b�� */
	char money_e13b[20];	/**< ԭʼ������ */
	char money_digit[20];	/**< ԭʼСд��� */
	char money_cn[20]; 		/**< ԭʼ��д��� */
	char vchdate[12];		/**< ƾ֤���� */
	char vchno_e13b[12];	/**< ����ƾ֤���� */
	char vchno_digit[12];	/**< ƾ֤���� */
	POINT base;				/**< ͼ���׼�� */
	RECT stamp_region;		/**< ӡ������ */
	RECT sign_region;		/**< ǩ���������� */
	char reserved[364];		/**< �����ռ� */
	int  successFlag;		/**< success flag */
};

/************************************************************************/
/* successFlag value.                                                   */
/************************************************************************/
#define		OCR_ACCNO		0x00000001	/**< accno ok */
#define		OCR_VCHNO		0x00000002	/**< vchno ok */
#define		OCR_MONEY		0x00000004	/**< money ok */
#define		OCR_FTYPE		0x00000008	/**< form type ok */
#define		OCR_E13B		0x00000010	/**< e13b code ok */
#define		OCR_EXCHNO		0x00000020	/**< exchno code ok */
#define 	OCR_MONEY_E13B	0x00000040	/**< money_e13b ok */
#define 	OCR_MONEY_DIGIT	0x00000080	/**< money_digit ok */
#define		OCR_MONEY_CN	0x00000100	/**< money_cn ok */
#define		OCR_VCHDATE		0x00000200  /**< vchdate ok */
#define		OCR_VCHNO_E13B	0x00000400	/**< vchno_e13b ok */
#define		OCR_VCHNO_DIGIT	0x00000800	/**< vchno_digit ok */

/************************************************************************/
/*                                                                      */
/************************************************************************/
class MemT;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get Voucher info from voucher's image.
 * 
 * @param img: 3-channel image data pointer
 * @param sizeX: size X of the image.
 * @param sizeY: size Y of the image.
 * @param dpi: dpi of the image.
 * @param fname: ԭʼ�ļ���������ΪNULL. ��������ֻ�ɨ��ʱ������ͨ��
				�˲���ֱ�Ӵ����ݿ��в�ѯE13B������
 * @param outInfo: caller alloced address to receive voucher info after process.
 * @return: 0-sucess.
 */
int vch_process(unsigned char* img,
				int sizeX,
				int sizeY,
				int dpi,
				const char* fname,
				VCHOCRRESULT *outInfo);

/**
 * Get Voucher info from voucher's image.
 * 
 * @param imgFilename: 3-channel image filename, bmp format
 * @param outInfo: caller alloced address to receive voucher info after process.
 * @return: 0-sucess.
 */
int vch_process_file(const char* imgFilename, VCHOCRRESULT *outInfo);



/**
 * Set MIL_ID of mil system handle	
 * @param milSystem: MIL_ID type of mil system handle.
 */
void vch_set_milsystem(long milSystem);

/**
 * ��ȡӡ�����ʺ�	
 * 
 * @param img: 3-channel image data pointer
 * @param sizeX: size X of the image.
 * @param sizeY: size Y of the image.
 * @param dpi: dpi of the image.
 * @param milsys: mil system.
 * @param outAccno: ��ȡ�õ����ʺŽ����
 * @return: 0-sucess.
 */
int ocr_stamp_card(unsigned char* img, 
				   int sizeX, int sizeY, int dpi, 
				   long milSys,
				   char* outAccno);

int ocr_stamp_card_file(const char* fname,int dpi,long milSys,
				   char* outAccno);


/*
 * �ļ�ת��Ϊͨ��ͼ	
 */
int bmp_rgb( const char* fname, MemT& outData, int& outSizeX, int& outSizeY, 
			int& band, int& outDpi);

/*
 * ͼ���С�ı�	
 */
int image_resize(unsigned char* srcImage,int srcX, int srcY, int Band, 
				 int destX, int destY, unsigned char* destImage);

/**
 * ������������ת�������ڰ��洦��ʹ��200dpi��ӡ��ʶ��ʹ��300dpi��
 * ��Ҫ��vch_process���ص�������б任����������ʶ����
 * 
 * @base	:	vch_process���ص������׼
 * @rcStamp	:	���㵽300dpi�ľ���ӡ������
 * @rcSign	:	���㵽300dpi�ľ���ǩ������
 * @useConfig	:	�Ƿ�ʹ�������ļ�����δ�����Զ�ʶ��ʱ������ʹ�������ļ�
 *					OcrInfoA.xml��ȡ�������ꡣ
 *					���loadConfigΪtrue����base��rcStamp, rcSign��Ϊ����
 *					��������������ֵ����֮ʹ�ø�ֵ���㡣
 * @return 0-success
 */
int convert_region(POINT& base,RECT& rcStamp, RECT& rcSign, bool useConfig );

//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif //__VCHPROC_H__