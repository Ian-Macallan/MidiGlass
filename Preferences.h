#if !defined(AFX_PREFERENCES_H__F3E64B41_D248_11D2_8147_444553540000__INCLUDED_)
#define AFX_PREFERENCES_H__F3E64B41_D248_11D2_8147_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Preferences.h : header file
//
#include "MWObjects.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CPreferences dialog

class CPreferences : public CDialog
{
	friend	HBRUSH	FriendCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	friend	BOOL	FriendEraseBkgnd(CWnd* pWnd, CDC* pDC);

// Construction
public:
	void DisplayValues();
	CPreferences(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPreferences)
	enum { IDD = IDD_PREFERENCES };
	CMWComboBox	m_In_Device;
	CMWEdit			m_WaveDirectory;
	CMWButton		m_Splitter;
	CMWButton		m_Large_Bar;
	CMWButton		m_ReBar;
	CMWButton		m_Play_Tune;
	CMWComboBox		m_Device;
	CMWButton		m_Skip_To_Note;
	CMWButton		m_Ok;
	CMWButton		m_Cancel;
	CMWComboBox		m_Start_Playing;
	CMWEdit			m_SoundFontFile;
	CMWButton		m_BrowseSoundFile;
	CMWButton		m_Use_Sound_Font;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreferences)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPreferences)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSkipToNote();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBrowse();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFERENCES_H__F3E64B41_D248_11D2_8147_444553540000__INCLUDED_)
