#if !defined(AFX_STAFFVIEW_H__874E2101_E775_11D2_8148_444553540000__INCLUDED_)
#define AFX_STAFFVIEW_H__874E2101_E775_11D2_8148_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaffView.h : header file
//

#include "MWObjects.h"

#include "StaffViewParms.h"

//
///////////////////////////////////////////////////////////////////////////////////////
// CStaffView view
//
///////////////////////////////////////////////////////////////////////////////////////
class CStaffView : public CScrollView
{
		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);
		friend	void	FriendActivate(BOOL bActivate, CView* pActivateView, CView* pDeactiveView, bool bForce);

	protected:
		CStaffView();           // protected constructor used by dynamic creation
		DECLARE_DYNCREATE(CStaffView)

	// Attributes
	public:

	// Operations
	public:
		void DrawAdditionalFrame ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx, long iLine, CMidiList *pList );
		void SetViewParameters();
		void			Round ( CDC *pDC, long iCx1, long iCy1, long iCx2, long iCy2 );
		void			DrawDot ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx, long iCy );
		void			DisplaySilencesForCount ( CDC *pDC, CPrintInfo *pInfo, CStaffViewParms *parmsView, long &iCx, long &iLine, int iCount );
		void			DisplaySilences ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long &iCx, long &iLine, int iBeat );
		void			ComputeSizeForPrinter ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView );
		void			ComputeSizeForScreen ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView );
		bool			TestLineInPage ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iLine );
		void			AddNewFrameWork ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long &iCx, long &iLine );
		void			ComputeCxAndLine ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long &iCx, long &iLine, long iAddCx );
		void			DrawBemol ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx, long iCy );

		void			DrawZigZag ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx, long iLine );
		void			DrawUpSquare ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx, long iLine );
		void			DrawDownSquare ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx, long iLine );
		void			DrawSlashOne ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx, long iLine );
		void			DrawSlashTwo ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx, long iLine );
		void			DrawSlashThree ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx, long iLine );

		long			ComputeEndOfLine (CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, int iLine );
		void			DrawAll ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView );

		CPen			* SetNormalPen ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView );
		CPen			* SetStrongPen( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView);
		CPen			* SetLightPen ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView );
		CPen			* SetHeavyPen( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView);

		CPen			* CreateNormalPen ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView );
		CPen			* CreateStrongPen( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView);
		CPen			* CreateLightPen ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView );
		CPen			* CreateHeavyPen( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView);

		void			ComputeAllSize (CDC *pDC, CPrintInfo *pInfo, CStaffViewParms *pParms, long iHeight );
		void			GetStaffViewSize ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, RECT *pRect );
		CFont			* CreateAnsiVarFont( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView );
		CFont			* SetAnsiVarFont( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView );
		void			DrawTrackName(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView);
		void			DrawTitle ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView );
		void			ForceToRedraw (CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView );
		void			DrawMesureText (CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx, long iCy, const char *pText );
		long			ComputeFrameWorkNumber(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView);
		long			FrameWorkHeight(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView);
		long			FrameWorkWidth(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView);
		long			BottomEdge (CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView);
		long			TopEdge (CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView);
		long			RightEdge(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView);
		long			LeftEdge ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView);
		bool			VerifyColumnInRect ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iCx );
		long			ComputeCy ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iLine );
		void			DrawSeparator(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iColumn, long iLine);
		CMidiList		* DrawOneMesure(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, 
							long &iCx, long &iLine, CMidiList *pList, 
							bool bSeparator );
		void			DrawOneNote ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, const long iCx, long &iLine, CMidiList *pList );
		void			DrawOneNoteVertQueue ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, int iCx, int iCy );
		void			DrawOneNoteHorzQueue ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, int iCx, int iCy, int number );
		void			DrawNotes ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView );
		void			DrawOneFrameWork ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView, long iLine );
		void			DrawFrameWork ( CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView );
		void			DisplayValues(CDC *pDC, CPrintInfo* pInfo, CStaffViewParms *parmsView);

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CStaffView)
		public:
		virtual void OnInitialUpdate();
		virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
		protected:
		virtual void OnDraw(CDC* pDC);      // overridden to draw this view
		virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
		virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
		virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
		virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
		virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
		virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
		virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
		//}}AFX_VIRTUAL

	// Implementation
	protected:
		virtual ~CStaffView();
	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif

		// Generated message map functions
	protected:
		bool				m_bScale;

		unsigned char		m_cNN;
		unsigned char		m_cDD;
		int					m_iMesure;
		int					m_iLastMesure;
		bool				m_bFirstTime;

		CStaffViewParms		m_ScreenParms;
		CStaffViewParms		m_PrinterParms;

		//		The Mesures
		char				m_cMesureHiTable [ MAX_INSIDE_MESURE ];
		char				m_cMesureLoTable [ MAX_INSIDE_MESURE ];

        CMWMenu             *m_pContextMenu;

		//{{AFX_MSG(CStaffView)
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		afx_msg void OnStaffHelp();
		afx_msg void OnStaffScale();
		afx_msg void OnFilePrint();
		afx_msg void OnFilePrintPreview();

		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnMove(int x, int y);
		afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
        afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STAFFVIEW_H__874E2101_E775_11D2_8148_444553540000__INCLUDED_)
