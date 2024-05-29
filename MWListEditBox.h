#if !defined(AFX_MWLISTEDITBOX_H__AF262D28_E2AD_11D2_8147_444553540000__INCLUDED_)
#define AFX_MWLISTEDITBOX_H__AF262D28_E2AD_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MWListEditBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMWListEditBox window

class CMWListEditBox : public CListBox
{
		DECLARE_DYNAMIC(CMWListEditBox)

		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

	// Construction
	public:
		CMWListEditBox();
		virtual ~CMWListEditBox();

	// Attributes
	public:

	// Operations
	public:
		void SendNormalMessage();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMWListEditBox)
		//}}AFX_VIRTUAL

	// Implementation
	public:
		void SetAnsiVarFont ();
		void SetAnsiFixedFont ();

		// Generated message map functions
	protected:
		//{{AFX_MSG(CMWListEditBox)
		afx_msg void OnKillfocus();
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()

	private :
		boolean		m_bSendNormal;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MWLISTEDITBOX_H__AF262D28_E2AD_11D2_8147_444553540000__INCLUDED_)
