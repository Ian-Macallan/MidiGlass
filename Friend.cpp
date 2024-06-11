#include "StdAfx.h"
#include "AfxWin.h"
#include "MidiGlassApp.h"
#include "Friend.h"
#include "MWColors.h"

extern CMidiWorksApp theApp;

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
HBRUSH FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    return CMWColors::OnCtlColor ( pDC, pWnd, nCtlColor );
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
BOOL FriendEraseBkgnd(CWnd* pWnd, CDC* pDC) 
{
    return CMWColors::OnEraseBkgnd ( pDC, pWnd );
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
BOOL FriendEraseBkgndScrollView(CScrollView *pWnd, CDC* pDC) 
{
    return CMWColors::OnEraseBkgnd ( pDC, pWnd );
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void FriendActivate ( BOOL bActivate, CView* pActivateView, CView* pDeactiveView, bool bForce ) 
{
	if ( ! theApp.m_bSplitterWindow )
	{
		CMainFrame	*mainFrame = dynamic_cast< CMainFrame * >( AfxGetMainWnd( ) );
		if ( mainFrame == NULL )
		{
			return;
		}

		CView		*pView;

		pView = mainFrame->GetActiveView ( );

		if ( bActivate )
		{
			if ( pActivateView->GetParentFrame ( ) != mainFrame )
			{
				pActivateView->GetParentFrame ( )->BringWindowToTop ( );
				 if ( pActivateView != pDeactiveView || bForce )
				 {
					mainFrame->SetActiveView ( pActivateView, FALSE );
				 }
			}
		}
	}
}