#if !defined(AFX_MWPROGRESSCTRL_H__2A4AFF87_D633_11D2_8147_444553540000__INCLUDED_)
#define AFX_MWPROGRESSCTRL_H__2A4AFF87_D633_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MWProgressCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMWProgressCtrl window

class CMWProgressCtrl : public CProgressCtrl
{
		DECLARE_DYNAMIC(CMWProgressCtrl)

		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

	// Construction
	public:
		CMWProgressCtrl();
		virtual ~CMWProgressCtrl();

	// Attributes
	public:

	// Operations
	public:
		void SendNormalMessage();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMWProgressCtrl)
		//}}AFX_VIRTUAL

	// Implementation
	public:
		void SetAnsiVarFont ();
		void SetAnsiFixedFont ();
		

		// Generated message map functions
	protected:
		//{{AFX_MSG(CMWProgressCtrl)
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnKillFocus(CWnd* pNewWnd);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()

	private :
		boolean		m_bSendNormal;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MWPROGRESSCTRL_H__2A4AFF87_D633_11D2_8147_444553540000__INCLUDED_)
