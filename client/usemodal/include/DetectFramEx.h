#ifndef __DETECTFRAMEX_H__
#define __DETECTFRAMEX_H__

class CDetectFram;

extern "C" int get_sub_buffer( unsigned char* A, int SizeX, int SizeY, int Band,
				  int offx, int offy, int sizex, int sizey, unsigned char* B );

extern "C" CDetectFram* ZMinshengConsign_Init();

extern "C" int ZMinshengConsign( CDetectFram* Cp,  void* pMilSystem, 
				unsigned char* Img, int SizeX, int SizeY, char* Id, char* Money );

extern "C" void ZMinshengConsign_Free( CDetectFram* Cp );

#endif //__DETECTFRAMEX_H__