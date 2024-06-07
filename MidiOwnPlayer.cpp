// MidiOwnPlayer.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "MidiOwnPlayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MidiDoc.h"
#include "MainFrm.h"

#define	SLIDE_SECONDS		30

extern CMidiWorksApp theApp;

//
///////////////////////////////////////////////////////////////////////////////////
// CMidiOwnPlayer
//
///////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMidiOwnPlayer, CMWFormView)

CMidiOwnPlayer::CMidiOwnPlayer()
	: CMWFormView(CMidiOwnPlayer::IDD)
{
	//{{AFX_DATA_INIT(CMidiOwnPlayer)
	//}}AFX_DATA_INIT
	m_bFirstTime	= true;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMidiOwnPlayer::~CMidiOwnPlayer()
{
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiOwnPlayer::DoDataExchange(CDataExchange* pDX)
{
	CMWFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMidiOwnPlayer)
	DDX_Control(pDX, IDC_SLIDER_SPEED, m_Speed);
	DDX_Control(pDX, IDC_SLIDER, m_Slider);
	DDX_Control(pDX, IDC_STATE, m_State);
	DDX_Control(pDX, IDC_MAXIMUM, m_Maximum);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Control(pDX, IDC_TIME, m_Time);
	//}}AFX_DATA_MAP
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMidiOwnPlayer, CMWFormView)
	//{{AFX_MSG_MAP(CMidiOwnPlayer)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER, OnReleasedcaptureSlider)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_SPEED, OnReleasedcaptureSliderSpeed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
///////////////////////////////////////////////////////////////////////////////////
// CMidiOwnPlayer diagnostics
//
///////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CMidiOwnPlayer::AssertValid() const
{
	CMWFormView::AssertValid();
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiOwnPlayer::Dump(CDumpContext& dc) const
{
	CMWFormView::Dump(dc);
}
#endif //_DEBUG

//
///////////////////////////////////////////////////////////////////////////////////
// CMidiOwnPlayer message handlers
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiOwnPlayer::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO
	if ( m_bFirstTime || ( lHint & RIGHT_MASK_PLAYER ) || lHint == 0 )
	{
		m_bFirstTime = false;
		DisplayValues ();
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiOwnPlayer::DisplayValues()
{
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;

	unsigned long		iHours;
	unsigned long		iMinutes;
	unsigned long		iSeconds;
	unsigned long		iMillisecs;

	unsigned long		iTime;

	char				szString [ MAX_HOUR_STRING ];

	// check that one item gets the focus
	pMidifile	= NULL;
	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		if ( pMidifile->LLIsPlaying ( ) )
		{
			SetTimer ( MIDI_TIMER, 500, NULL );
		}
	}

	if ( pMidifile != NULL )
	{
		m_Progress.SetRange32 ( 0, pMidifile->GetMaximumHMSm () / 1000 );

		int		iMin, iMax;
		m_Slider.GetRange ( iMin, iMax );
		if (    iMin != 0 
			 || iMax != ( int) ( pMidifile->GetMaximumHMSm () / 1000 ) )
		{
			m_Slider.SetRange ( 0, pMidifile->GetMaximumHMSm () / 1000 );
			m_Slider.SetPos ( 0 );
		}

		m_Speed.GetRange ( iMin, iMax );
		if ( iMin != 0 || iMax != 100 )
		{
			m_Speed.SetRange ( 0, 100 );
			m_Speed.SetPos ( 0 );
		}

		//		Set the maximum
		m_Progress.SetRange32 ( 0, pMidifile->GetMaximumHMSm () / 1000 );
		if ( pMidifile->LLIsPlaying () )
		{
			m_State.SetWindowText ( "Playing" );
		}

		if ( pMidifile->LLIsPaused () )
		{
			m_State.SetWindowText ( "Paused" );
		}

		if ( pMidifile->LLIsStopped () )
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
	if ( pMidifile != NULL )
	{
		iTime = pMidifile->LLGetTime ();
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

	sprintf_s ( szString, sizeof ( szString ), "%02d:%02d:%02d",
				iHours, iMinutes, iSeconds );

	m_Time.SetWindowText ( szString );

	//		The set the maximum
	iTime = 0xffffffff;
	if ( pMidifile != NULL )
	{
		iTime = pMidifile->GetMaximumHMSm ();
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

	sprintf_s ( szString, sizeof ( szString ), "%02d:%02d:%02d", iHours, iMinutes, iSeconds );

	m_Maximum.SetWindowText ( szString );

	//		The set the maximum
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiOwnPlayer::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO
	DisplayValues ();

	CMWFormView::OnTimer(nIDEvent);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiOwnPlayer::OnReleasedcaptureSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;

	pMidifile	= NULL;
	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		pMidifile->LLSlide ( m_Slider.GetPos () );
	}

	//		Start a Timer
	SetTimer ( MIDI_TIMER, 500, NULL );

	DisplayValues ();

	*pResult = 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiOwnPlayer::OnReleasedcaptureSliderSpeed(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO
	
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;

	pMidifile	= NULL;
	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		pMidifile->LLSetSpeed ( m_Speed.GetPos () );
	}

	*pResult = 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiOwnPlayer::OnInitialUpdate() 
{
	CMWFormView::OnInitialUpdate();
	
	// TODO
	if ( ! theApp.m_bSplitterWindow )
	{
		GetParentFrame ( )->RecalcLayout ( );
		if ( GetParentFrame ( ) != dynamic_cast< CMainFrame * >( AfxGetMainWnd() ) )
		{
			ResizeParentToFit ( FALSE );
			ResizeParentToFit ( TRUE );
		}
	}
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiOwnPlayer::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO
	FriendActivate( bActivate, pActivateView, pDeactiveView, false);
	
	CMWFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


