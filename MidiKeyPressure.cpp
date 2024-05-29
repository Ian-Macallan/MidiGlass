#include "stdafx.h"
#include "MidiText.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <stdio.h>

IMPLEMENT_DYNAMIC(CMidiKeyPressure, CMidiEvent)

//
////////////////////////////////////////////////////////////////////////
//		Key Pressure
//
////////////////////////////////////////////////////////////////////////
CMidiKeyPressure::CMidiKeyPressure ( unsigned char cCommand, unsigned char *pBuffer )
{
	m_MidiEventStartOfBuffer	= pBuffer;

	m_cMidiEventCode			= MIDI_EVENT_CODE_KEYPRES;
	m_cMidiEventChannel			= 0xff;
	m_cMidiPitch				= 0x00;
	m_cMidiPressure				= 0x00;

	//		Check Event code
	if ( ( cCommand & OPERATION_MASK ) != m_cMidiEventCode )
	{
		EventMessageBox("CMidiKeyPressure Code Differ");
	}

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_KEYPRESSURE;

	m_cMidiEventChannel			= cCommand & CHANNEL_MASK;

	//		Set Event code.
	m_cMidiEventCode			|= ( m_cMidiEventChannel & CHANNEL_MASK );

	//
	m_cMidiPitch				= *pBuffer;
	m_cMidiEventSubCode			= m_cMidiPitch;
	pBuffer++;

	m_cMidiPressure				= *pBuffer;
	pBuffer++;

	m_MidiEventEndOfBuffer		= pBuffer;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiKeyPressure::CMidiKeyPressure ( unsigned int iChannel, unsigned char cVal1, unsigned char cVal2 )
{
	m_cMidiEventCode			= MIDI_EVENT_CODE_KEYPRES;
	m_cMidiEventChannel			= iChannel & CHANNEL_MASK;
	m_cMidiPitch				= cVal1;
	m_cMidiEventSubCode			= cVal1;
	m_cMidiPressure				= cVal2;

	//		Set Event code.
	m_cMidiEventCode			|= ( m_cMidiEventChannel & CHANNEL_MASK );

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_KEYPRESSURE;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiKeyPressure::~CMidiKeyPressure ()
{
	Free ();
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiKeyPressure::Save( FILE *hFile)
{
	if ( hFile != NULL )
	{
		fwrite ( &m_cMidiEventCode, 1, 1, hFile );
		fwrite ( &m_cMidiPitch, 1, 1, hFile );
		fwrite ( &m_cMidiPressure, 1, 1, hFile );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
DWORD CMidiKeyPressure::GetChunk()
{
	m_dwMidiChunk = m_cMidiEventCode + 256 * ( m_cMidiPitch + 256 * m_cMidiPressure );
	return m_dwMidiChunk;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiKeyPressure::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_KEYPRESSURE ) );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiKeyPressure::Trace( FILE *hFile)
{
	if ( hFile != NULL )
	{
		fprintf ( hFile, 
			"Channel: %2d - %s, Pitch %d, Pressure %d\n", 
			m_cMidiEventChannel,
			GetEventSmallText ( MIDI_EVENT_KEYPRESSURE ),
			m_cMidiPitch, m_cMidiPressure );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiKeyPressure::Free ()
{
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiKeyPressure::GetEventValue1() const
{
	return m_cMidiPitch;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiKeyPressure::GetEventValue2() const
{
	return m_cMidiPressure;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiKeyPressure::SetEventValue1(unsigned char  val)
{
	m_cMidiPitch = val;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiKeyPressure::SetEventValue2(unsigned char  val)
{
	m_cMidiPressure = val;
}

