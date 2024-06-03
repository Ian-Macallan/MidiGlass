// MidiDoc.cpp : implementation of the CMidiDoc class
//

#include "stdafx.h"
#include "MidiGlassApp.h"

#include "MidiDoc.h"

#include "CFluidSynth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MainFrm.h"
#include "WaitTreatment.h"

extern	CMidiWorksApp theApp;

extern "C" int _chdir ( const char * );

//
/////////////////////////////////////////////////////////////////////////////////
// CMidiDoc
//
/////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMidiDoc, CDocument)

//
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMidiDoc, CDocument)
	//{{AFX_MSG_MAP(CMidiDoc)
	ON_COMMAND(ID_FILE_SEND_MAIL, OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, OnUpdateFileSendMail)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////////
// CMidiDoc construction/destruction
//
/////////////////////////////////////////////////////////////////////////////////
CMidiDoc::CMidiDoc()
{
	// TODO
	CString			strReturned;

	m_pMidiFile		= NULL;
	m_iMidiTrack	= 0;

	//		Read the registry to see if we need to start playing
	theApp.m_iStart_Playing = theApp.GetProfileInt ( "Settings", "Start Playing", 0 );
	theApp.m_bSkip_To_Note = theApp.GetProfileInt ( "Settings", "Skip To Note", 0 );
	theApp.m_iMidiDevice = theApp.GetProfileInt ( "Settings", "Midi Device", 0 );
	theApp.m_iMidiInDevice = theApp.GetProfileInt ( "Settings", "Midi Input Device", 0 );
	theApp.m_bPlay_Tune_In_AboutBox = theApp.GetProfileInt ( "Settings", "Play Tune In About Box", 1 );
	theApp.m_WaveDirectory = theApp.GetProfileString ( "Settings", "Wave Directory", "C:\\TEMP" );
	theApp.m_SoundFontFile = theApp.GetProfileString ( "Settings", "Sound Font File", "" );
	theApp.m_bUse_Sound_Font = theApp.GetProfileInt ( "Settings", "Use Sound Font", 0 );

	//
	theApp.m_synth_midi_bank_select		= theApp.GetProfileString ( "Settings", "synth.midi-bank-select", "gs" );
	theApp.m_synth_gain					= theApp.GetProfileString ( "Settings", "synth.gain", "0.4" );

	theApp.WriteProfileString ( "Settings", "synth.midi-bank-select", theApp.m_synth_midi_bank_select );
	theApp.WriteProfileString ( "Settings", "synth.gain", theApp.m_synth_gain );

	//
	if ( ! CFluidSynth::IsOK(NULL) )
	{
		theApp.m_bUse_Sound_Font = 0;
		theApp.WriteProfileInt ( "Settings", "Use Sound Font", theApp.m_bUse_Sound_Font );
	}
	strReturned =  theApp.GetProfileString( "Explorer", "Path", "" );
	strcpy_s ( theApp.m_Explorer_Pathname, 
				sizeof ( theApp.m_Explorer_Pathname ), strReturned );
}

//
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
CMidiDoc::~CMidiDoc()
{
}

//
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
BOOL CMidiDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO
	// (SDI documents will reuse this document)
	if ( m_pMidiFile != NULL )
	{
		delete m_pMidiFile;
		m_pMidiFile		= NULL;
		m_iMidiTrack	= 0;
	}

	return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////////
// CMidiDoc serialization
//
/////////////////////////////////////////////////////////////////////////////////
void CMidiDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO
	}
	else
	{
		// TODO
	}
}

//
/////////////////////////////////////////////////////////////////////////////////
// CMidiDoc diagnostics
//
/////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CMidiDoc::AssertValid() const
{
	CDocument::AssertValid();
}

//
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
void CMidiDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

//
/////////////////////////////////////////////////////////////////////////////////
// CMidiDoc commands
//
/////////////////////////////////////////////////////////////////////////////////
BOOL CMidiDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO
	CWaitTreatment::BeginWait();

	//
	if ( m_pMidiFile != NULL )
	{
		delete m_pMidiFile;
		m_pMidiFile		= NULL;
		m_iMidiTrack	= 0;
	}

	//		Record the Directory Path Name
	char		szDirectory [ MAX_PATHNAME_STRING ];
	bool		bFinished;
	int			iX;
	strcpy_s ( szDirectory,  sizeof ( szDirectory ), lpszPathName );

	bFinished = false;
	iX = (int) strlen ( szDirectory );
	while ( ! bFinished && iX > 0 )
	{
		iX--;
		if ( szDirectory [ iX ] == '\\' )
		{
			bFinished = true;
		}
		szDirectory [ iX ] = 0;
	}

	theApp.WriteProfileString ( "Settings", "DirectoryName", szDirectory );

	_chdir ( szDirectory );

	//		Convert name to long filename
	char		szPathname [ MAX_PATHNAME_STRING ];

	WIN32_FIND_DATA		tagFile;

	strcpy_s ( szPathname, sizeof ( szPathname ), lpszPathName );

	HANDLE hFile = FindFirstFile ( szPathname, &tagFile );
	if ( hFile != INVALID_HANDLE_VALUE )
	{
		strcpy_s ( szPathname, sizeof ( szPathname ), szDirectory );
		strcat_s ( szPathname, sizeof ( szPathname ), "\\" );
		strcat_s ( szPathname, sizeof ( szPathname ), tagFile.cFileName );
	}

	//		read the midi file
	m_pMidiFile	= new CMidiFile ( );
	m_pMidiFile->ReadMidiFile ( szPathname );

	//
	CWaitTreatment::EndWait();

	//
	//		Start Playing
	m_pMidiFile->SetMidiDevice ( theApp.m_iMidiDevice );
	m_pMidiFile->SetSoundFont ( theApp.m_SoundFontFile, theApp.m_bUse_Sound_Font );

	switch ( theApp.m_iStart_Playing )
	{
		case 1:
		{
			HINSTANCE			hInst;

			hInst = 
				ShellExecute ( NULL, "play", szPathname, "", "", SW_SHOWDEFAULT );
			break;
		}

		case 2:
		{
			m_pMidiFile->MciPlay ( theApp.m_pMainWnd->m_hWnd );
			break;
		}

		case 3:
		{
			if ( theApp.m_bSkip_To_Note )
			{
				m_pMidiFile->LLSetSkippingMode ( true );
			}
			else
			{
				m_pMidiFile->LLSetSkippingMode ( false );
			}

			m_pMidiFile->LLPlay ();
			break;
		}

		case 0 :
		default :
		{
			break;
		}
	}

	return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
void CMidiDoc::OnCloseDocument() 
{
	// TODO
	if ( m_pMidiFile != NULL )
	{
		delete m_pMidiFile;
		m_pMidiFile		= NULL;
		m_iMidiTrack	= 0;
	}
	
	CDocument::OnCloseDocument();
}

//
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
CMidiFile * CMidiDoc::GetDocumentMidiFile()
{
	return ( m_pMidiFile );
}

//
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
void CMidiDoc::SetEventMask(unsigned short iMask)
{
	m_iEventMask = iMask;
}

//
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
unsigned short CMidiDoc::GetEventMask()
{
	return ( m_iEventMask );
}

//
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
unsigned short CMidiDoc::GetCurrentTrack()
{
	return ( m_iMidiTrack );
}

//
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
void CMidiDoc::SetCurrentTrack(unsigned short iTrack)
{
	m_iMidiTrack = iTrack;
}

//
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
BOOL CMidiDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO
	if ( m_pMidiFile != NULL )
	{
		m_pMidiFile->Save ( ( char * ) lpszPathName );
		SetModifiedFlag ( FALSE );
	}

	return TRUE;
//	return CDocument::OnSaveDocument(lpszPathName);
}

//
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
void CMidiDoc::OnFileSendMail() 
{
	// TODO
	CDocument::OnFileSendMail ();
}

//
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
void CMidiDoc::OnUpdateFileSendMail(CCmdUI* pCmdUI) 
{
	// TODO
	CDocument::OnUpdateFileSendMail (pCmdUI);
	pCmdUI->Enable ( FALSE );
}

//
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
void CMidiDoc::SetTitle(LPCTSTR lpszTitle) 
{
	// TODO
	
	CDocument::SetTitle(lpszTitle);
}

//
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
void CMidiDoc::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU) 
{
	// TODO
	char		szPathname [ MAX_PATHNAME_STRING ];

	strcpy_s ( szPathname, sizeof ( szPathname ), lpszPathName );
	if ( m_pMidiFile != NULL )
	{
		if ( m_pMidiFile->GetMidiPathName ( ) != NULL )
		{
			strcpy_s ( szPathname, sizeof ( szPathname ), m_pMidiFile->GetMidiPathName ( ) );
			if ( m_pMidiFile->IsRmiFile ( szPathname ) )
			{
				strcat_s ( szPathname, sizeof ( szPathname ), ".mid" );
			}
		}
	}
	
	CDocument::SetPathName(szPathname, bAddToMRU);
}
