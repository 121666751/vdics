#ifndef _RECNUM_
#define _RECNUM_

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RECNUM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RECNUM_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifdef RECNUM_EXPORTS
#define RECNUM_API __declspec(dllexport)
#else
#define RECNUM_API __declspec(dllimport)
#endif

// This class is exported from the RecNum.dll
class RECNUM_API CRecNum {
public:

	CRecNum(void);
	virtual ~CRecNum(); 

	int InitModel( char* fName, int nbmodel );
	// return -1 : 模板不存在; 0 : Ok
	// 模板名字 Model10.bmp, Model5.bmp
	//int Rec1Number( MIL_ID* MilImage, int Size_X, int Size_Y, int nbAlgo = 3 );

	int Rec1Number( unsigned char* MilImage, int SizeX, int SizeY, int nbAlgo );
	// nbAlgo : 1 : 一种算法(图像清晰); 3 : 三种算法(图像模糊)
	// return -1 : 识别错误; >= 0 : Ok

//	int RecAChar( char* fName, int nbmodel, unsigned char* A );
	int RecAChar( char* fName, int nbmodel, unsigned char* A, int SizeX, int SizeY );


private:

	int Size_Norm_X, Size_Norm_Y, Size_Feature, NumModel;
	unsigned char* Model2D;
	double* ModelGrille, *ModelMorph;
	int Size_Max_X, Size_Max_Y;

	void Zil_ProbCorrelation2D( unsigned char *OneObjet, unsigned char * Model, 
		int SizeObjet, double* p1, double* p2, int* k1, int* k2 );
	void Zil_ProbCorrelation1D( double *OneObjet, double* Model,
			int SizeObjet, int SizeModel, double* p1, double* p2, int* k1, int* k2 );

	void ExtraGrille(unsigned char *Image, int Nx, int Ny, double *Feature );
	void DetectBox( unsigned char* Image, int Nx, int Ny, 
		int* x0, int* x1, int* y0, int* y1);

	void PositionLeftAndBreadth( unsigned char* Image, int Nx, int Ny, double * Feature );
	void Project0( unsigned char* Image, int Nx, int Ny, double * Feature );

	void Project00( unsigned char* Image, int Nx, int Ny, int * Feature );

	void Project90( unsigned char* Image, int Nx, int Ny, double * Feature );
	void NumHole0( unsigned char* Image, int Nx, int Ny, double * Feature );
	void NumHole90( unsigned char* Image, int Nx, int Ny, double * Feature );
	void NumHole45( unsigned char* Image, int Nx, int Ny, double * Feature );
	void NumHole135( unsigned char* Image, int Nx, int Ny, double * Feature );
	void ExtraMorph( unsigned char *Image,int Nnx, int Nny, double *Feature );

	int Resize( unsigned char* A, int SizeXa, int SizeYa, 
			unsigned char* B, int SizeXb, int SizeYb, int Band );

};

extern RECNUM_API int nRecNum;

RECNUM_API int fnRecNum(void);

#endif