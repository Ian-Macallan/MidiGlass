#if !defined(AFX_MWCOMBOBOX_H__453A25CB_D61D_11D2_8147_444553540000__INCLUDED_)
#define AFX_MWCOMBOBOX_H__453A25CB_D61D_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MWComboBox.h : header file
//

//
////////////////////////////////////////////////////////////////////////
// CMWComboBox window
////////////////////////////////////////////////////////////////////////
class CMWComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CMWComboBox)

	friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

	private :
		int		m_iInitialSel;

	// Construction
	public:
		CMWComboBox();
		virtual ~CMWComboBox();

	// Attributes
	public:

	// Operations
	public:
		int SetCurSel(int nSelect);
		int GetCurSel() const;

		BOOL GetModify() const;
		void SetModify( BOOL bModified = TRUE );

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMWComboBox)
		//}}AFX_VIRTUAL

	// Implementation
	public:
		void SetAnsiVarFont ();
		void SetAnsiFixedFont ();

		void SendNormalMessage();
		virtual void ReLoad();

		// Generated message map functions
	protected:
		//{{AFX_MSG(CMWComboBox)
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		afx_msg void OnKillfocus();
		afx_msg void OnSelchange();
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()

	private :
		boolean		m_bSendNormal;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MWCOMBOBOX_H__453A25CB_D61D_11D2_8147_444553540000__INCLUDED_)
