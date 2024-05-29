#if !defined(AFX_MWSCROLLBAR_H__B7ED55C2_E2DD_11D2_8147_444553540000__INCLUDED_)
#define AFX_MWSCROLLBAR_H__B7ED55C2_E2DD_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MWScrollBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMWScrollBar window

class CMWScrollBar : public CScrollBar
{
		DECLARE_DYNAMIC(CMWScrollBar)

		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

	// Construction
	public:
		CMWScrollBar();
		virtual ~CMWScrollBar();

	// Attributes
	public:

	// Operations
	public:
		void SendNormalMessage();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMWScrollBar)
		//}}AFX_VIRTUAL

	// Implementation
	public:
		void SetAnsiVarFont ();
		void SetAnsiFixedFont ();

		// Generated message map functions
	protected:
		//{{AFX_MSG(CMWScrollBar)
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()

	private :
		boolean		m_bSendNormal;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MWSCROLLBAR_H__B7ED55C2_E2DD_11D2_8147_444553540000__INCLUDED_)
