// MainFrm.cpp : implementation of the CMainFrame class
//

#include "StdAfx.h"
#include "MidiGlassApp.h"

#include "MainFrm.h"

#include "AutomaticVersionHeader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <Shellapi.h>
#pragma comment (lib, "Shell32.lib")


#include "MidiDoc.h"

#include "CorrectOptions.h"
#include "TracksView.h"
#include "GeneralView.h"
#include "MidiTextView.h"
#include "EventView.h"
#include "MidiOwnPlayer.h"
#include "ErrorView.h"
#include "Preferences.h"
#include "Parameters.h"
#include "PlayList.h"
#include "SysExDialog.h"
#include "MidiSysEx.h"
#include "FindEvent.h"
#include "RecordThread.h"
#include "WaitTreatment.h"

extern	CMidiWorksApp		theApp;
extern	CMidiSysEx			clSysExGSReset [ MAX_SYSEX_LINES ];
extern	CMidiSysEx			clSysExGMReset [ MAX_SYSEX_LINES ];
extern	CMidiSysEx			clSysExXGReset [ MAX_SYSEX_LINES ];

//		The current number of views
#define	MAX_VIEWS			12

extern "C" int _chdir ( const char * );

//
///////////////////////////////////////////////////////////////////////////////////
// CMainFrame
///////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_ERROR, OnUpdateIndicatorError)
	ON_WM_CREATE()
	ON_COMMAND(ID_CONTENT_EVENTS_TEXT, OnContentEventsText)
	ON_COMMAND(ID_CONTENT_HEADER, OnContentHeader)
	ON_COMMAND(ID_CONTENT_EVENTS_COPYRIGHT, OnContentEventsCopyright)
	ON_COMMAND(ID_CONTENT_EVENTS_CUEPOINT, OnContentEventsCuepoint)
	ON_COMMAND(ID_CONTENT_EVENTS_INSTRUMENTNAME, OnContentEventsInstrumentname)
	ON_COMMAND(ID_CONTENT_EVENTS_LYRICS, OnContentEventsLyrics)
	ON_COMMAND(ID_CONTENT_EVENTS_MARKERS, OnContentEventsMarkers)
	ON_COMMAND(ID_CONTENT_EVENTS_SEQUENCE, OnContentEventsSequence)
	ON_COMMAND(ID_CONTENT_EVENTS_SEQUENCER, OnContentEventsSequencer)
	ON_COMMAND(ID_CONTENT_EVENTS_TRACKNAME, OnContentEventsTrackname)
	ON_COMMAND(ID_CONTENT_TRACKS, OnContentTracks)
	ON_COMMAND(ID_FILE_TRACE, OnFileTrace)
	ON_COMMAND(ID_CORRECT_ENDOFTRACK, OnCorrectEndoftrack)
	ON_COMMAND(ID_CORRECT_STARTOFTRACK, OnCorrectStartoftrack)
	ON_COMMAND(ID_FILE_SEQUENCER_PLAY, OnFileSequencerPlay)
	ON_COMMAND(ID_FILE_SEQUENCER_STOP, OnFileSequencerStop)
	ON_COMMAND(ID_FILE_MEDIAPLAYER, OnFileMediaplayer)
	ON_COMMAND(ID_FILE_SEQUENCER_PLAYANDLOOP, OnFileSequencerPlayandloop)
	ON_COMMAND(ID_FILE_SEQUENCER_PLAYLL, OnFileSequencerPlayll)
	ON_COMMAND(ID_FILE_SEQUENCER_STOPLL, OnFileSequencerStopll)
	ON_COMMAND(ID_CONTENT_PLAYER, OnContentPlayer)
	ON_COMMAND(ID_FILE_SEQUENCER_PLAYANDLOOPLL, OnFileSequencerPlayandloopll)
	ON_COMMAND(ID_FILE_SEQUENCER_PAUSELL, OnFileSequencerPausell)
	ON_COMMAND(ID_FILE_SEQUENCER_BACKWARDLL, OnFileSequencerBackwardll)
	ON_COMMAND(ID_FILE_SEQUENCER_FORWARDLL, OnFileSequencerForwardll)
	ON_COMMAND(ID_FILE_SEQUENCER_RESUMELL, OnFileSequencerResumell)
	ON_COMMAND(ID_VIEW_MCITOOLBAR, OnViewMcitoolbar)
	ON_COMMAND(ID_VIEW_MIDITOOLBAR, OnViewMiditoolbar)
	ON_COMMAND(ID_CORRECT_FILTERTRACKS, OnCorrectFiltertracks)
	ON_COMMAND(ID_CORRECT_ADD_GMRESET, OnCorrectAddGmreset)
	ON_COMMAND(ID_CORRECT_ADD_GSRESET, OnCorrectAddGsreset)
	ON_COMMAND(ID_CORRECT_ALL, OnCorrectAll)
	ON_COMMAND(ID_CORRECT_OPTIONS, OnCorrectOptions)
	ON_COMMAND(ID_CONTENT_SYSTEMEXCLUSIVE, OnContentSystemexclusive)
	ON_COMMAND(ID_CONTENT_ERRORMESSAGES, OnContentErrormessages)
	ON_COMMAND(ID_FILE_PREFERENCES, OnFilePreferences)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_CONTENT_PARAMETERS, OnContentParameters)
	ON_COMMAND(ID_REMOVE_CONTROLLER_BANKSELECTHIGH, OnRemoveControllerBankselecthigh)
	ON_COMMAND(ID_REMOVE_CONTROLLER_BANKSELECTLOW, OnRemoveControllerBankselectlow)
	ON_COMMAND(ID_REMOVE_COPYRIGHT, OnRemoveCopyright)
	ON_COMMAND(ID_REMOVE_CUEPOINT, OnRemoveCuepoint)
	ON_COMMAND(ID_REMOVE_INSTRUMENTNAME, OnRemoveInstrumentname)
	ON_COMMAND(ID_REMOVE_LYRICS, OnRemoveLyrics)
	ON_COMMAND(ID_REMOVE_MARKERS, OnRemoveMarkers)
	ON_COMMAND(ID_REMOVE_SEQUENCE, OnRemoveSequence)
	ON_COMMAND(ID_REMOVE_SEQUENCER, OnRemoveSequencer)
	ON_COMMAND(ID_REMOVE_SYSTEMEXCLUSIVE, OnRemoveSystemexclusive)
	ON_COMMAND(ID_REMOVE_TEXT, OnRemoveText)
	ON_COMMAND(ID_REMOVE_TRACKNAME, OnRemoveTrackname)
	ON_COMMAND(ID_CONTENT_PLAYLIST, OnContentPlaylist)
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_ADDTOTHEPLAYLIST, OnFileAddtotheplaylist)
	ON_COMMAND(ID_VIEW_MAINTOOLBAR, OnViewMaintoolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MAINTOOLBAR, OnUpdateViewMaintoolbar)
	ON_COMMAND(ID_CORRECT_TIMESIGNATURE, OnCorrectTimesignature)
	ON_COMMAND(ID_CORRECT_KEYSIGNATURE, OnCorrectKeysignature)
	ON_COMMAND(ID_CORRECT_MOVE_ALL, OnCorrectMoveAll)
	ON_COMMAND(ID_TOOLS_TOOL1, OnToolsTool1)
	ON_COMMAND(ID_TOOLS_TOOL2, OnToolsTool2)
	ON_COMMAND(ID_TOOLS_TOOL3, OnToolsTool3)
	ON_COMMAND(ID_TOOLS_TOOL4, OnToolsTool4)
	ON_COMMAND(ID_TOOLS_TOOL5, OnToolsTool5)
	ON_COMMAND(ID_TOOLS_TOOL6, OnToolsTool6)
	ON_COMMAND(ID_TOOLS_TOOL7, OnToolsTool7)
	ON_COMMAND(ID_TOOLS_TOOL8, OnToolsTool8)
	ON_COMMAND(ID_TOOLS_EXPLORER, OnToolsExplorer)
	ON_COMMAND(ID_CONTENT_CHANNELS, OnContentChannels)
	ON_COMMAND(ID_VIEW_RIGHTVIEW, OnViewRightview)
	ON_COMMAND(ID_VIEW_LEFTVIEW, OnViewLeftview)
	ON_COMMAND(ID_VIEW_TWOVIEWS, OnViewTwoviews)
	ON_COMMAND(ID_REMOVE_RESETALLCONTROLERS, OnRemoveControllersResetallcontrolers)
	ON_COMMAND(ID_CONTENT_START_IE, OnContentStartIe)
	ON_COMMAND(ID_CONTENT_STOP_IE, OnContentStopIe)
	ON_COMMAND(ID_VIEW_IETOOLBAR, OnViewIetoolbar)
	ON_COMMAND(ID_VIEW_SF2TOOLBAR, OnViewSF2toolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SF2TOOLBAR, &CMainFrame::OnUpdateViewSF2Toolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_IETOOLBAR, OnUpdateViewIetoolbar)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_VIEW_STATUS_BAR, OnViewStatusBar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnUpdateViewStatusBar)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEQUENCER_PLAYLL, OnUpdateFileSequencerPlayll)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEQUENCER_STOPLL, OnUpdateFileSequencerStopll)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEQUENCER_FORWARDLL, OnUpdateFileSequencerForwardll)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEQUENCER_BACKWARDLL, OnUpdateFileSequencerBackwardll)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEQUENCER_PAUSELL, OnUpdateFileSequencerPausell)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEQUENCER_PLAYANDLOOPLL, OnUpdateFileSequencerPlayandloopll)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEQUENCER_PLAY, OnUpdateFileSequencerPlay)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEQUENCER_PLAYANDLOOP, OnUpdateFileSequencerPlayandloop)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEQUENCER_STOP, OnUpdateFileSequencerStop)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEQUENCER_RESUMELL, OnUpdateFileSequencerResumell)
	ON_UPDATE_COMMAND_UI(ID_CONTENT_STOP_IE, OnUpdateContentStopIe)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_TRACE, OnUpdateFileTrace)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, OnUpdateFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_ADDTOTHEPLAYLIST, OnUpdateFileAddtotheplaylist)
	ON_UPDATE_COMMAND_UI(ID_CORRECT_ADD_GMRESET, OnUpdateCorrectAddGmreset)
	ON_UPDATE_COMMAND_UI(ID_CORRECT_ADD_GSRESET, OnUpdateCorrectAddGsreset)
	ON_UPDATE_COMMAND_UI(ID_CORRECT_ALL, OnUpdateCorrectAll)
	ON_UPDATE_COMMAND_UI(ID_CORRECT_ENDOFTRACK, OnUpdateCorrectEndoftrack)
	ON_UPDATE_COMMAND_UI(ID_CORRECT_FILTERTRACKS, OnUpdateCorrectFiltertracks)
	ON_UPDATE_COMMAND_UI(ID_CORRECT_KEYSIGNATURE, OnUpdateCorrectKeysignature)
	ON_UPDATE_COMMAND_UI(ID_CORRECT_MOVE_ALL, OnUpdateCorrectMoveAll)
	ON_UPDATE_COMMAND_UI(ID_CORRECT_STARTOFTRACK, OnUpdateCorrectStartoftrack)
	ON_UPDATE_COMMAND_UI(ID_CORRECT_TIMESIGNATURE, OnUpdateCorrectTimesignature)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_CONTROLLER_BANKSELECTHIGH, OnUpdateRemoveControllerBankselecthigh)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_CONTROLLER_BANKSELECTLOW, OnUpdateRemoveControllerBankselectlow)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_COPYRIGHT, OnUpdateRemoveCopyright)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_CUEPOINT, OnUpdateRemoveCuepoint)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_INSTRUMENTNAME, OnUpdateRemoveInstrumentname)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_LYRICS, OnUpdateRemoveLyrics)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_MARKERS, OnUpdateRemoveMarkers)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_RESETALLCONTROLERS, OnUpdateRemoveResetallcontrolers)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_SEQUENCE, OnUpdateRemoveSequence)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_SEQUENCER, OnUpdateRemoveSequencer)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_SYSTEMEXCLUSIVE, OnUpdateRemoveSystemexclusive)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_TEXT, OnUpdateRemoveText)
	ON_UPDATE_COMMAND_UI(ID_REMOVE_TRACKNAME, OnUpdateRemoveTrackname)
	ON_COMMAND(ID_CONTENT_STAFF, OnContentStaff)
	ON_COMMAND(ID_TOOLS_TOOL10, OnToolsTool10)
	ON_COMMAND(ID_TOOLS_TOOL11, OnToolsTool11)
	ON_COMMAND(ID_TOOLS_TOOL12, OnToolsTool12)
	ON_COMMAND(ID_TOOLS_TOOL13, OnToolsTool13)
	ON_COMMAND(ID_TOOLS_TOOL14, OnToolsTool14)
	ON_COMMAND(ID_TOOLS_TOOL15, OnToolsTool15)
	ON_COMMAND(ID_TOOLS_TOOL16, OnToolsTool16)
	ON_COMMAND(ID_TOOLS_TOOL17, OnToolsTool17)
	ON_COMMAND(ID_TOOLS_TOOL18, OnToolsTool18)
	ON_COMMAND(ID_TOOLS_TOOL19, OnToolsTool19)
	ON_COMMAND(ID_TOOLS_TOOL20, OnToolsTool20)
	ON_COMMAND(ID_TOOLS_TOOL9, OnToolsTool9)
	ON_COMMAND(ID_CORRECT_TRUNCATE_NOTE, OnCorrectTruncateNote)
	ON_UPDATE_COMMAND_UI(ID_CORRECT_TRUNCATE_NOTE, OnUpdateCorrectTruncateNote)
	ON_COMMAND(ID_CONTENT_NOTES, OnContentNotes)
	ON_COMMAND(ID_CONTENT_TRACKS_LIST, OnContentTracksList)
	ON_UPDATE_COMMAND_UI(ID_CONTENT_TRACKS_LIST, OnUpdateContentTracksList)
	ON_COMMAND(ID_CONTENT_MIDIEXPLORER, OnContentMidiexplorer)
	ON_UPDATE_COMMAND_UI(ID_CONTENT_MIDIEXPLORER, OnUpdateContentMidiexplorer)
	ON_WM_CTLCOLOR()
	ON_UPDATE_COMMAND_UI(ID_CONTENT_CHANNELS, OnUpdateContentChannels)
	ON_UPDATE_COMMAND_UI(ID_CONTENT_HEADER, OnUpdateContentHeader)
	ON_UPDATE_COMMAND_UI(ID_CONTENT_NOTES, OnUpdateContentNotes)
	ON_UPDATE_COMMAND_UI(ID_CONTENT_PLAYER, OnUpdateContentPlayer)
	ON_UPDATE_COMMAND_UI(ID_CONTENT_PLAYLIST, OnUpdateContentPlaylist)
	ON_UPDATE_COMMAND_UI(ID_CONTENT_STAFF, OnUpdateContentStaff)
	ON_UPDATE_COMMAND_UI(ID_CONTENT_TRACKS, OnUpdateContentTracks)
	ON_UPDATE_COMMAND_UI(ID_CONTENT_ERRORMESSAGES, OnUpdateContentErrormessages)
	ON_COMMAND(ID_CORRECT_ADD_XGRESET, OnCorrectAddXgreset)
	ON_UPDATE_COMMAND_UI(ID_CORRECT_ADD_XGRESET, OnUpdateCorrectAddXgreset)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_COMMAND(ID_EDIT_FINDAGAIN, OnEditFindagain)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FINDAGAIN, OnUpdateEditFindagain)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)
	ON_COMMAND(ID_FILE_SAVEGEOMETRY, OnFileSavegeometry)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVEGEOMETRY, OnUpdateFileSavegeometry)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_CONTENT_EVENTS_SHOWHIDE, OnContentEventsShowhide)
	ON_UPDATE_COMMAND_UI(ID_CONTENT_EVENTS_SHOWHIDE, OnUpdateContentEventsShowhide)
	ON_MESSAGE(MM_MCINOTIFY,OnMciNotify)
	ON_UPDATE_COMMAND_UI ( ID_VIEW_MCITOOLBAR, OnUpdateViewMciToolBar )
	ON_UPDATE_COMMAND_UI ( ID_VIEW_MIDITOOLBAR, OnUpdateViewMidiToolBar )
	ON_COMMAND(ID_FILE_RECORD, OnFileRecord)
	ON_UPDATE_COMMAND_UI(ID_FILE_RECORD, OnUpdateFileRecord)
    ON_WM_INITMENU()
    ON_WM_INITMENUPOPUP()
    ON_WM_DRAWITEM()
    ON_WM_MEASUREITEM()
    END_MESSAGE_MAP()
	//}}AFX_MSG_MAP

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_ERROR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

//
///////////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
///////////////////////////////////////////////////////////////////////////////////
CMainFrame::CMainFrame()
{
	// TODO
	m_pMidiView		= NULL;
	m_bClosing		= false;

    m_pContextMenu  = NULL;
    m_pSysMenu      = NULL;

    //
	ResetViewHandles();

	//	Now read the registry for value for corrections
	theApp.m_bMainToolBar		= theApp.GetProfileInt ( "Settings", "Tool Bar", 1 );
	theApp.m_bMciToolBar		= theApp.GetProfileInt ( "Settings", "MCI Tool Bar", 0 );
	theApp.m_bMidiToolBar		= theApp.GetProfileInt ( "Settings", "Midi Tool Bar", 1 );
	theApp.m_bIeToolBar			= theApp.GetProfileInt ( "Settings", "IE Tool Bar", 1 );
	theApp.m_bSf2ToolBar		= theApp.GetProfileInt ( "Settings", "SF2 Tool Bar", 1 );
	theApp.m_bStatusBar			= theApp.GetProfileInt ( "Settings", "Status Bar", 1 );

	theApp.m_bLargeBar			= theApp.GetProfileInt ( "Settings", "Large Bar", 1 );
	theApp.m_bReBar				= theApp.GetProfileInt ( "Settings", "Re Bar", 1 );

	//	Now read the registry for value for corrections
	theApp.m_iSysEx					= theApp.GetProfileInt ( "Correction", "System Exclusive", 1 );
	theApp.m_bAddStartOfTrack		= theApp.GetProfileInt ( "Correction", "Add Start Of Track", 1 );
	theApp.m_bAddEndOfTrack			= theApp.GetProfileInt ( "Correction", "Add End Of Track", 1 );
	theApp.m_bBankSelectLow			= theApp.GetProfileInt ( "Correction", "Remove Bank Select Low", 1 );
	theApp.m_bBankSelectHigh		= theApp.GetProfileInt ( "Correction", "Remove Bank Select High", 1 );
	theApp.m_bFilter				= theApp.GetProfileInt ( "Correction", "Filter Tracks By Channel", 1 );
	theApp.m_bCorrectStartOfTrack	= theApp.GetProfileInt ( "Correction", "Correct Start Of Track", 1 );

	//	The Colors
	theApp.m_bOwnBackground		= theApp.GetProfileInt ( "Settings", "Background Color", 1 );
	theApp.m_bOwnControlColor	= theApp.GetProfileInt ( "Settings", "Control Color", 1 );

	theApp.m_iSplitterWindow	= theApp.GetProfileInt ( "Settings", "Split Window", 1 );
	theApp.m_bSplitterWindow	= ( theApp.m_iSplitterWindow != 0 );

	//	Read the system exclusives
	int		iX;

	for ( iX = 0; iX < MAX_SYSEX_LINES; iX++ )
	{
		clSysExGMReset [ iX ].Delete ();
		clSysExGSReset [ iX ].Delete ();
		clSysExXGReset [ iX ].Delete ();
	}

	char		szString [ MAX_LONG_STRING ];
	CString		strReturned;

	iX = 0;
	do
	{
		sprintf_s ( szString, sizeof ( szString ), "SysEx %03d", iX );
		strReturned =  theApp.GetProfileString( "GM Reset", szString, "" );
		if ( strReturned != "" )
		{
			clSysExGMReset [ iX ].Set ( strReturned );
		}
		iX++;
	}
	while ( strReturned != "" );

	iX = 0;
	do
	{
		sprintf_s ( szString, sizeof ( szString ), "SysEx %03d", iX );
		strReturned =  theApp.GetProfileString( "GS Reset", szString, "" );
		if ( strReturned != "" )
		{
			clSysExGSReset [ iX ].Set ( strReturned );
		}
		iX++;
	}
	while ( strReturned != "" );

	iX = 0;
	do
	{
		sprintf_s ( szString, sizeof ( szString ), "SysEx %03d", iX );
		strReturned =  theApp.GetProfileString( "XG Reset", szString, "" );
		if ( strReturned != "" )
		{
			clSysExXGReset [ iX ].Set ( strReturned );
		}
		iX++;
	}
	while ( strReturned != "" );

	//		Set default directory
	CString		Directory;
	char		szDirectory [ MAX_PATHNAME_STRING ];

	Directory = theApp.GetProfileString ( "Settings", "DirectoryName" );

	strcpy_s ( szDirectory, sizeof ( szDirectory ), Directory );
	_chdir ( szDirectory );

	//		Search for the .mid type
	char			szValue [ MAX_REGISTRY_STRING ];
	long			iLength;
	LONG			iLong; 
	iLength = sizeof ( szValue ) - 1;
	iLong = RegQueryValue( HKEY_CLASSES_ROOT, ".mid",  szValue, &iLength );
	if ( iLong == ERROR_SUCCESS )
	{
		char		szCreate [ MAX_REGISTRY_STRING ];
		char		szAction [ MAX_REGISTRY_STRING ];
		char		szFilename [ MAX_PATHNAME_STRING ];
		char		szPathname [ MAX_PATHNAME_STRING ];
		HKEY		hkResult;
		DWORD		dwType;

		strcpy_s ( szCreate, sizeof ( szCreate ), szValue );
#ifdef	_DEBUG
		strcat_s ( szCreate, sizeof ( szCreate ), "\\shell\\mwmodifydebug" );
#endif
#ifndef	_DEBUG
		strcat_s ( szCreate, sizeof ( szCreate ), "\\shell\\mwmodify" );
#endif
		iLong = RegCreateKey( HKEY_CLASSES_ROOT, szCreate,  &hkResult );

		dwType	= REG_SZ;
#ifdef	_DEBUG
		strcpy_s ( szAction, sizeof ( szAction ), "Update using MidiGlass (&Debug Version)" );
#endif
#ifndef	_DEBUG
		strcpy_s ( szAction, sizeof ( szAction ), "Update using Midi&Glass" );
#endif
		iLong = RegSetValue(	HKEY_CLASSES_ROOT, szCreate, dwType, 
								szAction, (DWORD)strlen ( szAction ) );

		strcpy_s ( szCreate, sizeof ( szCreate ), szValue );
#ifdef	_DEBUG
		strcat_s ( szCreate, sizeof ( szCreate ), "\\shell\\mwmodifydebug\\command" );
#endif
#ifndef	_DEBUG
		strcat_s ( szCreate, sizeof ( szCreate ), "\\shell\\mwmodify\\command" );
#endif
		iLong = RegCreateKey( HKEY_CLASSES_ROOT, szCreate,  &hkResult );

		dwType	= REG_SZ;

		DWORD dwResult = GetModuleFileName(
							NULL,
							szFilename,
							sizeof ( szFilename ) );
 
		strcpy_s ( szPathname, sizeof ( szPathname ), "\"" );
		strcat_s ( szPathname, sizeof ( szPathname ), szFilename );
		strcat_s ( szPathname, sizeof ( szPathname ), "\"" );
		strcat_s ( szPathname, sizeof ( szPathname ), " \"%1\"" );
		iLong = RegSetValue(	HKEY_CLASSES_ROOT, szCreate, dwType, 
								szPathname, (DWORD)strlen ( szPathname ) );

	}

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMainFrame::~CMainFrame()
{
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if ( CreateOwnToolBars ( ) == - 1 )
	{
		return -1;
	}

	if ( CreateOwnStatusBar ( ) == - 1 )
	{
		return -1;
	}

	//	Set Text and Size
	SetOwnToolBars ( );

	//	Size and place window and previously
	SizeAndPlaceWindow ();

	//	Set The Icon
	HICON hFrameIcon = theApp.LoadIcon ( IDI_MIDI1 );
	SetIcon( hFrameIcon, FALSE );

	//	Add the Tools menu
	AddToolsMenu ();

	//	Load Bar State
	LoadOwnToolBar ( );

	//	Display Bars now
	DisplayOwnToolBars ( );

    //
    CMenu *pMenu = GetMenu();
    m_Menu.SetApplicationMenu ( this, pMenu );

	return 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO
	//  the CREATESTRUCT cs

	return TRUE;
}

//
///////////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics
//
///////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

//
///////////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO

	if ( theApp.m_bSplitterWindow )
	{
		CreateSplitterWindows(lpcs, pContext);
	}
	else
	{
		BOOL bRes = CFrameWnd::OnCreateClient(lpcs, pContext);
		return bRes;
	}

	return true;

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentEventsText() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	if ( pMididoc == NULL )
	{
		return;
	}

	pMididoc->SetEventMask ( MASK_TEXT );

	OnContentEvents (RIGHT_VIEW_TEXT);

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentHeader() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	if ( pMididoc == NULL )
	{
		return;
	}

	if ( theApp.m_bSplitterWindow )
	{
		if ( ! CreateColViewFull ( 1, ( CView ** ) & m_RightGeneralView, 
							RUNTIME_CLASS(CGeneralView), RIGHT_VIEW_HEADER ) )
		{
			return;
		}

	}
	else
	{
		ActivateOrHideView ( m_RightGeneralView, "General Frame Geometry" );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentEvents( int iView )
{
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	if ( pMididoc == NULL )
	{
		return;
	}

	if ( theApp.m_bSplitterWindow )
	{
		if ( ! CreateColViewFull ( 1, ( CView ** ) & m_RightTextView, 
							RUNTIME_CLASS(CMidiTextView), iView ) )
		{
			return;
		}

	}
	else
	{
		SetActiveView ( m_RightTextView );
		//	ActivateOrHideView ( m_RightTextView );
	}
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentEventsCopyright() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	if ( pMididoc == NULL )
	{
		return;
	}

	pMididoc->SetEventMask ( MASK_COPYRIGHT );

	OnContentEvents (RIGHT_VIEW_COPYRIGHT);
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentEventsCuepoint() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	if ( pMididoc == NULL )
	{
		return;
	}

	pMididoc->SetEventMask ( MASK_CUEPOINT );

	OnContentEvents (RIGHT_VIEW_CUEPOINT);
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentEventsInstrumentname() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	if ( pMididoc == NULL )
	{
		return;
	}

	pMididoc->SetEventMask ( MASK_INSTRNAME );

	OnContentEvents (RIGHT_VIEW_INSTRNAME);
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentEventsLyrics() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	if ( pMididoc == NULL )
	{
		return;
	}

	pMididoc->SetEventMask ( MASK_LYRICS );

	OnContentEvents (RIGHT_VIEW_LYRICS);
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentEventsMarkers() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	if ( pMididoc == NULL )
	{
		return;
	}

	pMididoc->SetEventMask ( MASK_MARKERS );

	OnContentEvents (RIGHT_VIEW_MARKERS);
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentEventsSequence() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	if ( pMididoc == NULL )
	{
		return;
	}

	pMididoc->SetEventMask ( MASK_SEQUENCE );

	OnContentEvents (RIGHT_VIEW_SEQUENCE);

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentEventsSequencer() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	if ( pMididoc == NULL )
	{
		return;
	}

	pMididoc->SetEventMask ( MASK_SEQUENCER );

	OnContentEvents (RIGHT_VIEW_SEQUENCER);

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentEventsTrackname() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	if ( pMididoc == NULL )
	{
		return;
	}

	pMididoc->SetEventMask ( MASK_TRACKNAME );

	OnContentEvents (RIGHT_VIEW_TRACKNAME);
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentTracks() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();

	if ( pMididoc == NULL )
	{
		return;
	}

	if ( theApp.m_bSplitterWindow )
	{
		if ( ! CreateColViewFull ( 1, ( CView ** ) & m_RightEventView, 
							RUNTIME_CLASS(CEventView), RIGHT_VIEW_EVENTS ) )
		{
			return;
		}
	}
	else
	{
		ActivateOrHideView ( m_RightEventView, "Event Frame Geometry" );
	}

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnFileTrace() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );

	char				szTraceFilename [ MAX_PATHNAME_STRING ];

	static char BASED_CODE szFilter[] = 
		"Trace File|*.log|All Files (*.*)|*.*||";

	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	CFileDialog l_FileDialog ( FALSE, NULL, "*.log", NULL, szFilter );

	/*
	 *		Starts a dialog box.
	 */
	if ( l_FileDialog.DoModal () == IDOK)
	{
		strcpy_s ( szTraceFilename, sizeof ( szTraceFilename ),
					l_FileDialog.GetFileName () );
		pMidifile->Trace ( szTraceFilename );
	}	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMidiList * CMainFrame::GetCurrentMidiEvent()
{
	if ( m_RightEventView != NULL )
	{
		return m_RightEventView->GetCurrentMidiEvent ();
	}
	else
	{
		return NULL;
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::RefreshFrame()
{
	//		Just to see the new window
	RECT			RectParent;
	CWnd			*wndParentSplitter;

	if ( theApp.m_bSplitterWindow )
	{
		if ( m_LeftView != NULL )
		{
			wndParentSplitter = m_LeftView->GetParent ();
			wndParentSplitter->GetWindowRect ( &RectParent );
			RectParent.right--;
			wndParentSplitter->MoveWindow (&RectParent, TRUE);
			RectParent.right++;
			wndParentSplitter->MoveWindow (&RectParent, TRUE);
		}
		else
		{
			if ( m_RightView != NULL )
			{
				wndParentSplitter = m_RightView->GetParent ();
				wndParentSplitter->GetWindowRect ( &RectParent );
				RectParent.right--;
				wndParentSplitter->MoveWindow (&RectParent, TRUE);
				RectParent.right++;
				wndParentSplitter->MoveWindow (&RectParent, TRUE);
			}
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::GetDialogBoxSize(unsigned iD, LPRECT rect)
{
	DLGTEMPLATE		*pDlgTemplate;

	HRSRC hrsrc = FindResource(NULL, MAKEINTRESOURCE( iD ), RT_DIALOG); 
	if ( hrsrc == NULL )
	{
		return;
	}

	HGLOBAL hglb = LoadResource(theApp.m_hInstance, hrsrc); 
	if ( hglb != NULL )
	{
		pDlgTemplate = ( DLGTEMPLATE *) LockResource(hglb);
		if ( pDlgTemplate != NULL )
		{
			rect->top		= 0;
			rect->left		= 0;
			rect->right		= pDlgTemplate->cx;
			rect->bottom	= pDlgTemplate->cy;

			long	baseunit	= GetDialogBaseUnits();
			int		baseunitX	= baseunit & 0xffff;
			int		baseunitY	= ( baseunit >> 16 ) &0xffff;

			rect->left   = ( rect->left   * baseunitX ) / 4;
			rect->right  = ( rect->right  * baseunitX ) / 4;
			rect->top    = ( rect->top    * baseunitY ) / 8;
			rect->bottom = ( rect->bottom * baseunitY ) / 8;
		}
	}

	UnlockResource ( hglb );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnCorrectEndoftrack() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );

	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	CWaitTreatment::BeginWait();
	pMidifile->AddEndOfTrack ();
	pMidifile->Update();
	CWaitTreatment::EndWait();

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS  | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );;

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnCorrectStartoftrack() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );

	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	CWaitTreatment::BeginWait();
	pMidifile->AddStartOfTrack ();
	pMidifile->Update();
	CWaitTreatment::EndWait();

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS  | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );;

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
LRESULT CMainFrame::OnMciNotify(WPARAM wParam, LPARAM lParam)
{
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );

	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return 0;
	}

	if ( pMidifile->MciGetPlayValue () )
	{
		//		Stop the play
		pMidifile->MciStop ();

		//		Replay If Loop
		pMidifile->MciPlayAndLoop ( m_hWnd );
	}
	else
	{
		//		Stop the play
		pMidifile->MciStop ( );

	}

	return 0;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnFileSequencerPlay() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	pMidifile->MciPlayOnce ( m_hWnd );
	
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMainFrame::OnFileSequencerStop() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );

	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	//		Stop Playing
	pMidifile->MciStop ( );

}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMainFrame::OnFileMediaplayer() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	HINSTANCE			hInst;

	if ( pMididoc == NULL )
	{
		return;
	}

	hInst = 
		ShellExecute ( NULL, "play", 
						pMididoc->GetPathName( ), "", "", SW_SHOWDEFAULT );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMainFrame::OnFileSequencerPlayandloop() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );

	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	pMidifile->MciPlayAndLoop ( m_hWnd );
	
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMainFrame::OnFileSequencerPlayll() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		First Update the view
	//		OnContentPlayer ( );

	//		Then start the midi file playing
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	if ( theApp.m_bSkip_To_Note )
	{
		pMidifile->LLSetSkippingMode ( true );
	}
	else
	{
		pMidifile->LLSetSkippingMode ( false );
	}

	pMidifile->SetMidiDevice ( theApp.m_iMidiDevice );
	pMidifile->SetSoundFont ( theApp.m_SoundFontFile, theApp.m_bUse_Sound_Font );
	pMidifile->LLPlay ();

	pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_PLAYER | RIGHT_MASK_CHANNELS | RIGHT_MASK_ERRORS );
	if ( m_RightPlayerView != NULL )
	{
		m_RightPlayerView->DisplayValues ();
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMainFrame::OnFileSequencerStopll() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );

	//		Kill the timer
	KillTimer ( MIDI_TIMER );

	//		Stop Playing
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	pMidifile->LLStop ( );

	pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_PLAYER | RIGHT_MASK_CHANNELS | RIGHT_MASK_ERRORS );
	if ( m_RightPlayerView != NULL )
	{
		m_RightPlayerView->DisplayValues ();
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentPlayer() 
{
	// TODO
	if ( theApp.m_bSplitterWindow )
	{
		if ( ! CreateColViewFull ( 1, ( CView ** ) & m_RightPlayerView, 
							RUNTIME_CLASS(CMidiOwnPlayer), RIGHT_VIEW_PLAYER ) )
		{
			return;
		}
	}
	else
	{
		ActivateOrHideView ( m_RightPlayerView, "Player Frame Geometry" );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMainFrame::OnFileSequencerPlayandloopll() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Then start the midi file playing
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	if ( theApp.m_bSkip_To_Note )
	{
		pMidifile->LLSetSkippingMode ( true );
	}
	else
	{
		pMidifile->LLSetSkippingMode ( false );
	}

	pMidifile->SetMidiDevice ( theApp.m_iMidiDevice );
	pMidifile->SetSoundFont ( theApp.m_SoundFontFile, theApp.m_bUse_Sound_Font );
	pMidifile->LLPlayAndLoop ();

	pMidifile->SetSoundFont ( theApp.m_SoundFontFile, theApp.m_bUse_Sound_Font );

	pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_PLAYER | RIGHT_MASK_CHANNELS | RIGHT_MASK_ERRORS );
	if ( m_RightPlayerView != NULL )
	{
		m_RightPlayerView->DisplayValues ();
	}

}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMainFrame::OnFileSequencerPausell() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Then start the midi file playing
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	if ( pMidifile->LLIsPlaying () )
	{
		pMidifile->LLPause ();
	}
	else
	{
		pMidifile->LLResume ();
	}

	pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_PLAYER | RIGHT_MASK_CHANNELS | RIGHT_MASK_ERRORS );
	if ( m_RightPlayerView != NULL )
	{
		m_RightPlayerView->DisplayValues ();
	}

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnFileSequencerBackwardll() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Then start the midi file playing
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	long iSeconds = ( pMidifile->LLGetTime () / 1000 );
	iSeconds -= SLIDE_SECONDS;
	if ( iSeconds < 0 )
	{
		iSeconds = 0;
	}

	pMidifile->SetMidiDevice ( theApp.m_iMidiDevice );
	pMidifile->SetSoundFont ( theApp.m_SoundFontFile, theApp.m_bUse_Sound_Font );
	pMidifile->LLSlide ( iSeconds );

	pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_PLAYER | RIGHT_MASK_CHANNELS | RIGHT_MASK_ERRORS );
	if ( m_RightPlayerView != NULL )
	{
		m_RightPlayerView->DisplayValues ();
	}

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnFileSequencerForwardll() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Then start the midi file playing
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	long iSeconds = ( pMidifile->LLGetTime () / 1000 );
	pMidifile->SetMidiDevice ( theApp.m_iMidiDevice );
	pMidifile->SetSoundFont ( theApp.m_SoundFontFile, theApp.m_bUse_Sound_Font );
	pMidifile->LLSlide ( iSeconds +  SLIDE_SECONDS );

	pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_PLAYER | RIGHT_MASK_ERRORS );
	if ( m_RightPlayerView != NULL )
	{
		m_RightPlayerView->DisplayValues ();
	}

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnFileSequencerResumell() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Then start the midi file playing
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	if ( pMidifile->LLIsPlaying () )
	{
		pMidifile->LLPause ();
	}
	else
	{
		pMidifile->LLResume ();
	}

	pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_PLAYER | RIGHT_MASK_CHANNELS | RIGHT_MASK_ERRORS );
	if ( m_RightPlayerView != NULL )
	{
		m_RightPlayerView->DisplayValues ();
	}

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnViewMcitoolbar() 
{
	// TODO
	int		bShow = IsMciBarVisible ();

	ShowOwnToolBar ( &m_wndMciToolBar, &m_wndMciLargeToolBar, ! bShow );
	ShowOwnReBar ( &m_wndMciReBar, &m_wndMciLargeReBar, ! bShow );
	theApp.m_bMciToolBar = IsMciBarVisible ();
	theApp.WriteProfileInt ( "Settings", "MCI Tool Bar", theApp.m_bMciToolBar );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnViewMiditoolbar() 
{
	// TODO
	int		bShow = IsMidiBarVisible ();

	ShowOwnToolBar ( &m_wndMidiToolBar, &m_wndMidiLargeToolBar, ! bShow );
	ShowOwnReBar ( &m_wndMidiToolBar, &m_wndMidiLargeReBar, ! bShow );
	theApp.m_bMidiToolBar = IsMidiBarVisible ();
	theApp.WriteProfileInt ( "Settings", "Midi Tool Bar", theApp.m_bMidiToolBar );
	
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnCorrectFiltertracks() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Then start the midi file playing
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	CWaitTreatment::BeginWait();
	pMidifile->FilterEventsTracks ( );
	pMidifile->Update();
	CWaitTreatment::EndWait();

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnCorrectAddGmreset() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Then start the midi file playing
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	CWaitTreatment::BeginWait();
	pMidifile->AddGMReset ( );
	pMidifile->Update();
	CWaitTreatment::EndWait();

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnCorrectAddGsreset() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Then start the midi file playing
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	CWaitTreatment::BeginWait();
	pMidifile->AddGSReset ( );
	pMidifile->Update();
	CWaitTreatment::EndWait();

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnCorrectAll() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	CWaitTreatment::BeginWait();

	pMidifile->CorrectAll ( theApp.m_iSysEx, 
		theApp.m_bAddStartOfTrack, theApp.m_bAddEndOfTrack,
		theApp.m_bBankSelectLow, theApp.m_bBankSelectHigh,
		theApp.m_bFilter, theApp.m_bCorrectStartOfTrack );

	pMidifile->Update();

	CWaitTreatment::EndWait();

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Options Dialog
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnCorrectOptions() 
{
	// TODO
	CCorrectOptions	dlgOptions;
	
	dlgOptions.DoModal ();
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//		System Exclusive Treatment
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentSystemexclusive() 
{
	// TODO
	CSysExDialog	dlgSysEx;

	dlgSysEx.DoModal ( );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Error Messages Display
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentErrormessages() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	if ( pMididoc == NULL )
	{
		return;
	}

	if ( theApp.m_bSplitterWindow )
	{
		if ( ! CreateColViewFull ( 1, ( CView ** ) & m_RightErrorView, 
							RUNTIME_CLASS(CErrorView), RIGHT_VIEW_ERRORS ) )
		{
			return;
		}
	}
	else
	{
		ActivateOrHideView ( m_RightErrorView, "Error Frame Geometry" );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnFilePreferences() 
{
	// TODO
	CPreferences		dlgPref;
	
	SaveOwnToolBar ( );

	if ( dlgPref.DoModal () == IDOK)
	{
		//		Set the new preferences
		theApp.WriteProfileInt ( "Settings", "Start Playing", theApp.m_iStart_Playing );
		theApp.WriteProfileInt ( "Settings", "SKip To Note", theApp.m_bSkip_To_Note );
		theApp.WriteProfileInt ( "Settings", "Midi Device", theApp.m_iMidiDevice );
		theApp.WriteProfileInt ( "Settings", "Midi Input Device", theApp.m_iMidiInDevice );
		theApp.WriteProfileInt ( "Settings", "Play Tune In About Box", theApp.m_bPlay_Tune_In_AboutBox );

		theApp.WriteProfileInt ( "Settings", "Large Bar", theApp.m_bLargeBar );
		theApp.WriteProfileInt ( "Settings", "Re Bar", theApp.m_bReBar );

		theApp.WriteProfileInt ( "Settings", "Background Color", theApp.m_bOwnBackground );
		theApp.WriteProfileInt ( "Settings", "Control Color", theApp.m_bOwnControlColor );

		theApp.WriteProfileInt ( "Settings", "Split Window", theApp.m_iSplitterWindow );

		theApp.WriteProfileString ( "Settings", "Wave Directory", theApp.m_WaveDirectory );

		theApp.WriteProfileInt ( "Settings", "Use Sound Font", theApp.m_bUse_Sound_Font );
		theApp.WriteProfileString ( "Settings", "Sound Font File", theApp.m_SoundFontFile );

		DisplayOwnToolBars ( );

//		LoadOwnToolBar ( );

	}

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnClose() 
{
	// TODO
	RECT		rectWindow;

	m_bClosing = true;

	if ( ! m_bPreview )
	{
		SaveOwnToolBar ( );

		//		If iconic do not save geometry
		if ( IsIconic ( ) )
		{
			theApp.WriteProfileInt ( "Settings", "Iconic", 1 );
			theApp.WriteProfileInt ( "Settings", "Zoomed", 0 );
		}

		if ( IsZoomed () )
		{
			theApp.WriteProfileInt ( "Settings", "Iconic", 0 );
			theApp.WriteProfileInt ( "Settings", "Zoomed", 1 );
		}

		//	Reset thos flags
		if ( ! IsIconic ( ) && ! IsZoomed () )
		{
			theApp.WriteProfileInt ( "Settings", "Iconic", 0 );
			theApp.WriteProfileInt ( "Settings", "Zoomed", 0 );
		}

		//		Get the window
		GetWindowRect ( &rectWindow );
		if ( rectWindow.left < GetSystemMetrics ( SM_CXSCREEN ) && 
			 rectWindow.top  < GetSystemMetrics ( SM_CYSCREEN ) )
		{
			theApp.WriteProfileInt ( "Settings", "Position x", rectWindow.left	);
			theApp.WriteProfileInt ( "Settings", "Position y", rectWindow.top );
			theApp.WriteProfileInt ( "Settings", "Size cx", rectWindow.right - rectWindow.left );
			theApp.WriteProfileInt ( "Settings", "Size cy", rectWindow.bottom - rectWindow.top);
		}

	}

	//		Destroy Windows
	CFrameWnd::OnClose();
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CFrameWnd::OnShowWindow(bShow, nStatus);
	
	// TODO
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::SizeAndPlaceWindow()
{
	//		Position the main window
	int iPosition_x	= theApp.GetProfileInt ( "Settings", "Position x", -1 );
	int iPosition_y	= theApp.GetProfileInt ( "Settings", "Position y", -1 );
	int iSize_cx	= theApp.GetProfileInt ( "Settings", "Size cx", -1 );
	int iSize_cy	= theApp.GetProfileInt ( "Settings", "Size cy", -1 );

	if ( iPosition_x != -1 )
	{
		if ( iPosition_x < GetSystemMetrics ( SM_CXSCREEN ) && 
			 iPosition_y < GetSystemMetrics ( SM_CYSCREEN ) )
		{
			MoveWindow ( iPosition_x, iPosition_y, iSize_cx, iSize_cy, TRUE );
		}
	}

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//		Display the parameter dialog box
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentParameters() 
{
	// TODO
	CParameters			dlgParam;
	char				szString [ MAX_CONTROLER_STRING ];
	int					iX;

	//		For All Parameters read them from registry
	for ( iX = 0; iX < MAX_CONTROLERS; iX++ )
	{

		sprintf_s ( szString, sizeof ( szString ), "Controler %03d", iX );

		theApp.m_cParameter_Settings [ iX ] = 
			theApp.GetProfileInt ( "Parameters", szString, -1 );

		if ( theApp.m_cParameter_Settings [ iX ] == -1 )
		{
			theApp.m_bParameter_Settings [ iX ] = false;
		}
		else
		{
			theApp.m_bParameter_Settings [ iX ] = true;
		}

	}

	if ( dlgParam.DoModal ( ) == IDOK )
	{
		//		For All Parameters write the in Registry
		for ( iX = 0; iX < MAX_CONTROLERS; iX++ )
		{

			sprintf_s ( szString, sizeof ( szString ), "Controler %03d", iX );

			if ( ! theApp.m_bParameter_Settings [ iX ] )
			{
				theApp.m_cParameter_Settings [ iX ] = -1;
				theApp.WriteProfileString( "Parameters", szString, NULL );
			}
			else
			{
				theApp.WriteProfileInt ( "Parameters", szString, theApp.m_cParameter_Settings [ iX ] );
			}
		}
	}

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnRemoveControllerBankselecthigh() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	pMidifile->RemoveEventCodeSubCode ( MIDI_EVENT_PARAMETER, CNTR_BANK_SELECT_MSB );

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnRemoveControllerBankselectlow() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );

	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	pMidifile->RemoveEventCodeSubCode ( MIDI_EVENT_PARAMETER, CNTR_BANK_SELECT_LSB );

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnRemoveCopyright() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	pMidifile->RemoveEventCode(MIDI_EVENT_COPYRIGHT);

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnRemoveCuepoint() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	pMidifile->RemoveEventCode(MIDI_EVENT_CUEPOINT);

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnRemoveInstrumentname() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	pMidifile->RemoveEventCode(MIDI_EVENT_INSTRNAME);

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnRemoveLyrics() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	pMidifile->RemoveEventCode(MIDI_EVENT_LYRICS);

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnRemoveMarkers() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	pMidifile->RemoveEventCode(MIDI_EVENT_MARKERS);

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS  | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );;

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnRemoveSequence() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	pMidifile->RemoveEventCode(MIDI_EVENT_SEQUENCE);

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS  | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );;

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnRemoveSequencer() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );

	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	pMidifile->RemoveEventCode(MIDI_EVENT_SEQUENCER);

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS  | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );;

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnRemoveSystemexclusive() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	pMidifile->RemoveEventCode(MIDI_EVENT_SYSEX);

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS  | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );;

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnRemoveText() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	pMidifile->RemoveEventCode(MIDI_EVENT_TEXT);

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS  | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );;

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnRemoveTrackname() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	pMidifile->RemoveEventCode(MIDI_EVENT_TRACKNAME);

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS  | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );;

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentPlaylist() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();

	if ( pMididoc == NULL )
	{
		return;
	}

	if ( theApp.m_bSplitterWindow )
	{
		if ( ! CreateColViewFull ( 1, ( CView ** ) & m_RightPlayListView, 
							RUNTIME_CLASS(CPlayList), RIGHT_VIEW_PLAYLIST ) )
		{
			return;
		}
	}
	else
	{
		ActivateOrHideView ( m_RightPlayListView, "PlayList Frame Geometry" );
	}
	
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	
	// TODO
	if ( theApp.m_PlayListThread != NULL )
	{
		theApp.m_dlgPlayerDialogList.EndDialog ( 0 );
	}

	if ( theApp.m_PlayDirThread != NULL )
	{
		theApp.m_dlgPlayerDialogDirectory.EndDialog ( 0 );
	}

	if ( theApp.m_ModifyThread != NULL )
	{
		theApp.m_ModifyThreadDialog.EndDialog ( 0 );
	}

	if ( theApp.m_RecordThread != NULL )
	{
		theApp.m_dlgRecordDialog.EndDialog ( 0 );
	}

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnFileAddtotheplaylist() 
{
	// TODO
	int					iCount;
	char				szPathname [ MAX_PATHNAME_STRING ];
	char				szString [ MAX_TUNE_STRING ];
	CString				strReturned;
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	//		First Save the current play list
	if ( m_RightPlayListView != NULL )
	{
		m_RightPlayListView->SaveTheList ();
	}

	strReturned = pMididoc->GetPathName ( );
	if ( strReturned != "" )
	{
		iCount = theApp.GetProfileInt( "Play List", "Tune Count", 0 );

		strcpy_s ( szPathname, sizeof ( szPathname ), strReturned );
		sprintf_s ( szString, sizeof ( szString ), "Tune %03d", iCount );
		theApp.WriteProfileString( "Play List", szString, szPathname );

		iCount++;
		theApp.WriteProfileInt( "Play List", "Tune Count", iCount );
	}

	pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_PLAYLIST );
	if ( m_RightPlayListView != NULL )
	{
		m_RightPlayListView->DisplayValues ();
	}

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
char * CMainFrame::GetCurrentPlaylistFilename()
{
	if ( m_RightPlayListView != NULL )
	{
		return m_RightPlayListView->GetCurrentPlaylistFilename();
	}
	else
	{
		return NULL;
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnViewMaintoolbar() 
{
	// TODO
	int		bShow = IsMainBarVisible ();

	ShowOwnToolBar ( &m_wndMainToolBar, &m_wndMainLargeToolBar, ! bShow );
	ShowOwnReBar ( &m_wndMainReBar, &m_wndMainLargeReBar, ! bShow );
	theApp.m_bMainToolBar = IsMainBarVisible ();
	theApp.WriteProfileInt ( "Settings", "Tool Bar", theApp.m_bMainToolBar );
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnCorrectTimesignature() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	CWaitTreatment::BeginWait();
	pMidifile->MoveEventToTrack ( MIDI_EVENT_TIMESIG, 0 );
	pMidifile->Update();
	CWaitTreatment::EndWait();

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS  | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );;

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnCorrectKeysignature() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	CWaitTreatment::BeginWait();
	pMidifile->MoveEventToTrack ( MIDI_EVENT_KEYSIG, 0 );
	pMidifile->Update();
	CWaitTreatment::EndWait();

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS  | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );;

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnCorrectMoveAll() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL )
	{
		return;
	}

	if ( pMidifile == NULL )
	{
		return;
	}

	CWaitTreatment::BeginWait();
	pMidifile->MoveEventToTrack ( MIDI_EVENT_ALL_EVENTS, 0 );
	pMidifile->Update();
	CWaitTreatment::EndWait();

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS  | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );;

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CMainFrame::ResetViewHandles()
{
	ResetLeftViewHandles ();
	ResetRightViewHandles ();
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::ResetLeftViewHandles()
{
	m_LeftView			= NULL;

	m_LeftTrackView		= NULL;
	m_LeftExplorerView	= NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::ResetRightViewHandles()
{
 	m_RightView			= NULL;
 
 	m_RightEventView	= NULL;
 	m_RightPlayerView	= NULL;
 	m_RightPlayListView	= NULL;
 	m_RightTextView		= NULL;
 	m_RightGeneralView	= NULL;
 	m_RightChannelView	= NULL;
 	// m_RightBrowserView	= NULL;
	m_RightStaffView	= NULL;
	m_RightNotesView	= NULL;
	m_RightErrorView	= NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMidiTrack *CMainFrame::GetCurrentTrack()
{
	if ( m_LeftTrackView != NULL )
	{
		return m_LeftTrackView->GetCurrentTrack ();
	}
	else
	{
		return NULL;
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentChannels() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();

	if ( pMididoc == NULL )
	{
		return;
	}

	if ( theApp.m_bSplitterWindow )
	{
		if ( ! CreateColViewFull ( 1, ( CView ** ) & m_RightChannelView, 
							RUNTIME_CLASS(CChannelView), RIGHT_VIEW_CHANNELS ) )
		{
			return;
		}
	}
	else
	{
		ActivateOrHideView ( m_RightChannelView, "Channel Frame Geometry" );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::SetActiveCWndAsView(CWnd *wnd)
{
	CView *view = dynamic_cast<CView *>(wnd);
	if ( view != NULL )
	{
		SetActiveView ( view );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnViewRightview() 
{
	// TODO
	CRect				rectFrame;
	CRect				rectDialog;
	CSize				size;

	if ( theApp.m_bSplitterWindow )
	{
		GetClientRect(&rectFrame);
		size		= rectFrame.Size();

		SetActiveCWndAsView(m_wndSplitter.GetPane(0,1));
		
		size		= rectFrame.Size();
		m_wndSplitter.SetColumnInfo ( 0, 0, 0 );
		m_wndSplitter.SetColumnInfo ( 1, size.cx, 0 );

		m_wndSplitter.RecalcLayout ();

		RefreshFrame ();

		RecalcLayout ();
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnViewLeftview() 
{
	// TODO
	CRect				rectFrame;
	CRect				rectDialog;
	CSize				size;

	if ( theApp.m_bSplitterWindow )
	{
		GetClientRect(&rectFrame);
		size		= rectFrame.Size();

		SetActiveCWndAsView(m_wndSplitter.GetPane(0,0));
		
		size		= rectFrame.Size();
		m_wndSplitter.SetColumnInfo ( 0, size.cx, 0 );
		m_wndSplitter.SetColumnInfo ( 1, 0, 0 );

		m_wndSplitter.RecalcLayout ();

		RefreshFrame ();

		RecalcLayout ();
	}

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnViewTwoviews() 
{
	// TODO
	CRect				rectFrame;
	CRect				rectDialog;
	CSize				size;

	if ( theApp.m_bSplitterWindow )
	{
		GetClientRect(&rectFrame);

		SetActiveCWndAsView(m_wndSplitter.GetPane(0,1));
		
		size		= rectFrame.Size();
		size.cx		= ( size.cx * 7 ) / 20;
		m_wndSplitter.SetColumnInfo ( 0, size.cx, 0 );

		size		= rectFrame.Size();
		size.cx		= ( size.cx * 13 ) / 20;
		m_wndSplitter.SetColumnInfo ( 1, size.cx, 0 );

		m_wndSplitter.RecalcLayout ();

		RefreshFrame ();

		RecalcLayout ();
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnRemoveControllersResetallcontrolers() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	pMidifile->RemoveEventCodeSubCode ( MIDI_EVENT_PARAMETER, CNTR_ALL_CONTROLLERS_OFF );

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS  | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );;

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentStartIe() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	if ( pMididoc == NULL )
	{
		return;
	}

	//
	HINSTANCE hInstance = ShellExecute(NULL, "open", PROGRAM_HTTP, NULL, NULL, SW_SHOWNORMAL);

#if 0
	if ( theApp.m_bSplitterWindow )
	{
		if ( ! CreateColViewFull ( 1, ( CView ** ) & m_RightBrowserView, 
							RUNTIME_CLASS(CMfcMsIeView), RIGHT_VIEW_BROWSER ) )
		{
			return;
		}
	}
	else
	{
		m_RightBrowserView->GetParentFrame ( )->ModifyStyle ( NULL, WS_VISIBLE );
		m_RightBrowserView->ModifyStyle ( NULL, WS_VISIBLE );
		SetActiveView ( m_RightBrowserView );
		Invalidate ( );
	}

	//		Start with the homepage url
	CString strUrl;
	strUrl.LoadString ( ID_HOMEPAGE_URL );
	m_RightBrowserView->Navigate2 ( strUrl );
#endif
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentStopIe() 
{
#if 0
	// TODO
	if ( m_RightBrowserView != NULL )
	{
		m_RightBrowserView->Stop ();

		if ( ! theApp.m_bSplitterWindow )
		{
			m_RightBrowserView->GetParentFrame ( )->ModifyStyle ( WS_VISIBLE, NULL );
			m_RightBrowserView->ModifyStyle ( WS_VISIBLE, NULL );
			Invalidate ( );
		}
	}
#endif
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnViewIetoolbar() 
{
	// TODO
	int		bShow = IsIeBarVisible ();

	ShowOwnToolBar ( &m_wndIeToolBar, &m_wndIeLargeToolBar, ! bShow );
	ShowOwnReBar ( &m_wndIeReBar, &m_wndIeLargeReBar, ! bShow );
	theApp.m_bIeToolBar = IsIeBarVisible ();
	theApp.WriteProfileInt ( "Settings", "IE Tool Bar", theApp.m_bIeToolBar );
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnViewSF2toolbar() 
{
	// TODO
	int		bShow = IsSF2BarVisible ();

	ShowOwnToolBar ( &m_wndSF2ToolBar, &m_wndSF2LargeToolBar, ! bShow );
	ShowOwnReBar ( &m_wndSF2ReBar, &m_wndSF2LargeReBar, ! bShow );
	theApp.m_bSf2ToolBar = IsSF2BarVisible ();
	theApp.WriteProfileInt ( "Settings", "SF2 Tool Bar", theApp.m_bSf2ToolBar );
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO
	CRect		rect;
	GetClientRect ( &rect );
	ClientToScreen ( &rect );

	CRect		rectRight;
	if ( m_RightView != NULL )
	{
		m_RightView->GetClientRect ( &rectRight );
		m_RightView->ClientToScreen ( &rectRight );
	}

	CRect		rectLeft;
	if ( m_LeftExplorerView != NULL )
	{
		if ( ! theApp.m_bSplitterWindow ) 
		{
			m_LeftExplorerView->BringWindowToTop ( );
			SetActiveView ( m_LeftExplorerView );
		}

		m_LeftExplorerView->GetClientRect ( &rectLeft );
		m_LeftExplorerView->ClientToScreen ( &rectLeft );
        m_LeftExplorerView->OnContextMenu( pWnd, point );
		return;
	}

	if ( rect.PtInRect ( point ) )
	{
		CMWMenu		menu;
		menu.LoadMenu ( IDR_MENU_VIEWS );
		m_pContextMenu = menu.GetSubMenu ( 0 );
		m_pContextMenu->TrackPopupMenu (
			TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, 
			point.x, point.y, this );
		return;
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnViewStatusBar() 
{
	// TODO
	ShowControlBar ( &m_wndStatusBar, 
						( m_wndStatusBar.GetStyle () & WS_VISIBLE ) == 0,
						FALSE );
	theApp.m_bStatusBar = ( ( m_wndStatusBar.GetStyle () & WS_VISIBLE ) != 0 );
	theApp.WriteProfileInt ( "Settings", "Status Bar", theApp.m_bStatusBar );
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMidiDoc *CMainFrame::GetMidiDoc()
{
	//		Then start the midi file playing
	return dynamic_cast< CMidiDoc * >( GetActiveDocument() );

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMidiFile * CMainFrame::GetMidiFile()
{
	CMidiDoc			*pMididoc = GetMidiDoc ();

	//		Get the midi file
	if ( pMididoc == NULL )
	{
		return NULL;
	}

	return pMididoc->GetDocumentMidiFile();

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentStaff() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();

	if ( pMididoc == NULL )
	{
		return;
	}

	if ( theApp.m_bSplitterWindow )
	{
		if ( ! CreateColViewFull ( 1, ( CView ** ) & m_RightStaffView, RUNTIME_CLASS(CStaffView), RIGHT_VIEW_STAFF ) )
		{
			return;
		}
	}
	else
	{
		ActivateOrHideView ( m_RightStaffView, "Staff Frame Geometry" );
	}

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnCorrectTruncateNote() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Correct all parameters
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	CWaitTreatment::BeginWait();
	pMidifile->CorrectNoteDuration ( );
	pMidifile->Update();
	CWaitTreatment::EndWait();

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS  | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );;

	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentNotes() 
{
	// TODO

	if ( theApp.m_bSplitterWindow )
	{
		if ( ! CreateColViewFull ( 1, ( CView ** ) & m_RightNotesView, 
							RUNTIME_CLASS(CNotesView), RIGHT_VIEW_NOTES ) )
		{
			return;
		}
	}
	else
	{
		ActivateOrHideView ( m_RightNotesView, "Notes Frame Geometry" );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentTracksList() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();

	if ( pMididoc == NULL )
	{
		return;
	}

	if ( theApp.m_bSplitterWindow )
	{
		if ( ! CreateColViewFull ( 0, ( CView ** ) & m_LeftTrackView, 
							RUNTIME_CLASS(CTracksView), LEFT_VIEW_TRACKS ) )
		{
			return;
		}
		
		//		Resize view
		OnViewTwoviews();

	}
	else
	{
		ActivateOrHideView ( m_LeftTrackView, "Track Frame Geometry" );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentMidiexplorer() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();

	if ( pMididoc == NULL )
	{
		return;
	}

	if ( theApp.m_bSplitterWindow )
	{
		if ( ! CreateColViewFull ( 0, ( CView ** ) & m_LeftExplorerView, 
							RUNTIME_CLASS(CMidiExplorerView), LEFT_VIEW_EXPLORER ) )
		{
			return;
		}
		
		//		Resize view
		OnViewTwoviews();
	}
	else
	{
		ActivateOrHideView ( m_LeftExplorerView, "Explorer Frame Geometry" );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
HBRUSH CMainFrame::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFrameWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO
	HBRUSH hBrush = FriendCtlColor(pDC, pWnd, nCtlColor);
	if ( hBrush != NULL )
	{
		return hBrush;
	}

	// TODO
	return hbr;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::OnEraseBkgnd(CDC* pDC) 
{
	// TODO
	return CFrameWnd::OnEraseBkgnd ( pDC );

/*
	if ( ! theApp.m_bSplitterWindow && ! m_bClosing )
	{
		BOOL bRes = FriendEraseBkgnd(this, pDC);
		if ( bRes )
		{
			return bRes;
		}
	}
 */

	CRect		rect;
	GetClientRect ( &rect );

	CBrush		brColor ( RGB (128,128,128) );
	CBrush		*pOldBrush = pDC->SelectObject ( &brColor );

	pDC->PatBlt ( rect.left, rect.top, rect.right, rect.bottom, PATCOPY );

	pDC->SelectObject ( pOldBrush );

	return true;

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::SetSearchInfo(	const CString strEvent,
								const CString strProgram,
								const CString strController,
								int V1_From, int V1_To, 
								int V2_From, int V2_To )
{
	if ( m_RightEventView != NULL )
	{
		m_RightEventView->SetSearchInfo(
			strEvent,strProgram,strController,
			V1_From, V1_To, V2_From, V2_To );
	}
	else
	{
		return;
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnCorrectAddXgreset() 
{
	// TODO
	CMidiDoc			*pMididoc	= GetMidiDoc ();
	CMidiFile			*pMidifile	= GetMidiFile ( );
	
	//		Then start the midi file playing
	if ( pMididoc == NULL || pMidifile == NULL )
	{
		return;
	}

	CWaitTreatment::BeginWait();
	pMidifile->AddXGReset ( );
	pMidifile->Update();
	CWaitTreatment::EndWait();

	SetModified ( TRUE );

	pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS  | RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );;

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
int CMainFrame::CreateOwnToolBars()
{
	DWORD dwCtrlStyle	= RBS_BANDBORDERS;
	DWORD dwStyle		= WS_CHILD | WS_CLIPSIBLINGS |
		WS_CLIPCHILDREN | CBRS_TOP;

	if ( ! m_wndReBar.Create ( this, dwCtrlStyle, dwStyle ) )
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if ( ! m_wndLargeReBar.Create ( this, dwCtrlStyle, dwStyle ) )
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	//		Tool bar style
	DWORD		tbCtrlStyle;
	DWORD		tbStyle;

	tbCtrlStyle =	TBSTYLE_FLAT;
	tbStyle		=	WS_CHILD | CBRS_TOP | CBRS_TOOLTIPS |
						CBRS_FLYBY | CBRS_GRIPPER | CBRS_SIZE_DYNAMIC;

	//	Create the first toolbar	
	if ( ! m_wndMainToolBar.CreateEx(this, tbCtrlStyle, tbStyle ) ||
		!  m_wndMainToolBar.LoadToolBar(IDR_MAINFRAME)					)
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (! m_wndMainLargeToolBar.CreateEx(this, tbCtrlStyle, tbStyle, CRect(0,0,0,0), IDR_LARGE_MAINFRAME ) ||
		! m_wndMainLargeToolBar.LoadToolBar(IDR_MAINFRAME)														)
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//		Create the MCI Toolbar
	if (!m_wndMciToolBar.CreateEx(this, tbCtrlStyle, tbStyle, CRect(0,0,0,0), IDR_MCI_TOOLBAR ) ||
		!m_wndMciToolBar.LoadToolBar(IDR_MCI_TOOLBAR)												)
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndMciLargeToolBar.CreateEx(this, tbCtrlStyle, tbStyle, CRect(0,0,0,0), IDR_MCI_LARGE_TOOLBAR ) ||
		!m_wndMciLargeToolBar.LoadToolBar(IDR_MCI_TOOLBAR) )
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//		Create the Midi Toolbar
	if (!m_wndMidiToolBar.CreateEx(this, tbCtrlStyle, tbStyle, 	CRect(0,0,0,0),IDR_MIDI_TOOLBAR ) ||
		!m_wndMidiToolBar.LoadToolBar(IDR_MIDI_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndMidiLargeToolBar.CreateEx(this, tbCtrlStyle, tbStyle, CRect(0,0,0,0),IDR_MIDI_LARGE_TOOLBAR ) ||
		!m_wndMidiLargeToolBar.LoadToolBar(IDR_MIDI_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//		Create the Ie Toolbar
	if (!m_wndIeToolBar.CreateEx(this, tbCtrlStyle, tbStyle, CRect(0,0,0,0), IDR_IE_TOOLBAR ) ||
		!m_wndIeToolBar.LoadToolBar(IDR_IE_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndIeLargeToolBar.CreateEx(this, tbCtrlStyle, tbStyle, CRect(0,0,0,0), IDR_IE_LARGE_TOOLBAR ) ||
		!m_wndIeLargeToolBar.LoadToolBar(IDR_IE_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//		Soubd Font
	if ( ! m_wndSF2ToolBar.CreateEx(this, tbCtrlStyle, tbStyle, CRect(0,0,0,0), IDR_SF2_TOOLBAR ) ||
		 ! m_wndSF2ToolBar.LoadToolBar(IDR_SF2_TOOLBAR) ||
		 ! m_wndSF2ToolBar.CreateComboBox ( m_wndSF2ToolBar.m_comboBox, 1, IDC_SOUNDFONT_COMBO_1, 240, 100) )
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//
	if ( ! m_wndSF2LargeToolBar.CreateEx(this, tbCtrlStyle, tbStyle, CRect(0,0,0,0), IDR_SF2_LARGE_TOOLBAR ) ||
		 ! m_wndSF2LargeToolBar.LoadToolBar(IDR_SF2_TOOLBAR) ||
		 ! m_wndSF2LargeToolBar.CreateComboBox ( m_wndSF2LargeToolBar.m_comboBox, 1, IDC_SOUNDFONT_COMBO_1, 240, 100) )
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}


	//		Now for the rebars
	tbStyle		=	WS_CHILD | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC;

	//	Create the first toolbar	
	if (! m_wndMainReBar.CreateEx(this, tbCtrlStyle, tbStyle ) ||
		! m_wndMainReBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (! m_wndMainLargeReBar.CreateEx(this, tbCtrlStyle, tbStyle, CRect(0,0,0,0), IDR_LARGE_MAINFRAME ) ||
		! m_wndMainLargeReBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//		Create the MCI Toolbar
	if ( ! m_wndMciReBar.CreateEx(this, tbCtrlStyle, tbStyle, CRect(0,0,0,0), IDR_MCI_TOOLBAR ) ||
		 ! m_wndMciReBar.LoadToolBar(IDR_MCI_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//
	if ( ! m_wndMciLargeReBar.CreateEx(this, tbCtrlStyle, tbStyle, CRect(0,0,0,0), IDR_MCI_LARGE_TOOLBAR ) ||
		 ! m_wndMciLargeReBar.LoadToolBar(IDR_MCI_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

    //		Create the Midi Toolbar
	if ( ! m_wndMidiReBar.CreateEx(this, tbCtrlStyle, tbStyle, CRect(0,0,0,0),IDR_MIDI_TOOLBAR ) ||
		 ! m_wndMidiReBar.LoadToolBar(IDR_MIDI_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//
	if ( ! m_wndMidiLargeReBar.CreateEx(this, tbCtrlStyle, tbStyle, CRect(0,0,0,0),IDR_MIDI_LARGE_TOOLBAR ) ||
		 ! m_wndMidiLargeReBar.LoadToolBar(IDR_MIDI_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//		Create the Ie Toolbar
	if ( ! m_wndIeReBar.CreateEx(this, tbCtrlStyle, tbStyle, CRect(0,0,0,0), IDR_IE_TOOLBAR ) ||
		 ! m_wndIeReBar.LoadToolBar(IDR_IE_TOOLBAR) )
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if ( ! m_wndIeLargeReBar.CreateEx(this, tbCtrlStyle, tbStyle, CRect(0,0,0,0), IDR_IE_LARGE_TOOLBAR ) ||
		 ! m_wndIeLargeReBar.LoadToolBar(IDR_IE_TOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//
	//	Sound Font
	if ( ! m_wndSF2ReBar.CreateEx(this, tbCtrlStyle, tbStyle, CRect(0,0,0,0), IDR_SF2_TOOLBAR ) ||
		 ! m_wndSF2ReBar.LoadToolBar(IDR_SF2_TOOLBAR) || 
		 ! m_wndSF2ReBar.CreateComboBox ( m_wndSF2ReBar.m_comboBox, 1, IDC_SOUNDFONT_COMBO_1, 240, 100) )
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	//
	//	Sound Font
	if ( ! m_wndSF2LargeReBar.CreateEx(this, tbCtrlStyle, tbStyle, CRect(0,0,0,0), IDR_SF2_LARGE_TOOLBAR ) ||
		 ! m_wndSF2LargeReBar.LoadToolBar(IDR_SF2_TOOLBAR) || 
		 ! m_wndSF2LargeReBar.CreateComboBox ( m_wndSF2LargeReBar.m_comboBox, 1, IDC_SOUNDFONT_COMBO_1, 240, 100) )
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	return 0;

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
int CMainFrame::CreateOwnStatusBar()
{
	//		Create the status bar
	if ( ! m_wndStatusBar.Create(this) || ! m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	//		Set size of the ID_INDICATOR_ERROR
	RECT				rectFrame;
	UINT				nID;
	UINT				nStyle;
	int					cxWidth;

	//		Get the Frame Window Client size
	GetClientRect ( &rectFrame );

	//		Get and reduce the size of the display text in the status bar
	m_wndStatusBar.GetPaneInfo ( 0, nID, nStyle, cxWidth );
	m_wndStatusBar.SetPaneInfo ( 0, nID, nStyle, 80 );

	m_wndStatusBar.GetPaneInfo ( 0, nID, nStyle, cxWidth );
	rectFrame.right -= cxWidth;

	//		Get the CAP size
	m_wndStatusBar.GetPaneInfo ( 2, nID, nStyle, cxWidth );
	rectFrame.right -= cxWidth;

	//		Get the NUM size
	m_wndStatusBar.GetPaneInfo ( 3, nID, nStyle, cxWidth );
	rectFrame.right -= cxWidth;

	//		Get The Lock Size
	m_wndStatusBar.GetPaneInfo ( 4, nID, nStyle, cxWidth );
	rectFrame.right -= cxWidth;

	//		Set the width for our error text
	m_wndStatusBar.SetPaneInfo( 1, ID_INDICATOR_ERROR, 0, rectFrame.right - 80 );

	if ( ! theApp.m_bStatusBar )
	{
		ShowControlBar ( &m_wndStatusBar, ( m_wndStatusBar.GetStyle () & WS_VISIBLE ) == 0, FALSE );
	}

	return 0;

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::SetOwnToolBars()
{
	//		First for tools bars
	m_wndMciLargeToolBar.SetButtonText ( 0, "Play" );
	m_wndMciLargeToolBar.SetButtonText ( 1, "Stop" );
	m_wndMciLargeToolBar.SetButtonText ( 2, "Loop" );
	m_wndMciLargeToolBar.SetSizes ( CSize ( 46, 42 ), CSize ( 16, 15 ) );

	m_wndMciToolBar.SetWindowText ( "MCI Player" );
	m_wndMciLargeToolBar.SetWindowText ( "MCI Player" );

	m_wndMciLargeToolBar.SetAnsiVarFont ();

	//	Set Text and Size
	m_wndMidiLargeToolBar.SetButtonText ( 0, "Back" );
	m_wndMidiLargeToolBar.SetButtonText ( 1, "Play" );
	m_wndMidiLargeToolBar.SetButtonText ( 2, "Loop" );
	m_wndMidiLargeToolBar.SetButtonText ( 3, "Pause" );
	m_wndMidiLargeToolBar.SetButtonText ( 4, "Forw." );
	m_wndMidiLargeToolBar.SetButtonText ( 5, "Stop" );
	m_wndMidiLargeToolBar.SetSizes ( CSize ( 46, 42 ), CSize ( 16, 15 ) );

	m_wndMidiToolBar.SetWindowText ( "Midi Player" );
	m_wndMidiLargeToolBar.SetWindowText ( "Midi Player" );

	m_wndMidiLargeToolBar.SetAnsiVarFont ();

	m_wndMainLargeToolBar.SetButtonText ( 0, "New" );
	m_wndMainLargeToolBar.SetButtonText ( 1, "Open" );
	m_wndMainLargeToolBar.SetButtonText ( 2, "Save" );
	m_wndMainLargeToolBar.SetButtonText ( 3, "Cut" );
	m_wndMainLargeToolBar.SetButtonText ( 4, "Copy" );
	m_wndMainLargeToolBar.SetButtonText ( 5, "Paste" );
	m_wndMainLargeToolBar.SetButtonText ( 6, "Print" );
	m_wndMainLargeToolBar.SetButtonText ( 7, "Help" );
	m_wndMainLargeToolBar.SetSizes ( CSize ( 46, 42 ), CSize ( 16, 15 ) );

	m_wndMainToolBar.SetWindowText ( "General Usage Tools" );
	m_wndMainLargeToolBar.SetWindowText ( "General Usage Tools" );

	m_wndMainLargeToolBar.SetAnsiVarFont ();

	//	Set Text and Size
	m_wndIeLargeToolBar.SetButtonText ( 0, "Start" );
	m_wndIeLargeToolBar.SetButtonText ( 1, "Stop" );
	m_wndIeLargeToolBar.SetSizes ( CSize ( 46, 42 ), CSize ( 16, 15 ) );

	m_wndIeToolBar.SetWindowText ( "Internet Explorer" );
	m_wndIeLargeToolBar.SetWindowText ( "Internet Explorer" );

	m_wndIeLargeToolBar.SetAnsiVarFont ();

	//	Set Text and Size
	m_wndSF2LargeToolBar.SetButtonText ( 0, "" );
	m_wndSF2LargeToolBar.SetButtonText ( 1, "" );
	m_wndSF2LargeToolBar.SetSizes ( CSize ( 46, 42 ), CSize ( 16, 15 ) );

	m_wndSF2ToolBar.SetWindowText ( "Sound Fount 2" );
	m_wndSF2LargeToolBar.SetWindowText ( "Sound Fount 2" );

	m_wndSF2LargeToolBar.SetAnsiVarFont ();

	//	Then for the ReBars
	m_wndMciLargeReBar.SetButtonText ( 0, "Play" );
	m_wndMciLargeReBar.SetButtonText ( 1, "Stop" );
	m_wndMciLargeReBar.SetButtonText ( 2, "Loop" );
	m_wndMciLargeReBar.SetSizes ( CSize ( 46, 42 ), CSize ( 16, 15 ) );

	m_wndMciReBar.SetWindowText ( "MCI Player" );
	m_wndMciLargeReBar.SetWindowText ( "MCI Player" );

	m_wndMciLargeReBar.SetAnsiVarFont ();

	//	Set Text and Size
	m_wndMidiLargeReBar.SetButtonText ( 0, "Back" );
	m_wndMidiLargeReBar.SetButtonText ( 1, "Play" );
	m_wndMidiLargeReBar.SetButtonText ( 2, "Loop" );
	m_wndMidiLargeReBar.SetButtonText ( 3, "Pause" );
	m_wndMidiLargeReBar.SetButtonText ( 4, "Forw." );
	m_wndMidiLargeReBar.SetButtonText ( 5, "Stop" );
	m_wndMidiLargeReBar.SetSizes ( CSize ( 46, 42 ), CSize ( 16, 15 ) );

	m_wndMidiReBar.SetWindowText ( "Midi Player" );
	m_wndMidiLargeReBar.SetWindowText ( "Midi Player" );

	m_wndMidiLargeReBar.SetAnsiVarFont ();

	m_wndMainLargeReBar.SetButtonText ( 0, "New" );
	m_wndMainLargeReBar.SetButtonText ( 1, "Open" );
	m_wndMainLargeReBar.SetButtonText ( 2, "Save" );
	m_wndMainLargeReBar.SetButtonText ( 3, "Cut" );
	m_wndMainLargeReBar.SetButtonText ( 4, "Copy" );
	m_wndMainLargeReBar.SetButtonText ( 5, "Paste" );
	m_wndMainLargeReBar.SetButtonText ( 6, "Print" );
	m_wndMainLargeReBar.SetButtonText ( 7, "Help" );
	m_wndMainLargeReBar.SetSizes ( CSize ( 46, 42 ), CSize ( 16, 15 ) );

	m_wndMainReBar.SetWindowText ( "General Usage Tools" );
	m_wndMainLargeReBar.SetWindowText ( "General Usage Tools" );

	m_wndMainLargeReBar.SetAnsiVarFont ();

	//	Set Text and Size
	m_wndIeLargeReBar.SetButtonText ( 0, "Start" );
	m_wndIeLargeReBar.SetButtonText ( 1, "Stop" );
	m_wndIeLargeReBar.SetSizes ( CSize ( 46, 42 ), CSize ( 16, 15 ) );

	m_wndIeReBar.SetWindowText ( "Internet Explorer" );
	m_wndIeLargeReBar.SetWindowText ( "Internet Explorer" );

	m_wndIeLargeReBar.SetAnsiVarFont ();

	//	Set Text and Size
	m_wndSF2LargeReBar.SetButtonText ( 0, "" );
	m_wndSF2LargeReBar.SetButtonText ( 1, "" );
	m_wndSF2LargeReBar.SetSizes ( CSize ( 46, 42 ), CSize ( 16, 15 ) );

	m_wndSF2ReBar.SetWindowText ( "Sound Fount 2" );
	m_wndSF2LargeReBar.SetWindowText ( "Sound Fount 2" );

	m_wndSF2LargeReBar.SetAnsiVarFont ();

	//		Do rebar
	m_wndReBar.AddBar ( &m_wndMainReBar );
	m_wndReBar.AddBar ( &m_wndMciReBar );
	m_wndReBar.AddBar ( &m_wndMidiReBar );
	m_wndReBar.AddBar ( &m_wndIeReBar );
	m_wndReBar.AddBar ( &m_wndSF2ReBar );

	m_wndLargeReBar.AddBar ( &m_wndMainLargeReBar );
	m_wndLargeReBar.AddBar ( &m_wndMciLargeReBar );
	m_wndLargeReBar.AddBar ( &m_wndMidiLargeReBar );
	m_wndLargeReBar.AddBar ( &m_wndIeLargeReBar );
	m_wndLargeReBar.AddBar ( &m_wndSF2LargeReBar );

	m_wndReBar.SetBarStyle ( CBRS_ALIGN_ANY );

	m_wndLargeReBar.SetBarStyle ( CBRS_ALIGN_ANY );

	DisplayOwnToolBars();

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::DisplayOwnToolBars()
{
	EnableDocking(CBRS_ALIGN_ANY);

	//	Enable Docking
	m_wndMainToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMciToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMidiToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndIeToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndSF2ToolBar.EnableDocking(CBRS_ALIGN_ANY);

	m_wndMainLargeToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMciLargeToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMidiLargeToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndIeLargeToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndSF2LargeToolBar.EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar ( &m_wndMainToolBar );
	DockControlBar ( &m_wndMciToolBar );
	DockControlBar ( &m_wndMidiToolBar );
	DockControlBar ( &m_wndIeToolBar );
	DockControlBar ( &m_wndSF2ToolBar );

	DockControlBar ( &m_wndMainLargeToolBar );
	DockControlBar ( &m_wndMciLargeToolBar );
	DockControlBar ( &m_wndMidiLargeToolBar );
	DockControlBar ( &m_wndIeLargeToolBar );
	DockControlBar ( &m_wndSF2LargeToolBar );

	ShowOwnToolBar ( &m_wndMainToolBar, &m_wndMainLargeToolBar, theApp.m_bMainToolBar );
	ShowOwnToolBar ( &m_wndMciToolBar, &m_wndMciLargeToolBar, theApp.m_bMciToolBar );
	ShowOwnToolBar ( &m_wndMidiToolBar, &m_wndMidiLargeToolBar, theApp.m_bMidiToolBar );
	ShowOwnToolBar ( &m_wndIeToolBar, &m_wndIeLargeToolBar, theApp.m_bIeToolBar );
	ShowOwnToolBar ( &m_wndSF2ToolBar, &m_wndSF2LargeToolBar, theApp.m_bSf2ToolBar );

	ShowOwnReBar ( &m_wndMainReBar, &m_wndMainLargeReBar, theApp.m_bMainToolBar );
	ShowOwnReBar ( &m_wndMciReBar, &m_wndMciLargeReBar, theApp.m_bMciToolBar );
	ShowOwnReBar ( &m_wndMidiReBar, &m_wndMidiLargeReBar, theApp.m_bMidiToolBar );
	ShowOwnReBar ( &m_wndIeReBar, &m_wndIeLargeReBar, theApp.m_bIeToolBar );
	ShowOwnReBar ( &m_wndSF2ReBar, &m_wndSF2LargeReBar, theApp.m_bSf2ToolBar );
	

	//	Hot rebar
	m_wndReBar.ModifyStyle ( WS_VISIBLE, NULL );
	m_wndLargeReBar.ModifyStyle ( WS_VISIBLE, NULL );
	if ( theApp.m_bReBar )
	{
		if ( theApp.m_bLargeBar )
		{
			m_wndLargeReBar.ModifyStyle ( NULL, WS_VISIBLE );
		}
		else
		{
			m_wndReBar.ModifyStyle ( NULL, WS_VISIBLE);
		}
	}

	//	Recalc layout
	RecalcLayout();

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::SaveOwnToolBar()
{
	CString		strToolText;

	if ( theApp.m_bLargeBar )
	{
		strToolText = "Large ";
	}
	else
	{
		strToolText = "Normal ";
	}

	if ( theApp.m_bReBar )
	{
		strToolText = strToolText + "ReBar State ";
	}
	else
	{
		strToolText = strToolText + "ToolBar State ";
	}

	SaveBarState ( strToolText );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::LoadOwnToolBar()
{
	CString		strToolText;

	if ( theApp.m_bLargeBar )
	{
		strToolText = "Large ";
	}
	else
	{
		strToolText = "Normal ";
	}

	if ( theApp.m_bReBar )
	{
		strToolText = strToolText + "ReBar State ";
	}
	else
	{
		strToolText = strToolText + "ToolBar State ";
	}

	LoadBarState ( strToolText );

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::ShowOwnToolBar(CMWToolBar *pToolBar, 
								CMWToolBar *pLargeToolBar, int bShow)
{
	if ( theApp.m_bReBar )
	{
		bShow = 0;
	}

	if ( theApp.m_bLargeBar )
	{
		if ( bShow )
		{
			pLargeToolBar->ModifyStyle ( NULL, WS_VISIBLE );
			pToolBar->ModifyStyle ( WS_VISIBLE, NULL );
		}
		else
		{
			pLargeToolBar->ModifyStyle ( WS_VISIBLE, NULL );
			pToolBar->ModifyStyle ( WS_VISIBLE, NULL );
		}

		ShowControlBar ( pToolBar, FALSE, FALSE );
		ShowControlBar ( pLargeToolBar, bShow, FALSE );
	}
	else
	{
		if ( bShow )
		{
			pLargeToolBar->ModifyStyle ( WS_VISIBLE, NULL );
			pToolBar->ModifyStyle ( NULL, WS_VISIBLE );
		}
		else
		{
			pLargeToolBar->ModifyStyle ( WS_VISIBLE, NULL );
			pToolBar->ModifyStyle ( WS_VISIBLE, NULL );
		}

		ShowControlBar ( pToolBar, bShow, FALSE );
		ShowControlBar ( pLargeToolBar, FALSE, FALSE );
	}

	CRect		rectFrame;

	GetWindowRect ( &rectFrame );
	rectFrame.right--;
	MoveWindow (&rectFrame, TRUE);
	rectFrame.right++;
	MoveWindow (&rectFrame, TRUE);

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::ShowOwnReBar(	CMWToolBar *pToolBar, 
								CMWToolBar *pLargeToolBar, int bShow)
{

	if ( ! theApp.m_bReBar )
	{
		bShow = 0;
	}

	if ( theApp.m_bLargeBar )
	{
		if ( bShow  )
		{
			pLargeToolBar->ModifyStyle ( NULL, WS_VISIBLE );
			pToolBar->ModifyStyle ( WS_VISIBLE, NULL );
		}
		else
		{
			pLargeToolBar->ModifyStyle ( WS_VISIBLE, NULL );
			pToolBar->ModifyStyle ( WS_VISIBLE, NULL );
		}

		ShowControlBar ( pToolBar, FALSE, FALSE );
		ShowControlBar ( pLargeToolBar, bShow && theApp.m_bReBar, FALSE );
	}
	else
	{
		if ( bShow )
		{
			pLargeToolBar->ModifyStyle ( WS_VISIBLE, NULL );
			pToolBar->ModifyStyle ( NULL, WS_VISIBLE );
		}
		else
		{
			pLargeToolBar->ModifyStyle ( WS_VISIBLE, NULL );
			pToolBar->ModifyStyle ( WS_VISIBLE, NULL );
		}

		ShowControlBar ( pToolBar, bShow, FALSE );
		ShowControlBar ( pLargeToolBar, FALSE, FALSE );
	}

	CRect		rectFrame;

	GetWindowRect ( &rectFrame );
	rectFrame.right--;
	MoveWindow (&rectFrame, TRUE);
	rectFrame.right++;
	MoveWindow (&rectFrame, TRUE);

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
bool CMainFrame::IsMainBarVisible()
{
	if ( IsBarVisible ( &m_wndMainToolBar, &m_wndMainLargeToolBar ) )
	{
		return true;
	}

	return IsBarVisible ( &m_wndMainReBar, &m_wndMainLargeReBar );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
bool CMainFrame::IsBarVisible(CMWToolBar *pBar, CMWToolBar *pLargeBar)
{
	if ( pBar->GetStyle () & WS_VISIBLE )
	{
		return true;
	}

	if ( pLargeBar->GetStyle () & WS_VISIBLE )
	{
		return true;
	}

	return false;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
bool CMainFrame::IsIeBarVisible()
{
	if ( IsBarVisible ( &m_wndIeToolBar, &m_wndIeLargeToolBar ) )
	{
		return true;
	}

	return IsBarVisible ( &m_wndIeReBar, &m_wndIeLargeReBar );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
bool CMainFrame::IsMciBarVisible()
{
	if ( IsBarVisible ( &m_wndMciToolBar, &m_wndMciLargeToolBar ) )
	{
		return true;
	}

	return IsBarVisible ( &m_wndMciReBar, &m_wndMciLargeReBar );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
bool CMainFrame::IsSF2BarVisible()
{
	if ( IsBarVisible ( &m_wndSF2ToolBar, &m_wndSF2LargeToolBar ) )
	{
		return true;
	}

	return IsBarVisible ( &m_wndSF2ReBar, &m_wndSF2LargeReBar );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
bool CMainFrame::IsMidiBarVisible()
{
	if ( IsBarVisible ( &m_wndMidiToolBar, &m_wndMidiLargeToolBar ) )
	{
		return true;
	}

	return IsBarVisible ( &m_wndMidiReBar, &m_wndMidiLargeReBar );

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::DeleteLeftView()
{
	if ( theApp.m_bSplitterWindow )
	{
		m_wndSplitter.DeleteView ( 0, 0 );

		ResetLeftViewHandles ();
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::DeleteRightView()
{
	if ( theApp.m_bSplitterWindow )
	{
		m_wndSplitter.DeleteView ( 0, 1 );

		ResetRightViewHandles ();
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::CreateSplitterWindows(LPCREATESTRUCT lpcs, CCreateContext *pContext)
{
	//		Rectangle and Document
	CRect				rect;
	CSize				size;

	//		Reset handles
	ResetViewHandles();

	//		Set the Left Pane for this splitter
	GetClientRect(&rect);
	size		= rect.Size();
	size.cx		= ( size.cx * 9 ) / 20;

	//		Create the splitter
	if (! m_wndSplitter.CreateStatic(this,1,2, WS_CHILD | WS_VISIBLE ))
	{
		TRACE0 ( "Unable to create Splitter\n" );
		return false;
	}

	int iLeftView = theApp.GetProfileInt ( "Settings", "Left View", LEFT_VIEW_TRACKS );
	int iRightView = theApp.GetProfileInt ( "Settings", "Right View", RIGHT_VIEW_HEADER );

	switch ( iLeftView )
	{
		case LEFT_VIEW_EXPLORER :
		{
			if ( ! CreateColView ( 0, pContext, ( CView ** )( &m_LeftExplorerView ), 
									RUNTIME_CLASS(CMidiExplorerView), size ) )
			{
				return false;
			}

			break;
		}

		case LEFT_VIEW_TRACKS :
		default :
		{
			if ( ! CreateColView ( 0, pContext, ( CView ** )( &m_LeftTrackView ), 
									RUNTIME_CLASS(CTracksView), size ) )
			{
				return false;
			}

			break;
		}
	}

	//		Set The right Pane for this splitter
	switch ( iRightView )
	{
		case RIGHT_VIEW_EVENTS :
		{
			if ( ! CreateColView ( 1, pContext, ( CView ** )( &m_RightEventView ), 
									RUNTIME_CLASS(CEventView), CSize(0,0) ) )
			{
				return false;
			}

			break;
		}

		case RIGHT_VIEW_TEXT :
		{
			if ( ! CreateColView ( 1, pContext, ( CView ** )( &m_RightTextView ), 
									RUNTIME_CLASS(CMidiTextView), CSize(0,0) ) )
			{
				return false;
			}

			OnContentEventsText ( );

			break;
		}

		case RIGHT_VIEW_COPYRIGHT :
		{
			if ( ! CreateColView ( 1, pContext, ( CView ** )( &m_RightTextView ), 
									RUNTIME_CLASS(CMidiTextView), CSize(0,0) ) )
			{
				return false;
			}

			OnContentEventsCopyright ( );

			break;
		}

		case RIGHT_VIEW_CUEPOINT :
		{
			if ( ! CreateColView ( 1, pContext, ( CView ** )( &m_RightTextView ), 
									RUNTIME_CLASS(CMidiTextView), CSize(0,0) ) )
			{
				return false;
			}

			OnContentEventsCuepoint ( );

			break;
		}

		case RIGHT_VIEW_INSTRNAME :
		{
			if ( ! CreateColView ( 1, pContext, ( CView ** )( &m_RightTextView ), 
									RUNTIME_CLASS(CMidiTextView), CSize(0,0) ) )
			{
				return false;
			}

			OnContentEventsInstrumentname ( );

			break;
		}

		case RIGHT_VIEW_LYRICS :
		{
			if ( ! CreateColView ( 1, pContext, ( CView ** )( &m_RightTextView ), 
									RUNTIME_CLASS(CMidiTextView), CSize(0,0) ) )
			{
				return false;
			}

			OnContentEventsLyrics ( );

			break;
		}

		case RIGHT_VIEW_MARKERS :
		{
			if ( ! CreateColView ( 1, pContext, ( CView ** )( &m_RightTextView ), 
									RUNTIME_CLASS(CMidiTextView), CSize(0,0) ) )
			{
				return false;
			}

			OnContentEventsMarkers ( );

			break;
		}

		case RIGHT_VIEW_SEQUENCE :
		{
			if ( ! CreateColView ( 1, pContext, ( CView ** )( &m_RightTextView ), 
									RUNTIME_CLASS(CMidiTextView), CSize(0,0) ) )
			{
				return false;
			}

			OnContentEventsSequence ( );

			break;
		}

		case RIGHT_VIEW_SEQUENCER :
		{
			if ( ! CreateColView ( 1, pContext, ( CView ** )( &m_RightTextView ), 
									RUNTIME_CLASS(CMidiTextView), CSize(0,0) ) )
			{
				return false;
			}

			OnContentEventsSequencer ( );

			break;
		}

		case RIGHT_VIEW_TRACKNAME :
		{
			if ( ! CreateColView ( 1, pContext, ( CView ** )( &m_RightTextView ), 
									RUNTIME_CLASS(CMidiTextView), CSize(0,0) ) )
			{
				return false;
			}

			OnContentEventsTrackname ( );

			break;
		}

		case RIGHT_VIEW_PLAYER :
		{
			if ( ! CreateColView ( 1, pContext, ( CView ** )( &m_RightPlayerView ), 
									RUNTIME_CLASS(CMidiOwnPlayer), CSize(0,0) ) )
			{
				return false;
			}

			break;
		}

		case RIGHT_VIEW_ERRORS :
		{
			if ( ! CreateColView ( 1, pContext, ( CView ** )( &m_RightErrorView ), 
									RUNTIME_CLASS(CErrorView), CSize(0,0) ) )
			{
				return false;
			}

			break;
		}

		case RIGHT_VIEW_PLAYLIST :
		{
			if ( ! CreateColView ( 1, pContext, ( CView ** )( &m_RightPlayListView ), 
									RUNTIME_CLASS(CPlayList), CSize(0,0) ) )
			{
				return false;
			}

			break;
		}

		case RIGHT_VIEW_CHANNELS :
		{
			if ( ! CreateColView ( 1, pContext, ( CView ** )( &m_RightChannelView ), 
									RUNTIME_CLASS(CChannelView), CSize(0,0) ) )
			{
				return false;
			}

			break;
		}

#if 0
		case RIGHT_VIEW_BROWSER :
		{
			if ( ! CreateColView ( 1, pContext, ( CView ** )( &m_RightBrowserView ), 
									RUNTIME_CLASS(CMfcMsIeView), CSize(0,0) ) )
			{
				return false;
			}

			//		Start with the homepage url
			CString strUrl;
			strUrl.LoadString ( ID_HOMEPAGE_URL );
			m_RightBrowserView->Navigate2 ( strUrl );

			break;
		}
#endif

		case RIGHT_VIEW_STAFF :
		{
			if ( ! CreateColView ( 1, pContext, ( CView ** )( &m_RightStaffView ), 
									RUNTIME_CLASS(CStaffView), CSize(0,0) ) )
			{
				return false;
			}

			break;
		}

		case RIGHT_VIEW_NOTES :
		{
			if ( ! CreateColView ( 1, pContext, ( CView ** )( &m_RightNotesView ), 
									RUNTIME_CLASS(CNotesView), CSize(0,0) ) )
			{
				return false;
			}

			break;
		}

		case RIGHT_VIEW_HEADER :
		default :
		{
			if ( ! CreateColView ( 1, pContext, ( CView ** )( &m_RightGeneralView ), 
									RUNTIME_CLASS(CGeneralView), CSize(0,0) ) )
			{
				return false;
			}

			break;
		}
	}

	//		Activate the input view
	SetActiveCWndAsView(m_wndSplitter.GetPane(0,0));

	return true;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::CreateViewWindows(LPCREATESTRUCT lpcs, CCreateContext *pContext)
{
	BOOL			bRes;

	//		Track View
	bRes = 
		CreateOneView ( lpcs, pContext, "Track View", 
			( CView ** )( &m_LeftTrackView ),
			RUNTIME_CLASS(CTracksView) );
	if ( ! bRes )
	{
		return false;
	}

	//		Explorer View
	bRes = 
		CreateOneView ( lpcs, pContext, "Explorer View", 
			( CView ** )( &m_LeftExplorerView ),
			RUNTIME_CLASS(CMidiExplorerView) );
	if ( ! bRes )
	{
		return false;
	}

	//		Events View
	bRes = 
		CreateOneView ( lpcs, pContext, "Events View", 
			( CView ** )( &m_RightEventView ),
			RUNTIME_CLASS(CEventView) );
	if ( ! bRes )
	{
		return false;
	}

	//		Text View
	bRes = 
		CreateOneView ( lpcs, pContext, "Text View", 
			( CView ** )( &m_RightTextView ),
			RUNTIME_CLASS(CMidiTextView) );
	if ( ! bRes )
	{
		return false;
	}

	//		Player View
	bRes = 
		CreateOneView ( lpcs, pContext, "Own Player View", 
			( CView ** )( &m_RightPlayerView ),
			RUNTIME_CLASS(CMidiOwnPlayer) );
	if ( ! bRes )
	{
		return false;
	}

	//		Error View
	bRes = 
		CreateOneView ( lpcs, pContext, "Error View", 
			( CView ** )( &m_RightErrorView ),
			RUNTIME_CLASS(CErrorView) );
	if ( ! bRes )
	{
		return false;
	}

	//		Play List View
	bRes = 
		CreateOneView ( lpcs, pContext, "Play List View", 
			( CView ** )( &m_RightPlayListView ),
			RUNTIME_CLASS(CPlayList) );
	if ( ! bRes )
	{
		return false;
	}

	//		Channels View
	bRes = 
		CreateOneView ( lpcs, pContext, "Channel View", 
			( CView ** )( &m_RightChannelView ),
			RUNTIME_CLASS(CChannelView) );
	if ( ! bRes )
	{
		return false;
	}

#if 0
	//		Browser View
	bRes = 
		CreateOneView ( lpcs, pContext, "Browser View", 
			( CView ** )( &m_RightBrowserView ),
			RUNTIME_CLASS(CMfcMsIeView) );
	if ( ! bRes )
	{
		return false;
	}
#endif

	//		Staff View
	bRes = 
		CreateOneView ( lpcs, pContext, "Staff View", 
			( CView ** )( &m_RightStaffView ),
			RUNTIME_CLASS(CStaffView) );
	if ( ! bRes )
	{
		return false;
	}

	//		Release Notes View
	bRes = 
		CreateOneView ( lpcs, pContext, "Release Notes View", 
			( CView ** )( &m_RightNotesView ),
			RUNTIME_CLASS(CNotesView) );
	if ( ! bRes )
	{
		return false;
	}

	//		General View
	bRes = 
		CreateOneView ( lpcs, pContext, "General View", 
			( CView ** )( &m_RightGeneralView ),
			RUNTIME_CLASS(CGeneralView) );
	if ( ! bRes )
	{
		return false;
	}

	RestoreFrameGeometry ( m_pMidiView );

	return true;
}

void CMainFrame::DestroyViewWindows()
{
	if ( m_RightNotesView != NULL )
	{
		m_RightNotesView->DestroyWindow ( );
	}

	if ( m_RightStaffView != NULL )
	{
		m_RightStaffView->DestroyWindow ( );
	}

#if 0
	if ( m_RightBrowserView != NULL )
	{
		m_RightBrowserView->DestroyWindow ( );
	}
#endif

	if ( m_RightPlayListView != NULL )
	{
		m_RightPlayListView->DestroyWindow ( );
	}

	if ( m_RightPlayerView != NULL )
	{
		m_RightPlayerView->DestroyWindow ( );
	}

	if ( m_RightEventView != NULL )
	{
		m_RightEventView->DestroyWindow ( );
	}

	if ( m_RightChannelView != NULL )
	{
		m_RightChannelView->DestroyWindow ( );
	}

	if ( m_RightErrorView != NULL )
	{
		m_RightErrorView->DestroyWindow ( );
	}

	if ( m_RightGeneralView != NULL )
	{
		m_RightGeneralView->DestroyWindow ( );
	}

	if ( m_RightTextView != NULL )
	{
		m_RightTextView->DestroyWindow ( );
	}

	if ( m_LeftTrackView != NULL )
	{
		m_LeftTrackView->DestroyWindow ( );
	}

	if ( m_LeftExplorerView != NULL )
	{
		m_LeftExplorerView->DestroyWindow ( );
	}

	ResetViewHandles ( );

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnEditFind() 
{
	// TODO
	if ( m_RightEventView != NULL )
	{
		CFindEvent		dlgFind;
		
		if ( dlgFind.DoModal ( ) == IDOK )
		{
			m_RightEventView->SearchEvents ();
		}
	}	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnEditFindagain() 
{
	// TODO
	if ( m_RightEventView != NULL )
	{
		m_RightEventView->IncrementCurrentIndice();
		m_RightEventView->SearchEvents ();
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::SaveFrameGeometry( CWnd *pWnd )
{
	SaveViewGeometry ( pWnd, m_RightNotesView, "Notes Frame Geometry" );
	SaveViewGeometry ( pWnd, m_RightStaffView, "Staff Frame Geometry" );
	// SaveViewGeometry ( pWnd, m_RightBrowserView, "Browser Frame Geometry" );
	SaveViewGeometry ( pWnd, m_RightPlayListView, "PlayList Frame Geometry" );
	SaveViewGeometry ( pWnd, m_RightPlayerView, "Player Frame Geometry" );
	SaveViewGeometry ( pWnd, m_RightEventView, "Event Frame Geometry" );
	SaveViewGeometry ( pWnd, m_RightChannelView, "Channel Frame Geometry" );
	SaveViewGeometry ( pWnd, m_RightErrorView, "Error Frame Geometry" );
	SaveViewGeometry ( pWnd, m_RightGeneralView, "General Frame Geometry" );
	SaveViewGeometry ( pWnd, m_RightTextView, "Text Frame Geometry" );

	SaveViewGeometry ( pWnd, m_LeftTrackView, "Track Frame Geometry" );
	SaveViewGeometry ( pWnd, m_LeftExplorerView, "Explorer Frame Geometry" );

	//		Retrieve the order of windows
	CWnd		*pFirstChild;
	CWnd		*pChildWnd;
	int			iOrder = 0;

	pFirstChild = pWnd->GetWindow( GW_CHILD );
	if ( pFirstChild != NULL )
	{
		pChildWnd = pFirstChild->GetWindow( GW_HWNDFIRST );

		while ( pChildWnd != NULL && iOrder < MAX_VIEWS )
		{
			if ( pChildWnd == m_RightNotesView->GetParentFrame () )
			{
				theApp.WriteProfileInt ( "Notes Frame Geometry", "Order", iOrder++ );
			}

			if ( pChildWnd == m_RightStaffView->GetParentFrame () )
			{
				theApp.WriteProfileInt ( "Staff Frame Geometry", "Order", iOrder++ );
			}

#if 0
			if ( pChildWnd == m_RightBrowserView->GetParentFrame () )
			{
				theApp.WriteProfileInt ( "Browser Frame Geometry", "Order", iOrder++ );
			}
#endif

			if ( pChildWnd == m_RightPlayListView->GetParentFrame () )
			{
				theApp.WriteProfileInt ( "PlayList Frame Geometry", "Order", iOrder++ );
			}

			if ( pChildWnd == m_RightPlayerView->GetParentFrame () )
			{
				theApp.WriteProfileInt ( "Player Frame Geometry", "Order", iOrder++ );
			}

			if ( pChildWnd == m_RightEventView->GetParentFrame () )
			{
				theApp.WriteProfileInt ( "Event Frame Geometry", "Order", iOrder++ );
			}

			if ( pChildWnd == m_RightChannelView->GetParentFrame () )
			{
				theApp.WriteProfileInt ( "Channel Frame Geometry", "Order", iOrder++ );
			}

			if ( pChildWnd == m_RightErrorView->GetParentFrame () )
			{
				theApp.WriteProfileInt ( "Error Frame Geometry", "Order", iOrder++ );
			}

			if ( pChildWnd == m_RightGeneralView->GetParentFrame () )
			{
				theApp.WriteProfileInt ( "General Frame Geometry", "Order", iOrder++ );
			}

			if ( pChildWnd == m_RightTextView->GetParentFrame () )
			{
				theApp.WriteProfileInt ( "Text Frame Geometry", "Order", iOrder++ );
			}

			if ( pChildWnd == m_LeftTrackView->GetParentFrame () )
			{
				theApp.WriteProfileInt ( "Track Frame Geometry", "Order", iOrder++ );
			}

			if ( pChildWnd == m_LeftExplorerView->GetParentFrame () )
			{
				theApp.WriteProfileInt ( "Explorer Frame Geometry", "Order", iOrder++ );
			}

			pChildWnd = pChildWnd->GetNextWindow ( GW_HWNDNEXT );
		}
	}

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::RestoreFrameGeometry( CWnd *pWnd )
{
	//		Restore View Geometry
	RestoreViewGeometry ( pWnd, m_RightNotesView, "Notes Frame Geometry" );
	RestoreViewGeometry ( pWnd, m_RightStaffView, "Staff Frame Geometry" );
	// RestoreViewGeometry ( pWnd, m_RightBrowserView, "Browser Frame Geometry" );
	RestoreViewGeometry ( pWnd, m_RightPlayListView, "PlayList Frame Geometry" );
	RestoreViewGeometry ( pWnd, m_RightPlayerView, "Player Frame Geometry" );
	RestoreViewGeometry ( pWnd, m_RightEventView, "Event Frame Geometry" );
	RestoreViewGeometry ( pWnd, m_RightChannelView, "Channel Frame Geometry" );
	RestoreViewGeometry ( pWnd, m_RightErrorView, "Error Frame Geometry" );
	RestoreViewGeometry ( pWnd, m_RightGeneralView, "General Frame Geometry" );
	RestoreViewGeometry ( pWnd, m_RightTextView, "Text Frame Geometry" );

	RestoreViewGeometry ( pWnd, m_LeftTrackView, "Track Frame Geometry" );
	RestoreViewGeometry ( pWnd, m_LeftExplorerView, "Explorer Frame Geometry" );

	//		Now restore the z-order
	int			iX;
	for ( iX = MAX_VIEWS - 1; iX >= 0; iX-- )
	{
		RestoreViewOrder ( iX, m_RightNotesView, "Notes Frame Geometry" );
		RestoreViewOrder ( iX, m_RightStaffView, "Staff Frame Geometry" );
		// RestoreViewOrder ( iX, m_RightBrowserView, "Browser Frame Geometry" );
		RestoreViewOrder ( iX, m_RightPlayListView, "PlayList Frame Geometry" );
		RestoreViewOrder ( iX, m_RightPlayerView, "Player Frame Geometry" );
		RestoreViewOrder ( iX, m_RightEventView, "Event Frame Geometry" );
		RestoreViewOrder ( iX, m_RightChannelView, "Channel Frame Geometry" );
		RestoreViewOrder ( iX, m_RightErrorView, "Error Frame Geometry" );
		RestoreViewOrder ( iX, m_RightGeneralView, "General Frame Geometry" );
		RestoreViewOrder ( iX, m_RightTextView, "Text Frame Geometry" );

		RestoreViewOrder ( iX, m_LeftTrackView, "Track Frame Geometry" );
		RestoreViewOrder ( iX, m_LeftExplorerView, "Explorer Frame Geometry" );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnFileSavegeometry() 
{
	// TODO
	if ( ! theApp.m_bSplitterWindow )
	{
		//		Get the parent view
		SaveFrameGeometry( m_pMidiView );
	}

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::CreateOneView ( 
		LPCREATESTRUCT lpcs, CCreateContext *pContext,
		const char *pTitle, 
		CView **pView, CRuntimeClass *pClass )
{
	DWORD			dwStyle = 
		WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | 
		WS_MINIMIZEBOX | WS_MAXIMIZEBOX  | WS_OVERLAPPED |
		WS_CHILD | WS_CLIPSIBLINGS |
		WS_VISIBLE;

	DWORD			dwStyleView = 
		WS_OVERLAPPED | WS_CHILD | WS_VISIBLE;

	CCreateContext	tagContext;

	CFrameWnd		*pFrame;

	pFrame = new CFrameWnd ();

	pFrame->Create ( NULL, pTitle, dwStyle, rectDefault, m_pMidiView );

	//		Set the icon for the frame window
	HICON hDlgIcon = theApp.LoadIcon ( IDI_MIDI1 );
	pFrame->SetIcon( hDlgIcon, FALSE );

	//		Get the active document.
	if ( pContext != NULL )
	{
		tagContext = *pContext;
	}

	//		Fills context variables
	tagContext.m_pLastView			= NULL;
	tagContext.m_pNewDocTemplate	= NULL;
	tagContext.m_pNewViewClass		= pClass;

	//		Create the views
	tagContext.m_pCurrentFrame		= pFrame;

	//		Track View
	*pView = dynamic_cast< CView * >( pFrame->CreateView ( &tagContext ) );
	if ( pView == NULL )
	{
		return false;
	}

	( *pView )->ModifyStyle ( NULL, dwStyleView );

	CMenu	*pMenu;
	pMenu = pFrame->GetSystemMenu ( FALSE );
	if ( pMenu != NULL )
	{
		pMenu->EnableMenuItem ( SC_CLOSE, MF_BYCOMMAND | MF_GRAYED );
	}

	return true;

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::SetMidiView(CMidiView *pView)
{
	m_pMidiView	= pView;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMidiView * CMainFrame::GetMidiView()
{
	return m_pMidiView;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::RestoreViewGeometry(CWnd *pWnd, CView *pView, const char *pText)
{
	RECT		rectFrame;
	RECT		rectMain;

	int			iPos_x;
	int			iPos_y;
	int			iSiz_x;
	int			iSiz_y;

	int			iVisible;

	if ( pView == NULL )
	{
		return;
	}

	pWnd->GetWindowRect ( &rectMain );

	//		Get Client Dimension
	pView->GetParentFrame ()->GetWindowRect ( &rectFrame );
	iPos_x		= theApp.GetProfileInt ( pText, "Pos x", rectFrame.left - rectMain.left );
	iPos_y		= theApp.GetProfileInt ( pText, "Pos y", rectFrame.top - rectMain.top );
	iSiz_x		= theApp.GetProfileInt ( pText, "Size cx", rectFrame.right - rectFrame.left );
	iSiz_y		= theApp.GetProfileInt ( pText, "Size cy", rectFrame.bottom - rectFrame.top );
 	pView->GetParentFrame ()->MoveWindow ( iPos_x, iPos_y, iSiz_x, iSiz_y, TRUE );

	iVisible	= theApp.GetProfileInt ( pText, "Visible", 0 );
	if ( ! iVisible )
	{
		pView->GetParentFrame ()->ModifyStyle ( WS_VISIBLE, NULL );
		pView->ModifyStyle ( WS_VISIBLE, NULL );
	}

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::SaveViewGeometry(CWnd *pWnd, CView *pView, const char *pText)
{
	RECT		rectFrame;
	RECT		rectMain;

	if ( pView == NULL )
	{
		return;
	}

	pWnd->GetWindowRect ( &rectMain );
	rectMain.top	= 0;
	rectMain.left	= 0;

	pView->GetParentFrame ()->GetWindowRect ( &rectFrame );
	pWnd->ScreenToClient ( &rectFrame );
	theApp.WriteProfileInt ( pText, "Pos x", rectFrame.left - rectMain.left );
	theApp.WriteProfileInt ( pText, "Pos y", rectFrame.top - rectMain.top );
	theApp.WriteProfileInt ( pText, "Size cx", rectFrame.right - rectFrame.left );
	theApp.WriteProfileInt ( pText, "Size cy", rectFrame.bottom - rectFrame.top );

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::CreateColView ( int iCol, CCreateContext *pContext, 
									CView **pView, 
									CRuntimeClass *pClass, CSize size )
{
	if ( ! m_wndSplitter.CreateView(0,iCol,pClass,size,pContext))
	{
		TRACE0 ( "Unable to create View\n" );
		return false;
	}

	if ( iCol == 0 )
	{
		m_LeftView			= dynamic_cast< CView * >( m_wndSplitter.GetPane(0,iCol) ); 
	}
	else
	{
		m_RightView			= dynamic_cast< CView * >(  m_wndSplitter.GetPane(0,iCol) ); 
	}

	*pView	= dynamic_cast< CView * >( m_wndSplitter.GetPane(0,iCol) );

	return true;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CMainFrame::CreateColViewFull ( int iCol, CView **pView, 
							CRuntimeClass *pClass, int iView )
{
	if ( iCol == 0 )
	{
		DeleteLeftView();
	}
	else
	{
		DeleteRightView();
	}

	if ( ! CreateColView ( iCol, NULL, pView, pClass, CSize(0,0) ) )
	{
		return false;
	}

	RefreshFrame ();

	RecalcLayout();

	SetActiveCWndAsView(m_wndSplitter.GetPane(0,iCol));
		
	if ( iCol == 0 )
	{
		theApp.WriteProfileInt ( "Settings", "Left View", iView );
	}
	else
	{
		theApp.WriteProfileInt ( "Settings", "Right View", iView );
	}

	return true;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::ActivateOrHideView(CView *pView, const char *pText)
{
	if ( ! theApp.m_bSplitterWindow )
	{
		if ( pView->IsWindowVisible ( ) )
		{
			pView->GetParentFrame ( )->ModifyStyle ( WS_VISIBLE, NULL );
			pView->ModifyStyle ( WS_VISIBLE, NULL );
			theApp.WriteProfileInt ( pText, "Visible", 0 );
		}
		else
		{
			pView->GetParentFrame ( )->ModifyStyle ( NULL, WS_VISIBLE );
			pView->ModifyStyle ( NULL, WS_VISIBLE );
			SetActiveView ( pView );
			theApp.WriteProfileInt ( pText, "Visible", 1 );
		}

		Invalidate ( );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnContentEventsShowhide() 
{
	// TODO
	ActivateOrHideView(m_RightTextView, "Text Frame Geometry" );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::RestoreViewOrder ( int iX, CView *pView, const char *pText )
{
	int iOrder;

	if ( pView->GetStyle ( ) & WS_VISIBLE )
	{
		iOrder = theApp.GetProfileInt ( pText, "Order", MAX_VIEWS );
		if ( iOrder <= iX )
		{
			pView->GetParentFrame ()->BringWindowToTop ();
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::SetModified ( BOOL bFlag )
{
	CMidiDoc			*pMididoc;
	
	pMididoc = GetMidiDoc ();
	if ( pMididoc != NULL )
	{
		pMididoc->SetModifiedFlag ( bFlag );
	}

	if ( ! theApp.m_bSplitterWindow && m_pMidiView != NULL )
	{
		pMididoc = dynamic_cast< CMidiDoc * >( m_pMidiView->GetDocument () );
		if ( pMididoc != NULL )
		{
			pMididoc->SetModifiedFlag ( bFlag );
		}
	}

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState) 
{
	// TODO
	m_bPreview = ( bPreview != 0 );
	CFrameWnd::OnSetPreviewMode(bPreview, pState);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnFileRecord() 
{
	// TODO
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

    //
	if ( theApp.m_RecordThread == NULL )
	{
		theApp.m_MidiFileName = GetMidiFile ( )->GetMidiPathName ();
		theApp.m_RecordThread = AfxBeginThread ( RUNTIME_CLASS ( CRecordThread ) );
	}
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnInitMenu(CMenu* pMenu)
{
    CFrameWnd::OnInitMenu(pMenu);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
    CFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

    if ( bSysMenu )
    {
        static CMWMenu     sysMenu;
        //  Conflicts with Menu
        // m_pSysMenu = sysMenu.SetSystemMenu ( this, pPopupMenu );
    }
    else
    {
        m_pSysMenu  = NULL;
    }

    // TODO: ajoutez ici le code de votre gestionnaire de messages
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // TODO
	if ( lpDrawItemStruct != NULL && lpDrawItemStruct->CtlType == ODT_MENU )
	{
        if ( m_pSysMenu != NULL )
        {
            m_pSysMenu->DrawItem ( lpDrawItemStruct, TRUE );
            return;
        }
        else 
        {
            m_Menu.DrawItem ( lpDrawItemStruct );
            return;
        }
	}

    CFrameWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    // TODO
	if ( lpMeasureItemStruct != NULL && lpMeasureItemStruct->CtlType == ODT_MENU )
	{
        if ( m_pSysMenu != NULL )
        {
            m_pSysMenu->MeasureItem ( lpMeasureItemStruct, TRUE );
            return;
        }
        else 
        {
            m_Menu.MeasureItem ( lpMeasureItemStruct );
            return;
        }
	}

    CFrameWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMainFrame::OnUpdateFrameMenu(HMENU hMenuAlt)
{
    // TODO
    CFrameWnd::OnUpdateFrameMenu(hMenuAlt);
}
