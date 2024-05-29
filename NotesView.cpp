//
///////////////////////////////////////////////////////////////////////////////////
// NotesView.cpp : implementation file
//
//
///////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MidiGlassApp.h"
#include "NotesView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMidiWorksApp theApp;

//		The Ritch text edit callback function
static long				iPosInBuffer;

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
DWORD CALLBACK NotesStreamCallback(  
	DWORD_PTR dwCookie,			// application-defined value
	LPBYTE pbBuff,			// pointer to a buffer
	LONG cb,				// number of bytes to read or write
	LONG *pcb				// pointer to number of bytes transferred
  );


DWORD CALLBACK NotesStreamCallback(  
	DWORD_PTR dwCookie,			// application-defined value
	LPBYTE pbBuff,			// pointer to a buffer
	LONG cb,				// number of bytes to read or write
	LONG *pcb				// pointer to number of bytes transferred
  )
{
	long					iSize;

	//		First reset bytes read from.
	*pcb		= 0;

	//		Load resource and so on
	HRSRC hRes = FindResource ( NULL, ( char * ) IDR_MIDIRTF1, "MIDIRTF" );

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
			}


			//		Here we need to do the job with the buffer
			iSize = dwSize - iPosInBuffer;
			if ( iSize > 0 )
			{
				*pcb = min ( iSize, cb );
				memcpy ( pbBuff, pBuffer + iPosInBuffer, *pcb ); 
				iPosInBuffer += *pcb;
			};

			if ( *pcb == 0 )
			{
				iPosInBuffer = 0;
			}

			//		Then free the buffer
			free ( pBuffer );
		}
	}
	
	return 0;
}
  
//
///////////////////////////////////////////////////////////////////////////////////
// CNotesView
//
///////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CNotesView, CMWFormView)

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CNotesView::CNotesView() : CMWFormView(CNotesView::IDD)
{
	//{{AFX_DATA_INIT(CNotesView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CNotesView::~CNotesView()
{
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CNotesView::DoDataExchange(CDataExchange* pDX)
{
	CMWFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNotesView)
	DDX_Control(pDX, IDC_RICHEDIT_TEXT, m_Rich_Edit);
	//}}AFX_DATA_MAP
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CNotesView, CMWFormView)
	//{{AFX_MSG_MAP(CNotesView)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_MOVE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
///////////////////////////////////////////////////////////////////////////////////
// CNotesView diagnostics
//
///////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CNotesView::AssertValid() const
{
	CMWFormView::AssertValid();
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CNotesView::Dump(CDumpContext& dc) const
{
	CMWFormView::Dump(dc);
}
#endif //_DEBUG

//
///////////////////////////////////////////////////////////////////////////////////
// CNotesView message handlers
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CNotesView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bRes = FriendEraseBkgndScrollView(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

	return CMWFormView::OnEraseBkgnd ( pDC );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CNotesView::OnInitialUpdate() 
{
	CMWFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	if ( ! theApp.m_bSplitterWindow )
	{
		GetParentFrame ( )->RecalcLayout ( );
		if ( GetParentFrame ( ) != dynamic_cast< CMainFrame * >( AfxGetMainWnd() ) )
		{
			ResizeParentToFit ( FALSE );
			ResizeParentToFit ( TRUE );
		}
	}

	EDITSTREAM		editStream;

	editStream.dwCookie		= 0;
	editStream.dwError		= 0; 
    editStream.pfnCallback	= NotesStreamCallback;

	m_Rich_Edit.SetSel( 0, -1 );

	iPosInBuffer = 0;

	long iNbBytes = m_Rich_Edit.StreamIn ( SF_RTF | SFF_SELECTION , editStream );

	m_Rich_Edit.SetSel( 0, 0 );

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CNotesView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	EDITSTREAM		editStream;

	editStream.dwCookie		= 0;
	editStream.dwError		= 0; 
    editStream.pfnCallback	= NotesStreamCallback;

	m_Rich_Edit.SetSel( 0, -1 );

	iPosInBuffer = 0;

	long iNbBytes = m_Rich_Edit.StreamIn ( SF_RTF | SFF_SELECTION , editStream );

	m_Rich_Edit.SetSel( 0, 0 );

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
HBRUSH CNotesView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CMWFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	HBRUSH hBrush = FriendCtlColor(pDC, pWnd, nCtlColor);
	if ( hBrush != NULL )
	{
		return hBrush;
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CNotesView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	FriendActivate( bActivate, pActivateView, pDeactiveView, false);
	
	CMWFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CNotesView::OnMove(int x, int y)
{
	CMWFormView::OnMove(x, y);

#if 1
	EnableScrollBarCtrl(SB_BOTH, FALSE);

	RECT	clientRect;
	GetClientRect ( &clientRect );

	if ( m_Rich_Edit.m_hWnd != NULL )
	{
		m_Rich_Edit.MoveWindow ( &clientRect, true );
	}
#endif
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CNotesView::OnSize(UINT nType, int cx, int cy)
{
	CMWFormView::OnSize(nType, cx, cy);

#if 1
	EnableScrollBarCtrl(SB_BOTH, FALSE);

	RECT	clientRect;
	GetClientRect ( &clientRect );

	if ( m_Rich_Edit.m_hWnd != NULL )
	{
		m_Rich_Edit.MoveWindow ( &clientRect, true );
	}
#endif
}
