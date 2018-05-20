#ifndef __ZIL_E13B_API_
#define __ZIL_E13B_API_

#ifdef ZIL_E13B_EXPORTS
#define ZIL_E13B_API __declspec(dllexport)
#else
#define ZIL_E13B_API __declspec(dllimport)
#endif

class ZIL_E13B_API CZil_E13b 
{
public:

	CZil_E13b();
	~CZil_E13b();

	int LoadModel(int Image_X, int Image_Y, int Dpi = 200 );
	int GetResultE13b( unsigned char* ImgOrg, char *Liste_result, int* N );
	void PrinterResult( int n, char *Liste_result );

private :

#define		Size_MotE13b_X			28
#define		Size_MotE13b_Y			36 
#define		Size_MotE13b_Feature	100
#define		N_E13b_Model			14 
#define		Name_File_Model			".\\config\\E13b14Norm28_36_100.zpy"

	bool bHaveModel;
	int Size_Image_X, Size_Image_Y, Size_Image_Org_X, Size_Image_Org_Y, *ProjImage;
	float iDpi;

	unsigned char *Image, *Model, *Feature, *lImage;

	int ProbCorrelation(void);
	int SegmentationAChar( int *X );
	void Extract_Feature(void);

	int Resize( unsigned char* A, int SizeXa, int SizeYa, 
				unsigned char* B, int SizeXb, int SizeYb, int Band );
	void Binarize( unsigned char* A, int SizeX, int SizeY );
};

extern ZIL_E13B_API int nZil_E13b;
ZIL_E13B_API int fnZil_E13b(void);

#endif