#include "stdafx.h"

#include "resource.h"

#include "MWNCColor.h"
#include "MWColors.h"
#include "MWMenu.h"
#include "MWGDI.h"
#include "MainFrm.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static CRect GetFrameFullRect ( const CRect &windowRECT )
{
    CRect frameFullRECT;
    frameFullRECT.left      = 0;
    frameFullRECT.top       = 0;
    frameFullRECT.right     = windowRECT.Width();
    frameFullRECT.bottom    = windowRECT.Height();
    return frameFullRECT;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static CRect GetCaptionFullRect ( const CRect &windowRECT )
{
    //  EG : 1
    int yBorder         = GetSystemMetrics(SM_CYBORDER);

    //  EG : 3
    int yFrame          = GetSystemMetrics ( SM_CYDLGFRAME );

    int yCaption        = GetSystemMetrics(SM_CYCAPTION);

    CRect captionFullRECT;

    captionFullRECT.left    = 1;
    captionFullRECT.top     = 1;
    captionFullRECT.right   = windowRECT.Width() - 1;
    captionFullRECT.bottom  = yCaption + 2 * yBorder + yFrame;

    return captionFullRECT;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static CRect GetCaptionInsideRect ( const CRect &windowRECT )
{
    //  EG : 1
    int xBorder         = GetSystemMetrics(SM_CXBORDER);
    int yBorder         = GetSystemMetrics(SM_CYBORDER);

    //  EG : 3
    int xFrame          = GetSystemMetrics ( SM_CXDLGFRAME );
    int yFrame          = GetSystemMetrics ( SM_CYDLGFRAME );

    int yCaption        = GetSystemMetrics(SM_CYCAPTION);

    int xLeft           = xBorder + xFrame;
    int yTop            = yBorder + yFrame;

    int x               = xLeft;
    int y               = yTop;
    int cx              = windowRECT.Width() - 2 * xFrame - xBorder;
    int cy              = yCaption + yBorder + yFrame;

    CRect   captionInsideRECT;
    captionInsideRECT.left      = x; 
    captionInsideRECT.top       = y;
    captionInsideRECT.right     = captionInsideRECT.left + cx;
    captionInsideRECT.bottom    = captionInsideRECT.top + cy;
    return captionInsideRECT;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static CRect GetClientFullRect ( const CRect &windowRECT )
{
    //  EG : 1
    int xBorder         = GetSystemMetrics(SM_CXBORDER);
    int yBorder         = GetSystemMetrics(SM_CYBORDER);

    //  EG : 3
    int xFrame          = GetSystemMetrics ( SM_CXDLGFRAME );
    int yFrame          = GetSystemMetrics ( SM_CYDLGFRAME );

    int xIcon           = GetSystemMetrics(SM_CXICON);
    int yIcon           = GetSystemMetrics(SM_CYICON);

    int xIconSmall      = GetSystemMetrics(SM_CXSMICON);
    int yIconSmall      = GetSystemMetrics(SM_CYSMICON);

    int yCaption        = GetSystemMetrics(SM_CYCAPTION);

    CRect clientFullRECT;

    clientFullRECT.top        = yBorder + yFrame + yCaption + yBorder;
    clientFullRECT.left       = 0;
    clientFullRECT.bottom     = windowRECT.Height();
    clientFullRECT.right      = windowRECT.Width();

    return clientFullRECT;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static int GetIconWidth ( )
{
    int xIcon           = GetSystemMetrics(SM_CXICON);
    return xIcon * 3 / 2 - 1;
}

static int GetIconHeight ( )
{
    int yCaption        = GetSystemMetrics(SM_CYCAPTION);
    return yCaption - 1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static CRect GetIconFullRECT ( const CRect &windowRECT, int left )
{
    // int xIcon           = GetSystemMetrics(SM_CXICON);
    // int yIcon           = GetSystemMetrics(SM_CYICON);

    // int xIconSmall      = GetSystemMetrics(SM_CXSMICON);
    // int yIconSmall      = GetSystemMetrics(SM_CYSMICON);

    int yCaption        = GetSystemMetrics(SM_CYCAPTION);

    int yBorder         = GetSystemMetrics(SM_CYBORDER);
    int yFrame          = GetSystemMetrics ( SM_CYDLGFRAME );

    CRect captionFullRECT   = GetCaptionFullRect ( windowRECT );

    CRect iconFullRECT;

    iconFullRECT.left       = left;
    iconFullRECT.top        = captionFullRECT.top; // yBorder + yFrame;
    iconFullRECT.right      = iconFullRECT.left + GetIconWidth();
    iconFullRECT.bottom     = iconFullRECT.top + GetIconHeight () + yFrame - 1; // iconFullRECT.top + yCaption - 1;

    return iconFullRECT;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CMWNCColor::CMWNCColor(void)
{
    m_pContextMenu          = NULL;
    m_iHover                = ICON_NOT_SET;

   m_bLeftPressed           = FALSE;
   m_windowRECT.top         = -1;
   m_windowRECT.left        = -1;
   m_LeftPressedPoint.x     = -1;
   m_LeftPressedPoint.y     = -1;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CMWNCColor::~CMWNCColor(void)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//  Not Used DrawIconEx is better
/////////////////////////////////////////////////////////////////////////////
void CMWNCColor::DrawResizedIcon ( CDC *pDC, HICON hIcon, const RECT *pRect )
{
    ICONINFO info;
    ZeroMemory ( &info, sizeof(info) );

    //
    BITMAP bmp;
    ZeroMemory(&bmp, sizeof(bmp));

    int xIcon   = 0;
    int yIcon  = 0;
    int pixels      = 0;

    //
    BOOL bRes = GetIconInfo(hIcon, &info);

    //
    if ( info.hbmColor )
    {
        const int nWrittenBytes = GetObject(info.hbmColor, sizeof(bmp), &bmp);
        if ( nWrittenBytes > 0 )
        {
            xIcon       = bmp.bmWidth;
            yIcon       = bmp.bmHeight;
            pixels      = bmp.bmBitsPixel;
        }
    }
    else if ( info.hbmMask )
    {
        // Icon has no color plane, image data stored in mask
        const int nWrittenBytes = GetObject(info.hbmMask, sizeof(bmp), &bmp);
        if(nWrittenBytes > 0)
        {
            xIcon   = bmp.bmWidth;
            yIcon  = bmp.bmHeight / 2;
            pixels      = 1;
        }
    }

    //
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);

    //
    CMWBitmap bitmap;
    bitmap.CreateBitmapIndirect ( &bmp );
    CBitmap* pOldBitmap = memDC.SelectObject ( &bitmap );
    memDC.DrawIcon ( 0, 0, hIcon );

    if ( true )
    {
        bRes = pDC->StretchBlt (
            pRect->left,        // x upper left
            pRect->top,         // y upper left
            // The next two lines specify the width and height.
            pRect->right - pRect->left,
            pRect->bottom - pRect->top,
            //  Source
            &memDC,             // source device context
            0, 0,               // x and y upper left
            xIcon,          // source bitmap width
            yIcon,         // source bitmap height
            SRCCOPY );          // raster operation
    }
    else
    {
        bRes = pDC->BitBlt (
            pRect->left,        // x upper left
            pRect->top,         // y upper left
            // The next two lines specify the width and height.
            pRect->right - pRect->left,
            pRect->bottom - pRect->top,
            //  Source
            &memDC,             // source device context
            0, 0,               // x and y upper left
            SRCCOPY );          // raster operation
    }

    //
    if ( info.hbmColor)
    {
        DeleteObject(info.hbmColor);
    }

    if ( info.hbmMask )
    {
        DeleteObject(info.hbmMask);
    }

    memDC.SelectObject ( pOldBitmap );

    //
    DeleteDC(memDC);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CMWNCColor::ScreenPointOverRect ( CWnd *pWnd, CPoint pt, CRect cRect )
{
    CRect windowRECT;
    pWnd->GetWindowRect(&windowRECT);

    cRect.left      += windowRECT.left;
    cRect.top       += windowRECT.top;

    cRect.right     += windowRECT.left;
    cRect.bottom    += windowRECT.top;

    return cRect.PtInRect( pt );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CMWNCColor::DrawIconFrame ( CDC *pDC, const CRect crect, bool bFillRect, CBrush *pbr )
{
    if ( pDC == NULL )
    {
        return;
    }

    if ( bFillRect )
    {
        RECT rect = crect;
        if ( pbr != NULL )
        {
            pDC->FillRect ( &rect, pbr );
        }
        else
        {
            pDC->FillRect ( &rect, CMWColors::GetBKNormalCBrush( CMWColors::m_iDarkTheme != 0 ) );
        }
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//  To draw Background only set icon to zero
/////////////////////////////////////////////////////////////////////////////
void CMWNCColor::DrawIcon ( CDC *pDC, UINT icon, const CRect crect, bool bFillRect, CBrush *pbr )
{
    if ( pDC == NULL )
    {
        return;
    }

    //
    DrawIconFrame ( pDC, crect, bFillRect, pbr );

    //
    if ( icon != 0 )
    {
        int xIconSmall  = GetSystemMetrics(SM_CXSMICON);
        int yIconSmall  = GetSystemMetrics(SM_CYSMICON);

        int xMargin = ( crect.Width() - xIconSmall ) / 2;
        int yMargin = ( crect.Height() - yIconSmall ) / 2;

        HICON hIcon = AfxGetApp()->LoadIcon(icon);
        DrawIconEx(pDC->m_hDC, crect.left + xMargin, crect.top + yMargin, hIcon, xIconSmall, yIconSmall, 0, NULL, DI_NORMAL);
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
RECT CMWNCColor::GetMenuRect ( CWnd *pWnd, CDC *pDC )
{
    RECT menuRect;
    ZeroMemory ( &menuRect, sizeof(menuRect) );

    //
    if ( pWnd == NULL || pDC == NULL )
    {
        return menuRect;
    }

    //
    CMenu *pMenu = pWnd->GetMenu();
    if ( pMenu != NULL && pMenu->m_hMenu)
    {
        int count = GetMenuItemCount ( pMenu->m_hMenu );
        for ( int i = 0; i < count; i++ )
        {
            RECT itemRect;
            GetMenuItemRect ( pWnd->m_hWnd, pMenu->m_hMenu, i, &itemRect );
            if ( i == 0 )
            {
                menuRect = itemRect;
            }
            else
            {
                if ( itemRect.right > menuRect.right )
                {
                    menuRect.right = itemRect.right;
                }
                if ( itemRect.left < menuRect.left )
                {
                    menuRect.left = itemRect.left;
                }
                if ( itemRect.top < menuRect.top )
                {
                    menuRect.top = itemRect.top;
                }
                if ( itemRect.bottom > menuRect.bottom )
                {
                    menuRect.bottom = itemRect.bottom;
                }
            }
        }

        //
        RECT windowRECT;
        pWnd->GetWindowRect ( &windowRECT );
        menuRect.top        -= windowRECT.top;
        menuRect.left       -= windowRECT.left;
        menuRect.bottom     -= windowRECT.top;
        menuRect.right      -= windowRECT.left;

    }

    //
    return menuRect;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CMWNCColor::PaintCaption( CWnd *pWnd, BOOL bActive, int darkIndicator )
{
    if ( CMWColors::m_iDarkTheme != 2 && CMWColors::m_iDarkTheme != darkIndicator )
    {
        return FALSE;
    }

    //  EG : 1
    int xBorder         = GetSystemMetrics(SM_CXBORDER);
    int yBorder         = GetSystemMetrics(SM_CYBORDER);

    //  EG : 3
    int xFrame          = GetSystemMetrics ( SM_CXDLGFRAME );
    int yFrame          = GetSystemMetrics ( SM_CYDLGFRAME );

    // int xIcon           = GetSystemMetrics(SM_CXICON);
    // int yIcon           = GetSystemMetrics(SM_CYICON);

    // int xIconSmall      = GetSystemMetrics(SM_CXSMICON);
    // int yIconSmall      = GetSystemMetrics(SM_CYSMICON);

    int yCaption        = GetSystemMetrics(SM_CYCAPTION);

    int xLeft           = xBorder + xFrame;
    int yTop            = yBorder + yFrame;

    int iconWidth       = GetIconWidth();
    int iconHeight      = GetIconHeight();

    //
    CDC* pDC = pWnd->GetWindowDC();

    //
    CRect windowRECT;
    pWnd->GetWindowRect(&windowRECT);

    //
    COLORREF foregroundColor = CMWColors::GetFGNormalCR(CMWColors::m_iDarkTheme != 0);

    //  First Draw Frmae Rect Around the window
    CRect frameFullRECT     = GetFrameFullRect ( windowRECT );
    pDC->FrameRect ( &frameFullRECT, CMWColors::GetCyanCBrush() );

    //  The The Caption Rect
    CRect captionFullRECT   = GetCaptionFullRect ( windowRECT );

    //
    if ( bActive )
    {
        pDC->FillSolidRect(&captionFullRECT, CMWColors::GetBKSelectedCR(CMWColors::m_iDarkTheme != 0));
    }
    else
    {
        pDC->FillSolidRect(&captionFullRECT, CMWColors::GetBKNormalCR(CMWColors::m_iDarkTheme != 0));
    }

    //
    //  Caption Rect
    CRect captionInsideRECT = GetCaptionInsideRect ( windowRECT );

    // Draw Old FRame (Window XP)
    UINT captionOption  = DFCS_CAPTIONMIN | DFCS_CAPTIONMAX | DFCS_CAPTIONCLOSE | DFCS_CAPTIONRESTORE | DFCS_CAPTIONHELP;
    captionOption       |= DFCS_FLAT;
    // pDC->DrawFrameControl ( captionInsideRECT, DFC_CAPTION, captionOption );

    char szTitle [ MAX_PATH ];
    pWnd->GetWindowText ( szTitle, sizeof(szTitle));
    CRect textRECT  = captionInsideRECT;
    textRECT.left   = textRECT.left + iconWidth + xBorder;

    pDC->SetBkMode (TRANSPARENT);
    pDC->SetTextColor (foregroundColor);
#if 0
    CRect calcRECT  = textRECT;
    pDC->DrawText ( szTitle, &calcRECT, DT_LEFT | DT_VCENTER | DT_CALCRECT );

    // textRECT.top    = textRECT.top + ( textRECT.Height() - calcRECT.Height() ) / 2;
#endif
    pDC->DrawText ( szTitle, &textRECT, DT_LEFT | DT_VCENTER );

    //
    DWORD dwStyle = pWnd->GetStyle();
    WINDOWPLACEMENT wp;
    pWnd->GetWindowPlacement( &wp );

    //  ICONS
    //////////////////////////////

    //  LEFT
    m_IconRect = GetIconFullRECT ( windowRECT, 1 /* xLeft */ );

    if ( ( dwStyle & WS_SYSMENU ) && ( dwStyle & WS_CAPTION ) )
    {
        DrawIcon ( pDC, IDR_MAINFRAME, m_IconRect );
    }

    //  RIGHT
    // Close
    m_CloseRect = GetIconFullRECT ( windowRECT, /* captionInsideRECT.right */ captionFullRECT.right - iconWidth );
    DrawIcon ( pDC, IDI_CLOSE, m_CloseRect );

    //  Maximize
    m_MaximizeRect = GetIconFullRECT ( windowRECT, m_CloseRect.left - iconWidth );
    if ( dwStyle & WS_MAXIMIZEBOX )
    {
        if ( wp.showCmd == SW_NORMAL )
        {
            DrawIcon ( pDC, IDI_MAXIMIZE, m_MaximizeRect );
        }
        else
        {
            DrawIcon ( pDC, IDI_WINDOWED, m_MaximizeRect );
        }
    }

    //  Minilmize
    m_MinimizeRect = GetIconFullRECT ( windowRECT, m_MaximizeRect.left - iconWidth );
    if ( dwStyle & WS_MINIMIZEBOX )
    {
        DrawIcon ( pDC, IDI_MINIMIZE, m_MinimizeRect );
    }

    //
    pWnd->ReleaseDC ( pDC );

    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CMWNCColor::PaintWindow( CWnd *pWnd, BOOL bActive, int darkIndicator )
{
    if ( CMWColors::m_iDarkTheme != 2 && CMWColors::m_iDarkTheme != darkIndicator )
    {
        return FALSE;
    }

    //
    CDC* pDC = pWnd->GetWindowDC();

    //
    //  Fill Background
    CRect windowRECT;
    pWnd->GetWindowRect(&windowRECT);

    //  First Draw Frmae Rect Around the window
    CRect frameFullRECT = GetFrameFullRect ( windowRECT );
    pDC->FrameRect ( &frameFullRECT, CMWColors::GetCyanCBrush() );

    //
    RECT clientFullRect = GetClientFullRect ( windowRECT );
    pDC->FillRect ( &clientFullRect, CMWColors::GetBKNormalCBrush(CMWColors::m_iDarkTheme != 0) );

    //
    pWnd->ReleaseDC ( pDC );

    //
    PaintCaption ( pWnd, bActive, darkIndicator );

    //
    {
        RECT clientRect;
        pWnd->GetClientRect( &clientRect );
        pWnd->InvalidateRect ( &clientRect );
    }

    //
    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CMWNCColor::Activate( CWnd *pWnd, BOOL bActive, int darkIndicator )
{
    if ( CMWColors::m_iDarkTheme != 2 && CMWColors::m_iDarkTheme != darkIndicator )
    {
        return FALSE;
    }

    return PaintCaption ( pWnd, bActive, darkIndicator );
    // return PaintWindow( pWnd, bActive );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CMWNCColor::OnNcLButtonDown(CWnd *pWnd, UINT nHitTest, CPoint point, int darkIndicator)
{
    //
    if ( CMWColors::m_iDarkTheme == 2 || CMWColors::m_iDarkTheme == darkIndicator )
    {
        if ( ScreenPointOverRect( pWnd, point, m_IconRect ) )
        {
            //  Default Treatment
            return FALSE;
        }

        else if ( ScreenPointOverRect( pWnd, point, m_CloseRect ) )
        {
            return TRUE;
        }

        else if ( ScreenPointOverRect( pWnd, point, m_MinimizeRect ) )
        {
            return TRUE;
        }

        else if ( ScreenPointOverRect( pWnd, point, m_MaximizeRect ) )
        {
            return TRUE;
        }

        //  Icons At RIght
        int xIcon   = GetSystemMetrics(SM_CXICON);

        //
        CRect iconsRect;
        iconsRect.top       = m_CloseRect.top;
        iconsRect.bottom    = m_CloseRect.bottom;
        iconsRect.left      = min(min(m_CloseRect.left, m_MinimizeRect.left), m_MaximizeRect.left);
        iconsRect.right     = max(max(m_CloseRect.right, m_MinimizeRect.right), m_MaximizeRect.right);
        iconsRect.left      = iconsRect.left - xIcon;
        if ( ScreenPointOverRect( pWnd, point, iconsRect ) )
        {
            return TRUE;
        }

        //  We Could handle move here
        CRect windowRECT;
        pWnd->GetWindowRect(&windowRECT);
        CRect captionRect = GetCaptionFullRect ( windowRECT );
        if ( ScreenPointOverRect( pWnd, point, captionRect ) )
        {
            m_bLeftPressed      = FALSE;
            m_windowRECT        = windowRECT;
            m_LeftPressedPoint  = point;
            return m_bLeftPressed;
        }
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CMWNCColor::OnNcLButtonUp(CWnd *pWnd, UINT nHitTest, CPoint point, int darkIndicator)
{
    m_bLeftPressed  = FALSE;

    //
    //
    if ( CMWColors::m_iDarkTheme == 2 || CMWColors::m_iDarkTheme == darkIndicator )
    {
        if ( ScreenPointOverRect( pWnd, point, m_IconRect ) )
        {
            //  Default Treatment
            return FALSE;
        }

        else if ( ScreenPointOverRect( pWnd, point, m_CloseRect ) )
        {
            //
            CMenu *pMenu = pWnd->GetSystemMenu ( FALSE );
            if ( pMenu != NULL )
            {
                MENUITEMINFO menuItemInfo;
                ZeroMemory ( &menuItemInfo, sizeof(menuItemInfo) );
                menuItemInfo.cbSize     = sizeof(menuItemInfo);
                menuItemInfo.fMask      = MIIM_STATE;
                BOOL bRes = pMenu->GetMenuItemInfo ( SC_CLOSE, &menuItemInfo );
                if ( ( menuItemInfo.fState & MFS_DISABLED ) == 0 && ( menuItemInfo.fState & MFS_GRAYED ) == 0 )
                {
                    ::SendMessage ( pWnd->m_hWnd, WM_SYSCOMMAND, SC_CLOSE, NULL );
                    return TRUE;
                }
            }
        }

        else if ( ScreenPointOverRect( pWnd, point, m_MinimizeRect ) )
        {
            //
            DWORD dwStyle = pWnd->GetStyle();
            WINDOWPLACEMENT wp;
            pWnd->GetWindowPlacement( &wp );
            if ( dwStyle & WS_MINIMIZEBOX )
            {
                ::SendMessage ( pWnd->m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, NULL );
            }
            return TRUE;
        }

        else if ( ScreenPointOverRect( pWnd, point, m_MaximizeRect ) )
        {
            //
            DWORD dwStyle = pWnd->GetStyle();
            WINDOWPLACEMENT wp;
            pWnd->GetWindowPlacement( &wp );
            if ( dwStyle & WS_MAXIMIZEBOX )
            {
                if ( wp.showCmd == SW_NORMAL )
                {
                    ::SendMessage ( pWnd->m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, NULL );
                }
                else
                {
                    ::SendMessage ( pWnd->m_hWnd, WM_SYSCOMMAND, SC_RESTORE, NULL );
                }
            }
            return TRUE;
        }

        //  Icons At RIght
        int xIcon   = GetSystemMetrics(SM_CXICON);

        CRect iconsRect;
        iconsRect.top       = m_CloseRect.top;
        iconsRect.bottom    = m_CloseRect.bottom;
        iconsRect.left      = min(min(m_CloseRect.left, m_MinimizeRect.left), m_MaximizeRect.left);
        iconsRect.right     = max(max(m_CloseRect.right, m_MinimizeRect.right), m_MaximizeRect.right);
        iconsRect.left      = iconsRect.left - xIcon;
        if ( ScreenPointOverRect( pWnd, point, iconsRect ) )
        {
            return TRUE;
        }

    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CMWNCColor::OnNcRButtonDown(CWnd *pWnd, UINT nHitTest, CPoint point, int darkIndicator)
{
    if ( CMWColors::m_iDarkTheme == 2 || CMWColors::m_iDarkTheme == darkIndicator )
    {
        CMenu *pMenu = pWnd->GetSystemMenu(FALSE);
        if ( pMenu )
        {
            //
            CMWMenu        menu;
            menu.Attach ( pMenu->m_hMenu );
            m_pContextMenu = menu.GetSubMenu ( 0 );
            m_pContextMenu->TrackPopupMenu ( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, pWnd );
            menu.Detach ( );
            return TRUE;
        }
    }

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CMWNCColor::OnLButtonUp(CWnd *pWnd, UINT nFlags, CPoint point)
{
    m_bLeftPressed  = FALSE;
    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CMWNCColor::OnNcMouseMove(CWnd *pWnd, UINT nHitTest, CPoint point, int darkIndicator)
{
    //
    if ( CMWColors::m_iDarkTheme == 2 || CMWColors::m_iDarkTheme == darkIndicator )
    {
        TRACKMOUSEEVENT tme;
        ZeroMemory ( &tme, sizeof(tme) );
        tme.cbSize      = sizeof(tme);
        tme.hwndTrack   = pWnd->m_hWnd;
        tme.dwFlags     = TME_NONCLIENT  | TME_HOVER | TME_LEAVE;
        tme.dwHoverTime = 100; // HOVER_DEFAULT;
        TrackMouseEvent ( &tme );

        if ( m_bLeftPressed )
        {
            pWnd->SetWindowPos ( NULL, 
                m_windowRECT.left + point.x - m_LeftPressedPoint.x,
                m_windowRECT.top + point.y - m_LeftPressedPoint.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
        }

    }

    //  Always returns FALSE to continue the process
    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CMWNCColor::OnMouseMove(CWnd *pWnd, UINT nFlags, CPoint point, int darkIndicator)
{
    //
    if ( CMWColors::m_iDarkTheme == 2 || CMWColors::m_iDarkTheme == darkIndicator )
    {
        //  Track Event
        TRACKMOUSEEVENT tme;
        ZeroMemory ( &tme, sizeof(tme) );
        tme.cbSize      = sizeof(tme);
        tme.hwndTrack   = pWnd->m_hWnd;
        tme.dwFlags     = TME_NONCLIENT  | TME_HOVER | TME_LEAVE;
        tme.dwHoverTime = HOVER_DEFAULT;
        TrackMouseEvent ( &tme );

        m_bLeftPressed  = FALSE;
    }

    //  Always returns FALSE to continue the process
    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CMWNCColor::DrawAllIcons (  CWnd *pWnd, UINT iconOnly )
{
    if ( iconOnly == ICON_NOT_SET )
    {
        return;
    }

    //
    DWORD dwStyle = pWnd->GetStyle();
    WINDOWPLACEMENT wp;
    pWnd->GetWindowPlacement( &wp );

    //
    CDC* pDC = pWnd->GetWindowDC();

    if ( iconOnly == 0 || iconOnly == IDI_CLOSE )
    {
        DrawIcon ( pDC, IDI_CLOSE, m_CloseRect );
    }

    if ( iconOnly == 0 || iconOnly == IDI_MAXIMIZE || iconOnly == IDI_WINDOWED )
    {
        if ( dwStyle & WS_MAXIMIZEBOX )
        {
            if ( wp.showCmd == SW_NORMAL )
            {
                DrawIcon ( pDC, IDI_MAXIMIZE, m_MaximizeRect );
            }
            else
            {
                DrawIcon ( pDC, IDI_WINDOWED, m_MaximizeRect );
            }
        }
        else
        {
            DrawIconFrame ( pDC, m_MaximizeRect );
        }
    }

    if ( iconOnly == 0 || iconOnly == IDI_MINIMIZE )
    {
        if ( dwStyle & WS_MINIMIZEBOX )
        {
            DrawIcon ( pDC, IDI_MINIMIZE, m_MinimizeRect );
        }
        else
        {
            DrawIconFrame ( pDC, m_MinimizeRect );
        }
    }

    pWnd->ReleaseDC ( pDC );

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CMWNCColor::OnNcMouseHover(CWnd *pWnd, UINT nFlags, CPoint point)
{
    //  Always returns FALSE to continue the process
    if ( ScreenPointOverRect( pWnd, point, m_CloseRect ) )
    {
        if ( m_iHover != IDI_CLOSE )
        {
            DrawAllIcons ( pWnd, m_iHover );

            CDC* pDC = pWnd->GetWindowDC();

            //
            CMenu *pMenu = pWnd->GetSystemMenu ( FALSE );
            if ( pMenu != NULL )
            {
                MENUITEMINFO menuItemInfo;
                ZeroMemory ( &menuItemInfo, sizeof(menuItemInfo) );
                menuItemInfo.cbSize     = sizeof(menuItemInfo);
                menuItemInfo.fMask      = MIIM_STATE;
                BOOL bRes = pMenu->GetMenuItemInfo ( SC_CLOSE, &menuItemInfo );
                if ( ( menuItemInfo.fState & MFS_DISABLED ) == 0 && ( menuItemInfo.fState & MFS_GRAYED ) == 0 )
                {
                    DrawIcon ( pDC, IDI_CLOSE, m_CloseRect, true, CMWColors::GetRedCBrush() );
                }
                else
                {
                    DrawIcon ( pDC, IDI_CLOSE, m_CloseRect, true, CMWColors::GetGreyCBrush() );
                }
            }

            pWnd->ReleaseDC ( pDC );
            m_iHover    = IDI_CLOSE;
        }
        return FALSE;
    }
    else if ( ScreenPointOverRect( pWnd, point, m_MaximizeRect ) )
    {
        //
        DWORD dwStyle = pWnd->GetStyle();
        WINDOWPLACEMENT wp;
        pWnd->GetWindowPlacement( &wp );

        if ( m_iHover != IDI_MAXIMIZE )
        {
            DrawAllIcons ( pWnd, m_iHover );

            CDC* pDC = pWnd->GetWindowDC();
            if ( dwStyle & WS_MAXIMIZEBOX )
            {
                if ( wp.showCmd == SW_NORMAL )
                {
                    DrawIcon ( pDC, IDI_MAXIMIZE, m_MaximizeRect, true, CMWColors::GetCyanCBrush() );
                }
                else
                {
                    DrawIcon ( pDC, IDI_WINDOWED, m_MaximizeRect, true, CMWColors::GetCyanCBrush() );
                }
            }
            else
            {
                DrawIconFrame ( pDC, m_MaximizeRect, true, CMWColors::GetBKNormalCBrush(CMWColors::m_iDarkTheme != 0) );
            }
            pWnd->ReleaseDC ( pDC );
            m_iHover    = IDI_MAXIMIZE;
        }
        return FALSE;
    }
    else if ( ScreenPointOverRect( pWnd, point, m_MinimizeRect ) )
    {
        //
        DWORD dwStyle = pWnd->GetStyle();
        WINDOWPLACEMENT wp;
        pWnd->GetWindowPlacement( &wp );

        if ( m_iHover != IDI_MINIMIZE )
        {
            DrawAllIcons ( pWnd, m_iHover );

            CDC* pDC = pWnd->GetWindowDC();
            if ( dwStyle & WS_MINIMIZEBOX )
            {
                DrawIcon ( pDC, IDI_MINIMIZE, m_MinimizeRect, true, CMWColors::GetCyanCBrush() );
            }
            else
            {
                DrawIconFrame ( pDC, m_MinimizeRect, true, CMWColors::GetBKNormalCBrush(CMWColors::m_iDarkTheme != 0) );
            }
            pWnd->ReleaseDC ( pDC );
            m_iHover    = IDI_MINIMIZE;
        }
        return FALSE;
    }

    DrawAllIcons ( pWnd, m_iHover );
    m_iHover = ICON_NOT_SET;

    return FALSE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CMWNCColor::OnNcMouseLeave(CWnd *pWnd )
{
    //  Always returns FALSE to continue the process
    DrawAllIcons ( pWnd, m_iHover );
    m_iHover        = ICON_NOT_SET;
    m_bLeftPressed  = FALSE;
    return FALSE;
}
