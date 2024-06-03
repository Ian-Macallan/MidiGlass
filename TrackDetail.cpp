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


CTrackDetail::CTrackDetail(CWnd* pParent /*=NULL*/)
	: CDialog(CTrackDetail::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrackDetail)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTrackDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrackDetail)
	DDX_Control(pDX, IDOK, m_Ok);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_TRACK_SIZE, m_Track_Size);
	DDX_Control(pDX, IDC_TRACK_NUMBER, m_Track_Number);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrackDetail, CDialog)
	//{{AFX_MSG_MAP(CTrackDetail)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackDetail message handlers

HBRUSH CTrackDetail::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

BOOL CTrackDetail::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
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

BOOL CTrackDetail::OnEraseBkgnd(CDC* pDC) 
{
	// TODO
	
	BOOL bRes = FriendEraseBkgnd(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

	return CDialog::OnEraseBkgnd ( pDC );
}
