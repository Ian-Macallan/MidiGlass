#pragma once

#include <fluidsynth.h>

#include <mmsystem.h>

//	End Point Type
typedef fluid_settings_t *		(*FCT_new_fluid_settings)(void);
typedef int						(*FCT_fluid_settings_setint)(fluid_settings_t *settings, const char *name, int val );
typedef int						(*FCT_fluid_settings_setnum)(fluid_settings_t *settings, const char *name, double  val );
typedef int						(*FCT_fluid_settings_setstr)(fluid_settings_t *settings, const char *name, const char *str );

typedef fluid_synth_t *			(*FCT_new_fluid_synth )(fluid_settings_t *settings);
typedef int						(*FCT_fluid_synth_sfload)(fluid_synth_t *synth, const char *filename, int reset_presets);
typedef fluid_audio_driver_t *	(*FCT_new_fluid_audio_driver)(fluid_settings_t *settings, fluid_synth_t *synth);
typedef int						(*FCT_fluid_synth_noteon)(fluid_synth_t *synth, int chan, int key, int vel);
typedef int						(*FCT_fluid_synth_noteoff)(fluid_synth_t *synth, int chan, int key);

typedef void					(*FCT_delete_fluid_audio_driver)(fluid_audio_driver_t *driver);
typedef void					(*FCT_delete_fluid_synth)(fluid_synth_t *synth);
typedef void					(*FCT_delete_fluid_settings)(fluid_settings_t *settings);

typedef int						(*FCT_fluid_synth_cc)(fluid_synth_t *synth, int chan, int ctrl, int val);
typedef int						(*FCT_fluid_synth_get_cc)(fluid_synth_t *synth, int chan, int ctrl, int *pval);
typedef int						(*FCT_fluid_synth_sysex)(fluid_synth_t *synth, const char *data, int len, char *response, int *response_len, int *handled, int dryrun);
typedef int						(*FCT_fluid_synth_pitch_bend)(fluid_synth_t *synth, int chan, int val);
typedef int						(*FCT_fluid_synth_get_pitch_bend)(fluid_synth_t *synth, int chan, int *ppitch_bend);
typedef int						(*FCT_fluid_synth_pitch_wheel_sens)(fluid_synth_t *synth, int chan, int val);
typedef int						(*FCT_fluid_synth_get_pitch_wheel_sens)(fluid_synth_t *synth, int chan, int *pval);
typedef int						(*FCT_fluid_synth_program_change)(fluid_synth_t *synth, int chan, int program);
typedef int						(*FCT_fluid_synth_channel_pressure)(fluid_synth_t *synth, int chan, int val);
typedef int						(*FCT_fluid_synth_key_pressure)(fluid_synth_t *synth, int chan, int key, int val);
typedef int						(*FCT_fluid_synth_bank_select)(fluid_synth_t *synth, int chan, int bank);
typedef int						(*FCT_fluid_synth_sfont_select)(fluid_synth_t *synth, int chan, int sfont_id);
typedef int						(*FCT_fluid_synth_program_select)(fluid_synth_t *synth, int chan, int sfont_id, int bank_num, int preset_num);
typedef int						(*FCT_fluid_synth_program_select_by_sfont_name)(fluid_synth_t *synth, int chan, const char *sfont_name, int bank_num, int preset_num);
typedef int						(*FCT_fluid_synth_get_program)(fluid_synth_t *synth, int chan, int *sfont_id, int *bank_num, int *preset_num);
typedef int						(*FCT_fluid_synth_unset_program)(fluid_synth_t *synth, int chan);
typedef int						(*FCT_fluid_synth_program_reset)(fluid_synth_t *synth);
typedef int						(*FCT_fluid_synth_system_reset)(fluid_synth_t *synth);
typedef int						(*FCT_fluid_synth_all_notes_off)(fluid_synth_t *synth, int chan);
typedef int						(*FCT_fluid_synth_all_sounds_off)(fluid_synth_t *synth, int chan);
typedef int						(*FCT_fluid_synth_set_gen)(fluid_synth_t *synth, int chan, int param, float value);
typedef float					(*FCT_fluid_synth_get_gen)(fluid_synth_t *synth, int chan, int param);


//
//////////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////////
#define MACRO_FCT_CALL(name) (*fct_##name)

#define MACRO_FCT_GET(name) fct_##name=(FCT_##name)GetProcAddress(hModule, #name); if(fct_##name==NULL) goto error;

#define MACRO_FCT_STATIC_DEF(name) static FCT_##name	fct_##name;
#define MACRO_FCT_STATIC_SET(name) FCT_##name	CFluidSynth::fct_##name = NULL;

//	Max 256 tracks on midi files
const int				FluildMaxSynth = 256;

//
class CFluidSynth
{

	//	Static Function Pointers
	public:
		MACRO_FCT_STATIC_DEF(new_fluid_settings)
		MACRO_FCT_STATIC_DEF(fluid_settings_setint)
		MACRO_FCT_STATIC_DEF(fluid_settings_setnum)
		MACRO_FCT_STATIC_DEF(fluid_settings_setstr)

		MACRO_FCT_STATIC_DEF(new_fluid_synth)
		MACRO_FCT_STATIC_DEF(fluid_synth_sfload)
		MACRO_FCT_STATIC_DEF(new_fluid_audio_driver)
		MACRO_FCT_STATIC_DEF(fluid_synth_noteon)
		MACRO_FCT_STATIC_DEF(fluid_synth_noteoff)
		MACRO_FCT_STATIC_DEF(fluid_synth_cc)
		MACRO_FCT_STATIC_DEF(fluid_synth_get_cc)
		MACRO_FCT_STATIC_DEF(fluid_synth_sysex)
		MACRO_FCT_STATIC_DEF(fluid_synth_pitch_bend)
		MACRO_FCT_STATIC_DEF(fluid_synth_get_pitch_bend)
		MACRO_FCT_STATIC_DEF(fluid_synth_pitch_wheel_sens)
		MACRO_FCT_STATIC_DEF(fluid_synth_get_pitch_wheel_sens)
		MACRO_FCT_STATIC_DEF(fluid_synth_program_change)
		MACRO_FCT_STATIC_DEF(fluid_synth_channel_pressure)
		MACRO_FCT_STATIC_DEF(fluid_synth_key_pressure)
		MACRO_FCT_STATIC_DEF(fluid_synth_bank_select)
		MACRO_FCT_STATIC_DEF(fluid_synth_sfont_select)
		MACRO_FCT_STATIC_DEF(fluid_synth_program_select)
		MACRO_FCT_STATIC_DEF(fluid_synth_program_select_by_sfont_name)
		MACRO_FCT_STATIC_DEF(fluid_synth_get_program)
		MACRO_FCT_STATIC_DEF(fluid_synth_unset_program)
		MACRO_FCT_STATIC_DEF(fluid_synth_program_reset)
		MACRO_FCT_STATIC_DEF(fluid_synth_system_reset)
		MACRO_FCT_STATIC_DEF(fluid_synth_all_notes_off)
		MACRO_FCT_STATIC_DEF(fluid_synth_all_sounds_off)
		MACRO_FCT_STATIC_DEF(fluid_synth_set_gen)
		MACRO_FCT_STATIC_DEF(fluid_synth_get_gen)
		MACRO_FCT_STATIC_DEF(delete_fluid_audio_driver)
		MACRO_FCT_STATIC_DEF(delete_fluid_synth)
		MACRO_FCT_STATIC_DEF(delete_fluid_settings)

	public:
		CFluidSynth(void);
		virtual ~CFluidSynth(void);

		static int InitDll(CMidiFile *pMidiFile);
		static boolean IsOK(CMidiFile *pMidiFile);

		static WORD midiOutOpen ( const char *sf2name, CMidiFile *pMidiFile, int iTrack = 0 );

		static WORD midiOutReset ( CMidiFile *pMidiFile, int iTrack = 0 );
		static WORD midiOutResetAll ( CMidiFile *pMidiFile );

		static WORD midiOutClose ( CMidiFile *pMidiFile, int iTrack = 0 );
		static WORD midiOutCloseAll ( CMidiFile *pMidiFile );

		static WORD midiOutLongMsg( LPMIDIHDR lpMidiOutHdr, UINT cbMidiOutHdr, int iTrack );
		static WORD midiOutShortMsg( DWORD dwMsg, int iTrack );

		static int GetSynthIndex ( int iTrack );
		static int FindPathname ( const char *pathname );

		static fluid_settings_t			*m_DefaultSettings;
		static fluid_settings_t			*m_settings [ FluildMaxSynth ];

		static fluid_synth_t			*m_DefaultSynthetizer;
		static fluid_synth_t			*m_synthetizer [ FluildMaxSynth ];

		static fluid_audio_driver_t		*m_audioDriver [ FluildMaxSynth ];
		static int						m_synthIndex [ FluildMaxSynth ];
		static char						*m_synthSF2Files [ FluildMaxSynth ];

		static boolean					m_bAllLoaded;
		static boolean					m_bDllLoaded;
		static boolean					m_bClosing;
		static boolean					m_bPlaying;
};

