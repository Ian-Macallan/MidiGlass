#if !defined(AFX_MWFRAMEWND_H__8EE0FB64_0D20_11D3_8148_444553540000__INCLUDED_)
#define AFX_MWFRAMEWND_H__8EE0FB64_0D20_11D3_8148_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MWFrameWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMWFrameWnd frame

class CMWFrameWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(CMWFrameWnd)
protected:
	CMWFrameWnd();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMWFrameWnd)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMWFrameWnd();

	// Generated message map functions
	//{{AFX_MSG(CMWFrameWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MWFRAMEWND_H__8EE0FB64_0D20_11D3_8148_444553540000__INCLUDED_)
