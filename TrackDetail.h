#if !defined(AFX_TRACKDETAIL_H__6D25D921_DE07_11D2_8147_444553540000__INCLUDED_)
#define AFX_TRACKDETAIL_H__6D25D921_DE07_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrackDetail.h : header file
//

#include "MWObjects.h"
#include "MWDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CTrackDetail dialog

class CTrackDetail : public CMWDialog
{
    DECLARE_DYNAMIC(CTrackDetail)

	friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

// Construction
public:
	CTrackDetail(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTrackDetail)
	enum { IDD = IDD_TRACK_DETAIL };
	CMWButton	m_Ok;
	CMWButton	m_Cancel;
	CMWEdit	m_Track_Size;
	CMWEdit	m_Track_Number;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrackDetail)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTrackDetail)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACKDETAIL_H__6D25D921_DE07_11D2_8147_444553540000__INCLUDED_)
