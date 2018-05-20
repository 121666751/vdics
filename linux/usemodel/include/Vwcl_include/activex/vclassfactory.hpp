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

#ifndef VCLASSFACTORY
#define VCLASSFACTORY

#include "../vstandard.h"
#include "vunknown.hpp"

/** Macro to implement an instance of a class factory object using just
the class factory name the caller wants, and the class to instantiate. For
example, if you have an ActiveX object named MyObject, this macro could be
used as follows:

VWCL_ACTIVEX_IMPLEMENT_CLASS_FACTORY(MyObjectClassFactory, MyObject)

At this time, this macro does not support COM aggregation. The class named
by class_name must be able to create an object from a parameterless Create()
function. If creating an object that requires initialization,
class_name::Create() must call another function with correct values.*/
#define VWCL_ACTIVEX_IMPLEMENT_CLASS_FACTORY(class_factory_name, class_name)	\
	class class_factory_name : public VClassFactory				\
	{															\
	public:														\
		STDMETHODIMP CreateInstance(IUnknown* pUnknownOuter,	\
									REFIID riid,				\
									VVOIDPTR* ppvObj)			\
		{														\
			*ppvObj = NULL;										\
			if ( pUnknownOuter )								\
				return CLASS_E_NOAGGREGATION;					\
			*ppvObj = class_name::Create();						\
			return (*ppvObj) ? S_OK : E_NOINTERFACE;			\
		}														\
		static class_factory_name* Create()						\
		{														\
			class_factory_name* pNew = VNEW class_factory_name	\
			VWCL_ACTIVEX_IMPLEMENT_STATIC_CREATE(pNew) }		\
	};

/** The following 3 macros are used when handling a request for a class
factory, for instance in DllGetClassObject() function. Simply handle the
function by calling VWCL_ACTIVEX_CLASS_FACTORY_MAP_BEGIN followed by as
many VWCL_ACTIVEX_CLASS_FACTORY_MAP_OBJECT as the number of available
objects that your code can create, and finally end with a call to
VWCL_ACTIVEX_CLASS_FACTORY_MAP_END. For example, if you create an
InProc object, your DllGetClassObject() function would look like this,
assuming you can only return two objects, one of type MyObject and the
other MyObjectAlso. Note that the class MyObjectClassFactory can be
generated using the VWCL_ACTIVEX_IMPLEMENT_CLASS_FACTORY macro, as shown here:

VWCL_ACTIVEX_IMPLEMENT_CLASS_FACTORY(MyObjectClassFactory, MyObject)
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, VVOIDPTR* ppvObj)
{
	VWCL_ACTIVEX_CLASS_FACTORY_MAP_BEGIN
		VWCL_ACTIVEX_CLASS_FACTORY_MAP_OBJECT(rclsid, IID_IMyObject, MyObjectClassFactory)
		VWCL_ACTIVEX_CLASS_FACTORY_MAP_OBJECT(rclsid, IID_IMyObjectAlso, MyObjectAlsoClassFactory)
	VWCL_ACTIVEX_CLASS_FACTORY_MAP_END
}
.*/

/** Macro to begin a class factory map. See above example for
more information.*/
#define VWCL_ACTIVEX_CLASS_FACTORY_MAP_BEGIN	\
	HRESULT hr =	E_OUTOFMEMORY;				\
	*ppvObj =		NULL;						\
	VClassFactory* pClassFactory = NULL;

/** Macro to create an instance of a class factory for a single object.
See above example for more information.*/
#define VWCL_ACTIVEX_CLASS_FACTORY_MAP_OBJECT(reqeusted_object_id, object_id, class_factory_name)	\
	if ( !pClassFactory && reqeusted_object_id == object_id )	\
		pClassFactory = class_factory_name::Create();

/** Macro to signal the end of a class factory map. See above example for
more information.*/
#define VWCL_ACTIVEX_CLASS_FACTORY_MAP_END					\
	if ( pClassFactory )									\
	{														\
		hr = pClassFactory->QueryInterface(riid, ppvObj);	\
		pClassFactory->Release();							\
	}														\
	return hr;

/** VClassFactory acts as a class factory for another object. See the
VWCL_ACTIVEX_IMPLEMENT_CLASS_FACTORY, VWCL_ACTIVEX_CLASS_FACTORY_MAP_BEGIN,
VWCL_ACTIVEX_CLASS_FACTORY_MAP_OBJECT, and VWCL_ACTIVEX_CLASS_FACTORY_MAP_END
macro's for more information. It is not possible to instantiate an object
of this type directly, since it is missing the
IClassFactory::CreateInstance() method. This method can be implemented by a
derived class or by using the above mentioned macro's.*/
class VClassFactory : public IClassFactory, public VUnknown
{
public:
	/** Implement IUnknown standard methods.*/
	VWCL_ACTIVEX_IMPLEMENT_IUNKNOWN_METHODS(VUnknown)

	/** IClassFactory::LockServer() Method. We simply use AddRef()
	and Release() to lock the server.*/
	STDMETHODIMP LockServer(VBOOL bLock)
	{
		if ( bLock )
			AddRef();
		else
			Release();

		return S_OK;
	}

protected:
	/** Protected constructor initializes the base VUnknown class with
	the class factory Interface ID IID_IClassFactory.*/
	VClassFactory()
		: VUnknown(IID_IClassFactory)
		{ m_pMostDerivedThis = this; }
};

#endif /* VCLASSFACTORY*/
