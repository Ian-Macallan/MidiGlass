#if !defined(AFX_MWRICHEDITCONTROL_H__3BE1F528_F1E3_11D2_8148_444553540000__INCLUDED_)
#define AFX_MWRICHEDITCONTROL_H__3BE1F528_F1E3_11D2_8148_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MWRichEditControl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMWRichEditControl window

class CMWRichEditControl : public CRichEditCtrl
{
		DECLARE_DYNAMIC(CMWRichEditControl)

		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

	// Construction
	public:
		CMWRichEditControl();

	// Attributes
	public:

	// Operations
	public:
		void SendNormalMessage();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMWRichEditControl)
		//}}AFX_VIRTUAL

	// Implementation
	public:
		virtual ~CMWRichEditControl();

		// Generated message map functions
	protected:
		//{{AFX_MSG(CMWRichEditControl)
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()

	private :
		boolean		m_bSendNormal;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MWRICHEDITCONTROL_H__3BE1F528_F1E3_11D2_8148_444553540000__INCLUDED_)
