/* VWCL - The Virtual Windows Class Library.
Copyright (C) 1996-2000 The VWCL Alliance

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.

Primary Author of this source code file:  Todd Osborne (todd@vwcl.org)
Other Author(s) of this source code file:
*/

#ifndef VCTL3DLIBRARY
#define VCTL3DLIBRARY

#include "../vstandard.h"
#include "vlibrary.hpp"

/** CTL3D runtime pointers to function prototypes.*/
typedef VWINBOOL (WINAPI* VCTL3D)(HANDLE);
typedef VWINBOOL (WINAPI* VCTL3D2)(HANDLE, WORD);

/** VCtl3DLibrary enables the dynamic use of the Microsoft 3-D Controls
Library. Simply creating an object os this type will start CTL3Dxx.DLL and
subclass all dialog boxes to use 3-D controls. Create will fail if NO3D is
specified on the command line. Objects of this class should be kept in
scope throughout the running of your application, or until you no longer
want 3D dialog support. This support is not needed on Windows NT 4.x or
higher, or Windows 95 or higher.*/
class VCtl3DLibrary : public VLibrary
{
public:
	/** Default constructor initializes the CTL3D library as needed.
	Skips if NO3D is specified on the application command line.*/
	VCtl3DLibrary()
	{
		/* VWCL must be initialized first!*/
		VASSERT(VGetInstanceHandle())

		/* Check for NO3D on command line.*/
		if ( !VGetApp().IsCommandLineOption(VTEXTA("NO3D")) )
		{
			/* Try to load the library.*/
			if ( Set(VTEXT("CTL3D32.DLL")) )
			{
				VCTL3D pReg =
					(VCTL3D)GetProcAddress(	m_hInstance,
											VTEXTA("Ctl3dRegister"));

				VCTL3D pSub =
					(VCTL3D)GetProcAddress(	m_hInstance,
											VTEXTA("Ctl3dAutoSubclass"));

				if ( pReg && pSub )
				{
					/* Register this app and subclass all dialogs.*/
					pReg(VGetInstanceHandle());
					pSub(VGetInstanceHandle());
				}
				else
					Free();
			}
		}
	}

	/** Virtual destructor unregisters CTL3D support as needed.*/
	virtual ~VCtl3DLibrary()
	{
		/* Un-register this app from CTL3Dxx.DLL?*/
		if ( m_hInstance )
		{
			VCTL3D pUnReg =
				(VCTL3D)GetProcAddress(m_hInstance, VTEXTA("Ctl3dUnregister"));

			if ( pUnReg )
				pUnReg(VGetInstanceHandle());
		}
	}
};

#endif /* VCTL3DLIBRARY*/
