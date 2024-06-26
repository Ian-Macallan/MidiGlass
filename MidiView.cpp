// MidiView.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "MidiView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
///////////////////////////////////////////////////////////////////////////////////
// CMidiView
//
///////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMidiView, CView)

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMidiView::CMidiView()
{
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMidiView::~CMidiView()
{
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMidiView, CView)
	//{{AFX_MSG_MAP(CMidiView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_MOVE()
	ON_WM_SIZE()
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//
///////////////////////////////////////////////////////////////////////////////////
// CMidiView drawing
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiView::OnDraw(CDC* pDC)
{
	CMidiDoc *pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
}

//
///////////////////////////////////////////////////////////////////////////////////
// CMidiView diagnostics
//
///////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CMidiView::AssertValid() const
{
	CView::AssertValid();
}

void CMidiView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

//
///////////////////////////////////////////////////////////////////////////////////
// CMidiView message handlers
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	//
	CMainFrame			*hMainFrame;

	hMainFrame = dynamic_cast< CMainFrame * >( AfxGetMainWnd() );

	if ( hMainFrame->GetMidiView () == NULL )
	{
		hMainFrame->SetMidiView ( this );

		//		Now create all view
		CCreateContext	tagContext;

		//		Get the active document.
		tagContext.m_pCurrentDoc		= GetDocument ( );

		//		Fills context variables
		tagContext.m_pLastView			= NULL;
		tagContext.m_pNewDocTemplate	= NULL;
		tagContext.m_pNewViewClass		= NULL;

		//		Create the views
		tagContext.m_pCurrentFrame		= NULL;

		hMainFrame->CreateViewWindows ( NULL, &tagContext );
	}	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	//
	
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiView::OnMove(int x, int y)
{
	CView::OnMove(x, y);

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

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

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

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CMidiView::OnEraseBkgnd(CDC* pDC)
{
    // TODO: ajoutez ici le code de votre gestionnaire de messages et/ou les param�tres par d�faut des appels
	BOOL bRes = FriendEraseBkgnd(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

    return CView::OnEraseBkgnd(pDC);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
HBRUSH CMidiView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CView::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Modifier ici les attributs du DC
	HBRUSH hBrush = FriendCtlColor(pDC, pWnd, nCtlColor);
	if ( hBrush != NULL )
	{
		return hBrush;
	}


    // TODO:  Retourner un autre pinceau si le pinceau par d�faut n'est pas souhait�
    return hbr;
}
