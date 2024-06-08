#include "stdafx.h"
#include "MWCHeaderCtrl.h"
#include "Friend.h"

IMPLEMENT_DYNAMIC(CMWCHeaderCtrl, CHeaderCtrl)

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
CMWCHeaderCtrl::CMWCHeaderCtrl(void)
{
}


//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
CMWCHeaderCtrl::~CMWCHeaderCtrl(void)
{
}
BEGIN_MESSAGE_MAP(CMWCHeaderCtrl, CHeaderCtrl)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CMWCHeaderCtrl::OnNMCustomdraw)
    ON_WM_ERASEBKGND()
    ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
void CMWCHeaderCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    //
    *pResult = 0;

    //

    if ( pNMCD )
    {
        switch(pNMCD->dwDrawStage)
        {
            case CDDS_PREPAINT:
            {
                *pResult = CDRF_NOTIFYITEMDRAW;
                *pResult = CDRF_NOTIFYSUBITEMDRAW;
                break;
            }

//          case CDDS_ITEMPREPAINT:
//          {
//              *pResult = CDRF_NOTIFYSUBITEMDRAW;
//              break;
//          }

            case CDDS_SUBITEM:
            case CDDS_ITEMPREPAINT|CDDS_SUBITEM:
            case CDDS_ITEMPREPAINT:
            {
                int iCol = ( int ) pNMCD->dwItemSpec;

                HDITEM  tagHeaderInfo;
                memset ( &tagHeaderInfo, 0, sizeof ( tagHeaderInfo ) );

                char        szText [ 64 ];
                ZeroMemory ( szText, sizeof(szText) );

                tagHeaderInfo.mask          = HDI_TEXT | HDI_IMAGE | HDI_FORMAT;
                tagHeaderInfo.pszText       = szText;
                tagHeaderInfo.cchTextMax    = sizeof ( szText ) - 1;
                GetItem ( iCol, &tagHeaderInfo );

                CRect rectLabel;
                BOOL bResult = GetItemRect( iCol, rectLabel );

                // get the device context.
                CDC *pDC= CDC::FromHandle ( pNMCD->hdc );

                pDC->SetBkMode ( TRANSPARENT );
                pDC->SetTextColor ( CMWColors::GetFGNormalCR(CMWColors::m_iDarkTheme != 0) );
                if ( tagHeaderInfo.iImage == 0 )
                {
                    CBrush *brush = CMWColors::GetBKNormalCBrush(CMWColors::m_iDarkTheme != 0);
                    pDC->FillRect ( &rectLabel, brush );
                    // EraseRectWithGrade0 ( &rectLabel, pDC );
                    // DrawBitmap ( pDC->m_hDC, &rectLabel );
                }
                else if ( tagHeaderInfo.iImage == 1 )
                {
                    CBrush *brush = CMWColors::GetBKNormalCBrush(CMWColors::m_iDarkTheme != 0);
                    pDC->FillRect ( &rectLabel, brush );
                    // EraseRectWithGrade1 ( &rectLabel, pDC );
                    // DrawBitmapDes ( pDC->m_hDC, &rectLabel );
                }
                else if ( tagHeaderInfo.iImage == 2 )
                {
                    CBrush *brush = CMWColors::GetBKNormalCBrush(CMWColors::m_iDarkTheme != 0);
                    pDC->FillRect ( &rectLabel, brush );
                    // EraseRectWithGrade2 ( &rectLabel, pDC );
                    // DrawBitmapAsc ( pDC->m_hDC, &rectLabel );
                }
                else
                {
                    if ( iCol % 2 == 0 )
                    {
                        CBrush *brush = CMWColors::GetBKNormalCBrush(CMWColors::m_iDarkTheme != 0);
                        pDC->FillRect ( &rectLabel, brush );
                    }
                    else
                    {
                        CBrush *brush = CMWColors::GetBKNormalCBrush(CMWColors::m_iDarkTheme != 0);
                        pDC->FillRect ( &rectLabel, brush );
                    }

                    //DrawBitmap ( pDC->m_hDC, &rectLabel );

                }

                // paint the text.
                rectLabel.left  += 2;
                rectLabel.right -= 2;

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

                pDC->DrawText(szText, rectLabel, nFormat);

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

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CMWCHeaderCtrl::OnEraseBkgnd(CDC* pDC)
{
    //
	BOOL bRes = FriendEraseBkgnd(this, pDC);
	if ( bRes )
	{
		return bRes;
	}

    return CHeaderCtrl::OnEraseBkgnd(pDC);
}

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
HBRUSH CMWCHeaderCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CHeaderCtrl::OnCtlColor(pDC, pWnd, nCtlColor);

    //

    //
    return hbr;
}
