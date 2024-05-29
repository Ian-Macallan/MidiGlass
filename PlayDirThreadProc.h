#if !defined(AFX_PLAYDIRTHREADPROC_H__BD148A62_F41B_11D2_8148_444553540000__INCLUDED_)
#define AFX_PLAYDIRTHREADPROC_H__BD148A62_F41B_11D2_8148_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayDirThreadProc.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CPlayDirThreadProc thread

class CPlayDirThreadProc : public CWinThread
{
	DECLARE_DYNCREATE(CPlayDirThreadProc)
protected:
	CPlayDirThreadProc();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayDirThreadProc)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPlayDirThreadProc();

	// Generated message map functions
	//{{AFX_MSG(CPlayDirThreadProc)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYDIRTHREADPROC_H__BD148A62_F41B_11D2_8148_444553540000__INCLUDED_)
