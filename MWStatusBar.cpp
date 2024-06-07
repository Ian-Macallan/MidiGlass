//
///////////////////////////////////////////////////////////////////////////////////
// MWStatusBar.cpp: implementation of the CMWStatusBar class.
//
///////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "MWStatusBar.h"
#include "Friend.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CMidiWorksApp theApp;

//
///////////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//
///////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CMWStatusBar, CStatusBar)

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMWStatusBar, CStatusBar)
	//{{AFX_MSG_MAP(CMWStatusBar)
	//}}AFX_MSG_MAP
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
    ON_WM_DRAWITEM()
    ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMWStatusBar::CMWStatusBar()
{
	m_bSendNormal = false;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMWStatusBar::~CMWStatusBar()
{

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWStatusBar::SetAnsiVarFont()
{
	::SendMessage ( this->m_hWnd, WM_SETFONT, ( WPARAM ) ( ::GetStockObject ( ANSI_VAR_FONT ) ), 1 );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWStatusBar::SetAnsiFixedFont()
{
	::SendMessage ( this->m_hWnd, WM_SETFONT, ( WPARAM ) ( ::GetStockObject ( ANSI_FIXED_FONT ) ), 1 );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWStatusBar::SendNormalMessage()
{
	m_bSendNormal	= true;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CMWStatusBar::OnEraseBkgnd(CDC* pDC)
{
    // TODO: ajoutez ici le code de votre gestionnaire de messages et/ou les paramètres par défaut des appels
	BOOL bRes = FriendEraseBkgnd(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

    return CStatusBar::OnEraseBkgnd(pDC);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
HBRUSH CMWStatusBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CStatusBar::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO
	HBRUSH hBrush = FriendCtlColor(pDC, pWnd, nCtlColor);
	if ( hBrush != NULL )
	{
		return hBrush;
	}

    // TODO:  Retourner un autre pinceau si le pinceau par défaut n'est pas souhaité
    return hbr;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWStatusBar::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    // TODO
    CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    if ( pDC )
    {
        pDC->SetTextColor ( CMWColors::GetFGNormalCR(CMWColors::m_iDarkTheme != 0) );
        pDC->SetBkColor ( CMWColors::GetBKNormalCR(CMWColors::m_iDarkTheme != 0) );
    }
    CStatusBar::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
HBRUSH CMWStatusBar::CtlColor(CDC* pDC, UINT nCtlColor)
{
    // TODO:  Modifier ici les attributs du DC
    // TODO
	HBRUSH hBrush = FriendCtlColor(pDC, this, nCtlColor);
	if ( hBrush != NULL )
	{
		return hBrush;
	}

    // TODO:  Retourner un pinceau non NULL si le gestionnaire du parent ne doit pas être appelé
    return NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWStatusBar::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct )
{
    static char szText [ MAX_PATH ];

    // TODO:  Ajoutez votre code pour dessiner l'élément spécifié
    // CStatusBar::DrawItem ( lpDrawItemStruct );
    CDC *pDC = CDC::FromHandle ( lpDrawItemStruct->hDC );
    if ( lpDrawItemStruct != NULL && pDC )
    {
        RECT rect       = lpDrawItemStruct->rcItem;
        rect.top        = rect.top;
        rect.left       = rect.left;
        rect.bottom     = rect.bottom;
        rect.right      = rect.right;

        CBrush *brush = CMWColors::GetBKNormalCBrush( CMWColors::m_iDarkTheme != 0 );
        pDC->FillRect ( &rect, brush );

        COLORREF crOldColor;
        COLORREF crOldBKColor;
        if ( lpDrawItemStruct->itemState & ODS_DISABLED )
        {
            crOldBKColor    = pDC->SetBkColor ( CMWColors::GetBKNormalCR(CMWColors::m_iDarkTheme != 0) );
            crOldColor      = pDC->SetTextColor ( CMWColors::GetFGDisabledCR(CMWColors::m_iDarkTheme != 0) );
        }
        else
        {
            crOldBKColor    = pDC->SetBkColor ( CMWColors::GetBKNormalCR(CMWColors::m_iDarkTheme != 0) );
            crOldColor      = pDC->SetTextColor ( CMWColors::GetFGNormalCR(CMWColors::m_iDarkTheme != 0)  );
        }

        CString paneString = GetPaneText ( lpDrawItemStruct->itemID );
        pDC->DrawText ( paneString, &lpDrawItemStruct->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER );

        pDC->SetBkColor( crOldBKColor);
        pDC->SetTextColor( crOldColor);
    }
}
