#ifndef		__FOO_STDDEFINE_H__
#define		__FOO_STDDEFINE_H__
#include <windows.h>
#pragma once

#ifndef PCTSTR
typedef const char* PCTSTR;
#endif

//=============================================================================
//	常数
//=============================================================================
const double	LIB_PI = 3.1415926536 ;
//=============================================================================
//	基本数据类型
//=============================================================================
typedef		signed char		INT8 ;
typedef		unsigned char	UINT8 ;
typedef		signed short	INT16 ;
typedef		unsigned short	UINT16 ;
typedef		signed int		INT32 ;
typedef		unsigned int	UINT32 ;
//=============================================================================
//	逻辑操作
//=============================================================================
#define		LOG_OPERATION_AND			0x00000001
#define		LOG_OPERATION_OR			0x00000002
#define		LOG_OPERATION_XOR			0x00000004
#define		LOG_OPERATION_ADD			0x00000008
#define		LOG_OPERATION_SUB			0x00000010
#define		LOG_OPERATION_MUL			0x00000020
#define		LOG_OPERATION_DIV			0x00000040
//=============================================================================
//	平面8个方向
//=============================================================================
#define		DIRECT_UPPER_LEFT			0x00000001
#define		DIRECT_UPPER				0x00000002
#define		DIRECT_UPPER_RIGHT			0x00000004
#define		DIRECT_LEFT					0x00000008
#define		DIRECT_RIGHT				0x00000010
#define		DIRECT_LOWER_LEFT			0x00000020
#define		DIRECT_LOWER				0x00000040
#define		DIRECT_LOWER_RIGHT			0x00000080
//=============================================================================
//	16, 32 位色的掩码
//=============================================================================
#define		MASK16_RED_565				0x0000F800
#define		MASK16_GREEN_565			0x000007E0
#define		MASK16_BLUE_565				0x0000001F
#define		MASK16_RED_555				0x00007C00
#define		MASK16_GREEN_555			0x000003E0
#define		MASK16_BLUE_555				0x0000001F
#define		MASK32_ALPHA				0xFF000000
#define		MASK32_RED					0x00FF0000
#define		MASK32_GREEN				0x0000FF00
#define		MASK32_BLUE					0x000000FF
//=============================================================================
//	RGBA通道
//=============================================================================
#define		CHANNEL_RED					0x00000001
#define		CHANNEL_GREEN				0x00000002
#define		CHANNEL_BLUE				0x00000004
#define		CHANNEL_ALPHA				0x00000008
//=============================================================================
//	图形文件格式
//=============================================================================
#define		IMAGE_UNKNOW				0x00000000
#define		IMAGE_BMP					0x00000001
#define		IMAGE_PCX					0x00000002
#define		IMAGE_JPEG					0x00000004
#define		IMAGE_GIF					0x00000008
#define		IMAGE_TGA					0x00000010
#define		IMAGE_TIFF					0x00000020
#define		IMAGE_PNG					0x00000040
#define		IMAGE_ICO					0x00000080
#define		IMAGE_WMF					0x00000100

#define		GIF_MAX_FRAME				0x00000800 // 最多支持2048帧的GIF
//=============================================================================
// Foolish Simple Demo Windows Classes
//=============================================================================
#define		WND_DEFAULT_CLASS		TEXT("Foolish_Class_Template")
#define		WND_PROPERTY_this		TEXT("this_address")
#define		INVALID_FILE_ATTRIBUTES	0xFFFFFFFF

#define		DLG_RESIZE_MOVE			0x00000001
#define		DLG_RESIZE_STRETCH		0x00000002

//=============================================================================
//	FCBitmap::DrawEx(...) 的 ShowParam 参数
//=============================================================================
typedef struct tagSHOWEFFECT
{
	DWORD	dwMethod ;		// 方法
	DWORD	dwSubMethod ;	// 具体方法
	DWORD	dwInterval ;	// 间隔的 (微/毫) 秒数
	DWORD	dwBlockLen ;	// 视方法而定
	RECT	rcClient ;		// 显示的区域
	SIZE	sizeStretch ;	// SHOWDIB_STRETCH 和 SHOWDIB_ISOSTRETCH 的目标大小
} SHOWEFFECT, * PSHOWEFFECT ;
//#define		SHOWDIB_CENTER			0x40000000 // 此方法可以和以下方法‘|’

#define		SHOWDIB_NORMAL			0x80000000
#define		SHOWDIB_STRETCH			0x00000001
#define		SHOWDIB_ISOSTRETCH		0x00000002 // 成比例缩放

//	SCAN方法 : dwInterval域为间隔<微>秒数
#define		SHOWDIB_SCAN			0x00000004 
#define			SHOWDIB_SCAN_TODOWN		0x0001 
#define			SHOWDIB_SCAN_TOUP		0x0002
#define			SHOWDIB_SCAN_TOLEFT		0x0004
#define			SHOWDIB_SCAN_TORIGHT	0x0008
#define			SHOWDIB_SCAN_HORIZON	0x0010
#define			SHOWDIB_SCAN_VERTICAL	0x0020
#define			SHOWDIB_SCAN_VINTERLACE	0x0040
#define			SHOWDIB_SCAN_HINTERLACE	0x0080

//	MOVE方法 : dwInterval域为间隔<微>秒数
//			   对于较大的图片会较慢
#define		SHOWDIB_MOVE			0x00000008
#define			SHOWDIB_MOVE_TODOWN		0x0001
#define			SHOWDIB_MOVE_TOUP		0x0002
#define			SHOWDIB_MOVE_TOLEFT		0x0004
#define			SHOWDIB_MOVE_TORIGHT	0x0008
#define			SHOWDIB_MOVE_HORIZON	0x0010
#define			SHOWDIB_MOVE_VERTICAL	0x0020

//	百叶窗方法 : dwInterval域为间隔<毫>秒数
//				 dwBlockLen域为块的间隔
#define		SHOWDIB_HSHUTTER		0x00000010
#define		SHOWDIB_VSHUTTER		0x00000020

//	马赛克显示方法 : dwInterval域为间隔<微>秒数
//					 dwBlockLen域为小正方形边长
#define		SHOWDIB_MOSAIC			0x00000040

//	渐现方法 : dwInterval域为间隔<毫>秒数
//			   dwBlockLen域为跨度
#define		SHOWDIB_FADE			0x00000080
#define		SHOWDIB_FADE_SCAN		0x00000100

//=============================================================================
//	FCDibEffect::Algorithm () 参数
//=============================================================================
#define		EFFECT_SMOOTH_1				0x00000001	// 平滑
#define		EFFECT_SMOOTH_2				0x00000002
#define		EFFECT_SMOOTH_3				0x00000004
#define		EFFECT_SHARP_1				0x00000008	// 锐化
#define		EFFECT_SHARP_2				0x00000010
#define		EFFECT_SHARP_3				0x00000020
#define		EFFECT_ENHANCE_VERT			0x00000040	// 边缘增强
#define		EFFECT_ENHANCE_HORZ			0x00000080
#define		EFFECT_ENHANCE_NORTH		0x00000100
#define		EFFECT_ENHANCE_SOUTH		0x00000200
#define		EFFECT_ENHANCE_WEST			0x00000400
#define		EFFECT_ENHANCE_EAST			0x00000800
#define		EFFECT_ENHANCE_NORTHEAST	0x00001000
#define		EFFECT_ENHANCE_SOUTHWEST	0x00002000
#define		EFFECT_ENHANCE_NORTHWEST	0x00004000
#define		EFFECT_ENHANCE_SOUTHEAST	0x00008000
#define		EFFECT_ENHANCE_VERTHORZ		0x00010000
#define		EFFECT_ENHANCE_LAPLACE1		0x00020000	// Laplace边缘增强
#define		EFFECT_ENHANCE_LAPLACE2		0x00040000
#define		EFFECT_ENHANCE_LAPLACE3		0x00080000
#define		EFFECT_ENHANCE_LAPLACE4		0x00100000

typedef struct tagFOODIB
{
	BITMAPINFOHEADER	DibInfo ;  // DIB Info
	HBITMAP				hBitmap ;  // Handle from CreateDIBSection (DIB-Handle)
	BYTE			  * pByte ;	   // Bitmap bits, left-bottom point
	BYTE			 ** ppLine ;   // Line-pointer , ppLine[] ; 自上而下
} FOODIB, * PFOODIB ;

typedef struct tagFPICINFO
{
	DWORD		dwImageType ;		// 文件类型, 见上 : IMAGE_XXX
	DWORD		dwWidth ;			// 宽
	DWORD		dwHeight ;			// 高
	DWORD		dwLoadTime ;		// 读取时间(ms)
	WORD		wBitCount ;			// Gif则为真实颜色位数 [1..8]
	WORD		wTotalFrame	;		// 文件所包含帧数
	WORD		wDelayTime ;		// 两帧间延时毫秒数
	BYTE		byInterlace ;		// 是否以交错方式存储
	BYTE		byTransFlag ;		// 是否有透明色
	RGBQUAD		rgbTransparency ;	// 透明色索引值, 此颜色不显示在屏幕上
} FPICINFO, * PFPICINFO ; // 图像文件信息

typedef struct tagVOLUMEINFO // 磁盘信息
{
	TCHAR	 Driver[32] ; 			
	TCHAR	 FileType[32] ;			// 文件系统类型 (such as "FAT32", "FAT16", "NTFS", "CDFS")
	TCHAR 	 VolumeName[32] ;		// 磁盘卷标
	DWORD 	 DiskType ;				// 磁盘类型 ( 见::GetDriveType )
	DWORD	 dwVolumeSerialNumber ;	// 磁盘序列号
} VOLUMEINFO, * PVOLUMEINFO ;

typedef struct tagDISPLAYMODE
{
	DWORD	dwBits ;
	DWORD	dwWidth ;
	DWORD	dwHeight ;
	DWORD	dwFrequency ;
} DISPLAYMODE, * PDISPLAYMODE ; // 屏幕信息

//=============================================================================

#endif