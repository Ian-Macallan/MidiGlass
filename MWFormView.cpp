// MWFormView.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "resource.h"
#include "MWFormView.h"

#include "MidiTrack.h"
#include "MainFrm.h"
#include "Friend.h"

//
/////////////////////////////////////////////////////////////////
// CMWFormView
//
/////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMWFormView, CFormView)

//
/////////////////////////////////////////////////////////////////
CMWFormView::CMWFormView() : CFormView(CMWFormView::IDD)
{
    m_pContextMenu  = NULL;
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
CMWFormView::CMWFormView(UINT nIDTemplate) : CFormView(nIDTemplate)
{

}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
CMWFormView::~CMWFormView()
{
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CMWFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMWFormView, CFormView)
    ON_WM_DRAWITEM()
    ON_WM_MEASUREITEM()
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


//
/////////////////////////////////////////////////////////////////
// Diagnostics de CMWFormView
//
/////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CMWFormView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMWFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CMWFormView::SetModified ( BOOL bModified )
{
	CMainFrame *mainFrame = dynamic_cast <CMainFrame *>( AfxGetMainWnd( ) );
	if ( mainFrame != NULL )
	{
		mainFrame->SetModified ( TRUE );
	}
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CMWFormView::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    //
	if ( nIDCtl == 0 )
	{
		if ( lpDrawItemStruct->CtlType == ODT_MENU )
		{
            if ( m_pContextMenu != NULL )
            {
			    m_pContextMenu->DrawItem ( lpDrawItemStruct, TRUE );
    			return;
            }
		}
	}

    CFormView::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CMWFormView::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
    //
	if ( nIDCtl == 0 )
	{
		if ( lpMeasureItemStruct->CtlType == ODT_MENU )
		{
            if ( m_pContextMenu != NULL )
            {
			    m_pContextMenu->MeasureItem ( lpMeasureItemStruct, TRUE );
    			return;
            }
		}
	}

    CFormView::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
BOOL CMWFormView::OnEraseBkgnd(CDC* pDC)
{
    //
	BOOL bRes = FriendEraseBkgndScrollView(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

    return CFormView::OnEraseBkgnd(pDC);
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
HBRUSH CMWFormView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

    //
	//
	HBRUSH hBrush = FriendCtlColor(pDC, pWnd, nCtlColor);
	if ( hBrush != NULL )
	{
		return hBrush;
	}

    //
    return hbr;
}
