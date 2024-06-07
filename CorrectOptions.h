#if !defined(AFX_CORRECTOPTIONS_H__25246DE1_CF0B_11D2_8147_444553540000__INCLUDED_)
#define AFX_CORRECTOPTIONS_H__25246DE1_CF0B_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CorrectOptions.h : header file
//
#include "MWObjects.h"
#include "afxwin.h"
#include "MWDialog.h"

//
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
// CCorrectOptions dialog
class CCorrectOptions : public CMWDialog
{
        DECLARE_DYNAMIC(CCorrectOptions)

		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgnd(CWnd* pWnd,CDC* pDC);

	// Construction
	public:
		CCorrectOptions(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
		//{{AFX_DATA(CCorrectOptions)
		enum { IDD = IDD_CORRECT_OPTION };
		CMWComboBox	m_SysEx;
		CMWButton	m_Ok;
		CMWButton	m_Cancel;
		CMWButton	m_Filter;
		CMWButton	m_BankSelectLow;
		CMWButton	m_BankSelectHigh;
		CMWButton	m_AddStartOfTrack;
		CMWButton	m_AddEndOfTrack;
		CMWButton	m_CorrectStartOfTrack;

		//}}AFX_DATA


	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CCorrectOptions)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		//}}AFX_VIRTUAL

	// Implementation
	protected:

		// Generated message map functions
		//{{AFX_MSG(CCorrectOptions)
		virtual BOOL OnInitDialog();
		virtual void OnOK();
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CORRECTOPTIONS_H__25246DE1_CF0B_11D2_8147_444553540000__INCLUDED_)
