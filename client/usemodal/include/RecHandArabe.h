#ifndef __RECHANDARABE_H__
#define __RECHANDARABE_H__


#define SOURCE_LIMITED_X_OF_CHECK_DIGIT_AREA					900
#define SOURCE_LIMITED_Y_OF_CHECK_DIGIT_AREA					500

// ****************************************************************
// Class <CZil_SegmentOfCheckArabDigit> used to Segment the Arab
//		digit of check.
//		INPUT: 
//				1.Mil System Identifier pointer
//				2.Source Image(RGB) buffer pointer
//				3.Source Image size(X and Y)
//				4.Grid size(X and Y)
//		OUTPUT:
//				1.Valid digit number
//				2.Each digit image size(X and Y array)
//				3.Each digit image buffer
// ****************************************************************

#ifdef RECHANDARABE_EXPORTS
#define RECHANDARABE_API __declspec(dllexport)
#else
#define RECHANDARABE_API __declspec(dllimport)
#endif

#pragma warning( disable : 4251 )

#include "zilNeural.h"

#include <string>
using namespace std;

// This class is exported from the RecHandArabe.dll
class RECHANDARABE_API CRecHandArabe 
{
public:
	// TODO: add your methods here.
	CRecHandArabe();
	virtual ~CRecHandArabe();

	int RecInit( string methode, void* pSys_ID = NULL );
	int RecCheckArabe( unsigned char* pImageBuf, int SizeX, int SizeY, char* Rlt );
	int DetectBox_Check( unsigned char *ImgErode, unsigned char *ImgNorm, 
							  int SizeX, int SizeY, int *Nb_Seri, int *Coor );

private:

	unsigned char * pSourceImage;
	int Image_X, Image_Y;
	int Mot_X, Mot_Y;

	// Operation:
	int Zil_DeleteSmallArea( MIL_ID *pSystemID, MIL_ID *BinImage, int Size_Area );
	int DetectBox_Check_( unsigned char *ImgErode, unsigned char *ImgNorm, 
							  int SizeX, int SizeY, int *Nb_Seri, int *Coor );
	int DetectBox_Cridit( unsigned char *A, unsigned char *ImgNorm, 
							  int SizeX, int SizeY, int *Nb_Seri, int *Coor );
	int DetectBox_Envoye( unsigned char *A, unsigned char *ImgNorm, 
							  int SizeX, int SizeY, int *Nb_Seri, int *Coor );
	int DetectBox_TelLet( unsigned char *A, unsigned char *ImgNorm, 
							  int SizeX, int SizeY, int *Nb_Seri, int *Coor );
	int Hand_Segment( unsigned char* pImg, int SizeX, int SizeY, 
		int* Nb_Seri, int* Seri_Size, unsigned int* Seri_Char );

	int DetectBox_Check_NongHang( unsigned char *ImgOrg, 
		int SizeX, int SizeY, int *Nb_Seri, int *Coor );


	string Methode;
	CZilNeural Neural;
	MIL_ID* pSystemID;
	CZil_HandArabNum_Identification Id;

};

extern RECHANDARABE_API int nRecHandArabe;

RECHANDARABE_API int fnRecHandArabe(void);

#endif
