#pragma once

#include "afxframewndex.h"

/////////////////////////////////////////////////////////////////////////////
// CMWFrameWnd frame

class CMWFrameWnd : public CFrameWnd
{
	    DECLARE_DYNCREATE(CMWFrameWnd)
    protected:
	    CMWFrameWnd();           // protected constructor used by dynamic creation

    // Attributes
    public:

    // Operations
    public:

    // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(CMWFrameWnd)
	    //}}AFX_VIRTUAL

    // Implementation
    protected:
	    virtual ~CMWFrameWnd();

	    // Generated message map functions
	    //{{AFX_MSG(CMWFrameWnd)
		    // NOTE - the ClassWizard will add and remove member functions here.
	    //}}AFX_MSG
	    DECLARE_MESSAGE_MAP()
};

