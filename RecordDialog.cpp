// RecordDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "RecordDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMidiWorksApp	theApp;

#include "strstr.h"

//
////////////////////////////////////////////////////////////////////////////////////////
// CRecordDialog dialog
//
////////////////////////////////////////////////////////////////////////////////////////
CRecordDialog::CRecordDialog(CWnd* pParent /*=NULL*/) : CMWDialog(CRecordDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecordDialog)
	//}}AFX_DATA_INIT
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CRecordDialog::DoDataExchange(CDataExchange* pDX)
{
	CMWDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecordDialog)
	DDX_Control(pDX, IDC_TEXT, m_Text);
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_Progress_Bar);
	DDX_Control(pDX, IDC_WAVE_DIRECTORY, m_WaveDirectory);
	DDX_Control(pDX, IDC_MIDIFILE, m_dlgMidiFile);
	DDX_Control(pDX, IDOK, m_Ok);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	//}}AFX_DATA_MAP
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CRecordDialog, CMWDialog)
	//{{AFX_MSG_MAP(CRecordDialog)
	ON_WM_DESTROY()
	ON_MESSAGE(MM_MCINOTIFY,OnMciNotify)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
////////////////////////////////////////////////////////////////////////////////////////
// CRecordDialog message handlers
//
////////////////////////////////////////////////////////////////////////////////////////
void CRecordDialog::OnDestroy() 
{
	CMWDialog::OnDestroy();
	
	//
	setLoopBackDone();

	//
	Sleep ( 250 );

	//
	if ( theApp.m_bUse_Sound_Font )
	{
		m_MidiFile.LLStop();
	}
	else
	{
		m_MidiFile.MciStop();
	}

	m_bClosing = true;

	KillTimer ( RECORD_THREAD_TIMER );

	theApp.m_RecordThread = NULL;
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
BOOL CRecordDialog::OnInitDialog() 
{
	CMWDialog::OnInitDialog();
	
	// TODO

	//		Say we are not saving.
	m_bSaved		= false;
	m_bClosing		= false;

	//		Set the icon.
	HICON hDlgIcon = theApp.LoadIcon ( IDR_MAINFRAME );
	SetIcon( hDlgIcon, FALSE );

	//		Set some values.
	m_dlgMidiFile.SetWindowText ( theApp.m_MidiFileName );
	m_WaveDirectory.SetWindowText ( theApp.m_WaveDirectory );

	//
	m_MidiFile.ReadMidiFile ( theApp.m_MidiFileName );

	m_Progress_Bar.SetRange ( 0, 59 );
	m_Progress_Bar.SetPos ( 0 );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CRecordDialog::OnOK() 
{
	// TODO
    //  Stop Current Playing
	m_Text.SetWindowText ( "Starting the recording. An internal loopback capture is at work" );
	m_Ok.EnableWindow ( false );

	//
	CString waveName = theApp.m_WaveDirectory + "\\" + FindFileName(theApp.m_MidiFileName) + ".wav";
	SetLoopBackFilename ( waveName );

	//
	m_pAudioloopbackThread = AfxBeginThread ( RUNTIME_CLASS ( CAudioLoopbackThread ) );
	if ( m_pAudioloopbackThread == NULL )
	{
		m_Text.RedrawWindow ( NULL, NULL );
		m_bSaved = true;
		m_Cancel.SetWindowText ( "Exit" );
		m_Cancel.EnableWindow ( true );
	}
	else
	{
		SetTimer ( RECORD_THREAD_TIMER, 1000, NULL );
	}

	//
	if ( theApp.m_bUse_Sound_Font )
	{
		m_MidiFile.SetSoundFont ( theApp.m_SoundFontFile, theApp.m_bUse_Sound_Font );
		m_MidiFile.LLPlay(m_hWnd);
	}
	else
	{
		m_MidiFile.MciPlay(m_hWnd);
	}

}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CRecordDialog::OnCancel() 
{
	//
	setLoopBackDone();

	//
	Sleep ( 250 );

	//
	if ( ! m_bSaved )
	{
		m_bSaved = true;
		m_Text.SetWindowText ( "Saving the recording. This could take some time depending on the recording." );
		m_Text.RedrawWindow ( NULL, NULL );
		m_Cancel.EnableWindow ( false );
	}

	m_bClosing = true;

	//
	if ( theApp.m_bUse_Sound_Font )
	{
		m_MidiFile.LLStop();
	}
	else
	{
		m_MidiFile.MciStop();
	}

	//
	CMWDialog::OnCancel();
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
LRESULT CRecordDialog::OnMciNotify(WPARAM wParam, LPARAM lParam)
{
	//
	setLoopBackDone();

	//
	Sleep ( 250 );

	//
	if ( ! m_bClosing )
	{
		//
		if ( ! m_bSaved )
		{
			m_bSaved = true;
			m_Text.SetWindowText ( "Saving the recording. This could take some time depending on the recording." );
			m_Text.RedrawWindow ( NULL, NULL );
			m_Cancel.EnableWindow ( false );
		}

		m_bClosing = true;

		CMWDialog::OnCancel ( );
	}

	return 0;
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CRecordDialog::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO
	int iPos = m_Progress_Bar.GetPos ( );
	iPos++;
	if ( iPos >= 60 )
	{
		iPos = 0;
	}
	m_Progress_Bar.SetPos ( iPos );

	CMWDialog::OnTimer(nIDEvent);
}
