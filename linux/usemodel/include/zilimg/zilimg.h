#ifndef __ZILIMG_H
#define __ZILIMG_H

#include <windows.h>
#include "zil.h"

/************************************************************************************
 * Structures
 ************************************************************************************/
#pragma pack(4)

typedef struct ZLUT {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
}ZLUT;

#pragma pack(8)

/*****************************************************************************************
Function:					min()
Putpose:			Finding smaller value of two unsigned char
Parameter:			a, b		Input Values
Return:				Unsigned char
******************************************************************************************/
//unsigned char min(unsigned char a, unsigned char b);
#ifndef min
#define min(a,b) (a<b)?a:b
#endif

/*****************************************************************************************
Function:					max()
Putpose:			Finding greater value of two unsigned char
Parameter:			a, b		Input Values
Return:				Unsigned char
******************************************************************************************/
//unsigned char max(unsigned char a, unsigned char b);
#ifndef max
#define max(a,b) (a>b)?a:b
#endif

#ifdef __cplusplus
extern "C" {
#endif

/************************************************************************************
 * Buffer Function prototypes
 ************************************************************************************/

/*
**	SystemId, DataType, Attribute has not been used in this version, just set to 0.
*/

/**
 * 初始化ZIL_ID图象句柄,并分配1维内存.
 *
 * @param SystemId: 0
 * @param SizeX: 数据大小
 * @param DataType: 0
 * @param Attribute: 0
 * @param BufIdPtr: 需要被初始化的ZIL_ID变量的地址
 * @retrun: 初始化后的ZIL_ID的值,此值即*BufIdPtr
 */
ZIL_ID 	ZbufAlloc1d             (ZIL_ID SystemId,
								 long SizeX,
								 long DataType,
								 long Attribute,
								 ZIL_ID *BufIdPtr);

/**
 * 初始化ZIL_ID图象句柄,并分配2维内存.此函数可用于分配灰度图象或256色彩色.
 *
 * @param SystemId: 0
 * @param SizeX: 图象数据X大小
 * @param SizeY: 图象数据Y大小
 * @param DataType: 0
 * @param Attribute: 0
 * @param BufIdPtr: 需要被初始化的ZIL_ID变量的地址
 * @retrun: 初始化后的ZIL_ID的值,此值即*BufIdPtr
 */
ZIL_ID 	ZbufAlloc2d				(ZIL_ID SystemId,
								 long SizeX,
								 long SizeY,
								 long DataType,
								 long Attribute,
								 ZIL_ID *BufIdPtr);

/**
 * 初始化ZIL_ID图象句柄,并分配2维内存.此函数可用于分配任意通道数量图象数据.
 *
 * @param SystemId: 0
 * @param SizeBand: 图象数据颜色通道数量,一般真彩色图象为3.
 * @param SizeX: 图象数据X大小
 * @param SizeY: 图象数据Y大小
 * @param DataType: 0
 * @param Attribute: 0
 * @param BufIdPtr: 需要被初始化的ZIL_ID变量的地址
 * @retrun: 初始化后的ZIL_ID的值,此值即*BufIdPtr
 */
ZIL_ID	ZbufAllocColor			(ZIL_ID SystemId,
								 long SizeBand,
								 long SizeX,
								 long SizeY,
								 long DataType,
								 long Attribute,
								 ZIL_ID *BufIdPtr);

ZIL_ID	ZbufAllocChildColor		(ZIL_ID ParentBufId,
								 long Band,
								 ZIL_ID *BufIdPtr);

/**
 * 释放已初始化的ZIL_ID图象句柄.仅限于ZbufXXX系列
 *
 * @param BufIdP: 被初始化的ZIL_ID句柄
 */
void 	ZbufFree             	(ZIL_ID BufId);


int 	ZbufPut2d				(ZIL_ID DestBufId,
								 long OffX,
								 long OffY,
								 long SizeX,
								 long SizeY,
								 void *UserArrayPtr);

/**
 * 拷贝图象数据到已分配的ZIL_ID图象句柄中. 如果此句柄已与显示句柄连接,即ZdispSelectXXX,
   则调用此函数后自动刷新显示窗口
 *
 * @param DestBufId: 已分配的ZIL_ID图象句柄
 * @param UserArrayPtr: 待拷贝数据的地址
 * @retrun: 0-success
 */
int 	ZbufPut					(ZIL_ID DestBufId,
								 void*  UserArrayPtr);


int		ZbufGet2d				(ZIL_ID SrcBufId,
								 long OffX,
								 long OffY,
								 long SizeX,
								 long SizeY,
								 void *UserArrayPtr);

void	ZbufClear				(ZIL_ID BufId,
								 long Color);

ZIL_ID 	ZbufImport				(char *Filename,
								 ZIL_ID FileFormatBufId,
								 long Operation,
								 ZIL_ID *BufIdPtr);

void 	ZbufExport				(char *FileName,
								 ZIL_ID FileFormatBufId,
								 ZIL_ID SrcBufId);

void	ZbufJPEGExport			(char *FileName,
								 ZIL_ID SrcBufId,
								 void *CustomSettingPtr);

int		ZbufCopyColor			(ZIL_ID SrcBufId,
								 ZIL_ID DestBufId,
								 long Band);

int		ZbufCopy				(ZIL_ID SrcBufId,
								 ZIL_ID DestBufId);


void	ZbufFlush				(ZIL_ID BufId);

unsigned char* ZbufGetDataPtr	(ZIL_ID BufId);


//***********************************************************************
// LUT -- Look Up Table Operations
//***********************************************************************

/**
 * 分配调色版句柄
 * 
 * @param Size: 调色板大小,一般为256
 * @param LutId: 需要被初始化的ZIL_ID变量的地址
 * @retrun: 初始化后的ZIL_ID的值,此值即*LutId
 */
ZIL_ID	ZlutAlloc			(int Size, ZIL_ID* LutId);

/**
 * 分配256色灰度调色版句柄
 * 
 * @param LutId: 需要被初始化的ZIL_ID变量的地址
 * @retrun: 初始化后的ZIL_ID的值,此值即*LutId
 */
ZIL_ID	ZlutAllocGray		(ZIL_ID* LutId);

/**
 * 取得调色版地址,用于直接对调色板进行操作
 * 
 * @param LutId: 已初始化的ZIL_ID调色板句柄
 * @retrun: 调色板结构指针
 */
ZLUT*	ZlutGetPtr			(ZIL_ID LutId);

/**
 * 释放已初始化的ZIL_ID调色板句柄.仅限于ZlutXXX系列
 *
 * @param LutId: 被初始化的ZIL_ID句柄
 */
void	ZlutFree			(ZIL_ID LutId);



//***********************************************************************
//  zdisp functions
//***********************************************************************

/**
 * 分配显示句柄
 * 
 * @param DisplayId: 需要被初始化的ZIL_ID变量的地址
 * @retrun: 初始化后的ZIL_ID的值,此值即*DisplayId
 */
ZIL_ID	ZdispAlloc			( ZIL_ID *DisplayId );

/**
 * 连接图象句柄,并显示图象数据到指定窗口
 * 
 * @param DisplayId: 已被初始化的ZIL_ID显示句柄
 * @param BufId: 已被初始化的ZIL_ID图象句柄
 * @param hWnd: 需要显示图象的Windows窗口句柄
 * @retrun: 0-success
 */
int		ZdispSelectWindow	( ZIL_ID DisplayId, ZIL_ID BufId, HWND hWnd );

/**
 * 连接图象句柄,并显示图象数据到指定窗口,并提供缩放功能
 * 
 * @param DisplayId: 已被初始化的ZIL_ID显示句柄
 * @param BufId: 已被初始化的ZIL_ID图象句柄
 * @param hWnd: 需要显示图象的Windows窗口句柄
 * @param Zoom: 是否进行缩放, true-按显示窗口大小进行缩放 false-显示原图
 * @param FocusX: 相对于显示窗口的缩放焦点X
 * @param FocusY: 相对于显示窗口的缩放焦点Y
 * @retrun: 0-success
 */
int		ZdispSelectWindowEx( ZIL_ID DisplayId,
							ZIL_ID BufId,
							HWND hWnd,
							bool Zoom,
							int FocusX,
							int FocusY );

/**
 * 连接图象句柄,并显示图象数据到ZIL自动创建的缺省窗口
 * 
 * @param DisplayId: 已被初始化的ZIL_ID显示句柄
 * @param BufId: 已被初始化的ZIL_ID图象句柄
 * @retrun: 0-success
 */
int		ZdispSelect			( ZIL_ID DisplayId, ZIL_ID BufId );

/**
 * 为显示句柄设置调色板, 当显示灰度图象或256色彩色图象时此步骤不可省略.
 * 
 * @param DisplayId: 已被初始化的ZIL_ID显示句柄
 * @param BufId: 已被初始化的ZIL_ID调色板句柄
 */
void	ZdispLut			( ZIL_ID DisplayId, ZIL_ID LutId );

/**
 * 释放已初始化的ZIL_ID显示句柄.仅限于ZdispXXX系列
 *
 * @param DisplayId: 被初始化的ZIL_ID句柄
 */
void	ZdispFree			( ZIL_ID DisplayId );



/*****************************************************************************************
 *	For Image process
 *****************************************************************************************/

/*****************************************************************************************
Function:					ZimFileConvert()
Purpose:			Convert image file from one format to the other.
					Support file format include jpg,png,tif,gif,pcx,bmp,tga.
Parameter:			destFileName	filename after convert
					srcFileName		filename to be converted
Return:				0 - success
******************************************************************************************/
int ZimFileConvert( const char* destFileName, const char* srcFileName);


/*****************************************************************************************
Function:					ZimFileConvertEx()
Purpose:			Convert image file from one format to the other.
					Support file format include jpg,png,tif,gif,pcx,bmp,tga.
Parameter:			destFileName	filename after convert
					destFormat		string supported format, eg. "jpg","bmp",...
					srcFileName		filename to be converted
					srcFormat		string supported format, eg. "jpg","bmp",...
Return:				0 - success
******************************************************************************************/
int ZimFileConvertEx( 	const char* destFileName, 
						const char* destFormat, 
						const char* srcFileName,
						const char* srcFormat);


/*****************************************************************************************
Function:					ZimFillPolygon()
Purpose:		
Return:				None
******************************************************************************************/

void ZimFillPolygon				(ZIL_ID SrcImageBufId,
								 ZIL_ID DestImageBufId,
								 long MarkValue);

/*****************************************************************************************
Function:					ZimErosion()
Purpose:			Performs erosion operation
Parameter:			pIN			Source image buffer
					pOUT		Result image buffer
					PWidth		Image width in pixels
					PHeight		Image height in pixels
					BWidth		Image width in bytes
					Mask		Pointer to structuring element data
					MaskLen		Length of structuring element data
Return:				None
******************************************************************************************/
void ZimErosion(unsigned char *pOUT, unsigned char *pIN, long PWidth, long PHeight, long
				BWidth, int Mask[9][2], long MaskLen);

/*****************************************************************************************
Function:					ZimDilation()
Purpose:			Performs dilation operation
Parameter:			pIN			Source image buffer
					pOUT		Result image buffer
					PWidth		Image width in pixels
					PHeight		Image height in pixels
					BWidth		Image width in bytes
					Mask		Pointer to the structuring element
					MaskLen		Length of structuring element data
return:				None
******************************************************************************************/
void ZimDilation(unsigned char *pOUT, unsigned char *pIN, long PWidth, long PHeight, long
				BWidth, int Mask[9][2], long MaskLen);

/*****************************************************************************************
Function:					ZimOpening()
Purpose:			Performs opening operation
Parameter:			pIN			Source image buffer
					pOUT		Result image buffer
					PWidth		Image width in pixels
					PHeight		Image height in pixels
					BWidth		Image width in bytes
					Mask		Pointer to the structuring element
					MaskLen		Length of structuring element data
return:				None
*****************************************************************************************/
void ZimOpening(unsigned char *pOUT, unsigned char *pIN, long PWidth, long PHeight, long
				BWidth, int Mask[9][2], long MaskLen);

/*****************************************************************************************
Function:					ZimClosing()
Purpose:			Performs closing operation
Parameter:			pIN			Source image buffer
					pOUT		Result image buffer
					PWidth		Image width in pixels
					PHeight		Image height in pixels
					BWidth		Image width in bytes
					Mask		Pointer to the structuring element
					MaskLen		Length of structuring element data
return:				None
******************************************************************************************/
void ZimClosing(unsigned char *pOUT, unsigned char *pIN, long PWidth, long PHeight, long
				BWidth, int Mask[9][2], long MaskLen);

/*****************************************************************************************
Function:					ZimSkeleton()
Purpose:			Finding image skeleton
Parameter:			pIN			Source image buffer
					pOUT		Result image buffer
					PWidth		Image width in pixels
					PHeight		Image height in pixels
					BWidth		Image width in bytes
					Mask		Pointer to structuring element data
					MaskLen		Length of structuring element data
Return:				None
******************************************************************************************/
void ZimSkeleton(unsigned char *pOUT, unsigned char *pIN, long PWidth, long PHeight, long
				BWidth, int Mask[9][2], long MaskLen);

/*****************************************************************************************
	<<<<													>>>>
	<<<<		GRAY SCALE MORPHOLOGY OPERATION				>>>>
	<<<<													>>>>
******************************************************************************************/
/*****************************************************************************************
					structuring element data
******************************************************************************************/
/*
int FlatSquare[9][2]={ 	{-1, -1}, {-1,  0}, {-1,  1},
						{ 0, -1}, { 0,  0}, { 0,  1},
						{ 1, -1}, { 1,  0}, { 1,  1} };
int FlatSqureVal[9]={0, 0, 0, 0, 0, 0, 0, 0, 0};
long FlatSquareLen=9;
int FlatLine[3][2]={ {0, -1}, {0, 0}, {0, 1} };
int FlatLineVal[3]={0, 0, 0};
long FlatLineLen=3;
*/

/*****************************************************************************************
Function:					GrayScaleErosion()
Purpose:			Performs gray scale erosion operation
Parameter:			pIN			Source image buffer
					pOUT		Result image buffer
					PWidth		Image width in pixels
					PHeight		Image height in pixels
					BWidth		Image width in pixels
					Mask		Image width in bytes
					MaskVal		Pointer to shape of structuring element
					MaskLen		Length of structuring element
					Radiu		Size of structuring element
Return:				None
*******************************************************************************************/
void GrayScaleErosion(unsigned char *pOUT, unsigned char *pIN, long PWidth, long PHeight,
					long BWidth, int Mask[9][2], int MaskVal[9], long MaskLen, int Radiu);

/*****************************************************************************************
Function:					GrayScaleDilation()
Purpose:			Performs gray scale erosion operation
Parameter:			pIN			Source image buffer
					pOUT		Result image buffer
					PWidth		Image width in pixels
					PHeight		Image height in pixels
					BWidth		Image width in pixels
					Mask		Image width in bytes
					MaskVal		Pointer to shape of structuring element
					MaskLen		Length of structuring element
					Radiu		Size of structuring element
Return:				None
*******************************************************************************************/
void GrayScaleDilation(unsigned char *pOUT, unsigned char *pIN, long PWidth, long PHeight,
					long BWidth, int Mask[9][2], int MaskVal[9], long MaskLen, int Radiu);

/******************************************************************************************
Function:					GrayScaleOpening()
Purpose:			Performs gary scale open operation
Parameter:			pIN			Source image buffer
					pOUT		Result image buffer
					PWidth		Image width in pixels
					PHeight		Image height in pixels
					BWidth		Image width in bytes
					Mask		Pointer to shape of structuring element
					MaskVal 	Pointer to scale value of structuring element
					MaskLen		Length of structuring element
					Radiu		Size of structuring element
Return:				None
******************************************************************************************/
void GrayScaleOpening(unsigned char *pOUT, unsigned char *pIN, long PWidth, long PHeight,
					long BWidth, int Mask[9][2], int MaskVal[9], long MaskLen, int Radiu);

/******************************************************************************************
Function:					GrayScaleClosing()
Purpose:			Performs gary scale close operation
Parameter:			pIN			Source image buffer
					pOUT		Result image buffer
					PWidth		Image width in pixels
					PHeight		Image height in pixels
					BWidth		Image width in bytes
					Mask		Pointer to shape of structuring element
					MaskVal 	Pointer to scale value of structuring element
					MaskLen		Length of structuring element
					Radiu		Size of structuring element
Return:				None
******************************************************************************************/
void GrayScaleClosing(unsigned char *pOUT, unsigned char *pIN, long PWidth, long PHeight,
					long BWidth, int Mask[9][2], int MaskVal[9], long MaskLen, int Radiu);


/******************************************************************************************
Function:					Bmp_MemCopy()
Purpose:			Convert data into bitmap format.
Parameter:			pDest		Target Data buffer
					pFrom		Source image buffer
					SrcX		Width of the Image
					SrcY		Height of the Image
					BitCount	8 -- 256 Color; 24 -- 24 bit Color
Return:				None
******************************************************************************************/


void Bmp_MemCopy( void* pDest, void* pFrom, int SrcX, int SrcY, int BitCount );

void Bmp_FillInfoHeader(int Width, int Height,int nBitsPerPixel, LPBITMAPINFO m_pInfo);



HBITMAP ZimRotateImg(unsigned char* pMem, int memSize, const char* format, int angle);

#ifdef __cplusplus
}
#endif


//////////////////////////////////////////////////////////////////
// Auto Link Section
#ifdef _MSC_VER
	#pragma comment(lib,"zil.lib")
#endif
///////////////////////////////////////////////////////////////////

#endif