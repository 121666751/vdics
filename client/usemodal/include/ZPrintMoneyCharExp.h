#ifndef __ZPRINTMONEYCHAREXP_H__
#define __ZPRINTMONEYCHAREXP_H__

class CZPrintMoneyChar;

extern "C" CZPrintMoneyChar* ZPrintMoneyChar_Load( void* pMilSystem );

extern "C" int ZPrintMoneyChar_GetMoney( CZPrintMoneyChar* Cp, unsigned char* Img, 
										int sizex, int sizey, char* dMoney );

extern "C" int ZPrintMoneyChar_GetArabe( CZPrintMoneyChar* Cp, unsigned char* Img, 
										int sizex, int sizey, char* cArabe );

extern "C" int ZPrintMoneyChar_GetDate( CZPrintMoneyChar* Cp, unsigned char* Img, 
										int sizex, int sizey, char* cDate );

extern "C" void ZPrintMoneyChar_Free( CZPrintMoneyChar* Cp );


#endif //__ZPRINTMONEYCHAREXP_H__