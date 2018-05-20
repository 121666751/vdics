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
 * ��ʼ��ZIL_IDͼ����,������1ά�ڴ�.
 *
 * @param SystemId: 0
 * @param SizeX: ���ݴ�С
 * @param DataType: 0
 * @param Attribute: 0
 * @param BufIdPtr: ��Ҫ����ʼ����ZIL_ID�����ĵ�ַ
 * @retrun: ��ʼ�����ZIL_ID��ֵ,��ֵ��*BufIdPtr
 */
ZIL_ID 	ZbufAlloc1d             (ZIL_ID SystemId,
								 long SizeX,
								 long DataType,
								 long Attribute,
								 ZIL_ID *BufIdPtr);

/**
 * ��ʼ��ZIL_IDͼ����,������2ά�ڴ�.�˺��������ڷ���Ҷ�ͼ���256ɫ��ɫ.
 *
 * @param SystemId: 0
 * @param SizeX: ͼ������X��С
 * @param SizeY: ͼ������Y��С
 * @param DataType: 0
 * @param Attribute: 0
 * @param BufIdPtr: ��Ҫ����ʼ����ZIL_ID�����ĵ�ַ
 * @retrun: ��ʼ�����ZIL_ID��ֵ,��ֵ��*BufIdPtr
 */
ZIL_ID 	ZbufAlloc2d				(ZIL_ID SystemId,
								 long SizeX,
								 long SizeY,
								 long DataType,
								 long Attribute,
								 ZIL_ID *BufIdPtr);

/**
 * ��ʼ��ZIL_IDͼ����,������2ά�ڴ�.�˺��������ڷ�������ͨ������ͼ������.
 *
 * @param SystemId: 0
 * @param SizeBand: ͼ��������ɫͨ������,һ�����ɫͼ��Ϊ3.
 * @param SizeX: ͼ������X��С
 * @param SizeY: ͼ������Y��С
 * @param DataType: 0
 * @param Attribute: 0
 * @param BufIdPtr: ��Ҫ����ʼ����ZIL_ID�����ĵ�ַ
 * @retrun: ��ʼ�����ZIL_ID��ֵ,��ֵ��*BufIdPtr
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
 * �ͷ��ѳ�ʼ����ZIL_IDͼ����.������ZbufXXXϵ��
 *
 * @param BufIdP: ����ʼ����ZIL_ID���
 */
void 	ZbufFree             	(ZIL_ID BufId);


int 	ZbufPut2d				(ZIL_ID DestBufId,
								 long OffX,
								 long OffY,
								 long SizeX,
								 long SizeY,
								 void *UserArrayPtr);

/**
 * ����ͼ�����ݵ��ѷ����ZIL_IDͼ������. ����˾��������ʾ�������,��ZdispSelectXXX,
   ����ô˺������Զ�ˢ����ʾ����
 *
 * @param DestBufId: �ѷ����ZIL_IDͼ����
 * @param UserArrayPtr: ���������ݵĵ�ַ
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
 * �����ɫ����
 * 
 * @param Size: ��ɫ���С,һ��Ϊ256
 * @param LutId: ��Ҫ����ʼ����ZIL_ID�����ĵ�ַ
 * @retrun: ��ʼ�����ZIL_ID��ֵ,��ֵ��*LutId
 */
ZIL_ID	ZlutAlloc			(int Size, ZIL_ID* LutId);

/**
 * ����256ɫ�Ҷȵ�ɫ����
 * 
 * @param LutId: ��Ҫ����ʼ����ZIL_ID�����ĵ�ַ
 * @retrun: ��ʼ�����ZIL_ID��ֵ,��ֵ��*LutId
 */
ZIL_ID	ZlutAllocGray		(ZIL_ID* LutId);

/**
 * ȡ�õ�ɫ���ַ,����ֱ�ӶԵ�ɫ����в���
 * 
 * @param LutId: �ѳ�ʼ����ZIL_ID��ɫ����
 * @retrun: ��ɫ��ṹָ��
 */
ZLUT*	ZlutGetPtr			(ZIL_ID LutId);

/**
 * �ͷ��ѳ�ʼ����ZIL_ID��ɫ����.������ZlutXXXϵ��
 *
 * @param LutId: ����ʼ����ZIL_ID���
 */
void	ZlutFree			(ZIL_ID LutId);



//***********************************************************************
//  zdisp functions
//***********************************************************************

/**
 * ������ʾ���
 * 
 * @param DisplayId: ��Ҫ����ʼ����ZIL_ID�����ĵ�ַ
 * @retrun: ��ʼ�����ZIL_ID��ֵ,��ֵ��*DisplayId
 */
ZIL_ID	ZdispAlloc			( ZIL_ID *DisplayId );

/**
 * ����ͼ����,����ʾͼ�����ݵ�ָ������
 * 
 * @param DisplayId: �ѱ���ʼ����ZIL_ID��ʾ���
 * @param BufId: �ѱ���ʼ����ZIL_IDͼ����
 * @param hWnd: ��Ҫ��ʾͼ���Windows���ھ��
 * @retrun: 0-success
 */
int		ZdispSelectWindow	( ZIL_ID DisplayId, ZIL_ID BufId, HWND hWnd );

/**
 * ����ͼ����,����ʾͼ�����ݵ�ָ������,���ṩ���Ź���
 * 
 * @param DisplayId: �ѱ���ʼ����ZIL_ID��ʾ���
 * @param BufId: �ѱ���ʼ����ZIL_IDͼ����
 * @param hWnd: ��Ҫ��ʾͼ���Windows���ھ��
 * @param Zoom: �Ƿ��������, true-����ʾ���ڴ�С�������� false-��ʾԭͼ
 * @param FocusX: �������ʾ���ڵ����Ž���X
 * @param FocusY: �������ʾ���ڵ����Ž���Y
 * @retrun: 0-success
 */
int		ZdispSelectWindowEx( ZIL_ID DisplayId,
							ZIL_ID BufId,
							HWND hWnd,
							bool Zoom,
							int FocusX,
							int FocusY );

/**
 * ����ͼ����,����ʾͼ�����ݵ�ZIL�Զ�������ȱʡ����
 * 
 * @param DisplayId: �ѱ���ʼ����ZIL_ID��ʾ���
 * @param BufId: �ѱ���ʼ����ZIL_IDͼ����
 * @retrun: 0-success
 */
int		ZdispSelect			( ZIL_ID DisplayId, ZIL_ID BufId );

/**
 * Ϊ��ʾ������õ�ɫ��, ����ʾ�Ҷ�ͼ���256ɫ��ɫͼ��ʱ�˲��費��ʡ��.
 * 
 * @param DisplayId: �ѱ���ʼ����ZIL_ID��ʾ���
 * @param BufId: �ѱ���ʼ����ZIL_ID��ɫ����
 */
void	ZdispLut			( ZIL_ID DisplayId, ZIL_ID LutId );

/**
 * �ͷ��ѳ�ʼ����ZIL_ID��ʾ���.������ZdispXXXϵ��
 *
 * @param DisplayId: ����ʼ����ZIL_ID���
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