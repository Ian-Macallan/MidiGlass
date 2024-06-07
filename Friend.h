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

#endif