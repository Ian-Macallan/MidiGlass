#if !defined(AFX_TRACKSVIEW_H__1EEF4D1B_B50C_11D2_8147_444553540000__INCLUDED_)
#define AFX_TRACKSVIEW_H__1EEF4D1B_B50C_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TracksView.h : header file
//

#include "MWObjects.h"
#include "MWFormView.h"
#include "MidiTrack.h"

/////////////////////////////////////////////////////////////////////////////
// CTracksView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CTracksView : public CMWFormView
{
		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgndScrollView(CScrollView* pWnd, CDC* pDC);
		friend	void	FriendActivate(BOOL bActivate, CView* pActivateView, CView* pDeactiveView, bool bForce);

	protected:
		CTracksView();           // protected constructor used by dynamic creation
		DECLARE_DYNCREATE(CTracksView)

	// Form Data
	public:
		//{{AFX_DATA(CTracksView)
		enum { IDD = IDD_TRACKS_VIEW };
		CMWListEditCtrl	m_Tracks_List;
		//}}AFX_DATA

	// Attributes
	public:

	// Operations
	public:
		void Merge ( int iFrom, int iTo );
		void UnSelectAll();
		void OnButtonUp();
		LRESULT OnEditSelect(WPARAM wParam, LPARAM lParam);
		LRESULT OnEditEnd(WPARAM wParam, LPARAM lParam);
		LRESULT OnEditBegin(WPARAM wParam, LPARAM lParam);
		void ShowTrackDetail();
		CMidiTrack *GetCurrentTrack();
		void DisplayValues();
		void AskAndDeleteTracks();
		void DeleteTracks();
		unsigned GetIDD();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CTracksView)
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

		int					m_iCurrentTrack;
		CListCtrl			* m_pTracks_List;
		bool				m_bFirstTime;

		virtual ~CTracksView();
	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif

		// Generated message map functions
		//{{AFX_MSG(CTracksView)
		afx_msg void OnClickTracksList(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnKeydownTracksList(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		afx_msg void OnTracksDelete();
		afx_msg void OnTracksHelp();
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnTracksPlaysolo();
		afx_msg void OnTracksProperty();
		afx_msg void OnDblclkTracksList(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnTracksMute();
		afx_msg void OnTracksAudible();
		afx_msg void OnTracksMuteall();
		afx_msg void OnTracksAudibleall();
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnBegindragTracksList(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnEditCut();
		afx_msg void OnEditPaste();
		afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
		afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnMove(int x, int y);
		afx_msg void OnSize(UINT nType, int cx, int cy);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACKSVIEW_H__1EEF4D1B_B50C_11D2_8147_444553540000__INCLUDED_)
