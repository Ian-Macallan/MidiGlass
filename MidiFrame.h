#if !defined(AFX_MIDIFRAME_H__4A0BCB44_0EB6_11D3_8148_444553540000__INCLUDED_)
#define AFX_MIDIFRAME_H__4A0BCB44_0EB6_11D3_8148_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MidiFrame.h : header file
//

#include "MWMenu.h"

/////////////////////////////////////////////////////////////////////////////
// CMidiFrame frame

class CMidiFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CMidiFrame)

    public:
	    CMidiFrame();           // protected constructor used by dynamic creation

    // Attributes
    public:
       CMWMenu     *m_pFrameMenu;
      CMWNCColor    m_NC;

    // Operations
    public:

    // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(CMidiFrame)
	    //}}AFX_VIRTUAL

    // Implementation
    public:
	    virtual ~CMidiFrame();

	    // Generated message map functions
	    //{{AFX_MSG(CMidiFrame)
		    // NOTE - the ClassWizard will add and remove member functions here.
	    //}}AFX_MSG
	    DECLARE_MESSAGE_MAP()
    public:
        afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
        afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
        afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//        afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
        afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

        afx_msg BOOL OnNcActivate(BOOL bActive);
        afx_msg void OnNcPaint();
        afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
        afx_msg void OnNcMouseLeave();
        afx_msg void OnNcMouseHover(UINT nFlags, CPoint point);
        afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
        afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIDIFRAME_H__4A0BCB44_0EB6_11D3_8148_444553540000__INCLUDED_)
