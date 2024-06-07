#if !defined(AFX_PLAYLISTPROPERTY_H__0396C621_D71A_11D2_8147_444553540000__INCLUDED_)
#define AFX_PLAYLISTPROPERTY_H__0396C621_D71A_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayListProperty.h : header file
//
#include "MWObjects.h"
#include "MWDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CPlayListProperty dialog

class CPlayListProperty : public CMWDialog
{
	friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

// Construction
public:
	CPlayListProperty(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPlayListProperty)
	enum { IDD = IDD_PLAYLIST_PROPERTY };
	CMWButton	m_Ok;
	CMWEdit	m_Filename;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayListProperty)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlayListProperty)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYLISTPROPERTY_H__0396C621_D71A_11D2_8147_444553540000__INCLUDED_)
