#ifndef __ZPYEXT_H__
#define __ZPYEXT_H__

// --------------------------------------------------------------------------
//	EXPORT DEFINE
#ifdef ZPYEXT_EXPORTS
#define ZPYEXT_API __declspec(dllexport)
#else
#define ZPYEXT_API __declspec(dllimport)
#endif
// --------------------------------------------------------------------------


#if !defined( NO_AUTO_LINK )



#include "ClButton.h"
#include "MaskEdit.h"
#include "MaskRichEdit.h"
#include "BmpGen.h"
#include "SortedHeaderCtrl.h"
#include "SortedListCtrl.h"
#include "ItemInfo.h"
#include "flatButton.h"
#include "Label.h"
#include "ColorEdit.h"
#include "GridCtrl.h"

// --------------------------------------------------------------------------
//	AUTO_LINK DEFINE
#ifdef _DEBUG
     // Link with the ANSI-Debug build of the library
     #pragma message( "Linking with ZpyExt_d.lib" )
     #pragma comment( lib, "ZpyExt_d.lib" )
#else
     // Link with the ANSI-Release build of the library
     //#pragma message( "Linking with zgdi.lib" )
     #pragma comment( lib, "ZpyExt.lib" )
#endif //!_DEBUG

#endif //!AUTO_LINK
// --------------------------------------------------------------------------


#endif //__ZPYEXT_H__