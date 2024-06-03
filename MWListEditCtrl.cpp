// MWListEditCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MidiGlassApp.h"
#include "MWListEditCtrl.h"
#include "MWObjects.h"
#include "Friend.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CMWListEditCtrl, CListCtrl)

//
/////////////////////////////////////////////////////////////////////////////
// CMWListEditCtrl
/////////////////////////////////////////////////////////////////////////////
CMWListEditCtrl::CMWListEditCtrl()
{
	int			iX;

	m_pEditControl = NULL;
	m_pSpinControl = NULL;

	for ( iX = 0; iX < MAX_COLUMNS; iX++ )
	{
		m_bEditColumn [ iX ] = true;
		m_bSpinColumn [ iX ] = false;
		m_iSpinLowRng [ iX ] = 0;
		m_iSpinHigRng [ iX ] = 255;
	}

	m_bSendNormal               = false;
    m_bHeaderControlSubclassed  = false;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CMWListEditCtrl::~CMWListEditCtrl()
{
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMWListEditCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMWListEditCtrl)
	ON_NOTIFY_REFLECT(NM_KILLFOCUS, OnKillfocus)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_MW_EDIT_END,OnEditEnd)
	ON_MESSAGE(WM_MW_EDIT_SELECT,OnEditSelect)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMWListEditCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////
// CMWListEditCtrl message handlers
//
/////////////////////////////////////////////////////////////////////////////
int CMWListEditCtrl::GetCurSel()
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
			return ( int ) iX;
		}

		iX++;
	}

	return -1;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
bool CMWListEditCtrl::EditColumn(int iSubItem)
{
	int				iItem;

	iItem = GetCurSel ();
	if ( iItem >= 0 )
	{
		return EditColumn( iItem, iSubItem);
	}

	return false;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
bool CMWListEditCtrl::EditColumn(int iItem, int iSubItem)
{
	BOOL			bResult;
	CRect			rectLabel;
	CString			strValue;
	LVCOLUMN		lvColumn;

	lvColumn.mask = LVCF_SUBITEM;
	bResult = GetColumn ( iSubItem, &lvColumn );
	if ( bResult )
	{
		if ( m_pEditControl != NULL )
		{
			OnEditEnd ( 0, ( LPARAM ) m_pEditControl );
		}

		if ( m_bEditColumn [ iSubItem ] )
		{
			bResult = GetSubItemRect( iItem, iSubItem, LVIR_LABEL, rectLabel );
			CreateSubItemControl(iItem, iSubItem, rectLabel);
			return true;
		}
	}

	return false;

}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
void CMWListEditCtrl::ResetContent()
{
	DeleteAllItems ();
	return;
}

//
/////////////////////////////////////////////////////////////////////////////
//	return always 1 : true
/////////////////////////////////////////////////////////////////////////////
LRESULT CMWListEditCtrl::OnEditEnd(WPARAM wParam, LPARAM lParam)
{
	CString			strValue;
	CWnd			*wndParent	= GetParent ();

	if ( m_pEditControl != NULL && m_pEditControl == ( CWnd * ) lParam )
	{
		if ( m_pSpinControl != NULL )
		{
			delete m_pSpinControl;
			m_pSpinControl = NULL;
		}

		if ( ! SendMessageToParent(WM_MW_EDIT_END) )
		{
			m_pEditControl->GetWindowText ( strValue );
			SetItemText ( m_iEditItem, m_iEditSubItem, strValue );

			delete m_pEditControl;
			m_pEditControl = NULL;
		}
		else
		{
			if ( m_pEditControl != NULL )
			{
				delete m_pEditControl;
				m_pEditControl = NULL;
			}
		}

	}

	return 1;
}

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
CWnd * CMWListEditCtrl::CreateSubItemControl(int iItem, int iSubItem, CRect rectLabel)
{
	BOOL			bResult;
	CString			strValue;
	CWnd			*wndParent	= GetParent ();

	m_iEditItem		= iItem;
	m_iEditSubItem	= iSubItem;

	m_pEditControl = ( CWnd * ) SendMessageToParent(WM_MW_EDIT_BEGIN);
	if ( m_pEditControl != NULL )
	{
		m_pEditControl->SetFocus ();

		return m_pEditControl;
	}

	m_pEditControl	= new CMWEditInList ();

	if ( m_pEditControl != NULL )
	{
		bResult = 
			( ( CMWEditInList * ) m_pEditControl )->Create (
				WS_CHILD | WS_BORDER | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
				rectLabel, this, IDC_EDIT_COLUMNS );
		if ( bResult == TRUE )
		{
			( ( CMWEditInList * ) m_pEditControl )->SetAnsiVarFont ( );
			strValue = GetItemText( iItem, iSubItem );
			m_pEditControl->SetWindowText ( strValue );
		}

		if ( m_bSpinColumn [ iSubItem ] )
		{
			// Create spin (up-down) control
			m_pSpinControl = new CSpinButtonCtrl ();

			RECT	rectSpin;

			rectSpin.left	= rectLabel.right - 4;
			rectSpin.right	= rectLabel.right;
			rectSpin.top	= rectLabel.top;
			rectSpin.bottom	= rectLabel.bottom;
			( ( CSpinButtonCtrl * ) m_pSpinControl )->Create (
				WS_VISIBLE | WS_CHILD |
				UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_AUTOBUDDY |
				UDS_SETBUDDYINT | UDS_WRAP,
				rectSpin, this, IDC_EDIT_COLUMNS_SPIN );
			( ( CSpinButtonCtrl * ) m_pSpinControl )->SetRange ( m_iSpinLowRng [ iSubItem ], m_iSpinHigRng [ iSubItem ] );
			( ( CSpinButtonCtrl * ) m_pSpinControl )->SetPos ( atoi ( strValue ) );

		}

		( ( CMWEditInList * ) m_pEditControl )->SetSel ( 0, (int) strlen ( strValue ) );
		( ( CMWEditInList * ) m_pEditControl )->SetFocus ( );
	}

	return ( CWnd * ) m_pEditControl;

}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CMWListEditCtrl::OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO
	CWnd	*wndParent	= GetParent ();

	LONG  id		= GetWindowLong ( this->m_hWnd, GWL_ID  );
	WPARAM wParam	= MAKEWPARAM(id, 0);
	::PostMessage ( wndParent->m_hWnd, WM_MW_LISTEDITCTRL_END, wParam, ( LPARAM ) this->m_hWnd );

	*pResult = 0;
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
LRESULT CMWListEditCtrl::SendMessageToParent(UINT wMsg)
{
	CWnd			*wndParent	= GetParent ();

	LONG  id		= GetWindowLong ( this->m_hWnd, GWL_ID  );
	WPARAM wParam	= MAKEWPARAM(id, 0);
	LRESULT lResult = ::SendMessage ( wndParent->m_hWnd, wMsg, m_iEditItem * MAX_COLUMNS + m_iEditSubItem, ( LPARAM ) m_pEditControl );
	return lResult;
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
LRESULT CMWListEditCtrl::PostMessageToParent(UINT wMsg)
{
	CWnd			*wndParent	= GetParent ();

	LONG  id		= GetWindowLong ( this->m_hWnd, GWL_ID  );
	WPARAM wParam	= MAKEWPARAM(id, 0);
	return ::PostMessage ( wndParent->m_hWnd, wMsg, m_iEditItem * MAX_COLUMNS + m_iEditSubItem, ( LPARAM ) m_pEditControl );
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CMWListEditCtrl::SetEditFlag(int iSubItem, bool bFlag)
{
	m_bEditColumn [ iSubItem ] = bFlag;
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
int CMWListEditCtrl::GetItemIndex()
{
	return m_iEditItem;
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
int CMWListEditCtrl::GetSubItemIndex()
{
	return m_iEditSubItem;
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
CWnd * CMWListEditCtrl::GetEditControlHandle()
{
	return m_pEditControl;
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CMWListEditCtrl::SetEditControlHandle(CWnd *wndControl)
{
	m_pEditControl = wndControl;
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CMWListEditCtrl::DeleteAllItems()
{

	if ( m_pSpinControl != NULL )
	{
		delete m_pSpinControl;
		m_pSpinControl = NULL;
	}

	//		Simply delete the control
	if ( m_pEditControl != NULL )
	{
		delete m_pEditControl;
		m_pEditControl = NULL;
	}

	//		Then use parent class deleteallitems
	CListCtrl::DeleteAllItems ();
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
LRESULT CMWListEditCtrl::OnEditSelect(WPARAM wParam, LPARAM lParam)
{
	CWnd	*wndParent	= GetParent ();

	LONG  id		= GetWindowLong ( this->m_hWnd, GWL_ID  );
	//WPARAM wParam	= MAKEWPARAM(id, 0);
	::PostMessage ( wndParent->m_hWnd, WM_MW_EDIT_SELECT, wParam, ( LPARAM ) this->m_hWnd );

	return 1;
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CMWListEditCtrl::EnableSpinCtrl(int iSubItem)
{
	m_bSpinColumn [ iSubItem ] = true;
}

void CMWListEditCtrl::SetSpinCtrlRange(int iSubItem, int iLow, int iHigh)
{
		m_iSpinLowRng [ iSubItem ] = iLow;
		m_iSpinHigRng [ iSubItem ] = iHigh;
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
void CMWListEditCtrl::SetAnsiVarFont()
{
	::SendMessage ( this->m_hWnd, WM_SETFONT, ( WPARAM ) ( ::GetStockObject ( ANSI_VAR_FONT ) ), 1 );

}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWListEditCtrl::SetAnsiFixedFont()
{
	::SendMessage ( this->m_hWnd, WM_SETFONT, ( WPARAM ) ( ::GetStockObject ( ANSI_FIXED_FONT ) ), 1 );
}

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
HBRUSH CMWListEditCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
BOOL CMWListEditCtrl::OnEraseBkgnd(CDC* pDC) 
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
void CMWListEditCtrl::SendNormalMessage()
{
	m_bSendNormal	= true;
}

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
void CMWListEditCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
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
                if ( true || iCol == 0 )
                {
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
                    CTRXBrush brush;
                    brush.CreateSolidBrush ( black0x20 );
                    pDC->FillRect ( &rectLabel, &brush );
                    pDC->SetTextColor ( white0xff );
                }
                else
                {
                    if ( iCol % 1 == 0 )
                    {
                        CTRXBrush brush;
                        brush.CreateSolidBrush ( black0x40 );
                        pDC->FillRect ( &rectLabel, &brush );
                        pDC->SetTextColor ( white0xee );
                    }
                    else
                    {
                        CTRXBrush brush;
                        brush.CreateSolidBrush ( black0x30 );
                        pDC->FillRect ( &rectLabel, &brush );
                        pDC->SetTextColor ( white0xdd );
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
