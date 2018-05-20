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

#ifndef VHTMLSTREAMBLOCK
#define VHTMLSTREAMBLOCK

#include "../vstandard.h"
#include "../strings/vstring.hpp"
#include "vhtmlstream.hpp"

/** This class is the same as VHTMLStream, but overrides OnStream() to send
output to a VString object. In addition, the constructor sends the <html>
starting tag, while the destructor sends the </html> ending tag to the
stream. This is useful mostly when writing a complete HTML stream in memory
before sending it to the client from a VWebServerReqeust object. It allows
error checking before sending and also may increase performance by sending
the entire stream to the client at one time. Some clients, and some servers,
have a problem moving small bits and bytes sent to it in rapid, small
chunks, and use of this class can correct some of these issues by
transferring content in bulk.*/
class VHTMLStreamBlock : public VHTMLStream
{
public:
	/** Default constuctor allocates the VString object with a default, or
	custom, granularity size. The default of 8k is sufficient for most HTML
	generation needs, but this value can be adjusted here. Because this
	object uses a VString, the granularity is not a maximum value. It
	represents how much the string block grows each time an allocation is
	needed that would require more memory than what is available. See
	VString for more information. After the string block object is created,
	the <html> header is sent to the stream. See VHTMLStream for more
	information on the pWebServerRequest parameter.*/
	VHTMLStreamBlock(	VWebServerRequest*	pWebServerRequest =
												NULL,
						VUINT				nGranularitySize =
												8 * 1024)
		: VHTMLStream(pWebServerRequest, VFALSE)
	{
		m_pStringBlock = NULL;
		Reset(nGranularitySize, VTRUE);
	}

	/** Virtual destructor sends the </html> tag string to the stream (if
	needed), and calls the base class OnStream() function to send the cached
	content to the output device. If an error occurred while building the
	string block, as determined by VString::GetErrorCount(), the content
	WILL NOT be sent, so your code should likely check for an error before
	destructing this object. You can then choose to send the content
	yourself, or choose an alternate path.*/
	virtual ~VHTMLStreamBlock()
	{
		/* Stream the </html> end tag?*/
		if ( m_bNeedsHTMLEndTag )
			HTMLEnd();

		/* If no error, send the content now.*/
		if (	m_pStringBlock &&
				!m_pStringBlock->GetErrorCount() &&
				m_pStringBlock->IsNotEmpty() )
			VHTMLStream::OnStream(	m_pStringBlock->String(),
									m_pStringBlock->GetLength());

		/* Delete the stringblock object.*/
		VDELETE_NULL(m_pStringBlock)
	}

	/** Return a pointer to the internal string object used for caching
	content.*/
	VString*		GetString() const
		{ return (VString*)m_pStringBlock; }

	/** Base class override resets the stream to empty, using the current
	granularity.*/
	virtual void	Reset(VBOOL bSendHTMLStartTag = VTRUE)
	{
		Reset(	(m_pStringBlock) ? m_pStringBlock->GetGranularity() : 0,
				bSendHTMLStartTag);
	}

	/** Reset the internal VString object to empty, set the error condition
	to VFALSE, and re-sends the <html> start tag if bSendHTMLStartTag is
	VTRUE. See the constructor documentation for more information.*/
	void			Reset(	VUINT nGranularitySize,
							VBOOL bSendHTMLStartTag =
									VTRUE)
	{
		if ( !m_pStringBlock )
			m_pStringBlock = VNEW VString;
		else
			m_pStringBlock->Empty();

		/* Stream the html start tag.*/
		if ( m_pStringBlock )
		{
			/* Reset this flag for now.*/
			m_bNeedsHTMLEndTag = VFALSE;

			m_pStringBlock->GetGranularity() =
				(nGranularitySize) ? nGranularitySize : 8 * 1024;

			VHTMLStream::Reset(bSendHTMLStartTag);
		}
	}

protected:
	/** Override sends data to string block object.*/
	virtual void	OnStream(	VSTRING_CONST	pszText,
								VULONG			nLength =
													0)
	{
		/* We need to check for pszText being NULL and be a NOOP.*/
		if ( pszText && m_pStringBlock )
			m_pStringBlock->Append(pszText);
		/* This will likely never happen, but if we don't have a VString,
		have the base class send pszText anyway.*/
		else
			VHTMLStream::OnStream(pszText, nLength);
	}

	/** Embedded Members.*/
	VString*		m_pStringBlock;
};

#endif /* VHTMLSTREAMBLOCK*/
