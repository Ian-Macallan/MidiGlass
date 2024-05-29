#if !defined(AFX_MWEVENTSCOMBOBOX_H__B7ED55C7_E2DD_11D2_8147_444553540000__INCLUDED_)
#define AFX_MWEVENTSCOMBOBOX_H__B7ED55C7_E2DD_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MWEventsComboBox.h : header file
//
#include "MWObjects.h"

//
///////////////////////////////////////////////////////////////////////////////////
// CMWEventsComboBox window
//
///////////////////////////////////////////////////////////////////////////////////
class CMWEventsComboBox : public CMWComboBox
{
	DECLARE_DYNAMIC(CMWEventsComboBox)

	friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

	// Construction
	public:
		CMWEventsComboBox();
		virtual ~CMWEventsComboBox();

	// Attributes
	public:

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMWEventsComboBox)
		//}}AFX_VIRTUAL

	// Implementation
	public:
		void SetAnsiVarFont ();
		void SetAnsiFixedFont ();

		void ResetContent();
		virtual void ReLoad();
		

		// Generated message map functions
	protected:
		//{{AFX_MSG(CMWEventsComboBox)
		afx_msg void OnSelchange();
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MWEVENTSCOMBOBOX_H__B7ED55C7_E2DD_11D2_8147_444553540000__INCLUDED_)
