#include "stdafx.h"
#include "MidiText.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <stdio.h>

IMPLEMENT_DYNAMIC(CMidiNoteOn, CMidiEvent)

//
////////////////////////////////////////////////////////////////////////
//		Note On
//
////////////////////////////////////////////////////////////////////////
CMidiNoteOn::CMidiNoteOn ( unsigned char cCommand, unsigned char *pBuffer )
{
	m_MidiEventStartOfBuffer	= pBuffer;

	m_cMidiEventCode			= MIDI_EVENT_CODE_NOTEON;
	m_cMidiEventChannel			= 0xff;
	m_cMidiPitch				= 0x00;
	m_cMidiVelocity				= 0x00;

	//		Check Event code
	if ( ( cCommand & OPERATION_MASK ) != m_cMidiEventCode )
	{
		EventMessageBox("CMidiKeyPressure Code Differ");
	}

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_NOTEON;

	m_cMidiEventChannel			= cCommand & CHANNEL_MASK;

	//		Set Event code.
	m_cMidiEventCode			|= ( m_cMidiEventChannel & CHANNEL_MASK );

	m_cMidiPitch				= *pBuffer;
	m_cMidiEventSubCode			= m_cMidiPitch;
	pBuffer++;

	m_cMidiVelocity				= *pBuffer;
	if ( m_cMidiVelocity == 0 )
	{
		m_iEventIndex			= MIDI_EVENT_NOTEOFF;
	}
	pBuffer++;

	m_MidiEventEndOfBuffer		= pBuffer;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiNoteOn::CMidiNoteOn ( unsigned int iChannel, unsigned char cVal1, unsigned char cVal2 )
{
	m_cMidiEventCode			= MIDI_EVENT_CODE_NOTEON;
	m_cMidiEventChannel			= iChannel & CHANNEL_MASK;
	m_cMidiPitch				= cVal1;
	m_cMidiEventSubCode			= cVal1;
	m_cMidiVelocity				= cVal2;

	//		Set Event code.
	m_cMidiEventCode			|= ( m_cMidiEventChannel & CHANNEL_MASK );

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_NOTEON;

	if ( m_cMidiVelocity == 0 )
	{
		m_iEventIndex			= MIDI_EVENT_NOTEOFF;
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiNoteOn::~CMidiNoteOn ()
{
	Free ();
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiNoteOn::Save( FILE *hFile)
{
	if ( hFile != NULL )
	{
		fwrite ( &m_cMidiEventCode, 1, 1, hFile );
		fwrite ( &m_cMidiPitch, 1, 1, hFile );
		fwrite ( &m_cMidiVelocity, 1, 1, hFile );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
DWORD CMidiNoteOn::GetChunk()
{
	m_dwMidiChunk = m_cMidiEventCode + 256 * ( m_cMidiPitch + 256 * m_cMidiVelocity );
	return m_dwMidiChunk;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiNoteOn::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_NOTEON ) );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiNoteOn::Trace( FILE *hFile)
{
	if ( hFile != NULL )
	{
		fprintf ( hFile, 
			"Channel: %2d - %s: '%s%d' ('%s'), (%d), Velocity %d\n", 
			m_cMidiEventChannel,
			GetEventSmallText ( MIDI_EVENT_NOTEON ),
			NoteText ( m_cMidiPitch ), 
			NoteLevel ( m_cMidiPitch ), 
			NoteTextFrench ( m_cMidiPitch ), 
			m_cMidiPitch, m_cMidiVelocity );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const char * CMidiNoteOn::GetExtraText( ) const
{
	static	char	szNoteOnText [ MAX_NOTE_STRING ];
	if ( m_cMidiEventChannel != PERCUSSION_CHANNEL )
	{
		sprintf_s ( szNoteOnText, sizeof ( szNoteOnText ), "%s%d", NoteText ( m_cMidiPitch ), NoteLevel ( m_cMidiPitch ) );
		return szNoteOnText;
	}
	else
	{
		return CMidiClass::DrumText ( m_cMidiPitch );
	}
	
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiNoteOn::Free ()
{
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiNoteOn::GetEventValue1() const
{
	return m_cMidiPitch;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiNoteOn::GetEventValue2() const
{
	return m_cMidiVelocity;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiNoteOn::SetEventValue1(unsigned char  val)
{
	m_cMidiPitch = val;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiNoteOn::SetEventValue2(unsigned char  val)
{
	m_cMidiVelocity = val;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiNoteOn::SetChannelAndCode(unsigned char cChannel)
{
	cChannel			= cChannel & CHANNEL_MASK;
	m_cMidiEventChannel = cChannel;
	m_cMidiEventCode	= m_cMidiEventCode & OPERATION_MASK;
	m_cMidiEventCode	= m_cMidiEventCode | m_cMidiEventChannel;

}
