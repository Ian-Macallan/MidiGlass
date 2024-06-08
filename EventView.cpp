// EventView.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "EventView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "MidiDoc.h"
#include "MidiFile.h"

#include "EventDetail.h"
#include "MainFrm.h"
#include "FindEvent.h"
#include "WaitTreatment.h"
#include "MWMenu.h"

extern CMidiWorksApp theApp;

//
///////////////////////////////////////////////////////////////////////////////////
// CEventView
//
///////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CEventView, CMWFormView)

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CEventView::CEventView() : CMWFormView(CEventView::IDD)
{
	//{{AFX_DATA_INIT(CEventView)
	//}}AFX_DATA_INIT
	m_pTrackEvents		= NULL;
	m_bFirstTime		= true;
	m_iCurrentIndice	= 0;
	for ( int i = 0; i < MAX_LINES; i++ )
	{
		m_MidiList [ i ] = NULL;
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CEventView::~CEventView()
{
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::DoDataExchange(CDataExchange* pDX)
{
	CMWFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventView)
	DDX_Control(pDX, IDC_SPIN, m_Spin);
	DDX_Control(pDX, IDC_TRACKNAME, m_TrackName);
	DDX_Control(pDX, IDC_TRACK_EVENT_LIST, m_TrackEvents);
	//}}AFX_DATA_MAP
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CEventView, CMWFormView)
	//{{AFX_MSG_MAP(CEventView)
	ON_NOTIFY(NM_DBLCLK, IDC_TRACK_EVENT_LIST, OnDblclkTrackEventList)
	ON_NOTIFY(LVN_KEYDOWN, IDC_TRACK_EVENT_LIST, OnKeydownTrackEventList)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EVENTS_DETAIL, OnEventsDetail)
	ON_COMMAND(ID_EVENTS_DELETE, OnEventsDelete)
	ON_COMMAND(ID_EVENTS_REMOVE, OnEventsRemove)
	ON_COMMAND(ID_EVENTS_INSERT_BEFORE, OnEventsInsertBefore)
	ON_COMMAND(ID_EVENTS_INSERT_AFTER, OnEventsInsertAfter)
	ON_COMMAND(ID_EVENTS_MODIFY, OnEventsModify)
	ON_COMMAND(ID_EVENTS_HELP, OnEventsHelp)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_TRACK_EVENT_LIST, OnGetdispinfoTrackEventList)
	ON_EN_CHANGE(IDC_TRACKNAME, OnChangeTrackname)
	ON_NOTIFY(NM_CLICK, IDC_TRACK_EVENT_LIST, OnClickTrackEventList)
	ON_WM_MOVE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
///////////////////////////////////////////////////////////////////////////////////
// CEventView diagnostics
//
///////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CEventView::AssertValid() const
{
	CMWFormView::AssertValid();
}

void CEventView::Dump(CDumpContext& dc) const
{
	CMWFormView::Dump(dc);
}
#endif //_DEBUG

//
///////////////////////////////////////////////////////////////////////////////////
// CEventView message handlers
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::DisplayValues()
{
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;
	unsigned int		iX;
	unsigned int		iTrackNumber;

	if ( m_pTrackEvents == NULL )
	{
		iX = 0;
		m_pTrackEvents = &m_TrackEvents;

		m_TrackEvents.InsertColumn ( iX++, "HMSm",		LVCFMT_LEFT, 72 + 20);
		m_TrackEvents.InsertColumn ( iX++, "MBT",		LVCFMT_LEFT, 72);
		m_TrackEvents.InsertColumn ( iX++, "Event",		LVCFMT_LEFT, 96);
		m_TrackEvents.InsertColumn ( iX++, "ch#",		LVCFMT_RIGHT, 32);
		m_TrackEvents.InsertColumn ( iX++, "Text",		LVCFMT_LEFT, 128);
		m_TrackEvents.InsertColumn ( iX++, "Duration",	LVCFMT_RIGHT, 64);
		m_TrackEvents.InsertColumn ( iX++, "v#1",		LVCFMT_RIGHT, 32);
		m_TrackEvents.InsertColumn ( iX++, "v#2",		LVCFMT_RIGHT, 32);
		m_TrackEvents.InsertColumn ( iX++, "v#3",		LVCFMT_RIGHT, 32);
		m_TrackEvents.InsertColumn ( iX++, "v#4",		LVCFMT_RIGHT, 32);
		m_TrackEvents.InsertColumn ( iX++, "v#5",		LVCFMT_RIGHT, 32);
		m_TrackEvents.InsertColumn ( iX++, "v#6",		LVCFMT_RIGHT, 32);

		m_TrackEvents.SetExtendedStyle ( LVS_EX_FULLROWSELECT );

		m_TrackEvents.SetImageList ( &theApp.m_imgImageEvents16x32, LVSIL_SMALL );
		m_TrackEvents.SetImageList ( &theApp.m_imgImageEvents32x32, LVSIL_NORMAL );

		m_Spin.SetRange ( 0, MAX_TRACKS - 1 );
	}

	//
	m_TrackEvents.DeleteAllItems ();

	//
	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		//		Recalc Addresses
		CWaitTreatment::BeginWait();
		pMidifile->Update ();
		CWaitTreatment::EndWait();

		//		And skip the previous pages.
		iTrackNumber = pMididoc->GetCurrentTrack ();
		m_Spin.SetPos ( iTrackNumber );

		FillEventTableAddress();
	}

	if (    m_iCurrentIndice >= 0 &&
			m_iCurrentIndice < m_TrackEvents.GetItemCount () )
	{
		m_TrackEvents.EnsureVisible ( m_iCurrentIndice, FALSE );
	}

	UnSelectAll ();
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	//
	if ( m_bFirstTime || ( lHint & RIGHT_MASK_EVENTS ) || lHint == 0 )
	{
		EnableScrollBarCtrl(SB_BOTH, FALSE);

		RECT	clientRect;
		GetClientRect ( &clientRect );

		if ( m_TrackEvents.m_hWnd != NULL )
		{
			RECT	nameRect;
			m_TrackName.GetWindowRect ( &nameRect );
			ScreenToClient ( &nameRect );
			clientRect.top = nameRect.bottom + 10;

			m_TrackEvents.MoveWindow ( &clientRect, true );
		}

		DisplayValues ();
		m_bFirstTime = false;
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::OnDblclkTrackEventList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//
	ShowEventDetail();
	
	*pResult = 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMidiList * CEventView::GetSelectedItem()
{
	int			iX;
	BOOL		bOk;

	// check that one item gets the focus
	iX		= 0;
	bOk		= FALSE;
	while ( ! bOk && ( iX < m_TrackEvents.GetItemCount() ) )
	{
		bOk = m_TrackEvents.GetItemState( iX, LVIS_SELECTED) & LVIS_SELECTED;
		if ( bOk )
		{
			return m_MidiList [ iX ];
		}

		iX++;
	}

	return NULL;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMidiList * CEventView::GetCurrentMidiEvent()
{
	return ( m_pCurrentEvent ); 
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::OnKeydownTrackEventList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	//

	if ( pLVKeyDow->wVKey == VK_DELETE )
	{
		AskAndDeleteEvents ( false );
	}
	else if ( pLVKeyDow->wVKey == VK_INSERT )
	{
		InsertEvents ( true );
	}
	
	*pResult = 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
unsigned CEventView::GetIDD()
{
	return ( IDD );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::InsertEvents( bool bBefore )
{
	InsertEventDetail(bBefore);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::ModifyEvents()
{
	ModifyEventDetail();
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::DeleteEvents(bool bRemove )
{
	int					iX;
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;
	CMidiList			*pMidilist;
	BOOL				bOk;
	int					iCountDeleted;

	// check that one item gets the focus
	pMidifile	= NULL;
	pMididoc	= dynamic_cast<CMidiDoc * >( GetDocument () );
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile == NULL )
	{
		return;
	}

	iCountDeleted = 0;

	if ( pMidifile != NULL )
	{
		iX			= 0;

		while ( iX < m_TrackEvents.GetItemCount() )
		{
			bOk = m_TrackEvents.GetItemState( iX, LVIS_SELECTED) & LVIS_SELECTED;
			if ( bOk )
			{
				pMidilist = m_MidiList [ iX ];
				if ( pMidilist != NULL )
				{
					m_MidiList [ iX ]	= NULL;
					int			iTrack  = pMididoc->GetCurrentTrack ();
					CMidiTrack	*pTrack = pMidifile->GetMidiTrackAddress ( iTrack );
					if ( bRemove )
					{
						pTrack->DeleteEventNoAdjust ( pMidilist );
					}
					else
					{
						pTrack->DeleteEventAdjustTime ( pMidilist );
					}
					iCountDeleted++;
				}
			}

			iX++;
		}
	}

	if ( iCountDeleted != 0 )
	{
		CMWFormView::SetModified ( TRUE );
		pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS | RIGHT_MASK_EVENTS );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	//
	CRect		rect;
	GetClientRect ( &rect );
	ClientToScreen ( &rect );

	if ( rect.PtInRect ( point ) )
	{
		CMWMenu		menu;
		menu.LoadMenu ( IDR_MENU_EVENTS );
		m_pContextMenu = menu.GetSubMenu ( 0 );
		m_pContextMenu->TrackPopupMenu (
			TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, 
			point.x, point.y, this );
		return;
	}
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::OnEventsDetail() 
{
	//
	ShowEventDetail();
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::ShowEventDetail()
{
	CEventDetail		dlg;
	CMidiDoc			*pMididoc;

	// check that one item gets the focus
	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	m_iCurrentIndice = m_TrackEvents.GetSelectionMark ();
	if ( m_iCurrentIndice >= 0 )
	{
		m_pCurrentEvent = m_MidiList [ m_iCurrentIndice ];
		if ( m_pCurrentEvent != NULL )
		{
			dlg.setShow ( m_pCurrentEvent );
			if ( dlg.DoModal () == IDOK )
			{
				if ( pMididoc != NULL )
				{
					pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS | RIGHT_MASK_EVENTS );
				}
			}
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::InsertEventDetail(bool bBefore)
{
	CEventDetail		dlg;
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;

	// check that one item gets the focus
	pMidifile	= NULL;
	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile == NULL )
	{
		return;
	}

	m_iCurrentIndice = m_TrackEvents.GetSelectionMark ();
	if ( m_iCurrentIndice >= 0 )
	{
		m_pCurrentEvent = m_MidiList [ m_iCurrentIndice ];
		if ( m_pCurrentEvent != NULL )
		{
			dlg.setInsert( m_pCurrentEvent );
			if ( dlg.DoModal () == IDOK )
			{
				CMidiList *pNewList = dlg.getNewMidiList();

				int			iTrack  = pMididoc->GetCurrentTrack ();
				CMidiTrack	*pTrack = pMidifile->GetMidiTrackAddress ( iTrack );
				if ( bBefore )
				{
					pTrack->InsertEventBefore ( m_pCurrentEvent, pNewList );
				}
				else
				{
					pTrack->InsertEventAfter ( m_pCurrentEvent, pNewList );
				}

				CWaitTreatment::BeginWait();
				pMidifile->Update();
				CWaitTreatment::EndWait();

				CMWFormView::SetModified ( TRUE );
				pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS | RIGHT_MASK_EVENTS );
			}
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::ModifyEventDetail()
{
	CEventDetail		dlg;
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;

	// check that one item gets the focus
	pMidifile	= NULL;
	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile == NULL )
	{
		return;
	}

	m_iCurrentIndice = m_TrackEvents.GetSelectionMark ();
	if ( m_iCurrentIndice >= 0 )
	{
		m_pCurrentEvent = m_MidiList [ m_iCurrentIndice ];
		if ( m_pCurrentEvent != NULL )
		{
			dlg.setModify( m_pCurrentEvent );

			if ( dlg.DoModal () == IDOK )
			{
				CMidiList *pNewList = dlg.getNewMidiList();

				int			iTrack  = pMididoc->GetCurrentTrack ();
				CMidiTrack	*pTrack = pMidifile->GetMidiTrackAddress ( iTrack );
				pTrack->ChangeCurrentEvent ( m_pCurrentEvent, pNewList );

				CWaitTreatment::BeginWait();
				pMidifile->Update();
				CWaitTreatment::EndWait();

				CMWFormView::SetModified ( TRUE );
				pMididoc->UpdateAllViews ( NULL, LEFT_MASK_TRACKS | RIGHT_MASK_EVENTS );
			}
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::AskAndDeleteEvents(bool bRemove)
{
	CString		Title;

	if ( GetSelectedItem() != NULL )
	{
		Title.LoadString ( AFX_IDS_APP_TITLE );
		char		szText [ MAX_LONG_STRING ];

		sprintf_s ( szText, sizeof ( szText ),
					"Delete %d Event(s)", m_TrackEvents.GetSelectedCount ( ) );
		if ( MessageBox ( szText, Title, 
			MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 ) == IDYES )
		{
			DeleteEvents ( bRemove );
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::OnEventsInsertBefore() 
{
	//
	InsertEvents(true);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::OnEventsInsertAfter() 
{
	//
	InsertEvents(false);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::OnEventsModify() 
{
	//
	ModifyEvents();
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::OnEventsDelete() 
{
	//
	AskAndDeleteEvents( false );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::OnEventsRemove() 
{
	//
	AskAndDeleteEvents( true );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::OnEventsHelp() 
{
	//
	CAboutDlg	dlg;
	dlg.DoModal ();
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::OnGetdispinfoTrackEventList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	//

	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;
	CMidiList			*pList;
	CMidiTrack			*pTrack;
	unsigned char		iChannel;
	char				szLimitText [ MAX_STRING ];

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		pTrack = pMidifile->GetMidiTrackAddress ( pMididoc->GetCurrentTrack () );
		if ( pDispInfo->item.iItem < MAX_LINES - 1 )
		{
			pList = m_MidiList [ pDispInfo->item.iItem ];
		}
		else
		{
			pList = NULL;
		}

		if(pDispInfo->item.mask & LVIF_STATE && pList != NULL )
		{
			//  Fill in the state information.
//            pDispInfo->item.state |= rndItem.state;
		}

        if(pDispInfo->item.mask & LVIF_IMAGE && pList != NULL )
		{
			//	Fill in the image information.
			pDispInfo->item.iImage = pList->GetEventIndex ();
		}

        if(pDispInfo->item.mask & LVIF_PARAM && pList != NULL )
		{
//			pDispInfo->item.lParam = (LPARAM ) pList;
		}

        if(pDispInfo->item.mask & LVIF_INDENT && pList != NULL )
		{
//			pDispInfo->item...
		}

        if ( pDispInfo->item.mask & LVIF_TEXT && pList != NULL )
		{
			unsigned char		*pValues;

			pValues = pList->GetValues ( );

			//  Fill in the text information.
            switch (pDispInfo->item.iSubItem)
			{
				case 0:
				{
					//  Copy the main item text.
					strcpy_s (	pDispInfo->item.pszText, pDispInfo->item.cchTextMax, ( char  * ) pList->GetHMSText( ) );
                    break;
				}
				case 1:
				{
                    //  Copy subitem 1 text.
                    strcpy_s (	pDispInfo->item.pszText, pDispInfo->item.cchTextMax, ( char * ) pList->GetMesureText( ) );
                    break;
				}

				//
				case 2:
				{
                    //  Copy subitem 2 text.
                    strcpy_s (	pDispInfo->item.pszText, pDispInfo->item.cchTextMax, ( char * ) pList->GetEventText ( ) );
                    break;
				}

				//
				case 3:
				{
                    //  Copy subitem 2 text.
					if ( pList->useChannel() )
					{
						iChannel = pList->GetChannel();
						sprintf_s ( pDispInfo->item.pszText, pDispInfo->item.cchTextMax, "%02d", iChannel );
					}
					else
					{
						strcpy_s ( pDispInfo->item.pszText, pDispInfo->item.cchTextMax, "NA" );
					}
                    break;
				}
				case 4:
				{
                        //  Copy subitem 2 text.
					if ( pList->GetTextAddress () != NULL )
					{
						memset ( szLimitText, 0, sizeof ( szLimitText ) );
						strncpy_s ( szLimitText, sizeof ( szLimitText ), ( char * ) pList->GetTextAddress (),
									sizeof ( szLimitText ) - 1 );
                        strcpy_s ( pDispInfo->item.pszText, pDispInfo->item.cchTextMax, szLimitText );
					}
					else
					{
						if ( pList->GetExtraText () != NULL )
						{
							memset ( szLimitText, 0, sizeof ( szLimitText ) );
							strncpy_s ( szLimitText, sizeof ( szLimitText ), pList->GetExtraText (),
										sizeof ( szLimitText ) - 1 );
							strcpy_s (	pDispInfo->item.pszText, pDispInfo->item.cchTextMax, szLimitText );
						}
						else
						{
							strcpy_s (	pDispInfo->item.pszText, pDispInfo->item.cchTextMax, "" );
						}
					}
                    break;
				}

				//	Note Duration
				case 5:
				{
					if ( pList->GetEventIndex ( ) == MIDI_EVENT_NOTEON )
					{
						sprintf_s (	pDispInfo->item.pszText, pDispInfo->item.cchTextMax,"%d", pList->GetNoteDuration () );
					}
					else
					{
						pDispInfo->item.pszText = "NA";
					}
                    break;
				}

				//
				case 6:
				{
					if ( pList->valueCount() >= 1 )
					{
						sprintf_s (	pDispInfo->item.pszText, pDispInfo->item.cchTextMax,"%03d", pValues [ 0 ] );
					}
					else
					{
						sprintf_s (	pDispInfo->item.pszText, pDispInfo->item.cchTextMax,"" );
					}
                    break;
				}

				case 7:
				{
					if ( pList->valueCount() >= 2 )
					{
	                    sprintf_s (	pDispInfo->item.pszText, pDispInfo->item.cchTextMax,"%03d", pValues [ 1 ] );
					}
					else
					{
						sprintf_s (	pDispInfo->item.pszText, pDispInfo->item.cchTextMax,"" );
					}
                    break;
				}

				case 8:
				{
					if ( pList->valueCount() >= 3 )
					{
	                    sprintf_s (	pDispInfo->item.pszText, pDispInfo->item.cchTextMax,"%03d", pValues [ 2 ] );
					}
					else
					{
						sprintf_s (	pDispInfo->item.pszText, pDispInfo->item.cchTextMax,"" );
					}
                    break;
				}

				case 9:
				{
					if ( pList->valueCount() >= 4 )
					{
	                    sprintf_s (	pDispInfo->item.pszText, pDispInfo->item.cchTextMax, "%03d", pValues [ 3 ] );
					}
					else
					{
						sprintf_s (	pDispInfo->item.pszText, pDispInfo->item.cchTextMax,"" );
					}
                    break;
				}

				case 10:
				{
					if ( pList->valueCount() >= 5 )
					{
	                    sprintf_s (	pDispInfo->item.pszText, pDispInfo->item.cchTextMax,"%03d", pValues [ 4 ] );
					}
					else
					{
						sprintf_s (	pDispInfo->item.pszText, pDispInfo->item.cchTextMax,"" );
					}
                    break;
				}
				case 11:
				{
					if ( pList->valueCount() >= 6 )
					{
	                    sprintf_s (	pDispInfo->item.pszText, pDispInfo->item.cchTextMax,"%03d", pValues [ 5 ] );
					}
					else
					{
						sprintf_s (	pDispInfo->item.pszText, pDispInfo->item.cchTextMax,"" );
					}
                    break;
				}

				default:
				{
                    break;
				}
			}

		}
	}

	*pResult = 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::OnChangeTrackname() 
{
	//
	// send this notification unless you override the CMWFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	//
	
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;
	int					iTrack;

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		if ( ! m_bFirstTime )
		{
			iTrack = m_Spin.GetPos ( );
			pMididoc->SetCurrentTrack ( iTrack );

			FillEventTableAddress();
		}
	}	

	if ( ! m_bFirstTime )
	{
		UnSelectAll ();
	}

	m_iCurrentIndice	= 0;

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::FillEventTableAddress()
{
	CMidiDoc			*pMididoc;
	CMidiFile			*pMidifile;
	CMidiTrack			*pTrack;
	unsigned int		iTrackNumber;
	unsigned int		iX;

	//	First fill the list with NULL
	for ( iX = 0; iX < MAX_LINES; iX++ )
	{
		m_MidiList [ iX ] = NULL;
	}

	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
	pMidifile	= NULL;
	if ( pMididoc != NULL )
	{
		pMidifile = pMididoc->GetDocumentMidiFile ();
	}

	if ( pMidifile != NULL )
	{
		iTrackNumber = pMididoc->GetCurrentTrack ();

		pTrack = pMidifile->GetMidiTrackAddress ( iTrackNumber );

		//		Because this routine can be called
		//		Thru on change track name
		//		The events
		if ( m_pTrackEvents != NULL )
		{
			m_TrackEvents.SetItemCount ( pMidifile->GetMidiTrackSize ( iTrackNumber ) );
		}

		//	Then With values
		iX = 0;
		m_MidiList [ iX ] = pTrack->GetFirstAddress ( );

		while ( iX < pMidifile->GetMidiTrackSize ( iTrackNumber ) && m_MidiList [ iX ] != NULL )
		{
			if ( iX < MAX_LINES - 1 )
			{
				m_MidiList [ iX + 1 ] = m_MidiList [ iX ]->GetNext ( );
			}
			iX++;
		}
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::OnClickTrackEventList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//
	m_iCurrentIndice = m_TrackEvents.GetSelectionMark ( );

	*pResult = 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::SetSearchInfo(	const CString strEvent, 
								const CString strProgram,
								const CString strController,
								int V1_From, int V1_To, 
								int V2_From, int V2_To )
{
	m_strEventSearched		= strEvent;
	m_strProgramSearched	= strProgram;
	m_strControllerSearched	= strController;
	m_V1_From				= V1_From;
	m_V1_To					= V1_To;
	m_V2_From				= V2_From;
	m_V2_To					= V2_To;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::UnSelectAll()
{
	int			iX;

	for ( iX = 0; iX < m_TrackEvents.GetItemCount ( ); iX++ )
	{
		m_TrackEvents.SetItemState ( iX, 0, LVIS_SELECTED );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::SearchEvents()
{
	int				iX;
	int				iCount;
	int				iFirstFound;

	//		Un Select All
	UnSelectAll ();

	//		First set the starting point
	iCount = m_TrackEvents.GetItemCount ( );
	if ( m_iCurrentIndice >= iCount )
	{
		m_iCurrentIndice = 0;
	}

	//		Now search
	unsigned		iEventIndex;
	int				iEventProgram;
	int				iEventController;

	CMidiClass		classTemp;

	iEventIndex			= classTemp.SearchClassEventText ( m_strEventSearched );

	iEventProgram		= classTemp.SearchProgramByText ( m_strProgramSearched );

	iEventController	= classTemp.SearchControlerByText ( m_strControllerSearched );

	iFirstFound		= -1;
	for ( iX = m_iCurrentIndice; iX < iCount; iX++ )
	{
		bool		bFound = false;

		if ( m_MidiList [ iX ] == NULL )
		{
			return;
		}

		if (	iEventIndex == m_MidiList [ iX ]->GetEventIndex ( ) 
			 && ( iEventProgram == m_MidiList [ iX ]->GetValue1 ( ) 
					|| iEventProgram == -1 )
			 && ( iEventController == m_MidiList [ iX ]->GetValue1 ( ) 
					|| iEventController == -1 ) )
		{
			if (	m_MidiList [ iX ]->GetValue1 ( ) >= m_V1_From
				 && m_MidiList [ iX ]->GetValue1 ( ) <= m_V1_To 
				 && m_MidiList [ iX ]->GetValue2 ( ) >= m_V2_From
				 && m_MidiList [ iX ]->GetValue2 ( ) <= m_V2_To   )
			{
				bFound = true;
			}
		}

		if ( bFound )
		{
			if ( iFirstFound == -1 )
			{
				iFirstFound = iX;
			}

			m_TrackEvents.SetItemState ( iX, LVIS_SELECTED, LVIS_SELECTED );
		}
	}

	if ( iFirstFound != -1 )
	{
		m_iCurrentIndice = iFirstFound;
		m_TrackEvents.EnsureVisible ( m_iCurrentIndice, FALSE );
		m_TrackEvents.SetFocus ( );
	}
	else
	{
		m_iCurrentIndice = 0;
		m_TrackEvents.EnsureVisible ( 0, FALSE );
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::OnInitialUpdate() 
{
	CMWFormView::OnInitialUpdate();
	
	//
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
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::IncrementCurrentIndice()
{
		m_iCurrentIndice += 1;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	//
	FriendActivate( bActivate, pActivateView, pDeactiveView, false);
	
	CMWFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::OnMove(int x, int y)
{
	CMWFormView::OnMove(x, y);

#if 1

	EnableScrollBarCtrl(SB_BOTH, FALSE);

	RECT	clientRect;
	GetClientRect ( &clientRect );

	if ( m_TrackEvents.m_hWnd != NULL )
	{
		RECT	nameRect;
		m_TrackName.GetWindowRect ( &nameRect );
		ScreenToClient ( &nameRect );
		clientRect.top = nameRect.bottom + 10;

		m_TrackEvents.MoveWindow ( &clientRect, true );
	}

#endif
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CEventView::OnSize(UINT nType, int cx, int cy)
{
	CMWFormView::OnSize(nType, cx, cy);

#if 1

	EnableScrollBarCtrl(SB_BOTH, FALSE);

	RECT	clientRect;
	GetClientRect ( &clientRect );

	if ( m_TrackEvents.m_hWnd != NULL )
	{
		RECT	nameRect;
		m_TrackName.GetWindowRect ( &nameRect );
		ScreenToClient ( &nameRect );
		clientRect.top = nameRect.bottom + 10;

		m_TrackEvents.MoveWindow ( &clientRect, true );
	}

#endif
}
