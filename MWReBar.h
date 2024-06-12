// MWReBar.h: interface for the CMWReBar class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CMWReBar : public CReBar  
{
		DECLARE_DYNAMIC(CMWReBar)

		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

	public:
		CMWReBar();
		virtual ~CMWReBar();

		void SetAnsiVarFont ();
		void SetAnsiFixedFont ();

		void SendNormalMessage();

	protected:
		DECLARE_MESSAGE_MAP()

	private :
		boolean		m_bSendNormal;
public:
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    virtual BOOL Create(CWnd* pParentWnd, DWORD dwCtrlStyle = RBS_BANDBORDERS, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP, UINT nID = AFX_IDW_REBAR);
    afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};

