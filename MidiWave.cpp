// MidiWave.cpp: implementation of the CMidiWave class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MidiWave.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "MidiGlassApp.h"

extern CMidiWorksApp	theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMidiWave::CMidiWave()
{
	m_wDeviceID			= -1;
	m_dwReturn			= 0;
	m_pMidiFile			= NULL;
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
CMidiWave::~CMidiWave()
{
	if ( m_pMidiFile != NULL )
	{
		m_pMidiFile->LLStop ( );

		delete m_pMidiFile;
		m_pMidiFile = NULL;
	}

	if ( m_wDeviceID != -1 )
	{
		m_dwReturn = mciSendCommand ( m_wDeviceID, MCI_STOP, MCI_WAIT, NULL );
		m_dwReturn = mciSendCommand ( m_wDeviceID, MCI_CLOSE, MCI_WAIT, NULL );

		m_wDeviceID			= -1;
	}

}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
DWORD CMidiWave::Record ( const char *pMidifile, int bSkipMode,
						 int wMidiDevice, const char *pWavedir,
						 HWND hWnd )
{
    //  Stop Current Playing
    CMainFrame *pMainFrame = dynamic_cast<CMainFrame *> ( AfxGetMainWnd( ) );
    if ( pMainFrame != NULL )
    {
        CMidiDoc			*pMididoc	= pMainFrame->GetMidiDoc ();
        CMidiFile			*pMidifile	= pMainFrame->GetMidiFile ( );
        if ( pMidifile != NULL )
        {
            if ( pMidifile->LLIsPlaying() )
            {
                pMidifile->LLStop();
            }

            if ( pMidifile->MciIsPlaying() )
            {
                pMidifile->MciStop();
            }
        }
    }

	m_WaveDirectory = pWavedir;
	m_hWnd			= hWnd;

	/*
	 *		Open a waveform device with a new file name
	 *		for recording.
	 */
	// MCI_DEVTYPE_WAVEFORM_AUDIO;
	m_mciOpenParms.lpstrDeviceType	= "waveaudio";
	m_mciOpenParms.lpstrElementName	= "";

	m_dwReturn = mciSendCommand ( 0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE | MCI_WAIT,	(DWORD_PTR) & m_mciOpenParms );

	//		Test if failed.
	if ( m_dwReturn )
	{
		m_wDeviceID			= -1;
		return m_dwReturn;
	}

	/*
	 *		Set parameters.
	 */
	m_mciWaveSetParms.wFormatTag		= WAVE_FORMAT_PCM;
	m_mciWaveSetParms.nChannels			= 2;		//	Stereo
	m_mciWaveSetParms.nSamplesPerSec	= 44100;	//	CD Quality
	m_mciWaveSetParms.wBitsPerSample	= 16;		//	16 bits
	m_mciWaveSetParms.nAvgBytesPerSec	= 
		( (	m_mciWaveSetParms.wBitsPerSample) / 8) *
			m_mciWaveSetParms.nChannels *
			m_mciWaveSetParms.nSamplesPerSec;
	m_mciWaveSetParms.nBlockAlign = 
		( (	m_mciWaveSetParms.wBitsPerSample) / 8) *
			m_mciWaveSetParms.nChannels;
   
	m_dwReturn = 
		mciSendCommand ( m_wDeviceID, MCI_SET,
							MCI_WAIT |
							MCI_WAVE_SET_FORMATTAG |
							MCI_WAVE_SET_BITSPERSAMPLE |
							MCI_WAVE_SET_CHANNELS |
							MCI_WAVE_SET_SAMPLESPERSEC |
							MCI_WAVE_SET_AVGBYTESPERSEC |
							MCI_WAVE_SET_BLOCKALIGN,
							( DWORD_PTR ) &m_mciWaveSetParms );
	if ( m_dwReturn )
	{
		mciSendCommand ( m_wDeviceID, MCI_CLOSE, MCI_WAIT, NULL );
		m_wDeviceID			= -1;
		return m_dwReturn;
	}

	/*
	 *		Device opened successfully.
	 *		Get device Id.
	 */
	m_wDeviceID = m_mciOpenParms.wDeviceID;

	/*
	 *		Starts playing.
	 */
	m_pMidiFile	= new CMidiFile ( );
	m_pMidiFile->ReadMidiFile ( pMidifile );

	m_pMidiFile->LLSetSkippingMode ( true );
	m_pMidiFile->SetMidiDevice ( wMidiDevice );
	m_pMidiFile->SetSoundFont ( theApp.m_SoundFontFile, theApp.m_bUse_Sound_Font );
	m_pMidiFile->LLPlay ( hWnd );

	/*
	 *		Begins recording.
	 *		Do not Wait for the recording to complete.
	 */
	m_mciRecordParms.dwTo = 0;
	m_dwReturn = mciSendCommand ( m_wDeviceID, MCI_RECORD, 0, ( DWORD_PTR ) &m_mciRecordParms );
	if ( m_dwReturn )
	{
		mciSendCommand ( m_wDeviceID, MCI_CLOSE, MCI_WAIT, NULL );
		m_wDeviceID			= -1;
		return m_dwReturn;
	}

	return m_dwReturn;
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
DWORD CMidiWave::Stop()
{

	if ( m_pMidiFile != NULL )
	{
		m_pMidiFile->LLStop ( );

		delete m_pMidiFile;
		m_pMidiFile = NULL;
	}

	if ( m_wDeviceID != -1 )
	{
		m_dwReturn = mciSendCommand ( m_wDeviceID, MCI_STOP, MCI_WAIT, NULL );

		m_dwReturn = mciSendCommand ( m_wDeviceID, MCI_CLOSE, MCI_WAIT, NULL );
	}

	m_wDeviceID			= -1;

	return m_dwReturn;
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
char * CMidiWave::GetShort(const char *pString)
{
	char		*pReturned;
	unsigned	iLen;

	iLen = (unsigned int) strlen ( pString );
	pReturned = ( char * ) pString + iLen;
	while ( pReturned > pString )
	{
		if ( *pReturned == '\\' )
		{
			return pReturned + 1;
		}
		pReturned--;
	}
	
	return ( pReturned );
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
DWORD CMidiWave::Save()
{
	char		szFilename [ MAX_PATHNAME_STRING ];

	if ( m_wDeviceID != -1 )
	{
		m_dwReturn = mciSendCommand ( m_wDeviceID, MCI_STOP, MCI_WAIT, NULL );

		strcpy_s ( szFilename, sizeof ( szFilename ), m_WaveDirectory );
		strcat_s ( szFilename, sizeof ( szFilename ), "\\" );
		strcat_s ( szFilename, sizeof ( szFilename ),
					GetShort ( m_pMidiFile->GetMidiPathName ( ) ) );
		strcat_s ( szFilename, sizeof ( szFilename ), ".wav" );

		if ( m_pMidiFile != NULL )
		{
			m_pMidiFile->LLStop ( );

			delete m_pMidiFile;
			m_pMidiFile = NULL;
		}

		m_mciSaveParms.lpfilename	= szFilename;
//		m_mciSaveParms.dwCallback	= ( DWORD )( m_hWnd );

		m_dwReturn = mciSendCommand ( m_wDeviceID, MCI_SAVE,
			MCI_SAVE_FILE | MCI_WAIT, ( DWORD_PTR ) & m_mciSaveParms );
		if ( m_dwReturn )
		{
			mciSendCommand ( m_wDeviceID, MCI_CLOSE, MCI_WAIT, NULL );
			m_wDeviceID			= -1;
			return m_dwReturn;
		}

		//		Delete content.
		MCI_WAVE_DELETE_PARMS		m_mciDeleteParms;

		m_mciDeleteParms.dwFrom		= 0;
//		m_mciDeleteParms.dwTo		= 60 * 60 * 1000;

		m_dwReturn = 
			mciSendCommand ( m_wDeviceID, MCI_DELETE, 
								MCI_FROM | MCI_WAIT, 
								(DWORD_PTR) & m_mciDeleteParms );

		m_dwReturn = mciSendCommand ( m_wDeviceID, MCI_CLOSE, MCI_WAIT, NULL );
		m_wDeviceID			= -1;
		return m_dwReturn;
	}

	return 0;

}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
DWORD CMidiWave::Play(const char *pWaveFile, HWND hWnd)
{
	m_hWnd			= hWnd;

	/*
	 *		Open a waveform device.
	 */
	m_mciOpenParms.lpstrDeviceType	= "waveaudio";
	m_mciOpenParms.lpstrElementName	= pWaveFile;

	m_dwReturn = mciSendCommand ( 0, 
		MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE | MCI_WAIT,
		(DWORD_PTR) & m_mciOpenParms );

	//		Test if failed.
	if ( m_dwReturn )
	{
		m_wDeviceID			= -1;
		return m_dwReturn;
	}

	/*
	 *		Device opened successfully.
	 *		Get device Id.
	 */
	m_wDeviceID = m_mciOpenParms.wDeviceID;

	/*
	 *		Start Playing.
	 *		Do not Wait for the Playing to complete.
	 */
	m_mciPlayParms.dwCallback = ( DWORD_PTR ) ( ( DWORD ) hWnd );
	m_dwReturn = mciSendCommand ( m_wDeviceID, MCI_PLAY, MCI_NOTIFY, ( DWORD_PTR ) &m_mciPlayParms );
	if ( m_dwReturn )
	{
		mciSendCommand ( m_wDeviceID, MCI_CLOSE, MCI_WAIT, NULL );
		m_wDeviceID			= -1;
		return m_dwReturn;
	}

	return m_dwReturn;
}
