#include "stdafx.h"
#include "MidiText.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <stdio.h>

IMPLEMENT_DYNAMIC(CMidiWheel, CMidiEvent)

//
////////////////////////////////////////////////////////////////////////
//		Wheel 
//
////////////////////////////////////////////////////////////////////////
CMidiWheel::CMidiWheel ( unsigned char cCommand, unsigned char *pBuffer )
{
	m_MidiEventStartOfBuffer	= pBuffer;

	m_cMidiEventCode			= MIDI_EVENT_CODE_WHEEL;
	m_cMidiEventChannel			= 0xff;
	m_cMidiValue1				= 0x00;
	m_cMidiValue2				= 0x00;

	//		Set Event code
	if ( ( cCommand & OPERATION_MASK ) != m_cMidiEventCode )
	{
		EventMessageBox("CMidiWheel Code Differ");
	}

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_WHEEL;

	m_cMidiEventChannel			= cCommand & CHANNEL_MASK;

	m_cMidiEventCode			|= ( m_cMidiEventChannel & CHANNEL_MASK );

	m_cMidiValue1				= *pBuffer;
	pBuffer++;

	m_cMidiValue2				= *pBuffer;
	pBuffer++;

	m_MidiEventEndOfBuffer		= pBuffer;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiWheel::CMidiWheel ( unsigned int iChannel, unsigned char cVal1, unsigned char cVal2 )
{
	m_cMidiEventCode			= MIDI_EVENT_CODE_WHEEL;
	m_cMidiEventChannel			= iChannel;
	m_cMidiValue1				= cVal1;
	m_cMidiValue2				= cVal2;

	//		Set Event code.
	m_cMidiEventCode			|= ( m_cMidiEventChannel & CHANNEL_MASK );

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_WHEEL;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiWheel::~CMidiWheel ()
{
	Free ();
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiWheel::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_WHEEL ) );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiWheel::Trace( FILE *hFile)
{
	if ( hFile != NULL )
	{
		fprintf ( hFile, "Channel: %2d - %s: %d, %d\n", 
			m_cMidiEventChannel, 
			GetEventSmallText ( MIDI_EVENT_WHEEL ),
			m_cMidiValue1, 
			m_cMidiValue2 );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiWheel::Save( FILE *hFile)
{
	if ( hFile != NULL )
	{
		fwrite ( &m_cMidiEventCode, 1, 1, hFile );
		fwrite ( &m_cMidiValue1, 1, 1, hFile );
		fwrite ( &m_cMidiValue2, 1, 1, hFile );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
DWORD CMidiWheel::GetChunk()
{
	m_dwMidiChunk = m_cMidiEventCode + 256 * ( m_cMidiValue1 + 256 * m_cMidiValue2 );
	return m_dwMidiChunk;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiWheel::Free ()
{
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiWheel::GetEventValue1() const
{
	return m_cMidiValue1;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiWheel::GetEventValue2() const
{
	return m_cMidiValue2;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiWheel::SetEventValue1(unsigned char  val)
{
	m_cMidiValue1 = val;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiWheel::SetEventValue2(unsigned char  val)
{
	m_cMidiValue2 = val;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiWheel::SetChannelAndCode(unsigned char cChannel)
{
	cChannel			= cChannel & CHANNEL_MASK;
	m_cMidiEventChannel = cChannel;
	m_cMidiEventCode	= m_cMidiEventCode & OPERATION_MASK;
	m_cMidiEventCode	= m_cMidiEventCode | m_cMidiEventChannel;
}
