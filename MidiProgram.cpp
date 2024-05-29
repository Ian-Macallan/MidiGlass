#include "stdafx.h"
#include "MidiText.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <stdio.h>

IMPLEMENT_DYNAMIC(CMidiProgram, CMidiEvent)

//
////////////////////////////////////////////////////////////////////////
//		Program
//
////////////////////////////////////////////////////////////////////////
CMidiProgram::CMidiProgram ( unsigned char cCommand, unsigned char *pBuffer )
{
	m_MidiEventStartOfBuffer	= pBuffer;

	m_cMidiEventCode			= MIDI_EVENT_CODE_PROGRAM;
	m_cMidiEventChannel			= 0xff;
	m_cMidiProgram				= 0x00;

	//		Set Event code
	if ( ( cCommand & OPERATION_MASK ) != m_cMidiEventCode )
	{
		EventMessageBox("CMidiKeyPressure Code Differ");
	}

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_PROGRAM;

	m_cMidiEventChannel			= cCommand & CHANNEL_MASK;
	m_cMidiEventCode			|= ( m_cMidiEventChannel & CHANNEL_MASK );

	m_cMidiProgram				= *pBuffer;
	m_cMidiEventSubCode			= *pBuffer;

	pBuffer++;
	m_MidiEventEndOfBuffer		= pBuffer;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiProgram::CMidiProgram ( unsigned int iChannel, unsigned char cVal1 )
{
	m_cMidiEventCode			= MIDI_EVENT_CODE_PROGRAM;
	m_cMidiEventChannel			= iChannel;
	m_cMidiProgram				= cVal1;
	m_cMidiEventSubCode		= cVal1;

	//		Set Event code.
	m_cMidiEventCode			|= ( iChannel & CHANNEL_MASK );

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_PROGRAM;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiProgram::~CMidiProgram ()
{
	Free ();
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiProgram::Save( FILE *hFile)
{
	if ( hFile != NULL )
	{
		fwrite ( &m_cMidiEventCode, 1, 1, hFile );
		fwrite ( &m_cMidiProgram, 1, 1, hFile );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
DWORD CMidiProgram::GetChunk()
{
	m_dwMidiChunk = m_cMidiEventCode + 256 * m_cMidiProgram;
	return m_dwMidiChunk;
}

//
////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiProgram::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_PROGRAM ) );
}

//
////////////////////////////////////////////////////////////////////////
void CMidiProgram::Trace( FILE *hFile)
{
	if ( hFile != NULL )
	{
		fprintf ( hFile, 
			"Channel: %2d - %s '%s', (%d)\n", 
			m_cMidiEventChannel, 
			GetEventSmallText ( MIDI_EVENT_PROGRAM ),
			InstrumentText ( m_cMidiProgram ),
			m_cMidiProgram );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const char * CMidiProgram::GetExtraText() const
{
	return InstrumentText ( m_cMidiProgram );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiProgram::Free ()
{
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiProgram::GetEventValue1() const
{
	return m_cMidiProgram;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiProgram::SetEventValue1(unsigned char  val)
{
	m_cMidiProgram = val;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiProgram::SetChannelAndCode(unsigned char cChannel)
{
	cChannel			= cChannel & CHANNEL_MASK;
	m_cMidiEventChannel = cChannel;
	m_cMidiEventCode	= m_cMidiEventCode & OPERATION_MASK;
	m_cMidiEventCode	= m_cMidiEventCode | m_cMidiEventChannel;
}
