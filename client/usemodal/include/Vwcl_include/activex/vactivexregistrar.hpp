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

#ifndef VACTIVEXREGISTRAR
#define VACTIVEXREGISTRAR

#include "../vstandard.h"
#include "../strings/vpathstring.hpp"
#include "../strings/vstring.hpp"
#include "../utility/vregistry.hpp"

/** VActiveXRegistrar is used when installing or un-installing
ActiveX component into or from the system registry.*/
class VActiveXRegistrar
{
public:
	/** Register an ActiveX Server. Only pszCLSID and pszClassName are
	required, others are optional.*/
	static VBOOL	RegisterServer(	VSTRING_CONST	pszCLSID,
									VSTRING_CONST	pszClassName,
									VSTRING_CONST	pszProgID =
														NULL,
									VSTRING_CONST	pszProgIndependantID =
														NULL,
									VBOOL			bInProcess =
														VTRUE,
									VSTRING_CONST	pszThreadingModel =
														NULL,
									HINSTANCE		hInstance =
														NULL)
	{
		VASSERT(VSTRLEN_CHECK(pszCLSID))
		VASSERT(VSTRLEN_CHECK(pszClassName))

		if ( !pszCLSID || !pszClassName )
			return VFALSE;

		VBOOL bResult = VTRUE;

		/* Default threading model?*/
		if ( !pszThreadingModel )
			pszThreadingModel = VTEXT("Apartment");

		/* Create programmatic ID's first if specified.*/
		if ( pszProgID && pszProgIndependantID )
		{
			VSTRING_CONST pszKeys[] = {pszProgID, pszProgIndependantID};

			for ( VUINT i = 0; i < VARRAY_SIZE(pszKeys); i++ )
			{
				VRegistry reg;

				if ( reg.CreateKey(pszKeys[i], HKEY_CLASSES_ROOT, KEY_WRITE) )
				{
					/* Save the class name, CLSID key, and current
					version values.*/
					if (	!reg.WriteString(	(VSTRING_CONST)NULL,
												pszClassName) ||
							!reg.WriteSubKey(VTEXT("CLSID"), pszCLSID) ||
							!reg.WriteSubKey(VTEXT("CurVer"), pszProgID) )
					{
						bResult = VFALSE;
					}
				}
				else
					bResult = VFALSE;

				/* No use in continuing now?*/
				if ( !bResult )
					break;
			}
		}

		if ( bResult )
		{
			VRegistry reg;

			/* Build relative path to HKEY_CLASSES_ROOT\CLSID.*/
			VString s(VTEXT("CLSID\\"));
			s += pszCLSID;

			/* Create CLSID key.*/
			if (	!s.GetErrorCount() &&
					reg.CreateKey(s, HKEY_CLASSES_ROOT, KEY_WRITE) )
			{
				/* Save the class name.*/
				reg.WriteString((VSTRING_CONST)NULL, pszClassName);

				/* Get the path to this module.*/
				VPathString ps(s);
				ps.LoadModulePath(VFALSE, VFALSE, hInstance);

				VRegistry regSub;

				if ( bInProcess && !s.GetErrorCount() )
				{
					/* Create the InprocServer32 key.*/
					if ( regSub.CreateKey(	VTEXT("InprocServer32"),
											reg.GetKey(), KEY_WRITE) )
					{
						/* Save the path and threading model.*/
						if (	!regSub.WriteString((VSTRING_CONST)NULL, s) ||
								!regSub.WriteString(VTEXT("ThreadingModel"),
								pszThreadingModel) )
							bResult = VFALSE;

						regSub.Close();
					}
				}

				if ( bResult )
				{
					/* Create the ProgID key.*/
					if ( pszProgIndependantID || pszProgID )
					{
						if ( regSub.CreateKey(	VTEXT("ProgID"),
												reg.GetKey(),
												KEY_WRITE) )
						{
							if ( !regSub.WriteString(
								(VSTRING_CONST)NULL,
								(pszProgID)
								? pszProgID
								: pszProgIndependantID) )
							{
								bResult = VFALSE;
							}

							regSub.Close();
						}
						else
							bResult = VFALSE;
					}
				}

				if ( bResult )
				{
					/* Create the Programmable key.*/
					if ( regSub.CreateKey(	VTEXT("Programmable"),
											reg.GetKey(),
											KEY_WRITE) )
						regSub.Close();
					else
						bResult = VFALSE;
				}

				if ( bResult )
				{
					/* Create the version independant prog ID.*/
					if ( pszProgIndependantID )
					{
						if ( regSub.CreateKey(
							VTEXT("VersionIndependentProgID"),
							reg.GetKey(),
							KEY_WRITE) )
						{
							if ( !regSub.WriteString(	(VSTRING_CONST)NULL,
														pszProgIndependantID) )
								bResult = VFALSE;

							regSub.Close();
						}
						else
							bResult = VFALSE;
					}
				}
			}
			else
				bResult = VFALSE;
		}

		return bResult;
	}

	/** Un-Register an ActiveX Server.*/
	static void		UnRegisterServer(	VSTRING_CONST pszCLSID,
										VSTRING_CONST pszProgID,
										VSTRING_CONST pszProgIndependantID)
	{
		/* Delete the CLSID?*/
		if ( pszCLSID )
		{
			VString s(VTEXT("CLSID\\"));
			s += pszCLSID;

			if ( !s.GetErrorCount() )
				VRegistry::DeleteKey(HKEY_CLASSES_ROOT, s);
		}

		if ( pszProgIndependantID )
			VRegistry::DeleteKey(HKEY_CLASSES_ROOT, pszProgIndependantID);

		if ( pszProgID )
			VRegistry::DeleteKey(HKEY_CLASSES_ROOT, pszProgID);
	}
};

#endif /* VACTIVEXREGISTRAR*/
