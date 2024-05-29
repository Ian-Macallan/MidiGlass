// TracksView.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "TracksView.h"
#include "MainFrm.h"

#include "WaitTreatment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MidiDoc.h"
#include "MidiFile.h"
#include "MainFrm.h"

#include "TrackDetail.h"

extern CMidiWorksApp theApp;

//
/////////////////////////////////////////////////////////////////////////////////////////////
// CTracksView
//
/////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CTracksView, CMWFormView)

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
CTracksView::CTracksView() : CMWFormView(CTracksView::IDD)
{
	//{{AFX_DATA_INIT(CTracksView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pTracks_List	= NULL;
	m_bFirstTime	= true;

	//		The dragg and drop
	m_bDragging		= false;
	m_hItemDrag		= -1;
	m_hItemDrop		= -1;

	//		Image list
	m_pImageList	= NULL;

}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
CTracksView::~CTracksView()
{
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::DoDataExchange(CDataExchange* pDX)
{
	CMWFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTracksView)
	DDX_Control(pDX, IDC_TRACKS_LIST, m_Tracks_List);
	//}}AFX_DATA_MAP
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CTracksView, CMWFormView)
	//{{AFX_MSG_MAP(CTracksView)
	ON_NOTIFY(NM_CLICK, IDC_TRACKS_LIST, OnClickTracksList)
	ON_NOTIFY(LVN_KEYDOWN, IDC_TRACKS_LIST, OnKeydownTracksList)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_TRACKS_DELETE, OnTracksDelete)
	ON_COMMAND(ID_TRACKS_HELP, OnTracksHelp)
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_TRACKS_PLAYSOLO, OnTracksPlaysolo)
	ON_COMMAND(ID_TRACKS_PROPERTY, OnTracksProperty)
	ON_NOTIFY(NM_DBLCLK, IDC_TRACKS_LIST, OnDblclkTracksList)
	ON_COMMAND(ID_TRACKS_MUTE, OnTracksMute)
	ON_COMMAND(ID_TRACKS_AUDIBLE, OnTracksAudible)
	ON_COMMAND(ID_TRACKS_MUTEALL, OnTracksMuteall)
	ON_COMMAND(ID_TRACKS_AUDIBLEALL, OnTracksAudibleall)
	ON_WM_ERASEBKGND()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_TRACKS_LIST, OnBegindragTracksList)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MW_EDIT_SELECT,OnEditSelect)
	ON_MESSAGE(WM_MW_EDIT_BEGIN,OnEditBegin)
	ON_MESSAGE(WM_MW_EDIT_END,OnEditEnd)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_WM_MOVE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////////////////////
// CTracksView diagnostics
//
/////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CTracksView::AssertValid() const
{
	CMWFormView::AssertValid();
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::Dump(CDumpContext& dc) const
{
	CMWFormView::Dump(dc);
}
#endif //_DEBUG

//
/////////////////////////////////////////////////////////////////////////////////////////////
// CTracksView message handlers
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( m_bFirstTime || ( lHint & LEFT_MASK_TRACKS ) || lHint == 0 )
	{
		m_bFirstTime = false;

		EnableScrollBarCtrl(SB_BOTH, FALSE);

		RECT	clientRect;
		GetClientRect ( &clientRect );

		if ( m_Tracks_List.m_hWnd != NULL )
		{
			m_Tracks_List.MoveWindow ( &clientRect, true );
		}

		DisplayValues ();
	}
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnClickTracksList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int					iX;
	BOOL				bOk;
	int					iTrack;

	CMidiDoc			*pMididoc;

	NMLISTVIEW			*nmListView = ( NMLISTVIEW * ) pNMHDR;

	if ( m_Tracks_List.EditColumn ( nmListView->iSubItem ) )
	{
		return;
	}

	// check that one item gets the focus
	iX		= 0;
	bOk		= FALSE;

	pMididoc = dynamic_cast< CMidiDoc * >( GetDocument () );

	if ( pMididoc != NULL )
	{
		while ( ! bOk && ( iX<m_Tracks_List.GetItemCount() ) )
		{
			bOk = m_Tracks_List.GetItemState( iX, LVIS_SELECTED) & LVIS_SELECTED;
			if ( bOk )
			{
				iTrack = (int) m_Tracks_List.GetItemData ( iX );

				if ( iTrack != m_iCurrentTrack )
				{
					m_iCurrentTrack = iTrack;
					pMididoc->SetCurrentTrack ( iTrack );
					pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_EVENTS | RIGHT_MASK_STAFF );
				}

				*pResult = 0;

				return;
			}

			iX++;
		}
	}

	*pResult = 0;
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
unsigned CTracksView::GetIDD()
{
	return IDD;
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnKeydownTracksList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here

	if ( pLVKeyDow->wVKey == VK_DELETE )
	{
		AskAndDeleteTracks();
	}
	
	*pResult = 0;

}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::DeleteTracks()
{
	int					iX;
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;
	unsigned			iTrack;
	BOOL				bOk;
	int					iCountDeleted;

	// check that one item gets the focus
	pMidifile	= NULL;
	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	iCountDeleted = 0;

	if ( pMidifile != NULL )
	{
		iX			= 0;

		while ( iX < m_Tracks_List.GetItemCount() )
		{
			bOk = m_Tracks_List.GetItemState( iX, LVIS_SELECTED) & LVIS_SELECTED;
			if ( bOk )
			{
				iTrack = (unsigned int)  m_Tracks_List.GetItemData ( iX );
				if ( iTrack != 0 )
				{
					pMidifile->GetMidiTrackAddress ( iTrack )->Delete ();
					iCountDeleted++;	
				}
				else
				{
					MessageBox ( "It is Not Authorized To Delete The Track 0", "Title", MB_OK | MB_ICONEXCLAMATION );
				}

			}
			iX++;
		}
	}

	if ( iCountDeleted != 0 )
	{
		CMWFormView::SetModified ( TRUE );

		pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS | RIGHT_MASK_EVENTS | RIGHT_MASK_STAFF );

		DisplayValues ();
	}

}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CRect		rect;
	GetClientRect ( &rect );
	ClientToScreen ( &rect );

	if ( rect.PtInRect ( point ) )
	{
		CMenu		menu;
		menu.LoadMenu ( IDR_MENU_TRACKS );
		CMenu	*pContextMenu = menu.GetSubMenu ( 0 );
		pContextMenu->TrackPopupMenu (
			TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, 
			point.x, point.y, this );
		return;
	}
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnTracksDelete() 
{
	// TODO: Add your command handler code here
	AskAndDeleteTracks();
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::AskAndDeleteTracks()
{
	CString		Title;

	if ( m_Tracks_List.GetCurSel ( ) >= 0 )
	{
		Title.LoadString ( AFX_IDS_APP_TITLE );
		if ( MessageBox ( "Delete Track(s)", Title, 
			MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 ) == IDYES )
		{
			DeleteTracks ();
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnTracksHelp() 
{
	// TODO: Add your command handler code here
	CAboutDlg	dlg;
	dlg.DoModal ();
}


//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
HBRUSH CTracksView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CMWFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
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
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::DisplayValues()
{
	char					szString [ MAX_INT_STRING ];
	int						iX;
	CMidiDoc				*pMididoc;
	CMidiFile				*pMidifile;
	CMidiTrack				*pTrack;
	int						iCount;
	unsigned char			iChannel;
	const char				*pProgram;
	bool					bResult;
	unsigned char			cVolume;

	if ( m_pTracks_List == NULL )
	{
		m_pTracks_List = &m_Tracks_List;

		m_Tracks_List.InsertColumn ( 0, "Track", LVCFMT_LEFT, 48 );
		m_Tracks_List.SetEditFlag ( 0, false );
		m_Tracks_List.InsertColumn ( 1, "Size", LVCFMT_RIGHT, 48);
		m_Tracks_List.SetEditFlag ( 1, false );
		m_Tracks_List.InsertColumn ( 2, "Muted", LVCFMT_LEFT, 48);
		m_Tracks_List.SetEditFlag ( 2, false );

		m_Tracks_List.EnableSpinCtrl ( 3 );
		m_Tracks_List.SetSpinCtrlRange ( 3, 0, 15 );
		m_Tracks_List.InsertColumn ( 3, "Chn", LVCFMT_RIGHT, 32);

		m_Tracks_List.InsertColumn ( 4, "Program", LVCFMT_LEFT, 128);
		m_Tracks_List.InsertColumn ( 5, "Name", LVCFMT_LEFT, 192);
		m_Tracks_List.InsertColumn ( 6, "Instrument", LVCFMT_LEFT, 128);

		m_Tracks_List.EnableSpinCtrl ( 7 );
		m_Tracks_List.SetSpinCtrlRange ( 7, 0, 127 );
		m_Tracks_List.InsertColumn ( 7, "Volume", LVCFMT_RIGHT, 48);

		m_Tracks_List.SetExtendedStyle ( LVS_EX_FULLROWSELECT );

		m_Tracks_List.SetImageList ( &theApp.m_imgImageListChannel16x14, LVSIL_SMALL );
//		m_Tracks_List.SetImageList ( &theApp.m_imgImageList32x2, LVSIL_NORMAL );
	}

	m_Tracks_List.DeleteAllItems ();

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		CWaitTreatment::BeginWait();
		pMidifile->Update ();
		CWaitTreatment::EndWait();

		iCount = 0;

		for ( iX = 0; iX < pMidifile->GetMidiFileTracksCount (); iX++ )
		{
			pTrack = pMidifile->GetMidiTrackAddress ( iX );
			if ( pTrack->GetTrackLength () > 0 )
			{
				sprintf_s ( szString, sizeof ( szString ), "%d", iX );
				m_Tracks_List.InsertItem ( iCount, szString, 0 );
				m_Tracks_List.SetItemData( iCount, ( DWORD ) iX );

				sprintf_s ( szString, sizeof ( szString ), "%6d", pMidifile->GetMidiTrackSize ( iX ) );
				m_Tracks_List.SetItemText ( iCount, 1, szString );

				if ( pMidifile->GetMuteState ( iX ) )
				{
					m_Tracks_List.SetItem ( iCount, 0, LVIF_IMAGE, NULL, 1, 0, 0, 0 );
					sprintf_s ( szString, sizeof ( szString ), "Yes" );
				}
				else
				{
					m_Tracks_List.SetItem ( iCount, 0, LVIF_IMAGE, NULL, 0, 0, 0, 0 );
					sprintf_s ( szString, sizeof ( szString ), "No" );
				}
				m_Tracks_List.SetItemText ( iCount, 2, szString );

				iChannel = pTrack->GetChannel ();
				if ( iChannel != 0xff )
				{
					sprintf_s ( szString, sizeof ( szString ), "%d", iChannel ); 
				}
				else
				{
					strcpy_s ( szString, sizeof ( szString ), "NA" ); 
				}
				m_Tracks_List.SetItemText ( iCount, 3, szString );

				pProgram = pTrack->GetProgramText ();
				if ( pProgram != NULL )
				{
					m_Tracks_List.SetItemText ( iCount, 4, pProgram);
				}
				else
				{
					m_Tracks_List.SetItemText ( iCount, 4, "NA" );
				}

				pProgram = pTrack->GetTrackName ();
				if ( pProgram != NULL )
				{
					m_Tracks_List.SetItemText ( iCount, 5, pProgram);
				}
				else
				{
					m_Tracks_List.SetItemText ( iCount, 5, "NA" );
				}

				pProgram = pTrack->GetInstrumentName ();
				if ( pProgram != NULL )
				{
					m_Tracks_List.SetItemText ( iCount, 6, pProgram);
				}
				else
				{
					m_Tracks_List.SetItemText ( iCount, 6, "NA" );
				}

				bResult = pTrack->GetVolume ( &cVolume );
				if ( bResult )
				{
					sprintf_s ( szString, sizeof ( szString ), "%d", cVolume );
					m_Tracks_List.SetItemText ( iCount, 7, szString);
				}
				else
				{
					m_Tracks_List.SetItemText ( iCount, 7, "64" );
				}

				iCount++;
			}
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnTracksPlaysolo() 
{
	// TODO: Add your command handler code here
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;
	int					iX;
	int					iTrack;

	pMidifile	= NULL;
	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( m_Tracks_List.GetCurSel ( ) >= 0 )
	{
		iTrack = (int) m_Tracks_List.GetItemData ( m_Tracks_List.GetCurSel ( ) );
		if ( iTrack >= 0 && pMidifile != NULL )
		{
			for ( iX = 0; iX < MAX_TRACKS; iX++ )
			{
				pMidifile->SetMuteState  ( iX, true );
			}

			pMidifile->SetMuteState  ( iTrack, false );

			pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS );
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
CMidiTrack *CTracksView::GetCurrentTrack()
{
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;

	pMidifile	= NULL;
	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		return pMidifile->GetMidiTrackAddress ( m_iCurrentTrack );
	}

	return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnTracksProperty() 
{
	// TODO: Add your command handler code here
	ShowTrackDetail ( );
	
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnDblclkTracksList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ShowTrackDetail ( );
	*pResult = 0;
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::ShowTrackDetail()
{
	CTrackDetail		dlg;
	CMidiDoc			*pMididoc;

	// check that one item gets the focus
	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );

	if ( m_Tracks_List.GetCurSel ( ) >= 0 )
	{
		m_iCurrentTrack = (int) m_Tracks_List.GetItemData ( m_Tracks_List.GetCurSel ( ) );
		if ( m_iCurrentTrack >= 0 )
		{
			if ( dlg.DoModal () == IDOK )
			{
				if ( pMididoc != NULL )
				{
					pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS | RIGHT_MASK_EVENTS | RIGHT_MASK_STAFF );
				}
			}
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnTracksMute() 
{
	// TODO: Add your command handler code here
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;

	pMidifile	= NULL;
	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		if ( m_Tracks_List.GetCurSel ( ) >= 0 )
		{
			m_iCurrentTrack = (int) m_Tracks_List.GetItemData ( m_Tracks_List.GetCurSel ( ) );
			if ( m_iCurrentTrack >= 0 )
			{
				pMidifile->SetMuteState  ( m_iCurrentTrack, true );
				if ( pMididoc != NULL )
				{
					pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS );
				}
			}

		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnTracksAudible() 
{
	// TODO: Add your command handler code here
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;

	pMidifile	= NULL;
	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		if ( m_Tracks_List.GetCurSel ( ) >= 0 )
		{
			m_iCurrentTrack = (int) m_Tracks_List.GetItemData ( m_Tracks_List.GetCurSel ( ) );
			if ( m_iCurrentTrack >= 0 )
			{
				pMidifile->SetMuteState  ( m_iCurrentTrack, false );
				if ( pMididoc != NULL )
				{
					pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS );
				}
			}
		}
	}
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnTracksMuteall() 
{
	// TODO: Add your command handler code here
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;
	int					iX;

	pMidifile	= NULL;
	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		for ( iX = 0; iX < MAX_TRACKS; iX++ )
		{
			pMidifile->SetMuteState  ( iX, true );
		}

		if ( pMididoc != NULL )
		{
			pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS );
		}
	}

}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnTracksAudibleall() 
{
	// TODO: Add your command handler code here
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;
	int					iX;

	pMidifile	= NULL;
	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		for ( iX = 0; iX < MAX_TRACKS; iX++ )
		{
			pMidifile->SetMuteState  ( iX, false );
		}

		if ( pMididoc != NULL )
		{
			pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS );
		}
	}

	
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//	Return the pointer on the Edit Object CWnd *
//
/////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTracksView::OnEditBegin(WPARAM wParam, LPARAM lParam)
{
	BOOL	bResult;
	int		iItem		= m_Tracks_List.GetItemIndex ( );
	int		iSubItem	= m_Tracks_List.GetSubItemIndex ( );
	char	szText [ MAX_INSTRUMENT_STRING ];

	if ( iSubItem == 4 )
	{
		CMWProgramsComboBox			*pCombo;
		CRect						rectLabel;

		bResult = m_Tracks_List.GetSubItemRect( iItem, iSubItem, LVIR_LABEL, rectLabel );
		rectLabel.bottom += ( ( rectLabel.bottom - rectLabel.top ) * 6 );

		pCombo = new ( CMWProgramsComboBox );
		pCombo->Create ( 
			WS_CHILD | WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | 
			CBS_DROPDOWNLIST, 
			rectLabel, ( CWnd * ) & m_Tracks_List, IDC_EDIT_COLUMNS );
		pCombo->ResetContent (); 

		m_Tracks_List.GetItemText( iItem, iSubItem, szText, sizeof ( szText ) );

		int iGetCurSel = pCombo->FindString( 0, szText );
		if ( iGetCurSel >= 0 )
		{
			pCombo->SetCurSel ( iGetCurSel );
		}

		return ( LRESULT ) pCombo;
	}
	else if ( iSubItem == 6 )
	{
		CMWSF2FilesComboBox			*pCombo;
		CRect						rectLabel;

		BOOL bResult = m_Tracks_List.GetSubItemRect( iItem, iSubItem, LVIR_LABEL, rectLabel );
		rectLabel.bottom += ( ( rectLabel.bottom - rectLabel.top ) * 6 );

		pCombo = new CMWSF2FilesComboBox( );
		pCombo->Create ( 
			WS_CHILD | WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | 
			// CBS_DROPDOWNLIST, 
			CBS_DROPDOWN,
			rectLabel, ( CWnd * ) & m_Tracks_List, IDC_EDIT_COLUMNS );
		pCombo->ResetContent (); 

		m_Tracks_List.GetItemText( iItem, iSubItem, szText, sizeof ( szText ) );

		int iGetCurSel = pCombo->FindString( 0, szText );
		if ( iGetCurSel >= 0 )
		{
			pCombo->SetCurSel ( iGetCurSel );
		}

		return ( LRESULT ) pCombo;
	}


	return NULL;
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CTracksView::OnEditEnd(WPARAM wParam, LPARAM lParam)
{
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;
	int					iChannel;
	int					iTrack;
	char				szText [ MAX_INSTRUMENT_STRING ];
	int					iItem;
	int					iSubItem;

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	iItem		= m_Tracks_List.GetItemIndex ( );
	iSubItem	= m_Tracks_List.GetSubItemIndex ( );

	m_Tracks_List.GetItemText ( iItem, 0, szText, sizeof ( szText ) );
	iTrack = atoi ( szText );

	//		The channel
	if ( iSubItem == 3 )
	{
		CMWEditInList		*pEdit;
		char				szText [ MAX_INT_STRING ];
		unsigned char		newChannel;

		pEdit = dynamic_cast< CMWEditInList *>( m_Tracks_List.GetEditControlHandle ( ) );
		if ( pEdit != NULL && pEdit->GetModify() )
		{
			pEdit->GetWindowText ( szText, sizeof ( szText ) );
			newChannel = atoi ( szText ); 

			if ( pMidifile != NULL && iTrack >= 0 )
			{
				iChannel = pMidifile->GetTrackChannel ( iTrack );
				if ( iChannel >= 0 && iChannel < MAX_CHANNELS && newChannel != iChannel )
				{
					//
					pMidifile->setEditChange ( true );

					//		Now Set the channel on the track
					pMidifile->SetTrackChannel ( iTrack, newChannel );
				}
				else
				{
					pMidifile->SetMidiErrorText ( "Program not changed : track is not single channel" );
				}

				if ( pMididoc != NULL )
				{
					CMWFormView::SetModified ( TRUE );
					pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_EVENTS | RIGHT_MASK_CHANNELS | RIGHT_MASK_ERRORS );
				}
			}
		}

		if ( pMidifile != NULL )
		{
			pMidifile->setEditChange ( false );
		}

		return 0;
	}

	//		The program name
	else if ( iSubItem == 4 )
	{
		CMWProgramsComboBox	*pCombo;

		pCombo = dynamic_cast<  CMWProgramsComboBox *>( m_Tracks_List.GetEditControlHandle ( ) );
		if ( pCombo != NULL )
		{
			int iCurSel = pCombo->GetCurSel ( );
			if ( iCurSel >= 0 && pCombo->GetModify() )
			{
				//		This update the view
				pCombo->GetLBText( iCurSel, szText );
				m_Tracks_List.SetItemText( iItem, iSubItem, szText );

				//		Now Set the chunk
				//		Send it to midi
				if ( pMidifile != NULL && iTrack >= 0 )
				{
					iChannel = pMidifile->GetTrackChannel ( iTrack );
					unsigned char oldProgram = pMidifile->GetChannelProgramNum ( iChannel );
					if ( iChannel >= 0 && iChannel < MAX_CHANNELS && iCurSel != oldProgram )
					{
						pMidifile->setEditChange ( true );

						pMidifile->SetChannelProgram ( iChannel, iCurSel );

						//	Immediate Chunk
						CMidiProgram	*pProgram = new CMidiProgram ( iChannel, iCurSel );

						pMidifile->SetImmediateChunk ( pProgram->GetChunk () );

						delete pProgram;
					}
					else
					{
						pMidifile->SetMidiErrorText ( "Program not changed : track is not single channel" );
					}

					pMidifile->SetProgramOnTrack( iTrack, iCurSel );

					if ( pMididoc != NULL )
					{
						CMWFormView::SetModified ( TRUE );
						pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_EVENTS | RIGHT_MASK_CHANNELS | RIGHT_MASK_ERRORS );
					}
				}
			}

			m_Tracks_List.SetEditControlHandle ( NULL );
			delete pCombo;

		}

		if ( pMidifile != NULL )
		{
			pMidifile->setEditChange ( false );
		}

		return 1;
	}

	//		The Track Name name
	else if ( iSubItem == 5 )
	{
		CMWEditInList		*pEdit;

		pEdit = dynamic_cast<  CMWEditInList *>( m_Tracks_List.GetEditControlHandle ( ) );
		if ( pEdit != NULL && pEdit->GetModify() )
		{
			pEdit->GetWindowText ( szText, sizeof ( szText ) );

			if ( pMidifile != NULL && iTrack >= 0 )
			{
				pMidifile->SetTrackNameOnTrack( iTrack, szText );

			}

			if ( pMididoc != NULL )
			{
				CMWFormView::SetModified ( TRUE );
				pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_EVENTS );
			}
		}

		if ( pMidifile != NULL )
		{
			pMidifile->setEditChange ( false );
		}

		return 0;
	}

	//	Instrument / SF2 File
	else if (iSubItem == 6 )
	{
		CMWSF2FilesComboBox		*pCombo;

		pCombo = dynamic_cast<  CMWSF2FilesComboBox *>( m_Tracks_List.GetEditControlHandle ( ) );
		if ( pCombo != NULL )
		{
			int iGetCurSel = pCombo->GetCurSel ( );
			if ( pCombo->GetModify() )
			{
				if ( iGetCurSel >= 0 )
				{
					pCombo->GetLBText( iGetCurSel, szText );
					m_Tracks_List.SetItemText( iItem, iSubItem, szText );

					//
					if ( pMidifile != NULL && iTrack >= 0 )
					{
						pMidifile->SetInstrumentNameOnTrack( iTrack, szText );
					}
				}
				else
				{
					ZeroMemory ( szText, sizeof(szText) );
					pCombo->GetWindowTextA(szText, sizeof(szText));
					m_Tracks_List.SetItemText( iItem, iSubItem, szText );

					//
					if ( pMidifile != NULL && iTrack >= 0 )
					{
						pMidifile->SetInstrumentNameOnTrack( iTrack, szText );
					}
				}

				if ( pMididoc != NULL )
				{
					CMWFormView::SetModified ( TRUE );
					pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_EVENTS );
				}

			}

			m_Tracks_List.SetEditControlHandle ( NULL );
			delete pCombo;

		}

		if ( pMidifile != NULL )
		{
			pMidifile->setEditChange ( false );
		}

		return 1;
	}

	//		The Old instrument name : Not Used
	else if ( iSubItem == 16 )	// Not Used
	{
		CMWEditInList		*pEdit;

		pEdit = dynamic_cast<  CMWEditInList *>( m_Tracks_List.GetEditControlHandle ( ) );
		if ( pEdit != NULL && pEdit->GetModify() )
		{
			pEdit->GetWindowText ( szText, sizeof ( szText ) );

			if ( pMidifile != NULL && iTrack >= 0 )
			{
				pMidifile->SetInstrumentNameOnTrack( iTrack, szText );

				if ( pMididoc != NULL )
				{
					CMWFormView::SetModified ( TRUE );
					pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_EVENTS );
				}
			}
		}

		if ( pMidifile != NULL )
		{
			pMidifile->setEditChange ( false );
		}

		return 0;
	}

	//		The volume
	else if ( iSubItem == 7 )
	{
		CMWEditInList		*pEdit;
		unsigned char		newVolume;

		pEdit = dynamic_cast<  CMWEditInList *>( m_Tracks_List.GetEditControlHandle ( ) );
		if ( pEdit != NULL && pEdit->GetModify() )
		{
			pEdit->GetWindowText ( szText, sizeof ( szText ) );
			newVolume = atoi ( szText ); 

			if ( pMidifile != NULL && iTrack >= 0 )
			{
				iChannel = pMidifile->GetTrackChannel ( iTrack );

				unsigned char		oldVolume = newVolume;
				pMidifile->GetChannelVolume ( iChannel, &oldVolume );

				if ( iChannel >= 0 && iChannel < MAX_CHANNELS && newVolume != oldVolume )
				{
					pMidifile->setEditChange ( true );

					pMidifile->SetChannelVolume ( iChannel, newVolume );

					//		Now Set the chunk
					//		7 Is the volume
					CMidiParameter	*pControler = new CMidiParameter ( iChannel, CNTR_VOLUME_MSB, newVolume );
					pMidifile->SetImmediateChunk ( pControler->GetChunk () );

					delete pControler;
				}
				else
				{
					pMidifile->SetMidiErrorText ( "Controller not changed : track is not single channel" );
				}

				pMidifile->SetVolumeOnTrack ( iTrack, newVolume );

				if ( pMididoc != NULL )
				{
					CMWFormView::SetModified ( TRUE );
					pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_EVENTS | RIGHT_MASK_CHANNELS | RIGHT_MASK_ERRORS );
				}
			}
		}

		if ( pMidifile != NULL )
		{
			pMidifile->setEditChange ( false );
		}

		return 0;
	}

	if ( pMidifile != NULL )
	{
		pMidifile->setEditChange ( false );
	}

	return 0;
}

//
/////////////////////////////////////////////////////////////////
//	Combo Box
/////////////////////////////////////////////////////////////////
LRESULT CTracksView::OnEditSelect(WPARAM wParam, LPARAM lParam)
{
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;
	int					iChannel;
	int					iTrack;
	char				szText [ MAX_PATH ];

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	int iItem		= m_Tracks_List.GetItemIndex ( );
	int iSubItem	= m_Tracks_List.GetSubItemIndex ( );

	m_Tracks_List.GetItemText ( iItem, 0, szText, sizeof ( szText ) );
	iTrack = atoi ( szText );

	if ( iSubItem == 4 )
	{
		CMWProgramsComboBox	*pCombo;

		pCombo = dynamic_cast<  CMWProgramsComboBox *>( m_Tracks_List.GetEditControlHandle ( ) );
		if ( pCombo != NULL )
		{
			int iGetCurSel = pCombo->GetCurSel ( );
			if ( iGetCurSel >= 0 && pCombo->GetModify() )
			{
				pCombo->GetLBText( iGetCurSel, szText );
				m_Tracks_List.SetItemText( iItem, iSubItem, szText );

				//		Now Set the chunk
				//		Send it to midi
				if ( pMidifile != NULL && iTrack >= 0 )
				{
					iChannel = pMidifile->GetTrackChannel ( iTrack );
					if ( iChannel >= 0 && iChannel < MAX_CHANNELS )
					{
						pMidifile->SetChannelProgram ( iChannel, iGetCurSel );


						//	Set Immediate Chunk
						CMidiProgram	*pProgram = new CMidiProgram ( iChannel, iGetCurSel );

						pMidifile->SetImmediateChunk ( pProgram->GetChunk () );

						delete pProgram;

					}
					else
					{
						pMidifile->SetMidiErrorText ( "Program not changed : track is not single channel" );
					}
				}

				if ( pMididoc != NULL )
				{
					CMWFormView::SetModified ( TRUE );
					pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_EVENTS | RIGHT_MASK_ERRORS );
				}
			}
		}

		return 1;
	}
	else if ( iSubItem == 6 )
	{
		CMWSF2FilesComboBox		*pCombo;

		pCombo = dynamic_cast<  CMWSF2FilesComboBox *>( m_Tracks_List.GetEditControlHandle ( ) );
		if ( pCombo != NULL )
		{
			if ( pCombo->GetModify() )
			{
				int iGetCurSel = pCombo->GetCurSel ( );
				if ( iGetCurSel >= 0 )
				{
					pCombo->GetLBText( iGetCurSel, szText );
					m_Tracks_List.SetItemText( iItem, iSubItem, szText );

					//
					if ( pMidifile != NULL && iTrack >= 0 )
					{
						pMidifile->SetInstrumentNameOnTrack( iTrack, szText );
					}
				}
				else
				{
					ZeroMemory ( szText, sizeof(szText) );
					pCombo->GetWindowTextA(szText, sizeof(szText));
					m_Tracks_List.SetItemText( iItem, iSubItem, szText );

					//
					if ( pMidifile != NULL && iTrack >= 0 )
					{
						pMidifile->SetInstrumentNameOnTrack( iTrack, szText );
					}
				}

				if ( pMididoc != NULL )
				{
					CMWFormView::SetModified ( TRUE );
					pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_EVENTS );
				}
			}

			return 1;
		}
	}


	return 0;
}

//
//////////////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////////////
BOOL CTracksView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bRes = FriendEraseBkgndScrollView(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

	return CMWFormView::OnEraseBkgnd ( pDC );

}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnBegindragTracksList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMidiDoc *pMididoc = dynamic_cast< CMidiDoc * >( GetDocument () );

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CPoint      ptAction;
	UINT        nFlags;

	GetCursorPos(&ptAction);
	ScreenToClient(&ptAction);
	if ( ! m_bDragging )
	{
		m_hItemDrag = m_Tracks_List.HitTest(ptAction, &nFlags);

		//		If the selected object is not the track zero
		if ( m_hItemDrag >= 0 )
		{
			if ( m_Tracks_List.GetItemData ( m_hItemDrag ) > 0 )
			{
				m_bDragging = TRUE;
				m_hItemDrop = -1;

				if ( m_pImageList == NULL )
				{
					//		Get the image list for dragging
					POINT		point;
					m_pImageList =
						m_Tracks_List.CreateDragImage ( m_hItemDrag, &point );
	//				m_pImageList->DragShowNolock(TRUE);
					m_pImageList->SetDragCursorImage ( 0, CPoint ( 0, 0 ) );
					m_pImageList->BeginDrag(0, CPoint(0,0));
					m_pImageList->DragEnter(this, ptAction);
					m_pImageList->DragMove(ptAction);

				}

				SetCapture();
			}
			else
			{
				m_hItemDrag = -1;

				if ( pMididoc != NULL )
				{
					CMWFormView::SetModified ( TRUE );
					pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS );
				}

				MessageBeep ( 0 );
			}
		}
		else
		{
			m_hItemDrag = -1;

			if ( pMididoc != NULL )
			{
				CMWFormView::SetModified ( TRUE );
				pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS );
			}

			MessageBeep ( 0 );
		}
	}

	
	*pResult = 0;
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if ( m_bDragging )
	{
		m_hItemDrop = m_Tracks_List.HitTest(point, &nFlags);
		if ( m_hItemDrop != -1 )
		{
			UnSelectAll ( );
			m_Tracks_List.SetItemState ( m_hItemDrop, LVIS_SELECTED, LVIS_SELECTED );
		}

		if ( m_pImageList != NULL )
		{
			m_pImageList->DragMove(point);
		}
	}

	CMWFormView::OnMouseMove(nFlags, point);
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnButtonUp();
	
	CMWFormView::OnLButtonUp(nFlags, point);
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnButtonUp()
{
	CMidiDoc			*pMididoc = dynamic_cast< CMidiDoc * >( GetDocument () );

	if ( m_bDragging )
	{
		if ( m_pImageList != NULL )
		{
			m_pImageList->DragLeave(this);
			m_pImageList->EndDrag();
			delete m_pImageList;
			m_pImageList = NULL;
		}

		if ( m_hItemDrag != m_hItemDrop && m_hItemDrop >= 0 )
		{
			char	szMessage [ MAX_LONG_STRING ];

			sprintf_s ( 
				szMessage, 
				sizeof ( szMessage ),
				"Do you want to merge track %d in track %d", 
				m_Tracks_List.GetItemData ( m_hItemDrag ),
				m_Tracks_List.GetItemData ( m_hItemDrop ) );
			int iYesNo = MessageBox ( szMessage, NULL, MB_YESNO | MB_ICONEXCLAMATION );
			if ( iYesNo == IDYES )
			{
				Merge ( (int) m_Tracks_List.GetItemData ( m_hItemDrag ),
						(int) m_Tracks_List.GetItemData ( m_hItemDrop ));
				if ( pMididoc != NULL )
				{
					CMWFormView::SetModified ( TRUE );
					pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS );
				}
			}
			else
			{
				if ( pMididoc != NULL )
				{
					CMWFormView::SetModified ( TRUE );
					pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS );
				}
			}
		}
		else
		{
			if ( pMididoc != NULL )
			{
				CMWFormView::SetModified ( TRUE );
				pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS );
			}

			MessageBeep(0);
		}

		m_hItemDrag = -1;
		m_hItemDrop = -1;
		m_bDragging = FALSE;

		ReleaseCapture();

	}


}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::UnSelectAll()
{
	int			iX;

	for ( iX = 0; iX < m_Tracks_List.GetItemCount ( ); iX++ )
	{
		m_Tracks_List.SetItemState ( iX, 0, LVIS_SELECTED );
	}

}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::Merge (int iFrom, int iTo)
{
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;

	// check that one item gets the focus
	pMidifile	= NULL;
	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		pMidifile->MoveEventFromTrackToTrack (
			MIDI_EVENT_ALL_EVENTS, iFrom, iTo );
		CMWFormView::SetModified ( TRUE );
		m_iCurrentTrack = iTo;
		pMididoc->SetCurrentTrack ( iTo );
		pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS | RIGHT_MASK_EVENTS );
	}

	return;
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnInitialUpdate() 
{
	CMWFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	if ( ! theApp.m_bSplitterWindow )
	{
		GetParentFrame ( )->RecalcLayout ( );
		if ( GetParentFrame ( ) != dynamic_cast< CMainFrame * >( AfxGetMainWnd() ) )
		{
			ResizeParentToFit ( FALSE );
			ResizeParentToFit ( TRUE );
		}
	}
	
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	FriendActivate( bActivate, pActivateView, pDeactiveView, false);
	
	CMWFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnEditCut() 
{
	// TODO: Add your command handler code here
	if ( m_Tracks_List.GetSelectedCount () == 1 )
	{
		m_hItemDrag = m_Tracks_List.GetCurSel ( );
	}	
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnEditPaste() 
{
	CMidiDoc			*pMididoc = dynamic_cast< CMidiDoc * >( GetDocument () );

	// TODO: Add your command handler code here
	if ( m_hItemDrag >= 0 )
	{
		m_hItemDrop = m_Tracks_List.GetCurSel ( );

		if ( m_hItemDrag != m_hItemDrop && m_hItemDrop >= 0 )
		{
			char	szMessage [ MAX_LONG_STRING ];

			sprintf_s ( 
				szMessage, 
				sizeof ( szMessage ),
				"Do you want to merge track %d in track %d", 
				m_Tracks_List.GetItemData ( m_hItemDrag ),
				m_Tracks_List.GetItemData ( m_hItemDrop ) );
			int iYesNo = 
				MessageBox ( szMessage, NULL, 
								MB_YESNO | MB_ICONEXCLAMATION );
			if ( iYesNo == IDYES )
			{
				Merge ( (int) m_Tracks_List.GetItemData ( m_hItemDrag ),
						(int) m_Tracks_List.GetItemData ( m_hItemDrop ));
				if ( pMididoc != NULL )
				{
					CMWFormView::SetModified ( TRUE );
				}
			}
			else
			{
				if ( pMididoc != NULL )
				{
					pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS );
				}
			}
		}
		else
		{
			if ( pMididoc != NULL )
			{
				pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS );
			}

			MessageBeep(0);
		}

		m_hItemDrag = -1;
		m_hItemDrop = -1;

	}

}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( m_hItemDrag >= 0 );	
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable ( m_Tracks_List.GetSelectedCount () == 1	);
}


//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnMove(int x, int y)
{
	CMWFormView::OnMove(x, y);

#if 1
	EnableScrollBarCtrl(SB_BOTH, FALSE);

	RECT	clientRect;
	GetClientRect ( &clientRect );

	if ( m_Tracks_List.m_hWnd != NULL )
	{
		m_Tracks_List.MoveWindow ( &clientRect, true );
	}
#endif

}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CTracksView::OnSize(UINT nType, int cx, int cy)
{
	CMWFormView::OnSize(nType, cx, cy);

#if 1
	EnableScrollBarCtrl(SB_BOTH, FALSE);

	RECT	clientRect;
	GetClientRect ( &clientRect );

	if ( m_Tracks_List.m_hWnd != NULL )
	{
		m_Tracks_List.MoveWindow ( &clientRect, true );
	}
#endif
}
