//
///////////////////////////////////////////////////////////////////////////////////
// MWEdit.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "MWEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMidiWorksApp theApp;

IMPLEMENT_DYNAMIC(CMWEdit, CEdit)

//
///////////////////////////////////////////////////////////////////////////////////
// CMWEdit
//
///////////////////////////////////////////////////////////////////////////////////
CMWEdit::CMWEdit()
{
	m_bSendNormal	= false;
	m_foreground	= NULL;
	m_background	= NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMWEdit::~CMWEdit()
{
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMWEdit, CEdit)
	//{{AFX_MSG_MAP(CMWEdit)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
///////////////////////////////////////////////////////////////////////////////////
// CMWEdit message handlers
//
///////////////////////////////////////////////////////////////////////////////////
HBRUSH CMWEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CEdit::OnCtlColor(pDC, pWnd, nCtlColor);
	
	//
	if ( m_foreground != m_background )
	{
		pDC->SetTextColor(m_foreground);
		pDC->SetBkColor(m_background);
		pDC->SetDCBrushColor(m_background);
		HBRUSH hBrush = (HBRUSH) GetStockObject(DC_BRUSH); // return a DC brush.
		return hBrush;
	}
	else
	{
		HBRUSH hBrush = FriendCtlColor(pDC, pWnd, nCtlColor);
		if ( hBrush != NULL )
		{
			return hBrush;
		}
	}

	//
	return hbr;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWEdit::OnKillfocus() 
{
	//
	CWnd	*wndParent	= GetParent ();

	if ( GetFocus ( ) != this && GetFocus ( ) != wndParent )
	{
		LONG  id		= GetWindowLong ( this->m_hWnd, GWL_ID );
		WPARAM wParam	= MAKEWPARAM(id, 0);
		::PostMessage ( wndParent->m_hWnd, WM_MW_EDIT_END, wParam, ( LPARAM ) this->m_hWnd );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWEdit::SetAnsiVarFont()
{
	::SendMessage ( this->m_hWnd, WM_SETFONT, ( WPARAM ) ( ::GetStockObject ( ANSI_VAR_FONT ) ), 1 );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWEdit::SetAnsiFixedFont()
{
	::SendMessage ( this->m_hWnd, WM_SETFONT, ( WPARAM ) ( ::GetStockObject ( ANSI_FIXED_FONT ) ), 1 );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWEdit::SetTextAttributes(COLORREF foreground, COLORREF background)
{
	m_foreground	 = foreground;
	m_background	 = background;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CMWEdit::OnEraseBkgnd(CDC* pDC) 
{
	//
	if ( m_foreground != m_background )
	{
		pDC->SetBkColor(m_background);
		pDC->SetDCBrushColor(m_background);
		HBRUSH hBrush = (HBRUSH) GetStockObject(DC_BRUSH);
		return TRUE;
	}
	else
	{
		BOOL bRes = FriendEraseBkgnd(this, pDC);
		if ( bRes )
		{
			return bRes;
		}
	}
	return CEdit::OnEraseBkgnd(pDC);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWEdit::SendNormalMessage()
{
	m_bSendNormal	= true;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CMWEdit::PreTranslateMessage(MSG* pMsg)
{
	//

	if ( false && pMsg != NULL && pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB )
	{
		CWnd	*wndParent		= GetParent ();
		CWnd	*wndGrandParent	= NULL;
		if ( wndParent != NULL )
		{
			wndGrandParent = wndParent->GetParent ();
		}

		//
		CView *pView = dynamic_cast<CView *>( wndParent );
		if ( pView == NULL )
		{
			pView = dynamic_cast<CView *>( wndGrandParent );
		}

		LPARAM lParam	= (LPARAM) this->m_hWnd;				// Important that is is the handle of current

		if ( wndGrandParent != NULL )
		{
			::PostMessage ( wndGrandParent->m_hWnd, WM_MW_EDIT_END, 0, ( LPARAM ) wndParent->m_hWnd );
		}
		else if ( wndParent != NULL )
		{
			::PostMessage ( wndParent->m_hWnd, WM_MW_EDIT_END, 0, ( LPARAM ) this->m_hWnd );
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}
