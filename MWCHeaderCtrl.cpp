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
    // TODO: ajoutez ici le code de votre gestionnaire de notification de contrôle
    *pResult = 0;

    // TODO : Add Here your Notification Handler

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
                pDC->SetTextColor ( white0xff );
                if ( tagHeaderInfo.iImage == 0 )
                {
                    CTRXBrush brush;
                    brush.CreateSolidBrush ( black0x20 );
                    pDC->FillRect ( &rectLabel, &brush );
                    // EraseRectWithGrade0 ( &rectLabel, pDC );
                    // DrawBitmap ( pDC->m_hDC, &rectLabel );
                }
                else if ( tagHeaderInfo.iImage == 1 )
                {
                    CTRXBrush brush;
                    brush.CreateSolidBrush ( black0x20 );
                    pDC->FillRect ( &rectLabel, &brush );
                    // EraseRectWithGrade1 ( &rectLabel, pDC );
                    // DrawBitmapDes ( pDC->m_hDC, &rectLabel );
                }
                else if ( tagHeaderInfo.iImage == 2 )
                {
                    CTRXBrush brush;
                    brush.CreateSolidBrush ( black0x20 );
                    pDC->FillRect ( &rectLabel, &brush );
                    // EraseRectWithGrade2 ( &rectLabel, pDC );
                    // DrawBitmapAsc ( pDC->m_hDC, &rectLabel );
                }
                else
                {
                    if ( iCol % 2 == 0 )
                    {
                        CTRXBrush brush;
                        brush.CreateSolidBrush ( black0x20 );
                        pDC->FillRect ( &rectLabel, &brush );
                    }
                    else
                    {
                        CTRXBrush brush;
                        brush.CreateSolidBrush ( black0x30 );
                        pDC->FillRect ( &rectLabel, &brush );
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
    // TODO: ajoutez ici le code de votre gestionnaire de messages et/ou les paramètres par défaut des appels
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

    // TODO:  Modifier ici les attributs du DC

    // TODO:  Retourner un autre pinceau si le pinceau par défaut n'est pas souhaité
    return hbr;
}
