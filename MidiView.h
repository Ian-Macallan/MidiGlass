#if !defined(AFX_MIDIVIEW_H__4A0BCB46_0EB6_11D3_8148_444553540000__INCLUDED_)
#define AFX_MIDIVIEW_H__4A0BCB46_0EB6_11D3_8148_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MidiView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMidiView view

class CMidiView : public CView
{
		friend	void	FriendActivate(BOOL bActivate, CView* pActivateView, CView* pDeactiveView, bool bForce);

	protected:
		CMidiView();           // protected constructor used by dynamic creation
		DECLARE_DYNCREATE(CMidiView)

	// Attributes
	public:

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMidiView)
		public:
		virtual void OnInitialUpdate();
		protected:
		virtual void OnDraw(CDC* pDC);      // overridden to draw this view
		virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
		//}}AFX_VIRTUAL

	// Implementation
	protected:
		virtual ~CMidiView();
	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif

		// Generated message map functions
	protected:
		//{{AFX_MSG(CMidiView)
			// NOTE - the ClassWizard will add and remove member functions here.
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnMove(int x, int y);
		afx_msg void OnSize(UINT nType, int cx, int cy);
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIDIVIEW_H__4A0BCB46_0EB6_11D3_8148_444553540000__INCLUDED_)
