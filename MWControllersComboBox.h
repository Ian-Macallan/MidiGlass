//
////////////////////////////////////////////////////////////////////////
// MWControllersComboBox.h: interface for the CMWControllersComboBox class.
//
////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MWCONTROLLERSCOMBOBOX_H__42262E25_0092_11D3_8148_444553540000__INCLUDED_)
#define AFX_MWCONTROLLERSCOMBOBOX_H__42262E25_0092_11D3_8148_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MWObjects.h"

//
////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
class CMWControllersComboBox : public CMWComboBox  
{
		DECLARE_DYNAMIC(CMWControllersComboBox)

		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

	public:
		CMWControllersComboBox();
		virtual ~CMWControllersComboBox();

	// Attributes
	public:

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMWControllersComboBox)
		//}}AFX_VIRTUAL


	// Implementation
	public:
		void SetAnsiVarFont();
		void SetAnsiFixedFont ();

		void ResetContent();
		virtual void ReLoad();

		// Generated message map functions
	protected:
		//{{AFX_MSG(CMWControllersComboBox)
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_MWCONTROLLERSCOMBOBOX_H__42262E25_0092_11D3_8148_444553540000__INCLUDED_)
