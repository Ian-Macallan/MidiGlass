//
////////////////////////////////////////////////////////////////////////////////////
// MidiFile.cpp: implementation of the CMidiFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MidiFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <stdio.h>
#include <stdlib.h>
#include <share.h>

#include "MidiGlassApp.h"
#include "WaitTreatment.h"

#include "strstr.h"

extern CMidiWorksApp	theApp;

//
////////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//
////////////////////////////////////////////////////////////////////////////////////
CMidiFile::CMidiFile()
{
	Init ();
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
CMidiFile::CMidiFile( const char *pFilename)
{
	Init ();

	ReadMidiFile ( pFilename );
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
CMidiFile::CMidiFile(char *pBuffer, int iLength)
{
	char			*pFilename = "MidiGlass.mid";

	Init ();

	//		First say we succeed
	SetMidiSuccess ();

	//		Set the filename.
	m_pMidiFilename			= ( char * ) malloc ( strlen ( pFilename ) + 1 );
	strcpy_s ( m_pMidiFilename, strlen ( pFilename ) + 1, pFilename );

	//		Get the size
	m_iMidiFileSize = iLength;

	//		Allocate a buffer
	m_pMidiFileBuffer = ( unsigned char * ) malloc ( iLength );

	if ( m_pMidiFileBuffer != NULL )
	{
		//		Read the file
		memcpy ( m_pMidiFileBuffer, pBuffer, iLength );
		SetHeaderInformation ();
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
CMidiFile::~CMidiFile()
{
	LLKillEventTimer ();

	Free ();
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::ReadMidiFile(const char *pFilename)
{
	unsigned int			iRead;

	//		First say we succeed
	SetMidiSuccess ();

	//		Set the filename.
	m_pMidiFilename			= ( char * ) malloc ( strlen ( pFilename ) + 1 );
	strcpy_s ( m_pMidiFilename, strlen ( pFilename ) + 1, pFilename );

	//	Search real file size and read file into the buffer.
	m_hMidiFile = _fsopen ( m_pMidiFilename, "rb", _SH_DENYNO );
	if  ( m_hMidiFile != NULL )
	{
		//		Position at end
		fseek( m_hMidiFile, 0l, SEEK_END );

		//		Get the size
		m_iMidiFileSize = ftell ( m_hMidiFile );

		//		Allocate a buffer
		m_pMidiFileBuffer = ( unsigned char * ) malloc ( m_iMidiFileSize );

		if ( m_pMidiFileBuffer != NULL )
		{
			//		Position at start
			fseek ( m_hMidiFile, 0l, SEEK_SET );

			//		Read the file
			iRead = (int) fread ( m_pMidiFileBuffer, 1, m_iMidiFileSize, m_hMidiFile );
			if ( iRead == m_iMidiFileSize )
			{
				SetHeaderInformation ();
			}
		}

		fclose ( m_hMidiFile );

		m_hMidiFile = NULL;

		SetMidiErrorText ( "Midi File has been read" );
	}
	else
	{
		SetMidiError ( );
		char	szErrorText [ 128 ];
		strerror_s ( szErrorText, sizeof ( szErrorText ), errno );
		SetMidiErrorText ( szErrorText );
	}

	SetWebAddress ( NULL );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetHeaderInformation()
{
	int					iX;
	unsigned			iMaximumTrackLength;
	unsigned char		*pFileBuffer;

	pFileBuffer = m_pMidiFileBuffer;

	while (	( strcmp ( ( char * ) pFileBuffer, "MThd" ) != 0 ) &&
			( m_iMidiFileSize > 0									) )
	{
		pFileBuffer++;
		m_iMidiFileSize--;
	}

	if ( m_iMidiFileSize == 0 )
	{
		return;
	}

	m_pMidiFileHeader		= ( unsigned char * ) malloc ( 5 );
	memset ( m_pMidiFileHeader, 0, 5 );
	memcpy ( m_pMidiFileHeader, pFileBuffer, 4 );

    m_iMidiFileLength		= Swap_Long_Endian ( pFileBuffer + 4 );
    m_iMidiFileFormat		= Swap_Short_Endian ( pFileBuffer + 8 );
	if (    m_iMidiFileFormat != 0 
		 || m_iMidiFileFormat != 1
		 || m_iMidiFileFormat != 2 )
	{
		m_iMidiFileFormat = 1;
	}

    m_iMidiFileMaxTracks	= Swap_Short_Endian ( pFileBuffer + 10 );
	if ( m_iMidiFileMaxTracks > MAX_TRACKS )
	{
		m_iMidiFileMaxTracks = MAX_TRACKS;
	}
    m_iMidiFileUsedTracks	= m_iMidiFileMaxTracks;
    m_iMidiFileDivisionLo	= * ( pFileBuffer + 12 );
    m_iMidiFileDivisionHi	= * ( pFileBuffer + 13 );

	//		Compute Midi File Position
	m_pMidiFilePosition		= pFileBuffer + 8 + m_iMidiFileLength;

	//		Do the following only if header is ok.
	if ( strcmp ( ( char * ) m_pMidiFileHeader, "MThd" ) == 0 )
	{
		unsigned char		*pBuffer;

		//		Treat all tracks
		pBuffer = m_pMidiFilePosition;

		for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
		{
			if ( pFileBuffer + m_iMidiFileSize > pBuffer )
			{
				iMaximumTrackLength = (unsigned int)(  ( pFileBuffer + m_iMidiFileSize ) - pBuffer );
				m_MidiFileTrack [ iX ].SetMaximumTrackLength ( 
					iMaximumTrackLength );
				pBuffer = m_MidiFileTrack [ iX ].ReadTrack ( iX, pBuffer );
			}
		}
	}
	else
	{
		SetMidiError ();
		SetMidiErrorText ( "No Midi Header MThd" );
	}
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::Init()
{
	int						iX;

	m_iMaximumMesure		= 0;
	m_pMidiFileHeader		= NULL;
    m_iMidiFileLength		= 6;
    m_iMidiFileSize			= 0;
    m_iMidiFileFormat		= 0;
    m_iMidiFileMaxTracks	= 0;
    m_iMidiFileUsedTracks	= 0;
    m_iMidiFileDivisionLo	= 2;
    m_iMidiFileDivisionHi	= 120;

	m_pMidiFilename			= NULL;
	m_pMidiFileBuffer		= NULL;
	m_hMidiFile				= NULL;
	m_pMidiFilePosition		= NULL;

	m_pSoundFont			= NULL;
	m_iSoundFont			= 0;

	m_pMidiTracename		= NULL;
	m_hTraceFile			= NULL;

	m_iTextTrackContext		= 0;
	m_pTextListContext		= NULL;

	m_wMidiDeviceID			= 0;
	m_bMidiDeviceOk			= false;
	m_dwMidiReturn			= 0;

	m_bEditChange			= false;

	for ( iX = 0; iX < MAX_CHANNELS; iX++ )
	{
		m_MidiChannels [ iX ].SetChannel ( iX );
	}

	memset ( ( void * ) &m_MidiOpenParms, 0, sizeof ( m_MidiOpenParms ) );
	memset ( ( void * ) &m_MidiPlayParms, 0, sizeof ( m_MidiPlayParms ) );
	memset ( ( void * ) &m_MidiStatusParms, 0, sizeof ( m_MidiStatusParms ) );

	//		Value is 4/4
	m_cMidiPieceNN			= 4;
	m_cMidiPieceDD			= 2;

	m_cMidiPieceTempo [ 0 ]	= 9;
	m_cMidiPieceTempo [ 1 ]	= 39;
	m_cMidiPieceTempo [ 2 ]	= 192;

	for  ( iX = 1; iX < MAX_TRACKS; iX++ )
	{
		m_MidiFileTrack [ iX ].SetMidiTrack0 ( 
			& m_MidiFileTrack [ 0 ], &m_cMidiPieceNN, &m_cMidiPieceDD,
			m_cMidiPieceTempo );
	}

	m_iMaximumHMSm			= 0;

	m_wLLDeviceID			= -1;
	m_bLLDeviceOk			= false;
	memset ( &m_LLMidiOutCaps, 0, sizeof ( m_LLMidiOutCaps ) );
	m_wLLResult				= 0;
	m_hLLMidiOut			= NULL;
	m_wLLReturn				= 0;
	m_bLLTimerOk			= false;
	m_wLLTimerID			= 0;
	m_iLLCurrentHMSm		= 0;
	m_bLLState				= MIDI_PLAYER_STOPPED;
	m_pLLStartChunk			= NULL;
	m_pLLCurrentChunk		= NULL;
	m_iLLStartingHMSm		= 0;
	m_iLLSpeedMul			= 1;
	m_iLLSpeedDiv			= 1;
	m_bLLPlayAndLoop		= false;
	m_hLLNotifyWnd			= NULL;

	m_bLLDeviceInpOk		= false;
	m_wLLDeviceInpID		= -1;
	m_hLLMidiInp			= NULL;
	memset ( &m_LLMidiInpCaps, 0, sizeof ( m_LLMidiInpCaps ) );
	m_pLLStartInputChunk	= NULL;
	m_pLLCurrentInputChunk	= NULL;
	m_iNumberOfInputChunk	= 0;
	m_bLLInputState			= MIDI_RECORDER_INACTIVE;

	m_iSysEx				= 1;
	m_bAddStartOfTrack		= 1;
	m_bAddEndOfTrack		= 1;
	m_bBankSelectLow		= 1;
	m_bBankSelectHigh		= 1;
	m_bFilter				= 1;
	m_bCorrectStartOfTrack	= 1;

	m_bSkipToFirstNote		= false;
	m_bDoSoundOff			= false;

	m_iMidiDevice			= -1;
	m_iMidiInpDevice		= -1;

	m_bImmediateChunk		= false;

	m_bMciPlaying			= false;

	m_Web_Address			= "";

	m_E_Mail				= "";

}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::Free()
{
	//		Stop Playing MCI
	MciStop ( );

	//		Stop Playing Midi
	LLStop ( );

	//		Stop Recording Midi
	LLInpStop ( );

	if ( m_pMidiFileHeader != NULL )
	{
		free ( m_pMidiFileHeader );
		m_pMidiFileHeader = NULL;
	}

	if ( m_pMidiFilename != NULL )
	{
		free ( m_pMidiFilename );
		m_pMidiFilename = NULL;
	}

	if ( m_pSoundFont != NULL )
	{
		free ( m_pSoundFont );
		m_pSoundFont = NULL;
	}

	if ( m_pMidiFileBuffer != NULL )
	{
		free ( m_pMidiFileBuffer );
		m_pMidiFileBuffer = NULL;
	}

	if ( m_hMidiFile != NULL )
	{
		fclose ( m_hMidiFile );
		m_hMidiFile = NULL;
	}

	if ( m_pMidiTracename != NULL )
	{
		free ( m_pMidiTracename );
		m_pMidiTracename = NULL;
	}

	if ( m_hTraceFile != NULL )
	{
		fclose ( m_hTraceFile );
		m_hTraceFile = NULL;
	}

	//		Then delete chunks
	if ( m_pLLStartChunk != NULL )
	{
		DeleteChunkList ();
	}

	if ( m_pLLStartInputChunk != NULL )
	{
		DeleteInputChunkList ();
	}

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::Trace(const char *pTracename)
{
	int					iX;

	//		First say we succeed
	SetMidiSuccess ();

	//		Update all values.
	Update ();

	//		Now Trace file
	m_pMidiTracename	= ( char * ) malloc ( strlen ( pTracename ) + 1 );
	strcpy_s ( m_pMidiTracename, strlen ( pTracename ) + 1, pTracename );

	m_hTraceFile = NULL;
	fopen_s ( &m_hTraceFile, m_pMidiTracename, "w" );
	if ( m_hTraceFile != NULL )
	{
		if ( m_pMidiFilename != NULL )
		{
			//		Write file informations
			fprintf ( m_hTraceFile, "Midi File:    \t'%s'\n", m_pMidiFilename );
			fprintf ( m_hTraceFile, "Length:       \t%6d\n", m_iMidiFileLength );
			fprintf ( m_hTraceFile, "Format:       \t%6d\n", m_iMidiFileFormat );
			fprintf ( m_hTraceFile, "Tracks:       \t%6d\n", m_iMidiFileUsedTracks );
			fprintf ( m_hTraceFile, "Division Low: \t%6d\n", m_iMidiFileDivisionLo );
			fprintf ( m_hTraceFile, "Division High:\t%6d\n", m_iMidiFileDivisionHi );

			//		Write tracks informations
			for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
			{
				m_MidiFileTrack [ iX ].Trace ( m_hTraceFile );
			}
		}

		fclose ( m_hTraceFile );
		m_hTraceFile = NULL;
	}
	else
	{
		SetMidiError ();
		char	szErrorText [ 128 ];
		strerror_s ( szErrorText, sizeof ( szErrorText ), errno );
		SetMidiErrorText ( szErrorText );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetMidiSignature()
{
	int					iX;

	for ( iX = 1; iX < m_iMidiFileMaxTracks; iX++ )
	{
		if ( m_MidiFileTrack [ iX ].GetTrackLength () != 0 )
		{
			m_MidiFileTrack [ iX ].SetMidiSignature ( );
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::ComputeMesure()
{
	int					iX;

	for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
	{
		if ( m_MidiFileTrack [ iX ].GetTrackLength () != 0 )
		{
			m_MidiFileTrack [ iX ].ComputeMesure ( m_iMidiFileDivisionHi, m_iMidiFileDivisionLo );
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::Save(const char *pFilename)
{
	int					iX;
	unsigned char		szTempLong [ 4 ];
	unsigned char		szTempShort [ 2 ];

	//		First say we succeed
	SetMidiSuccess ();

	//		Update all values
	Update ();

	//		Reset name
	if ( m_pMidiFilename != NULL )
	{
		free ( m_pMidiFilename );
		m_pMidiFilename = NULL;
	}

	//		If file opened close it
	if ( m_hMidiFile != NULL )
	{
		fclose ( m_hMidiFile );
		m_hMidiFile = NULL;
	}

	//		Allocate space for name
	m_pMidiFilename	= ( char * ) malloc ( strlen ( pFilename ) + 1 );
	strcpy_s ( m_pMidiFilename, strlen ( pFilename ) + 1, pFilename );

	//		Open new file
	m_hMidiFile = NULL;
	fopen_s ( &m_hMidiFile, m_pMidiFilename, "wb" );
	if ( m_hMidiFile != NULL )
	{
		//		Write file informations
		fwrite ( "MThd", 1, 4, m_hMidiFile );

		memcpy ( szTempLong, ( void *) &m_iMidiFileLength, 4 );
		Swap_Long_Endian ( szTempLong );
		fwrite ( szTempLong, 1, 4, m_hMidiFile );

		//		Set the format
		if ( m_iMidiFileUsedTracks == 1 )
		{
			m_iMidiFileFormat = 0;
		}
		else
		{
			m_iMidiFileFormat = 1;
		}

		memcpy ( szTempShort, ( void * ) &m_iMidiFileFormat, 2 );
		Swap_Short_Endian ( szTempShort );
		fwrite ( szTempShort, 1, 2, m_hMidiFile );

		//		Write tracks
		memcpy ( szTempShort, ( void * ) &m_iMidiFileUsedTracks, 2 );
		Swap_Short_Endian ( szTempShort );
		fwrite ( szTempShort, 1, 2, m_hMidiFile );

		fwrite ( ( char * ) &m_iMidiFileDivisionLo, 1, 1, m_hMidiFile );
		fwrite ( ( char * ) &m_iMidiFileDivisionHi, 1, 1, m_hMidiFile );

		//		Write tracks informations
		for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
		{
			if ( m_MidiFileTrack [ iX ].GetTrackLength () != 0 )
			{
				m_MidiFileTrack [ iX ].Save ( m_hMidiFile );
			}
		}

		fclose ( m_hMidiFile );
		m_hMidiFile = NULL;
	}
	else
	{
		SetMidiError ();
		char	szErrorText [ 128 ];
		strerror_s ( szErrorText, sizeof ( szErrorText ), errno );
		SetMidiErrorText ( szErrorText );
	}

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
short int CMidiFile::GetMidiFileFormat() const
{
	return ( m_iMidiFileFormat );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
short int CMidiFile::GetMidiFileTracksCount() const
{
	return m_iMidiFileMaxTracks;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned char CMidiFile::GetMidiFileDivisionLo() const
{
	return ( m_iMidiFileDivisionLo );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned char CMidiFile::GetMidiFileDivisionHi() const
{
	return ( m_iMidiFileDivisionHi );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Get Texts
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned char * CMidiFile::GetText( unsigned short cSubCode ) 
{
	unsigned char			*pString;
	int						iX;

	//		For All tracks
	for ( iX = m_iTextTrackContext; iX < m_iMidiFileMaxTracks; iX++ )
	{
		if ( m_MidiFileTrack [ iX ].GetTrackLength () != 0 )
		{
			//		First get the list address to position on
			//		start or previous entry.
			//		Set the Context Address List
			if ( m_pTextListContext == NULL )
			{
				m_pTextListContext = m_MidiFileTrack [ iX ].GetFirstAddress ( );
			}

			while ( m_pTextListContext != NULL )
			{
				pString = m_pTextListContext->GetTextAddress ( cSubCode );
				m_pTextListContext = m_MidiFileTrack [ iX ].GetNextAddress ( m_pTextListContext );
				if ( pString != NULL )
				{
					m_iTextTrackContext = iX;
					return ( pString );
				}

			}
		}
	}

	//		We have finished the list: reset values
	m_iTextTrackContext = 0;
	m_pTextListContext	= NULL;

	return ( NULL );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::ResetContext()
{
	m_iTextTrackContext = 0;
	m_pTextListContext	= NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned long CMidiFile::GetMidiTrackSize(unsigned int iTrack) const
{
	return m_MidiFileTrack [ iTrack].GetTrackLength ();
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
const unsigned char *CMidiFile::GetTrackEvent ( unsigned int iTrack ) 
{
	const unsigned char *pString;

	//		Set The Context Track
	m_iTextTrackContext = iTrack;

	//		Set the Context Address List
	if ( m_pTextListContext == NULL )
	{
		m_pTextListContext = 
			m_MidiFileTrack [ iTrack ].GetFirstAddress ( );
	}

	if ( m_pTextListContext != NULL )
	{
		pString = m_pTextListContext->GetEventText ( );
		return ( pString );
	}

	//		We have finished the list: reset values
	m_iTextTrackContext = 0;
	m_pTextListContext	= NULL;

	return ( NULL );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
CMidiList *CMidiFile::AdvanceTrackEvent()
{
	m_pTextListContext = 
		m_MidiFileTrack [ m_iTextTrackContext ].GetNextAddress ( m_pTextListContext );
	if ( m_pTextListContext == NULL )
	{
		//		We have finished the list: reset values
		m_iTextTrackContext = 0;
	}

	return ( m_pTextListContext );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned char * CMidiFile::GetMesureText() 
{
	//		Set the Context Address List
	if ( m_pTextListContext != NULL )
	{
		return ( m_pTextListContext->GetMesureText ( ) );
	}

	//		We have finished the list: reset values
	m_iTextTrackContext = 0;
	m_pTextListContext	= NULL;

	return ( NULL );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned char CMidiFile::GetChannel() 
{
	//		Set the Context Address List
	if ( m_pTextListContext != NULL )
	{
		return ( m_pTextListContext->GetChannel ( ) );
	}

	//		We have finished the list: reset values
	m_iTextTrackContext = 0;
	m_pTextListContext	= NULL;

	return ( NULL );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned char * CMidiFile::GetValues() 
{
	//		Set the Context Address List
	if ( m_pTextListContext != NULL )
	{
		return ( m_pTextListContext->GetValues ( ) );
	}

	//		We have finished the list: reset values
	m_iTextTrackContext = 0;
	m_pTextListContext	= NULL;

	return ( NULL );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned char * CMidiFile::GetTextAddress() 
{
	if ( m_pTextListContext != NULL )
	{
		return ( m_pTextListContext->GetTextAddress ( MASK_ALL ) );
	}

	//		We have finished the list: reset values
	m_iTextTrackContext = 0;
	m_pTextListContext	= NULL;

	return ( NULL );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
const char * CMidiFile::GetExtraText () 
{
	if ( m_pTextListContext != NULL )
	{
		return m_pTextListContext->GetExtraText ();
	}

	//		We have finished the list: reset values
	m_iTextTrackContext = 0;
	m_pTextListContext	= NULL;

	return ( NULL );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
CMidiList * CMidiFile::GetMidiListContext() const
{
	return m_pTextListContext;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
CMidiTrack * CMidiFile::GetMidiTrackAddress(unsigned int iTrack)
{
	return ( & ( m_MidiFileTrack [iTrack ] ) );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//	Add Start Of Track Starting with track 1
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::AddStartOfTrack()
{
	int					iX;

	//		Remove Start of Tracks
	RemoveEventCode ( MIDI_EVENT_STARTTRACK );

	//		Remove Start of Tracks 20
	RemoveEventCode ( MIDI_EVENT_STARTTRACK20 );

	//		For all track starting 1 add start of track
	for ( iX = 1; iX < m_iMidiFileMaxTracks; iX++ )
	{
		if ( m_MidiFileTrack [ iX ].GetTrackLength () != 0 )
		{
			m_MidiFileTrack [ iX ].AddStartOfTrack ();
		}
	}

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::CorrectStartOfTrack()
{
	//		Remove Start of Tracks on track0
	RemoveEventCode ( MIDI_EVENT_STARTTRACK, false, true );

	//		Remove Start of Tracks 20
	RemoveEventCode ( MIDI_EVENT_STARTTRACK20, false, true );

	//		Remove Start of Tracks on other tracks
	RemoveEventCode ( MIDI_EVENT_STARTTRACK, true, false, true );

	//		Remove Start of Tracks on other tracks
	RemoveEventCode ( MIDI_EVENT_STARTTRACK20, true, false, true );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::AddEndOfTrack()
{
	int					iX;

	//		Remove End Of Tracks
	RemoveEventCode ( MIDI_EVENT_ENDOFTRACK );

	//		For all tracks add end of track
	for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
	{
		if ( m_MidiFileTrack [ iX ].GetTrackLength () != 0 )
		{
			m_MidiFileTrack [ iX ].AddEndOfTrack ();
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetMidiFileDivisionHi(unsigned char cHi)
{
	m_iMidiFileDivisionHi = cHi;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetMidiFileDivisionLo(unsigned char cLo)
{
	m_iMidiFileDivisionLo = cLo;
}

void CMidiFile::SetMidiTempo()
{
	int					iX;

	for ( iX = 1; iX < m_iMidiFileMaxTracks; iX++ )
	{
		if ( m_MidiFileTrack [ iX ].GetTrackLength () != 0 )
		{
			m_MidiFileTrack [ iX ].SetMidiTempo ( );
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned char * CMidiFile::GetHMSText() 
{
	//		Set the Context Address List
	if ( m_pTextListContext != NULL )
	{
		return ( m_pTextListContext->GetHMSText ( ) );
	}

	//		We have finished the list: reset values
	m_iTextTrackContext = 0;
	m_pTextListContext	= NULL;

	return ( NULL );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::ComputeTempo()
{
	int					iX;

	for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
	{
		if ( m_MidiFileTrack [ iX ].GetTrackLength () != 0 )
		{
			m_MidiFileTrack [ iX ].ComputeTempo ( m_iMidiFileDivisionHi, m_iMidiFileDivisionLo );
		}
	}

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Update file for Mesure and Tempo
//		And All 
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::Update()
{
	//		Compute the number of tracks.
	m_iMidiFileMaxTracks	= ComputeMaxTracks ();

	//		Compute the number of tracks.
	m_iMidiFileUsedTracks	= ComputeUsedTracks ();

	//		Count the number of events
	CountEvents ();

	//		Set Midi signature in all piece.
	//		Midi Signature 4/2 or anything
	SetMidiSignature ();

	//		Set the tempo in all piece
	SetMidiTempo ();

	//		Compute Mesure
	ComputeMesure ();

	//		Compute Tempo
	ComputeTempo ();

	//		Set the largest value.
	SetMaximumHMSm();

	//		Compute Notes Duration
	ComputeNoteDuration ( );

	//		Set the largest value.
	SetMaximumMesure();
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned CMidiFile::ComputeMaxTracks()
{
	int			iX;
	int			iRes;

	iRes = 0;

	for ( iX = 0; iX < MAX_TRACKS; iX++ )
	{
		if ( m_MidiFileTrack [ iX ].GetTrackLength () != 0 )
		{
			iRes = iX + 1;
		}
	}

	return iRes;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned CMidiFile::ComputeUsedTracks()
{
	int			iX;
	int			iRes;

	iRes = 0;

	for ( iX = 0; iX < MAX_TRACKS; iX++ )
	{
		if ( m_MidiFileTrack [ iX ].GetTrackLength () != 0 )
		{
			iRes = iRes + 1;
		}
	}

	return iRes;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::DeleteInputChunkList()
{
	CMidiInputChunk	*pList;
	CMidiInputChunk	*pNext;

	pList = m_pLLStartInputChunk;

	while ( pList != NULL )
	{
		pNext = pList->GetNext ();
		delete	pList;
		pList = pNext;
	}

	m_iNumberOfInputChunk	= 0;

	m_pLLStartInputChunk	= NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::DeleteChunkList()
{
	CMidiPlayerChunk	*pList;
	CMidiPlayerChunk	*pNext;

	pList = m_pLLStartChunk;

	while ( pList != NULL )
	{
		pNext = pList->GetNext ();
		delete	pList;
		pList = pNext;
	}

	m_pLLStartChunk = NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetMaximumHMSm()
{
	int						iX;
	CMidiTrack				*pTrack;
	CMidiList				*pList;
	m_iMaximumHMSm = 0;

	for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
	{
		pTrack = & ( m_MidiFileTrack [ iX ] );
		if ( pTrack->GetTrackLength ( ) > 0 )
		{
			pList = pTrack->GetFirstAddress ( );
		}
		else
		{
			pList = NULL;
		}

		while ( pList != NULL )
		{
			if ( pList->GetAbsoluteHMSm () > m_iMaximumHMSm )
			{
				m_iMaximumHMSm = pList->GetAbsoluteHMSm ();
			}

			pList = pList->GetNext ();
		}

	}	//	For all tracks


}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned CMidiFile::GetMaximumHMSm() const
{
	return m_iMaximumHMSm;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Filter event by channels
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::FilterEventsTracks()
{
	int						iX;
	CMidiTrack				*pTrack;
	CMidiList				*pList;
	CMidiList				*pNext;
	CMidiList				*pFrom [ MAX_TRACKS ];
	int						iLastTrack;
	int						iFilterTrack;
	bool					bDoFilter;
	int						iOldChannel;
	int						iChannel;

	int						iCount;

	//		First Update all datas.
	Update ();

	for ( iX = 0; iX < MAX_TRACKS; iX++ )
	{
		pFrom [ iX ] = NULL;
	}

	//		Find the last track among all.
	iLastTrack = 0;
	for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
	{
		pTrack = & ( m_MidiFileTrack [ iX ] );
		if ( pTrack->GetTrackLength ( ) > 0 )
		{
			iLastTrack = iX;
		}

	}	//	For all tracks

	//		Scan last Track to see if it is necessary to
	//		do the filtering
	iFilterTrack = -1;
	bDoFilter = false;

	for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
	{
		if ( pTrack->GetTrackLength ( ) > 0 )
		{

			pTrack = & ( m_MidiFileTrack [ iX ] );
			pList = pTrack->GetFirstAddress ( );
			iOldChannel = -1;
			while ( pList != NULL )
			{
				pNext = pList->GetNext ();

				switch  ( pList->GetEventIndex() )
				{
					case MIDI_EVENT_NOTEOFF	:
					case MIDI_EVENT_NOTEON :
					case MIDI_EVENT_KEYPRESSURE	:
					case MIDI_EVENT_PARAMETER :
					case MIDI_EVENT_PROGRAM :
					case MIDI_EVENT_CHANNELPRES	:
					case MIDI_EVENT_WHEEL :
					{
						//		Look if there is different channels in the track
						iChannel = pList->GetChannel ( ) + 1;
						if ( iOldChannel == -1 )
						{
							iOldChannel  = iChannel;
						}

						if ( iChannel != iOldChannel )
						{
							bDoFilter		= true;
							iFilterTrack	= iX;
						}

						break;
					}

					default :
					{
						break;
					}

				}

				pList = pNext;

			}

		}

	}	//	For all tracks

	if ( m_iMidiFileMaxTracks == 1 )
	{
		iFilterTrack = 0;
		bDoFilter = true;
	}

	if ( ! bDoFilter )
	{
		return;
	}

	//		Now scan last Track
	pTrack = & ( m_MidiFileTrack [ iFilterTrack ] );
	pList = pTrack->GetFirstAddress ( );
	iCount = 0;
	while ( pList != NULL && iCount++ < MAX_LINES )
	{
		pNext = pList->GetNext ();

		switch  ( pList->GetEventIndex() )
		{
			case MIDI_EVENT_NOTEOFF	:
			case MIDI_EVENT_NOTEON :
			case MIDI_EVENT_KEYPRESSURE	:
			case MIDI_EVENT_PARAMETER :
			case MIDI_EVENT_PROGRAM :
			case MIDI_EVENT_CHANNELPRES	:
			case MIDI_EVENT_WHEEL :
			{
				//		Remove from current list without deleting it
				pTrack->RemoveEventFromList ( pList, true );

				//		Add this value at the end of the current track.
				iChannel = pList->GetChannel ( );
				m_MidiFileTrack [ iChannel + iLastTrack + 1 ].Append ( & pList, & ( pFrom [ iChannel + iLastTrack + 1 ] ) );

				break;
			}

			default :
			{
				break;
			}

		}

		pList = pNext;

	}

	Update ();

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Add GM Reset
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::AddGMReset()
{
	//		Remove System Exclusive Messages
	RemoveSystemExclusive ();

	//		Add GM Reset
	m_MidiFileTrack [ 0 ].AddGMReset ();
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Add GS Reset
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::AddGSReset()
{
	//		Remove System Exclusive Messages
	RemoveSystemExclusive ();

	//		Add GS Reset
	m_MidiFileTrack [ 0 ].AddGSReset ();
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//	Correct All Things
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::CorrectAll( unsigned iSysEx, 
		int iAddStartOfTrack, int iAddEndOfTrack,
		int	iBankSelectLow, int iBankSelectHigh,
		int	iFilter, int iCorrectStartOfTrack	)
{

	m_iSysEx					= iSysEx;
	m_bAddStartOfTrack			= iAddStartOfTrack;
	m_bAddEndOfTrack			= iAddEndOfTrack;
	m_bBankSelectLow			= iBankSelectLow;
	m_bBankSelectHigh			= iBankSelectHigh;
	m_bFilter					= iFilter;
	m_bCorrectStartOfTrack		= iCorrectStartOfTrack;

	//		Filter Events in Tracks
	if ( m_bFilter )
	{
		FilterEventsTracks();
	}

	//		Add Start of Track
	if ( m_bAddStartOfTrack )
	{
		AddStartOfTrack();
	}

	//		Correct Start of Track
	if ( m_bCorrectStartOfTrack )
	{
		CorrectStartOfTrack();
	}

	//		Remove Bank Select High
	if ( m_bBankSelectHigh )
	{
		RemoveEventCodeSubCode ( MIDI_EVENT_PARAMETER, CNTR_BANK_SELECT_MSB );
	}

	//		Remove Bank Select Low
	if ( m_bBankSelectLow )
	{
		RemoveEventCodeSubCode ( MIDI_EVENT_PARAMETER, CNTR_BANK_SELECT_LSB );
	}

	//		Add GM Reset
	switch  ( m_iSysEx  )
	{
		case 0 :
		{
			break;
		}

		case 1 :
		{
			AddGMReset ();
			break;
		}

		case 2 :
		{
			AddGSReset ();
			break;
		}

		case 3 :
		{
			AddXGReset ();
			break;
		}

		default :
		{
			break;
		}
	}

	//		Add End of Track
	if ( m_bAddEndOfTrack )
	{
		AddEndOfTrack();
	}

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Remove system exclusive
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::RemoveSystemExclusive()
{
	RemoveEventCode ( MIDI_EVENT_SYSEX );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Remove an Event Code
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::RemoveEventCode (	unsigned int iType, 
									bool bSkipTrack0, bool bTrack0Only,
									bool bSkipFirstEvent, bool bFirstEventOnly )
{
	int						iX;

	//		Update necessary values
	Update ( );

	//
	int	iFirstTrack	= 0;
	int iLastTrack	= m_iMidiFileMaxTracks;
	if ( bSkipTrack0 )
	{
		iFirstTrack = 1;
	}

	if ( bTrack0Only )
	{
		iLastTrack = 1;
	}

	//		Check if there are other tracks.
	for ( iX = iFirstTrack; iX < iLastTrack; iX++ )
	{
		m_MidiFileTrack [ iX ].RemoveEventCode ( iType, bSkipFirstEvent, bFirstEventOnly );
	}	//	For all tracks

	//		Update necessary values
	Update ( );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Remove an Event by Type and Subtype
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::RemoveEventCodeSubCode(unsigned int iType, 
									   unsigned char cSubType)
{
	int						iX;
	CMidiTrack				*pTrack;

	//		Update necessary values
	Update ( );

	//		Check if there are other tracks.
	for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
	{
		pTrack = & ( m_MidiFileTrack [ iX ] );
		pTrack->RemoveEventCodeSubCode ( iType, cSubType );
	}	//	For all tracks

	//		Update necessary values
	Update ( );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetMidiDevice(int iDevice)
{
	m_iMidiDevice = iDevice - 1;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetSoundFont(const char *pSoundFont, int iSoundFont )
{
	if ( m_pSoundFont == NULL )
	{
		m_pSoundFont = ( char * ) malloc ( MAX_PATH );
	}

	strcpy_s ( m_pSoundFont, MAX_PATH, pSoundFont );
	m_iSoundFont	= iSoundFont;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetMidiInpDevice(int iDevice)
{
	m_iMidiInpDevice = iDevice - 1;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::CountEvents()
{
	int						iX;

	//		Check if there are other tracks.
	for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
	{
		m_MidiFileTrack [ iX ].CountEvents ( );
	}

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::UpdateTrackNumber()
{
	int						iX;
	CMidiTrack				*pTrack;

	//		Check if there are other tracks.
	for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
	{
		pTrack = & ( m_MidiFileTrack [ iX ] );
		pTrack->UpdateTrackNumber ( );
	}

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::MoveEventToTrack(unsigned int iType, int iTrack)
{
	int						iX;
	CMidiTrack				*pTrack;
	CMidiList				*pList;
	CMidiList				*pNext;
	CMidiList				*pFrom;

	//		First Update all datas.
	Update ();

	//		Now scan all tracks
	if ( iType == MIDI_EVENT_ALL_EVENTS )
	{
		for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
		{
			pTrack = & ( m_MidiFileTrack [ iX ] );
			pFrom	= m_MidiFileTrack [ iTrack ].GetFirstAddress ();

			if ( pTrack->GetTrackLength ( ) > 0 && iX != iTrack )
			{

				pList = pTrack->GetFirstAddress ( );
				while ( pList != NULL )
				{
					pNext = pList->GetNext ();

					//		Remove from current list without deleting it
					pTrack->RemoveEventFromList ( pList, true );

					//		Add this value at the correct position in track
					m_MidiFileTrack [ iTrack ].InsertAfter ( & pList, &pFrom );

					pList = pNext;

				}

			}

		}
	}
	else
	{
		for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
		{
			pTrack = & ( m_MidiFileTrack [ iX ] );

			if ( pTrack->GetTrackLength ( ) > 0 && iX != iTrack )
			{

				pList = pTrack->GetFirstAddress ( );
				while ( pList != NULL )
				{
					pNext = pList->GetNext ();

					if ( pList->GetEventIndex() == iType )
					{
						//		Remove from current list without deleting it
						pTrack->RemoveEventFromList ( pList, true );

						//		Add this value at the correct position in track
						m_MidiFileTrack [ iTrack ].Insert ( & pList );
					}

					pList = pNext;

				}

			}

		}
	}

	AddEndOfTrack ();

	Update ();

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CMidiFile::GetMuteState(int iX) const
{
	return m_MidiFileTrack [ iX ].GetMuteState ( );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetMuteState(int iX, bool bMute)
{
	m_MidiFileTrack [ iX ].SetMuteState ( bMute );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CMidiFile::GetChannelMuteState( int iX ) const
{
	return m_MidiChannels [ iX ].GetMuteState ( );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetChannelMuteState(int iX, bool bMute)
{
	m_MidiChannels [ iX ].SetMuteState ( bMute );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
unsigned char CMidiFile::GetChannelProgramNum(int iChannel)
{
	int						iX;
	CMidiTrack				*pTrack;
	CMidiList				*pList;
	CMidiList				*pNext;

	if ( m_MidiChannels [ iChannel ].IsProgramSet () )
	{
		return m_MidiChannels [ iChannel ].GetProgram ();
	}

	//		For all Tracks
	for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
	{
		pTrack = & ( m_MidiFileTrack [ iX ] );
		pList = pTrack->GetFirstAddress ();
		while ( pList != NULL )
		{
			pNext = pList->GetNext ();
			if ( pList->GetEventIndex () == MIDI_EVENT_PROGRAM && pList->GetChannel ( ) == iChannel )
			{
				m_MidiChannels [ iChannel ].SetProgram ( pList->GetValue1 () );
				return pList->GetValue1 ();
			}
			pList = pNext;
		}

	}	//	For all tracks

	return NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
const char *CMidiFile::GetChannelProgram(int iChannel)
{
	int						iX;
	CMidiTrack				*pTrack;
	CMidiList				*pList;
	CMidiList				*pNext;

	//		If the program is already set in the channel
	//		return it
	if ( m_MidiChannels [ iChannel ].IsProgramSet () )
	{
		return GetInstrumentText ( m_MidiChannels [ iChannel ].GetProgram () );
	}

	//		For all Tracks
	for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
	{
		pTrack = & ( m_MidiFileTrack [ iX ] );
		pList = pTrack->GetFirstAddress ();
		while ( pList != NULL )
		{
			pNext = pList->GetNext ();
			if ( pList->GetEventIndex () == MIDI_EVENT_PROGRAM && pList->GetChannel ( ) == iChannel )
			{
				m_MidiChannels [ iChannel ].SetProgram ( pList->GetValue1 () );
				return pList->GetExtraText ();
			}
			pList = pNext;
		}

	}	//	For all tracks

	return NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//	Immediate Chunk is for applying modification during playback
//	It use a bool to say there is an immediate chunk to treat
//	It can be programs / controller
///////////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetImmediateChunk(DWORD dwChunk)
{
	if ( Lock ( ) )
	{
		m_ImmediateChunk.SetDwordChunk ( dwChunk );
		m_bImmediateChunk = true;
		UnLock ( ) ;
		return;

	}

	int		iCount;

	for ( iCount = 0; iCount < 10; iCount++ )
	{
		if ( ! m_bImmediateChunk || ! LLIsPlaying () )
		{
			m_ImmediateChunk.SetDwordChunk ( dwChunk );
			m_bImmediateChunk = true;
			return;
		}

		Sleep ( 100 );
	}
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetChannelProgram(int iChannel, unsigned char iProgram)
{
	m_MidiChannels [ iChannel ].SetProgram ( iProgram );
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetProgramOnTrack(unsigned int iTrack, unsigned char iProgram)
{
	//		Update necessary values
	if ( m_bEditChange )
	{
		Update ( );
	}

	m_MidiFileTrack [ iTrack ].AddProgram ( iProgram );

	//		Update necessary values
	if ( m_bEditChange )
	{
		Update ( );
	}

}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
unsigned char CMidiFile::GetTrackChannel(int iTrack) const
{
	return m_MidiFileTrack [ iTrack ].GetChannel ();
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
bool CMidiFile::GetChannelVolume(int iChannel, unsigned char *cVolume) 
{
	int						iX;
	CMidiTrack				*pTrack;
	CMidiList				*pList;
	CMidiList				*pNext;
	//		If the program is already set in the channel
	//		return it
	if ( m_MidiChannels [ iChannel ].IsVolumeSet () )
	{
		return m_MidiChannels [ iChannel ].GetVolume ( cVolume );
	}

	//		For all Tracks
	for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
	{
		pTrack = & ( m_MidiFileTrack [ iX ] );
		pList = pTrack->GetFirstAddress ();
		while ( pList != NULL )
		{
			pNext = pList->GetNext ();
			if (    pList->GetEventIndex () == MIDI_EVENT_PARAMETER
				 && pList->GetSubCode () == 7
				 && pList->GetChannel ( ) == iChannel )
			{
				m_MidiChannels [ iChannel ].SetVolume ( pList->GetValue2 () );
				*cVolume = pList->GetValue2 ();
				return true;
			}
			pList = pNext;
		}

	}	//	For all tracks

	return false;
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetVolumeOnTrack(int iTrack, unsigned char cVolume)
{
	//		Update necessary values
	if ( m_bEditChange )
	{
		Update ( );
	}

	m_MidiFileTrack [ iTrack ].AddVolume ( cVolume );

	//		Update necessary values
	if ( m_bEditChange )
	{
		Update ( );
	}

}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetChannelVolume(int iChannel, unsigned char cVolume)
{
	m_MidiChannels [ iChannel ].SetVolume ( cVolume );
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetTrackNameOnTrack(int iTrack, const char *szText)
{
	//		Update necessary values
	if ( m_bEditChange )
	{
		Update ( );
	}

	m_MidiFileTrack [ iTrack ].AddTrackName ( szText );

	//		Update necessary values
	if ( m_bEditChange )
	{
		Update ( );
	}

}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
const char *CMidiFile::GetInstrument ( int iTrack )
{
	if ( iTrack >= 0 && iTrack < MAX_TRACKS )
	{
		return m_MidiFileTrack [ iTrack ].GetInstrumentName ();
	}

	return "";
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetInstrumentNameOnTrack(int iTrack, const char *szText)
{
	//		Update necessary values
	if ( m_bEditChange )
	{
		Update ( );
	}

	char *pOldInstrument = m_MidiFileTrack [ iTrack ].GetInstrumentName();

	if ( pOldInstrument == NULL || strcmp ( pOldInstrument, szText ) != 0 )
	{
		m_bEditChange = true;
	}

	m_MidiFileTrack [ iTrack ].AddInstrumentName ( szText );

	//		Update necessary values
	if ( m_bEditChange )
	{
		Update ( );
	}

}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::ComputeNoteDuration()
{
	int						iX;
	CMidiTrack				*pTrack;

	//		Check if there are other tracks.
	for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
	{
		pTrack = & ( m_MidiFileTrack [ iX ] );
		pTrack->ComputeNoteDuration ( m_iMidiFileDivisionHi,  m_iMidiFileDivisionLo );
	}

}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
unsigned short CMidiFile::GetMaximumMesure() const
{
	return m_iMaximumMesure;
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetMaximumMesure()
{
	int						iX;
	CMidiTrack				*pTrack;
	CMidiList				*pList;
	m_iMaximumMesure = 0;

	for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
	{
		pTrack = & ( m_MidiFileTrack [ iX ] );
		if ( pTrack->GetTrackLength ( ) > 0 )
		{
			pList = pTrack->GetFirstAddress ( );
		}
		else
		{
			pList = NULL;
		}

		while ( pList != NULL )
		{
			if ( pList->GetMesure () > m_iMaximumMesure )
			{
				m_iMaximumMesure = pList->GetMesure ();
			}

			pList = pList->GetNext ();
		}

	}	//	For all tracks

}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
CMidiChannel * CMidiFile::GetChannelAddress(int iChannel) 
{
	return & ( m_MidiChannels [ iChannel ] );
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetTrackChannel(int iTrack, unsigned char cChannel)
{
	m_MidiFileTrack [ iTrack ].SetTrackChannel ( cChannel );
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::CorrectNoteDuration()
{
	int						iX;
	CMidiTrack				*pTrack;

	//		Check if there are other tracks.
	for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
	{
		pTrack = & ( m_MidiFileTrack [ iX ] );
		pTrack->CorrectNoteDuration (
			m_iMidiFileDivisionHi, 
			m_iMidiFileDivisionLo );
	}

}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
char * CMidiFile::GetMidiPathName() const
{
	return m_pMidiFilename;
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::AddXGReset()
{
	//		Remove System Exclusive Messages
	RemoveSystemExclusive ();

	//		Add GM Reset
	m_MidiFileTrack [ 0 ].AddXGReset ();
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::MoveEventFromTrackToTrack(unsigned int iType, int iFrom, int iTo)
{
	CMidiTrack				*pTrack;
	CMidiList				*pList;
	CMidiList				*pNext;
	CMidiList				*pFrom;

	//		First Update all datas.
	Update ();

	//		Now scan all tracks
	if ( iType == MIDI_EVENT_ALL_EVENTS )
	{
		pTrack = & ( m_MidiFileTrack [ iFrom ] );
		pFrom	= m_MidiFileTrack [ iTo ].GetFirstAddress ();

		if ( pTrack->GetTrackLength ( ) > 0 && iFrom != iTo )
		{

			pList = pTrack->GetFirstAddress ( );
			while ( pList != NULL )
			{
				pNext = pList->GetNext ();

				//		Remove from current list without deleting it
				pTrack->RemoveEventFromList ( pList, true );

				//		Add this value at the correct position in the track
				m_MidiFileTrack [ iTo ].InsertAfter ( & pList, &pFrom );

				pList = pNext;

			}

		}
	}
	else
	{
		pTrack = & ( m_MidiFileTrack [ iFrom ] );

		if ( pTrack->GetTrackLength ( ) > 0 && iFrom != iTo )
		{

			pList = pTrack->GetFirstAddress ( );
			while ( pList != NULL )
			{
				pNext = pList->GetNext ();

				if ( pList->GetEventIndex() == iType )
				{
					//		Remove from current list without deleting it
					pTrack->RemoveEventFromList ( pList, true );

					//		Add this value at the the correct position in the track.
					m_MidiFileTrack [ iTo ].Insert ( & pList );
				}

				pList = pNext;

			}

		}

	}

	AddEndOfTrack ();

	Update ();

}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
BOOL CMidiFile::Lock()
{
	if ( m_CriticalSectionPlay.m_hObject )
	{
		return m_CriticalSectionPlay.Lock ( 2000 );
	}

	return TRUE;
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
BOOL CMidiFile::UnLock()
{
	if ( m_CriticalSectionPlay.m_hObject )
	{
		return m_CriticalSectionPlay.Unlock ( );
	}

	return TRUE;
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
BOOL CMidiFile::LockInput()
{
	if ( m_CriticalSectionRecord.m_hObject )
	{
		return m_CriticalSectionRecord.Lock ( 2000 );
	}

	return TRUE;
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
BOOL CMidiFile::UnLockInput()
{
	if ( m_CriticalSectionRecord.m_hObject )
	{
		return m_CriticalSectionRecord.Unlock ( );
	}

	return TRUE;
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
CString CMidiFile::GetWebAddress()
{
	return m_Web_Address;
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetWebAddress(const char *pAddress)
{
	if ( pAddress != NULL )
	{
		m_Web_Address = pAddress;
		return;
	}

	SetInternetAddresses ( );

}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
CString CMidiFile::GetEMail()
{
	return m_E_Mail;
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
void CMidiFile::SetInternetAddresses()
{
	CMidiTrack				*pTrack;
	CMidiList				*pList;
	int						iX;

	for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
	{
		pTrack = & ( m_MidiFileTrack [ iX ] );
		if ( pTrack->GetTrackLength ( ) > 0 )
		{
			//		For all events
			pList = pTrack->GetFirstAddress ( );
			while ( pList != NULL )
			{
				switch ( pList->GetEventIndex ( ) )
				{
					case MIDI_EVENT_SEQUENCE :
					case MIDI_EVENT_TEXT :
					case MIDI_EVENT_COPYRIGHT :
					case MIDI_EVENT_TRACKNAME :
					case MIDI_EVENT_INSTRNAME :
					case MIDI_EVENT_LYRICS :
					case MIDI_EVENT_MARKERS :
					case MIDI_EVENT_CUEPOINT :
					case MIDI_EVENT_SEQUENCER :
					{
						bool	bWebTreated = false;
						bool	bEmailTreated = false;

						char *pText = ( char * )( pList->GetTextAddress ( ) );
						if ( pText != NULL )
						{
							if ( m_Web_Address == "" )
							{
								if ( strstr ( pText, "http://" ) != NULL )
								{
									m_Web_Address = strstr ( pText, "http://" );
									bWebTreated = true;
								}
							}

							if ( m_Web_Address == "" )
							{
								if ( strstr ( pText, "www" ) != NULL )
								{
									m_Web_Address = "http://";
									m_Web_Address += strstr ( pText, "www" );
									bWebTreated = true;
								}
							}

							if ( m_Web_Address == "" )
							{
								if ( strstr ( pText, "://" ) != NULL )
								{
									m_Web_Address = "http";
									m_Web_Address += strstr ( pText, "://" );
									bWebTreated = true;
								}
							}

							if ( strstr ( pText, "mailto:" ) != NULL && ! bEmailTreated )
							{
								if ( m_E_Mail != "" )
								{
									m_E_Mail += ";";
								}
								m_E_Mail += strstr ( pText, "mailto:" );

								bEmailTreated = true;

							}

							if ( strchr ( pText, '@' ) != NULL && ! bEmailTreated )
							{
								if ( m_E_Mail != "" )
								{
									m_E_Mail += ";";
								}
								else
								{
									m_E_Mail += "mailto:";
								}

								m_E_Mail += pText;

								bEmailTreated = true;

							}
						}
						break;
					}
				}


				pList = pList->GetNext ( );
			}
		}

	}	//	For all tracks


}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
int CMidiFile::GetNumberOfInputChunk()
{
	return m_iNumberOfInputChunk;
}

//
////////////////////////////////////////////////////////////////////////////
//		Save the input chunk in a file
////////////////////////////////////////////////////////////////////////////
void CMidiFile::SaveInputChunk(const char *pFilename)
{
	CMidiInputChunk			*pInputChunk;
	CMidiTrack				*pTrackZero;
	CMidiTrack				*pTrackOne;
	CMidiList				*pFromZero;
	CMidiList				*pFromOne;
	CMidiList				*pList;

	pInputChunk	= m_pLLStartInputChunk;
	pTrackZero	= & ( m_MidiFileTrack [ 0 ] );
	pTrackOne	= & ( m_MidiFileTrack [ 1 ] );

	int						iTime = 0;

	//		Track 0
	pFromZero	= pTrackZero->GetFirstAddress ();

	//		Set the track zero infos
	pList = 
		new CMidiList ( 
				&m_cMidiPieceNN, &m_cMidiPieceDD, 
				m_cMidiPieceTempo, MIDI_EVENT_SETTEMPO,
				0, 0,
				0, NULL, 
				m_cMidiPieceTempo [ 0 ],
				m_cMidiPieceTempo [ 1 ],
				m_cMidiPieceTempo [ 2 ]
				);

	pTrackZero->Append ( &pList, &pFromZero );

	//		Now track one
	pFromOne	= pTrackOne->GetFirstAddress ();

	//		Loop on all chunk
	while ( pInputChunk )
	{
		int		iDelta;

		iDelta	= pInputChunk->GetChunkTime ( ) - iTime;

		//		Create a new event
		//		Depending on the chunk
		if ( pInputChunk->GetChunkType ( ) == MIDI_EVENT_SYSEX )
		{
			pList = 
				new CMidiList (
						&m_cMidiPieceNN, &m_cMidiPieceDD, 
						m_cMidiPieceTempo, MIDI_EVENT_SYSEX, 
						iDelta, 0, 
						pInputChunk->GetSysexLength (),
						( unsigned char * ) pInputChunk->GetSysexAddress ( ) );
			//		Set Absolute time
			pList->SetAbsoluteTime ( pInputChunk->GetChunkTime ( ) );

			//		Insert the event in the track
			pTrackZero->Append ( &pList, &pFromZero );

		}
		else
		{
			int		iChannel	= pInputChunk->GetChunkByte1 ( ) & CHANNEL_MASK;
			int		iEvent		= pInputChunk->GetChunkByte1 ( ) & OPERATION_MASK;
			int		iVal2		= pInputChunk->GetChunkByte2 ( );
			int		iVal3		= pInputChunk->GetChunkByte3 ( );
			int		iVal4		= pInputChunk->GetChunkByte4 ( );

			pList = 
				new CMidiList (
						&m_cMidiPieceNN, &m_cMidiPieceDD, 
						m_cMidiPieceTempo, pInputChunk->GetChunkType ( ),
						0, iChannel,
						0, NULL, 
						iVal2,
						iVal3,
						iVal4
						);

			//		Set Absolute time
			pList->SetAbsoluteTime ( pInputChunk->GetChunkTime ( ) );

			//		Insert the event in the track
			pTrackOne->Append ( &pList, &pFromOne );

		}

		iTime = pInputChunk->GetChunkTime ( );

		//		Go to Next
		pInputChunk = pInputChunk->GetNext ( );
	}

	//		Now Update all values
	Update ( );

	//		Now save file
	Save ( pFilename );

	//		Re-Init and Re-Read
	Init ( );

	ReadMidiFile ( pFilename );

	//		Correct the file again.
	CorrectAll ( false, true, true, false, false, false, false );	// YBA

	//		Now save again file
	// Save ( pFilename );

}

//
////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////
boolean CMidiFile::getEditChange()
{
	return m_bEditChange;
}

//
////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////
boolean CMidiFile::setEditChange( boolean bEditChange )
{
	boolean oldEditChange = m_bEditChange;
	m_bEditChange = bEditChange;
	return oldEditChange;
}
