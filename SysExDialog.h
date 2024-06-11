#if !defined(AFX_SYSEXDIALOG_H__A75A3D61_018A_11D3_8148_444553540000__INCLUDED_)
#define AFX_SYSEXDIALOG_H__A75A3D61_018A_11D3_8148_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysExDialog.h : header file
//
#include "MidiFile.h"
#include "MWDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CSysExDialog dialog

class CSysExDialog : public CMWDialog
{
    DECLARE_DYNAMIC(CSysExDialog)

    // Construction
    public:
	    void CheckSysEx ( CString &strSysEx );
	    void WriteSysEx();
	    void ReadSysEx ( );
	    CSysExDialog(CWnd* pParent = NULL);   // standard constructor

    // Dialog Data
	    //{{AFX_DATA(CSysExDialog)
	    enum { IDD = IDD_SYSEX };
	    CMWButton	m_Stop_Receiving;
	    CMWButton	m_Start_Receiving;
	    CEdit	    m_Size_Received;
	    CMWButton	m_Default_Value;
	    CMWListCtrl	m_SysEx_Values;
	    CMWComboBox	m_SysEx;
	    //}}AFX_DATA


    // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(CSysExDialog)
	    protected:
	    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	    //}}AFX_VIRTUAL

    // Implementation
    protected:
	    bool m_bFirstTime;
	    CMidiFile	m_MidiFile;

	    // Generated message map functions
	    //{{AFX_MSG(CSysExDialog)
	    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	    virtual BOOL OnInitDialog();
	    afx_msg void OnEndlabeleditSysexValues(NMHDR* pNMHDR, LRESULT* pResult);
	    afx_msg void OnSelendokSysex();
	    afx_msg void OnDefaultValues();
	    afx_msg void OnStartReceive();
	    afx_msg void OnStopReceive();
	    afx_msg void OnCheckReceive();
	    afx_msg void OnTimer(UINT_PTR nIDEvent);
	    virtual void OnOK();
	    afx_msg void OnClose();
	    //}}AFX_MSG
	    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSEXDIALOG_H__A75A3D61_018A_11D3_8148_444553540000__INCLUDED_)
