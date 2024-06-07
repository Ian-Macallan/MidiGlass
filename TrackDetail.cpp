// TrackDetail.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "TrackDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMidiWorksApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTrackDetail dialog


CTrackDetail::CTrackDetail(CWnd* pParent /*=NULL*/) : CMWDialog(CTrackDetail::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrackDetail)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTrackDetail::DoDataExchange(CDataExchange* pDX)
{
	CMWDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrackDetail)
	DDX_Control(pDX, IDOK, m_Ok);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_TRACK_SIZE, m_Track_Size);
	DDX_Control(pDX, IDC_TRACK_NUMBER, m_Track_Number);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrackDetail, CMWDialog)
	//{{AFX_MSG_MAP(CTrackDetail)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
// CTrackDetail message handlers
//
/////////////////////////////////////////////////////////////////////////////


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CTrackDetail::OnInitDialog() 
{
	CMWDialog::OnInitDialog();
	
	// TODO
	CMainFrame			*pMainFrame;
	char				szString [ MAX_INT_STRING ];
	CMidiTrack			*pTrack;

	pMainFrame = ( CMainFrame * ) GetParent();
	pMainFrame = dynamic_cast< CMainFrame * >( AfxGetMainWnd( ) );

	//		Fills the combo text.
	pTrack = pMainFrame->GetCurrentTrack ();

	if ( pTrack != NULL )
	{
		sprintf_s ( szString, sizeof ( szString ), "%03d", pTrack->GetTrackNumber ( ) );
		m_Track_Number.SetWindowText ( szString );
		sprintf_s ( szString, sizeof ( szString ), "%06d", pTrack->GetTrackLength ( ) );
		m_Track_Size.SetWindowText ( szString );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

