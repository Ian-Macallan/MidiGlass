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

//
///////////////////////////////////////////////////////////////////////////////////
// Code
//
///////////////////////////////////////////////////////////////////////////////////
DWORD CMidiFile::MciPlayOnce(HWND hWndNotify)
{
	m_bPlayAndLoop = false;

	return MciPlay ( hWndNotify );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
DWORD CMidiFile::MciStop()
{
	m_bPlayAndLoop	= false;
	m_bMciPlaying	= false;

	if ( m_bMidiDeviceOk )
	{
		m_dwMidiReturn = mciSendCommand (m_wMidiDeviceID, MCI_STOP, 0, NULL );

		m_dwMidiReturn = mciSendCommand (m_wMidiDeviceID, MCI_CLOSE, 0, NULL );

		m_wMidiDeviceID = 0;

		m_bMidiDeviceOk	= false;

		if ( m_dwMidiReturn != MMSYSERR_NOERROR )
		{
			SetMidiError ();
		}

		char	szErrorText [ 128 ];
		strerror_s ( szErrorText, sizeof ( szErrorText ), m_dwMidiReturn );
		SetMidiErrorText ( szErrorText );

		return m_dwMidiReturn;
	}

	return MMSYSERR_NOERROR;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
DWORD CMidiFile::MciPlayAndLoop(HWND hWndNotify)
{
	//
	if ( LLIsPlaying() )
	{
		LLStop();
	}

	m_bPlayAndLoop = true;

	return MciPlay ( hWndNotify );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
DWORD CMidiFile::MciPlay(HWND hWndNotify)
{
	//
	if ( LLIsPlaying() )
	{
		LLStop();
	}

	//		Open the Device
	m_MidiOpenParms.lpstrDeviceType		= "sequencer";
	m_MidiOpenParms.lpstrElementName	= m_pMidiFilename;
	if ( m_dwMidiReturn = mciSendCommand ( NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR) &m_MidiOpenParms ) )
	{
		//		Failed
		return	m_dwMidiReturn;
	}

	//		Device succesfully opened: get device ID
	m_wMidiDeviceID = m_MidiOpenParms.wDeviceID;
	m_bMidiDeviceOk = true;

	//		See if the output port is the midi mapper.
	m_MidiStatusParms.dwItem = MCI_SEQ_STATUS_PORT;
	if ( m_dwMidiReturn = mciSendCommand (m_wMidiDeviceID, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR) &m_MidiStatusParms ) )
	{
		//		Failed
		mciSendCommand (m_wMidiDeviceID, MCI_CLOSE, 0, NULL );
		return m_dwMidiReturn;
	}

	//		Begin Playback.
	m_MidiPlayParms.dwCallback = (DWORD_PTR) hWndNotify;
	m_MidiPlayParms.dwFrom		= 0;
	if ( m_dwMidiReturn = mciSendCommand ( m_wMidiDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR) &m_MidiPlayParms ) )
	{
		//		Failed
		mciSendCommand (m_wMidiDeviceID, MCI_CLOSE, 0, NULL );

		if ( m_dwMidiReturn != MMSYSERR_NOERROR )
		{
			SetMidiError ();
		}

		char	szErrorText [ 128 ];
		strerror_s ( szErrorText, sizeof ( szErrorText ), m_dwMidiReturn );
		SetMidiErrorText ( szErrorText );

		return m_dwMidiReturn;
	}

	m_bMciPlaying = true;

	return MMSYSERR_NOERROR;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
bool CMidiFile::MciGetPlayValue() const
{
	return m_bPlayAndLoop;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
bool CMidiFile::MciIsPlaying() const
{
	return m_bMciPlaying;
}
