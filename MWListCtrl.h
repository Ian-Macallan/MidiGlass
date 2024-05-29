#if !defined(AFX_MWLISTCTRL_H__453A25C9_D61D_11D2_8147_444553540000__INCLUDED_)
#define AFX_MWLISTCTRL_H__453A25C9_D61D_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MWListCtrl.h : header file
//

#include "MWCHeaderCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CMWListCtrl window

class CMWListCtrl : public CListCtrl
{
		DECLARE_DYNAMIC(CMWListCtrl)

		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

	// Construction
	public:
		CMWListCtrl();

	// Attributes
	public:

	// Operations
	public:
		void SendNormalMessage();

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMWListCtrl)
		//}}AFX_VIRTUAL

	// Implementation
	public:
		void SetAnsiVarFont ();
		void SetAnsiFixedFont ();

		char * GetShort ( const char *pString );
		void ResetContent( );
		int SetCurSel( int nSelect );
		int GetText( int nIndex, LPTSTR lpszBuffer, size_t iBuffer );
		int InsertString ( int nIndex, LPCTSTR lpszItem );
		UINT DeleteString ( UINT nIndex );
		int GetCount ( );
		int GetCurSel ( );
		int AddString ( LPCTSTR lpszItem );
		virtual ~CMWListCtrl();

		// Generated message map functions
	protected:
		bool m_bHeaderDone;
		//{{AFX_MSG(CMWListCtrl)
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnKillfocus(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()

	private :
		boolean		    m_bSendNormal;
        CMWCHeaderCtrl  m_HeaderCtrl;
        bool            m_bHeaderControlSubclassed;

    public:
        afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MWLISTCTRL_H__453A25C9_D61D_11D2_8147_444553540000__INCLUDED_)
