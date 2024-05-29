#include "stdafx.h"
#include "MidiText.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <stdio.h>

IMPLEMENT_DYNAMIC(CMidiStartOfTrack, CMidiEvent)

//
////////////////////////////////////////////////////////////////////////
//		Start of Track
//
////////////////////////////////////////////////////////////////////////
CMidiStartOfTrack::CMidiStartOfTrack( unsigned char cCommand, unsigned char *pBuffer )
{
	m_MidiEventStartOfBuffer	= pBuffer;
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_START_1;
	m_cMidiEventSubCode2		= MIDI_EVENT_CODE_META_START_2;

	m_cMidiPort					= 0x00;

	//		Set Event code
	if ( cCommand != m_cMidiEventCode )
	{
		EventMessageBox("CMidiStartOfTrack Code Differ");
	}

	if ( *pBuffer != m_cMidiEventSubCode )
	{
		EventMessageBox("CMidiStartOfTrack SubCode Differ");
	}
	pBuffer++;

	if ( *pBuffer != m_cMidiEventSubCode2 )
	{
		EventMessageBox("CMidiStartOfTrack SubCode2 Differ");
	}
	pBuffer++;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_STARTTRACK;

	m_cMidiPort					= ( *pBuffer );
	pBuffer++;

	//
	m_MidiEventEndOfBuffer		= pBuffer;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiStartOfTrack::CMidiStartOfTrack( unsigned char cPort )
{
	m_MidiEventStartOfBuffer	= NULL;
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_START_1;
	m_cMidiEventSubCode2		= MIDI_EVENT_CODE_META_START_2;

	m_cMidiPort					= cPort;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_STARTTRACK;

	m_MidiEventEndOfBuffer		= NULL;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiStartOfTrack::~CMidiStartOfTrack()
{
	Free ();
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiStartOfTrack::GetEventValue1() const
{
	return m_cMidiPort;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiStartOfTrack::SetEventValue1(unsigned char cPort)
{
	m_cMidiPort = cPort;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiStartOfTrack::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_STARTTRACK ) );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiStartOfTrack::Trace( FILE *hFile)
{
	if ( hFile != NULL )
	{
		fprintf ( hFile, "%s - Port: %d\n",
			GetEventSmallText ( MIDI_EVENT_STARTTRACK ),
			m_cMidiPort );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiStartOfTrack::Save( FILE *hFile)
{
	if ( hFile != NULL )
	{
		fwrite ( &m_cMidiEventCode, 1, 1, hFile );
		fwrite ( &m_cMidiEventSubCode, 1, 1, hFile );
		fwrite ( &m_cMidiEventSubCode2, 1, 1, hFile );
		fwrite ( &m_cMidiPort, 1, 1, hFile );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiStartOfTrack::Free()
{
}

IMPLEMENT_DYNAMIC(CMidiStartOfTrack20, CMidiEvent)

//
////////////////////////////////////////////////////////////////////////
//		Start of Track
//
////////////////////////////////////////////////////////////////////////
CMidiStartOfTrack20::CMidiStartOfTrack20( unsigned char cCommand, unsigned char *pBuffer ) : CMidiStartOfTrack ( cCommand, pBuffer )
{
	m_MidiEventStartOfBuffer	= pBuffer;

	m_cMidiEventCode			= MIDI_EVENT_CODE_META;
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_ST20_1;
	m_cMidiEventSubCode2		= MIDI_EVENT_CODE_META_ST20_2;
	m_cMidiPort					= 0x00;


	//		Set Event code
	if ( cCommand != m_cMidiEventCode )
	{
		EventMessageBox("CMidiStartOfTrack20 Code Differ");
	}

	if ( *pBuffer != m_cMidiEventSubCode )
	{
		EventMessageBox("CMidiStartOfTrack20 SubCode Differ");
	}
	pBuffer++;

	if ( *pBuffer != m_cMidiEventSubCode2 )
	{
		EventMessageBox("CMidiStartOfTrack SubCode2 Differ");
	}
	pBuffer++;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_STARTTRACK20;

	m_cMidiPort					= ( *pBuffer );

	pBuffer++;
	m_MidiEventEndOfBuffer		= pBuffer;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiStartOfTrack20::CMidiStartOfTrack20( unsigned char cPort ) : CMidiStartOfTrack ( cPort )
{
	m_MidiEventStartOfBuffer	= NULL;

	m_cMidiEventCode			= MIDI_EVENT_CODE_META;
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_ST20_1;
	m_cMidiEventSubCode2		= MIDI_EVENT_CODE_META_ST20_2;

	m_cMidiPort					= cPort;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_STARTTRACK20;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiStartOfTrack20::~CMidiStartOfTrack20()
{
	Free ();
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiStartOfTrack20::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_STARTTRACK20 ) );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiStartOfTrack20::Trace( FILE *hFile)
{
	if ( hFile != NULL )
	{
		fprintf ( hFile, "%s - Port: %d\n",
			GetEventSmallText ( MIDI_EVENT_STARTTRACK20 ),
			m_cMidiPort );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiStartOfTrack20::Free()
{
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiStartOfTrack20::GetEventValue1() const
{
	return m_cMidiPort;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiStartOfTrack20::SetEventValue1(unsigned char cPort)
{
	m_cMidiPort = cPort;
}

