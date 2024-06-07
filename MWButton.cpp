//
///////////////////////////////////////////////////////////////////////////////////
// MWButton.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "MWButton.h"
#include "Friend.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMidiWorksApp theApp;

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CMWButton, CButton)

//
///////////////////////////////////////////////////////////////////////////////////
// CMWButton
//
///////////////////////////////////////////////////////////////////////////////////
CMWButton::CMWButton()
{
	m_bSendNormal = false;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMWButton::~CMWButton()
{
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMWButton, CButton)
	//{{AFX_MSG_MAP(CMWButton)
	ON_WM_CTLCOLOR()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
///////////////////////////////////////////////////////////////////////////////////
// CMWButton message handlers
//
///////////////////////////////////////////////////////////////////////////////////
HBRUSH CMWButton::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CButton::OnCtlColor(pDC, pWnd, nCtlColor);
	
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
void CMWButton::OnKillFocus(CWnd* pNewWnd) 
{
	CButton::OnKillFocus(pNewWnd);
	
	// TODO
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
void CMWButton::SetAnsiVarFont()
{
	::SendMessage ( this->m_hWnd, WM_SETFONT, ( WPARAM ) ( ::GetStockObject ( ANSI_VAR_FONT ) ), 1 );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWButton::SetAnsiFixedFont()
{
	::SendMessage ( this->m_hWnd, WM_SETFONT, ( WPARAM ) ( ::GetStockObject ( ANSI_FIXED_FONT ) ), 1 );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CMWButton::OnEraseBkgnd(CDC* pDC) 
{
	// TODO
	BOOL bRes = FriendEraseBkgnd(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

	return CButton::OnEraseBkgnd(pDC);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWButton::SendNormalMessage()
{
	m_bSendNormal	= true;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // TODO
    UINT uStyle = DFCS_BUTTONPUSH;

    static char szText [ MAX_PATH ];

    CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    if ( pDC )
    {

        // This code only works with buttons.
        ASSERT ( lpDrawItemStruct->CtlType == ODT_BUTTON );

        // If drawing selected, add the pushed style to DrawFrameControl.
        if ( lpDrawItemStruct->itemState & ODS_SELECTED )
        {
            uStyle |= DFCS_PUSHED;
        }

        // Draw the button frame.
        // pDC->DrawEdge ( &lpDrawItemStruct->rcItem, EDGE_ETCHED, BF_ADJUST );
        pDC->DrawFrameControl ( &lpDrawItemStruct->rcItem, DFC_BUTTON, uStyle );
        RECT rect = lpDrawItemStruct->rcItem;
        rect.top        = rect.top + 1;
        rect.left       = rect.left + 1;
        rect.bottom     = rect.bottom - 2;
        rect.right      = rect.right - 2;

        CBrush *brush = CMWColors::GetBKNormalCBrush( CMWColors::m_iDarkTheme != 0 );
        pDC->FillRect ( &rect, brush );

        // Get the button's text.
        GetWindowText ( szText, sizeof(szText) );

        // Draw the button text using the text color white.
        COLORREF crOldColor;
        if ( lpDrawItemStruct->itemState & ODS_DISABLED )
        {
            crOldColor = pDC->SetTextColor ( CMWColors::GetFGDisabledCR(CMWColors::m_iDarkTheme != 0) );
        }
        else
        {
            crOldColor = pDC->SetTextColor ( CMWColors::GetFGNormalCR(CMWColors::m_iDarkTheme != 0)  );
        }

        pDC->DrawText ( szText, &lpDrawItemStruct->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER );

        pDC->SetTextColor( crOldColor);
    }

    // CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
