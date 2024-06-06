// MWFormView.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "resource.h"
#include "MWFormView.h"

#include "MidiTrack.h"
#include "MainFrm.h"

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
    // TODO: ajoutez ici le code de votre gestionnaire de messages et/ou les paramètres par défaut des appels
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
    // TODO: ajoutez ici le code de votre gestionnaire de messages et/ou les paramètres par défaut des appels
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
