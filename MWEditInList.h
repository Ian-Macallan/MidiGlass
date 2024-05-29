#if !defined(AFX_MWEDITINLIST_H__AF262D2A_E2AD_11D2_8147_444553540000__INCLUDED_)
#define AFX_MWEDITINLIST_H__AF262D2A_E2AD_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MWEditInList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMWEditInList window

class CMWEditInList : public CMWEdit
{
		DECLARE_DYNAMIC(CMWEditInList)

		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

	// Construction
	public:
		CMWEditInList();
		virtual ~CMWEditInList();

	// Attributes
	public:

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMWEditInList)
		//}}AFX_VIRTUAL

	// Implementation
	public:
		void SetAnsiVarFont ();
		void SetAnsiFixedFont ();

		// Generated message map functions
	protected:
		//{{AFX_MSG(CMWEditInList)
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnKillfocus();
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()

	public:
		virtual BOOL PreTranslateMessage(MSG* pMsg);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MWEDITINLIST_H__AF262D2A_E2AD_11D2_8147_444553540000__INCLUDED_)
