#include "stdafx.h"
#include "MidiBinary.h"
//#include "MidiText.h"

#include <Windows.h>

IMPLEMENT_DYNAMIC(CMidiBinary, CMidiEvent)

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
CMidiBinary::CMidiBinary(unsigned char cCommand, unsigned char *pBuffer)
{
	m_MidiBinaryBuffer			= NULL;
	m_iMidiBinaryLength			= 0;

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
	m_iMidiBinaryLength	= ConvertText2Value ( pBuffer );
	if ( m_iMidiBinaryLength > 0x1000 )
	{
		m_iMidiBinaryLength = 1;
	}
	pBuffer += m_iMidiClassLength;

	m_MidiBinaryBuffer = ( unsigned char * ) malloc ( m_iMidiBinaryLength + 1 );
	ZeroMemory ( m_MidiBinaryBuffer, m_iMidiBinaryLength + 1 );

	memcpy ( ( char * ) m_MidiBinaryBuffer, ( char * ) pBuffer, m_iMidiBinaryLength );
	pBuffer	+= m_iMidiBinaryLength;

	m_MidiEventEndOfBuffer		= pBuffer;

}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
CMidiBinary::CMidiBinary ( const unsigned char *pBuffer, const int iSize )
{
	m_MidiBinaryBuffer			= NULL;
	m_iMidiBinaryLength			= 0;

	m_MidiEventStartOfBuffer	= NULL;

	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//	Now set the Sequence subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_TEXT;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_TEXT;

	if ( iSize > 0 )
	{
		m_iMidiBinaryLength			= iSize;

		m_MidiBinaryBuffer = ( unsigned char * ) malloc ( m_iMidiBinaryLength + 1 );
		ZeroMemory ( m_MidiBinaryBuffer, m_iMidiBinaryLength + 1 );

		memcpy ( ( char * ) m_MidiBinaryBuffer, ( char * ) pBuffer, m_iMidiBinaryLength );

		m_MidiEventEndOfBuffer		= NULL;
	}
	else
	{
		SetText ( pBuffer );
	}
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
CMidiBinary::CMidiBinary ( const unsigned char *pBuffer )
{
	m_MidiBinaryBuffer			= NULL;
	m_iMidiBinaryLength			= 0;

	m_MidiEventStartOfBuffer	= NULL;

	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//	Now set the Sequence subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_TEXT;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_TEXT;

	SetText ( pBuffer );

}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
CMidiBinary::~CMidiBinary(void)
{
	Free ( );
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiBinary::GetBinaryBuffer() const
{
	return m_MidiBinaryBuffer;
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
const unsigned long CMidiBinary::GetBinaryBufferLength() const
{
	return m_iMidiBinaryLength;
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiBinary::Save ( FILE *hFile)
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
		if ( m_MidiBinaryBuffer != NULL )
		{
			int			iClassLength;

			iClassLength = GetMidiClassLength ( m_iMidiBinaryLength );
			if ( iClassLength == 0 )
			{
				//		Force abort
				m_MidiBinaryBuffer = NULL;
				*m_MidiBinaryBuffer = ' ';
			}

			fwrite ( ConvertValue2Text ( m_iMidiBinaryLength ), 1, iClassLength, hFile );
			if ( m_iMidiBinaryLength > 0 )
			{
				fwrite ( m_MidiBinaryBuffer, 1, m_iMidiBinaryLength, hFile );
			}
		}
		else
		{
			fwrite ( ConvertValue2Text ( 0 ), 1, GetMidiClassLength ( 0 ), hFile );
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiBinary::Free()
{
	if ( m_MidiBinaryBuffer != NULL )
	{
		free ( m_MidiBinaryBuffer );
		m_MidiBinaryBuffer = NULL;
	}
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiBinary::SetText ( const unsigned char *pBuffer )
{
	if ( m_MidiBinaryBuffer != NULL )
	{
		free ( m_MidiBinaryBuffer );
		m_MidiBinaryBuffer = NULL;
	}

	//
	m_iMidiBinaryLength = 0;

	//	First Step Count Bytes
	//	Second Step Store Bytes
	for ( int step = 0; step <= 1; step++ )
	{
		if ( step == 1 )
		{
			m_MidiBinaryBuffer = ( unsigned char * ) malloc ( m_iMidiBinaryLength + 1 );
			ZeroMemory ( m_MidiBinaryBuffer, m_iMidiBinaryLength + 1 );
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
						m_iMidiBinaryLength++;
					}
					hexacount = 0;
				}
				else if ( step == 1 )
				{
					//	Store 
					if ( hexacount > 1 )
					{
						m_MidiBinaryBuffer [ index ] = value;
						index++;
					}
				}
				//	Reset Value
				value = 0;
			}
		}
	}

	//
	if ( m_MidiBinaryBuffer == NULL )
	{
		m_iMidiBinaryLength = 1;
		m_MidiBinaryBuffer = ( unsigned char * ) malloc ( m_iMidiBinaryLength + 1 );
		ZeroMemory ( m_MidiBinaryBuffer, m_iMidiBinaryLength + 1 );
		m_MidiBinaryBuffer [ 0 ] = 0x00;
	}
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiBinary::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_TEXT ) );
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
unsigned char *CMidiBinary::GetTextAddress( unsigned short cSubCode ) const
{
	return NULL;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiBinary::Trace( FILE *hFile)
{
	unsigned int					iX;

	if ( hFile != NULL )
	{
		if ( m_MidiBinaryBuffer != NULL )
		{
			fprintf ( hFile, "%s:\t", GetEventSmallText ( MIDI_EVENT_TEXT ) );
			for ( iX = 0; iX < m_iMidiBinaryLength; iX++ )
			{
				fprintf ( hFile, " %02x", m_MidiBinaryBuffer [ iX ] );
			}
			fprintf ( hFile, "\n" );
		}
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
const char * CMidiBinary::GetExtraText() const
{
	unsigned int					iX;
	unsigned int					iLen;

	static	char	szBinaryText [ MAX_LONG_STRING ];
	memset ( szBinaryText, 0, sizeof ( szBinaryText ) );

	for ( iX = 0; iX < m_iMidiBinaryLength; iX++ )
	{
		iLen = (int) strlen ( szBinaryText );
		if ( iLen < sizeof ( szBinaryText ) - 8 )
		{
			sprintf_s ( szBinaryText + iLen, sizeof ( szBinaryText ) - iLen, "%02x ", m_MidiBinaryBuffer [ iX ] );
		}
		else
		{
			strcat_s ( szBinaryText, sizeof ( szBinaryText ), "..." );
			return szBinaryText;
		}
	}

	return szBinaryText;
}

IMPLEMENT_DYNAMIC(CMidiSequence, CMidiBinary)

//
/////////////////////////////////////////////////////////////////////////////////////////
//		Sequence
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiSequence::CMidiSequence( unsigned char cCommand, unsigned char *pBuffer ) : CMidiBinary(cCommand,  pBuffer )
{
	m_MidiEventStartOfBuffer	= pBuffer;

	//
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//		Check event code.
	if ( cCommand != m_cMidiEventCode )
	{
		EventMessageBox("CMidiSequence Code Differ");
	}

	//	Now set the Sequence subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_SEQ_1;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_SEQUENCE;

	if ( *pBuffer != m_cMidiEventSubCode )
	{
		EventMessageBox("CMidiSequence SubCode Differ");
	}
	pBuffer++;

}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiSequence::CMidiSequence( const unsigned char *pBuffer, const int iSize ) : CMidiBinary ( pBuffer, iSize)
{
	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//	Now set the Sequence subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_SEQ_1;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_SEQUENCE;
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiSequence::CMidiSequence( const unsigned char *pBuffer ) : CMidiBinary ( pBuffer )
{
	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//	Now set the Sequence subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_SEQ_1;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_SEQUENCE;
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiSequence::~CMidiSequence()
{
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiSequence::SetText ( const unsigned char *pBuffer )
{
	CMidiBinary::SetText ( pBuffer );
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiSequence::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_SEQUENCE ) );
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiSequence::Trace( FILE *hFile)
{
	unsigned int					iX;

	if ( hFile != NULL )
	{
		if ( m_MidiBinaryBuffer != NULL )
		{
			fprintf ( hFile, "%s:\t", GetEventSmallText ( MIDI_EVENT_SEQUENCE ) );
			for ( iX = 0; iX < m_iMidiBinaryLength; iX++ )
			{
				fprintf ( hFile, " %02x", m_MidiBinaryBuffer [ iX ] );
			}
			fprintf ( hFile, "\n" );
		}
	}

}

IMPLEMENT_DYNAMIC(CMidiSequencer, CMidiBinary)

//
/////////////////////////////////////////////////////////////////////////////////////////
//		Sequencer
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiSequencer::CMidiSequencer( unsigned char cCommand, unsigned char *pBuffer ) : CMidiBinary(cCommand,  pBuffer )
{
	m_MidiEventStartOfBuffer	= pBuffer;

	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//		Check event code.
	if ( cCommand != m_cMidiEventCode )
	{
		EventMessageBox("CMidiSequencer Code Differ");
	}

	//	Now set the Sequencer subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_SEQR;

	if ( *pBuffer != m_cMidiEventSubCode )
	{
		EventMessageBox("CMidiSequencer SubCode Differ");
	}
	pBuffer++;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_SEQUENCER;

}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiSequencer::CMidiSequencer( const unsigned char *pBuffer, const int iSize ) : CMidiBinary( pBuffer, iSize )
{
	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//	Now set the Sequencer subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_SEQR;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_SEQUENCER;

}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiSequencer::CMidiSequencer( const unsigned char *pBuffer ) : CMidiBinary ( pBuffer )
{
	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//	Now set the Sequencer subcode.
	m_cMidiEventSubCode			= MIDI_EVENT_CODE_META_SEQR;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_SEQUENCER;

}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiSequencer::~CMidiSequencer()
{
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiSequencer::SetText ( const unsigned char *pBuffer )
{
	CMidiBinary::SetText ( pBuffer );
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiSequencer::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_SEQUENCER ) );
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiSequencer::Trace( FILE *hFile)
{
	unsigned int					iX;

	if ( hFile != NULL )
	{
		if ( m_MidiBinaryBuffer != NULL )
		{
			fprintf ( hFile, "%s:\t", GetEventSmallText ( MIDI_EVENT_SEQUENCER ) );
			for ( iX = 0; iX < m_iMidiBinaryLength; iX++ )
			{
				fprintf ( hFile, " %02x", m_MidiBinaryBuffer [ iX ] );
			}
			fprintf ( hFile, "\n" );
		}
	}
}
