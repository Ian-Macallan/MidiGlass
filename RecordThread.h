#if !defined(AFX_RECORDTHREAD_H__09A065AB_3050_11D3_8149_444553540000__INCLUDED_)
#define AFX_RECORDTHREAD_H__09A065AB_3050_11D3_8149_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecordThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CRecordThread thread

class CRecordThread : public CWinThread
{
	DECLARE_DYNCREATE(CRecordThread)
protected:
	CRecordThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CRecordThread();

	// Generated message map functions
	//{{AFX_MSG(CRecordThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDTHREAD_H__09A065AB_3050_11D3_8149_444553540000__INCLUDED_)
