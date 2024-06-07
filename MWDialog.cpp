#include "stdafx.h"
#include "MWDialog.h"
#include "Friend.h"

IMPLEMENT_DYNAMIC(CMWDialog, CDialog)

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CMWDialog::CMWDialog(UINT idd, CWnd *parent) : CDialog ( idd, parent )
{
}


//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CMWDialog::~CMWDialog(void)
{
}
BEGIN_MESSAGE_MAP(CMWDialog, CDialog)
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BOOL CMWDialog::OnEraseBkgnd(CDC* pDC)
{
	// TODO
	BOOL bRes = FriendEraseBkgnd(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

    return CDialog::OnEraseBkgnd(pDC);
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
HBRUSH CMWDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO
	HBRUSH hBrush = FriendCtlColor(pDC, pWnd, nCtlColor);
	if ( hBrush != NULL )
	{
		return hBrush;
	}

    // TODO:  Retourner un autre pinceau si le pinceau par d�faut n'est pas souhait�
    return hbr;
}
