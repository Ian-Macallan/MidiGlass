// MidiExplorerView.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "MidiExplorerView.h"
#include "PlayDirThreadProc.h"
#include "ModifyThread.h"
#include "RecordThread.h"
#include "Friend.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #include <olestd.h>
#include <shlobj.h>
#include <direct.h>
#include <io.h>
#include <time.h>

extern CMidiWorksApp theApp;

#define	NO_IMAGE_LIST_FOR_OLE_DRAG

//
////////////////////////////////////////////////////////////////////////////////////////
// CMidiExplorerView
//
////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMidiExplorerView, CTreeView)

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
CMidiExplorerView::CMidiExplorerView()
{
	//		View Initialisation
	m_bFirstTime		= true;
	m_pImageList		= NULL;
	m_pOldImageList		= NULL;

	//		Drag And Drop
	m_bDragging			= false;
	m_hItemDrag			= NULL;
	m_hItemDrop			= NULL;

	//		Ole Drag And Drop
	m_bDraggingOle		= false;
	m_prevDropEffectOle	= DROPEFFECT_NONE;

}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
CMidiExplorerView::~CMidiExplorerView()
{
	theApp.WriteProfileString ( "Explorer", "Path", theApp.m_Explorer_Pathname );
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMidiExplorerView, CTreeView)
	//{{AFX_MSG_MAP(CMidiExplorerView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EXPLORER_HELP, OnExplorerHelp)
	ON_COMMAND(ID_EXPLORER_RENAME, OnExplorerRename)
	ON_COMMAND(ID_EXPLORER_OPEN, OnExplorerOpen)
	ON_COMMAND(ID_EXPLORER_DELETE, OnExplorerDelete)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_EXPLORER_PLAYDIR, OnExplorerPlaydir)
	ON_COMMAND(ID_EXPLORER_MODIFYFILES, OnExplorerModifyfiles)
	ON_COMMAND(ID_EXPLORER_REFRESH, OnExplorerRefresh)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DROPFILES()
	ON_COMMAND(ID_EXPLORER_RECORD, OnExplorerRecord)
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )


   	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
////////////////////////////////////////////////////////////////////////////////////////
// CMidiExplorerView drawing
//
////////////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::OnDraw(CDC* pDC)
{
	CMidiDoc *pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );
}

//
////////////////////////////////////////////////////////////////////////////////////////
// CMidiExplorerView diagnostics
//
////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void CMidiExplorerView::AssertValid() const
{
	CTreeView::AssertValid();
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

//
////////////////////////////////////////////////////////////////////////////////////////
// CMidiExplorerView message handlers
//
////////////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	
	// TODO: Add your specialized creation code here
	// TODO: Add your specialized code here and/or call the base class
	if ( m_bFirstTime )
	{
		ModifyStyle ( NULL, 
						TVS_EDITLABELS | TVS_HASBUTTONS |
						TVS_HASLINES | TVS_LINESATROOT );

		GetTreeCtrl ( ).SetImageList ( &theApp.m_imgExplorerIcons16x14, LVSIL_NORMAL );
		GetTreeCtrl ( ).SetImageList ( &theApp.m_imgExplorerIcons16x14, LVSIL_STATE );

		DisplayDrives();

		//		Create the tip tool.
		m_ToolTip.Create ( this, TTS_ALWAYSTIP );

		RECT		Rect;
		GetClientRect ( &Rect );
		BOOL bRes = m_ToolTip.AddTool( this, LPSTR_TEXTCALLBACK );
		m_ToolTip.Activate ( FALSE );

		// Reset first time
		m_bFirstTime = false;

        GetTreeCtrl ( ).SetTextColor ( white0xff );
        GetTreeCtrl ( ).SetBkColor ( black0x20 );
        GetTreeCtrl ( ).SetLineColor ( white0xdd );
	}
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::DisplayDrives()
{
	char			szBuffer [ MAX_LONG_STRING ];
	char			*pBuffer;
	char			szDirectory [ MAX_PATHNAME_STRING ];

	strcpy_s ( szDirectory,  sizeof ( szDirectory ), theApp.m_Explorer_Pathname );
	DWORD dwLen = GetLogicalDriveStrings ( sizeof ( szBuffer ), szBuffer );

	if ( dwLen > 0 )
	{
		//		Get the first address of pBuffer
		pBuffer = szBuffer;

		while ( *pBuffer )
		{
			TVINSERTSTRUCT		tvInsertStruct;
			tvInsertStruct.hParent					= NULL;
			tvInsertStruct.hInsertAfter				= NULL;
			tvInsertStruct.item.mask				=
				TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE | TVIF_TEXT;

			tvInsertStruct.item.hItem				= NULL;
			tvInsertStruct.item.state				= NULL;
			tvInsertStruct.item.stateMask			= NULL;
			tvInsertStruct.item.pszText				= pBuffer;
			tvInsertStruct.item.cchTextMax			= (int)strlen ( pBuffer );
			tvInsertStruct.item.cChildren			= I_CHILDRENCALLBACK;

			// Say it is a drive disk
			tvInsertStruct.item.lParam				= 0;

			UINT iType = GetDriveType ( pBuffer );

			switch ( iType )
			{
				case DRIVE_NO_ROOT_DIR :	//	The root directory does not exist. 
				{
					tvInsertStruct.item.iImage				= 0;
					tvInsertStruct.item.iSelectedImage		= 0;
					break;
				}

				case DRIVE_REMOVABLE :		//	The disk can be removed from the drive. 
				{
					tvInsertStruct.item.iImage				= 3;
					tvInsertStruct.item.iSelectedImage		= 3;
					break;
				}

				case DRIVE_FIXED :			//	The disk cannot be removed from the drive. 
				{
					tvInsertStruct.item.iImage				= 4;
					tvInsertStruct.item.iSelectedImage		= 5;
					break;
				}

				case DRIVE_REMOTE :			//	The drive is a remote (network) drive. 
				{
					tvInsertStruct.item.iImage				= 7;
					tvInsertStruct.item.iSelectedImage		= 7;
					break;
				}

				case DRIVE_CDROM :			//	The drive is a CD-ROM drive. 
				{
					tvInsertStruct.item.iImage				= 6;
					tvInsertStruct.item.iSelectedImage		= 6;
					break;
				}

				case DRIVE_RAMDISK :		//	The drive is a RAM disk. 
				{
					tvInsertStruct.item.iImage				= 0;
					tvInsertStruct.item.iSelectedImage		= 0;
					break;
				}

				case DRIVE_UNKNOWN :		//	The drive type cannot be determined. 
				default :
				{
					tvInsertStruct.item.iImage				= 4;
					tvInsertStruct.item.iSelectedImage		= 5;
					break;
				}

			}

			HTREEITEM hTreeItem = GetTreeCtrl ( ).InsertItem( &tvInsertStruct );

			if ( strncmp ( pBuffer, szDirectory, strlen ( pBuffer ) ) == 0 )
			{
				ExpandDisk( hTreeItem, szDirectory + strlen ( pBuffer ) );
			}

			pBuffer = pBuffer + strlen ( pBuffer ) + 1;
		}
	}

	GetTreeCtrl ().SortChildren ( NULL );

}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::TreatSelectedItem(HTREEITEM hTreeItem)
{
	memset ( theApp.m_Explorer_Pathname, 0, sizeof ( theApp.m_Explorer_Pathname ) );
	GetFullPathName ( hTreeItem, theApp.m_Explorer_Pathname,
						sizeof ( theApp.m_Explorer_Pathname ) );

	if ( ! GetTreeCtrl ().ItemHasChildren( hTreeItem ) )
	{
		if ( GetTreeCtrl ().GetItemData ( hTreeItem ) != 2 )
		{
			DisplayDirectory ( hTreeItem );
		}

		GetTreeCtrl ( ).SelectItem ( hTreeItem );
	}
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::DisplayDirectory(HTREEITEM hTreeItem)
{
	char		szDirectory [ MAX_PATHNAME_STRING ];
	CString		strDirectory;


    /*
     *      Structure for find file.
     */
    HANDLE               	iStream;
    WIN32_FIND_DATA			tagFile;

	/*
	 *		The Filename.
	 */
	char					szFilename [ MAX_PATHNAME_STRING ];

	memset ( szDirectory, 0, sizeof ( szDirectory ) );
	GetFullPathName ( hTreeItem, szDirectory,
						sizeof ( szDirectory ) );
	strDirectory = szDirectory;
	strcat_s ( szDirectory, sizeof ( szDirectory ), "*.*" );

    /*
     *      Find first name.
     */
    iStream = FindFirstFile ( szDirectory, &tagFile );
    if ( iStream != INVALID_HANDLE_VALUE )
    {
		TVINSERTSTRUCT		tvInsertStruct;

		strcpy_s ( szFilename, sizeof ( szFilename ), tagFile.cFileName );
		if ( tagFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			tvInsertStruct.item.iImage				= 0;
			tvInsertStruct.item.iSelectedImage		= 1;

			//		Say it is a directory
			tvInsertStruct.item.lParam				= 1;
		}
		else
		{
			tvInsertStruct.item.iImage				= 2;
			tvInsertStruct.item.iSelectedImage		= 8;

			//		Say it is a filename
			tvInsertStruct.item.lParam				= 2;
		}

		tvInsertStruct.hParent					= hTreeItem;
		tvInsertStruct.hInsertAfter				= NULL;
		tvInsertStruct.item.mask				=
			TVIF_IMAGE | TVIF_PARAM |
			TVIF_SELECTEDIMAGE | TVIF_TEXT;

		tvInsertStruct.item.hItem				= NULL;
		tvInsertStruct.item.state				= NULL;
		tvInsertStruct.item.stateMask			= NULL;
		tvInsertStruct.item.pszText				= szFilename;
		tvInsertStruct.item.cchTextMax			= (int)strlen ( szFilename );
		tvInsertStruct.item.cChildren			= I_CHILDRENCALLBACK;

		FilterPathName ( &tagFile );

		if ( ! (	tagFile.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ||
					tagFile.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM ) )
		{
			HTREEITEM hSubTreeItem = GetTreeCtrl ( ).InsertItem( &tvInsertStruct );
		}

		while ( FindNextFile ( iStream, &tagFile ) != FALSE )
        {
			strcpy_s ( szFilename, sizeof ( szFilename ), tagFile.cFileName );
			if ( tagFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				tvInsertStruct.item.iImage				= 0;
				tvInsertStruct.item.iSelectedImage		= 1;

				//		Directory
				tvInsertStruct.item.lParam				= 1;
			}
			else
			{
				tvInsertStruct.item.iImage				= 2;
				tvInsertStruct.item.iSelectedImage		= 8;

				//		A filename
				tvInsertStruct.item.lParam				= 2;
			}

			tvInsertStruct.hParent					= hTreeItem;
			tvInsertStruct.hInsertAfter				= NULL;
			tvInsertStruct.item.mask				=
				TVIF_IMAGE | TVIF_PARAM |
				TVIF_SELECTEDIMAGE | TVIF_TEXT;

			tvInsertStruct.item.hItem				= NULL;
			tvInsertStruct.item.state				= NULL;
			tvInsertStruct.item.stateMask			= NULL;
			tvInsertStruct.item.pszText				= szFilename;
			tvInsertStruct.item.cchTextMax			= (int)strlen ( szFilename );
			tvInsertStruct.item.cChildren			= I_CHILDRENCALLBACK;

			FilterPathName( &tagFile );

			if ( ! ( tagFile.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN || tagFile.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM ) )
			{
				HTREEITEM hSubTreeItem = GetTreeCtrl ( ).InsertItem( &tvInsertStruct );
			}
        }

		FindClose ( iStream );
    }

	GetTreeCtrl ().SortChildren ( hTreeItem );

}

//
////////////////////////////////////////////////////////////////////////////////////////
//		Get the full path name
//
////////////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::GetFullPathName(HTREEITEM hTreeItem, 
										char *pFilename, size_t iFilename )
{
	
	HTREEITEM hParentTreeItem = GetTreeCtrl ().GetParentItem ( hTreeItem );
	if ( hParentTreeItem != NULL )
	{
		GetFullPathName ( hParentTreeItem, pFilename, iFilename );
	}

	TVITEM		tvItem;

	tvItem.mask		= TVIF_PARAM;
	tvItem.hItem	= hTreeItem;

	BOOL bRes = GetTreeCtrl ().GetItem( &tvItem );
	if ( bRes )
	{
		strcat_s ( pFilename, iFilename, GetTreeCtrl ().GetItemText ( hTreeItem ) );
		switch ( tvItem.lParam )
		{
			case 0:
			{
				break;
			}

			case 1:
			{
				strcat_s ( pFilename, iFilename, "\\" );
				break;
			}

			case 2:
			{
				break;
			}

			default:
			{
				break;
			}

		}
	}

}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::FilterPathName(WIN32_FIND_DATA *tagFile)
{
	CMidiClass		clForMidi;

	if ( tagFile->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
	{
		if ( strcmp ( tagFile->cFileName, "." ) == 0 )
		{
			tagFile->dwFileAttributes = tagFile->dwFileAttributes | FILE_ATTRIBUTE_HIDDEN;
		}

		if ( strcmp ( tagFile->cFileName, ".." ) == 0 )
		{
			tagFile->dwFileAttributes = tagFile->dwFileAttributes | FILE_ATTRIBUTE_HIDDEN;
		}

		return;
	}

	if ( clForMidi.IsMidiFile ( tagFile->cFileName ) )
	{
		return;
	}

	tagFile->dwFileAttributes = tagFile->dwFileAttributes | FILE_ATTRIBUTE_HIDDEN;
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if ( ! m_bFirstTime )
	{
		m_ToolTip.Activate ( FALSE );

		m_ToolTip.Activate ( TRUE );
	}

	*pResult = 0;

}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CMidiClass		clForMidi;

	KillTimer ( EXPLORER_VIEW_TIMER );

	HTREEITEM hTreeItem = GetTreeCtrl ().GetSelectedItem ( );
	if ( hTreeItem != NULL )
	{
		//		Is it a file
		char		szFilename [ MAX_PATHNAME_STRING ];
		memset ( szFilename, 0, sizeof ( szFilename ) );
		GetFullPathName ( hTreeItem, szFilename, sizeof ( szFilename ) );
		if ( clForMidi.IsMidiFile ( szFilename ) )
		{
			CMidiDoc			*pMididoc;

			// check that one item gets the focus
			pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );

			if ( pMididoc != NULL )
			{
				if ( pMididoc->SaveModified ( ) )
				{
					pMididoc->OnOpenDocument(szFilename);
					pMididoc->SetPathName(szFilename);
					pMididoc->UpdateAllViews ( this, LEFT_MASK_ALL | RIGHT_MASK_ALL );
				}
			}
		}
	}
	
	*pResult = 0;
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( pNMTreeView != NULL )
	{
		if ( pNMTreeView->itemNew.hItem != pNMTreeView->itemOld.hItem )
		{
			if ( pNMTreeView->itemOld.hItem != NULL )
			{
//				BOOL bRes = GetTreeCtrl ().Expand( pNMTreeView->itemOld.hItem, TVE_COLLAPSE );
			}

			if ( pNMTreeView->itemNew.hItem != NULL )
			{
				TreatSelectedItem( pNMTreeView->itemNew.hItem );
			}
		}
	}

	if ( ! m_bFirstTime )
	{
		m_ToolTip.Activate ( FALSE );
		m_ToolTip.Activate ( TRUE );
	}

	*pResult = 0;
}

void CMidiExplorerView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	CRect		rect;
	GetClientRect ( &rect );
	ClientToScreen ( &rect );

	if ( rect.PtInRect ( point ) )
	{
		CMenu		menu;
		menu.LoadMenu ( IDR_MENU_EXPLORER );
		CMenu	*pContextMenu = menu.GetSubMenu ( 0 );
		pContextMenu->TrackPopupMenu (
			TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, 
			point.x, point.y, this );
		return;
	}
	
	
}

void CMidiExplorerView::OnExplorerHelp() 
{
	// TODO: Add your command handler code here
	CAboutDlg	dlg;
	dlg.DoModal ();
}

void CMidiExplorerView::OnExplorerRename() 
{
	// TODO: Add your command handler code here
	HTREEITEM hTreeItem = GetTreeCtrl ().GetSelectedItem ( );
	if ( hTreeItem != NULL )
	{
		CEdit *hEdit = GetTreeCtrl ().EditLabel ( hTreeItem );
	}
}

void CMidiExplorerView::OnExplorerOpen() 
{
	// TODO: Add your command handler code here
	CMidiClass		clForMidi;

	HTREEITEM hTreeItem = GetTreeCtrl ().GetSelectedItem ( );
	if ( hTreeItem != NULL )
	{
		//		Is it a file
		char		szFilename [ MAX_PATHNAME_STRING ];
		memset ( szFilename, 0, sizeof ( szFilename ) );
		GetFullPathName ( hTreeItem, szFilename, sizeof ( szFilename ) );
		if ( clForMidi.IsMidiFile ( szFilename ) )
		{
			CMidiDoc			*pMididoc;

			// check that one item gets the focus
			pMididoc	= dynamic_cast< CMidiDoc * >( GetDocument () );

			if ( pMididoc != NULL )
			{
				if ( pMididoc->SaveModified ( ) )
				{
					pMididoc->OnOpenDocument(szFilename);
					pMididoc->SetPathName(szFilename);
					pMididoc->UpdateAllViews ( this, LEFT_MASK_ALL | RIGHT_MASK_ALL );
				}
			}
		}
	}
}

void CMidiExplorerView::OnExplorerDelete() 
{
	// TODO: Add your command handler code here
	HTREEITEM			hSel;
	TVITEM				tvItem;

	hSel = GetTreeCtrl ().GetSelectedItem ( );
	if ( hSel != NULL )
	{
		tvItem.mask		= TVIF_PARAM;
		tvItem.hItem	= hSel;

		BOOL bRes = GetTreeCtrl ().GetItem( &tvItem );
		if ( bRes && tvItem.lParam == 2 ) // Sucess and Only files
		{
			CString	strReturned = GetTreeCtrl ().GetItemText ( hSel );
			if ( MessageBox ( "Do You Want to Delete " + strReturned,
				NULL, MB_YESNO | MB_ICONEXCLAMATION ) == IDYES )
			{
				char		szName [ MAX_PATHNAME_STRING ];
				memset ( szName, 0, sizeof ( szName ) );
				GetFullPathName ( hSel, szName, sizeof ( szName ) );
				if ( _unlink ( szName ) == 0 )
				{
					GetTreeCtrl ().DeleteItem ( hSel );
				}
			}
		}
	}
	
}

void CMidiExplorerView::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CMidiExplorerView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	TVITEM	tvItem;

	tvItem = pTVDispInfo->item;
	if ( tvItem.hItem != NULL && ( tvItem.lParam == 1 || tvItem.lParam == 2 ) )
	{
		char		szOldName [ MAX_PATHNAME_STRING ];
		char		szNewName [ MAX_PATHNAME_STRING ];

		memset ( szOldName, 0, sizeof ( szOldName ) );
		memset ( szNewName, 0, sizeof ( szNewName ) );

		CString	strOldName = GetTreeCtrl ().GetItemText ( tvItem.hItem );
		GetFullPathName ( tvItem.hItem, szOldName, sizeof ( szOldName ) );

		GetTreeCtrl ().SetItemText ( tvItem.hItem, tvItem.pszText );

		GetFullPathName ( tvItem.hItem, szNewName, sizeof ( szNewName ) );

		if ( rename ( szOldName, szNewName ) != 0 )
		{
			GetTreeCtrl ().SetItemText ( tvItem.hItem, strOldName );
		}
	}

	*pResult = 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( pTVKeyDown->wVKey == VK_DELETE )
	{
		OnExplorerDelete ( );
	}
	
	*pResult = 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::ExpandDisk( HTREEITEM hTreeItem, char *pDirectory )
{
	BOOL			bRes;

	bRes = GetTreeCtrl ().Expand( hTreeItem, TVE_EXPAND );

	HTREEITEM hSel = ExpandDirectory ( hTreeItem, pDirectory );
	if ( hSel != NULL )
	{
		bRes = GetTreeCtrl ().SelectItem( hSel );
	}
}


//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
HTREEITEM CMidiExplorerView::ExpandDirectory(HTREEITEM hTreeItem, char *pDirectory )
{
	char		szDirectory [ MAX_PATHNAME_STRING ];
	CString		strDirectory;

	HTREEITEM	hItemFound	= hTreeItem;

    /*
     *      Structure for find file.
     */
    HANDLE					iStream;
    WIN32_FIND_DATA			tagFile;

	/*
	 *		The Filename.
	 */
	char					szFilename [ MAX_PATHNAME_STRING ];

	memset ( szDirectory, 0, sizeof ( szDirectory ) );
	GetFullPathName ( hTreeItem, szDirectory, sizeof ( szDirectory ) );
	strDirectory = szDirectory;
	strcat_s ( szDirectory, sizeof ( szDirectory ), "*.*" );

	char	*pSlash = strchr ( pDirectory, '\\' );
	if ( pSlash != NULL )
	{
		*pSlash = 0;
	}

    /*
     *      Find first name.
     */
    iStream = FindFirstFile ( szDirectory, &tagFile );
    if ( iStream != INVALID_HANDLE_VALUE )
    {
		TVINSERTSTRUCT		tvInsertStruct;

		strcpy_s ( szFilename, sizeof ( szFilename ), tagFile.cFileName );
		if ( tagFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			tvInsertStruct.item.iImage				= 0;
			tvInsertStruct.item.iSelectedImage		= 1;

			//		Say it is a directory
			tvInsertStruct.item.lParam				= 1;
		}
		else
		{
			tvInsertStruct.item.iImage				= 2;
			tvInsertStruct.item.iSelectedImage		= 8;

			//		Say it is a filename
			tvInsertStruct.item.lParam				= 2;
		}

		tvInsertStruct.hParent					= hTreeItem;
		tvInsertStruct.hInsertAfter				= NULL;
		tvInsertStruct.item.mask				=
			TVIF_IMAGE | TVIF_PARAM |
			TVIF_SELECTEDIMAGE | TVIF_TEXT;

		tvInsertStruct.item.hItem				= NULL;
		tvInsertStruct.item.state				= NULL;
		tvInsertStruct.item.stateMask			= NULL;
		tvInsertStruct.item.pszText				= szFilename;
		tvInsertStruct.item.cchTextMax			= (int)strlen ( szFilename );
		tvInsertStruct.item.cChildren			= I_CHILDRENCALLBACK;

		FilterPathName ( &tagFile );

		if ( ! ( tagFile.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN || tagFile.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM ) )
		{
			HTREEITEM hSubTreeItem = GetTreeCtrl ( ).InsertItem( &tvInsertStruct );
			if ( strcmp ( tagFile.cFileName, pDirectory ) == 0 )
			{
				if ( tagFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					hItemFound = 
						ExpandDirectory(hSubTreeItem, 
							pDirectory + strlen ( tagFile.cFileName ) + 1 );
				}
				else
				{
					hItemFound = hSubTreeItem;
				}
			}
		}


        while ( FindNextFile ( iStream, &tagFile ) != FALSE )
        {
			strcpy_s ( szFilename, sizeof ( szFilename ), tagFile.cFileName );
			if ( tagFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				tvInsertStruct.item.iImage				= 0;
				tvInsertStruct.item.iSelectedImage		= 1;

				//		Directory
				tvInsertStruct.item.lParam				= 1;
			}
			else
			{
				tvInsertStruct.item.iImage				= 2;
				tvInsertStruct.item.iSelectedImage		= 8;

				//		A filename
				tvInsertStruct.item.lParam				= 2;
			}

			tvInsertStruct.hParent					= hTreeItem;
			tvInsertStruct.hInsertAfter				= NULL;
			tvInsertStruct.item.mask				=
				TVIF_IMAGE | TVIF_PARAM |
				TVIF_SELECTEDIMAGE | TVIF_TEXT;

			tvInsertStruct.item.hItem				= NULL;
			tvInsertStruct.item.state				= NULL;
			tvInsertStruct.item.stateMask			= NULL;
			tvInsertStruct.item.pszText				= szFilename;
			tvInsertStruct.item.cchTextMax			= (int)strlen ( szFilename );
			tvInsertStruct.item.cChildren			= I_CHILDRENCALLBACK;

			FilterPathName( &tagFile );

			if ( ! ( tagFile.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN || tagFile.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM ) )
			{
				HTREEITEM hSubTreeItem = GetTreeCtrl ( ).InsertItem( &tvInsertStruct );
				if ( strcmp ( tagFile.cFileName, pDirectory ) == 0 )
				{
					if ( tagFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
					{
						hItemFound = 
							ExpandDirectory(hSubTreeItem, 
								pDirectory + strlen ( tagFile.cFileName ) + 1 );
					}
					else
					{
						hItemFound = hSubTreeItem;
					}
				}
			}
        }

		FindClose ( iStream );
    }

	GetTreeCtrl ().SortChildren ( hTreeItem );

	BOOL bRes = GetTreeCtrl ().Expand( hTreeItem, TVE_EXPAND );

	return hItemFound;
}


//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT		iFlags;
	HTREEITEM hTreeItem = GetTreeCtrl ().HitTest( point, &iFlags );
	if ( hTreeItem != NULL )
	{
		GetTreeCtrl ().SelectItem ( hTreeItem );
	}

	CTreeView::OnRButtonDown(nFlags, point);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::OnExplorerPlaydir() 
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

	// TODO: Add your command handler code here
	if ( theApp.m_PlayDirThread == NULL && theApp.m_PlayListThread == NULL )
	{
		theApp.m_PlayDirThread = AfxBeginThread ( RUNTIME_CLASS ( CPlayDirThreadProc ) );
	}
	
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::OnExplorerModifyfiles() 
{
	// TODO: Add your command handler code here
	if ( theApp.m_ModifyThread == NULL )
	{
		theApp.m_ModifyThread = AfxBeginThread ( RUNTIME_CLASS ( CModifyThread ) );
	}
	
}

void CMidiExplorerView::OnExplorerRefresh() 
{
	// TODO: Add your command handler code here
	HTREEITEM				hSel;
	hSel = GetTreeCtrl ().GetSelectedItem ( );
	if ( hSel != NULL )
	{
		RefreshView ( hSel );
	}
}

void CMidiExplorerView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CPoint      ptAction;
	UINT        nFlags;

	GetCursorPos(&ptAction);
	ScreenToClient(&ptAction);
	if ( ! m_bDragging )
	{
		m_hItemDrag = GetTreeCtrl ().HitTest(ptAction, &nFlags);

		//		If the selected object is not a drive disk
		if ( GetTreeCtrl ().GetItemData ( m_hItemDrag ) != 0 )
		{
			m_bDragging = TRUE;
			m_hItemDrop = NULL;

			if ( m_pImageList == NULL )
			{
				//		Get the image list for dragging
				m_pImageList = GetTreeCtrl ().CreateDragImage ( m_hItemDrag );
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
			m_hItemDrag = NULL;
			MessageBeep ( 0 );
		}
	}

	*pResult = 0;
}

void CMidiExplorerView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	HTREEITEM           hItem;
	UINT                iFlags;
	RECT				rectView;

	if ( m_bDragging )
	{
		KillTimer ( EXPLORER_VIEW_TIMER );

		if ( m_pImageList != NULL )
		{
			m_pImageList->DragMove(point);

			if ( ( hItem = GetTreeCtrl ().HitTest(point, &iFlags ) ) != NULL)
			{
				m_pImageList->DragLeave(this);
				GetTreeCtrl ().SelectDropTarget(hItem);
				GetClientRect ( &rectView );

				//		Ensure visibility at end of the window
				if ( rectView.bottom - point.y < 10 )
				{
					EnsureVisible ( hItem, false );
				}

				if ( point.y - rectView.top < 10 )
				{
					EnsureVisible ( hItem, true );
				}

				m_hItemDrop = hItem;
				m_pImageList->DragEnter(this, point);

				SetTimer ( EXPLORER_VIEW_TIMER, 1000, NULL );

			}
		}
	}
	
	CTreeView::OnMouseMove(nFlags, point);
}

void CMidiExplorerView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnButtonUp();
	
	CTreeView::OnLButtonUp(nFlags, point);
}

void CMidiExplorerView::OnButtonUp()
{
	KillTimer ( EXPLORER_VIEW_TIMER );

	if ( m_bDragging )
	{
		if ( m_pImageList != NULL )
		{
			m_pImageList->DragLeave(this);
			m_pImageList->EndDrag();
			delete m_pImageList;
			m_pImageList = NULL;
		}

		if (	m_hItemDrag != m_hItemDrop
				&& ! IsChildNodeOf ( m_hItemDrop, m_hItemDrag ) 
				&& GetTreeCtrl ().GetParentItem ( m_hItemDrag ) != m_hItemDrop )
		{
			HTREEITEM		hParentDrag;

			hParentDrag = GetTreeCtrl ().GetParentItem ( m_hItemDrag );

			if ( TransferItem ( m_hItemDrag, m_hItemDrop ) )
			{
				if ( hParentDrag != NULL )
				{
					RefreshView ( hParentDrag );
				}

				if ( m_hItemDrop != NULL )
				{
					RefreshView ( m_hItemDrop );
				}
			}
			else
			{
				MessageBeep(0);
			}
		}
		else
		{
			MessageBeep(0);
		}

		m_hItemDrag = NULL;
		m_hItemDrop = NULL;
		m_bDragging = FALSE;

		ReleaseCapture();

		GetTreeCtrl ().SelectDropTarget(NULL);

	}

}

BOOL CMidiExplorerView::TransferItem(HTREEITEM hItemDrag, HTREEITEM hItemDrop)
{
	char				szOldPathname [MAX_PATHNAME_STRING];
	char				szNewPathname [MAX_PATHNAME_STRING];
	CString				strOldFile;
	CString				strNewFile;
	CString				strFile;

	KillTimer ( EXPLORER_VIEW_TIMER );

	memset ( szOldPathname, 0, sizeof ( szOldPathname ) );
	memset ( szNewPathname, 0, sizeof ( szNewPathname ) );
	
	GetFullPathName ( hItemDrag, szOldPathname, sizeof ( szOldPathname ) );
	GetFullPathName ( hItemDrop, szNewPathname, sizeof ( szNewPathname ) );

	if (	GetTreeCtrl ().GetItemData ( hItemDrop ) == 0
		 || GetTreeCtrl ().GetItemData ( hItemDrop ) == 1 )
	{
		if ( szOldPathname [ strlen ( szOldPathname ) - 1 ] == '\\' )
		{
			szOldPathname [ strlen ( szOldPathname ) - 1 ] = 0;
		}

		strOldFile = szOldPathname;
		strFile = GetTreeCtrl ().GetItemText ( hItemDrag );
		strcat_s ( szNewPathname, sizeof ( szNewPathname ), strFile );
		strNewFile = szNewPathname;
		int iYesNo = 
			MessageBox ( 
				"Do you want to move " + strOldFile + "\nto " + strNewFile,
					NULL,
					MB_YESNO | MB_ICONEXCLAMATION );
		if ( iYesNo == IDYES )
		{
			if ( rename ( szOldPathname, szNewPathname ) == 0 )
			{
				return TRUE;
			}
		}

		return FALSE;
	}

	if ( GetTreeCtrl ().GetItemData ( hItemDrop ) == 2 )
	{
		strOldFile = szOldPathname;
		strNewFile = szNewPathname;
		int iYesNo = 
			MessageBox (
				"Do you want to replace " + strNewFile + "\nby " + strOldFile,
				NULL,
				MB_YESNO | MB_ICONEXCLAMATION );
		if ( iYesNo == IDYES )
		{
			_unlink ( szNewPathname );
			if ( rename ( szOldPathname, szNewPathname ) == 0 )
			{
				return TRUE;
			}
		}

		return FALSE;
	}

	return FALSE;
}

BOOL CMidiExplorerView::IsChildNodeOf(HTREEITEM hItemChild, HTREEITEM hItemSuspectedParent)
{
	do
	{
		if (hItemChild == hItemSuspectedParent)
			break;
	}
	while ((hItemChild = GetTreeCtrl ().GetParentItem(hItemChild)) != NULL);

	return (hItemChild != NULL);
}

void CMidiExplorerView::RefreshView(HTREEITEM hItem)
{
	/*
	 *		The pathname
	 */
	char					szDirectory [ MAX_PATHNAME_STRING ];
	HTREEITEM				hChildItem;
	LPARAM					lParam;
	
	//		If it is a file
	lParam = GetTreeCtrl ().GetItemData ( hItem );
	if ( lParam == 2 )
	{
		//		Get The parent
		hItem = GetTreeCtrl ().GetParentItem ( hItem );
	}

	memset ( szDirectory, 0, sizeof ( szDirectory ) );
	GetFullPathName ( hItem, szDirectory, sizeof ( szDirectory ) );

	hChildItem = GetTreeCtrl ().GetChildItem( hItem );
	if ( hChildItem != NULL )
	{
		DeleteItemTree ( hChildItem );
	}

	TreatSelectedItem(hItem);

}

void CMidiExplorerView::EnsureVisible(HTREEITEM hItem, bool bTop )
{
	if ( bTop )
	{
		GetTreeCtrl ().EnsureVisible ( GetPrevItem( hItem ) );
	}
	else
	{
		GetTreeCtrl ().EnsureVisible ( GetNextItem( hItem ) );
	}
}

HTREEITEM CMidiExplorerView::GetPrevItem(HTREEITEM hItem)
{
	int				iCount = 0;

	while ( iCount++ < 2 )
	{
		if ( GetTreeCtrl ().GetPrevSiblingItem( hItem ) )
		{
			hItem = GetTreeCtrl ().GetPrevSiblingItem( hItem );
		}
		else
		{
			if ( GetTreeCtrl ().GetParentItem ( hItem ) )
			{
				hItem = GetTreeCtrl ().GetParentItem ( hItem );
			}
		}
	}

	return hItem;
}

HTREEITEM CMidiExplorerView::GetNextItem(HTREEITEM hItem)
{
	int				iCount = 0;

	while ( iCount++ < 2 )
	{
		if ( GetTreeCtrl ().GetNextSiblingItem( hItem ) )
		{
			hItem = GetTreeCtrl ().GetNextSiblingItem( hItem );
		}
		else
		{
			if ( GetTreeCtrl ().GetParentItem ( hItem ) )
			{
				hItem = GetTreeCtrl ().GetParentItem ( hItem );
				if ( GetTreeCtrl ().GetNextSiblingItem( hItem ) )
				{
					hItem = GetTreeCtrl ().GetNextSiblingItem( hItem );
				}
			}
		}
	}

	return hItem;
}

void CMidiExplorerView::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint      ptAction;
	UINT        nFlags;

	KillTimer ( EXPLORER_VIEW_TIMER );

	if ( m_bDragging )
	{
		GetCursorPos(&ptAction);
		ScreenToClient(&ptAction);

		HTREEITEM hTreeItem = GetTreeCtrl ().HitTest( ptAction, &nFlags );
		if ( hTreeItem != NULL )
		{
			if ( GetTreeCtrl ().GetItemData ( hTreeItem ) != 2 )
			{
				if ( ! GetTreeCtrl ().ItemHasChildren( hTreeItem ) )
				{
					m_pImageList->DragMove(ptAction);
					m_pImageList->DragLeave(this);

					DisplayDirectory ( hTreeItem );

					m_pImageList->DragEnter(this, ptAction);
				}
				BOOL bRes = GetTreeCtrl ().Expand( hTreeItem, TVE_EXPAND );
			}
		}
	}

	CTreeView::OnTimer(nIDEvent);
}

void CMidiExplorerView::DeleteItemTree(HTREEITEM hItem)
{
	HTREEITEM		hChildItem;
	HTREEITEM		hNextItem;

	if ( hItem != NULL )
	{
		hChildItem =  GetTreeCtrl ().GetChildItem ( hItem );
		if ( hChildItem != NULL )
		{
			DeleteItemTree( hChildItem );
		}

		hNextItem = GetTreeCtrl ().GetNextSiblingItem ( hItem );
		if ( hNextItem != NULL )
		{
			DeleteItemTree ( hNextItem );
		}

		if ( hItem == m_hItemDrag )
		{
			m_hItemDrag = NULL;
		}

		if ( hItem == m_hItemDrop )
		{
			m_hItemDrop = NULL;
		}

		GetTreeCtrl ().DeleteItem ( hItem );
	}
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
DROPEFFECT CMidiExplorerView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	// TODO: Add your specialized code here and/or call the base class
	CMidiClass		clForMidi;

	if ( m_prevDropEffectOle == DROPEFFECT_NONE)
	{

		//		Get the filename that is dragged
		if ( ! GetDragFilename(pDataObject) )
		{
			return DROPEFFECT_NONE;
		}

		//		Test if it is a midi file
		if ( ! clForMidi.IsMidiFile ( m_DragFilename ) )
		{
			return DROPEFFECT_NONE;
		}

		//		Say we are dragging
		m_bDraggingOle	= true;

		m_hItemDrop		= NULL;

		//		Create the drag image
#ifndef	NO_IMAGE_LIST_FOR_OLE_DRAG
		if ( m_pImageList == NULL )
		{

			//		Get the image list for dragging
			m_pImageList = new CImageList;

			m_pImageList->Create ( &theApp.m_imgExplorerIcons16x14 );
			m_pOldImageList = m_pImageList->GetDragImage ( &m_OldPoint, &m_OldHotSpot );
//			m_pImageList->DragShowNolock(TRUE);
			m_pImageList->SetDragCursorImage ( 9, CPoint( 0, 0 ) );
			m_pImageList->BeginDrag(0, CPoint(0,0));
			m_pImageList->DragEnter(this, point);
			m_pImageList->DragMove(point);
		}
#endif

	}
	else
	{
		DragClean ( );

		return DROPEFFECT_NONE;
	}

	//		And do the Drag Over
	return OnDragOver(pDataObject,dwKeyState,point);

}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::OnDragLeave() 
{
	DragClean ( );

	return;
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
DROPEFFECT CMidiExplorerView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point) 
{
	// TODO: Add your specialized code here and/or call the base class
	static		SYSTEMTIME		OldSystemTime;
	static		CPoint			oldPoint (0, 0);

	SYSTEMTIME					systemTime;

	GetSystemTime( &systemTime );

	//		Test if we are dragging
	if ( ! m_bDraggingOle )
	{
		return DROPEFFECT_NONE;
	}

	HTREEITEM           hItem;
	UINT                iFlags;
	RECT				rectView;

	if ( m_bDraggingOle )
	{
		if ( ( hItem = GetTreeCtrl ().HitTest(point, &iFlags ) ) != NULL)
		{
			int			iDelta;

			if ( m_pImageList != NULL )
			{
				m_pImageList->DragLeave(this);
			}

			iDelta = systemTime.wSecond * 1000 - OldSystemTime.wSecond * 1000
						+ systemTime.wMilliseconds - OldSystemTime.wMilliseconds;
			if ( iDelta < 0 )
			{
				iDelta += 60000;
			}

			//		If we are at the same place since two seconds
			if ( iDelta >= 1500 && oldPoint == point )
			{
				if ( GetTreeCtrl ().GetItemData ( hItem ) != 2 )
				{
					if ( ! GetTreeCtrl ().ItemHasChildren( hItem ) )
					{
						DisplayDirectory ( hItem );
					}
					BOOL bRes = GetTreeCtrl ().Expand( hItem, TVE_EXPAND );
				}
			}

			if ( oldPoint != point )
			{
				oldPoint		= point;
				OldSystemTime	= systemTime;
			}

			GetTreeCtrl ().SelectDropTarget(hItem);

			GetClientRect ( &rectView );

			//		Ensure visibility at end of the window
			if ( rectView.bottom - point.y < 10 )
			{
				EnsureVisible ( hItem, false );
			}

			if ( point.y - rectView.top < 10 )
			{
				EnsureVisible ( hItem, true );
			}

			m_hItemDrop = hItem;

			if ( m_pImageList != NULL )
			{
				m_pImageList->DragEnter(this, point);
			}

			SetTimer ( EXPLORER_VIEW_TIMER, 1000, NULL );
		}

	}
	
	return SetDropEffect(pDataObject,dwKeyState,point);
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
BOOL CMidiExplorerView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point) 
{
	// TODO: Add your specialized code here and/or call the base class
	ASSERT_VALID(this);

	//		Clean up focus rect
	DragClean ( );

	//		We will treat it differently
	//		By the AcceptDragFiles and WM_DROPFILES 
	//		message
	return FALSE;

	HTREEITEM           hItem;
	UINT                iFlags;

	if ( ( hItem = GetTreeCtrl ().HitTest(point, &iFlags ) ) != NULL)
	{
		//		Do not accept to copy on file
		if ( GetTreeCtrl ().GetItemData ( hItem ) == 2 )
		{
			return FALSE;
		}

		//		Get the full path name from the destination item
		char		szPathname [ MAX_PATHNAME_STRING ];
		memset ( szPathname, 0, sizeof ( szPathname ) );
		GetFullPathName ( hItem, szPathname, sizeof ( szPathname ) );

		//		The long filename
		CString			strDragFilename;
		CString			strNewFilename;
		CString			strDropAction;

		WIN32_FIND_DATA		tagFile;
		HANDLE hFile = FindFirstFile ( m_DragFilename, &tagFile );
		FindClose ( hFile );
		strcat_s ( szPathname, sizeof ( szPathname ), tagFile.cFileName );
		strDragFilename	= m_DragFilename;
		strNewFilename	= szPathname;

		//		On the drop effect
		switch ( dropEffect )
		{
			case DROPEFFECT_COPY :
			{
				strDropAction = "Copy from ";
				break;
			}

			case DROPEFFECT_LINK :
			{
				strDropAction = "Link from ";
				break;
			}

			case DROPEFFECT_MOVE :
			{
				strDropAction = "Move from ";
				break;
			}

			default:
			{
				return FALSE;
				break;
			}
		}

		int iYesNo = 
			MessageBox ( 
				strDropAction + strDragFilename + "\n To " + strNewFilename,
				NULL, MB_YESNO | MB_ICONEXCLAMATION );
		if ( iYesNo != IDYES )
		{
			return FALSE;
		}

		//		On the drop effect
		switch ( dropEffect )
		{
			case DROPEFFECT_COPY :
			{
				return FALSE;
				break;
			}

			case DROPEFFECT_LINK :
			{
				return FALSE;
				break;
			}

			case DROPEFFECT_MOVE :
			{
				if ( rename ( strDragFilename,  strNewFilename ) == 0 )
				{
					return TRUE;
				}
				break;
			}

			default:
			{
				return FALSE;
				break;
			}
		}

	}

	return FALSE;
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
BOOL CMidiExplorerView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CTreeView::OnScrollBy(sizeScroll, bDoScroll);
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
int CMidiExplorerView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	if ( m_ExplorerDropTarget.Register ( this ) )
	{
		DragAcceptFiles( TRUE );
	}
	else
	{
		MessageBox ( "Unable to Register to OLE", NULL );
	}

	return 0;
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
bool CMidiExplorerView::GetDragFilename(COleDataObject *pDataObject)
{
	//		Get the data.
	STGMEDIUM		stgMedium;
	HGLOBAL			hGlobal;

	memset ( m_DragFilename, 0, sizeof ( m_DragFilename ) );

	if ( pDataObject->IsDataAvailable( RegisterClipboardFormat ( LPSTR ( "CF_FILENAME" ) ) ) )
	{
		BOOL bRes = 
			pDataObject->GetData ( RegisterClipboardFormat( LPSTR ( "CF_FILENAME" ) ), &stgMedium );

		if ( stgMedium.tymed == TYMED_HGLOBAL )
		{
			hGlobal= stgMedium.hGlobal;
			char	*pFilename = ( char * ) GlobalLock ( hGlobal );
			if ( pFilename != NULL )
			{
				strcpy_s ( m_DragFilename, sizeof ( m_DragFilename ), pFilename );

				//		This will retrieve the full name
				WIN32_FIND_DATA		tagFile;
				HANDLE hFile = FindFirstFile ( pFilename, &tagFile );
				FindClose ( hFile );
				return true;
			}
		}
	}

	return false;
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
DROPEFFECT CMidiExplorerView::SetDropEffect(COleDataObject *pDataObject, DWORD dwKeyState, CPoint point)
{
	DROPEFFECT de = DROPEFFECT_NONE;

	// check for force link
	if ((dwKeyState & (MK_CONTROL|MK_SHIFT)) == (MK_CONTROL|MK_SHIFT))
	{
		de = DROPEFFECT_LINK;
	}
	// check for force copy
	else if ((dwKeyState & MK_CONTROL) == MK_CONTROL)
	{
		de = DROPEFFECT_COPY;
	}
	// check for force move
	else if ((dwKeyState & MK_ALT) == MK_ALT)
	{
		de = DROPEFFECT_MOVE;
	}
	// default -- recommended action is move
	else
	{
		de = DROPEFFECT_MOVE;
	}

	return de;
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	POINT			ptAction;
	UINT		    nFlags;

	char			szFilename [ MAX_PATHNAME_STRING ];

	if ( ! DragQueryPoint( hDropInfo, &ptAction ) )
	{
		GetCursorPos(&ptAction);
		ScreenToClient(&ptAction);
	}

	UINT iCount = 
		DragQueryFile ( hDropInfo, 0xFFFFFFFF, 
						szFilename, sizeof ( szFilename ) );

	HTREEITEM hItem = GetTreeCtrl ().HitTest( ptAction, &nFlags );
	if ( hItem != NULL )
	{

		if ( iCount == 1 || GetTreeCtrl ().GetItemData ( hItem ) != 2 )
		{
			//		First get the count of files
			UINT		iX;

			if ( iCount >= 0 )
			{
				for ( iX = 0; iX < iCount; iX++ )
				{
					UINT iRes = 
						DragQueryFile ( hDropInfo, iX, 
										szFilename, sizeof ( szFilename ) );

					//		Get the full path name from the destination item
					char		szPathname [ MAX_PATHNAME_STRING ];
					memset ( szPathname, 0, sizeof ( szPathname ) );
					GetFullPathName ( hItem, szPathname, sizeof ( szPathname ) );

					//		The long filename
					CString			strDragFilename;
					CString			strNewFilename;

					WIN32_FIND_DATA		tagFile;
					HANDLE hFile = FindFirstFile ( szFilename, &tagFile );
					if ( iCount != 1 )
					{
						strcat_s ( szPathname, sizeof ( szPathname ), tagFile.cFileName );
					}
					strDragFilename	= szFilename;
					strNewFilename	= szPathname;

					int iYesNo =
						MessageBox (	"Move from " + strDragFilename + 
									"\nTo " + strNewFilename, NULL,
									MB_YESNO | MB_ICONEXCLAMATION );
					if ( iYesNo == IDYES )
					{
						if ( rename ( strDragFilename, strNewFilename ) == 0 )
						{
							MessageBeep ( 0 );
						}
					}

				}

			}
		}
		else
		{
			MessageBeep ( 0 );
		}
	}
	else
	{
		MessageBeep ( 0 );
	}

	CTreeView::OnDropFiles(hDropInfo);
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::DragClean()
{
	// TODO: Add your specialized code here and/or call the base class
	m_prevDropEffectOle = DROPEFFECT_NONE;

	//		Say we are dragging
	m_bDraggingOle = false;

	//		Reset the item where to drop
	m_hItemDrop = NULL;

	//		Delete the image
	if ( m_pImageList != NULL)
	{
		m_pImageList->DragLeave(this);
		m_pImageList->EndDrag();
		delete m_pImageList;
		m_pImageList = NULL;
	}

	GetTreeCtrl ().SelectDropTarget ( NULL );

}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	// TODO: Add your specialized code here and/or call the base class
	FriendActivate( bActivate, pActivateView, pDeactiveView, true);

	CTreeView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class

	CTreeView::PostNcDestroy();
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void CMidiExplorerView::OnExplorerRecord() 
{
	// TODO: Add your command handler code here
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

   //	
	CMidiClass		clForMidi;

	HTREEITEM hTreeItem = GetTreeCtrl ().GetSelectedItem ( );
	if ( hTreeItem != NULL )
	{
		//		Is it a file
		char		szFilename [ MAX_PATHNAME_STRING ];
		memset ( szFilename, 0, sizeof ( szFilename ) );
		GetFullPathName ( hTreeItem, szFilename, sizeof ( szFilename ) );
		if ( clForMidi.IsMidiFile ( szFilename ) )
		{
			if ( theApp.m_RecordThread == NULL )
			{
				theApp.m_MidiFileName = szFilename;
				theApp.m_RecordThread = AfxBeginThread ( RUNTIME_CLASS ( CRecordThread ) );
			}
		}
	}
}

//
////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
BOOL CMidiExplorerView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_ToolTip.RelayEvent ( pMsg );

	return CTreeView::PreTranslateMessage(pMsg);
}

//
////////////////////////////////////////////////////////////////////////////////////////
//Notification handler - add entry to class definition
//
////////////////////////////////////////////////////////////////////////////////////////
BOOL CMidiExplorerView::OnToolTipNotify(UINT id, NMHDR *pNMH, LRESULT *pResult)
{
	TOOLTIPTEXT *pText = (TOOLTIPTEXT *)pNMH;

	POINT			ptAction;
	UINT		    nFlags;

	char					szFilename [ MAX_PATHNAME_STRING ];
    HANDLE                	iStream;
    WIN32_FIND_DATA			tagFile;

	GetCursorPos(&ptAction);
	ScreenToClient(&ptAction);

	HTREEITEM hItem = GetTreeCtrl ().HitTest( ptAction, &nFlags );
	if ( hItem != NULL )
	{
		memset ( szFilename, 0, sizeof ( szFilename ) );
		GetFullPathName ( hItem, szFilename, sizeof ( szFilename ) );
	    iStream = FindFirstFile ( szFilename, &tagFile );
		if ( iStream != INVALID_HANDLE_VALUE )
		{
			CTime tmdt ( tagFile.ftLastWriteTime );

            CString fmt = tmdt.Format ( "%d %B %Y %H:%M:%S" );
			sprintf_s ( szFilename, sizeof ( szFilename ), "%s - Size:%d", ( const char *) fmt , tagFile.nFileSizeLow );
			pText->lpszText = szFilename;
			return TRUE;
		}

	}

	return FALSE;
}
