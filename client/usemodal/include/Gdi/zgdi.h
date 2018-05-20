#ifndef _ZGDI_H
#define _ZGDI_H

#include "dib.h"

#ifdef _DEBUG
     // Link with the ANSI-Debug build of the library
     #pragma message( "Linking with zgdi_d.lib" )
     #pragma comment( lib, "zgdi_d.lib" )
#else
     // Link with the ANSI-Release build of the library
     //#pragma message( "Linking with zgdi.lib" )
     #pragma comment( lib, "zgdi.lib" )
#endif //!_DEBUG


#endif //!_ZGDI_H
