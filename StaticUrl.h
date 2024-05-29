#if !defined(AFX_STATICURL_H__24EA7CC1_71DB_11D3_814A_444553540000__INCLUDED_)
#define AFX_STATICURL_H__24EA7CC1_71DB_11D3_814A_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticUrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticUrl window

class CStaticUrl : public CStatic
{
	// Construction
	public:
		CStaticUrl();

	// Attributes
	public:

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CStaticUrl)
		//}}AFX_VIRTUAL

	// Implementation
	public:
		virtual ~CStaticUrl();

		// Generated message map functions
	protected:
		HCURSOR m_hCursor;
		//{{AFX_MSG(CStaticUrl)
		afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		//}}AFX_MSG

		DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICURL_H__24EA7CC1_71DB_11D3_814A_444553540000__INCLUDED_)
