#include "stdafx.h"
#include "MidiText.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <stdio.h>

IMPLEMENT_DYNAMIC(CMidiParameter, CMidiEvent)

//
////////////////////////////////////////////////////////////////////////
//		Parameters
//
////////////////////////////////////////////////////////////////////////
CMidiParameter::CMidiParameter ( unsigned char cCommand, unsigned char *pBuffer )
{
	m_MidiEventStartOfBuffer	= pBuffer;

	m_cMidiEventCode			= MIDI_EVENT_CODE_PARAM;
	m_cMidiEventChannel			= 0xff;
	m_cMidiParameter			= 0x00;
	m_cMidiSetting				= 0x00;

	//		Check Event code
	if ( ( cCommand & OPERATION_MASK ) != m_cMidiEventCode )
	{
		EventMessageBox("CMidiKeyPressure Code Differ");
	}

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_PARAMETER;

	m_cMidiEventChannel			= cCommand & CHANNEL_MASK;

	//		Set Event code.
	m_cMidiEventCode			|= ( m_cMidiEventChannel & CHANNEL_MASK );

	m_cMidiParameter			= *pBuffer;
	m_cMidiEventSubCode			= *pBuffer;
	pBuffer++;

	m_cMidiSetting				= *pBuffer;
	pBuffer++;

	m_MidiEventEndOfBuffer		= pBuffer;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiParameter::CMidiParameter ( unsigned int iChannel, unsigned char cVal1, unsigned char cVal2 )
{
	m_cMidiEventCode			= MIDI_EVENT_CODE_PARAM;
	m_cMidiEventChannel			= iChannel & CHANNEL_MASK;
	m_cMidiParameter			= cVal1;
	m_cMidiEventSubCode			= cVal1;
	m_cMidiSetting				= cVal2;

	//	
	m_cMidiEventCode			|= ( m_cMidiEventChannel & CHANNEL_MASK );

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_PARAMETER;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiParameter::~CMidiParameter ()
{
	Free ();
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiParameter::Save( FILE *hFile)
{
	if ( hFile != NULL )
	{
		fwrite ( &m_cMidiEventCode, 1, 1, hFile );
		fwrite ( &m_cMidiParameter, 1, 1, hFile );
		fwrite ( &m_cMidiSetting, 1, 1, hFile );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
DWORD CMidiParameter::GetChunk()
{
	m_dwMidiChunk = m_cMidiEventCode + 256 * ( m_cMidiParameter + 256 * m_cMidiSetting );
	return m_dwMidiChunk;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiParameter::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_PARAMETER ) );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiParameter::Trace( FILE *hFile)
{
	if ( hFile != NULL )
	{
		fprintf ( hFile, 
			"Channel: %2d - '%s', (%d), Setting %d\n", 
			m_cMidiEventChannel,
			ControlerText ( m_cMidiParameter ),
			m_cMidiParameter,
			m_cMidiSetting );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const char * CMidiParameter::GetExtraText() const
{
	return ControlerText ( m_cMidiParameter );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiParameter::Free ()
{
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiParameter::GetEventValue1() const
{
	return m_cMidiParameter;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
unsigned char CMidiParameter::GetEventValue2() const
{
	return m_cMidiSetting;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiParameter::SetEventValue1(unsigned char  val)
{
	m_cMidiParameter = val;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiParameter::SetEventValue2(unsigned char  val)
{
	m_cMidiSetting = val;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiParameter::SetChannelAndCode(unsigned char cChannel)
{
	cChannel			= cChannel & CHANNEL_MASK;
	m_cMidiEventChannel = cChannel;
	m_cMidiEventCode	= m_cMidiEventCode & OPERATION_MASK;
	m_cMidiEventCode	= m_cMidiEventCode | m_cMidiEventChannel;

}
