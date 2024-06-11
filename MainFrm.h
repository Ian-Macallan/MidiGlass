//
/////////////////////////////////////////////////////////////////////////////
// MainFrm.h : interface of the CMainFrame class
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__F6E8EC4C_B230_11D2_8147_444553540000__INCLUDED_)
#define AFX_MAINFRM_H__F6E8EC4C_B230_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MidiDefs.h"

#include "MWObjects.h"

#include "EventView.h"
#include "MidiTextView.h"
#include "MidiOwnPlayer.h"
#include "TracksView.h"
#include "ChannelView.h"
#include "ErrorView.h"
#include "GeneralView.h"
#include "MfcMsIeView.h"
#include "PlayList.h"
#include "StaffView.h"
#include "NotesView.h"
#include "MidiExplorerView.h"

#include "MidiList.h"
#include "MidiFile.h"
#include "MidiTrack.h"

#include "MidiDoc.h"
#include "MidiView.h"
#include "ToolBarSoundFont.h"
#include "MWMenu.h"
#include "MWNCColor.h"
#include "MidiFrame.h"

//
/////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
class CMainFrame : public CFrameWnd
{
		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

	protected: // create from serialization only
		CMainFrame();
		DECLARE_DYNCREATE(CMainFrame)

        CMWNCColor  m_NC;

	// Attributes
	public:

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMainFrame)
		public:
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
		virtual void OnSetPreviewMode(BOOL bPreview, CPrintPreviewState* pState);
		protected:
		virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
		//}}AFX_VIRTUAL

	// Implementation
	public:
		void SetModified ( BOOL bFlag );
		void ActivateOrHideView ( CView *pView, const char *pText );
		bool IsViewPresentAndVisible ( CCmdUI *pCmdUI, CView *pView );
		void SaveViewGeometry ( CWnd *pWnd, CView *pView, const char *pText );
		void RestoreViewOrder ( int iX, CView *pView, const char *pText );
		void RestoreViewGeometry ( CWnd *pWnd, CView *pView, const char *pText );
		CMidiView * GetMidiView();
		void SetMidiView ( CMidiView *m_pMidiView );
		BOOL CreateColView ( int iCol, CCreateContext *pContext, 
								CView **pView, 
								CRuntimeClass *pClass, CSize size );
		BOOL CreateColViewFull ( int iCol, CView **pView, 
								CRuntimeClass *pClass, int iView );
		BOOL CreateOneView ( LPCREATESTRUCT lpcs, CCreateContext *pContext,
								const char *pTitle, CView **pView, 
								CRuntimeClass *pClass );

		void RestoreFrameGeometry( CWnd *pVnd );
		void SaveFrameGeometry( CWnd *pWnd );
		void DestroyViewWindows();
		BOOL CreateSplitterWindows ( LPCREATESTRUCT lpcs, CCreateContext* pContext );
		BOOL CreateViewWindows (LPCREATESTRUCT lpcs, CCreateContext *pContext);
		void DeleteRightView();
		void DeleteLeftView();
		bool IsMidiBarVisible();
		bool IsMciBarVisible();
		bool IsIeBarVisible();
		bool IsSF2BarVisible();
		bool IsBarVisible ( CMWToolBar *pBar, CMWToolBar *pLargeBar );
		bool IsMainBarVisible();
		void ShowOwnToolBar ( CMWToolBar *pToolBar, CMWToolBar *pLargeToolBar, int bShow );
		void ShowOwnReBar ( CMWToolBar *pToolBar, CMWToolBar *pLargeToolBar, int bShow );
		void LoadOwnToolBar();
		void SaveOwnToolBar();
		void DisplayOwnToolBars();
		void SetOwnToolBars();
		void SetActiveCWndAsView(CWnd *wnd);
		int CreateOwnStatusBar();
		int CreateOwnToolBars ( );
		void SetSearchInfo( const CString strEvent, const CString strProgram, const CString strController,
							int V1_From, int V1_To, int V2_From, int V2_To );
		void ResetLeftViewHandles();
		void DisableMenuWhenNoMidifile(CCmdUI* pCmdUI);
		CMidiDoc * GetMidiDoc ();
		CMidiFile * GetMidiFile ();
		CMidiTrack *GetCurrentTrack();
		void OnToolsToolNumber ( UINT iID );
		void AddToolsMenu();
		void ResetRightViewHandles();
		void ResetViewHandles();
		char * GetCurrentPlaylistFilename ();
		void SizeAndPlaceWindow();
		void OnUpdateViewMciToolBar ( CCmdUI *pCmdUI );
		void OnUpdateViewMidiToolBar ( CCmdUI *pCmdUI );
		LRESULT OnMciNotify( WPARAM wParam, LPARAM lParam );
		void GetDialogBoxSize( unsigned iD, LPRECT rect );
		void RefreshFrame();
		CMidiList * GetCurrentMidiEvent();
		void OnContentEvents( int iView );
		virtual ~CMainFrame();
	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif

	protected:  
		// control bar embedded members
		CMWStatusBar		m_wndStatusBar;

		//		Tools bars
		CMWToolBar			m_wndMainToolBar;
		CMWToolBar			m_wndMciToolBar;
		CMWToolBar			m_wndMidiToolBar;
		CMWToolBar			m_wndIeToolBar;
		CToolBarSoundFont	m_wndSF2ToolBar;

		CMWToolBar			m_wndMainLargeToolBar;
		CMWToolBar			m_wndMciLargeToolBar;
		CMWToolBar			m_wndMidiLargeToolBar;
		CMWToolBar			m_wndIeLargeToolBar;
		CToolBarSoundFont	m_wndSF2LargeToolBar;

		CMWToolBar			m_wndMainReBar;
		CMWToolBar			m_wndMciReBar;
		CMWToolBar			m_wndMidiReBar;
		CMWToolBar			m_wndIeReBar;
		CToolBarSoundFont	m_wndSF2ReBar;

		CMWToolBar			m_wndMainLargeReBar;
		CMWToolBar			m_wndMciLargeReBar;
		CMWToolBar			m_wndMidiLargeReBar;
		CMWToolBar			m_wndIeLargeReBar;
		CToolBarSoundFont	m_wndSF2LargeReBar;

		CMWReBar			m_wndReBar;
		CMWReBar			m_wndLargeReBar;

        CMWMenu             m_AppMenu;
        CMWMenu             *m_pContextMenu;
        CMWMenu             *m_pSysMenu;

	// Generated message map functions
	protected:
		bool m_bPreview;
		CMidiView			* m_pMidiView;
		bool				m_bClosing;

		//		Views
		CNotesView			* m_RightNotesView;
		CStaffView			* m_RightStaffView;
		// CMfcMsIeView		* m_RightBrowserView;
		CPlayList			* m_RightPlayListView;
		CMidiOwnPlayer		* m_RightPlayerView;
		CEventView			* m_RightEventView;
		CChannelView		* m_RightChannelView;
		CErrorView			* m_RightErrorView;
		CGeneralView		* m_RightGeneralView;
		CMidiTextView		* m_RightTextView;

		CTracksView			* m_LeftTrackView;
		CMidiExplorerView	* m_LeftExplorerView;

		CView				* m_LeftView;
		CView				* m_RightView;

		CSplitterWnd		m_wndSplitter;

		//{{AFX_MSG(CMainFrame)
		afx_msg void OnUpdateIndicatorError(CCmdUI *pCmdUI);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnContentEventsText();
		afx_msg void OnContentHeader();
		afx_msg void OnContentEventsCopyright();
		afx_msg void OnContentEventsCuepoint();
		afx_msg void OnContentEventsInstrumentname();
		afx_msg void OnContentEventsLyrics();
		afx_msg void OnContentEventsMarkers();
		afx_msg void OnContentEventsSequence();
		afx_msg void OnContentEventsSequencer();
		afx_msg void OnContentEventsTrackname();
		afx_msg void OnContentTracks();
		afx_msg void OnFileTrace();
		afx_msg void OnCorrectEndoftrack();
		afx_msg void OnCorrectStartoftrack();
		afx_msg void OnFileSequencerPlay();
		afx_msg void OnFileSequencerStop();
		afx_msg void OnFileMediaplayer();
		afx_msg void OnFileSequencerPlayandloop();
		afx_msg void OnFileSequencerPlayll();
		afx_msg void OnFileSequencerStopll();
		afx_msg void OnContentPlayer();
		afx_msg void OnFileSequencerPlayandloopll();
		afx_msg void OnFileSequencerPausell();
		afx_msg void OnFileSequencerBackwardll();
		afx_msg void OnFileSequencerForwardll();
		afx_msg void OnFileSequencerResumell();
		afx_msg void OnViewMcitoolbar();
		afx_msg void OnViewMiditoolbar();
		afx_msg void OnCorrectFiltertracks();
		afx_msg void OnCorrectAddGmreset();
		afx_msg void OnCorrectAddGsreset();
		afx_msg void OnCorrectAll();
		afx_msg void OnCorrectOptions();
		afx_msg void OnContentSystemexclusive();
		afx_msg void OnContentErrormessages();
		afx_msg void OnFilePreferences();
		afx_msg void OnClose();
		afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
		afx_msg void OnContentParameters();
		afx_msg void OnRemoveControllerBankselecthigh();
		afx_msg void OnRemoveControllerBankselectlow();
		afx_msg void OnRemoveCopyright();
		afx_msg void OnRemoveCuepoint();
		afx_msg void OnRemoveInstrumentname();
		afx_msg void OnRemoveLyrics();
		afx_msg void OnRemoveMarkers();
		afx_msg void OnRemoveSequence();
		afx_msg void OnRemoveSequencer();
		afx_msg void OnRemoveSystemexclusive();
		afx_msg void OnRemoveText();
		afx_msg void OnRemoveTrackname();
		afx_msg void OnContentPlaylist();
		afx_msg void OnDestroy();
		afx_msg void OnFileAddtotheplaylist();
		afx_msg void OnViewMaintoolbar();
		afx_msg void OnUpdateViewMaintoolbar(CCmdUI* pCmdUI);
		afx_msg void OnCorrectTimesignature();
		afx_msg void OnCorrectKeysignature();
		afx_msg void OnCorrectMoveAll();
		afx_msg void OnToolsTool1();
		afx_msg void OnToolsTool2();
		afx_msg void OnToolsTool3();
		afx_msg void OnToolsTool4();
		afx_msg void OnToolsTool5();
		afx_msg void OnToolsTool6();
		afx_msg void OnToolsTool7();
		afx_msg void OnToolsTool8();
		afx_msg void OnToolsExplorer();
		afx_msg void OnContentChannels();
		afx_msg void OnViewRightview();
		afx_msg void OnViewLeftview();
		afx_msg void OnViewTwoviews();
		afx_msg void OnRemoveControllersResetallcontrolers();
		afx_msg void OnContentStartIe();
		afx_msg void OnContentStopIe();
		afx_msg void OnViewIetoolbar();
		afx_msg void OnViewSF2toolbar();
		afx_msg void OnUpdateViewIetoolbar(CCmdUI* pCmdUI);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		afx_msg void OnViewStatusBar();
		afx_msg void OnUpdateViewStatusBar(CCmdUI* pCmdUI);
		afx_msg void OnUpdateFileSequencerPlayll(CCmdUI* pCmdUI);
		afx_msg void OnUpdateFileSequencerStopll(CCmdUI* pCmdUI);
		afx_msg void OnUpdateFileSequencerForwardll(CCmdUI* pCmdUI);
		afx_msg void OnUpdateFileSequencerBackwardll(CCmdUI* pCmdUI);
		afx_msg void OnUpdateFileSequencerPausell(CCmdUI* pCmdUI);
		afx_msg void OnUpdateFileSequencerPlayandloopll(CCmdUI* pCmdUI);
		afx_msg void OnUpdateFileSequencerPlay(CCmdUI* pCmdUI);
		afx_msg void OnUpdateFileSequencerPlayandloop(CCmdUI* pCmdUI);
		afx_msg void OnUpdateFileSequencerStop(CCmdUI* pCmdUI);
		afx_msg void OnUpdateFileSequencerResumell(CCmdUI* pCmdUI);
		afx_msg void OnUpdateContentStopIe(CCmdUI* pCmdUI);
		afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
		afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
		afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
		afx_msg void OnUpdateFileTrace(CCmdUI* pCmdUI);
		afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
		afx_msg void OnUpdateFileSendMail(CCmdUI* pCmdUI);
		afx_msg void OnUpdateFileAddtotheplaylist(CCmdUI* pCmdUI);
		afx_msg void OnUpdateCorrectAddGmreset(CCmdUI* pCmdUI);
		afx_msg void OnUpdateCorrectAddGsreset(CCmdUI* pCmdUI);
		afx_msg void OnUpdateCorrectAll(CCmdUI* pCmdUI);
		afx_msg void OnUpdateCorrectEndoftrack(CCmdUI* pCmdUI);
		afx_msg void OnUpdateCorrectFiltertracks(CCmdUI* pCmdUI);
		afx_msg void OnUpdateCorrectKeysignature(CCmdUI* pCmdUI);
		afx_msg void OnUpdateCorrectMoveAll(CCmdUI* pCmdUI);
		afx_msg void OnUpdateCorrectStartoftrack(CCmdUI* pCmdUI);
		afx_msg void OnUpdateCorrectTimesignature(CCmdUI* pCmdUI);
		afx_msg void OnUpdateRemoveControllerBankselecthigh(CCmdUI* pCmdUI);
		afx_msg void OnUpdateRemoveControllerBankselectlow(CCmdUI* pCmdUI);
		afx_msg void OnUpdateRemoveCopyright(CCmdUI* pCmdUI);
		afx_msg void OnUpdateRemoveCuepoint(CCmdUI* pCmdUI);
		afx_msg void OnUpdateRemoveInstrumentname(CCmdUI* pCmdUI);
		afx_msg void OnUpdateRemoveLyrics(CCmdUI* pCmdUI);
		afx_msg void OnUpdateRemoveMarkers(CCmdUI* pCmdUI);
		afx_msg void OnUpdateRemoveResetallcontrolers(CCmdUI* pCmdUI);
		afx_msg void OnUpdateRemoveSequence(CCmdUI* pCmdUI);
		afx_msg void OnUpdateRemoveSequencer(CCmdUI* pCmdUI);
		afx_msg void OnUpdateRemoveSystemexclusive(CCmdUI* pCmdUI);
		afx_msg void OnUpdateRemoveText(CCmdUI* pCmdUI);
		afx_msg void OnUpdateRemoveTrackname(CCmdUI* pCmdUI);
		afx_msg void OnContentStaff();
		afx_msg void OnToolsTool10();
		afx_msg void OnToolsTool11();
		afx_msg void OnToolsTool12();
		afx_msg void OnToolsTool13();
		afx_msg void OnToolsTool14();
		afx_msg void OnToolsTool15();
		afx_msg void OnToolsTool16();
		afx_msg void OnToolsTool17();
		afx_msg void OnToolsTool18();
		afx_msg void OnToolsTool19();
		afx_msg void OnToolsTool20();
		afx_msg void OnToolsTool9();
		afx_msg void OnCorrectTruncateNote();
		afx_msg void OnUpdateCorrectTruncateNote(CCmdUI* pCmdUI);
		afx_msg void OnContentNotes();
		afx_msg void OnContentTracksList();
		afx_msg void OnUpdateContentTracksList(CCmdUI* pCmdUI);
		afx_msg void OnContentMidiexplorer();
		afx_msg void OnUpdateContentMidiexplorer(CCmdUI* pCmdUI);
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnUpdateContentChannels(CCmdUI* pCmdUI);
		afx_msg void OnUpdateContentHeader(CCmdUI* pCmdUI);
		afx_msg void OnUpdateContentNotes(CCmdUI* pCmdUI);
		afx_msg void OnUpdateContentPlayer(CCmdUI* pCmdUI);
		afx_msg void OnUpdateContentPlaylist(CCmdUI* pCmdUI);
		afx_msg void OnUpdateContentStaff(CCmdUI* pCmdUI);
		afx_msg void OnUpdateContentTracks(CCmdUI* pCmdUI);
		afx_msg void OnUpdateContentErrormessages(CCmdUI* pCmdUI);
		afx_msg void OnCorrectAddXgreset();
		afx_msg void OnUpdateCorrectAddXgreset(CCmdUI* pCmdUI);
		afx_msg void OnEditFind();
		afx_msg void OnEditFindagain();
		afx_msg void OnUpdateEditFindagain(CCmdUI* pCmdUI);
		afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
		afx_msg void OnFileSavegeometry();
		afx_msg void OnUpdateFileSavegeometry(CCmdUI* pCmdUI);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnContentEventsShowhide();
		afx_msg void OnUpdateContentEventsShowhide(CCmdUI* pCmdUI);
		afx_msg void OnFileRecord();
		afx_msg void OnUpdateFileRecord(CCmdUI* pCmdUI);
		afx_msg void OnUpdateViewSF2Toolbar(CCmdUI *pCmdUI);
        afx_msg void OnInitMenu(CMenu* pMenu);
        afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
        afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
        afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

    public:
        virtual void OnUpdateFrameMenu(HMENU hMenuAlt);
        afx_msg BOOL OnNcActivate(BOOL bActive);
        afx_msg void OnNcPaint();
        afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
        afx_msg void OnNcMouseLeave();
        afx_msg void OnNcMouseHover(UINT nFlags, CPoint point);
        afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
        afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__F6E8EC4C_B230_11D2_8147_444553540000__INCLUDED_)
