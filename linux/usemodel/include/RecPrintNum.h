#ifndef __RECPRINTNUM_
#define __RECPRINTNUM_

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RECPRINTNUM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RECPRINTNUM_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifdef RECPRINTNUM_EXPORTS
#define RECPRINTNUM_API __declspec(dllexport)
#else
#define RECPRINTNUM_API __declspec(dllimport)
#endif


#include <RecNum.h>
#include <string>
using namespace std;

// This class is exported from the RecPrintNum.dll

class RECPRINTNUM_API CRecPrintNum 
{

public:

	CRecPrintNum(void);

	// TODO: add your methods here.
	virtual ~CRecPrintNum(); 
	int GetPrintNum( string Methode, unsigned char* pMilImg0, int SizeX, int SizeY, int nbCffr, char* Rlt );
	int GetPrintNum2d( string Methode, unsigned char* pMilImg0, int SizeX, int SizeY, int nbCffr, char* Rlt, void* pMilSystem );

	void Filtre( unsigned char* A, unsigned char* B, int SizeX, int SizeY, int Band );
	int Binarize_Hist_Bleu( unsigned char* A, int SizeX, int SizeY, int Band );
	int Binarize_Hist_Red( unsigned char* A, int SizeX, int SizeY, int Band );
	int Binarize_Hist_Red_Bleu( unsigned char* A, int SizeX, int SizeY, int Band );
	int Binarize_Hist_JiaoHang( unsigned char* A, int SizeX, int SizeY, int Band );
	int Binarize_NongHang( unsigned char* A, int SizeX, int SizeY, int Band, void* pMilSystem = NULL );

	int GetChar( string fName, int nbmodel, unsigned char* A, int SizeX, int SizeY );
	int DetectZoneChar( unsigned char* A, int SizeXa, int SizeYa, unsigned char* B, int SizeXb, int SizeYb );
	void ZilOpen2( unsigned char* ImgSrc, unsigned char* ImgDes, int SizeX, int SizeY, int dlt );

	void Binarize( string Methode, unsigned char* A, int SizeX, int SizeY, int Band, void* pMilSystem = NULL );

private:

	struct PARAMc
	{
		string Methode;
		string Width;
		string Height;
		string Space;
		string NumMCffre;
		string ModelFile;
		string RealNumCheck;
		string NumRCffre;
	};
	struct PARAMi
	{
		int Width;
		int Height;
		int Space;
		int NumMCffre;
		int NumRCffre;
	};
	enum ERR
	{
		ERR_XMLFILENOEXIST = -1,
		ERR_XMLNOLOAD = -2,
		ERR_NOTFIND = -3
	};

	PARAMi Parami;

	void ZilErode2( unsigned char* ImgSrc, unsigned char* ImgDes, int SizeX, int SizeY, int dlt );
	void ZilDilate2( unsigned char* ImgSrc, unsigned char* ImgDes, int SizeX, int SizeY, int dlt );
//	void ZilOpen2( unsigned char* ImgSrc, unsigned char* ImgDes, int SizeX, int SizeY, int dlt );

	int First_Position( unsigned char* A, int SizeX, int SizeY, int* X0, int* X1, int* Y0, int* Y1 );
	int Coor_Chaque_Char( unsigned char* A, int SizeX, int SizeY, int* Y0, int* Y1, int* Coor );
	int LoadParam( string XMLFile, string _Methode, PARAMc& Param );
	int LoadModel( string modelfile );
	void TransChar_Cffre( PARAMc& Paramc );

//	void Binarize( string Methode, unsigned char* A, int SizeX, int SizeY, int Band );
//	int Binarize_Hist_Red_Bleu( unsigned char* A, int SizeX, int SizeY, int Band );
//	int Binarize_Hist_Red( unsigned char* A, int SizeX, int SizeY, int Band );
//	int Binarize_Hist_Bleu( unsigned char* A, int SizeX, int SizeY, int Band );

	int GetNum( unsigned char* A, int SizeX, int SizeY, char* Rlt );
	int Detect_Zone_Position( unsigned char* A, 
		int SizeX, int SizeY, int* X0, int* X1, int* Y0, int* Y1 );

	int Size_Norm_X, Size_Norm_Y, SizeRealX, numChar, isLoadModel;
	unsigned char* Modelp;
	CRecNum pRecNum;

	int Zil_DeleteSmallArea( void* pmSystem, void* Image, int Size_Area );
	int GetNum2d( unsigned char* A, int SizeX, int SizeY, char* Rlt, void* iDSys );
	int ZProbCorrelation2D( unsigned char*OneObjet, int SizeX, int SizeY, 
		unsigned char* Model, int NumModel );

};

extern RECPRINTNUM_API int nRecPrintNum;
RECPRINTNUM_API int fnRecPrintNum(void);

#endif