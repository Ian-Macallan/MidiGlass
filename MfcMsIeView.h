#if !defined(AFX_MFCMSIEVIEW_H__C9BB3CC4_E519_11D2_8148_444553540000__INCLUDED_)
#define AFX_MFCMSIEVIEW_H__C9BB3CC4_E519_11D2_8148_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MfcMsIeView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMfcMsIeView html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CMfcMsIeView : public CHtmlView
{
	friend	void	FriendActivate(BOOL bActivate, CView* pActivateView, CView* pDeactiveView, bool bForce);

protected:
	CMfcMsIeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMfcMsIeView)

// html Data
public:
	//{{AFX_DATA(CMfcMsIeView)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfcMsIeView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMfcMsIeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMfcMsIeView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCMSIEVIEW_H__C9BB3CC4_E519_11D2_8148_444553540000__INCLUDED_)
