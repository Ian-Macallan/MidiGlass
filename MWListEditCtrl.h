#if !defined(AFX_MWLISTEDITCTRL_H__AF262D27_E2AD_11D2_8147_444553540000__INCLUDED_)
#define AFX_MWLISTEDITCTRL_H__AF262D27_E2AD_11D2_8147_444553540000__INCLUDED_

#include "MWEdit.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MidiDefs.h"
#include "MWCHeaderCtrl.h"

// MWListEditCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMWListEditCtrl window

class CMWListEditCtrl : public CListCtrl
{
		DECLARE_DYNAMIC(CMWListEditCtrl)

		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

	// Construction
	public:
		CMWListEditCtrl();

	// Attributes
	public:

	// Operations
	public:
		void SendNormalMessage();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMWListEditCtrl)
		//}}AFX_VIRTUAL

	// Implementation
	public:
		void SetAnsiVarFont ();
		void SetAnsiFixedFont ();

		void SetSpinCtrlRange ( int iSubItem, int iLow, int iHigh );
		void EnableSpinCtrl ( int iSubItem );
		LRESULT OnEditSelect(WPARAM wParam, LPARAM lParam);
		void DeleteAllItems();
		void SetEditControlHandle ( CWnd *wndControl );
		CWnd * GetEditControlHandle();
		int GetSubItemIndex();
		int GetItemIndex();
		void SetEditFlag ( int iSubItem, bool bFlag );
		LRESULT PostMessageToParent ( UINT wMsg );
		LRESULT SendMessageToParent ( UINT wMsg );
		bool EditColumn ( int iItem, int iSubItem );
		virtual CWnd * CreateSubItemControl ( int iItem, int iSubItem, CRect rectLabel );
		LRESULT OnEditEnd(WPARAM wParam, LPARAM lParam);
		void ResetContent();
		bool EditColumn ( int iSubItem );
		int GetCurSel();
		virtual ~CMWListEditCtrl();

		// Generated message map functions
	protected:
		bool			m_bEditColumn [ MAX_COLUMNS ];
		bool			m_bSpinColumn [ MAX_COLUMNS ];
		unsigned		m_iSpinLowRng [ MAX_COLUMNS ];
		unsigned		m_iSpinHigRng [ MAX_COLUMNS ];
		int				m_iEditItem;
		int				m_iEditSubItem;
		CWnd			*m_pEditControl;
		CWnd			*m_pSpinControl;

        CMWCHeaderCtrl  m_HeaderCtrl;
        bool            m_bHeaderControlSubclassed;

		//{{AFX_MSG(CMWListEditCtrl)
		afx_msg void OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()

	private :
		boolean		m_bSendNormal;
    public:
        afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MWLISTEDITCTRL_H__AF262D27_E2AD_11D2_8147_444553540000__INCLUDED_)
