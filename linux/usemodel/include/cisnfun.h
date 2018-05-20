#ifndef __CISNFUN_H__
#define __CISNFUN_H__

#ifdef __cplusplus
extern "C" {
#endif

// Error Code for function return value
enum {
	ZS_OK			= 0,
	ZS_MEM_ERR		= -1999,
	ZS_UNCMP_FAIL	= -2000
};

// Enum for TypeId in BMPIMG
enum SEAL_T{
	BI_SEAL_IMG		= 0,
	BI_SIGN_IMG		= 1,
	BI_NOTE_TXT		= 2,
	BI_GRP_PACK		= 3,
	BI_GRP_ITEM		= 4,
	BI_RAW_DATA		= 5,
};

//
// Note: the BMPIMG structure can be used to seal group package or seal group item
// to the group package: x-size, imgdata-package data
// to the group item:	x-money death line, imgdata- group info
struct BMPIMG{
	int x;                  // width of image
	int y;                  // height of image.
	char channels;			// 1 or 3

	bool IsMissMatch;		// this field only used in Identifyresult image, it means whether
	                        // the stamp is found or not.
	                        // true - miss matched, false - match
	bool IsPub;             // true - public seal,false - private seal
    char Reserved;          // not used now
	int  TypeId;			// SEAL_T value.

	unsigned char* imgdata;
};

struct IDBMPIMG
{
//	bool	bIsMissMatch;

	short   StampType;

	short	idres;
    //##ModelId=3AF8BD8E00BE
	BMPIMG	bmpimg;

	IDBMPIMG() { /*bIsMissMatch = false;*/ StampType = 0; idres = -1; memset(&bmpimg, 0, sizeof(BMPIMG)); }
};

int zs_get_vchinfo(int chkno, int chktype,  char* retAccount);
int zs_use_vch(int clientID, int chknoA,int chknoB, int chktype);

int zs_identify( int netno,
				char* acc,
				double imgTimeA,
				double imgTimeB,
				int vchNo,
				int vchType,
				double vchTime,
				double LastModify,
				int reason,
				int* retAutoID,
				int* retAccType );

/*
** User defined Message for download rate when identify remote client.
** The WPARAM is tID member of struct TRANSRATE, and the LPARAM is the iRate member.
*/
#define ZWM_DOWNLOAD_RATE	WM_USER+5198

int zs_identify_ex( int netno,
				char* acc,
				double imgTimeA,
				double imgTimeB,
				int vchNo,
				int vchType,
				double vchTime,
				double LastModify,
				int reason,
				int* retAutoID,
				int* retAccType,
				void* hWnd );


/*
retSingle:	 0 - NetWork,   1 - Single
*/
double zs_get_systime(int *retSingle);

int zs_get_autoid(int netno, char* acc);

int zs_get_host();

// ******************************************************************************
// Package counts of stamps into one memory block in order to save to file
// Note: This function will alloc memory and return through parameter retMem,
// Which should be free by call function zs_mem_free after usage.
// ******************************************************************************
int zs_pk_stamp( BMPIMG* img, int count, unsigned char** retMem, int *retSize );



#define zs_pk_free zs_mem_free
// ******************************************************************************
// Free memory alloced in this library
// ******************************************************************************
int zs_mem_free( unsigned char* Mem );



// ******************************************************************************
// Unpack data from memory wrapped by zs_pk_stamp into BMPIMG queue.
// Note: This function will alloc memory and return through parameter retImg,
// Which should be free by call function zs_img_free after usage.
// ******************************************************************************
int zs_upk_stamp( unsigned char* Mem, int Size, BMPIMG** retImg, int* retCount );



#define zs_upk_free zs_img_free
// ******************************************************************************
// Free memory with BMPIMG format alloced in this library
// ******************************************************************************
int zs_img_free( BMPIMG* img, int count);


// ******************************************************************************
// Decode data from memory wrapped by Qiu's package format into BMPIMG queue.
// Note: This function will alloc memory and return through parameter retImg,
// Which should be free by call function zs_img_free after usage.
// ******************************************************************************
int zs_decode_stamplib(unsigned char* DataFromLib, BMPIMG** retImg,
						int* retCount );

int zs_decode_stampgroup(unsigned char* SealGroupPackage, BMPIMG** retImg,
						int* retCount, int* retPubSealIndex);

int zs_judge_money_privalige( const char* seal_found, double cur_money,
						unsigned char* group_package );

int zs_get_shapeimg(BMPIMG* img, long MilSystem);

int zs_get_angleimg(BMPIMG* img, int angle);

int zs_img_done(const char* FileName);

int zs_img_done_ex(const char* fileName, unsigned long host);

// ******************************************************************************
// Get Image data from imgsrv by filename directly
// Note: This function will alloc memory and return through parameter retMem,
// Which should be free by call function zs_mem_free after usage.
// ******************************************************************************
int zs_img_byfile(char* filename_str, unsigned char** retMem, int* retSize);

int zs_img_byfile_ex(char const* filename_str, char const* const needType, 
					 unsigned char **retMem, int* retSize, int* retScannerType);

//----------------------------------------------------------------------------

#define QUEUE_IS_OVERFLOW			-100
#define QUEUE_IS_EMPTY				-101
#define SOCKET_FAILED				-102
#define QUERY_IMAGE_SUCCESS			0


void * ImageQueueInit();
int GetAnImage(void * QueueID, bool GoonQueryImg, unsigned char ** pVchImage);
void ImageQueueFree(void * QueueID);

//-----------------------------------------------------------------------------

// **************************************************************************
//  The following functions is used in programs which need to show histroy
//  stamps of special client.
// **************************************************************************
struct HistoryStampLib
{
    int     ClientAutoID;
    double  EnableDate;
    int     LibLength;
    int     Tag;
    char    Reservered[8];
    unsigned char* LibData;
};

enum {
    HSL_CONNECT_ERR     = 710,
    HSL_ACC_NOT_FOUND   = 711,
    HSL_STAMP_NOT_FOUND = 715
};

struct ClientInfo;      // this struct was defined in zvchinfodef.h
struct HistoryInfo
{
    ClientInfo*			TheAccInfo;
    HistoryStampLib*	TheStampLib;
    int					StampLibCount;

    HistoryInfo() { memset(this,0,sizeof(HistoryInfo)); }
};

/*
struct tagACCOUNTINFO;      // this struct was defined in zvchinfodef.h
struct HistoryInfo
{
    tagACCOUNTINFO* TheAccInfo;
    HistoryStampLib* TheStampLib;
    int              StampLibCount;

    HistoryInfo() { memset(this,0,sizeof(HistoryInfo)); }
};*/

// user should should alloc retHistInfo before put its address here.
// the memory in the HistoryInfo should be alloced/dealloced by the following funcs.
int zs_fetch_hsl(int netNo,char* accNo, HistoryInfo* retHisInfo);
int zs_free_hsl(HistoryInfo* hisInfo);



struct ImgFileInfo
{
	char fname[40];		/**< filename without extend name of the image file */
	char format[8];		/**< image format. eg "jpg", "bmp", "pnl" etc */
	int  fsize;			/**< file size */
	int	 dpi;			/**< resolution. 200 or 300 */
	int  scannerType;	/**< pre-defined scanner. see zvchtran.h */
	double calX;		/**< X calibration value */
	double calY;		/**< Y calibration value */
	char reserve[100];	/**< Unused */
};


/**
 * @param fname:	filename to get from imgsrv. NULL from random.
 * @param outInfo:  [out] Pointer to a ImgFileInfo structure that receives the information
 * @param outData:	[out] Pointer's Address that make pointer link to function alloced memory
					stored file data. it must be free by zs_mem_free after use.
 * @return:			0 - success, other failed.

 * get an image file from imgsrv.
 */
int zs_fetch_file( const char* fname, const char* host, ImgFileInfo *outInfo, unsigned char** outData );


struct ImgData
{
	int	x;				/**< x size of the image */
	int y;				/**< y size of the image */
	int channels;		/**< channels of the image. 1-gray, 3-color */
	int dpi;			/**< resolution. 200 or 300 */

};


/**
 * @param fdata:	[in] file data to be convert.
 * @param fsize:	[in] file data size.
 * @param dpi:		[in] resolution of image. 
							0 - auto get from file. 
							>0 - use dpi value.
 * @param fileformat: [in] file data's image format, ex "jpg","bmp"...
 * @param outInfo:	[out] Pointer to a ImgData structure that receives the information
 * @param outMem:	[out] Pointer's Address that make pointer link to function alloced memory
					stored image data. it must be free by zs_mem_free after use.

 * @return:		0 - success.
 */
int zs_file_to_image( unsigned char* fdata, int fsize, int dpi, const char* fileformat,
						ImgData* outInfo, unsigned char** outMem );



///////////////////////////////////////////////////////////////////////
int zs_new_autoid(int netNo,const char *accNo);

#ifdef __cplusplus
}
#endif


//////////////////////////////////////////////////////////////////
// Auto Link Section
#ifdef _MSC_VER
	#pragma comment(lib,"cisn.lib")
#endif
///////////////////////////////////////////////////////////////////

#endif //__CISNFUN_H__