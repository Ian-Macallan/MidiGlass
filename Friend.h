#pragma once

#include <Windows.h>
#include <afx.h>
#include <afxcmn.h>
#include "MWGDI.h"
#include "MWColors.h"

#if !defined(FRIEND_FUNCTION)
#define	FRIEND_FUNCTION


HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);
BOOL	FriendEraseBkgndScrollView(CScrollView *pWnd, CDC* pDC);
void	FriendActivate(BOOL bActivate, CView* pActivateView, CView* pDeactiveView, bool bForce);

#if 0
static COLORREF white0xff = COLORREF(RGB(0xff,0xff,0xff));
static COLORREF white0xee = COLORREF(RGB(0xee,0xee,0xee));
static COLORREF white0xdd = COLORREF(RGB(0xdd,0xdd,0xdd));

static COLORREF grey0x80 = COLORREF(RGB(0x80,0x80,0x80));

static COLORREF black0x00 = COLORREF(RGB(0x00,0x00,0x00));
static COLORREF black0x20 = COLORREF(RGB(0x20,0x20,0x20));
static COLORREF black0x30 = COLORREF(RGB(0x30,0x30,0x30));
static COLORREF black0x40 = COLORREF(RGB(0x40,0x40,0x40));
#endif


#endif