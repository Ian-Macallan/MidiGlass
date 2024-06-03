// MidiTextView.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "MidiTextView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MidiDoc.h"
#include "MidiFile.h"
#include "MainFrm.h"

extern CMidiWorksApp	theApp;

/////////////////////////////////////////////////////////////////////////////
// CMidiTextView

IMPLEMENT_DYNCREATE(CMidiTextView, CMWFormView)

CMidiTextView::CMidiTextView()
	: CMWFormView(CMidiTextView::IDD)
{
	//{{AFX_DATA_INIT(CMidiTextView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMidiTextView::~CMidiTextView()
{
}

void CMidiTextView::DoDataExchange(CDataExchange* pDX)
{
	CMWFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMidiTextView)
	DDX_Control(pDX, IDC_COMPACT, m_Compact);
	DDX_Control(pDX, IDC_SPIN_EVENT, m_Spin_Event);
	DDX_Control(pDX, IDC_TEXT, m_Text);
	DDX_Control(pDX, IDC_EVENTNAME, m_EventName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMidiTextView, CMWFormView)
	//{{AFX_MSG_MAP(CMidiTextView)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_EVENT, OnDeltaposSpinEvent)
	ON_BN_CLICKED(IDC_COMPACT, OnCompact)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_WM_MOVE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMidiTextView diagnostics

#ifdef _DEBUG
void CMidiTextView::AssertValid() const
{
	CMWFormView::AssertValid();
}

void CMidiTextView::Dump(CDumpContext& dc) const
{
	CMWFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMidiTextView message handlers

void CMidiTextView::DisplayValues()
{
	char				szString [ MAX_SMALL_STRING ];
	unsigned char		*pString;
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;
	unsigned short		iMask;

	iMask = MASK_TEXT;

	m_Spin_Event.SetRange ( MIDI_EVENT_SEQUENCE, MIDI_EVENT_SEQUENCER );

	m_Compact.SetCheck ( theApp.GetProfileInt ( "Settings", "Compact Text", 0 ) );

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
		iMask = pMididoc->GetEventMask ();
		switch ( iMask )
		{
			case MASK_SEQUENCE :
			{
				m_EventName.SetWindowText ( "Sequence Text" );
				break;
			}
			case MASK_TEXT :
			{
				m_EventName.SetWindowText ( "Text" );
				break;
			}
			case MASK_COPYRIGHT :
			{
				m_EventName.SetWindowText ( "Copyright Text" );
				break;
			}
			case MASK_TRACKNAME :
			{
				m_EventName.SetWindowText ( "Track Names" );
				break;
			}
			case MASK_INSTRNAME :
			{
				m_EventName.SetWindowText ( "Instrument names" );
				break;
			}
			case MASK_LYRICS :
			{
				m_EventName.SetWindowText ( "Lyrics" );
				break;
			}
			case MASK_MARKERS :
			{
				m_EventName.SetWindowText ( "Markers Text" );
				break;
			}
			case MASK_CUEPOINT :
			{
				m_EventName.SetWindowText ( "Cue Point" );
				break;
			}
			case MASK_SEQUENCER :
			{
				m_EventName.SetWindowText ( "Sequencer Text" );
				break;
			}
			default :
			{
				pMididoc->SetEventMask ( MASK_COPYRIGHT );
				m_EventName.SetWindowText ( "Copyright" );
				break;
			}
		}
	}

	if ( pMidifile != NULL )
	{
		pMidifile->ResetContext ();

		m_Text.SetWindowText ( "" );

		strcpy_s ( szString, sizeof ( szString ),"\r\n" );

		int iCount = 0;

		do
		{
			//		Get Copyright Text
			pString = 
				pMidifile->GetText ( iMask );

			if ( pString != NULL )
			{
				m_Text.SetSel ( -1, -1, true );
				m_Text.ReplaceSel ( ( char * ) pString );

				if (	! m_Compact.GetCheck ( ) 
					 || IsLineEmpty ( ( char * ) pString ) )
				{
					m_Text.SetSel ( -1, -1, true );
					m_Text.ReplaceSel ( szString );
				}
			}
			iCount++;
		}
		while ( pString != NULL && iCount < MAX_LINES );

	}
	else
	{
		m_Text.SetWindowText ( "" );
	}
	

}

void CMidiTextView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO
	if ( m_bFirstTime || ( lHint & RIGHT_MASK_TEXT ) || lHint == 0 )
	{
		m_bFirstTime = false;
		DisplayValues ();
	}
	
}

unsigned CMidiTextView::GetIDD()
{
	return ( IDD );
}

HBRUSH CMidiTextView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CMWFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO
	HBRUSH hBrush = FriendCtlColor(pDC, pWnd, nCtlColor);
	if ( hBrush != NULL )
	{
		return hBrush;
	}

	// TODO
	return hbr;
}

void CMidiTextView::OnDeltaposSpinEvent(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO
	CMidiDoc			*pMididoc;

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	if ( pMididoc != NULL )
	{
		int iPos = pNMUpDown->iPos + pNMUpDown->iDelta;
		if ( iPos < MIDI_EVENT_SEQUENCE )
		{
			iPos = MIDI_EVENT_SEQUENCER;
		}
	
		if ( iPos > MIDI_EVENT_SEQUENCER )
		{
			iPos = MIDI_EVENT_SEQUENCE;
		}

		switch ( iPos )
		{
			case MIDI_EVENT_SEQUENCE :
			{
				pMididoc->SetEventMask ( MASK_SEQUENCE );
				break;
			}
			case MIDI_EVENT_TEXT :
			{
				pMididoc->SetEventMask ( MASK_TEXT );
				break;
			}
			case MIDI_EVENT_COPYRIGHT :
			{
				pMididoc->SetEventMask ( MASK_COPYRIGHT );
				break;
			}
			case MIDI_EVENT_TRACKNAME :
			{
				pMididoc->SetEventMask ( MASK_TRACKNAME );
				break;
			}
			case MIDI_EVENT_INSTRNAME :
			{
				pMididoc->SetEventMask ( MASK_INSTRNAME );
				break;
			}
			case MIDI_EVENT_LYRICS :
			{
				pMididoc->SetEventMask ( MASK_LYRICS );
				break;
			}
			case MIDI_EVENT_MARKERS :
			{
				pMididoc->SetEventMask ( MASK_MARKERS );
				break;
			}
			case MIDI_EVENT_CUEPOINT :
			{
				pMididoc->SetEventMask ( MASK_CUEPOINT );
				break;
			}
			case MIDI_EVENT_SEQUENCER :
			{
				pMididoc->SetEventMask ( MASK_SEQUENCER );
				break;
			}
		}

		DisplayValues ( );
	}
	
	*pResult = 0;
}

bool CMidiTextView::IsLineEmpty(char *pText)
{
	while ( *pText )
	{
		if ( *pText != ' ' && *pText != '\n' && *pText != '\r' )
		{
			return false;
		}

		pText++;
	}

	return true;

}

void CMidiTextView::OnCompact() 
{
	// TODO
	if ( m_Compact.GetCheck ( ) )
	{
		theApp.WriteProfileInt ( "Settings", "Compact Text", 1 );
		DisplayValues ( );
	}
	else
	{
		theApp.WriteProfileInt ( "Settings", "Compact Text", 0 );
		DisplayValues ( );
	}
}

BOOL CMidiTextView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO
	
	BOOL bRes = FriendEraseBkgndScrollView(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

	return CMWFormView::OnEraseBkgnd ( pDC );
}

void CMidiTextView::OnInitialUpdate() 
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

void CMidiTextView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO
	FriendActivate( bActivate, pActivateView, pDeactiveView, false);
	
	CMWFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


void CMidiTextView::OnMove(int x, int y)
{
	CMWFormView::OnMove(x, y);

	RECT	clientRect;
	GetClientRect ( &clientRect );

#if 0
	EnableScrollBarCtrl(SB_BOTH, FALSE);

	if ( m_Rich_Edit.m_hWnd != NULL )
	{
		m_Rich_Edit.MoveWindow ( &clientRect, true );
	}
#endif
}


void CMidiTextView::OnSize(UINT nType, int cx, int cy)
{
	CMWFormView::OnSize(nType, cx, cy);

#if 0
	EnableScrollBarCtrl(SB_BOTH, FALSE);

	RECT	clientRect;
	GetClientRect ( &clientRect );

	if ( m_Rich_Edit.m_hWnd != NULL )
	{
		m_Rich_Edit.MoveWindow ( &clientRect, true );
	}
#endif
}
