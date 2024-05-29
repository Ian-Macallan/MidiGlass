#if !defined(AFX_ERRORVIEW_H__C12AEDE1_CF38_11D2_8147_444553540000__INCLUDED_)
#define AFX_ERRORVIEW_H__C12AEDE1_CF38_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ErrorView.h : header file
//

#include "MWObjects.h"
#include "MWFormView.h"

/////////////////////////////////////////////////////////////////////////////
// CErrorView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CErrorView : public CMWFormView
{
	friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	friend	BOOL	FriendEraseBkgndScrollView(CScrollView* pWnd, CDC* pDC);
	friend	void	FriendActivate(BOOL bActivate, CView* pActivateView, CView* pDeactiveView, bool bForce);

protected:
	CErrorView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CErrorView)

protected :
	bool				m_bFirstTime;

// Form Data
public:
	//{{AFX_DATA(CErrorView)
	enum { IDD = IDD_ERRORTEXT };
	CMWEdit	m_ErrorMessage;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void DisplayValues();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CErrorView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CErrorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CErrorView)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	CStatic m_Error_Header;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ERRORVIEW_H__C12AEDE1_CF38_11D2_8147_444553540000__INCLUDED_)
#include "afxwin.h"
