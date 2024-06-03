// PlayList.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "PlayList.h"
#include "PlayTuneThread.h"
#include "MWObjects.h"

#include "MidiDoc.h"
#include "MidiFile.h"
#include "PlayListProperty.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMidiWorksApp	theApp;

/////////////////////////////////////////////////////////////////////////////
// CPlayList

IMPLEMENT_DYNCREATE(CPlayList, CMWFormView)

CPlayList::CPlayList() : CMWFormView(CPlayList::IDD)
{
	//{{AFX_DATA_INIT(CPlayList)
	//}}AFX_DATA_INIT
	m_bFirstTime = true;

	//		The dragg and drop
	m_bDragging		= false;
	m_hItemDrag		= -1;
	m_hItemDrop		= -1;

	//		Image list
	m_pImageList	= NULL;
}

CPlayList::~CPlayList()
{
}

void CPlayList::DoDataExchange(CDataExchange* pDX)
{
	CMWFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayList)
	DDX_Control(pDX, IDC_ARRANGE, m_Arrange);
	DDX_Control(pDX, IDC_DISPLAY, m_Display);
	DDX_Control(pDX, IDC_PLAYLIST, m_PlayList);
	DDX_Control(pDX, IDC_UP, m_Up);
	DDX_Control(pDX, IDC_STOP, m_Stop);
	DDX_Control(pDX, IDC_PLAY, m_Play);
	DDX_Control(pDX, IDC_DOWN, m_Down);
	DDX_Control(pDX, IDC_SAVE, m_Save);
	DDX_Control(pDX, IDC_REMOVE, m_Remove);
	DDX_Control(pDX, IDC_ADD, m_Add);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlayList, CMWFormView)
	//{{AFX_MSG_MAP(CPlayList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_UP, OnUp)
	ON_BN_CLICKED(IDC_DOWN, OnDown)
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_DBLCLK, IDC_PLAYLIST, OnDblclkPlaylist)
	ON_NOTIFY(NM_KILLFOCUS, IDC_PLAYLIST, OnKillfocusPlaylist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PLAYLIST, OnItemchangedPlaylist)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_PLAYLIST, OnBegindragPlaylist)
	ON_CBN_SELCHANGE(IDC_DISPLAY, OnSelchangeDisplay)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PLAYLIST_OPENFILE, OnPlaylistOpenfile)
	ON_COMMAND(ID_PLAYLIST_HELP, OnPlaylistHelp)
	ON_COMMAND(ID_PLAYLIST_PROPERTY, OnPlaylistProperty)
	ON_BN_CLICKED(IDC_ARRANGE, OnArrange)
	ON_COMMAND(ID_LIST, OnList)
	ON_COMMAND(ID_LARGEICONS, OnLargeicons)
	ON_COMMAND(ID_SMALLICONS, OnSmallicons)
	ON_COMMAND(ID_REPORT, OnReport)
	ON_COMMAND(ID_ARRANGE, OnMenuArrange)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayList diagnostics

#ifdef _DEBUG
void CPlayList::AssertValid() const
{
	CMWFormView::AssertValid();
}

void CPlayList::Dump(CDumpContext& dc) const
{
	CMWFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPlayList message handlers

void CPlayList::OnAdd() 
{
	// TODO
	static char BASED_CODE szFilter[] = 
		"Midi Files|*.mid|KAR Files|*.kar|RMI Files|*.rmi|All Files (*.*)|*.*||";

	CFileDialog l_FileDialog ( TRUE, NULL, "*.mid", 
		OFN_ALLOWMULTISELECT | OFN_READONLY, szFilter );

	/*
	 *		Starts a dialog box.
	 */
	if ( l_FileDialog.DoModal () == IDOK)
	{
		POSITION		pos;

		pos = l_FileDialog.GetStartPosition ( );

		while ( pos != NULL )
		{
			strcpy_s (	m_szFilename, sizeof ( m_szFilename ),
						l_FileDialog.GetNextPathName( pos ) );
			m_PlayList.AddString ( m_szFilename );
			m_bPlayListModified = true;
		}
	}	
	
}

void CPlayList::OnRemove() 
{
	// TODO
	int			iX;

	iX = m_PlayList.GetCurSel ( );
	if ( iX >= 0 &&	iX < m_PlayList.GetCount ( ) )
	{
		m_PlayList.DeleteString ( iX ); 
		m_bPlayListModified = true;
	}

	if ( iX < m_PlayList.GetCount ( ) )
	{
		m_PlayList.SetCurSel ( iX );
	}
	else
	{
		if ( m_PlayList.GetCount ( ) > 0 )
		{
			m_PlayList.SetCurSel ( m_PlayList.GetCount ( ) - 1 );
		}
	}

	DisplayButtons ( );
}

void CPlayList::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO
	if ( m_bFirstTime || ( lHint & RIGHT_MASK_PLAYLIST ) || lHint == 0 )
	{
		m_bFirstTime = false;
		DisplayValues ();
	}
	
}

void CPlayList::DisplayValues()
{
	int			iX;
	char		szString [ MAX_TUNE_STRING ];
	CString		strReturned;

	//
	//		First Read existing keys
	m_PlayList.ResetContent ( );
	m_PlayList.SetExtendedStyle ( LVS_EX_FULLROWSELECT );

	//	m_PlayList.SetBkImage( theApp.m_bmBackEmpty );
	int iSelected = m_Display.GetCurSel ();
	if ( ! ( iSelected >= 0 && iSelected <= 3 ) )
	{
		iSelected = 3;
	}

	m_Display.SetCurSel ( iSelected );

	iX = 0;
	do
	{
		sprintf_s ( szString, sizeof ( szString ), "Tune %03d", iX );
		strReturned =  theApp.GetProfileString( "Play List", szString, NULL );
		if ( strReturned != "" )
		{
			m_PlayList.AddString ( strReturned );
		}
		iX++;
	}
	while ( strReturned != "" );

	iX = m_PlayList.GetCurSel ( );
	if ( ! ( iX >= 0 &&	iX < m_PlayList.GetCount ( ) ) )
	{
		m_PlayList.SetCurSel ( 
			theApp.GetProfileInt( "Play List", "Tune Number", 0 ) );
	}

	DisplayButtons ();
}

void CPlayList::OnSave() 
{
	// TODO
	SaveTheList ( );
	DisplayButtons ();
}

void CPlayList::OnUp() 
{
	// TODO
	int			iX;
	char		szPathname [ MAX_PATHNAME_STRING ];

	iX = m_PlayList.GetCurSel ( );
	if ( iX > 0 &&	iX < m_PlayList.GetCount ( ) )
	{
		m_PlayList.GetText ( iX, szPathname, sizeof ( szPathname ) );
		m_PlayList.DeleteString ( iX );
		m_PlayList.InsertString ( iX - 1, szPathname );
		m_PlayList.SetCurSel ( iX - 1 );
		m_bPlayListModified = true;
	}

}

void CPlayList::OnDown() 
{
	// TODO
	int			iX;
	char		szPathname [ MAX_PATHNAME_STRING ];

	iX = m_PlayList.GetCurSel ( );
	if ( iX >= 0 &&	iX < m_PlayList.GetCount ( ) - 1 )
	{
		m_PlayList.GetText ( iX, szPathname, sizeof ( szPathname ) );
		m_PlayList.DeleteString ( iX );
		m_PlayList.InsertString ( iX + 1, szPathname );
		m_PlayList.SetCurSel ( iX + 1 );
		m_bPlayListModified = true;
	}
}

void CPlayList::OnPlay() 
{
    CMainFrame *pMainFrame = dynamic_cast<CMainFrame *> ( AfxGetMainWnd( ) );
    if ( pMainFrame != NULL )
    {
        CMidiDoc			*pMididoc	= pMainFrame->GetMidiDoc ();
        CMidiFile			*pMidifile	= pMainFrame->GetMidiFile ( );
        if ( pMidifile != NULL )
        {
            if ( pMidifile->LLIsPlaying() )
            {
                pMidifile->LLStop();
            }

            if ( pMidifile->MciIsPlaying() )
            {
                pMidifile->MciStop();
            }
        }
    }

	// TODO
	if ( theApp.m_PlayListThread == NULL && theApp.m_PlayDirThread == NULL )
	{
		theApp.m_PlayListThread = AfxBeginThread ( RUNTIME_CLASS ( CPlayTuneThread ) );
	}
	DisplayButtons ();
}

void CPlayList::OnStop() 
{
	// TODO
	if ( theApp.m_PlayListThread != NULL )
	{
		theApp.m_dlgPlayerDialogList.EndDialog ( 0 );
	}
	DisplayButtons ();
}

int CPlayList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMWFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO
	m_bPlayListModified = false;
	
	return 0;
}

void CPlayList::SaveTheList()
{
	int			iCount;
	int			iX;
	char		szPathname [ MAX_PATHNAME_STRING ];
	char		szString [ MAX_TUNE_STRING ];
	CString		strReturned;

	if ( ! m_bPlayListModified )
	{
		return;
	}

	//
	//		First Delete existing keys
	iX = 0;
	do
	{
		sprintf_s ( szString, sizeof ( szString ), "Tune %03d", iX );
		strReturned =  theApp.GetProfileString( "Play List", szString, NULL );
		if ( strReturned != "" )
		{
			theApp.WriteProfileString( "Play List", szString, NULL );
		}
		iX++;
	}
	while ( strReturned != "" );

	//
	//		Then store the values
	iCount = m_PlayList.GetCount ( );
	iX = 0;
	while ( iX < iCount )
	{
		sprintf_s ( szString, sizeof ( szString ), "Tune %03d", iX );
		m_PlayList.GetText ( iX, szPathname, sizeof ( szPathname ) );
		theApp.WriteProfileString( "Play List", szString, szPathname );
		iX++;
	}

	iX = m_PlayList.GetCurSel ( );
	if ( iX >= 0 )
	{
		theApp.WriteProfileInt( "Play List", "Tune Number", iX );
	}

	iX = m_PlayList.GetCount ( );
	theApp.WriteProfileInt( "Play List", "Tune Count", iX );

	m_bPlayListModified = false;
}

void CPlayList::DisplayButtons()
{

	if ( m_bPlayListModified )
	{
		m_Save.EnableWindow ( TRUE );
	}
	else
	{
		m_Save.EnableWindow ( FALSE );
	}

	int iX = m_PlayList.GetCurSel ( );
	if ( iX >= 0 && iX < m_PlayList.GetCount ( ) )
	{
		m_Remove.EnableWindow ( TRUE );
		m_Up.EnableWindow ( TRUE );
		m_Down.EnableWindow ( TRUE );
	}
	else
	{
		m_Remove.EnableWindow ( FALSE );
		m_Up.EnableWindow ( FALSE );
		m_Down.EnableWindow ( FALSE );
	}

	if ( theApp.m_PlayListThread != NULL )
	{
		m_Play.EnableWindow ( FALSE );
		m_Stop.EnableWindow ( TRUE );
	}
	else
	{
		m_Play.EnableWindow ( TRUE );
		m_Stop.EnableWindow ( FALSE );
	}

}


void CPlayList::OnDestroy() 
{
	CMWFormView::OnDestroy();
	
	// TODO
	if ( m_bPlayListModified )
	{
		if ( MessageBox ( "Save the Playing List", "Playing List", MB_YESNO ) == IDYES )
		{
			SaveTheList ( );
		}

		m_bPlayListModified = false;
	
	}
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
HBRUSH CPlayList::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CMWFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO
	HBRUSH hBrush = FriendCtlColor(pDC, pWnd, nCtlColor);
	if ( hBrush != NULL )
	{
		return hBrush;
	}

	// TODO
	return hbr;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayList::OnDblclkPlaylist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO

	//		Ask for saving the list
	if ( m_bPlayListModified )
	{
		if ( MessageBox ( "Save the Playing List", "Playing List", MB_YESNO ) == IDYES )
		{
			SaveTheList ();
		}
	}

    CMainFrame *pMainFrame = dynamic_cast<CMainFrame *> ( AfxGetMainWnd( ) );
    if ( pMainFrame != NULL )
    {
        CMidiDoc			*pMididoc	= pMainFrame->GetMidiDoc ();
        CMidiFile			*pMidifile	= pMainFrame->GetMidiFile ( );
        if ( pMidifile != NULL )
        {
            if ( pMidifile->LLIsPlaying() )
            {
                pMidifile->LLStop();
            }

            if ( pMidifile->MciIsPlaying() )
            {
                pMidifile->MciStop();
            }
        }
    }

	//		Play The Tune
	if ( theApp.m_PlayListThread == NULL && theApp.m_PlayDirThread == NULL )
	{
		theApp.WriteProfileInt( "Play List", "Tune Number", m_PlayList.GetCurSel ( ) );

		theApp.m_PlayListThread = AfxBeginThread ( RUNTIME_CLASS ( CPlayTuneThread ) );
	}
	*pResult = 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayList::OnKillfocusPlaylist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO
	DisplayButtons ();
	*pResult = 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayList::OnItemchangedPlaylist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO
	DisplayButtons ();
	*pResult = 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayList::OnBegindragPlaylist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO
	CPoint      ptAction;
	UINT        nFlags;

	GetCursorPos(&ptAction);
	m_PlayList.ScreenToClient(&ptAction);

	if ( ! m_bDragging )
	{
		m_hItemDrag = m_PlayList.HitTest(ptAction, &nFlags);

		//		If the selected object is not the track zero
		if ( m_hItemDrag >= 0 )
		{
			if ( m_PlayList.GetItemData ( m_hItemDrag ) >= 0 )
			{
				m_bDragging = TRUE;
				m_hItemDrop = -1;

				if ( m_pImageList == NULL )
				{
					//		Get the image list for dragging
					POINT		point;
					m_pImageList =
						m_PlayList.CreateDragImage ( m_hItemDrag, &point );
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

				m_PlayList.InvalidateRect ( NULL, TRUE );

				MessageBeep ( 0 );
			}
		}
		else
		{
			m_hItemDrag = -1;

			m_PlayList.InvalidateRect ( NULL, TRUE );

			MessageBeep ( 0 );
		}
	}

	*pResult = 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CPlayList::OnSelchangeDisplay() 
{
	// TODO
	int iSelected = m_Display.GetCurSel ();
	
	switch ( iSelected )
	{
		case 0:
		{
			m_PlayList.ModifyStyle ( LVS_TYPEMASK, LVS_ICON );
			break;
		}

		case 1:
		{
			m_PlayList.ModifyStyle ( LVS_TYPEMASK, LVS_SMALLICON );
			break;
		}

		case 2:
		{
			m_PlayList.ModifyStyle ( LVS_TYPEMASK, LVS_LIST );
			break;
		}

		default :
		case 3:
		{
			m_PlayList.ModifyStyle ( LVS_TYPEMASK, LVS_REPORT );
			break;
		}
	}

	if ( m_Arrange.GetCheck () )
	{
		m_PlayList.Arrange ( LVA_ALIGNTOP );
	}

}

void CPlayList::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO
	CRect		rect;
	GetClientRect ( &rect );
	ClientToScreen ( &rect );

	if ( rect.PtInRect ( point ) )
	{
		CMenu		menu;
		menu.LoadMenu ( IDR_MENU_PLAYLIST );
		CMenu	*pContextMenu = menu.GetSubMenu ( 0 );
		pContextMenu->TrackPopupMenu (
			TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, 
			point.x, point.y, this );
		return;
	}
	
	
}

void CPlayList::OnPlaylistOpenfile() 
{
	// TODO
	CMidiDoc			*pMididoc;

	// check that one item gets the focus
	pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );

	int iSelected = m_PlayList.GetCurSel ();

	if ( iSelected >= 0 && iSelected < m_PlayList.GetCount () )
	{
		theApp.WriteProfileInt( "Play List", "Tune Number", m_PlayList.GetCurSel ( ) );

		m_PlayList.GetText ( iSelected, m_szFilename, sizeof ( m_szFilename ) );
		if ( pMididoc != NULL )
		{
			if ( pMididoc->SaveModified ( ) )
			{
				pMididoc->OnOpenDocument(m_szFilename);
				pMididoc->SetPathName(m_szFilename);
				pMididoc->UpdateAllViews ( this, LEFT_MASK_ALL | RIGHT_MASK_ALL );
			}
		}
	}
		
}

void CPlayList::OnPlaylistHelp() 
{
	// TODO
	CAboutDlg	dlg;
	dlg.DoModal ();
	
}

void CPlayList::OnPlaylistProperty() 
{
	// TODO
	CPlayListProperty	dlgProp;

	int iSelected = m_PlayList.GetCurSel ();
	if ( iSelected >= 0 && iSelected < m_PlayList.GetCount () )
	{
		m_PlayList.GetText ( iSelected, m_szFilename, sizeof ( m_szFilename ) );
	}

	dlgProp.DoModal ();
}

char * CPlayList::GetCurrentPlaylistFilename()
{
		return m_szFilename;
}

void CPlayList::OnArrange() 
{
	// TODO
	if ( m_Arrange.GetCheck () )
	{
		m_PlayList.Arrange ( LVA_ALIGNTOP );
	}
}

void CPlayList::OnList() 
{
	// TODO
	m_PlayList.ModifyStyle ( LVS_TYPEMASK, LVS_LIST );
	
}

void CPlayList::OnLargeicons() 
{
	// TODO
	m_PlayList.ModifyStyle ( LVS_TYPEMASK, LVS_ICON );
	
}

void CPlayList::OnSmallicons() 
{
	// TODO
	m_PlayList.ModifyStyle ( LVS_TYPEMASK, LVS_SMALLICON );
	
}

void CPlayList::OnReport() 
{
	// TODO
	m_PlayList.ModifyStyle ( LVS_TYPEMASK, LVS_REPORT );
	
}

void CPlayList::OnMenuArrange() 
{
	// TODO
	m_PlayList.Arrange ( LVA_ALIGNTOP );
}

BOOL CPlayList::OnEraseBkgnd(CDC* pDC) 
{
	// TODO
	
	BOOL bRes = FriendEraseBkgndScrollView(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

	return CMWFormView::OnEraseBkgnd ( pDC );
}

void CPlayList::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO
	OnButtonUp();
	
	CMWFormView::OnLButtonUp(nFlags, point);
}

void CPlayList::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO
	
	if ( m_bDragging )
	{
		CPoint      ptAction;

		GetCursorPos(&ptAction);
		m_PlayList.ScreenToClient(&ptAction);

		m_hItemDrop = m_PlayList.HitTest(ptAction, &nFlags);
		if ( m_hItemDrop != -1 )
		{
			UnSelectAll ( );
			m_PlayList.SetItemState ( m_hItemDrop, LVIS_SELECTED, LVIS_SELECTED );

			RECT				rectView;
			m_PlayList.GetClientRect ( &rectView );

			//		Ensure visibility at end of the window
			if ( rectView.bottom - ptAction.y < 10 )
			{
				if ( m_hItemDrop < m_PlayList.GetCount ( ) - 1 )
				{
					m_pImageList->DragLeave(this);
					m_PlayList.EnsureVisible ( m_hItemDrop + 1, false );
					m_pImageList->DragEnter(this, point);
				}
			}

			if ( ptAction.y - rectView.top < 10 )
			{
				if ( m_hItemDrop > 0 )
				{
					m_pImageList->DragLeave(this);
					m_PlayList.EnsureVisible ( m_hItemDrop - 1, false );
					m_pImageList->DragEnter(this, point);
				}
			}
		}

		if ( m_pImageList != NULL )
		{
			m_pImageList->DragMove(point);
		}
	}

	CMWFormView::OnMouseMove(nFlags, point);
}

void CPlayList::OnButtonUp()
{
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

			if ( m_hItemDrag > m_hItemDrop )
			{
				sprintf_s ( szMessage, sizeof ( szMessage ), "Do you want to move file %s before %s", 
					(const char *) m_PlayList.GetItemText ( m_hItemDrag, 0 ),
					(const char *) m_PlayList.GetItemText ( m_hItemDrop, 0 ) );
			}
			else
			{
				sprintf_s ( szMessage, sizeof ( szMessage ), "Do you want to move file %s after %s", 
					(const char *) m_PlayList.GetItemText ( m_hItemDrag, 0 ),
					(const char *) m_PlayList.GetItemText ( m_hItemDrop, 0 ) );
			}
			int iYesNo = 
				MessageBox ( szMessage, NULL, 
								MB_YESNO | MB_ICONEXCLAMATION );
			if ( iYesNo == IDYES )
			{
				char		szPathname [ MAX_PATHNAME_STRING ];

				m_PlayList.GetText ( m_hItemDrag, szPathname, sizeof ( szPathname ) );
				m_PlayList.DeleteString ( m_hItemDrag );
				m_PlayList.InsertString ( m_hItemDrop, szPathname );
				m_PlayList.SetCurSel ( m_hItemDrop );
				m_bPlayListModified = true;
				DisplayButtons ();
			}

			m_PlayList.InvalidateRect ( NULL, TRUE );

		}
		else
		{
			m_PlayList.InvalidateRect ( NULL, TRUE );

			MessageBeep(0);
		}

		m_hItemDrag = -1;
		m_hItemDrop = -1;
		m_bDragging = FALSE;

		ReleaseCapture();

//		m_PlayList.SelectDropTarget(NULL);

	}

}

void CPlayList::UnSelectAll()
{
	int			iX;

	for ( iX = 0; iX < m_PlayList.GetItemCount ( ); iX++ )
	{
		m_PlayList.SetItemState ( iX, 0, LVIS_SELECTED );
	}

}

void CPlayList::OnInitialUpdate() 
{
	CMWFormView::OnInitialUpdate();
	
	// TODO
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

void CPlayList::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO
	FriendActivate( bActivate, pActivateView, pDeactiveView, false);
	
	CMWFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
