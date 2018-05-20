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

#include "../windows/vwindow.hpp"

/* Pointer to function in COMCTL32.DLL. VWCL attempts to initialize common
controls only as needed. If the common control library exports the
InitCommonControlsEx() function, this is very efficient. Otherwise VWCL
will use the older InitCommonControls() to initialize all controls.*/
typedef VWINBOOL (WINAPI* VINITCOMMONCONTROLSEX)
	(INITCOMMONCONTROLSEX* pInitCtrls);

/** Local global that tells us what controls have been initialied.*/
VUINT gnControlsInitialized = 0;

/** Global function to initialize a single common control as needed.*/
void VInitCommonControl(VRTTI_TYPE nCommonControlType)
{
	/* Anything to do? If gnControlsInitialized == 0xffffffff
	then all control have been initialized.*/
	if ( gnControlsInitialized != 0xffffffff )
	{
		/* Get the bitmask for the control in question.*/
		VUINT nBits = 0;

		switch ( nCommonControlType )
		{
			case VWindow::VWCL_RTTI_HEADER:
			case VWindow::VWCL_RTTI_LISTVIEW:
				nBits = ICC_LISTVIEW_CLASSES;
				break;

			case VWindow::VWCL_RTTI_TOOLBAR:
			case VWindow::VWCL_RTTI_STATUSBAR:
			case VWindow::VWCL_RTTI_TOOLTIP:
				nBits = ICC_BAR_CLASSES;
				break;

			case VWindow::VWCL_RTTI_TAB:
			case VWindow::VWCL_RTTI_TAB_WINDOW:
				nBits = ICC_TAB_CLASSES;
				break;

			case VWindow::VWCL_RTTI_TREEVIEW:
			case VWindow::VWCL_RTTI_DIRECTORY_TREEVIEW:
			case VWindow::VWCL_RTTI_HIERARCHICAL_DATABASE_TREEVIEW:
				nBits = ICC_TREEVIEW_CLASSES;
				break;

			case VWindow::VWCL_RTTI_PROGRESS:
				nBits = ICC_PROGRESS_CLASS;
				break;

			case VWindow::VWCL_RTTI_REBAR:
				nBits = ICC_COOL_CLASSES;
				break;

			#ifdef VWCL_DEBUG
				/* An unknown or unsupported common control type.*/
				default:
					VASSERT(VFALSE)
			#endif
		}

		/* Have we already initialized a control of this type? If yes, skip.*/
		if ( nBits && !(gnControlsInitialized & nBits) )
		{
			HINSTANCE hLib = GetModuleHandle(VTEXT("COMCTL32.DLL"));
			VASSERT(hLib)

			if ( hLib )
			{
				/* See if we find a InitCommonControlsEx() function in the
				COMCTL32.DLL library.*/
				VINITCOMMONCONTROLSEX pfn = (VINITCOMMONCONTROLSEX)
					GetProcAddress(	hLib,
									"InitCommonControlsEx");

				if ( pfn )
				{
					/* Populate INITCOMMONCONTROLSEX struct.*/
					INITCOMMONCONTROLSEX icc =
						{	sizeof(INITCOMMONCONTROLSEX),
							nBits
						};

					/* Make call and update the controls initialized bits.*/
					if ( pfn(&icc) )
						gnControlsInitialized |= nBits;

					return;
				}
			}

			/* Revert to old InitCommonControls() function and
			populate gnControlsInitialized with all bits.*/
			InitCommonControls();
			gnControlsInitialized = 0xffffffff;
		}
	}
}
