// MidiGlassApp.h : main header file for the MIDI application
//

#if !defined(AFX_MIDIABOUT_H_INCLUDED_)
#define AFX_MIDIABOUT_H_INCLUDED_

#include "StaticUrl.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "MidiDefs.h"
#include "MWObjects.h"
#include "MidiFile.h"
#include "PlayerDialog.h"	// Added by ClassView

#include "ModifyThreadDialog.h"	// Added by ClassView
#include "MainFrm.h"

/////////////////////////////////////////////////////////////////////////////
// CMidiWorksApp:
// See Midi Works.cpp for the implementation of this class
//
class CAboutDlg : public CDialog
{
	friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CStaticUrl	m_Internal_Name;
	CStaticUrl	m_Comments;
	CMWStatic	m_Product_Name;
	CMWButton	m_Tune;
	CMWEdit	m_Product_Version;
	CMWEdit	m_Legal_Copyright;
	CMWEdit	m_Legal_Trademark;
	CMWButton	m_Ok;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CMidiFile * m_pMidiFile;
	//{{AFX_MSG(CAboutDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnInternalName();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnTune();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnComments();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIDIABOUT_H_INCLUDED_)
