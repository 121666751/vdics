#ifndef __ZILNEURAL_H__
#define __ZILNEURAL_H__

#ifdef ZILNEURAL_EXPORTS
#define ZILNEURAL_API __declspec(dllexport)
#else
#define ZILNEURAL_API __declspec(dllimport)
#endif

#include <mil.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <memory.h>


//#define HANDWRITING_ARAB_NUM_FEATURE			159		//Handwriting arab number features
//#define DEFAULT_HAND_ARAB_NUM_ERROR			0.65	// Default handwriting number error
#define NORMALIZED_HAND_NUM_IMAGE_SIZE_X		15
#define NORMALIZED_HAND_NUM_IMAGE_SIZE_Y		25


typedef struct __BackPropagationNet
{
	int iNumBeginUnit;
	int iNumHiddenUnit;
	int iNumEndUnit;

	float * W;			// need memory size = iNumHiddenUnit * iNumBeginUnit
	float * Seta;		// need memory size = iNumHiddenUnit
	float * V;			// need memory size = iNumHiddenUnit * iNumEndUnit
	float * Gama;		// need memory size = iNumEndUnit
} BackPropagationNet, *LPBackPropagationNet;


// ***************************************************************
// Class <CZil_BackPropagationNetParam> used to Initialize the 
//		Error Back-Propagation Neural Network Parameters.
//		MUST be declared ONCE at the beginning of program
// ***************************************************************
class ZILNEURAL_API CZilNeural
{
//Atributes
public:
	bool bHaveParam;	// bool sign used to check whether the BP-Net Param have been read
	int iNumBeginUnit;
	int iNumHiddenUnit;
	int iNumEndUnit;

	float * W;			// need memory size = iNumHiddenUnit * iNumBeginUnit
	float * Seta;		// need memory size = iNumHiddenUnit
	float * V;			// need memory size = iNumHiddenUnit * iNumEndUnit
	float * Gama;		// need memory size = iNumEndUnit
private:
	char *NetParamFile;

//Operation
public:
	CZilNeural();
	~CZilNeural();
	int NeuralInit( int N, int P, int Q, char *Name_File_Param );

private:

};


// ****************************************************************************
// CLASS <CZil_BPNetMatch> used to identify the handwriting Arab number charactor.
//		this class should use a `CBackPorpagationNetParam` object
//		to obtain the BP-Network parameters
// ****************************************************************************
class ZILNEURAL_API CZil_BPNetMatch
{
public:

	CZil_BPNetMatch(CZilNeural * pBPNetParam, double error)

		: N(pBPNetParam->iNumBeginUnit), P(pBPNetParam->iNumHiddenUnit),
		  Q(pBPNetParam->iNumEndUnit), err(error),
		  W(pBPNetParam->W), Seta(pBPNetParam->Seta),
		  V(pBPNetParam->V), Gama(pBPNetParam->Gama) {}

	virtual ~CZil_BPNetMatch() {}

	void MatchFromCBPNet(double *Feature, int *Digit);
	int MatchFromCBPNet(double *Feature);

private:

	int N, P, Q;
	double err;
	float *W, *Seta, *V, *Gama;
	double *A;
	void OneLearn( double *Ct );
};


// ***********************************************************************
// CLASS <CZil_HwArabFeatureHist> used to abstract handwriting Arab number 
//		charators' feature list.
//		This class will do this by THREE steps
//		1. Nomalize the size of image
//		2. Make the feature list
//		3. Compress the feature list to 103
//
//		The result will be stored in a linear array
// ***********************************************************************
class ZILNEURAL_API CZil_HwArabFeatureHist  
{
public:

	CZil_HwArabFeatureHist(MIL_ID * pSysId) : pSystemId(pSysId) {}
	virtual ~CZil_HwArabFeatureHist() {}

	void ExtraFeature( unsigned char *Image, int Nx, int Ny, double *fFeature );

private:
	MIL_ID * pSystemId;
	void Normalize( unsigned char* cFeature, double *fFeature );
	void PositionLeftAndBreadth( unsigned char* Image, unsigned char* cFeature );
	void NumHole0( unsigned char* Image, unsigned char* cFeature );
	void NumHole90( unsigned char* Image, unsigned char* cFeature );
	void NumHole45( unsigned char* Image, unsigned char* cFeature );
	void NumHole135( unsigned char* Image, unsigned char* cFeature );
	int PreProcess( unsigned char *Image, int Nx, int Ny, unsigned char * cFeature );
};

// *********************************************************************************
// CLASS <CZil_HandArabNum_Identification> used to identify the handwriting Arab 
//		number.
//		Befor use this class, you should do the following first:
//		1. Declare an object of `CZil_BackPropagationNetParam`
//		2. Prepare an image array to identify
//		3. Make sure the X size and Y size of this image
//		4. Make sure the accept level(reject error)
//
// *********************************************************************************
class ZILNEURAL_API CZil_HandArabNum_Identification
{
public:

	CZil_HandArabNum_Identification(){};
/*MIL_ID * pSysId, unsigned char* image, int Nx, int Ny,
		CZilNeural * pBPNetParam, double error=0.93  DEFAULT_HAND_ARAB_NUM_ERROR){};

//		:	pSystemId(pSysId), ptagImage(image), tagNx(Nx), tagNy(Ny), 
//			tagNnx(NORMALIZED_HAND_NUM_IMAGE_SIZE_X), tagNny(NORMALIZED_HAND_NUM_IMAGE_SIZE_Y),
//			ptagBPNetParam(pBPNetParam), tagError(error){
//			
//			tagFeature = new double[pBPNetParam->iNumBeginUnit];
//		}
*/
	~CZil_HandArabNum_Identification(){ delete[] tagFeature; };

	void IDInit( CZilNeural * pBPNetParam, void * pSysId = NULL, double error=0.05 )
	{
//		tagNnx = NORMALIZED_HAND_NUM_IMAGE_SIZE_X;
//		tagNny = NORMALIZED_HAND_NUM_IMAGE_SIZE_Y;
		pSystemId = (MIL_ID*)pSysId;
		ptagBPNetParam = pBPNetParam;
		tagError = error;
		tagFeature = new double[pBPNetParam->iNumBeginUnit];
	};

//	void IdentifyResult( int * iDigit );
	int IdentifyResult( unsigned char* image, int Nx, int Ny );

private:
	MIL_ID * pSystemId;
	unsigned char * ptagImage;
	int tagNx, tagNy, tagNnx, tagNny;
	CZilNeural * ptagBPNetParam;
	double tagError;
	double * tagFeature/*[HANDWRITING_ARAB_NUM_FEATURE]*/;
};

/*
// This class is exported from the ZilNeural.dll
class ZILNEURAL_API CZilNeural {
public:
	CZilNeural(void);
	// TODO: add your methods here.
};
*/
extern ZILNEURAL_API int nZilNeural;

ZILNEURAL_API int fnZilNeural(void);

#endif