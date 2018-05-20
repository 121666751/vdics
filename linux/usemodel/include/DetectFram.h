// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DETECTFRAM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DETECTFRAM_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifndef __DETECTFRAM_
#define __DETECTFRAM_

#ifdef DETECTFRAM_EXPORTS
#define DETECTFRAM_API __declspec(dllexport)
#else
#define DETECTFRAM_API __declspec(dllimport)
#endif

#include "zilNeural.h"

struct DetectHandle
{
	CZilNeural Neural;
	MIL_ID* pSystemID;
	CZil_HandArabNum_Identification Id;
};

// This class is exported from the DetectFram.dll
class DETECTFRAM_API CDetectFram 
{
public:

#define Pi				3.1415926
#define InterA			20
#define NFoisA			4 
#define LargA			(InterA*NFoisA)
#define DepA			((90-InterA/2)*NFoisA)
#define Sp				3000
#define pDplac			1100

	CDetectFram( /*void* pmil*/ );
	~CDetectFram();
	// TODO: add your methods here.

//	int DetectFram( char* fname );
	int DetectFram( unsigned char* A, int SizeX, int SizeY, int Band, int &x0, int &y0/*, MIL_ID* idsys*/ );
	int OpenAcc( void* pMilSysId, unsigned char* A, int SizeX, int SizeY, char* ResStr );

	int ZMinshengConsign__( void* pMilSystem, 
				unsigned char* ImgOrg, int SizeX200, int SizeY200, char* Id, char* Money );
	int ZNongHangConsign__( void* pMilSystem, 
				unsigned char* ImgOrg, int SizeX200, int SizeY200, char* Id, char* Money );

private:
	int ix0, ix1, iy0, iy1, moy, Dpi;
	double dCos[LargA], dSin[LargA], mAlpha;
	unsigned char* ImgOrg3c, *ImgGray, *Img3c_r, *ImgGray_r;
	int GrayMoy, Seuil;
	DetectHandle* han;
	int openacc;
	char _reserved[92]; // not used;

	void TransRgb2Gray( unsigned char* ImgSrc, unsigned char* ImgDst, int SizeX, int SizeY );
	void AnnBox_Pour_Simple( unsigned char* A, int SizeX, int SizeY );
	int DetectFram_Simple( unsigned char* ImgGray, int SizeX, int SizeY );
	void Binarize( unsigned char* A, int SizeX, int SizeY, int Seuil );
	void ZilErode2( unsigned char* ImgSrc, unsigned char* ImgDes, int SizeX, int SizeY, int dlt );

	void Resize(int SizeX, int SizeY);
	int DetectAlpha( unsigned char* A, int Sizex, int Sizey, int iSeuil );
	void Rotate_BiLinear( unsigned char* A, unsigned char* B, int Sizex, int Sizey, double theta );
//	void AnnBox_Pour_2( unsigned char* A, int SizeX, int SizeY );
	int DetectCode( unsigned char* A, int SizeX, int SizeY, char* ResStr );

//	CZilNeural Neural;
//	MIL_ID* pSystemID;
//	CZil_HandArabNum_Identification Id;

	int RecInit();
	int Zil_DeleteSmallArea( unsigned char *Img, int SizeX, int SizeY, int Size_Area );

	void Binarize_id( unsigned char* Img, int SizeX, int SizeY );
	int Rec_Id( unsigned char* Img, int SizeX, int SizeY, char* Rlt );
	int Rec_Money( unsigned char* Img, int SizeX, int SizeY, char* Rlt );
};

//extern DETECTFRAM_API int nDetectFram;
//DETECTFRAM_API int fnDetectFram(void);

#endif


