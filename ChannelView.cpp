//
///////////////////////////////////////////////////////////////////////////////////////////
// ChannelView.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MidiGlassApp.h"
#include "ChannelView.h"

#include "MidiDoc.h"
#include "MidiFile.h"

#include "WaitTreatment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMidiWorksApp theApp;

//
///////////////////////////////////////////////////////////////////////////////////////////
// CChannelView
//
///////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CChannelView, CMWFormView)

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
CChannelView::CChannelView() : CMWFormView(CChannelView::IDD)
{
	//{{AFX_DATA_INIT(CChannelView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bFirstTime	= true;
	m_pChannels		= NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
CChannelView::~CChannelView()
{
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CChannelView::DoDataExchange(CDataExchange* pDX)
{
	CMWFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChannelView)
	DDX_Control(pDX, IDC_CHANNELS, m_Channels);
	//}}AFX_DATA_MAP
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CChannelView, CMWFormView)
	//{{AFX_MSG_MAP(CChannelView)
	ON_WM_CTLCOLOR()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CHANNELS_HELP, OnChannelsHelp)
	ON_COMMAND(ID_CHANNELS_AUDIBLE, OnChannelsAudible)
	ON_COMMAND(ID_CHANNELS_MUTE, OnChannelsMute)
	ON_COMMAND(ID_CHANNELS_MUTEALL, OnChannelsMuteall)
	ON_COMMAND(ID_CHANNELS_AUDIBLEALL, OnChannelsAudibleall)
	ON_COMMAND(ID_CHANNELS_SOLO, OnChannelsSolo)
	ON_NOTIFY(NM_CLICK, IDC_CHANNELS, OnClickChannels)
	ON_NOTIFY(NM_DBLCLK, IDC_CHANNELS, OnDblclkChannels)
	ON_WM_TIMER()
	ON_MESSAGE(WM_MW_EDIT_SELECT,OnEditSelect)
	ON_MESSAGE(WM_MW_EDIT_BEGIN,OnEditBegin)
	ON_MESSAGE(WM_MW_EDIT_END,OnEditEnd)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
///////////////////////////////////////////////////////////////////////////////////////////
// CChannelView diagnostics
//
///////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CChannelView::AssertValid() const
{
	CMWFormView::AssertValid();
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CChannelView::Dump(CDumpContext& dc) const
{
	CMWFormView::Dump(dc);
}
#endif //_DEBUG

//
///////////////////////////////////////////////////////////////////////////////////////////
// CChannelView message handlers
//
///////////////////////////////////////////////////////////////////////////////////////////
void CChannelView::DisplayValues()
{
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;
	CMidiChannel		*pMidichannel;
	int					iX;
	int					iChannel;
	char				szString [ MAX_INT_STRING ];
	const char			*pProgram;
	bool				bResult;
	unsigned char		cVolume;

	if ( m_pChannels == NULL )
	{
		m_pChannels = &m_Channels;

		iX	= 0;

		m_Channels.SetEditFlag ( iX, false );
		m_Channels.InsertColumn ( iX++, "Channel",	LVCFMT_LEFT, 72 + 20);
		m_Channels.SetEditFlag ( iX, false );
		m_Channels.InsertColumn ( iX++, "Muted",	LVCFMT_LEFT, 72);

		m_Channels.InsertColumn ( iX++, "Program",	LVCFMT_LEFT, 128);

		m_Channels.EnableSpinCtrl ( iX );
		m_Channels.SetSpinCtrlRange ( iX, 0, 127 );
		m_Channels.InsertColumn ( iX++, "Volume",	LVCFMT_RIGHT, 48);

		m_Channels.SetExtendedStyle ( LVS_EX_FULLROWSELECT );

		m_Channels.SetImageList ( &theApp.m_imgImageListChannel16x14, LVSIL_SMALL );
//		m_Channels.SetImageList ( &theApp.m_imgImageList32x2, LVSIL_NORMAL );
	}

	//		Empty the box
	m_Channels.ResetContent ();

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		if ( pMidifile->LLIsPlaying ( ) )
		{
			SetTimer ( CHANNEL_TIMER, 500, NULL );
		}
	}

	if ( pMidifile != NULL )
	{
		for ( iChannel = 0; iChannel < MAX_CHANNELS; iChannel++ )
		{
			iX = 0;

			pMidichannel = pMidifile->GetChannelAddress ( iChannel );

			sprintf_s ( szString, sizeof ( szString ), "%d", iChannel );
			m_Channels.InsertItem ( iChannel, szString, iX++ );
			m_Channels.SetItemData ( iChannel, ( DWORD ) iChannel );
			if ( pMidichannel->GetMuteState ( ) )
			{
				m_Channels.SetItem ( iChannel, 0, LVIF_IMAGE, NULL, 1, 0, 0, 0 );
				strcpy_s ( szString, sizeof ( szString ),"Yes" );
			}
			else
			{
				unsigned char			cNote;
				unsigned char			cVolume;
				int						iImage;

				if ( pMidifile->LLIsPlaying () )
				{
					pMidichannel->GetCurrentNoteAndVolume ( &cNote, &cVolume );
					pMidichannel->ResetVolume ( );
					cVolume = cVolume & 0x7f;
					if ( cVolume == 0 )
					{
						iImage = 2;
					}
					else
					{
						iImage	= ( cVolume * 7 ) / 0x7f + 3;
					}

					m_Channels.SetItem ( iX, 0, LVIF_IMAGE, NULL, iImage, 0, 0, 0 );
				}
				else
				{
					m_Channels.SetItem ( iChannel, 0, LVIF_IMAGE, NULL, 0, 0, 0, 0 );
				}
				strcpy_s ( szString, sizeof ( szString ),"No" );
			}

			m_Channels.SetItemText ( iChannel, iX++, szString );

			pProgram = pMidifile->GetChannelProgram ( iChannel );
			if ( pProgram != NULL )
			{
				m_Channels.SetItemText ( iChannel, iX++, pProgram );
			}
			else
			{
				m_Channels.SetItemText ( iChannel, iX++, "NA" );
			}

			bResult = pMidifile->GetChannelVolume ( iChannel, &cVolume );
			if ( bResult )
			{
				sprintf_s ( szString, sizeof ( szString ), "%d", cVolume );
				m_Channels.SetItemText ( iChannel, iX++, szString );
			}
			else
			{
				m_Channels.SetItemText ( iChannel, iX++, "64" );
			}
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
HBRUSH CChannelView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CChannelView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( m_bFirstTime || ( lHint & RIGHT_MASK_CHANNELS ) || lHint == 0 )
	{
		m_bFirstTime = false;

#if 1
		EnableScrollBarCtrl(SB_BOTH, FALSE);

		RECT	clientRect;
		GetClientRect ( &clientRect );

		if ( m_Channels.m_hWnd != NULL )
		{
			m_Channels.MoveWindow ( &clientRect, true );
		}
#endif

		DisplayValues ();
	}
	
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CChannelView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CRect		rect;
	GetClientRect ( &rect );
	ClientToScreen ( &rect );

	if ( rect.PtInRect ( point ) )
	{
		CMenu		menu;
		menu.LoadMenu ( IDR_MENU_CHANNELS );
		CMenu	*pContextMenu = menu.GetSubMenu ( 0 );
		pContextMenu->TrackPopupMenu ( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, this );
		return;
	}
	
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CChannelView::OnChannelsHelp() 
{
	// TODO: Add your command handler code here
	CAboutDlg	dlg;
	dlg.DoModal ();
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CChannelView::OnChannelsAudible() 
{
	// TODO: Add your command handler code here
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	int iX = m_Channels.GetCurSel ();
	if ( iX >= 0 && pMidifile != NULL )
	{
		pMidifile->SetChannelMuteState ( iX, false ) ;
		pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_CHANNELS );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CChannelView::OnChannelsMute() 
{
	// TODO: Add your command handler code here
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	int iX = m_Channels.GetCurSel ();
	if ( iX >= 0 && pMidifile != NULL )
	{
		pMidifile->SetChannelMuteState ( iX, true ) ;
		pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_CHANNELS );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CChannelView::OnChannelsMuteall() 
{
	// TODO: Add your command handler code here
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;
	int					iX;

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		for ( iX = 0; iX < MAX_CHANNELS; iX++ )
		{
			pMidifile->SetChannelMuteState ( iX, true ) ;
		}
		pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_CHANNELS );
	}	
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CChannelView::OnChannelsAudibleall() 
{
	// TODO: Add your command handler code here
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;
	int					iX;

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		for ( iX = 0; iX < MAX_CHANNELS; iX++ )
		{
			pMidifile->SetChannelMuteState ( iX, false ) ;
		}
		pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_CHANNELS );
	}	
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CChannelView::OnChannelsSolo() 
{
	// TODO: Add your command handler code here
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	int iSel = m_Channels.GetCurSel ();
	if ( iSel >= 0 && pMidifile != NULL )
	{
		int iX;
		for ( iX = 0; iX < MAX_CHANNELS; iX++ )
		{
			pMidifile->SetChannelMuteState ( iX, true ) ;
		}
		pMidifile->SetChannelMuteState ( iSel, false ) ;
		pMididoc->UpdateAllViews ( NULL, RIGHT_MASK_CHANNELS );
	}

}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CChannelView::OnClickChannels(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMLISTVIEW			*nmListView = ( NMLISTVIEW * ) pNMHDR;

	m_Channels.EditColumn ( nmListView->iSubItem );

	*pResult = 0;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CChannelView::OnDblclkChannels(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

//
/////////////////////////////////////////////////////////////////
//	Return the pointer on the Edit Object
/////////////////////////////////////////////////////////////////
LRESULT CChannelView::OnEditBegin(WPARAM wParam, LPARAM lParam)
{
	BOOL	bResult;
	int		iItem		= m_Channels.GetItemIndex ( );
	int		iSubItem	= m_Channels.GetSubItemIndex ( );
	char	szText [ MAX_INSTRUMENT_STRING ];

	if ( iSubItem == 2 )
	{
		CMWProgramsComboBox			*pCombo;
		CRect						rectLabel;

		bResult = m_Channels.GetSubItemRect( iItem, iSubItem, LVIR_LABEL, rectLabel );
		rectLabel.bottom += ( ( rectLabel.bottom - rectLabel.top ) * 6 );

		pCombo = new CMWProgramsComboBox( );
		pCombo->Create ( 
			WS_CHILD | WS_BORDER | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | 
			CBS_DROPDOWNLIST, rectLabel, & m_Channels, IDC_EDIT_COLUMNS );
		pCombo->ResetContent (); 

		m_Channels.GetItemText( iItem, iSubItem, szText, sizeof ( szText ) );

		int iFound = pCombo->FindString( 0, szText );
		if ( iFound >= 0 )
		{
			pCombo->SetCurSel ( iFound );
		}

		return ( LRESULT ) pCombo;
	}

	return NULL;

}

//
/////////////////////////////////////////////////////////////////
//	Return 1 if foudn and treated
/////////////////////////////////////////////////////////////////
LRESULT CChannelView::OnEditEnd(WPARAM wParam, LPARAM lParam)
{
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( m_Channels.GetSubItemIndex ( ) == 2 )
	{
		char					szText [ MAX_INSTRUMENT_STRING ];
		CMWProgramsComboBox		*pCombo;
		int						iItem		= m_Channels.GetItemIndex ( );
		int						iSubItem	= m_Channels.GetSubItemIndex ( );


		pCombo = dynamic_cast < CMWProgramsComboBox *>( m_Channels.GetEditControlHandle ( ) );
		if ( pCombo != NULL && pCombo->GetModify() )
		{
			int iFound = pCombo->GetCurSel ( );
			if ( iFound >= 0 )
			{
				pCombo->GetLBText( iFound, szText );
				m_Channels.SetItemText( iItem, iSubItem, szText );

				//		Now Set the chunk
				CMidiProgram	*pProgram = new CMidiProgram ( iItem, iFound );

				if ( pMidifile != NULL )
				{
					//	Apply program if playing
					pMidifile->SetImmediateChunk ( pProgram->GetChunk () );
				}

				delete pProgram;
			}

			m_Channels.SetEditControlHandle ( NULL );
			
			delete pCombo;
		}

		if ( pMidifile != NULL )
		{
			pMidifile->setEditChange ( false );
		}

		return 1;
	}

	if ( m_Channels.GetSubItemIndex ( ) == 3 )
	{
		CMWEditInList			*pEdit;
		char					szText [ MAX_INT_STRING ];
		unsigned				cVolume;
		int						iItem		= m_Channels.GetItemIndex ( );
		int						iSubItem	= m_Channels.GetSubItemIndex ( );

		pEdit = dynamic_cast < CMWEditInList *>( m_Channels.GetEditControlHandle ( ) );
		if ( pEdit != NULL && pEdit->GetModify() )
		{
			pEdit->GetWindowText ( szText, sizeof ( szText ) );
			cVolume = atoi ( szText ); 

			//		Now Set the chunk
			CMidiParameter	*pControler;
			pControler = new CMidiParameter ( iItem, 7, cVolume );

			if ( pMidifile != NULL )
			{
				//	Apply Controller if playing
				pMidifile->SetImmediateChunk ( pControler->GetChunk () );
			}

			delete pControler;
		}

		if ( pMidifile != NULL )
		{
			pMidifile->setEditChange ( false );
		}

		return 1; //2024.01.11
	}


	if ( pMidifile != NULL )
	{
		pMidifile->setEditChange ( false );
	}

	return 0;

}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
LRESULT CChannelView::OnEditSelect(WPARAM wParam, LPARAM lParam)
{
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( m_Channels.GetSubItemIndex ( ) == 2 )
	{
		char				szText [ MAX_INSTRUMENT_STRING ];
		CMWProgramsComboBox	*pCombo;
		int					iItem		= m_Channels.GetItemIndex ( );
		int					iSubItem	= m_Channels.GetSubItemIndex ( );


		pCombo = dynamic_cast<  CMWProgramsComboBox *>( m_Channels.GetEditControlHandle ( ) );
		if ( pCombo != NULL && pCombo->GetModify() )
		{
			int iFound = pCombo->GetCurSel ( );
			if ( iFound >= 0 )
			{
				//		Get the number from combo
				pCombo->GetLBText( iFound, szText );
				m_Channels.SetItemText( iItem, iSubItem, szText );

				//		Send it to midi
				if ( pMidifile != NULL )
				{
					pMidifile->SetChannelProgram ( iItem, iFound );

					//		Now Set the chunk
					CMidiProgram	*pProgram = new CMidiProgram ( iItem, iFound );

					//	Apply Program if playing
					pMidifile->SetImmediateChunk ( pProgram->GetChunk () );

					delete pProgram;
				}
			}
		}

		return 1;
	}

	return 0;
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CChannelView::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	//	Update images for the view
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile == NULL )
	{
		KillTimer ( CHANNEL_TIMER );
		DisplayValues ( );
		return;
	}

	if ( ! pMidifile->LLIsPlaying () )
	{
		KillTimer ( CHANNEL_TIMER );
		DisplayValues ( );
		return;
	}

	CMidiChannel			*pMidichannel;
	int						iX;
	unsigned char			cNote;
	unsigned char			cVolume;
	int						iImage;

	for ( iX = 0; iX < MAX_CHANNELS; iX++ )
	{
		pMidichannel = pMidifile->GetChannelAddress ( iX );
		if ( pMidichannel->GetMuteState ( ) )
		{
			m_Channels.SetItem ( iX, 0, LVIF_IMAGE, NULL, 1, 0, 0, 0 );
		}
		else
		{
			pMidichannel->GetCurrentNoteAndVolume ( &cNote, &cVolume );
			pMidichannel->ResetVolume ( );
			cVolume = cVolume & 0x7f;
			if ( cVolume == 0 )
			{
				iImage = 2;
			}
			else
			{
				iImage	= ( cVolume * 7 ) / 0x7f + 3;
			}

			m_Channels.SetItem ( iX, 0, LVIF_IMAGE, NULL, iImage, 0, 0, 0 );
		}
	}
	
	CMWFormView::OnTimer(nIDEvent);
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
BOOL CChannelView::OnEraseBkgnd(CDC* pDC) 
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
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CChannelView::OnInitialUpdate() 
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
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CChannelView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	FriendActivate( bActivate, pActivateView, pDeactiveView, false);
	
	CMWFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CChannelView::OnSize(UINT nType, int cx, int cy)
{
	CMWFormView::OnSize(nType, cx, cy);

#if 1
	EnableScrollBarCtrl(SB_BOTH, FALSE);

	RECT	clientRect;
	GetClientRect ( &clientRect );

	if ( m_Channels.m_hWnd != NULL )
	{
		m_Channels.MoveWindow ( &clientRect, true );
	}
#endif
}

//
///////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////
void CChannelView::OnMove(int x, int y)
{
	CMWFormView::OnMove(x, y);

#if 1
	EnableScrollBarCtrl(SB_BOTH, FALSE);

	RECT	clientRect;
	GetClientRect ( &clientRect );

	if ( m_Channels.m_hWnd != NULL )
	{
		m_Channels.MoveWindow ( &clientRect, true );
	}

#endif
}
