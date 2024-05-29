// PlayerDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "PlayerDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <direct.h>
#include <io.h>

extern CMidiWorksApp	theApp;

//
///////////////////////////////////////////////////////////////////////////////////
// CPlayerDialog dialog
//
///////////////////////////////////////////////////////////////////////////////////
CPlayerDialog::CPlayerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPlayerDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlayerDialog)
	//}}AFX_DATA_INIT
	m_pChannels		= NULL;
	m_bFirstTime	= true;
	m_bInitializing	= true;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayerDialog)
	DDX_Control(pDX, IDC_BACK, m_Back);
	DDX_Control(pDX, IDC_FORW, m_Forw);
	DDX_Control(pDX, IDC_CHANNELS, m_Channels);
	DDX_Control(pDX, IDC_TIME, m_Time);
	DDX_Control(pDX, IDC_STATE, m_State);
	DDX_Control(pDX, IDC_SLIDER_SPEED, m_Slider_Speed);
	DDX_Control(pDX, IDC_SLIDER, m_Slider);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Control(pDX, IDC_MAXIMUM, m_Maximum);
	DDX_Control(pDX, IDC_TUNE_COMBO, m_Tune_Combo);
	DDX_Control(pDX, IDC_SPIN_NUMBER, m_Spin_Number);
	DDX_Control(pDX, IDC_NUMBER, m_Number);
	DDX_Control(pDX, IDC_STOP, m_Stop);
	DDX_Control(pDX, IDC_PLAY, m_Play);
	DDX_Control(pDX, IDC_PAUSE, m_Pause);
	DDX_Control(pDX, IDC_FORWARD, m_Forward);
	DDX_Control(pDX, IDC_BACKWARD, m_Backward);
	//}}AFX_DATA_MAP
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CPlayerDialog, CDialog)
	//{{AFX_MSG_MAP(CPlayerDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_BACKWARD, OnBackward)
	ON_BN_CLICKED(IDC_PAUSE, OnPause)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_BN_CLICKED(IDC_FORWARD, OnForward)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_NUMBER, OnChangeNumber)
	ON_CBN_SELCHANGE(IDC_TUNE_COMBO, OnSelchangeTuneCombo)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER, OnReleasedcaptureSlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_SPEED, OnReleasedcaptureSliderSpeed)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	ON_BN_CLICKED(IDC_FORW, OnForw)
	ON_WM_DROPFILES()
	ON_MESSAGE(MM_MCINOTIFY,OnMciNotify)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
///////////////////////////////////////////////////////////////////////////////////
// CPlayerDialog message handlers
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CPlayerDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	BOOL			bRes;

	m_bInitializing		= true;

	bRes				= FALSE;

	InitChannelList ();

	m_iPlayer = -1;
	if ( theApp.m_PlayDirThread != NULL )
	{
		m_iPlayer = PLAYER_PLAY_DIR;
	}
	else if ( theApp.m_PlayListThread != NULL )
	{
		m_iPlayer = PLAYER_PLAY_LIST;
	}

	switch ( m_iPlayer )
	{
		case PLAYER_PLAY_DIR :
		{
			bRes = InitPlayerDirectory ();
			break;
		}

		case PLAYER_PLAY_LIST :
		{
			bRes = InitPlayerList ();
			break;
		}
	}

	m_bInitializing		= false;

	m_bFirstTime		= false;

	return bRes;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::PlayTune()
{
	KillTimer ( PLAY_THREAD_TIMER );

	SetTimer ( PLAY_THREAD_TIMER, 500, NULL );

	switch ( m_iPlayer )
	{
		case PLAYER_PLAY_DIR :
		{
			PlayTuneDirectory ();
			break;
		}

		case PLAYER_PLAY_LIST :
		{
			PlayTuneList ();
			break;
		}
	}

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	KillTimer ( PLAY_THREAD_TIMER );

	// TODO: Add your message handler code here
	switch ( m_iPlayer )
	{
		case PLAYER_PLAY_DIR :
		{
			OnDestroyDirectory ();
			break;
		}

		case PLAYER_PLAY_LIST :
		{
			OnDestroyList ();
			break;
		}
	}

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
LRESULT CPlayerDialog::OnMciNotify(WPARAM wParam, LPARAM lParam)
{
	switch ( m_iPlayer )
	{
		case PLAYER_PLAY_DIR :
		{
			return OnMciNotifyDirectory ( wParam, lParam);
			break;
		}

		case PLAYER_PLAY_LIST :
		{
			return OnMciNotifyList ( wParam, lParam);
			break;
		}
	}

	return 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnPlay() 
{
	// TODO: Add your control notification handler code here
	switch ( m_iPlayer )
	{
		case PLAYER_PLAY_DIR :
		{
			OnPlayDirectory ();
			break;
		}

		case PLAYER_PLAY_LIST :
		{
			OnPlayList ();
			break;
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnBackward() 
{
	// TODO: Add your control notification handler code here
	switch ( m_iPlayer )
	{
		case PLAYER_PLAY_DIR :
		{
			OnBackwardDirectory ();
			break;
		}

		case PLAYER_PLAY_LIST :
		{
			OnBackwardList ();
			break;
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnPause() 
{
	// TODO: Add your control notification handler code here
	switch ( m_iPlayer )
	{
		case PLAYER_PLAY_DIR :
		{
			OnPauseDirectory ();
			break;
		}

		case PLAYER_PLAY_LIST :
		{
			OnPauseList ();
			break;
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnStop() 
{
	// TODO: Add your control notification handler code here
	switch ( m_iPlayer )
	{
		case PLAYER_PLAY_DIR :
		{
			OnStopDirectory ();
			break;
		}

		case PLAYER_PLAY_LIST :
		{
			OnStopList ();
			break;
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnForward() 
{
	// TODO: Add your control notification handler code here
	switch ( m_iPlayer )
	{
		case PLAYER_PLAY_DIR :
		{
			OnForwardDirectory ();
			break;
		}

		case PLAYER_PLAY_LIST :
		{
			OnForwardList ();
			break;
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
HBRUSH CPlayerDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	HBRUSH hBrush = FriendCtlColor(pDC, pWnd, nCtlColor);
	if ( hBrush != NULL )
	{
		return hBrush;
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
char * CPlayerDialog::GetShort(const char *pString)
{
	char		*pReturned;
	unsigned	iLen;

	iLen = (int) strlen ( pString );
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
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CPlayerDialog::InitPlayerDirectory()
{
	HICON hDlgIcon = theApp.LoadIcon ( IDR_MAINFRAME );
	SetIcon( hDlgIcon, FALSE );

	//		Say no tune at start
	m_iTune		= -1;

	//		Retrieve values from the registry to start playing
	m_pMidiFile		= NULL;

	//		Say No action is request at this thime
	m_iAction		= ACTION_NOACTION;

	CreateButtons ( );

	//		Now fills the table with the name of the file
	int		iX;
	for ( iX = 0; iX < MAX_PLAYFILES; iX++ )
	{
		m_pFilenames [ iX ] = NULL;
	}

	//		Now load files in the table.
	char		szDirectory [ MAX_PATHNAME_STRING ];
	char		szFilename [ MAX_PATHNAME_STRING ];

    /*
     *      Structure for find file.
     */
    HANDLE                	iStream;
    WIN32_FIND_DATA			tagFile;
	CMidiClass				clForFile;


	strcpy_s ( m_Directory, sizeof ( m_Directory ), theApp.m_Explorer_Pathname );

	strcpy_s ( szDirectory, sizeof ( szDirectory ), m_Directory );
	strcat_s ( szDirectory, sizeof ( szDirectory ), "*.*" );

	//		First Do the combo box
	m_Tune_Combo.ResetContent ( );

	//		For all files
    iStream = FindFirstFile ( szDirectory, &tagFile );
	m_iFilenames = 0;
    if ( iStream != INVALID_HANDLE_VALUE )
    {
		strcpy_s ( szFilename, sizeof ( szFilename ), m_Directory );
		strcat_s ( szFilename, sizeof ( szFilename ), tagFile.cFileName );

		if ( clForFile.IsMidiFile ( tagFile.cFileName ) )
		{
			m_pFilenames [ m_iFilenames ] =
				( char * ) malloc ( strlen ( szFilename ) + 1 );
			strcpy_s (	m_pFilenames [ m_iFilenames ],
						strlen ( szFilename ) + 1, szFilename );
			m_iFilenames++;

			//		Set the combo
			m_Tune_Combo.AddString ( szFilename );

		}

		while (	FindNextFile ( iStream, &tagFile ) != FALSE && 
				m_iFilenames < MAX_PLAYFILES )
		{
			strcpy_s ( szFilename, sizeof ( szFilename ), m_Directory );
			strcat_s ( szFilename, sizeof ( szFilename ), tagFile.cFileName );

			if ( clForFile.IsMidiFile ( tagFile.cFileName ) )
			{
				m_pFilenames [ m_iFilenames ] =
				( char * ) malloc ( strlen ( szFilename ) + 1 );
				strcpy_s (	m_pFilenames [ m_iFilenames ],
							strlen ( szFilename ) + 1, szFilename );
				m_iFilenames++;

				//		Set the combo
				m_Tune_Combo.AddString ( szFilename );
			}
		}

		FindClose ( iStream );
	}

	m_Spin_Number.SetRange ( 0, m_iFilenames - 1 );
	if ( m_iFilenames == 0 )
	{
		return FALSE;
	}

	//		now Start Playing Tune
	m_iAction	= ACTION_PLAY_NEXT;
	m_iTune		= 0;

	PlayTune ();

	return TRUE;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::PlayTuneDirectory()
{
	char		szPathname [ MAX_PATHNAME_STRING ];

	//		If the file is already there do nothing
	if ( m_pMidiFile != NULL )
	{
		return;
	}

	if ( m_iFilenames == 0 )
	{
		return;
	}

	if ( m_iTune >= m_iFilenames )
	{
		m_iTune = 0;
	}

	strcpy_s ( szPathname, sizeof ( szPathname ), m_pFilenames [ m_iTune ] );

	//		Set the path name in the dialog box
	m_Tune_Combo.SelectString ( 0, szPathname );

	//		Play the current tune
	m_Spin_Number.SetPos ( m_iTune );

	m_pMidiFile	= new CMidiFile ( );
	m_pMidiFile->ReadMidiFile ( szPathname );

	m_pMidiFile->LLSetSkippingMode( theApp.m_bSkip_To_Note == 1 );
	m_pMidiFile->SetMidiDevice ( theApp.m_iMidiDevice );
	m_pMidiFile->SetSoundFont ( theApp.m_SoundFontFile, theApp.m_bUse_Sound_Font );

	//		Set the dialog header text to the filename
	SetWindowText ( GetShort ( szPathname ) );

	//		Start playing with the Current Window
	m_pMidiFile->LLPlay ( m_hWnd );

	DisplayInstruments ( );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
LRESULT CPlayerDialog::OnMciNotifyDirectory(WPARAM wParam, LPARAM lParam)
{

	//		The notification here is when closing

	switch ( m_iAction )
	{
		case ACTION_PLAY_NEXT :
		{
			StopAndDelete ( false );

			m_iTune++;
			m_iAction = ACTION_PLAY_NEXT;
			PlayTune ( );
			break;
		}

		case ACTION_PLAY :
		{
			StopAndDelete ( false );

			m_iAction = ACTION_PLAY_NEXT;
			PlayTune ( );
			break;
		}

		case ACTION_FORWARD :
		case ACTION_BACKWARD :
		{
			StopAndDelete ( false );

			m_iAction = ACTION_PLAY_NEXT;
			PlayTune ( );
			break;
		}

		case ACTION_PAUSE :
		{
			break;
		}

		case ACTION_STOP :
		default :
		{
			StopAndDelete ( false );

			m_iAction = ACTION_STOP;
			break;
		}
	}

	//		Return
	return 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnPlayDirectory()
{
	//		Say a Backward action is required
	m_iAction = ACTION_PLAY;

	//		Stop and Delete the midifile
	if ( m_pMidiFile != NULL )
	{
		StopAndDelete ( false );
	}
	else
	{
		PlayTune ();
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnDestroyDirectory()
{
	//		Say a Stop action is required
	m_iAction		= ACTION_STOP;
	m_iFilenames	= 0;

	StopAndDelete ( false );

	int		iX;
	for ( iX = 0; iX < MAX_PLAYFILES; iX++ )
	{
		if ( m_pFilenames [ iX ] != NULL )
		{
			free ( m_pFilenames [ iX ] );
			m_pFilenames [ iX ] = NULL;
		}
	}

	theApp.m_PlayDirThread = NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnBackwardDirectory()
{
	//		Increment the Tune to Play
	if ( m_iTune > 0 )
	{
		m_iTune--;
	}
	else
	{
		if ( m_iFilenames > 0 )
		{
			m_iTune = m_iFilenames - 1;
		}
		else
		{
			m_iTune = 0;
		}
	}

	//		Say a Backward action is required
	m_iAction = ACTION_BACKWARD;

	//		Stop and Delete the midifile
	if ( m_pMidiFile != NULL )
	{
		StopAndDelete ( true );
	}
	else
	{
		PlayTune ();
	}
	

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnPauseDirectory()
{
	//		Say a Pause action is required
	//		Pause or Resume Only
	if ( m_pMidiFile != NULL )
	{
		if ( m_pMidiFile->LLIsPlaying ( ) )
		{
			m_iAction = ACTION_PAUSE;

			m_pMidiFile->LLPause ( );

			return;
		}

		if ( m_pMidiFile->LLIsPaused ( ) )
		{
			m_iAction = ACTION_PLAY_NEXT;

			m_pMidiFile->LLResume ( );

			return;
		}
	}

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnStopDirectory()
{
	//		Say a Stop action is required
	m_iAction = ACTION_STOP;

	//		Stop and Delete the midifile
	StopAndDelete ( true );
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnForwardDirectory()
{
	//		Increment the Tune to Play
	m_iTune++;
	if ( m_iTune >= m_iFilenames )
	{
		m_iTune = 0;
	}

	//		Say a Forward action is required
	m_iAction = ACTION_BACKWARD;
	
	//		Stop and Delete the midifile
	if ( m_pMidiFile != NULL )
	{
		StopAndDelete ( true );
	}
	else
	{
		PlayTune ();
	}
	

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CPlayerDialog::InitPlayerList()
{
	HICON hDlgIcon = theApp.LoadIcon ( IDR_MAINFRAME );
	SetIcon( hDlgIcon, FALSE );

	//		Retrieve values from the registry to start playing
	m_pMidiFile		= NULL;

	//		Say No action is request at this thime
	m_iAction		= ACTION_NOACTION;

	m_Tune_Combo.ResetContent ( );

	char		szString [ MAX_TUNE_STRING ];
	CString		strReturned;

	m_iTune = 0;
	do
	{
		sprintf_s ( szString, sizeof ( szString ), "Tune %03d", m_iTune );
		strReturned =  theApp.GetProfileString( "Play List", szString, NULL );
		if ( strReturned != "" )
		{
			m_Tune_Combo.AddString ( strReturned );
			m_iTune++;
		}
	}
	while ( strReturned != "" );

	m_Spin_Number.SetRange ( 0, m_iTune - 1 );
	if ( m_iTune == 0 )
	{
		return FALSE;
	}

	//		Get the tune to start with
	m_iTune = theApp.GetProfileInt( "Play List", "Tune Number", 0 );

	CreateButtons ( );

	//		now Start Playing Tune
	m_iAction = ACTION_PLAY_NEXT;

	PlayTune ();

	return TRUE;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::PlayTuneList()
{
	char		szPathname [ MAX_PATHNAME_STRING ];
	char		szString [ MAX_TUNE_STRING ];
	CString		strReturned;

	//		If the file is already there do nothing
	if ( m_pMidiFile != NULL )
	{
		return;
	}

	//		Play the current tune
	sprintf_s ( szString, sizeof ( szString ), "Tune %03d", m_iTune );
	strReturned =  theApp.GetProfileString( "Play List", szString, NULL );

	if ( strReturned == "" )
	{
		m_iTune = 0;
		sprintf_s ( szString, sizeof ( szString ), "Tune %03d", m_iTune );
		strReturned =  theApp.GetProfileString( "Play List", szString, NULL );
	}

	if ( strReturned != "" )
	{
		strcpy_s ( szPathname, sizeof ( szPathname ), strReturned );

		m_pMidiFile	= new CMidiFile ( );
		m_pMidiFile->ReadMidiFile ( szPathname );

		m_pMidiFile->LLSetSkippingMode ( theApp.m_bSkip_To_Note == 1 );
		m_pMidiFile->SetMidiDevice ( theApp.m_iMidiDevice );
		m_pMidiFile->SetSoundFont ( theApp.m_SoundFontFile, theApp.m_bUse_Sound_Font );

		//		Set the path name in the dialog box
		m_Tune_Combo.SelectString ( 0, szPathname );
		m_Spin_Number.SetPos ( m_iTune );

		//		Set the dialog header text to the filename
		SetWindowText ( GetShort ( szPathname ) );

		//		Start playing with the Current Window
		m_pMidiFile->LLPlay ( m_hWnd );

		DisplayInstruments ( );

	}
	else
	{
		OnCancel ();
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
LRESULT CPlayerDialog::OnMciNotifyList(WPARAM wParam, LPARAM lParam)
{

	//		The notification here is when closing
	switch ( m_iAction )
	{
		case ACTION_PLAY_NEXT :
		{
			StopAndDelete ( false );
			m_iTune++;
			m_iAction = ACTION_PLAY_NEXT;
			PlayTune ( );
			break;
		}

		case ACTION_PLAY :
		{
			StopAndDelete ( false );
			m_iAction = ACTION_PLAY_NEXT;
			PlayTune ( );
			break;
		}

		case ACTION_FORWARD :
		case ACTION_BACKWARD :
		{
			StopAndDelete ( false );
			m_iAction = ACTION_PLAY_NEXT;
			PlayTune ( );
			break;
		}

		case ACTION_PAUSE :
		{
			break;
		}

		case ACTION_STOP :
		default :
		{
			StopAndDelete ( false );
			m_iAction = ACTION_STOP;
			break;
		}
	}

	//		Return
	return 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnPlayList()
{
	//		Say a Play
	m_iAction = ACTION_PLAY;

	//		Stop and Delete the midifile
	if ( m_pMidiFile != NULL )
	{
		StopAndDelete ( false );
	}
	else
	{
		PlayTune ();
	}

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnDestroyList()
{
	//		Say a Stop action is required

	//		Say a Stop action is required
	m_iAction = ACTION_STOP;

	StopAndDelete ( false );

	theApp.m_PlayListThread = NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnBackwardList()
{
	//		Increment the Tune to Play
	if ( m_iTune > 0 )
	{
		m_iTune--;
	}
	else
	{
		m_iTune = theApp.GetProfileInt( "Play List", "Tune Count", 0 );
		if ( m_iTune > 0 )
		{
			m_iTune--;
		}
	}

	//		Say a Backward action is required
	m_iAction = ACTION_BACKWARD;

	//		Stop and Delete the midifile
	if ( m_pMidiFile != NULL )
	{
		StopAndDelete ( true );
	}
	else
	{
		PlayTune ();
	}
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnPauseList()
{
	//		Say a Pause action is required
	//		Pause or Resume Only
	if ( m_pMidiFile != NULL )
	{
		if ( m_pMidiFile->LLIsPlaying ( ) )
		{
			m_iAction = ACTION_PAUSE;

			m_pMidiFile->LLPause ( );

			return;
		}

		if ( m_pMidiFile->LLIsPaused ( ) )
		{
			m_iAction = ACTION_PLAY_NEXT;

			m_pMidiFile->LLResume ( );

			return;
		}
	}

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnStopList()
{
	//		Say a Stop action is required
	m_iAction = ACTION_STOP;

	//		Stop and Delete the midifile
	StopAndDelete ( true );
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnForwardList( )
{
	//		Increment the Tune to Play
	m_iTune++;
	if ( m_iTune >= ( int ) theApp.GetProfileInt( "Play List", "Tune Count", 0 ) )
	{
		m_iTune = 0;
	}

	//		Say a Forward action is required
	m_iAction = ACTION_BACKWARD;
	
	//		Stop and Delete the midifile
	if ( m_pMidiFile != NULL )
	{
		StopAndDelete ( true );
	}
	else
	{
		PlayTune ();
	}
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	//		Here we will do some refresh
	DisplayMidiCounters();

	CDialog::OnTimer(nIDEvent);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::DisplayMidiCounters()
{
	unsigned long		iHours;
	unsigned long		iMinutes;
	unsigned long		iSeconds;
	unsigned long		iMillisecs;

	unsigned long		iTime;

	char				szString [ MAX_HOUR_STRING ];

	// check that one item gets the focus
	if ( m_pMidiFile != NULL )
	{
		m_Progress.SetRange32 ( 0, m_pMidiFile->GetMaximumHMSm () / 1000 );

		int		iMin, iMax;
		m_Slider.GetRange ( iMin, iMax );
		if (    iMin != 0 
			 || iMax != ( int) ( m_pMidiFile->GetMaximumHMSm () / 1000 ) )
		{
			m_Slider.SetRange ( 0, m_pMidiFile->GetMaximumHMSm () / 1000 );
			m_Slider.SetPos ( 0 );
		}

		m_Slider_Speed.GetRange ( iMin, iMax );
		if ( iMin != 0 || iMax != 100 )
		{
			m_Slider_Speed.SetRange ( 0, 100 );
			m_Slider_Speed.SetPos ( 0 );
		}

		//		Set the maximum
		m_Progress.SetRange32 ( 0, m_pMidiFile->GetMaximumHMSm () / 1000 );
		if ( m_pMidiFile->LLIsPlaying () )
		{
			m_State.SetWindowText ( "Playing" );
		}

		if ( m_pMidiFile->LLIsPaused () )
		{
			m_State.SetWindowText ( "Paused" );
		}

		if ( m_pMidiFile->LLIsStopped () )
		{
			m_State.SetWindowText ( "Stopped" );
		}

	}
	else
	{
		m_State.SetWindowText ( "Empty" );
	}

	//		Get the current time
	iTime = 0;
	if ( m_pMidiFile != NULL )
	{
		iTime = m_pMidiFile->LLGetTime ();
	}

	//		Set The progress bar
	m_Progress.SetPos ( iTime / 1000 );

	//		Display Time in Ascii
	if ( iTime != 0xffffffff )
	{
		iMillisecs	= iTime;
		iSeconds	= iMillisecs / 1000;
		iMinutes	= iSeconds / 60;
		iHours		= iMinutes / 60;

		iMillisecs	%= 1000;
		iSeconds	%= 60;
		iMinutes	%= 60;
	}
	else
	{
		iMillisecs	= 999;
		iSeconds	= 99;
		iMinutes	= 99;
		iHours		= 99;
	}

	sprintf_s ( szString, sizeof ( szString ),
				"%02d:%02d:%02d", iHours, iMinutes, iSeconds );

	m_Time.SetWindowText ( szString );

	//		The set the maximum
	iTime = 0xffffffff;
	if ( m_pMidiFile != NULL )
	{
		iTime = m_pMidiFile->GetMaximumHMSm ();
	}

	//		Display Time in Ascii
	if ( iTime != 0xffffffff )
	{
		iMillisecs	= iTime;
		iSeconds	= iMillisecs / 1000;
		iMinutes	= iSeconds / 60;
		iHours		= iMinutes / 60;

		iMillisecs	%= 1000;
		iSeconds	%= 60;
		iMinutes	%= 60;
	}
	else
	{
		iMillisecs	= 999;
		iSeconds	= 99;
		iMinutes	= 99;
		iHours		= 99;
	}

	sprintf_s ( szString, sizeof ( szString ),
				"%02d:%02d:%02d", iHours, iMinutes, iSeconds );

	m_Maximum.SetWindowText ( szString );

	//		Treat the channels
	CMidiChannel			*pMidichannel;
	int						iX;
	unsigned char			cNote;
	unsigned char			cVolume;
	int						iImage;

	for ( iX = 0; iX < MAX_CHANNELS; iX++ )
	{
		if ( m_pMidiFile != NULL )
		{
			pMidichannel = m_pMidiFile->GetChannelAddress ( iX );
			pMidichannel->GetCurrentNoteAndVolume ( &cNote, &cVolume );
			pMidichannel->ResetVolume ( );
			cVolume = cVolume & 0x7f;
			if ( cVolume == 0 )
			{
				iImage = 2;
			}
			else
			{
				iImage	= ( cVolume * 7 ) / 0x7f + 3;
			}

			m_Channels.SetItem ( iX, 0, LVIF_IMAGE, NULL, iImage, 0, 0, 0 );
		}
		else
		{
			m_Channels.SetItem ( iX, 0, LVIF_IMAGE, NULL, 2, 0, 0, 0 );
			m_Channels.SetItemText ( iX, 0, "" );
		}
	}
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnChangeNumber() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if ( ! m_bInitializing && m_Tune_Combo.m_hWnd != NULL && m_Spin_Number.m_hWnd != NULL )
	{
		m_iTune =  m_Spin_Number.GetPos ( );
		if ( m_iTune >= 0 && m_iTune != m_Tune_Combo.GetCurSel ( ) )
		{
			m_Tune_Combo.SetCurSel ( m_iTune );

			m_iAction = ACTION_PLAY;

			StopAndDelete ( true );
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnSelchangeTuneCombo() 
{
	// TODO: Add your control notification handler code here
	if ( ! m_bInitializing && m_Tune_Combo.m_hWnd != NULL && m_Spin_Number.m_hWnd != NULL )
	{
		m_iTune = m_Tune_Combo.GetCurSel ( );
		if ( m_iTune >= 0 && m_iTune != m_Spin_Number.GetPos ( ) )
		{
			m_Spin_Number.SetPos ( m_iTune );

			m_iAction = ACTION_PLAY;

			StopAndDelete ( true );
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::InitChannelList()
{
	int			iChannel;

	//		The image list seems to be deleted with the
	//		thread so we go thru a copy of it
	CImageList	*imgListNew;

	//		Create the new image list with the desired one
	imgListNew = new CImageList;

	imgListNew->Create ( &theApp.m_imgImageListChannel16x14 );

	m_pChannels = &m_Channels;

	m_Channels.InsertColumn ( 0, "Chan", LVCFMT_LEFT, 128 );

	m_Channels.SetImageList ( imgListNew, LVSIL_SMALL );

	//		Empty the box
	m_Channels.DeleteAllItems ();

	for ( iChannel = 0; iChannel < MAX_CHANNELS; iChannel++ )
	{
		m_Channels.InsertItem ( iChannel, "", 0 );
		m_Channels.SetItemData ( iChannel, ( DWORD ) iChannel );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::DisplayInstruments()
{
	//		Treat the channels
	CMidiChannel			*pMidichannel;
	int						iX;
	unsigned char			cNote;
	unsigned char			cVolume;
	int						iImage;

	for ( iX = 0; iX < MAX_CHANNELS; iX++ )
	{
		if ( m_pMidiFile != NULL )
		{
			pMidichannel = m_pMidiFile->GetChannelAddress ( iX );
			pMidichannel->GetCurrentNoteAndVolume ( &cNote, &cVolume );
			pMidichannel->ResetVolume ( );
			cVolume = cVolume & 0x7f;
			if ( cVolume == 0 )
			{
				iImage = 2;
			}
			else
			{
				iImage	= ( cVolume * 7 ) / 0x7f + 3;
			}

			m_Channels.SetItem ( iX, 0, LVIF_IMAGE, NULL, iImage, 0, 0, 0 );
			m_Channels.SetItemText ( iX, 0, m_pMidiFile->GetChannelProgram ( iX ) );
		}
		else
		{
			m_Channels.SetItem ( iX, 0, LVIF_IMAGE, NULL, 2, 0, 0, 0 );
			m_Channels.SetItemText ( iX, 0, "" );
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnReleasedcaptureSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if ( m_pMidiFile != NULL )
	{
		m_pMidiFile->LLSlide ( m_Slider.GetPos () );
	}

	*pResult = 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnReleasedcaptureSliderSpeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if ( m_pMidiFile != NULL )
	{
		m_pMidiFile->LLSetSpeed ( m_Slider_Speed.GetPos () );
	}
	
	*pResult = 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnBack() 
{
	// TODO: Add your control notification handler code here
	if ( m_pMidiFile != NULL )
	{
		long iSeconds = ( m_pMidiFile->LLGetTime () / 1000 );
		iSeconds -= SLIDE_SECONDS;
		if ( iSeconds < 0 )
		{
			iSeconds = 0;
		}

		m_pMidiFile->LLSlide ( iSeconds );

	}
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnForw() 
{
	// TODO: Add your control notification handler code here
	if ( m_pMidiFile != NULL )
	{
		long iSeconds = ( m_pMidiFile->LLGetTime () / 1000 );
		m_pMidiFile->LLSlide ( iSeconds +  SLIDE_SECONDS );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::StopAndDelete( bool bNotify )
{
	if ( m_pMidiFile != NULL )
	{
		m_pMidiFile->LLStop ( );
		if ( bNotify )
		{
			m_pMidiFile->LLSendMCINotification ( 0, 0 );
		}
		delete m_pMidiFile;
		m_pMidiFile = NULL;
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	char			szFilename [ MAX_PATHNAME_STRING ];

	//		First get the count of files
	UINT		iX;

	UINT iCount = 
		DragQueryFile ( hDropInfo, 0xFFFFFFFF, 
						szFilename, sizeof ( szFilename ) );
	if ( iCount >= 0 && m_iPlayer == PLAYER_PLAY_DIR )
	{
		for ( iX = 0; iX < iCount; iX++ )
		{
			UINT iRes = 
				DragQueryFile ( hDropInfo, iX, szFilename, sizeof ( szFilename ) );
			m_pFilenames [ m_iFilenames + iX ] =
				( char * ) malloc ( strlen ( szFilename ) + 1 );
			strcpy_s (	m_pFilenames [ m_iFilenames + iX ], 
						strlen ( szFilename ) + 1, szFilename );

			//		Set the combo
			m_Tune_Combo.AddString ( szFilename );

		}

		m_iFilenames += iCount;

		m_Spin_Number.SetRange ( 0, m_iFilenames - 1 );
	}

	CDialog::OnDropFiles(hDropInfo);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::CreateButtons()
{
	m_pBitmapPlay = new CBitmap;
	m_pBitmapPlay->LoadBitmap ( IDB_PLAY );
	m_hBitmapPlay = HBITMAP ( *m_pBitmapPlay );
	m_Play.SetBitmap ( m_hBitmapPlay );
	ResizeButtons(m_pBitmapPlay, &m_Play);

	m_pBitmapBackward = new CBitmap;
	m_pBitmapBackward->LoadBitmap ( IDB_BACKWARD );
	m_hBitmapBackward = HBITMAP ( *m_pBitmapBackward );
	m_Backward.SetBitmap ( m_hBitmapBackward );
	ResizeButtons(m_pBitmapBackward, &m_Backward);

	m_pBitmapBack = new CBitmap;
	m_pBitmapBack->LoadBitmap ( IDB_BACK );
	m_hBitmapBack = HBITMAP ( *m_pBitmapBack );
	m_Back.SetBitmap ( m_hBitmapBack );
	ResizeButtons(m_pBitmapBack, &m_Back);

	m_pBitmapPause = new CBitmap;
	m_pBitmapPause->LoadBitmap ( IDB_PAUSE );
	m_hBitmapPause = HBITMAP ( *m_pBitmapPause );
	m_Pause.SetBitmap ( m_hBitmapPause );
	ResizeButtons(m_pBitmapPause, &m_Pause);

	m_pBitmapForward = new CBitmap;
	m_pBitmapForward->LoadBitmap ( IDB_FORWARD );
	m_hBitmapForward = HBITMAP ( *m_pBitmapForward );
	m_Forward.SetBitmap ( m_hBitmapForward );
	ResizeButtons(m_pBitmapForward, &m_Forward);

	m_pBitmapForw = new CBitmap;
	m_pBitmapForw->LoadBitmap ( IDB_FORW );
	m_hBitmapForw = HBITMAP ( *m_pBitmapForw );
	m_Forw.SetBitmap ( m_hBitmapForw );
	ResizeButtons(m_pBitmapForw, &m_Forw);

	m_pBitmapStop = new CBitmap;
	m_pBitmapStop->LoadBitmap ( IDB_STOP );
	m_hBitmapStop = HBITMAP ( *m_pBitmapStop );
	m_Stop.SetBitmap ( m_hBitmapStop );
	ResizeButtons(m_pBitmapStop, &m_Stop);

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayerDialog::ResizeButtons(CBitmap *pBitmap, CMWButton *pButton)
{
	BITMAP			tagBitmap;
	RECT			rect;
	pBitmap->GetBitmap ( & tagBitmap );
	pButton->GetWindowRect ( &rect );
	ScreenToClient ( &rect );
	rect.bottom	= rect.top + tagBitmap.bmHeight + 4;
	rect.right	= rect.left + tagBitmap.bmWidth + 4;
	pButton->MoveWindow ( &rect );

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CPlayerDialog::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	BOOL bRes = FriendEraseBkgnd(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

	return CDialog::OnEraseBkgnd ( pDC );
}
