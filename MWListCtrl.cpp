//
///////////////////////////////////////////////////////////////////////////////////
// MWListCtrl.cpp : implementation file
//
///////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "MWListCtrl.h"
#include "Friend.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMidiWorksApp theApp;

IMPLEMENT_DYNAMIC(CMWListCtrl, CListCtrl)

//
///////////////////////////////////////////////////////////////////////////////////
// CMWListCtrl
//
///////////////////////////////////////////////////////////////////////////////////
CMWListCtrl::CMWListCtrl()
{
	m_bHeaderDone               = false;
    m_bHeaderControlSubclassed  = false;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
CMWListCtrl::~CMWListCtrl()
{
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMWListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMWListCtrl)
	ON_WM_CTLCOLOR()
	ON_NOTIFY_REFLECT(NM_KILLFOCUS, OnKillfocus)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMWListCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()

//
///////////////////////////////////////////////////////////////////////////////////
// CMWListCtrl message handlers
//
///////////////////////////////////////////////////////////////////////////////////
HBRUSH CMWListCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CListCtrl::OnCtlColor(pDC, pWnd, nCtlColor);
	
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
int CMWListCtrl::AddString(LPCTSTR lpszItem)
{
	return InsertString(GetCount (), lpszItem);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
int CMWListCtrl::GetCurSel()
{
	int			iX;
	BOOL		bOk;

	// check that one item gets the focus
	iX		= 0;
	bOk		= FALSE;
	while ( ! bOk && ( iX < GetItemCount() ) )
	{
		bOk = GetItemState( iX, LVIS_SELECTED) & LVIS_SELECTED;
		if ( bOk )
		{
			return ( int ) GetItemData ( iX );
		}

		iX++;
	}

	return -1;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
int CMWListCtrl::GetCount()
{
	return GetItemCount( );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
UINT CMWListCtrl::DeleteString(UINT nIndex)
{
	int			iX;
	int			iCount;
	char		strReturned [ MAX_LONG_STRING ];
	BOOL		bRes;

	bRes	= FALSE;
	iCount	= GetCount ();
	if ( iCount > 0 )
	{
		iX = nIndex + 1;
		while ( iX < iCount )
		{
			if ( iX > 0 )
			{
				GetItemText( iX, 1, strReturned, sizeof ( strReturned ) );
				SetItemText ( iX - 1, 0, GetShort ( strReturned ) );
				SetItemText ( iX - 1, 1, strReturned );
			}
			iX++;
		}

		bRes = DeleteItem ( iCount - 1 );
	}

	return bRes;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
int CMWListCtrl::InsertString(int nIndex, LPCTSTR lpszItem)
{
	int			iX;
	int			iCount;
	char		strReturned [ MAX_LONG_STRING ];

	if ( ! m_bHeaderDone )
	{
		SetImageList ( &theApp.m_imgImageList16x2, LVSIL_SMALL );
		SetImageList ( &theApp.m_imgImageList32x2, LVSIL_NORMAL );
		InsertColumn ( 0, "Name",			LVCFMT_LEFT, 72);
		InsertColumn ( 1, "Description",	LVCFMT_LEFT, 72);
		m_bHeaderDone = true;
	}

	iCount	= GetCount ();
	if ( nIndex < iCount )
	{
		iX = iCount + 1;
		while ( iX > nIndex )
		{
			GetItemText( iX - 1, 1, strReturned, sizeof ( strReturned ) );
			if ( iX > iCount )
			{
				InsertItem ( iX, GetShort ( strReturned ), 0 );
				SetItemText ( iX, 1, strReturned );
				SetItemData ( iX, ( DWORD ) iX );
			}
			else
			{
				SetItemText ( iX, 0, GetShort ( strReturned ) );
				SetItemText ( iX, 1, strReturned );
				SetItemData ( iX, ( DWORD ) iX );
			}
			iX--;
		}

		SetItemText ( nIndex, 0, GetShort ( lpszItem ) );
		SetItemText ( nIndex, 1, lpszItem );
		SetItemData ( nIndex, ( DWORD ) nIndex );
	}
	//		Only Insert the item
	else
	{
		InsertItem ( nIndex, GetShort ( lpszItem ), 0 );
		SetItemText ( nIndex, 1, lpszItem );
		SetItemData ( nIndex, ( DWORD ) nIndex );
	}

	int iStringWidth = GetStringWidth( GetShort ( lpszItem ) ) + 32;
	int iColumnWidth = GetColumnWidth( 0 );

	if ( iStringWidth > iColumnWidth )
	{
		SetColumnWidth ( 0, iStringWidth );
	}

	iStringWidth = GetStringWidth( lpszItem ) + 32;
	iColumnWidth = GetColumnWidth( 1 );

	if ( iStringWidth > iColumnWidth )
	{
		SetColumnWidth ( 1, iStringWidth );
	}

	return 0;
}

//
/////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////
int CMWListCtrl::GetText(int nIndex, LPTSTR lpszBuffer, size_t iBuffer )
{
	CString strReturned = GetItemText( nIndex, 1 );

	strcpy_s ( lpszBuffer, iBuffer, strReturned );

	return 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
int CMWListCtrl::SetCurSel(int nSelect)
{
	int		iCount;
	int		iX;

	iX		= 0;
	iCount	= GetCount ();
	while ( iX < iCount )
	{
		SetItemState( iX, 0, LVIS_SELECTED | LVIS_FOCUSED);
		iX++;
	}

	SetItemState( nSelect, LVIS_SELECTED | LVIS_FOCUSED, 
						LVIS_SELECTED | LVIS_FOCUSED);
	SetFocus ( );

	EnsureVisible( nSelect, FALSE );

	return 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWListCtrl::ResetContent()
{
	DeleteAllItems ();
	return;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
char * CMWListCtrl::GetShort(const char *pString)
{
	char		*pReturned;
	unsigned	iLen;

	iLen = (unsigned int) strlen ( pString );
	pReturned = ( char * ) pString + iLen;
	while ( pReturned > pString )
	{
		if ( *pReturned == '\\' )
		{
			return pReturned + 1;
		}
		pReturned--;
	}
	
	return ( pReturned );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWListCtrl::OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO
	CWnd	*wndParent	= GetParent ();

	LONG  id		= GetWindowLong ( this->m_hWnd, GWL_ID  );
	WPARAM wParam	= MAKEWPARAM(id, 0);
	::PostMessage ( wndParent->m_hWnd, WM_MW_LISTCTRL_END, wParam, ( LPARAM ) this->m_hWnd );
	
	*pResult = 0;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWListCtrl::SetAnsiVarFont()
{
	::SendMessage ( this->m_hWnd, WM_SETFONT, ( WPARAM ) ( ::GetStockObject ( ANSI_VAR_FONT ) ), 1 );

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWListCtrl::SetAnsiFixedFont()
{
	::SendMessage ( this->m_hWnd, WM_SETFONT, ( WPARAM ) ( ::GetStockObject ( ANSI_FIXED_FONT ) ), 1 );
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
BOOL CMWListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO
	BOOL bRes = FriendEraseBkgnd(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

	return CListCtrl::OnEraseBkgnd(pDC);
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWListCtrl::SendNormalMessage()
{
	m_bSendNormal	= true;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    // TODO
    *pResult = 0;

    // TODO
    if( GetHeaderCtrl ( ) && m_bHeaderControlSubclassed == false )
    {
        m_HeaderCtrl.SubclassWindow(GetHeaderCtrl()->m_hWnd);
        m_bHeaderControlSubclassed = true;
        m_HeaderCtrl.RedrawWindow ( );
    }

    //  for this notification, the structure is actually a
    //  NMLVCUSTOMDRAW that tells you what's going on with the custom
    //  draw action. So, we'll need to cast the generic pNMHDR pointer.
    LPNMLVCUSTOMDRAW    pLVCD   = (LPNMLVCUSTOMDRAW)pNMHDR;

    if ( pNMCD )
    {
        switch ( pNMCD->dwDrawStage )
        {
            case CDDS_PREPAINT :
            {
                *pResult = CDRF_NOTIFYITEMDRAW;
                break;
            }

            case CDDS_ITEMPREPAINT :
            {
                *pResult = CDRF_NOTIFYSUBITEMDRAW;
                break;
            }
    
            case CDDS_ITEMPREPAINT | CDDS_SUBITEM :
            {
                CRect rectLabel (0,0,0,0);
                CRect rectIcon (0,0,0,0);
                
                //
                int iCol        = pLVCD->iSubItem;
                int iRow        = ( int ) pNMCD->dwItemSpec;
                CString strItem = GetItemText ( iRow, iCol );
                BOOL bResult    = GetSubItemRect ( iRow, iCol, LVIR_LABEL, rectLabel );
                BOOL bSelected  = GetItemState ( iRow, LVIS_SELECTED ) & LVIS_SELECTED;

                //
                LVITEM lvItem;
                ZeroMemory ( &lvItem, sizeof(lvItem) );
                lvItem.iItem        = iRow;
                lvItem.iSubItem     = iCol;
                lvItem.mask         = LVIF_IMAGE;
                BOOL bImage         = GetItem ( &lvItem );
                if ( bImage )
                {
                    bResult    = GetSubItemRect ( iRow, iCol, LVIR_ICON, rectIcon );
                    CImageList *pImageList = GetImageList ( LVSIL_NORMAL );
                    if ( pImageList != NULL )
                    {
                        CDC *pDC= CDC::FromHandle ( pNMCD->hdc );
                        POINT pt; pt.x = rectIcon.left; pt.y = rectIcon.top;
                        SIZE sz; sz.cx = rectIcon.Width(); sz.cy = rectIcon.Height();
                        pImageList->DrawEx( pDC, lvItem.iImage, pt, sz, 0, 0, ILD_NORMAL );

                        // *pResult = CDRF_DODEFAULT;
                        // break;
                    }

                    pImageList = GetImageList ( LVSIL_SMALL );
                    if ( pImageList != NULL )
                    {
                        CDC *pDC= CDC::FromHandle ( pNMCD->hdc );
                        POINT pt; pt.x = rectIcon.left; pt.y = rectIcon.top;
                        SIZE sz; sz.cx = rectIcon.Width(); sz.cy = rectIcon.Height();
                        pImageList->DrawEx( pDC, lvItem.iImage, pt, sz, 0, 0, ILD_NORMAL );

                        // *pResult = CDRF_DODEFAULT;
                        // break;
                    }

                    pImageList = GetImageList ( LVSIL_STATE );
                    if ( pImageList != NULL )
                    {
                        CDC *pDC= CDC::FromHandle ( pNMCD->hdc );
                        POINT pt; pt.x = rectIcon.left; pt.y = rectIcon.top;
                        SIZE sz; sz.cx = rectIcon.Width(); sz.cy = rectIcon.Height();
                        pImageList->DrawEx( pDC, lvItem.iImage, pt, sz, 0, 0, ILD_NORMAL );

                        // *pResult = CDRF_DODEFAULT;
                        // break;
                    }
                }

                //
                HDITEM  tagHeaderInfo;
                memset ( &tagHeaderInfo, 0, sizeof ( tagHeaderInfo ) );

                tagHeaderInfo.mask          =  HDI_FORMAT;
                GetHeaderCtrl()->GetItem ( iCol, &tagHeaderInfo );

                // get the device context.
                CDC *pDC= CDC::FromHandle ( pNMCD->hdc );

                //  Test If line is selected
                if ( bSelected )
                {
                    CBrush *brush = CMWColors::GetBKSelectedCBrush(CMWColors::m_iDarkTheme != 0);
                    pDC->FillRect ( &rectLabel, brush );
                    pDC->SetTextColor ( CMWColors::GetFGSelectedCR(CMWColors::m_iDarkTheme != 0) );
                }
                else
                {
                    if ( iCol % 1 == 0 )
                    {
                        CBrush *brush = CMWColors::GetBKNormalCBrush(CMWColors::m_iDarkTheme != 0);
                        pDC->FillRect ( &rectLabel, brush );
                        pDC->SetTextColor ( CMWColors::GetFGNormalCR(CMWColors::m_iDarkTheme != 0) );
                    }
                    else
                    {
                        CBrush *brush = CMWColors::GetBKNormalCBrush(CMWColors::m_iDarkTheme != 0);
                        pDC->FillRect ( &rectLabel, brush );
                        pDC->SetTextColor ( CMWColors::GetFGNormalCR(CMWColors::m_iDarkTheme != 0) );
                    }
                }

                //
                //  Paint The Text
                rectLabel.left  += 1;
                rectLabel.right -= 1;
                UINT nFormat = DT_SINGLELINE|DT_VCENTER;
                if ( tagHeaderInfo.fmt  & HDF_LEFT )
                {
                    nFormat |= DT_LEFT;
                }
                else if ( tagHeaderInfo.fmt  & HDF_RIGHT  )
                {
                    nFormat |= DT_RIGHT;
                }
                else if ( tagHeaderInfo.fmt  & HDF_CENTER )
                {
                    nFormat |= DT_CENTER;
                }
                pDC->DrawText(strItem , rectLabel, nFormat );

                *pResult= CDRF_SKIPDEFAULT;

                break;
            }

            default:
            {
                *pResult = CDRF_DODEFAULT;
                break;
            }
        }
    }

}
