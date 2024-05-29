#pragma once

#include <Windows.h>
#include <afx.h>
#include <afxcmn.h>

#if !defined(FRIEND_FUNCTION)
#define	FRIEND_FUNCTION


class CTRXPen : public CPen
{
    public :
        virtual ~CTRXPen()
        {
            if ( this->m_hObject != NULL )
            {
                DeleteObject ();
            }
        }
};

class CTRXBrush : public CBrush
{
    public :
        virtual ~CTRXBrush()
        {
            if ( this->m_hObject != NULL )
            {
                DeleteObject ();
            }
        }
};

class CTRXBitmap: public CBitmap
{
    public :
        virtual ~CTRXBitmap()
        {
            if ( this->m_hObject != NULL )
            {
                DeleteObject ();
            }
        }
};


HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);
BOOL	FriendEraseBkgndScrollView(CScrollView *pWnd, CDC* pDC);
void	FriendActivate(BOOL bActivate, CView* pActivateView, CView* pDeactiveView, bool bForce);


static COLORREF white0xff = COLORREF(RGB(0xff,0xff,0xff));
static COLORREF white0xee = COLORREF(RGB(0xee,0xee,0xee));
static COLORREF white0xdd = COLORREF(RGB(0xdd,0xdd,0xdd));

static COLORREF grey0x80 = COLORREF(RGB(0x80,0x80,0x80));

static COLORREF black0x00 = COLORREF(RGB(0x00,0x00,0x00));
static COLORREF black0x20 = COLORREF(RGB(0x20,0x20,0x20));
static COLORREF black0x30 = COLORREF(RGB(0x30,0x30,0x30));
static COLORREF black0x40 = COLORREF(RGB(0x40,0x40,0x40));


#endif