#ifndef SCANNERINI_H_INCLUDED_C52C75FE
#define SCANNERINI_H_INCLUDED_C52C75FE
#include <stdio.h>

#include <list>
using namespace std ;

struct vch_type_t
{
    int   sid;
    char  vchname[100];
    int   format;
    int   rotate;
    int   sideA;
    int   sideB;
    int   procA;
    int   procB;
    int   dpi ;
    float vch_sizex;
    float vch_sizey;
    int   colortype;
    int   scantype;
    char  reserved[100];
};

struct scanner_info_t
{
    char  scanner_name[50];
    char  prefix[5];
    unsigned long   scanner_sn;
    float calx;
    float caly;
    int   offset;
    char  path[100];
    int   pkgflag;
    int   vchtcount;
    vch_type_t onevch;
    int   needRotate;
    char  reserved[100];
};

struct image_info_t
{
    char  fname[100];
    char  bname[100];
    int   dpi;
    int   rotate;
    int   sideA;
    int   sideB;
    int   procA;
    int   procB;
    int   fcount;
    char  ip[24];
    char  prefix[4];
    char  date[12];
    char  session[4];
    char  pkgno[12];
    char  reserved[100];
};
typedef list<image_info_t> IMAGELIST;
typedef list<vch_type_t> VCHTYPELIST;

class VchList
{
public:
	VCHTYPELIST myList;
};






//##ModelId=3AD14CCD0078
class ScannerIni
{
public:
	//##ModelId=3AF7B650006A
/*	char ScannerSN[10];

	//##ModelId=3AF7B6500074
	float calX;

	//##ModelId=3AF7B650007E
	float calY;
	//##ModelId=3AF7B68A0059
	char SavePath[128];

    
    int  dpi;
    bool formatflag;

    bool bAutoDel;
    bool bUseNetNo;
    bool bUseFilePipe;


    int ScannerType;
 */
    char m_iniPath[128];
    scanner_info_t  m_scannerInfo;
    vch_type_t  m_vchType;
    VchList    *pList;
    ScannerIni();
    ~ScannerIni();
    int m_Date;

	//##ModelId=3AD14CDC0066
	int LoadIniData(char *filename);
	//##ModelId=3AF7B6DD006D
	int SaveIniData();
    FILE* fp;
 private:
    char _iniFile[128];
    


};



#endif /* SCANNERINI_H_INCLUDED_C52C75FE */
