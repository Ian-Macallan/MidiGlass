#if !defined(AFX_NOTESVIEW_H__3BE1F524_F1E3_11D2_8148_444553540000__INCLUDED_)
#define AFX_NOTESVIEW_H__3BE1F524_F1E3_11D2_8148_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotesView.h : header file
//

#include "MWFormView.h"

/////////////////////////////////////////////////////////////////////////////
// CNotesView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CNotesView : public CMWFormView
{
	friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	friend	BOOL	FriendEraseBkgndScrollView(CScrollView* pWnd, CDC* pDC);
	friend	void	FriendActivate(BOOL bActivate, CView* pActivateView, CView* pDeactiveView, bool bForce);

protected:
	CNotesView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CNotesView)

// Form Data
public:
	//{{AFX_DATA(CNotesView)
	enum { IDD = IDD_NOTES_VIEW };
	CRichEditCtrl	m_Rich_Edit;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNotesView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CNotesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CNotesView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTESVIEW_H__3BE1F524_F1E3_11D2_8148_444553540000__INCLUDED_)
