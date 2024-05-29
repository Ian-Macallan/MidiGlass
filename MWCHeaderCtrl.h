#pragma once

#include <Windows.h>

//
/////////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////////
class CMWCHeaderCtrl : public CHeaderCtrl
{
    DECLARE_DYNAMIC(CMWCHeaderCtrl)

    public:
        CMWCHeaderCtrl(void);
        virtual ~CMWCHeaderCtrl(void);

        DECLARE_MESSAGE_MAP()
        afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

