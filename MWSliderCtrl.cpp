// MWSliderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "MWSliderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMidiWorksApp theApp;

IMPLEMENT_DYNAMIC(CMWSliderCtrl, CSliderCtrl)

//
/////////////////////////////////////////////////////////////////////////////////////
// CMWSliderCtrl
//
/////////////////////////////////////////////////////////////////////////////////////
CMWSliderCtrl::CMWSliderCtrl()
{
	m_bSendNormal = false;
}

//
/////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////
CMWSliderCtrl::~CMWSliderCtrl()
{
}

//
/////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMWSliderCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(CMWSliderCtrl)
	ON_WM_CTLCOLOR()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////////////
// CMWSliderCtrl message handlers
//
/////////////////////////////////////////////////////////////////////////////////////
HBRUSH CMWSliderCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CSliderCtrl::OnCtlColor(pDC, pWnd, nCtlColor);
	
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
/////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////
void CMWSliderCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CSliderCtrl::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	CWnd	*wndParent	= GetParent ();

	if ( GetFocus ( ) != this && GetFocus ( ) != wndParent )
	{
		LONG  id		= GetWindowLong ( this->m_hWnd, GWL_ID  );
		WPARAM wParam	= MAKEWPARAM(id, 0);
		::PostMessage ( wndParent->m_hWnd, WM_MW_EDIT_END, wParam, ( LPARAM ) this->m_hWnd );
	}
	
}

//
/////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////
void CMWSliderCtrl::SetAnsiVarFont()
{
	::SendMessage ( this->m_hWnd, WM_SETFONT, ( WPARAM ) ( ::GetStockObject ( ANSI_VAR_FONT ) ), 1 );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWSliderCtrl::SetAnsiFixedFont()
{
	::SendMessage ( this->m_hWnd, WM_SETFONT, ( WPARAM ) ( ::GetStockObject ( ANSI_FIXED_FONT ) ), 1 );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWSliderCtrl::SendNormalMessage()
{
	m_bSendNormal	= true;
}

