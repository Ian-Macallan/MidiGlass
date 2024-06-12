// MWToolBar.h: interface for the CMWToolBar class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "afxtoolbar.h"

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

public:
    afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

