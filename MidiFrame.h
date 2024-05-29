#if !defined(AFX_MIDIFRAME_H__4A0BCB44_0EB6_11D3_8148_444553540000__INCLUDED_)
#define AFX_MIDIFRAME_H__4A0BCB44_0EB6_11D3_8148_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MidiFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMidiFrame frame

class CMidiFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CMidiFrame)
protected:
	CMidiFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMidiFrame)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMidiFrame();

	// Generated message map functions
	//{{AFX_MSG(CMidiFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIDIFRAME_H__4A0BCB44_0EB6_11D3_8148_444553540000__INCLUDED_)
