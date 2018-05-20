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

#ifndef VAUTOMATIONOBJECT
#define VAUTOMATIONOBJECT

#include "../vstandard.h"
#include "../strings/vpathstring.hpp"
#include "../strings/vstring.hpp"
#include "../strings/vstringconverter.hpp"
#include "vunknown.hpp"

/** VAutomationObject is the base class for other classes that expose an
ActiveX (OLE) Automation interface through the IDispatch COM interface.
This class creates the basic IDispatch object for a class, and properly
handles QueryInterface().*/
class VAutomationObject : public VUnknown
{
public:
	/** Standard implementation of QueryInterface() returns object pointers
	for IID_IDispatch, the object specific IDispatch, and the type
	information interfaces. If another IID is requested, the function returns
	the base class QueryInterface() information.*/
	STDMETHODIMP				QueryInterface(	REFIID		riid,
												VVOIDPTR*	ppvObj)
	{
		if (	m_pDispatchInterace &&
				(riid == IID_IDispatch || riid == m_classDispatchIID) )
			return m_pDispatchInterace->QueryInterface(IID_IDispatch, ppvObj);

		if ( m_pTypeInfo && riid == IID_ITypeInfo )
			return m_pTypeInfo->QueryInterface(IID_ITypeInfo, ppvObj);

		return VUnknown::QueryInterface(riid, ppvObj);
	}

	/** Create a new VAutomationObject which must be initialized with the
	object Interface ID (IID) and Dispatch Interface (DIID). This version
	of Create() attempts to load the type library from application resources.
	On success, a VAutomationObject pointer will be returned, NULL on
	failure.*/
	static VAutomationObject*	Create(
		IID const&	classIID,
		IID const&	classDispatchIID,
		HINSTANCE	hTypeLibraryResourceInstance =
						NULL)
	{
		/* Get the module name of hTypeLibraryResourceInstance.*/
		VString s;
		VPathString ps(s);

		/* Create from file and return.*/
		if ( ps.LoadModulePath(VFALSE, VFALSE, hTypeLibraryResourceInstance) )
			return Create(classIID, classDispatchIID, s);

		return NULL;
	}

	/** Create a new VAutomationObject which must be initialized with the
	object Interface ID (IID) and Dispatch Interface (DIID). This version
	of Create() attempts to load the type library from a file. On success,
	a VAutomationObject pointer will be returned, NULL on failure.*/
	static VAutomationObject*	Create(	IID const&		classIID,
										IID const&		classDispatchIID,
										VSTRING_CONST	pszFileName)
	{
		VAutomationObject* pNew =
			VNEW VAutomationObject(classIID, classDispatchIID, pszFileName);

		VWCL_ACTIVEX_IMPLEMENT_STATIC_CREATE(pNew)
	}

	/** Return the internal IDispatch interface. This function will NOT
	AddRef() the interface.*/
	IDispatch*					GetDispatch() const
		{ return m_pDispatchInterace; }

	/** Return the internal IDispatch interface ID.*/
	IID							GetDispatchIID() const
		{ return m_classDispatchIID; }

	/** Return the internal ITypeInfo interface. This function will NOT
	AddRef() the interface.*/
	ITypeInfo*					GetTypeInfo() const
		{ return m_pTypeInfo; }

protected:
	/** Protected constructor called from static Create() functions.*/
	VAutomationObject(	IID const&		classIID,
						IID const&		classDispatchIID,
						VSTRING_CONST	pszFileName)
		: VUnknown(classIID)
	{
		/* This must be known!*/
		VASSERT(VSTRLEN_CHECK(pszFileName))

		/* GUID_NULL is not valid!*/
		VASSERT(classIID != GUID_NULL)

		/* Initialize members.*/
		m_classDispatchIID =	classDispatchIID;
		m_pMostDerivedThis =	this;
		m_pDispatchInterace =	NULL;
		m_pTypeInfo =			NULL;

		ITypeLib* pTypeLibrary;

		/* Load the type library.*/
		if ( SUCCEEDED(LoadTypeLib(	VStringConverter(pszFileName),
									&pTypeLibrary)) )
		{
			/* Get the type information.*/
			if ( SUCCEEDED(pTypeLibrary->GetTypeInfoOfGuid(	m_classIID,
															&m_pTypeInfo)) )
			{
				/* Create a standard IDispatch interface for this object.*/
				if ( FAILED(CreateStdDispatch(
					GetUnknown(),
					m_pMostDerivedThis,
					m_pTypeInfo,
					(LPUNKNOWN*)&m_pDispatchInterace)) )
				{
					m_pTypeInfo->Release();
					m_pTypeInfo = NULL;
				}
			}

			/* We don't need the type library any more.*/
			pTypeLibrary->Release();
		}

		/* Did we succeed?*/
		if ( !m_pDispatchInterace )
			Error(VTRUE);
	}

	/** Protected destructor releases internal dispatch and type
	information interfaces.*/
	virtual ~VAutomationObject()
	{
		if ( m_pDispatchInterace )
			m_pDispatchInterace->Release();

		if ( m_pTypeInfo )
			m_pTypeInfo->Release();
	}

	/** Embedded Members.*/
	IDispatch*					m_pDispatchInterace;
	ITypeInfo*					m_pTypeInfo;
	IID							m_classDispatchIID;
};

#endif /* VAUTOMATIONOBJECT*/
