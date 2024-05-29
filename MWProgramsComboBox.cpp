// MWProgramsComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "MWProgramsComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CMWProgramsComboBox, CMWComboBox)

//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CMWProgramsComboBox
//
/////////////////////////////////////////////////////////////////////////////
CMWProgramsComboBox::CMWProgramsComboBox()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CMWProgramsComboBox::~CMWProgramsComboBox()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMWProgramsComboBox, CMWComboBox)
	//{{AFX_MSG_MAP(CMWProgramsComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
// CMWProgramsComboBox message handlers
//
/////////////////////////////////////////////////////////////////////////////
void CMWProgramsComboBox::ReLoad()
{
	CMWComboBox::ResetContent ();

	CMidiClass clForTheText;

	int iX = 0;
	while ( clForTheText.GetInstrumentText ( iX ) != NULL )
	{
		AddString ( clForTheText.GetInstrumentText ( iX ) );
		iX++;
	}

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CMWProgramsComboBox::ResetContent()
{
	SetAnsiVarFont ();

	ReLoad();

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CMWProgramsComboBox::OnSelchange() 
{
	CMWComboBox::OnSelchange();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CMWProgramsComboBox::SetAnsiVarFont()
{
	CMWComboBox::SetAnsiVarFont();
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWProgramsComboBox::SetAnsiFixedFont()
{
	CMWComboBox::SetAnsiFixedFont();
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
HBRUSH CMWProgramsComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CMWComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
	
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
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CMWProgramsComboBox::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bRes = FriendEraseBkgnd(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

	return CMWComboBox::OnEraseBkgnd(pDC);
}
