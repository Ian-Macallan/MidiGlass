#include "stdafx.h"

#include "MidiGlassApp.h"

#include "MainFrm.h"
#include "MidiDoc.h"

#include "AutomaticVersionHeader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "GeneralView.h"
#include "TracksView.h"

extern CMidiWorksApp theApp;

//
static int iMidiFileIndex = 1;

//
/////////////////////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
//
/////////////////////////////////////////////////////////////////////////////////////////////
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT

	m_pMidiFile = NULL;
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_INTERNAL_NAME, m_Internal_Name);
	DDX_Control(pDX, IDC_COMMENTS, m_Comments);
	DDX_Control(pDX, IDC_PRODUCT_NAME, m_Product_Name);
	DDX_Control(pDX, IDC_TUNE, m_Tune);
	DDX_Control(pDX, IDC_PRODUCT_VERSION, m_Product_Version);
	DDX_Control(pDX, IDC_LEGAL_COPYRIGHT, m_Legal_Copyright);
	DDX_Control(pDX, IDC_LEGAL_TRADEMARK, m_Legal_Trademark);
	DDX_Control(pDX, IDOK, m_Ok);
	//}}AFX_DATA_MAP
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_INTERNAL_NAME, OnInternalName)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_TUNE, OnTune)
	ON_WM_DROPFILES()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_COMMENTS, OnComments)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
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
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO
	DWORD			dwLength;
	DWORD			dwHandle;
	char			szFilename [ MAX_PATHNAME_STRING ];
	char			szText [ MAX_PATHNAME_STRING ];
	char			*pData;
	BOOL			bResult;
	DWORD			dwResult;
	void			*pBufferInfo;
	UINT			iBufferLen;

	//		Get module name
	dwResult = 
		GetModuleFileName(	NULL,
							szFilename,
							sizeof ( szFilename ) );
 
	//		Get Size to allocate for info
	dwLength = GetFileVersionInfoSize ( szFilename, &dwHandle );
	if ( dwLength > 0 )
	{
		pData = new char [ dwLength ];

		bResult = 
			GetFileVersionInfo(szFilename, dwHandle, dwLength, pData );
		if ( bResult )
		{
			bResult = VerQueryValue ( pData, "\\", &pBufferInfo, &iBufferLen );

			bResult = VerQueryValue ( pData, "\\StringFileInfo\\040904b0\\LegalCopyright", &pBufferInfo, &iBufferLen );
			if ( bResult )
			{
				memset ( szText, 0, sizeof ( szText ) );
				memcpy ( szText, pBufferInfo, iBufferLen );
				m_Legal_Copyright.SetWindowText ( szText );
			}

			bResult = VerQueryValue ( pData, "\\StringFileInfo\\040904b0\\InternalName", &pBufferInfo, &iBufferLen );
			if ( bResult )
			{
				memset ( szText, 0, sizeof ( szText ) );
				memcpy ( szText, pBufferInfo, iBufferLen );
				m_Internal_Name.SetWindowText ( szText );
			}

			bResult = VerQueryValue ( pData, "\\StringFileInfo\\040904b0\\LegalTrademarks", &pBufferInfo, &iBufferLen );
			if ( bResult )
			{
				memset ( szText, 0, sizeof ( szText ) );
				memcpy ( szText, pBufferInfo, iBufferLen );
				m_Legal_Trademark.SetWindowText ( szText );
			}

			bResult = VerQueryValue ( pData, "\\StringFileInfo\\040904b0\\Comments", &pBufferInfo, &iBufferLen );
			if ( bResult )
			{
				memset ( szText, 0, sizeof ( szText ) );
				memcpy ( szText, pBufferInfo, iBufferLen );
				m_Comments.SetWindowText ( szText );
			}

			bResult = VerQueryValue ( pData, "\\StringFileInfo\\040904b0\\ProductName", &pBufferInfo, &iBufferLen );
			if ( bResult )
			{
				memset ( szText, 0, sizeof ( szText ) );
				memcpy ( szText, pBufferInfo, iBufferLen );
				m_Product_Name.SetWindowText ( szText );
			}

			bResult = VerQueryValue ( pData, "\\StringFileInfo\\040904b0\\ProductVersion", &pBufferInfo, &iBufferLen );
			if ( bResult )
			{
				memset ( szText, 0, sizeof ( szText ) );
				memcpy ( szText, pBufferInfo, iBufferLen );
				m_Product_Version.SetWindowText ( szText );
			}
		}

		delete pData;

	}
	
	theApp.m_bPlay_Tune_In_AboutBox = theApp.GetProfileInt ( "Settings", "Play Tune In About Box", 1 );
	theApp.m_iMidiDevice = theApp.GetProfileInt ( "Settings", "Midi Device", 0 );

	//	Start playing a tune
	if ( theApp.m_bPlay_Tune_In_AboutBox )
	{
		m_Tune.EnableWindow ( FALSE );
		m_Tune.ModifyStyle ( WS_VISIBLE, NULL );
		OnTune ( );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CAboutDlg::OnInternalName() 
{
	// TODO
	HINSTANCE			hInst;
	CString				strUrl;

	strUrl.LoadString ( ID_HOMEPAGE_URL );
	hInst = ShellExecute ( NULL, "open", strUrl, "", "", SW_SHOWDEFAULT );
	
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CAboutDlg::OnOK() 
{
	// TODO
	CDialog::OnOK();
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CAboutDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO
	if ( m_pMidiFile != NULL )
	{
		m_pMidiFile->LLStop ();
		delete m_pMidiFile;
		m_pMidiFile = NULL;
	}

}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CAboutDlg::OnTune() 
{
	// TODO

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

    //
	HRSRC hRes = NULL;
    if ( iMidiFileIndex % 2 == 0 )
    {
        hRes = FindResource ( NULL, ( char * ) IDR_MIDIFILE1, "MIDIFILE" );
    }
    else
    {
        hRes = FindResource ( NULL, ( char * ) IDR_MIDIFILE2, "MIDIFILE" );
    }
    iMidiFileIndex++;

    //
	if ( hRes != NULL )
	{
		char		*pBuffer;

		DWORD dwSize = SizeofResource ( NULL, hRes );
		if ( dwSize > 0 )
		{
			pBuffer = ( char * ) malloc ( dwSize );

			HGLOBAL hResData = LoadResource ( NULL, hRes );
			if ( hResData != NULL )
			{
				char		*pResData = ( char * ) LockResource ( hResData );

				memcpy ( pBuffer, pResData, dwSize );

				//		Now read it in memory
				m_pMidiFile = new CMidiFile ( pBuffer, dwSize );
				m_pMidiFile->LLSetSkippingMode( theApp.m_bSkip_To_Note == 1 );
				m_pMidiFile->SetMidiDevice ( theApp.m_iMidiDevice );
				m_pMidiFile->SetSoundFont ( theApp.m_SoundFontFile, theApp.m_bUse_Sound_Font );
				m_pMidiFile->LLPlay ( );
			}


			free ( pBuffer );
		}
	}
	
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CAboutDlg::OnDropFiles(HDROP hDropInfo) 
{
	// TODO
	char			szFilename [ MAX_PATHNAME_STRING ];

	//		First get the count of files
	UINT iCount = 
		DragQueryFile ( hDropInfo, 0xFFFFFFFF, 
						szFilename, sizeof ( szFilename ) );
	if ( iCount >= 0 )
	{
		UINT iRes = 
			DragQueryFile ( hDropInfo, 0, szFilename, sizeof ( szFilename ) );
		
		//		Now play the file
		if ( m_pMidiFile != NULL )
		{
			m_pMidiFile->LLStop ();
			delete m_pMidiFile;
			m_pMidiFile = NULL;
		}

		//		Now read it in memory
		m_pMidiFile = new CMidiFile ( szFilename );
		m_pMidiFile->LLSetSkippingMode( theApp.m_bSkip_To_Note == 1 );
		m_pMidiFile->SetMidiDevice ( theApp.m_iMidiDevice );
		m_pMidiFile->SetSoundFont ( theApp.m_SoundFontFile, theApp.m_bUse_Sound_Font );
		m_pMidiFile->LLPlay ( );
	}

	CDialog::OnDropFiles(hDropInfo);
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CAboutDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO
	BOOL bRes = FriendEraseBkgnd(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

	return CDialog::OnEraseBkgnd ( pDC );
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CAboutDlg::OnComments() 
{
	// TODO
	HINSTANCE			hInst;
	CString				strUrl;

	m_Comments.GetWindowText ( strUrl );
	hInst = 
		ShellExecute ( NULL, "open", strUrl, "", "", SW_SHOWDEFAULT );
	
}
