//
/////////////////////////////////////////////////////////////////////////////////////////
// MidiText.cpp: implementation of the CMidiText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MidiText.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <stdio.h>

IMPLEMENT_DYNAMIC(CMidiText, CMidiEvent)

//
/////////////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//		Text
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiText::CMidiText( unsigned char cCommand, unsigned char *pBuffer )
{
	unsigned char				*pString;

	m_MidiEventStartOfBuffer	= pBuffer;

	//
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//		Check event code.
	if ( cCommand != m_cMidiEventCode )
	{
		EventMessageBox("CMidiText Code Differ");
	}

	//	Now set the Sequence subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_TEXT;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_TEXT;

	pBuffer += 1;
	m_iMidiTextLength	= ConvertText2Value ( pBuffer );
	if ( m_iMidiTextLength > 0x1000 )
	{
		m_iMidiTextLength = 1;
	}
	pBuffer += m_iMidiClassLength;

	m_MidiTextBuffer = ( unsigned char * ) malloc ( m_iMidiTextLength + 1 );
	memset ( ( char * ) m_MidiTextBuffer, 0, m_iMidiTextLength + 1 );
	memcpy ( ( char * ) m_MidiTextBuffer, ( char * ) pBuffer, m_iMidiTextLength );
	pBuffer	+= m_iMidiTextLength;

	m_MidiEventEndOfBuffer		= pBuffer;

	//		Filter the text
	pString = m_MidiTextBuffer;
	while ( *pString )
	{
		if ( *pString < ' ' )
		{
			*pString = ' ';
		}

		if ( *pString == 0x7f )
		{
			*pString = ' ';
		}

		if ( *pString == 0xff )
		{
			*pString = ' ';
		}

		pString++;
	}	
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//		Text
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiText::CMidiText( const unsigned char *pBuffer )
{
	unsigned char				*pString;

	m_MidiEventStartOfBuffer	= NULL;

	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//	Now set the Sequence subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_TEXT;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_TEXT;

	m_iMidiTextLength	= (int) strlen ( ( char * ) pBuffer );

	m_MidiTextBuffer = ( unsigned char * ) malloc ( m_iMidiTextLength + 1 );
	memset ( ( char * ) m_MidiTextBuffer, 0, m_iMidiTextLength + 1 );
	memcpy ( ( char * ) m_MidiTextBuffer, ( char * ) pBuffer, m_iMidiTextLength );

	m_MidiEventEndOfBuffer		= NULL;

	//		Filter the text
	pString = m_MidiTextBuffer;
	while ( *pString )
	{
		if ( *pString < ' ' )
		{
			*pString = ' ';
		}

		if ( *pString == 0x7f )
		{
			*pString = ' ';
		}

		if ( *pString == 0xff )
		{
			*pString = ' ';
		}

		pString++;
	}	
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiText::~CMidiText()
{
	Free ();
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiText::SetText ( const unsigned char *pBuffer )
{
	if ( m_MidiTextBuffer != NULL )
	{
		free ( m_MidiTextBuffer );
		m_MidiTextBuffer = NULL;
	}

	m_iMidiTextLength	= (int) strlen ( ( char * ) pBuffer );

	m_MidiTextBuffer = ( unsigned char * ) malloc ( m_iMidiTextLength + 1 );
	memset ( ( char * ) m_MidiTextBuffer, 0, m_iMidiTextLength + 1 );
	memcpy ( ( char * ) m_MidiTextBuffer, ( char * ) pBuffer, m_iMidiTextLength );

}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiText::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_TEXT ) );
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiText::Trace( FILE *hFile)
{
	if ( hFile != NULL )
	{
		if ( m_MidiTextBuffer != NULL )
		{
			fprintf ( hFile, "%s\t:\t'%s'\n", GetEventSmallText ( MIDI_EVENT_TEXT ), m_MidiTextBuffer );
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiText::Free()
{
	if ( m_MidiTextBuffer != NULL )
	{
		free ( m_MidiTextBuffer );
		m_MidiTextBuffer = NULL;
	}
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
unsigned char *CMidiText::GetTextAddress( unsigned short cSubCode ) const
{

	if ( GetSubCodeMask ( ) & cSubCode )
	{
		return ( m_MidiTextBuffer );
	}
	else
	{
		return NULL;
	}
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiText::Save ( FILE *hFile)
{
	char		szTemp [ 2 ];

	if ( hFile != NULL )
	{
		//		Correct unknown texts to normal texts
		if ( m_iEventIndex == MIDI_EVENT_UNKNOWNTEXT )
		{
			m_cMidiEventSubCode = MIDI_EVENT_CODE_META_TEXT;
		}

		szTemp [ 0 ] = m_cMidiEventCode;
		szTemp [ 1 ] = m_cMidiEventSubCode;

		fwrite ( szTemp, 1, 2, hFile );
		if ( m_MidiTextBuffer != NULL )
		{
			int			iClassLength;

			iClassLength = GetMidiClassLength ( m_iMidiTextLength );
			if ( iClassLength == 0 )
			{
				//		Force abort
				m_MidiTextBuffer = NULL;
				*m_MidiTextBuffer = ' ';
			}

			fwrite ( ConvertValue2Text ( m_iMidiTextLength ), 1, iClassLength, hFile );
			if ( m_iMidiTextLength > 0 )
			{
				fwrite ( m_MidiTextBuffer, 1, m_iMidiTextLength, hFile );
			}
		}
		else
		{
			fwrite ( ConvertValue2Text ( 0 ), 1, GetMidiClassLength ( 0 ), hFile );
		}
	}
}

IMPLEMENT_DYNAMIC(CMidiCopyRight, CMidiText)

//
/////////////////////////////////////////////////////////////////////////////////////////
//		Copyright
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiCopyRight::CMidiCopyRight( unsigned char cCommand, unsigned char *pBuffer ) : CMidiText(cCommand,  pBuffer )
{
	m_MidiEventStartOfBuffer	= pBuffer;

	//
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//		Check event code.
	if ( cCommand != m_cMidiEventCode )
	{
		EventMessageBox("CMidiCopyRight Code Differ");
	}

	//	Now set the Copyright subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_COPY;

	if ( *pBuffer != m_cMidiEventSubCode )
	{
		EventMessageBox("CMidiCopyRight SubCode Differ");
	}

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_COPYRIGHT;

}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiCopyRight::CMidiCopyRight( const unsigned char *pBuffer ) : CMidiText( pBuffer )
{
	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//	Now set the Copyright subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_COPY;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_COPYRIGHT;

}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiCopyRight::~CMidiCopyRight()
{
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiCopyRight::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_COPYRIGHT ) );
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiCopyRight::Trace( FILE *hFile)
{
	if ( hFile != NULL )
	{
		if ( m_MidiTextBuffer != NULL )
		{
			fprintf ( hFile, 
				"%s\t:\t'%s'\n", 
				GetEventSmallText ( MIDI_EVENT_COPYRIGHT ),
				m_MidiTextBuffer );
		}
	}
}

IMPLEMENT_DYNAMIC(CMidiTrackName, CMidiText)

//
/////////////////////////////////////////////////////////////////////////////////////////
//		Track Name
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiTrackName::CMidiTrackName( unsigned char cCommand, unsigned char *pBuffer ) : CMidiText(cCommand,  pBuffer )
{
	m_MidiEventStartOfBuffer	= pBuffer;

	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//		Check event code.
	if ( cCommand != m_cMidiEventCode )
	{
		EventMessageBox("CMidiTrackName Code Differ");
	}

	//	Now set the Track name subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_TRKNAME;

	if ( *pBuffer != m_cMidiEventSubCode )
	{
		EventMessageBox("CMidiTrackName SubCode Differ");
	}

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_TRACKNAME;
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiTrackName::CMidiTrackName( const unsigned char *pBuffer ) : CMidiText( pBuffer )
{
	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//	Now set the Track name subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_TRKNAME;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_TRACKNAME;
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiTrackName::~CMidiTrackName()
{
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiTrackName::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_TRACKNAME ) );
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiTrackName::Trace( FILE *hFile)
{
	if ( hFile != NULL )
	{
		if ( m_MidiTextBuffer != NULL )
		{
			fprintf ( hFile, 
				"%s\t:\t'%s'\n", 
				GetEventSmallText ( MIDI_EVENT_TRACKNAME ),
				m_MidiTextBuffer );
		}
	}
}

IMPLEMENT_DYNAMIC(CMidiInstrumentName, CMidiText)

//
/////////////////////////////////////////////////////////////////////////////////////////
//		Instrument name
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiInstrumentName::CMidiInstrumentName( unsigned char cCommand, unsigned char *pBuffer ) : CMidiText ( cCommand,  pBuffer )
{
	m_MidiEventStartOfBuffer	= pBuffer;

	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//		Check event code.
	if ( cCommand != m_cMidiEventCode )
	{
		EventMessageBox("CMidiInstrumentName Code Differ");
	}

	//	Now set the Instrument Name subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_INSNAME;

	if ( *pBuffer != m_cMidiEventSubCode )
	{
		EventMessageBox("CMidiInstrumentName SubCode Differ");
	}

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_INSTRNAME;

}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiInstrumentName::CMidiInstrumentName( const unsigned char *pBuffer ) : CMidiText( pBuffer )
{
	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//	Now set the Instrument Name subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_INSNAME;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_INSTRNAME;

}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiInstrumentName::~CMidiInstrumentName()
{
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiInstrumentName::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_INSTRNAME ) );
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiInstrumentName::Trace( FILE *hFile)
{
	if ( hFile != NULL )
	{
		if ( m_MidiTextBuffer != NULL )
		{
			fprintf ( hFile, 
				"%s\t:\t'%s'\n", 
				GetEventSmallText ( MIDI_EVENT_INSTRNAME ),
				m_MidiTextBuffer );
		}
	}
}

IMPLEMENT_DYNAMIC(CMidiLyrics, CMidiEvent)

//
/////////////////////////////////////////////////////////////////////////////////////////
//		Lyrics
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiLyrics::CMidiLyrics( unsigned char cCommand, unsigned char *pBuffer ):CMidiText(cCommand,  pBuffer )
{
	m_MidiEventStartOfBuffer	= pBuffer;

	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//		Check event code.
	if ( cCommand != m_cMidiEventCode )
	{
		EventMessageBox("CMidiLyrics Code Differ");
	}

	//	Now set the Lyrics subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_LYRICS;

	if ( *pBuffer != m_cMidiEventSubCode )
	{
		EventMessageBox("CMidiLyrics SubCode Differ");
	}

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_LYRICS;

}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiLyrics::CMidiLyrics( const unsigned char *pBuffer ) : CMidiText( pBuffer )
{
	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//	Now set the Lyrics subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_LYRICS;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_LYRICS;

}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiLyrics::~CMidiLyrics()
{
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiLyrics::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_LYRICS ) );
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiLyrics::Trace( FILE *hFile)
{
	if ( hFile != NULL )
	{
		if ( m_MidiTextBuffer != NULL )
		{
			fprintf ( hFile, 
				"%s\t:\t'%s'\n", 
				GetEventSmallText ( MIDI_EVENT_LYRICS ),
				m_MidiTextBuffer );
		}
	}
}

IMPLEMENT_DYNAMIC(CMidiMarkers, CMidiEvent)

//
/////////////////////////////////////////////////////////////////////////////////////////
//		Markers
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiMarkers::CMidiMarkers( unsigned char cCommand, unsigned char *pBuffer ):CMidiText(cCommand,  pBuffer )
{
	m_MidiEventStartOfBuffer	= pBuffer;

	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//		Check event code.
	if ( cCommand != m_cMidiEventCode )
	{
		EventMessageBox("CMidiMarkers Code Differ");
	}

	//	Now set the Markers subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_MARKER;

	if ( *pBuffer != m_cMidiEventSubCode )
	{
		EventMessageBox("CMidiMarkers SubCode Differ");
	}

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_MARKERS;

}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiMarkers::CMidiMarkers( const unsigned char *pBuffer ) : CMidiText( pBuffer )
{
	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//	Now set the Markers subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_MARKER;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_MARKERS;

}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiMarkers::~CMidiMarkers()
{
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiMarkers::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_MARKERS ) );
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiMarkers::Trace( FILE *hFile)
{
	if ( hFile != NULL )
	{
		if ( m_MidiTextBuffer != NULL )
		{
			fprintf ( hFile, 
				"%s\t:\t'%s'\n", 
				GetEventSmallText ( MIDI_EVENT_MARKERS ),
				m_MidiTextBuffer );
		}
	}
}

IMPLEMENT_DYNAMIC(CMidiCuePoint, CMidiEvent)

//
/////////////////////////////////////////////////////////////////////////////////////////
//		Cue Point
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiCuePoint::CMidiCuePoint( unsigned char cCommand, unsigned char *pBuffer ):CMidiText(cCommand,  pBuffer )
{
	m_MidiEventStartOfBuffer	= pBuffer;

	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//		Check event code.
	if ( cCommand != m_cMidiEventCode )
	{
		EventMessageBox("CMidiMarkers Code Differ");
	}

	//	Now set the Cue Point subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_CUEPOINT;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_CUEPOINT;

}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiCuePoint::CMidiCuePoint( const unsigned char *pBuffer ) : CMidiText( pBuffer )
{
	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//	Now set the Cue Point subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_CUEPOINT;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_CUEPOINT;

}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiCuePoint::~CMidiCuePoint()
{
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiCuePoint::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_CUEPOINT ) );
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiCuePoint::Trace( FILE *hFile)
{
	if ( hFile != NULL )
	{
		if ( m_MidiTextBuffer != NULL )
		{
			fprintf ( hFile, 
				"%s\t:\t'%s'\n", 
				GetEventSmallText ( MIDI_EVENT_CUEPOINT ),
				m_MidiTextBuffer );
		}
	}
}


IMPLEMENT_DYNAMIC(CMidiUnknownText, CMidiText)

//
/////////////////////////////////////////////////////////////////////////////////////////
//		Unknown Text
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiUnknownText::CMidiUnknownText( unsigned char cCommand, unsigned char *pBuffer ):CMidiText(cCommand,  pBuffer )
{
	m_MidiEventStartOfBuffer	= pBuffer;

	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//		Check event code.
	if ( cCommand != m_cMidiEventCode )
	{
		EventMessageBox("CMidiUnknownText Code Differ");
	}

	//	Now set the Sequence subcode.
	m_cMidiEventSubCode			= *pBuffer;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_UNKNOWNTEXT;
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiUnknownText::CMidiUnknownText( const unsigned char *pBuffer ) : CMidiText( pBuffer )
{
	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//	Now set the Sequence subcode.
	m_cMidiEventSubCode			= *pBuffer;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_UNKNOWNTEXT;
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiUnknownText::~CMidiUnknownText()
{
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiUnknownText::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_UNKNOWNTEXT ) );
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiUnknownText::Trace( FILE *hFile)
{
	if ( hFile != NULL )
	{
		if ( m_MidiTextBuffer != NULL )
		{
			fprintf ( hFile, 
				"%s\t:\t'%s'\n", 
				GetEventSmallText ( MIDI_EVENT_UNKNOWNTEXT ),
				m_MidiTextBuffer );
		}
	}
}

