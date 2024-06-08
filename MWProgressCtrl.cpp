//
///////////////////////////////////////////////////////////////////////////////////
// MWProgressCtrl.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "MWProgressCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMidiWorksApp theApp;

IMPLEMENT_DYNAMIC(CMWProgressCtrl, CProgressCtrl)

//
///////////////////////////////////////////////////////////////////////////////////
// CMWProgressCtrl
//
///////////////////////////////////////////////////////////////////////////////////
CMWProgressCtrl::CMWProgressCtrl()
{
	m_bSendNormal = false;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMWProgressCtrl::~CMWProgressCtrl()
{
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMWProgressCtrl, CProgressCtrl)
	//{{AFX_MSG_MAP(CMWProgressCtrl)
	ON_WM_CTLCOLOR()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
///////////////////////////////////////////////////////////////////////////////////
// CMWProgressCtrl message handlers
//
///////////////////////////////////////////////////////////////////////////////////
HBRUSH CMWProgressCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CProgressCtrl::OnCtlColor(pDC, pWnd, nCtlColor);
	
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
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWProgressCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CProgressCtrl::OnKillFocus(pNewWnd);
	
	//
	CWnd	*wndParent	= GetParent ();

	if ( GetFocus ( ) != this && GetFocus ( ) != wndParent )
	{
		LONG  id		= GetWindowLong ( this->m_hWnd, GWL_ID  );
		WPARAM wParam	= MAKEWPARAM(id, 0);
		::PostMessage ( wndParent->m_hWnd, WM_MW_EDIT_END, wParam, ( LPARAM ) this->m_hWnd );
	}
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWProgressCtrl::SetAnsiVarFont()
{
	::SendMessage ( this->m_hWnd, WM_SETFONT, ( WPARAM ) ( ::GetStockObject ( ANSI_VAR_FONT ) ), 1 );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWProgressCtrl::SetAnsiFixedFont()
{
	::SendMessage ( this->m_hWnd, WM_SETFONT, ( WPARAM ) ( ::GetStockObject ( ANSI_FIXED_FONT ) ), 1 );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CMWProgressCtrl::OnEraseBkgnd(CDC* pDC) 
{
	//
	BOOL bRes = FriendEraseBkgnd(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

	return CProgressCtrl::OnEraseBkgnd(pDC);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWProgressCtrl::SendNormalMessage()
{
	m_bSendNormal	= true;
}

