// MidiFrame.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "MidiFrame.h"
#include "Friend.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
////////////////////////////////////////////////////////////////////////////////////////
// CMidiFrame
//
////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMidiFrame, CFrameWnd)

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
CMidiFrame::CMidiFrame()
{
    m_pFrameMenu      = NULL;
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
CMidiFrame::~CMidiFrame()
{
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMidiFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMidiFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
    ON_WM_CTLCOLOR()
    ON_WM_ERASEBKGND()
    ON_WM_SHOWWINDOW()
    ON_WM_INITMENUPOPUP()
    ON_WM_DRAWITEM()
    ON_WM_MEASUREITEM()
    ON_WM_NCACTIVATE()
    ON_WM_NCPAINT()
    ON_WM_NCMOUSEMOVE()
    ON_WM_NCMOUSELEAVE()
    ON_WM_NCMOUSEHOVER()
    ON_WM_NCLBUTTONUP()
    ON_WM_NCLBUTTONDOWN()

//    ON_WM_DRAWITEM()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMidiFrame message handlers

//
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
HBRUSH CMidiFrame::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CFrameWnd::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  Modifier ici les attributs du DC
    //
	//
	HBRUSH hBrush = FriendCtlColor(pDC, pWnd, nCtlColor);
	if ( hBrush != NULL )
	{
		return hBrush;
	}


    // TODO:  Retourner un autre pinceau si le pinceau par défaut n'est pas souhaité
    return hbr;
}


//
/////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////
BOOL CMidiFrame::OnEraseBkgnd(CDC* pDC)
{
    // TODO: ajoutez ici le code de votre gestionnaire de messages et/ou les paramètres par défaut des appels
    //
	BOOL bRes = FriendEraseBkgnd(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

    return CFrameWnd::OnEraseBkgnd(pDC);
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CMidiFrame::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    //
	if ( nIDCtl == 0 )
	{
		if ( lpDrawItemStruct->CtlType == ODT_MENU )
		{
            if ( m_pFrameMenu != NULL )
            {
                m_pFrameMenu->DrawItem ( lpDrawItemStruct );
                return;
            }
		}
	}

    CFrameWnd::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CMidiFrame::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    // 
	if ( nIDCtl == 0 )
	{
		if ( lpMeasureItemStruct->CtlType == ODT_MENU )
		{
            if ( m_pFrameMenu != NULL )
            {
                m_pFrameMenu->MeasureItem ( lpMeasureItemStruct );
                return;
            }
		}
	}

    CFrameWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

// 
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CMidiFrame::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
    CFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

    if ( bSysMenu && pPopupMenu != NULL )
    {
        //
        static CMWMenu     frameMenu;
        //  A GetSystemMenu ( FALSE ) must have been done before
        //  So the pPopupMenu will reflect the SystemMenu
        m_pFrameMenu = frameMenu.SetSystemMenu ( this, pPopupMenu );
    }
    else
    {
        m_pFrameMenu  = NULL;
    }
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CMidiFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CFrameWnd::OnShowWindow(bShow, nStatus);

    // TODO: ajoutez ici le code de votre gestionnaire de messages
    //
    CMenu *pMenu = GetSystemMenu ( FALSE );
    if ( pMenu != NULL )
    {
        // CMWMenu::SetOwnDraw ( pMenu->m_hMenu, true );
    }

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CMidiFrame::OnNcActivate(BOOL bActive)
{
    //
    BOOL bTreated = m_NC.Activate (  this, bActive, 1 );
    if ( bTreated )
    {
        return TRUE;
    }

    //
    return CFrameWnd::OnNcActivate(bActive);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiFrame::OnNcPaint()
{
    //
    BOOL bTreated = m_NC.PaintWindow ( this, 0, 1 );
    if ( bTreated )
    {
        return;
    }

    //
    CFrameWnd::OnNcPaint();
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiFrame::OnNcMouseMove(UINT nHitTest, CPoint point)
{
    //
    BOOL bTreated = m_NC.OnNcMouseMove ( this, nHitTest, point, 1 );
    if ( bTreated )
    {
        return;
    }
    CFrameWnd::OnNcMouseMove(nHitTest, point);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiFrame::OnNcMouseLeave()
{
    //
    BOOL bTreated = m_NC.OnNcMouseLeave ( this );
    if ( bTreated )
    {
        return;
    }

    CFrameWnd::OnNcMouseLeave();
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiFrame::OnNcMouseHover(UINT nFlags, CPoint point)
{
    //
    BOOL bTreated = m_NC.OnNcMouseHover ( this, nFlags, point );
    if ( bTreated )
    {
        return;
    }

    CFrameWnd::OnNcMouseHover(nFlags, point);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiFrame::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
    //
    BOOL bTreated = m_NC.OnNcLButtonUp ( this, nHitTest, point, 1 );
    if ( bTreated )
    {
        return;
    }

    CFrameWnd::OnNcLButtonUp(nHitTest, point);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiFrame::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
    //
    BOOL bTreated = m_NC.OnNcLButtonDown ( this, nHitTest, point, 1 );
    if ( bTreated )
    {
        return;
    }

    CFrameWnd::OnNcLButtonDown(nHitTest, point);
}
