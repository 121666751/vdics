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

Primary Author of this source code file:	Todd Osborne
											(todd@vwcl.org)
Other Author(s) of this source code file:	Ingve Vormestrand
											(ingve@online.no)
*/

#ifndef VHEADER
#define VHEADER

#include "../vstandard.h"
#include "../structures/vpoint.hpp"
#include "../structures/vrect.hpp"
#include "vcommoncontrol.hpp"

/** Macro that declares a HD_ITEM struct named item and zero-fills it.*/
#define VDECLARE_HD_ITEM	\
	HD_ITEM item;			\
	VZEROSTRUCT(item);

/** Macro that declares a HD_LAYOUT struct named layout and zero-fills it.*/
#define VDECLARE_HD_LAYOUT	\
	HD_LAYOUT layout;		\
	VZEROSTRUCT(layout);

/** VHeader provides extended wrapping of the Win32 header common
control.*/
class VHeader : public VCommonControl
{
public:
	/** Default constructor initializes RTTI information.*/
	VHeader(VRTTI_TYPE nRTTI = VWCL_RTTI_HEADER)
		: VCommonControl(nRTTI)
		{;}

	/** Create the header control. Returns VTRUE on success, VFALSE on
	error. nStyle can be any of the standard WS_xxx style bits described in
	VWindow::Create(), plus header control specific ones including:

	HDS_HOTTRACK - A header item is highlighted when the pointer pauses
	over it.

	HDS_DRAGDROP - Header items can be reordered by dragging and
	dropping them.

	HDS_FULLDRAG - The header control to displays column contents while the
	user resizes a column.

	HDS_BUTTONS - Header items behave like buttons.

	HDS_HORZ - The header control is horizontal.

	HDS_VERT - The header control is vertical (this style may not currently
	be implemented by Microsoft).

	HDS_HIDDEN - The header control is not visible in details mode.*/
	VBOOL			Create(	VWindow const&	windowParent,
							VRect const&	r,
							VUINT			nID,
							VUINT			nStyle =
												WS_BORDER |
												WS_TABSTOP |
												WS_VISIBLE |
												HDS_HORZ)
	{
		return VCommonControl::Create(	windowParent,
										r,
										WS_CHILD | nStyle,
										WC_HEADER,
										NULL,
										nID);
	}

	/** Creates a semi-transparent version of an item's image for use as a
	dragging image. Returns a handle to an image list that contains the new
	image as its only element. nIndex specifies the zero-based index of the
	item within the header control. The image assigned to this item is the
	basis for the transparent image.*/
	HIMAGELIST		CreateDragImage(VUINT nIndex) const
		{ return (HIMAGELIST)VWINDOW_SM1(HDM_CREATEDRAGIMAGE, nIndex); }

	/** Deletes an item from a header control. Returns VTRUE if successful,
	or VFALSE otherwise. nIndex is the index of the item to delete.*/
	VBOOL			DeleteItem(VUINT nIndex) const
		{ return VWINDOW_SMB1(HDM_DELETEITEM, nIndex); }

	/** Retrieves information about an item in a header control. Returns VTRUE
	if successful, or VFALSE otherwise. nIndex  is the index of the item for
	which information is to be retrieved. item is a reference to a HD_ITEM
	structure. When called, the mask member indicates the type of information
	being requested. When the method returns, the other members receive the
	requested information. If the mask member specifies zero, the function
	returns VTRUE, but copies no information to the structure.*/
	VBOOL			GetItem(VUINT		nIndex,
							HD_ITEM&	item)
		{ return VWINDOW_SMB2(HDM_GETITEM, nIndex, &item); }

	/** Return the number of headers displayed in the control.*/
	VUINT			GetItemCount() const
	{
		VLONG nResult = VWINDOW_SM0(HDM_GETITEMCOUNT);

		if ( nResult > 0 )
			return (VUINT)nResult;

		return 0;
	}

	/** Retrieves the bounding rectangle for a given item in a header
	control. Returns VTRUE if successful, VFALSE otherwise. nIndex specifies
	the zero-based index of the header control item for which to retrieve the
	bounding rectangle. rect is a reference to the VRect object that receives
	the bounding rectangle information on success.*/
	VBOOL			GetItemRect(VUINT	nIndex,
								VRect&	rect) const
		{ return VWINDOW_SMB2(HDM_GETITEMRECT, nIndex, rect.GetPointer()); }

	/** Tests a point to determine which header item, if any, is at the
	specified point. Returns the index of the item at the specified
	position. This method can also return extended information about the
	hit test results using the pnDetail parameter, if not NULL. pnDetail
	will be one or more (bitmask) of the following values when the
	returned value is NOT -1:

	HHT_NOWHERE - The point is inside the header control's bounding rectangle
	but is not over a header item.

	HHT_ONDIVIDER - The point is on the divider between two header items.

	HHT_ONDIVOPEN - The point is on the divider of an item that has a width
	of zero. Dragging the divider reveals the item instead of resizing the
	item to the left of the divider.

	HHT_ONHEADER - The point is inside the header control's bounding
	rectangle.

	HHT_TOLEFT - The point is to the left of the header control's bounding
	rectangle.
	
	HHT_TORIGHT - The point is to the right of the header control's bounding
	rectangle.*/
	VLONG			HitTest(VPoint const&	pt,
							VUINT*			pnDetail =
												NULL) const
	{
		/* Initialize structure.*/
		HD_HITTESTINFO hti;
		VZEROSTRUCT(hti);

		hti.pt.x = pt.GetX();
		hti.pt.y = pt.GetY();

		VLONG nResult = VWINDOW_SM2(HDM_HITTEST, 0, &hti);

		if ( pnDetail )
			*pnDetail = (nResult != -1) ? hti.flags : 0;

		return nResult;
	}

	/** Get the image list associated with this header control, or NULL if
	there is not one.*/
	HIMAGELIST		ImageList() const
		{ return (HIMAGELIST)VWINDOW_SM0(HDM_GETIMAGELIST); }

	/** Assigns an image list to an existing header control. Returns the
	handle to the image list previously associated with the control. Returns
	NULL upon failure, or if no image list was set previously.*/
	HIMAGELIST		ImageList(HIMAGELIST hImageList) const
		{ return (HIMAGELIST)VWINDOW_SM2(HDM_SETIMAGELIST, 0, hImageList); }

	/** Inserts a new item into a header control. Returns the index of the
	new item if successful, or -1 otherwise. nIndex of the item after which
	the new item is to be inserted. The new item is inserted at the end of
	the header control if index is greater than or equal to the number of
	items in the control. If nIndex is zero, the new item is inserted at
	the beginning of the header control. item is a reference to a HD_ITEM
	structure that contains information about the new item. item must be
	fully populated with correct values before calling this method.*/
	VLONG			InsertItem(	VUINT			nIndex,
								HD_ITEM const&	item) const
		{ return VWINDOW_SM2(HDM_INSERTITEM, nIndex, &item); }

	/** Retrieves the correct size and position of a header control within
	the parent window. Returns VTRUE if successful, or VFALSE otherwise.
	layout is a reference to a HD_LAYOUT structure. The prc member specifies
	the coordinates of a rectangle, and the pwpos member receives the size
	and position for the header control within the rectangle.*/
	VBOOL			Layout(HD_LAYOUT& layout) const
		{ return VWINDOW_SMB2(HDM_LAYOUT, 0, &layout); }

	
	/** Retrieves an index value for an item based on its order in the header
	control. Returns a value that indicates the item index. If nOrder is
	invalid (negative or too large), the return equals nOrder. nOrder is which
	the item appears within the header control, from left to right. For example,
	the index value of the item in the far left column would be 0. The value for
	the next item to the right would be 1, and so on.*/
	VLONG			Order(VUINT nOrder) const
		{ return VWINDOW_SM1(HDM_ORDERTOINDEX, nOrder); }

	/** Retrieves the current left-to-right order of items in a header
	control. Returns an array of integer values on success, NULL on failure.
	The array returned contains an item index for each item in the header,
	read from left to right. The array returned from this method must be
	free'd by the caller when done, using VDELETE_ARRAY or similiar
	method. The end of the array returned is signaled by the value -1.*/
	VINT*			OrderArray() const
	{
		VINT* pnValues =	NULL;
		VLONG nCount =		GetItemCount();

		if ( nCount )
		{
			/* Allocate array plus 1 for -1 terminator.*/
			pnValues = VNEW VINT[nCount + 1];
			
			if ( pnValues && VWINDOW_SM2(HDM_GETORDERARRAY, nCount, pnValues) )
				pnValues[nCount] = -1;
			else
				VDELETE_ARRAY_NULL(pnValues)
		}

		return pnValues;
	}

	/** Sets the left-to-right order of header items. Returns VTRUE if
	successful, VFALSE otherwise. nSize is the size of the buffer at
	pnArray, in elements. This value must equal the value returned by
	GetItemCount(). pnArray is an array of integers that specifies
	the order in which items should be displayed, from left to right.
	For example, if the contents of the array are {2,0,1}, the control
	displays item 2, item 0, and item 1, from left to right.*/
	VBOOL			OrderArray(	VUINT	nSize,
								VINT*	pnArray) const
	{
		/* This must be true!*/
		VASSERT(nSize == GetItemCount())
		
		return VWINDOW_SMB2(HDM_SETORDERARRAY, nSize, pnArray);
	}

	/** Changes the color of a divider between header items to indicate the
	destination of an external drag-and-drop operation. Returns a value
	equal to the index of the divider that the control highlighted. nFlag
	specifies the type of value represented by nInputValue. This value can
	be one of the following: VTRUE which indicates that nInputValue holds
	the client coordinates of the pointer. VFALSE which indicates that
	nInputValue holds a divider index value. nInputValue is interpreted
	depending on the value of nFlag. If nFlag is VTRUE, nInputValue represents
	the x- and y-coordinates of the pointer. The x-coordinate is in the low
	word, and the y-coordinate is in the high word. When the header control
	receives the message, it highlights the appropriate divider based on the
	nInputValue coordinates. If nFlag is VFALSE, nInputValue represents the
	integer index of the divider to be highlighted. This method creates an
	effect that a header control automatically produces when it has the
	HDS_DRAGDROP style. The HDM_SETHOTDIVIDER message is intended to be used
	when the owner of the control handles drag-and-drop operations manually.*/
	VLONG			SetHotDivider(	VBOOL bFlag,
									VUINT nInputValue) const
		{ return VWINDOW_SM2(HDM_SETHOTDIVIDER, bFlag, nInputValue); }

	/** Sets the attributes of the specified item in a header control. Returns
	VTRUE on success, VFALSE on failure. nIndex specifies the index of the
	item whose attributes are to be changed. item is a reference to a HD_ITEM
	structure that contains item information. When called, the mask member of
	the structure must be set to indicate which attributes are being set.*/
	VBOOL			SetItem(VUINT			nIndex,
							HD_ITEM const&	item) const
		{ return VWINDOW_SMB2(HDM_SETITEM, nIndex, &item); }

	/** Returns the UNICODE character format flag for the control.*/
	VBOOL			UnicodeFormat() const
		{ return VWINDOW_SMB0(HDM_GETUNICODEFORMAT); }

	/** Sets the UNICODE character format flag for the control. This function
	allows you to change the character set used by the control at run time
	rather than having to re-create the control. Returns the previous UNICODE
	format flag for the control. bUNICODE determines the character set that is
	used by the control. If this value is VTRUE, the control will use UNICODE
	characters. If this value is VFALSE, the control will use ANSI
	characters.*/
	VBOOL			UnicodeFormat(VBOOL bUNICODE = VTRUE) const
		{ return VWINDOW_SMB1(HDM_SETUNICODEFORMAT, bUNICODE); }

protected:
	/** Notifies that the user has begun dragging a divider in the control,
	that is, the user has pressed the left mouse button while the mouse cursor
	is on a divider in the header control. Return VFALSE to allow tracking of
	the divider, or VTRUE to prevent tracking. The base class returns VFALSE.
	nmHeader is a reference to a NMHEADER structure that contains information
	about the header control and the header item.*/
	virtual VBOOL	OnBeginTrack(	NMHEADER&	nmHeader,
									VLPARAM&	lCommonControlResult)
		{ return VFALSE; }

	/** Notifies that the user double-clicked the divider area of the control.
	nmHeader is a reference to a NMHEADER structure that contains information
	about the header control and the header item.*/
	virtual void	OnDividerDoubleClick(	NMHEADER&	nmHeader,
											VLPARAM&	lCommonControlResult)
		{;}

	/** Notifies that the user has finished dragging a divider in the control.
	nmHeader is a reference to a NMHEADER structure that contains information
	about the header control and the header item.*/
	virtual void	OnEndTrack(	NMHEADER&	nmHeader,
								VLPARAM&	lCommonControlResult)
		{;}

	/** Called when the control needs information about a callback header
	item. nmDispInfo is a refernce to a NMHDDISPINFO structure. On input,
	the fields of the structure specify what information is required and the
	item of interest. Fill the appropriate members of the structure to return
	the requested information to the header control. If your message handler
	sets the mask member of the NMHDDISPINFO structure to HDI_DI_SETITEM,
	the header control stores the information and will not request it again.*/
	virtual void	OnGetDispInfo(	NMHDDISPINFO& nmDispInfo,
									VLPARAM&		lCommonControlResult)
		{;}

	/** Notifies that the attributes of a header item have changed. nmHeader
	is a reference to a NMHEADER structure that contains information about
	the header control and the header item, including the attributes that
	have changed.*/
	virtual void	OnItemChanged(	NMHEADER&	nmHeader,
									VLPARAM&	lCommonControlResult)
		{;}

	/** Notifies that the attributes of a header item are about to change.
	Return VFALSE to allow the changes, or VTRUE to prevent them. nmHeader
	is a reference to a NMHEADER structure that contains information about
	the header control and the header item, including the attributes that
	are about to change. The base class returns VFALSE to allow the changes.*/
	virtual VBOOL	OnItemChanging(	NMHEADER&	nmHeader,
									VLPARAM&	lCommonControlResult)
		{ return VFALSE; }

	/** Notifies that the user clicked the control. nmHeader is a reference
	to a NMHEADER structure that contains information about the header
	control and the header item.*/
	virtual void	OnItemClick(	NMHEADER&	nmHeader,
									VLPARAM&	lCommonControlResult)
		{;}

	/** Notifies that the user double-clicked the control. nmHeader is a
	reference to a NMHEADER structure that contains information about the
	header control and the header item. Only header controls that are set
	to the HDS_BUTTONS style send this notification.*/
	virtual void	OnItemDoubleClick(	NMHEADER&	nmHeader,
										VLPARAM&	lCommonControlResult)
		{;}

	/** Notifies that the user is dragging a divider in the control. Return
	VFALSE to continue tracking or VTRUE to end tracking. The base class
	returns VFALSE. nmHeader is a reference to a NMHEADER structure that
	contains information about the header control and the header item.*/
	virtual VBOOL	OnTrack(	NMHEADER&	nmHeader,
								VLPARAM&	lCommonControlResult)
		{ return VFALSE; }

	/** Watches for notifications to call reflected notify handlers to
	perform extended operations.*/
	virtual VLONG	OnReflectedNotify(	NMHDR&		nmhdr,
										VLPARAM&	lCommonControlResult)
	{
		NMHEADER*		pNMHeader =		(NMHEADER*)&nmhdr;
		NMHDDISPINFO*	pNMDispInfo =	(NMHDDISPINFO*)&nmhdr;

		switch ( nmhdr.code )
		{
			case HDN_ITEMCHANGED:
				OnItemChanged(				VREF_PTR(pNMHeader),
											lCommonControlResult);
				return 0;

			case HDN_ITEMCHANGING:
				return OnItemChanging(		VREF_PTR(pNMHeader),
											lCommonControlResult);

			case HDN_ITEMCLICK:
				OnItemClick(				VREF_PTR(pNMHeader),
											lCommonControlResult);
				return 0;

			case HDN_ITEMDBLCLICK:
				OnItemDoubleClick(			VREF_PTR(pNMHeader),
											lCommonControlResult);
				return 0;

			case HDN_DIVIDERDBLCLICK:
				OnDividerDoubleClick(		VREF_PTR(pNMHeader),
											lCommonControlResult);
				return 0;

			case HDN_BEGINTRACK:
				return OnBeginTrack(		VREF_PTR(pNMHeader),
											lCommonControlResult);

			case HDN_ENDTRACK:
				OnEndTrack(					VREF_PTR(pNMHeader),
											lCommonControlResult);
				return 0;

			case HDN_TRACK:
				return OnTrack(				VREF_PTR(pNMHeader),
											lCommonControlResult);

			case HDN_GETDISPINFO:
				OnGetDispInfo(				VREF_PTR(pNMDispInfo),
											lCommonControlResult);
				return 0;
		}

		return VCommonControl::OnReflectedNotify(nmhdr, lCommonControlResult);
	}
};

#endif /* VHEADER*/
