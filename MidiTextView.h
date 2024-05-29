#if !defined(AFX_MIDITEXTVIEW_H__1EEF4D19_B50C_11D2_8147_444553540000__INCLUDED_)
#define AFX_MIDITEXTVIEW_H__1EEF4D19_B50C_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MidiTextView.h : header file
//

#include "MWObjects.h"
#include "MWFormView.h"

/////////////////////////////////////////////////////////////////////////////
// CMidiTextView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CMidiTextView : public CMWFormView
{
	friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	friend	BOOL	FriendEraseBkgndScrollView(CScrollView* pWnd, CDC* pDC);
	friend	void	FriendActivate(BOOL bActivate, CView* pActivateView, CView* pDeactiveView, bool bForce);

protected:
	CMidiTextView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMidiTextView)

// Form Data
public:
	//{{AFX_DATA(CMidiTextView)
	enum { IDD = IDD_TEXT_VIEW };
	CMWButton	m_Compact;
	CSpinButtonCtrl	m_Spin_Event;
	CMWEdit	m_Text;
	CMWEdit	m_EventName;
	//}}AFX_DATA

// Attributes
public:
protected :
	bool				m_bFirstTime;

// Operations
public:
	bool IsLineEmpty ( char *pText );
	unsigned GetIDD();
	void DisplayValues();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMidiTextView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMidiTextView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMidiTextView)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDeltaposSpinEvent(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCompact();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIDITEXTVIEW_H__1EEF4D19_B50C_11D2_8147_444553540000__INCLUDED_)
