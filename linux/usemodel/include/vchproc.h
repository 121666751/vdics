#ifndef __VCHPROC_H__
#define __VCHPROC_H__

#include <wtypes.h>

struct VCHOCRRESULT
{
	char accno[40];			/**< 帐号 */
	char vchno[16];			/**< 凭证号 */
	char vchtype[8];		/**< 凭证类型 */
	char money[20];			/**< 金额 */
	char exchno[20];		/**< 交换号 */
	char vchtypename[40];	/**< 凭证名称 */
	char orginE13b[60];		/**< 原始e13b码 */
	char money_e13b[20];	/**< 原始磁码金额 */
	char money_digit[20];	/**< 原始小写金额 */
	char money_cn[20]; 		/**< 原始大写金额 */
	char vchdate[12];		/**< 凭证日期 */
	char vchno_e13b[12];	/**< 磁码凭证号码 */
	char vchno_digit[12];	/**< 凭证号码 */
	POINT base;				/**< 图像基准点 */
	RECT stamp_region;		/**< 印鉴区域 */
	RECT sign_region;		/**< 签字搜索区域 */
	char reserved[364];		/**< 保留空间 */
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
 * @param fname: 原始文件名，可以为NULL. 当采用清分机扫描时，可以通过
				此参数直接从数据库中查询E13B码结果。
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
 * 提取印鉴卡帐号	
 * 
 * @param img: 3-channel image data pointer
 * @param sizeX: size X of the image.
 * @param sizeY: size Y of the image.
 * @param dpi: dpi of the image.
 * @param milsys: mil system.
 * @param outAccno: 提取得到的帐号结果。
 * @return: 0-sucess.
 */
int ocr_stamp_card(unsigned char* img, 
				   int sizeX, int sizeY, int dpi, 
				   long milSys,
				   char* outAccno);

int ocr_stamp_card_file(const char* fname,int dpi,long milSys,
				   char* outAccno);


/*
 * 文件转换为通道图	
 */
int bmp_rgb( const char* fname, MemT& outData, int& outSizeX, int& outSizeY, 
			int& band, int& outDpi);

/*
 * 图像大小改变	
 */
int image_resize(unsigned char* srcImage,int srcX, int srcY, int Band, 
				 int destX, int destY, unsigned char* destImage);

/**
 * 进行区域坐标转换。由于版面处理使用200dpi，印章识别使用300dpi，
 * 需要将vch_process返回的坐标进行变换，方能用于识别处理
 * 
 * @base	:	vch_process返回的区域基准
 * @rcStamp	:	换算到300dpi的绝对印鉴区域
 * @rcSign	:	换算到300dpi的绝对签字区域
 * @useConfig	:	是否使用配置文件。当未进行自动识别时，可以使用配置文件
 *					OcrInfoA.xml读取区域坐标。
 *					如果loadConfig为true，则base，rcStamp, rcSign做为返回
 *					参数，返回配置值；反之使用该值计算。
 * @return 0-success
 */
int convert_region(POINT& base,RECT& rcStamp, RECT& rcSign, bool useConfig );

//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif //__VCHPROC_H__