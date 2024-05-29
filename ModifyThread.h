#if !defined(AFX_MODIFYTHREAD_H__BD1F84E2_F431_11D2_8148_444553540000__INCLUDED_)
#define AFX_MODIFYTHREAD_H__BD1F84E2_F431_11D2_8148_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModifyThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CModifyThread thread

class CModifyThread : public CWinThread
{
	DECLARE_DYNCREATE(CModifyThread)
protected:
	CModifyThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModifyThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CModifyThread();

	// Generated message map functions
	//{{AFX_MSG(CModifyThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODIFYTHREAD_H__BD1F84E2_F431_11D2_8148_444553540000__INCLUDED_)
