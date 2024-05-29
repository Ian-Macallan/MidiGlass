#if !defined(AFX_MWEDIT_H__453A25C6_D61D_11D2_8147_444553540000__INCLUDED_)
#define AFX_MWEDIT_H__453A25C6_D61D_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MWEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMWEdit window

class CMWEdit : public CEdit
{
		DECLARE_DYNAMIC(CMWEdit)

		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

	// Construction
	public:
		CMWEdit();
		virtual ~CMWEdit();

	// Attributes
	public:
		COLORREF m_foreground;
		COLORREF m_background;

	// Operations
	public:
		void SendNormalMessage();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMWEdit)
		//}}AFX_VIRTUAL

	// Implementation
	public:
		void SetAnsiVarFont ();
		void SetAnsiFixedFont ();
		void SetTextAttributes(COLORREF foreground, COLORREF background);

		// Generated message map functions
	public:
		//{{AFX_MSG(CMWEdit)
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnKillfocus();
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()

	protected :
		boolean		m_bSendNormal;

	public:
		virtual BOOL PreTranslateMessage(MSG* pMsg);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MWEDIT_H__453A25C6_D61D_11D2_8147_444553540000__INCLUDED_)
