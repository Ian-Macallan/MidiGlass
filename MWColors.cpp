#include "stdafx.h"
#include "MWColors.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
static DWORD rgbBlack           = RGB(0x00,0x00,0x00);
static DWORD rgbWhite           = RGB(0xFF,0xFF,0xFF);
static DWORD rgbGrey            = RGB(0x80,0x80,0x80);
static DWORD rgbGreyC0          = RGB(0xC0,0xC0,0xC0);

static DWORD rgbRed             = RGB(0xFF,0x00,0x00);
static DWORD rgbGreen           = RGB(0x00,0xFF,0x00);
static DWORD rgbBlue            = RGB(0x00,0x00,0xFF);

static DWORD rgbCyan            = RGB(0x00,0xFF,0xFF);
static DWORD rgbMagenta         = RGB(0xFF,0x00,0xFF);
static DWORD rgbYellow          = RGB(0xFF,0xFF,0x00);

static DWORD rgbBlack10         = RGB(0x10,0x10,0x10);
static DWORD rgbBlack20         = RGB(0x20,0x20,0x20);
static DWORD rgbBlack30         = RGB(0x30,0x30,0x30);
static DWORD rgbBlack40         = RGB(0x40,0x40,0x40);
static DWORD rgbWhiteCF         = RGB(0xCF,0xCF,0xCF);
static DWORD rgbWhiteDF         = RGB(0xDF,0xDF,0xDF);
static DWORD rgbWhiteEF         = RGB(0xEF,0xEF,0xEF);

static COLORREF crBlack         = COLORREF(rgbBlack);
static COLORREF crWhite         = COLORREF(rgbWhite);
static COLORREF crGrey          = COLORREF(rgbGrey);
static COLORREF crGreyC0        = COLORREF(rgbGreyC0);

static COLORREF crRed           = COLORREF(rgbRed);
static COLORREF crGreen         = COLORREF(rgbGreen);
static COLORREF crBlue          = COLORREF(rgbBlue);

static COLORREF crCyan          = COLORREF(rgbCyan);
static COLORREF crMagenta       = COLORREF(rgbMagenta);
static COLORREF crYellow        = COLORREF(rgbYellow);

static COLORREF crBlack10       = COLORREF(rgbBlack10);
static COLORREF crBlack20       = COLORREF(rgbBlack20);
static COLORREF crBlack30       = COLORREF(rgbBlack30);
static COLORREF crBlack40       = COLORREF(rgbBlack40);
static COLORREF crWhiteCF       = COLORREF(rgbWhiteCF);
static COLORREF crWhiteDF       = COLORREF(rgbWhiteDF);
static COLORREF crWhiteEF       = COLORREF(rgbWhiteEF);


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CBrush CMWColors::BlackBrush;
CBrush CMWColors::WhiteBrush;
CBrush CMWColors::GreyBrush;

CBrush CMWColors::RedBrush;
CBrush CMWColors::GreenBrush;
CBrush CMWColors::BlueBrush;

CBrush CMWColors::CyanBrush;
CBrush CMWColors::MagentaBrush;
CBrush CMWColors::YellowBrush;

CBrush CMWColors::Grey192Brush;

CBrush CMWColors::Black10Brush;
CBrush CMWColors::Black20Brush;
CBrush CMWColors::Black30Brush;
CBrush CMWColors::Black40Brush;
CBrush CMWColors::WhiteCFBrush;
CBrush CMWColors::WhiteDFBrush;
CBrush CMWColors::WhiteEFBrush;

//
/////////////////////////////////////////////////////////////////////////////
//  0 Default Color
//  1 Dark Client Area
//  2 Dark Client Area and Caption
int             CMWColors::m_iDarkTheme        = 0;


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CMWColors::CMWColors(void)
{
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CMWColors::~CMWColors(void)
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CMWColors::Instanciate()
{
    BOOL bBrush;

    //
    bBrush = BlackBrush.CreateSolidBrush ( crBlack );
    if ( bBrush )
    {
        BlackBrush.UnrealizeObject ( );
    }

    //
    bBrush = WhiteBrush.CreateSolidBrush ( crWhite );
    if ( bBrush )
    {
        WhiteBrush.UnrealizeObject ( );
    }

    //
    bBrush = GreyBrush.CreateSolidBrush ( crGrey );
    if ( bBrush )
    {
        GreyBrush.UnrealizeObject ( );
    }

    //
    bBrush = RedBrush.CreateSolidBrush ( crRed );
    if ( bBrush )
    {
        RedBrush.UnrealizeObject ( );
    }

    //
    //
    bBrush = GreenBrush.CreateSolidBrush ( crGreen );
    if ( bBrush )
    {
        GreenBrush.UnrealizeObject ( );
    }

    //
    //
    bBrush = BlueBrush.CreateSolidBrush ( crBlue );
    if ( bBrush )
    {
        BlueBrush.UnrealizeObject ( );
    }

    //
    bBrush = CyanBrush.CreateSolidBrush ( crCyan );
    if ( bBrush )
    {
        CyanBrush.UnrealizeObject ( );
    }

    //
    //
    bBrush = MagentaBrush.CreateSolidBrush ( crMagenta );
    if ( bBrush )
    {
        MagentaBrush.UnrealizeObject ( );
    }

    //
    //
    bBrush = YellowBrush.CreateSolidBrush ( crYellow );
    if ( bBrush )
    {
        YellowBrush.UnrealizeObject ( );
    }

    //
    bBrush = Grey192Brush.CreateSolidBrush ( crGreyC0 );
    if ( bBrush )
    {
        Grey192Brush.UnrealizeObject ( );
    }

    //
    bBrush = Black10Brush.CreateSolidBrush ( crBlack10 );
    if ( bBrush )
    {
        Black10Brush.UnrealizeObject ( );
    }

    //
    bBrush = Black20Brush.CreateSolidBrush ( crBlack20 );
    if ( bBrush )
    {
        Black20Brush.UnrealizeObject ( );
    }

    //
    bBrush = Black30Brush.CreateSolidBrush ( crBlack30 );
    if ( bBrush )
    {
        Black30Brush.UnrealizeObject ( );
    }

    //
    bBrush = Black40Brush.CreateSolidBrush ( crBlack40 );
    if ( bBrush )
    {
        Black40Brush.UnrealizeObject ( );
    }

    //
    bBrush = WhiteCFBrush.CreateSolidBrush ( crWhiteCF );
    if ( bBrush )
    {
        WhiteCFBrush.UnrealizeObject ( );
    }

    //
    bBrush = WhiteDFBrush.CreateSolidBrush ( crWhiteDF );
    if ( bBrush )
    {
        WhiteDFBrush.UnrealizeObject ( );
    }

    //
    bBrush = WhiteEFBrush.CreateSolidBrush ( crWhiteEF );
    if ( bBrush )
    {
        WhiteEFBrush.UnrealizeObject ( );
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
HBRUSH CMWColors::GetBlackBrush()
{
    return ( HBRUSH ) BlackBrush.GetSafeHandle ( );
}

HBRUSH CMWColors::GetWhiteBrush()
{
    return ( HBRUSH ) WhiteBrush.GetSafeHandle ( );
}

HBRUSH CMWColors::GetGreyBrush()
{
    return ( HBRUSH ) GreyBrush.GetSafeHandle ( );
}

HBRUSH CMWColors::GetRedBrush()
{
    return ( HBRUSH ) RedBrush.GetSafeHandle ( );
}

HBRUSH CMWColors::GetGreenBrush()
{
    return ( HBRUSH ) GreenBrush.GetSafeHandle ( );
}

HBRUSH CMWColors::GetBlueBrush()
{
    return ( HBRUSH ) BlueBrush.GetSafeHandle ( );
}

HBRUSH CMWColors::GetCyanBrush()
{
    return ( HBRUSH ) CyanBrush.GetSafeHandle ( );
}

HBRUSH CMWColors::GetMagentaBrush()
{
    return ( HBRUSH ) MagentaBrush.GetSafeHandle ( );
}

HBRUSH CMWColors::GetYellowBrush()
{
    return ( HBRUSH ) YellowBrush.GetSafeHandle ( );
}

HBRUSH CMWColors::GetGreyC0Brush()
{
    return ( HBRUSH ) Grey192Brush.GetSafeHandle ( );
}

HBRUSH CMWColors::GetBlack10Brush()
{
    return ( HBRUSH ) Black10Brush.GetSafeHandle ( );
}

HBRUSH CMWColors::GetBlack20Brush()
{
    return ( HBRUSH ) Black20Brush.GetSafeHandle ( );
}

HBRUSH CMWColors::GetBlack30Brush()
{
    return ( HBRUSH ) Black30Brush.GetSafeHandle ( );
}

HBRUSH CMWColors::GetBlack40Brush()
{
    return ( HBRUSH ) Black40Brush.GetSafeHandle ( );
}

HBRUSH CMWColors::GetWhiteCFBrush()
{
    return ( HBRUSH ) WhiteCFBrush.GetSafeHandle ( );
}

HBRUSH CMWColors::GetWhiteDFBrush()
{
    return ( HBRUSH ) WhiteDFBrush.GetSafeHandle ( );
}

HBRUSH CMWColors::GetWhiteEFBrush()
{
    return ( HBRUSH ) WhiteEFBrush.GetSafeHandle ( );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CBrush *CMWColors::GetBlackCBrush()
{
    return &BlackBrush;
}

CBrush *CMWColors::GetWhiteCBrush()
{
    return &WhiteBrush;
}

CBrush *CMWColors::GetGreyCBrush()
{
    return &GreyBrush;
}

CBrush *CMWColors::GetRedCBrush()
{
    return &RedBrush;
}

CBrush *CMWColors::GetGreenCBrush()
{
    return &GreenBrush;
}

CBrush *CMWColors::GetBlueCBrush()
{
    return &BlueBrush;
}

CBrush *CMWColors::GetCyanCBrush()
{
    return &CyanBrush;
}

CBrush *CMWColors::GetMagentaCBrush()
{
    return &MagentaBrush;
}

CBrush *CMWColors::GetYellowCBrush()
{
    return &YellowBrush;
}

CBrush *CMWColors::GetGreyC0CBrush()
{
    return &Grey192Brush;
}

CBrush *CMWColors::GetBlack10CBrush()
{
    return &Black10Brush;
}

CBrush *CMWColors::GetBlack20CBrush()
{
    return &Black20Brush;
}

CBrush *CMWColors::GetBlack30CBrush()
{
    return &Black30Brush;
}

CBrush *CMWColors::GetBlack40CBrush()
{
    return &Black40Brush;
}

CBrush *CMWColors::GetWhiteCFCBrush()
{
    return &WhiteCFBrush;
}

CBrush *CMWColors::GetWhiteDFCBrush()
{
    return &WhiteDFBrush;
}

CBrush *CMWColors::GetWhiteEFCBrush()
{
    return &WhiteEFBrush;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
COLORREF CMWColors::GetBlackCR()
{
    return crBlack;
}

COLORREF CMWColors::GetWhiteCR()
{
    return crWhite;
}

COLORREF CMWColors::GetGreyCR()
{
    return crGrey;
}

COLORREF CMWColors::GetRedCR()
{
    return crRed;
}

COLORREF CMWColors::GetGreenCR()
{
    return crGreen;
}

COLORREF CMWColors::GetBlueCR()
{
    return crBlue;
}

COLORREF CMWColors::GetCyanCR()
{
    return crCyan;
}

COLORREF CMWColors::GetMagentaCR()
{
    return crMagenta;
}

COLORREF CMWColors::GetYellowCR()
{
    return crYellow;
}

COLORREF CMWColors::GetGreyC0CR()
{
    return crGreyC0;
}

COLORREF CMWColors::GetBlack10CR()
{
    return crBlack10;
}

COLORREF CMWColors::GetBlack20CR()
{
    return crBlack20;
}

COLORREF CMWColors::GetBlack30CR()
{
    return crBlack30;
}

COLORREF CMWColors::GetBlack40CR()
{
    return crBlack40;
}

COLORREF CMWColors::GetWhiteCFCR()
{
    return crWhiteCF;
}

COLORREF CMWColors::GetWhiteDFCR()
{
    return crWhiteDF;
}

COLORREF CMWColors::GetWhiteEFCR()
{
    return crWhiteEF;
}


//
/////////////////////////////////////////////////////////////////////////////
//
//  BUTTON, CLOCK, COMBOBOX, COMMUNICATIONS, CONTROLPANEL, DATEPICKER, DRAGDROP,
//  EDIT, EXPLORERBAR, FLYOUT, GLOBALS, HEADER, LISTBOX, LISTVIEW, MENU, MENUBAND,
//  NAVIGATION, PAGE, PROGRESS, REBAR, SCROLLBAR, SEARCHEDITBOX, SPIN, STARTPANEL,
//  STATUS, TAB, TASKBAND, TASKBAR, TASKDIALOG, TEXTSTYLE, TOOLBAR, TOOLTIP,
//  TRACKBAR, TRAYNOTIFY, TREEVIEW, WINDOW
/////////////////////////////////////////////////////////////////////////////
HBRUSH CMWColors::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    //
    HBRUSH hBrush = NULL;

    if ( CMWColors::m_iDarkTheme == 0 )
    {
        return hBrush;
    }

    if ( pDC == NULL )
    {
        return hBrush;
    }

    //
    switch(nCtlColor)
    {
        case CTLCOLOR_MSGBOX :
        {
            pDC->SetTextColor(CMWColors::GetWhiteCR());
            pDC->SetBkColor(CMWColors::GetBlack30CR());
            pDC->SetBkMode ( TRANSPARENT );
            hBrush = CMWColors::GetBlack30Brush();
            break;
        }

        case CTLCOLOR_EDIT:
        {
            pDC->SetTextColor(CMWColors::GetWhiteCR());
            pDC->SetBkColor(CMWColors::GetBlack10CR());
            pDC->SetBkMode ( TRANSPARENT );
            hBrush = CMWColors::GetBlack10Brush();
            break;
        }

        case CTLCOLOR_LISTBOX:
        {
            pDC->SetTextColor(CMWColors::GetWhiteCR());
            pDC->SetBkColor(CMWColors::GetBlack30CR());
            pDC->SetBkMode ( TRANSPARENT );
            hBrush = CMWColors::GetBlack20Brush();
            break;
        }

        case CTLCOLOR_BTN:
        {
            pDC->SetTextColor(CMWColors::GetWhiteCR());
            pDC->SetBkColor(CMWColors::GetBlack30CR());
            pDC->SetBkMode ( TRANSPARENT );
            hBrush = CMWColors::GetBlack30Brush();
            break;
        }

        case CTLCOLOR_DLG:
        {
            pDC->SetTextColor(CMWColors::GetWhiteCR());
            pDC->SetBkColor(CMWColors::GetBlack30CR());
            pDC->SetBkMode ( TRANSPARENT );
            hBrush = CMWColors::GetBlack30Brush();
            break;
        }

        case CTLCOLOR_SCROLLBAR:
        {
            pDC->SetTextColor(CMWColors::GetWhiteCR());
            pDC->SetBkColor(CMWColors::GetBlack30CR());
            pDC->SetBkMode ( TRANSPARENT );
            hBrush = CMWColors::GetBlack30Brush();
            break;
        }

        //  Static And Read Only Edit
        case CTLCOLOR_STATIC:
        {
            pDC->SetTextColor(CMWColors::GetWhiteCR());
            pDC->SetBkColor(CMWColors::GetBlack30CR());
            //  Transparent makes scroll garbage
            // pDC->SetBkMode ( TRANSPARENT );
            hBrush = CMWColors::GetBlack30Brush();
            break;
        }

        default :
        {
            pDC->SetTextColor(CMWColors::GetWhiteCR());
            pDC->SetBkColor(CMWColors::GetBlack30CR());
            pDC->SetBkMode ( TRANSPARENT );
            hBrush = CMWColors::GetBlack30Brush();
            break;
        }
    }

    //
    return hBrush;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CMWColors::OnEraseBkgnd(CDC* pDC, CWnd* pWnd)
{
    if ( CMWColors::m_iDarkTheme == 0 )
    {
        return FALSE;
    }

    if ( pDC == NULL )
    {
        return FALSE;
    }

    //
    RECT rectFrame;
    pWnd->GetClientRect ( &rectFrame );

    //
    pDC->FillRect ( &rectFrame, CMWColors::GetBlack30CBrush() );

    //
    return TRUE;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CMWColors::SetWindowTheme ( CWnd *pWnd )
{
    ::SetWindowTheme ( pWnd->GetSafeHwnd(), WINDOWS_THEME_RESET  );
}

//
/////////////////////////////////////////////////////////////////////////////
//  Public
/////////////////////////////////////////////////////////////////////////////
//
//  Foreground COLORREF
COLORREF CMWColors::GetFGNormalCR ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteDFCR();
    }
    else
    {
        return GetBlack30CR();
    }
}

COLORREF CMWColors::GetFGSelectedCR ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteCR();
    }
    else
    {
        return GetBlackCR();
    }
}


COLORREF CMWColors::GetFGDisabledCR ( bool bDark )
{
    if ( bDark )
    {
        return GetGreyCR();
    }
    else
    {
        return GetGreyCR();
    }
}

COLORREF CMWColors::GetFGMenuCR ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteDFCR();
    }
    else
    {
        return GetBlack40CR();
    }
}

COLORREF CMWColors::GetFGMenuSelectedCR ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteCR();
    }
    else
    {
        return GetBlackCR();
    }
}

COLORREF CMWColors::GetFGHeaderCR ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteEFCR();
    }
    else
    {
        return GetBlack10CR();
    }
}

//
//  Background COLORREF
COLORREF CMWColors::GetBKNormalCR ( bool bDark )
{
    return GetFGNormalCR ( ! bDark );
}

COLORREF CMWColors::GetBKSelectedCR ( bool bDark )
{
    return GetFGSelectedCR ( ! bDark );
}

COLORREF CMWColors::GetBKDisabledCR ( bool bDark )
{
    return GetFGDisabledCR ( ! bDark );
}

COLORREF CMWColors::GetBKMenuCR ( bool bDark )
{
    return GetFGMenuCR ( ! bDark );
}

COLORREF CMWColors::GetBKMenuSelectedCR ( bool bDark )
{
    return GetFGMenuSelectedCR ( ! bDark );
}

COLORREF CMWColors::GetBKHeaderCR ( bool bDark )
{
    return GetFGHeaderCR ( ! bDark );
}

//  Foreground CBrush
CBrush * CMWColors::GetFGNormalCBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteDFCBrush();
    }
    else
    {
        return GetBlack30CBrush();
    }
}

CBrush * CMWColors::GetFGSelectedCBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteCBrush();
    }
    else
    {
        return GetBlackCBrush();
    }
}

CBrush * CMWColors::GetFGDisabledCBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetGreyCBrush();
    }
    else
    {
        return GetGreyCBrush();
    }
}

CBrush * CMWColors::GetFGMenuCBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteDFCBrush();
    }
    else
    {
        return GetBlack40CBrush();
    }
}

CBrush * CMWColors::GetFGMenuSelectedCBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteCBrush();
    }
    else
    {
        return GetBlackCBrush();
    }
}

CBrush * CMWColors::GetFGHeaderCBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteEFCBrush();
    }
    else
    {
        return GetBlack10CBrush();
    }
}

//
CBrush * CMWColors::GetBKNormalCBrush ( bool bDark )
{
    return GetFGNormalCBrush ( ! bDark );
}

CBrush * CMWColors::GetBKSelectedCBrush( bool bDark )
{
    return GetFGSelectedCBrush ( ! bDark );
}

CBrush * CMWColors::GetBKDisabledCBrush ( bool bDark )
{
    return GetFGDisabledCBrush ( ! bDark );
}

CBrush * CMWColors::GetBKMenuCBrush( bool bDark )
{
    return GetFGMenuCBrush ( ! bDark );
}

CBrush * CMWColors::GetBKMenuSelectedCBrush( bool bDark )
{
    return GetFGMenuSelectedCBrush ( ! bDark );
}

CBrush * CMWColors::GetBKHeaderCBrush( bool bDark )
{
    return GetFGHeaderCBrush ( ! bDark );
}

//  Foreground HBRUSH
HBRUSH CMWColors::GetFGNormalBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteDFBrush();
    }
    else
    {
        return GetBlack30Brush();
    }
}

HBRUSH CMWColors::GetFGSelectedBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteBrush();
    }
    else
    {
        return GetBlackBrush();
    }
}

HBRUSH CMWColors::GetFGDisabledBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetGreyBrush();
    }
    else
    {
        return GetGreyBrush();
    }
}

HBRUSH CMWColors::GetFGMenuBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteDFBrush();
    }
    else
    {
        return GetBlack40Brush();
    }
}

HBRUSH CMWColors::GetFGMenuSelectedBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteBrush();
    }
    else
    {
        return GetBlackBrush();
    }
}

HBRUSH CMWColors::GetFGHeaderBrush ( bool bDark )
{
    if ( bDark )
    {
        return GetWhiteEFBrush();
    }
    else
    {
        return GetBlack10Brush();
    }
}

//
HBRUSH CMWColors::GetBKNormalBrush ( bool bDark )
{
    return GetFGNormalBrush ( ! bDark );
}

HBRUSH CMWColors::GetBKSelectedBrush( bool bDark )
{
    return GetFGSelectedBrush ( ! bDark );
}

HBRUSH CMWColors::GetBKDisabledBrush ( bool bDark )
{
    return GetFGDisabledBrush ( ! bDark );
}

HBRUSH CMWColors::GetBKMenuBrush( bool bDark )
{
    return GetFGMenuBrush ( ! bDark );
}

HBRUSH CMWColors::GetBKMenuSelectedBrush( bool bDark )
{
    return GetFGMenuSelectedBrush ( ! bDark );
}

HBRUSH CMWColors::GetBKHeaderBrush( bool bDark )
{
    return GetFGHeaderBrush ( ! bDark );
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
COLORREF CMWColors::GetFGTipColor(bool bDark)
{
    if ( bDark )
    {
        return CMWColors::GetWhiteCR();
    }
    else
    {
        return FALSE;
    }
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
COLORREF CMWColors::GetBKTipColor(bool bDark)
{
    if ( bDark )
    {
        return CMWColors::GetBlackCR();
    }
    else
    {
        return FALSE;
    }
}


