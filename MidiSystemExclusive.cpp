#include "stdafx.h"
#include "MidiText.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <stdio.h>

IMPLEMENT_DYNAMIC(CMidiSystemExclusive, CMidiEvent)

//
////////////////////////////////////////////////////////////////////////
//		System Exclusive
//
////////////////////////////////////////////////////////////////////////
CMidiSystemExclusive::CMidiSystemExclusive( unsigned char cCommand, unsigned char *pBuffer )
{
	m_MidiEventStartOfBuffer	= pBuffer;
	m_cMidiEventCode			= MIDI_EVENT_CODE_SYSEX;
	m_iMidiSysexLength			= 0;
	m_MidiSysexBuffer			= NULL;
	m_MidiSysexFull				= NULL;

	//		Set event code
	if ( cCommand != m_cMidiEventCode )
	{
		EventMessageBox("CMidiSystemExclusive Code Differ");
	}

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_SYSEX;

	m_iMidiSysexLength	= ConvertText2Value ( pBuffer );
	pBuffer += m_iMidiClassLength;

	m_MidiSysexBuffer = ( unsigned char * ) malloc ( m_iMidiSysexLength + 1 );
	ZeroMemory ( m_MidiSysexBuffer,  m_iMidiSysexLength + 1 );

	memcpy ( ( char * ) m_MidiSysexBuffer, ( char * ) pBuffer, m_iMidiSysexLength );

	pBuffer						+= m_iMidiSysexLength;
	m_MidiEventEndOfBuffer		= pBuffer;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiSystemExclusive::CMidiSystemExclusive( const unsigned char *pText, unsigned int iLen )
{
	m_cMidiEventCode			= MIDI_EVENT_CODE_SYSEX;
	m_iMidiSysexLength			= iLen;
	m_MidiSysexBuffer			= NULL;
	m_MidiSysexFull				= NULL;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_SYSEX;

	m_MidiSysexBuffer = ( unsigned char * ) malloc ( m_iMidiSysexLength + 1 );
	ZeroMemory ( m_MidiSysexBuffer,  m_iMidiSysexLength + 1 );

	memcpy ( ( char * ) m_MidiSysexBuffer, ( char * ) pText, m_iMidiSysexLength );

}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMidiSystemExclusive::~CMidiSystemExclusive()
{
	Free ();
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiSystemExclusive::GetSysexBuffer()
{
	return m_MidiSysexBuffer;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
int CMidiSystemExclusive::GetSysexBufferLength()
{
	return m_iMidiSysexLength;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiSystemExclusive::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_SYSEX ) );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiSystemExclusive::Trace ( FILE *hFile )
{
	unsigned int					iX;

	if ( hFile != NULL )
	{
		if ( m_MidiSysexBuffer != NULL )
		{
			fprintf ( hFile, "%s:\t", GetEventSmallText ( MIDI_EVENT_SYSEX ) );
			for ( iX = 0; iX < m_iMidiSysexLength; iX++ )
			{
				fprintf ( hFile, " %02x", m_MidiSysexBuffer [ iX ] );
			}
			fprintf ( hFile, "\n" );
		}
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const char * CMidiSystemExclusive::GetExtraText() const
{
	unsigned int					iX;
	unsigned int					iLen;

	static	char	szExclusiveText [ MAX_LONG_STRING ];
	memset ( szExclusiveText, 0, sizeof ( szExclusiveText ) );

	for ( iX = 0; iX < m_iMidiSysexLength; iX++ )
	{
		iLen = (int) strlen ( szExclusiveText );
		if ( iLen < sizeof ( szExclusiveText ) - 8 )
		{
			sprintf_s ( szExclusiveText + iLen, sizeof ( szExclusiveText ) - iLen, "%02x ", m_MidiSysexBuffer [ iX ] );
		}
		else
		{
			strcat_s ( szExclusiveText, sizeof ( szExclusiveText ), "..." );
			return szExclusiveText;
		}
	}

	return szExclusiveText;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiSystemExclusive::Save( FILE *hFile)
{
	if ( hFile != NULL )
	{
		if ( m_MidiSysexBuffer != NULL )
		{
			fwrite ( &m_cMidiEventCode, 1, 1, hFile );
			fwrite ( ConvertValue2Text ( m_iMidiSysexLength ), 1, GetMidiClassLength (m_iMidiSysexLength), hFile );
			fwrite ( m_MidiSysexBuffer, 1, m_iMidiSysexLength, hFile );
		}
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiSystemExclusive::Free()
{
	if ( m_MidiSysexBuffer != NULL )
	{
		free ( m_MidiSysexBuffer );
		m_MidiSysexBuffer = NULL;
	}

	if ( m_MidiSysexFull != NULL )
	{
		free ( m_MidiSysexFull );
		m_MidiSysexFull = NULL;
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
MIDIHDR * CMidiSystemExclusive::GetSysexChunk()
{
	if ( m_MidiSysexFull == NULL )
	{
		m_MidiSysexFull = ( unsigned char * ) malloc (	m_iMidiSysexLength + GetMidiClassLength (m_iMidiSysexLength) + 1 );
	}

	m_MidiSysexFull [ 0 ] = m_cMidiEventCode;
	memcpy ( m_MidiSysexFull + 1, ConvertValue2Text ( m_iMidiSysexLength ), GetMidiClassLength (m_iMidiSysexLength) );
	memcpy ( m_MidiSysexFull + GetMidiClassLength (m_iMidiSysexLength), m_MidiSysexBuffer, m_iMidiSysexLength );

	memset ( &m_MidiSysexChunk, 0, sizeof ( m_MidiSysexChunk ) );
	m_MidiSysexChunk.lpData				= ( char * ) m_MidiSysexFull;
	m_MidiSysexChunk.dwBufferLength		= m_iMidiSysexLength + GetMidiClassLength (m_iMidiSysexLength) + 1;
	m_MidiSysexChunk.dwBytesRecorded	= m_MidiSysexChunk.dwBufferLength;

	if ( m_iMidiSysexLength != 0 )
	{
		return &m_MidiSysexChunk;
	}
	else
	{
		return NULL;
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiSystemExclusive::SetText ( const unsigned char *pBuffer )
{
	if ( m_MidiSysexBuffer != NULL )
	{
		free ( m_MidiSysexBuffer );
		m_MidiSysexBuffer = NULL;
	}

	//
	m_iMidiSysexLength = 0;

	//	First Step Count Bytes
	//	Second Step Store Bytes
	for ( int step = 0; step <= 1; step++ )
	{
		if ( step == 1 )
		{
			m_MidiSysexBuffer = ( unsigned char * ) malloc ( m_iMidiSysexLength + 1 );
			ZeroMemory ( m_MidiSysexBuffer, m_iMidiSysexLength + 1 );
		}

		//
		unsigned char value = 0;
		int index			= 0;
		int hexacount		= 0;

		//
		for ( size_t i = 0; i < strlen((char *)pBuffer) + 1; i++ )
		{
			if ( pBuffer [ i ] >= '0' &&  pBuffer [ i ] <= '9' )
			{
				value = value << 4;
				value += ( pBuffer [ i ] - '30' );
				hexacount++;
			}
			else if ( pBuffer [ i ] >= 'A' &&  pBuffer [ i ] <= 'F' )
			{
				value = value << 4;
				value += ( pBuffer [ i ] - 'A' + 10 );
				hexacount++;
			}
			else if ( pBuffer [ i ] >= 'a' &&  pBuffer [ i ] <= 'f' )
			{
				value = value << 4;
				value += ( pBuffer [ i ] - 'a' + 10 );
				hexacount++;
			}
			else
			{
				if ( step == 0 )
				{
					//	Count 
					if ( hexacount > 1 )
					{
						m_iMidiSysexLength++;
					}
					hexacount = 0;
				}
				else if ( step == 1 )
				{
					//	Store 
					if ( hexacount > 1 )
					{
						m_MidiSysexBuffer [ index ] = value;
						index++;
					}
				}
				//	Reset Value
				value = 0;
			}
		}
	}

	//
	if ( m_MidiSysexBuffer == NULL )
	{
		m_iMidiSysexLength = 1;
		m_MidiSysexBuffer = ( unsigned char * ) malloc ( m_iMidiSysexLength + 1 );
		ZeroMemory ( m_MidiSysexBuffer, m_iMidiSysexLength + 1 );
		m_MidiSysexBuffer [ 0 ] = 0xf7;
	}
}

