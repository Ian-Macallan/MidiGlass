// ErrorView.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "ErrorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MidiDoc.h"
#include "MidiFile.h"
#include "MainFrm.h"

extern CMidiWorksApp theApp;

//
////////////////////////////////////////////////////////////////////////
// CErrorView
//
////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CErrorView, CMWFormView)

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CErrorView::CErrorView() : CMWFormView(CErrorView::IDD)
{
	//{{AFX_DATA_INIT(CErrorView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_bFirstTime = true;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CErrorView::~CErrorView()
{
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CErrorView::DoDataExchange(CDataExchange* pDX)
{
	CMWFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CErrorView)
	DDX_Control(pDX, IDC_ERROR_HEADER, m_Error_Header);
	DDX_Control(pDX, IDC_ERROR_MESSAGE, m_ErrorMessage);
	//}}AFX_DATA_MAP
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CErrorView, CMWFormView)
	//{{AFX_MSG_MAP(CErrorView)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
////////////////////////////////////////////////////////////////////////
// CErrorView diagnostics
//
////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CErrorView::AssertValid() const
{
	CMWFormView::AssertValid();
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CErrorView::Dump(CDumpContext& dc) const
{
	CMWFormView::Dump(dc);
}
#endif //_DEBUG

//
////////////////////////////////////////////////////////////////////////
// CErrorView message handlers
//
////////////////////////////////////////////////////////////////////////
HBRUSH CErrorView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CMWFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	if  ( pWnd != NULL && pWnd == &m_ErrorMessage )
	{
		CMWEdit *pEdit = dynamic_cast<CMWEdit *>(pWnd);
		if ( pEdit != NULL )
		{
			return pEdit->OnCtlColor( pDC, pWnd, nCtlColor );
		}
	}
	//
	HBRUSH hBrush = FriendCtlColor(pDC, pWnd, nCtlColor);
	if ( hBrush != NULL )
	{
		return hBrush;
	}

	//
	return hbr;
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CErrorView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	//
	if ( m_bFirstTime || ( lHint & RIGHT_MASK_ERRORS ) || lHint == 0 )
	{
		// m_ErrorMessage.SetTextAttributes ( white, black );
		m_ErrorMessage.SetAnsiFixedFont();
#if 1
		EnableScrollBarCtrl(SB_BOTH, FALSE);

		RECT	clientRect;
		GetClientRect ( &clientRect );

		if ( m_ErrorMessage.m_hWnd != NULL )
		{
			RECT	nameRect;
			m_Error_Header.GetWindowRect ( &nameRect );
			ScreenToClient ( &nameRect );
			clientRect.top = nameRect.bottom + 10;

			m_ErrorMessage.MoveWindow ( &clientRect, true );
		}
#endif
		m_bFirstTime = false;
		DisplayValues ();
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CErrorView::DisplayValues()
{
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;
	const char			*pString;

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		pString = pMidifile->GetFullErrorText ();
		if ( pString != NULL )
		{
			m_ErrorMessage.SetWindowText ( pString );
		}
		else
		{
			m_ErrorMessage.SetWindowText ( "" );
		}
	}
	else
	{
		m_ErrorMessage.SetWindowText ( "" );
	}
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
BOOL CErrorView::OnEraseBkgnd(CDC* pDC) 
{
	//
	
	BOOL bRes = FriendEraseBkgndScrollView(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

	return CMWFormView::OnEraseBkgnd ( pDC );
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CErrorView::OnInitialUpdate() 
{
	CMWFormView::OnInitialUpdate();
	
	//
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
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CErrorView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	//
	FriendActivate( bActivate, pActivateView, pDeactiveView, false);
	
	CMWFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CErrorView::OnSize(UINT nType, int cx, int cy)
{
	CMWFormView::OnSize(nType, cx, cy);

#if 1
	EnableScrollBarCtrl(SB_BOTH, FALSE);

	RECT	clientRect;
	GetClientRect ( &clientRect );

	if ( m_ErrorMessage.m_hWnd != NULL )
	{
		RECT	nameRect;
		m_Error_Header.GetWindowRect ( &nameRect );
		ScreenToClient ( &nameRect );
		clientRect.top = nameRect.bottom + 10;

		m_ErrorMessage.MoveWindow ( &clientRect, true );
	}
#endif
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
void CErrorView::OnMove(int x, int y)
{
	CMWFormView::OnMove(x, y);

#if 1
	EnableScrollBarCtrl(SB_BOTH, FALSE);

	RECT	clientRect;
	GetClientRect ( &clientRect );

	if ( m_ErrorMessage.m_hWnd != NULL )
	{
		RECT	nameRect;
		m_Error_Header.GetWindowRect ( &nameRect );
		ScreenToClient ( &nameRect );
		clientRect.top = nameRect.bottom + 10;
		m_ErrorMessage.MoveWindow ( &clientRect, true );
	}
#endif
}
