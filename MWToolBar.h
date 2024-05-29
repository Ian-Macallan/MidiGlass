// MWToolBar.h: interface for the CMWToolBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MWTOOLBAR_H__453A25C5_D61D_11D2_8147_444553540000__INCLUDED_)
#define AFX_MWTOOLBAR_H__453A25C5_D61D_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMWToolBar : public CToolBar  
{
		DECLARE_DYNAMIC(CMWToolBar)

		friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

	public:
		CMWToolBar();
		virtual ~CMWToolBar();

		void SetAnsiVarFont ();
		void SetAnsiFixedFont ();

		void SendNormalMessage();

	protected:
		CFont		*m_pFont;

		DECLARE_MESSAGE_MAP()

	private :
		boolean		m_bSendNormal;

};

#endif // !defined(AFX_MWTOOLBAR_H__453A25C5_D61D_11D2_8147_444553540000__INCLUDED_)
