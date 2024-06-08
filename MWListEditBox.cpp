//
///////////////////////////////////////////////////////////////////////////////////
// MWListEditBox.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "MWListEditBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CMWListEditBox, CListBox)

//
///////////////////////////////////////////////////////////////////////////////////
// CMWListEditBox
//
///////////////////////////////////////////////////////////////////////////////////
CMWListEditBox::CMWListEditBox()
{
	m_bSendNormal = false;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMWListEditBox::~CMWListEditBox()
{
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMWListEditBox, CListBox)
	//{{AFX_MSG_MAP(CMWListEditBox)
	ON_CONTROL_REFLECT(LBN_KILLFOCUS, OnKillfocus)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
///////////////////////////////////////////////////////////////////////////////////
// CMWListEditBox message handlers
//
///////////////////////////////////////////////////////////////////////////////////
void CMWListEditBox::OnKillfocus() 
{
	//
	CWnd	*wndParent	= GetParent ();

	LONG  id		= GetWindowLong ( this->m_hWnd, GWL_ID  );
	WPARAM wParam	= MAKEWPARAM(id, 0);
	::PostMessage ( wndParent->m_hWnd, WM_MW_LISTEDITBOX_END, wParam, ( LPARAM ) this->m_hWnd );
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWListEditBox::SetAnsiVarFont()
{
	::SendMessage ( this->m_hWnd, WM_SETFONT, ( WPARAM ) ( ::GetStockObject ( ANSI_VAR_FONT ) ), 1 );

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWListEditBox::SetAnsiFixedFont()
{
	::SendMessage ( this->m_hWnd, WM_SETFONT, ( WPARAM ) ( ::GetStockObject ( ANSI_FIXED_FONT ) ), 1 );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
HBRUSH CMWListEditBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CListBox::OnCtlColor(pDC, pWnd, nCtlColor);
	
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
BOOL CMWListEditBox::OnEraseBkgnd(CDC* pDC) 
{
	//
	BOOL bRes = FriendEraseBkgnd(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

	return CListBox::OnEraseBkgnd(pDC);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWListEditBox::SendNormalMessage()
{
	m_bSendNormal	= true;
}

