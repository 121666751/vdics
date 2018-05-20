#ifndef __ZIMEX_H__
#define __ZIMEX_H__
#include <libmem.h>

extern "C" int RGB2JpegFile( unsigned char* pImgSrc, int nSizeXSrc, int nSizeYSrc,
					double scale, int quality, char* filename );

extern "C" void JpegFile2RGB( char* filenamesrc, int& SizeX, int& SizeY, MemT& data );

extern "C" int ResizeJpegFile2JpegFile( char* filenamesrc, char* filenamedst,
					double scale, int quality, int rotate );

#endif //__ZIMEX_H__



