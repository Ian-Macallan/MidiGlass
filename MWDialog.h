#pragma once

#include <Windows.h>

//
/////////////////////////////////////////////////////////////////////////////
class CMWDialog : public CDialog
{
    DECLARE_DYNAMIC(CMWDialog)

    public:
        CMWDialog(UINT idd, CWnd *parent = NULL);
        virtual ~CMWDialog(void);


        DECLARE_MESSAGE_MAP()
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

