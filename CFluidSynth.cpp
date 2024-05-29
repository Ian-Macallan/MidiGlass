#include "stdafx.h"

#include "MidiGlassApp.h"

#include "CFluidSynth.h"

#include <fluidsynth.h>

#include "strstr.h"

extern	CMidiWorksApp theApp;

static char szFluidMessage [ MAX_PATH ] = "";

//
class CriticalSectionClass
{
    private:
        static CCriticalSection *m_pCriticalSection;
        static bool             m_bCriticalSectionInit;

    public :
        //
        CriticalSectionClass()
        {
        }

        //
        static void create()
        {
            if ( ! m_bCriticalSectionInit )
            {
                m_pCriticalSection      = new CCriticalSection();
                m_bCriticalSectionInit  = true;
            }
        }

        //
        void Lock()
        {
            if ( m_pCriticalSection != NULL )
            {
                m_pCriticalSection->Lock();
            }
        }

        //
        void Unlock()
        {
            if ( m_pCriticalSection != NULL )
            {
                m_pCriticalSection->Unlock();
            }
        }

        //
        virtual ~CriticalSectionClass()
        {
            Unlock();
        }
};

static CriticalSectionClass criticalSectionStatic;

bool CriticalSectionClass::m_bCriticalSectionInit           = false;
CCriticalSection *CriticalSectionClass::m_pCriticalSection  = NULL;

//
//////////////////////////////////////////////////////////////////////////////////////
//	Init Static
//////////////////////////////////////////////////////////////////////////////////////
	MACRO_FCT_STATIC_SET(new_fluid_settings)
	MACRO_FCT_STATIC_SET(fluid_settings_setint)
	MACRO_FCT_STATIC_SET(fluid_settings_setnum)
	MACRO_FCT_STATIC_SET(fluid_settings_setstr)

	MACRO_FCT_STATIC_SET(new_fluid_synth)
	MACRO_FCT_STATIC_SET(fluid_synth_sfload)

	MACRO_FCT_STATIC_SET(new_fluid_audio_driver)

	MACRO_FCT_STATIC_SET(fluid_synth_noteon)
	MACRO_FCT_STATIC_SET(fluid_synth_noteoff)
	MACRO_FCT_STATIC_SET(fluid_synth_cc)
	MACRO_FCT_STATIC_SET(fluid_synth_get_cc)
	MACRO_FCT_STATIC_SET(fluid_synth_sysex)
	MACRO_FCT_STATIC_SET(fluid_synth_pitch_bend)
	MACRO_FCT_STATIC_SET(fluid_synth_get_pitch_bend)
	MACRO_FCT_STATIC_SET(fluid_synth_pitch_wheel_sens)
	MACRO_FCT_STATIC_SET(fluid_synth_get_pitch_wheel_sens)
	MACRO_FCT_STATIC_SET(fluid_synth_program_change)
	MACRO_FCT_STATIC_SET(fluid_synth_channel_pressure)
	MACRO_FCT_STATIC_SET(fluid_synth_key_pressure)
	MACRO_FCT_STATIC_SET(fluid_synth_bank_select)
	MACRO_FCT_STATIC_SET(fluid_synth_sfont_select)
	MACRO_FCT_STATIC_SET(fluid_synth_program_select)
	MACRO_FCT_STATIC_SET(fluid_synth_program_select_by_sfont_name)
	MACRO_FCT_STATIC_SET(fluid_synth_get_program)
	MACRO_FCT_STATIC_SET(fluid_synth_unset_program)
	MACRO_FCT_STATIC_SET(fluid_synth_program_reset)
	MACRO_FCT_STATIC_SET(fluid_synth_system_reset)
	MACRO_FCT_STATIC_SET(fluid_synth_all_notes_off)
	MACRO_FCT_STATIC_SET(fluid_synth_all_sounds_off)
	MACRO_FCT_STATIC_SET(fluid_synth_set_gen)
	MACRO_FCT_STATIC_SET(fluid_synth_get_gen)
	MACRO_FCT_STATIC_SET(delete_fluid_audio_driver)
	MACRO_FCT_STATIC_SET(delete_fluid_synth)
	MACRO_FCT_STATIC_SET(delete_fluid_settings)

//
fluid_settings_t		*CFluidSynth::m_DefaultSettings					= NULL;
fluid_synth_t			*CFluidSynth::m_DefaultSynthetizer				= NULL;

fluid_settings_t		*CFluidSynth::m_settings [FluildMaxSynth]		= { NULL };
fluid_synth_t			*CFluidSynth::m_synthetizer	[FluildMaxSynth]	= { NULL };
fluid_audio_driver_t	*CFluidSynth::m_audioDriver [FluildMaxSynth]	= { NULL };
int						CFluidSynth::m_synthIndex [FluildMaxSynth]		= { 0 };
char					*CFluidSynth::m_synthSF2Files [FluildMaxSynth]	= { NULL };

//
boolean					CFluidSynth::m_bAllLoaded						= false;
boolean					CFluidSynth::m_bDllLoaded						= false;
boolean					CFluidSynth::m_bClosing 						= false;
boolean					CFluidSynth::m_bPlaying 						= false;

//
//////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////
CFluidSynth::CFluidSynth(void)
{
    criticalSectionStatic.create();
}


//
//////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////
CFluidSynth::~CFluidSynth(void)
{
	for ( int i = 0; i < FluildMaxSynth; i++ )
	{
		if ( m_synthSF2Files [ i ] != NULL )
		{
			free ( m_synthSF2Files [ i ] );
			m_synthSF2Files [ i ] = NULL;
		}
	}

	if ( m_DefaultSettings != NULL )
	{
		free ( m_DefaultSettings );
		m_DefaultSettings = NULL;
	}

	if ( m_DefaultSynthetizer != NULL )
	{
		free ( m_DefaultSynthetizer );
		m_DefaultSynthetizer = NULL;
	}
}

//
//////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////
int CFluidSynth::InitDll(CMidiFile *pMidiFile)
{
    criticalSectionStatic.create();

    //
	//	Alreay initialized
	if ( m_bDllLoaded )
	{
		return 0;
	}

    // Load Library
	HMODULE hModule = LoadLibrary ( "libfluidsynth-3.dll" );
	if ( hModule == NULL )
	{
		pMidiFile->SetMidiErrorText ( "libfluidsynth-3.dll not found" );
		return -1;
	}

	//	Get Entry Points
	MACRO_FCT_GET(new_fluid_settings)
	MACRO_FCT_GET(fluid_settings_setint)
	MACRO_FCT_GET(fluid_settings_setnum)
	MACRO_FCT_GET(fluid_settings_setstr)
	MACRO_FCT_GET(new_fluid_synth)
	MACRO_FCT_GET(fluid_synth_sfload)
	MACRO_FCT_GET(new_fluid_audio_driver)
	MACRO_FCT_GET(fluid_synth_noteon)
	MACRO_FCT_GET(fluid_synth_noteoff)
	MACRO_FCT_GET(fluid_synth_cc)
	MACRO_FCT_GET(fluid_synth_get_cc)
	MACRO_FCT_GET(fluid_synth_sysex)
	MACRO_FCT_GET(fluid_synth_pitch_bend)
	MACRO_FCT_GET(fluid_synth_get_pitch_bend)
	MACRO_FCT_GET(fluid_synth_pitch_wheel_sens)
	MACRO_FCT_GET(fluid_synth_get_pitch_wheel_sens)
	MACRO_FCT_GET(fluid_synth_program_change)
	MACRO_FCT_GET(fluid_synth_channel_pressure)
	MACRO_FCT_GET(fluid_synth_key_pressure)
	MACRO_FCT_GET(fluid_synth_bank_select)
	MACRO_FCT_GET(fluid_synth_sfont_select)
	MACRO_FCT_GET(fluid_synth_program_select)
	MACRO_FCT_GET(fluid_synth_program_select_by_sfont_name)
	MACRO_FCT_GET(fluid_synth_get_program)
	MACRO_FCT_GET(fluid_synth_unset_program)
	MACRO_FCT_GET(fluid_synth_program_reset)
	MACRO_FCT_GET(fluid_synth_system_reset)
	MACRO_FCT_GET(fluid_synth_all_notes_off)
	MACRO_FCT_GET(fluid_synth_all_sounds_off)
	MACRO_FCT_GET(fluid_synth_set_gen)
	MACRO_FCT_GET(fluid_synth_get_gen)
	MACRO_FCT_GET(delete_fluid_audio_driver)
	MACRO_FCT_GET(delete_fluid_synth)
	MACRO_FCT_GET(delete_fluid_settings)

	for ( int i = 0; i < FluildMaxSynth; i++ )
	{
		m_settings [ i ]		= NULL;
		m_synthetizer [ i ]		= NULL;
		m_audioDriver [ i ]		= NULL;
		m_synthIndex [ i ]		= -1;
		m_synthSF2Files [ i ]	= NULL;
	}

	m_DefaultSettings			= NULL;
	m_DefaultSynthetizer		= NULL;

	m_bAllLoaded		= true;
	m_bDllLoaded		= true;

	return 0;

error :
	if ( pMidiFile != NULL )
	{
		pMidiFile->SetMidiErrorText ( "Some Entry Point not found in libfluidsynth-3.dll" );
	}
	return -1;
}

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
boolean CFluidSynth::IsOK(CMidiFile *pMidiFile)
{
	if ( m_bAllLoaded && m_bDllLoaded )
	{
		return true;
	}

	if ( pMidiFile != NULL )
	{
		pMidiFile->SetMidiErrorText ( "FluidSynth not initialized" );
	}

	return false;
}

//
///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
int CFluidSynth::GetSynthIndex ( int iTrack )
{
	if ( iTrack >= 0 && iTrack < FluildMaxSynth )
	{
		int index = m_synthIndex [ iTrack ];
		if ( index >= 0 && index < FluildMaxSynth )
		{
			return index;
		}
	}

	return 0;
}

//
///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
int CFluidSynth::FindPathname ( const char *pathname )
{
	for ( int i = 0; i < FluildMaxSynth; i++ )
	{
		if ( m_synthSF2Files [ i ] != NULL )
		{
			if ( strcmp (  m_synthSF2Files [ i ], pathname ) == 0 )
			{
				return i;
			}
		}
	}
	return -1;
}

//
////////////////////////////////////////////////////////////////////////////////////
//	https://www.fluidsynth.org/api/group__midi__messages.html
//	https://github.com/FluidSynth/fluidsynth/wiki/FluidFeatures#midi-control-change-implementation-chart
////////////////////////////////////////////////////////////////////////////////////
WORD CFluidSynth::midiOutOpen ( const char *sf2name, CMidiFile *pMidiFile, int iTrack )
{
    CriticalSectionClass critSect;
    critSect.Lock();

	if ( ! m_bDllLoaded )
	{
		InitDll(pMidiFile);
	}

	if ( pMidiFile == NULL )
	{
		return MMSYSERR_NOMEM;
	}

	if ( ! m_bDllLoaded )
	{
		pMidiFile->SetMidiErrorText("Not Initialized");
		return MMSYSERR_NOMEM;
	}

	//
	char szPathname [ MAX_PATH ];
	ZeroMemory(szPathname, sizeof(szPathname));
	strcpy_s ( szPathname, sizeof(szPathname), theApp.m_SoundFontFile );
	RemoveFileName ( szPathname );
	if ( ! EndsWith ( szPathname, '\\' ) )
	{
		strcat_s ( szPathname, sizeof(szPathname), "\\" );
	}
	strcat_s ( szPathname, sizeof(szPathname), FindFileName(sf2name) );

	//	Check if file exist
	if ( ! CheckPathExist ( szPathname ) )
	{
		sprintf_s ( szFluidMessage, sizeof(szFluidMessage), "File %s does not exist", szPathname );
		pMidiFile->SetMidiErrorText(szFluidMessage);

		//	If Not replace it by default file
		ZeroMemory(szPathname, sizeof(szPathname));
		strcpy_s ( szPathname, sizeof(szPathname), theApp.m_SoundFontFile );
	}

	//	Already there and opened
	int index = FindPathname ( szPathname );
	if ( index != -1 )
	{
		m_synthIndex [ iTrack ] = index;
		return MMSYSERR_NOERROR;
	}

	//
	if ( m_DefaultSettings == NULL )
	{
		m_DefaultSettings = MACRO_FCT_CALL(new_fluid_settings)();
	}

	//
	if ( m_DefaultSynthetizer == NULL )
	{
		m_DefaultSynthetizer = MACRO_FCT_CALL(new_fluid_synth)(m_DefaultSettings);
	}

	//
	if ( iTrack >= 0 && iTrack < FluildMaxSynth && m_synthIndex [ iTrack ] == -1 )
	{
		WORD result = 0;

		//	Say An Index is used
		m_synthIndex [ iTrack ] = iTrack;
		m_synthSF2Files [ iTrack ]	= ( char * ) malloc ( strlen(szPathname) + 1 );
		strcpy_s ( m_synthSF2Files [ iTrack ], strlen(szPathname) + 1, szPathname );

		//
		m_settings [ iTrack ] = MACRO_FCT_CALL(new_fluid_settings)();
		if ( m_settings [ iTrack ] == NULL)
		{
			pMidiFile->SetMidiErrorText("new_fluid_settings fails");
			return MMSYSERR_NOMEM;
		}

		//
		//	https://www.fluidsynth.org/api/settings [ currentSynth ]_synth.html
		if ( strlen(theApp.m_synth_midi_bank_select) > 0 )
		{
			result = MACRO_FCT_CALL(fluid_settings_setstr)(m_settings [ iTrack ], "synth.midi-bank-select", theApp.m_synth_midi_bank_select );
		}

		if ( atoi(theApp.m_synth_gain) > 0 )
		{
			result = MACRO_FCT_CALL(fluid_settings_setnum)(m_settings [ iTrack ], "synth.gain", atof(theApp.m_synth_gain) );
		}

		//
		m_synthetizer [ iTrack ] = MACRO_FCT_CALL(new_fluid_synth)(m_settings [ iTrack ]);
		if( m_synthetizer [ iTrack ] == NULL )
		{
			pMidiFile->SetMidiErrorText("new_fluid_synth fails");
			return MMSYSERR_NOMEM;
		}
	
		int sfont_id = MACRO_FCT_CALL(fluid_synth_sfload)(m_synthetizer [ iTrack ],  m_synthSF2Files [ iTrack ], 1);
		if ( sfont_id == FLUID_FAILED )
		{
			pMidiFile->SetMidiErrorText("fluid_synth_sfload fails with soundfont");
			sfont_id = MACRO_FCT_CALL(fluid_synth_sfload)(m_DefaultSynthetizer, theApp.m_SoundFontFile, 1);
			if ( sfont_id == FLUID_FAILED )
			{
				pMidiFile->SetMidiErrorText("fluid_synth_sfload fails with default soundfont");
				return MMSYSERR_NOMEM;
			}
		}

		/* Create the audio driver. The synthesizer starts playing as soon
		   as the driver is created. */
		m_audioDriver [ iTrack ] = (*fct_new_fluid_audio_driver)(m_settings [ iTrack ], m_synthetizer [ iTrack ]);
		if ( m_audioDriver [ iTrack ] == NULL)
		{
			pMidiFile->SetMidiErrorText("fct_new_fluid_audio_driver fails");
			return MMSYSERR_NOMEM;
		}
	}

	sprintf_s ( szFluidMessage, sizeof(szFluidMessage), "Using %s", szPathname );
	pMidiFile->SetMidiErrorText(szFluidMessage);

	return MMSYSERR_NOERROR;
}

//
///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
WORD CFluidSynth::midiOutReset ( CMidiFile *pMidiFile, int iTrack )
{
    CriticalSectionClass critSect;
    critSect.Lock();

    if ( ! m_bDllLoaded )
	{
		if ( pMidiFile != NULL )
		{
			pMidiFile->SetMidiErrorText("Not Initialized");
		}
		return MMSYSERR_NOERROR;
	}

	if ( iTrack >= 0 && iTrack < FluildMaxSynth && m_synthIndex [ iTrack ] != -1 && m_synthetizer [ iTrack ] != NULL)
	{
		WORD result = 0;

		result = MACRO_FCT_CALL(fluid_synth_system_reset)(m_synthetizer [ iTrack ] );
	}

	return MMSYSERR_NOERROR;
}

//
///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
WORD CFluidSynth::midiOutResetAll ( CMidiFile *pMidiFile )
{
    for ( int i = 0; i < FluildMaxSynth; i++ )
	{
		midiOutReset ( pMidiFile, i );
	}

	return MMSYSERR_NOERROR;
}

//
///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
WORD CFluidSynth::midiOutClose ( CMidiFile *pMidiFile, int iTrack )
{
    CriticalSectionClass critSect;
    critSect.Lock();

    if ( ! m_bDllLoaded )
	{
		if ( pMidiFile != NULL )
		{
			pMidiFile->SetMidiErrorText("Not Initialized");
		}
		return MMSYSERR_NOERROR;
	}

    //
    m_bClosing  = true;

	WORD result = 0;
	if ( iTrack >= 0 && iTrack < FluildMaxSynth && m_synthIndex [ iTrack ] != -1 )
	{
		if ( m_audioDriver [ iTrack ] != NULL )
		{
			MACRO_FCT_CALL(delete_fluid_audio_driver)(m_audioDriver [ iTrack ]);
			m_audioDriver [ iTrack ] = NULL;
		}

		if ( m_synthetizer [ iTrack ] != NULL )
		{
			MACRO_FCT_CALL(delete_fluid_synth)(m_synthetizer [ iTrack ]);
			m_synthetizer [ iTrack ] = NULL;
		}

		if ( m_settings [ iTrack ] != NULL )
		{
			MACRO_FCT_CALL(delete_fluid_settings)(m_settings [ iTrack ]);
			m_settings [ iTrack ] = NULL;
		}

		//	Index is free
		m_synthIndex [ iTrack ] = -1;
		if ( m_synthSF2Files [ iTrack ] != NULL )
		{
			free ( m_synthSF2Files [ iTrack ] );
			m_synthSF2Files [ iTrack ] = NULL;
		}
	}

    m_bClosing  = false;
	return MMSYSERR_NOERROR;
}

//
///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
WORD CFluidSynth::midiOutCloseAll ( CMidiFile *pMidiFile )
{
	for ( int i = 0; i < FluildMaxSynth; i++ )
	{
		midiOutClose ( pMidiFile, i );
	}

	return MMSYSERR_NOERROR;
}

//
///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
WORD CFluidSynth::midiOutLongMsg ( LPMIDIHDR lpMidiOutHdr, UINT cbMidiOutHdr, int iTrack )
{
    CriticalSectionClass critSect;
    critSect.Lock();

    if ( ! m_bDllLoaded )
	{
		return MMSYSERR_NOERROR;
	}

	//
	int index = GetSynthIndex ( iTrack );
    if ( index == -1 )
    {
        return MMSYSERR_NOERROR;
    }

    if ( m_synthetizer [ index ] == NULL )
    {
        return MMSYSERR_NOERROR;
    }

    //
    m_bPlaying  = true;

	//
	WORD result = 0;

	static char		response [ 256 ];
	static int		responseLen = sizeof(response);
	static int		handled = 0;

	size_t first	= 0;
	size_t size	= lpMidiOutHdr->dwBufferLength;
	for ( size_t i = 0; i < lpMidiOutHdr->dwBufferLength; i++ )
	{
		unsigned char cByte = (unsigned char)lpMidiOutHdr->lpData [ i ];

		if ( cByte == MIDI_EVENT_CODE_SYSEX )
		{
			first = i + 1;
			size--;
		}

		if ( cByte == 0xf7 )
		{
			// size = i;
			break;
		}
	}

	int dryrun = 0;
	result = MACRO_FCT_CALL(fluid_synth_sysex)( 
			m_synthetizer [ index ],
			lpMidiOutHdr->lpData + first, (int) size,
			response, &responseLen, &handled, dryrun );

    m_bPlaying  = false;

	return MMSYSERR_NOERROR;
}

//
///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
WORD CFluidSynth::midiOutShortMsg( DWORD dwMsg, int iTrack )
{
    CriticalSectionClass critSect;
    critSect.Lock();

    if ( ! m_bDllLoaded )
	{
		return MMSYSERR_NOERROR;
	}

    //
    WORD result		= 0;

	//
	int index = GetSynthIndex ( iTrack );
    if ( index == -1 )
    {
        return MMSYSERR_NOERROR;
    }

    if ( m_synthetizer [ index ] == NULL )
    {
        return MMSYSERR_NOERROR;
    }

    if ( m_bClosing )
    {
        return MMSYSERR_NOERROR;
    }

    //
    m_bPlaying  = true;

    //
	int event		= dwMsg & OPERATION_MASK;
	int channel		= dwMsg & CHANNEL_MASK;
	int val1		= ( ( dwMsg & 0xff00 ) >> 8 );
	int val2		= ( ( dwMsg & 0xff0000 ) >> 16 );

	switch ( event )
	{
		//	Note Off
		case MIDI_EVENT_CODE_NOTEOFF :
		{
			result = MACRO_FCT_CALL(fluid_synth_noteoff)(m_synthetizer [ index ], channel, val1 );
			break;
		}

		//	Not On
		case MIDI_EVENT_CODE_NOTEON :
		{
			result = MACRO_FCT_CALL(fluid_synth_noteon)(m_synthetizer [ index ], channel, val1, val2);
			break;
		}

		//	Key Pressure
		case MIDI_EVENT_CODE_KEYPRES :
		{
			result = MACRO_FCT_CALL(fluid_synth_key_pressure)(m_synthetizer [ index ], channel, val1, val2 );
			break;
		}

		//	Parameters
		case MIDI_EVENT_CODE_PARAM :
		{
			switch ( val1 )
			{
				case 0x01 :		//	Bank select low
				case 0x20 :		//	Bank Select high
				default :
				{
					result = MACRO_FCT_CALL(fluid_synth_cc)(m_synthetizer [ index ], channel, val1, val2 );
					break;
				}
			}
			break;
		}

		//	Program
		case MIDI_EVENT_CODE_PROGRAM :
		{
			result = MACRO_FCT_CALL(fluid_synth_program_change)(m_synthetizer [ index ], channel, val1 );
			break; 
		}

		//	Channel Pressure
		case MIDI_EVENT_CODE_CHANPRESS :
		{
			result = MACRO_FCT_CALL(fluid_synth_channel_pressure)(m_synthetizer [ index ], channel, val1 );
			break;
		}

		//	Wheel
		case MIDI_EVENT_CODE_WHEEL :
		{
			result = MACRO_FCT_CALL(fluid_synth_pitch_wheel_sens)(m_synthetizer [ index ], channel, val1 );
			break;
		}
	}

    //
    m_bPlaying  = false;

    //
	return MMSYSERR_NOERROR;
}
