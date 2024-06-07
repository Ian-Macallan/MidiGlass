#if !defined(AFX_FINDEVENT_H__42262E21_0092_11D3_8148_444553540000__INCLUDED_)
#define AFX_FINDEVENT_H__42262E21_0092_11D3_8148_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindEvent.h : header file
//

#include "MWDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CFindEvent dialog

class CFindEvent : public CMWDialog
{
	friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

// Construction
public:
	void EnableDisable();
	CFindEvent(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFindEvent)
	enum { IDD = IDD_FIND_EVENT };
	CMWEdit	m_V2_To;
	CMWEdit	m_V2_From;
	CSpinButtonCtrl	m_Spin_V2_To;
	CSpinButtonCtrl	m_Spin_V2_From;
	CMWEdit	m_V1_To;
	CMWEdit	m_V1_From;
	CSpinButtonCtrl	m_Spin_V1_To;
	CSpinButtonCtrl	m_Spin_V1_From;
	CMWControllersComboBox	m_Controller_Combo;
	CMWProgramsComboBox	m_Program_Combo;
	CMWEventsComboBox	m_Event_Combo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindEvent)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFindEvent)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeEventCombo();
	afx_msg void OnSelendokEventCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDEVENT_H__42262E21_0092_11D3_8148_444553540000__INCLUDED_)
