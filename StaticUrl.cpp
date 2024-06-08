//
////////////////////////////////////////////////////////////////////////////
// StaticUrl.cpp : implementation file
//
////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MidiGlassApp.h"
#include "StaticUrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
////////////////////////////////////////////////////////////////////////////
// CStaticUrl
//
////////////////////////////////////////////////////////////////////////////
CStaticUrl::CStaticUrl()
{
	m_hCursor = NULL;
}

//
////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////
CStaticUrl::~CStaticUrl()
{
}

//
////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CStaticUrl, CStatic)
	//{{AFX_MSG_MAP(CStaticUrl)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
////////////////////////////////////////////////////////////////////////////
// CStaticUrl message handlers
//
////////////////////////////////////////////////////////////////////////////
HBRUSH CStaticUrl::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	//

	COLORREF bkColor = GetSysColor ( COLOR_MENU );
	pDC->SetBkColor ( bkColor );
#ifdef	COLOR_HOTLIGHT
	pDC->SetTextColor ( GetSysColor ( COLOR_HOTLIGHT ) );
#else
	pDC->SetTextColor ( COLORREF (0xff0000 ) );
#endif
	pDC->SetBkMode ( OPAQUE );

	return HBRUSH ( CBrush ( GetSysColor ( COLOR_MENU ) ) );

	//
	return NULL;
}

//
////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////
void CStaticUrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	//
	
	if ( m_hCursor == NULL )
	{
		m_hCursor = LoadCursor ( AfxGetInstanceHandle (), MAKEINTRESOURCE ( IDC_POINT_TO ) );
	}

	::SetCursor ( m_hCursor );

	CStatic::OnMouseMove(nFlags, point);
}
