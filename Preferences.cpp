// Preferences.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "Preferences.h"
#include "CFluidSynth.h"
#include "MWColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMidiWorksApp theApp;

//
/////////////////////////////////////////////////////////////////////////////
// CPreferences dialog
//
//////////////////////////////////////////////////////////////////////////
CPreferences::CPreferences(CWnd* pParent /*=NULL*/) : CMWDialog(CPreferences::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreferences)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

//
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CPreferences::DoDataExchange(CDataExchange* pDX)
{
	CMWDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreferences)
	DDX_Control(pDX, IDC_IN_DEVICES, m_In_Device);
	DDX_Control(pDX, IDC_WAVE_DIRECTORY, m_WaveDirectory);
	DDX_Control(pDX, IDC_SPLITTER, m_Splitter);
	DDX_Control(pDX, IDC_LARGE_BAR, m_Large_Bar);
	DDX_Control(pDX, IDC_REBAR, m_ReBar);
	DDX_Control(pDX, IDC_PLAY_TUNE, m_Play_Tune);
	DDX_Control(pDX, IDC_DEVICES, m_Device);
	DDX_Control(pDX, IDC_SKIP_TO_NOTE, m_Skip_To_Note);
	DDX_Control(pDX, IDOK, m_Ok);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_START_PLAYING, m_Start_Playing);
	DDX_Control(pDX, IDC_SOUND_FONT_FILE, m_SoundFontFile);
	DDX_Control(pDX, ID_BROWSE, m_BrowseSoundFile);
	DDX_Control(pDX, IDC_USE_SF, m_Use_Sound_Font);
	//}}AFX_DATA_MAP
}


//
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CPreferences, CMWDialog)
	//{{AFX_MSG_MAP(CPreferences)
	ON_BN_CLICKED(IDC_SKIP_TO_NOTE, OnSkipToNote)
	ON_BN_CLICKED(ID_BROWSE, &CPreferences::OnBnClickedBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
//////////////////////////////////////////////////////////////////////////
// CPreferences message handlers
//
//////////////////////////////////////////////////////////////////////////
BOOL CPreferences::OnInitDialog() 
{
	CMWDialog::OnInitDialog();
	
	// TODO
	DisplayValues ( );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CPreferences::DisplayValues()
{
	theApp.m_iStart_Playing = theApp.GetProfileInt ( "Settings", "Start Playing", 0 );
	m_Start_Playing.SetCurSel ( theApp.m_iStart_Playing );

	theApp.m_bSkip_To_Note = theApp.GetProfileInt ( "Settings", "Skip To Note", 0 );
	m_Skip_To_Note.SetCheck ( theApp.m_bSkip_To_Note  );

	theApp.m_iMidiDevice = theApp.GetProfileInt ( "Settings", "Midi Device", 0 );

	theApp.m_iMidiInDevice = theApp.GetProfileInt ( "Settings", "Midi Input Device", 0 );

	theApp.m_bPlay_Tune_In_AboutBox	= theApp.GetProfileInt ( "Settings", "Play Tune In About Box", 1 );
	m_Play_Tune.SetCheck ( theApp.m_bPlay_Tune_In_AboutBox );

	theApp.m_bLargeBar	= theApp.GetProfileInt ( "Settings", "Large Bar", 1 );
	m_Large_Bar.SetCheck ( theApp.m_bLargeBar );

	theApp.m_bReBar	= theApp.GetProfileInt ( "Settings", "Re Bar", 1 );
	m_ReBar.SetCheck ( theApp.m_bReBar );

	theApp.m_iSplitterWindow	= theApp.GetProfileInt ( "Settings", "Split Window", 1 );
	m_Splitter.SetCheck ( theApp.m_iSplitterWindow );

	theApp.m_WaveDirectory = theApp.GetProfileString ( "Settings", "Wave Directory", "C:\\TEMP" );
	m_WaveDirectory.SetWindowText ( theApp.m_WaveDirectory );

	theApp.m_SoundFontFile = theApp.GetProfileString ( "Settings", "Sound Font File", "" );
	m_SoundFontFile.SetWindowText ( theApp.m_SoundFontFile );

	theApp.m_bUse_Sound_Font	= theApp.GetProfileInt ( "Settings", "Use Sound Font", 0 );
	m_Use_Sound_Font.SetCheck ( theApp.m_bUse_Sound_Font );

	if ( ! CFluidSynth::IsOK(NULL) )
	{
		m_Use_Sound_Font.SetCheck ( FALSE );
		m_SoundFontFile.EnableWindow ( FALSE );
		m_BrowseSoundFile.EnableWindow ( FALSE );
		m_Use_Sound_Font.EnableWindow ( FALSE );
	}

	//		Search a device
	unsigned		iX;
	UINT			wRes;
	MIDIOUTCAPS		midiOutCaps;
	MIDIINCAPS		midiInpCaps;
	char			szText [ MAX_LONG_STRING ];

	m_Device.ResetContent ();
	m_Device.AddString ( "No Output Device Selected" );

	for  ( iX = 0; iX < midiOutGetNumDevs(); iX++ )
	{
		wRes = midiOutGetDevCaps( iX, & midiOutCaps, sizeof ( midiOutCaps ) );
		if ( wRes == MMSYSERR_NOERROR )
		{
			strcpy_s ( szText, sizeof ( szText ), midiOutCaps.szPname );
			switch ( midiOutCaps.wTechnology )
			{
				case MOD_MIDIPORT :
				{
					strcat_s ( szText, sizeof ( szText ), " (MIDI hardware port)" );
					break;
				}
				case MOD_SYNTH :
				{
					strcat_s ( szText, sizeof ( szText ), " (Synthesizer)" );
					break;
				}
				case MOD_SQSYNTH :
				{
					strcat_s ( szText, sizeof ( szText ), " (Square Wave synthesizer)" );
					break;
				}
				case MOD_FMSYNTH :
				{
					strcat_s ( szText, sizeof ( szText ), " (FM synthesizer)" );
					break;
				}
				case MOD_MAPPER :
				{
					strcat_s ( szText, sizeof ( szText ), " (Microsoft MIDI mapper)" );
					break;
				}
				case MOD_WAVETABLE :
				{
					strcat_s ( szText, sizeof ( szText ), " (Wave Table Synthesizer)" );
					break;
				}
				case MOD_SWSYNTH :
				{
					strcat_s ( szText, sizeof ( szText ), " (Software Synthesizer)" );
					break;
				}
				default :
				{
					strcat_s ( szText, sizeof ( szText ), " (Unknown)" );
					break;
				}
			}

			m_Device.AddString ( szText );
		}

	}
 
	m_Device.SetCurSel ( theApp.m_iMidiDevice );


	m_In_Device.ResetContent ();
	m_In_Device.AddString ( "No Input Device Selected" );

	for  ( iX = 0; iX < midiInGetNumDevs(); iX++ )
	{
		wRes = midiInGetDevCaps( iX, & midiInpCaps, sizeof ( midiInpCaps ) );
		if ( wRes == MMSYSERR_NOERROR )
		{
			strcpy_s ( szText, sizeof ( szText ), midiInpCaps.szPname );

			m_In_Device.AddString ( szText );
		}

	}
 
	m_In_Device.SetCurSel ( theApp.m_iMidiInDevice );



}

//
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CPreferences::OnOK() 
{
	// TODO
	theApp.m_iStart_Playing = m_Start_Playing.GetCurSel ();
	theApp.m_iMidiDevice	= m_Device.GetCurSel ( );
	theApp.m_iMidiInDevice	= m_In_Device.GetCurSel ( );

	if ( m_Play_Tune.GetCheck () )
	{
		theApp.m_bPlay_Tune_In_AboutBox	= 1;
	}
	else
	{
		theApp.m_bPlay_Tune_In_AboutBox	= 0;
	}

	if ( m_Skip_To_Note.GetCheck ( ) )
	{
		theApp.m_bSkip_To_Note = 1;
	}
	else
	{
		theApp.m_bSkip_To_Note = 0;
	}

	if ( m_Large_Bar.GetCheck () )
	{
		theApp.m_bLargeBar	= 1;
	}
	else
	{
		theApp.m_bLargeBar	= 0;
	}
	
	if ( m_ReBar.GetCheck () )
	{
		theApp.m_bReBar	= 1;
	}
	else
	{
		theApp.m_bReBar	= 0;
	}

	if ( m_Use_Sound_Font.GetCheck ( ) )
	{
		theApp.m_bUse_Sound_Font = 1;
	}
	else
	{
		theApp.m_bUse_Sound_Font = 0;
	}

	if ( m_Splitter.GetCheck ( ) )
	{
		theApp.m_iSplitterWindow = 1;
	}
	else
	{
		theApp.m_iSplitterWindow = 0;
	}

	m_WaveDirectory.GetWindowText ( theApp.m_WaveDirectory );
	m_SoundFontFile.GetWindowText ( theApp.m_SoundFontFile );

	CMWDialog::OnOK();
}

//
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CPreferences::OnSkipToNote() 
{
	// TODO
}

//
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CPreferences::OnBnClickedBrowse()
{
	// TODO
	OPENFILENAME ofn		;			// common dialog box structure
	ZeroMemory ( &ofn, sizeof(ofn) );

	//
	char szFile[MAX_PATH];		// buffer for file name
	char szDir[MAX_PATH];		// buffer for file name
	strcpy_s ( szFile, sizeof(szFile), theApp.m_SoundFontFile );
	strcpy_s ( szDir, sizeof(szDir), theApp.m_SoundFontFile );

	for ( int i = (int) strlen ( szDir ) - 1; i >= 0; i-- )
	{
		if ( szDir [ i ] == '\\' )
		{
			szDir [ i ] = '\0';
			break;
		}
	}

	// Initialize OPENFILENAME
	ofn.lStructSize			= sizeof(ofn); 
	ofn.hwndOwner			= this->m_hWnd;
	ofn.lpstrFile			= szFile;

	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0]		= '\0';
	ofn.nMaxFile			= sizeof(szFile);
	ofn.lpstrFilter			= "All\0*.*\0Sound Files\0*.sf2\0";
	ofn.nFilterIndex		= 1;
	ofn.lpstrFileTitle		= NULL;
	ofn.nMaxFileTitle		= 0;
	ofn.lpstrInitialDir		= szDir;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 
	if (GetOpenFileName(&ofn)==TRUE) 
	{
		m_SoundFontFile.SetWindowText ( szFile );
	}
}

