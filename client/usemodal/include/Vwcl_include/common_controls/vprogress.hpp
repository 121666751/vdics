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

#ifndef VPROGRESS
#define VPROGRESS

#include "../vstandard.h"
#include "../numbers/vrtti.hpp"
#include "../structures/vrect.hpp"
#include "../windows/vwindow.hpp"
#include "vcommoncontrol.hpp"

/** VProgress is simple wrapping of the Win32 progress bar common control.*/
class VProgress : public VCommonControl
{
public:
	/** Default constructor initializes RTTI information in base class.*/
	VProgress(VRTTI_TYPE nRTTI = VWCL_RTTI_PROGRESS)
		: VCommonControl(nRTTI)
		{;}

	/** Create the progress control. Returns VTRUE on success, VFALSE on
	error. nStyle can be any of the standard WS_xxx style bits described in
	VWindow::Create(), plus progress control specific ones including:

	PBS_VERTICAL - Displays progress information vertically, top to bottom.
	Without this flag, the progress bar control displays horizontally, left
	to right.

	PBS_SMOOTH - Displays gradual, smooth filling in the progress bar
	control. Without this flag, the control will fill with blocks.*/
	VBOOL	Create(	VWindow const&	windowParent,
					VRect const&	r,
					VUINT			nID,
					VUINT			nStyle =
										WS_BORDER | WS_VISIBLE)
	{
		return VCommonControl::Create(	windowParent,
										r,
										WS_CHILD | nStyle,
										PROGRESS_CLASS,
										NULL,
										nID);
	}

	/** Advances the current position of a progress bar by a specified
	increment and redraws the bar to reflect the new position.*/
	VINT	OffsetPos(VINT nPos) const
		{ return (VINT)VWINDOW_SM1(PBM_DELTAPOS, nPos); }

	/** Sets the current position for a progress bar and redraws the bar to
	reflect the new position. Returns the previous position.*/
	VINT	SetPos(VINT nPos) const
		{ return VWINDOW_SM1(PBM_SETPOS, nPos); }

	/** Sets the minimum and maximum values for a progress bar and redraws
	the bar to reflect the new range. Returns the previous range values if
	successful, or zero otherwise. The low-order word specifies the previous
	minimum value, and the high-order word specifies the previous maximum
	value.*/
	VLONG	SetRange(	VINT nLower,
						VINT nUpper) const
		{ return VWINDOW_SM3(PBM_SETRANGE, 0, nLower, nUpper); }

	/** Specifies the step increment for a progress bar. The step increment
	is the amount by which the progress bar increases its current position
	whenever StepIt() is used. By default, the step increment is set to
	10.Returns the previous step increment.*/
	VINT	SetStep(VINT nStep) const
		{ return VWINDOW_SM1(PBM_SETSTEP, nStep); }

	/** Advances the current position for a progress bar by the step
	increment and redraws the bar to reflect the new position. An application
	sets the step increment with SetStep() function. Returns the
	previous position. When the position exceeds the maximum range value,
	this message resets the current position so that the progress indicator
	starts over again from the beginning.*/
	VINT	StepIt() const
		{ return (VINT)VWINDOW_SM0(PBM_STEPIT); }
};

#endif /* VPROGRESS*/
