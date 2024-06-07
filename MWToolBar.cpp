//
///////////////////////////////////////////////////////////////////////////////////
// MWToolBar.cpp: implementation of the CMWToolBar class.
//
///////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "MWToolBar.h"
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

IMPLEMENT_DYNAMIC(CMWToolBar, CToolBar)

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMWToolBar, CToolBar)
	//{{AFX_MSG_MAP(CMWToolBar)
	//}}AFX_MSG_MAP
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMWToolBar::OnNMCustomdraw)
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMWToolBar::CMWToolBar()
{
	m_pFont			= NULL;
	m_bSendNormal	= false;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMWToolBar::~CMWToolBar()
{
	if ( m_pFont != NULL )
	{
		delete m_pFont;
		m_pFont = NULL;
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWToolBar::SetAnsiVarFont()
{
	CFont		*pFont;
	LOGFONT		logFont;

	pFont = GetFont ( );
	pFont->GetLogFont ( & logFont );

	if ( m_pFont != NULL )
	{
		delete m_pFont;
		m_pFont = NULL;
	}

	// logFont.lfWeight = FW_BOLD;
	m_pFont = new CFont ( );
	m_pFont->CreateFontIndirect ( &logFont );

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWToolBar::SetAnsiFixedFont()
{
	CFont		*pFont;
	LOGFONT		logFont;

	pFont = GetFont ( );
	pFont->GetLogFont ( & logFont );

	if ( m_pFont != NULL )
	{
		delete m_pFont;
		m_pFont = NULL;
	}

	// logFont.lfWeight = FW_BOLD;
	m_pFont = new CFont ( );
	m_pFont->CreateFontIndirect ( &logFont );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWToolBar::SendNormalMessage()
{
	m_bSendNormal	= true;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWToolBar::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO: ajoutez ici le code de votre gestionnaire de notification de contrôle

    NMCUSTOMDRAW* pCD = (NMCUSTOMDRAW*)pNMHDR;

    switch (pCD->dwDrawStage)
    {
        case CDDS_PREPAINT :
        {
            *pResult = CDRF_NOTIFYITEMDRAW;
            return;
        }

        //
        case CDDS_ITEMPREPAINT :
        {
            RECT    rectTool = pCD->rc;

            UINT    nIndex = -1;
            UINT    nID;
            UINT    nStyle;
            int     iImage;
            //  pCD->dwItemSpec Seems To be The IDB
            CToolBarCtrl &toolBarCtrl   = GetToolBarCtrl();

            //
            for ( int i = 0; i < toolBarCtrl.GetButtonCount( ) ; i++ )
            {
                GetButtonInfo ( i, nID, nStyle, iImage );
                if ( nID == pCD->dwItemSpec )
                {
                    nIndex  = i;
                    break;
                }
            }

            if ( nIndex == -1 )
            {
                *pResult = CDRF_DODEFAULT;
                return;
            }


            int cxIcon  = GetSystemMetrics ( SM_CXSMICON );
            int cyIcon  = GetSystemMetrics ( SM_CYSMICON );

            CString buttonText = GetButtonText ( nIndex );

            CImageList *pImageList      = toolBarCtrl.GetImageList();
            if ( pImageList != NULL )
            {
                CDC *pDC = CDC::FromHandle ( pNMCD->hdc );

                UINT uState     = pCD->uItemState;

                UINT uStyle     = DFCS_BUTTONPUSH;
                if ( uState & ODS_SELECTED )
                {
                    uStyle |= DFCS_FLAT;
                    pDC->SetBkMode ( TRANSPARENT );
                    pDC->SetBkColor ( CMWColors::GetBKHeaderCR(CMWColors::m_iDarkTheme != 0) );
                    pDC->SetTextColor ( CMWColors::GetFGHeaderCR(CMWColors::m_iDarkTheme != 0) );
                }
                else if ( uState & ODS_DISABLED || uState & ODS_GRAYED )
                {
                    uStyle |= DFCS_INACTIVE;
                    pDC->SetBkMode ( TRANSPARENT );
                    pDC->SetBkColor ( CMWColors::GetBKHeaderCR(CMWColors::m_iDarkTheme != 0) );
                    pDC->SetTextColor (  CMWColors::GetFGDisabledCR(CMWColors::m_iDarkTheme != 0) );
                }
                else if ( uState & ODS_HOTLIGHT )
                {
                    uStyle |= DFCS_PUSHED;
                    pDC->SetBkMode ( TRANSPARENT );
                    pDC->SetBkColor ( CMWColors::GetBKHeaderCR(CMWColors::m_iDarkTheme != 0) );
                    pDC->SetTextColor ( CMWColors::GetFGSelectedCR(CMWColors::m_iDarkTheme != 0) );
                }
                else
                {
                    uStyle |= DFCS_FLAT;
                    pDC->SetBkMode ( TRANSPARENT );
                    pDC->SetBkColor ( CMWColors::GetBKHeaderCR(CMWColors::m_iDarkTheme != 0) );
                    pDC->SetTextColor ( CMWColors::GetFGHeaderCR(CMWColors::m_iDarkTheme != 0) );
                }

                //
                pDC->DrawFrameControl ( &rectTool, DFC_BUTTON, uStyle );

                int space = 1;
                if (  rectTool.right - rectTool.left > cxIcon )
                {
                    rectTool.left       = rectTool.left + space;
                    rectTool.right      = rectTool.right - space;
                }

                if ( rectTool.bottom - rectTool.top > cyIcon )
                {
                    rectTool.top        = rectTool.top + space;
                    rectTool.bottom     = rectTool.bottom - space;
                }

                FillRect( pDC->GetSafeHdc(), &rectTool, CMWColors::GetBKHeaderBrush(CMWColors::m_iDarkTheme != 0) );

                //
                int width       = rectTool.right - rectTool.left;
                int height      = rectTool.bottom - rectTool.top;

                int xMargin     = ( width - cxIcon ) / 2;
                int yMargin     = ( height - cyIcon ) / ( cyIcon / 2 );
                POINT pt;
                pt.x = rectTool.left;
                pt.y = rectTool.top + yMargin;

                if (  rectTool.right - rectTool.left > cxIcon )
                {
                    pt.x += xMargin;
                }

                if ( rectTool.bottom - rectTool.top > cyIcon )
                {
                    pt.y += yMargin;
                }

                pImageList->Draw ( pDC, nIndex, pt, ILD_NORMAL );

                RECT rectText   = rectTool;
                // rectText.top    += cxIcon + 2;

                //
                CFont* pOldFont     = ( CFont * ) pDC->SelectStockObject( ANSI_VAR_FONT );
                pDC->DrawText ( buttonText, &rectText, DT_SINGLELINE | DT_BOTTOM | DT_CENTER );
                pDC->SelectObject ( pOldFont );
            }
            *pResult = CDRF_SKIPDEFAULT;
            // *pResult = CDRF_DODEFAULT;
            return;
        }
    }

    *pResult = CDRF_DODEFAULT;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CMWToolBar::OnEraseBkgnd(CDC* pDC)
{
    // TODO: ajoutez ici le code de votre gestionnaire de messages et/ou les paramètres par défaut des appels
	BOOL bRes = FriendEraseBkgnd(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

    return CToolBar::OnEraseBkgnd(pDC);
}
