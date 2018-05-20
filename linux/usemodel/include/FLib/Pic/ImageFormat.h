#ifndef		__FOO_IMAGEFORMAT_H__
#define		__FOO_IMAGEFORMAT_H__
#include <windows.h>
#pragma once

//=============================================================================
//	PCX 图像文件头
//=============================================================================
#include <pshpack2.h>
typedef struct tagPCXHEAD
{
	BYTE	signal ;		// 00 - always 0x0A
	BYTE	version ;		// 01 - version number (0,2,3,4,5)
	BYTE	encoding ;		// 02 - always 1 (RLE)
	BYTE	bit_per_pixel ;	// 03 - 每平面像素bit数 (1 or 4 or 8)
	WORD	xmin ;			// 04 
	WORD	ymin ;			// 06   此四个域指出
	WORD	xmax ;			// 08     图像大小
	WORD	ymax ;			// 0A  
	WORD	Xresolution ;	// 0C - 屏幕X解析度
	WORD	Yresolution ;	// 0E - 屏幕Y解析度
	BYTE	palette[48] ;	// 10 - 1 or 4 Bits 图像调色板
	BYTE	reserved ;		// 40 - Set 0
	BYTE	color_planes ;	// 41 - 色彩平面数目 (1 or 3 or 4)
	WORD	byte_per_line ;	// 42 - 每行字节宽度 (每色彩plane)
	WORD	palette_type ;	// 44 - always 1
	BYTE	filler[58] ;	// 46 - Set 0
} PCXHEAD, * PPCXHEAD ; // 128 - Bytes
#include <poppack.h>

//=============================================================================
//	TGA 图像文件头 ( 与BMP文件一样，像素从左下角存储 )
//=============================================================================
#include <pshpack1.h>
typedef struct tagTGAHEAD
{
	BYTE	byID_Length ;	// 图像识别信息大小
	BYTE	byPalType ;		// 00 : 无调色板	01 : 有调色板
	BYTE	byImageType ;	// 01-UC+PAL, 02-UC+NP, 09-RLE+PAL, 0x0A-RLE+NP
	WORD	wPalFirstNdx ;	// 调色板起始索引
	WORD	wPalLength ;	// 调色板长度
	BYTE	byPalBits ;		// 调色板中每一颜色所占位数
	WORD	wLeft ;			// 相对于屏幕左下角X坐标
	WORD	wBottom ;		// 相对于屏幕左下角Y坐标
	WORD	wWidth ;		// 宽
	WORD	wHeight ;		// 高
	BYTE	byColorBits ;	// 色彩位数
	struct 
	{
		BYTE	AlphaBits : 4 ;	// 每像素Alpha Channel位数
		BYTE	HorMirror : 1 ;	// 为1表示图像数据左右颠倒存储
		BYTE	VerMirror : 1 ;	// 为1表示图像数据上下颠倒存储
		BYTE	Reserved : 2 ;
	} Descriptor ;
} TGAHEAD, * PTGAHEAD ; // 18 - Bytes
#include <poppack.h>

//=============================================================================
//	GIF 文件头
//=============================================================================
#include <pshpack1.h>
typedef struct tagGIFHEAD
{
	BYTE	bySignature[3] ;	// 固定为 "GIF"
	BYTE	byVersion[3]   ;	// "89a" 或 "87a"
	WORD	wWidth  ;			// 屏幕宽度 
	WORD	wHeight ;			// 屏幕高度
	struct
	{
		BYTE	PalBits	  : 3 ;	// 确定调色板大小 : 3 * (1 << (PalBits + 1))
		BYTE	SortFlag  : 1 ;	// 调色板中的颜色是否按使用率排序
		BYTE	ColorRes  : 3 ;	// 可计算分辨率   : 3 * (1 << (ColorRes + 1))
		BYTE	GlobalPal : 1 ;	// 全局调色板, 为 1 时PalBits才有意义
	} GlobalFlag ;
	BYTE	byBackground ;		// 图像小于逻辑屏幕时, 用此颜色填充未被覆盖的区域
	BYTE	byAspect ;			// 逻辑屏幕宽高比
} GIFHEAD, * PGIFHEAD ;  // 13 - Bytes
#include <poppack.h>

/************************************************************/
/*		全局调色板在文件中的偏移量总是13					*/
/************************************************************/
typedef struct tagGIFINFO
{
	BYTE	byVersion[4]   ;		// "89a" 或 "87a"
	WORD	wTotalFrame	;			// 总帧数
	WORD	wCurrentFrame ;			// 当前帧号 [0..wTotalFrame]
	WORD	wGolPalNum ;			// 全局调色板颜色个数, 为0则无
	WORD	wDelayTime ;			// 两帧间延时毫秒数
	BYTE	byInterlace ;			// 是否以交错方式存储
	BYTE	byTransFlag ;			// 是否有透明色
	BYTE	byTransparencyIndex ;	// 透明色索引值, 此颜色不显示在屏幕上
	BYTE	byBitCount ;			// 为真实ColorBit数
	DWORD	* pFrameIndexArray ;	// 帧指针, 在文件中的偏移量, 定位在0x2C后的第一个字节
	BYTE	* pStart ;				// 数据流的起始指针
}  GIFINFO, * PGIFINFO ;

//===================================================================

#endif