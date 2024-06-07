// MWStatusBar.h: interface for the CMWStatusBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MWSTATUSBAR_H__453A25CC_D61D_11D2_8147_444553540000__INCLUDED_)
#define AFX_MWSTATUSBAR_H__453A25CC_D61D_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//
///////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////
class CMWStatusBar : public CStatusBar  
{
		DECLARE_DYNAMIC(CMWStatusBar)


		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

	public:
		CMWStatusBar();
		virtual ~CMWStatusBar();

		void SetAnsiVarFont ();
		void SetAnsiFixedFont ();

		void SendNormalMessage();

	protected:
		DECLARE_MESSAGE_MAP()

	private :
		boolean		m_bSendNormal;

    public:
        afx_msg BOOL OnEraseBkgnd(CDC* pDC);
        afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
        afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
        afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
        virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};

#endif // !defined(AFX_MWSTATUSBAR_H__453A25CC_D61D_11D2_8147_444553540000__INCLUDED_)
