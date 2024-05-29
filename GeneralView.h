#if !defined(AFX_GENERALVIEW_H__1EEF4D1A_B50C_11D2_8147_444553540000__INCLUDED_)
#define AFX_GENERALVIEW_H__1EEF4D1A_B50C_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GeneralView.h : header file
//
#include "MWObjects.h"
#include "StaticUrl.h"
#include "MWFormView.h"

//
/////////////////////////////////////////////////////////////////
// CGeneralView form view
//
/////////////////////////////////////////////////////////////////
#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//
/////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////
class CGeneralView : public CMWFormView
{
		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgndScrollView(CScrollView* pWnd, CDC* pDC);
		friend	void	FriendActivate(BOOL bActivate, CView* pActivateView, CView* pDeactiveView, bool bForce);

	protected:
		CGeneralView();           // protected constructor used by dynamic creation
		DECLARE_DYNCREATE(CGeneralView)

	// Form Data
	public:
		//{{AFX_DATA(CGeneralView)
		enum { IDD = IDD_GENERAL_VIEW };
		CStaticUrl	m_Web_Address;
		CStaticUrl	m_Mail_Address;
		CMWEdit	m_Text;
		CMWEdit	m_NbTracks;
		CMWEdit	m_Format;
		CMWEdit	m_Division_Lo;
		CMWEdit	m_Division_Hi;
		//}}AFX_DATA

	// Attributes
	public:
	protected :
		bool				m_bFirstTime;

	// Operations
	public:
		unsigned GetIDD();
		void DisplayValues();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CGeneralView)
		public:
		virtual void OnInitialUpdate();
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
		virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
		//}}AFX_VIRTUAL

	// Implementation
	protected:
		virtual ~CGeneralView();
	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif

		// Generated message map functions
		//{{AFX_MSG(CGeneralView)
		afx_msg void OnChangeDivisionHi();
		afx_msg void OnChangeDivisionLo();
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnMail();
		afx_msg void OnWeb();
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnMove(int x, int y);
		afx_msg void OnSize(UINT nType, int cx, int cy);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERALVIEW_H__1EEF4D1A_B50C_11D2_8147_444553540000__INCLUDED_)
