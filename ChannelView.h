#if !defined(AFX_CHANNELVIEW_H__FB9271E2_DEF8_11D2_8147_444553540000__INCLUDED_)
#define AFX_CHANNELVIEW_H__FB9271E2_DEF8_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChannelView.h : header file
//

#include "MWFormView.h"

/////////////////////////////////////////////////////////////////////////////
// CChannelView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CChannelView : public CMWFormView
{
		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgndScrollView(CScrollView* pWnd, CDC* pDC);
		friend	void	FriendActivate(BOOL bActivate, CView* pActivateView, CView* pDeactiveView, bool bForce );

	protected:
		CChannelView();           // protected constructor used by dynamic creation
		DECLARE_DYNCREATE(CChannelView)

	// Form Data
	public:
		//{{AFX_DATA(CChannelView)
		enum { IDD = IDD_CHANNELS };
		CMWListEditCtrl	m_Channels;
		//}}AFX_DATA

	// Attributes
	public:

	// Operations
	public:
		LRESULT OnEditSelect(WPARAM wParam, LPARAM lParam);
		LRESULT OnEditEnd(WPARAM wParam, LPARAM lParam);
		LRESULT OnEditBegin(WPARAM wParam, LPARAM lParam);
		void DisplayValues();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CChannelView)
		public:
		virtual void OnInitialUpdate();
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
		virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
		//}}AFX_VIRTUAL

	// Implementation
	protected:
		CMWListEditCtrl * m_pChannels;
		bool m_bFirstTime;
		virtual ~CChannelView();
	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif

		// Generated message map functions
		//{{AFX_MSG(CChannelView)
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		afx_msg void OnChannelsHelp();
		afx_msg void OnChannelsAudible();
		afx_msg void OnChannelsMute();
		afx_msg void OnChannelsMuteall();
		afx_msg void OnChannelsAudibleall();
		afx_msg void OnChannelsSolo();
		afx_msg void OnClickChannels(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnDblclkChannels(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnMove(int x, int y);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANNELVIEW_H__FB9271E2_DEF8_11D2_8147_444553540000__INCLUDED_)
