#include "stdafx.h"
#include "MidiFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#ifdef	DEBUG_NEW
#define new DEBUG_NEW
#endif
#endif

#include "MidiDefs.h"
#include "CFluidSynth.h"

static	bool bMidiFileOneShotTimer = false;

//
///////////////////////////////////////////////////////////////////////////////////
//		Callback Prototypes
//
///////////////////////////////////////////////////////////////////////////////////
void PASCAL MidiFileOneShotTimer (	UINT wTimerID, UINT wMsg,
									DWORD_PTR dwUser, 
									DWORD_PTR dwl, DWORD_PTR dwParam2 );

void CALLBACK MidiFileDone (	HMIDIOUT hmo, UINT wMsg,
								DWORD_PTR dwInstance, 
								DWORD_PTR dwParam1, DWORD_PTR dwParam2 );

void CALLBACK MidiFileInpDone (	HMIDIIN hmi, UINT wMsg,
								DWORD_PTR dwInstance, 
								DWORD_PTR dwParam1, DWORD_PTR dwParam2 );

//
///////////////////////////////////////////////////////////////////////////////////
//		The call back for events
///////////////////////////////////////////////////////////////////////////////////
void PASCAL MidiFileOneShotTimer (	UINT wTimerID, UINT wMsg,
									DWORD_PTR dwUser, 
									DWORD_PTR dwParam1, DWORD_PTR dwParam2 )
{
	bMidiFileOneShotTimer = false;

	if ( dwUser != NULL )
	{
		( ( CMidiFile * ) dwUser )->LLPlayRoutine( false, true, false );
	}

	UNREFERENCED_PARAMETER ( dwParam2 );
	UNREFERENCED_PARAMETER ( dwParam1 );
	UNREFERENCED_PARAMETER ( dwUser );
	UNREFERENCED_PARAMETER ( wMsg );
	UNREFERENCED_PARAMETER ( wTimerID );

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CALLBACK MidiFileDone (	HMIDIOUT hmo, UINT wMsg,
								DWORD_PTR dwInstance, 
								DWORD_PTR dwParam1, DWORD_PTR dwParam2 )
{
	//		Just add a small tempo continuing after SYSEX
	//		Else in some case the SYS EX is eaten
	Sleep ( SLEEP_TIME_SYSEX );		//	In MilliSeconds

	if ( dwInstance != NULL && wMsg == MM_MOM_DONE )
	{
		( ( CMidiFile * ) dwInstance )->LLPlayRoutine( false, false, true );
	}

	UNREFERENCED_PARAMETER ( dwParam2 );
	UNREFERENCED_PARAMETER ( dwParam1 );
	UNREFERENCED_PARAMETER ( dwInstance );
	UNREFERENCED_PARAMETER ( wMsg );
	UNREFERENCED_PARAMETER ( hmo );

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CALLBACK MidiFileInpDone (	HMIDIIN hmi, UINT wMsg,
								DWORD_PTR dwInstance, 
								DWORD_PTR dwParam1, DWORD_PTR dwParam2 )
{
	//		Just add a small tempo continuing after SYSEX
	//		Else in some case the SYS EX is eaten
	if ( dwInstance != NULL )
	{
		dwInstance = dwInstance;
		if ( wMsg == MM_MIM_OPEN )
		{
			wMsg = wMsg;
		}

		if ( wMsg == MM_MIM_DATA )
		{
			if ( dwInstance != NULL )
			{
				( ( CMidiFile * ) dwInstance )->LLInpRecordChunk ( dwParam1, dwParam2 );
			}
		}

		if ( wMsg == MM_MIM_LONGDATA )
		{
			if ( dwInstance != NULL )
			{
				( ( CMidiFile * ) dwInstance )->LLInpRecordChunk ( ( MIDIHDR * ) dwParam1, dwParam2 );
			}
		}

		if ( wMsg == MM_MIM_MOREDATA )
		{
			wMsg = wMsg;
		}

		if ( wMsg == MM_MIM_ERROR )
		{
			wMsg = wMsg;
		}

		if ( wMsg == MM_MIM_LONGERROR )
		{
			wMsg = wMsg;
		}

		if ( wMsg == MM_MIM_CLOSE )
		{
			wMsg = wMsg;
		}

	}

	UNREFERENCED_PARAMETER ( dwParam2 );
	UNREFERENCED_PARAMETER ( dwParam1 );
	UNREFERENCED_PARAMETER ( dwInstance );
	UNREFERENCED_PARAMETER ( wMsg );
	UNREFERENCED_PARAMETER ( hmi );

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
unsigned long CMidiFile::LLGetTime() const
{
	return m_iLLCurrentHMSm;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
bool CMidiFile::LLIsPlaying() const
{
	if ( m_bLLState == MIDI_PLAYER_PLAYING )
	{
		return true;
	}

	return false;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
bool CMidiFile::LLIsRecording() const
{
	if ( m_bLLInputState == MIDI_RECORDER_RECORDING )
	{
		return true;
	}

	return false;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
bool CMidiFile::LLIsPaused() const
{
	if ( m_bLLState == MIDI_PLAYER_PAUSED )
	{
		return true;
	}

	return false;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
bool CMidiFile::LLIsStopped() const
{
	if ( m_bLLState == MIDI_PLAYER_STOPPED )
	{
		return true;
	}

	return false;
}

//
///////////////////////////////////////////////////////////////////////////////////
//		Fills the playing list
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiFile::LLFillChunkList()
{
	CMidiTrack			*pTrack;
	CMidiList			*PlayList [ MAX_TRACKS ];
	int					iX;
	unsigned long		iLess;
	unsigned			iChannel;

	unsigned long		iPreviousHMCm;
	unsigned long		iCurrentHMCm;

	DWORD				dwDwordChunk;
	MIDIHDR				*pSysexChunk;

	DWORD				dwOldChunk;
	CMidiPlayerChunk	*pPreviousChunk;
	CMidiPlayerChunk	*pPlayerChunk;

	CMidiList			*pFoundList;
	int					iTrackFound;
	bool				bToBeStored;
	unsigned			iRemoved;
	unsigned			iSkipCode;
	unsigned			iStored;

	bool				bDone;
	DWORD				dwChannelChunks [ MAX_CHANNELS ];

	//		First if something exists delete it
	if ( m_pLLStartChunk != NULL )
	{
		DeleteChunkList ();
	}

	//		Then fill chunk list with existing informations
	//		Reset the absolute value of HMSm
	//		For all tracks get the first pointer
	//		or set a NULL if track is empty.
	for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
	{
		pTrack = & ( m_MidiFileTrack [ iX ] );

		//		Test if the track is ok 
		if ( pTrack->GetTrackLength ( ) > 0 )
		{
			PlayList [ iX ] = pTrack->GetFirstAddress ( );
		}
		else
		{
			PlayList [ iX ] = NULL;
		}	

	}	//	For all tracks

	//		For all tracks search all values that are less than
	//		the current value.
	iLess					= 0;
	iPreviousHMCm			= 0;

	pPreviousChunk			= NULL;
	pPlayerChunk			= NULL;
	dwOldChunk				= 0;
	iRemoved				= 0;
	iSkipCode				= 0;
	iStored					= 0;

	//		Reset chunks by channels
	for ( iX = 0; iX < MAX_CHANNELS; iX++ )
	{
		dwChannelChunks [ iX ] = 0;
	}

	//		Loop until the end of the tracks
	bDone = false;

	while ( ! bDone )
	{
		//		Now find the less value among all tracks.
		iTrackFound	= -1;
		for ( iX = 0; iX < m_iMidiFileMaxTracks; iX++ )
		{
			if ( PlayList [ iX ] != NULL )
			{
				if ( iTrackFound < 0 )
				{
					iTrackFound = iX;
					iLess = PlayList [ iX ]->GetAbsoluteHMSm ();
				}

				if ( iLess > PlayList [ iX ]->GetAbsoluteHMSm () )
				{
					iTrackFound = iX;
					iLess = PlayList [ iX ]->GetAbsoluteHMSm ();
				}
			}
		}

		//		Here we have found the less.
		if ( iTrackFound >= 0 )
		{
			if ( PlayList [ iTrackFound ] != NULL )
			{
				iCurrentHMCm = PlayList [ iTrackFound ]->GetAbsoluteHMSm ();

				pFoundList = PlayList [ iTrackFound ];
				PlayList [ iTrackFound ] = PlayList [ iTrackFound ]->GetNext ();

				//
				//		Test if we have a SYSEX
				if ( pFoundList->GetEventIndex () == MIDI_EVENT_SYSEX )
				{
					//		Retrieve the sysex chunk
					pSysexChunk = pFoundList->GetSysexChunk ();
					if ( pSysexChunk != NULL )
					{
						//	Store it
						pPlayerChunk = 
							new CMidiPlayerChunk ( 
									MIDI_EVENT_SYSEX,
									pPreviousChunk, NULL, 
									iCurrentHMCm,
									iCurrentHMCm - iPreviousHMCm,
									pSysexChunk );
						if ( m_pLLStartChunk == NULL )
						{
							m_pLLStartChunk = pPlayerChunk;
						}

						if ( pPreviousChunk != NULL )
						{
							pPreviousChunk->SetNext ( pPlayerChunk );
						}

						iStored++;

						pPreviousChunk = pPlayerChunk;

						iPreviousHMCm = iCurrentHMCm;
					}
				}
				//	Other than Sysex
				else
				{
					//		Retrieve the chunk
					dwDwordChunk = pFoundList->GetChunk ();

					//		If the chunk is valid
					if ( dwDwordChunk != NULL )
					{
						bToBeStored = true;

						//		Avoid dupplicate events except for notes
						if (	pFoundList->GetEventIndex () != MIDI_EVENT_NOTEON
							 && pFoundList->GetEventIndex () != MIDI_EVENT_NOTEOFF )
						{
							//		If it is the same chunk as the previous
							if ( dwDwordChunk == dwOldChunk )
							{
								bToBeStored = false;
								iRemoved++;
							}

							//		Store the value by channel too
							iChannel = ( ( dwDwordChunk ) & CHANNEL_MASK );
							if ( dwChannelChunks [ iChannel ] == dwDwordChunk )
							{
								bToBeStored = false;
								iRemoved++;
							}
						}

						//		Store the chunk
						if ( bToBeStored )
						{
							//	Store it with the channel
							pPlayerChunk = 
								new CMidiPlayerChunk (
										pFoundList->GetEventIndex (),
										pPreviousChunk, NULL, 
										iCurrentHMCm,
										iCurrentHMCm - iPreviousHMCm,
										dwDwordChunk, 
										pFoundList->GetChannel (),
										iTrackFound );
							if ( m_pLLStartChunk == NULL )
							{
								m_pLLStartChunk = pPlayerChunk;
							}

							if ( pPreviousChunk != NULL )
							{
								pPreviousChunk->SetNext ( pPlayerChunk );
							}

							iStored++;

							pPreviousChunk = pPlayerChunk;

							iPreviousHMCm = iCurrentHMCm;

							//		Skip the code when similar
							if ( ( dwOldChunk   & 0xff ) == ( dwDwordChunk & 0xff ) )
							{
								pPlayerChunk->SkipCode();
								iSkipCode++;
							}

							//		Set the old value to this value
							dwOldChunk = dwDwordChunk;

							//		Store the value by channel too
							//		for events other than notes
							if (	pFoundList->GetEventIndex () != MIDI_EVENT_NOTEON
								 && pFoundList->GetEventIndex () != MIDI_EVENT_NOTEOFF )
							{
								iChannel = ( ( dwDwordChunk ) & CHANNEL_MASK );
								dwChannelChunks [ iChannel ] = dwDwordChunk;
							}

						}

						//	Continue with the tracks

					}	// if Chunk is ! NULL

				}	//	Else not a SYSEX

			} // If != NULL

		} // if iTrackFound >= 0
		else
		{
			bDone = TRUE;
		}

	} //	While ! bDone

	//		At the end
	return;

}

//
///////////////////////////////////////////////////////////////////////////////////
//		Kill event timer
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiFile::LLKillEventTimer()
{
	BOOL bRes = Lock ( );
	if ( m_wLLTimerID )
	{
		timeKillEvent ( m_wLLTimerID );
	}

	if ( bRes )
	{
		UnLock ( );
	}

	bMidiFileOneShotTimer = false;
	m_wLLTimerID = 0;

	return;

	int			iCount;

	//	Kill timer
	iCount = 0;
	while ( bMidiFileOneShotTimer && iCount < 100000 )
	{
		if ( m_wLLTimerID )
		{
			timeKillEvent ( m_wLLTimerID );
		}

		iCount++;
	}

	bMidiFileOneShotTimer = false;
	m_wLLTimerID = 0;

}

//
///////////////////////////////////////////////////////////////////////////////////
//		Pause the playing.
//
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLPause()
{

	//		First test if we are playing
	//		If we are paused: resume
	if ( LLIsPaused () )
	{
		return LLResume ();
	}

	if ( LLIsStopped ( ) )
	{
		return MMSYSERR_NOERROR;
	}

	//		Say we are not playing
	m_bLLState	= MIDI_PLAYER_PAUSED;

	//	Kill timer
	LLKillEventTimer ();

	//		End Timer
	if ( m_bLLTimerOk )
	{
		timeEndPeriod ( m_wLLTimerRes );
	}
	m_bLLTimerOk = false;

	//		Reset channel on this device
	//		And close the device
	if ( m_bLLDeviceOk )
	{
		LLSoundOff ();

#if 0
		LLReset ();

		if ( m_iSoundFont )
		{
			m_wLLResult = CFluidSynth::midiOutClose();
			m_wLLDeviceID = -1;
			m_bLLDeviceOk = false;
		}
		else
		{
			m_wLLResult = midiOutClose ( m_hLLMidiOut );
			m_wLLDeviceID = -1;
			m_bLLDeviceOk = false;
		}
#endif
	}

	//
	return MMSYSERR_NOERROR;

}

//
//
///////////////////////////////////////////////////////////////////////////////////
//		Resume playing
//
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLResume()
{
	//
	if ( MciIsPlaying() )
	{
		MciStop();
	}

	//
	UINT			dwResult;

	//		First test if we are playing
	//		If we are paused: resume
	if ( LLIsPlaying ( ) )
	{
		return MMSYSERR_NOERROR;
	}

	if ( LLIsStopped () )
	{
		return LLPlay ( m_hLLNotifyWnd );
	}

	//		Really paused so open device and go on
	dwResult = LLOpen ( true );
	if ( dwResult != MMSYSERR_NOERROR )
	{
		m_bLLState = MIDI_PLAYER_STOPPED;
		return dwResult;
	}

	return LLPlayRoutine( false, false, true );

}

//
///////////////////////////////////////////////////////////////////////////////////
//		Open device for playing
//
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLOpen ( boolean bResume )
{
	UINT				iX;

	//
	//	2012.01.07 Solving Problems ?
	Sleep ( 10 );

	//		If device is already known and already opened
	if ( m_bLLDeviceOk )
	{
		SetMidiErrorText ( "LLOpen the device is Ok" );
		if ( bResume )
		{
			//
			LLBeginTimer ();

			m_bLLState = MIDI_PLAYER_PLAYING;
		}

		return MMSYSERR_NOERROR;
	}

	//
	//	Sound Font
	//	Here we could open multiple sound font
	if ( m_iSoundFont )
	{
		for ( int i = 0; i < GetMidiFileTracksCount(); i++ )
		{
			int iTrackSize = GetMidiTrackSize ( i );
			if ( iTrackSize > 0 )
			{
				const char *pInstrument = GetInstrument(i);
				if ( pInstrument == NULL )
				{
					m_wLLReturn = CFluidSynth::midiOutOpen ( m_pSoundFont, this, i );
				}
				else
				{
					m_wLLReturn = CFluidSynth::midiOutOpen ( pInstrument, this, i );
				}
			}
		}
		if ( m_wLLReturn == MMSYSERR_NOERROR )
		{
			m_bLLDeviceOk	= true;
		}
	}
	//
	//	MM System
	else
	{
		//		If the device is already fixed get it
		if ( m_iMidiDevice >= 0 )
		{
			m_wLLDeviceID	= m_iMidiDevice;
			m_bLLDeviceOk	= true;
		}
		else
		{
			//		Search a device
			for  ( iX = 0; iX < midiOutGetNumDevs(); iX++ )
			{
				m_wLLResult = midiOutGetDevCaps( iX, & m_LLMidiOutCaps, sizeof ( m_LLMidiOutCaps ) );
				if ( m_wLLResult == MMSYSERR_NOERROR )
				{
					if ( m_LLMidiOutCaps.wTechnology == MOD_MAPPER )
					{
						m_bLLDeviceOk = true;
						m_wLLDeviceID = iX;
					}
				}
				else
				{
					SetMidiErrorText ( "LLOpen unable to find a device" );
					return m_wLLResult;
				}
			}

			//		Search a device
			if ( ! m_bLLDeviceOk )
			{
				for  ( iX = 0; iX < midiOutGetNumDevs(); iX++ )
				{
					m_wLLResult = midiOutGetDevCaps( iX, & m_LLMidiOutCaps, sizeof ( m_LLMidiOutCaps ) );
					if ( m_wLLResult == MMSYSERR_NOERROR )
					{
						if ( m_LLMidiOutCaps.wTechnology == MOD_FMSYNTH )
						{
							m_bLLDeviceOk = true;
							m_wLLDeviceID = iX;
							break;
						}


						if ( ! m_bLLDeviceOk )
						{
							m_bLLDeviceOk = true;
							m_wLLDeviceID = iX;
							if ( m_LLMidiOutCaps.wTechnology == MOD_SWSYNTH )
							{
								m_bLLDeviceOk = true;
								m_wLLDeviceID = iX;
								break;
							}
 						}
					}
					else
					{
						SetMidiErrorText ( "LLOpen unable to find a device" );
						return m_wLLResult;
					}
				}
			}

			//		Search a device
			if ( ! m_bLLDeviceOk )
			{
				for  ( iX = 0; iX < midiOutGetNumDevs(); iX++ )
				{
					m_wLLResult = midiOutGetDevCaps( iX, & m_LLMidiOutCaps, sizeof ( m_LLMidiOutCaps ) );
					if ( m_wLLResult == MMSYSERR_NOERROR )
					{
						if ( m_LLMidiOutCaps.wTechnology == MOD_MIDIPORT )
						{
							m_bLLDeviceOk = true;
							m_wLLDeviceID = iX;
 						}
					}
					else
					{
						SetMidiErrorText ( "LLOpen unable to find a device" );
						return m_wLLResult;
					}
				}
			}
		}
	
		//
		if ( ! m_bLLDeviceOk )
		{
			SetMidiErrorText ( "LLOpen the device is not ok" );
			return m_wLLResult;
		}

		//
		//		Now opens the device
		m_wLLReturn = 
			midiOutOpen(  &m_hLLMidiOut, m_wLLDeviceID, ( DWORD_PTR ) &MidiFileDone, ( DWORD_PTR ) this, CALLBACK_FUNCTION );
	}

	//
	if ( m_wLLReturn != MMSYSERR_NOERROR )
	{
		LLSetErrorText ( m_wLLReturn );
		SetMidiErrorText ( "LLOpen open fails" );

		m_bLLDeviceOk = false;
		m_wLLDeviceID = -1;

		return m_wLLReturn;
	}

	//		Reset Output
	if ( m_iSoundFont )
	{
		m_wLLResult = CFluidSynth::midiOutReset ( this );
		m_wLLResult = CFluidSynth::midiOutResetAll ( this );
	}
	else
	{
		m_wLLResult = midiOutReset( m_hLLMidiOut );
	}

	//
	if ( m_wLLResult != MMSYSERR_NOERROR )
	{
		SetMidiErrorText ( "LLOpen reset fails" );
		LLStop ();
		return m_wLLResult;
	}

	//
	LLBeginTimer ();

	m_bLLState = MIDI_PLAYER_PLAYING;

	return MMSYSERR_NOERROR;
}

//
///////////////////////////////////////////////////////////////////////////////////
//		Play the file scanning the tracks
//		for events.
//	bStart indicate a start of play
//	bTimer indicate we are called from a timer
//	bSysex indicate we are called from a sysex
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLPlayTracks(bool bStart, bool bTimer, bool bSysex )
{
	DWORD				DwordChunk;
	MIDIHDR				SysexChunk;
	unsigned			iType;
	unsigned long		iDeltaHMCm;
	unsigned char		iChannel;
	int					iTrack;

	//		Get the system time in milliseconds
	m_iLLSystemTime = timeGetTime ();

	//		If there is a pending timer just return
	if ( bMidiFileOneShotTimer )
	{
		return MMSYSERR_NOERROR;
	}

	//		Test if we are in Playing Mode
	if ( LLIsPaused () || LLIsStopped () )
	{
		SetMidiErrorText ( "LLPlayTracks We are in Pause or Stop Mode" );
		return MMSYSERR_NOERROR;
	}

	//		Test if device is OK
	if ( ! m_bLLDeviceOk )
	{
		SetMidiErrorText ( "LLPlayTracks The Device is not OK" );
		return MMSYSERR_NOERROR;
	}

	//		Test if we need to do a reset
	if ( m_bDoSoundOff )
	{
		LLSoundOff ();
		m_bDoSoundOff = false;
	}

	//		If it is the begining on the treatment
	if ( bStart )
	{
		//		Test if we need to skip
		if ( m_bSkipToFirstNote )
		{
			m_bSkippingDone = false;
		}
		else
		{
			m_bSkippingDone = true;
		}

		//		If no remaining timers
		LLFillChunkList ();

		//		Reset the absolute value of HMSm
		m_iLLCurrentHMSm = 0;

		//		Set the starting chunk to the begining
		m_pLLCurrentChunk = m_pLLStartChunk;

		if ( m_pLLCurrentChunk != NULL )
		{
			iType = m_pLLCurrentChunk->GetValues ( &m_iLLCurrentHMSm, &iDeltaHMCm, &DwordChunk, &SysexChunk, &iChannel, &iTrack );
			if ( iDeltaHMCm > 0 )
			{
				bTimer = true;
//				If we want to wait until the real start
//				we can use a timer.
//				LLTimer ( iDeltaHMCm );
//				return MMSYSERR_NOERROR;
			}
			else
			{
				//	Same as if we came from a timer
				bTimer = true;
			}
		}
		else
		{
			LLStop ();
			return MMSYSERR_NOERROR;
		}

	}	// if bStart

	//		Test if we come from a sysex
	if ( bSysex )
	{
		if ( m_pLLCurrentChunk != NULL )
		{
			iType = m_pLLCurrentChunk->GetValues ( &m_iLLCurrentHMSm, &iDeltaHMCm, &DwordChunk, &SysexChunk, &iChannel, &iTrack );
			if ( iDeltaHMCm > 0 )
			{
				LLOneShotTimer ( iDeltaHMCm );
				return MMSYSERR_NOERROR;
			}
			else
			{
				//	Same as if we came from a timer
				//	To treat immediatly the message
				bTimer = true;
			}
		}
		else
		{
			LLStop ();
			return MMSYSERR_NOERROR;
		}
	}

	//		Test if come from a timer
	if ( bTimer )
	{
		//		Yes so play next chunk with timers
		if ( m_iLLCurrentHMSm >= m_iLLStartingHMSm )
		{
			LLPlayChunk ( false, false );
		}
		else
		{
			LLPlayChunk ( true, true );
		}

		return MMSYSERR_NOERROR;
	}

	//		At the end
	return MMSYSERR_NOERROR;

}

//
///////////////////////////////////////////////////////////////////////////////////
//		Play a chunk
//		bNoTimer if true indicate we are slidding
//		bNoNotes if true indicate we do not play notes
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLPlayChunk( bool bNoTimer, bool bNoNotes )
{
	DWORD				DwordChunk;
	MIDIHDR				SysexChunk;
	unsigned			iType;
	unsigned long		iDeltaHMCm;
	unsigned char		iChannel;
	int					iTrack;

	if ( ! LLIsPlaying () )
	{
		return MMSYSERR_NOERROR;
	}

	//		If the chunk is null: stop
	if ( m_pLLCurrentChunk == NULL )
	{
		LLStop ();

		//		End of playing
		LLSendMCINotification ( 0, 0 );

		return MMSYSERR_NOERROR;
	}

	//		Now for all chunk in the list
	iType = m_pLLCurrentChunk->GetValues ( &m_iLLCurrentHMSm, &iDeltaHMCm, &DwordChunk, &SysexChunk, &iChannel, &iTrack );

	//
	do
	{
		//
		//		Test if we have a SYSEX
		if ( iType == MIDI_EVENT_SYSEX )
		{
			//		Get the next before doing anything
			//		This will avoid loops.
			//		This is to correct a Bug involving NT
			//		That seems to send the result message
			//		before we ended the routine
			m_pLLCurrentChunk = m_pLLCurrentChunk->GetNext ();

			if ( m_iSoundFont )
			{
				m_wLLResult = 
					CFluidSynth::midiOutLongMsg ( &SysexChunk, sizeof ( MIDIHDR ), iTrack );
			}
			else
			{
				//		Prepare it
				m_wLLResult = midiOutPrepareHeader( m_hLLMidiOut, &SysexChunk, sizeof ( MIDIHDR ) );
				if ( m_wLLResult != MMSYSERR_NOERROR )
				{
					LLSetErrorText ( m_wLLResult );
					SetMidiErrorText ( "LLPlayChunk midiOutPrepareHeader error" );
				}

				//		Send it
				m_wLLResult = midiOutLongMsg ( m_hLLMidiOut, &SysexChunk, sizeof ( MIDIHDR ) );
				if ( m_wLLResult != MMSYSERR_NOERROR )
				{
					LLSetErrorText ( m_wLLResult );
					SetMidiErrorText ( "LLPlayChunk midiOutLongMsg error" );
				}

				//		Un Prepare it
				/*
				 *
				m_wLLResult =
					midiOutUnprepareHeader( m_hLLMidiOut, 
						&SysexChunk, sizeof ( MIDIHDR ) );
				if ( m_wLLResult != MMSYSERR_NOERROR )
				{
					LLSetErrorText ( m_wLLResult );
					SetMidiErrorText ( "LLPlayChunk midiOutUnprepareHeader error" );
				}
				 *
				 */

				//		Return to program A MM_MOM_DONE will be received to continue
				return MMSYSERR_NOERROR;
			}


		}
		//
		//		Not a system exclusive
		//
		else
		{
			//		Get the next before doing anything
			//		This will avoid loops.
			//		This is to correct a Bug involving NT
			//		That seems to send the result message
			//		before we ended the routine
			m_pLLCurrentChunk = m_pLLCurrentChunk->GetNext ();

			//		Test if there is an imediate chunk
			//		This is something due to a modification and to be applied
			if ( m_bImmediateChunk )
			{
				if ( m_iSoundFont )
				{
					//	Stop Note on this channel
					m_wLLResult = CFluidSynth::midiOutShortMsg ( LLSoundOffChunk ( m_ImmediateChunk.GetChannel () ), iTrack );

					//	Play on this channel
					DwordChunk = m_ImmediateChunk.GetDwordChunk ();
					m_wLLResult =  CFluidSynth::midiOutShortMsg ( DwordChunk, iTrack );
				}
				else
				{
					//	Stop Note on this channel
					m_wLLResult = midiOutShortMsg ( m_hLLMidiOut, LLSoundOffChunk ( m_ImmediateChunk.GetChannel () ) );

					//	Play on this channel
					DwordChunk = m_ImmediateChunk.GetDwordChunk ();
					m_wLLResult = midiOutShortMsg ( m_hLLMidiOut, DwordChunk );
					if ( m_wLLResult != MMSYSERR_NOERROR )
					{
						LLSetErrorText ( m_wLLResult );
						SetMidiErrorText ( "LLPlayChunk midiOutShortMsg error" );
					}
				}

				//
				//	Chunk Applied : Reset
				m_bImmediateChunk = false;
			}

			//		If we are in skipping mode
			if ( m_bSkipToFirstNote && ! m_bSkippingDone )
			{
				//		if a note is encountered say skipping have been done
				if ( iType == MIDI_EVENT_NOTEON )
				{
					m_bSkippingDone = true;

					//	But Sleep just some time
					//	This is done only one time when we use skip to first note
					Sleep ( SLEEP_TIME_SYSEX );

					//		Get the system time in milliseconds
					//		To avoid a to fast start
					m_iLLSystemTime = timeGetTime ();

				}
			}

			if ( ! bNoNotes || ( iType != MIDI_EVENT_NOTEON ) )
			{
				bool		bToPlay = true;

				//	Test if we need to play the channel
				if ( iChannel != 0xff )
				{
					//		Record the current note and its volume
					if ( iType == MIDI_EVENT_NOTEOFF )
					{
						m_MidiChannels [ iChannel ].ResetVolume ( );
					}

					if ( iType == MIDI_EVENT_NOTEON )
					{
						m_MidiChannels [ iChannel ].SetCurrentNoteAndVolume ( DwordChunk );
					}

					//		Send a sound off on this channel
					if ( m_MidiChannels [ iChannel ].GetMuteState ( ) )
					{
						if ( iType == MIDI_EVENT_NOTEON )
						{
							bToPlay = false;
						}
					}
				}

				//	Test if we need to play the track
				if ( iTrack >= 0 )
				{
					if ( m_MidiFileTrack [ iTrack ].GetMuteState ( ) )
					{
						if ( iType == MIDI_EVENT_NOTEON )
						{
							bToPlay = false;
						}
					}
				}

				if ( bToPlay )
				{
					if ( m_iSoundFont )
					{
						m_wLLResult = CFluidSynth::midiOutShortMsg ( DwordChunk, iTrack );
					}
					else
					{
						m_wLLResult = midiOutShortMsg( m_hLLMidiOut, DwordChunk );
						if ( m_wLLResult != MMSYSERR_NOERROR )
						{
							LLSetErrorText ( m_wLLResult );
							SetMidiErrorText ( "LLPlayChunk midiOutShortMsg error" );
						}
					}
				}

			}

		}	//	Else not a SYSEX

		//		Do we need to start a timer
		if ( m_pLLCurrentChunk != NULL )
		{
			iType = m_pLLCurrentChunk->GetValues ( &m_iLLCurrentHMSm, &iDeltaHMCm, &DwordChunk, &SysexChunk, &iChannel, &iTrack );
		}
	}
	while ( m_pLLCurrentChunk != NULL && iType != MIDI_EVENT_SYSEX  &&	iDeltaHMCm == 0 );

	//		Start a new timer with the delta time
	//		that is in milli of seconds.
	if ( m_pLLCurrentChunk != NULL )
	{
		if ( bNoTimer  )
		{
			LLOneShotTimer ( MIN_WAIT_TIME * 1 );
		}
		else
		{
			//		If we are not in skipping mode
			//		or the skipping have been done
			//		let a correct delta time
			if ( ! m_bSkipToFirstNote || m_bSkippingDone )
			{
				LLOneShotTimer ( iDeltaHMCm );
			}
			//
			//		Else set a small delta time
			//
			else
			{
				LLOneShotTimer ( MIN_WAIT_TIME * 20 );
			}
		}
	}
	else
	{
		//		If we are playing loop
		//		set the current chunk to the starting chunk
		if ( m_bLLPlayAndLoop && m_pLLStartChunk != NULL )
		{
			//		Test if we need to skip
			if ( m_bSkipToFirstNote )
			{
				m_bSkippingDone = false;
			}
			else
			{
				m_bSkippingDone = true;
			}

			m_pLLCurrentChunk	= m_pLLStartChunk;
			m_iLLCurrentHMSm	= 0;
			m_iLLStartingHMSm	= 0;
			LLOneShotTimer ( MIN_WAIT_TIME * 20 );
		}
		else
		{
			//		Stop
			LLStop ();

			//		End of playing
			LLSendMCINotification ( 0, 0 );

			return MMSYSERR_NOERROR;
		}
	}

	return MMSYSERR_NOERROR;

}

//
///////////////////////////////////////////////////////////////////////////////////
//		Stop playing
//
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLStop()
{

	//		Say we are not playing
	m_bLLState	= MIDI_PLAYER_STOPPED;

	//		Say we stop the loop
	m_bLLPlayAndLoop = false;

	//		End the Timer
	LLEndTimer ();

	//		Close the Device
	LLClose ();

	return MMSYSERR_NOERROR;

}

//
///////////////////////////////////////////////////////////////////////////////////
//		A low level Play
//
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLPlay ( HWND hWnd )
{
	UINT			dwResult;

	//
	if ( MciIsPlaying() )
	{
		MciStop();
	}

	//		Update all values
	Update ( );

	//
	m_iLLStartingHMSm		= 0;
	m_hLLNotifyWnd			= hWnd;

	if ( LLIsStopped () || LLIsPaused () )
	{
		dwResult = LLOpen ( false );
		if ( dwResult != MMSYSERR_NOERROR )
		{
			SetMidiErrorText ( "LLPlay We cannot open the device" );
			m_bLLState = MIDI_PLAYER_STOPPED;
			return dwResult;
		}
	}

	//		Start playing tracks.
	return LLPlayRoutine ( true, false, false );

}

//
///////////////////////////////////////////////////////////////////////////////////
//		A low level Play
//
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLSlide(unsigned long iSeconds )
{
	UINT			dwResult;

	//		Update all values
	Update ( );

	//		Set the new starting value.
	m_iLLStartingHMSm		= iSeconds * 1000;

	if ( LLIsStopped () || LLIsPaused ( ) )
	{
		dwResult = LLOpen ( true );
		if ( dwResult != MMSYSERR_NOERROR )
		{
			SetMidiErrorText ( "LLSlide We cannot open the device when sliding" );
			m_bLLState = MIDI_PLAYER_STOPPED;
			return dwResult;
		}

		return LLPlayRoutine ( true, false, false );

	}

	//		If device is not OK
	if ( ! m_bLLDeviceOk )
	{
		SetMidiErrorText ( "LLSlide The Device is not Ok when sliding" );
		m_bLLState = MIDI_PLAYER_STOPPED;
		return MMSYSERR_NOERROR;
	}

	//		Reset the absolute value of HMSm
	if ( m_iLLCurrentHMSm > m_iLLStartingHMSm )
	{
		//		Set the starting chunk to the begining
		m_pLLCurrentChunk = m_pLLStartChunk;
	}

	//		Just recall that we will do a reset the next time
	//		in play track
	m_bDoSoundOff = true;

	//		Since we are already playing just return
	//	return LLPlayRoutine ( false, false, false );
	return MMSYSERR_NOERROR;

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
DWORD CMidiFile::LLResetChunk(int iChannel)
{
	DWORD					dwChunk;

	//	0xb0 is controller with 0x79 reset all controller
	dwChunk = ( 0xb0 | iChannel ) + 256 * ( 0x79 + 256 * 0 );

	return dwChunk;

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
DWORD CMidiFile::LLSoundOffChunk(int iChannel)
{
	DWORD					dwChunk;

	//	0xb0 is controller with 0x78 all note off
	dwChunk = ( 0xb0 | iChannel ) + 256 * ( 0x78 + 256 * 0 );

	return dwChunk;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLSoundOff()
{
	int						iX;
	DWORD					dwChunk;

	if ( m_bLLDeviceOk )
	{
		//		All Sound Off
		for ( iX = 0; iX <= 15; iX++ )
		{
			//		Send an All Sound Off
			dwChunk = LLSoundOffChunk ( iX );
			if ( m_iSoundFont )
			{
				CFluidSynth::midiOutShortMsg ( dwChunk, iX );
			}
			else
			{
				midiOutShortMsg( m_hLLMidiOut, dwChunk );
			}
		}

	}

	return MMSYSERR_NOERROR;
}

//
///////////////////////////////////////////////////////////////////////////////////
//		Reset
//
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLReset()
{
	int						iX;
	DWORD					dwChunk;

	if ( m_bLLDeviceOk )
	{
		//		Reset All Channels
		for ( iX = 0; iX <= 15; iX++ )
		{
			//		Send a Reset All Controller
			dwChunk = LLResetChunk ( iX );
			if ( m_iSoundFont )
			{
				CFluidSynth::midiOutShortMsg( dwChunk, iX );
			}
			else
			{
				midiOutShortMsg( m_hLLMidiOut, dwChunk );
			}
		}

		if ( m_iSoundFont )
		{
			m_wLLResult = CFluidSynth::midiOutReset ( this );
			m_wLLResult = CFluidSynth::midiOutResetAll ( this );
		}
		else
		{
			m_wLLResult = midiOutReset( m_hLLMidiOut );
		}

	}

	return MMSYSERR_NOERROR;

}

//
///////////////////////////////////////////////////////////////////////////////////
//		Close file
//
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLClose()
{
	//		Close device
	if ( m_bLLDeviceOk )
	{
		LLSoundOff ();
		LLReset ();

		if ( m_iSoundFont )
		{
			m_wLLResult = CFluidSynth::midiOutClose ( this );
			m_wLLResult = CFluidSynth::midiOutCloseAll ( this );
		}
		else
		{
			m_wLLResult = midiOutClose ( m_hLLMidiOut );
		}
	}

	m_wLLDeviceID = -1;

	m_bLLDeviceOk = false;

	return MMSYSERR_NOERROR;
}

//
///////////////////////////////////////////////////////////////////////////////////
//		Kill / End a timer
//
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLEndTimer()
{
	//	Kill timer
	LLKillEventTimer ();

	//		End Timer
	if ( m_bLLTimerOk )
	{
		timeEndPeriod ( m_wLLTimerRes );
	}
	m_bLLTimerOk = false;

	return MMSYSERR_NOERROR;
}

//
///////////////////////////////////////////////////////////////////////////////////
//		Initialize a timer
//
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLBeginTimer()
{
	//		Initialize Timers with 1 milli second
#define	TARGET_RESOLUTION		1

	TIMECAPS		tc;

	if ( timeGetDevCaps ( &tc, sizeof (TIMECAPS ) ) != TIMERR_NOERROR )
	{
		m_bLLTimerOk = false;
	}

	//		Get the timer resolution.
	m_wLLTimerRes = min ( max ( tc.wPeriodMin, TARGET_RESOLUTION ), tc.wPeriodMax );

	//		Begin timer.
	timeBeginPeriod ( m_wLLTimerRes );

	m_bLLTimerOk = true;

	return MMSYSERR_NOERROR;

}

//
///////////////////////////////////////////////////////////////////////////////////
//		Set the reading speed
//
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLSetSpeed(unsigned int iSpeed)
{
	m_iLLSpeedMul = 100;
	m_iLLSpeedDiv = ( 100 + iSpeed );
	return MMSYSERR_NOERROR;

}

//
///////////////////////////////////////////////////////////////////////////////////
//		Start a timer
//
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLOneShotTimer ( long iDelta )
{
	unsigned long	iTime;

	//		Get the current time
	iTime = timeGetTime ();

	//		iTime is now the time for computation
	//		of the music.
	//		It must be substract from the delta.
	iTime -= m_iLLSystemTime;

	//		Compute new delta time.
	iDelta -= iTime;
	if ( iDelta <= 0 )
	{
		iDelta = MIN_WAIT_TIME;
	}

	//		Say we have a timer now
	bMidiFileOneShotTimer = true;

	iDelta *= m_iLLSpeedMul;
	iDelta /= m_iLLSpeedDiv;

	if ( iDelta <= 0 )
	{
		iDelta = MIN_WAIT_TIME;
	}

	m_wLLTimerID = timeSetEvent ( iDelta, m_wLLTimerRes, ( LPTIMECALLBACK ) &MidiFileOneShotTimer, ( DWORD_PTR ) this, TIME_ONESHOT );

	return m_wLLTimerID;

}

//
///////////////////////////////////////////////////////////////////////////////////
//		Play and loop
//
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLPlayAndLoop( HWND hWnd )
{
	//
	if ( MciIsPlaying() )
	{
		MciStop();
	}

	//
	m_bLLPlayAndLoop = true;

	return LLPlay ( hWnd );
}

//
///////////////////////////////////////////////////////////////////////////////////
//		Set error text
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiFile::LLSetErrorText(unsigned int iError)
{
	char	szText [ MAX_LONG_STRING ];

	midiOutGetErrorText(  iError, szText, sizeof ( szText ) - 1 );

	SetMidiError ();
	SetMidiErrorText ( szText );
}

//
///////////////////////////////////////////////////////////////////////////////////
//		Set the skipping mode
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiFile::LLSetSkippingMode(bool bSkip)
{
	m_bSkipToFirstNote = bSkip;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiFile::LLSendMCINotification(WPARAM wParam, LPARAM lParam)
{
	if ( m_hLLNotifyWnd != NULL )
	{
		::PostMessage ( m_hLLNotifyWnd, MM_MCINOTIFY, wParam, lParam );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//	bStart indicate a start of play
//	bTimer indicate we are called from a timer
//	bSysex indicate we are called from a sysex
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLPlayRoutine (bool bStart, bool bTimer, bool bSysex)
{
		BOOL bRes = Lock ( );

		UINT iRes = LLPlayTracks ( bStart, bTimer, bSysex );

		if ( bRes )
		{
			UnLock ( );
		}

		return iRes;
}

//
///////////////////////////////////////////////////////////////////////////////////
//		Open device for playing
//
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLInpOpen()
{
	UINT				iX;

	//		If device is already known and already opened
	if ( m_bLLDeviceInpOk )
	{
		SetMidiErrorText ( "LLInpOpen the device is Ok" );
		return MMSYSERR_NOERROR;
	}

	//		If the device is already fixed get it
	if ( m_iMidiInpDevice >= 0 )
	{
		m_wLLDeviceInpID	= m_iMidiInpDevice;
		m_bLLDeviceInpOk	= true;
	}
	else
	{
		//		Search a device
		for  ( iX = 0; iX < midiInGetNumDevs(); iX++ )
		{
			m_wLLResult = midiInGetDevCaps( iX, & m_LLMidiInpCaps, sizeof ( m_LLMidiInpCaps ) );
			if ( m_wLLResult == MMSYSERR_NOERROR )
			{
				m_bLLDeviceInpOk = true;
				m_wLLDeviceInpID = iX;
			}
			else
			{
				LLSetErrorInpText(m_wLLResult);
				SetMidiErrorText ( "LLInpOpen unable to find a device" );
				return m_wLLResult;
			}
		}
	}
	
	if ( ! m_bLLDeviceInpOk )
	{
		SetMidiErrorText ( "LLInpOpen the device is not ok" );
		return m_wLLResult;
	}

	//
	//		Now opens the device
	m_wLLReturn = 
		midiInOpen(  &m_hLLMidiInp, m_wLLDeviceInpID,
						( DWORD_PTR ) &MidiFileInpDone , ( DWORD_PTR ) this,  
						CALLBACK_FUNCTION );
	if ( m_wLLReturn != MMSYSERR_NOERROR )
	{
		LLSetErrorInpText ( m_wLLReturn );
		SetMidiErrorText ( "LLInpOpen open fails" );

		m_bLLDeviceInpOk = false;
		m_wLLDeviceInpID = -1;

		return m_wLLReturn;
	}

	//		Reset Output
	m_wLLResult = midiInReset( m_hLLMidiInp );
	if ( m_wLLResult != MMSYSERR_NOERROR )
	{
		LLSetErrorInpText(m_wLLResult);
		SetMidiErrorText ( "LLInpOpen reset fails" );
		LLInpStop ();
		return m_wLLResult;
	}

	return MMSYSERR_NOERROR;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLInpStop()
{
	//		Unprepare device
	m_wLLResult = midiInUnprepareHeader ( m_hLLMidiInp, &m_LLMidiHdr, sizeof ( m_hLLMidiInp ) );

	//		Close the Device
	if ( m_bLLDeviceInpOk )
	{
		m_wLLResult = midiInStop ( m_hLLMidiInp );
	}

	LLInpClose ();

	m_bLLInputState			= MIDI_RECORDER_INACTIVE;

	return MMSYSERR_NOERROR;

}

//
///////////////////////////////////////////////////////////////////////////////////
//		Close file
//
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLInpClose()
{
	//		Close device
	if ( m_bLLDeviceInpOk )
	{
		LLInpReset ();
		m_wLLResult = midiInClose ( m_hLLMidiInp );
	}

	m_wLLDeviceInpID = -1;

	m_bLLDeviceInpOk = false;

	return MMSYSERR_NOERROR;
}

//
///////////////////////////////////////////////////////////////////////////////////
//		Reset
//
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLInpReset()
{

	if ( m_bLLDeviceInpOk )
	{
		m_wLLResult = midiInReset( m_hLLMidiInp );
	}

	return MMSYSERR_NOERROR;

}

//
///////////////////////////////////////////////////////////////////////////////////
//		A low level Play
//		This is to record sysex
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLInpRecord ( HWND hWnd )
{
	UINT			dwResult;

	if ( m_bLLInputState != MIDI_RECORDER_INACTIVE )
	{
		return MMSYSERR_NOERROR;
	}

	//		Update all values
	m_hLLNotifyWnd			= hWnd;

	dwResult = LLInpOpen ();
	if ( dwResult != MMSYSERR_NOERROR )
	{
		LLSetErrorInpText(m_wLLResult);
		SetMidiErrorText ( "LLPlay We cannot open the device" );
		return dwResult;
	}

	//		Start recording tracks.
	return LLInpStart ( );

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
UINT CMidiFile::LLInpStart ( )
{
	//		Delete existing chunk
	if ( m_pLLStartInputChunk != NULL )
	{
		DeleteInputChunkList ();
	}

	//		Set the current chunk to NULL
	m_pLLCurrentInputChunk = m_pLLStartInputChunk;

	//	Reset the current header
	memset ( &m_LLMidiHdr, 0, sizeof ( m_LLMidiHdr ) );
	m_LLMidiHdr.lpData			= new char [ MIDI_BUFFER_SIZE ];
	m_LLMidiHdr.dwBufferLength	= MIDI_BUFFER_SIZE;

	//	Prepare the new
	m_wLLResult = midiInPrepareHeader ( m_hLLMidiInp, &m_LLMidiHdr, sizeof ( m_LLMidiHdr ) );
	if ( m_wLLResult != MMSYSERR_NOERROR )
	{
		LLSetErrorInpText(m_wLLResult);
		SetMidiErrorText ( "LLPlay We cannot prepare the header" );
		LLInpStop();
		return m_wLLResult;
	}

	//	Add to the buffer
	m_wLLResult = midiInAddBuffer( m_hLLMidiInp, &m_LLMidiHdr, sizeof ( m_LLMidiHdr ) );
	if ( m_wLLResult != MMSYSERR_NOERROR )
	{
		LLSetErrorInpText(m_wLLResult);
		SetMidiErrorText ( "LLPlay We cannot add the header" );
		LLInpStop();
		return m_wLLResult;
	}

	//	Now start input
	m_wLLResult = midiInStart ( m_hLLMidiInp );
	if ( m_wLLResult != MMSYSERR_NOERROR )
	{
		LLSetErrorInpText(m_wLLResult);
		SetMidiErrorText ( "LLPlay We cannot start" );
		LLInpStop();
		return m_wLLResult;
	}

	m_bLLInputState			= MIDI_RECORDER_RECORDING;

	return m_wLLResult;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiFile::LLSetErrorInpText(unsigned int iError)
{
	char	szText [ MAX_LONG_STRING ];

	midiInGetErrorText(  iError, szText, sizeof ( szText ) - 1 );

	SetMidiError ();
	SetMidiErrorText ( szText );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
MIDIHDR *CMidiFile::LLGetHeaderAddress ( )
{
	return &m_LLMidiHdr;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiFile::LLInpRecordChunk ( DWORD_PTR dwChunk, DWORD_PTR dwParam2 )
{
	//		Test if it is something like a message.
	DWORD	dwLow = dwChunk & OPERATION_MASK;
	if ( dwLow != MIDI_EVENT_CODE_SYSEX )
	{
		LockInput ( );

		CMidiInputChunk *pInputChunk = 
		new CMidiInputChunk (
			MIDI_EVENT_UNKNOWN,
			m_pLLCurrentInputChunk, NULL, 
			(DWORD) dwParam2,
			(DWORD) dwChunk );

		m_iNumberOfInputChunk++;

		if ( m_pLLCurrentInputChunk != NULL )
		{
			m_pLLCurrentInputChunk->SetNext ( pInputChunk );
		}

		m_pLLCurrentInputChunk = pInputChunk;

		if ( m_pLLStartInputChunk == NULL )
		{
			m_pLLStartInputChunk = m_pLLCurrentInputChunk;
		}

		UnLockInput ( );

	}

	return;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiFile::LLInpRecordChunk ( MIDIHDR *pSysex, DWORD_PTR dwParam2 )
{
	if ( pSysex )
	{
		if ( pSysex->dwBytesRecorded > 0 )
		{
			LockInput ( );

			CMidiInputChunk *pInputChunk = 
				new CMidiInputChunk (
					MIDI_EVENT_SYSEX,
					m_pLLCurrentInputChunk, NULL, 
					(DWORD) dwParam2,
					pSysex );

			m_iNumberOfInputChunk++;

			if ( m_pLLCurrentInputChunk != NULL )
			{
				m_pLLCurrentInputChunk->SetNext ( pInputChunk );
			}

			m_pLLCurrentInputChunk = pInputChunk;

			if ( m_pLLStartInputChunk == NULL )
			{
				m_pLLStartInputChunk = m_pLLCurrentInputChunk;
			}

			UnLockInput ( );
		}
	}

	return;
}
