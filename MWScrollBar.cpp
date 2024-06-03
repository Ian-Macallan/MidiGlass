// MWScrollBar.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "MWScrollBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CMWScrollBar, CScrollBar)

/////////////////////////////////////////////////////////////////////////////
// CMWScrollBar

CMWScrollBar::CMWScrollBar()
{
	m_bSendNormal = false;
}

//
/////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////
CMWScrollBar::~CMWScrollBar()
{
}

//
/////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMWScrollBar, CScrollBar)
	//{{AFX_MSG_MAP(CMWScrollBar)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////////////
// CMWScrollBar message handlers
//
/////////////////////////////////////////////////////////////////////////////////////
void CMWScrollBar::SetAnsiVarFont()
{
	::SendMessage ( this->m_hWnd, WM_SETFONT, ( WPARAM ) ( ::GetStockObject ( ANSI_VAR_FONT ) ), 1 );

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWScrollBar::SetAnsiFixedFont()
{
	::SendMessage ( this->m_hWnd, WM_SETFONT, ( WPARAM ) ( ::GetStockObject ( ANSI_FIXED_FONT ) ), 1 );
}

//
/////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////
HBRUSH CMWScrollBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CScrollBar::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO
	HBRUSH hBrush = FriendCtlColor(pDC, pWnd, nCtlColor);
	if ( hBrush != NULL )
	{
		return hBrush;
	}

	// TODO
	return hbr;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWScrollBar::SendNormalMessage()
{
	m_bSendNormal	= true;
}

