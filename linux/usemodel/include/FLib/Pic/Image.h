//////////////////////////////////////////////////////////////////
//																//
//		用途 : 图像文件处理操作 Jpg,Gif,Bmp,Pcx,Tga,Png,Tif		//
//		创建 : [Foolish] / 2001-4-27							//
//		更新 : 2002-11-9										//
//		主页 : http://crazybit.topcities.com/					//
//		邮箱 : crazybit@263.net									//
//									(c) 1999 - 2002 =USTC= 付黎	//
//////////////////////////////////////////////////////////////////
#ifndef	 __FOO_PICIMAGE_H__
#define	 __FOO_PICIMAGE_H__
#include "Effect.h"
#include "..\Compress\Lzw.h"
extern "C"
{
	#include "..\jpeg\jpeglib.h"
	#include "..\jpeg\jerror.h"
	#include "..\tiff\tiffio.h"
} ;
#pragma once

//===================================================================
//	图像文件处理
//===================================================================
/********************************************************************/
/*		对于Save, 文件存在则覆盖									*/
/********************************************************************/
class FCImage : public FCDibEffect
{
public :
	FCImage () ;
	virtual ~FCImage () ;
	void	operator= (const FCImage & pic) ;
	void	operator= (const FCDibEffect & pic) ;
	void	operator= (const FCDib & pic) ;
	void	GetImageInfo (PCTSTR szFileName, FPICINFO * PicInfo) ;
	void	GetGifInfo (GIFINFO * gifinfo) ;

	/************************************************************/
	/*					自动处理								*/
	/************************************************************/
	BOOL	Load (PCTSTR szFileName) ;
	BOOL	Save (PCTSTR szFileName) ;

	/************************************************************/
	/*				BMP图像文件模块								*/
	/*		支  持 : 1, 4, 8, 16, 24, 32位色图像				*/
	/*		不支持 : RLE格式的位图								*/
	/************************************************************/
	BOOL	LoadBmp (PCTSTR szFileName) ;
	BOOL	SaveBmp (PCTSTR szFileName) ;

	/************************************************************/
	/*				PCX图像文件模块								*/
	/*		支  持 : 1, 4, 8, 24位色图像						*/
	/*		不支持 : 4 plane 的16位色位图						*/
	/************************************************************/
	BOOL	LoadPcx (BYTE * pStart, int iFileSize) ; // 读内存
	BOOL	LoadPcx (PCTSTR resName, PCTSTR resType) ; // 从资源中读取
	BOOL	LoadPcx (PCTSTR szFileName) ;
	BOOL	SavePcx (PCTSTR szFileName) ;

	/************************************************************/
	/*				TGA图像文件模块								*/
	/*		支  持 : 8, 16, 24, 32位色图像						*/
	/*		不支持 : 15位色图像, 15,16位色调色板图像,			*/
	/*				 左右镜像的TGA								*/
	/************************************************************/
	BOOL	LoadTga (BYTE * pStart) ; // 读内存
	BOOL	LoadTga (PCTSTR resName, PCTSTR resType) ; // 从资源中读取
	BOOL	LoadTga (PCTSTR szFileName) ;
	BOOL	SaveTga (PCTSTR szFileName, bool bEncode = true) ; // 是否使用RLE压缩

	/************************************************************/
	/*				JPEG图像文件模块							*/
	/************************************************************/
	BOOL	LoadJpeg (BYTE * pStart, int iFileSize) ; // 读内存
	BOOL	LoadJpeg (PCTSTR resName, PCTSTR resType) ; // 从资源中读取
	BOOL	LoadJpeg (PCTSTR szFileName) ;
	BOOL	SaveJpeg (PCTSTR szFileName, DWORD dwQuality = 85) ; // 只对8(gray), 24位色有效

	/************************************************************/
	/*				TIFF图像文件模块							*/
	/*		不支持 : 16位色DIB的Save							*/
	/************************************************************/
	BOOL	LoadTif (BYTE * pStart, int iFileSize) ; // 读内存
	BOOL	LoadTif (PCTSTR resName, PCTSTR resType) ; // 从资源中读取
	BOOL	LoadTif (PCTSTR szFileName) ;
	BOOL	SaveTif (PCTSTR szFileName, int nEncodeMode = COMPRESSION_JPEG) ;

	/************************************************************/
	/*				PNG图像文件模块								*/
	/*	 本模块使用 libpng version 1.2.1 (December 12, 2001)	*/	
	/*		Copyright (c) 1998-2001 Glenn Randers-Pehrson		*/
	/************************************************************/
	BOOL	LoadPng (BYTE * pStart) ; // 读内存
	BOOL	LoadPng (PCTSTR resName, PCTSTR resType) ; // 从资源中读取
	BOOL	LoadPng (PCTSTR szFileName) ;
	BOOL	SavePng (PCTSTR szFileName, bool bInterlace = true) ;

	/************************************************************/
	/*				GIF图像文件模块								*/
	/*		支  持 : 1--8位色动画, 交错存储GIF解码				*/
	/*				 1, 4, 8位色动画GIF编码						*/
	/*		不支持 :											*/
	/************************************************************/
	BOOL	LoadGif (PCTSTR resName, PCTSTR resType) ; // 从资源中读取第一帧
	BOOL	LoadGif (PCTSTR szFileName) ; // 读入第一帧
	BOOL	LoadGifFrame (int iNumber) ; // iNumber = [1..wTotalFrame]
	int		LoadGifNextFrame () ; // 读入下一帧, 必须先调用了LoadGif (0-失败, 1-成功, 2-最后一帧)

	//	把当前DIB存为gif文件, bGolPal为是否使用此帧调色板作为全局调色板
	BOOL	SaveGif (PCTSTR szFileName, bool bGolPal = true) ;
	//	把当前DIB加入到gif文件的最后, bGolPal为是否使用全局调色板作为此帧的调色板
	//	dwDelay为两帧间延时毫秒数, dwTransIndex为透明色,-1表示无透明色
	BOOL	AddGifFrame (PCTSTR szFileName, bool bGolPal = false,
						 DWORD dwDelay = 100, DWORD dwTransIndex = -1) ;
//	BOOL	InsertGifFrame (PCTSTR szFileName, const FCDib & dib) ;

private :
	GIFINFO		* m_pGifInfo ;
private :
	int		ImageType (PCTSTR szFileName) ;	 // 根据文件名后缀获得文件类型
	bool	__fooGifScanFile (BYTE * pStart, int iFileSize) ; // 扫描获取Gif文件信息, 填充m_pGifInfo
	void	__fooGifInitInfo (int iFileSize) ; // 初始化m_pGifInfo
	void	__fooGifFreeInfo () ; // 释放m_pGifInfo资源
	void	__fooGifDecode (BYTE * InBuffer) ; // 解码, 因为在SEH中类的析构函数不能插入
	DWORD	__fooGifEncode (BYTE * InBuffer) ; // 编码, 因为在SEH中类的析构函数不能插入
	BYTE  *	__fooImageReadFile (PCTSTR szFileName, HANDLE * hFile, HANDLE * hMap) ; // 映射文件
	BYTE  *	__fooImageSaveFile (PCTSTR szFileName, HANDLE * hFile, HANDLE * hMap, int iMaxSize) ; // 映射文件
	BYTE  *	__fooGifOpenFileForAddFrame (PCTSTR szFileName, HANDLE * hFile, HANDLE * hMap, int iMaxSize) ; // 映射文件
	void	__fooImageUnmapFile (BYTE * pBase, HANDLE hMap, HANDLE hFile) ; // 撤销映射
// JPEG helper class in order to load JPEG from memory
private :
class FCJpegStream : public jpeg_source_mgr
{
public:
	FCJpegStream (BYTE * pStart, int iFileSize)
	{
		next_input_byte = NULL ; bytes_in_buffer = 0 ; // base class member
		m_pBase = pStart ; m_iFileSize = iFileSize ;
		m_END[0] = (JOCTET) 0xFF ; m_END[1] = (JOCTET) JPEG_EOI ;
		fill_input_buffer = FillInputBuffer ;
		skip_input_data = SkipInputData ;
		resync_to_restart = jpeg_resync_to_restart ; // use default method
		init_source = InitSource ; term_source = TermSource ; // none use
	}
	static boolean FillInputBuffer (j_decompress_ptr cinfo)
	{
		FCJpegStream	* pSrc = (FCJpegStream *)cinfo->src ;
		pSrc->next_input_byte = (pSrc->m_iFileSize > 0) ? pSrc->m_pBase : pSrc->m_END ;
		pSrc->bytes_in_buffer = (pSrc->m_iFileSize > 0) ? pSrc->m_iFileSize : 2 ; // fake EOI marker
		return TRUE ;
	}
	static void SkipInputData (j_decompress_ptr cinfo, long num_bytes)
	{
		FCJpegStream	* pSrc = (FCJpegStream *)cinfo->src ;
		if (num_bytes > 0)
			if (pSrc->bytes_in_buffer > num_bytes)
			{
				pSrc->next_input_byte += num_bytes ;
				pSrc->bytes_in_buffer -= num_bytes ;
			}
			else
			{
				pSrc->next_input_byte = pSrc->m_END ;
				pSrc->bytes_in_buffer = 2 ;
			}
	}
	static void TermSource (j_decompress_ptr cinfo) { }
	static void InitSource(j_decompress_ptr cinfo) { }
public :
	BYTE		* m_pBase ;
	int			m_iFileSize ;
	BYTE		m_END[2] ;
} ;
} ;

//=============================================================================
//	Implement
//=============================================================================
inline FCImage::FCImage () {
	m_pGifInfo = NULL ;
}
inline FCImage::~FCImage () {
	this->__fooGifFreeInfo () ;
}
inline void  FCImage::operator= (const FCImage & pic) {
	FCDib::operator = (*(FCDib *)&pic) ;
}
inline void  FCImage::operator= (const FCDibEffect & pic) {
	FCDib::operator = (*(FCDib *)&pic) ;
}
inline void  FCImage::operator= (const FCDib & pic) {
	FCDib::operator = (*(FCDib *)&pic) ;
}
inline void  FCImage::__fooGifDecode (BYTE * InBuffer) {
	FCLzw	lzw ;
	lzw.LZW_GIF_Decode (InBuffer, this->GetBits (0), this->Width(), this->Height(), (m_pGifInfo->byInterlace != 0)) ;
}
inline DWORD  FCImage::__fooGifEncode (BYTE * OutBuffer) {
	FCLzw	lzw ;
	return lzw.LZW_GIF_Encode (this->GetBits (0), OutBuffer, this->Width(), this->Height(), this->ColorBits()) ;
}
inline void  FCImage::GetGifInfo (GIFINFO * gifinfo) {
	if ((gifinfo != NULL) && (m_pGifInfo != NULL))
		CopyMemory (gifinfo, m_pGifInfo, sizeof(GIFINFO)) ;
}
inline void  FCImage::__fooGifInitInfo (int iFileSize) {
	if (m_pGifInfo == NULL) // 第一次读取GIF
	{
		m_pGifInfo = new GIFINFO ;
		//	分配帧索引, 最多支持2048帧图 (StdDefine.h)
		m_pGifInfo->pFrameIndexArray = new DWORD [GIF_MAX_FRAME] ;
	}
	else
		delete[] m_pGifInfo->pStart ;
	m_pGifInfo->pStart = new BYTE [iFileSize] ;
	ZeroMemory (m_pGifInfo, 16) ;
	ZeroMemory (m_pGifInfo->pFrameIndexArray, sizeof(DWORD) * GIF_MAX_FRAME) ;
}
inline void  FCImage::__fooGifFreeInfo () {
	if (m_pGifInfo != NULL) // 已经为读取GIF分配了m_pGifInfo
	{
		delete[] m_pGifInfo->pFrameIndexArray ;
		delete[] m_pGifInfo->pStart ;
		delete m_pGifInfo ;
		m_pGifInfo = NULL ;
	}
}
inline BYTE *  FCImage::__fooImageReadFile (PCTSTR szFileName, HANDLE * hFile, HANDLE * hMap) {
	*hFile = ::CreateFile (szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL) ;
	if (*hFile == INVALID_HANDLE_VALUE)
		return NULL ;
	*hMap = ::CreateFileMapping (*hFile, NULL, PAGE_READONLY, 0, 0, NULL) ;
	if (*hMap == NULL)
		return NULL ;
	return (BYTE *) ::MapViewOfFile (*hMap, FILE_MAP_READ, 0, 0, 0) ;
}
inline BYTE *  FCImage::__fooImageSaveFile (PCTSTR szFileName, HANDLE * hFile, HANDLE * hMap, int iMaxSize) {
	*hFile = ::CreateFile (szFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL) ;
	if (*hFile == INVALID_HANDLE_VALUE)
		return NULL ;
	*hMap = ::CreateFileMapping (*hFile, NULL, PAGE_READWRITE, 0, iMaxSize, NULL) ;
	if (*hMap == NULL)
		return NULL ;
	return (BYTE *) ::MapViewOfFile (*hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0) ;
}
inline BYTE *  FCImage::__fooGifOpenFileForAddFrame (PCTSTR szFileName, HANDLE * hFile, HANDLE * hMap, int iMaxSize) {
	//	用在AddGifFrame函数中
	*hFile = ::CreateFile (szFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL) ;
	if (*hFile == INVALID_HANDLE_VALUE)
		return NULL ;
	*hMap = ::CreateFileMapping (*hFile, NULL, PAGE_READWRITE, 0, iMaxSize, NULL) ;
	if (*hMap == NULL)
		return NULL ;
	return (BYTE *) ::MapViewOfFile (*hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0) ;
}
inline void  FCImage::__fooImageUnmapFile (BYTE * pBase, HANDLE hMap, HANDLE hFile) {
	if (pBase != NULL)	::UnmapViewOfFile (pBase) ;
	if (hMap != NULL)	::CloseHandle (hMap) ;
	if (hFile != INVALID_HANDLE_VALUE)	::CloseHandle (hFile) ;
}

#endif