// MWEventsComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "MWEventsComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CMWEventsComboBox, CMWComboBox)

//
///////////////////////////////////////////////////////////////////////////////////
// CMWEventsComboBox
//
///////////////////////////////////////////////////////////////////////////////////
CMWEventsComboBox::CMWEventsComboBox()
{
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMWEventsComboBox::~CMWEventsComboBox()
{
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMWEventsComboBox, CComboBox)
	//{{AFX_MSG_MAP(CMWEventsComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
///////////////////////////////////////////////////////////////////////////////////
// CMWEventsComboBox message handlers
//
///////////////////////////////////////////////////////////////////////////////////
void CMWEventsComboBox::ReLoad()
{
	CMWComboBox::ResetContent ();

	CMidiClass clForTheText;

	int iX = 0;
	while ( clForTheText.GetClassEventText ( iX ) != NULL )
	{
		AddString ( clForTheText.GetClassEventText ( iX ) );
		iX++;
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWEventsComboBox::ResetContent()
{
	SetAnsiVarFont ( );

	ReLoad();

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWEventsComboBox::OnSelchange() 
{
	// TODO: Add your control notification handler code here
	CMWComboBox::OnSelchange();
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWEventsComboBox::SetAnsiVarFont()
{
	CMWComboBox::SetAnsiVarFont();
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWEventsComboBox::SetAnsiFixedFont()
{
	CMWComboBox::SetAnsiFixedFont();
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
HBRUSH CMWEventsComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	HBRUSH hBrush = FriendCtlColor(pDC, pWnd, nCtlColor);
	if ( hBrush != NULL )
	{
		return hBrush;
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CMWEventsComboBox::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bRes = FriendEraseBkgnd(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

	return CComboBox::OnEraseBkgnd(pDC);
}
