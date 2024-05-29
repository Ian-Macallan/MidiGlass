#include "stdafx.h"
#include "MidiText.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <stdio.h>

IMPLEMENT_DYNAMIC(CMidiEndOfTrack, CMidiEvent)

//
////////////////////////////////////////////////////////////////////////////////////
//		End of Track
//
////////////////////////////////////////////////////////////////////////////////////
CMidiEndOfTrack::CMidiEndOfTrack ( unsigned char cCommand, unsigned char *pBuffer )
{
	m_MidiEventStartOfBuffer	= pBuffer;

	m_cMidiEventCode			= MIDI_EVENT_CODE_META;
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_EOT_1;
	m_cMidiEventSubCode2		= MIDI_EVENT_CODE_META_EOT_2;

	//		Set Event code
	if ( cCommand != m_cMidiEventCode )
	{
		EventMessageBox("CMidiEndOfTrack Code Differ");
	}

	if ( *pBuffer != m_cMidiEventSubCode )
	{
		EventMessageBox("CMidiEndOfTrack SubCode Differ");
	}
	pBuffer++;

	if ( *pBuffer != m_cMidiEventSubCode2 )
	{
		EventMessageBox("CMidiEndOfTrack SubCode2 Differ");
	}
	pBuffer++;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_ENDOFTRACK;

	m_MidiEventEndOfBuffer		= pBuffer;
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
CMidiEndOfTrack::CMidiEndOfTrack(  )
{
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_EOT_1;
	m_cMidiEventSubCode2		= MIDI_EVENT_CODE_META_EOT_2;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_ENDOFTRACK;

}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
CMidiEndOfTrack::~CMidiEndOfTrack()
{
	Free ();
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMidiEndOfTrack::Save( FILE *hFile)
{
	if ( hFile != NULL )
	{
		fwrite ( &m_cMidiEventCode, 1, 1, hFile );
		fwrite ( &m_cMidiEventSubCode, 1, 1, hFile );
		fwrite ( &m_cMidiEventSubCode2, 1, 1, hFile );
	}
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiEndOfTrack::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_ENDOFTRACK ) );
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMidiEndOfTrack::Trace( FILE *hFile)
{
	if ( hFile != NULL )
	{
		fprintf ( hFile, "%s\n", GetEventSmallText ( MIDI_EVENT_ENDOFTRACK ) );
	}
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMidiEndOfTrack::Free()
{
}

