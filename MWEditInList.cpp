//
///////////////////////////////////////////////////////////////////////////////////
// MWEditInList.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "MWEditInList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CMWEditInList, CMWEdit)

//
///////////////////////////////////////////////////////////////////////////////////
// CMWEditInList
//
///////////////////////////////////////////////////////////////////////////////////
CMWEditInList::CMWEditInList() : CMWEdit()
{
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMWEditInList::~CMWEditInList()
{
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMWEditInList, CMWEdit)
	//{{AFX_MSG_MAP(CMWEditInList)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
///////////////////////////////////////////////////////////////////////////////////
// CMWEditInList message handlers
//
///////////////////////////////////////////////////////////////////////////////////
void CMWEditInList::OnKillfocus() 
{
	// TODO: Add your control notification handler code here
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
void CMWEditInList::SetAnsiVarFont()
{
	::SendMessage ( this->m_hWnd, WM_SETFONT, ( WPARAM ) ( ::GetStockObject ( ANSI_VAR_FONT ) ), 1 );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWEditInList::SetAnsiFixedFont()
{
	::SendMessage ( this->m_hWnd, WM_SETFONT, ( WPARAM ) ( ::GetStockObject ( ANSI_FIXED_FONT ) ), 1 );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
HBRUSH CMWEditInList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	//	Call Directly CEdit
	HBRUSH hbr = CEdit::OnCtlColor(pDC, pWnd, nCtlColor);
	
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
BOOL CMWEditInList::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bRes = FriendEraseBkgnd(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

	//	Call Directly CEdit
	return CEdit::OnEraseBkgnd(pDC);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CMWEditInList::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ajoutez ici votre code spécialisé et/ou l'appel de la classe de base

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

	//	Call Directly CEdit
	return CEdit::PreTranslateMessage(pMsg);
}
