#if !defined(AFX_MWBUTTON_H__453A25CA_D61D_11D2_8147_444553540000__INCLUDED_)
#define AFX_MWBUTTON_H__453A25CA_D61D_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MWButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMWButton window

class CMWButton : public CButton
{
		DECLARE_DYNAMIC(CMWButton)

		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

	// Construction
	public:
		CMWButton();
		virtual ~CMWButton();

	// Attributes
	public:

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMWButton)
		//}}AFX_VIRTUAL

	// Implementation
	public:
		void SetAnsiVarFont ();
		void SetAnsiFixedFont ();

		void SendNormalMessage();

		// Generated message map functions
	protected:
		//{{AFX_MSG(CMWButton)
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnKillFocus(CWnd* pNewWnd);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    public:
        afx_msg void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct);
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()

	private :
		boolean		m_bSendNormal;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MWBUTTON_H__453A25CA_D61D_11D2_8147_444553540000__INCLUDED_)
