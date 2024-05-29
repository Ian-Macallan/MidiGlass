//
/////////////////////////////////////////////////////////////////////////////////////////
// MidiText.cpp: implementation of the CMidiUnknown class.
//
/////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MidiText.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <stdio.h>

IMPLEMENT_DYNAMIC(CMidiUnknown, CMidiEvent)

//
/////////////////////////////////////////////////////////////////////////////////////////
//		Unknown
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiUnknown::CMidiUnknown ( unsigned char cCommand, unsigned char *pBuffer )
{
	m_MidiEventStartOfBuffer	= pBuffer;

	//		Set event code.
	m_cMidiEventCode			= MIDI_EVENT_CODE_META;

	//		Check event code.
	if ( cCommand != m_cMidiEventCode )
	{
		EventMessageBox("CMidiUnknown Code Differ");
	}

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_UNKNOWN;

	m_cMidiEventChannel			= cCommand & CHANNEL_MASK;

	m_pMidiString				= NULL;
//	m_pMidiString				= ( unsigned char * ) malloc ( 1 );

	pBuffer++;
	m_MidiEventEndOfBuffer		= pBuffer;
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiUnknown::CMidiUnknown ( )
{
	m_MidiEventStartOfBuffer	= NULL;

	//		Set Event code
	m_cMidiEventCode			= MIDI_EVENT_UNKNOWN;

	//	Set the Event Index.
	m_iEventIndex				= MIDI_EVENT_UNKNOWN;

	m_cMidiEventChannel			= 0xff;

	m_pMidiString				= NULL;

	m_MidiEventEndOfBuffer		= NULL;
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiUnknown::GetEventText() const
{
	return ( GetEventSmallText ( MIDI_EVENT_UNKNOWN ) );
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiUnknown::Trace( FILE *hFile)
{
	if ( hFile != NULL )
	{
		fprintf ( hFile, "Channel: %2d - %s: %x\n", 
			m_cMidiEventChannel, 
			GetEventSmallText ( MIDI_EVENT_UNKNOWN ),
			m_cMidiEventCode );
	}
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiUnknown::Save( FILE *hFile)
{
	char				szTemp [ 2 ];
	char				szText [ MAX_SHORT_STRING ];
	int					iTextLength;
	unsigned char		cEventCode, cEventSubCode1;
	int			iClassLength;

	if ( hFile != NULL )
	{
		sprintf_s ( szText, sizeof ( szText ), "Warning !!! Unknown Event 0x%x", 
					m_cMidiEventCode );
		iTextLength = (int) strlen ( szText );
		//		Set event code of text.
		cEventCode		= 0xff;

		cEventSubCode1	= 0x01;

		szTemp [ 0 ] = cEventCode;
		szTemp [ 1 ] = cEventSubCode1;

		fwrite ( szTemp, 1, 2, hFile );
		if ( szText != NULL )
		{
			iClassLength = GetMidiClassLength ( iTextLength );

			fwrite ( ConvertValue2Text ( iTextLength ),
						1, iClassLength, hFile );
			if ( iTextLength > 0 )
			{
				fwrite ( szText, 1, iTextLength, hFile );
			}
		}
		else
		{
			fwrite ( ConvertValue2Text ( 0 ),
						1, GetMidiClassLength ( 0 ), hFile );
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
CMidiUnknown::~CMidiUnknown ()
{
	Free ();
}

//
/////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////
void CMidiUnknown::Free ()
{
	if ( m_pMidiString != NULL )
	{
		free ( m_pMidiString );
		m_pMidiString = NULL;
	}
}


