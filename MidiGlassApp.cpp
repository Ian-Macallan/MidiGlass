// Midi Works.cpp : Defines the class behaviors for the application.
//
#include "stdafx.h"

#include "MidiGlassApp.h"

#include "MainFrm.h"
#include "MidiDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "GeneralView.h"
#include "TracksView.h"
#include "MidiExplorerView.h"
#include "MfcMsIeView.h"
#include "MidiView.h"

#include "AutomaticVersionHeader.h"

#include "strstr.h"

#include "CFluidSynth.h"

extern CMidiWorksApp theApp;

#pragma comment(lib, "Shlwapi.lib")

#define     EXCEPTION_ERROR_HANDLER
#include "ExceptionErrorHandler.h"
#include "PrintRoutine.h"

//
////////////////////////////////////////////////////////////////////////////////////////////
// CMidiWorksApp
//
////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMidiWorksApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

//
////////////////////////////////////////////////////////////////////////////////////////////
// CMidiWorksApp construction
//
////////////////////////////////////////////////////////////////////////////////////////////
CMidiWorksApp::CMidiWorksApp()
{
	// TODO
	// Place all significant initialization in InitInstance
	m_RecordThread				= NULL;

	m_PlayListThread			= NULL;

	m_PlayDirThread				= NULL;

	m_ModifyThread				= NULL;

	m_bSplitterWindow			= false;

	m_bUse_Sound_Font			= false;

}

//
////////////////////////////////////////////////////////////////////////////////////////////
// The one and only CMidiWorksApp object
//
////////////////////////////////////////////////////////////////////////////////////////////
CMidiWorksApp theApp;

//
////////////////////////////////////////////////////////////////////////////////////////////
// CMidiWorksApp initialization
//
////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMidiWorksApp::InitInstance()
{
    //
    if ( m_lpCmdLine != NULL )
    {
        if ( strncmp ( m_lpCmdLine, "-debug", strlen("-debug") ) == 0 )
        {
            DebugMode   = true;
            InvokeExceptionErrorHandler( );
        }
    }

    //
	AfxEnableControlContainer();

	// Initialize OLE 2.0 libraries
	AfxOleInit();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	// Enable3dControls();			// Call this when using MFC in a shared DLL
#else
//	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	//		Load Rich edit library
	BOOL bRes = AfxInitRichEdit( );

	// Change the registry key under which our settings are stored.
	// TODO
	// such as the name of your company or organization.
	SetRegistryKey(_T(PROGRAM_REGKEY));

	//
	//	To Write Settings
	GetModule ();

	LoadStdProfileSettings( 8 );  // Load standard INI file options (including MRU)

	//
	int iLoaded = CFluidSynth::InitDll(NULL);

	//		Load the background bitmap once
	m_bmBackGround.LoadBitmap ( IDB_BACKGROUND );
	m_bmBackEmpty.LoadBitmap ( IDB_BACK_EMPTY );

	m_brBackGround.CreatePatternBrush ( &m_bmBackGround );
	m_brBackEmpty.CreatePatternBrush ( &m_bmBackEmpty );
	m_brBackWhite.CreateSolidBrush ( 0xffffff );

	m_imgExplorerIcons16x14.Create ( IDB_EXPLORER_ICONS, 16, 14, CLR_NONE );
	m_imgImageListChannel16x14.Create ( IDB_CHANNEL_STATE, 16, 14, CLR_NONE );

	m_imgImageEvents16x32.Create ( IDB_EVENT_ICONS, 16, 32, CLR_NONE );
	m_imgImageEvents32x32.Create ( IDB_EVENT_ICONS32, 32, 32, CLR_NONE );

	m_imgImageList16x2.Create ( IDB_IMAGE_16X2, 16, 2, CLR_NONE );
	m_imgImageList32x2.Create ( IDB_IMAGE_32X2, 32, 2, CLR_NONE );
	m_imgImageList48x2.Create ( IDB_IMAGE_48X2, 48, 2, CLR_NONE );

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	m_pSplitterDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMidiDoc),
		RUNTIME_CLASS(CMainFrame),				// main SDI frame window
		RUNTIME_CLASS(CMidiView));				// No default view
	AddDocTemplate(m_pSplitterDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
    {
		return FALSE;
    }

    //
#if 0
	WORD wLanguageID = MAKELANGID(
		LANG_ENGLISH,		//	USHORT usPrimaryLanguage,  // primary language identifier
		SUBLANG_ENGLISH_US	//	USHORT usSubLanguage       // sublanguage identifier
	);

	//
	DWORD	dwLCID = MAKELCID(
	  wLanguageID,			//	WORD wLanguageID,  // language identifier
	  SORT_DEFAULT			//	WORD wSortID       // sorting identifier
	);

	//
	BOOL bSetted = SetThreadLocale ( 
		dwLCID		//	LCID Locale   // locale identifier
	);

    //
    LANGID oldLang =  SetThreadUILanguage( wLanguageID );
#endif


	//	We can say we accept files from drag and drop
	//	This will be enough to open file when drag it in my window
	m_pMainWnd->DragAcceptFiles ( TRUE );
	EnableShellOpen ( );
	RegisterShellFileTypes( TRUE );

	// The one and only window has been initialized, so show and update it.

	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	//
	return TRUE;
}

//
////////////////////////////////////////////////////////////////////////////////////////////
// App command to run the dialog
//
////////////////////////////////////////////////////////////////////////////////////////////
void CMidiWorksApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

//
////////////////////////////////////////////////////////////////////////////////////////////
// CMidiWorksApp message handlers
//
////////////////////////////////////////////////////////////////////////////////////////////

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
BOOL CMidiWorksApp::WriteProfileIniFile ( const char *section,  const char *keyName, const char *pValue )
{
	if ( StartsWith ( section, "Large ReBar" ) )
	{
		return FALSE;
	}

	BOOL bWritten = 
		WritePrivateProfileString (
			section,			//	__in  LPCTSTR lpAppName,
			keyName,			//	__in  LPCTSTR lpKeyName,
			pValue,				//	__in  LPCTSTR lpString,
			InitFileName		//	__in  LPCTSTR lpFileName
		);
	if ( bWritten <= 0 )
	{
		return FALSE;
	}

	return TRUE;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
BOOL CMidiWorksApp::WriteProfileInt ( const char *section,  const char *keyName, int value )
{
	char szText [ 64 ];
	sprintf_s ( szText, sizeof(szText), "%d", value );
	return WriteProfileIniFile ( section, keyName, szText );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
BOOL CMidiWorksApp::WriteProfileDouble ( const char *section, const char *keyName, double value )
{
	char szText [ 64 ];
	sprintf_s ( szText, sizeof(szText), "%f", value );
	return WriteProfileIniFile ( section, keyName, szText );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
BOOL CMidiWorksApp::WriteProfileString ( const char *section,  const char *keyName, const char *pValue )
{
	return WriteProfileIniFile ( section, keyName, pValue );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
BOOL CMidiWorksApp::ReadProfileIniFile ( const char *section, const char *keyName, char *pValue, size_t iValue, const char *pDefault )
{
	ZeroMemory ( pValue, iValue * sizeof(char) );

	if ( StartsWith ( section, "Large ReBar" ) )
	{
		return FALSE;
	}

	DWORD dwResult = 
		GetPrivateProfileString(
			section,						//	__in   LPCTSTR lpAppName,
			keyName,						//	__in   LPCTSTR lpKeyName,
			pDefault,						//	__in   LPCTSTR lpDefault,
			pValue,							//	__out  LPTSTR lpReturnedString,
			(DWORD) iValue,					//	__in   DWORD nSize,
			InitFileName					//	__in   LPCTSTR lpFileName
		);
	if ( dwResult <= 0 )
	{
		return FALSE;
	}

	return TRUE;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
UINT CMidiWorksApp::GetProfileInt ( const char *section,  const char *keyName, int defaultValue )
{
	UINT result = 0;
	char szText [ 64 ];
	char szDefault [ 64 ];
	sprintf_s ( szText, sizeof(szText), "%d", defaultValue );
	strcpy_s ( szDefault, szText );
	BOOL bRead = ReadProfileIniFile ( section, keyName, szText, sizeof(szText), szDefault );
	if ( ! bRead )
	{
		return defaultValue;
	}
	result = atoi ( szText );
	return result;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
double CMidiWorksApp::GetProfileDouble ( const char *section,  const char *keyName, double defaultValue )
{
	double result = 0;

	char szText [ 64 ];
	char szDefault [ 64 ];
	sprintf_s ( szText, sizeof(szText), "%f", defaultValue );
	strcpy_s ( szDefault, szText );
	BOOL bRead = ReadProfileIniFile ( section, keyName, szText, sizeof(szText), szDefault );
	if ( ! bRead )
	{
		return defaultValue;
	}
	result = atof ( szText );

	return result;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CString CMidiWorksApp::GetProfileString( const char *section,  const char *keyName, const char *pDefaultValue )
{
	if ( pDefaultValue == NULL )
	{
		pDefaultValue = "";
	}
	CString result;

	char szText [ MAX_PATH ];
	strcpy_s ( szText, sizeof(szText), pDefaultValue );
	BOOL bRead = ReadProfileIniFile ( section, keyName, szText, sizeof(szText), pDefaultValue );
	if ( ! bRead )
	{
		return pDefaultValue;
	}
	result = szText;

	return result;
}


//
////////////////////////////////////////////////////////////////////////
//		Search Init File
////////////////////////////////////////////////////////////////////////
bool CMidiWorksApp::SearchInitFile ( const char *pInitFileName, size_t iInitFileName )
{
	char	initName [ MAX_PATH ];
	char	initPathName [ MAX_PATH ];
	char	*pVariable		= NULL;
	size_t	requiredSize	= 0;

	strcpy_s ( initName, sizeof(initName), FindFileName ( pInitFileName ) );

	//
	//		First If ini file is here.
	if ( PathFileExists ( pInitFileName ) )
	{
		return true;
	}

	//
	//		Then Search Environnement
	getenv_s ( &requiredSize, NULL, 0, "PATH" );
	if ( requiredSize == 0 )
	{
		return false;
	}

	//
	size_t	iVariable	= ( requiredSize + 1 ) * sizeof(char) + 1;
	pVariable	= ( char * ) malloc ( iVariable );

	getenv_s ( &requiredSize, pVariable, requiredSize + 1, "PATH" );

	//		Treat Token for PATH
	char	strDelimit[]   = ";";
	char	*strToken	= NULL;
	char	*context	= NULL;

	//
	//		Treat Tokens
	strToken = strtok_s ( pVariable, strDelimit, &context);
	while( strToken != NULL )
	{
		//
		//		Test Filename
		strcpy_s ( initPathName, sizeof(initPathName), strToken );
		if ( ( strlen(initPathName) > 0 ) && ( initPathName [ strlen(initPathName) - 1 ] != '\\' ) )
		{
			strcat_s ( initPathName, sizeof(initPathName), "\\" );
		}
		strcat_s ( initPathName, sizeof(initPathName), initName );

		if ( PathFileExists ( initPathName ) )
		{
			strcpy_s ( InitFileName, iInitFileName, initPathName );
			free ( pVariable );
			return true;
		}

		//		Get next token: 
		strToken = strtok_s( NULL, strDelimit, &context);
	}

	free ( pVariable );

	return false;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CMidiWorksApp::GetModule ()
{
	DWORD dwResult = 
		GetModuleFileName ( NULL,						//  __in_opt  HMODULE hModule,
							ModuleFileName,				//	__out     LPTSTR lpFilename,
							sizeof(ModuleFileName)		//	__in      DWORD nSize
							);
	strcpy_s ( InitFileName, sizeof(InitFileName), ModuleFileName );
	RemoveFileType ( InitFileName );
    if ( EndsWithI ( InitFileName, "x64" ) || EndsWithI ( InitFileName, "x32" ) )
    {
        InitFileName [ strlen(InitFileName) - strlen("x64") ] = '\0';
    }

	strcat_s ( InitFileName, sizeof(InitFileName), ".ini" );

	//
	//		Search Init File and in Path
	SearchInitFile ( InitFileName, sizeof(InitFileName) );

	return;

}
