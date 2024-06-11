#pragma once

#include <Windows.h>
#include "MWMenu.h"
#include "MWNCColor.h"

//
/////////////////////////////////////////////////////////////////////////////
class CMWDialog : public CDialog
{
    DECLARE_DYNAMIC(CMWDialog)

    public:
        CMWDialog(UINT idd, CWnd *parent = NULL);
        virtual ~CMWDialog(void);


        DECLARE_MESSAGE_MAP()

    protected :
        //
        CMWMenu            *m_pMenu;
    //  Handle Npn Client
        CMWNCColor          m_NC;

    public :
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

        afx_msg BOOL OnNcActivate(BOOL bActive);
        afx_msg void OnNcPaint();
        afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
        afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
        afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
        afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
        afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
        afx_msg void OnMouseMove(UINT nFlags, CPoint point);
        afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
        afx_msg void OnNcMouseHover(UINT nFlags, CPoint point);
        afx_msg void OnNcMouseLeave();
        virtual BOOL OnInitDialog();
};

