#if !defined(AFX_MWSTATIC_H__453A25C7_D61D_11D2_8147_444553540000__INCLUDED_)
#define AFX_MWSTATIC_H__453A25C7_D61D_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MWStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMWStatic window

class CMWStatic : public CStatic
{
		DECLARE_DYNAMIC(CMWStatic)

		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

	// Construction
	public:
		CMWStatic();
		virtual ~CMWStatic();

	// Attributes
	public:

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMWStatic)
		//}}AFX_VIRTUAL

	// Implementation
	public:

		void SetAnsiVarFont ();
		void SetAnsiFixedFont ();

		void SendNormalMessage();

		// Generated message map functions
	protected:
		//{{AFX_MSG(CMWStatic)
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

#endif // !defined(AFX_MWSTATIC_H__453A25C7_D61D_11D2_8147_444553540000__INCLUDED_)
