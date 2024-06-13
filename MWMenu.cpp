#include "stdafx.h"

#include "MWMenu.h"
#include "MidiGlassApp.h"
#include "Friend.h"
#include "strstr.h"

static  const int EXTRA_PIXELS_WIDTH    = 8;
static  const int EXTRA_PIXELS_HEIGHT   = 6;
static  const int SEPARATOR_HEIGHT      = 5;

#define MENU_TEXT_SIZE  256

extern CMidiWorksApp theApp;

const UINT MENU_ITEM_MASK   = MIIM_BITMAP | MIIM_CHECKMARKS | MIIM_DATA |
                              MIIM_FTYPE | MIIM_ID | MIIM_STATE | MIIM_STRING |
                              MIIM_SUBMENU;
const UINT MENU_MASK        = MIM_BACKGROUND | MIM_HELPID | MIM_MAXHEIGHT | MIM_MENUDATA | MIM_STYLE;

//
static const bool IgnoreByPos    = true;

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CMWMenu, CMenu)

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMWMenu::CMWMenu(void)
{
    m_pWnd              = NULL;
    m_pSubMenu          = NULL;
    m_hCheckWhiteIcon   = AfxGetApp()->LoadIcon(IDI_CHECK_WHITE);
}

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
CMWMenu::~CMWMenu(void)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CMWMenu::CreateMenu()
{
    return CMenu::CreateMenu();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CMWMenu::CreatePopupMenu()
{
    return CMenu::CreatePopupMenu();
}

//
/////////////////////////////////////////////////////////////////////////////
//  This is the entry that Initialize Popup Menu
/////////////////////////////////////////////////////////////////////////////
BOOL CMWMenu::LoadMenu(LPCTSTR lpszResourceName)
{
    return CMenu::LoadMenu(lpszResourceName);
}

//
/////////////////////////////////////////////////////////////////////////////
//  This is the entry that Initialize Popup Menu
/////////////////////////////////////////////////////////////////////////////
BOOL CMWMenu::LoadMenu(UINT nIDResource)
{
    return CMenu::LoadMenu(nIDResource);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CMWMenu::Attach(HMENU hMenu)
{
    if ( m_pSubMenu != NULL )
    {
        return m_pSubMenu->Attach ( hMenu );
    }
    else
    {
        return CMenu::Attach ( hMenu );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
HMENU CMWMenu::Detach()
{
    if ( m_pSubMenu != NULL )
    {
        return m_pSubMenu->Detach ( );
    }
    else
    {
        return CMenu::Detach ( );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CMWMenu *CMWMenu::SetSystemMenu(CWnd* pWnd, CMenu* pSysMenu)
{
    m_pWnd = pWnd;

    //
    if ( pSysMenu != NULL )
    {
        m_pSubMenu = pSysMenu;
        SetOwnDraw ( m_pSubMenu->m_hMenu, CMWColors::m_iDarkTheme != 0, ID_SYS_MENU );
    }
    else
    {
        m_pSubMenu = pSysMenu;
        SetOwnDraw ( m_hMenu, CMWColors::m_iDarkTheme != 0, ID_SYS_MENU );
    }

    return this;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CMWMenu *CMWMenu::SetApplicationMenu(CWnd* pWnd, CMenu *pMenu )
{
    m_pWnd      = pWnd;
    m_pSubMenu  = NULL;

    //
#if 0
    pMenu->Detach ( );

    //
    LoadMenu ( IDR_MAINFRAME );
    SetOwnDraw ( m_hMenu, CMWColors::m_iDarkTheme != 0  );
    pWnd->SetMenu ( this );
#else
    Attach ( pMenu->m_hMenu );
    SetOwnDraw ( m_hMenu, CMWColors::m_iDarkTheme != 0 );
#endif
    return this;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CMWMenu::UnSetSystemMenu(CMenu* pSysMenu)
{
    //
    if ( pSysMenu != NULL )
    {
        SetOwnDraw ( pSysMenu->m_hMenu, false );
    }
    else
    {
        SetOwnDraw ( m_hMenu, false );
    }
}
//
/////////////////////////////////////////////////////////////////////////////
//  The hMenu must be the handle of the submenu
/////////////////////////////////////////////////////////////////////////////
int CMWMenu::SetOwnDraw ( HMENU hMenu, bool bOwnDrawn, int level )
{
    //  MFT_OWNERDRAW
    //
    //      Retrieve the Menu
    CMenu   *menu = CMenu::FromHandle ( hMenu );

    int count = 0;

    //
    //      If no handle : return
    if ( menu == NULL )
    {
        return count;
    }

    //
    //      If menu is not correct: return
    if ( menu->m_hMenu == NULL )
    {
        return count;
    }

    //  Set TOP Level Data
    MENUINFO menuInfo;
    ZeroMemory ( &menuInfo, sizeof(menuInfo) );
    if ( level == 0 )
    {
        menuInfo.cbSize         = sizeof(menuInfo);
        menuInfo.fMask          = MIM_BACKGROUND | MIM_MENUDATA;
        menuInfo.hbrBack        = CMWColors::GetBKMenuBrush ( CMWColors::m_iDarkTheme != 0 );
        menuInfo.dwMenuData     = level;
    }
    else
    {
        menuInfo.cbSize         = sizeof(menuInfo);
        menuInfo.fMask          = MIM_MENUDATA;
        menuInfo.dwMenuData     = level;

    }
    BOOL bSetMenuInfo = menu->SetMenuInfo ( &menuInfo );

    //
    for ( UINT iPos = 0; iPos < (UINT) menu->GetMenuItemCount ( ); iPos++ )
    {
        char            szText [ MENU_TEXT_SIZE ];
        ZeroMemory ( szText, sizeof(szText) );

        MENUITEMINFO    menuItemInfo;
        ZeroMemory ( &menuItemInfo, sizeof ( menuItemInfo ) );
        menuItemInfo.cbSize     = sizeof ( menuItemInfo );

        menuItemInfo.cch        = sizeof ( szText );
        menuItemInfo.dwTypeData = szText;
        menuItemInfo.fMask      = MENU_ITEM_MASK;
        BOOL bRes = menu->GetMenuItemInfo ( iPos, &menuItemInfo, TRUE );
        if ( bRes )
        {
            menuItemInfo.fMask          = MIIM_FTYPE | MIIM_DATA;
            if ( bOwnDrawn )
            {
                menuItemInfo.fType          |= MFT_OWNERDRAW;
            }
            else if ( menuItemInfo.fType & MFT_OWNERDRAW )
            {
                menuItemInfo.fType          ^= MFT_OWNERDRAW;
            }

            //
            menuItemInfo.dwItemData     = ( level << 8 ) | iPos;
            bRes = menu->SetMenuItemInfo ( iPos, &menuItemInfo, TRUE );
            if ( ! bRes )
            {
                DWORD dwError = GetLastError();
                int i = 0;
            }
            count++;

#ifdef _DEBUG
            char szDebug [ MAX_PATH ];
            ZeroMemory ( szDebug, sizeof(szDebug) );
            sprintf_s ( szDebug, sizeof(szDebug), "SetOwnDraw '%s' 0x%x 0x%lx\n", szText, menuItemInfo.wID, (long) menuItemInfo.dwItemData );
            OutputDebugString ( szDebug );
#endif
            if ( menuItemInfo.hSubMenu )
            {
                count += SetOwnDraw ( menuItemInfo.hSubMenu, bOwnDrawn, level + 1 );
            }
        }
        else
        {
#ifdef _DEBUG
            OutputDebugString ( "SetOwnDraw Not Found\n" );
#endif
        }
    }

    return count;
}

//
/////////////////////////////////////////////////////////////////////////////
//  This is the entry that Initialize Popup Menu
/////////////////////////////////////////////////////////////////////////////
CMWMenu *CMWMenu::GetSubMenu ( int nPos )
{
    m_pSubMenu  = CMenu::GetSubMenu( nPos );
    return this;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CMenu *CMWMenu::GetSubMenu ()
{
    return m_pSubMenu;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CMWMenu::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct, BOOL fByPos )
{
    if ( CMWColors::m_iDarkTheme != 0 )
    {
        if ( m_pSubMenu != NULL )
        {
            DrawMenuItem( lpDrawItemStruct, fByPos );
        }
        else if ( m_hMenu != NULL ) 
        {
            DrawMenuItem( lpDrawItemStruct, fByPos );
        }
        else
        {
            CMenu::DrawItem(lpDrawItemStruct);
        }
    }
    else
    {
        CMenu::DrawItem(lpDrawItemStruct);
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CMWMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct, BOOL fByPos )
{
    if ( CMWColors::m_iDarkTheme != 0 )
    {
        if ( m_pSubMenu != NULL )
        {
            MeasureMenuItem(lpMeasureItemStruct, fByPos );
        }
        else if ( m_hMenu != NULL ) 
        {
            MeasureMenuItem(lpMeasureItemStruct, fByPos);
        }
        else
        {
            CMenu::MeasureItem(lpMeasureItemStruct);
        }
    }
    else
    {
        CMenu::MeasureItem(lpMeasureItemStruct);
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//  For Submenu
/////////////////////////////////////////////////////////////////////////////
BOOL CMWMenu::TrackPopupMenu(UINT nFlags, int x, int y, CWnd* pWnd, LPCRECT lpRect )
{
    m_pWnd = pWnd;

    if ( m_pSubMenu != NULL )
    {
        SetOwnDraw(m_pSubMenu->m_hMenu, CMWColors::m_iDarkTheme != 0, ID_POP_MENU );
        return  m_pSubMenu->TrackPopupMenu( nFlags,  x,  y, pWnd, lpRect );
    }
    else
    {
        SetOwnDraw(m_hMenu, CMWColors::m_iDarkTheme != 0, ID_POP_MENU );
        return  CMenu::TrackPopupMenu( nFlags,  x,  y, pWnd, lpRect );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
UINT CMWMenu::GetMenuItemCount ( )
{
    if ( m_pSubMenu )
    {
        return m_pSubMenu->GetMenuItemCount ( );
    }
    else
    {
        return CMenu::GetMenuItemCount ( );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//  For Submenu
/////////////////////////////////////////////////////////////////////////////
BOOL CMWMenu::GetMenuItemInfo ( UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos )
{
    if ( m_pSubMenu )
    {
        return m_pSubMenu->GetMenuItemInfo ( uItem, lpMenuItemInfo, fByPos );
    }
    else
    {
        return CMenu::GetMenuItemInfo ( uItem, lpMenuItemInfo, fByPos );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//  For Submenu
/////////////////////////////////////////////////////////////////////////////
BOOL CMWMenu::SetMenuItemInfo ( UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos )
{
    lpMenuItemInfo->fType |= MFT_OWNERDRAW;
    if ( m_pSubMenu )
    {
        return m_pSubMenu->SetMenuItemInfo ( uItem, lpMenuItemInfo, fByPos );
    }
    else
    {
        return CMenu::SetMenuItemInfo ( uItem, lpMenuItemInfo, fByPos );
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CMWMenu::SetMenuInfo(LPCMENUINFO lpcmi) 
{
    if ( m_pSubMenu )
    {
        return m_pSubMenu->SetMenuInfo ( lpcmi );
    }
    else
    {
        return CMenu::SetMenuInfo ( lpcmi );
    }

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CMWMenu::GetMenuInfo(LPMENUINFO lpcmi) const
{
    if ( m_pSubMenu )
    {
        return m_pSubMenu->GetMenuInfo ( lpcmi );
    }
    else
    {
        return CMenu::GetMenuInfo ( lpcmi );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//  For Submenu
/////////////////////////////////////////////////////////////////////////////
UINT CMWMenu::EnableMenuItem ( UINT nIDEnableItem, UINT nEnable )
{
    if ( m_pSubMenu )
    {
        return m_pSubMenu->EnableMenuItem ( nIDEnableItem, nEnable );
    }
    else
    {
        return CMenu::EnableMenuItem ( nIDEnableItem, nEnable );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CMWMenu::MeasureMenuItem ( CDC *pDC, const char *pText, CSize *pSize, MENUITEMINFO *menuInfo )
{
    //
    //      Initial Size
    int xCheckIcon  = GetSystemMetrics(SM_CXMENUCHECK);
    int yCheckIcon  = GetSystemMetrics(SM_CYMENUCHECK);

    pSize->cx = xCheckIcon + EXTRA_PIXELS_WIDTH;

    //
    if ( pText != NULL && strlen ( pText ) > 0 )
    {
        pSize->cy = yCheckIcon + EXTRA_PIXELS_HEIGHT;

        CFont* pOldFont     = ( CFont * ) pDC->SelectStockObject( ANSI_VAR_FONT );
        INT tabStop         = ( INT ) strlen ( pText );
        CSize sizeText      = pDC->GetOutputTabbedTextExtent ( pText, -1, 1, &tabStop );
        pSize->cx           += sizeText.cx + xCheckIcon;
        if ( sizeText.cy > pSize->cy )
        {
            pSize->cy = sizeText.cy;
        }

        pDC->SelectObject ( pOldFont );
    }
    else
    {
        if ( pSize->cy < SEPARATOR_HEIGHT )
        {
            pSize->cy       = SEPARATOR_HEIGHT;
        }

        if ( menuInfo != NULL && menuInfo->hSubMenu == NULL )
        {
            pSize->cx           += (EXTRA_PIXELS_WIDTH * 2 );
        }
    }

    return;
}

//
/////////////////////////////////////////////////////////////////////////////
//  For Submenu
/////////////////////////////////////////////////////////////////////////////
void CMWMenu::MeasureMenuItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct, BOOL fByPos )
{
    //
    if ( lpMeasureItemStruct == NULL )
    {
        return;
    }

    if ( IgnoreByPos )
    {
        fByPos  = FALSE;
    }

    //
    MENUINFO cmi;
    ZeroMemory ( &cmi, sizeof(cmi) );
    cmi.cbSize  = sizeof(cmi);
    cmi.fMask   = MENU_MASK;
    BOOL bRes   = GetMenuInfo ( &cmi );

    //
    //      The Size
    CSize               size (0,0);

    //
    //      Get Info
    char    szText [ MENU_TEXT_SIZE ];
    ZeroMemory ( szText, sizeof(szText) );

    MENUITEMINFO    menuItemInfo;
    ZeroMemory ( &menuItemInfo, sizeof ( menuItemInfo ) );

    menuItemInfo.cbSize  = sizeof (menuItemInfo);

    menuItemInfo.fMask          = MENU_ITEM_MASK;
    menuItemInfo.cch            = sizeof ( szText );
    menuItemInfo.dwTypeData     = szText;
    UINT uItem  = ( UINT ) lpMeasureItemStruct->itemData & ID_MNU_MASK;
    if ( ! fByPos )
    {
        uItem  = ( UINT ) lpMeasureItemStruct->itemID;
    }

    //
    bRes = GetMenuItemInfo ( uItem, &menuItemInfo, fByPos );
    if ( bRes )
    {
        if ( m_pWnd )
        {
            CDC *pDC    = m_pWnd->GetDC ( );
            if ( pDC )
            {
                ULONG_PTR itemData = menuItemInfo.dwItemData;

                //  Avoid Top Level
                if ( ( itemData & ID_MNU_MASK ) != itemData )
                {
                    strcat_s  ( szText, sizeof(szText), "SHIFT+F1" );
                }
                else
                {
                    UpperCaseString ( szText );
                }
                MeasureMenuItem ( pDC, szText, &size, &menuItemInfo );
                size.cx     = size.cx + 1;  // Leave some spacez
                size.cy     = size.cy + 3;  // Leave some spacez
                int iRes    = m_pWnd->ReleaseDC ( pDC );
            }
        }
    }
    else
    {
#ifdef _DEBUG
        OutputDebugString ( "DrawMenuItem Not Found\n" );
#endif
    }

#ifdef _DEBUG
    static char szDebug [ MAX_PATH ];
    sprintf_s ( szDebug, sizeof(szDebug), 
        "MeasureMenuItem '%s' 0x%x 0x%x 0x%llx 0x%llx\n", szText, menuItemInfo.wID, menuItemInfo.fType, menuItemInfo.hSubMenu, lpMeasureItemStruct->itemData );
    OutputDebugString ( szDebug );
#endif

    lpMeasureItemStruct->itemWidth      = size.cx;
    lpMeasureItemStruct->itemHeight     = size.cy;

}

//
//====================================================================================
//      Draw an Item
//====================================================================================
void CMWMenu::DrawMenuItem (   LPDRAWITEMSTRUCT lpDrawItemStruct, CDC *pDC,
                               CRect *pRect, const char *pText, MENUITEMINFO *menuInfo )
{
    int xCheckIcon  = GetSystemMetrics(SM_CXMENUCHECK);
    int yCheckIcon  = GetSystemMetrics(SM_CYMENUCHECK);

    const   int     TextAttrLeft    =   DT_EXPANDTABS | DT_LEFT | DT_VCENTER | DT_SINGLELINE;
    const   int     TextAttrRight   =   DT_EXPANDTABS | DT_RIGHT | DT_VCENTER | DT_SINGLELINE;

    COLORREF        colorFg         = 0;
    int             bkMode          = 0;

    HBRUSH      hOldBrush       = NULL;
    BOOL        bDone           = FALSE;

    char    szText [ MENU_TEXT_SIZE ];
    ZeroMemory ( szText, sizeof(szText) );

    //
    //  The entire control needs to be drawn
    //  This is called line by line
    if ( lpDrawItemStruct->itemAction & ODA_DRAWENTIRE )
    {
        bDone = pDC->DrawFrameControl ( &lpDrawItemStruct->rcItem, DFC_POPUPMENU, 0 );
        RECT rect = *pRect;
        rect.top        = rect.top + 1;
        rect.left       = rect.left + 1;
        rect.right      = rect.right - 1;
        rect.bottom     = rect.bottom - 1;
        pDC->FillRect ( pRect, CMWColors::GetBKNormalCBrush(CMWColors::m_iDarkTheme != 0));
    }

    int xIconSmall  = GetSystemMetrics(SM_CXSMICON);
    int yIconSmall  = GetSystemMetrics(SM_CYSMICON);

    //
    if ( lpDrawItemStruct->itemState & ODS_CHECKED )
    {
        // We Will Have to Draw a Bitmap
        hOldBrush       = ( HBRUSH ) pDC->SelectObject ( CMWColors::GetFGNormalBrush(CMWColors::m_iDarkTheme != 0) );

        //
        DrawIconEx ( pDC->m_hDC, lpDrawItemStruct->rcItem.left, lpDrawItemStruct->rcItem.top, 
                    m_hCheckWhiteIcon, xIconSmall, yIconSmall, 0, NULL, DI_NORMAL );

        //
        pDC->SelectObject ( hOldBrush );
    }
    else if ( menuInfo->hbmpItem != NULL )
    {
        hOldBrush       = ( HBRUSH ) pDC->SelectObject ( CMWColors::GetFGNormalBrush(CMWColors::m_iDarkTheme != 0) );

        RECT iconRect   = lpDrawItemStruct->rcItem;
        iconRect.top++;
        iconRect.left++;
        iconRect.bottom++;
        iconRect.right++;
        if ( menuInfo->hbmpItem == HBMMENU_POPUP_RESTORE )
        {
            //
            HICON hIcon   = AfxGetApp()->LoadIcon(IDI_WINDOWED);
            DrawIconEx ( pDC->m_hDC, iconRect.left, iconRect.top, hIcon, xIconSmall, yIconSmall, 0, NULL, DI_NORMAL );
        }
        else if ( menuInfo->hbmpItem == HBMMENU_POPUP_MINIMIZE )
        {
            HICON hIcon   = AfxGetApp()->LoadIcon(IDI_MINIMIZE);
            DrawIconEx ( pDC->m_hDC, iconRect.left, iconRect.top, hIcon, xIconSmall, yIconSmall, 0, NULL, DI_NORMAL );
        }
        else if ( menuInfo->hbmpItem == HBMMENU_POPUP_MAXIMIZE )
        {
            HICON hIcon   = AfxGetApp()->LoadIcon(IDI_MAXIMIZE);
            DrawIconEx ( pDC->m_hDC, iconRect.left, iconRect.top, hIcon, xIconSmall, yIconSmall, 0, NULL, DI_NORMAL );
        }
        else if ( menuInfo->hbmpItem == HBMMENU_POPUP_CLOSE  )
        {
            HICON hIcon   = AfxGetApp()->LoadIcon(IDI_CLOSE);
            DrawIconEx ( pDC->m_hDC, iconRect.left, iconRect.top, hIcon, xIconSmall, yIconSmall, 0, NULL, DI_NORMAL );
        }
        //
        pDC->SelectObject ( hOldBrush );
    }

    //
    if ( lpDrawItemStruct->itemAction & ( ODA_FOCUS | ODA_SELECT ) )
    {
        hOldBrush = ( HBRUSH ) pDC->SelectObject ( CMWColors::GetFGSelectedBrush(CMWColors::m_iDarkTheme != 0) );
    }
    else
    {
        hOldBrush = ( HBRUSH ) pDC->SelectObject ( CMWColors::GetFGNormalBrush(CMWColors::m_iDarkTheme != 0) );
    }

    //
    if ( lpDrawItemStruct->itemState & ODS_SELECTED )
    {
        if ( lpDrawItemStruct->itemState & ( ODS_GRAYED | ODS_DISABLED ) )
        {
            colorFg = pDC->SetTextColor ( CMWColors::GetFGDisabledCR(CMWColors::m_iDarkTheme != 0) );
        }
        else
        {
            colorFg = pDC->SetTextColor ( CMWColors::GetFGSelectedCR(CMWColors::m_iDarkTheme != 0) );
        }
    }
    else
    {
        if ( lpDrawItemStruct->itemState & ( ODS_GRAYED | ODS_DISABLED ) )
        {
            colorFg = pDC->SetTextColor ( CMWColors::GetFGDisabledCR(CMWColors::m_iDarkTheme != 0) );
        }
        else
        {
            colorFg = pDC->SetTextColor ( CMWColors::GetFGNormalCR(CMWColors::m_iDarkTheme != 0) );
        }
    }

    //
    pRect->left += xCheckIcon;

    if ( pText != NULL && strlen ( pText ) > 0 )
    {
        if ( lpDrawItemStruct->itemState & ODS_SELECTED )
        {
            pDC->FillRect ( pRect, CMWColors::GetBKSelectedCBrush(CMWColors::m_iDarkTheme != 0) );
        }
        else
        {
            pDC->FillRect ( pRect, CMWColors::GetBKNormalCBrush(CMWColors::m_iDarkTheme != 0) );
        }

        bkMode          = pDC->SetBkMode ( TRANSPARENT );
        pRect->left     += EXTRA_PIXELS_WIDTH / 2;

        strcpy_s ( szText, sizeof(szText), pText );
        char *pTab = strchr( szText, '\t' );
        if ( pTab != NULL )
        {
            *pTab = '\0';
            pTab++;
        }

        pDC->DrawText ( szText, pRect, TextAttrLeft );
        if ( pTab )
        {
            CRect rect = pRect;
            rect.right = rect.right - EXTRA_PIXELS_WIDTH / 2;
            pDC->DrawText ( pTab, &rect, TextAttrRight );
        }

        pDC->SetBkMode ( bkMode );
    }
    else
    {
        if ( lpDrawItemStruct->itemState & ODS_SELECTED )
        {
            pDC->FillRect ( pRect, CMWColors::GetBKSelectedCBrush(CMWColors::m_iDarkTheme != 0) );
        }
        else
        {
            pDC->FillRect ( pRect, CMWColors::GetBKNormalCBrush(CMWColors::m_iDarkTheme != 0) );
        }

        bkMode          = pDC->SetBkMode ( TRANSPARENT );

        int penSize = 1;
        CMWPen penBlack;
        penBlack.CreatePen(PS_SOLID, penSize, CMWColors::GetFGNormalCR(CMWColors::m_iDarkTheme != 0) );
        CPen* pOldPen = pDC->SelectObject(&penBlack);

        pRect->left     += EXTRA_PIXELS_WIDTH / 2;
        int middle = ( pRect->bottom  - pRect->top ) / 2;
        pDC->MoveTo ( pRect->left, pRect->top + middle );
        pDC->LineTo ( pRect->right - EXTRA_PIXELS_WIDTH / 2, pRect->top + middle );

        pDC->SelectObject(pOldPen);
        penBlack.DeleteObject();

    }

    pDC->SetTextColor ( colorFg );

    pDC->SelectObject ( hOldBrush );

}

//
//====================================================================================
//      Main Entry DrawItem
//
//      Must be a little cleaned
//====================================================================================
void CMWMenu::DrawMenuItem(LPDRAWITEMSTRUCT lpDrawItemStruct, BOOL fByPos )
{
    //
    if ( lpDrawItemStruct == NULL )
    {
        return;
    }

    if ( IgnoreByPos )
    {
        fByPos  = FALSE;
    }

    //
    //      Get Menu Informations
    char    szText [ MENU_TEXT_SIZE ];
    ZeroMemory ( szText, sizeof(szText) );

    //
    MENUITEMINFO    menuItemInfo;
    ZeroMemory ( &menuItemInfo, sizeof ( menuItemInfo ) );

    menuItemInfo.cbSize  = sizeof ( menuItemInfo );

    //
    menuItemInfo.fMask      = MENU_ITEM_MASK;
    menuItemInfo.cch        = sizeof ( szText ) - 1;
    menuItemInfo.dwTypeData = szText;
    UINT id   = ( UINT ) lpDrawItemStruct->itemData & ID_MNU_MASK;
    if ( ! fByPos )
    {
        id   = ( UINT ) lpDrawItemStruct->itemID;
    }

    //
    BOOL bRes = GetMenuItemInfo ( id, &menuItemInfo, fByPos );
    if ( bRes )
    {
        //
        //      Menu rectangle
        CRect   rectMenu = lpDrawItemStruct->rcItem;

        CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
        if ( pDC )
        {
            //  Only for Top Level
            ULONG_PTR itemData = menuItemInfo.dwItemData;
            if ( ( itemData & ID_MNU_MASK ) == itemData )
            {
                UpperCaseString ( szText );
            }
            DrawMenuItem ( lpDrawItemStruct, pDC, &rectMenu, szText, &menuItemInfo );
        }
    }
    else
    {
#ifdef _DEBUG
        OutputDebugString ( "DrawMenuItem Not Found\n" );
#endif
    }

#ifdef _DEBUG
    static char szDebug [ MAX_PATH ];
    sprintf_s ( szDebug, sizeof(szDebug), "DrawMenuItem '%s' 0x%lx 0x%x 0x%llx\n", szText, lpDrawItemStruct->itemData, lpDrawItemStruct->itemID, lpDrawItemStruct->itemData );
    OutputDebugString ( szDebug );
#endif
}

//
//====================================================================================
//      Compute the maximum Bitmap size
//====================================================================================
void CMWMenu::ComputeBitmapMaxSize ( HBITMAP hBitmap )
{
    if ( hBitmap )
    {
        //
        //      Bitmap
        CBitmap *pBitmap = CBitmap::FromHandle ( hBitmap );
        if ( pBitmap )
        {
            //
            //      Bitmap Info
            BITMAP              bmInfo;
            ZeroMemory ( &bmInfo, sizeof ( bmInfo ) );

            if ( pBitmap->GetBitmap ( &bmInfo ) )
            {
                if ( m_iBitmapWidth < bmInfo.bmWidth )
                {
                    m_iBitmapWidth = bmInfo.bmWidth;
                }

                if ( m_iBitmapHeight < bmInfo.bmHeight )
                {
                    m_iBitmapHeight = bmInfo.bmHeight;
                }
            }
        }
    }
}

//
//====================================================================================
//      Compute the maximum Bitmap size
//====================================================================================
void CMWMenu::ComputeBitmapMaxSize (  )
{
    m_iBitmapWidth  = 0;
    m_iBitmapHeight = 0;

    for ( UINT iPos = 0; iPos < GetMenuItemCount ( ); iPos++ )
    {
        MENUITEMINFO    menuItemInfo;
        ZeroMemory ( &menuItemInfo, sizeof ( menuItemInfo ) );
        menuItemInfo.cbSize     = sizeof ( menuItemInfo );

        menuItemInfo.fMask      =   MIIM_BITMAP | MIIM_CHECKMARKS;
        BOOL bRes = GetMenuItemInfo ( iPos, &menuItemInfo, TRUE );
        if ( bRes )
        {
            if ( menuItemInfo.hbmpChecked )
            {
                ComputeBitmapMaxSize ( menuItemInfo.hbmpChecked  );
            }

            if ( menuItemInfo.hbmpUnchecked )
            {
                ComputeBitmapMaxSize ( menuItemInfo.hbmpUnchecked );
            }

            if ( menuItemInfo.hbmpItem )
            {
                ComputeBitmapMaxSize ( menuItemInfo.hbmpItem  );
            }

        }
    }

    m_iBitmapWidth  += (EXTRA_PIXELS_WIDTH*2);
    m_iBitmapHeight += (EXTRA_PIXELS_HEIGHT*2);
}

//
//====================================================================================
//      Draw a bitmap
//====================================================================================
void CMWMenu::DrawBitmap(CDC *pDC, CBitmap *pBitmap, CRect *pRect)
{
    //
    //      Memory DC
    CDC     memDC;
    memDC.CreateCompatibleDC ( pDC );           // ...create it

    //
    //      Select the Bitmap
    if ( memDC.m_hDC )
    {
        CBitmap *pOldBitmap = memDC.SelectObject ( pBitmap );
        BITMAP bm;
        pBitmap->GetBitmap(&bm);
        // blast bits to screen
        pDC->FillRect ( pRect, CMWColors::GetBKMenuCBrush(CMWColors::m_iDarkTheme != 0 ) );
        pDC->BitBlt ( pRect->left, pRect->top, pRect->Width(), pRect->Height(), &memDC, 0, 0, SRCCOPY);

        memDC.SelectObject ( pOldBitmap );          // restore DC
    }
}

//
//====================================================================================
//      Draw a Bitmap
//====================================================================================
int CMWMenu::DrawBitmap(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    if ( lpDrawItemStruct == NULL )
    {
        return m_iBitmapWidth;
    }

    CRect rectBitmap    = lpDrawItemStruct->rcItem;

    //
    //      Bitmap Info
    BITMAP              bmInfo;
    ZeroMemory ( &bmInfo, sizeof ( bmInfo ) );

    //
    //      Get Menu Informations
    MENUITEMINFO    menuItemInfo;
    ZeroMemory ( &menuItemInfo, sizeof ( menuItemInfo ) );
    menuItemInfo.cbSize  = sizeof ( menuItemInfo );

    menuItemInfo.fMask      = MIIM_BITMAP | MIIM_CHECKMARKS;
    UINT uItem  = ( UINT ) lpDrawItemStruct->itemData;
    BOOL bRes = GetMenuItemInfo ( uItem, &menuItemInfo, TRUE );
    if ( bRes )
    {

        CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
        if ( pDC )
        {
            // EraseRectWithGrade1 ( &rectBitmap, pDC );

            //
            //      No Bitmap to display
            if ( menuItemInfo.hbmpUnchecked == NULL )
            {
                return m_iBitmapWidth;
            }

            CBitmap *pBitmap;
            pBitmap             = CBitmap::FromHandle ( menuItemInfo.hbmpUnchecked );
            if ( pBitmap == NULL )
            {
                return m_iBitmapWidth;
            }

            pBitmap->GetBitmap ( &bmInfo );
            rectBitmap.top      += EXTRA_PIXELS_HEIGHT;
            rectBitmap.left     += EXTRA_PIXELS_WIDTH;
            rectBitmap.right    = rectBitmap.left + bmInfo.bmWidth + EXTRA_PIXELS_WIDTH;
            rectBitmap.bottom   = rectBitmap.top + bmInfo.bmHeight + EXTRA_PIXELS_HEIGHT;

            //
            if ( lpDrawItemStruct->itemState & ODS_SELECTED )
            {
                pDC->DrawFrameControl( &rectBitmap, DFC_BUTTON, DFCS_BUTTONPUSH );
            }

            //
            //      Draw the Bitmap
            DrawBitmap ( pDC, pBitmap, &rectBitmap );
        }
    }

    return ( m_iBitmapWidth );

}

//
//====================================================================================
//
//====================================================================================
BOOL CMWMenu::SetMenuItemBitmaps ( UINT nPosition, UINT nFlags, const CBitmap *pBmpUnchecked,
                                        const CBitmap *pBmpChecked )
{
    if ( m_pSubMenu )
    {
        return m_pSubMenu->SetMenuItemBitmaps ( nPosition, nFlags, pBmpUnchecked, pBmpChecked );
    }

    return FALSE;
}

//
//====================================================================================
//
//====================================================================================
BOOL CMWMenu::InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, LPCTSTR lpszNewItem)
{
    nFlags |= MF_OWNERDRAW;

    if ( m_pSubMenu )
    {
        return m_pSubMenu->InsertMenu ( nPosition, nFlags, nIDNewItem, lpszNewItem );
    }
    else
    {
        return CMenu::InsertMenu ( nPosition, nFlags, nIDNewItem, lpszNewItem );
    }
}

//
//====================================================================================
//
//====================================================================================
BOOL CMWMenu::InsertMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const CBitmap* pBmp)
{
    nFlags |= MF_OWNERDRAW;

    if ( m_pSubMenu )
    {
        return m_pSubMenu->InsertMenu ( nPosition, nFlags, nIDNewItem, pBmp );
    }
    else
    {
        return CMenu::InsertMenu ( nPosition, nFlags, nIDNewItem, pBmp );
    }
}

//
//====================================================================================
//
//====================================================================================
BOOL CMWMenu::InsertMenuItem(UINT uItem, LPMENUITEMINFO lpMenuItemInfo, BOOL fByPos)
{
    lpMenuItemInfo->fType |= MFT_OWNERDRAW;

    if ( m_pSubMenu )
    {
        return m_pSubMenu->InsertMenuItem ( uItem, lpMenuItemInfo, fByPos );
    }
    else
    {
        return CMenu::InsertMenuItem ( uItem, lpMenuItemInfo, fByPos );
    }
}

//
//====================================================================================
//
//====================================================================================
BOOL CMWMenu::ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, LPCTSTR lpszNewItem)
{
    nFlags |= MF_OWNERDRAW;

    if ( m_pSubMenu )
    {
        return m_pSubMenu->ModifyMenu ( nPosition, nFlags, nIDNewItem, lpszNewItem );
    }
    else
    {
        return CMenu::ModifyMenu ( nPosition, nFlags, nIDNewItem, lpszNewItem );
    }
}

//
//====================================================================================
//
//====================================================================================
BOOL CMWMenu::ModifyMenu(UINT nPosition, UINT nFlags, UINT_PTR nIDNewItem, const CBitmap* pBmp)
{
    nFlags |= MF_OWNERDRAW;

    if ( m_pSubMenu )
    {
        return m_pSubMenu->ModifyMenu ( nPosition, nFlags, nIDNewItem, pBmp );
    }
    else
    {
        return CMenu::ModifyMenu ( nPosition, nFlags, nIDNewItem, pBmp );
    }
}

//
//====================================================================================
//
//====================================================================================
BOOL CMWMenu::AppendMenu(UINT nFlags, UINT_PTR nIDNewItem, LPCTSTR lpszNewItem)
{
    nFlags |= MF_OWNERDRAW;

    if ( m_pSubMenu )
    {
        return m_pSubMenu->AppendMenu ( nFlags, nIDNewItem, lpszNewItem );
    }
    else
    {
        return CMenu::AppendMenu ( nFlags, nIDNewItem, lpszNewItem );
    }
}

//
//====================================================================================
//
//====================================================================================
BOOL CMWMenu::AppendMenu(UINT nFlags, UINT_PTR nIDNewItem, const CBitmap* pBmp)
{
    nFlags |= MF_OWNERDRAW;

    if ( m_pSubMenu )
    {
        return m_pSubMenu->AppendMenu ( nFlags, nIDNewItem, pBmp );
    }
    else
    {
        return CMenu::AppendMenu ( nFlags, nIDNewItem, pBmp );
    }
}
