// MfcMsIeView.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "MfcMsIeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMfcMsIeView

IMPLEMENT_DYNCREATE(CMfcMsIeView, CHtmlView)

CMfcMsIeView::CMfcMsIeView()
{
	//{{AFX_DATA_INIT(CMfcMsIeView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMfcMsIeView::~CMfcMsIeView()
{
}

void CMfcMsIeView::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMfcMsIeView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMfcMsIeView, CHtmlView)
	//{{AFX_MSG_MAP(CMfcMsIeView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMfcMsIeView diagnostics

#ifdef _DEBUG
void CMfcMsIeView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CMfcMsIeView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMfcMsIeView message handlers

void CMfcMsIeView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO
	FriendActivate( bActivate, pActivateView, pDeactiveView, false);
	
	CHtmlView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
