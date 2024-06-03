// ModifyThreadDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "ModifyThreadDialog.h"
#include "WaitTreatment.h"

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
// CModifyThreadDialog dialog
//
///////////////////////////////////////////////////////////////////////////////////
CModifyThreadDialog::CModifyThreadDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CModifyThreadDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CModifyThreadDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CModifyThreadDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CModifyThreadDialog)
	DDX_Control(pDX, IDC_NUMBER, m_Number);
	DDX_Control(pDX, IDC_TUNE, m_Tune);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CModifyThreadDialog, CDialog)
	//{{AFX_MSG_MAP(CModifyThreadDialog)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_ERASEBKGND,OnEraseBkgnd)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
///////////////////////////////////////////////////////////////////////////////////
// CModifyThreadDialog message handlers
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CModifyThreadDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO
	HICON hDlgIcon = theApp.LoadIcon ( IDR_MAINFRAME );
	SetIcon( hDlgIcon, FALSE );

	//		Retrieve values from the registry to start playing
	m_pMidiFile		= NULL;

	//		Now fills the table with the name of the file
	int		iX;
	for ( iX = 0; iX < MAX_PLAYFILES; iX++ )
	{
		m_pFilenames [ iX ] = NULL;
	}
	m_iFilenames = 0;

	//		Now load files in the table.
	char		szDirectory [ MAX_PATHNAME_STRING ];
	char		szFilename [ MAX_PATHNAME_STRING ];

    /*
     *      Structure for find file.
     */
    HANDLE                	iStream;
    WIN32_FIND_DATA			tagFile;

	strcpy_s ( m_Directory, sizeof ( m_Directory ), theApp.m_Explorer_Pathname );

	strcpy_s ( szDirectory, sizeof ( szDirectory ), m_Directory );
	strcat_s ( szDirectory, sizeof ( szDirectory ), "*.mid" );

    /*
     *      Find first name.
     */
    iStream = FindFirstFile ( szDirectory, &tagFile );
    if ( iStream != INVALID_HANDLE_VALUE )
    {
		strcpy_s ( szFilename, sizeof ( szFilename ), m_Directory );
		strcat_s ( szFilename, sizeof ( szFilename ), tagFile.cFileName );

		m_pFilenames [ m_iFilenames ] =
			( char * ) malloc ( strlen ( szFilename ) + 1 );
		strcpy_s (	 m_pFilenames [ m_iFilenames ], strlen ( szFilename ) + 1, 
					szFilename );
		m_iFilenames++;

		while ( FindNextFile ( iStream, &tagFile ) != FALSE &&
				m_iFilenames < MAX_PLAYFILES )
		{
			strcpy_s ( szFilename, sizeof ( szFilename ), m_Directory );
			strcat_s ( szFilename, sizeof ( szFilename ), tagFile.cFileName );

			m_pFilenames [ m_iFilenames ] =
				( char * ) malloc ( strlen ( szFilename ) + 1 );
			strcpy_s (	m_pFilenames [ m_iFilenames ],
						strlen ( szFilename ) + 1, szFilename );
			m_iFilenames++;

		}

		FindClose ( iStream );
	}

	//		now Start Modifying Files
	m_iFileNumber		= 0;
	
	//		Start a Timer
	SetTimer ( CORRECT_THREAD_TIMER, 100, NULL );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CModifyThreadDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO

	//		Say a Stop action is required
	m_iFilenames	= 0;

	if ( m_pMidiFile != NULL )
	{
		delete m_pMidiFile;
		m_pMidiFile = NULL;
	}

	int		iX;
	for ( iX = 0; iX < MAX_PLAYFILES; iX++ )
	{
		if ( m_pFilenames [ iX ] != NULL )
		{
			free ( m_pFilenames [ iX ] );
			m_pFilenames [ iX ] = NULL;
		}
	}

	theApp.m_ModifyThread = NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
LRESULT CModifyThreadDialog::OnEraseBkgnd(WPARAM wParam, LPARAM lParam)
{
	HDC		hDC;
	CDC		cDC;
	
	hDC			= (HDC) wParam; 
	cDC.Attach ( hDC );

	CRect				rectFrame;

	GetClientRect(&rectFrame);
	cDC.FillRect ( &rectFrame, &theApp.m_brBackGround );

	return 1;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
HBRUSH CModifyThreadDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO
	hbr = ( HBRUSH ) theApp.m_brBackEmpty;
	pDC->SetBkMode ( TRANSPARENT );
	// TODO
	return hbr;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CModifyThreadDialog::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO
	KillTimer ( CORRECT_THREAD_TIMER );

	if ( m_iFilenames > 0 && m_iFileNumber < m_iFilenames )
	{
		char		szString [ MAX_TUNE_STRING ];

		//		Play the current tune
		sprintf_s ( szString, sizeof ( szString ), "%03d", m_iFileNumber );
		m_Number.SetWindowText ( szString );

		//		Set the path name in the dialog box
		m_Tune.SetWindowText ( m_pFilenames [ m_iFileNumber ] );

		CWaitTreatment::BeginWait();

		m_pMidiFile	= new CMidiFile ( );
		m_pMidiFile->ReadMidiFile ( m_pFilenames [ m_iFileNumber ] );

		m_pMidiFile->CorrectAll ( theApp.m_iSysEx, 
			theApp.m_bAddStartOfTrack, theApp.m_bAddEndOfTrack,
			theApp.m_bBankSelectLow, theApp.m_bBankSelectHigh,
			theApp.m_bFilter, theApp.m_bCorrectStartOfTrack	);

		m_pMidiFile->Save ( m_pFilenames [ m_iFileNumber ] );

		delete m_pMidiFile;
		m_pMidiFile = NULL;

		CWaitTreatment::EndWait();

		m_iFileNumber++;

		//		Start a Timer
		SetTimer ( CORRECT_THREAD_TIMER, 1000, NULL );
	}
	else
	{
		CDialog::OnCancel();
	}

	CDialog::OnTimer(nIDEvent);
}
