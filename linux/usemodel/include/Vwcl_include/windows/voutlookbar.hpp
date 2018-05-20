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

#ifndef VOUTLOOKBAR
#define VOUTLOOKBAR

#include "../vstandard.h"
#include "../collections/vlinkedliststringitem.hpp"
#include "../collections/vsortedstringlinkedlist.hpp"
#include "../gdi/vdc.hpp"
#include "../gdi/vclientdc.hpp"
#include "../gdi/v3drect.hpp"
#include "../gdi/vfont.hpp"
#include "../gdi/vgdiobjectselector.hpp"
#include "../numbers/vbitmask.hpp"
#include "../structures/vpoint.hpp"
#include "../structures/vrect.hpp"
#include "../strings/vstring.hpp"
#include "../windows/vwindow.hpp"

/** Forward Declaration(s).*/
class VOutlookBar;

/** This is the abstract base class for both the VOutlookBarItem and
VOutlookBarHeader. It provides basic functionality needed by
both classes. It is derived from VLinkedListStringItem, which
is used to store the object name.*/
class VOutlookBarBase : public VLinkedListStringItem
{
friend class VOutlookBar;

public:
	/** Default constructor can be initialized with the object name.
	In addition, the user defined data can be set during construction.*/
	VOutlookBarBase(VSTRING_CONST	pszName =
										NULL,
					VUINT			nUserData =
										0)
		: VLinkedListStringItem(pszName)
	{
		/* Initialize Members.*/
		m_nUserData =		nUserData;
		m_nDrawTextFormat =	0;
	}

	/** Returns a refence to a user defined value. The class initializes
	this to 0, but you are free to store any data you like here.*/
	VUINT&			GetUserData() const
		{ return (VUINT&)m_nUserData; }

	/** Returns VTRUE if this object is actually a VOutlookBarItem
	object, VFALSE if not. If VFALSE is returned the object is
	a VOutlookBarHeader.*/
	virtual VBOOL	IsItem() = 0;

protected:
	/** Embedded Members.*/
	VRect			m_Rect;
	VUINT			m_nUserData;
	VUINT			m_nDrawTextFormat;
};

/** Objects of this type are items (as opposed to headers) displayed in
a VOutlookBar window. Items are the individual members contained by
a VOutlookBarHeader. Each item normally displays an icon and text.*/
class VOutlookBarItem : public VOutlookBarBase
{
public:
	/** Default constructor can be initialized with the object name and a
	handle to the icon to display. In addition, the user defined data can
	be set during construction.*/
	VOutlookBarItem(VSTRING_CONST	pszName =
										NULL,
					HICON			hIcon =
										NULL,
					VUINT			nUserData =
										0)
		: VOutlookBarBase(pszName, nUserData)
	{
		/* Initialize Members.*/
		m_hIcon = hIcon;
	}

	/** Constructor can be initialized with the object name and icon,
	which will be loaded from resources. In addition, the user defined
	data can be set during construction.*/
	VOutlookBarItem(VSTRING_CONST	pszName,
					VUINT			nIconID =
										0,
					VUINT			nUserData =
										0,
					HINSTANCE		hResource =
										NULL)
		: VOutlookBarBase(pszName, nUserData)
	{
		/* Initialize Members.*/
		if ( nIconID )
			m_hIcon = LoadIcon(	(hResource)
								? hResource
								: VGetResourceHandle(),
								MAKEINTRESOURCE(nIconID));
		else
			m_hIcon = NULL;
	}

	/** Draw this item. This function will be called to render each
	item. This base class function draws the icon specified for this
	object (centered left to right), draws a focus rectangle around it
	if bDrawFocus is VTRUE, and text using the font, text background mode,
	and text color specified before this function is called. If using
	DrawText() to render the text, as this base class function does, you
	should honor the m_nDrawTextFormat member, which is the format options
	to pass to DrawText(). When drawing each element (icon, text, etc.) each
	element should be separated from the previous one by nHeaderSpacing
	pixels. It is not valid to call this method when the object has an
	invalid rectangle.*/
	virtual void		Draw(	VDC&	dc,
								VBOOL	bDrawFocus,
								VUINT	nHeaderSpacing)
	{
		/* This should not happen.*/
		VASSERT(m_Rect.IsNotEmpty())

		/* Starting location. We offset it nHeaderSpacing pixels each
		time we draw an element.*/
		VUINT nCurrentYPos = m_Rect.GetTop();

		/* Draw the icon?*/
		if ( m_hIcon )
		{
			/* Offset for icon spacing.*/
			nCurrentYPos += nHeaderSpacing;

			/* Draw the icon.*/
			dc.DrawIcon(m_Rect.GetHalfWidth() - 16, nCurrentYPos, m_hIcon);

			/* Draw focus border.*/
			if ( bDrawFocus )
			{
				/* This rectangle draws 2 pixels outside of icon
				area. This is based on a 32x32 icon image only.*/
				V3DRect r(	(VLONG)m_Rect.GetHalfWidth() - 18,
							(VLONG)nCurrentYPos - 2,
							(VLONG)m_Rect.GetHalfWidth() + 18,
							(VLONG)nCurrentYPos + 34,
							V3DRect::OPTION_NO_FILL,
							RGB(0,0,0),
							RGB(255,255,255),
							RGB(0,0,0));

				r.Draw(dc);
			}

			/* Offset current position to below icon.*/
			nCurrentYPos += 32;
		}

		/* Draw the text?*/
		if ( IsNotEmpty() )
		{
			/* Create bounding rectangle for text.*/
			VRect r(m_Rect);

			/* Offset the top.*/
			r.GetTop() = (nCurrentYPos + nHeaderSpacing);

			/* Draw the text.*/
			dc.DrawText(String(), r, m_nDrawTextFormat, GetLength());
		}
	}

	/** Return a reference to the icon.*/
	HICON&				GetIcon() const
		{ return (HICON&)m_hIcon; }

	/** Since these objects are stored in a linked list, call this function
	to return a pointer to the next item, or NULL if there are no more items.
	To get a pointer to the first item, use
	VOutlookBarHeader::GetFirstItem().*/
	VOutlookBarItem*	GetNext() const
		{ return (VOutlookBarItem*)m_pNextItem; }

	/** Override informs caller that this object is an item, not a header.*/
	VBOOL				IsItem()
		{ return VTRUE; }

protected:
	/** Embedded Members.*/
	HICON				m_hIcon;
};

/** Objects of this type are headers (as opposed to items) displayed in
a VOutlookBar window. A header object contains a linked list of
VOutlookBarItem objects, and remembers which one is active. It also has
provisions for governing item sorting order and other sorting options.*/
class VOutlookBarHeader : public VOutlookBarBase
{
friend class VOutlookBar;

public:
	/** Default constructor can be initialized with the object name. In
	addition, the user defined data can be set during construction.*/
	VOutlookBarHeader(VSTRING_CONST	pszName =
										NULL,
						VUINT		nUserData =
										0)
		: VOutlookBarBase(pszName, nUserData)
		{ Init(); }

	/** Draw this header. This function will be called to render each
	header. This base class function fills the background with the user
	defined 3D face color using GetSysColor(). It then draws the borders
	in colors consistent with Microsoft Outlook, and draws the text using
	the font, text background mode, and text color selected into the device
	context before this function is called. If using DrawText() to render the
	text, as this base class function does, you should honor the
	m_nDrawTextFormat member, which is the format options to pass to
	DrawText(). It is not valid to call this method when the object has an
	invalid rectangle.*/
	virtual void				Draw(VDC& dc)
	{
		/* This should not happen.*/
		VASSERT(m_Rect.IsNotEmpty())

		/* Initialize 3D rectangle for drawing.*/
		V3DRect rHeader(m_Rect,
						V3DRect::OPTION_SHRINK_RECT_ON_DRAW,
						GetSysColor(COLOR_3DFACE),
						RGB(255,255,255),
						RGB(127,127,127));

		/* Draw border.*/
		rHeader.Draw(dc);

		/* Draw the text?*/
		if ( IsNotEmpty() )
			dc.DrawText(String(), rHeader, m_nDrawTextFormat, GetLength());
	}

	/** Get a pointer to the first VOutlookBarItem object contained by this
	header. You can then walk the list of items using
	VOutlookBarItem::GetNext(). This function will return the first item
	contained by this header, or NULL if the header has no items.*/
	VOutlookBarItem*			GetFirstItem() const
		{ return (VOutlookBarItem*)m_listItems.m_pFirstItem; }

	/** Since objects of this type are stored in a linked list, call this
	method to get a pointer to the next header object, or NULL if there are
	no remaining headers. To get a pointer to the first header, use the
	VOutlookBar::GetFirstHeader() method.*/
	VOutlookBarHeader*			GetNext() const
		{ return (VOutlookBarHeader*)m_pNextItem; }

	/** Override informs caller that this object is a header, not an item.*/
	VBOOL						IsItem()
		{ return VFALSE; }

	/** Set the sorting options. The nOptions is a bitmask of the options
	for the VSortedStringLinkedList class. The default sorting is unsorted,
	and duplicate named items are not allowed.*/
	void						SetSortingOptions(VUINT nOptions)
		{ m_listItems.GetOptions() = nOptions; }

protected:
	/** Internal function to initialize the class.*/
	void						Init()
	{
		m_pActiveItem = NULL;

		m_listItems.GetOptions() =
			VSortedStringLinkedList::OPTION_NO_DUPLICATES |
			VSortedStringLinkedList::OPTION_UNSORTED;
	}

	/** Embedded Members.*/
	VSortedStringLinkedList		m_listItems;
	VOutlookBarItem*			m_pActiveItem;
};

/** Microsoft Outlook invented a control that presents the user a selection
of icons, each below a parent header. It is a powerful and popular control for
navigation. VOutlookBar is very similar to this control used in Outlook, hence
the name. The control displays a series of headers and items. Headers are
normally 3-D buttons. There can only be one active header at a time, and the
items contained by this header are displayed below it. When the user selects
a header, normally by clicking it with the mouse, it becomes the active
header, moves to the top-most part of the control not obscured by other
headers, and displays its contained items. The user can then select an item
that belongs to the active header, and it becomes the (only) active item.

This class encapsulates all of this functionality. In addition, you can set
the fonts and colors to be used for headers and items, and they can be set
independantly of each other. Header and Items support multiline and singleline
text. Virtual functions in the VOutlookBar, VOutlookBarItem, and
VOutlookBarHeader classes provide for a great deal of customization. See the
outlook_test VWCL sample application for a sample of how to use this
control.*/
class VOutlookBar : public VWindow
{
public:
	/** Constructor initializes class.*/
	VOutlookBar(VRTTI_TYPE nRTTI = VWCL_RTTI_OUTLOOK_BAR)
		: VWindow(nRTTI)
	{
		/* Initialize Members.*/
		m_pActiveHeader =				NULL;
		m_nMinHeaderHeight =			20;
		m_nHeaderSpacing =				5;
		m_nItemSpacing =				8;
		m_nBottomHeadersStartY =		0;
		m_crHeaderText =				RGB(0,0,0);
		m_crItemText =					RGB(255,255,255);

		/* Initialize header options.*/
		m_HeaderOptions = HEADER_OPTION_DEFAULT;

		/* Initialize sorting.*/
		m_listHeaders.GetOptions() =
			VSortedStringLinkedList::OPTION_NO_DUPLICATES |
			VSortedStringLinkedList::OPTION_UNSORTED;

		/* We need to handle our own painting.*/
		GetOptions().Append(OPTION_NEED_ON_PAINT);

		/* We benefit from double-buffering.*/
		GetOptions().Append(OPTION_DOUBLE_BUFFERED_PAINT);

		/* Initialize fonts by creating duplicates of ANSI_VAR_FONT.*/
		m_FontHeaders.Duplicate(GetStockFont(ANSI_VAR_FONT));
		m_FontItems.Duplicate(GetStockFont(ANSI_VAR_FONT));

		m_ItemOptions = ITEM_OPTION_DEFAULT;
	}

	/** Destructor performs object cleanup.*/
	virtual ~VOutlookBar()
	{
		/* Determine if the header and item fonts are the same. We
		only want to delete one of them.*/
		if (	m_FontHeaders.GetHandle() &&
				m_FontHeaders.GetHandle() == m_FontItems.GetHandle() )
		{
			m_FontHeaders.GetHandle() = NULL;
		}
	}

	/** Get the active header. Returns pointer to the object on success,
	NULL on failure. NULL means there is no active header.*/
	VOutlookBarHeader*			ActiveHeader() const
		{ return m_pActiveHeader; }

	/** Set the active header. NULL is allowed for pHeader, which will clear
	the selection. Calling code should call the Refresh() method after
	using this function to update the user interface. This function is
	virtual, since it can be called internally by the class when the
	user clicks a header. Override this function to receive notifications
	in your derived class of header selection changes. You can also override
	OnActiveHeader() to receive this notification. Overrides of this
	function MUST call this base class method. Returns VTRUE if the active
	header changed, VFALSE otherwise.*/
	virtual VBOOL				ActiveHeader(VOutlookBarHeader* pHeader)
	{
		/* pHeader must be a part of the list owned by us!*/
		VASSERT(!pHeader || m_listHeaders.Find(pHeader))

		/* Did anything change?*/
		if ( m_pActiveHeader != pHeader )
		{
			/* Make this header active.*/
			m_pActiveHeader = pHeader;

			/* Call virtual functions.*/
			OnActiveHeader(m_pActiveHeader);

			if ( !pHeader )
				OnActiveItem(NULL);

			return VTRUE;
		}

		return VFALSE;
	}

	/** Get the active item. Returns a pointer to the active item, of the
	active header, or NULL if no header or item is active.*/
	VOutlookBarItem*			ActiveItem() const
		{ return (m_pActiveHeader) ? m_pActiveHeader->m_pActiveItem : NULL; }

	/** Set the active item of the active header. NULL is allowed for pItem,
	which will clear the selection. Calling code should call the Refresh()
	method after using this function to update the user interface. This
	function is virtual, since it can be called internally by the class when
	the user clicks an item. Override this function to receive notifications
	in your derived class of item selection changes. You can also override
	OnActiveItem() to receive this notification. Overrides of this function
	MUST call this base class method. Returns VTRUE if the active item changed,
	VFALSE otherwise. If OnActiveItem() should always be called, even if the
	item selection did not change, you can pass VTRUE for bAlwaysNotify. This
	could be the case when the header selection changes, causing a new header
	to be active, but where the active item for that header did not change.*/
	virtual VBOOL				ActiveItem(
		VOutlookBarItem*	pItem,
		VBOOL				bAlwaysNotify =
								VFALSE)
	{
		/* This must be known!*/
		VASSERT(m_pActiveHeader)

		/* pItem must be a member of the active header.*/
		VASSERT(!pItem || m_pActiveHeader->m_listItems.Find(pItem))

		if ( m_pActiveHeader )
		{
			if ( pItem )
			{
				/* Did anything change?*/
				if ( bAlwaysNotify || m_pActiveHeader->m_pActiveItem != pItem )
				{
					/* Save this item as active.*/
					m_pActiveHeader->m_pActiveItem = pItem;

					/* Call virtual function.*/
					OnActiveItem(pItem);

					return VTRUE;
				}
			}
			/* Just clear current selection?*/
			else if ( bAlwaysNotify || m_pActiveHeader->m_pActiveItem )
			{
				m_pActiveHeader->m_pActiveItem = NULL;
				OnActiveItem(NULL);

				return VTRUE;
			}
		}

		return VFALSE;
	}

	/** Add a new header to the list. pHeader must be a VOutlookBarHeader or
	derived object, and MUST be allocated with VNEW. It is not valid to
	pass stack based objects. Once added, the object pointed to by pHeader
	is owned by this class, and you should never attempt to delete it.
	If AddHeader() fails, it will return VFALSE. In this case, you must delete
	the header, since this class did not assume ownership of it. If adding
	the first header to the collection, the header will be considered the
	active header. If this is not desired, use ActiveHeader() to change
	the current header. If there is already an active header, it will not be
	modified. If you want pHeader to be the active header, after it is added
	to the list, you can pass VTRUE for the bActive parameter. You should call
	the Refresh() method to update the control. If this method fails, it
	probably means that an item already existed in the list with the same name
	as pItem. This check for no duplicates can be turned off using the
	SetHeaderSortingOptions() method.*/
	VBOOL						AddHeader(
		VOutlookBarHeader*	pHeader,
		VBOOL				bActive =
								VFALSE)
	{
		/* This must be known!*/
		VASSERT(pHeader)

		if ( pHeader )
		{
			/* pHeader must NOT already be a part of the list owned by us,
			or any other header for that matter!*/
			VASSERT(!m_listHeaders.Find(pHeader))

			/* Add to collection. It can refuse based on sorting options.*/
			if ( pHeader && m_listHeaders.Add(pHeader) )
			{
				/* Should we activate this header?*/
				if ( bActive || !m_pActiveHeader )
				{
					ActiveHeader(pHeader);
					ActiveItem(pHeader->m_pActiveItem, VTRUE);
				}

				return VTRUE;
			}

			/* Assert only to tell you AddHeader() failed. Check the
			sorting options, since that is really the only reason adding
			to the list could fail. Remember, duplicates are by default
			disabled!*/
			VASSERT(VFALSE)
		}

		return VFALSE;
	}

	/** Add a new item to the list. pItem must be a VOutlookBarItem or
	derived object, and MUST be allocated with VNEW. It is not valid to
	pass stack based objects. Once added, the object pointed to by pItem
	is owned by this class, and you should never attempt to delete it.
	If AddItem() fails, it will return VFALSE. In this case, you must delete
	the item, since this class did not assume ownership of it. You should call
	the Refresh() method of the owner VOutlookBar object to update the
	control. If this method fails, it probably means that an item already
	existed in the list with the same name as pItem. This check for no
	duplicates can be turned off using the SetSortingOptions() method
	of the VOutlookBarHeader class. By default, pItem will be added to the
	active header. If this is not desired, you can pass a valid header object
	as pHeader. If pItem is the first item added to the header, it will be
	set to be the active item for that header. If there is already an active
	item, it will not be changed. If the new item should be made active, you
	can pass VTRUE for the bActive parameter.*/
	VBOOL						AddItem(VOutlookBarItem*	pItem,
										VOutlookBarHeader*	pHeader =
																NULL,
										VBOOL				bActive =
																VFALSE)
	{
		/* Get the header to use?*/
		if ( !pHeader )
			pHeader = m_pActiveHeader;

		/* This must be known!*/
		VASSERT(pItem)

		/* This must be known too!*/
		VASSERT(pHeader)

		if ( !pItem || !pHeader )
			return VFALSE;

		/* pItem must NOT already be a part of ANY header!*/
		#ifdef VWCL_DEBUG
			VOutlookBarHeader* pCheckHeader = GetFirstHeader();

			while ( pCheckHeader )
			{
				VASSERT(!pCheckHeader->m_listItems.Find(pItem))
				pCheckHeader = pCheckHeader->GetNext();
			}
		#endif

		/* Add to collection. It can refuse based on sorting options.*/
		if ( pItem && pHeader->m_listItems.Add(pItem) )
		{
			if ( bActive || !pHeader->m_pActiveItem )
			{
				/* If added to the active header, make it active, otherwise
				just set a pointer to the item in the inactive header.*/
				if ( pHeader == m_pActiveHeader )
					ActiveItem(pItem, bActive);
				else
					pHeader->m_pActiveItem = pItem;
			}

			return VTRUE;
		}

		/* Assert only to tell you AddItem() failed. Check the
		sorting options, since that is really the only reason adding
		to the list could fail. Remember, duplicates are by default
		disabled!*/
		VASSERT(VFALSE)

		return VFALSE;
	}

	/** Create the control. Returns VTRUE on success, VFALSE on failure.
	See the base class VWindow::Create() for more information about
	the nStyle parameter.*/
	VBOOL						Create(	VWindow&		windowParent,
										VRect const&	rect,
										VUINT			nID,
										VUINT			nStyle =
															WS_CHILD |
															WS_VISIBLE)
	{
		/* Base class creates the real window.*/
		return VWindow::Create(	windowParent,
								rect,
								nStyle,
								NULL,
								NULL,
								nID);
	}

	/** Delete a header from the list. pHeader must be a VOutlookBarHeader or
	derived object, and MUST be allocated with VNEW. It is not valid to
	pass stack based objects. If the object pointed to by pHeader should
	not be deleted, you can pass VFALSE for the bDeleteObject parameter.
	In this case, calling code is responsible for deleting the object,
	since this object will no longer maintain it. You should call the
	Refresh() method to update the control. Returns VTRUE if the header was
	deleted, VFALSE if not. If you delete the active header, the first header
	in the list will be made active.*/
	VBOOL						DeleteHeader(
		VOutlookBarHeader*	pHeader,
		VBOOL				bDeleteObject =
								VTRUE)
	{
		/* This must be known!*/
		VASSERT(pHeader)

		/* pHeader must be a part of the list owned by us!*/
		VASSERT(m_listHeaders.Find(pHeader))

		/* Try to delete the object now.*/
		if ( pHeader && m_listHeaders.Delete(pHeader, bDeleteObject) )
		{
			/* Was this the active header?*/
			if ( pHeader == m_pActiveHeader )
			{
				/* Activate the first header.*/
				ActiveHeader(GetFirstHeader());

				/* Activate this headers item too, or NULL.*/
				if ( m_pActiveHeader )
					ActiveItem(m_pActiveHeader->m_pActiveItem, VTRUE);
				else
					OnActiveItem(NULL);
			}

			return VTRUE;
		}

		return VFALSE;
	}

	/** Same as above, but deletes a header when the string name is known.*/
	VBOOL						DeleteHeader(
		VSTRING_CONST	pszName,
		VBOOL			bDeleteObject =
							VTRUE)
	{
		VOutlookBarHeader* pHeader = FindHeader(pszName);

		if ( pHeader )
			return DeleteHeader(pHeader, bDeleteObject);

		return VFALSE;
	}

	/** Delete an item from the list. pItem must be a VOutlookBarItem or
	derived object, and MUST be allocated with VNEW. It is not valid to
	pass stack based objects. If the object pointed to by pItem should
	not be deleted, you can pass VFALSE for the bDeleteObject parameter.
	In this case, calling code is responsible for deleting the object,
	since this object will no longer maintain it. You should call the
	Refresh() method of the owner VOutlookBar object to update the
	control. Returns VTRUE if the item was deleted, VFALSE if not. By
	default, this method assumes pItem is part of the active header. If
	you would like to delete an item from a different header, you can
	pass an object pointer to the header as pHeader.*/
	VBOOL						DeleteItem(
		VOutlookBarItem*	pItem,
		VBOOL				bDeleteObject =
								VTRUE,
		VOutlookBarHeader*	pHeader =
								NULL)
	{
		/* This must be known!*/
		VASSERT(pItem)

		if ( pItem )
		{
			/* Find the header that contains this item?*/
			if ( !pHeader )
				pHeader = FindItemHeader(pItem);

			/* pItem must be a part of a list owned by us!*/
			VASSERT(pHeader)

			if ( pHeader )
			{
				/* Try to delete the object now.*/
				if ( pHeader->m_listItems.Delete(pItem, bDeleteObject) )
				{
					/* Was the item active for the active header?*/
					if (	pHeader == m_pActiveHeader &&
							pItem == m_pActiveHeader->m_pActiveItem )
					{
						/* Set the active item to the first item, or NULL.*/
						ActiveItem(pHeader->GetFirstItem(), VTRUE);
					}
					/* Just set no active item.*/
					else if ( pHeader->m_pActiveItem == pItem )
						pHeader->m_pActiveItem = NULL;

					return VTRUE;
				}
			}
		}

		return VFALSE;
	}

	/** Same as above, but deletes an item when the string name is known.*/
	VBOOL						DeleteItem(	VSTRING_CONST	pszName,
											VBOOL			bDeleteObject =
																VTRUE)
	{
		VOutlookBarHeader*	pHeader =	NULL;
		VOutlookBarItem*	pItem =		FindItem(pszName, &pHeader);

		if ( pItem )
			return DeleteItem(pItem, bDeleteObject, pHeader);

		return VFALSE;
	}

	/** This method provides all drawing control logic. It draws all
	visible headers and item by cycling through the list of objects.
	It also uses a clipping region to prevent items from drawing
	outside of their bounds.*/
	virtual void				Draw(VDC& dc)
	{
		/* Don't waste time when there is nothing to do.*/
		VRect r;
		GetClientRect(r);

		if ( r.IsEmpty() )
			return;

		/* Select the font.*/
		VGDIObjectSelector	osHeaderFont(	dc,
											m_FontHeaders);

		/* Complete DC settings.*/
		VINT nOldBkMode =		dc.BackgroundMode(TRANSPARENT);
		COLORREF crOldColor =	dc.TextColor(m_crHeaderText);

		/* Fill the background.*/
		FillBackground(dc, r);

		/* Walk list, drawing each item.*/
		VOutlookBarHeader* pHeader = GetFirstHeader();

		while ( pHeader )
		{
			/* Draw this header?*/
			if ( pHeader->m_Rect.IsNotEmpty() )
			{
				pHeader->Draw(dc);

				/* Is this header active? Draw its items.*/
				if ( pHeader == m_pActiveHeader )
				{
					/* Select the item font.*/
					VGDIObjectSelector osItemFont(	dc,
													m_FontItems);

					HRGN hClippingRegion = NULL;

					/* We should create a clipping region to prevent
					items from drawing where we are going to draw bottom
					headers, if we are going to draw them at all.*/
					if ( m_nBottomHeadersStartY > 0 )
					{
						hClippingRegion = CreateRectRgn(
							0,
							pHeader->m_Rect.GetBottom(),
							pHeader->m_Rect.GetWidth(),
							m_nBottomHeadersStartY);

						if ( hClippingRegion )
							SelectClipRgn(dc.GetHandle(), hClippingRegion);
					}

					/* Draw items in item color.*/
					dc.TextColor(m_crItemText);

					/* Walk items in this header.*/
					VOutlookBarItem* pItem = pHeader->GetFirstItem();

					while ( pItem )
					{
						/* If we don't draw this item, we won't draw
						anymore, so break loop.*/
						if ( pItem->m_Rect.IsNotEmpty() )
						{
							/* Determine focus rectangle status. Initially
							look at whether or not this item is the active
							item or not.*/
							VBOOL bDrawFocus =
								(pHeader->m_pActiveItem == pItem)
								? VTRUE
								: VFALSE;

							if (	bDrawFocus &&
									m_ItemOptions.IsNotSet(
										ITEM_OPTION_ICON_BORDER_WHEN_ACTIVE) )
							{
								bDrawFocus = VFALSE;
							}

							pItem->Draw(dc,
										bDrawFocus,
										m_nHeaderSpacing);

							/* Next Item.*/
							pItem = pItem->GetNext();
						}
						else
							pItem = NULL;
					}

					/* Reset to header color.*/
					dc.TextColor(m_crHeaderText);

					/* Remove clipping region.*/
					if ( hClippingRegion )
					{
						SelectClipRgn(dc.GetHandle(), NULL);
						DeleteObject(hClippingRegion);
					}
				}
			}

			/* Next header.*/
			pHeader = pHeader->GetNext();
		}

		/* Restore DC settings.*/
		dc.BackgroundMode(nOldBkMode);
		dc.TextColor(crOldColor);

		/* Draw header lines?*/
		DrawHeaderLinesAsNeeded(dc);
	}

	/** This method draws a black line below and left of the active header.
	This simulates the way Microsoft Outlook does it. The lines are actually
	draw in the pixel space of the items below the active header. This
	function honors the header options, which can be set using the
	GetHeaderOptions() method. If the option bit
	HEADER_OPTION_BORDER_BELOW_ACTIVE is enabled, this line will draw the
	border lines. Otherwise, this function is a NO-OP. If there is not an
	active header, this function does nothing.*/
	virtual void				DrawHeaderLinesAsNeeded(VDC& dc)
	{
		VASSERT(dc.GetHandle())

		if (	m_pActiveHeader &&
				m_HeaderOptions.IsSet(HEADER_OPTION_BORDER_BELOW_ACTIVE) )
		{
			VRect r;
			GetClientRect(r);

			/* Determine bottom location.*/
			VUINT nBottom = r.GetBottom();

			if ( m_pActiveHeader->GetNext() )
				nBottom =
					m_pActiveHeader->GetNext()->m_Rect.GetTop() - 1;

			/* Initialize 3D rectangle for drawing.*/
			V3DRect rLines(	0,
							m_pActiveHeader->m_Rect.GetBottom() + 1,
							r.GetRight(),
							nBottom,
							V3DRect::OPTION_NO_FILL |
							V3DRect::OPTION_NO_DRAW_RIGHT |
							V3DRect::OPTION_NO_DRAW_BOTTOM,
							0,
							RGB(0,0,0));

			rLines.Draw(dc);
		}
	}

	/** Find a header in the list with the name specified. Returns object
	pointer on success, NULL on failure. The search is not case sensitive
	by default, but this can be enabled by SetHeaderSortingOptions()
	method.*/
	VOutlookBarHeader*			FindHeader(VSTRING_CONST pszName) const
		{ return (VOutlookBarHeader*)m_listHeaders.Find(pszName); }

	/** Find an item in the list with the name specified. Returns object
	pointer on success, NULL on failure. The search is not case sensitive
	by default, but this can be enabled by using the sorting options of
	the VSortedStringLinkedList class, which can be set by using the
	SetSortingOptions() method of the VOutlookBarHeader class. This method
	can return the header where the object was found. To enable this, pass
	a pointer to a pointer to the header object.*/
	VOutlookBarItem*			FindItem(
		VSTRING_CONST		pszName,
		VOutlookBarHeader**	ppHeader =
								NULL) const
	{
		VOutlookBarItem* pItem = NULL;

		if ( ppHeader )
			*ppHeader = NULL;

		/* Find the header that contains this item.*/
		VOutlookBarHeader* pHeader = GetFirstHeader();

		while ( pHeader )
		{
			pItem = (VOutlookBarItem*)pHeader->m_listItems.Find(pszName);

			if ( pItem )
			{
				if ( ppHeader )
					*ppHeader = pHeader;

				break;
			}
			else
				pHeader = pHeader->GetNext();
		}

		return pItem;
	}

	/** Given a VOutlookBarItem pointer, find the header that contains it.
	Returns the header object on success, NULL on failure.*/
	VOutlookBarHeader*			FindItemHeader(VOutlookBarItem* pItem) const
	{
		VASSERT(pItem)

		if ( !pItem )
			return NULL;

		/* See if it is in the active header first.*/
		if ( m_pActiveHeader && m_pActiveHeader->m_listItems.Find(pItem) )
			return m_pActiveHeader;

		/* Find the header that contains this item.*/
		VOutlookBarHeader* pHeader = GetFirstHeader();

		while ( pHeader )
		{
			/* No reason to search active header, we already did it.*/
			if (	pHeader != m_pActiveHeader &&
					pHeader->m_listItems.Find(pItem) )
			{
				return pHeader;
			}

			pHeader = pHeader->GetNext();
		}

		return NULL;
	}

	/** Return a pointer to the first VOutlookBarHeader object contained
	by the control. You can then use VOutlookBarHeader::GetNext() to get
	a pointer to the next header object, or VOutlookBarHeader::GetFirstItem()
	to get a pointer to the first item contained by the headers. Returns
	object pointer on success, NULL if there are no headers controlled by
	this control.*/
	VOutlookBarHeader*			GetFirstHeader() const
		{ return (VOutlookBarHeader*)m_listHeaders.m_pFirstItem; }

	/** Return the number of headers in the control.*/
	VUINT						GetHeaderCount() const
		{ return m_listHeaders.GetCount(); }

	/** Return the number of items in the specified header. If pHeader is
	NULL, the active header will be assumed.*/
	VUINT						GetHeaderItemCount(
		VOutlookBarHeader* pHeader = NULL) const
	{
		if ( !pHeader )
			pHeader = m_pActiveHeader;

		VASSERT(pHeader)

		if ( pHeader )
			return pHeader->m_listItems.GetCount();

		return 0;
	}

	/** Return a refence to the font used when drawing headers. This
	font is initialized to ANSI_VAR_FONT by the class, but can be
	changed by client code. This object owns the font, and it will be
	destroyed when this object is destroyed.*/
	VFont&						GetHeaderFont() const
		{ return (VFont&)m_FontHeaders; }

	/** Return a reference to the text color use when drawing headers.
	The default is black.*/
	COLORREF&					GetHeaderTextColor() const
		{ return (COLORREF&)m_crHeaderText; }

	/** Return a refence to the font used when drawing items. This
	font is initialized to ANSI_VAR_FONT by the class, but can be
	changed by client code. This object owns the font, and it will be
	destroyed when this object is destroyed.*/
	VFont&						GetItemFont() const
		{ return (VFont&)m_FontItems; }

	/** Return a reference to the text color use when drawing items.
	The default is white.*/
	COLORREF&					GetItemTextColor() const
		{ return (COLORREF&)m_crItemText; }

	/** Options that apply to all items. These can be set with the
	GetItemOptions() method. The default is ITEM_OPTION_DEFAULT,
	which enables a border around the icon when active.*/
	enum	{	ITEM_OPTION_NONE =							0x0000,
				ITEM_OPTION_ICON_BORDER_WHEN_ACTIVE =		0x0001,
				ITEM_OPTION_DEFAULT =
					ITEM_OPTION_ICON_BORDER_WHEN_ACTIVE,
			};

	/** Return a reference to the bitmask object used to store the
	options that govern how items are displayed. These values are
	defined in the enumeration above.*/
	VBitmask&					GetItemOptions() const
		{ return (VBitmask&)m_ItemOptions; }

	/** Options that apply to all headers. These can be set with the
	GetHeaderOptions() method. The default is HEADER_OPTION_DEFAULT,
	which enables a border around the items below the active header.*/
	enum	{	HEADER_OPTION_NONE =					0x0000,
				HEADER_OPTION_BORDER_BELOW_ACTIVE =		0x0001,
				HEADER_OPTION_DEFAULT =
					HEADER_OPTION_BORDER_BELOW_ACTIVE,
			};

	/** Return a reference to the bitmask object used to store the
	options that govern how headers are displayed. These values are
	defined in the enumeration above.*/
	VBitmask&					GetHeaderOptions() const
		{ return (VBitmask&)m_HeaderOptions; }

	/** Returns a reference to the minimum height (in pixels) for
	each header. The default is 20.*/
	VUINT&						GetMinHeaderHeight() const
		{ return (VUINT&)m_nMinHeaderHeight; }

	/** Returns a reference to the number of pixels use to offset the
	icon from the header, the text from the icon, and the distance below
	an item where the next header can begin. The default is 5 pixels.*/
	VUINT&						GetHeaderSpacing() const
		{ return (VUINT&)m_nHeaderSpacing; }

	/** Returns a reference to the number of pixels use to offset an item
	from where the next item can begin. The default is 8 pixels.*/
	VUINT&						GetItemSpacing() const
		{ return (VUINT&)m_nItemSpacing; }

	/** Given a pixel width, determine how much height text would need to
	consume to fit in the width. Returns the formatting required for the
	DrawText() API. nWidth should contain the width of the area to draw in.
	On exit, nHeight will contain the calculated height. If the string
	passed as strText is empty, nHeight should be 0 only for items. A header
	height still needs to be initialized to the minimum height. The font
	to use for calculations must already be selected into the device context
	defined by the dc parameter.*/
	virtual VUINT				GetTextHeight(
		VDC const&		dc,
		VString const&	strText,
		VUINT			nWidth,
		VUINT&			nHeight,
		VBOOL			bSizeIsForHeader)
	{
		/* This must be valid!*/
		VASSERT(dc.GetHandle())

		/* Set out parameter.*/
		nHeight = 0;

		/* Initial format.*/
		VUINT nFormat =
			DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX;

		/* Do we have text?*/
		if ( strText.IsNotEmpty() )
		{
			/* Determine text height.*/
			SIZE size;
			VZEROSTRUCT(size);
			GetTextExtentPoint32(	dc.GetHandle(),
									strText,
									strText.GetLength(),
									&size);

			/* Will the text fit on a single line?*/
			if ( size.cx > (VLONG)nWidth )
			{
				/* The text won't fit. Correct format.*/
				nFormat &= ~(DT_VCENTER | DT_SINGLELINE);
				nFormat |= DT_WORDBREAK;

				VRect rNeeded(0, 0, nWidth, 0);

				nHeight = dc.DrawText(	strText,
										rNeeded,
										nFormat | DT_CALCRECT,
										strText.GetLength());

				/* Fudge factor.*/
				nHeight += (nHeight / 8);
			}
			else
				nHeight = size.cy;
		}

		/* Headers must have size. Enforce minimums.*/
		if ( bSizeIsForHeader && nHeight < m_nMinHeaderHeight )
			nHeight = m_nMinHeaderHeight;

		return nFormat;
	}

	/** Determine what type of object is below the point indicated
	by the x and y coordinates. This function will return a VOutlookBarBase
	object pointer on success, NULL if nothing is below the point. If
	you are only interested in determining the header, use the HitTestHeader()
	method. If interested only in items, use the HitTestItem() function. If
	the object found, if one is found at all, should be selected, pass
	VTRUE for the bSelect parameter. This is not done by default.*/
	VOutlookBarBase*			HitTest(	VLONG nX,
											VLONG nY,
											VBOOL bSelect =
													VFALSE)
	{
		/* Initialize point.*/
		VPoint pt(nX, nY);

		/* Look in headers first. Don't return the first one found,
		instead keep looking. The reason is that a later header could
		start in the same pixel space, since it could be obscuring
		part of the header above it.*/
		VOutlookBarHeader* pHeader =		GetFirstHeader();
		VOutlookBarHeader* pFoundHeader =	NULL;

		while ( pHeader )
		{
			if (	pHeader->m_Rect.IsNotEmpty() &&
					pHeader->m_Rect.IsPointInside(pt) )
			{
				pFoundHeader = pHeader;
			}

			pHeader = pHeader->GetNext();
		}

		if ( pFoundHeader )
		{
			if ( bSelect && ActiveHeader(pFoundHeader) )
			{
				ActiveItem(m_pActiveHeader->m_pActiveItem, VTRUE);
				Refresh();
			}

			return pFoundHeader;
		}

		/* We did not find a header. Look for items in the active header.*/
		if ( m_pActiveHeader )
		{
			VOutlookBarItem* pItem = m_pActiveHeader->GetFirstItem();

			while ( pItem )
			{
				if ( pItem->m_Rect.IsPointInside(pt) )
				{
					if ( bSelect && ActiveItem(pItem) )
						Refresh();

					return pItem;
				}

				pItem = pItem->GetNext();
			}
		}

		/* Nothing.*/
		return NULL;
	}

	/** Same as HitTest(), but determine if the object below the specified
	point is a header. Returns a pointer to a header object if the point is
	over the header, NULL otherwise.*/
	VOutlookBarHeader*			HitTestHeader(	VLONG nX,
												VLONG nY,
												VBOOL bSelect =
														VFALSE)
	{
		VOutlookBarBase* pBase = HitTest(nX, nY);

		if ( pBase && !pBase->IsItem() )
		{
			VOutlookBarHeader* pHeader = (VOutlookBarHeader*)pBase;

			if ( bSelect && ActiveHeader(pHeader) )
			{
				ActiveItem(m_pActiveHeader->m_pActiveItem, VTRUE);
				Refresh();
			}

			return pHeader;
		}

		return NULL;
	}

	/** Same as HitTest(), but determine if the object below the specified
	point is an item. Returns a pointer to an item object if the point is
	over the item, NULL otherwise.*/
	VOutlookBarItem*			HitTestItem(VLONG nX,
											VLONG nY,
											VBOOL bSelect =
													VFALSE)
	{
		VOutlookBarBase* pBase = HitTest(nX, nY);

		if ( pBase && pBase->IsItem() )
		{
			VOutlookBarItem* pItem = (VOutlookBarItem*)pBase;

			if ( bSelect && ActiveItem(pItem) )
				Refresh();
			
			return pItem;
		}

		return NULL;
	}

	/** Refresh the control. This should be done whenever changes occur
	that could affect the display. This function is safe to call at all
	times, since it checks for valid control visibility first.*/
	void						Refresh(VBOOL bRedraw = VTRUE)
	{
		/* The goal of this function is to set the rectangles in all
		VISIBLE headers and ACTIVE items. All others must be set
		to empty rectangles.*/

		/* Initialize all to nothing.*/
		VOutlookBarHeader* pHeader = GetFirstHeader();

		while ( pHeader )
		{
			ClearBaseObject(pHeader);
			pHeader = pHeader->GetNext();
		}

		/* Start with client rectangle.*/
		VRect r;
		GetClientRect(r);

		/* Don't waste time when there is nothing to do.*/
		if ( r.IsEmpty() )
			return;

		/* Get our DC.*/
		VClientDC dc(VREF_THIS);

		if ( !dc.GetHandle() )
		{
			/* If this fails, did we leak GDI?*/
			VASSERT(VFALSE)
			return;
		}

		/* Select the font to use.*/
		VGDIObjectSelector	osHeaderFont(	dc,
											m_FontHeaders);

		/* We need to remember the current Y position.*/
		VLONG nCurrentYPos = 0;

		/* For speed, cache these.*/
		VLONG nControlWidth =	r.GetRight();
		VLONG nControlHeight =	r.GetBottom();

		/* We need to calculate the area of all headers AFTER the active
		header. They are displayed at the bottom.*/
		pHeader = GetFirstHeader();

		m_nBottomHeadersStartY =	0;
		VBOOL bFoundActive =		VFALSE;

		while ( pHeader )
		{
			if ( bFoundActive || pHeader == m_pActiveHeader )
			{
				/* If this is already set, account for this header.*/
				if ( bFoundActive )
				{
					VUINT nHeight =	0;

					/* Save DrawText() format.*/
					pHeader->m_nDrawTextFormat =
						GetTextHeight(	dc,
										VREF_PTR(pHeader),
										nControlWidth,
										nHeight,
										VTRUE);

					/* This is not allowed to be 0!*/
					VASSERT(nHeight)

					/* Save height as bottom parameter.*/
					pHeader->m_Rect.GetBottom() = nHeight;

					/* Offset.*/
					m_nBottomHeadersStartY += nHeight;
				}
				/* Skip this header, and start looking for next ones.*/
				else
					bFoundActive = VTRUE;
			}

			/* Next header.*/
			pHeader = pHeader->GetNext();
		}

		/* m_nBottomHeadersStartY now contains the height of bottom headers.
		We want it to contain the Y coordinate, so fix it.*/
		if ( m_nBottomHeadersStartY )
		{
			m_nBottomHeadersStartY =
				nControlHeight - m_nBottomHeadersStartY;

			/* When bottom headers are displayed, there MUST be an
			active header!*/
			VASSERT(m_pActiveHeader)

			/* If negative, there is not enough room to draw all headers,
			but we need to remember this offscreen starting point.*/
			if ( m_nBottomHeadersStartY < 0 )
				nCurrentYPos = m_nBottomHeadersStartY;
		}

		/* Now set rectangles in objects.*/
		pHeader = GetFirstHeader();
		VOutlookBarHeader* pLastVisibleHeader = NULL;

		while ( pHeader )
		{
			/* Determine the height of this header. This will have
			already been calculated for bottom placed headers. This
			is an optimization to avoid having to call GetTextRect()
			a second time.*/
			VUINT nHeaderHeight = pHeader->m_Rect.GetBottom();

			if ( nHeaderHeight )
				pHeader->m_Rect.GetBottom() = 0;
			else
				pHeader->m_nDrawTextFormat =
					GetTextHeight(	dc,
									VREF_PTR(pHeader),
									nControlWidth,
									nHeaderHeight,
									VTRUE);

			/* This is not allowed to be 0!*/
			VASSERT(nHeaderHeight)

			/* Offset the drawing position.*/
			nCurrentYPos += nHeaderHeight;

			/* Save rectangle.*/
			pHeader->m_Rect.Set(0,
								nCurrentYPos - nHeaderHeight,
								nControlWidth,
								nCurrentYPos);

			/* Render the active headers items?*/
			if ( pHeader == m_pActiveHeader )
			{
				/* Select the item font.*/
				VGDIObjectSelector osItemFont(	dc,
												m_FontItems);

				/* Calculate items.*/
				VOutlookBarItem* pItem = pHeader->GetFirstItem();

				/* Walk items. Skip items that won't be visible.*/
				while (	(pItem) &&
						(!m_nBottomHeadersStartY ||
						nCurrentYPos < m_nBottomHeadersStartY) )
				{
					VUINT nItemHeight = 0;

					/* Account for icon?*/
					if ( pItem->GetIcon() )
						nItemHeight += (32 + m_nHeaderSpacing);

					/* Determine text height.*/
					VUINT nItemTextHeight = 0;

					pItem->m_nDrawTextFormat =
						GetTextHeight(	dc,
										VREF_PTR(pItem),
										nControlWidth,
										nItemTextHeight,
										VFALSE);

					/* Account for text?*/
					if ( nItemTextHeight )
						nItemHeight +=
							(nItemTextHeight + m_nHeaderSpacing);

					/* Set text rectangle?*/
					if ( nItemHeight )
						pItem->m_Rect.Set(
							0,
							nCurrentYPos,
							nControlWidth,
							nCurrentYPos + nItemHeight);

					/* Save new current position.*/
					nCurrentYPos += nItemHeight;

					/* Next Item.*/
					pItem = pItem->GetNext();

					/* If there are more items, include item spacing.*/
					if ( pItem )
						nCurrentYPos += m_nItemSpacing;
				}

				/* All remaining headers are at the bottom.*/
				nCurrentYPos = m_nBottomHeadersStartY;
			}

			/* Next header.*/
			pHeader = pHeader->GetNext();
		}

		/* Update UI?*/
		if ( bRedraw )
			InvalidateRectUpdate();
	}

	/** Set the sorting options for headers. The nOptions is a bitmask of the
	options for the VSortedStringLinkedList class. The default sorting is
	unsorted, and duplicate named headers are not allowed.*/
	void						SetHeaderSortingOptions(VUINT nOptions)
		{ m_listHeaders.GetOptions() = nOptions; }

protected:
	/** Clear the rectangle and DrawText() mode from the object. If
	pBase is a header, all items in the header will also be cleared.*/
	void				ClearBaseObject(VOutlookBarBase* pBase)
	{
		VASSERT(pBase)

		pBase->m_Rect.Empty();
		pBase->m_nDrawTextFormat = 0;

		/* Is this a header? If yes, clear all items too.*/
		if ( !pBase->IsItem() )
		{
			VOutlookBarItem* pItem =
				((VOutlookBarHeader*)pBase)->GetFirstItem();

			while ( pItem )
			{
				ClearBaseObject(pItem);
				pItem = pItem->GetNext();
			}
		}
	}

	/** Fill the background of the control. This base class implementation
	fills the background with a gray brush. Override to fill with any
	color you desire.*/
	virtual void		FillBackground(	VDC&			dc,
										VRect const&	rFill)
		{ dc.FillRect(rFill, GetStockBrush(GRAY_BRUSH)); }

	/** This method is called when a header is made active. Override this
	method to receive these notifications. The base class does nothing.
	pActiveHeader will be NULL if there is not an active header.*/
	virtual void		OnActiveHeader(VOutlookBarHeader* pActiveHeader)
		{;}

	/** This method is called when an item is made active. Override this
	method to receive these notifications. The base class does nothing.
	pActiveItem will be NULL if there is not an active item for the active
	header.*/
	virtual void		OnActiveItem(VOutlookBarItem* pActiveItem)
		{;}

	/** Override performs drawing by calling the Draw() method.*/
	virtual VBOOL		OnPaint(VDC& dc)
	{
		Draw(dc);
		return VFALSE;
	}

	/** Override calls refresh to keep control in sync.*/
	virtual VLONG		OnSize(	VWPARAM	nFlags,
								VINT	ncx,
								VINT	ncy)
	{
		Refresh(VFALSE);
		return 0;
	}

	/** This override customizes the extended style bits.*/
	virtual VBOOL		PreCreateWindow(CREATESTRUCT& CreateStruct)
	{
		CreateStruct.dwExStyle = WS_EX_STATICEDGE;
		return VWindow::PreCreateWindow(CreateStruct);
	}

	/** Overrides looks for messages important to the control.*/
	virtual VLONG		WindowProc(	HWND	hWnd,
									VUINT	nMessage,
									VWPARAM	wParam,
									VLPARAM	lParam)
	{
		/* Look for click to activate header or item.*/
		if ( nMessage == WM_LBUTTONDOWN )
			HitTest(LOWORD(lParam), HIWORD(lParam), VTRUE);
		/* Look for erase background to reduce flicker.*/
		else if ( nMessage == WM_ERASEBKGND )
			return VTRUE;

		return VWindow::WindowProc(hWnd, nMessage, wParam, lParam);
	}

	/** Embedded Members.*/
	VSortedStringLinkedList	m_listHeaders;
	VOutlookBarHeader*		m_pActiveHeader;
	VFont					m_FontHeaders;
	VFont					m_FontItems;
	VBitmask				m_ItemOptions;
	VBitmask				m_HeaderOptions;
	COLORREF				m_crHeaderText;
	COLORREF				m_crItemText;
	VUINT					m_nMinHeaderHeight;
	VUINT					m_nHeaderSpacing;
	VUINT					m_nItemSpacing;
	VLONG					m_nBottomHeadersStartY;
};

#endif /* VOUTLOOKBAR*/