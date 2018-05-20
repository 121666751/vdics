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

#ifndef VSPLITMAINWINDOW
#define VSPLITMAINWINDOW

#include "../vstandard.h"
#include "../gdi/vdc.hpp"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vmainwindow.hpp"
#include "vsplitsupport.hpp"

/** VSplitMainWindow is a main application window with split window support.
It derives its functionality from both VMainWindow and VSplitSupport, and
implements the VSplitSupport macro's to route messages to this class. See
VMainWindow and VSplitSupport documentation for more information.*/
class VSplitMainWindow : public VMainWindow, public VSplitSupport
{
public:
	/** Default constructor initializes RTTI information and
	split support options in base classes.*/
	VSplitMainWindow(	VUINT		nSplitSupportOptions =
										VSplitSupport::OPTION_DEFAULT,
						VRTTI_TYPE	nRTTI =
										VWCL_RTTI_SPLIT_MAIN_WINDOW)
		: VMainWindow(nRTTI), VSplitSupport(nSplitSupportOptions)
		{;}

protected:
	/** Macros implement message mapping to VSplitSupport base class.*/
	VWCL_SPLIT_WINDOW_PROC(VMainWindow)

	/** VMainWindow override sets client rectangle in VSplitSupport to r
	and calls VSplitSupport::RepositionChildren() to re-arrange the
	position of attached windows.*/
	virtual void	RecalcLayout(VRect rect)
	{
		SetClientRectangle(VRECT_BREAK(rect));
		RepositionChildren(VREF_THIS);
		VMainWindow::RecalcLayout(rect);
	}

	/** VMainWindow override moves split position to offset for showing
	or hiding control bars.*/
	virtual void	ShowControlBar(	VUINT nControlBar =
											IDW_TOOLBAR,
									VBOOL bShow =
											VTRUE)
	{
		/* This code is only needed when doing horizontal splits, and
		then only when visibile.*/
		if ( IsSplitterHorizontal() && IsVisible() )
		{
			VWindow const*	pControlBar =		GetControlBar(nControlBar);
			VINT			nControlHeight =	0;

			if ( pControlBar )
			{
				VRect r;
				pControlBar->GetRect(r);
				nControlHeight = r.GetHeight();
			}

			/* Determine if we need to offset the split position.*/
			if ( nControlHeight && bShow )
			{
				if ( nControlBar == IDW_TOOLBAR )
				{
					if ( GetSplitPosition() <= nControlHeight )
						GetSplitPosition() += nControlHeight;
				}
				else
				{
					VRect rClient, rSplitter;
					((VWindow*)this)->GetClientRect(rClient);
					GetSplitterRect(rSplitter);

					if (	rSplitter.GetBottom() >=
							rClient.GetBottom() - nControlHeight )
						GetSplitPosition() -= nControlHeight;
				}
			}
		}

		/* Let base class do the showing or hiding and window update.*/
		VMainWindow::ShowControlBar(nControlBar, bShow);
	}
};

#endif /* VSPLITMAINWINDOW*/
