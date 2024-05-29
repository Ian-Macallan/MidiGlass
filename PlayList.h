#if !defined(AFX_PLAYLIST_H__48DE5426_D3E4_11D2_8147_444553540000__INCLUDED_)
#define AFX_PLAYLIST_H__48DE5426_D3E4_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayList.h : header file
//
#include "MWObjects.h"
#include "MWFormView.h"

/////////////////////////////////////////////////////////////////////////////
// CPlayList form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CPlayList : public CMWFormView
{
		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgndScrollView(CScrollView * Wnd, CDC* pDC);
		friend	void	FriendActivate(BOOL bActivate, CView* pActivateView, CView* pDeactiveView, bool bForce);

	protected:
		CPlayList();           // protected constructor used by dynamic creation
		DECLARE_DYNCREATE(CPlayList)

	// Form Data
	public:
		//{{AFX_DATA(CPlayList)
		enum { IDD = IDD_PLAYLIST };
		CMWButton	m_Arrange;
		CComboBox	m_Display;
		CMWListCtrl	m_PlayList;
		CMWButton	m_Up;
		CMWButton	m_Stop;
		CMWButton	m_Play;
		CMWButton	m_Down;
		CMWButton	m_Save;
		CMWButton	m_Remove;
		CMWButton	m_Add;
		//}}AFX_DATA

	// Attributes
	public:

	// Operations
	public:
		void UnSelectAll();
		void OnButtonUp();
		char * GetCurrentPlaylistFilename();
		void DisplayButtons();
		void SaveTheList();
		void DisplayValues();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CPlayList)
		public:
		virtual void OnInitialUpdate();
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
		virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
		//}}AFX_VIRTUAL

	// Implementation
	protected:
		//		Simple drag and drop
		bool				m_bDragging;
		int					m_hItemDrag;
		int					m_hItemDrop;

		//		Image list
		CImageList			*m_pImageList;

		bool				m_bPlayListModified;
		char				m_szFilename [ MAX_PATHNAME_STRING ];
		bool				m_bFirstTime;

	protected:
		virtual ~CPlayList();
	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif

		// Generated message map functions
		//{{AFX_MSG(CPlayList)
		afx_msg void OnAdd();
		afx_msg void OnRemove();
		afx_msg void OnSave();
		afx_msg void OnUp();
		afx_msg void OnDown();
		afx_msg void OnPlay();
		afx_msg void OnStop();
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnDestroy();
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnDblclkPlaylist(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnKillfocusPlaylist(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnItemchangedPlaylist(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnBegindragPlaylist(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnSelchangeDisplay();
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		afx_msg void OnPlaylistOpenfile();
		afx_msg void OnPlaylistHelp();
		afx_msg void OnPlaylistProperty();
		afx_msg void OnArrange();
		afx_msg void OnList();
		afx_msg void OnLargeicons();
		afx_msg void OnSmallicons();
		afx_msg void OnReport();
		afx_msg void OnMenuArrange();
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYLIST_H__48DE5426_D3E4_11D2_8147_444553540000__INCLUDED_)
