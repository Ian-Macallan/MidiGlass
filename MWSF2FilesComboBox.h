#pragma once

// MWProgramsComboBox.h : header file
//
#include "MWObjects.h"

//
///////////////////////////////////////////////////////////////////////////////////////////
// CMWSF2FilesComboBox window
//
///////////////////////////////////////////////////////////////////////////////////////////
class CMWSF2FilesComboBox : public CMWComboBox
{
	DECLARE_DYNAMIC(CMWSF2FilesComboBox)

	friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

	// Construction
	public:
		CMWSF2FilesComboBox();
		virtual ~CMWSF2FilesComboBox();

	// Attributes
	public:

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMWSF2FilesComboBox)
		//}}AFX_VIRTUAL

	// Implementation
	public:

		void SetAnsiVarFont ();
		void SetAnsiFixedFont ();

		void ResetContent();

		virtual void ReLoad();

		// Generated message map functions
	protected:
		//{{AFX_MSG(CMWSF2FilesComboBox)
		afx_msg void OnSelchange();
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnDropdown();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
