// MidiTrack.cpp: implementation of the CMidiTrack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MidiTrack.h"
#include "MidiSysEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#ifdef	DEBUG_NEW
#define new DEBUG_NEW
#endif
#endif

#include <stdio.h>

extern	CMidiSysEx			clSysExGSReset [ MAX_SYSEX_LINES ];
extern	CMidiSysEx			clSysExGMReset [ MAX_SYSEX_LINES ];
extern	CMidiSysEx			clSysExXGReset [ MAX_SYSEX_LINES ];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMidiTrack::CMidiTrack()
{
	Init ();
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMidiTrack::~CMidiTrack()
{
	Free ();
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
unsigned char *CMidiTrack::ReadTrack ( int iTrack, unsigned char *pBuffer )
{
	CMidiList					*pOld;
	CMidiList					*pNew;
	int							iLenHeader;

	Reset ();

	while ( ( strcmp ( ( char * ) pBuffer, "MTrk" ) != 0 ) && ( m_iMidiTrackMaximumLength > 0 ) )
	{
		pBuffer++;
		m_iMidiTrackMaximumLength--;
	}

	m_pMidiTrackBuffer			= pBuffer;

	m_iMidiTrackNumber			= iTrack;

	//		Set default time signature for starting track.
	m_cMidiPieceNN				= 4;
	m_cMidiPieceDD				= 2;

	//	If track is Ok ?
	m_pMidiTrackHeader			= ( unsigned char * ) malloc ( 5 );
	memset ( m_pMidiTrackHeader, 0, 5 );
	memcpy ( m_pMidiTrackHeader, pBuffer, 4 );

	iLenHeader					= 8;
	m_iMidiTrackLength			= Swap_Long_Endian ( m_pMidiTrackBuffer + 4 );
	m_pMidiTrackPosition		= m_pMidiTrackBuffer + iLenHeader;

	if ( m_iMidiTrackLength > m_iMidiTrackMaximumLength )
	{
		m_iMidiTrackLength = m_iMidiTrackMaximumLength;
	}

	//	If track is Ok ?
	if ( strcmp ( ( char * ) m_pMidiTrackHeader, "MTrk" ) == 0 )
	{
		pOld = NULL;
		pNew = NULL;

		unsigned char		cOldCommand = 0x00;

		// the track is ok so look inside
		while (	( m_pMidiTrackPosition < m_pMidiTrackBuffer + iLenHeader + m_iMidiTrackLength ) &&
				( strncmp ( ( char * ) m_pMidiTrackPosition, "MTrk", 4 ) != 0 ) )
		{
			pNew = 
				new CMidiList (
					&m_cMidiPieceNN, &m_cMidiPieceDD, m_cMidiPieceTempo, 
					m_pMidiTrackPosition, &cOldCommand );
			m_iMidiTrackEventCount++;

			//	The case of the root of the list.
			if ( m_pMidiTrackBegList == NULL )
			{
				m_pMidiTrackBegList = pNew;
			}

			//	Update old item if exist.
			//	The next of the old is the new item.
			if  ( pOld != NULL )
			{
				pOld->SetNext ( pNew );
			}

			//	Update the previous of the new
			//	The previous of the new is the old
			pNew->SetPrevious ( pOld );

			//	There is no next item.
			pNew->SetNext ( NULL );

			pOld = pNew;

			m_pMidiTrackPosition = pNew->GetNextAddress ();
		}
	}

	//	Now compute absolute time.
	ComputeAbsoluteTime ();

	//	Return The forecast next track position
	return ( m_pMidiTrackBuffer + iLenHeader + m_iMidiTrackLength );

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::DeleteListToTheEnd(CMidiList *pList)
{
	CMidiList					*pNext		= NULL;
	CMidiList					*pPrevious	= NULL;
	
	if ( pList != NULL )
	{
		pPrevious = pList->GetPrevious();
	}

	while ( pList != NULL )
	{
		pNext = pList->GetNext ();
		delete pList;
		pList = pNext;
	}

	AlterEnd ( NULL, pPrevious );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::Trace(FILE *hFile)
{
	CMidiList					*pList;

	if ( hFile != NULL )
	{
		fprintf ( hFile, "Track %3d Length:\t%6d\n", m_iMidiTrackNumber, m_iMidiTrackLength );

		pList = m_pMidiTrackBegList;
		while ( pList != NULL )
		{
			pList->Trace ( hFile );
			pList = pList->GetNext ();
		}

	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::Free()
{

	if ( m_pMidiTrackBegList != NULL )
	{
		DeleteListToTheEnd ( m_pMidiTrackBegList );
		m_pMidiTrackBegList = NULL;
	}

	if ( m_pMidiTrackHeader != NULL )
	{
		free ( m_pMidiTrackHeader );
		m_pMidiTrackHeader = NULL;
	}

}

//
///////////////////////////////////////////////////////////////////////////////////
//		Compute Absolute time.
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::ComputeAbsoluteTime()
{
	CMidiList					*pList;
	unsigned long				iTime;

	iTime	= 0;
	pList	= m_pMidiTrackBegList;
	while ( pList != NULL )
	{
		iTime = pList->ComputeAbsoluteTime ( iTime );
		pList = pList->GetNext ();
	}

}

//
///////////////////////////////////////////////////////////////////////////////////
//		Set midi signature for events in tracks other than track 0.
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::SetMidiSignature ( )
{
	CMidiList					*pList;
	unsigned long				iTime;

	CMidiList					*pListTrack0;
	CMidiList					*pNextTrack0;
	unsigned long				iNextTime0;
	unsigned char				cNNTrack0;
	unsigned char				cDDTrack0;

	//	Now compute absolute time.
	ComputeAbsoluteTime ();

	//		Get the pointer on Track 0.
	if ( m_pMidiTrack0 != NULL )
	{
		pListTrack0		= m_pMidiTrack0->GetListPointer ();
	}
	else
	{
		pListTrack0		= GetListPointer ();
	}

	if ( pListTrack0 == NULL )
	{
		return;
	}

	cNNTrack0		= pListTrack0->GetNN ();
	cDDTrack0		= pListTrack0->GetDD ();

	//		Get the current time from TRACK 0
	iNextTime0		= pListTrack0->GetAbsoluteTime ();

	//		Loop on list onf Track N
	pList	= m_pMidiTrackBegList;
	while ( pList != NULL )
	{
		iTime = pList->GetAbsoluteTime ();

		//		Test if we have the time less than the next time
		if ( iTime >= iNextTime0 )
		{
			//		Search the next time greater than the
			//		current time.
			pNextTrack0 = pListTrack0;
			while ( iTime >= iNextTime0 && pNextTrack0 != NULL )
			{
				pNextTrack0 = pNextTrack0->GetNext ();

				if ( pNextTrack0 != NULL )
				{
					iNextTime0 = pNextTrack0->GetAbsoluteTime ();
					//		If the time is less set the value
					//		for the current pointer
					if ( iNextTime0 <= iTime )
					{
						pListTrack0 = pNextTrack0;
					}
				}
				else
				{
					iNextTime0 = 0xffffffff;	// Stop the scan of track 0
				}
			}
		}

		//		Set the NN and DD.
		pList->SetNN ( pListTrack0->GetNN () );
		pList->SetDD ( pListTrack0->GetDD () );

		pList = pList->GetNext ();
	}

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMidiList * CMidiTrack::GetListPointer() const
{
	return m_pMidiTrackBegList;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::Save(FILE *hFile)
{
	CMidiList					*pList;
	unsigned char				szTempLong [ 4 ];
	DWORD						iTrackLengthPosition;
	DWORD						iTrackStartPosition;
	DWORD						iTrackEndPosition;
	DWORD						iTrackLength;

	if ( hFile != NULL )
	{
		fwrite ( "MTrk", 1, 4, hFile );

		//		Get the position of length.
		iTrackLengthPosition = ftell ( hFile );

		memcpy ( szTempLong, ( void * ) &m_iMidiTrackLength, 4 );
		Swap_Long_Endian ( szTempLong );
		fwrite ( szTempLong, 1, 4, hFile );

		//		Get the starting position of the track.
		iTrackStartPosition = ftell ( hFile );

		pList = m_pMidiTrackBegList;
		while ( pList != NULL )
		{
			pList->Save ( hFile );
			pList = pList->GetNext ();
		}

		//		Get The Ending Position of the Track
		iTrackEndPosition = ftell ( hFile );

		iTrackLength = iTrackEndPosition - iTrackStartPosition;

		//		Go to length position
		fseek ( hFile, iTrackLengthPosition, SEEK_SET );

		memcpy ( szTempLong, ( void * ) &iTrackLength, 4 );
		Swap_Long_Endian ( szTempLong );
		fwrite ( szTempLong, 1, 4, hFile );

		//		Go to End again
		fseek ( hFile, iTrackEndPosition, SEEK_SET );

	}

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMidiList *CMidiTrack::GetListAddress(CMidiList *pList) const
{
	if ( pList == NULL )
	{
		return ( GetFirstAddress () );
	}
	else
	{
		return ( pList->GetNext () );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMidiList * CMidiTrack::GetFirstAddress() const
{
	return ( m_pMidiTrackBegList );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMidiList * CMidiTrack::GetNextAddress(CMidiList *pList) const
{
	if ( pList == NULL )
	{
		return NULL;
	}
	else
	{
		return pList->GetNext ();
	}

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::SetMaximumTrackLength(unsigned int iLength)
{
	m_iMidiTrackMaximumLength = iLength - 8; 
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
unsigned CMidiTrack::GetTrackLength() const
{
	return m_iMidiTrackLength;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::Reset()
{
	m_iMidiTrackNumber			= 0;

	m_pMidiTrackBegList			= NULL;
	m_pMidiTrackHeader			= NULL;
	m_iMidiTrackLength			= 0;

	m_pMidiTrackBuffer			= NULL;
	m_pMidiTrackPosition		= NULL;
	m_iMidiTrackEventCount		= 0;

	m_cMidiPieceTempo [ 0 ]		= 9;
	m_cMidiPieceTempo [ 1 ]		= 39;
	m_cMidiPieceTempo [ 2 ]		= 192;

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::Init()
{
	m_iMidiTrackMaximumLength	= 0;
	m_pMidiTrack0				= NULL;

	m_bMute						= false;

	Reset ();
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::Delete()
{
	Free ();

	Init ();
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::AddStartOfTrack()
{
	CMidiList	*pStartOfList = NULL;
	CMidiList	*pStartOfTrack = NULL;

	pStartOfList	= m_pMidiTrackBegList;

	//		If nothing in the track exit.
	if ( pStartOfList == NULL )
	{
		return;
	}

	//		Test if a start of track exists.
	if (    ( pStartOfList->GetEventIndex ( ) == MIDI_EVENT_STARTTRACK )
		 || ( pStartOfList->GetEventIndex ( ) == MIDI_EVENT_STARTTRACK20 ) )
	{
		return;
	}

	//	With a zero Delta
	pStartOfTrack = new CMidiList ( &m_cMidiPieceNN, &m_cMidiPieceDD, m_cMidiPieceTempo, MIDI_EVENT_STARTTRACK, 0 );

	m_pMidiTrackBegList		= pStartOfTrack;
	pStartOfTrack->SetNext ( pStartOfList );
	pStartOfList->SetPrevious ( pStartOfTrack );

	//	Now compute absolute time.
	ComputeAbsoluteTime ();

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::AddEndOfTrack()
{
	CMidiList	*pCurrentList, *pEndOfTrack, *pLastOfTheList;

	pCurrentList = m_pMidiTrackBegList;

	//		If nothing in the track exit.
	if ( pCurrentList == NULL )
	{
		return;
	}

	pLastOfTheList = NULL;
	while ( pCurrentList != NULL )
	{
		pLastOfTheList = pCurrentList;
		pCurrentList = pCurrentList->GetNext ( );
	}

	//		Test if a start of track exists.
	if ( pLastOfTheList->GetEventIndex ( ) == MIDI_EVENT_ENDOFTRACK )
	{
		return;
	}

	pEndOfTrack = new CMidiList ( &m_cMidiPieceNN, &m_cMidiPieceDD, m_cMidiPieceTempo, MIDI_EVENT_ENDOFTRACK, 0 );

	pLastOfTheList->SetNext ( pEndOfTrack );
	pEndOfTrack->SetPrevious ( pLastOfTheList );

	//	Now compute absolute time.
	ComputeAbsoluteTime ();

}

//
///////////////////////////////////////////////////////////////////////////////////
//		Set midi tempo for events in tracks other than track 0.
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::SetMidiTempo ( )
{
	CMidiList					*pList;
	unsigned long				iTime;

	CMidiList					*pListTrack0;
	CMidiList					*pNextTrack0;
	unsigned long				iNextTime0;
	unsigned char				cTempoTrack0 [ 3 ];

	//	Now compute absolute time.
	ComputeAbsoluteTime ();

	//		Get The pointer on Track 0
	if ( m_pMidiTrack0 != NULL )
	{
		pListTrack0		= m_pMidiTrack0->GetListPointer ();
	}
	else
	{
		pListTrack0		= GetListPointer ();
	}

	if ( pListTrack0 == NULL )
	{
		return;
	}

	memcpy ( cTempoTrack0, pListTrack0->GetTempo ( ), sizeof ( cTempoTrack0 ) );

	//		Get the current time from TRACK 0
	iNextTime0		= pListTrack0->GetAbsoluteTime ();

	//		Loop on list onf Track N
	pList	= m_pMidiTrackBegList;
	while ( pList != NULL )
	{
		iTime = pList->GetAbsoluteTime ();

		//		Test if we have the time less than the next time
		if ( iTime >= iNextTime0 )
		{
			//		Search the next time greater than the
			//		current time.
			pNextTrack0 = pListTrack0;
			while ( iTime >= iNextTime0 && pNextTrack0 != NULL )
			{
				pNextTrack0 = pNextTrack0->GetNext ();
				if ( pNextTrack0 != NULL )
				{
					iNextTime0 = pNextTrack0->GetAbsoluteTime ();
					//		If the time is less set the value
					//		for the current pointer
					if ( iNextTime0 <= iTime )
					{
						pListTrack0 = pNextTrack0;
					}
				}
				else
				{
					iNextTime0 = 0xffffffff;	// Stop the scan of track 0
				}
			}
		}

		//		Set the Tempo.
		pList->SetTempo ( pListTrack0->GetTempo () );

		pList = pList->GetNext ();
	}

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::DeleteEventAdjustTime(CMidiList *pCurrentList)
{
	RemoveEventFromList ( pCurrentList, true );

	delete pCurrentList;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::DeleteEventNoAdjust(CMidiList *pCurrentList)
{
	RemoveEventFromList ( pCurrentList, false );

	delete pCurrentList;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::InsertEventBefore ( CMidiList *pCurrentList, CMidiList *pNewMidiList  )
{
	pCurrentList->InsertBefore ( pNewMidiList );

	AlterBeginOrEnd ( pCurrentList, pNewMidiList );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::InsertEventAfter ( CMidiList *pCurrentList, CMidiList *pNewMidiList  )
{
	pCurrentList->InsertAfter( pNewMidiList );

	AlterBeginOrEnd ( pCurrentList, pNewMidiList );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::ChangeCurrentEvent ( CMidiList *pCurrentList, CMidiList *pNewMidiList  )
{
	pCurrentList->ChangeCurrent ( pNewMidiList );

	AlterBeginOrEnd ( pCurrentList, pNewMidiList );

	//
	delete pCurrentList;

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::ComputeMesure ( unsigned char cHi, unsigned char cLo  )
{
	CMidiList					*pList;
	unsigned long				iTime;

	unsigned short				iMesure;
	unsigned short				iBeat;
	unsigned short				iTimeFrame;

	unsigned char				cNN;
	unsigned char				cDD;

	CMidiList					*pListTrack0;
	CMidiList					*pNextTrack0;
	unsigned long				iNextTime0;

	//		Set default time signature for starting track.
	cNN					= 4;
	cDD					= 2;

	//		Initialize mesure variables.
	iMesure			= 0;
	iBeat			= 0;
	iTimeFrame			= 0;

	//		Loop on list on Track N
	pList	= m_pMidiTrackBegList;

	//		Compute Absolute times
	ComputeAbsoluteTime ();

	//	Depending on Track 0 or other
	if ( m_iMidiTrackNumber == 0 )
	{
		//		Loop on list on Track Zero
		pList	= m_pMidiTrackBegList;
		while ( pList != NULL )
		{
			//		Simply Compute Mesure
			pList->ComputeMesure ( 
				cHi, cLo,
				cNN, cDD, 
				iMesure, iBeat, iTimeFrame,
				pList->GetDeltaTime () );

			//		Get the new NN and DD
			cNN				= pList->GetNN ();
			cDD				= pList->GetDD ();

			//		Get the new mesure
			iMesure			= pList->GetMesure ();
			iBeat			= pList->GetBeat ();
			iTimeFrame		= pList->GetTimeFrame ();

			//		Get Next
			pList = pList->GetNext ();
		}

	}
	else	// Not the Track 0
	{
		//		Get the pointer on Track 0.
		pListTrack0		= m_pMidiTrack0->GetListPointer ();

		if ( pListTrack0 == NULL )
		{
			return;
		}

		//		Get the current time from TRACK 0
		iNextTime0		= pListTrack0->GetAbsoluteTime ();

		//		For all events in the track
		while ( pList != NULL )
		{
			//		Get the absolute time.
			iTime = pList->GetAbsoluteTime ();

			//		Test if we have the time less than the next time
			if ( iTime >= iNextTime0 )
			{
				//		Search the next time greater than the
				//		current time.
				pNextTrack0 = pListTrack0;
				while ( iTime >= iNextTime0 && pNextTrack0 != NULL )
				{
					pNextTrack0 = pNextTrack0->GetNext ();
					if ( pNextTrack0 != NULL )
					{
						iNextTime0 = pNextTrack0->GetAbsoluteTime ();
						//		If the time is less set the value
						//		for the current pointer
						if ( iNextTime0 <= iTime )
						{
							pListTrack0 = pNextTrack0;
						}
					}
					else
					{
						iNextTime0 = 0xffffffff;	// Stop the scan of track 0
					}
				}
			}

			iMesure		= pListTrack0->GetMesure ();
			iBeat		= pListTrack0->GetBeat ();
			iTimeFrame		= pListTrack0->GetTimeFrame ();

			//		Get the NN and DD for track 0
			cNN				= pListTrack0->GetNN ();
			cDD				= pListTrack0->GetDD ();

			//		Compute Mesure.
			iTime = pList->GetAbsoluteTime () - pListTrack0->GetAbsoluteTime ();
			pList->ComputeMesure ( 
				cHi, cLo,
				cNN, cDD, 
				iMesure, iBeat, iTimeFrame, 
				iTime );

			//		Goto next event
			pList = pList->GetNext ();
		}

	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::SetMidiTrack0 (	CMidiTrack		*pTrack0,
									unsigned char	*cNN, 
									unsigned char	*cDD,
									unsigned char	*cTempo )
{
	m_pMidiTrack0 = pTrack0;

	//		Value is 4/4
	m_cMidiPieceNN			= *cNN;
	m_cMidiPieceDD			= *cDD;

	m_cMidiPieceTempo [ 0 ]	= cTempo [ 0 ];
	m_cMidiPieceTempo [ 1 ]	= cTempo [ 1 ];
	m_cMidiPieceTempo [ 2 ]	= cTempo [ 2 ];

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::ComputeTempo ( unsigned char cHi, unsigned char cLo )
{
	CMidiList					*pList;
	unsigned long				iTime;
	unsigned long				iOldTime;

	unsigned short				iHours;
	unsigned short				iMinutes;
	unsigned short				iSeconds;
	unsigned short				iMillisecs;

	unsigned short				iOldHours;
	unsigned short				iOldMinutes;
	unsigned short				iOldSeconds;
	unsigned short				iOldMillisecs;

	unsigned char				cTempo [ 3 ];
	unsigned char				cOldTempo [ 3 ];

	CMidiList					*pListTrack0;
	CMidiList					*pNextTrack0;
	unsigned long				iNextTime0;

	//		Set default time signature for starting track.
	cTempo [ 0 ]		= 9;
	cTempo [ 1 ]		= 39;
	cTempo [ 2 ]		= 192;

	//		Initialize mesure variables.
	iHours				= 0;
	iMinutes			= 0;
	iSeconds			= 0;
	iMillisecs			= 0;

	//		Loop on list on Track N
	pList	= m_pMidiTrackBegList;

	//		Compute Absolute times
	ComputeAbsoluteTime ();

	//	Depending on Track 0 or other
	if ( m_iMidiTrackNumber == 0 )
	{
		//		Initialise old time.
		iOldTime			= 0;

		//		Initialize Old mesure variables.
		iOldHours			= 0;
		iOldMinutes			= 0;
		iOldSeconds			= 0;
		iOldMillisecs		= 0;

		//		Set old tempo
		cOldTempo [ 0 ]		= 9;
		cOldTempo [ 1 ]		= 39;
		cOldTempo [ 2 ]		= 192;

		//		Loop on list on Track Zero
		pList	= m_pMidiTrackBegList;
		while ( pList != NULL )
		{
			//		Get the absolute time.
			iTime = pList->GetAbsoluteTime () - iOldTime;

			//		Get the new NN and DD
			memcpy ( cTempo, pList->GetTempo (), sizeof ( cTempo ) );

			//		Simply Compute Mesure
			pList->ComputeTempo ( 
				cHi, cLo,
				cOldTempo,
				iOldHours, iOldMinutes, iOldSeconds, iOldMillisecs,
				iTime );

			//		Test if the tempo is the same as the old
			if ( memcmp ( cTempo, cOldTempo, sizeof ( cTempo ) ) != 0 )
			{
				//		Set the old time to the current absolute time
				iOldTime		= pList->GetAbsoluteTime ();

				//		Get the new mesure hour, minutes...
				iOldHours		= pList->GetHours ();
				iOldMinutes		= pList->GetMinutes ();
				iOldSeconds		= pList->GetSeconds ();
				iOldMillisecs	= pList->GetMillisecs ();

				//		Set the new old tempo
				cOldTempo [ 0 ]		= cTempo [ 0 ];
				cOldTempo [ 1 ]		= cTempo [ 1 ];
				cOldTempo [ 2 ]		= cTempo [ 2 ];

			}

			//		Get Next
			pList = pList->GetNext ();
		}

	}
	else	// Not the Track 0
	{
		//		Get the pointer on Track 0.
		pListTrack0		= m_pMidiTrack0->GetListPointer ();

		if ( pListTrack0 == NULL )
		{
			return;
		}

		//		Get the current time from TRACK 0
		iNextTime0		= pListTrack0->GetAbsoluteTime ();

		//		For all events in the track
		while ( pList != NULL )
		{
			//		Get the absolute time.
			iTime = pList->GetAbsoluteTime ();

			//		Test if we have the time less than the next time
			if ( iTime >= iNextTime0 )
			{
				//		Search the next time greater than the
				//		current time.
				pNextTrack0 = pListTrack0;
				while ( iTime >= iNextTime0 && pNextTrack0 != NULL )
				{
					pNextTrack0 = pNextTrack0->GetNext ();
					if ( pNextTrack0 != NULL )
					{
						iNextTime0 = pNextTrack0->GetAbsoluteTime ();
						//		If the time is less set the value
						//		for the current pointer
						if ( iNextTime0 <= iTime )
						{
							pListTrack0 = pNextTrack0;
						}
					}
					else
					{
						iNextTime0 = 0xffffffff;	// Stop the scan of track 0
					}
				}
			}

			iHours			= pListTrack0->GetHours ();
			iMinutes		= pListTrack0->GetMinutes ();
			iSeconds		= pListTrack0->GetSeconds ();
			iMillisecs		= pListTrack0->GetMillisecs ();

			//		Get the Tempo for track 0
			memcpy ( cTempo, pListTrack0->GetTempo (), sizeof ( cTempo ) );

			//		Compute Tempo.
			iTime = pList->GetAbsoluteTime () - 
					pListTrack0->GetAbsoluteTime ();

			pList->ComputeTempo ( 
				cHi, cLo,
				cTempo,
				iHours, iMinutes, iSeconds, iMillisecs,
				iTime );

			//		Goto next event
			pList = pList->GetNext ();
		}

	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::Append(CMidiList **pList, CMidiList **pAt)
{
	CMidiList				*pTemp;
	unsigned long			iDelta;
	unsigned long			iAbsolute;

	m_iMidiTrackEventCount++;

	m_iMidiTrackLength++;

	if ( *pAt == NULL )
	{
		*pAt = m_pMidiTrackBegList;
	}

	pTemp	= *pAt;
	while ( pTemp != NULL )
	{
		*pAt = pTemp;
		pTemp = pTemp->GetNext ( );
	}

	//	The case of the root of the list.
	if ( m_pMidiTrackBegList == NULL )
	{
		m_pMidiTrackBegList = *pList;
	}

	//	Update old item if exist.
	//	The next of the old is the new item.
	if  ( *pAt != NULL )
	{
		( *pAt ) ->SetNext ( *pList );
	}

	//	Update the previous of the new
	//	The previous of the new is the old
	( *pList )->SetPrevious ( *pAt );

	//	There is no next item.
	( *pList )->SetNext ( NULL );

	//	Now Compute correct delta time.
	iAbsolute = 0;
	if ( *pAt != NULL )
	{
		iAbsolute = (*pAt)->GetAbsoluteTime ();
	}

	iDelta = 0;
	if ( ( *pList )->GetAbsoluteTime () >= iAbsolute )
	{
		iDelta = ( *pList )->GetAbsoluteTime () - iAbsolute;
	}

	( *pList )->SetDeltaTime ( iDelta );

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::AddGMReset()
{
	CMidiList		*pList, *pNew;
	char			szSysEx [ MAX_LONG_STRING ];
	unsigned		iLenSysEx;

	pList = m_pMidiTrackBegList;

	//		Insert from bottom to top
	int				iX;

	//		Start Track and GM Reset Lines
	for ( iX = MAX_SYSEX_LINES - 1; iX >= 0; iX-- )
	{
		if ( ! clSysExGMReset [ iX ].Empty  ( ) )
		{
			//	Always add a start of track before sysex
			pNew = new CMidiList ( &m_cMidiPieceNN, &m_cMidiPieceDD, m_cMidiPieceTempo, MIDI_EVENT_STARTTRACK, 0 );
			pNew->SetNext ( pList );
			if ( pList != NULL )
			{
				pList->SetPrevious ( pNew );
			}
			pList = pNew;

			//		Midi GM
			iLenSysEx = clSysExGMReset [ iX ].Get ( szSysEx );
			pNew = 
				new CMidiList ( &m_cMidiPieceNN, &m_cMidiPieceDD, 
					m_cMidiPieceTempo, MIDI_EVENT_SYSEX, 0, 0, 
							iLenSysEx, ( unsigned char * ) szSysEx );
			pNew->SetNext ( pList );
			if ( pList != NULL )
			{
				pList->SetPrevious ( pNew );
			}

			pList = pNew;

		}
	}

	pNew = new CMidiList ( &m_cMidiPieceNN, &m_cMidiPieceDD, m_cMidiPieceTempo, MIDI_EVENT_STARTTRACK, 0 );
	pNew->SetNext ( pList );
	if ( pList != NULL )
	{
		pList->SetPrevious ( pNew );
	}
	pList = pNew;

	//		Track Name
	pNew = new CMidiList ( &m_cMidiPieceNN, &m_cMidiPieceDD, m_cMidiPieceTempo, MIDI_EVENT_TRACKNAME, 0, 0, 
							(unsigned int) strlen ( "Midi GM" ), ( unsigned char * ) "Midi GM" );
	pNew->SetNext ( pList );
	if ( pList != NULL )
	{
		pList->SetPrevious ( pNew );
	}
	pList = pNew;

	//		Now set the start of the track
	m_pMidiTrackBegList	= pNew;

	//	Now compute absolute time.
	ComputeAbsoluteTime ();

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::AddGSReset()
{
	CMidiList		*pList, *pNew;
	char			szSysEx [ MAX_LONG_STRING ];
	unsigned		iLenSysEx;


	pList = m_pMidiTrackBegList;

	//		Insert from bottom to top
	int				iX;

	//		Start Track and GM Reset Lines
	for ( iX = MAX_SYSEX_LINES - 1; iX >= 0 ; iX-- )
	{
		if ( ! clSysExGSReset [ iX ].Empty  ( ) )
		{
			//	Always add a start of track before sysex
			pNew = new CMidiList ( &m_cMidiPieceNN, &m_cMidiPieceDD, m_cMidiPieceTempo, MIDI_EVENT_STARTTRACK, 0 );
			pNew->SetNext ( pList );
			if ( pList != NULL )
			{
				pList->SetPrevious ( pNew );
			}
			pList = pNew;

			//		Midi GS
			iLenSysEx = clSysExGSReset [ iX ].Get ( szSysEx );
			pNew = 
				new CMidiList ( &m_cMidiPieceNN, &m_cMidiPieceDD, 
					m_cMidiPieceTempo, MIDI_EVENT_SYSEX, 0, 0, 
							iLenSysEx, ( unsigned char * ) szSysEx );
			pNew->SetNext ( pList );
			if ( pList != NULL )
			{
				pList->SetPrevious ( pNew );
			}

			pList = pNew;

		}
	}

	pNew = new CMidiList ( &m_cMidiPieceNN, &m_cMidiPieceDD, m_cMidiPieceTempo, MIDI_EVENT_STARTTRACK, 0 );
	pNew->SetNext ( pList );
	if ( pList != NULL )
	{
		pList->SetPrevious ( pNew );
	}
	pList = pNew;

	//		Track Name
	pNew = 
		new CMidiList ( &m_cMidiPieceNN, &m_cMidiPieceDD, 
						m_cMidiPieceTempo, MIDI_EVENT_TRACKNAME, 0, 0, (unsigned int)strlen ( "Midi GS" ), 
						( unsigned char * ) "Midi GS" );
	pNew->SetNext ( pList );
	if ( pList != NULL )
	{
		pList->SetPrevious ( pNew );
	}
	pList = pNew;

	//		Now set the start of the track
	m_pMidiTrackBegList	= pNew;

	//	Now compute absolute time.
	ComputeAbsoluteTime ();

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::AlterBegin ( CMidiList *pOldMidiList, CMidiList *pNewMidiList )
{
	if ( pOldMidiList == m_pMidiTrackBegList || pOldMidiList == NULL )
	{
		m_pMidiTrackBegList = pNewMidiList;
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::AlterEnd( CMidiList *pOldMidiList, CMidiList *pNewMidiList )
{
	if ( pOldMidiList == m_pMidiTrackEndList || pOldMidiList == NULL )
	{
		m_pMidiTrackEndList = pNewMidiList;
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::AlterBeginOrEnd ( CMidiList *pOldMidiList, CMidiList *pNewMidiList )
{
	AlterBegin ( pOldMidiList, pNewMidiList );
	AlterEnd ( pOldMidiList, pNewMidiList );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::RemoveEventFromList(CMidiList *pCurrentList, bool bAdjust)
{
	if ( pCurrentList == NULL )
	{
		return;
	}

	if ( pCurrentList == m_pMidiTrackBegList )
	{
		m_pMidiTrackBegList = m_pMidiTrackBegList->GetNext ();
	}

	pCurrentList->RemoveFromList (bAdjust);
}

//
///////////////////////////////////////////////////////////////////////////////////
//		Insert :
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::Insert(CMidiList **pList)
{
	CMidiList				*pTemp;
	CMidiList				*pOld;
	unsigned long			iDelta;
	unsigned long			iAbsolute;

	m_iMidiTrackEventCount++;

	m_iMidiTrackLength++;

	//	Find the first value that is greater
	pTemp	= m_pMidiTrackBegList;
	pOld	= pTemp;
	while ( pTemp != NULL )
	{
		if ( pTemp->GetAbsoluteTime () > (*pList )->GetAbsoluteTime () )
		{
			pTemp = NULL;
		}
		else
		{
			pOld = pTemp;
			pTemp = pTemp->GetNext ( );
		}
	}

	//	The case of the root of the list.
	if ( m_pMidiTrackBegList == NULL || pOld == m_pMidiTrackBegList )
	{
		m_pMidiTrackBegList = *pList;
	}

	//	Update old item if exist.
	//	The next of the old is the new item.
	pTemp = NULL;
	if  ( pOld != NULL )
	{
		pTemp = pOld->GetNext ( );
		pOld->SetNext ( *pList );
	}

	//	If pTemp is not equal to zero
	if ( pTemp != NULL )
	{
		pTemp->SetPrevious ( *pList );
	}

	//	Update the previous of the new
	//	The previous of the new is the old
	( *pList )->SetPrevious ( pOld );

	//	There is no next item.
	( *pList )->SetNext ( pTemp );

	//	Now Compute correct delta time for the correct event.
	iAbsolute = 0;
	if ( pOld != NULL )
	{
		iAbsolute = pOld->GetAbsoluteTime ();
	}

	iDelta = ( *pList )->GetAbsoluteTime () - iAbsolute;

	( *pList )->SetDeltaTime ( iDelta );

	//	And compute the correct delta timle for the next event
	//	if any
	pTemp = ( *pList )->GetNext ();
	if ( pTemp != NULL )
	{
		unsigned long iNextDelta = pTemp->GetDeltaTime ();
		pTemp->SetDeltaTime ( iNextDelta - iDelta );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
int CMidiTrack::CountEvents()
{
	CMidiList		*pList;

	m_iMidiTrackEventCount	= 0;
	m_iMidiTrackLength		= 0;

	//		Loop on list on Track N
	pList	= m_pMidiTrackBegList;
	while ( pList != NULL )
	{
		m_iMidiTrackEventCount++;
		m_iMidiTrackLength++;
		pList = pList->GetNext ();
	}

	return m_iMidiTrackEventCount;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::UpdateTrackNumber()
{
	CMidiList		*pList;

	//
	pList	= m_pMidiTrackBegList;
	while ( pList != NULL )
	{
		pList->SetTrack ( m_iMidiTrackNumber );

		pList = pList->GetNext ();
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::InsertAfter(CMidiList **pList, CMidiList **pAt)
{
	CMidiList				*pTemp;
	unsigned long			iDelta;
	unsigned long			iAbsolute;

	m_iMidiTrackEventCount++;

	m_iMidiTrackLength++;

	//	If pAt is null replace it with start
	if ( *pAt == NULL )
	{
		*pAt = m_pMidiTrackBegList;
	}

	//	Find the first value that is greater
	pTemp	= *pAt;
	*pAt	= pTemp;
	while ( pTemp != NULL )
	{
		if ( pTemp->GetAbsoluteTime () > (*pList )->GetAbsoluteTime () )
		{
			pTemp = NULL;
		}
		else
		{
			*pAt	= pTemp;
			pTemp	= pTemp->GetNext ( );
		}

	}

	//	The case of the root of the list.
	if ( m_pMidiTrackBegList == NULL )
	{
		m_pMidiTrackBegList = *pList;
	}

	//	Update old item if exist.
	//	The next of the old is the new item.
	pTemp = NULL;
	if  ( *pAt != NULL )
	{
		pTemp = ( *pAt )->GetNext ( );
		( *pAt )->SetNext ( *pList );
	}

	//	If pTemp is not equal to zero
	if ( pTemp != NULL )
	{
		pTemp->SetPrevious ( *pList );
	}

	//	Update the previous of the new
	//	The previous of the new is the old
	( *pList )->SetPrevious ( *pAt );

	//	There is no next item.
	( *pList )->SetNext ( pTemp );

	//	Now Compute correct delta time for the correct event.
	iAbsolute = 0;
	if ( *pAt != NULL )
	{
		iAbsolute = ( *pAt )->GetAbsoluteTime ();
	}

	iDelta = ( *pList )->GetAbsoluteTime () - iAbsolute;

	( *pList )->SetDeltaTime ( iDelta );

	//	And compute the correct delta timle for the next event
	//	if any
	pTemp = ( *pList )->GetNext ();
	if ( pTemp != NULL )
	{
		unsigned long iNextDelta = pTemp->GetDeltaTime ();
		pTemp->SetDeltaTime ( iNextDelta - iDelta );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
int CMidiTrack::GetTrackNumber() const
{
	return m_iMidiTrackNumber;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
bool CMidiTrack::GetMuteState() const
{
	return m_bMute;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::SetMuteState(bool bMute)
{
	m_bMute = bMute;
}

//
///////////////////////////////////////////////////////////////////////////////////
//	Scan full list to see if we have only on channel
///////////////////////////////////////////////////////////////////////////////////
unsigned char CMidiTrack::GetChannel() const
{
	CMidiList		*pList;
	unsigned char	returnChannel;
	unsigned char	iEventChan;

	//		Loop on list on Track N
	pList		= m_pMidiTrackBegList;
	returnChannel	= 0xff;
	while ( pList != NULL )
	{
		if ( pList->useChannel() )
		{
			iEventChan = pList->GetChannel ();
			if ( iEventChan >= 0 && iEventChan < MAX_CHANNELS )
			{
				//	Return Channel no already set
				if ( returnChannel == 0xff )
				{
					returnChannel = iEventChan;
				}
				//	Already Set
				else
				{
					// Channel has Changed
					if ( iEventChan != returnChannel )
					{
						//	Multiple channel exist
						//	Say no Channel
						return 0xff;
					}
				}
			}
		}
		pList = pList->GetNext ();
	}

	return returnChannel;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
const char * CMidiTrack::GetProgramText() const
{
	CMidiList		*pList;

	//		Loop on list on Track N
	pList	= m_pMidiTrackBegList;
	while ( pList != NULL )
	{
		if ( pList->GetEventIndex () == MIDI_EVENT_PROGRAM )
		{
			return pList->GetExtraText ( );
		}
		pList = pList->GetNext ();
	}

	return NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
char * CMidiTrack::GetTrackName() const
{
	CMidiList		*pList;

	//		Loop on list on Track N
	pList	= m_pMidiTrackBegList;
	while ( pList != NULL )
	{
		if ( pList->GetEventIndex () == MIDI_EVENT_TRACKNAME )
		{
			return ( char * ) pList->GetTextAddress ( );
		}
		pList = pList->GetNext ();
	}

	return NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
char * CMidiTrack::GetInstrumentName() const
{
	CMidiList		*pList;

	//		Loop on list on Track N
	pList	= m_pMidiTrackBegList;
	while ( pList != NULL )
	{
		if ( pList->GetEventIndex () == MIDI_EVENT_INSTRNAME )
		{
			return ( char * ) pList->GetTextAddress ( );
		}
		pList = pList->GetNext ();
	}

	return NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::AddProgram(unsigned char iProgram)
{
	CMidiList			*pList, *pNew;
	unsigned char		iChannel;

	//		If nothing in the track exit.
	if ( m_pMidiTrackBegList == NULL )
	{
		return;
	}
	
	//		Test if a start of track exists.
	if (    ( m_pMidiTrackBegList->GetEventIndex ( ) != MIDI_EVENT_STARTTRACK )
		 && ( m_pMidiTrackBegList->GetEventIndex ( ) != MIDI_EVENT_STARTTRACK20 ) )
	{
		return;
	}

	//		Only if the channel is correct
	iChannel = GetChannel ();
	if ( iChannel >= 0 && iChannel < MAX_CHANNELS )
	{
		//		Remove first program
		RemoveEventCode ( MIDI_EVENT_PROGRAM, false, true );

		//		Point after the start of track
		pList = m_pMidiTrackBegList;

		//		Create a new event program
		pNew = 
			new CMidiList (
				&m_cMidiPieceNN, &m_cMidiPieceDD, 
				m_cMidiPieceTempo, MIDI_EVENT_PROGRAM, 0, 
				iChannel, 0, NULL, iProgram );

		//		Now Insert Event just after the start of track
		InsertAfter (&pNew, &pList );
	}

	//	Now compute absolute time.
	ComputeAbsoluteTime ();

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::RemoveEventCode(unsigned int iCode, bool bSkipFirstEvent, bool bFirstEventOnly )
{
	CMidiList			*pList;
	CMidiList			*pNext;

	//		First Remove Program if any
	pList = GetFirstAddress ();
	if ( pList == NULL )
	{
		return;
	}

	if ( bSkipFirstEvent )
	{
		pList = pList->GetNext ();
	}

	while ( pList != NULL )
	{
		pNext = pList->GetNext ();
		if ( pList->GetEventIndex () == iCode )
		{
			DeleteEventAdjustTime ( pList );
			if ( bFirstEventOnly )
			{
				pNext = NULL;
			}
		}
		pList = pNext;
	}

	//	Now compute absolute time.
	ComputeAbsoluteTime ();

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::RemoveEventCodeSubCode(unsigned int iType, unsigned char cSubType, bool bFirstEventOnly)
{
	CMidiList				*pList;
	CMidiList				*pNext;

	pList = GetFirstAddress ();
	while ( pList != NULL )
	{
		pNext = pList->GetNext ();
		if (	pList->GetEventIndex () == iType && pList->GetSubCode () == cSubType )
		{
			DeleteEventAdjustTime ( pList );
			if ( bFirstEventOnly )
			{
				pNext = NULL;
			}
		}
		pList = pNext;
	}

	//	Now compute absolute time.
	ComputeAbsoluteTime ();

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
bool CMidiTrack::GetVolume(unsigned char *cVolume) const
{
	CMidiList		*pList;

	//		Loop on list on Track N
	pList	= m_pMidiTrackBegList;
	while ( pList != NULL )
	{
		if (    ( pList->GetEventIndex () == MIDI_EVENT_PARAMETER ) && ( pList->GetSubCode () == CNTR_VOLUME_MSB ) )
		{
			*cVolume = pList->GetValue2 ( );
			return true;
		}
		pList = pList->GetNext ();
	}

	return false;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::AddVolume(unsigned char cVolume)
{
	CMidiList			*pList, *pNew;
	unsigned char		iChannel;

	//		If nothing in the track exit.
	if ( m_pMidiTrackBegList == NULL )
	{
		return;
	}
	
	//		Test if a start of track exists.
	if (    ( m_pMidiTrackBegList->GetEventIndex ( ) != MIDI_EVENT_STARTTRACK )
		 && ( m_pMidiTrackBegList->GetEventIndex ( ) != MIDI_EVENT_STARTTRACK20 ) )
	{
		return;
	}

	//		Only if the channel is correct
	iChannel = GetChannel ();
	if ( iChannel >= 0 && iChannel < MAX_CHANNELS )
	{
		//		Remove volume parameter
		RemoveEventCodeSubCode ( MIDI_EVENT_PARAMETER, CNTR_VOLUME_MSB, true );

		//		Point after the start of track
		pList = m_pMidiTrackBegList;

		//		Create a new event program
		pNew = new CMidiList ( 
			&m_cMidiPieceNN, &m_cMidiPieceDD, m_cMidiPieceTempo,
			MIDI_EVENT_PARAMETER, 0, iChannel, 0, NULL, 7, cVolume );

		//		Now Insert Event just after the start of track
		InsertAfter ( &pNew, &pList );
	}

	//	Now compute absolute time.
	ComputeAbsoluteTime ();

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::AddTrackName ( const char *pTrackName )
{
	CMidiList			*pList, *pNew;

	//		If nothing in the track exit.
	if ( m_pMidiTrackBegList == NULL )
	{
		return;
	}
	
	//		Remove first track name
	RemoveEventCode ( MIDI_EVENT_TRACKNAME, false, true );

	//		If nothing in the track exit.
	if ( m_pMidiTrackBegList == NULL )
	{
		return;
	}
	
	//		Create a new event program
	pNew = new CMidiList ( &m_cMidiPieceNN, &m_cMidiPieceDD, m_cMidiPieceTempo, MIDI_EVENT_TRACKNAME, 0,
		0, (unsigned int)strlen ( pTrackName ), ( unsigned char * ) pTrackName );

	//		Test if a start of track exists.
	if (    ( m_pMidiTrackBegList->GetEventIndex ( ) != MIDI_EVENT_STARTTRACK )
		 && ( m_pMidiTrackBegList->GetEventIndex ( ) != MIDI_EVENT_STARTTRACK20 ) )
	{
		pList = m_pMidiTrackBegList;

		m_pMidiTrackBegList	= pNew;
		pNew->SetNext ( pList );

		pList->SetPrevious ( pNew );

		return;
	}

	//		Point after the start of track
	pList = m_pMidiTrackBegList;

	//		Now Insert Event just after the start of track
	InsertAfter ( &pNew, &pList );

	//	Now compute absolute time.
	ComputeAbsoluteTime ();

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::AddInstrumentName ( const char *pInstrument )
{
	CMidiList			*pList, *pNew;

	//		If nothing in the track exit.
	if ( m_pMidiTrackBegList == NULL )
	{
		return;
	}
	
	//		Remove first instrument name
	RemoveEventCode ( MIDI_EVENT_INSTRNAME, false, true );

	//		If nothing in the track exit.
	if ( m_pMidiTrackBegList == NULL )
	{
		return;
	}
	
	//		Create a new event program
	pNew = new CMidiList ( &m_cMidiPieceNN, &m_cMidiPieceDD, m_cMidiPieceTempo, MIDI_EVENT_INSTRNAME, 0,
		0, (unsigned int) strlen ( pInstrument ), ( unsigned char * ) pInstrument );

	//		Point after the start of track
	pList = m_pMidiTrackBegList;

	//		Test if a start of track exists.
	if (    ( m_pMidiTrackBegList->GetEventIndex ( ) != MIDI_EVENT_STARTTRACK )
		 && ( m_pMidiTrackBegList->GetEventIndex ( ) != MIDI_EVENT_STARTTRACK20 ) )
	{
		//		Now Insert Event just before the start of track
		InsertBefore ( &pNew, &pList );
	}
	else
	{
		//		Now Insert Event just after the start of track
		InsertAfter ( &pNew, &pList );
	}

	//	Now compute absolute time.
	ComputeAbsoluteTime ();

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::ComputeNoteDuration( unsigned char cHi, unsigned char cLo )
{
	CMidiList		*pNoteOnList;
	CMidiList		*pNoteOffList;

	unsigned long	lNoteOn;
	unsigned long	lNoteOff;

	unsigned char	cNoteOn;
	unsigned char	cNoteOff;

	unsigned char	cNoteOnPitch;
	unsigned char	cNoteOffPitch;

	//		First Reset note off used on all notes
	pNoteOnList	= m_pMidiTrackBegList;
	while ( pNoteOnList != NULL )
	{
		pNoteOnList->SetNoteOffUsed ( false );
		pNoteOnList = pNoteOnList->GetNext ();
	}

	//		Loop on list on Track N
	pNoteOnList	= m_pMidiTrackBegList;
	while ( pNoteOnList != NULL )
	{
		//	Test if the event is a note on
		if ( pNoteOnList->GetEventIndex ( ) == MIDI_EVENT_NOTEON )
		{
			lNoteOn			= pNoteOnList->GetAbsoluteTime();
			cNoteOn			= pNoteOnList->GetValue1();
			cNoteOnPitch	= pNoteOnList->GetValue2();

			lNoteOff	= 0xffffffff;
			pNoteOffList = pNoteOnList->GetNext ();
			while ( pNoteOffList != NULL )
			{
				if (		pNoteOffList->GetEventIndex ( ) == MIDI_EVENT_NOTEOFF 
					 &&	!	pNoteOffList->GetNoteOffUsed ( )                      )
				{
					lNoteOff		= pNoteOffList->GetAbsoluteTime();
					cNoteOff		= pNoteOffList->GetValue1();
					cNoteOffPitch	= pNoteOffList->GetValue2();

					if ( cNoteOn == cNoteOff )
					{
						pNoteOffList->SetNoteOffUsed ( true );
						pNoteOffList = NULL;
					}
					else
					{
						pNoteOffList = pNoteOffList->GetNext ();
					}

				}
				else
				{
					pNoteOffList = pNoteOffList->GetNext ();
				}
			}

			pNoteOnList->SetNoteDuration ( cHi, cLo, lNoteOff - lNoteOn );

		}

		pNoteOnList = pNoteOnList->GetNext ();
	}

	return;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::SetTrackChannel(unsigned char cChannel)
{
	CMidiList				*pList;

	pList = GetFirstAddress ();
	while ( pList != NULL )
	{
		pList->SetChannelAndCode ( cChannel );
		pList = pList->GetNext ( );
	}

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::CorrectNoteDuration(unsigned char cHi, unsigned char cLo)
{
	CMidiList		*pNoteOnList;
	CMidiList		*pNoteOffList;

	unsigned long	lNoteOn;
	unsigned long	lNoteOff;

	unsigned char	cNoteOn;
	unsigned char	cNoteOff;

	unsigned char	cNoteOnPitch;
	unsigned char	cNoteOffPitch;

	CMidiList		*pNewNoteOff;
	CMidiList		*pNewTextOff;

	//		First Reset note off used on all notes
	pNoteOnList	= m_pMidiTrackBegList;
	while ( pNoteOnList != NULL )
	{
		pNoteOnList->SetNoteOffUsed ( false );
		pNoteOnList = pNoteOnList->GetNext ();
	}

	//		Loop on list on Track N
	pNoteOnList	= m_pMidiTrackBegList;
	while ( pNoteOnList != NULL )
	{
		//	Test if the event is a note on
		if ( pNoteOnList->GetEventIndex ( ) == MIDI_EVENT_NOTEON )
		{
			lNoteOn			= pNoteOnList->GetAbsoluteTime();
			cNoteOn			= pNoteOnList->GetValue1();
			cNoteOnPitch	= pNoteOnList->GetValue2();

			lNoteOff	= 0xffffffff;
			pNoteOffList = pNoteOnList->GetNext ();
			while ( pNoteOffList != NULL )
			{
				if (		pNoteOffList->GetEventIndex ( ) == MIDI_EVENT_NOTEOFF 
					 &&	!	pNoteOffList->GetNoteOffUsed ( )                      )
				{
					lNoteOff		= pNoteOffList->GetAbsoluteTime();
					cNoteOff		= pNoteOffList->GetValue1();
					cNoteOffPitch	= pNoteOffList->GetValue2();

					if ( cNoteOn == cNoteOff )
					{
						pNoteOffList->SetNoteOffUsed ( true );
						pNoteOffList = NULL;
					}
					else
					{
						pNoteOffList = pNoteOffList->GetNext ();
					}
				}
				else
				{
					//		Test if we have over past the delta allowed
					//		The 1920 is 4 rond notes at 120 beat
					if (	pNoteOffList->GetAbsoluteTime() - pNoteOnList->GetAbsoluteTime()  > 1920 ||
							pNoteOffList->GetEventIndex ( ) == MIDI_EVENT_ENDOFTRACK )
					{
						char		*pText = "Note Off Inserted Here";

						pNewTextOff =
							new CMidiList ( &m_cMidiPieceNN, &m_cMidiPieceDD, m_cMidiPieceTempo, 
								MIDI_EVENT_TEXT, 0,
								0,
								(unsigned int) strlen ( pText ), ( unsigned char * ) pText );
						pNewTextOff->SetAbsoluteTime ( pNoteOffList->GetAbsoluteTime() );

						pNewNoteOff = 
							new CMidiList ( &m_cMidiPieceNN, &m_cMidiPieceDD, m_cMidiPieceTempo, 
								MIDI_EVENT_NOTEOFF, 0,
								pNoteOnList->GetChannel ( ),
								0, NULL,
								pNoteOnList->GetValue1 (), 0 );
						pNewNoteOff->SetAbsoluteTime ( pNoteOffList->GetAbsoluteTime() );

						InsertBefore ( &pNewTextOff, &pNoteOffList );
						InsertBefore ( &pNewNoteOff, &pNoteOffList );

						pNoteOffList = NULL;
					}
					else
					{
						pNoteOffList = pNoteOffList->GetNext ();
					}
				}
			}

			pNoteOnList->SetNoteDuration ( cHi, cLo, lNoteOff - lNoteOn );

		}

		pNoteOnList = pNoteOnList->GetNext ();
	}

	return;
}

//
///////////////////////////////////////////////////////////////////////////////////
//		Insert before but 
//		the absolute times must be equal to have a delta time equal to zero
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::InsertBefore(CMidiList **pList, CMidiList **pAt)
{
	CMidiList				*pTemp;
	unsigned long			iDelta;
	unsigned long			iAbsolute;

	m_iMidiTrackEventCount++;

	m_iMidiTrackLength++;

	//	If pAt is null replace it with start
	if ( *pAt == NULL )
	{
		*pAt = m_pMidiTrackBegList;
	}

	//	Find the first value that is greater
	pTemp	= *pAt;
	*pAt	= pTemp;
	while ( pTemp != NULL )
	{
		if ( pTemp->GetAbsoluteTime () > (*pList )->GetAbsoluteTime () )
		{
			pTemp = NULL;
		}
		else
		{
			*pAt	= pTemp;
			pTemp	= pTemp->GetNext ( );
		}

	}

	//	The case of the root of the list.
	if ( m_pMidiTrackBegList == NULL )
	{
		m_pMidiTrackBegList = *pList;
	}

	//	Update old item if exist.
	//	The next of the old is the new item.
	pTemp = NULL;
	if  ( *pAt != NULL )
	{
		pTemp = ( *pAt )->GetPrevious ( );
		( *pAt )->SetPrevious ( *pList );
	}

	//		If pTemp is not equal to zero
	if ( pTemp != NULL )
	{
		pTemp->SetNext ( *pList );
	}
	//		The previous is null so we are the first
	else
	{
		m_pMidiTrackBegList = *pList;
	}

	//	Update the previous of the new
	//	The previous of the new is the old
	( *pList )->SetNext ( *pAt );

	//	There is no next item.
	( *pList )->SetPrevious ( pTemp );

	//	Now Compute correct delta time for the correct event.
	iAbsolute = 0;
	if ( pTemp != NULL )
	{
		iAbsolute = pTemp->GetAbsoluteTime ();
	}

	iDelta = ( *pList )->GetAbsoluteTime () - iAbsolute;

	( *pList )->SetDeltaTime ( iDelta );

	//	And compute the correct delta time for the next event
	//	if any
	pTemp = ( *pList )->GetNext ();
	if ( pTemp != NULL )
	{
		unsigned long iNextDelta = pTemp->GetDeltaTime ();
		pTemp->SetDeltaTime ( iNextDelta - iDelta );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::AddXGReset()
{
	CMidiList		*pList, *pNew;
	char			szSysEx [ MAX_LONG_STRING ];
	unsigned		iLenSysEx;

	pList = m_pMidiTrackBegList;

	//		Insert from bottom to top
	int				iX;

	//		Start Track and XG Reset Lines
	for ( iX = MAX_SYSEX_LINES - 1; iX >= 0 ; iX-- )
	{
		if ( ! clSysExXGReset [ iX ].Empty  ( ) )
		{
			//	Always add a start of track before sysex
			pNew = new CMidiList ( &m_cMidiPieceNN, &m_cMidiPieceDD, m_cMidiPieceTempo, MIDI_EVENT_STARTTRACK, 0 );
			pNew->SetNext ( pList );
			if ( pList != NULL )
			{
				pList->SetPrevious ( pNew );
			}
			pList = pNew;

			//		Midi XG
			iLenSysEx = clSysExXGReset [ iX ].Get ( szSysEx );
			pNew = 
				new CMidiList ( &m_cMidiPieceNN, &m_cMidiPieceDD, 
					m_cMidiPieceTempo, MIDI_EVENT_SYSEX, 0, 0, 
							iLenSysEx, ( unsigned char * ) szSysEx );
			pNew->SetNext ( pList );
			if ( pList != NULL )
			{
				pList->SetPrevious ( pNew );
			}

			pList = pNew;

		}
	}

	pNew = new CMidiList ( &m_cMidiPieceNN, &m_cMidiPieceDD, m_cMidiPieceTempo, MIDI_EVENT_STARTTRACK, 0 );
	pNew->SetNext ( pList );
	if ( pList != NULL )
	{
		pList->SetPrevious ( pNew );
	}
	pList = pNew;

	//		Track Name
	pNew = new CMidiList (	&m_cMidiPieceNN, &m_cMidiPieceDD, 
							m_cMidiPieceTempo, MIDI_EVENT_TRACKNAME, 0, 0, 
							(unsigned int) strlen ( "Midi XG" ), 
							( unsigned char * ) "Midi XG" );
	pNew->SetNext ( pList );
	if ( pList != NULL )
	{
		pList->SetPrevious ( pNew );
	}
	pList = pNew;

	//		Now set the start of the track
	m_pMidiTrackBegList	= pNew;

	//	Now compute absolute time.
	ComputeAbsoluteTime ();

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiTrack::AppendWithDelta(CMidiList **pList, CMidiList **pAt)
{
	CMidiList				*pTemp;

	m_iMidiTrackEventCount++;

	m_iMidiTrackLength++;

	if ( *pAt == NULL )
	{
		*pAt = m_pMidiTrackBegList;
	}

	pTemp	= *pAt;
	while ( pTemp != NULL )
	{
		*pAt = pTemp;
		pTemp = pTemp->GetNext ( );
	}

	//	The case of the root of the list.
	if ( m_pMidiTrackBegList == NULL )
	{
		m_pMidiTrackBegList = *pList;
	}

	//	Update old item if exist.
	//	The next of the old is the new item.
	if  ( *pAt != NULL )
	{
		( *pAt ) ->SetNext ( *pList );
	}

	//	Update the previous of the new
	//	The previous of the new is the old
	( *pList )->SetPrevious ( *pAt );

	//	There is no next item.
	( *pList )->SetNext ( NULL );

}
