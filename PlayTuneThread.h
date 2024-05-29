#if !defined(AFX_PLAYTUNETHEAD_H__CE4DEB65_D49E_11D2_8147_444553540000__INCLUDED_)
#define AFX_PLAYTUNETHEAD_H__CE4DEB65_D49E_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayTuneThead.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CPlayTuneThread thread

class CPlayTuneThread : public CWinThread
{
	DECLARE_DYNCREATE(CPlayTuneThread)
protected:
	CPlayTuneThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayTuneThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPlayTuneThread();

	// Generated message map functions
	//{{AFX_MSG(CPlayTuneThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYTUNETHEAD_H__CE4DEB65_D49E_11D2_8147_444553540000__INCLUDED_)
