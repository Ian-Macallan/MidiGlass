#include "StdAfx.h"
#include "AfxWin.h"
#include "MidiGlassApp.h"
#include "Friend.h"

extern CMidiWorksApp theApp;

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
HBRUSH FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr;

	if ( ! theApp.m_bOwnControlColor )
	{
		return NULL;
	}

	switch ( nCtlColor )
	{
        case CTLCOLOR_MSGBOX :
        {
			hbr = ( HBRUSH ) theApp.m_brBackEmpty;
            pDC->SetTextColor ( white0xff );
            pDC->SetBkColor ( black0x20 );
			pDC->SetBkMode ( TRANSPARENT );
			break;
        }
		case CTLCOLOR_EDIT :
		{
			hbr = ( HBRUSH ) theApp.m_brBackEmpty;
            pDC->SetTextColor ( white0xff );
            pDC->SetBkColor ( black0x20 );
			pDC->SetBkMode ( TRANSPARENT );
			break;
		}

        case CTLCOLOR_LISTBOX :
        {
			hbr = ( HBRUSH ) theApp.m_brBackEmpty;
            pDC->SetTextColor ( white0xff );
            pDC->SetBkColor ( black0x20 );
			pDC->SetBkMode ( TRANSPARENT );
			break;
        }

        case CTLCOLOR_BTN :
        {
			hbr = ( HBRUSH ) theApp.m_brBackEmpty;
            pDC->SetTextColor ( white0xff );
            pDC->SetBkColor ( black0x20 );
			pDC->SetBkMode ( TRANSPARENT );
			break;
        }

        case CTLCOLOR_DLG :
        {
			hbr = ( HBRUSH ) theApp.m_brBackEmpty;
            pDC->SetTextColor ( white0xff );
            pDC->SetBkColor ( black0x20 );
			pDC->SetBkMode ( TRANSPARENT );
			break;
        }

        case CTLCOLOR_STATIC :
		{
			hbr = ( HBRUSH ) theApp.m_brBackEmpty;
            pDC->SetTextColor ( white0xff );
            pDC->SetBkColor ( black0x20 );
			// pDC->SetBkMode ( TRANSPARENT );
			break;
		}

		default:
		{
			hbr = ( HBRUSH ) theApp.m_brBackEmpty;
            pDC->SetTextColor ( white0xff );
            pDC->SetBkColor ( black0x20 );
			pDC->SetBkMode ( TRANSPARENT );
			break;
		}
	}

	return hbr;
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
BOOL FriendEraseBkgnd(CWnd* pWnd, CDC* pDC) 
{
	// TODO
	CRect				rectFrame;
	CRect				rectBitmap;
	CSize				sizeBitmap;
	BITMAP				bmInfo;
	CDC					memDC;

	if ( ! theApp.m_bOwnBackground )
	{
		return FALSE;
	}

	//		Create the compatible DC
	memDC.CreateCompatibleDC ( NULL );

	//		Select Bitmap Object
	memDC.SelectObject ( &theApp.m_bmBackGround );

	//		Get bitmap Dimension
	theApp.m_bmBackGround.GetBitmap ( &bmInfo );
	sizeBitmap.cx		= bmInfo.bmWidth;
	sizeBitmap.cy		= bmInfo.bmHeight;

	//		Set Bitmap Rect
	rectBitmap.top		= 0;
	rectBitmap.left		= 0;
	rectBitmap.right	= bmInfo.bmWidth;
	rectBitmap.bottom	= bmInfo.bmHeight;

	//		Get windows size
	pWnd->GetClientRect(&rectFrame);

	//		Now Fill The Rectangle
	//	pDC->FillRect ( &rectFrame, &theApp.m_brBackGround );

	//		Strech the bitmap
	pDC->StretchBlt (	
		0, 0, 
		rectFrame.right, 
		rectFrame.bottom,
		&memDC,
		0, 0, rectBitmap.right, rectBitmap.bottom,
		SRCCOPY );

	memDC.DeleteDC ( );

	return TRUE;
	
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
BOOL FriendEraseBkgndScrollView(CScrollView *pWnd, CDC* pDC) 
{
	// TODO
	CRect				rectFrame;
	CRect				rectBitmap;
	CSize				sizeBitmap;
	BITMAP				bmInfo;
	CDC					memDC;

	CSize				sizeScrollView;
	CPoint				pointScrollView;
	CDC					memDCView;
	CBitmap				bmView;

	if ( ! theApp.m_bOwnBackground )
	{
		return FALSE;
	}

	//		Get the scroll view size and position.
	sizeScrollView = pWnd->GetTotalSize ( );
	pointScrollView	= pWnd->GetScrollPosition ( );

	//		Get bitmap Dimension
	theApp.m_bmBackGround.GetBitmap ( &bmInfo );
	sizeBitmap.cx		= bmInfo.bmWidth;
	sizeBitmap.cy		= bmInfo.bmHeight;

	//		Set Bitmap Rect
	rectBitmap.top		= 0;
	rectBitmap.left		= 0;
	rectBitmap.right	= bmInfo.bmWidth;
	rectBitmap.bottom	= bmInfo.bmHeight;

	//		Get windows size
	pWnd->GetClientRect(&rectFrame);


	//		Create the compatible DC
	memDC.CreateCompatibleDC ( NULL );

	//		Immediatly Select Bitmap Object
	memDC.SelectObject ( &theApp.m_bmBackGround );

	//		Create the View compatible DC
	memDCView.CreateCompatibleDC ( &memDC );

	//		Immediatly Select Bitmap Object
	// memDCView.SelectObject ( &theApp.m_bmBackGround );

	//		Create a compatible bitmap
	//		Important to create the bitmap in the normal DC
	bmView.CreateCompatibleBitmap ( 
		pDC, 
		max ( sizeScrollView.cx, rectFrame.right ),
		max ( sizeScrollView.cy, rectFrame.bottom ) );

	//		Immediatly Select Bitmap Object
	memDCView.SelectObject ( bmView );

	memDCView.SetStretchBltMode( STRETCH_DELETESCANS );

	//		Strech the bitmap into the memory view DC
	memDCView.StretchBlt (	
		0, 0, 
		max ( sizeScrollView.cx, rectFrame.right ),
		max ( sizeScrollView.cy, rectFrame.bottom ),
		&memDC,
		0, 0, 
		rectBitmap.right, rectBitmap.bottom,
		SRCCOPY );


	//		Now Copy only the rectangle associated to
	//		the view
	pDC->BitBlt (
		0, 0, 
		rectFrame.right, rectFrame.bottom, 
		&memDCView, 
		pointScrollView.x, pointScrollView.y, 
		SRCCOPY );

	memDC.DeleteDC ();
	memDCView.DeleteDC ();

	return TRUE;
	
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